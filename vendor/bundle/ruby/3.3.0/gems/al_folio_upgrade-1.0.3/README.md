# al-folio-upgrade

`al_folio_upgrade` is the upgrade CLI for `al-folio` v1.x.

## Commands

- `al-folio upgrade audit`
- `al-folio upgrade apply --safe`
- `al-folio upgrade report`
- `al-folio upgrade overrides audit`
- `al-folio upgrade overrides diff LOCAL_PATH`
- `al-folio upgrade overrides accept [--all|LOCAL_PATH ...]`

## What it checks

- Core config contract (`al_folio.*`, Tailwind, Distill)
- Required plugin ownership wiring (for example `al_icons`)
- Legacy Bootstrap/jQuery markers
- Distill remote-loader policy
- Local override drift when `theme: al_folio_core` is enabled
- Plugin-owned local asset drift (for example copied search, icon, Distill, citation, and external-post runtime files)
- Local files that shadow installed plugin-owned layouts, includes, Sass, templates, and assets
- Migration manifest availability from `al_folio_core`

## Local override workflow

Customized sites can keep local copies of plugin-owned files, but those copies do not produce Git merge conflicts when the owning gem changes. Use the overrides workflow after dependency updates:

1. Run `bundle exec al-folio upgrade overrides audit`.
2. For every stale or unacknowledged override, run `bundle exec al-folio upgrade overrides diff LOCAL_PATH`.
3. Reconcile the local file with the plugin-owned upstream file.
4. Run `bundle exec al-folio upgrade overrides accept LOCAL_PATH` or `bundle exec al-folio upgrade overrides accept --all`.

Acknowledgements are stored in `.al-folio-overrides.yml`. Commit that file in customized sites so future gem updates can flag upstream changes explicitly.

## Ecosystem context

- Starter execution/docs live in `al-folio`.
- Upgrade policy/audit behavior is owned by this plugin.

## Contributing

Audit/apply/report logic updates should be proposed in this repository.
