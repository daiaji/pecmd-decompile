#!/usr/bin/env bash
# build_msvc.sh - R23: thin bash wrapper over build_msvc.py (UTF-8 chain).
# Usage:
#   bash tools/build_msvc.sh              full build -> build/msvc/pecmd_msvc.exe
#   bash tools/build_msvc.sh syntax       syntax gate (cl /Zs, no link)
# See tools/build_msvc.py for the full design notes (R23: zero-CJK cmd layer,
# full UTF-8, relative-path rsp/bat, green gate preserved).
# NOTE: exec python with a RELATIVE script path (cwd=self dir) — passing a
# POSIX path built by bash to Windows python trips MSYS arg conversion.
set -euo pipefail
cd "$(dirname "$0")"
exec python build_msvc.py "$@"