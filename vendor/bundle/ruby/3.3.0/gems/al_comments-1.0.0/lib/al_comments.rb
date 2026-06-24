require 'jekyll'
require 'json'

module AlComments
  class CommentsTag < Liquid::Tag
    def render(context)
      site = context.registers[:site]
      return '' unless site

      page = context.registers[:page] || context['page'] || {}
      output = []
      disqus_shortname = resolve_disqus_shortname(site)

      if disqus_shortname && truthy?(fetch_key(page, 'disqus_comments'))
        output << disqus_html(site, page, disqus_shortname)
      end

      if truthy?(fetch_key(page, 'giscus_comments'))
        output << giscus_html(site, page)
      end

      output.join("\n")
    end

    private

    def truthy?(value)
      return false if value.nil? || value == false
      return true if value == true
      return true if value.is_a?(Numeric) && value != 0

      normalized = value.to_s.strip.downcase
      %w[true 1 yes y on].include?(normalized)
    end

    def post_layout?(page)
      fetch_key(page, 'layout').to_s == 'post'
    end

    def fetch_key(hash, *keys)
      return nil unless hash.respond_to?(:key?)

      keys.each do |key|
        return hash[key] if hash.key?(key)

        string_key = key.to_s
        return hash[string_key] if hash.key?(string_key)

        symbol_key = key.respond_to?(:to_sym) ? key.to_sym : nil
        return hash[symbol_key] if symbol_key && hash.key?(symbol_key)
      end

      nil
    end

    def value_blank?(value)
      value.nil? || value.to_s.strip.empty?
    end

    def resolve_disqus_shortname(site)
      direct = fetch_key(site.config, 'disqus_shortname')
      return direct.to_s unless value_blank?(direct)

      disqus = fetch_key(site.config, 'disqus')
      return nil unless disqus.is_a?(Hash)

      nested = fetch_key(disqus, 'shortname')
      return nil if value_blank?(nested)

      nested.to_s
    end

    def resolve_giscus_config(site)
      giscus = fetch_key(site.config, 'giscus')
      return giscus if giscus.is_a?(Hash)

      comments = fetch_key(site.config, 'comments')
      return {} unless comments.is_a?(Hash)

      nested = fetch_key(comments, 'giscus')
      nested.is_a?(Hash) ? nested : {}
    end

    def resolve_giscus_repo(_site, giscus)
      repo = fetch_key(giscus, 'repo', 'repository', 'repo_name')
      return '' if value_blank?(repo)

      repo.to_s
    end

    def missing_giscus_fields(giscus, repo)
      required = {
        'repo' => repo,
        'repo_id' => fetch_key(giscus, 'repo_id'),
        'category' => fetch_key(giscus, 'category'),
        'category_id' => fetch_key(giscus, 'category_id')
      }

      required.each_with_object([]) do |(name, value), missing|
        missing << name if value_blank?(value)
      end
    end

    def giscus_warning_html(style, spacer, missing_fields)
      details = if missing_fields.empty?
                  ''
                else
                  "<p>Missing required keys: <code>#{missing_fields.join(', ')}</code>.</p>"
                end

      warning = <<~HTML
        <blockquote class="block-danger">
          <h5>giscus comments misconfigured</h5>
          <p>Please follow instructions at <a href="http://giscus.app">http://giscus.app</a> and update your giscus configuration.</p>
          #{details}
        </blockquote>
      HTML

      %(<div id="giscus_thread"#{style}>#{spacer}\n#{warning}</div>)
    end

    def disqus_html(site, page, disqus_shortname)
      max_width = fetch_key(site.config, 'max_width')
      <<~HTML
        <div id="disqus_thread" style="max-width: #{max_width}; margin: 0 auto;">
          <script type="text/javascript">
            var disqus_shortname  = #{disqus_shortname.to_json};
            var disqus_identifier = #{fetch_key(page, 'id').to_s.to_json};
            var disqus_title      = #{fetch_key(page, 'title').to_s.to_json};
            (function() {
              var dsq = document.createElement('script');
              dsq.type = 'text/javascript';
              dsq.async = true;
              dsq.src = '//' + disqus_shortname + '.disqus.com/embed.js';
              (document.getElementsByTagName('head')[0] || document.getElementsByTagName('body')[0]).appendChild(dsq);
            })();
          </script>
          <noscript>
            Please enable JavaScript to view the
            <a href="https://disqus.com/?ref_noscript">comments powered by Disqus.</a>
          </noscript>
        </div>
      HTML
    end

    def giscus_html(site, page)
      giscus = resolve_giscus_config(site)
      repo = resolve_giscus_repo(site, giscus)
      max_width = fetch_key(site.config, 'max_width')
      style = post_layout?(page) ? " style=\"max-width: #{max_width}; margin: 0 auto;\"" : ''
      spacer = post_layout?(page) ? "\n  <br>" : ''
      missing_fields = missing_giscus_fields(giscus, repo)

      unless missing_fields.empty?
        return giscus_warning_html(style, spacer, missing_fields)
      end

      <<~HTML
        <div id="giscus_thread"#{style}>#{spacer}
          <script>
            (function setupGiscus() {
              function determineGiscusTheme() {
                #{theme_detection(site)}
              }

              var giscusTheme = determineGiscusTheme();
              var attrs = {
                src: "https://giscus.app/client.js",
                "data-repo": #{repo.to_s.to_json},
                "data-repo-id": #{fetch_key(giscus, 'repo_id').to_s.to_json},
                "data-category": #{fetch_key(giscus, 'category').to_s.to_json},
                "data-category-id": #{fetch_key(giscus, 'category_id').to_s.to_json},
                "data-mapping": #{fetch_key(giscus, 'mapping').to_s.to_json},
                "data-strict": #{fetch_key(giscus, 'strict').to_s.to_json},
                "data-reactions-enabled": #{fetch_key(giscus, 'reactions_enabled').to_s.to_json},
                "data-emit-metadata": #{fetch_key(giscus, 'emit_metadata').to_s.to_json},
                "data-input-position": #{fetch_key(giscus, 'input_position').to_s.to_json},
                "data-theme": giscusTheme,
                "data-lang": #{fetch_key(giscus, 'lang').to_s.to_json},
                crossorigin: "anonymous",
                async: true
              };

              var giscusScript = document.createElement("script");
              Object.entries(attrs).forEach(function(entry) { giscusScript.setAttribute(entry[0], entry[1]); });
              var host = document.getElementById("giscus_thread");
              if (host) host.appendChild(giscusScript);
            })();
          </script>
          <noscript>
            Please enable JavaScript to view the
            <a href="http://giscus.app/?ref_noscript">comments powered by giscus.</a>
          </noscript>
        </div>
      HTML
    end

    def theme_detection(site)
      giscus = resolve_giscus_config(site)
      dark_theme = fetch_key(giscus, 'dark_theme').to_s
      light_theme = fetch_key(giscus, 'light_theme').to_s
      dark_theme = 'dark' if value_blank?(dark_theme)
      light_theme = 'light' if value_blank?(light_theme)

      if fetch_key(site.config, 'enable_darkmode')
        <<~JS
          var theme = localStorage.getItem("theme") || document.documentElement.getAttribute("data-theme") || "system";
          if (theme === "dark") return #{dark_theme.to_json};
          if (theme === "light") return #{light_theme.to_json};
          var prefersDark = window.matchMedia("(prefers-color-scheme: dark)").matches;
          return prefersDark ? #{dark_theme.to_json} : #{light_theme.to_json};
        JS
      else
        %(return #{light_theme.to_json};)
      end
    end
  end
end

Liquid::Template.register_tag('al_comments', AlComments::CommentsTag)
