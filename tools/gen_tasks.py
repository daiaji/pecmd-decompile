#!/usr/bin/env python3
"""gen_tasks.py — PECMD 防重复重构进度账本

扫描 refactored/ 下已重构 .c 文件头注释中的地址 (@0x140xxxxxx),
与 decompiled.c 全部函数对比, 生成剩余任务清单。

用法:
  gen_tasks.py                 # 汇总进度
  gen_tasks.py --list [过滤]   # 列出剩余函数 (按分类)
  gen_tasks.py --list-biz N    # 列出前 N 个 BIZ 函数
"""
import re
import os
import sys
import json

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # refactored/
ROOT = os.path.dirname(BASE)                                         # repo root (pecmd-decompile)
SRC = os.path.join(ROOT, 'decompiled.c')

DONE_CACHE = None
# B5 工具债固化 (§8.3 检查单第7项): 易失 /tmp 路径改为仓库内固化 JSON
CLASSES = json.load(open(os.path.join(BASE, 'tools', 'pecmd_func_classes.json'))) \
    if os.path.exists(os.path.join(BASE, 'tools', 'pecmd_func_classes.json')) else {}

KW = {'if', 'for', 'while', 'switch', 'return', 'sizeof', 'else', 'do',
      'case', 'default', 'typedef', 'struct', 'enum', 'static_assert'}


def done_functions():
    global DONE_CACHE
    if DONE_CACHE is not None:
        return DONE_CACHE
    done = {}
    for root, _dirs, files in os.walk(BASE):
        for path in files:
            if not path.endswith('.c'):
                continue
            p = os.path.join(root, path)
            if not os.path.isfile(p):
                continue
        txt = open(p, encoding='utf-8', errors='replace').read()
        # 跳过 TODO/依赖标注行, 防止把待重构地址当已完成
        for line in txt.split('\n'):
            if 'TODO' in line or '依赖' in line or '待重构' in line or '后续批次' in line:
                continue
            for m in re.finditer(r'(?:@|真实体\s*@)0x(140[0-9a-f]{6})', line):
                done.setdefault(m.group(1).lower(), []).append(path)
        # 函数名兜底 (PECMD_ 前缀)
        for m in re.finditer(r'\bPECMD_[A-Za-z0-9_]+\s*\(', txt):
            done.setdefault('name:' + m.group(0).split('(')[0].strip(), []).append(path)
    DONE_CACHE = done
    return done


def all_functions():
    """从 decompiled.c 解析全部函数 (带分类)"""
    pat = re.compile(r'/\* =+ (\S+) @ ([0-9a-f]+)  size=(\d+) =+ \*/\n'
                     r'(.*?)(?=\n/\* =+ |\Z)', re.S)
    src = open(SRC, encoding='utf-8', errors='replace').read()
    funcs = []
    for m in pat.finditer(src):
        name, addr, size = m.group(1), m.group(2), int(m.group(3))
        cls = 'UNCLASS'
        for c, fs in CLASSES.items():
            if any(f['addr'] == addr for f in fs):
                cls = c
                break
        funcs.append({'name': name, 'addr': addr, 'size': size, 'class': cls})
    return funcs


def main():
    done = done_functions()
    funcs = all_functions()
    remaining = [f for f in funcs if f['addr'].lower() not in done]

    if '--list' in sys.argv:
        kw = sys.argv[sys.argv.index('--list') + 1] if len(sys.argv) > sys.argv.index('--list') + 1 else ''
        hits = [f for f in remaining if not kw or kw.lower() in f['name'].lower()]
        for f in hits:
            print(f"{f['addr']} {f['name']:30s} size={f['size']:5d} {f['class']}")
        return
    if '--list-biz' in sys.argv:
        n = int(sys.argv[sys.argv.index('--list-biz') + 1])
        biz = sorted([f for f in remaining if f['class'] == 'BIZ'], key=lambda x: -x['size'])[:n]
        for f in biz:
            print(f"{f['addr']} {f['name']:30s} size={f['size']:5d}")
        return

    by_cls = {}
    for f in remaining:
        by_cls.setdefault(f['class'], 0)
        by_cls[f['class']] += 1
    print(f"已重构: {len(done)} / 总: {len(funcs)} / 剩余: {len(remaining)}")
    for c in sorted(by_cls):
        print(f"  {c:12s} {by_cls[c]}")


if __name__ == '__main__':
    main()
