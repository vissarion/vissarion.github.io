#!/usr/bin/env bash
set -euo pipefail

# Sync vendored Distill runtime assets from a pinned upstream ref.
#
# Default upstream source:
#   https://github.com/al-org-dev/distill-template.git (branch: al-folio)
#
# Usage:
#   scripts/distill/sync_distill.sh [upstream-ref]
#
# Examples:
#   scripts/distill/sync_distill.sh
#   scripts/distill/sync_distill.sh <commit-sha>

UPSTREAM_REPO="${UPSTREAM_REPO:-https://github.com/al-org-dev/distill-template.git}"
UPSTREAM_BRANCH="${UPSTREAM_BRANCH:-al-folio}"
DEFAULT_UPSTREAM_REF="d907ccdb526166c615f53487ec01e92e92f28f46"
UPSTREAM_REF="${1:-${UPSTREAM_REF:-$DEFAULT_UPSTREAM_REF}}"

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TMP_DIR="$(mktemp -d)"

cleanup() {
  rm -rf "${TMP_DIR}"
}
trap cleanup EXIT

git clone --quiet --depth 1 --branch "${UPSTREAM_BRANCH}" "${UPSTREAM_REPO}" "${TMP_DIR}/distill-template"
pushd "${TMP_DIR}/distill-template" >/dev/null

# Ensure deterministic vendoring from an explicit ref.
git fetch --quiet --depth 1 origin "${UPSTREAM_REF}"
git checkout --quiet "${UPSTREAM_REF}"

OUT_DIR="${ROOT}/assets/js/distillpub"
mkdir -p "${OUT_DIR}"
cp dist/template.v2.js "${OUT_DIR}/template.v2.js"
cp dist/template.v2.js.map "${OUT_DIR}/template.v2.js.map"
cp dist/transforms.v2.js "${OUT_DIR}/transforms.v2.js"
cp dist/transforms.v2.js.map "${OUT_DIR}/transforms.v2.js.map"
cp dist/overrides.js "${OUT_DIR}/overrides.js"

SOURCE_COMMIT="$(git rev-parse HEAD)"
SOURCE_COMMIT_SHORT="$(git rev-parse --short HEAD)"
popd >/dev/null

TEMPLATE_SHA256="$(shasum -a 256 "${OUT_DIR}/template.v2.js" | awk '{print $1}')"
TRANSFORMS_SHA256="$(shasum -a 256 "${OUT_DIR}/transforms.v2.js" | awk '{print $1}')"
SYNCED_AT_UTC="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"

cat > "${OUT_DIR}/provenance.json" <<JSON
{
  "upstream_repo": "${UPSTREAM_REPO}",
  "upstream_branch": "${UPSTREAM_BRANCH}",
  "upstream_ref": "${SOURCE_COMMIT}",
  "upstream_ref_short": "${SOURCE_COMMIT_SHORT}",
  "synced_at_utc": "${SYNCED_AT_UTC}",
  "toolchain": {
    "sync_mode": "copy-dist-artifacts"
  },
  "remote_loader_patched": false,
  "assets": {
    "template.v2.js": "${TEMPLATE_SHA256}",
    "transforms.v2.js": "${TRANSFORMS_SHA256}",
    "overrides.js": "$(shasum -a 256 "${OUT_DIR}/overrides.js" | awk '{print $1}')"
  }
}
JSON

echo "Synced Distill runtime from ${UPSTREAM_REPO}@${SOURCE_COMMIT}"
echo "Updated assets in ${OUT_DIR}"
