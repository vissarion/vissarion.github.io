# al-folio-distill

`al_folio_distill` provides Distill support for `al-folio` v1.x.

## What it provides

- Distill layout renderer tag: `{% al_folio_distill_render %}`
- Distill templates/includes for `layout: distill`
- Distill runtime assets under `/assets/js/distillpub/*`
- Distill stylesheet at `/assets/css/al-folio-distill.css`

## Installation

```ruby
gem 'al_folio_distill'
```

```yaml
plugins:
  - al_folio_distill
al_folio:
  features:
    distill:
      enabled: true
```

`al_folio_core` delegates Distill rendering to this plugin.

## Vendored runtime policy

This gem ships prebuilt Distill runtime assets for end users (no npm at gem-install time).

- Source of truth: `al-org-dev/distill-template` (`al-folio` branch)
- Sync script: `scripts/distill/sync_distill.sh`
- Provenance metadata: `assets/js/distillpub/provenance.json`

Refresh assets:

```bash
./scripts/distill/sync_distill.sh
# or pin a specific ref
./scripts/distill/sync_distill.sh <commit-sha>
```

## Contributing

Distill runtime/template behavior belongs here. Starter-only docs/demo changes belong in `al-folio`.
