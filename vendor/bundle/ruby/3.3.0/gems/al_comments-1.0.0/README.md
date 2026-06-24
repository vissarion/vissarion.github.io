# Al-Comments

A Jekyll plugin that provides reusable comment integrations for al-folio-compatible themes.

## Features

- Giscus comments
- Disqus comments
- Theme-aware Giscus setup

## Installation

Add this to your `Gemfile`:

```ruby
gem 'al_comments'
```

Then run:

```bash
bundle install
```

Enable in `_config.yml`:

```yaml
plugins:
  - al_comments
```

Render comments where needed:

```liquid
{% al_comments %}
```
