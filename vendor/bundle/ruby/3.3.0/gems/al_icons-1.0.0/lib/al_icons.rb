# frozen_string_literal: true

require 'jekyll'
require 'liquid'
require_relative 'al_icons/version'

module AlIcons
  LIBRARIES = {
    'fontawesome' => ['fontawesome', 'Font Awesome'],
    'academicons' => ['academicons', 'Academicons'],
    'scholar-icons' => ['scholar-icons', 'Scholar Icons']
  }.freeze

  class IconStylesTag < Liquid::Tag
    def render(context)
      site = context.registers[:site]
      return '' unless site

      libs = site.config['third_party_libraries'] || {}

      rendered = LIBRARIES.map do |config_key, display_name|
        key, label = display_name
        cfg = libs[key] || libs[config_key]
        next '' unless cfg.is_a?(Hash)

        href = cfg.dig('url', 'css')
        next '' unless href.to_s.strip != ''

        integrity = cfg.dig('integrity', 'css')
        if integrity.to_s.strip != ''
          <<~HTML
            <!-- #{label} -->
            <link defer rel="stylesheet" href="#{href}" integrity="#{integrity}" crossorigin="anonymous">
          HTML
        else
          <<~HTML
            <!-- #{label} -->
            <link defer rel="stylesheet" href="#{href}">
          HTML
        end
      end

      rendered.reject(&:empty?).join("\n")
    end
  end
end

Liquid::Template.register_tag('al_icons_styles', AlIcons::IconStylesTag)
