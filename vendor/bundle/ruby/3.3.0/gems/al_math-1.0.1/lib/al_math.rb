require 'jekyll'

module AlMath
  PLUGIN_NAME = 'al_math'
  ASSETS_DIR = 'assets'

  class PluginStaticFile < Jekyll::StaticFile; end

  class AssetsGenerator < Jekyll::Generator
    safe true
    priority :low

    def generate(site)
      plugin_lib_path = File.expand_path('.', __dir__)
      Dir.glob(File.join(plugin_lib_path, ASSETS_DIR, PLUGIN_NAME, '**', '*')).each do |source_path|
        next if File.directory?(source_path)

        relative_dir = File.dirname(source_path).sub("#{plugin_lib_path}/", '')
        site.static_files << PluginStaticFile.new(site, plugin_lib_path, relative_dir, File.basename(source_path))
      end
    end
  end

  class MathStylesTag < Liquid::Tag
    def render(context)
      site = context.registers[:site]
      page = context.registers[:page] || context['page'] || {}
      return '' unless site
      return '' unless truthy?(page['tikzjax'])

      libs = site.config['third_party_libraries'] || {}
      tikz_css = libs.dig('tikzjax', 'url', 'css')
      return '' if tikz_css.to_s.empty?

      tikz_integrity = libs.dig('tikzjax', 'integrity', 'css')
      if tikz_integrity.to_s.empty?
        %(<link defer rel="stylesheet" type="text/css" href="#{tikz_css}" crossorigin="anonymous">)
      else
        %(<link defer rel="stylesheet" type="text/css" href="#{tikz_css}" integrity="#{tikz_integrity}" crossorigin="anonymous">)
      end
    end

    private

    def truthy?(value)
      value == true || value.to_s == 'true'
    end
  end

  class MathScriptsTag < Liquid::Tag
    def render(context)
      site = context.registers[:site]
      page = context.registers[:page] || context['page'] || {}
      return '' unless site

      cfg = site.config
      libs = cfg['third_party_libraries'] || {}
      baseurl = cfg['baseurl'] || ''
      out = []

      if truthy?(cfg['enable_math'])
        out << %(<script defer type="text/javascript" id="MathJax-script" src="#{libs.dig('mathjax', 'url', 'js')}" integrity="#{libs.dig('mathjax', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        if truthy?(page['pseudocode'])
          out << %(<script src="#{baseurl}/assets/al_math/js/pseudocode-setup.js"></script>)
          out << %(<script type="text/javascript" src="#{libs.dig('pseudocode', 'url', 'js')}" integrity="#{libs.dig('pseudocode', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        else
          out << %(<script src="#{baseurl}/assets/al_math/js/mathjax-setup.js"></script>)
          out << %(<script defer src="#{libs.dig('polyfill', 'url', 'js')}" crossorigin="anonymous"></script>)
        end
      end

      if truthy?(page['tikzjax'])
        tikz_js = libs.dig('tikzjax', 'url', 'js')
        tikz_integrity = libs.dig('tikzjax', 'integrity', 'js')
        if !tikz_js.to_s.empty?
          if tikz_integrity.to_s.empty?
            out << %(<script defer src="#{tikz_js}" type="text/javascript" crossorigin="anonymous"></script>)
          else
            out << %(<script defer src="#{tikz_js}" type="text/javascript" integrity="#{tikz_integrity}" crossorigin="anonymous"></script>)
          end
        end
      end

      out.join("\n")
    end

    private

    def truthy?(value)
      value == true || value.to_s == 'true'
    end
  end
end

Liquid::Template.register_tag('al_math_styles', AlMath::MathStylesTag)
Liquid::Template.register_tag('al_math_scripts', AlMath::MathScriptsTag)
