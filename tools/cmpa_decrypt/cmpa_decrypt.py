#!/usr/bin/env python3
"""CMPS CMPa 解码器 — 纯静态还原 PECMD CMPS 加密脚本
算法来源：PECMD_ProcessEncodedFile (FUN_140086944) + 流变换 (0x140068830) + LZ 解压 (FUN_14005c260)
验证：test.wcs 加密→本解码→明文一致（含中文 UTF-16LE）
"""
import struct, sys, io

# Windows 控制台/管道统一 UTF-8，避免中文按 GBK 输出成乱码
if sys.stdout.encoding and sys.stdout.encoding.lower() not in ('utf-8', 'utf8'):
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')
    sys.stderr = io.TextIOWrapper(sys.stderr.buffer, encoding='utf-8', errors='replace')

def xor_stream(data, mul, n0=0x10):
    out = bytearray()
    n = n0
    for b in data:
        t = ((2*n + 3) * mul) & 0xFFFFFFFF
        k = ((t >> 21) & 0x7E) ^ ((t >> 14) & 0xFF) ^ ((t >> 7) & 0xFF) ^ (t & 0xFF)
        out.append(b ^ (k & 0xFF))
        n += 1
    return bytes(out)

def lz_decompress(src):
    win = bytearray([0x20] * 0xfee + [0] * (4096 - 0xfee))
    wpos = 0xfee
    out = bytearray()
    pos = 0
    def r8():
        nonlocal pos
        if pos >= len(src): return -1
        b = src[pos]; pos += 1
        return b
    bitbuf = 0
    while True:
        flag = (bitbuf >> 9) & 1
        bitbuf >>= 1
        if flag == 0:
            b = r8()
            if b == -1: break
            bitbuf = b | 0xff00
        if bitbuf & 1:
            c = r8()
            if c == -1: break
            out.append(c); win[wpos] = c
            wpos = (wpos + 1) & 0xfff
        else:
            a = r8()
            if a == -1: break
            b2 = r8()
            if b2 == -1: break
            off = a | ((b2 & 0xf0) << 4)
            ln = (b2 & 0xf) + 3
            si = off & 0xfff
            for _ in range(ln):
                c = win[si]
                out.append(c); win[wpos] = c
                wpos = (wpos + 1) & 0xfff
                si = (si + 1) & 0xfff
    return bytes(out)

def decode_cmpa(data, mode=0x14):
    assert data[4:8] == b'CMPa', f'not CMPa: {data[4:8]!r}'
    seed = struct.unpack('<I', data[8:12])[0]
    K = 0x5aa59669
    X = ((((seed >> 16) & 0xFFFF) << 16) | ((seed & 0xFFFF) ^ mode)) ^ K
    X &= 0xFFFFFFFF
    print(f'[info] seed=0x{seed:08x} X=0x{X:08x} mul=0x{X:08x} mode=0x{mode:02x}')
    lz = xor_stream(data[16:], X)  # mul = X 全 32 位
    plain = lz_decompress(lz)
    return plain

def main():
    if len(sys.argv) < 3:
        print('usage: cmpa_decrypt.py <in.cmp> <out.wcs> [mode]')
        return 1
    data = open(sys.argv[1], 'rb').read()
    mode = int(sys.argv[3], 16) if len(sys.argv) > 3 else 0x14
    plain = decode_cmpa(data, mode)
    # 明文：UTF-16LE 解码为文本；失败则原样字节
    txt = None
    try:
        txt = plain.decode('utf-16-le')
    except Exception:
        pass
    open(sys.argv[2], 'wb').write(plain)
    print(f'[ok] 明文 {len(plain)} 字节 -> {sys.argv[2]}')
    if txt is not None:
        # 额外存一份 UTF-8 文本（含 BOM，记事本/编辑器直接可读）
        u8path = sys.argv[2] + '.txt' if not sys.argv[2].lower().endswith('.txt') else sys.argv[2]
        with open(u8path, 'w', encoding='utf-8-sig', newline='') as f:
            f.write(txt)
        print(f'[ok] UTF-8 文本 -> {u8path}')
        print('----- 内容预览 -----')
        print(txt[:500])
    return 0

if __name__ == '__main__':
    sys.exit(main())
