# AGENT_HANDOFF — PECMD 反编译还原 交接文档（给下一个 AGENT 自足执行）

> 目的：让下个 AGENT 在【无本会话上下文】的情况下，凭此文档直接接续任务。
> 工作区基准（已核对，全绿）：`refactored/`，git 工作干净，`build/link=0 FAIL/0 error`。
> 本文档信息全部经脚本核实，非猜测；若与代码不符，以代码为准。

---

## 0. 一句话总目标
把 PECMD 反编译出来的 `FUN_140xxxxxx` 符号逐个【还原成真实 C 函数体】后【命名成 `PECMD_*`】，使代码中 `FUN_(` 调用点持续下降，直至只剩已登记的硬骨头；**全程保持 `./build.sh core_*.c` 0 FAIL + `gcc ... -lm` 0 undef/multi/error**。

## 1. 已完成（基线，务必先复现）
- **调用点**：`FUN_(` 全库计数 **1562**（原始 4837，已消 3275 = **-67.7%**）。
- **rename_map**：**1157** 个 `FUN_140xxxxxx → PECMD_*` 映射（`tools/rename_map.json`）。
- **还原真体**：~150+ 个函数（含：自直移叶层 ~85；自主子代理完成的 4c0bc[1348行]、3c06c、26f00e26c 中的 9 个最深巨、4e2cc/ef91c/0f2384/03e220/02b9ec/0db648/02d708/045c90/02cc30/0b1724/02c634/**0d2e90[1553行]** 等）。
- 文档：`TASKS.md`§7-10、`REVIEW.md`§91-122、`FUNC_NAMES.md`。

## 2. 纪律（每轮硬性，不要违反）
1. **保持绿**：每批改动后立即 `bash build.sh core_*.c`（0 FAIL；本机无 /bin/bash 时用 `bash build.sh`）+ `gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c -lm -o /tmp/opencode/link_test`（mkdir -p /tmp/opencode，必须 0 undef/multi/error）。
2. **同文件并发禁止**：`link_stubs.c` 同一时刻只有一个编辑者（主代理自直移 或 一个子代理独占）。子代理提示词【必须】附自主条款（见 §6）。
3. **不虚构语义**：无法忠实移植的段/函数 → SKIP + 登记具体原因，绝不硬编。
4. 命名后**必须同步** `link_stubs.c`：`apply_rename.py` 只改 core_*.c，改完要额外在 core_*.c + link_stubs.c 对 `FUN_140xxxxxx` 词边界替换成 `PECMD_*`（否则 link 报 undefined PECMD_*）。
5. `apply_rename.py` 碰撞检查只看 rename_map 值，**不看代码里已存在的 PECMD_* 符号**——应用前先对 `include/*.h + core_*.c + link_stubs.c` 里所有 `\bPECMD_[A-Za-z0-9_]+` 建占用集，剔冲突；否则会 link 炸（历史上 662 错）。

## 3. 项目类型/陷阱（已沉淀，写码照做）
- `WCHAR = uint16_t`、`int64_t = long`（LP64）、`longlong = long`、`LARGE_INTEGER` 是 union（`.QuadPart`/`.s`/`.s.LowPart`）。
- **Linux wchar_t 是 4 字节**：所有 `L"..."` 传 `LPCWSTR`(=const uint16_t*) 处必须 `(const uint16_t *)L"..."` 或 `(LPCWSTR)L"..."`。
- **int64_t(long) ≠ long long**：混用会 `-Wincompatible-pointer-types`（本机当 error），需 `(long long *)`/`(int64_t *)` 精确强转。
- **`&DAT_000000NN` 是伪地址常量** → 一律写 `(void *)(uintptr_t)0xNN`，别去找定义。
- Ghidra 残留：`CONCAT44/71/31/22`、`._N_M_` 字节切片、`extraout_var`/`unaff_*`、`undefinedX`、`_struct_N_` → 按 `link_stubs.c` 内既有还原体的展开模式做（grep 参考）。
- 无参 `(void)` 桩被带参调用 → 就地改该桩为实参签名（同 TU 内才需，跨 TU 由调用方 extern 担）。
- 函数指针槽/全局：`DAT_14013xxx` 多为运行时槽，取真值用 `python3 tools/pe_data_extract.py ../PECMD.exe <addr> <n>`（或 registry，`tools/pe_registry.json`）；纯运行时槽不要硬塞常量，置 0 + 调用点跳过（与真实未装载一致）。

## 4. 下一步任务（优先级排序）
### 任务 A（最高优先）——完成 `FUN_14000e26c` 剩余段（3948 行巨，已到段6）
- 交接配方已在 **`tools/fun_14000e26c_HANDOFF.md`**（务必先读），body 源 **`/tmp/opencode/fun_body.txt`**（157KB，decompiled.c @7726），转换工具 **`/tmp/opencode/convert_seg.py`**。
- 落地现状：`link_stubs.c` 781 行起为 `uint64_t FUN_14000e26c(uint64_t a..h, 8 参)` 定义；**SEG1-5 已落地且绿**（段 5 完结标记在尾部、`return 0; }` 之前；`LAB_14001668b/LAB_140014f96` 是还占位、未定义的标签）。
- 剩余：SEG6(2384-2520, 深度0) → SEG7(2521-2800, 中段切需合成闭合) → SEG8(2801-3100) → SEG9(3101-3418, 深度0) → **SEG10(3419-3944 终段：真实返回 `return _Var39;`，按 `_Var39.v` 拼 `((uint64_t)(uint32_t)_Var39.ft.dwLowDateTime)|((uint64_t)_Var39.ft.dwHighDateTime<<32)`，删全部占位标签与合成闭合)**。
- 段 6 明确需要的符号（已核实）：`PECMD_SkipTokenWs`（定义在 **core_b1_remaining.c:3271**，`int64_t *`，只需在 link_stubs 顶部加 extern，签名 `int64_t *`；调用 `PECMD_SkipTokenWs((long long *)&local_x)`）；`DAT_14013ca90`（LPCWSTR 惰性槽）与 `DAT_14013ca4c`（short）**尚未定义** → 在 link_stubs.c 顶部加文件作用域全局。
- 完成标准：全绿 + `nm` 确认 `FUN_14000e26c` 为唯一强符号 + 记 REVIEW。
### 任务 B —— 命名轮（降调用点主引擎）
- 当前业务区（非 0x14010-11 CRT）未命名唯一 FUN_：**69 个**（核心目标 `FUN_140xxxxxx → PECMD_*`）。可先重新扫：`tools/nameable_60.json`（59 个老清单，已部分命名，重扫更新）、或现扫“decompiled 有体且未被 rename_map 覆盖的 FUN_”。
- 每名应用后调用点降几十~几百；**这是把 1562 继续往下打的主要手段**。
- 命名语义：读体→PECMD_<CamelCase>，核碰撞（§2.5），apply_rename + link_stubs 同步 + 测绿。
### 任务 C —— 剩余登记项收口
- CRT 区（0x14010-11，142 个）：多为无调用点数学内联，wchar 族 2B 专属保留，调用点影响≈0。
- 登记巨型 4：`084a5c/0b2ca8/0bb718/034788`（type-not-settling/C++vtable，需 Windows 运行时对照）→ 已在 REVIEW/P5 登记。
- `tools/p5_residue.json` + REVIEW§120 已有登记；确保与最终一致。

## 5. 工具与数据（交接自足）
- 提取真值：`python3 tools/pe_data_extract.py ../PECMD.exe <addr> <n>`
- 数据注册表：`tools/pe_registry.json`（1082 项，0 缺失）
- 反汇编：`objdump -d -M intel --start-address=0x140xxx --stop-address=0x140xxx+N ../PECMD.exe`
- 应用命名：`python3 tools/apply_rename.py '{"FUN_140xxxxxx":"PECMD_Y"}'`（后需手动同步 link_stubs）

## 6. 子代理自主条款（派发时【必须】附在提示词末尾）
```
【自主处理纪律 - 禁止提问/卡住】
- 歧义/缺失/目标含糊 → 不提问不等待，采用最合理保守解释并在报告注明取舍。
- 缺符号/类型/依赖 → 自行补最小桩/局部typedef/前向声明（与文件既有约定一致）。
- 无法忠实验证 → 那段按"不虚构语义"处理并说明（尽量全落地；巨型函数用 send_message 自我延续分段）。
- 永远不以"需要用户决定"结束回合；必以交付物（代码改动+验证+报告）结束。
- 验证：bash build.sh core_*.c(0 FAIL) + gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c -lm -o /tmp/opencode/link_test(0 undef/multi/error)。
```

## 7. 交接完成判据
- 00e26c 全量落地绿（任务 A 完成）。
- 命名轮把业务未命名 69 收窄到最小（任务 B），每轮降调用点。
- 剩余仅 CRT(无调用)/登记巨型4/真不可核验 → REVIEW/TASKS 同步 + 里程碑。