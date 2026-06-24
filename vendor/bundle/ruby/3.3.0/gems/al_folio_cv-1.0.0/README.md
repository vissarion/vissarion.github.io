# al-folio-cv

CV rendering plugin for al-folio v1.x.

## What it provides

- Shared CV rendering templates used by `layout: cv`
- CV helper Liquid tag: `{% al_folio_cv_render %}`
- Packaged stylesheet at `/assets/css/al-folio-cv.css`

## Usage

Add the gem and plugin:

```ruby
gem 'al_folio_cv'
```

```yml
plugins:
  - al_folio_cv
al_folio:
  features:
    cv:
      enabled: true
```

`al_folio_core` delegates `layout: cv` rendering to this plugin.
