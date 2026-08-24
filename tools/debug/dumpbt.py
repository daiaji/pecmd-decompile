#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""dumpbt.py — WER LocalDumps minidump 定位工具 v2 (异常流+模块+Memory64List 栈回扫+map符号)

用法:
    python tools\\debug\\dumpbt.py <dump.dmp> [map]

输出: 异常码/地址、崩溃模块+RVA+map符号、关键寄存器、栈上 pecmd_msvc 返回地址候选。
坑位: MINIDUMP_EXCEPTION 的 ExceptionAddress 在流内 offset 24 (非文档 16);
ThreadContext 的 Rva 直指 CONTEXT (无 MEMORY_DESCRIPTOR 头), x64 RIP 在 ctx+0xF8;
MSVC map 第三列 Rva+Base 为 16 位裸 hex。
"""
import bisect
import os
import struct
import sys

FASTFAIL = {1: "GF_TRANSIENT_TYPES", 2: "FATAL_APP_EXIT(abort)", 3: "RFS",
            4: "GUARD_ICALL", 5: "GUARD_WRITE", 6: "INVALID_FIBER_SWITCH",
            7: "INVALID_SET_OF_CONTEXT"}
AVCODES = {0xC0000005: "ACCESS_VIOLATION", 0xC0000409: "STACK_BUFFER_OVERRUN/fastfail",
           0xC0000374: "HEAP_CORRUPTION", 0xC00000FD: "STACK_OVERFLOW",
           0xC000041D: "FATAL_USER_CALLBACK", 0xE06D7363: "C++ EH", 0x80000003: "BREAKPOINT"}


def load(path):
    with open(path, "rb") as f:
        return f.read()


def streams_of(data):
    _, _, n, dirrva = struct.unpack_from("<IIII", data, 0)
    out = {}
    for i in range(n):
        st, sz, rva = struct.unpack_from("<III", data, dirrva + 12 * i)
        if sz:
            out.setdefault(st, (sz, rva))
    return out


def exc_of(data, st):
    if 6 not in st:
        return None
    _, off = st[6]
    code = struct.unpack_from("<I", data, off + 8)[0]
    addr = struct.unpack_from("<Q", data, off + 24)[0]  # 实测 offset 24
    csize, crva = struct.unpack_from("<II", data, off + 160)
    regs = {}
    if crva and csize >= 0x100:
        names = ["rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
                 "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "rip"]
        for i, nm in enumerate(names):
            regs[nm] = struct.unpack_from("<Q", data, crva + 0x78 + 8 * i)[0]
    # AV 细节: EXCEPTION_INFORMATION[0]=读/写, [1]=访问的地址
    info0 = struct.unpack_from("<Q", data, off + 32)[0]
    info1 = struct.unpack_from("<Q", data, off + 40)[0]
    return {"code": code & 0xFFFFFFFF, "addr": addr, "regs": regs,
            "av_rw": info0, "av_target": info1}


def mods_of(data, st):
    out = []
    if 4 not in st:
        return out
    _, off = st[4]
    n = struct.unpack_from("<I", data, off)[0]
    for i in range(n):
        m = off + 4 + i * 108
        base, size, _, _, namerva = struct.unpack_from("<QIIII", data, m)
        nlen = struct.unpack_from("<I", data, namerva)[0]
        name = data[namerva + 4:namerva + 4 + nlen].decode("utf-16-le", "replace")
        out.append({"base": base, "size": size, "name": os.path.basename(name)})
    return out


def mem64_of(data, st):
    if 9 not in st:
        return []
    _, off = st[9]
    n, base_rva = struct.unpack_from("<QQ", data, off)
    rng, rva = [], base_rva
    for i in range(n):
        s, sz = struct.unpack_from("<QQ", data, off + 16 + 16 * i)
        rng.append((s, s + sz, rva))
        rva += sz
    return rng


def read_va(data, rng, va, size):
    for s, e, rva in rng:
        if s <= va < e:
            k = min(size, e - va)
            return data[rva + va - s: rva + va - s + k]
    return None


def map_syms(mapfile):
    syms = []
    if mapfile and os.path.exists(mapfile):
        for line in open(mapfile, "r", encoding="latin-1", errors="replace"):
            p = line.split()
            if len(p) >= 3 and ":" in p[0]:
                try:
                    va = int(p[2], 16)
                except ValueError:
                    continue
                if 0x140000000 <= va < 0x150000000:
                    syms.append((va, p[1]))
        syms.sort()
    return syms


def sym(syms, rva):
    t = 0x140000000 + rva
    i = bisect.bisect_right(syms, (t, "\xff")) - 1
    if 0 <= i < len(syms):
        va, nm = syms[i]
        return "%s+0x%x" % (nm, t - va)
    return "??"


def main():
    dump = sys.argv[1]
    mapfile = sys.argv[2] if len(sys.argv) > 2 else os.path.join(
        os.path.dirname(dump), "..", "..", "repo", "build")
    cand = [sys.argv[2] if len(sys.argv) > 2 else None,
            os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..",
                         "build", "msvc", "pecmd_msvc.map")]
    mapfile = next((c for c in cand if c and os.path.exists(c)), None)

    data = load(dump)
    st = streams_of(data)
    ex = exc_of(data, st)
    mods = mods_of(data, st)
    syms = map_syms(mapfile)
    if ex is None:
        print("no exception stream")
        return
    tag = AVCODES.get(ex["code"], "")
    print("exception: 0x%08X %s" % (ex["code"], tag))
    if ex["code"] == 0xC0000005:
        print("  %s target=0x%x" % ("DEP-exec/read?" if ex["av_rw"] == 8 else
                                    ("write" if ex["av_rw"] else "read"), ex["av_target"]))
    if ex["code"] == 0xC0000409:
        rcx = ex["regs"].get("rcx", 0)
        print("  fastfail code rcx=%u (%s)" % (rcx, FASTFAIL.get(rcx, "")))

    def where(va):
        for m in mods:
            if m["base"] <= va < m["base"] + m["size"]:
                rva = va - m["base"]
                extra = (" sym=" + sym(syms, rva)) if m["name"].lower().startswith("pecmd") else ""
                return "%s!+0x%x%s" % (m["name"], rva, extra)
        return "(unknown module)"

    print("fault ip : 0x%012X  %s" % (ex["addr"], where(ex["addr"])))
    for k in ["rip", "rax", "rcx", "rdx", "r8", "r9", "rsp"]:
        if k in ex["regs"]:
            v = ex["regs"][k]
            print("%s=0x%012X  %s" % (k, v, where(v) if v > 0x10000 else ""))

    rng = mem64_of(data, st)
    rsp = ex["regs"].get("rsp", 0)
    buf = read_va(data, rng, rsp, 0x8000)
    tgt = next((m for m in mods if m["name"].lower().startswith("pecmd_msvc")), None)
    if buf and tgt:
        lo, hi = tgt["base"], tgt["base"] + tgt["size"]
        print("--- stack candidates (rsp+0..0x8000) ---")
        seen = set()
        for off in range(0, len(buf) - 8):
            v = struct.unpack_from("<Q", buf, off)[0]
            if lo <= v < hi and v not in seen:
                seen.add(v)
                print("  rsp+0x%-5x 0x%012X  %s" % (off, v, sym(syms, v - tgt["base"])))


if __name__ == "__main__":
    main()
