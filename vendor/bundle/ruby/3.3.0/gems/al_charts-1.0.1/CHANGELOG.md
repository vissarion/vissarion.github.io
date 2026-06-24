# Changelog

## 1.0.1 - 2026-06-01

- Rewrote `chartjs-setup.js` in vanilla JS. It was still jQuery (`$(document).ready`, `$(".language-chartjs")`), so with jQuery removed in al-folio v1 it threw `ReferenceError: $ is not defined` on load and every `chartjs` chart silently failed to render. Behavior and the `.language-chartjs` selector are unchanged.

## 0.1.0 - 2026-02-07

- Initial gem release.
- Added standalone charts and diagram asset pipeline for Jekyll/al-folio sites.
