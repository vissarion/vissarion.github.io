# Changelog

## 1.0.2 - 2026-06-01

- De-jQueried the vendored `assets/js/distillpub/overrides.js`. Its top-level `$(window).on("load", ...)` threw `ReferenceError: $ is not defined` on every distill page (jQuery was removed in al-folio v1), so the footnote/citation dark-theme overrides never applied and the page logged a console error. Switched to `window.addEventListener("load", ...)`; the handler body was already vanilla. Updated the pinned SHA-256 in `provenance.json` and the runtime contract test, and recorded the change under `local_patches` (port it to the upstream distill-template so a future re-sync preserves it).

## 1.0.1 - 2026-02-17

- Updated Distill scripts template to use `tabs.js` (non-minified path) instead of removed `tabs.min.js`.
- Switched back-to-top loading to the shared CDN contract (`third_party_libraries['vanilla-back-to-top']`) to avoid missing local asset lookups.

## 1.0.0

- Initial Distill extraction from `al_folio_core`.
