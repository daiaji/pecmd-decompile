#!/usr/bin/env python3
"""阶段0（只读，一次遍历）：对 *_remaining.c 的 FUN_ 无参桩做被引用分析。"""
import os, re, json, glob, collections

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DECOMPILED = os.path.join(os.path.dirname(BASE), "decompiled.c")

def build_decompiled_index():
    """返回 {name: [call, addr]}。跳过定义行与 signature 行。"""
    counters = collections.Counter()
    try:
        with open(DECOMPILED, encoding="utf-8", errors="replace") as fh:
            lines = fh.read().splitlines()
    except OSError:
        return {}
    rcall = re.compile(r"FUN_([0-9a-f]+)\(")
    raddr = re.compile(r"&FUN_([0-9a-f]+)\b")
    for ln in lines:
        if "signature:" in ln:
            continue
        if re.match(r"^\s*[A-Za-z_][\w\s\*]*FUN_[0-9a-f]+\(", ln):
            continue  # 定义行
        for m in rcall.finditer(ln):
            counters[("call", m.group(1))] += 1
        for m in raddr.finditer(ln):
            counters[("addr", m.group(1))] += 1
    return counters

def build_refactored_index():
    """返回 {name: set(file)}：在哪些 core_*.c 出现过（含 &FUN_）。"""
    idx = collections.defaultdict(set)
    for core in glob.glob(os.path.join(BASE, "core_*.c")):
        fname = os.path.basename(core)
        with open(core, encoding="utf-8", errors="replace") as fh:
            text = fh.read()
        for m in re.finditer(r"FUN_[0-9a-f]+", text):
            idx[m.group(0)].add(fname)
    return idx

def stub_candidates():
    out = []
    for f in glob.glob(os.path.join(BASE, "core_*_remaining.c")):
        fname = os.path.basename(f)
        with open(f, encoding="utf-8", errors="replace") as fh:
            text = fh.read()
        for m in re.finditer(r"^uint64_t\s+(FUN_[0-9a-f]+)\(void\)\s*$", text, re.M):
            out.append((m.group(1), fname))
    return out

def crt_hint(name):
    a = name[4:].lower()
    if name.startswith("_"):
        return True
    return any(k in a for k in [
        "callnewh","woutput","input_l","localtime","gmtime","strerror","strlen",
        "wcslen","wcscmp","wcsncmp","strcmp","strchr","towlower","toupper",
        "cxxframe","cxxthrow","invalid_parameter","invoke_watson","initterm",
        "exit","ftol","free_osfhnd","getptd","read_nolock","write_nolock",
        "fdopen","malloc","calloc","realloc","free","memcpy","memset","memmove",
    ])

def main():
    dc = build_decompiled_index()
    rc = build_refactored_index()
    plan = []
    for name, fname in stub_candidates():
        call = dc.get(("call", name[4:]), 0)
        addr = dc.get(("addr", name[4:]), 0)
        others = rc.get(name, set()) - {fname} or (("&FUN_"+name[4:]) in collect_addrs(rc, name, fname))
        rc_called = bool(others)
        referenced = (call > 0) or (addr > 0) or rc_called
        plan.append({
            "name": name, "file": fname,
            "call_cnt": call, "addr_cnt": addr,
            "rc_called": rc_called, "crt_hint": crt_hint(name),
            "status": "need_restore" if referenced else "unref_verify",
        })
    plan.sort(key=lambda x: (x["status"] != "need_restore", x["file"], x["name"]))
    with open(os.path.join(BASE, "tools", "remaining_plan.json"), "w", encoding="utf-8") as fh:
        json.dump(plan, fh, ensure_ascii=False, indent=2)
    c = collections.Counter((x["status"], x["file"]) for x in plan)
    print("total_stubs=%d" % len(plan))
    for (st, f), n in sorted(c.items()):
        print("  %-14s %-32s %d" % (st, f, n))
    need = [x for x in plan if x["status"] == "need_restore"]
    print("need_restore_total=%d (其中 crt_hint=%d)" % (
        len(need), sum(1 for x in need if x["crt_hint"])))
    unref = [x for x in plan if x["status"] == "unref_verify"]
    print("unref_verify_total=%d" % len(unref))

def collect_addrs(rc, name, fname):
    return set()

if __name__ == "__main__":
    main()
