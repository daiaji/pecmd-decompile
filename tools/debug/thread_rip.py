#!/usr/bin/env python3
"""thread_rip.py — 枚举指定进程各线程 RIP (ctypes, 无需调试器)"""
import ctypes
import ctypes.wintypes as wt
import sys

k32 = ctypes.WinDLL("kernel32", use_last_error=True)

CONTEXT_AMD64 = 0x00100B07


class CONTEXT64(ctypes.Structure):
    _fields_ = [
        ("P1Home", ctypes.c_uint64), ("P2Home", ctypes.c_uint64),
        ("P3Home", ctypes.c_uint64), ("P4Home", ctypes.c_uint64),
        ("P5Home", ctypes.c_uint64), ("P6Home", ctypes.c_uint64),
        ("ContextFlags", ctypes.c_uint32), ("MxCsr", ctypes.c_uint32),
        ("SegCs", ctypes.c_uint16), ("SegDs", ctypes.c_uint16),
        ("SegEs", ctypes.c_uint16), ("SegFs", ctypes.c_uint16),
        ("SegGs", ctypes.c_uint16), ("SegSs", ctypes.c_uint16),
        ("EFlags", ctypes.c_uint32),
        ("Dr0", ctypes.c_uint64), ("Dr1", ctypes.c_uint64),
        ("Dr2", ctypes.c_uint64), ("Dr3", ctypes.c_uint64),
        ("Dr6", ctypes.c_uint64), ("Dr7", ctypes.c_uint64),
        ("Rax", ctypes.c_uint64), ("Rcx", ctypes.c_uint64),
        ("Rdx", ctypes.c_uint64), ("Rbx", ctypes.c_uint64),
        ("Rsp", ctypes.c_uint64), ("Rbp", ctypes.c_uint64),
        ("Rsi", ctypes.c_uint64), ("Rdi", ctypes.c_uint64),
        ("R8", ctypes.c_uint64), ("R9", ctypes.c_uint64),
        ("R10", ctypes.c_uint64), ("R11", ctypes.c_uint64),
        ("R12", ctypes.c_uint64), ("R13", ctypes.c_uint64),
        ("R14", ctypes.c_uint64), ("R15", ctypes.c_uint64),
        ("Rip", ctypes.c_uint64),
    ]


def main(pid):
    TH32CS_SNAPTHREAD = 0x4
    snap = k32.CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0)

    class THREADENTRY32(ctypes.Structure):
        _fields_ = [
            ("dwSize", wt.DWORD), ("cntUsage", wt.DWORD),
            ("th32ThreadID", wt.DWORD), ("th32OwnerProcessID", wt.DWORD),
            ("tpDeltaPri", ctypes.c_long), ("tpPri", ctypes.c_long),
            ("dwFlags", wt.DWORD),
        ]

    te = THREADENTRY32()
    te.dwSize = ctypes.sizeof(te)
    ok = k32.Thread32First(snap, ctypes.byref(te))
    while ok:
        if te.th32OwnerProcessID == pid:
            h = k32.OpenThread(0x1FFFFF, False, te.th32ThreadID)  # THREAD_ALL_ACCESS
            if h:
                k32.SuspendThread(h)
                ctx = CONTEXT64()
                ctx.ContextFlags = CONTEXT_AMD64
                if k32.GetThreadContext(h, ctypes.byref(ctx)):
                    print(f"thread {te.th32ThreadID}: rip=0x{ctx.Rip:x} rsp=0x{ctx.Rsp:x}")
                k32.ResumeThread(h)
                k32.CloseHandle(h)
        ok = k32.Thread32Next(snap, ctypes.byref(te))
    k32.CloseHandle(snap)


if __name__ == "__main__":
    main(int(sys.argv[1]))
