#!/bin/bash
# run_case.sh — WIN 端真机用例执行器 (§4.C.1-0.6 win_real 后端, §8.4 三步)
#
# 三步协议: 部署 → 运行 → 回捞
#   部署: 将用例目录复制到 C:\pectest\<case>\
#   运行: PECMD.EXE LOAD main.pecmd (gui=false 可无人值守; gui=true 需人工在场)
#   回捞: 收集 out/ 产物到 results/<backend>/<case>/ (按 manifest 校验完整性)
#
# 用法 (WIN 端, WSL 或 git-bash):
#   bash run_case.sh <case_id> [--out <相对输出目录>]
#   例:  bash run_case.sh 001_envi_smoke
# 产物回 Linux 后: python3 harness/runners/diff_case.py 001_envi_smoke

set -euo pipefail

HARNESS_DIR="$(cd "$(dirname "$0")/.." && pwd)"
CASE_ID="${1:?用法: run_case.sh <case_id>}"
CASE_SRC="$HARNESS_DIR/corpus/cases/$CASE_ID"
MANIFEST="$CASE_SRC/manifest.json"
OUT_DIR="${2:-$HARNESS_DIR/results/win_real/$CASE_ID}"

PECMD_EXE="${PECMD_EXE:-C:\\pectest\\PECMD.EXE}"   # 原版 EXE 路径(真机)
PECTEST_ROOT="${PECTEST_ROOT:-C:\\pectest}"
CASE_DST="${PECTEST_ROOT}\\${CASE_ID}"

[ -f "$MANIFEST" ] || { echo "FAIL: manifest 缺失 $MANIFEST"; exit 1; }

# --- 1. 部署 ---
echo "[1/4] 部署 $CASE_ID → $CASE_DST"
mkdir -p "$OUT_DIR"
cmd.exe /c "if exist ${CASE_DST//\//\\} rmdir /s /q ${CASE_DST//\//\\}" 2>/dev/null || true
# 用 cp 到 WSL 可见的挂载点 (WSL 下 C: 盘 = /mnt/c)
MNT_DST="${PECTEST_ROOT/C:/\/mnt\/c}"
mkdir -p "$MNT_DST/${CASE_ID}"
cp "$CASE_SRC"/main.pecmd "$MNT_DST/${CASE_ID}/"

# --- 2. 生成 epilogue (按 manifest vars, 单行合并版, 不依赖 WRITE -a) ---
echo "[2/4] 生成 epilogue"
VARS=$(python3 -c "import json;print('|'.join(json.load(open('$MANIFEST'))['vars']))")
cat > "$MNT_DST/${CASE_ID}/epilogue.pecmd" <<EOF
ENVI T_CASE=$CASE_ID
WRITE $PECTEST_ROOT\\out\\vars.txt,CASE=$CASE_ID|$VARS
ENVI T_DONE=OK
WRITE $PECTEST_ROOT\\out\\done.txt,%T_DONE%
EOF

# --- 3. 运行 (原版 EXE; 重定向到用例目录的 out.ini) ---
echo "[3/4] 运行 PECMD (timeout=${TIMEOUT_S:-60}s)"
mkdir -p "$MNT_DST/out"
# 将主脚本 + epilogue 合并为单入口 (epilogue 在 main 之后执行)
cat "$MNT_DST/${CASE_ID}/main.pecmd" "$MNT_DST/${CASE_ID}/epilogue.pecmd" \
    > "$MNT_DST/${CASE_ID}/run_all.pecmd"
(cd "$MNT_DST" && timeout "${TIMEOUT_S:-60}" "$PECMD_EXE" LOAD "$PECTEST_ROOT\\${CASE_ID}\\run_all.pecmd") \
    > "$OUT_DIR/stdout.txt" 2>&1
echo "exit=$?" | tee "$OUT_DIR/exit.txt"

# --- 4. 回捞 ---
echo "[4/4] 回捞产物"
for f in vars.txt done.txt; do
    if [ -f "$MNT_DST/out/$f" ]; then
        cp "$MNT_DST/out/$f" "$OUT_DIR/$f"
    else
        echo "WARN: 产物缺失 $f (用例可能失败)" | tee -a "$OUT_DIR/exit.txt"
    fi
done
echo "完成 → $OUT_DIR (拷回 Linux 后运行 diff_case.py)"