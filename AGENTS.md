# AGENTS.md — PECMD 反编译参考库 · 代理作业守则（权威入口）

> 所有 AI 代理进入本仓库的作业契约。细节展开见文末「权威文档与技能索引」；
> 本文与专档冲突时以本文为准，本文未尽事项以各专档为准。

## 项目一句话

把 Ghidra 反编译输出（`reference\decompiled.c`，约 18 万行）还原为可编译、与原版
`PECMD.EXE` 行为对拍一致的 MSVC 参考实现，最终服务 Lua FFI 重写的高保真对照。

## 目录地图（R21 重构后：git 根 = 项目根）

```
D:\repo\PECMD反编译\          # ★ git 根 = 项目根（原 refactored 子层已整体上提一级）
├── src\**                    # 还原实现（commands/runtime/lang/kernel/ui/app/misc/device 分包）
├── restored_bodies.c         # dc 直移巨体（ECD 等）；unimplemented_stubs.c / win32_api_stubs.c / crt_shims.c
├── include\                  # stubs_common.h / pecmd_defs.h / win32_stub.h
├── harness\runners           # run_case.py / diff_case.py（对拍裁判）
├── harness\golden            # golden 语料（win_real / win_real_orig）
├── docs\                     # 病理档案与规程（含 DEBUGGER_HANDOFF_legacy.md；索引见 docs\README.md）
├── analysis\                 # 轮次报告（r19a_/r19b_/r19d_… 前缀=轮次-线别）
├── tools\                    # build_msvc.py/.sh (R23 构建主链, UTF-8) / msvc_build.bat (旧对照) / make_symsnap.sh / post_build.sh / run_corpus.sh / 批处理 JSON（勿移动，脚本引用相对路径）
├── .agents\skills\           # 技能库（SKILL.md 格式）
├── build\msvc\               # exe + pecmd_msvc.map + symsnap.txt + build_*.log
├── AGENTS.md / HANDOVER_PROGRESS.md / REVIEW.md / FUNC_NAMES.md …
└── reference\                # 逆向参考件（只读参考，不参与构建）
    ├── decompiled.c          # 唯一语义真值（Ghidra 原始输出，base 0x140000000）
    ├── PECMD.exe / PECMD原始.EXE  # 原版二进制（无 ASLR 运行副本另在 C:\pectest\PECMD.EXE）
    ├── functions.csv/.txt, sx.py, big_funcs\, split\, ghidra_out\   # Ghidra 工作产物
    └── archive\              # 历史 bundle/tar.gz/legacy handover/根目录 build 遗注
C:\pectest\                   # 部署与运行现场：pecmd_msvc.exe、DEPLOYED_BUILD.txt、dumps\、memfail.log

⚠ 过渡期注意：analysis\ 与 docs\ 历史文档中的绝对路径 `...\refactored\...` 指旧布局；
对应文件一律按「refactored\X ⇒ 根\X」换算。历史文档原文不改（保留证据原貌）。
```

## 硬性纪律（每条都有事故背书）

### 构建（主代理专属，子代理禁止构建）
1. **构建前三查**：无在途 windbg 会话（含子代理取证会话，LNK1201 主因）→ 磁盘余量 → 上次构建日志无未处置错误。码页由脚本自管（R23 新链 chcp 65001；旧 bat 自管 936）。
2. 双绿门：exit 0 且输出含 `[msvc_build] OK`。R23 起构建日志按 UTF-8 解码读尾部（旧 build_*.log 历史件仍按 GBK），错误正则 `error (C\d+|LNK\d+)`。
3. 构建成功后必跑 `tools\make_symsnap.sh` 刷新 `build\msvc\symsnap.txt`（或 `bash tools/post_build.sh` 一站完成 symsnap+部署+身份戳）。
4. ⚠ `cmd /c "... && cd /d X && ..."` 里的 cd 只影响子进程——后续 PowerShell 步骤一律绝对路径或显式 Set-Location。

### 部署与身份（V3）
`taskkill /IM pecmd_msvc.exe` → Copy exe 到 `C:\pectest\` → 写 `C:\pectest\DEPLOYED_BUILD.txt`
（git hash + md5 + 时间）。**分析任何日志/dump 前先核对该文件与当前构建一致**。

### 断点/活体调试（V-Gate 五道门 → 技能 `vgate-live-debug`）
- 断点选址唯一合法来源 = 当期 `build\msvc\symsnap.txt`；禁用历史笔记里的 module+offset。
- 下点三验证（bl 地址 / u 指令边界 / 首命中 rip 落 map 区间）；"从未命中"结论须 V1+V2 满足后才成立。
- 挂死进程优先 `attach_process` 抓栈（零地址依赖），其次才是 bp。
- 否定性高影响结论归档/上报前过三关：当期构建复现、阳性对照（bp kernel32!ExitProcess）、第二代理证伪。

### 探针（V4）
- 手写 CRT extern（`fopen/fprintf/fclose`），**禁 `#include <stdio.h>`**（撞 win32_api_stubs 内联）。
- 探针块进出**保存/恢复 GetLastError**（fopen("a") 成功会置 183，曾污染退出码链）。
- `TEMP PROBE` 统一命名；定位完成后 T5 统一拆除；注释文本内不得出现非终止用途的 `*/` 序列。

### 行为保真红线
- `decompiled.c` 是唯一语义真值；改写必须附 dc 行号证据，无法核验时登记 divergences 而非猜测。
- Ghidra 伪影（变参丢弃 / `unaff_` 寄存器残留 / CONCAT 噪声）既不能原样编译成 UB，也不能臆造补齐——
  标注 `TODO(verify)` 并回溯原版 EXE 反汇编定案。
- **禁止行为注入**：不给扫描器加 NUL 兜底、不补裸 WaitForSingleObject——先证明 dc 同位置确实如此。
- 分配器契约：分配返回 hdr+8（头 {size, magic 0xaa55}）；释放一律 `HeapFree(ptr-8)` ≡ `PECMD_FreeStrBuf`；
  ReleaseRefCount 后必须置 NULL 再 FreeStrBuf（防 double-free）。

### Windows 中文编码
- 中文文本写盘走 write/edit 工具，或 `$env:PYTHONUTF8='1'` 的 python；**不走 PowerShell 管道直喂**。
- PowerShell 管道必须 `[Console]::OutputEncoding=[Text.Encoding]::UTF8`。
- `.ps1` 脚本注释 ASCII-only（PS5.1 读无 BOM UTF-8 会解析崩）。

### 委派协议
- 子代理交付物 = 落盘文件（先写盘后回话，防收尾消息丢失）。
- 子代理证据必须绑定构建身份（DEPLOYED_BUILD/map 时间戳）；未绑定的 RVA 结论标"待版本核验"。

## 常用命令速查

```bash
# 构建（R23 起: build_msvc.py 全托管 UTF-8 链, bash 薄包装; 语法门加 syntax 参数）
bash tools/build_msvc.sh            # 全量构建 -> build/msvc/pecmd_msvc.exe (双绿门: exit 0 + [msvc_build] OK)
bash tools/build_msvc.sh syntax     # 语法门 cl /Zs
# 部署三连（构建绿后）—— post_build.sh 一站完成: symsnap 刷新 + 部署 + 身份戳
bash tools/post_build.sh . C:/pectest
# 对拍（单案 / 全量）
python harness/runners/run_case.py <case_id> --exe msvc --timeout 240
bash tools/run_corpus.sh            # 全量双跑 + diff
# 活体抓栈（挂死现场）—— 断点选址唯一来源: build/msvc/symsnap.txt (V1)
mcp attach_process <pid> → ~* k 30 → read_memory → end_session
```
> R23 注: 构建链已全面 UTF-8 化并零中文进 cmd 层（rsp/bat 全相对路径）;
> 工具脚本以 python 内嵌为稳（awk/perl 已于 R23 末随注册表 PATH 修正恢复, Git\usr\bin）。

## 权威文档与技能索引

| 主题 | 权威位置 |
|---|---|
| windbg MCP 病理 A–H + SOP + R20 版本锁定纪律 | `docs\WINDBG_MCP_ISSUES.md` |
| R18 断点误诊撤回档案 | `docs\WINDBG_MCP_REPRO_UPSTREAM.md`（勿发上游） |
| 技能库 | `.agents\skills\`：pecmd-build / vgate-live-debug / win-crash-triage / pecmd-semantics / msvc-compat |
| 文档总索引 | `docs\README.md` |
| 轮次账本（唯一权威） | 本目录 `HANDOVER_PROGRESS.md`（上级目录同名文件已废弃，仅存指针） |
| 语义地址速查 | `docs\provenance.map` + 技能 pecmd-semantics |
