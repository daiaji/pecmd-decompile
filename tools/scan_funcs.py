#!/usr/bin/env python3
"""scan_funcs.py — 扫描 decompiled.c，生成 TASKS.md 任务队列

函数分类:
  - thunk: 函数体为 jmp 跳板 (导出包装), 自动跳过并记录目标
  - named: 有符号名的方法/函数 (精修重构)
  - anon:  FUN_xxxxxxxx 匿名函数 (按引用频率排序, 批量重构)

用法: scan_funcs.py <decompiled.c> <组件名> [-o TASKS.md]
"""
import re
import sys
from collections import Counter


def main():
    src = sys.argv[1]
    comp = sys.argv[2]
    out = sys.argv[sys.argv.index('-o') + 1] if '-o' in sys.argv else None

    text = open(src, encoding='utf-8', errors='replace').read()
    funcs = re.findall(
        r'/\* ===== (.*?) @ ([0-9a-fx]+)(?: \(real @([0-9a-fx]+)\))? ===== \*/\n(.*?)(?=\n/\* ===== |\Z)',
        text, re.DOTALL)

    rows = []
    for name, addr, real, body in funcs:
        # thunk 检测: 函数体只有 jmp (5 字节) 或明显跳板
        sig = body.split('\n', 1)[0] if body else ''
        # thunk 判定: thunk_ 前缀或函数体极短(跳板)
        is_thunk = name.startswith('thunk_') or len(body.strip()) < 40
        kind = 'thunk' if is_thunk else ('named' if not re.match(r'FUN_[0-9a-f]+', name) else 'anon')
        rows.append({'name': name, 'addr': addr, 'real': real, 'kind': kind, 'body': body})

    # 匿名函数重要度: 统计被其他函数体引用的次数
    refs = Counter()
    for r in rows:
        if r['kind'] == 'anon':
            refs[r['name']] += 0
    for r in rows:
        for m in re.finditer(r'FUN_[0-9a-fA-F]{6,8}', r['body']):
            refs[m.group(0)] += 1

    lines = [f"# {comp} 任务队列 (生成: scan_funcs.py)", ""]
    lines.append(f"总数: {len(rows)}  thunk: {sum(1 for r in rows if r['kind']=='thunk')}  "
                 f"named: {sum(1 for r in rows if r['kind']=='named')}  "
                 f"anon: {sum(1 for r in rows if r['kind']=='anon')}")
    lines.append("")
    lines.append("## thunk (自动跳过, 记录目标)")
    for r in sorted((r for r in rows if r['kind'] == 'thunk'), key=lambda x: x['addr']):
        lines.append(f"- [ ] `{r['name']}` @{r['addr']} -> {r['real']}")
    lines.append("")
    lines.append("## named (精修重构)")
    for r in sorted((r for r in rows if r['kind'] == 'named'), key=lambda x: x['addr']):
        lines.append(f"- [ ] `{r['name']}` @{r['addr']} -> {r['real']}")
    lines.append("")
    lines.append("## anon (按引用次数排序)")
    for r in sorted((r for r in rows if r['kind'] == 'anon'), key=lambda x: -refs[x['name']]):
        lines.append(f"- [ ] `{r['name']}` @{r['addr']} refs={refs[r['name']]}")

    result = '\n'.join(lines) + '\n'
    if out:
        open(out, 'w', encoding='utf-8').write(result)
        print(f"written: {out} ({len(rows)} funcs)")
    else:
        print(result)


if __name__ == '__main__':
    main()
