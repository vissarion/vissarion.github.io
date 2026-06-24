# Changelog

## 1.0.2 - 2026-02-17

- Finalized plugin-owned lightbox runtime by shipping a vanilla Lightbox2-compatible adapter path.
- Ensured static asset generator picks up all plugin runtime assets under `lib/assets/al_img_tools/**`.
- Added test coverage for adapter CSS/JS packaging contracts.
- Reduced noisy static-file logging by treating unchanged asset copies as debug-level skips.

## 1.0.1 - 2026-02-17

- Replaced Lightbox2 CDN runtime dependency with a plugin-owned vanilla lightbox adapter.
- Added plugin-owned lightbox adapter CSS/JS assets and static asset registration.
- Removed jQuery dependency from medium zoom initialization.

## 0.1.0 - 2026-02-07

- Initial gem release.
- Added standalone image tooling tags and assets (zoom, gallery, sliders, lightboxes).
