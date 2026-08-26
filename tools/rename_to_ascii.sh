#!/usr/bin/env bash
# rename_to_ascii.sh - R23: post-rename reference updater for the project root
# folder rename D:\repo\PECMD反编译 -> D:\repo\pecmd-decompile.
# Run AFTER the folder has been renamed (from anywhere; it resolves the repo
# root from its own location). Only active docs are rewritten; historical
# archives (analysis/, REVIEW.md, legacy docs) keep their text per discipline,
# and AGENTS.md gets a conversion note instead. ASCII-only comments.
set -euo pipefail
cd "$(dirname "$0")"
ROOT="$(cd .. && pwd)"
NEW_NAME="pecmd-decompile"
OLD_ABS_WIN="D:\\repo\\PECMD反编译"
OLD_ABS_UNIX="D:/repo/PECMD反编译"
NEW_ABS_WIN="D:\\repo\\${NEW_NAME}"
NEW_ABS_UNIX="D:/repo/${NEW_NAME}"

# Active docs whitelist (rewrite in place). Historical files stay untouched.
ACTIVE=(
  "$ROOT/AGENTS.md"
  "$ROOT/.agents/skills/pecmd-build/SKILL.md"
  "$ROOT/docs/WINDBG_MCP_ISSUES.md"
)

echo "[rename] repo root: $ROOT"
for f in "${ACTIVE[@]}"; do
  [ -f "$f" ] || { echo "SKIP (missing): $f"; continue; }
  python - "$f" "$OLD_ABS_WIN" "$NEW_ABS_WIN" "$OLD_ABS_UNIX" "$NEW_ABS_UNIX" <<'PYEOF'
import io, os, sys
path, ow, nw, ou, nu = sys.argv[1:]
with io.open(path, "r", encoding="utf-8") as fh:
    t = fh.read()
n = t.count(ow) + t.count(ou)
t = t.replace(ow, nw).replace(ou, nu)
with io.open(path, "w", encoding="utf-8", newline="") as fh:
    fh.write(t)
print("[rename] %-45s %d ref(s)" % (os.path.basename(path), n))
PYEOF
done

# Append conversion note to AGENTS.md transition section (idempotent guard).
NOTE="⚠ R23 改名: 根目录已由 \`D:\\repo\\PECMD反编译\` 更名为 \`D:\\repo\\pecmd-decompile\`；
活性文档已同步, 历史档案中的旧路径按「PECMD反编译 ⇒ pecmd-decompile」换算。"
# Idempotence guard: check the NOTE's own marker (arrow + new name). Checking
# plain "pecmd-decompile" would always match AFTER the path replacement above,
# silently skipping the note forever (R23 bug found during handover review).
grep -q "⇒ pecmd-decompile" "$ROOT/AGENTS.md" || printf '%s\n' "$NOTE" >> "$ROOT/AGENTS.md"

echo "[rename] done. Remaining historical refs (keep as evidence):"
grep -rl "PECMD反编译" "$ROOT" 2>/dev/null | grep -v "^$ROOT/build/" | grep -v "^$ROOT/reference/" | grep -v "__pycache__" | head -20 || true