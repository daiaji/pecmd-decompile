# R25-j 子代理任务书：ADSL(PECMD_WlanManage) dc 直移补全 — 消除「假真体」

## 背景
- 仓库：D:\repo\pecmd-decompile（PECMD 反编译参考库）。dc = reference/decompiled.c 唯一语义真值。
- D-18 登记（docs/divergences.md）：ADSL 动词的处理器 `PECMD_WlanManage @0x140099f18`
  现状为**语义级重写且缺约 2/3 分支**（census 硬证据：dc 859 行/104 if/157 调用 vs
  msvc 403 行/33 if/72 调用），与 D-01/R25-c「看似有实为缩水」同族。
- 你的任务：用 **dc 直移**整体替换该语义级重写体，使分支数回到 dc 同构（~104 if）。

## 目标函数与文件（单文件独占）
- dc 源：reference/decompiled.c 行 96610-97471（`/* ========== FUN_140099f18 @ 140099f18 size=7183 ========== */`
  起，至 97472 行 FUN_14009bb28 头之前）。
- 目标文件（你只能写这一个文件）：src/device/core_b3r_h2.c
  - 现体：行 1965-2373 附近的 `ulonglong PECMD_WlanManage(longlong *param_1, LPCWSTR param_2)`
    （含头注释块），整体替换为直移体。函数外其余内容一律不动。
  - 调用方在 restored_bodies.c:6664（ADSL tag 0x4c534441），签名勿变。

## 移植纪律
1. 先通读 dc:96610-97471 全部语句再动笔；逐段附 dc 行号注释（项目直移体惯例，参考
   restored_bodies.c 直移体与 R25-g 校准样例 core_b2d.c 注释风格）。
2. 变量名沿用 dc 名（Ghidra 名可保留）；本轮**不做**语义化改名（L1 是后续批次的事）。
3. 已知可用依赖（勿重复定义）：
   - WLAN 函数指针槽 10 个：include/pecmd_globals.h:431-441 声明、src/kernel/core_globals.c:1140+
     定义（g_pWlanOpenHandle/g_pWlanGetAvailableNetworkList/g_pWlanCloseHandle/g_pWlanConnect/
     g_pWlanScan/g_pWlanRegisterNotification/g_pWlanEnumInterfaces/g_pWlanDisconnect/
     g_pWlanSetProfile/g_pWlanFreeMemory，各自映射 DAT_14013d788..7d0 槽）。若 dc 体调用的
     Wlan* 槽不在上列表中，先在 pecmd_globals.h/core_globals.c 之外找（不得改这两个文件），
     找不到则在本文件 extern 声明 + 注释 `TODO(verify): 槽地址待核`，不得臆造签名。
   - 字符串/缓冲助手（PECMD_StrBldCopyWide/PECMD_FreeStrBuf/PECMD_WideStrLen/PECMD_AllocStrSlot 等）
     已有真体，直接用；缺的按本文件既有 extern 惯例声明。
4. Ghidra 伪影处理（项目成文纪律）：
   - dc 变参调用丢弃参数、`unaff_` 寄存器残留、CONCAT 拼接噪声：不得原样编译成 UB，
     也不得臆造补齐——按 `.agents/skills/msvc-compat/SKILL.md` 的 9 类已定案手法处理，
     无法定案处标 `TODO(verify)` 并附 dc 行号。
   - dc 的指针差一律是 `(longlong)a-(longlong)b` 字节域整数；若语句含 `>>1` 按「字节差>>1=元素数」
     直译（见 analysis/r25h_ptrdiff_unit_audit.md 不变量）。msvc 侧写 cast 形态
     `(int64_t)((intptr_t)a-(intptr_t)b)>>1`。
5. 分配器契约：分配返回 hdr+8；释放一律 PECMD_FreeStrBuf；ReleaseRefCount 后置 NULL 再 Free。
6. 结构体布局（WLAN_INTERFACE_INFO_LIST 等 wlanapi 原生结构）：按 dc 访存偏移逐字段还原，
   不引入 Windows 原生头依赖（本文件未 include wlanapi.h，保持现状态）。
7. 禁止：构建（build）、git 写操作、windbg/Ghidra MCP、改动本任务书列出的文件之外任何文件。
   （构建验证由主代理统一做；你交付 = 落盘文件。）

## 纯度自检（交付前必做）
- 替换后用 python 对比：新体行数 ≥ dc 行数×0.9；`if` 计数 ≥ 95（dc=104，允许 Ghidra
  嵌套与 while 归并的少量出入，逐条注明原因）；调用点计数与 dc 157 对表（列出函数名→msvc 调用数）。
- 逐分支核对表：dc 的 104 个 if 分支 → 新体对应行号，写成报告表。

## 交付物（全部落盘，先写盘后回话）
1. src/device/core_b3r_h2.c — 直移后的 PECMD_WlanManage（唯一允许修改的文件）。
2. analysis/r25j_adsl_wlan_port.md — 报告：映射总表（dc 行→msvc 行）、分支核对表、
   外部依赖清单（已有真体/extern 新增/TODO(verify)）、伪影处理决策、自检数据。
3. 回话末尾给结构化摘要：RESTORED/SKIP(原因) 清单 + 自检数字。

【自主处理纪律 - 禁止提问/卡住】
- 你是一个自主执行体：遇到任何歧义、缺失信息、目标含糊、与预期不符的情况，不要提问、不要等待确认、不要卡住。
- 一律按以下方式自行决策并推进：
  1) 歧义 → 采用最合理/最保守的解释（不臆造语义、以证据为准），并在报告中注明你的取舍；
  2) 缺失符号/类型/依赖 → 自行补最小桩（与项目约定一致），或按文件既有模式处理；
  3) 无法验证 → 按"不虚构语义"原则登记 SKIP(具体原因) 并继续下一个目标；
  4) 遇到工具/环境错误 → 先重试一次；仍失败则记录原因并切换到可推进的替代路径；
  5) 永远不要以"需要用户决定"结束回合；总是以可交付物（代码/报告/文件）结束。
- 完成后给出结构化报告：每个目标 RESTORED/SKIP(原因)/未触及，以及最终自检状态。

## 附加纪律（自主执行时同样适用）
- 单文件独占：你只能写被分配的文件，绝不并发写共享文件（除非明确你是唯一编辑者）。
- 不虚构语义：无法忠实移植的按 SKIP+登记处理。
- 不提交 git（由主代理统一提交）。
