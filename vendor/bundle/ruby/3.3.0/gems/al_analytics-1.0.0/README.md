# Al-Analytics

A Jekyll plugin that provides integrations with various analytics services for al-folio sites.

## Supported Analytics Services

- Google Analytics
- Cronitor Analytics
- Pirsch Analytics
- OpenPanel Analytics

## Installation

Add this line to your Jekyll site's Gemfile:

```ruby
gem 'al_analytics'
```

And then execute:

```bash
$ bundle install
```

## Usage

1. Add the plugin to your site's `_config.yml`:

```yaml
plugins:
  - al_analytics
```

2. Configure your analytics services in `_config.yml`:

```yaml
enable_cookie_consent: false

enable_google_analytics: true
google_analytics: "G-XXXXXXXXXX"

enable_cronitor_analytics: false
cronitor_analytics: "XXXXXXXXX"

enable_pirsch_analytics: false
pirsch_analytics: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

enable_openpanel_analytics: false
openpanel_analytics: "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"
```

The plugin also supports the legacy `analytics:` hash used by earlier releases.

3. Add the analytics tag to your layout file (e.g., `_layouts/default.html`):

```liquid
{% al_analytics_scripts %}
```

## Development

After checking out the repo, run `bundle install` to install dependencies.

## Contributing

Bug reports and pull requests are welcome on GitHub.
