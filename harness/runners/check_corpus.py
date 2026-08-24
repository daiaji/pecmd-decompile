#!/usr/bin/env python3
"""check_corpus.py — 语料包结构自检 (§4.C.1-0.2 manifest 契约)

校验每个用例: manifest.json 必填字段 / main.pecmd 存在 / vars 合法。
用法: python3 harness/runners/check_corpus.py [--strict]
"""
import json, os, sys, re

CASES = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "corpus", "cases")
REQUIRED = {"id", "cmds", "vars", "timeout_s", "flaky", "gui"}
ID_RE = re.compile(r"^\d{3}_[a-z0-9_]+$")

def _force_utf8_console():
    # WIN 端 GBK 控制台兼容: ✓ 等字符在 cp936 下不可编码
    for s in (sys.stdout, sys.stderr):
        reconf = getattr(s, "reconfigure", None)
        if reconf is not None:
            reconf(encoding="utf-8", errors="replace")

def main():
    _force_utf8_console()
    strict = "--strict" in sys.argv
    problems = []
    cases = sorted(os.listdir(CASES))
    for c in cases:
        d = os.path.join(CASES, c)
        if not os.path.isdir(d):
            continue
        mf = os.path.join(d, "manifest.json")
        if not os.path.exists(mf):
            problems.append(f"[{c}] manifest.json 缺失"); continue
        m = json.load(open(mf, encoding="utf-8"))
        for k in REQUIRED:
            if k not in m:
                problems.append(f"[{c}] manifest 缺字段 {k}")
        if not ID_RE.match(m.get("id", "")):
            problems.append(f"[{c}] id 非法: {m.get('id')!r} (须 3位序号_小写slug)")
        if m.get("id") != c:
            problems.append(f"[{c}] id 与目录名不一致")
        if not isinstance(m.get("vars", []), list) or not all(isinstance(v, str) and v for v in m.get("vars", [])):
            problems.append(f"[{c}] vars 须为非空字符串列表")
        if not isinstance(m.get("timeout_s", 0), int) or m.get("timeout_s", 0) <= 0:
            problems.append(f"[{c}] timeout_s 须为正整数")
        if not os.path.exists(os.path.join(d, "main.pecmd")):
            problems.append(f"[{c}] main.pecmd 缺失")
        else:
            s = open(os.path.join(d, "main.pecmd"), encoding="utf-8").read()
            if "\r" in s:
                problems.append(f"[{c}] main.pecmd 含 CRLF")
            # vars 引用自洽性: %VAR% 引用的变量应在 manifest vars 或 ENVI 已赋值
            for v in set(re.findall(r"%([A-Za-z_][A-Za-z0-9_]*)%", s)):
                if v not in m.get("vars", []) and not re.search(rf"ENVI\s+{v}\s*=", s):
                    if strict:
                        problems.append(f"[{c}] %{v}% 未在 vars 声明且未 ENVI 赋值")
    print(f"检查 {len(cases)} 个用例目录")
    if problems:
        print("发现问题:")
        for p in problems:
            print("  -", p)
        sys.exit(1)
    print("全部通过 ✓")

if __name__ == "__main__":
    main()
