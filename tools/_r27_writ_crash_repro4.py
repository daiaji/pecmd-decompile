# -*- coding: utf-8 -*-
# R27: WRIT 崩溃复现第四轮 — 展开后引号/逗号 (引号扫描器在变量展开之后运行)
import os
import subprocess

D = r'C:\pectest\writ_probe'
os.makedirs(D, exist_ok=True)

# 变量值经 %..% 展开后形成参数结构变化 -> 引号扫描器路径
CASES = [
    ('y1_quote_var',   'ENVI C1=a"b\nWRIT %F%,%C1%'),
    ('y2_comma_var',   'ENVI C2=a,b\nWRIT %F%,%C2%'),
    ('y3_qc_var',      'ENVI C3=a"b,c\nWRIT %F%,%C3%'),
    ('y4_bare_quote',  'ENVI C4="\nWRIT %F%,%C4%x'),
    ('y5_lead_quote',  'ENVI C5="lead\nWRIT %F%,%C5%'),
    ('y6_quote_pair',  'ENVI C6="x"\nWRIT %F%,%C6%'),
    ('y7_three_arg',   'ENVI C7=a,b,c\nWRIT %F%,%C7%'),
    ('y8_trail_q',     'ENVI C8=abc"\nWRIT %F%,%C8%'),
    ('y9_q_in_starv',  'ENVI C9=q"r\nWRIT *v F,%C9%'),
    ('y10_qpos',       'ENCI X=x\nENVI C10=+0,a"b\nWRIT %F%,%C10%'),
]

env = os.environ.copy()
for c in range(ord('A'), ord('Z') + 1):
    env.pop(chr(c), None)

for tag, block in CASES:
    p = os.path.join(D, tag + '.pecmd')
    lines = ['ENVI F=' + os.path.join(D, 'out.txt')] + block.split('\n') + ['ENVI DONE=yes']
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
