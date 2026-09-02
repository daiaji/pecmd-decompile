# -*- coding: utf-8 -*-
# R27: PART-on-VHD 探针 v3 — 格式矩阵 + EXEC echo 值回捞 (借用 harness 通道惯例)
# 安全边界: 仅操作本 VHD; PART 仅 list 族 (只读)。
import os
import subprocess
import sys

D = r'C:\pectest\vhd_part'
VHD = os.path.join(D, 'p64.vhdx')
VALS = os.path.join(D, 'vals.txt')
os.makedirs(D, exist_ok=True)

def run(cmd, **kw):
    r = subprocess.run(cmd, capture_output=True, text=True, **kw)
    return r.returncode, (r.stdout or '') + (r.stderr or '')

def diskpart(script):
    sp = os.path.join(D, 'dp.txt')
    with open(sp, 'w', newline='\r\n') as f:
        f.write(script + '\n')
    return run(['diskpart', '/s', sp])

def meta(s):
    for ch in ('^', '&', '|', '<', '>'):
        s = s.replace(ch, '^' + ch)
    return s

rc, out = diskpart(
    f'create vdisk file="{VHD}" maximum=64 type=fixed\n'
    f'select vdisk file="{VHD}"\n'
    'attach vdisk\n')

rc, out = run(['powershell', '-NoProfile', '-Command',
               '(Get-Disk | Where-Object Location -like "*p64.vhdx*").Number'])
num = out.strip().splitlines()[-1].strip() if out.strip() else ''
print('=== VHD disk number: %r ===' % num)
if not num.isdigit():
    diskpart(f'select vdisk file="{VHD}"\ndetach vdisk\n')
    if os.path.exists(VHD):
        os.remove(VHD)
    sys.exit(1)

# 格式矩阵: 4 变体, 每变体独立变量
probe = os.path.join(D, 'probe.pecmd')
lines = [
    'ENVI A=x', 'PART list disk,A',
    'ENVI B=x', 'PART B,list disk',
    'ENVI C=x', 'PART list disk C',
    'ENVI D2=x', 'PART D2,list disk,',
    'ENVI DONE=yes',
]
if os.path.exists(VALS):
    os.remove(VALS)
content = meta('A=%A%|B=%B%|C=%C%|D2=%D2%|DONE=%DONE%')
lines.append(r'EXEC -hide =C:\Windows\System32\cmd.exe /c echo %s>"%s"' % (content, VALS))
with open(probe, 'w', newline='\r\n') as f:
    f.write('\n'.join(lines) + '\n')

env = os.environ.copy()
for c in range(ord('A'), ord('Z') + 1):
    env.pop(chr(c), None)

results = {}
for label, exe in (('orig', r'C:\pectest\PECMD.EXE'),
                   ('msvc', r'C:\pectest\pecmd_msvc.exe')):
    if os.path.exists(VALS):
        os.remove(VALS)
    r = subprocess.run([exe, 'LOAD', probe], cwd=r'C:\pectest', env=env,
                       capture_output=True, text=True, timeout=60)
    vals = open(VALS, encoding='utf-8', errors='replace').read().strip() if os.path.exists(VALS) else '<no-vals-file>'
    results[label] = (r.returncode, vals)
    print(f'=== [{label}] exit={r.returncode} vals={vals!r}')

diskpart(f'select vdisk file="{VHD}"\ndetach vdisk\n')
if os.path.exists(VHD):
    os.remove(VHD)
print('=== detached + cleaned ===')
