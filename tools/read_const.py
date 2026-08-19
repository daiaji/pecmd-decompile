#!/usr/bin/env python3
"""read_const.py — 从 PE 文件读取全局常量 (double/float/int/字符串)

用法:
  read_const.py <dll路径> [地址列表或 -a 自动扫描]
  例: read_const.py ICR8kOMR.dll 0x100EE068 0x100EF3D8 0x100EF610
"""
import struct
import sys


def load_pe(path):
    with open(path, 'rb') as f:
        d = f.read()
    e_lfanew = struct.unpack_from('<I', d, 0x3C)[0]
    magic = struct.unpack_from('<H', d, e_lfanew + 24)[0]
    nsec = struct.unpack_from('<H', d, e_lfanew + 6)[0]
    image_base = struct.unpack_from('<I', d, e_lfanew + 24 + 28)[0]
    if magic == 0x10b:
        sec_base = e_lfanew + 24 + 0xE0
    else:
        sec_base = e_lfanew + 24 + 0xF0
    secs = []
    for i in range(nsec):
        so = sec_base + i * 40
        vs = struct.unpack_from('<I', d, so + 8)[0]
        va = struct.unpack_from('<I', d, so + 12)[0]
        ro = struct.unpack_from('<I', d, so + 20)[0]
        secs.append((va, vs, ro))
    return d, image_base, secs


def va2off(d, image_base, secs, va):
    rva = va - image_base
    for sva, svs, sro in secs:
        if sva <= rva < sva + svs:
            return sro + (rva - sva)
    return None


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(1)
    d, image_base, secs = load_pe(sys.argv[1])

    for a in sys.argv[2:]:
        va = int(a, 16) if a.startswith('0x') else int(a)
        off = va2off(d, image_base, secs, va)
        if off is None:
            print(f"{a}: NOT IN FILE")
            continue
        # 尝试多种解释
        dbl = struct.unpack_from('<d', d, off)[0]
        flt = struct.unpack_from('<f', d, off)[0]
        i32 = struct.unpack_from('<i', d, off)[0]
        # 字符串 (最多 64 字节可打印)
        s = b''
        for j in range(64):
            b = d[off + j]
            if b == 0:
                break
            if 0x20 <= b < 0x7F:
                s += bytes([b])
            else:
                s = b''
                break
        print(f"{a}: double={dbl!r}  float={flt!r}  int={i32}  str={s.decode('latin1') if s else ''}")


if __name__ == '__main__':
    main()
