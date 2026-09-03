# -*- coding: utf-8 -*-
# R27-d: D-28 探测 — hdN 形态 (词修复后) + init/clean 写案语法探测
# 安全: 仅操作夹具自建 VHD; 写案前确认磁盘号定位。
import os
import subprocess
import sys

D = r'C:\pectest\vhd_part'
VHD = os.path.join(D, 'd28.vhdx')
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

rc, _ = diskpart(
    f'create vdisk file="{VHD}" maximum=100 type=fixed\n'
    f'select vdisk file="{VHD}"\nattach vdisk\n')
rc, out = run(['powershell', '-NoProfile', '-Command',
               '(Get-Disk | Where-Object Location -like "*d28.vhdx*").Number'])
num = out.strip().splitlines()[-1].strip() if out.strip() else ''
print('=== VHD disk:', num, '===')
if not num.isdigit():
    diskpart(f'select vdisk file="{VHD}"\ndetach vdisk\n')
    sys.exit(1)

env = os.environ.copy()
for c in range(ord('A'), ord('Z') + 1):
    env.pop(chr(c), None)

def probe(tag, lines, exe):
    p = os.path.join(D, tag + '.pecmd')
    if os.path.exists(VALS):
        os.remove(VALS)
    full = ['ENVI A=x'] + lines + ['ENVI DONE=yes']
    full.append(r'EXEC -hide =C:\Windows\System32\cmd.exe /c echo %s>"%s"'
                % (meta('A=%A%|DONE=%DONE%'), VALS))
    with open(p, 'w', newline='\r\n') as f:
        f.write('\n'.join(full) + '\n')
    try:
        r = subprocess.run([exe, 'LOAD', p], cwd=r'C:\pectest', env=env,
                           capture_output=True, text=True, timeout=30)
        code = r.returncode & 0xffffffff
        vals = open(VALS, encoding='utf-8', errors='replace').read().strip() \
            if os.path.exists(VALS) else '<none>'
        print('[%s] exit=0x%08x vals=%r' % (tag, code, vals))
    except subprocess.TimeoutExpired:
        print('[%s] TIMEOUT' % tag)

ORIG = r'C:\pectest\PECMD.EXE'
MSVC = r'C:\pectest\pecmd_msvc.exe'

# 1) hdN 形态 (词修复后)
lines_hd = [f'PART list disk hd{num},A']
print('--- hdN form (D-28 core) ---')
probe('d28_hd_orig', lines_hd, ORIG)
probe('d28_hd_msvc', lines_hd, MSVC)

# 2) init 语法探测 (写案 — VHD 上安全)
print('--- init probes (orig only; VHD is disposable) ---')
for tag, stmt in (
    ('i1', f'PART init hd{num},A'),
    ('i2', f'PART init hd{num} -gpt,A'),
    ('i3', f'PART init hd{num} -mbr,A'),
):
    probe('d28_' + tag + '_orig', [stmt], ORIG)

diskpart(f'select vdisk file="{VHD}"\ndetach vdisk\n')
if os.path.exists(VHD):
    os.remove(VHD)
print('=== detached + cleaned ===')
