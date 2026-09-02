# -*- coding: utf-8 -*-
# R27: PART-on-VHD 探针 v2 — 格式矩阵 + 双引擎对照 (原版 vs msvc)
# 安全边界: 仅操作本 VHD (虚拟盘); PART 仅用 list 族 (只读枚举)。
import os
import subprocess
import sys

D = r'C:\pectest\vhd_part'
VHD = os.path.join(D, 'p64.vhdx')
os.makedirs(D, exist_ok=True)

def run(cmd, **kw):
    r = subprocess.run(cmd, capture_output=True, text=True, **kw)
    return r.returncode, (r.stdout or '') + (r.stderr or '')

def diskpart(script):
    sp = os.path.join(D, 'dp.txt')
    with open(sp, 'w', newline='\r\n') as f:
        f.write(script + '\n')
    return run(['diskpart', '/s', sp])

rc, out = diskpart(
    f'create vdisk file="{VHD}" maximum=64 type=fixed\n'
    f'select vdisk file="{VHD}"\n'
    'attach vdisk\n')
print('=== diskpart attach rc=%d ===' % rc)

rc, out = run(['powershell', '-NoProfile', '-Command',
               '(Get-Disk | Where-Object Location -like "*p64.vhdx*").Number'])
num = out.strip().splitlines()[-1].strip() if out.strip() else ''
print('=== VHD disk number: %r ===' % num)
if not num.isdigit():
    diskpart(f'select vdisk file="{VHD}"\ndetach vdisk\n')
    if os.path.exists(VHD):
        os.remove(VHD)
    sys.exit(1)

# 格式矩阵: 每变体独立变量 (vars 逐条锁定)
probe = os.path.join(D, 'probe.pecmd')
lines = [
    'ENVI V1=x',
    'PART list disk,V1',
    'ENVI V2=x',
    'PART list part,V2',
    'ENVI V3=x',
    'PART list drv,V3',
    'ENVI V4=x',
    'PART list disk',
    'ENVI DONE=yes',
]
with open(probe, 'w', newline='\r\n') as f:
    f.write('\n'.join(lines) + '\n')

env = os.environ.copy()
for c in range(ord('A'), ord('Z') + 1):
    env.pop(chr(c), None)

for label, exe in (('orig', r'C:\pectest\PECMD.EXE'),
                   ('msvc', r'C:\pectest\pecmd_msvc.exe')):
    r = subprocess.run([exe, 'LOAD', probe], cwd=r'C:\pectest', env=env,
                       capture_output=True, text=True, timeout=60)
    print(f'=== [{label}] exit={r.returncode} ===')

# 变量值回捞: PECMD 无外部回捞口 -> 用第二条探针把变量 WRITE 到文件? WRITE 有 D-26 风险。
# 改用 ENVI 写文件 (PECMD ENVI 支持写文件? 语料 005 用 WRITE)。探针 v3: 循环外写 vars
probe2 = os.path.join(D, 'probe2.pecmd')
lines2 = [
    'ENVI V1=x',
    'PART list disk,V1',
    'ENVI V2=x',
    'PART list part,V2',
    'ENVI V3=x',
    'PART list drv,V3',
    'ENVI DONE=yes',
]
with open(probe2, 'w', newline='\r\n') as f:
    f.write('\n'.join(lines2) + '\n')
print('probe2 written (value-capture via harness epilogue is next step)')

diskpart(f'select vdisk file="{VHD}"\ndetach vdisk\n')
if os.path.exists(VHD):
    os.remove(VHD)
print('=== detached + cleaned ===')
