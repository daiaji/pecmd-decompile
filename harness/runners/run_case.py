#!/usr/bin/env python3
"""run_case.py — WIN 端真机用例执行器 (纯 WIN 化, 替代 run_case.sh, ROADMAP §5 改动点 2)

三步协议: 部署 → 生成 epilogue → 运行 → 回捞 (尾声产物校验)
双跑:
    原版 EXE → results/win_real_orig/<case>/   (真值参考; golden 录制唯一来源)
    编译产物 → results/win_real/<case>/        (被测方; = diff_case.py 默认后端)

用法 (Windows 原生 Python ≥3.8):
    python harness/runners/run_case.py 001_envi_smoke                  # 双跑
    python harness/runners/run_case.py 001_envi_smoke --exe orig       # 仅原版
    python harness/runners/run_case.py 001_envi_smoke --exe msvc       # 仅编译产物
    python harness/runners/run_case.py --all                           # 全语料逐条双跑
    python harness/runners/run_case.py 001_envi_smoke --exe orig --record-golden

回捞后判读:
    python harness/runners/diff_case.py 001_envi_smoke   # golden/win_real vs results/win_real
"""
import argparse
import json
import os
import shutil
import subprocess
import sys

HARNESS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CASES_ROOT = os.path.join(HARNESS, "corpus", "cases")
RESULTS_ROOT = os.path.join(HARNESS, "results")
GOLDEN_ROOT = os.path.join(HARNESS, "golden")

BACKEND_MSVC = "win_real"        # 被测方后端名 (= diff_case.py 默认后端)
BACKEND_ORIG = "win_real_orig"   # 原版 EXE 后端名 (真值来源)

ARTIFACTS = ("vars.txt", "done.txt", "vars_val.txt")  # 尾声产物 (epilogue 写出到 <pectest_root>\out\); vars_val.txt = R24f-c 变量值回捞 (U-2), 仅 manifest.vars 非空时存在


def fail(msg):
    print(f"FAIL: {msg}", file=sys.stderr)
    sys.exit(1)


def force_utf8_console():
    # WIN 端 GBK 控制台兼容: 中文/符号输出在 cp936 下可能不可编码
    for s in (sys.stdout, sys.stderr):
        reconf = getattr(s, "reconfigure", None)
        if reconf is not None:
            reconf(encoding="utf-8", errors="replace")


def load_manifest(case_id):
    mf = os.path.join(CASES_ROOT, case_id, "manifest.json")
    if not os.path.isfile(mf):
        fail(f"manifest 缺失 {mf}")
    with open(mf, encoding="utf-8") as f:
        return json.load(f)


def list_cases():
    return sorted(d for d in os.listdir(CASES_ROOT)
                  if os.path.isdir(os.path.join(CASES_ROOT, d)))


def deploy(case_id, case_src, pectest_root):
    """[1/4] 部署: 清理目标目录并复制 main.pecmd → C:\\pectest\\<case>\\"""
    dst = os.path.join(pectest_root, case_id)
    if os.path.isdir(dst):
        shutil.rmtree(dst)
    os.makedirs(dst)
    shutil.copy2(os.path.join(case_src, "main.pecmd"),
                 os.path.join(dst, "main.pecmd"))
    return dst


def _cmd_meta_escape(s):
    """cmd.exe 元字符转义 (^X)。用于 echo 内容安全通过 /c 传递。
    注意: % 不转义 — cmd /c(非 batch)下单个 % 原样输出, 且 PECMD 已先做变量展开。"""
    for ch in ("^", "&", "|", "<", ">"):
        s = s.replace(ch, "^" + ch)
    return s


def make_epilogue(dst, case_id, manifest, pectest_root):
    """[2/4] 尾声: 按 manifest.vars 回捞产物。

    T2 改造 (REVIEW §131): WRITE 在还原构建中尚不可靠, 统一改走
    EXEC =cmd.exe /c echo ...>"file" 通道 —— 原版实测三种变体全通过
    (%VAR% 展开/^| 转义/CRLF), msvc 构建经 S7 分发链接通后同样可用。
    双方用同一通道 ⇒ golden 与 results 字节格式一致。
    """
    out_dir = os.path.join(pectest_root, "out")
    os.makedirs(out_dir, exist_ok=True)
    cmd_exe = os.path.join(os.environ.get("SystemRoot", r"C:\Windows"),
                           "System32", "cmd.exe")
    vars_bar = "|".join(manifest.get("vars", []))
    vars_content = _cmd_meta_escape(f"CASE={case_id}|{vars_bar}")
    vars_path = os.path.join(out_dir, "vars.txt")
    done_path = os.path.join(out_dir, "done.txt")
    lines = [
        f"ENVI T_CASE={case_id}",
        f'EXEC ={cmd_exe} /c echo {vars_content}>"{vars_path}"',
        "ENVI T_DONE=OK",
        f'EXEC ={cmd_exe} /c echo %T_DONE%>"{done_path}"',
    ]
    # R24f-c (U-2 夹具升级第一步): 新增 vars_val.txt —— 变量值回捞。
    # 默认关闭: manifest {"vars_val": true} 才追加第三条 EXEC (实测批量 63 案下
    # 多一条 EXEC 会放大既有的 EXEC 通道偶发哑火 → 语料回归门不被新工件扰动)。
    vars_list = manifest.get("vars", [])
    if vars_list and manifest.get("vars_val"):
        vals_content = "|".join(f"{v}=%{v}%" for v in vars_list)
        vals_content = _cmd_meta_escape(f"CASE={case_id}|{vals_content}")
        vals_path = os.path.join(out_dir, "vars_val.txt")
        lines.append(f'EXEC ={cmd_exe} /c echo {vals_content}>"{vals_path}"')
    epilogue = "\n".join(lines) + "\n"

    main_path = os.path.join(dst, "main.pecmd")
    with open(main_path, "rb") as f:
        main_bytes = f.read()
    sep = b"" if main_bytes.endswith(b"\n") else b"\n"  # 防止 main 缺尾换行导致首尾命令粘连
    with open(os.path.join(dst, "run_all.pecmd"), "wb") as f:
        f.write(main_bytes + sep + epilogue.encode("utf-8"))
    return out_dir


def run_exe(label, exe_path, backend, case_id, dst, out_dir, timeout_s,
            pectest_root, notes):
    """[3/4] 运行 + [4/4] 回捞: cwd=pectest_root, LOAD 用例绝对路径脚本"""
    result_dir = os.path.join(RESULTS_ROOT, backend, case_id)
    os.makedirs(result_dir, exist_ok=True)

    # 清空共享 out 目录, 防止上一用例残留产物污染本次回捞
    if os.path.isdir(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)

    exit_code = None
    stdout_bytes = b""
    try:
        proc = subprocess.run(
            [exe_path, "LOAD", os.path.join(dst, "run_all.pecmd")],
            cwd=pectest_root,
            timeout=timeout_s,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
        exit_code = proc.returncode
        stdout_bytes = proc.stdout
    except FileNotFoundError:
        fail(f"[{label}] EXE 不存在: {exe_path}")
    except subprocess.TimeoutExpired as e:
        exit_code = 124  # 沿用 coreutils timeout 惯例值
        stdout_bytes = e.stdout

    with open(os.path.join(result_dir, "stdout.txt"), "wb") as f:
        f.write(stdout_bytes or b"")
    # exit.txt 保持纯 "exit=N" 格式: diff_case.py 按 int 解析, 警告不得混入
    with open(os.path.join(result_dir, "exit.txt"), "w", encoding="utf-8",
              newline="\n") as f:
        f.write(f"exit={exit_code}\n")

    ok = True
    for name in ARTIFACTS:
        src = os.path.join(out_dir, name)
        if os.path.isfile(src):
            shutil.copy2(src, os.path.join(result_dir, name))
        else:
            msg = f"[{label}] 产物缺失 {name} (用例可能失败)"
            print(f"WARN: {msg}", file=sys.stderr)
            notes.append(msg)

    if notes:  # 证据留档 (不进 exit.txt, 避免 verdict 解析被污染)
        with open(os.path.join(result_dir, "run_notes.txt"), "a",
                  encoding="utf-8", newline="\n") as f:
            f.write("\n".join(notes) + "\n")

    tag = "完成" if ok else "完成(产物不全)"
    print(f"[{label}] {tag} → {result_dir} (exit={exit_code})")


def run_case(args, case_id):
    case_src = os.path.join(CASES_ROOT, case_id)
    manifest = load_manifest(case_id)
    timeout_s = args.timeout or manifest.get("timeout_s", 60)

    if manifest.get("gui"):
        print(f"WARN: [{case_id}] gui=true, 需人工在场监督", file=sys.stderr)

    targets = []  # (label, exe, backend)
    if args.exe in ("both", "orig"):
        targets.append(("orig", args.orig_exe, BACKEND_ORIG))
    if args.exe in ("both", "msvc"):
        targets.append(("msvc", args.msvc_exe, BACKEND_MSVC))

    # 显式 --exe msvc 时缺产物视为硬错误; both 下允许跳过 (P0 产出前为常态)
    for label, exe, _ in targets:
        if label == "msvc" and args.exe == "msvc" and not os.path.isfile(exe):
            fail(f"编译产物不存在: {exe} (先运行 bash tools/build_msvc.sh)")
    if all(not os.path.isfile(exe) for _, exe, _ in targets):
        fail(f"无可用 EXE: {args.orig_exe} / {args.msvc_exe}")

    notes_by_label = {label: [] for label, _, _ in targets}
    for label, exe, backend in targets:
        if not os.path.isfile(exe):
            print(f"WARN: [{label}] 跳过 (产物不存在): {exe}", file=sys.stderr)
            continue
        dst = deploy(case_id, case_src, args.pectest_root)
        out_dir = make_epilogue(dst, case_id, manifest, args.pectest_root)
        run_exe(label, exe, backend, case_id, dst, out_dir, timeout_s,
                args.pectest_root, notes_by_label[label])


def record_golden(case_id):
    """golden 录制: 原版 EXE 结果 (win_real_orig) 晋升为 golden/win_real/<case>/"""
    src = os.path.join(RESULTS_ROOT, BACKEND_ORIG, case_id)
    if not os.path.isdir(src):
        fail(f"无原版结果可录制: {src} (先 --exe orig 跑一遍)")
    dst = os.path.join(GOLDEN_ROOT, "win_real", case_id)
    os.makedirs(dst, exist_ok=True)
    copied = []
    for name in ARTIFACTS + ("stdout.txt", "exit.txt"):
        p = os.path.join(src, name)
        if os.path.isfile(p):
            shutil.copy2(p, os.path.join(dst, name))
            copied.append(name)
    print(f"GOLDEN: {len(copied)} 个产物 → {dst}")


def main():
    force_utf8_console()
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("case_id", nargs="?", default="",
                    help="用例 ID, 如 001_envi_smoke (--all 时可省略)")
    ap.add_argument("--all", action="store_true", help="全语料逐条执行")
    ap.add_argument("--exe", choices=["both", "orig", "msvc"], default="both",
                    help="驱动哪些 EXE (默认 both 双跑)")
    ap.add_argument("--timeout", type=int, default=None,
                    help="覆盖 manifest.timeout_s")
    ap.add_argument("--record-golden", action="store_true",
                    help="把原版 EXE 结果晋升为 golden/win_real/<case>/ (单一裁判纪律)")
    ap.add_argument("--pectest-root",
                    default=os.environ.get("PECTEST_ROOT", r"C:\pectest"))
    ap.add_argument("--orig-exe", default=os.environ.get("PECMD_EXE", ""),
                    help="原版 EXE 路径 (默认 <pectest-root>\\PECMD.EXE)")
    ap.add_argument("--msvc-exe", default="",
                    help="编译产物路径 (默认 <pectest-root>\\pecmd_msvc.exe)")
    args = ap.parse_args()

    if os.name != "nt":
        fail("须以 Windows 原生 Python 运行 (纯 WIN 化裁定, PRODUCTION_ROADMAP §4.1)")

    if not args.orig_exe:
        args.orig_exe = os.path.join(args.pectest_root, "PECMD.EXE")
    if not args.msvc_exe:
        args.msvc_exe = os.path.join(args.pectest_root, "pecmd_msvc.exe")

    cases = list_cases() if args.all else [args.case_id]
    if not cases or not cases[0]:
        ap.error("缺少 case_id (或使用 --all)")

    for c in cases:
        print(f"=== {c} ===")
        run_case(args, c)

    if args.record_golden:
        for c in cases:
            record_golden(c)


if __name__ == "__main__":
    main()
