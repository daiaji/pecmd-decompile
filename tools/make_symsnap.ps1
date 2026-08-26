# make_symsnap.ps1 - V-Gate gate-0 infrastructure (R20)
# Run after successful build: generates build\msvc\symsnap.txt
# (git hash + key-symbol RVA snapshot). Breakpoint addresses MUST come
# from this file, never from stale notes (discipline V1).
# NOTE: ASCII-only on purpose - Windows PowerShell 5.1 misparses UTF-8
# scripts without BOM (Chinese comments broke the parser once already).
param([string]$RepoRoot = "D:\repo\PECMD反编译")
Set-Location $RepoRoot
$map = Join-Path $RepoRoot 'build\msvc\pecmd_msvc.map'
$exe = Join-Path $RepoRoot 'build\msvc\pecmd_msvc.exe'
if (-not (Test-Path $map))  { Write-Error 'map missing'; exit 1 }
if (-not (Test-Path $exe))  { Write-Error 'exe missing'; exit 1 }

$gitHash = ''
try { $gitHash = (& git rev-parse --short HEAD) 2>$null } catch {}
$buildTime = (Get-Item $exe).LastWriteTime.ToString('yyyy-MM-dd HH:mm:ss')
$syms = @('PECMD_ProcessScriptBlock','PECMD_ExecCmdDispatch','PECMD_ExpandEnvVars',
          'PECMD_MainW','PECMD_RunCommand','PECMD_RunScriptText','PECMD_WaitTickCount',
          'PECMD_CreateProcessW','PECMD_OpenFileHandle','PECMD_GetExitCodeGlobal',
          'PECMD_DispatchBuiltin','XDigit','PECMD_RunStartupScript','PECMD_RunBootScriptInFiber')

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add("# symsnap build=$gitHash time=$buildTime")
$lines.Add("# RVA = VA - 0x140000000 (map col 3). Sole legal source for bp addresses (V1).")
foreach ($s in $syms) {
    $pat = '\s' + [regex]::Escape($s) + '\s+([0-9a-f]{16})'
    $hit = Select-String -Path $map -Pattern $pat | Select-Object -First 1
    if ($hit) {
        $rva = [Convert]::ToInt64($hit.Matches[0].Groups[1].Value, 16) - 0x140000000
        $lines.Add(('{0} 0x{1:x}' -f $s, $rva))
    } else {
        $lines.Add("$s NOT_FOUND")
    }
}
$out = Join-Path $RepoRoot 'build\msvc\symsnap.txt'
$lines | Set-Content $out -Encoding ASCII
Write-Output ("symsnap written: {0} ({1} lines)" -f $out, $lines.Count)
