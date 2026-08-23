#!/usr/bin/env bash
# wine_smoke.sh — PECMD 核心能力 Wine 冒烟验证（PRODUCTION_ROADMAP Phase0 前置探针）
# ⚠️ 本脚本为【用户手跑工具】——代理禁止自动执行(GUI 启动禁令, TASKS §10)
# 用法: bash tools/wine_smoke.sh
# 依赖: 本机 wine/wineserver；Wine 前缀由本脚本初始化(已存在则复用)
# 验证项: 解析器 → ENVI 变量 → %VAR% 展开 → WRITE 文件IO → IFEX $文件存在
#         → HASH 命令(MD5, 新真体) → REGI 注册表写(HKCU) —— 每项独立产物独立判读
set -u
export WINEPREFIX="${WINEPREFIX:-$HOME/pecmd-wine-prefix}"
export WINEARCH=win64
export LC_ALL=C
REPO="$(cd "$(dirname "$0")/.." && pwd)"          # refactored/
EXE_SRC="$REPO/../PECMD原始.EXE"
SLAVE="C:\\pectest"                                # 前缀内工作目录(drive_c/pectest)
WORK="$WINEPREFIX/drive_c/pectest"
SMOKE="$WORK/smoke"

LOG_DIR=/tmp/opencode; mkdir -p "$LOG_DIR"
LOG=$LOG_DIR/wine_smoke.log

pass=0; fail=0; declare -a RESULTS
chk(){ local name="$1" ok="$2"; if [ "$ok" = "1" ]; then RESULTS+=("PASS  $name"); pass=$((pass+1)); else RESULTS+=("FAIL  $name"); fail=$((fail+1)); fi; }

echo "== [1/6] 前缀初始化 =="
if [ ! -d "$WINEPREFIX/drive_c" ]; then
  mkdir -p "$WINEPREFIX"; wineboot -i >/dev/null 2>&1
  sleep 3
fi
[ -d "$WINEPREFIX/drive_c" ] || { echo "FATAL: 前缀初始化失败(见 $LOG)"; exit 2; }
# 无头保险(已有值则不动)
wine reg add 'HKCU\Software\Wine\Drivers' /v Graphics /d null /f >/dev/null 2>&1 || true

echo "== [2/6] 部署 PECMD 与语料 =="
[ -f "$EXE_SRC" ] || { echo "FATAL: 找不到 $EXE_SRC"; exit 2; }
mkdir -p "$SMOKE/out"
cp -f "$EXE_SRC" "$WORK/PECMD.EXE"

printf 'PECMD smoke payload A\r\n' > "$SMOKE/payload_a.txt"

# ---- 脚本体先写 body 文件 ----
cat > "$SMOKE/main_body.ini" <<'INI'
// ---- 能力验证用例(每行一个独立可判读点) ----
ENVI V_A=Hello
ENVI V_B=Wine
WRITE C:\pectest\smoke\out\vars.txt,V_A=%V_A%_V_B=%V_B%

IFEX $C:\pectest\smoke\payload_a.txt,WRITE C:\pectest\smoke\out\ifex.txt,PAYLOAD_SEEN

HASH $C:\pectest\smoke\payload_a.txt,%V_HASH%
WRITE C:\pectest\smoke\out\hash.txt,%V_HASH%

REGI HKCU\Software\PECMD_Smoke\V1=HelloRegi

ENVI T_DONE=OK
WRITE C:\pectest\smoke\out\done.txt,%T_DONE%
INI

# ---- 合成最终 main.ini = UTF-8 BOM + CRLF 正文 ----
# (加载器据 core_b3r_h3.c 的 BOM 表选 CP_UTF8=65001)
sed 's/$/\r/' "$SMOKE/main_body.ini" > "$SMOKE/main_crlf.ini"
printf '\xef\xbb\xbf' > "$SMOKE/main.ini"
cat "$SMOKE/main_crlf.ini" >> "$SMOKE/main.ini"
cp -f "$SMOKE/main.ini" "$SMOKE/main_backup.ini"

echo "== [3/6] 运行 PECMD LOAD =="
cd "$WORK"
timeout 120 wine PECMD.EXE LOAD 'C:\pectest\smoke\main.ini' >"$LOG_DIR/pecmd_run.log" 2>&1
rc=$?
echo "exit=$rc (log: $LOG_DIR/pecmd_run.log)"
wineserver -w 2>/dev/null || true

echo "== [4/6] 判读 =="
# ① ENVI+展开+WRITE
conv(){ [ -f "$1" ] && { iconv -f UTF-16LE -t UTF-8 "$1" 2>/dev/null || cat "$1"; }; }
conv "$SMOKE/out/vars.txt" 2>/dev/null | grep -q "V_A=Hello_V_B=Wine"; chk "① ENVI变量+%展开%+WRITE" "$?"
# ② IFEX 文件存在判断
conv "$SMOKE/out/ifex.txt" 2>/dev/null | grep -q "PAYLOAD_SEEN"; chk "② IFEX \$文件存在判断" "$?"
# ③ HASH 引擎输出 32 位十六进制
conv "$SMOKE/out/hash.txt" 2>/dev/null | grep -qE "^[0-9A-Fa-f]{32}"; chk "③ HASH 命令(MD5 真体)" "$?"
# ④ REGI 注册表写入(HKCU)
v=$(wine reg query 'HKCU\Software\PECMD_Smoke' /v V1 2>/dev/null | grep -i "HelloRegi")
[ -n "$v" ]; chk "④ REGI 注册表写 HKCU" "$?"
# ⑤ 收尾标记(解析器跑到最后一行)
[ "$(cat "$SMOKE/out/done.txt" 2>/dev/null)" = "OK" ]; chk "⑤ 全程执行到末行(done=OK)" "$?"

echo "== [5/6] 结果 =="
for r in "${RESULTS[@]}"; do echo "  $r"; done
echo "== [6/6] 总计: PASS=$pass FAIL=$fail =="
[ $fail -eq 0 ] && echo "VERDICT: 冒烟通过——解析/变量/文件IO/条件/HASH/注册表链路可用" \
              || echo "VERDICT: 存在失败项——按上表定位(运行日志 $LOG_DIR/pecmd_run.log)"
exit $fail
