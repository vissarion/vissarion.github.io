# al-citations

`al_citations` fetches and renders citation counts for `al-folio` v1.x and compatible Jekyll sites.

## Installation

```ruby
gem 'al_citations'
```

```yaml
plugins:
  - al_citations
```

## Usage

Google Scholar:

```liquid
{% google_scholar_citations scholar_id article_id %}
```

InspireHEP:

```liquid
{% inspirehep_citations recid %}
```

Example:

```liquid
Citations: {% google_scholar_citations "YOUR_SCHOLAR_ID" "ARTICLE_ID" %} InspireHEP Citations: {% inspirehep_citations "INSPIRE_RECID" %}
```

## Ecosystem context

- Starter integration/docs live in `al-folio`.
- Citation provider logic and rendering behavior are owned here.

## Contributing

Provider adapters, formatting, and reliability fixes should be contributed in this repository.
