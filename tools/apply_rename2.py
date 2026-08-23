#!/usr/bin/env python3
"""apply_rename2.py — 增强版批量改名 (apply_rename.py 的超集)

修复/增强:
1. 大小写变体: 库内存在 FUN_140027EAC 类大写十六进制拼写; 对每个地址自动探测
   core(+link_stubs) 中全部实际拼写并逐一替换。
2. PECMD_ 占用检查: 除 rename_map 值外, 还对 include/*.h + core_*.c + link_stubs.c 中
   已存在的 \\bPECMD_\\w+ 建占用集, 撞名即拒绝该条(不中断整批)。
3. 可选 --sync-ls: 同时改写 link_stubs.c (仅当无人独占 link_stubs.c 时可用!)。
4. 输入支持 proposals JSON: {"proposals":[{"addr":..,"old":..,"name":..}, ...]}
   或旧式 {"FUN_140xxxxxx":"PECMD_X", ...}。

用法:
  python3 tools/apply_rename2.py tools/name_proposals.json [--dry] [--sync-ls] [--min-conf high]
  python3 tools/apply_rename2.py '{"FUN_14005c828":"PECMD_ResolveApi"}' [--dry]
"""
import os, re, glob, json, sys

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MAP_F = os.path.join(BASE, "tools", "rename_map.json")
NAMES_F = os.path.join(BASE, "FUNC_NAMES.md")

def norm_addr_key(k):
    """接受 FUN_<9hex任意大小写> / <9hex> / 9hex"""
    m = re.fullmatch(r"(?:FUN_)?([0-9a-fA-F]{9})", k.strip())
    if not m:
        raise ValueError("bad addr key: %r" % k)
    return m.group(1).lower()

def load_mapping(arg):
    if arg.endswith(".json") and os.path.exists(arg):
        d = json.load(open(arg, encoding="utf-8"))
        items = d.get("proposals", d) if isinstance(d, dict) else d
        out = {}
        if isinstance(items, dict):
            for k, v in items.items():
                out[norm_addr_key(k)] = v
        else:
            for it in items:
                conf = it.get("confidence", "high")
                out.setdefault(norm_addr_key(it.get("old") or it["addr"]), {
                    "name": it["name"], "confidence": conf})
        return {k: (v if isinstance(v, str) else v["name"]) for k, v in out.items()}, \
               {k: (v if isinstance(v, str) else v.get("confidence", "high")) for k, v in out.items()}
    d = json.loads(arg)
    return {norm_addr_key(k): v for k, v in d.items()}, {norm_addr_key(k): "high" for k in d}

def main():
    args = sys.argv[1:]
    dry = "--dry" in args
    sync_ls = "--sync-ls" in args
    min_conf = None
    if "--min-conf" in args:
        min_conf = args[args.index("--min-conf") + 1]
        args.remove("--min-conf"); args.remove(min_conf)
    mapping, confs = load_mapping(args[0])

    targets = sorted(glob.glob(os.path.join(BASE, "core_*.c")))
    if sync_ls:
        targets.append(os.path.join(BASE, "link_stubs.c"))
    texts = {f: open(f, encoding="utf-8", errors="replace").read() for f in targets}
    allsrc = "".join(texts.values())
    for f in glob.glob(os.path.join(BASE, "include", "*.h")):
        allsrc += open(f, encoding="utf-8", errors="replace").read()

    rename_map = json.load(open(MAP_F, encoding="utf-8"))

    # 占用集: rename_map 值 + 代码中已存在的 PECMD_* 符号
    occupied = set(rename_map.values())
    occupied |= set(re.findall(r"\bPECMD_[A-Za-z][A-Za-z0-9_]*", allsrc))

    # 地址 -> 实际拼写变体探测 (在目标文件中)
    def variants(addr):
        vs = set()
        pat = re.compile(r"\bFUN_" + addr + r"\b", re.I)
        for txt in texts.values():
            for m in pat.finditer(txt):
                vs.add(m.group(0))
        return vs

    applied, rejected, total = {}, {}, 0
    for addr, name in mapping.items():
        c = confs.get(addr, "high")
        order = {"high": 0, "medium": 1, "low": 2}
        if min_conf and order.get(c, 3) > order[min_conf]:
            rejected[addr] = "confidence %s < %s" % (c, min_conf); continue
        if name in occupied:
            rejected[addr] = "name occupied: " + name; continue
        if not re.fullmatch(r"PECMD_[A-Za-z][A-Za-z0-9_]*", name):
            rejected[addr] = "bad name format"; continue
        vs = variants(addr)
        if not vs:
            rejected[addr] = "no occurrences in targets"; continue
        occupied.add(name)
        applied[addr] = (name, vs)

    # 执行替换
    for f, txt in texts.items():
        new = txt
        n_here = 0
        for addr, (name, vs) in applied.items():
            for v in vs:
                pat = re.compile(r"\b" + re.escape(v) + r"\b")
                n_here += len(pat.findall(new))
                new = pat.sub(name, new)
        if new != txt and not dry:
            open(f, "w", encoding="utf-8").write(new)
        print("W  %-24s %6d 替换" % (os.path.relpath(f, BASE), n_here))
        total += n_here

    print("applied:", len(applied), " rejected:", len(rejected), " total replacements:", total)
    for a, r in list(rejected.items())[:40]:
        print("  REJ", a, r)
    if dry:
        print("(dry run — 未写盘)")
        return

    # rename_map.json: 统一存小写 FUN_ 键
    for addr in applied:
        rename_map["FUN_" + addr] = mapping[addr]
    json.dump(rename_map, open(MAP_F, "w", encoding="utf-8"), ensure_ascii=False, indent=2)

    # FUNC_NAMES.md 追加
    with open(NAMES_F, encoding="utf-8") as fh:
        ntxt = fh.read()
    rows = ["| 0x%s | FUN_%s | %s |" % (a, a, mapping[a]) for a in applied if ("FUN_" + a) not in ntxt and a not in ntxt]
    if rows:
        marker = "|---|\n"
        idx = ntxt.find(marker)
        if idx >= 0:
            ins = idx + len(marker)
            ntxt = ntxt[:ins] + "\n".join(rows) + "\n" + ntxt[ins:]
        else:
            ntxt = ntxt.rstrip("\n") + "\n" + "\n".join(rows) + "\n"
        open(NAMES_F, "w", encoding="utf-8").write(ntxt)
        print("FUNC_NAMES.md updated +%d rows" % len(rows))
    print("rename_map.json -> total", len(rename_map))

if __name__ == "__main__":
    main()
