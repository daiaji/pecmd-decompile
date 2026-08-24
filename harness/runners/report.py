#!/usr/bin/env python3
"""report.py — Linux 端 verdict 汇总报告 (§4.C.1-0.9 一条命令出报告)

用法: python3 harness/runners/report.py [--backend win_real]
"""
import json
import os
import sys

HARNESS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RESULTS_ROOT = os.path.join(HARNESS, "results")


def force_utf8_console():
    # WIN 端 GBK 控制台兼容: 中文/符号输出在 cp936 下可能不可编码
    for s in (sys.stdout, sys.stderr):
        reconf = getattr(s, "reconfigure", None)
        if reconf is not None:
            reconf(encoding="utf-8", errors="replace")


def main():
    force_utf8_console()
    backend = "win_real"
    if "--backend" in sys.argv:
        backend = sys.argv[sys.argv.index("--backend") + 1]
    rdir = os.path.join(RESULTS_ROOT, backend)
    if not os.path.isdir(rdir):
        print(f"无结果目录: {rdir} (WIN 端回捞后生成)")
        sys.exit(0)
    cases = sorted(os.listdir(rdir))
    rows = []
    for c in cases:
        vf = os.path.join(rdir, c, "verdict.json")
        if not os.path.exists(vf):
            rows.append((c, "NO-VERDICT", ""))
            continue
        v = json.load(open(vf, encoding="utf-8"))
        detail = ""
        if v.get("vars", {}).get("diff"):
            detail = "; ".join(d[:60] for d in v["vars"]["diff"][:2])
        rows.append((c, v.get("verdict", "?"), detail))
    print(f"{'CASE':<24} {'VERDICT':<10} DETAIL")
    print("-" * 70)
    for c, v, d in rows:
        print(f"{c:<24} {v:<10} {d}")
    npass = sum(1 for _, v, _ in rows if v == "PASS")
    print(f"\n{len(rows)} 用例: PASS {npass} / FAIL {len(rows) - npass}")


if __name__ == "__main__":
    main()