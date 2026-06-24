# frozen_string_literal: true

require "jekyll"
require "liquid"
require_relative "al_cookie/version"

module AlCookie
  PLUGIN_ROOT = File.expand_path("..", __dir__)
  TEMPLATES_ROOT = File.join(PLUGIN_ROOT, "lib", "templates")
  ASSETS_ROOT = File.join(PLUGIN_ROOT, "lib", "assets")

  class PluginStaticFile < Jekyll::StaticFile; end

  module_function

  def enabled?(site)
    site.config["enable_cookie_consent"] == true
  end

  def render_setup_script(context)
    template_path = File.join(TEMPLATES_ROOT, "cookie_consent_setup.js.liquid")
    template = Liquid::Template.parse(File.read(template_path))
    payload = context.registers[:site].site_payload
    payload["page"] = context.registers[:page] || {}
    template.render(payload, registers: context.registers)
  end

  class AssetsGenerator < Jekyll::Generator
    safe true
    priority :low

    def generate(site)
      return unless AlCookie.enabled?(site)

      Dir.glob(File.join(ASSETS_ROOT, "**", "*")).sort.each do |source_path|
        next if File.directory?(source_path)

        relative_dir = File.dirname(source_path).sub("#{PLUGIN_ROOT}/", "")
        site.static_files << PluginStaticFile.new(site, PLUGIN_ROOT, relative_dir, File.basename(source_path))
      end
    end
  end

  class CookieStylesTag < Liquid::Tag
    def render(context)
      site = context.registers[:site]
      return "" unless site && AlCookie.enabled?(site)

      libs = site.config["third_party_libraries"] || {}
      <<~HTML
        <link
          defer
          rel="stylesheet"
          href="#{libs.dig("vanilla-cookieconsent", "url", "css")}"
          integrity="#{libs.dig("vanilla-cookieconsent", "integrity", "css")}"
          crossorigin="anonymous"
        >
      HTML
    end
  end

  class CookieScriptsTag < Liquid::Tag
    def render(context)
      site = context.registers[:site]
      return "" unless site && AlCookie.enabled?(site)

      libs = site.config["third_party_libraries"] || {}
      baseurl = site.config["baseurl"] || ""
      setup_script = AlCookie.render_setup_script(context)

      <<~HTML
        <script
          defer
          src="#{libs.dig("vanilla-cookieconsent", "url", "js")}"
          integrity="#{libs.dig("vanilla-cookieconsent", "integrity", "js")}"
          crossorigin="anonymous"
        ></script>
        <script defer src="#{baseurl}/assets/al_cookie/js/cookie-theme-sync.js"></script>
        <script>
        #{setup_script}
        </script>
      HTML
    end
  end
end

Liquid::Template.register_tag("al_cookie_styles", AlCookie::CookieStylesTag)
Liquid::Template.register_tag("al_cookie_scripts", AlCookie::CookieScriptsTag)
