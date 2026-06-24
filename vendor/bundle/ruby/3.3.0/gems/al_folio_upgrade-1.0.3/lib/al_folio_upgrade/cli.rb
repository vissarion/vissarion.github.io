# frozen_string_literal: true

require "optparse"
require "pathname"
require "yaml"
require "date"
require "digest"
require "English"

begin
  require "al_folio_core"
rescue LoadError
  # Optional at runtime; fallback lookup paths are used if unavailable.
end

module AlFolioUpgrade
  class CLI
    REPORT_PATH = "al-folio-upgrade-report.md"
    OVERRIDE_ACK_PATH = ".al-folio-overrides.yml"

    Finding = Struct.new(:id, :severity, :message, :file, :line, :snippet, keyword_init: true)
    OverrideResult = Struct.new(
      :local_path,
      :plugin_path,
      :owner,
      :version,
      :local_sha,
      :upstream_sha,
      :status,
      keyword_init: true
    )

    FILE_GLOBS = [
      "_config.yml",
      "_includes/**/*.{liquid,html}",
      "_layouts/**/*.{liquid,html}",
      "_pages/**/*.{md,markdown,liquid,html}",
      "_posts/**/*.{md,markdown,liquid,html}",
      "assets/js/**/*.js",
      "assets/css/**/*.css",
      "assets/tailwind/**/*.css",
    ].freeze

    IGNORE_PATH_PATTERNS = [
      /\/distillpub\//,
      /\/search\/ninja-footer\.min\.js$/,
      /\/bootstrap\.bundle\.min\.js$/,
      /\/bootstrap-toc\.min\.js$/,
      /\.min\.js$/,
      /\.map$/,
    ].freeze

    SAFE_REPLACEMENTS = [
      { from: /\bfont-weight-bold\b/, to: "font-bold" },
      { from: /\bfont-weight-medium\b/, to: "font-medium" },
      { from: /\bfont-weight-lighter\b/, to: "font-light" },
      { from: %r{https://distill\.pub/template\.v2\.js}, to: "/assets/js/distillpub/template.v2.js" },
      { from: %r{assets/tailwind/input\.css}, to: "assets/tailwind/app.css" },
    ].freeze

    CORE_OVERRIDE_FILES = %w[
      _includes/head.liquid
      _includes/scripts.liquid
      _layouts/default.liquid
      _layouts/post.liquid
      _layouts/page.liquid
      _layouts/distill.liquid
      assets/js/common.js
      assets/js/theme.js
      assets/js/tooltips-setup.js
      assets/tailwind/app.css
      tailwind.config.js
    ].freeze

    PLUGIN_OWNED_LOCAL_PATHS = {
      "_plugins/external-posts.rb" => "al_ext_posts",
      "_plugins/google-scholar-citations.rb" => "al_citations",
      "_plugins/inspirehep-citations.rb" => "al_citations",
      "_plugins/hide-custom-bibtex.rb" => "al_folio_core",
      "_plugins/details.rb" => "al_folio_core",
      "_plugins/file-exists.rb" => "al_folio_core",
      "_plugins/remove-accents.rb" => "al_folio_core",
      "assets/js/distillpub/**/*" => "al_folio_distill",
      "assets/js/search/**/*" => "al_search",
      "assets/webfonts/**/*" => "al_icons",
      "assets/fonts/academicons.*" => "al_icons",
      "assets/fonts/scholar-icons.*" => "al_icons"
    }.freeze

    def initialize(root: Dir.pwd, stdout: $stdout, stderr: $stderr)
      @root = Pathname.new(root)
      @stdout = stdout
      @stderr = stderr
    end

    def run(argv)
      return usage(1) if argv.empty?

      command = argv.shift
      unless command == "upgrade"
        @stderr.puts("Unsupported command: #{command}")
        return usage(1)
      end

      subcommand = argv.shift
      case subcommand
      when "audit"
        options = { fail_on_blocking: true }
        OptionParser.new do |opts|
          opts.on("--no-fail", "Do not fail even when blocking findings exist") do
            options[:fail_on_blocking] = false
          end
        end.parse!(argv)

        findings = audit
        write_report(findings)
        print_summary(findings)
        return 1 if options[:fail_on_blocking] && blocking?(findings)

        0
      when "apply"
        options = { safe: false }
        OptionParser.new do |opts|
          opts.on("--safe", "Apply only deterministic safe codemods") do
            options[:safe] = true
          end
        end.parse!(argv)

        unless options[:safe]
          @stderr.puts("Only --safe mode is supported in v1.x.")
          return 1
        end

        changed_files = apply_safe_codemods
        findings = audit
        write_report(findings)
        @stdout.puts("Applied safe codemods to #{changed_files} file(s).")
        print_summary(findings)
        0
      when "report"
        findings = audit
        write_report(findings)
        print_summary(findings)
        0
      when "overrides"
        run_overrides(argv)
      else
        @stderr.puts("Unsupported subcommand: #{subcommand.inspect}")
        usage(1)
      end
    end

    private

    def usage(code)
      @stdout.puts("Usage: al-folio upgrade [audit|apply --safe|report|overrides] [--no-fail]")
      @stdout.puts("       al-folio upgrade overrides audit [--fail-on-stale]")
      @stdout.puts("       al-folio upgrade overrides diff LOCAL_PATH")
      @stdout.puts("       al-folio upgrade overrides accept [--all|LOCAL_PATH ...]")
      code
    end

    def run_overrides(argv)
      command = argv.shift
      case command
      when "audit"
        options = { fail_on_stale: false }
        OptionParser.new do |opts|
          opts.on("--fail-on-stale", "Exit non-zero when stale/unacknowledged overrides are found") do
            options[:fail_on_stale] = true
          end
        end.parse!(argv)

        results = local_override_results
        print_override_audit(results)
        return 1 if options[:fail_on_stale] && override_attention_required?(results)

        0
      when "diff"
        local_path = argv.shift
        if local_path.nil? || local_path.empty?
          @stderr.puts("Usage: al-folio upgrade overrides diff LOCAL_PATH")
          return 1
        end

        diff_override(local_path)
      when "accept"
        options = { all: false }
        OptionParser.new do |opts|
          opts.on("--all", "Acknowledge all detected local overrides") do
            options[:all] = true
          end
        end.parse!(argv)

        paths = options[:all] ? :all : argv
        acknowledge_overrides(paths)
      else
        @stderr.puts("Unsupported overrides subcommand: #{command.inspect}")
        usage(1)
      end
    end

    def blocking?(findings)
      findings.any? { |finding| finding.severity == :blocking }
    end

    def audit
      findings = []
      check_manifest_contract(findings)
      check_config_contract(findings)
      check_legacy_assets(findings)
      check_legacy_patterns(findings)
      check_distill_runtime(findings)
      check_local_override_drift(findings)
      check_plugin_owned_local_assets(findings)
      findings
    end

    def check_manifest_contract(findings)
      manifests = manifest_paths
      if manifests.empty?
        findings << Finding.new(
          id: "missing_migration_manifests",
          severity: :warning,
          message: "No migration manifests found. Install/update `al_folio_core` to get release contracts.",
          file: "migrations/",
          line: 1,
          snippet: "Expected at least one `x.y.z_to_a.b.c.yml` manifest."
        )
      end
    end

    def check_config_contract(findings)
      config_path = @root.join("_config.yml")
      return unless config_path.file?

      content = config_path.read
      parsed = begin
        parse_yaml(content) || {}
      rescue StandardError => e
        findings << Finding.new(
          id: "invalid_config_yaml",
          severity: :blocking,
          message: "_config.yml could not be parsed: #{e.message}",
          file: "_config.yml",
          line: 1,
          snippet: "Fix YAML syntax before running upgrade codemods."
        )
        return
      end

      al_folio = parsed.is_a?(Hash) ? parsed["al_folio"] : nil
      unless al_folio.is_a?(Hash)
        findings << Finding.new(
          id: "missing_al_folio_namespace",
          severity: :blocking,
          message: "Missing `al_folio` config namespace required for v1.x.",
          file: "_config.yml",
          line: 1,
          snippet: "Add al_folio.api_version, style_engine, compat, and upgrade keys."
        )
        return
      end

      unless al_folio["style_engine"] == "tailwind"
        findings << Finding.new(
          id: "style_engine_not_tailwind",
          severity: :blocking,
          message: "`al_folio.style_engine` should be set to `tailwind` for v1.x.",
          file: "_config.yml",
          line: 1,
          snippet: "Set al_folio.style_engine: tailwind"
        )
      end

      unless al_folio["tailwind"].is_a?(Hash)
        findings << Finding.new(
          id: "missing_tailwind_namespace",
          severity: :warning,
          message: "Missing `al_folio.tailwind` namespace for v1 tailwind runtime contract.",
          file: "_config.yml",
          line: 1,
          snippet: "Add al_folio.tailwind.version/preflight/css_entry."
        )
      end

      unless al_folio["distill"].is_a?(Hash)
        findings << Finding.new(
          id: "missing_distill_namespace",
          severity: :warning,
          message: "Missing `al_folio.distill` namespace for Distill runtime contract.",
          file: "_config.yml",
          line: 1,
          snippet: "Add al_folio.distill.engine/source/allow_remote_loader."
        )
      end

      plugins = Array(parsed["plugins"]).map(&:to_s)
      return if plugins.include?("al_icons")

      findings << Finding.new(
        id: "missing_al_icons_plugin",
        severity: :warning,
        message: "Missing `al_icons` in plugin list; icon runtime ownership moved out of core.",
        file: "_config.yml",
        line: 1,
        snippet: "Add `- al_icons` under plugins."
      )
    end

    def check_legacy_assets(findings)
      files = ["_includes/head.liquid", "_includes/scripts.liquid"]
      patterns = [
        /bootstrap\.min\.css/,
        /mdbootstrap|mdb\.min\.(?:css|js)/,
        /third_party_libraries\.jquery/,
        /bootstrap\.bundle\.min\.js/,
      ]

      files.each do |file|
        path = @root.join(file)
        next unless path.file?

        path.each_line.with_index(1) do |line, number|
          next unless patterns.any? { |pattern| line.match?(pattern) }

          findings << Finding.new(
            id: "legacy_bootstrap_runtime_asset",
            severity: :blocking,
            message: "Legacy Bootstrap/jQuery/MDB runtime assets are still referenced in core includes.",
            file: file,
            line: number,
            snippet: line.strip
          )
        end
      end
    end

    def check_legacy_patterns(findings)
      each_candidate_file do |relative, line, number|
        if line.match?(/data-toggle\s*=\s*["'](?:collapse|dropdown|tooltip|popover|table)["']/)
          findings << Finding.new(
            id: "legacy_data_toggle",
            severity: :warning,
            message: "Legacy Bootstrap `data-toggle` marker found.",
            file: relative,
            line: number,
            snippet: line.strip
          )
        end

        if line.match?(/\$\(|jQuery\b/)
          findings << Finding.new(
            id: "legacy_jquery_usage",
            severity: :warning,
            message: "jQuery usage found; migrate to vanilla JS APIs.",
            file: relative,
            line: number,
            snippet: line.strip
          )
        end
      end
    end

    def check_distill_runtime(findings)
      config_path = @root.join("_config.yml")
      allow_remote_loader = false
      if config_path.file?
        begin
          parsed = parse_yaml(config_path.read) || {}
          allow_remote_loader = parsed.dig("al_folio", "distill", "allow_remote_loader") == true
        rescue StandardError
          allow_remote_loader = false
        end
      end

      return if allow_remote_loader

      distill_runtime_paths.each do |transforms_path|
        report_file = if transforms_path.to_s.start_with?("#{@root}#{File::SEPARATOR}")
                        transforms_path.relative_path_from(@root).to_s
                      else
                        "al_folio_distill:#{transforms_path}"
                      end

        transforms_path.each_line.with_index(1) do |line, number|
          next unless line.match?(%r{https://distill\.pub/template\.v2\.js})

          findings << Finding.new(
            id: "distill_remote_loader_enabled",
            severity: :blocking,
            message: "Distill runtime still references remote template loader while allow_remote_loader is false.",
            file: report_file,
            line: number,
            snippet: line.strip
          )
        end
      end
    end

    def distill_runtime_paths
      paths = [@root.join("assets/js/distillpub/transforms.v2.js")]
      specs = []
      specs << Gem.loaded_specs["al_folio_distill"] if Gem.loaded_specs.key?("al_folio_distill")
      begin
        specs << Gem::Specification.find_by_name("al_folio_distill")
      rescue Gem::LoadError
        # Optional gem; ignore when not installed.
      end

      specs.compact.uniq(&:full_gem_path).each do |spec|
        paths << Pathname.new(File.join(spec.full_gem_path, "assets/js/distillpub/transforms.v2.js"))
      end
      paths.select(&:file?).uniq
    end

    def check_local_override_drift(findings)
      local_override_results.each do |override|
        case override.status
        when :identical
          findings << Finding.new(
            id: "local_override_identical",
            severity: :warning,
            message: "Local override is identical to `#{override.owner}` #{override.version}; remove it unless it is intentional.",
            file: override.local_path,
            line: 1,
            snippet: "Matches #{override.owner}:#{override.plugin_path}."
          )
        when :unacknowledged
          findings << Finding.new(
            id: "local_override_unacknowledged",
            severity: :warning,
            message: "Local override shadows `#{override.owner}` #{override.version}; review and acknowledge it with `al-folio upgrade overrides accept #{override.local_path}`.",
            file: override.local_path,
            line: 1,
            snippet: "Diff with `al-folio upgrade overrides diff #{override.local_path}`."
          )
        when :stale
          findings << Finding.new(
            id: "local_override_upstream_changed",
            severity: :warning,
            message: "`#{override.owner}` changed the upstream file since this local override was acknowledged.",
            file: override.local_path,
            line: 1,
            snippet: "Reconcile with `al-folio upgrade overrides diff #{override.local_path}`."
          )
        when :local_changed
          findings << Finding.new(
            id: "local_override_changed_since_ack",
            severity: :warning,
            message: "Local override changed since it was last acknowledged.",
            file: override.local_path,
            line: 1,
            snippet: "Review and re-acknowledge with `al-folio upgrade overrides accept #{override.local_path}`."
          )
        end
      end
    end

    def local_override_results
      acknowledgements = override_acknowledgements
      override_candidates.map do |candidate|
        local_sha = sha256(@root.join(candidate[:local_path]))
        upstream_sha = sha256(candidate[:plugin_absolute_path])
        ack = acknowledgements[candidate[:local_path]]

        status = if local_sha == upstream_sha
                   :identical
                 elsif ack.nil?
                   :unacknowledged
                 elsif ack["upstream_sha256"] != upstream_sha
                   :stale
                 elsif ack["local_sha256"] != local_sha
                   :local_changed
                 else
                   :acknowledged
                 end

        OverrideResult.new(
          local_path: candidate[:local_path],
          plugin_path: candidate[:plugin_path],
          owner: candidate[:owner],
          version: candidate[:version],
          local_sha: local_sha,
          upstream_sha: upstream_sha,
          status: status
        )
      end.sort_by(&:local_path)
    end

    def print_override_audit(results)
      if results.empty?
        @stdout.puts("No local overrides shadowing installed al-folio plugin files were detected.")
        return
      end

      @stdout.puts("Detected #{results.length} local override(s):")
      results.each do |override|
        @stdout.puts(
          "- #{override.local_path}: #{override.status} " \
          "(#{override.owner} #{override.version}, upstream #{override.plugin_path})"
        )
      end
      @stdout.puts("Acknowledgement file: #{OVERRIDE_ACK_PATH}")
    end

    def diff_override(local_path)
      override = local_override_results.find { |result| result.local_path == normalize_relative_path(local_path) }
      unless override
        @stderr.puts("No plugin-owned override found for #{local_path}.")
        return 1
      end

      candidate = override_candidates.find { |entry| entry[:local_path] == override.local_path }
      system("diff", "-u", candidate[:plugin_absolute_path].to_s, @root.join(override.local_path).to_s)
      $CHILD_STATUS&.exitstatus || 0
    end

    def acknowledge_overrides(paths)
      results = local_override_results
      selected = if paths == :all
                   results
                 else
                   wanted = Array(paths).map { |path| normalize_relative_path(path) }
                   results.select { |result| wanted.include?(result.local_path) }
                 end

      if selected.empty?
        @stderr.puts("No matching local overrides to acknowledge.")
        return 1
      end

      data = override_ack_file
      data["version"] = 1
      data["overrides"] ||= {}

      selected.each do |override|
        data["overrides"][override.local_path] = {
          "owner" => override.owner,
          "gem_version" => override.version,
          "upstream_path" => override.plugin_path,
          "upstream_sha256" => override.upstream_sha,
          "local_sha256" => override.local_sha,
          "acknowledged_at" => Date.today.iso8601,
        }
      end

      sorted = data["overrides"].sort.to_h
      data["overrides"] = sorted
      File.write(@root.join(OVERRIDE_ACK_PATH), YAML.dump(data))
      @stdout.puts("Acknowledged #{selected.length} local override(s) in #{OVERRIDE_ACK_PATH}.")
      0
    end

    def override_attention_required?(results)
      results.any? { |result| %i[unacknowledged stale local_changed identical].include?(result.status) }
    end

    def override_candidates
      plugin_file_entries.each_with_object([]) do |entry, candidates|
        local = @root.join(entry[:local_path])
        next unless local.file?

        candidates << entry
      end
    end

    def plugin_file_entries
      entries = []
      al_folio_plugin_specs.each do |spec|
        root = Pathname.new(spec.full_gem_path)
        next unless root.directory?

        plugin_globs.each do |glob|
          Dir.glob(root.join(glob)).sort.each do |path|
            next unless File.file?(path)

            plugin_path = Pathname.new(path).relative_path_from(root).to_s
            local_path = local_override_path(plugin_path)
            next unless local_path

            entries << {
              local_path: local_path,
              plugin_path: plugin_path,
              plugin_absolute_path: Pathname.new(path),
              owner: spec.name,
              version: spec.version.to_s,
            }
          end
        end
      end

      entries.uniq { |entry| [entry[:owner], entry[:local_path], entry[:plugin_path]] }
    end

    def plugin_globs
      [
        "_includes/**/*",
        "_layouts/**/*",
        "_sass/**/*",
        "assets/**/*",
        "templates/**/*",
        "lib/assets/**/*",
        "lib/templates/**/*",
      ]
    end

    def local_override_path(plugin_path)
      case plugin_path
      when %r{\Atemplates/(.+)}
        "_includes/#{Regexp.last_match(1)}"
      when %r{\Alib/templates/(.+)}
        "_includes/#{Regexp.last_match(1)}"
      when %r{\Alib/assets/(.+)}
        "assets/#{Regexp.last_match(1)}"
      when %r{\A(?:_includes|_layouts|_sass|assets)/}
        plugin_path
      end
    end

    def al_folio_plugin_specs
      Gem::Specification.each.select do |spec|
        spec.name.start_with?("al_") && File.directory?(spec.full_gem_path)
      end
    end

    def override_ack_file
      path = @root.join(OVERRIDE_ACK_PATH)
      return {} unless path.file?

      parsed = parse_yaml(path.read) || {}
      parsed.is_a?(Hash) ? parsed : {}
    rescue StandardError
      {}
    end

    def override_acknowledgements
      overrides = override_ack_file["overrides"]
      overrides.is_a?(Hash) ? overrides.transform_keys(&:to_s) : {}
    end

    def sha256(path)
      Digest::SHA256.file(path).hexdigest
    end

    def normalize_relative_path(path)
      Pathname.new(path.to_s).cleanpath.to_s.sub(%r{\A\./}, "")
    end

    def check_core_override_drift(findings)
      return unless using_core_theme?

      CORE_OVERRIDE_FILES.each do |relative|
        path = @root.join(relative)
        next unless path.file?

        findings << Finding.new(
          id: "core_override_drift",
          severity: :warning,
          message: "Local override shadows `al_folio_core` theme file and may need manual review during upgrades.",
          file: relative,
          line: 1,
          snippet: "Local override present."
        )
      end
    end

    def check_plugin_owned_local_assets(findings)
      PLUGIN_OWNED_LOCAL_PATHS.each do |glob, owner_plugin|
        Dir.glob(@root.join(glob)).sort.each do |path|
          next unless File.file?(path)

          relative = Pathname.new(path).relative_path_from(@root).to_s
          findings << Finding.new(
            id: "plugin_owned_local_asset",
            severity: :warning,
            message: "Local asset path is plugin-owned by `#{owner_plugin}` and may drift from release contracts.",
            file: relative,
            line: 1,
            snippet: "Prefer plugin-managed runtime assets for this path."
          )
        end
      end
    end

    def each_candidate_file
      FILE_GLOBS.each do |glob|
        Dir.glob(@root.join(glob)).sort.each do |path|
          next unless File.file?(path)
          next if ignored_path?(path)

          rel = Pathname.new(path).relative_path_from(@root).to_s
          File.foreach(path).with_index(1) do |line, number|
            yield rel, line, number
          end
        end
      end
    end

    def apply_safe_codemods
      changed_files = 0

      each_text_file do |path|
        original = File.read(path)
        updated = original.dup

        SAFE_REPLACEMENTS.each do |rule|
          updated = updated.gsub(rule[:from], rule[:to])
        end

        if Pathname.new(path).relative_path_from(@root).to_s == "_config.yml"
          updated = ensure_al_folio_namespace(updated)
        end

        next if updated == original

        File.write(path, updated)
        changed_files += 1
      end

      changed_files
    end

    def ensure_al_folio_namespace(content)
      if content.match?(/^al_folio:\s*$/)
        content = ensure_tailwind_namespace(content)
        content = ensure_distill_namespace(content)
        return content
      end

      block = <<~YAML

        al_folio:
          api_version: 1
          style_engine: tailwind
          tailwind:
            version: 4.1.18
            preflight: false
            css_entry: assets/tailwind/app.css
          distill:
            engine: distillpub-template
            source: al-org-dev/distill-template#al-folio
            allow_remote_loader: true
          compat:
            bootstrap:
              enabled: false
              support_window: v1.0-v1.2
              deprecates_in: v1.3
              removed_in: v2.0
          upgrade:
            channel: stable
            auto_apply_safe_fixes: false
      YAML
      content + block
    end

    def ensure_tailwind_namespace(content)
      return content if nested_namespace_present?(content, "tailwind")

      insertion = [
        "  tailwind:",
        "    version: 4.1.18",
        "    preflight: false",
        "    css_entry: assets/tailwind/app.css",
      ].join("\n")
      content.sub(/^al_folio:\s*$/) { |match| "#{match}\n#{insertion}" }
    end

    def ensure_distill_namespace(content)
      return content if nested_namespace_present?(content, "distill")

      insertion = [
        "  distill:",
        "    engine: distillpub-template",
        "    source: al-org-dev/distill-template#al-folio",
        "    allow_remote_loader: true",
      ].join("\n")
      content.sub(/^al_folio:\s*$/) { |match| "#{match}\n#{insertion}" }
    end

    def nested_namespace_present?(content, key)
      parsed = parse_yaml(content) || {}
      return false unless parsed.is_a?(Hash)

      al_folio = parsed["al_folio"]
      al_folio.is_a?(Hash) && al_folio[key].is_a?(Hash)
    rescue StandardError
      false
    end

    def using_core_theme?
      config_path = @root.join("_config.yml")
      return false unless config_path.file?

      begin
        parsed = parse_yaml(config_path.read) || {}
        parsed["theme"] == "al_folio_core" || Array(parsed["plugins"]).include?("al_folio_core")
      rescue StandardError
        false
      end
    end

    def parse_yaml(content)
      YAML.safe_load(content, permitted_classes: [Date, Time], aliases: true)
    end

    def manifest_paths
      if defined?(AlFolioCore) && AlFolioCore.respond_to?(:migration_manifest_paths)
        return Array(AlFolioCore.migration_manifest_paths).select { |path| File.file?(path) }
      end

      Dir.glob(@root.join("migrations/*.yml")).sort
    end

    def each_text_file
      FILE_GLOBS.each do |glob|
        Dir.glob(@root.join(glob)).sort.each do |path|
          next unless File.file?(path)
          next if ignored_path?(path)

          yield path
        end
      end
    end

    def ignored_path?(path)
      normalized = path.to_s
      IGNORE_PATH_PATTERNS.any? { |pattern| normalized.match?(pattern) }
    end

    def write_report(findings)
      by_severity = findings.group_by(&:severity)
      blocking = by_severity.fetch(:blocking, [])
      warning = by_severity.fetch(:warning, [])

      File.write(@root.join(REPORT_PATH), <<~MD)
        # al-folio upgrade report

        Generated by `bundle exec al-folio upgrade report`.

        ## Summary

        - Blocking findings: #{blocking.count}
        - Non-blocking findings: #{warning.count}

        ## Blocking

        #{format_findings(blocking)}

        ## Non-blocking

        #{format_findings(warning)}
      MD
    end

    def format_findings(findings)
      return "- None\n" if findings.empty?

      findings.map do |finding|
        "- [#{finding.id}] #{finding.message} (`#{finding.file}:#{finding.line}`)\n  - Snippet: `#{finding.snippet}`"
      end.join("\n") + "\n"
    end

    def print_summary(findings)
      blocking = findings.count { |finding| finding.severity == :blocking }
      warning = findings.count { |finding| finding.severity == :warning }
      @stdout.puts("Upgrade audit complete. Blocking: #{blocking}, Non-blocking: #{warning}.")
      @stdout.puts("Report: #{REPORT_PATH}")
    end
  end
end
