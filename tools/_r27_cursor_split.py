# -*- coding: utf-8 -*-
# R27-c: i28g 解析区游标分离 — dc:90111-91660 区间的 cur -> sub (local_res10 语义)
# 规则: 行内含 dc:N 锚, N∈[90111,91660] 的行及其无锚延续行(区间内)做 \bcur\b -> sub
import io
import re

P = 'src/commands/core_b3r_i28g.c'
LO, HI = 90111, 91660
src = io.open(P, encoding='utf-8').read()
lines = src.split('\n')

# 1) 锚单调性检查
anchors = []
for i, ln in enumerate(lines):
    for m in re.finditer(r'dc:(\d+)', ln):
        anchors.append((i + 1, int(m.group(1))))
mono_ok = all(anchors[k][1] <= anchors[k + 1][1] for k in range(len(anchors) - 1))
print('anchors:', len(anchors), 'monotonic:', mono_ok)
if not mono_ok:
    bad = [(a, b) for a, b in zip(anchors, anchors[1:]) if a[1] > b[1]][:5]
    print('non-monotonic samples:', bad)

# 2) 区间替换 (锚驱动 + 无锚延续)
out = []
in_zone = False
changed = 0
touched = 0
for i, ln in enumerate(lines, 1):
    ms = [int(m.group(1)) for m in re.finditer(r'dc:(\d+)', ln)]
    if ms:
        last = ms[-1]
        if LO <= last <= HI:
            in_zone = True
        elif last > HI or last < 90018:
            in_zone = False
        # 锚 <LO 但 >90037 (初始化区) -> 不在区间
        elif last < LO:
            in_zone = False
    new, n = re.subn(r'\bcur\b', 'sub', ln)
    if in_zone and n:
        changed += n
        touched += 1
    out.append(new)

print('replaced:', changed, 'on', touched, 'lines')
io.open(P, 'w', encoding='utf-8', newline='\n').write('\n'.join(out))
