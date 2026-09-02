# -*- coding: utf-8 -*-
# R27: WRIT 崩溃复现第三轮 — 循环上下文/编码/特殊内容
import os
import subprocess

D = r'C:\pectest\writ_probe'
os.makedirs(D, exist_ok=True)

CASES = [
    ('x1_forx_loop',  'FORX W,a,b,WRIT %F%,%W%'),
    ('x2_team',       'TEAM WRIT %F%,l1|WRIT %F%,l2'),
    ('x3_empty_body', 'WRIT %F%,'),
    ('x4_neg_pos',    'WRIT %F%,abc,-1'),
    ('x5_long',       'WRIT %F%,' + 'A' * 4096),
    ('x6_percent',    'WRIT %F%,100%%done'),
    ('x7_eq_start',   'WRIT %F%,=x'),
    ('x8_starv_q',    'WRIT *v F,"q"'),
    ('x9_cjk',        'WRIT %F%,中文内容测试'),
    ('x10_uni',       'WRIT -UNI %F%,unicode text'),
    ('x11_qcomma',    'WRIT %F%,"a,b"'),
    ('x12_trailc',    'WRIT %F%,line1,'),
]

env = os.environ.copy()
for c in range(ord('A'), ord('Z') + 1):
    env.pop(chr(c), None)

for tag, stmt in CASES:
    p = os.path.join(D, tag + '.pecmd')
    lines = [
        'ENVI F=' + os.path.join(D, 'out.txt'),
        stmt,
        'ENVI DONE=yes',
    ]
    with open(p, 'w', newline='\r\n') as f:
        f.write('\n'.join(lines) + '\n')
    out = os.path.join(D, 'out.txt')
    if os.path.exists(out):
        os.remove(out)
    try:
        r = subprocess.run([r'C:\pectest\PECMD.EXE', 'LOAD', p],
                           cwd=r'C:\pectest', env=env,
                           capture_output=True, text=True, timeout=20)
        code = r.returncode & 0xffffffff
        crashed = (code == 0xC0000005)
        print('%s: exit=0x%08x %s | file=%s' % (
            tag, code, '<== CRASH' if crashed else '',
            'yes' if os.path.exists(out) else 'no'))
    except subprocess.TimeoutExpired:
        print('%s: TIMEOUT' % tag)
