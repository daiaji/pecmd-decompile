# run_corpus.ps1 - R21: full-corpus verdict runner (absolute-path safe)
# Usage:  powershell -File tools\run_corpus.ps1            # all cases + diff
#         powershell -File tools\run_corpus.ps1 021_sub_call -Timeout 240   # single case
param(
    [string]$RepoRoot = "D:\repo\PECMD反编译",
    [string]$CaseId = "",
    [int]$Timeout = 0,
    [switch]$SkipDiff,
    [ValidateSet('both','orig','msvc')] [string]$Exe = 'both'
)
Push-Location $RepoRoot
try {
    $argList = @('harness\runners\run_case.py')
    if ($CaseId) { $argList += $CaseId } else { $argList += '--all' }
    if ($Timeout -gt 0) { $argList += @('--timeout', $Timeout) }
    $argList += @('--exe', $Exe)
    & python @argList 2>&1 | Select-Object -Last 5
    if (-not $CaseId -and -not $SkipDiff) {
        "=====DIFF====="
        & python harness\runners\diff_case.py --all 2>&1 | Select-Object -Last 10
    }
} finally { Pop-Location }
