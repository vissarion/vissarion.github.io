# Changelog

## 1.0.3 - 2026-05-25

- Added local override audit/diff/acknowledgement commands so customized sites can detect when local copies shadow changed plugin-owned files.

## 1.0.2 - 2026-05-24

- Expanded upgrade audit coverage for legacy local plugins and runtime assets that are now owned by v1 feature gems.

## 1.0.1 - 2026-02-17

- Added ownership-aware checks for plugin-owned local runtime assets (icons/search paths).
- Added config contract warning when `al_icons` is missing from plugin wiring.

## 1.0.0 - 2026-02-08

- Initial release.
- Added `al-folio upgrade audit`, `al-folio upgrade apply --safe`, and `al-folio upgrade report`.
