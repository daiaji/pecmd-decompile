# -*- coding: utf-8 -*-
"""gen_fourcc.py — S14-B1: 从 PSB 动词级联提取 FourCC 常量生成 include/pecmd_fourcc.h
用法: python tools/gen_fourcc.py [--check]
数据源: restored_bodies.c 中 `local_158 == 0x........` 直比常量(s19 §2.2: 只收 == 直比)
对账: 可打印 4 字节动词值应 = s12 matrix 权威口径 117 个逻辑动词(±别名)
"""
import re, sys, io, os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # refactored/
RB = os.path.join(ROOT, 'restored_bodies.c')
OUT = os.path.join(ROOT, 'include', 'pecmd_fourcc.h')

def printable4(v):
    b = [(v >> (8 * i)) & 0xFF for i in range(4)]
    chars = []
    for byte in b:
        c = chr(byte) if 0x20 <= byte < 0x7f else None
        if c is None or not c.isalnum() and c not in '_-':
            return None
        chars.append(c)
    return ''.join(chars)

def main():
    src = open(RB, 'rb').read().decode('utf-8', 'replace')
    hits = sorted(set(int(m, 16) for m in
                  re.findall(r'local_158\s*==\s*0x([0-9A-Fa-f]{8})', src)))
    verbs = {}
    others = []
    for v in hits:
        name = printable4(v)
        if name is None:
            others.append(v)
            continue
        # 大小写归一键: PSB 级联用大写比较(dc:44689 同款), 小写变体视为同词别名
        key = name.upper()
        verbs.setdefault(key, v)
    print('total consts=%d printable-verb values=%d non-printable=%d'
          % (len(hits), len(verbs), len(others)))

    lines = []
    lines.append('/* pecmd_fourcc.h — S14-B1 自动生成 (tools/gen_fourcc.py), DO NOT EDIT BY HAND')
    lines.append(' * PSB 动词级联 FourCC 常量可读化 (dc:44xxx if-级联 ↔ rb local_158 直比)')
    lines.append(' * 口径: 只收 local_158 == 0xXXXXXXXX 直比; 大小写归一后 %d 互异动词值' % len(verbs))
    lines.append(' */')
    lines.append('#ifndef PECMD_FOURCC_H')
    lines.append('#define PECMD_FOURCC_H')
    lines.append('')
    lines.append('#include <stdint.h>')
    lines.append('')
    for key in sorted(verbs):
        v = verbs[key]
        lines.append('#define FOURCC_%-10s 0x%08Xu   /* "%s" */' % (key, v, key))
    lines.append('')
    lines.append('static inline void pecmd_fourcc_to_str(uint32_t v, wchar_t out[5])')
    lines.append('{')
    lines.append('    for (int i = 0; i < 4; i++) out[i] = (wchar_t)((v >> (8 * i)) & 0xFF);')
    lines.append('    out[4] = 0;')
    lines.append('}')
    lines.append('')
    lines.append('#endif /* PECMD_FOURCC_H */')

    if '--check' in sys.argv:
        print('check mode: no file written; verb count=%d (expect 117±alias)' % len(verbs))
        return

    with open(OUT, 'wb') as f:
        f.write('\r\n'.join(lines).encode('utf-8'))
    print('written:', OUT)

if __name__ == '__main__':
    main()
