# Al-Folio Bootstrap Compat

Optional compatibility plugin for al-folio v1.x to support legacy Bootstrap-marked content during migration windows.

## Included runtime assets

- `assets/css/bootstrap-compat.css`
- `assets/js/bootstrap-compat.js`

The plugin publishes these assets into the generated site when:

```yaml
al_folio:
  compat:
    bootstrap:
      enabled: true
```

## Support window

- Fully supported through `v1.2`
- Deprecated in `v1.3`
- Removed in `v2.0`
