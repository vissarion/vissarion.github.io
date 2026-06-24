# frozen_string_literal: true

require "jekyll"
require_relative "al_folio_bootstrap_compat/version"

module AlFolioBootstrapCompat
  PLUGIN_ROOT = File.expand_path("..", __dir__)
  ASSETS_ROOT = File.join(PLUGIN_ROOT, "assets")
  SUPPORT_MESSAGE = "enabled (supported through v1.2, deprecated in v1.3, removed in v2.0)"

  class PluginStaticFile < Jekyll::StaticFile; end

  class AssetsGenerator < Jekyll::Generator
    safe true
    priority :low

    def generate(site)
      return unless site.config.dig("al_folio", "compat", "bootstrap", "enabled") == true

      Dir.glob(File.join(ASSETS_ROOT, "**", "*")).sort.each do |source_path|
        next if File.directory?(source_path)

        relative_dir = File.dirname(source_path).sub("#{PLUGIN_ROOT}/", "")
        site.static_files << PluginStaticFile.new(site, PLUGIN_ROOT, relative_dir, File.basename(source_path))
      end
    end
  end
end

Jekyll::Hooks.register :site, :post_read do |site|
  enabled = site.config.dig("al_folio", "compat", "bootstrap", "enabled") == true
  next unless enabled

  Jekyll.logger.info("al_folio_bootstrap_compat:", AlFolioBootstrapCompat::SUPPORT_MESSAGE)
end
