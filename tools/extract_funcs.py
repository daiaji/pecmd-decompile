#!/usr/bin/env python3
"""extract_funcs.py — 从 Ghidra decompiled.c 中按名称/前缀提取函数 (PECMD 版)

用法:
  extract_funcs.py <decompiled.c> <name|pattern>... [-o out.c]
  名称支持: 精确名 (mainW) / 前缀 (FUN_14001) / 正则
"""
import re
import sys
import os

# PECMD 块头格式: /* ========== FUN_140001000 @ 140001000  size=380 ========== */
BLOCK = re.compile(
    r'/\* =+ (\S+) @ ([0-9a-f]+)(?: \(real @([0-9a-f]+)\))?  size=\d+ =+ \*/\n'
    r'(.*?)(?=\n/\* =+ |\Z)',
    re.DOTALL,
)


def parse_functions(text):
    funcs = []
    for m in BLOCK.finditer(text):
        name, addr, real, body = m.group(1), m.group(2), m.group(3), m.group(4)
        sig_line = body.split('\n', 1)[0] if body else ''
        funcs.append({'name': name, 'addr': addr, 'real': real,
                      'sig': sig_line, 'body': body.strip('\n')})
    return funcs


def match(f, patterns):
    for p in patterns:
        if p in f['name']:
            return True
        try:
            if re.fullmatch(p, f['name']):
                return True
        except re.error:
            pass
    return False


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(1)
    src = sys.argv[1]
    patterns = [a for a in sys.argv[2:] if a != '-o']
    out = None
    if '-o' in sys.argv:
        i = sys.argv.index('-o')
        out = sys.argv[i + 1]

    text = open(src, encoding='utf-8', errors='replace').read()
    funcs = parse_functions(text)
    hits = [f for f in funcs if match(f, patterns)]
    if not hits:
        print('无匹配函数')
        sys.exit(1)

    buf = []
    for f in hits:
        buf.append(f'/* ========== {f["name"]} @ {f["addr"]} ========== */')
        buf.append(f['body'] + '\n')

    if out:
        with open(out, 'w', encoding='utf-8') as fp:
            fp.write('\n'.join(buf) + '\n')
        print(f'提取 {len(hits)} 个函数 -> {out}')
    else:
        print('\n'.join(buf))


if __name__ == '__main__':
    main()
