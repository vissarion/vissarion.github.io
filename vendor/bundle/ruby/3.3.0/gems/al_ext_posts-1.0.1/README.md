# al-ext-posts

`al_ext_posts` imports and renders external posts for `al-folio` v1.x and compatible Jekyll sites.

## Installation

```ruby
gem 'al_ext_posts'
```

```yaml
plugins:
  - al_ext_posts
```

## Usage

Configure external sources in `_config.yml`:

```yaml
external_sources:
  - name: "My Blog"
    rss_url: "https://myblog.com/feed.xml"
    categories: ["external", "blog"]
    tags: ["rss", "updates"]
  - name: "Another Source"
    categories: ["external"]
    tags: ["manual-curation"]
    posts:
      - url: "https://example.com/post1"
        published_date: "2024-03-20"
      - url: "https://example.com/post2"
        published_date: "2024-03-21"
```

Supported source types:

- RSS feeds (`rss_url`)
- Manual URL entries (`posts` with `url` + `published_date`)

## Ecosystem context

- Starter demo content/wiring lives in `al-folio`.
- External post ingestion logic is owned here.

## Contributing

Parser/source behavior changes should be contributed in this repository.
