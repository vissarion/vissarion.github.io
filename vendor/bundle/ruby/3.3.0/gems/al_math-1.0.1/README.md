# Al-Math

A Jekyll plugin that provides math-related assets and script loading.

## Installation

```ruby
gem 'al_math'
```

Enable in `_config.yml`:

```yaml
plugins:
  - al_math
```

## Usage

```liquid
{% al_math_styles %}
{% al_math_scripts %}
```

TikZJax runtime assets are loaded from `third_party_libraries.tikzjax` when a page sets `tikzjax: true`.

```yaml
third_party_libraries:
  tikzjax:
    url:
      css: https://cdn.jsdelivr.net/npm/@planktimerr/tikzjax@1.0.8/dist/fonts.css
      js: https://cdn.jsdelivr.net/npm/@planktimerr/tikzjax@1.0.8/dist/tikzjax.js
    integrity:
      css: <optional-sri-hash>
      js: <optional-sri-hash>
```
