#!/usr/bin/env python3
"""parse_dump.py — minidump exception RIP -> module+offset (WIN 端崩溃定位辅助)"""
import struct
import sys


def main(path):
    with open(path, "rb") as f:
        assert f.read(4) == b"MDMP"
        f.read(4)                                   # version
        n_streams = struct.unpack("<I", f.read(4))[0]
        dir_rva = struct.unpack("<I", f.read(4))[0]
        streams = {}
        for i in range(n_streams):
            f.seek(dir_rva + i * 12)
            st, size, rva = struct.unpack("<III", f.read(12))
            if size:
                streams[st] = (size, rva)

        # ExceptionStream (6)
        size, rva = streams[6]
        f.seek(rva)
        exc = f.read(size)
        tid = struct.unpack("<I", exc[0:4])[0]
        code = struct.unpack("<I", exc[8:12])[0]
        exc_addr = struct.unpack("<Q", exc[16:24])[0]
        print(f"exception code=0x{code:08x} addr=0x{exc_addr:x} tid={tid}")

        # thread context: MEMORY_DESCRIPTOR{StartOfMemoryRange(8),DataSize(4),Rva(4)} then x64 CONTEXT
        thread_ctx_rva = struct.unpack("<I", exc[164:168])[0]
        if thread_ctx_rva:
            f.seek(thread_ctx_rva)
            f.read(8)                                # start of memory range
            ctx_size = struct.unpack("<I", f.read(4))[0]
            ctx_rva = struct.unpack("<I", f.read(4))[0]
            f.seek(ctx_rva)
            ctx = f.read(ctx_size)
            rip = struct.unpack("<Q", ctx[0xF8:0x100])[0]
            rsp = struct.unpack("<Q", ctx[0x98:0xA0])[0]
            rax = struct.unpack("<Q", ctx[0x78:0x80])[0]
            print(f"context rip=0x{rip:x} rsp=0x{rsp:x} rax=0x{rax:x}")
            # stack scan for return addresses
            stack_candidates = []
            for off in range(0, 512, 8):
                try:
                    v = struct.unpack("<Q", ctx[0x98 + off : 0xA0 + off])[0]
                except struct.error:
                    break
                stack_candidates.append(v)

        # ModuleListStream (4)
        size, rva = streams[4]
        f.seek(rva)
        n_mods = struct.unpack("<I", f.read(4))[0]
        mods = []
        for _ in range(n_mods):
            base, msize, tstamp, name_rva = struct.unpack("<QIII", f.read(20))
            f.seek(f.tell() + 52 + 24)               # VS_FIXEDFILEINFO + Cv + Misc + Reserved
            save = f.tell()
            f.seek(name_rva)
            ln = struct.unpack("<I", f.read(4))[0]
            nm = f.read(ln).decode("utf-16-le")
            f.seek(save)
            mods.append((base, msize, nm))

        def locate(a):
            for base, msize, nm in mods:
                if base <= a < base + msize:
                    return f"{nm.split(chr(92))[-1]}+0x{a - base:x}"
            return None

        loc = locate(exc_addr)
        print(f"exception in: {loc}")
        for i, v in enumerate(stack_candidates):
            loc = locate(v)
            if loc and "pecmd_msvc" in loc:
                print(f"ret@rsp+0x{i * 8:03x}: {loc}")


if __name__ == "__main__":
    main(sys.argv[1])
