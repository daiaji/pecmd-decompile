# -*- coding: utf-8 -*-
# R27: WRIT 引号路径受控复现 (原版 PECMD.EXE)
# 预期登记缺陷: 特定 WRIT 写法触发 0xC0000005 (原版自身缺陷, 非移植引入)。
# 安全: 全部文件写入隔离目录 C:\pectest\writ_probe\, 崩溃仅殃及 PECMD 自身进程。
import os
import subprocess

D = r'C:\pectest\writ_probe'
os.makedirs(D, exist_ok=True)

CASES = [
    ('w1_basic',      'WRIT %F%,line1'),
    ('w2_quotedpath', 'WRIT "%F%",line1'),
    ('w3_quotedbody', 'WRIT %F%,"quoted body"'),
    ('w4_embedquote', 'WRIT %F%,a"b"c'),
    ('w5_threearg',   'WRIT %F%,line1,+0'),
    ('w6_quoteeq',    'WRIT %F%,key="value"'),
    ('w7_comma',      'WRIT %F%,a,b'),
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
            tag, code, '<== 0xC0000005 CRASH' if crashed else '',
            'yes' if os.path.exists(out) else 'no'))
    except subprocess.TimeoutExpired:
        print('%s: TIMEOUT' % tag)
