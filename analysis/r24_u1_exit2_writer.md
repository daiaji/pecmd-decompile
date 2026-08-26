# R24 U-1 取证报告 — 原版"把 2 写进退出码缓存槽"的写入者（工单0 闭合）

> 活体取证子代理产出 | 单文件独占：本报告
> 日期：2026-08-27 | 代号：U-1（s17 §8 / r24_fail_triage.md 工单0）
> 方法：**cdb 10.0.29617.1000（WinDbg 附带的原生 cdbX64，非 windbg-MCP；本会话无 MCP 调试工具注册）** 硬件写断点（ba w8）+ 软件断点 + capstone 全量静态扫描，三轨互证。方案 1（MCP `debugger_watch_memory`）按"工具不可用→同构回退 cdb 原生断点=同一方案的另一载体"处理，**非 U-1 静态回退**——动态断点全部真实命中。

## 0. 身份与卫生

| 项 | 证据 |
|---|---|
| 被测物 | `C:\pectest\PECMD.EXE`，MD5=`4b97f08dd4b8391619d0ee7620c48a9c` = `reference\PECMD.exe`（同源 ✓）；PE：x64，ImageBase=0x140000000，**DYNAMIC_BASE=False（无 ASLR）**，dllchars=0x8000，入口 RVA=0x1081EC |
| 断点地址合法性 | 会话内 `lm m PECMD` → base=0000000140000000 固定，直接使用绝对地址 ✓ |
| 卫生 | 每个 cdb session 用完即退（-cf+EOF）；结束后 taskkill /IM PECMD.EXE /F、/IM pecmd_msvc.exe /F、/IM cdbX64.exe /F 三清，无残留（本次运行期间 windbg-mcp.exe 为环境既有守护进程，未触碰）；golden 复跑 005/013/016/031/061 与档案一致（2/2/2/0/0） |
| 只读纪律 | 未改任何 .c/.h、未构建、未 commit；仅新增本报告与运行现场脚本/日志（见 §7） |

## 1. 一句话裁定（闭合 U-1）

**把"2"写进退出码缓存槽的指令 = `PECMD_ProcessScriptBlock`（FUN_14004c0bc，PSB）尾部 LAB_14004c525 的存储：`0x14004C533  mov qword ptr [rax], rdx`（guard 0x14004C52A-2D：仅非零 `local_180` 才写）。**

- 槽目标在运行时 = **`rax = *(script-context+0x50)` = `0x14013D188`**（根脚本静态结构 0x14013D130 的 +0x58 域；`g_pExitCode`(0x14013D180) 在起脚本前即被改指向 0x14013D188）。
- **`0x14013CAF0`（s17 断言为"缓存"）在全部被测案中从未被写非零值**——InitProcessGlobals(dc:2220-2221) 只把它清零、并让 g_pExitCode 初始指向它（`0x14000521A and qword[14013caf0],0 ; 0x140005229 mov [14013d180],rax`），随后即被运行机制改指 0x14013D188；进程退出时 `PECMD_GetExitCodeGlobal`(0x14005B7DC) 读 `*(0x14013D180)` → `*(0x14013D188)` → ExitProcess。dynamic 终值三槽实测：`d180=0x14013d188, d188=2, caf0=0`。
- **值 2 的来源分簇**（黄金实证→动态实证）：
  1. **自带成功=2 的动词**：WRITE（FUN_14009f070，dc:44914/100374 区）顶层返回 2 → PSB 尾直接写（005：1 次存储）；FILE 族（010/012：尾写 + 终值 2 实测）。
  2. **IFEX/FIND 条件为假**：真值机 = 处理器（共享体 0x140034788）把选中分支**改写成前缀 "ELSE " 的行**再嵌套重入 PSB（0x1400357E5 调用），**"ELSE <分支>" 行执行返回 2** → 内层尾写 2 → 外层（处理器返回值=子执行结果，child→parent 回传）再写 2（013/016 各 2 次存储实测；013 实为文件缺失=假分支！）。
  3. **IFEX/FIND 条件为真**：直接跑分支行（无前缀），返回=分支命令自身返回值（ENVI→0、EXEC→0、WRITE→2）——**031/061 golden=0 的真相：其条件为真、分支为 ENVI，返回 0，全程 0 次尾写（动态实测 stops=0）**。r24 的 U-5"语义冲突"不存在。

## 2. 静态证据（capstone，.text 全量）

| 地址 | 指令 | 语义 | 备注 |
|---|---|---|---|
| 0x14000521A | `and qword ptr [rip+X]→0x14013caf0, 0` | InitProcessGlobals 清缓存(dc:2220) | 对 caf0 唯一直接写 |
| 0x140005222 | `lea rax,[0x14013caf0]` | 缓存地址 | caf0 唯一直接读/取址 |
| 0x140005229 | `mov [0x14013d180], rax` | g_pExitCode := &caf0(dc:2221) | 对 d180 唯一直接写 |
| 0x14004C52F/533 | `mov rax,[rdi+50h]` → `mov [rax],rdx` | **★写入者**（LAB_14004c525，dc:45139-45141） | PSB 尾；仅非零 |
| 0x14004D998-9FF | `mov rax,[rdi+50h]; mov rdx,[rax]; mov [rax],rdx` (+0xd0) | EXIT 动词分支写缓存(dc:44935-38) | 本案未用（无 EXIT 行） |
| 0x14004DC90/9C/9F | `mov [rax],rcx` 等 | EXIT 链式写(dc:45027) | 同上 |
| 0x14005B7DC | `mov rax,[0x14013d180]; mov rax,[rax]; ret` | **PECMD_GetExitCodeGlobal 读缓存**(dc:54066) | 退出消费点 |
| 0x14004DEFD / 0x14004CC92 | `call 140035b08` (r8b=1) / `call 140035b24` (r8b=0) | PSB IFEX/FIND 分支入口 | thunk→共享体 |
| 0x140035B08/0x140035B24 | thunk：`xor/mov r8b,1; call 140034788` | IFEX(=1)/FIND(=0) 判别入体 | r8b=模式 |
| 0x1400357E5 | `mov rcx,r12; ...; call 14004c0bc` | **共享体嵌套重入 PSB 跑分支行** | 分支行=param_2 |
| 0x1400357C0-7EA | `mov rdx,[rsp+50h]…call 14004c0bc` | 分支行选择/前缀组装 | false 分支带 "ELSE " |

静态扫描复核 s17：指向 0x14013caf0 的 RIP-rel 仅 2 处（AND 写 + LEA），指向 0x14013d180 的 2 处（init 写 + 退出读）；0x14013d188 **零直接引用**（纯指针/派生寻址）——故写入侧必经寄存器中转 [reg+0x50] 型，与 s17 §8 判断一致；但 s17 的"缓存=0x14013caf0"定位**需修正为 0x14013d188**（见 §1）。

## 3. 每案动态证据（运行现场原始记录，日志在 C:\pectest\u1logs\）

约定：`存储数` = PSB 尾写断点命中次数（每次=槽上发生一次非零写入）；终值三槽 = 退出读断点（0x14005B7E3）时 `dq 14013d180/14013d188/14013caf0`。

| 案 | 脚本要点 | golden | 存储数 | 命中断点 RIP / 值 | 嵌套分支行文本（du 实测） | 终值 d188 / d180→ | 结论 |
|---|---|---|---|---|---|---|---|
| 005_write_file | WRITE %F%,line1（F=C:\pectest\out\demo.txt） | 2 | **1** | 0x14004C533，rdx=**2**，rax=0x14013d188 | —（无分支） | 2 / →0x14013d188 | WRITE 返回 2 直写 |
| 010_file_copy | FILE %F%=>copy.txt | 2 | 1 | 0x14004C533（值 2） | — | 2 / →0x14013d188 | FILE 成功返 2 直写 |
| 012_file_missing_src | FILE nofile=>x2.txt | 2 | 1 | 0x14004C533（值 2） | — | 2 / →0x14013d188 | FILE（源缺失）仍返 2 |
| 013_ifex_file | IFEX out\demo.txt, ENVI R=exists! ELSE ENVI R=missing | 2 | **2** | 0x14004C533 ×2 均 rdx=2（内层 rsp=14eca0 / 外层 rsp=14efe0） | **"ELSE ENVI R=missing"** | 2 / →0x14013d188 | demo.txt **实为缺失**（见 §6-2 WRITE 不落盘）→ 假分支→"ELSE 行"→2 |
| 016_find_str | ENVI S=hello world; FIND $%S%=world, ENVI R=found! ELSE ENVI R=nofind | 2 | **2** | 0x14004C533 ×2 均 rdx=2 | **"ELSE ENVI R=nofind"** | 2 / →0x14013d188 | **等值比较**："hello world"≠"world"→假→ELSE→2（"命中"标签是误读） |
| 031_set_assign | SET A=pecmd; FIND $%A%=pecmd, ENVI R=setok! ELSE ENVI R=setbad | 0 | **0** | —（无尾写） | **"ENVI R=setok"**（真分支） | 0 / →0x14013d188 | 等值："pecmd"=="pecmd"→真→ENVI 分支→0；SET 本身返 0 |
| 061_logs_smoke | LOGS out\s61.log; IFEX out\s61.log, ENVI R=logs_open! ELSE …; ENVI BODY_DONE=YES | 0 | **0** | —（无尾写） | **"ENVI R=logs_open"**（真分支） | 0 / →0x14013d188 | LOGS 创建了 s61.log→IFEX 真→ENVI→0；LOGS 自身返 0 |
| 001_envi_smoke | ENVI 行（对照组） | 0 | **0** | — | — | 0 | 顶层 ENVI 返 0（无写） |
| 探针 p1/p2 | (SET|ENVI) A=pecmd; FIND $%A%=pecmd, EXEC-echo…! ELSE EXEC-echo… | 0 | —（黑盒） | 文件=MATCH | — | — | 等值=真→EXEC 分支→0；SET/ENVI 变量均可展开 |
| 探针 p3 | LOGS s61x.log; IFEX 之, EXEC-echo EX! ELSE … | 0 | — | 文件=EX、s61x.log 已建 | — | — | IFEX 真→分支 EXEC→0 |
| 探针 p4/p5 | IFEX (缺失), EXEC-echo…! ELSE EXEC-echo… | 2 | — | exit=2（ELSE 路径） | — | — | 假→ELSE 行→2（分支 EXEC 文件未产生，见 §6-1） |
| 探针 p8 | ENVI S=hello world; FIND $%S%=world,… | 2 | — | exit=2 | — | — | 与 016 同判：假（等值） |
| 探针 p9/p10 | WRITE 多种形态 | 2 | — | exit=2，**无任何落盘文件** | — | — | 见 §6-2 |

注：013/016 的"内层/外层"两次写 = 共享体内部嵌套 PSB 尾写（分支行）+ 外层 PSB 尾写（处理器返回值=子结果回传）；005/010/012 无分支故单次。控制组 031/061/001 = 0 次写与 golden=0 精确互证，**不存在任何"清 0 覆盖"机制**（s17 曾怀疑的 reset 无需存在）。

## 4. 语义发现（附带可靠修正，均有动态行文本/文件级实证）

1. **`FIND $X=Y` 是等值比较（非子串查找）**：016 "hello world" vs "world" → 假（实测走 ELSE）；031/043 等值 → 真。r24 表中"016 子串命中"、043"空格值"等标签需改写；F 簇借道案 046/047/063/064 的 golden=2 大概率同样来自"假→ELSE→2"而非"命中→2"。
2. **IFEX/FIND 条件为假时执行 "ELSE <分支>" 前缀行，该行返回 2**（非分支命令自身返回值）；条件为真时执行裸分支行，返回分支命令自身值。处理器（0x140034788 共享体，4312B 待续译）的返回值=嵌套执行结果（child→parent errorlevel 回传语义，dc:103263-269 对应）。
3. **EXIT 动词另有写缓存路径**（0x14004D9FF、0x14004DC9C，dc:44937/45027），本案语料无 EXIT 行，未触发（登记备用）。

## 5. 对 C1（IFEX/FIND 去桩）的返回值影响裁定 ★

**否决"IFEX/FIND 恒返 2"的估值前提；正确语义 = 条件分派 + 分支行回执行：**

- **条件真** → 执行真分支行（裸文本）→ 返回值 = 分支命令自己的返回值（ENVI/EXEC→0；WRITE/FILE→2）→ PSB 尾按非零写槽。此时 **handler 返回 0 是合法且必要**（对照案 031/061 必须保持 0，否则引入新 FAIL）。
- **条件假** → 组装并执行 "ELSE <假分支行>" → **该行返回 2** → handler 回传 2 → 槽=2（013/014/016/017 与 F 簇借道案的 golden=2 来源）。
- **`FIND $A=B` 判定按等值（大小写？待定）实现**；当前 msvc 侧若按子串实现，输出将系统性相反。
- 去桩工作量由"恒返 2"变更回"共享体完整移植（分支选择+ELSE 前缀+嵌套执行+回传）"，但这是**行为语义必需**，不能简化为常量；可先做"等价实现"：handler 内完成 (a) 条件求值 (b) 真→执行分支并回传其值 (c) 假→执行 ELSE 前缀行并回传 2——以原版四案（013/016/031/061）动态行文本为验收锚。
- 联动：031/061 在 C1 修复后必须**继续 golden=0**——本报告给出手写验收判据（真分支 ENVI→0、无尾写）。

## 6. 未闭合点与附带发现（不虚构语义）

- **6-1（U-1 分支 SKIP）**：p4/p5（假→ELSE 分支为 `EXEC =cmd /c echo …>…`）exit=2 但 EXEC 未产出文件——"ELSE-EXEC"行的参数传递/重定向解析异常，需另案（影响 064_exec_explicit 借道判定）。登记 SKIP(未逐指令复核 ELSE-EXEC 行解析)。
- **6-2（新发现 U-7 候选）**：**原版 WRITE 动词返回 2 但从不创建目标文件**——p6/p7/p9/p10 共 4 种语法形态（path 裸/引号内容/utf8/变量路径/无内容）+ 005 全程实测 `out\demo.txt` 从未存在。推论：语料 005-009/041/044/045 的 golden 只约束 exit（=2 约定码），**从未验证落盘**；msvc 侧 WRITE 的文件创建行为无 golden 锚（对拍盲区）。已在 dc:FUN_14009f070(dc:99499+) 见入口对参数 "-UNI" 前缀的判别（uVar11=0x4b0），文件 IO 段未逐行对译——登记 SKIP(进一步归因)。
- **6-3（细化修正 s17/S20）**：S20 §19 主张"g_pExitCode 指向堆缓存"与实测不符：运行时 `0x14013d180 → 0x14013d188`（静态 .data），非堆；0x14013caf0 仅作 init 期临时指向。
- **6-4（U-2 联动）**：013/016 的 ENVI R=xxx 分支在本夹具不可观测（vars 为名字模板），本报告以"嵌套分支行文本（du）+存储序列"取代，验收建议直接采用本报告 §3 行文本锚。

## 7. 产物与清理

- 报告：`analysis/r24_u1_exit2_writer.md`（本文件）
- 运行现场日志（证据，保留在 C:\pectest，均绑定本日原版运行）：`u1logs\*.log`（5 案 v5 断点日志、013/016/031/061/001 入口追踪日志、010/012 日志）；脚本 `u1_dbg*.txt`、`u1_scan*.py`、`u1_digest*.py`、`probes\p*.pecmd`（均为现场取证件，非仓库源码）。
- 进程卫生：PECMD.EXE / pecmd_msvc.exe / cdbX64.exe 三清无残留（taskkill 实证）；未触碰 windbg-mcp.exe 环境守护进程；未改仓库源码、未构建、未 commit。