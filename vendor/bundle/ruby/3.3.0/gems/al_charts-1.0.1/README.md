# al-charts

`al_charts` provides chart/diagram runtime loading and setup logic for `al-folio` v1.x.

## Installation

```ruby
gem 'al_charts'
```

```yaml
plugins:
  - al_charts
```

## Usage

Render chart runtime assets:

```liquid
{% al_charts_scripts %}
```

## Ecosystem context

- Starter wiring and examples live in `al-folio`.
- Shared theme/runtime contracts are defined by `al_folio_core`.

## Contributing

Chart runtime behavior and edge cases should be contributed in this repository.
