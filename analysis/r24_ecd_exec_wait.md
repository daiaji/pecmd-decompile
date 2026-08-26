# R24 ECD EXEC 等待补齐静态对照

> 只读静态对照 · 唯一语义真值 `reference/decompiled.c`（dc）· 我方转写体根 `restored_bodies.c`（rb）
> 工单：ECD（ExecCmdDispatch = FUN_14000e26c）深水段 dc:11260+ ↔ rb:3700-3760 一带，核查
> ECD 内部是否缺少「等待子进程结束 / 读取退出码」逻辑；对应症状 064_exec_explicit exit=0 vs 2、
> harness 尾声 EXEC 通道部分案产物缺失。
> 交付物仅本文件；本次未改任何 .c/.h，未构建，未跑案。
> 行号锚定：所有关键锚线均经 `grep -n` 复核（§5 锚线表），其余区间行号来自 sed 分段直读
> （11140-11200 / 11200-11300 / 11420-11540 / 9630-9700 等），已与既有 R19 报告
> `analysis/e26c_exec_wait_rootcause.md` 的行号交叉一致（该文行号经锚线复核为准确，本文采信）。

---

## 0. 结论先行（TL;DR）

**缺口不存在。** ECD「等待子进程结束 + 读取退出码」逻辑在 msvc 侧完整存在，且与 dc 逐块同构：

- **等待** = MsgWaitForMultipleObjects 多句柄环（句柄数组末位 = hProcess，命中即退），
  全 ECD 函数内**没有任何 WaitForSingleObject 用于 EXEC 主等待路径**（唯一的信号等待 = `*ex`
  分支的 `WaitForSingleObject(hProcess, INFINITE)`，dc:10900 ↔ rb:3428，两侧均有）；
- **超时/轮询** = GetTickCount 截止预算（local_c28，每轮上限 1000ms，dc:11483-11492 ↔ rb:4028-4038）;
- **错误码写回** = `GetExitCodeProcess → local_db4 → *param_6`（dc:9687-9688 ↔ rb:2178-2179），
  参位映射双侧一致（dc:8092-8094 ↔ rb:588-590：默认 `&local_bc0`，否则 `param_6`）。

**最小补齐方案 = 无需粘贴任何 dc 块**（没有缺失块可粘）。残留动作仅为两个清理/复核项：
① rb:3835/4039 的 `TODO(verify)`（MsgWait 包装内联）可裁定 **resolve**（§3.1 给出依据）；
② `PECMD_CreateProcessW` 的 TEMP PROBE 已无活体价值，按纪律 T5 拆除（改代码，超出本只读工单 → 交主代理）。

**064/010-012 症状的静态归属**：不在 ECD 等待段（两侧俱在且同构）。exit=0 vs 2 的出口差异与
尾声产物缺失更可能落在 R19 已裁定的 DAT_14013a24f 早退门（未回归验证）与退出码传播链
（*param_6 之后 EXEC 动词/全局退出码如何消费），详见 §4。

---

## 1. dc 侧：ECD 实体定位与深水段控制流

### 1.1 实体定位

- 实体 = `FUN_14000e26c @ 0x14000e26c`，size=33864，dc:7726-11670（dc:11671 `}` 后即
  `FUN_1400166b4`，dc:11672 起）。FUNC_NAMES.md:331 命名 **PECMD_ExecCmdDispatch**（即 ECD）。
  签名 `_FILETIME __fastcall (longlong *param_1, _FILETIME param_2, longlong *param_3, _FILETIME param_4,
  uint param_5, _FILETIME *param_6, _FILETIME param_7, undefined8 *param_8)`（dc:7724-7727）。
- 函数内 CreateProcess 族调用点（grep 实证）：
  | 语义 | dc 行 | 目标 | 包装实体 |
  |---|---|---|---|
  | ShellExecuteExW 兜底 | 10485 | SHELLEXECUTEINFOW | 直调 |
  | CreateProcessWithLogonW | 10552 | 凭据启动 | 直调 |
  | `*ex` 自再入 + **WaitForSingleObject(hProcess,INFINITE)** | 10897-10900 | 控制台附加路径 | 直调 |
  | FUN_140101e04（=PECMD_CreateProcessW，FUNC_NAMES.md:805） | 10770 / 10968 | 外部程序兜底 | rb:3288 / rb:3498 同名调用 |
- g_flagA24F 相关读点 9 处（e26c 报告登记），其中 EXEC 等待门三处：dc:11286/11464/11482（rb:3826/4063/4027）。

### 1.2 深水段逐段概括（dc:11167 起，等待语义相关全程）

| 段 | dc 行 | 控制流概要 | 等待/错误码相关标注 |
|---|---|---|---|
| S1 句柄清理+GUI 就绪 | 11141-11172 | be0/d80/cf0/c88/c20 依次 CloseHandle；`param_5!=0‖bVar2` 时 `WaitForInputIdle(hProcess,5000)`（dc:11167） | WaitForInputIdle 仅 GUI 就绪等待，非进程退出等待 |
| S2 同步 ACK+脚本块 | 11174-11181 | `param_5&1` → FUN_14003e694(WriteSysAck)；`local_a10` → FUN_14004c0bc(ProcessScriptBlock) | — |
| S3 快跳守卫 | 11179 | `if ((local_df4==0) && (local_a98<1))` → 直落 LAB_140014f96（无句柄场景：置变量/走 f96 收尾，**不等进程**）；else → S4 | 快跳 = 无进程句柄的合法路径，两侧一致 |
| S4 句柄入列 | 11189-11199 | `local_970[d08]=hProcess; d08++`（dc:11189-11190）；关 hThread；`d48!=0` 时 FUN_1400195f0(1000ms 消息泵)+FUN_1400229f8(PatchRemoteWinExec)，hProcess==0 时按 PID OpenProcess 补 | **等句柄载体 = 数组末位 hProcess** |
| S5 快照+尺寸准备 | 11210-11260 | LAB_14001512a：快照 `local_e07=DAT_14013a24f`（dc:11210-11211）；`e00!=0`（dc:11214）→ 分配 0x100004 缓冲、`@`/`=` 前缀位处理、SetProcessWorkingSetSize、`local_df8=GetTickCount()`（dc:11249）、超时入参 `local_d04=1000` 下限、`local_db0=local_df8`（dc:11256） | 时间预算起点在此建立 |
| S6 捕获环（dc 捕获模式 `ddc&8`） | 11261-11421 | ConnectNamedPipe → ReadFile 初读 → LAB_1400153e3 环头：**早退门 dc:11286**（`DAT_14013a24f<1 && c80<2` → a5e）→ 预算计算 → `local_9a0=FUN_14001bbac(d08,970,0,d04,0x4ff)`（dc:11295，MsgWait 包装）→ 结果 0 读管道 → 行裁剪/回写变量/脚本块 → 5 预算 PeekMessage 泵（dc:11464）→ 回环 | 环退出 = a5e（早退/收尾）或 c69 汇合 |
| S7 行末处理 | 11420-11476 | CRLF/计数裁剪 + dbc ReadFile + 5 预算泵 + `goto LAB_1400153e3` | — |
| S8 非捕获等待环 | 11477-11518 | `e00 != 0xffffff9c`（dc:11477）→ `d18=c80; c48=c28`（11478-11479）→ LAB_140015a9b 环头：**早退门 dc:11482** → 预算 `DVar21=1000` 或 `min(1000, c28-(tick-t0))` 截止（11483-11492）→ `iVar52=FUN_14001bbac(...)`（dc:11494）→ **`if (DVar11-1 == iVar52) goto c69`（dc:11495，末句柄=hProcess 受信即退）** → 单字节管道 ReadFile（管道句柄在列时）→ `DAT_14013a24f>0 && PeekMessage!=WM_QUIT` 泵（11505-11516）→ goto 环头；`e00==0xffffff9c` → 直落 c69（dc:11518-11520） | **进程等待即此环**；无 WaitForSingleObject |
| S9 汇合收尾 | 9665-9688 | LAB_140015a5e（9665）：free b18；LAB_140015c69（9667）：三处 NUL 收口（9668-9670）、`local_db4=0`（9672）、**终止守卫 9673-9686**（`e00!=0xffffff9c && e07>0 && dc0!=1` 才 TerminateJobObject/TerminateProcess 或 `-force -exitcode=%d -tree *%d` 重执行 → **`GetExitCodeProcess(hProcess,&local_db4)`（dc:9687）→ `local_990->dwLowDateTime = local_db4`（dc:9688，写 *param_6）** | 错误码写回唯一路径 |
| S10 f96/`=0x2b9` 收尾/清理 | 11521-11670 | param_8 回写 hProcess、hProcess 置 0、bwinpeshl 自再入 `FUN_14000e26c(...,&DAT_14013c9c8)`（dc:11595，递归）→ 全句柄 CloseHandle 链 → `return _Var39;`（dc:11670） | 无等待 |

### 1.3 工单三问的直接回答（dc 侧）

- **是否等句柄**：是。唯一机制 = MsgWait 多句柄环（dc:11295/11494，经 FUN_14001bbac），
  命中判定 `DVar11-1 == iVar52`（dc:11495）。全函数 `WaitForSingleObject` 仅出现一次
  （dc:10900，`*ex` 再入路径），非 EXEC 主等待。**不存在 WaitForSingleObject(hProcess) 形式的长等**——这是原版设计，非缺失。
- **超时/轮询结构**：GetTickCount 截止预算 + 每轮 1000ms 上限（dc:11483-11492），无预算时
  （普通 EXEC 的 c28=0）按 1000ms/轮无限轮询直至进程受信；另有 5 条 PeekMessage 消息预算泵（dc:11464）。
- **错误码写回路径**：`GetExitCodeProcess(hProcess,&local_db4)` → `*param_6 = local_db4`
  （dc:9687-9688），终止守卫（dc:9673-9686）在等环之后、读码之前。

---

## 2. msvc 侧（rb）逐段对照与结构差异清单

### 2.1 段级映射（S 编号沿用 §1.2）

| dc 段 | dc 行 | rb 行 | 对照结果 |
|---|---|---|---|
| S1 | 11141-11172 | 3690-3708 | 同构（CloseHandle 链 + WaitForInputIdle(5000) rb:3707） |
| S2 | 11174-11181 | 3710-3716 | 同构（PECMD_WriteSysAck rb:3710；PECMD_ProcessScriptBlock rb:3714） |
| S3 | 11179 | 3719 | 同构（快跳守卫 `df4==0 && a98<1`） |
| S4 | 11189-11199 | 3729-3745 | 同构（追加 rb:3729-3730；WaitHandlesOrMessages(1000) rb:3736；PatchRemoteWinExec rb:3745） |
| S5 | 11210-11260 | 3750-3799 | 同构（快照 rb:3751；e00!=0 门 rb:3754；tick 起点 rb:3797 一带） |
| S6 | 11261-11421 | 3802-4017 | 同构（捕获早退门 rb:3826；MsgWait rb:3837；预算/泵 rb:3838-4017） |
| S7 | 11420-11476 | 4001-4017 | 同构 |
| S8 | 11477-11518 | 4022-4061 | 同构（门 rb:4022；装载 rb:4023-4024；早退门 rb:4027；MsgWait rb:4040；命中判定 rb:4041） |
| S9 | 9665-9688 | 2160-2187 | 同构（c69 rb:2162；db4=0 rb:2163；终止守卫 rb:2164-2183；**GetExitCodeProcess rb:2178；写回 rb:2179**） |
| S10 | 11521-11670 | 4065-4210+ | 同构（递归=E 自再入 rb:4106-4108 以 PECMD_ExecCmdDispatch 现名自调；清理链 rb:4113-4210+） |

### 2.2 结构性差异清单（本工单相关范围内全部条目）

| # | 差异 | dc 行 | msvc 行 | 性质裁定 |
|---|---|---|---|---|
| D1 | **MsgWait 调用形态**：dc 经包装 FUN_14001bbac（dc:16282-16289，size=45，`void` 透传壳，结果仅靠 EAX 残留回传；FUNC_NAMES.md:57 = PECMD_MsgWaitForObjects）；rb 在 e26c 两个调用点**直接内联** `MsgWaitForMultipleObjects` 并取返回值 | 11295 / 11494 | 3837 / 4040（rb 注释同处，含 TODO(verify)） | **等价，可 resolve**：rb 侧包装还原体（core_b1_remaining.c:8165）忠实保留 void 形态；原版编译器将包装内联后消费 EAX，内联直代即恢复机器级真实语义（同类 Ghidra EAX 泄漏伪影，别处同模式 dc:37355/103052 见 §4 follow-up） |
| D2 | **PECMD_CreateProcessW 返回形态**：dc 包装 FUN_140101e04（dc:158395-158402，size=103）`CreateProcessW(...); return;`（EAX 泄漏 BOOL）；rb core_b9_remaining.c:194-238 返回 `r != 0` 并做 GetLastError 透明化（R20 曾修 183 污染，fopen 探针前置） | 10770 / 10968（调用点） 158395-158402（本体） | rb:3288 / rb:3498（调用点） core_b9_remaining.c:194-238（本体） | **值等价**（CreateProcessW BOOL ≠0 ↔ r!=0）；探针为诊断遗留，按 T5 应拆除（改代码，交主代理） |
| D3 | **写回形态**：`local_990->dwLowDateTime = local_db4;` vs `*(uint32_t *)local_990 = local_db4;` | 9688 | 2179 | **等价**（FILETIME 首 DWORD = dwLowDateTime）；local_990 参位映射双侧一致（dc:8092-8094 ↔ rb:588-590：默认 &local_bc0，否则 param_6） |
| D4 | **命名映射噪声**：dc 的 FUN_1400637dc / FUN_140063424 / FUN_14005b104 等在 rb 为 PECMD_StrDupA / PECMD_GrowByteBuffer / PECMD_FreeStrBuf 等（捕获路径内部） | 11337-11360 等 | 3935-3945 一带 | 登记名映射（handoff 既有），语义未展开核对 → SKIP(功能性，非等待语义) |
| D5 | **早退门读点**：结构逐字一致（`DAT_14013a24f<1 && d18<2`）；但 rb 读点绑定的存储对象曾分裂（e26c 根因：unimplemented_stubs.c:981 int64_t=0 vs core_globals.c:63 g_flagA24F） | 11286 / 11482 | 3826 / 4027 | **R19 已裁定的行为差异源**，非结构缺失；修复（对象统一+WaitTickCount 直移）落地后需构建回归验证 |
| D6 | **dc:11595 自递归 vs rb:4106-4108 PECMD_ExecCmdDispatch 自调**：同名不同形（Ghidra 对递归写出 FUN_14000e26c，rb 用现名） | 11595 | 4107 | 等价（同一函数自再入，bwinpeshl DELAY 路径） |
| D7 | 函数尾清理：dc 双份 CloseHandle(d80/cf0/c88/c20…)链与 rb 同构；其中 `CloseHandle((HANDLE)0x0)`（dc:11596-11597 ↔ rb:4134-4136，c20 分支）为 **dc 自身伪影**，rb 忠实照抄 | 11596-11597 | 4134-4136 | 伪影一致（无行为影响；CloseHandle(NULL) 直接失败返回） |

> 未发现「缺整块 / 改写语义 / 顺序不同」级别的差异。S3-S9 全部段落在 rb 中以同序、同判定、同写回存在。

---

## 3. 结论与最小补齐方案

### 3.1 缺口裁定：不存在等待/读码缺口

- 等待环（S8，rb:4022-4061）、捕获环（S6，rb:3802-4017）、终止守卫+读码+写回（S9，rb:2160-2187）
  全部在 rb 中可逐行对应 dc。**没有需要「从 dc 粘贴到 rb」的缺失块**。
- 唯一登记未决点 D1（MsgWait 包装内联）经本报告读 dc:16282-16289 裁定为**语义正确**：
  包装体 `MsgWaitForMultipleObjects(...); return;` 无任何额外逻辑（无重试/无标志检查/无消息泵），
  返回值完全依赖 EAX 泄漏，故 rb 直接内联取返回值 = 机器级等价。建议把 rb:3835/4039 注释的
  TODO(verify) 更新为 verified（附本报告依据），属注释改动，可并入下次行为无关提交。

### 3.2 最小补齐清单（按优先级）

| 项 | 动作 | 位置 | 依赖符号可用性 |
|---|---|---|---|
| P1（已完成，需回归验证） | R19 修复：DAT_14013a24f 对象统一为 g_flagA24F + PECMD_WaitTickCount 直移（e26c 报告补丁①②③） | unimplemented_stubs.c / stubs_common.h / restored_bodies.c:7385+ | g_flagA24F 由 pecmd_defs.h:116 / core_globals.c:63 提供，stubs_common.h 宏别名方案与既有 T1b g_hHeap 先例一致 |
| P2（注释级，可并入） | rb:3835/4039 TODO(verify) → resolve | restored_bodies.c | — |
| P3（改代码，交主代理） | 拆除 PECMD_CreateProcessW 的 TEMP PROBE 块（R20 183 修复已完成，探针无活体价值；保存/恢复 LastError 逻辑保留） | core_b9_remaining.c:194-238 | — |
| P4（无需动作） | ECD 等待/读码段：**无粘贴需求** | — | — |

依赖核验（§工单要求「依赖符号是否已在 stubs_common.h/xproto.h 声明」）：
`PECMD_WaitHandlesOrMessages` stubs_common.h:1148 ✓、`PECMD_GrowByteBuffer` :1029 ✓、
`PECMD_ConvertLfToCrlf` :1176 ✓、`PECMD_AllocStringSlot2` :2860 ✓、`PECMD_CmdKill` :3243 ✓、
`PECMD_NotifyMainWindowRefresh` :1099/xproto.h:215 ✓；`PECMD_PatchRemoteWinExec` 由 rb:255 自声明 +
core_b2c.c:189 实现 ✓；`PECMD_MsgWaitForObjects` 包装还原体 core_b1_remaining.c:8165 ✓。
**结论：即使未来需要在这段补代码，依赖面已全部齐备，无新增桩需求。**

### 3.3 预期影响案（静态推演，非实测）

- **064_exec_explicit**（`EXEC =cmd /c echo ...>out\s64_exec.txt` + IFEX 断言）：
  ECD 等待段无差异 ⇒ `*param_6` 写回时机双侧一致，**不是**"缺等待导致 exit=0"。exit 0 vs 2 的出入口
  更可能落在 *param_6 之后的退出码传播链（EXEC 动词如何把 *param_6 汇入 PECMD 进程退出码 /
  PECMD_GetExitCodeGlobal @0x14005b7dc，core_init.c:70），以及原版在多个案中稳定产出 exit=2 的
  GetLastError 关联基线（R20 memfail 记录：061/065/WRITE 族原版=2）→ **待运行时探测定点**（V4：
  ECD 出口 rb:2179 抓 *param_6，比对原版同点），静态部分 SKIP(需 live 验证).
- **010-012 / 尾声 EXEC 通道**（run_case.py epilogue `EXEC =cmd.exe /c echo ...>vars.txt` 通道，
  run_case.py:100-102）：产物缺失 = 等环未跑完即被 harvest。两个静态候选：
  ① **R19 早退门**（若对象分裂修复未经回归，`DAT_14013a24f=0` → rb:4027 恒真 → 直落 c69 →
     GetExitCodeProcess 于子进程存活时 → 不等即返，文件未落盘）——与 e26c 根因同链，优先级最高；
  ② 捕获/回写路径（S6/S7）的变量处理差异（D4 SKIP 项）——可能性低。
  结论：**静态上 ECD 无缺块；010-012 缺产物的直接嫌疑仍是 R19 门的回归状态，而非本工单的"缺等待"。**

---

## 4. 附加核验与登记

### 4.1 地址身份自检（重要）

- 工单参考「PECMD_CreateProcessW @0x1407f0（symsnap）」：`build/msvc/pecmd_msvc.map:1237`
  显示 `0001:0013f7f0 PECMD_CreateProcessW 00000001401407f0 f core_b9_remaining.obj` —— **0x1407f0
  是 msvc 链接 RVA**（bp 选址用，V4 合法），**不是原版 dc 地址**。dc 侧 0x1401407f0 处无任何函数
  （grep `FUN_140140` 为空）；dc 真身 = `FUN_140101e04 @ 0x140101e04`（FUNC_NAMES.md:805）。两者不冲突，但对照 dc 时必须用 0x140101e04。
- 同理 `PECMD_ExecCmdDispatch 0x1ad440`（symsnap）= msvc 链接址；dc 址 = 0x14000e26c。

### 4.2 既有 e26c 报告行号交叉确认

本报告关键锚线（grep -n 实证）与 `analysis/e26c_exec_wait_rootcause.md` 完全一致：
dc:11189 追加 / dc:11286 捕获早退门 / dc:11482 二环早退门 / dc:11494 MsgWait / dc:9687 GetExitCodeProcess /
rb:3719 守卫 / rb:3729 追加 / rb:3826 / rb:4027 —— 无编号漂移。（本文 S 段中的区间行号另经 sed 直读复核。）

### 4.3 follow-up 登记（非本工单范围）

- F1：FUN_14001bbac 远程调用点 dc:37355 / dc:103052 同样消费 EAX 泄漏返回值；其 rb 侧
  （core_b2f.c:7888 以 `int64_t` 声明调用、core_b3r_h4.c:1638 以 `void` 声明调用）对返回值的使用
  形态不一致，若相关路径（TEAM WAIT / 循环等待）出现等待行为差异，优先复查此包装的返回消费。
- F2：064 exit=2 的原版来源需在 EXEC 动词出口（*param_6 消费点）定点（V4 探针或 TTD），本工单静态无法定案。

---

## 5. 锚线表（全部 grep -n 实证，可复核）

| 语义 | dc 行 | rb 行 |
|---|---|---|
| WaitForInputIdle(5000) | 11167 | 3707 |
| 快跳守卫 | 11179 | 3719 |
| 句柄追加 `970[d08]=hProcess` | 11189-11190 | 3729-3730 |
| WriteSysAck | 11174-11175 | 3710 |
| WaitHandlesOrMessages(1000) | 11186-11188（sed 11140-11200 读区） | 3736 |
| PatchRemoteWinExec | 11196（sed 读区） | 3745 |
| LAB_14001512a 快照 e07 | 11210-11211（e26c 表） | 3750-3751 |
| e00!=0 门 | 11214（e26c 表） | 3754 |
| 捕获早退门 | 11286 | 3826 |
| 捕获 MsgWait | 11295 | 3837 |
| 泵预算门 | 11464 | 4063（e26c:4053-4058 一带） |
| 非捕获门 `e00!=0xffffff9c` | 11477 | 4022 |
| d18/c48 装载 | 11478-11479 | 4023-4024 |
| 二环早退门 | 11482 | 4027 |
| 二环 MsgWait | 11494 | 4040 |
| 命中判定 `DVar11-1==iVar52` | 11495 | 4041 |
| LAB_140015a5e / c69 | 9665 / 9667 | 2160 / 2162 |
| 终止守卫 | 9673-9686 | 2164-2183 |
| GetExitCodeProcess | 9687 | 2178 |
| 写回 `*param_6` | 9688 | 2179 |
| 包装 FUN_14001bbac 本体 | 16282-16289 | core_b1_remaining.c:8165 |
| CreateProcessW 包装本体 | 158395-158402 | core_b9_remaining.c:194-238 |
| `*ex` 再入 WaitForSingleObject(INFINITE) | 10900 | 3428 |

## 6. 置信度与残留风险

- **等待/读码缺口不存在**：高（~95%）。S3-S9 双侧逐行同构，锚线全部 grep 实证；包装内联裁定有 dc 本体直读支撑。
- **064 exit=0 vs 2 的归因**：中（~60%）——已排除"ECD 缺等待"这一嫌疑（结构不存在缺口），
  但不能静态排除调用侧传播差异；F2 需 live 定点。
- 残留风险：① S6/S7 捕获路径内部函数映射（D4）未逐一展开语义（非等待语义，影响低）；
  ② D1 裁定基于 dc 包装体直读，若原版二进制该包装存在 Ghidra 未恢复的隐藏分支（size=45 极小，可能性低）需汇编复核；
  ③ 本报告未构建/未跑案，P1 修复的回归状态未验证（按纪律交主代理回归）。