# -*- coding: utf-8 -*-
# R27: PART-on-VHD 最小链路验证
# 创建固定 64MB VHD -> attach -> 探测磁盘号 -> 原版 PECMD PART list 只读探针 -> detach -> 清理
# 安全边界: 全程仅操作本 VHD (虚拟盘), 不触碰任何实体磁盘; PART 只用 list (只读枚举)。
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

# 1) create + attach
rc, out = diskpart(
    f'create vdisk file="{VHD}" maximum=64 type=fixed\n'
    f'select vdisk file="{VHD}"\n'
    'attach vdisk\n')
print('=== diskpart create+attach rc=%d ===' % rc)
print(out[-800:])

# 2) locate disk number
rc, out = run(['powershell', '-NoProfile', '-Command',
               '(Get-Disk | Where-Object Location -like "*p64.vhdx*").Number'])
num = out.strip().splitlines()[-1].strip() if out.strip() else ''
print('=== VHD disk number: %r ===' % num)
if not num.isdigit():
    print('FAIL: no disk number; detach+cleanup')
    diskpart(f'select vdisk file="{VHD}"\ndetach vdisk\n')
    if os.path.exists(VHD):
        os.remove(VHD)
    sys.exit(1)

# 3) original PECMD probe: PART list disk / part (read-only)
probe = os.path.join(D, 'probe.pecmd')
lines = [
    'PART list disk,VDK',
    'PART list part,VPK',
    'PART list drv,VRV',
    'ENVI DONE=yes',
]
with open(probe, 'w', newline='\r\n') as f:
    f.write('\n'.join(lines) + '\n')

env = os.environ.copy()
for c in range(ord('A'), ord('Z') + 1):  # 与 harness 同款: 清单字符环境变量
    env.pop(chr(c), None)
r = subprocess.run([r'C:\pectest\PECMD.EXE', 'LOAD', probe],
                   cwd=r'C:\pectest', env=env, capture_output=True, text=True,
                   timeout=60)
print('=== orig PECMD PART probe exit=%d ===' % r.returncode)
print((r.stdout or '')[:500])

# 4) results
rc, out = run(['powershell', '-NoProfile', '-Command',
               f'$d=Get-Disk {num}; $p=Get-Partition -DiskNumber {num} -ErrorAction SilentlyContinue; '
               f'"diskstate=" + $d.PartitionStyle + " parts=" + (@($p).Count)'])
print('=== post-state ===')
print(out.strip())

# 5) detach + cleanup
diskpart(f'select vdisk file="{VHD}"\ndetach vdisk\n')
if os.path.exists(VHD):
    os.remove(VHD)
print('=== detached + cleaned ===')
