#!/usr/bin/env python3
"""diff_case.py — Linux 端 verdict 计算器 (§4.C.1-0.5 契约)

比较 golden/<backend>/<case>/ 与 results/<backend>/<case>/ 的产物,
输出 verdict.json。掩码规则集中在 masks.conf (时间戳/路径/随机值正则)。

用法:
  python3 harness/runners/diff_case.py <case_id> [--backend win_real]
  python3 harness/runners/diff_case.py --all          # 全部用例
"""
import json
import os
import re
import sys

HARNESS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GOLDEN_ROOT = os.path.join(HARNESS, "golden")
RESULTS_ROOT = os.path.join(HARNESS, "results")
MASKS = os.path.join(HARNESS, "runners", "masks.conf")


def load_masks():
    pats = []
    if os.path.exists(MASKS):
        for line in open(MASKS, encoding="utf-8"):
            line = line.strip()
            if line and not line.startswith("#"):
                try:
                    pats.append(re.compile(line))
                except re.error as e:
                    print(f"WARN: 掩码正则无效 {line!r}: {e}")
    return pats


def normalize(text, masks):
    for p in masks:
        text = p.sub("<MASKED>", text)
    return text


def diff_files(golden, result, masks):
    """返回 (same, detail)。行尾归一 (T2): EXEC echo 通道产出 CRLF,
    与历史 LF 产物混比时统一为 LF 再比较。"""
    g = open(golden, encoding="utf-8", errors="replace").read().replace("\r\n", "\n")
    r = open(result, encoding="utf-8", errors="replace").read().replace("\r\n", "\n")
    if normalize(g, masks) == normalize(r, masks):
        return True, []
    # 逐行 diff (前 10 行)
    gl = normalize(g, masks).splitlines()
    rl = normalize(r, masks).splitlines()
    diff = [f"  g: {a}\n  r: {b}" for a, b in zip(gl, rl) if a != b][:10]
    if len(gl) != len(rl):
        diff.append(f"  (行数 g={len(gl)} r={len(rl)})")
    return False, diff


def verdict_for(case_id, backend="win_real"):
    gdir = os.path.join(GOLDEN_ROOT, backend, case_id)
    rdir = os.path.join(RESULTS_ROOT, backend, case_id)
    manifest = os.path.join(HARNESS, "corpus", "cases", case_id, "manifest.json")
    cmds = []
    vars_ = []
    if os.path.exists(manifest):
        m = json.load(open(manifest, encoding="utf-8"))
        cmds = m.get("cmds", [])
        vars_ = m.get("vars", [])
    masks = load_masks()

    # stdout / exit
    verdict = {"case": case_id, "backend": backend, "stdout": {"same": True},
               "exit": {"exp": None, "got": None}, "vars": {"same": True, "diff": []},
               "fs": {"same": True, "added": [], "removed": [], "changed": []},
               "reg": {"same": True}, "verdict": "PASS", "flaky_pool": False}

    # exit
    if os.path.exists(os.path.join(gdir, "exit.txt")) and os.path.exists(os.path.join(rdir, "exit.txt")):
        try:
            ge = int(open(os.path.join(gdir, "exit.txt")).read().split("=")[1].strip())
            re_ = int(open(os.path.join(rdir, "exit.txt")).read().split("=")[1].strip())
        except (IndexError, ValueError):
            ge = re_ = None
        verdict["exit"] = {"exp": ge, "got": re_}
        if ge != re_:
            verdict["verdict"] = "FAIL"

    # stdout (若双方都有)
    gs, rs = os.path.join(gdir, "stdout.txt"), os.path.join(rdir, "stdout.txt")
    if os.path.exists(gs) and os.path.exists(rs):
        same, diff = diff_files(gs, rs, masks)
        verdict["stdout"] = {"same": same}
        if not same:
            verdict["verdict"] = "FAIL"

    # vars.txt (变量表核心证据)
    gv, rv = os.path.join(gdir, "vars.txt"), os.path.join(rdir, "vars.txt")
    if os.path.exists(gv) and os.path.exists(rv):
        same, diff = diff_files(gv, rv, masks)
        verdict["vars"] = {"same": same, "diff": diff[:5]}
        if not same:
            verdict["verdict"] = "FAIL"

    # vars_val.txt (R24f-c U-2 值级回捞; R27 起纳入判定 — 双方存在时值必须一致,
    # 封堵"变量存在但值不同"的假绿盲区, FORX-L/NL 语料 067/068 即此类)
    gvv, rvv = (os.path.join(gdir, "vars_val.txt"),
                os.path.join(rdir, "vars_val.txt"))
    if os.path.exists(gvv) and os.path.exists(rvv):
        same, diff = diff_files(gvv, rvv, masks)
        verdict["vars_val"] = {"same": same, "diff": diff[:5]}
        if not same:
            verdict["verdict"] = "FAIL"

    # done.txt 存在性 (运行完成信号)
    gd, rd = os.path.join(gdir, "done.txt"), os.path.join(rdir, "done.txt")
    if os.path.exists(gd) and not os.path.exists(rd):
        verdict["verdict"] = "FAIL"
        verdict["fs"]["removed"].append("done.txt")

    # golden 完整性: 关键产物缺失 = NO-GOLDEN (首次录制前不得判 PASS)
    if not os.path.isdir(gdir) or not os.path.exists(os.path.join(gdir, "vars.txt")):
        verdict["verdict"] = "NO-GOLDEN"
        verdict["_note"] = "金标准缺失: 需先以原版 EXE 真机录制 (§6.7 单一裁判)"

    return verdict


def save_verdict(v, backend):
    """verdict.json 落盘到 results/<backend>/<case>/ (report.py 的数据源)"""
    rdir = os.path.join(RESULTS_ROOT, backend, v["case"])
    if not os.path.isdir(rdir):
        return
    with open(os.path.join(rdir, "verdict.json"), "w", encoding="utf-8",
              newline="\n") as f:
        json.dump(v, f, indent=1, ensure_ascii=False)


def force_utf8_console():
    # WIN 端 GBK 控制台兼容: 中文/符号输出在 cp936 下可能不可编码
    for s in (sys.stdout, sys.stderr):
        reconf = getattr(s, "reconfigure", None)
        if reconf is not None:
            reconf(encoding="utf-8", errors="replace")


def main():
    force_utf8_console()
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    backend = "win_real"
    if "--backend" in sys.argv:
        backend = sys.argv[sys.argv.index("--backend") + 1]

    if "--all" in sys.argv:
        cases = sorted(os.listdir(os.path.join(HARNESS, "corpus", "cases")))
        out = {"cases": [], "summary": {"total": 0, "pass": 0, "fail": 0}}
        for c in cases:
            v = verdict_for(c, backend)
            save_verdict(v, backend)
            out["cases"].append(v)
            out["summary"]["total"] += 1
            out["summary"]["pass" if v["verdict"] == "PASS" else "fail"] += 1
        print(json.dumps(out["summary"], indent=1, ensure_ascii=False))
        return

    if not args:
        print(__doc__)
        sys.exit(1)
    v = verdict_for(args[0], backend)
    save_verdict(v, backend)
    print(json.dumps(v, indent=1, ensure_ascii=False))
    sys.exit(0 if v["verdict"] == "PASS" else 2)


if __name__ == "__main__":
    main()