#!/usr/bin/env bash
# run_corpus.sh - R21: corpus verdict runner (absolute-path safe).
# Usage:
#   bash tools/run_corpus.sh                          # all cases + diff
#   CASE=021_sub_call TIMEOUT=60 EXE=msvc bash tools/run_corpus.sh
set -euo pipefail
REPO_ROOT="${1:-$(cd "$(dirname "$0")/.." && pwd)}"
CASE_ID="${CASE_ID:-}"
TIMEOUT="${TIMEOUT:-0}"
EXE="${EXE:-both}"
cd "$REPO_ROOT"

args=(harness/runners/run_case.py)
if [ -n "$CASE_ID" ]; then args+=("$CASE_ID"); else args+=(--all); fi
[ "$TIMEOUT" -gt 0 ] && args+=(--timeout "$TIMEOUT")
args+=(--exe "$EXE")

python "${args[@]}" 2>&1 | tail -n 5

if [ -z "$CASE_ID" ] && [ "$SKIP_DIFF" != "1" ]; then
  echo "=====DIFF====="
  python harness/runners/diff_case.py --all 2>&1 | tail -n 10
fi
