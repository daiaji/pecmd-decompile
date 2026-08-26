#!/usr/bin/env bash
# make_symsnap.sh - V-Gate gate-0: refresh build/msvc/symsnap.txt after a green build.
# Sole legal source of breakpoint addresses (discipline V1). ASCII-only comments.
set -euo pipefail
REPO_ROOT="${1:-$(cd "$(dirname "$0")/.." && pwd)}"
MAP="$REPO_ROOT/build/msvc/pecmd_msvc.map"
EXE="$REPO_ROOT/build/msvc/pecmd_msvc.exe"
[ -f "$MAP" ] || { echo "ERROR: map missing: $MAP" >&2; exit 1; }
[ -f "$EXE" ] || { echo "ERROR: exe missing: $EXE" >&2; exit 1; }

HASH="$(git -C "$REPO_ROOT" rev-parse --short HEAD 2>/dev/null || true)"
BTIME="$(date -r "$EXE" '+%Y-%m-%d %H:%M:%S' 2>/dev/null || date '+%Y-%m-%d %H:%M:%S')"
OUT="$REPO_ROOT/build/msvc/symsnap.txt"

{
  echo "# symsnap build=$HASH time=$BTIME"
  echo "# RVA = VA - 0x140000000 (map col 3). Sole legal source for bp addresses (V1)."
  for s in PECMD_ProcessScriptBlock PECMD_ExecCmdDispatch PECMD_ExpandEnvVars \
           PECMD_MainW PECMD_RunCommand PECMD_RunScriptText PECMD_WaitTickCount \
           PECMD_CreateProcessW PECMD_OpenFileHandle PECMD_GetExitCodeGlobal \
           PECMD_DispatchBuiltin XDigit PECMD_RunStartupScript PECMD_RunBootScriptInFiber; do
    va=$(awk -v sym="$s" '$2==sym{print $3; exit}' "$MAP")
    if [ -n "${va:-}" ]; then
      rva=$(( 16#$va - 16#140000000 ))
      printf '%s 0x%x\n' "$s" "$rva"
    else
      echo "$s NOT_FOUND"
    fi
  done
} > "$OUT"
echo "symsnap written: $OUT ($(wc -l < "$OUT") lines)"
