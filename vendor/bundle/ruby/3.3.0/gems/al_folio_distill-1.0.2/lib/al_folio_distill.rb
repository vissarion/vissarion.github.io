# frozen_string_literal: true

require "jekyll"
require_relative "al_folio_distill/version"

module AlFolioDistill
  PLUGIN_ROOT = File.expand_path("..", __dir__)
  TEMPLATES_ROOT = File.join(PLUGIN_ROOT, "templates")
  ASSETS_ROOT = File.join(PLUGIN_ROOT, "assets")
  DISTILL_REMOTE_LOADER_PATTERN = %r{https://distill\.pub/template\.v2\.js}

  class PluginStaticFile < Jekyll::StaticFile; end

  module_function

  def enabled?(site)
    site.config.dig("al_folio", "features", "distill", "enabled") != false
  end

  def remote_loader_allowed?(site)
    site.config.dig("al_folio", "distill", "allow_remote_loader") == true
  end

  class AssetsGenerator < Jekyll::Generator
    safe true
    priority :low

    def generate(site)
      return unless AlFolioDistill.enabled?(site)

      Dir.glob(File.join(ASSETS_ROOT, "**", "*")).sort.each do |source_path|
        next if File.directory?(source_path)

        relative_dir = File.dirname(source_path).sub("#{PLUGIN_ROOT}/", "")
        site.static_files << PluginStaticFile.new(site, PLUGIN_ROOT, relative_dir, File.basename(source_path))
      end
    end
  end

  class RenderTag < Liquid::Tag
    def render(context)
      site = context.registers[:site]
      return "" unless site && AlFolioDistill.enabled?(site)

      Liquid::Template.parse("{% include distill/render.liquid %}").render!(
        context.environments.first,
        registers: context.registers
      )
    end
  end
end

Liquid::Template.register_tag("al_folio_distill_render", AlFolioDistill::RenderTag)

Jekyll::Hooks.register :site, :after_init do |site|
  next unless site.respond_to?(:includes_load_paths)

  include_path = AlFolioDistill::TEMPLATES_ROOT
  site.includes_load_paths << include_path unless site.includes_load_paths.include?(include_path)
end

Jekyll::Hooks.register :site, :post_read do |site|
  next unless AlFolioDistill.enabled?(site)
  next if AlFolioDistill.remote_loader_allowed?(site)

  transforms_path = File.join(AlFolioDistill::ASSETS_ROOT, "js", "distillpub", "transforms.v2.js")
  next unless File.file?(transforms_path)

  content = File.read(transforms_path)
  if content.match?(AlFolioDistill::DISTILL_REMOTE_LOADER_PATTERN)
    Jekyll.logger.warn("al_folio_distill:", "remote Distill template loader detected while `al_folio.distill.allow_remote_loader` is false")
  end
end
