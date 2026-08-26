# R24 g_cmdTable 注册链静态排查（工单 D-16）

- 轮次/线别: R24 / 静态分析子代理（只读，未改任何 src/、*.c、include/、restored_bodies.c；未构建/部署/run_case；未 git commit）
- 交付物: 本文件（唯一落盘物，UTF-8）
- 真值源: `reference/decompiled.c`（dc，181421 行，base 0x140000000）、`build/msvc/pecmd_msvc.map`、原版 PE 资源节（自研遍历，见 §6 SKIP 注）
- 环境: 本会话无 Ghidra MCP / windbg MCP 工具 → 依任务书以 dc 文本 + map 推断为主，PE 资源取证用只读 python 手写解析（全部调用记录于 §6）

---

## 0. 结论摘要（TL;DR）

1. **dispatch 语义校正（对 D-16 前置命题的静态修正）**：`PECMD_DispatchBuiltin`(@140016bc8) 不是"全动词拦截器"。dc 中它只在动词未命中 FourCC 级联、无修饰前缀、且 `FUN_14002fd88`(ParseCommandPath) 判"未找到"时才被调用（dc:44262-44273），且只查 5 张**运行时**表（别名/插件/自定义）。98 动词内置命令走的是**硬编码 `local_158 == 0x……` 直比级联**（编译器生成，散落 restored_bodies.c 各 executor；s14 普查 + `tools/gen_fourcc.py` 口径），与 g_cmdTable**无数据流关系**；r19b 已证 WRITE(5 字母)→`uVar21==0x404` 门控不过→默认分支→DispatchBuiltin(表全空)→ECD(外部程序) 兜底**属原版行为**（dc 同构）。即：**表恒空并不构成当前 19 个 PASS 案的对拍偏差**（原版在同一场景下表也空，行为一致；r19b 的 183≠2 是另一处退出码槽污染，已另案修）。
2. **真正断裂点 = 桩接线错误（3 处）＋资源缺失（1 处），全部为"潜伏缺口"**（当前语料无触发源，不烧线）：
   - G1: "LoadPlugin=" 处理器（core_b1_remaining.c:5318-5323 ↔ dc:7412）调用的是 **stub `FUN_14000C764`**（unimplemented_stubs.c:271，恒返 0），而 dc FUN_14000c764 的真体 `PECMD_RegisterFileAssociations`（core_b1_remaining.c:4830）**零调用者（死代码）**。map 实锤：`FUN_14000C764 → unimplemented_stubs.obj`、`PECMD_RegisterFileAssociations → core_b1_remaining.obj`。
   - G2: `PECMD_RegisterFileAssociations` 内 INDATA 解码调用的是 **stub `PECMD_ResDecode`**（unimplemented_stubs.c:208，恒返 0）；真体实际名为 `FUN_1400E7D58`（core_resdecode.c:52）且未被该链引用。map 实锤两符号并存。
   - G3: 真体 `FUN_1400E7D58` 的内核依赖 `PECMD_DecodeEncTextToUtf16`(@0x1400e7994)、`FUN_140060A74`、`PECMD_ResReenc`、`FUN_14005B184` **全部仍为 stub**（其中 FUN_14005B184 恒返 0=“相等”，会把 BOM 判定全部判成 UTF-16BE）→ 即便接好 G2，解码也不可用。
   - G4: msvc 构建**无 .rc/资源**（全仓无 .rc；sources.rsp 无资源项）。原版 PE 内嵌 `INDATA`(字符串类型) name=4..35（name=4 恰为 18 字节 `LOAD: HELLO1  #1  `，即 dc:6988 特取的那个）、`SCRIPTINIT/100`(756B 明文启动脚本)、`SCRIPT/101..203`(各 1B 空)、`EXEDATA/100`(1B) 等 —— 印证 D-17。
3. **R16 修复前提需要修正**：「补 msvc 初始化调用」不成立 —— **原版不存在启动即注册路径**。LoadPlugin= 是条件触发：脚本/命令行行首呈 `^`/`-mode`/`&&&` 形态 → `PECMD_EvalSpecialToken`(dc:27052/27068) → `PECMD_ParseEnvSwitches`(dc:7412) → LoadPlugin。最小修复 = 把 stub 转发到真体（沿用项目既有「同址别名归一转发」模式），而不是新增初始化调用。
4. REGI 等已 PASS 案**不受影响**：REGI 为四字母动词，命中其 executor 的 FourCC 直比链（s15: 包装器 b08/b24），不经 DispatchBuiltin 五表；修复本链不改任何 corpus 行为（语料无 LoadPlugin= 触发，表依旧为空，与原版一致）。

---

## 1. 原版侧调用链重建（dc 行号证据）

### 1.1 g_cmdTable1..5 地址映射（dc ↔ msvc）

| dc 全局 | msvc 符号 | 定义处 |
|---|---|---|
| DAT_14013ca98 / DAT_14013caa0 | g_cmdTable1Count / g_cmdTable1 | core_globals.c:254-258 |
| DAT_14013a078 / DAT_14013a080 | g_cmdTable2Count / g_cmdTable2 | core_globals.c:236-240 |
| DAT_14013a058 / DAT_14013a060 | g_cmdTable3Count / g_cmdTable3 | core_globals.c:230-234 |
| DAT_14013a098 / DAT_14013a0a0 | g_cmdTable4Count / g_cmdTable4 | core_globals.c:242-246 |
| DAT_14013a0c8 / DAT_14013a0c0 | g_cmdTable5Len / g_cmdTable5 | core_globals.c:248-252 |
| DAT_14013cb08 | g_flagCB08（LoadPlugin 一次性守卫） | core_globals.c:350 |

### 1.2 填充点清单 —— 唯一活体填充者 = `FUN_14000c764`(LoadPlugin，dc:6897，size=1909)

- **cmdTable1**（.pecmdplugin.*.PEI 插件表，0x18B/条，name@+0x10，funcptr@+0）：dc:6974-6975 `FUN_140009068(&DAT_14013ca98, pattern, dir)`。FUN_140009068（dc:4940，size=372）＝ msvc `PECMD_LoadPluginPecmdTbl`（core_b1_remaining.c:2220）：FindFirstFile 扫描 → LoadLibrary → `GetProcAddress("PECMDTBL")` → 前 4B 为条目数，其后每条 0x18B 追加进 cmdTable1。
- **cmdTable2**（0x10B/条 = {value@+0, name@+8} 双槽，别名展开表）：dc:6988-7040。INDATA 资源按行扫 `LOAD:` 前缀：
  - dc:6988 `FUN_14001ea18(DAT_14013cf70, 0x4, L"INDATA", &local_60, 0)`（资源类型串 "INDATA"、**名字 ID=4**）
  - dc:6989 `FUN_1400e7d58(&local_60, 1)`（ResDecode，flags=1）
  - dc:7028/7039 `FUN_140005818(&DAT_14013a080, …)` 双槽追加 + dc:7040 `DAT_14013a078++`（计数+1）
- **关联表（非 cmdTable）**：dc:6955-6971 `.exe/.com/.ntr/.cmd/.bat` → `PTR_PTR_14013a050`；dc:6978-6983 `.$*.dll/.$*.dlx` → `PTR_PTR_14013a090`；dc:7050-7062 `.wci/.wcs/.wce/.wcz` → `PTR_PTR_14013a070`（均经 FUN_140008ea4 = msvc PECMD_ScanDirectory）。
- **cmdTable5**（变量别名表，另路）：`Alias` token → `FUN_14000cedc`（dc:7074 = msvc PECMD_RegisterTableItem，core_b1_remaining.c:5010；dc:7164-7206 操作 DAT_14013a0c0/0c8）。与 LoadPlugin 无关但同属“注册链”，一并登记。
- **生命周期/清理**：dc:181219/181231/181243/181255/181267（退出清理，五表释放）。

### 1.3 LoadPlugin 的角色与触发点

- **触发 handler**：dc:7407-7412，位于 **FUN_14000d2a0 @ 14000d2a0（size=3609，dc:7217 起）** —— msvc `PECMD_ParseEnvSwitches`（core_b1_remaining.c:5158）：
  ```c
  dc:7409  if (DAT_14013cb08 != '\0') return 1;          // 一次性守卫（已加载即跳过）
  dc:7412  DAT_14013cb08 = FUN_14000c764((LPWSTR)(local_res20 + 0x16));  // 参数= "LoadPlugin=" 之后的串
  ```
  该函数整体是 PECMD 命令行/首行 "key=value" 开关解析器（同函数内还处理 Arg=、Clipboard=、Clipboard?=、QueryCmd=、Alias、WndProc、TextDelay=、EnviBroad=、memvar=、LoadEnvi、FORCELOCAL 等，dc:7360-7469；msvc 同构 5289-5380）。
- **FUN_14000d2a0 的全部调用点（dc 全量仅 2 处）**：
  - dc:27052 `FUN_14000d2a0(…, 0x26)` —— 行首 `^`(0x5e) 或 `-mode` 形态；
  - dc:27068 `FUN_14000d2a0(…, 0)` —— 行首 `&&&` 三连 & 形态。
  二者均位于 **FUN_14002d33c @ 14002d33c**（dc:27011，size=969）＝ msvc `PECMD_EvalSpecialToken`（core_b2e.c:1358）。
- **FUN_14002d33c 的全部调用点（dc 仅 1 处）**：dc:44227，位于 **FUN_14004c0bc @ 14004c0bc（size=8717）＝ msvc `PECMD_ProcessScriptBlock`**（restored_bodies.c:5828，「大分发器/PSB」），该处是脚本行执行入口（特殊 token 求值槽）。
- **启动早期链**：WinMain 等价主逻辑（PECMD_MainW 系，@0x140009ccc）→ `FUN_140004fd4`（core_exec.c:113，@0x140004fd4）→ `PECMD_RunStartupScript`（@0x14004eb34，dc:45452-45864，core_script2.c:193）：
  - dc:45546 `FUN_14001ea18(…, 0x64=100, "SCRIPTINIT", …)` + dc:45548 ResDecode → 非空则经 `PECMD_SetControlCommandTrigger`(dc:45565 FUN_1400b6240) 执行；
  - dc:45600 `FUN_14001ea18(…, 0x65=101, "SCRIPT", …)` + dc:45602 解码 → 得内置脚本；
  - 然后按 cmdline/autoapp 分发（dc:45626-45864）→ `PECMD_RunCommand` / `PECMD_ProcessScriptBlock` 逐行执行。
  **关键静态事实：上述任何启动路径都没有直接调用 FUN_14000d2a0 或 FUN_14000c764。** 只有执行到以 `^`/`-mode`/`&&&` 开头的脚本/命令行 token 时，才会经 EvalSpecialToken → ParseEnvSwitches 命中 LoadPlugin=。⇒ **原版没有"启动即注册"路径，LoadPlugin 是条件触发**。
- **原版自身启动资源实测**（§6 方法）：SCRIPTINIT/100 = 756B 明文（`//ENVI^ Alias *opt sprintf=CALL $**c…`，mdyblog 经典 init 块），**其中无 LoadPlugin= 行**；SCRIPT/101..203 各 1B（空）。⇒ 原版默认启动下 g_cmdTable1/2 同样为空。INDATA/4=18B = `LOAD: HELLO1  #1  `（独一行）；按 dc:7004-7040 解析 → 注册恰好 1 条 {name:"HELLO1"(+ 截断), value:"#…#HELLO1" 形态} 别名，且**仅当 LoadPlugin= 触发时**（INDATA/4 自身不自动烧录）。原始字节附 §7。

### 1.4 VERB 表（98 动词 FourCC）加载源 —— 答案：**硬编码直比级联，与资源/运行期表均无关**

- 证据：s14 普查（`analysis/s14_fourcc_replacement_census.md`）——`local_158 == 0x……` 直比 98 个互异动词值全部位于 **restored_bodies.c 代码现场**（99 处含别名）；`tools/gen_fourcc.py` 数据源即 `restored_bodies.c` 的 `local_158 == 0x…{8}` 直比（口径：只收 == 直比）；`include/pecmd_fourcc.h` 为其生成结果。s12 权威口径 117 逻辑动词（±别名）。
- dc 现场：大分发器内的 4 字母门控 `uVar21 == 0x404`（低字节=动词标识符长度，r19b §2.1）与 `local_158` 逐值比较（dc:44197-44240 一带的 0x444553/0x524944/0x544553/0x45524854…；rb:6229-6268 同构）。**该级联只服务 ≤4 字母 token**；`WRITE`(5 字母) 在两版都落默认分支（r19b：`uVar21=0x505 → LAB_14004df3c`）。
- 与 cmdTable 的关系：**无**。DispatchBuiltin 只是兜底别名/插件/自定义表。

### 1.5 DispatchBuiltin 语义与调用点

- dc:11911-12080 `FUN_140016bc8`（size=1094）＝ msvc `PECMD_DispatchBuiltin`（core_b1_remaining.c:5633）。扫描顺序 table1(0x18B 直调 funcptr，dc:11947-11964) → table2(组串 `value+" "+rest` 后 `FUN_140031454`=RunCommand 执行，dc:11968-11983) → table3(`--exe:` 组串→`FUN_14000e26c`，dc:11986-12003) → table4(`FUN_1400408d0`，dc:12007-12021) → table5(临界区 DAT_14013e190 内 name/value 查，`FUN_14007de70`+`FUN_14004c0bc` 重入，dc:12023-12039)。
- 调用点 dc:44269-44273：`sVar26==0`（无 `*`,`-`,`+`,`@` 前缀）且 `local_res18[0]<0`（ParseCommandPath 未定罪）→ 构造 verb 词 + rest → `FUN_140016bc8(ctx, param_3, local_f0, rest, &local_180.s)`；返回 0（=未命中）→ 继续 bare-path：ExpandVarDispatch → 脚本文件判定（`PECMD_IsPecmdScriptFile`）→ `PECMD_RunCommand`（脚本）/ ECD 外部程序路径（rb:6289-6329 同构）。

---

## 2. msvc 侧现状（对照）

| dc 函数 | msvc 符号 / 位置 | 状态 |
|---|---|---|
| FUN_14000c764 (LoadPlugin) | `PECMD_RegisterFileAssociations` core_b1_remaining.c:4830（真体，含 INDATA LOAD: 扫描 4925-4983） | **0 调用者（死代码）**；handle 器调的是 stub（见 G1） |
| FUN_140009068 | `PECMD_LoadPluginPecmdTbl` core_b1_remaining.c:2220（static，PEI→cmdTable1） | 仅被死代码调用 |
| FUN_14000cedc | `PECMD_RegisterTableItem` core_b1_remaining.c:5010（Alias→cmdTable5） | 被 ParseEnvSwitches "Alias" 分支调用（5326-5328），活 |
| FUN_14000d2a0 | `PECMD_ParseEnvSwitches` core_b1_remaining.c:5158 | 活；**"LoadPlugin=" 分支调 stub**（5318-5323） |
| FUN_14002d33c | `PECMD_EvalSpecialToken` core_b2e.c:1358 | 活；'^'/'-mode'→1388，'&&&'→1406 |
| FUN_14004c0bc | `PECMD_ProcessScriptBlock` restored_bodies.c:5828 | 活；dc:44227→rb:6246；dc:44273→rb:6296 |
| FUN_14004eb34 | `PECMD_RunStartupScript` core_script2.c:193（dc 45452-45864 直移） | 活；FUN_14001EA18(SCRIPTINIT/SCRIPT) + FUN_1400E7D58（真名） |
| FUN_140016bc8 | `PECMD_DispatchBuiltin` core_b1_remaining.c:5633 | 活；五表查（5670-5809） |
| FUN_14001ea18 (资源装载) | `FUN_14001EA18` core_exec2.c:390 | 活（依赖 FUN_140068984 —— **stub**，见 G3） |
| FUN_1400e7d58 (ResDecode) | 真体名 `FUN_1400E7D58` core_resdecode.c:52 | **未被 cmdTable 链引用**；注册链调的是 stub 名 `PECMD_ResDecode` |

初始化路径（与 dc 拓扑 1:1，无差异）：`PECMD_MainW`(core_main.c:62) → `PECMD_InitEnvironmentVars`(core_init.c:78, @0x140027690) → … → `FUN_140004FD4`(core_exec.c:113) → `PECMD_RunStartupScript`(core_script2.c:193) → `PECMD_RunCommand`/`PECMD_ProcessScriptBlock`。**没有任何 init 函数调用 RegisterFileAssociations / ParseEnvSwitches** —— 与原版一致，故「补初始化调用」不成立。

---

## 3. 缺口清单（dc 证据 + msvc 锚点）

| # | 缺口 | dc 证据 | msvc 现状 |
|---|---|---|---|
| G1 | LoadPlugin= 处理器未接真体 | dc:7412 `DAT_14013cb08 = FUN_14000c764(…)` | core_b1_remaining.c:5322 调 stub `FUN_14000C764`（unimplemented_stubs.c:271，恒返 0）；真体 PECMD_RegisterFileAssociations 同文件 4830，零调用。map：FUN_14000C764@unimplemented_stubs.obj，PECMD_RegisterFileAssociations@core_b1_remaining.obj（1402db10-1402db10 区）。守卫 g_flagCB08 因 stub 恒 0 永不锁存 |
| G2 | ResDecode 名字接线错误 | dc:6989 `FUN_1400e7d58(&local_60,1)` | core_b1_remaining.c:4929 调 stub `PECMD_ResDecode`（unimplemented_stubs.c:208，恒返 0）；真体名 FUN_1400E7D58（core_resdecode.c:52）未被引用。map：PECMD_ResDecode@unimplemented_stubs.obj(1401e5530)，FUN_1400E7D58@core_resdecode.obj(140119590) |
| G3 | 解码内核未还原 | dc:18386 `FUN_140068984`；dc:142191 `FUN_1400e7d58`（依赖 @0x1400e7994 DecodeEncTextToUtf16 等） | FUN_140068984=空桩（unimplemented_stubs.c:220）；PECMD_DecodeEncTextToUtf16=桩（:240）；FUN_140060A74=no-op 桩（:228）；PECMD_ResReenc=桩（:217）；**FUN_14005B184=恒返 0 桩（:225）**（0=相等 ⇒ BOM 判定全部“成立”，会把任意数据按 UTF-16BE 处理）。⇒ 即便 G1/G2 接好，INDATA/SCRIPTINIT 解码产物也为空/损坏 |
| G4 | msvc 构建无任何资源 | 原版资源节取证（§7）：INDATA/4..35、SCRIPTINIT/100=756B、SCRIPT/101..203=1B、EXEDATA/100=1B、HELP/LANG、RT_MENU 10021 等 | 全仓无 .rc；sources.rsp 无 resource 条目；FUN_14001EA18(g_hInst,4,"INDATA")/SCRIPTINIT/SCRIPT 全部 FindResource 失败（GetLastError 1813）→ 表 2 与启动脚本资源双缺失（D-17 成立） |
| G5 | 触发场景无活体验证 | dc:27052/27068（'^'/'&&&' 形态）、dc:44227 | msvc 同拓扑（core_b2e.c:1386-1410）。当前语料/黄金无 LoadPlugin= 行（golden grep 0 命中）→ 两版表同空，无对拍偏差，属潜伏缺口 |
| G6 | INDATA/4 语义残留（对照项） | dc:7022-7040 `#<token2>` 拼接 | msvc 4960-4978 同构恢复。INDATA/4 仅 1 行 `LOAD: HELLO1  #1  `→ 1 条别名 {name:"HELLO1", value:"# #1 HELLO1" 形态}；展开式精确串依赖 ResDecode 产物，**待活体验证** |

---

## 4. 修复建议（最小落地点 + 风险）

### A.（推荐第一优先，最小、零行为扰动的）“同址别名归一转发”战法
沿用 unimplemented_stubs.c 既有先例（`PECMD_ScriptCopy→FUN_140017CDC` :248、`PECMD_ParseCommandPath→FUN_14002FD88` :253、`PECMD_CopyPathToken→FUN_14006E030` :261）：
1. `FUN_14000C764(LPCWSTR p)` → `return PECMD_RegisterFileAssociations(p);`（真体同 TU，需前置声明）；
2. `PECMD_ResDecode(int64_t *ps, uint32_t flag)` → `return FUN_1400E7D58(ps, flag);`（core_resdecode.c 已链接）；
3. 恢复解码内核真体：`PECMD_DecodeEncTextToUtf16`(@0x1400e7994) 为**必选项**；`FUN_140060A74`(@0x140060a74 字节交换)、`FUN_14005B184`(@0x14005b184 定长比较，当前恒 0 桩会破坏 BOM 判定——**优先级高于 G1/G2**，因为它是“安全网下唯一会让真体错判”的桩)、`PECMD_ResReenc`(@0x140075c7c)。FUN_140068984(@0x140068984, size=637) 需还原（FUN_14001EA18 主解码步）。
   - 这些是 G3 的原子集；C 语言还原可从 dc:142188、dc:64928、dc:18331、dc:1400e7994 直移，粒度与现项目恢复惯例一致。
4. 参数来源：LoadPlugin= 处理器把 `(LPWSTR)(p + 11)` 原样传入（core_b1_remaining.c:5322 已经写好）；无需新增全局状态。

### B.（D-17 架构级，独立决策项）资源嵌入
从原版 PE 提取资源字节（INDATA/4..35、SCRIPTINIT/100、SCRIPT/101..203、HELP/LANG、RT_MENU 10021 等，§7 表）构建 .rc/bin include 嵌入 msvc，接 FUN_14001EA18 链。
- **前提**：先完成 A（否则解码仍空）。
- **风险红条**：嵌入即改变 RunStartupScript 行为（SCRIPTINIT/100 是真脚本，会开始执行）——必须先与原版逐行对拍后才可开放；且**切勿**包装成“启动即调 LoadPlugin” —— 原版为条件触发，主动预热会在含 bare-path 兜底语义的语料上制造新偏差（r19b: WRITE→ECD 是两版共有原版行为）。

### C.（不做，仅登记）触发形态复现
`g_cmdlineMode`（"PECMD**pecmd-cmd*" 前缀，dc:45611-45624）与 `^`/`&&&` 行首形态如何被真实 PECMD.INI/命令行使用，静态无从定案 → 记 V3（见 §5）。

### 风险注记（对已 PASS 的 19 案）
- **零影响**。修复 A 只在"脚本含 `^/&&&/LoadPlugin=` 行"这一当前不存在的场景下改变行为；REGI/ENVI/GETF/IFEX 等四字母动词命中各自 executor 的 FourCC 直比链（s15 包装器查询表），不经 DispatchBuiltin；语料无 LoadPlugin= → 两版表同空 → 行为同位（r19b §”无需改动的地方“ + §2.1 同款结论）。
- 唯一需提防的耦合点：cmdTable2 命中后会 `PECMD_RunCommand(value+" "+rest)` 重入执行 —— 若未来语料真的触发注册，同一动词可能被别名抢先于四字母链之外执行；这是原版固有语义，不是回归。

---

## 5. 待活体验证清单（本任务仅静态，以下均为 V 项）

- **V1**：原版在真实触发场景（含 `^LoadPlugin=…`/`&&&…` 或首个 token 为 `-mode` 的脚本行）下 g_cmdTable1/2 最终内容 dd（预期 cmdTable2 含 INDATA/4 那条 HELLO1 别名；cmdTable1 取决于模块目录是否存在 `*.pecmdplugin.*.PEI`）。
- **V2**：原版 IRP/启动早期是否还有 dc 之外的 LoadPlugin 触发面（已静态排除 dc 文本 3 处引用，仍以活体 bp `FUN_14000c764` 二次证伪，需当期 symsnap 选址）。
- **V3**：`^`(`0x26` 模式)/`&&&` 行首形态在真实 PECMD.INI/命令行中的出现方式（`g_cmdlineMode` 前缀 "PECMD**pecmd-cmd*" 语义边界）。
- **V4**：修复 A 后 msvc 侧 ResDecode(flags=1,key=0) 对 INDATA/4 的产物 === 原版等值（预期 UTF-16LE `LOAD: HELLO1  #1  `），以及 `{value,name}` 两槽展开式的逐字节对拍。
- **V5**：原版 PECMD.INI/自动运行脚本是否含 LoadPlugin=（golden 语料 grep 0 命中，仍以部署现场确认）。

---

## 6. SKIP 登记与取证方法

- Ghidra MCP：**不可用**（本会话无 MCP 工具暴露）→ 依任务书以 dc 文本 + build/msvc/pecmd_msvc.map 推断，登记 SKIP。PE 资源取证改用只读 python 手写遍历（无第三方依赖）。
- 取证调用记录：
  1. `awk/sed/grep`：dc 函数头定位（FUN_14000befc/bfcc/c430/c764/cedc/d2a0/…；FUN_14004c0bc@dc:43816；FUN_140016bc8@dc:11911；FUN_14002d33c@dc:27011；FUN_14001ea18@dc:18331；FUN_1400e7d58@dc:142188；FUN_140009068@dc:4940；FUN_140068984@dc:64928）；
  2. `grep dc`：g_cmdTable 地址族 35 处命中、FUN_14000c764/14001ea18/140009068/140005818 全量引用、FUN_14000d2a0/FUN_14002d33c/FUN_140016bc8 全量引用；
  3. `grep src+restored_bodies`：g_cmdTable 60 处、PECMD_ParseEnvSwitches/EvalSpecialToken/DispatchBuiltin/RegisterFileAssociations 引用（零调用者确认）、ResDecode 双符号确认；
  4. `pecmd_msvc.map`：FUN_14000C764/PECMD_ResDecode → unimplemented_stubs.obj；PECMD_RegisterFileAssociations/PECMD_ParseEnvSwitches/PECMD_DispatchBuiltin → core_b1_remaining.obj；FUN_14001EA18 → core_exec2.obj；FUN_1400E7D58 → core_resdecode.obj；g_cmdTable1/2/Count/g_flagCB08 <common> 地址（1402c7c68/1402c7c98/1402c7c70/1402c7ca0/1402c7b9c）；
  5. python PE 资源遍历（read-only）：dd[2]=0x148000 sz=0xabc70，.rsrc ro=0x140c00；递归 3 级目录（named/id 入口）读出类型-名-语言-大小清单（§7）。
- 未执行：任何构建/部署/run_case/dd/bp；未改任何源码文件。

---

## 7. 附：原版 PE 资源节取证（reference/PECMD.exe = PECMD原始.EXE，双文件逐字节同）

| 类型 | 名称 | 语言 | 大小(B) | 备注 |
|---|---|---|---|---|
| INDATA(串) | **4** | 2052 | **18** | LoadPlugin 特取项（dc:6988 id=0x4） |
| INDATA(串) | 5..9 | 2052 | 550/558/552/2681/6144 | CMPS 压缩磁盘引导图（"NO NAME FAT12/NTLDR is missing/BOOTMGR…"）|
| INDATA(串) | 10,20..35 | 2052 | 6752/1622/34989/117657/894/91475/1288/3544/14137/8792/157372/6895/4824/1984/55369/781/3434 | "#xx|INDATA" 脚本内嵌资源族 |
| SCRIPTINIT(串) | 100 | 2052 | **756** | 明文启动脚本（mdyblog init：`//ENVI^ Alias *opt sprintf=CALL $**c **qd **ret:* user32.dll,wsprintfW,`…`SET __bInitWin=1`…）；**无 LoadPlugin= 行** |
| SCRIPT(串) | 101,102,103,201,202,203 | 2052 | 1/16/1/1/1/1 | 内置 SCRIPT 资源（基本为空） |
| EXEDATA(串) | 100 | 2052 | 1 | 空 |
| TAGDATA(串) | 10005 | 2052 | 45 | |
| RT_MENU(4) | 10021 | 2052 | 802 | 启动菜单（"PECMD_TrayPopupMenu" + "PECMD.%…% HELP"…） |
| RT_DIALOG(5)/RT_ICON/RT_ICONGROUP/RT_STRING/RT_VERSION(16)/RT_MANIFEST(24)/RCDATA | … | 2052 | … | 常规 UI 资源 |

**INDATA/4 原文（18B，ASCII）**：`4c 4f 41 44 3a 20 48 45 4c 4c 4f 31 20 20 23 31 20 20` = `"LOAD: HELLO1  #1  "`。

**SCRIPTINIT/100 全文（756B，GBK/ASCII，无加密）**：
```
//ENVI^ Alias *opt sprintf=CALL $**c **qd **ret:* user32.dll,wsprintfW,
//ENVI^ ALIAS *opt Beep=CALL $**qd# **ret:* Kernel32.dll,Beep,
//ENVI^ alias *opt MemoryCopy=CALL $**c **ret:*  .,MemoryCopy, 
//CALL @
SET __bInitWin=1
{
ENVI^ EnviMode=1  // 2=有初始窗口 1=无
FIND $PECMD.EXE DOWN=%@,!!  TEAM SET &::__Autoapp=$%*| EXIT FILE  // SHOWDOWN of START-MENU [PECMD.EXE DOWN]
FIND $*MyMain=%1,!! TEAM SET &::__Autoapp=*MAIN *#102| EXIT FILE  //EXECUTE MAIN *#102
NAME &&MyName0M=%MyNAME%
FIND $_=_%1,!! FIND $PECMD_M=%&MyName0M%,!! SET &::__Autoapp=*MAIN *#102| EXIT  FILE  //EXECUTE MAIN 
IFEX %&MyName%.autoapp.wcs,  SET &::__Autoapp=+"%&MyName%.autoapp.wcs"  //FILE
//TEAM SET &::__Autoapp=#| EXIT FILE  //disable call inner cmd
}
//。
```

---

## 8. 关键行号索引（速查）

- dc:6894-7070 FUN_14000c764（LoadPlugin 真体）；dc:6975 cmdTable1 填；dc:6988-7004 INDATA+ResDecode+LOAD: 行扫；dc:7028/7039-7040 cmdTable2 填；dc:7412 LoadPlugin= 触发；dc:27052/27068 ParseEnvSwitches 双调用；dc:44227 EvalSpecialToken；dc:44262-44273 DispatchBuiltin 调用门；dc:45546/45600 SCRIPTINIT/SCRIPT 装载。
- msvc: core_b1_remaining.c:4830（真体，死）、:4928-4929（INDATA+ResDecode stub）、:5317-5323（LoadPlugin= → stub）、:5158（ParseEnvSwitches）、:5633（DispatchBuiltin）、:2220（PEI 表 loader）；unimplemented_stubs.c:208/220/217/225/228/240/271；core_resdecode.c:52；core_exec2.c:390；core_script2.c:193/272/344；restored_bodies.c:5828/6246/6296；core_b2e.c:1358/1388/1406；core_globals.c:230-258/350。