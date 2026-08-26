#!/usr/bin/env python3
# build_msvc.py - R23: MSVC build (python-full implementation, UTF-8 chain).
# Usage:
#   python tools/build_msvc.py              full build -> build/msvc/pecmd_msvc.exe
#   python tools/build_msvc.py syntax       syntax gate (cl /Zs, no link)
#
# Design (R23 decisions):
#   - ZERO CJK reaches the cmd layer: sources.rsp and the temp .bat are
#     written with RELATIVE paths (src\\app\\core_init.c, build\\msvc\\...),
#     and cmd.exe runs with cwd=repo root. The CJK root path lives only in
#     bash/python (UTF-8 native) and in cmd's process cwd, never in a file
#     cmd/cl parses.
#   - Full UTF-8: chcp 65001 for the cmd child; stdout decoded as UTF-8.
#     cl /utf-8 already handles UTF-8 sources. GBK/936 no longer used.
#   - cl.exe needs vcvarsall (cmd-only) -> materialized as temp bat:
#     build/msvc/_msvc_cmd.bat (ASCII content, UTF-8 encoded file).
#   - Green gate unchanged: exit 0 + "[msvc_build] OK" + exe exists.
import os, subprocess, sys

def main():
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    outdir = os.path.join(root, "build", "msvc")
    os.makedirs(outdir, exist_ok=True)
    syn = "/Zs" if (len(sys.argv) > 1 and sys.argv[1] == "syntax") else ""

    # --- 1) sources.rsp: relative paths, ASCII content, UTF-8 file ---
    rsp_path = os.path.join("build", "msvc", "sources.rsp")  # relative on purpose
    rsp_abs = os.path.join(root, rsp_path)
    tucount = 0
    with open(rsp_abs, "w", encoding="utf-8", newline="\n") as f:
        for dp, _dirs, files in os.walk(os.path.join(root, "src")):
            for fn in sorted(files):
                if fn.endswith(".c"):
                    rel = os.path.relpath(os.path.join(dp, fn), root)
                    f.write('"%s"\n' % rel.replace("\\", "\\\\"))
                    tucount += 1
        for fn in ("restored_bodies.c", "win32_api_stubs.c",
                   "unimplemented_stubs.c", "crt_shims.c"):
            f.write('"%s"\n' % fn)
        tucount += 4
    print("[build_msvc] rsp: %d TUs -> %s" % (tucount, rsp_abs))

    # --- 2) temp bat: ASCII content (relative paths), UTF-8 file ---
    vcvars = r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
    cflags = "/nologo /W4 /std:c11 /utf-8 /Zi /FS /D_CRT_SECURE_NO_WARNINGS"
    libs = ("advapi32.lib user32.lib gdi32.lib shell32.lib ole32.lib ws2_32.lib "
            "winmm.lib cfgmgr32.lib setupapi.lib shlwapi.lib version.lib")
    if syn:
        cl = ('cl %s %s /I"include" @"%s"' % (cflags, syn, rsp_path))
        gate = "@echo [msvc_build] OK: syntax gate passed."
    else:
        cl = ('cl %s /I"include" @"%s" /Fe"build/msvc/pecmd_msvc.exe" '
              '/Fo"build/msvc/" /Fd"build/msvc/" /link %s '
              '/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup /MAP:"build/msvc/pecmd_msvc.map"'
              % (cflags, rsp_path, libs))
        gate = ('@if exist "build\\msvc\\pecmd_msvc.exe" ( '
                '@echo [msvc_build] OK: build\\msvc\\pecmd_msvc.exe produced. '
                '@echo [msvc_build] Next: copy to C:\\pectest\\ then run_case.py will pick it up. '
                ') else ( @echo [msvc_build] FAIL: exe missing despite exit 0. & @exit /b 1 )')
    bat = ("@echo off\r\n"
           "chcp 65001 >nul\r\n"
           '@call "%s" x64 >nul\r\n' % vcvars +
           cl + "\r\n"
           "@if errorlevel 1 ( @echo [msvc_build] FAIL: see errors above. & @exit /b 1 )\r\n"
           + gate + "\r\n")
    bat_abs = os.path.join(outdir, "_msvc_cmd.bat")
    with open(bat_abs, "w", encoding="utf-8", newline="") as f:
        f.write(bat)

    # --- 3) run under cmd.exe with cwd=root (bat paths are relative) ---
    if not syn and os.path.exists(os.path.join(outdir, "pecmd_msvc.exe")):
        os.remove(os.path.join(outdir, "pecmd_msvc.exe"))
    p = subprocess.run(["cmd.exe", "/c", "build\\msvc\\_msvc_cmd.bat"],
                       cwd=root, capture_output=True)
    out = (p.stdout + p.stderr).decode("utf-8", errors="replace")
    sys.stdout.write(out)
    ok = (p.returncode == 0) and ("[msvc_build] OK" in out)
    if not syn:
        ok = ok and os.path.exists(os.path.join(outdir, "pecmd_msvc.exe"))
    print("[build_msvc] green-gate: %s (exit=%d)" % ("PASS" if ok else "FAIL", p.returncode))
    return 0 if ok else 1

if __name__ == "__main__":
    sys.exit(main())