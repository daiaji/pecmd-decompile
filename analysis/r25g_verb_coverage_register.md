# R25-g 动词级实现状态盘点 — 67 个零语料验证动词 × msvc 实现四级判定

- 线别：R25-f P2（盘点子代理 B）｜日期：2026-08-29｜性质：**纯静态只读取证**（未构建、未运行、未动 src/docs、无 windbg/Ghidra MCP）。
- 上游输入：账本 `HANDOVER_PROGRESS.md` §R25-b（2026-08-28）——FourCC 权威 98 动词（含 IPAD/EDIT 表外 2 则 95 可核），语料覆盖 28/95，**67 动词零语料验证**。
- 交付目的：为 `docs/divergences.md` 的 67 动词登记（对冲 R25-b「其还原代码目前处于简化桩/未验证态」的粗粒度表述）提供逐动词实现状态判定表，可直接并入或以本报告为索引挂账。
- 结论基准：msvc 当前工作树快照 vs dc `reference/decompiled.c`（唯一语义真值，基址 0x140000000）。行号绑定本快照，重构/重链后须复核。

## 0. 判定口径

| 级别 | 定义 |
|------|------|
| **真体** | 处理函数为 dc 忠实直移/等价实现：dc 分发级联分支存在 + msvc 体与 dc 体行/分支结构对得上（census 见 §4）+（多数）带 dc 直移注记 |
| **简化桩** | 有函数体但逻辑明显缩水（分支/调用数显著低于 dc）或自标「简化/TODO(verify)」占位主导 |
| **恒0/no-op 桩** | 函数体为 `(void)x…return 0` 型空壳（unimplemented_stubs.c 或等价空实现），全仓无第二真体（已逐个排查 D-01 式「桩遮蔽真体」双体模式，7 桩均**无**被遮蔽真体） |
| **缺失** | 分发表无此动词或处理函数不存在（本轮 0 例） |

动词→处理函数映射来源：PSB 分发级联 `restored_bodies.c` `PECMD_ProcessScriptBlock`（↔ dc `FUN_14004c0bc @0x14004c0bc`，级联体 dc:44213–45200 附近，`local_158` FourCC 直比，67/67 动词分支全部在场，rb 分支行号见证据列）。msvc 函数名为 `tools/rename_map.json` 启发式重命名标签，**不代表语义**，判定只认 dc 地址与函数体。

## 1. 统计摘要

| 级别 | 计数 | 动词 |
|------|------|------|
| 真体 | **59** | 含 5 个带注记真体（LOAD/ITEM/SERV/SHOW/PUTF，见 §2 表内注） |
| 简化桩 | **1** | ADSL（403/859 行、33/104 分支，语义级重写且缺约 2/3 分支） |
| 恒0/no-op 桩 | **7** | MESS MSTR SBAR SITE SOCK SPIN USER |
| 缺失 | **0** | — |

关键发现（修正 R25-b 的悲观先验）：

1. **67 个零语料动词中 59 个（88%）处理函数是 dc 直移真体**——「零语料验证」≠「实现缺失」，风险主要是不被测试暴露的直移缺陷（R24 经验：goto 映射/缓冲区尺寸类），而非空桩。
2. **7 个恒0桩全部集中在 5 个文件**：`unimplemented_stubs.c`（MSTR/USER）、`src/ui/core_b7c.c`（SBAR/SPIN/SITE）、`src/ui/core_b8_remaining.c`（SOCK）、`src/commands/core_b3_remaining.c`（MESS，带完整 SKIP 分析注记，dc 体 4933B GUI 文本编辑器）。
3. **MESS 桩有双重暴露**：除 MESS 动词分支外，`src/commands/core_b3l.c:1385` 与 `src/commands/core_calc_expr.c:983` 两处内部调用同样吃恒0桩（消息弹窗静默不显示），影响面大于单动词。
4. **USER 是「看似忠实实为分歧」的反例**：dc `FUN_14001af0c` 本体仅 7 字节（`core_skip_registry.c:17` 据此判 SKIP），但其调用的 `FUN_14001ada8`（dc:15525，346 字节，逗号分隔参数解析 + 逐段处理）有真实副作用且 msvc 全仓无实现——恒0桩与原版存在真实行为差，非平凡跳过。
5. **ADSL 是本轮唯一新发现的「看似有实现实为缩水」型**（与已登记先例 R25-c PECMD_AssignString 族、D-01 桩遮蔽族同风险类别），census 硬证据：dc 104 if/157 调用 vs msvc 33 if/72 调用。

## 2. 67 动词登记总表

证据列格式：`dc: 起始行 (L行/I分支/C调用) ↔ msvc (L/I/C)`；分支级联行 = `restored_bodies.c` 中该动词 `local_158` 直比分支行。风险列全部为**静态推断**（标注推断性质），未运行验证。

| 动词 | dc 函数/地址 (dc 行) | msvc 位置 | 状态 | 证据 | 若被语料覆盖的预期风险（推断） |
|------|----------------------|-----------|------|------|-------------------------------|
| ADSL | PECMD_WlanManage @0x140099f18 (dc:96610, 7183B) | src/device/core_b3r_h2.c:1971 | **简化桩** | dc 859L/104I/157C ↔ msvc 403L/33I/72C；分支/调用数缺约 2/3，语义级重写非直移；级联 rb:6664 | WLAN 枚举/扫描/连接等部分分支静默缺失 → 部分子命令静默无操作或错值；**同 D-01/R25-c「看似有实现」族，最易误判为已完成** |
| BASE | PECMD_AttachControlImage @0x1400bf068 (dc:118218) | src/commands/core_b3r_i28f.c:662 | 真体 | 96L/8I/26C ↔ 99L/8I/26C；级联 rb:6491 | 低（直移） |
| BROW | PECMD_ShowBrowseFolder @0x140087690 (dc:86212) | src/commands/core_b3r_d.c:420 | 真体 | 412L/45I/97C ↔ 428L/45I/92C；级联 rb:7107 | 低（直移） |
| CMPS | PECMD_ProcessEncodedFile @0x140086944 (dc:85716) | src/device/core_b3r_h1.c:1296 | 真体 | 490L/47I/71C ↔ 518L/47I/68C；级联 rb:6899 | 低（直移） |
| COME | 内联分支 dc:44435（写 DAT_14013a248） | restored_bodies.c:6442（级联内联块） | 真体 | rb 内联块逐语句直移（`g_charTableF` ≡ dc DAT_14013a248；SkipLeadingControls ≡ FUN_1400170b0） | 低；NOTE 与 COME 共块 |
| DATE | PECMD_FormatTimeString @0x1400a3310 (dc:102066) | src/commands/core_b3r_h4.c:957 | 真体 | 375L/46I/22G/65C ↔ 388L/46I/22G/64C；级联 rb:6495 | 低（直移） |
| DEVI | PECMD_DriverInstall @0x1400474a8 (dc:41278, 19474B) | src/commands/core_b3_remaining.c:1844 | 真体 | 2535L/348I/117G/308C ↔ 2908L/348I/117G/278C（C 差为 helper 合并，I/G/B 全等）；级联 rb:6559 | 低（大体直移） |
| DFMT | PECMD_FormatVolume @0x140078f00 (dc:76803) | src/device/core_b3r_g6.c:55 | 真体 | 208L/23I/3G/46C ↔ 204L/23I/3G/46C；级联 rb:7048 | 低（直移；涉及卷格式化实际写盘，语料覆盖时须防真格式化副作用） |
| DISK | PECMD_DiskFindLoadHives @0x1400d7038 (dc:131901) | src/ui/core_b7c.c:9859 | 真体 | 395L/44I/8G/119C ↔ 414L/44I/7G/124C；级联 rb:6610；无参时委托 SHOW | 低（直移；加载蜂巢路径涉注册表写，注意语料隔离） |
| DISP | PECMD_DispCommand @0x140038d30 (dc:33325) | src/commands/core_b2f.c:5883 | 真体 | 871L/119I/31G ↔ 914L/119I/31G；级联 rb:6809 | 低（直移） |
| DOWN | PECMD_DownCheckShutdown @0x14003d92c (dc:36092) | src/commands/core_b2c.c:1275 | 真体 | 69L/12I/7C ↔ 70L/12I/7C；级联 rb:6699 | 低；但语义涉关机检查，语料覆盖时注意进程生命周期差异（推断） |
| DTIM | PECMD_ParseMsgDispatch @0x140083f74 (dc:84071) | src/device/core_b3r_h1.c:417 | 真体 | 116L/8I/29C ↔ 122L/8I/30C；级联 rb:6703 | 低（直移） |
| EJEC | PECMD_DriveLetterSet @0x14007c88c (dc:79207) | src/device/core_b3r_g7.c:123 | 真体 | 188L/27I/5G/13C ↔ 157L/27I/5G/13C（L 差为声明合并）；级联 rb:6348 | 低（直移） |
| EXIT | 内联分支 dc:44918（退出码位打包） | restored_bodies.c:6926（级联内联块，~117 行） | 真体 | 内联块直移：`error` 检查/EvalParenStripped/VarLookup/13 token 位映射（_SUB…BREAK）逐语句对应 dc | 低；本块是 D-05/D-11 修复域邻接区，回归时留意 |
| FBWF | PECMD_SetFbwfThreshold @0x140020e40 (dc:19706, 114B) | src/commands/core_b2a.c:192 | 真体 | dc 13L/0I/5C ↔ 12L/0I/5C；级联 rb:6528 | 低（dc 本身即小函数） |
| FLNK | PECMD_LinkFile @0x1400969d4 (dc:94842) | src/device/core_b3r_h2.c:320 | 真体 | 178L/23I/39C ↔ 193L/23I/37C；级联 rb:6606 | 低（直移） |
| FONT | PECMD_FontCommand @0x14002f454 (dc:28185) | src/commands/core_b2f.c:2340 | 真体 | 314L/38I/6G ↔ 318L/38I/6G；级联 rb:7056 | 低（直移） |
| GROU | PECMD_ParseWindowPosition @0x1400b9f1c (dc:115329) | src/commands/core_b3r_i28e.c:101 | 真体 | 136L/15I/24C ↔ 141L/15I/25C；级联 rb:7089 | 低（直移） |
| HELP | PECMD_ParseHlpDoc @0x1400403d8 (dc:37692) | src/commands/core_b3_remaining.c:901 | 真体 | 147L/12I/3G/27C ↔ 151L/12I/3G/26C；级联 rb:6778（分支先 FindResourceW 探 0x12e 资源，msvc 重建 exe **天然无该资源** → 与 D-17 同构：资源缺失路径恒走跳转，ParseHlpDoc 在 msvc 镜像内可能永不可达——推断，需语料/活体验证） | 中：HELP 大概率恒跳转兜底（静默无操作），且此风险不修真体也解不了（资源架构性缺失） |
| HIDE | PECMD_WritePELogonDword @0x14001b7b8 (dc:16101, 58B) | src/misc/core_b1_remaining.c:8102 | 真体 | 9L/0I/2C ↔ 9L/0I/2C；级联 rb:6337 | 低（写注册表 Logon 值） |
| HOME | PECMD_HomeSetStartPage @0x1400c42ac (dc:121326) | src/ui/core_b7b.c:117 | 真体 | 77L/6I/20C ↔ 67L/6I/20C；级联 rb:6487 | 低（直移） |
| HOTK | PECMD_HotkeyControl @0x140023640 (dc:21232) | src/commands/core_b2e.c:178 | 真体 | 205L/19I/32C ↔ 207L/19I/33C；级联 rb:6614 | 低（直移） |
| IMAG | PECMD_ImageCommand @0x1400be64c (dc:117862) | src/commands/core_b3r_i28f.c:317 | 真体 | 307L/32I/64C ↔ 337L/32I/65C；级联 rb:6532（仅 4 字动词长时进入） | 低（直移；涉 WIM 挂载类系统副作用，语料需隔离） |
| INIT | PECMD_ProcessInitCommand @0x14002e3d4 (dc:27551) | src/commands/core_b2d.c:1204 | 真体 | 120L/14I/25C ↔ 108L/14I/25C；级联 rb:6887 | 低（直移） |
| ITEM | PECMD_CreateButtonControl @0x1400bd764 (dc:117371) | src/commands/core_b3r_i28e.c:780 | 真体（注①） | 261L/30I/50C ↔ 306L/30I/51C；G 12→2 为 goto 结构化重写；体内 2 处 TODO(verify)（:1032/:1033 寄存器低 32 位清零语义）；级联 rb:6323 | 低-中：控件创建主体直移，2 个未定位寄存器语义点可能在特定参数下错值（推断） |
| KILL | PECMD_CmdKill ≡ FUN_14003cd0c (dc:35605, 1897B) | 转发壳 unimplemented_stubs.c:423 → 真体 FUN_14003CD0C src/commands/core_b2f.c:7575 | 真体 | dc 321L/45I/15G/28C ↔ 真体 331L/45I/15G/28C；级联 rb:6633（分支内先 `*svr` 前缀判断再分流 CmdKill/CloseRestartByName，两真体均在） | 低（直移） |
| LIST | PECMD_AddControlWide @0x1400b4b58 (dc:112226) | src/commands/core_b3r_i28d.c:160 | 真体 | 179L/14I/39C ↔ 175L/14I/40C；级联 rb:6877 | 低（直移） |
| LOAD | PECMD_RunCommand @0x140031454 (dc:29484, 6510B) | 拆分移植：src/lang/core_scriptrun.c:258（段2 主展开，447L）+ src/lang/core_srparse.c `PECMD_SrParsePrefix`（段1 前缀解析，148L 体） | 真体（注②） | 合计 595L/71I/5G/93C ↔ dc 868L/105I/31G/131C；**分支数缺约 1/3，未逐行核（待核）**；已知已记录偏差：段1 dc:241/260「回退 N 字符」以「回退到词起点」等价实现（core_srparse.c:24）；5 处 TODO(verify)（core_scriptrun.c:311/416/552/625/649）；sysinit_end 链已实现（:316-319, :668-673）；级联 rb:6380 | 中：LOAD 是脚本调度主链，真体但 ① 词回退近似 ② 1/3 分支差未定性 ③ 5 处 TODO——语料覆盖大概率暴露分支类 FAIL（与 R24 剩余 FAIL 同型），而非静默 |
| LOCK | PECMD_LoadTasksWait @0x1400a4504 (dc:102777) | src/commands/core_b3r_h4.c:1352 | 真体 | 335L/40I/54C ↔ 350L/40I/55C；级联 rb:6576 | 低（直移） |
| LOGO | PECMD_LaunchCommand @0x140046840 (dc:40850) | src/commands/core_b3_remaining.c:1363 | 真体 | 85L/5I/7C ↔ 88L/5I/7C；级联 rb:6761 | 低（直移） |
| LOOP | PECMD_ExecPathList @0x140035860 (dc:31386) | src/commands/core_b2c.c:999 | 真体 | 107L/12I/14C ↔ 110L/12I/14C；级联 rb:6689 | 低（直移） |
| LPOS | PECMD_SearchStringAndLocate @0x1400a0d38 (dc:100616) | src/commands/core_b3r_h3.c:430 | 真体 | 444L/67I/5G ↔ 485L/66I/5G（I 67→66 微差，待核但量级 1）；级联 rb:6681 | 低（直移） |
| MAIN | PECMD_ScriptMainEntry @0x140045c90 (dc:40513) | restored_bodies.c:11712（真体直移注记） | 真体 | 331L/47I/66C ↔ 341L/46I/70C（I 47→46 微差，待核）；级联 rb:6714 | 低（直移；MAIN 重启链涉进程生命周期，语料注意） |
| MEMO | PECMD_MsgCommand @0x140083274 (dc:83576) | src/device/core_b3r_h1.c:151 | 真体 | 247L/27I/49C ↔ 261L/27I/50C；级联 rb:6765 | 低（直移） |
| MENU | PECMD_CreateMenuItem @0x1400a600c (dc:103829) | src/commands/core_b3r_h4.c:724 | 真体 | 209L/29I/44C ↔ 227L/29I/47C；级联 rb:7085 | 低（直移） |
| MESS | PECMD_MessShowMsgBox @0x1400bb718 (dc:116238, 4933B) | src/commands/core_b3_remaining.c:28049 | **恒0桩** | msvc 16L/0I/1C 恒返 0；SKIP 注记（GUI 控件体文本编辑器，4933B，dc 743L/111I/68C）；全仓无第二真体；级联 rb:6903；**内部调用点 ×2 同吃此桩**（core_b3l.c:1385、core_calc_expr.c:983） | 高置信静默无操作：弹窗恒不显示、恒返 0（原版返回控件状态码）→ 依赖其返回值的流程分支错向 |
| MSTR | PECMD_MstrStringOps @0x1400a2390 (dc:101435, 3966B) | unimplemented_stubs.c:182 | **恒0桩** | 1L 恒返 0；dc 622L/88I/67C 全缺；全仓无第二真体；级联 rb:6844 | 高置信静默无操作：MSTR 变量串操作全无效且恒返 0，脚本产出错值不报错，排查极难 |
| NAME | PECMD_ParseExtPathArg @0x140096880 (dc:94784) | src/device/core_b3r_h2.c:182 | 真体 | 52L/5I/15C ↔ 51L/5I/15C；级联 rb:6477（与 MANF 共用 LAB_14004d206，rb:6680 同款） | 低（直移） |
| NOTE | 内联分支 dc:44497（goto LAB_14004ce16） | restored_bodies.c:6504（与 COME 共块） | 真体 | 同 COME（g_charTableF 置位逻辑） | 低 |
| NTPC | PECMD_SntpResolveServer @0x140072e94 (dc:72637) | src/device/core_b3r_g4.c:136 | 真体 | 115L/10I/23C ↔ 118L/10I/22C；级联 rb:6365 | 低（涉网络 SNTP 请求，语料需隔离/离线判定） |
| NUMK | PECMD_SetNumLockState @0x14001ad30 (dc:15498, 120B) | src/misc/core_b1_remaining.c:7741 | 真体 | 24L/3I/1C ↔ 22L/3I/1C；级联 rb:6511 | 低（直移） |
| PAGE | PECMD_CreatePageFile @0x14002a910 (dc:25444) | src/commands/core_b2e.c:778 | 真体 | 204L/18I/28C ↔ 202L/18I/27C；级联 rb:6468 | 低（直移；实际创建页面文件，语料需隔离） |
| PBAR | PECMD_ParseControlCommand @0x1400ab51c (dc:106950) | src/commands/core_b3r_i28a.c:562 | 真体 | 64L/7I/12C ↔ 65L/7I/12C；级联 rb:6814 | 低（直移） |
| PCIP | PECMD_PcipCommand @0x1400aee2c (dc:108815) | src/commands/core_b3r_i28c.c:123 | 真体 | 619L/75I/149C ↔ 682L/77I/158C（微差待核）；级联 rb:6774 | 低（直移） |
| PINT | PECMD_PinStartMenuTask @0x1400987ec (dc:95851) | src/commands/core_b3_remaining.c:23076 | 真体 | 750L/85I/113C ↔ 824L/86I/108C；级联 rb:7052 | 低（直移） |
| PUTF | PECMD_DdCopyCommand @0x1400d2e90 (dc:129678, 11447B) | restored_bodies.c:8037（体 1552L） | 真体（注③） | dc 1547L/234I/53G/175C ↔ 体 1552L 直移；**依赖 2 个 leaf 子桩**：`PECMD_HandleDuplicateValid`（恒返 NULL，restored_bodies.c:8036）/`PECMD_NormalizeDiskDevicePath`（恒返 '\0'，:8037 行内）——段盘容量查询/盘设备名归一路径缩水（推断） | 中：主体直移；dd 复制涉真实写盘，且磁盘容量查询路径恒返回失败 → 特定目标盘参数下行为分叉（推断） |
| RAMD | PECMD_RunRamdriv @0x14002fc48 (dc:28505) | src/commands/core_b2b.c:1148 | 真体 | 51L/5I/11C ↔ 50L/5I/12C；级联 rb:6399 | 低（直移；涉 RAM 盘创建系统副作用） |
| RAND | PECMD_EvalRandFunction @0x1400a3f08 (dc:102447) | src/commands/core_b3l.c:1184 | 真体 | 40L/3I/10C ↔ 38L/3I/10C；级联 rb:6403 | 低（直移；随机值语料须做值域对拍非精确值对拍） |
| SBAR | PECMD_SbarAddControl @0x1400c56fc (dc:122089) | src/ui/core_b7c.c:445 | **恒0桩** | 8L 恒返 0（自标「简化桩」但体纯返 0 ≡ no-op）；dc 162L/13I/34C；级联 rb:6821 | 高置信静默无操作：状态栏控件不创建 |
| SCRN | PECMD_ScreenAreaCommand @0x140097c70 (dc:95450) | src/device/core_b3r_h2.c:836 | 真体 | 393L/54I/47C ↔ 404L/54I/47C；级联 rb:6749 | 低（直移；改屏幕分辨率，语料需隔离） |
| SEND | PECMD_SendInputEvents @0x14003c9e8 (dc:35494) | src/commands/core_b2d.c:1995 | 真体 | 103L/10I/18C ↔ 105L/10I/18C；级联 rb:6407 | 低（直移；注 SendInput 系统级键鼠注入，语料须防污染宿主） |
| SERV | PECMD_ServiceControl @0x140020018 (dc:19175) | src/commands/core_b2f.c:174（:103 为声明） | 真体（注④） | dc 392L/47I/57C ↔ 490L/48I/93C（msvc 侧 helper 展开，I/G/B 对齐）；1 处 TODO(verify)（:170 返回值 unaff_RBP 按 0 处理）；级联 rb:7068 | 低-中：服务启停涉系统状态，返回值 TODO 点可能在错误路径产生错误码偏差（推断） |
| SHOW | PECMD_PartShowHideDrive @0x1400cd3a8 (dc:126546, 12480B) | src/ui/core_b7c.c:6496（:9857 为前向声明） | 真体（注⑤） | dc 1674L/256I/67G ↔ 2293L/258I/72G（SEG1-8 分段注记）；10 处 TODO(verify)（含 :6798「近似体」、返回值 unaff_RBX 保守取 0）；级联 rb:7102 | 低-中：主体直移；盘符显隐涉注册表/挂载点系统副作用，返回值契约点可能影响后续 IFEX 判定（推断） |
| SHUT | PECMD_ShutPowerAction @0x14003db00 (dc:36170) | src/commands/core_b2e.c:2288 | 真体 | 191L/25I/35C ↔ 209L/25I/36C；级联 rb:7073 | 低（直移；**关机/重启终局副作用**，语料覆盖即杀运行环境——覆盖设计须 mock 或子进程隔离） |
| SITE | PECMD_SiteSetFileAttrTime @0x1400d0468 (dc:128226) | src/ui/core_b7c.c:489 | **恒0桩** | 10L 恒返 {0,0}（自标「简化桩」）；dc 265L/31I/39C；级联 rb:6499 | 高置信静默无操作：文件时间/属性设置无效 |
| SLID | PECMD_ParseControlCenterArgs @0x1400abc74 (dc:107179) | src/commands/core_b3r_i28b.c:271 | 真体 | 145L/11I/32C ↔ 146L/11I/33C；级联 rb:6342 | 低（直移） |
| SOCK | PECMD_SockCreateIpcObj @0x1400e1228 (dc:137534, 8287B) | src/ui/core_b8_remaining.c:41 | **恒0桩** | 9L 恒返 0（自标「简化桩」+TODO(verify) 需完整还原）；dc 1156L/158I/180C；级联 rb:6580 | 崩/静默双风险（推断）：IPC 对象创建恒返 0，后续对「对象」的解引用/use 可能 AV，或整链静默失败 |
| SPIN | PECMD_SpinAddControl @0x1400c5300 (dc:121962) | src/ui/core_b7c.c:432 | **恒0桩** | 8L 恒返 0（同 SBAR 模式）；dc 121L/9I/31C；级联 rb:6736 | 高置信静默无操作：spin 控件不创建 |
| SUBJ | PECMD_DefineDosDevice @0x140076144 (dc:74939) | src/commands/core_b3_remaining.c:17813 | 真体 | 171L/24I/25C ↔ 172L/24I/25C；级联 rb:6572 | 低（直移；盘符映射系统副作用） |
| SWIN | PECMD_SubCommand @0x1400ab724 (dc:107020) | src/commands/core_b3r_i28b.c:113 | 真体 | 153L/11I/36C ↔ 152L/11I/32C；级联 rb:6742 | 低（直移；msvc 函数名 SubCommand 是启发式标签，与 SWIN 语义无涉） |
| TABL | PECMD_TablCreateControl @0x1400caf78 (dc:125345) | src/ui/core_b7c.c:6082 | 真体 | 272L/27I/11G ↔ 310L/28I/9G（goto 结构化微调）；级联 rb:6618 | 低（直移） |
| TABS | PECMD_AddControlStar @0x1400b48e0 (dc:112145) | src/commands/core_b3r_i28d.c:77 | 真体 | 75L/5I/19C ↔ 77L/5I/19C；级联 rb:6856 | 低（直移） |
| TEXT | PECMD_ShowScreenText @0x140046ac4 (dc:40941) | src/commands/core_b3_remaining.c:1452 | 真体 | 262L/35I/30C ↔ 261L/35I/29C；级联 rb:7081 | 低（直移） |
| THRD | PECMD_LaunchAsyncScriptThread @0x1400b0bf8 (dc:109815) | src/commands/core_b3r_i28c.c:846 | 真体 | 482L/50I/8G ↔ 497L/50I/8G；级联 rb:6424（THREAD 6 字前缀校验后共用 LAB_14004c9cc） | 低（直移；异步线程时序类对拍须留裕量，推断） |
| TIPS | PECMD_LoadIconAndTooltip @0x1400ba35c (dc:115471) | src/commands/core_b3r_i28e.c:252 | 真体 | 428L/64I/22G ↔ 480L/64I/27G；级联 rb:6894 | 低（直移） |
| TREE | PECMD_ParseControlDef @0x1400bdf6c (dc:117638) | src/commands/core_b3r_i28f.c:76 | 真体 | 218L/20I/4G ↔ 231L/19I/2G（微差待核）；级联 rb:6463 | 低（直移） |
| USER | PECMD_UserCmdHandler @0x14001af0c (dc:15584, **7B**) | unimplemented_stubs.c:172 | **恒0桩** | dc 体 6L：仅 `FUN_14001ada8(param_1,0)` 一跳（dc:15525，346B，逗号分隔参数解析 + FUN_14007bf44 逐段处理，**有真实副作用**）；msvc 桩恒返 0 且 FUN_14001ada8 全仓无实现；`core_skip_registry.c:17` 按 size=7 判 SKIP；级联 rb:6828 | 高置信静默无操作 + **非平凡跳过**：原版 USER 行有副作用（解析并执行参数段），msvc 完全无操作——恢复需连同 FUN_14001ada8 一起真体化 |

注①–⑤：真体体内残留的局部风险点（均为已标注 TODO(verify)/子桩，不改变四级判定）：
- 注② LOAD：dc:241/260 词回退近似 + 分支数差 1/3 待逐行核（本轮 census 口径，见 §4 局限 2）。
- 注③ PUTF：leaf 子桩 ×2（restored_bodies.c:8036-8037），dc 对应 helper 的真实语义未核。
- 注④ SERV：返回值 unaff_RBP 按 0（core_b2f.c:170）。
- 注⑤ SHOW：返回值 unaff_RBX 保守取 0 + :6798 近似体（与 D-06 unaff_ 寄存器残留家族同型）。
- 注① ITEM：:1032/:1033 寄存器低 32 位清零语义 TODO。

## 3. 建议优先真体化 Top5（常见度×风险，主观排序）

| # | 动词 | 理由 |
|---|------|------|
| 1 | **MESS** | 静默面最大：除动词分支外还有 2 个内部调用点（core_b3l.c:1385 / core_calc_expr.c:983）被桩污染——任何依赖弹窗反馈/其返回值的链路都已隐性失真。dc 4933B 但桩内 SKIP 注记已完成失效路径分析，且调用方有 NULL 回退，恢复后回归风险可控 |
| 2 | **MSTR** | 文本/变量处理主力动词，3966B 全缺、恒返 0：脚本产出错值不报错，是最难排查的静默错误型；后续语料一旦覆盖 MSTR，FAIL 无法与其它变量链缺陷隔离 |
| 3 | **SOCK** | 8287B 全缺且返回「对象」恒 0：与 MSTR 同为静默型但多一层**解引用崩溃风险**（后续对 0 句柄/对象的使用，推断）；IPC 链一旦被语料踩中可能直接把对拍变成 dump 分诊 |
| 4 | **ADSL** | 唯一「看似真体实缩水」：census 硬证据（33/104 分支），与 D-01/R25-c 同族——不登记极易被后续轮次当成已完成而跳过；建议先做 dc 逐行补齐或降级标注为桩，二选一消除「假真体」状态 |
| 5 | **USER** | 恢复性价比最高：dc 入口仅 7B + helper 346B，两函数合计规模小；且当前 stub 与原版存在**真实行为差**（副作用链全丢），属「非平凡跳过」需从 divergences 挂账中除名的最小代价项 |

（SBAR/SPIN/SITE 同为恒0桩但触发面窄、模式一致，可打包成一个「b7c 控件/时间桩批」随后处理。）

## 4. 方法与局限

方法（全部静态，可复现）：

1. **分发映射**：`restored_bodies.c` `PECMD_ProcessScriptBlock`（↔ dc `FUN_14004c0bc @0x14004c0bc`）内 `local_158 == 0xXXXXXXXX` FourCC 直比级联；67/67 动词分支全部定位（脚本对 `include/pecmd_fourcc.h` 的 98 值做大小写不敏感匹配，表外动词不计入本轮口径）。
2. **定义定位**：对每动词处理函数全仓 grep 定义（src/ + restored_bodies.c + unimplemented_stubs.c + win32_api_stubs.c + crt_shims.c + include/），区分定义/extern 声明/转发壳。
3. **census 证据**：对 dc 与 msvc 函数体（花括号配平提取）统计 `L=行数 / I=if( / G=goto / B=break / C=PECMD_|FUN_ 调用数`；真体判据 = I/G/B 全等（或微差可解释）且体含 dc 直移特征；恒0桩判据 = 体为 `(void)…return 0` 型且全仓无第二定义（逐个排查 D-01 式桩遮蔽）。
4. **dc 地址**：`docs/provenance.map`；dc 函数尺寸：`reference/functions.csv` size 列；dc 体起始行由脚本在 `reference/decompiled.c` 定位。

局限（读表须知）：

1. **census 相等 ≠ 语义等价**：寄存器残留（unaff_）、变参丢弃、缓冲区尺寸、ABI 契约类失真不体现在分支计数上——「真体」只保证控制流骨架直移，逐语句忠实性以各文件 dc 直移注记与既往审计为准（如 D-03/D-14 类缺陷均诞生于「真体」内）。
2. **LOAD 分支差未定性**：dc 105I vs msvc 合计 71I，可能是段1/段2 重构合并也可能真缺分支；本轮未逐行核对，标「待核」。
3. **微差未逐条核**：LPOS(I−1)/MAIN(I−1)/TREE(I−1,G−2)/TABL(G−2)/ITEM(G 12→2) 等结构化 goto 重写未逐条对照 dc 原文。
4. **PUTF leaf 子桩**：`PECMD_HandleDuplicateValid`/`PECMD_NormalizeDiskDevicePath` 在 dc 中的对应 helper 真实语义未核，只登记了桩位置。
5. **风险列全部是推断**：未运行任何 EXE、未做对拍；「崩/错/挂/静默」按实现状态与函数职责推断，置信度已逐行标注（高置信=体为恒0/无操作；推断=依赖后续行为链）。
6. **HELP/资源类**：msvc 重建 exe 不含 INDATA/HLP 资源（D-16/D-17 已登记架构级断裂），HELP 分支的真体性不受此影响，但其可达性在 msvc 镜像内存疑——该类问题不在本表修复范围。
7. **行号时效**：所有 file:line 绑定本报告生成时的工作树快照；任何重构/重链后引用本表须按 divergences.md §0 规则复核。
8. **动词语义名**：SWIN→PECMD_SubCommand、SUBJ→PECMD_DefineDosDevice 等 rename_map 标签为启发式命名，本表一律以 FourCC+dc 地址为准；四字/三字动词与 `uVar21` 长度前缀（如 0x303/0x404）的对应关系未展开核验（不影响本轮判定）。
