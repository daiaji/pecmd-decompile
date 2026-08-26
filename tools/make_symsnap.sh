#!/usr/bin/env bash
# make_symsnap.sh - V-Gate gate-0: refresh build/msvc/symsnap.txt after a green build.
# Sole legal source of breakpoint addresses (discipline V1). ASCII-only comments.
# R23: awk dependency removed (broken in this env); map parsed by inline python.
set -euo pipefail
REPO_ROOT="${1:-$(cd "$(dirname "$0")/.." && pwd)}"
MAP="$REPO_ROOT/build/msvc/pecmd_msvc.map"
EXE="$REPO_ROOT/build/msvc/pecmd_msvc.exe"
[ -f "$MAP" ] || { echo "ERROR: map missing: $MAP" >&2; exit 1; }
[ -f "$EXE" ] || { echo "ERROR: exe missing: $EXE" >&2; exit 1; }

HASH="$(git -C "$REPO_ROOT" rev-parse --short HEAD 2>/dev/null || true)"
BTIME="$(date -r "$EXE" '+%Y-%m-%d %H:%M:%S' 2>/dev/null || date '+%Y-%m-%d %H:%M:%S')"
OUT="$REPO_ROOT/build/msvc/symsnap.txt"
MAP_WIN="$(cd "$REPO_ROOT" && pwd -W 2>/dev/null)/build/msvc/pecmd_msvc.map" || MAP_WIN="$MAP"

python -c '
import re, sys, os
syms = ["PECMD_ProcessScriptBlock","PECMD_ExecCmdDispatch","PECMD_ExpandEnvVars",
        "PECMD_MainW","PECMD_RunCommand","PECMD_RunScriptText","PECMD_WaitTickCount",
        "PECMD_CreateProcessW","PECMD_OpenFileHandle","PECMD_GetExitCodeGlobal",
        "PECMD_DispatchBuiltin","XDigit","PECMD_RunStartupScript","PECMD_RunBootScriptInFiber"]
map_file = sys.argv[1]
out_file = sys.argv[2]
base = 0x140000000
found = {}
with open(map_file, "rb") as f:
    for raw in f:
        line = raw.decode("ascii", errors="replace")
        parts = line.split()
        if len(parts) >= 3 and parts[1] in syms:
            try:
                va = int(parts[2], 16)
            except ValueError:
                continue
            found.setdefault(parts[1], va)
lines = ["# symsnap syms-parsed-by-python"]
for s in syms:
    if s in found:
        lines.append("%s 0x%x" % (s, found[s] - base))
    else:
        lines.append("%s NOT_FOUND" % s)
with open(out_file, "w", encoding="utf-8", newline="\n") as f:
    f.write("\n".join(lines) + "\n")
' "$MAP" "$OUT"

{ echo "# symsnap build=$HASH time=$BTIME"
  echo "# RVA = VA - 0x140000000 (map col 3). Sole legal source for bp addresses (V1)."
  tail -n +2 "$OUT"
} > "$OUT.tmp" && mv "$OUT.tmp" "$OUT"
echo "symsnap written: $OUT ($(wc -l < "$OUT") lines)"