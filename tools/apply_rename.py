#!/usr/bin/env python3
"""apply_rename.py — 将 {FUN_<addr>: PECMD_<name>} 批处理改名应用到 core_*.c。

- 词边界替换 FUN_<addr> -> PECMD_<name>（覆盖定义/extern/调用点/取址/头注释标签）。
- 保留 @0x<addr> 头注释地址。
- 同步追加 rename_map.json 与 FUNC_NAMES.md。
用法: python3 tools/apply_rename.py '<json-mapping>' [--dry]
"""
import os, re, glob, json, sys, collections

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MAP_F = os.path.join(BASE, "tools", "rename_map.json")
NAMES_F = os.path.join(BASE, "FUNC_NAMES.md")

def main():
    dry = "--dry" in sys.argv
    mapping = json.loads(sys.argv[1])
    # validate names
    for addr, name in mapping.items():
        assert re.fullmatch(r"FUN_140[0-9a-f]{6}", addr), addr
        assert re.fullmatch(r"PECMD_[A-Za-z][A-Za-z0-9_]*", name), name
    rename_map = json.load(open(MAP_F, encoding="utf-8"))
    # collision check against existing
    existing = set(rename_map.values())
    for name in mapping.values():
        if name in existing:
            print("COLLISION name already used:", name); sys.exit(2)
    total = 0
    for f in sorted(glob.glob(os.path.join(BASE, "core_*.c"))):
        txt = open(f, encoding="utf-8", errors="replace").read()
        new = txt
        for addr, name in mapping.items():
            pat = re.compile(r"\b" + re.escape(addr) + r"\b")
            n = len(pat.findall(new))
            if n:
                new = pat.sub(name, new)
                total += n
        if new != txt and not dry:
            open(f, "w", encoding="utf-8").write(new)
            print("W  %-40s %6d 替换" % (os.path.basename(f), sum(len(re.findall(r"\b"+re.escape(a)+r"\b", txt)) for a in mapping)))
    print("total replacements:", total)
    if dry:
        return
    # update rename_map.json
    for addr, name in mapping.items():
        rename_map[addr] = name
    json.dump(rename_map, open(MAP_F, "w", encoding="utf-8"), ensure_ascii=False, indent=2)
    # update FUNC_NAMES.md
    with open(NAMES_F, encoding="utf-8") as fh:
        ntxt = fh.read()
    rows = []
    for addr, name in mapping.items():
        if addr not in ntxt:
            rows.append("| 0x%s | %s | %s |" % (addr[4:].lower(), addr, name))
    if rows:
        # insert before the first blank after the table header (or append at end)
        marker = "|---|\n"
        idx = ntxt.find(marker)
        if idx >= 0:
            ins = idx + len(marker)
            ntxt = ntxt[:ins] + "\n".join(rows) + "\n" + ntxt[ins:]
        else:
            ntxt = ntxt.rstrip("\n") + "\n" + "\n".join(rows) + "\n"
        open(NAMES_F, "w", encoding="utf-8").write(ntxt)
        print("FUNC_NAMES.md updated +%d rows" % len(rows))
    print("rename_map.json updated -> total", len(rename_map))

if __name__ == "__main__":
    main()
