#!/usr/bin/env bash
# post_build.sh - R21: one-shot post-build pipeline (run AFTER "[msvc_build] OK").
# 1) refresh symsnap.txt  2) deploy exe to C:\pectest  3) write DEPLOYED_BUILD.txt stamp.
# Usage: bash tools/post_build.sh [repo_root] [deploy_dir(C:/style)]
set -euo pipefail
REPO_ROOT="${1:-$(cd "$(dirname "$0")/.." && pwd)}"
DEPLOY_DIR="${2:-C:/pectest}"
EXE_SRC="$REPO_ROOT/build/msvc/pecmd_msvc.exe"
EXE_DST="$DEPLOY_DIR/pecmd_msvc.exe"
[ -f "$EXE_SRC" ] || { echo "ERROR: exe missing: $EXE_SRC (build first)" >&2; exit 1; }

# 1) symsnap refresh (V-Gate gate-0 source of bp addresses)
bash "$(dirname "$0")/make_symsnap.sh" "$REPO_ROOT"

# 2) deploy (kill first: GUI message pump locks the exe)
taskkill //F //IM pecmd_msvc.exe >/dev/null 2>&1 || true
cp -f "$EXE_SRC" "$EXE_DST"

# 3) identity stamp (V3)
MD5="$(md5sum "$EXE_DST" | cut -d' ' -f1)"
HASH="$(git -C "$REPO_ROOT" rev-parse --short HEAD 2>/dev/null || echo unknown)"
LINE="hash=$HASH md5=$MD5 time=$(date '+%Y-%m-%d %H:%M:%S')"
printf '%s\n' "$LINE" > "$DEPLOY_DIR/DEPLOYED_BUILD.txt"
echo "$LINE"
