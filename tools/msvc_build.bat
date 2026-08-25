@echo off
rem ============================================================
rem msvc_build.bat - MSVC build entry (PRODUCTION_ROADMAP v4 P0-3)
rem
rem Usage:
rem   1) From "x64 Native Tools Command Prompt for VS 2019":
rem        tools\msvc_build.bat            full build -> build\msvc\pecmd_msvc.exe
rem        tools\msvc_build.bat syntax     syntax gate (cl /Zs, replaces gcc reference gate on WIN)
rem   2) Or init VS env first from any terminal:
rem        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
rem        tools\msvc_build.bat
rem
rem Notes:
rem   - ASCII-only on purpose: avoids cmd codepage pitfalls (GBK console).
rem   - Sources are collected into a response file to dodge the 8k
rem     command-line length limit (~95 TU + 4 stub files).
rem   - /utf-8 is mandatory: sources are UTF-8 with Chinese comments;
rem     without it MSVC assumes ACP and may misparse (C4819/C2001).
rem   - Until P0-1/P0-2 land (33 compound literals + crt_shims trim),
rem     the full build is EXPECTED to fail; syntax gate shows them all.
rem ============================================================
setlocal enabledelayedexpansion
rem S11: 确定性码页 - 防止外部控制台码页污染 sources.rsp 路径字节(曾致 C1083 全灭)
chcp 936 >nul

where cl >nul 2>nul
if errorlevel 1 (
  echo [msvc_build] ERROR: cl.exe not in PATH.
  echo [msvc_build] Run from "x64 Native Tools Command Prompt for VS 2019",
  echo [msvc_build] or first: call "...vcvarsall.bat" x64
  exit /b 1
)

rem Normalize repo root from this script's location (tools\ -> repo root)
for %%I in ("%~dp0..") do set "ROOT=%%~fI"
set "OUTDIR=%ROOT%\build\msvc"
if not exist "%OUTDIR%" mkdir "%OUTDIR%"

set "SYN="
if /i "%~1"=="syntax" set "SYN=/Zs"

set "CFLAGS=/nologo /W4 /std:c11 /utf-8 /Zi /FS /D_CRT_SECURE_NO_WARNINGS"

rem --- Collect sources into response file ---
set "RSP=%OUTDIR%\sources.rsp"
type nul > "%RSP%"
for /r "%ROOT%\src" %%f in (*.c) do >>"%RSP%" echo "%%f"
>>"%RSP%" echo "%ROOT%\restored_bodies.c"
>>"%RSP%" echo "%ROOT%\win32_api_stubs.c"
>>"%RSP%" echo "%ROOT%\unimplemented_stubs.c"
>>"%RSP%" echo "%ROOT%\crt_shims.c"

rem Forward slashes avoid the trailing-backslash-before-quote parsing trap
set "OUTFWD=%OUTDIR:\=/%"

if defined SYN (
  rem Syntax-only gate: no obj/exe produced, link never runs
  cl %CFLAGS% %SYN% /I"%ROOT%\include" @"%RSP%"
) else (
  del /q "%OUTDIR%\pecmd_msvc.exe" 2>nul
  cl %CFLAGS% /I"%ROOT%\include" @"%RSP%" ^
     /Fe"%OUTFWD%/pecmd_msvc.exe" /Fo"%OUTFWD%/" /Fd"%OUTFWD%/" ^
     /link advapi32.lib user32.lib gdi32.lib shell32.lib ole32.lib ws2_32.lib winmm.lib ^
     cfgmgr32.lib setupapi.lib shlwapi.lib version.lib ^
     /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup /MAP:"%OUTFWD%/pecmd_msvc.map"
)
if errorlevel 1 (
  echo [msvc_build] FAIL: see errors above.
  exit /b 1
)

if defined SYN (
  echo [msvc_build] OK: syntax gate passed.
) else (
  if exist "%OUTDIR%\pecmd_msvc.exe" (
    echo [msvc_build] OK: build\msvc\pecmd_msvc.exe produced.
    echo [msvc_build] Next: copy to C:\pectest\ then run_case.py will pick it up.
  ) else (
    echo [msvc_build] FAIL: exe missing despite exit 0.
    exit /b 1
  )
)
endlocal
