# -*- coding: utf-8 -*-
# R27: WRIT 修饰符变体复现 (第二轮)
import os
import subprocess

D = r'C:\pectest\writ_probe'
os.makedirs(D, exist_ok=True)

CASES = [
    ('m1_starv',      'WRIT *v F,line1'),          # *v: 写变量?
    ('m2_starfv',     'WRIT *fv F,line1'),
    ('m3_starfix',    'WRIT *fix %F%,line1'),
    ('m4_starnobom',  'WRIT *nobom %F%,line1'),
    ('m5_starc',      'WRIT *c %F%,line1'),
    ('m6_starsparse', 'WRIT *sparse %F%,line1'),
    ('m7_multi_star', 'WRIT *fix*nobom %F%,line1'),
    ('m8_star_first', 'WRIT *v,%F%,line1'),
    ('m9_star_space', 'WRIT *v %F%,line1,extra'),
    ('m10_eq_val',    'WRIT *v F=key"quoted"'),
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
