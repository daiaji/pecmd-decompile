# R24f 探针全量台账（T5 拆除工单 · 只读清点）

> 工单：为 T5 探针拆除建立全量台账。**作业纪律：只读** —— 除本文件外未修改任何文件；
> 未构建、未执行 git 操作、未运行任何 PECMD/测试。全部结论来自 grep（ripgrep 语义）+ read 上下文核对。
>
> 范围：`src/`、`restored_bodies.c`、`win32_api_stubs.c`、`crt_shims.c`、`unimplemented_stubs.c`、`include/` 的 C 源码。
> 结论分级（语义风险）：**低**＝只读 fprintf、无数据流/流程改变；**中**＝缺 GetLastError 保存/恢复、或位于 GetLastError 消费者/退出码链附近（r19b 已实证 fopen("a") 成功置 183 污染退出码链的先例）；**高**＝改写业务变量或流程（本轮未发现）。
> 参考规范：AGENTS.md 探针纪律 V4（手写 CRT extern、禁 stdio.h、进出保存/恢复 GetLastError、`TEMP PROBE` 统一命名、T5 统一拆除）。

---

## 1. 搜索命中总览（全仓 grep → 仅计 C 源码）

| 搜索项 | 全仓命中 | C 源码命中 | 说明 |
|---|---|---|---|
| a) `TEMP PROBE` 注释标记 | 79 | **61** | 含 `#include` 行注释、CRT extern 原型块注释、文件头/函数注释、死代码注释；rest 为 AGENTS.md / analysis / docs / REVIEW / HANDOVER |
| b) `memfail.log` | 73 | **fopen 语句 39 处**（另 `PROBE_STEP` 宏展开 9 个调用点 → 潜在写盘点合计 **47**） | 其余为 docs/analysis/HANDOVER 引用 |
| c) `[ELC]` | 8 | **3**（均 core_b2f.c） | 其余 5 处为 HANDOVER_PROGRESS / docs 记录 |
| d) `fopen(` path 含 `C:\pectest` 兜底 | 120(含 pectest) | 与 b) 完全重合，**无漏网探针** | 39 行 fopen 全部指向 memfail.log |
| 附加 1) `SG_Probe` | 31 | 定义 1（core_scriptdep.c）+ 调用 **7**（core_scriptdep×3、core_script×2、core_b3e×2） | R20-A CALL 裸路径探针网，有 LastError 恢复 |
| 附加 2) `P8_Probe` | 31(同上 grep) | 定义 1（unimplemented_stubs.c）+ 调用 **8**（6 个桩转发函数） | S8 系列，**无** LastError 恢复 |
| 附加 3) `S7_MARK` | — | 定义 1 + 调用 **10**（core_script2.c） | [S7] bisect 标记 |
| 附加 4) 裸探针 | — | **1**：core_b3r_h3.c:2354（`[L31@2349]`，无 TEMP PROBE 标注） | 规范违规项 |
| 附加 5) 死代码 | — | **1**：core_b9_remaining.c:8-11（CRT extern 残块，全文件已无引用） | [CPW] 探针 R24 已拆（r24_ecd_exec_wait.md），extern 未清 |
| include/、win32_api_stubs.c、crt_shims.c | — | **0 命中** | 无需处理 |

**分类小计（C 源码）**：memfail 探针块 38（写盘点 47 含宏展开）×低-中风险；[ELC]×3；SG×7 调用；P8×8 调用；[S7] bisect×10+1 内联；OOM 探针×5；支撑代码（CRT extern / `#include <stdio.h>`）×13（其中 1 处已死）；裸探针×1。

---

## 2. 台账表（文件:行 | 类别 | 语义风险 | 建议）

表格按文件分组；「函数」列给出所在函数与 corpus 覆盖判断（主链=corpus 001-065 常规执行路径）。

### 2.1 restored_bodies.c（全部为 memfail 探针，均只读 fprintf、均无 LastError 恢复）

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :4-7 CRT extern 原型块 | 支撑 | 文件级 | 低（无副作用） | 随本文件探针清零后删除 |
| :261-267 `[ECD]` | memfail 纯日志 | PECMD_ExecCmdDispatch 入口 / 主链 | 中（高频；缺 LastError 恢复；只读 line 指针） | **可拆（纯删）** 批1 |
| :5907-5913 `[PSB]` | memfail 纯日志 | PECMD_ProcessScriptBlock 入口 / 主链 | 中（每行 PSB 触发；缺恢复） | **可拆（纯删）** 批1 |
| :6307-6314 `[BP]` | memfail 纯日志 | PSB 内 bare-path 判定 / 主链 | 中（缺恢复） | **可拆（纯删）** 批1 |
| :7178-7186 `[WB]` | memfail 纯日志 | PSB 尾部 verb 分发 LAB_14004c525 / 主链 | 中-偏高（缺恢复；位于动词返回值写 `*(script+0x50)` 退出码链之前；r19b 183 先例同型） | **需人工复核**，与退出码链对拍同批拆（批1末） |
| :7493-7496 `[AWS]` | memfail 纯日志 | PECMD_AppendWideStr 入口 / 主链 | 中（高频；缺恢复） | **可拆（纯删）** 批1 |
| :7857-7867 `[DEB ent]` | memfail 纯日志 | PSB 行循环入口（S9'） / 主链 | 中（高频；缺恢复） | **可拆（纯删）** 批1 |
| :8031-8037 `[DEB line]` | memfail 纯日志 | PSB 行分发到 PSB / 主链 | 中（缺恢复） | **可拆（纯删）** 批1 |

### 2.2 unimplemented_stubs.c（S8 系列 P8_Probe，均无 LastError 恢复）

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :34-43 `P8_Probe` 定义 | P8 探针 | 文件级 | 中（缺恢复；违反 V4） | **可拆**；门A 通过后批量删（批3）；桩替换时随真体落地自然移除 |
| :180/182 | P8 | PECMD_ExpandCommandLine 包夹 / S8 诊断链 | 中（缺恢复；只读） | 同上 |
| :185/187 | P8 | PECMD_ExpandVarsRecursive 包夹 / S8 链 | 中 | 同上 |
| :264 | P8 | PECMD_CopyPathToken / S8 链 | 中 | 同上 |
| :516/526 | P8 | FUN_140063888（SCWin/StrCopyW）/ S8 链 | 中 | 同上 |
| :570 | P8 | FUN_140003a20（RunCmdLine）/ S8 链（当前行执行器为桩，少触发） | 中 | 同上 |

### 2.3 SG_Probe 网（R20-A CALL 裸路径；SG_Probe 自身**有** LastError 保存/恢复，合规）

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| core_scriptdep.c:212-226 定义 | SG | 文件级 | 低（有恢复） | 纯删（批3） |
| core_scriptdep.c:229/240/251 | SG | PECMD_InvokeSubRoutine（invoke-enter/pos/ret）/ CALL/SUB 路径（corpus 覆盖有限） | 低 | 保留至 CALL/SUB 挂死死点结论复核完成（批3） |
| core_script.c:78-87 | SG | 段扫描环 seg-top/seg-scan-done（H3 失 NUL 停机检测） / 主链 | 低 | 同上（批3） |
| core_b3e.c:103-107 | SG | PECMD_DispatchCommand CALL 裸路径（dispatch-exec/ret） / 主链 | 低 | 同上（批3） |

### 2.4 src/lang/core_scriptrun.c

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :21 `#include <stdio.h>` | 支撑 | 文件级 | 低 | 随拆 |
| :229-236 `[S10]` | memfail 纯日志 | srx_ExecuteScriptFile（LOAD 装载）/ 主链 | 中（缺恢复） | **可拆（纯删）** 批1 |
| :269-272 | memfail | PECMD_RunCommand 入口 / 主链 | 中（每行触发） | **可拆** 批1 |
| :283-286 | memfail | PECMD_RunCommand SrParsePrefix 前 / 主链 | 中 | **可拆** 批1 |
| :601-608 `[S10] drive-hit` | memfail | PECMD_RunCommand 逐盘符探测分支 / 低频 | 中 | **可拆** 批1 |
| :701-709 `[RCCLEAN]` | memfail | PECMD_RunCommand 收尾（FreeStrBuf 前）/ 主链 | 中（缺恢复） | **可拆** 批1 |

### 2.5 src/lang/core_script2.c（[S7] bisect 标记簇 + 入口/出口探针）

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :47-49 文件头注释 | 文档 | — | 低 | 随拆更新 |
| :51 `#include <stdio.h>` | 支撑 | — | 低 | 随拆 |
| :128-136 `S7_MARK` 定义 | [S7] bisect | 文件级 static | 中（缺恢复） | 纯删（批3） |
| :195-196 | memfail | PECMD_RunStartupScript 入口 / 启动链=主链 | 中 | **可拆** 批1 |
| :373-383 bisect 内联块 | [S7] bisect | PECMD_RunStartupScript 模式判定后（只读 g_cmdlineMode/autoFirst/autoRem/scriptText）/ 启动链 | 中 | 可拆（批3，与启动链 AV 结论同批） |
| :393 `br-mode0` | [S7] bisect | 同上分发树 / 启动链 | 中 | 批量 10 处 S7_MARK 调用点（393/622/640/644/646/668/674/676/684/686）一并拆；**tail-7(686) 与尾段 LOGS 收尾 AV 时序证据相关，建议留到启动链 AV 定案**（批3） |
| :698-701 | memfail | PECMD_RunStartupScript 出口 / 启动链 | 中（缺恢复） | **可拆** 批1 |

### 2.6 src/lang/core_execmain.c

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :45 `#include <stdio.h>` | 支撑 | — | 低 | 随拆 |
| :96-103 `[RSTI]` | memfail 纯日志 | PECMD_RunScriptText 入口 / 主链 | 中（缺恢复） | **可拆** 批1 |
| :471-478 `[RSTX]` | memfail 纯日志 | PECMD_RunScriptText 出口（RefCountRelease 前）/ 主链 | 中 | **可拆** 批1 |

### 2.7 src/app/core_main.c

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :32 `#include <stdio.h>` | 支撑 | — | 低 | 随拆 |
| :76-79 | memfail | PECMD_MainW heap-init / 每个进程必经 | 中 | **可拆** 批1 |
| :89-92 | memfail | PECMD_MainW InitEnvVars 前 / 必经 | 中 | **可拆** 批1 |
| :172-179 `[MAINLOOP]` | memfail | PECMD_MainW 主循环前 / 必经 | 中 | **可拆** 批1 |

### 2.8 src/app/core_init.c

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :17 `#include <stdio.h>` | 支撑 | — | 低 | 随拆 |
| :88-89 `PROBE_STEP` 宏定义 | memfail 宏 | PECMD_InitEnvironmentVars / 必经 | 中（缺恢复；每步触发） | **可拆**：宏定义 + 9 个调用点（90/92/94/96/98/100/102/115/118）一并删，批1 |

### 2.9 src/lang/core_exec2.c

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :38-41 CRT extern 原型 | 支撑 | — | 低 | 随拆 |
| :189-198 `[OFH]` | memfail 纯日志 | PECMD_OpenFileHandle / 主链 | **低（有 GetLastError 保存/恢复，V4 合规）** | **可拆** 批1（删除块即连带删除 le_restore 保护，行为回归原文） |

### 2.10 src/lang/core_exec4.c

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :23 `#include <stdio.h>` | 支撑 | — | 低 | 随拆 |
| :221-235 `[REL]` | memfail+HeapValidate | PECMD_RefCountRelease 释放分支 / 主链 | 中-偏高（临界区内 HeapValidate 整堆校验成本高；调 _get_heap_handle；缺恢复；无流程改变） | **可拆（纯删）** 批1；块内局部 extern `_get_heap_handle` 一并删 |
| :268-277 `[ADOPT]` | memfail+HeapValidate | PECMD_AdoptRefCountedString / 主链 | 同上 | **可拆（纯删）** 批1 |

### 2.11 src/runtime/core_var.c、src/kernel/core_string.c（OOM 探针簇）

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| core_var.c:17 `#include` | 支撑 | — | 低 | 随拆 |
| core_var.c:65-72 | OOM 探针 | PECMD_AllocWStringBuffer 分配失败循环 / 罕见路径 | 低（只读；仅分配失败触发；缺恢复但无下游 GetLastError 消费者风险低） | **可拆（纯删）** 批0 |
| core_string.c:25 `#include` | 支撑 | — | 低 | 随拆 |
| core_string.c:51-58 | OOM site=1 | PECMD_HeapRealloc 分配失败循环 / 罕见路径 | 低 | **可拆** 批0 |
| core_string.c:71-78 | OOM site=2 | PECMD_HeapRealloc ReAlloc 失败 / 罕见路径 | 低 | **可拆** 批0 |
| core_string.c:136-143 | OOM site=3 | PECMD_StrDupAlloc / 罕见路径 | 低 | **可拆** 批0 |
| core_string.c:211-219 | OOM 弹窗探针 | FUN_1400630D0（每次 OOM 弹窗触发）/ 罕见路径 | 低 | **可拆** 批0 |

### 2.12 src/commands/core_b2f.c（[ELC] 决策探针簇，T5 保留项）

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :1862-1865 CRT extern 原型 | 支撑 | — | 低 | 随 [ELC] 拆除 |
| :2672-2683 `[ELC] s=…` | **ELC 决策探针** | PECMD_EvalLoopCondition 入口（读 script+0xd/0xda/cond/flags）/ 031/061 深水区主链 | 中（只读；缺 LastError 恢复；IF 求值每步触发） | **清单式保留**：依赖 R24e 031/061 进展（真判假数据唯一来源），031 定案后拆（批2） |
| :3603-3612 `[ELC] cmp i7=…` | ELC 决策探针 | PECMD_EvalLoopCondition 等值比较操作数 / 同上 | 中（只读） | 同上，**清单式保留**（批2） |
| :4434-4443 `[ELC] ret=…` | ELC 决策探针 | PECMD_IfexFindExecutor 调 032dc4 返回值 / 同上 | 中（只读） | 同上，**清单式保留**（批2） |

### 2.13 src/commands/core_b3r_h3.c

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :45-48 CRT extern 原型 | 支撑 | — | 低 | 随拆 |
| :1831-1834 `[WIN] entered` | memfail 纯日志 | PECMD_WriteFileEncoded 入口 / 主链（WRITE 动词） | 中（缺恢复） | **可拆** 批1 |
| :2354 `[L31@2349]`（裸探针） | memfail 纯日志 | PECMD_WriteFileEncoded 内 / 主链 | ⚠ 中（**无 TEMP PROBE 标注 + 无 LastError 恢复**，规范违规；只读 LVar31） | **需人工复核**：确认后按裸探针拆除并补登记（批1 内单独处理） |
| :2638-2644 `[WRET]` | memfail 纯日志 | PECMD_WriteFileEncoded 返回路径 | 中（缺恢复） | **可拆** 批1 |
| :2648-2654 `[WRET2]` | memfail 纯日志 | PECMD_WriteFileEncoded 全路径出口 | 中（缺恢复） | **可拆** 批1 |

### 2.14 src/misc/core_b9_remaining.c（死代码）

| 探针点 | 类别 | 所在函数 / corpus | 语义风险 | 建议 |
|---|---|---|---|---|
| :8-11 CRT extern 残块 | **死代码** | 文件级（[CPW] R24 已拆，199-200 注释证实；grep 全文件已无 fopen/fprintf/fclose 使用） | 低（无引用） | **立即拆（纯删）** 批0 |

---

## 3. 分类统计

| 类别 | 数量 | 拆除难度 |
|---|---|---|
| memfail 探针块（fopen 写盘点，含 PROBE_STEP 展开） | 47（39 语句位置） | 纯删为主（13 处缺 LastError 恢复需先核对下游消费者；[WB] 需复核） |
| [ELC] 决策探针 | 3 | **清单式保留**（依赖 R24e 031/061 深水区定案） |
| SG_Probe 探针（R20-A CALL 网） | 定义 1 + 调用 7 | 纯删（但建议 CALL/SUB 结论复核后拆） |
| P8_Probe 探针（S8 诊断链） | 定义 1 + 调用 8 | 纯删（随桩体替换 / 门A 通过后） |
| [S7] bisect（S7_MARK 簇） | 定义 1 + 调用 10 + 内联 1 | 纯删（tail-7 留到启动链 AV 定案） |
| OOM 探针 | 5 | 纯删（批0） |
| 支撑代码（`#include <stdio.h>` + CRT extern 块） | 13（含 1 死代码） | 随各文件探针清零后删（b9r 批0） |
| 裸探针（缺标注） | 1（b3r:2354） | 先人工复核 |
| **合计 TEMP PROBE 标记（C 源码）** | **61** | — |

---

## 4. 推荐拆除顺序（建议批次）

- **批 0（零风险，立即可拆）**：core_b9_remaining.c:8-11 死 extern；OOM 探针 5 处（core_string.c:51/71/136/211、core_var.c:65）。
- **批 1（无风险纯 fprintf 块，先拆）**：全部 memfail 常规块 —— rb[ECD/PSB/BP/DEB×2/AWS]、csr[S10×2/RunCommand×2/RCCLEAN]、cs2[StartupScript enter/done]、cem[RSTI/RSTX]、cma[MainW×2/MAINLOOP]、cin[PROBE_STEP 宏+9 调用]、ce2[OFH]、ce4[REL/ADOPT]、b3r[WIN/WRET/WRET2 + 2354 裸探针复核]。
  - ⚠ 拆除前逐点核对：该块下游 3-5 语句内有无 GetLastError 消费者（r19b 183 污染同型先例）；[WB]（rb:7178）单独放批 1 末尾，与退出码链对拍复核同批。
- **批 2（清单式保留，031 定案后拆）**：`[ELC]`×3（core_b2f.c:2672/3603/4434）—— R24e 031/061 深水区「真判假」结论依赖其数据；031 定案前**只清点不动**；CRT extern（b2f:1862-1865）随其后清理。
- **批 3（依赖门A/CALL/SUB 结论）**：SG_Probe 网（core_scriptdep.c:212-251、core_script.c:78-87、core_b3e.c:103-107）；P8_Probe 网（unimplemented_stubs.c，随桩体替换自然消失）；[S7] bisect 簇（core_script2.c，tail-7 等启动链 AV 定案后）。
- **支撑代码收尾**：每个文件最后一处探针拆除后，删除该文件 `#include <stdio.h>` 与 CRT extern 原型块（csr:21、cs2:51、cem:45、cma:32、cin:17、ce4:23、cv:17、csg:25、rb:4-7、ce2:38-41、b3r:45-48）。

## 5. 注意与风险提示

1. **GetLastError 纪律（AGENTS.md V4）**：仅 [OFH]（core_exec2.c:189）与 SG_Probe 合规；其余 memfail 写盘点均无保存/恢复 —— 拆除本身即消除污染源，但拆前必须确认各点下游 GetLastError 消费者（[WB] 尤其，位于退出码链）。
2. **不改变业务语义的证据**：本轮逐一核对 ±15 行，全部探针为只读 fprintf（读取局部变量/参数/内存字节），无任何对业务变量或控制流的改写；唯一流程相关点是各函数入口/出口的时序观测，拆除不影响函数间数据流。
3. **解码问题**：restored_bodies.c / core_b3r_h3.c / core_b2f.c 等文件注释为 GBK 字节被按 UTF-8 读出的 mojibake，拆除时整块删除即可（不改文本，保留证据原貌）。
4. **[L31@2349] 裸探针（core_b3r_h3.c:2354）**：缺 TEMP PROBE 命名标注，是 d) 兜底搜索捕获的漏网项，需在工单中单独登记处理。
5. **corpus 对拍预期**：批 1 拆除后重跑全语料，预期仅 memfail.log 消失、verdict 不变（探针为 I/O 增量，R19a §5.2 已评估为低影响）；[ELC] 未拆期间 031/061 用例对拍数据仍有效。
6. **本台账与 HANDOVER_PROGRESS.md「memfail 历史残留 8 处」口径差异**：该记录为旧轮次口径；本轮实测（含 R24e J 簇新增 [ELC] 与 R24 后结构调整）为 61 处 TEMP PROBE 标记 / 47 写盘点，以本台账为准。

*（本文件为 R24f 只读清点产物；不构成任何代码修改。）*