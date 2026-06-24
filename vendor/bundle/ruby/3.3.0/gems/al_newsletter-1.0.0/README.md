# Al-Newsletter

A Jekyll plugin that provides a reusable newsletter form and JS handlers.

## Installation

```ruby
gem 'al_newsletter'
```

Enable in `_config.yml`:

```yaml
plugins:
  - al_newsletter
```

## Usage

```liquid
{% al_newsletter_form align=center margin=true %}
```

```liquid
{% al_newsletter_scripts %}
```
