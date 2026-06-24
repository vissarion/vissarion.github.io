# Al-Img-Tools

A Jekyll plugin that provides various image manipulation features for al-folio sites.

## Features

- Image comparison sliders
- Lightbox-style galleries (vanilla adapter for `data-lightbox` markup)
- Medium zoom
- Image sliders
- PhotoSwipe galleries
- Spotlight galleries
- VenoBox galleries

## Installation

Add this line to your Jekyll site's Gemfile:

```ruby
gem 'al_img_tools'
```

And then execute:

```bash
$ bundle install
```

## Usage

1. Add the plugin to your site's `_config.yml`:

```yaml
plugins:
  - al_img_tools
```

2. Use image features in your pages:

```yaml
---
layout: page
title: Gallery
images:
  lightbox2: true # Enable lightbox adapter (or use `gallery: true`)
  compare: true # Enable image comparison slider
  slider: true # Enable image slider
  photoswipe: true # Enable PhotoSwipe gallery
  spotlight: true # Enable Spotlight gallery
  venobox: true # Enable VenoBox gallery
  medium_zoom: true # Optional per-page medium zoom override
---
```

3. Add the image tags to your layout:

- In the `<head>` (for CSS):

```liquid
{% al_img_tools_styles %}
```

- Before `</body>` (for JavaScript):

```liquid
{% al_img_tools_scripts %}
```

## Development

After checking out the repo, run `bundle install` to install dependencies.

## Contributing

Bug reports and pull requests are welcome on GitHub.
