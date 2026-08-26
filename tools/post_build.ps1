# post_build.ps1 - R21: one-shot post-build pipeline (run AFTER [msvc_build] OK)
# 1) refresh symsnap.txt  2) deploy exe to C:\pectest  3) write DEPLOYED_BUILD.txt stamp
# ASCII-only comments (PS5.1 no-BOM rule). Absolute paths only (cmd-cd lesson).
param(
    [string]$RepoRoot = "D:\repo\PECMD反编译",
    [string]$DeployDir = "C:\pectest"
)
$ErrorActionPreference = 'Stop'
$exe = Join-Path $RepoRoot 'build\msvc\pecmd_msvc.exe'
if (-not (Test-Path $exe)) { Write-Error 'exe missing - build first'; exit 1 }

# 1) symsnap refresh (V-Gate gate-0 source of bp addresses)
& powershell -NoProfile -ExecutionPolicy Bypass -File `
    (Join-Path $RepoRoot 'tools\make_symsnap.ps1') -RepoRoot $RepoRoot

# 2) deploy (kill first: GUI message pump locks the exe)
Stop-Process -Name pecmd_msvc -Force -ErrorAction SilentlyContinue
Copy-Item $exe (Join-Path $DeployDir 'pecmd_msvc.exe') -Force

# 3) identity stamp (V3)
$md5 = (Get-FileHash (Join-Path $DeployDir 'pecmd_msvc.exe') -Algorithm MD5).Hash
$h = ''
try { Push-Location $RepoRoot; $h = (& git rev-parse --short HEAD) } finally { Pop-Location }
$line = "hash=$h md5=$md5 time=$((Get-Date).ToString('yyyy-MM-dd HH:mm:ss'))"
Set-Content (Join-Path $DeployDir 'DEPLOYED_BUILD.txt') $line
Write-Output $line
