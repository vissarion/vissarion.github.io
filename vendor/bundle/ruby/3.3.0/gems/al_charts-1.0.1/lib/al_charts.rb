require 'jekyll'

module AlCharts
  PLUGIN_NAME = 'al_charts'
  ASSETS_DIR = 'assets'
  JS_DIR = 'js'

  class PluginStaticFile < Jekyll::StaticFile; end

  class AssetsGenerator < Jekyll::Generator
    safe true
    priority :low

    def generate(site)
      plugin_lib_path = File.expand_path('.', __dir__)
      Dir.glob(File.join(plugin_lib_path, ASSETS_DIR, PLUGIN_NAME, JS_DIR, '*.js')).each do |source_path|
        site.static_files << PluginStaticFile.new(site, plugin_lib_path, File.join(ASSETS_DIR, PLUGIN_NAME, JS_DIR), File.basename(source_path))
      end
    end
  end

  class ChartsScriptsTag < Liquid::Tag
    def render(context)
      site = context.registers[:site]
      return '' unless site

      page = context.registers[:page] || context['page'] || {}
      cfg = site.config
      libs = cfg['third_party_libraries'] || {}
      baseurl = cfg['baseurl'] || ''
      out = []

      if truthy?(nested_value(page, 'mermaid', 'enabled'))
        out << %(<script defer src="#{libs.dig('mermaid', 'url', 'js')}" integrity="#{libs.dig('mermaid', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        if truthy?(nested_value(page, 'mermaid', 'zoomable'))
          out << %(<script defer src="#{libs.dig('d3', 'url', 'js')}" integrity="#{libs.dig('d3', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        end
        out << %(<script defer src="#{baseurl}/assets/al_charts/js/mermaid-setup.js" type="text/javascript"></script>)
      end

      if truthy?(page['code_diff'])
        out << %(<script src="#{libs.dig('diff2html', 'url', 'js')}" integrity="#{libs.dig('diff2html', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        out << %(<script defer src="#{baseurl}/assets/al_charts/js/diff2html-setup.js" type="text/javascript"></script>)
      end

      if truthy?(page['map'])
        out << %(<script src="#{libs.dig('leaflet', 'url', 'js')}" integrity="#{libs.dig('leaflet', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        out << %(<script defer src="#{baseurl}/assets/al_charts/js/leaflet-setup.js" type="text/javascript"></script>)
      end

      if truthy?(nested_value(page, 'chart', 'chartjs'))
        out << %(<script defer src="#{libs.dig('chartjs', 'url', 'js')}" integrity="#{libs.dig('chartjs', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        out << %(<script defer src="#{baseurl}/assets/al_charts/js/chartjs-setup.js" type="text/javascript"></script>)
      end

      if truthy?(nested_value(page, 'chart', 'echarts'))
        out << %(<script src="#{libs.dig('echarts', 'url', 'js', 'library')}" integrity="#{libs.dig('echarts', 'integrity', 'js', 'library')}" crossorigin="anonymous"></script>)
        if truthy?(cfg['enable_darkmode'])
          out << %(<script src="#{libs.dig('echarts', 'url', 'js', 'dark_theme')}" integrity="#{libs.dig('echarts', 'integrity', 'js', 'dark_theme')}" crossorigin="anonymous"></script>)
        end
        out << %(<script defer src="#{baseurl}/assets/al_charts/js/echarts-setup.js" type="text/javascript"></script>)
      end

      if truthy?(nested_value(page, 'chart', 'plotly'))
        out << %(<script defer src="#{libs.dig('plotly', 'url', 'js')}" integrity="#{libs.dig('plotly', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        out << %(<script defer src="#{baseurl}/assets/al_charts/js/plotly-setup.js" type="text/javascript"></script>)
      end

      if truthy?(nested_value(page, 'chart', 'vega_lite'))
        out << %(<script defer src="#{libs.dig('vega', 'url', 'js')}" integrity="#{libs.dig('vega', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        out << %(<script defer src="#{libs.dig('vega-lite', 'url', 'js')}" integrity="#{libs.dig('vega-lite', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        out << %(<script defer src="#{libs.dig('vega-embed', 'url', 'js')}" integrity="#{libs.dig('vega-embed', 'integrity', 'js')}" crossorigin="anonymous"></script>)
        out << %(<script defer src="#{baseurl}/assets/al_charts/js/vega-setup.js" type="text/javascript"></script>)
      end

      out.join("\n")
    end

    private

    def truthy?(value)
      value == true || value.to_s == 'true'
    end

    def nested_value(container, *keys)
      current = container
      keys.each do |key|
        return nil unless current.respond_to?(:[])

        current = current[key]
      end
      current
    end
  end
end

Liquid::Template.register_tag('al_charts_scripts', AlCharts::ChartsScriptsTag)
