require 'jekyll'

module AlNewsletter
  PLUGIN_NAME = 'al_newsletter'
  ASSETS_DIR = 'assets'
  JS_DIR = 'js'

  class PluginStaticFile < Jekyll::StaticFile; end

  class AssetsGenerator < Jekyll::Generator
    safe true
    priority :low

    def generate(site)
      plugin_lib_path = File.expand_path('.', __dir__)
      source_path = File.join(plugin_lib_path, ASSETS_DIR, PLUGIN_NAME, JS_DIR, 'newsletter.js')
      return unless File.exist?(source_path)

      site.static_files << PluginStaticFile.new(site, plugin_lib_path, File.join(ASSETS_DIR, PLUGIN_NAME, JS_DIR), 'newsletter.js')
    end
  end

  class NewsletterFormTag < Liquid::Tag
    def initialize(tag_name, markup, tokens)
      super
      @opts = parse_options(markup)
    end

    def render(context)
      site = context.registers[:site]
      return '' unless site
      return '' unless site.config['newsletter'].is_a?(Hash)

      endpoint = site.config.dig('newsletter', 'endpoint').to_s
      align = @opts['align'] || infer_align(@opts)
      justify = case align
                when 'left' then 'flex-start'
                when 'right' then 'flex-end'
                else 'center'
                end
      margin = @opts['margin'] == 'true' ? ' style="margin: 20px"' : ''

      <<~HTML
        <div class="newsletter-form-container"#{margin}>
          <form class="newsletter-form" action="https://app.loops.so/api/newsletter-form/#{endpoint}" method="POST" style="justify-content: #{justify}">
            <input class="newsletter-form-input" name="newsletter-form-input" type="email" placeholder="user@example.com" required="">
            <button type="submit" class="newsletter-form-button" style="justify-content: #{justify}">subscribe</button>
            <button type="button" class="newsletter-loading-button" style="justify-content: #{justify}">Please wait...</button>
          </form>

          <div class="newsletter-success" style="justify-content: #{justify}">
            <p class="newsletter-success-message">You're subscribed!</p>
          </div>

          <div class="newsletter-error" style="justify-content: #{justify}">
            <p class="newsletter-error-message">Oops! Something went wrong, please try again</p>
          </div>

          <button class="newsletter-back-button" type="button" onmouseout='this.style.textDecoration="none"' onmouseover='this.style.textDecoration="underline"'>
            &larr; Back
          </button>
        </div>

        <noscript>
          <style>
            .newsletter-form-container {
              display: none;
            }
          </style>
        </noscript>
      HTML
    end

    private

    def parse_options(markup)
      opts = {}
      markup.to_s.scan(/(\w+)=(\w+)/) { |k, v| opts[k] = v }
      opts
    end

    def infer_align(opts)
      return 'left' if opts['left'] == 'true'
      return 'right' if opts['right'] == 'true'
      'center'
    end
  end

  class NewsletterScriptsTag < Liquid::Tag
    def render(context)
      site = context['site'] || {}
      baseurl = site['baseurl'] || ''
      %(<script defer src="#{baseurl}/assets/al_newsletter/js/newsletter.js"></script>)
    end
  end
end

Liquid::Template.register_tag('al_newsletter_form', AlNewsletter::NewsletterFormTag)
Liquid::Template.register_tag('al_newsletter_scripts', AlNewsletter::NewsletterScriptsTag)
