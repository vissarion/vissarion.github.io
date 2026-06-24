# al-icons

`al-icons` owns icon runtime assets for al-folio v1.x.

## Responsibilities

- Render icon stylesheet tags via `{% al_icons_styles %}`
- Centralize pinned icon CDN URLs and optional SRI usage
- Keep icon ownership out of `al_folio_core` and starter runtime files

## Usage

Add plugin to `_config.yml`:

```yml
plugins:
  - al_icons
```

Add parse-safe include wrapper in theme/head templates:

```liquid
{% include plugins/al_icons_styles.liquid %}
```

Expected config:

```yml
third_party_libraries:
  fontawesome:
    url:
      css: https://cdn.jsdelivr.net/npm/@fortawesome/fontawesome-free@7.2.0/css/all.min.css
    integrity:
      css: ...
  academicons:
    url:
      css: https://cdn.jsdelivr.net/npm/academicons@1.9.5/css/academicons.min.css
    integrity:
      css: ...
  scholar-icons:
    url:
      css: https://cdn.jsdelivr.net/npm/scholar-icons@1.0.3/css/scholar-icons.css
    integrity:
      css: ...
```

`integrity.css` is optional per library; when omitted, the tag renders without integrity/crossorigin attributes.
