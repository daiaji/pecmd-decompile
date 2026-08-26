# S14 结构化重构 · 只读预研报告（s19_s14_preflight.md）

- 角色：只读预研子代理（本文件为**唯一写入产出**；未修改任何源码/构建脚本，未运行 EXE，未动 git/调试器）
- 日期：会话时钟 2026-08（S14 立项后、T4 收敛前的只读侦察）
- 服务对象：PRODUCTION_ROADMAP.md §S14「结构化重写与现代化」（拆巨函数 / FourCC 可读化 / 去 goto / 行为文档四波次 S14-a~d）
- 前置条件现状：**T4 语料全 PASS 尚未达成**（实测 43 案 = 16 PASS / 27 FAIL，见 §5.1）——因此本报告只产出"动刀地图"，不动任何代码。

## 0. 执行摘要

| 指标 | 实测值 | 口径 |
|---|---|---|
| 扫描范围 | `refactored/src/**/*.c` 95 文件 + `restored_bodies.c`（18,685 行），共 **96 TU / 2,100 个函数定义** | 花括号深度扫描（注释/字符串消毒后计数），边界与审计锚点偏差 ≤10 行 |
| ≥800 行巨函数 | **18 个**；≥400 行 52 个 | 行数口径 |
| >2000 token（chars/4 估算） | **112 个**（token 为估算口径，见 §1.1 SKIP 注记） | 任务双阈值之第二阈值 |
| 第一巨物 | **PECMD_ExecCmdDispatch（ECD）= 3,961 行**（rb:260–4220）——比任务假设的"三大巨物"体量更极端 | — |
| FourCC 常量 | 全树 8 位 hex 且四字节可打印 ASCII：**137 处命中 / 133 个互异值**；其中 **129 值集中在 PSB 单函数内**，对应 s12 matrix 权威口径的 **117 个逻辑动词** + switch 降级边界伪影 | 见 §2 |
| goto 总量 | **2,287 条 goto / 321 个函数**（15.3% 函数带 goto）；目标标签 LAB_*（Ghidra 流水线伪影）占 **75.4%**，手写命名标签占 24.6% | 见 §3 |
| 保护网现状 | TEMP PROBE 探针仅覆盖 **16 个函数**；43 案语料 16 绿 27 红；冷区 UI/DISK/RAMD 巨函数**零保护裸奔** | 见 §5 |

一句话结论：**S14-b 的动刀对象体量比预期大一个量级（ECD≈4000 行而非"千行级"）；FourCC 高度集中使 S14-a 成为低风险快赢；goto 三分之二是机械伪影可批量顺序化；但 18 个巨函数中仅 4~5 个有活体保护网——必须按 §5.3 的"先建保护后动刀"批次执行。**

---

## 1. 巨函数清单（任务 1）

### 1.1 方法与口径

- 扫描器：PowerShell 花括号深度扫描（先对源码做注释/字符串字面量消毒再计数 `{`/`}`，函数签名=depth==0 处含 `(` 的头块）。已用已知锚点校验：
  - PSB 实测 rb:**5828–7184**（1,357 行）↔ 任务假设 "rb:5822 起 ≈1400 行" ✅ 吻合（±10 行为注释归属差异；s11_psb_loop_audit 记 5821–7172）；
  - ECD 实测 rb:**260–4220** ↔ 任务 "rb:253 起" ✅（253–255 为前置注释块）；
  - RunCommand 实测 core_scriptrun.c:**261–731 = 471 行** —— ⚠️ **不满足 >800 行阈值**，保守修正注记见 §6。
- token 估算：`tokens ≈ chars/4`（LLM BPE 经验系数）。>2000 token ⇒ 体 >8KB 的稠密函数共 112 个，其中绝大多数同时 <800 行——该阈值实际筛出的是"中型稠密函数"集合，**排序价值以行数+分支密度为准**（下表 Score）。
- 拆分价值分 `Score = 行数 + 25×FourCC互异值数 + 8×goto总数`（动词分臂潜力与控制流纠缠度的加权；热路径加成在 §5 定性给出，不入公式）。

### 1.2 Top 20（按拆分价值降序）

| # | 函数 | 文件:起止行 | 行数 | tok≈ | goto(L/N)¹ | FourCC | dc 锚点 / 职责推断 |
|---|---|---|---|---|---|---|---|
| 1 | PECMD_ExecCmdDispatch (ECD) | restored_bodies.c:260–4220 | **3961** | 42.5k | 140/1 | 0 | @FUN_14000e26c (dc:7726-)；命令执行总兜底：boot 开关(install/service/-shel)、`"PECMD EXEC "` 重入(rb:564)、进程启动优先级/桌面/注册表 Run 键/winpe 引导链 |
| 2 | PECMD_ProcessScriptBlock (PSB) | restored_bodies.c:5828–7184 | **1357** | 15.1k | 52/0 | **129** | @FUN_14004c0bc (dc:43816–45163)；单行脚本处理：^转义/%变量展开×10 上限/FourCC 动词分发主表（117 动词宇宙唯一宿主） |
| 3 | PECMD_DriverInstall | src\commands\core_b3_remaining.c:1845–4749 | **2905** | 39.5k | 117/1 | 0 | @FUN_1400474a8；DEVI 设备安装驱动族（SetupDi/CmGetDevNodeStatus 函数指针全局群） |
| 4 | PECMD_PartShowHideDrive | src\ui\core_b7c.c:6490–8781 | **2292** | 37.7k | 70/4 | 0 | @FUN_1400cd3a8；SHOW 盘符显隐：SEG5~9 分段注释已在（物理盘枚举/GPT 过滤/#1..#5 态迁移重扫） |
| 5 | PECMD_EnviMemReadWrite | src\ui\core_b7c.c:2008–3805 | **1798** | 20.6k | 96/1 | 0 | @FUN_1400c6324；ENVI 内存读写变体 + 类型名后置修饰 s/0/@ 共用段 |
| 6 | PECMD_RegiEditRegistry | src\ui\core_b7c.c:551–1887 | **1337** | 14.7k | 71/12 | 0 | @FUN_1400c13f8；REGI 两遍扫描（原行→展开重扫），'='写/'!'删/','查 |
| 7 | FUN_1400d9818 | restored_bodies.c:16126–17232 | 1107 | 13.7k | 57/1 | 0 | @0x1400d9818；INDIR 虚表槽驱动的控件宿主分发（param_4 POINT 按位重解释注记在案） |
| 8 | PECMD_EvalLoopCondition | src\commands\core_b2f.c:2662–3723 | 1062 | 11.3k | 60/1 | 0 | @FUN_140032dc4；FOR/LOOP 大型 IF 条件求值器（CONCATxx 已整数化） |
| 9 | PECMD_LoadObjectIcon | src\commands\core_b3_remaining.c:26387–27455 | 1069 | 11.2k | 50/0 | 0 | @FUN_1400b6e24；GUI 对象图标加载（HBITMAP 族） |
| 10 | PECMD_RegexReplace | src\commands\core_b3_remaining.c:6078–7327 | 1250 | 15.3k | 13/0 | 0 | @FUN_14005182c size=7902；正则替换引擎 |
| 11 | FUN_1400f6db0 | restored_bodies.c:17389–18519 | 1131 | 12.0k | 5/0 | 0 | @0x1400f6db0 size=8526；列表宿主自绘引擎（WM_DRAWITEM 网格/进度底色/多子项布局），零直接调用者（虚表槽 PTR_FUN_14012c670 家族） |
| 12 | PECMD_CodeConvertCommand | src\commands\core_cmd4.c:94–1047 | 954 | 8.7k | 0/26 | 0 | @FUN_140088300；CODE 编码转换命令（L_var_path/L_data_ok 等**手写语义标签**密集区） |
| 13 | PECMD_DispCommand | src\commands\core_b2f.c:5257–6168 | 912 | 9.7k | 31/1 | 0 | @FUN_140038d30；DISP 显示命令主体 |
| 14 | PECMD_RamdMountImDisk | src\ui\core_b8_remaining.c:146–1176 | 1031 | 10.2k | 16/1 | 0 | @FUN_1400e9724；RAMD 参数解析→imdisk 命令行构造（'*'规格/','字段/proxy-devio 共享内存路径） |
| 15 | PECMD_ExpressionArithmetic | src\commands\core_b3_remaining.c:16236–16788 | 553 | 5.3k | **0/67** | 0 | @FUN_140070da8；CALC 表达式算术核——**全树 goto 密度冠军**（67 个 L_hexaddr 标签跳转表状态机） |
| 16 | PECMD_ReadTextLine | src\commands\core_b3r_h3.c:924–1817 | 894 | 10.4k | 16/0 | 0 | @FUN_14009da54；READLINE 读行：BOM/编码识别、*fix *nl *left 修饰、多态栈槽注记在案 |
| 17 | PECMD_WriteFileEncoded | src\commands\core_b3r_h3.c:1826–2634 | 809 | 10.1k | 19/0 | 0 | @FUN_14009f070；WRITE 写文件：codepage 判定/*fix *v *fv *c *sparse *nobom/BOM 写入 |
| 18 | FUN_140100ac4 | restored_bodies.c:15391–16113 | 723 | 8.5k | 36/0 | 0 | @0x140100ac4；控件 VAL/CHECK/ENABLE 属性应用 + 鼠标定位（INDIR 虚表槽；static 承载避同名空桩注记在案） |
| 19 | PECMD_ParseImdiskMountArgs | src\commands\core_b2f.c:3859–4654 | 796 | 11.3k | 21/2 | 0 | @FUN_1400369d0；RAMD -a/-e/-l/-s/-v 选项解析（CONCAT44/extraout 近似 TODO 在案） |
| 20 | PECMD_PinStartMenuTask | src\commands\core_b3_remaining.c:23071–23893 | 823 | 11.2k | 16/0 | 0 | @FUN_1400987ec；开始菜单/任务栏固定 |

¹ goto 列格式 `LAB_目标数/命名标签数`。

**落选注记（≥800 行但排名 21+）**：DescribePartitionInfo(752)、PatternMatch(546, goto 密度 0.082)、HelpDlgProc(816)、FUN_14003ED4C(734)——均在 §3/§5 中另行引用。

---

## 2. FourCC 分布与「常量→动词名」对照表生成方案（任务 2）

### 2.1 全树普查结果

- 匹配口径：`\b0x[0-9a-fA-F]{8}\b` 且按小端解码 b0..b3 四字节全部落在可打印 ASCII (0x20–0x7E)。
- 结果：**137 处命中 / 133 个互异值**。分布极度集中：

| 宿主函数 | 互异值数 | 命中数 | 性质判定 |
|---|---|---|---|
| **PECMD_ProcessScriptBlock**（rb:5828–7184） | **129** | 130 | 动词分发主表：`local_158 == 0x????????` if-else 链 |
| PECMD_FindPartitionStartSector（b3_remaining:18691–18772） | 3 | 4 | **非动词数据签名**：MBRS(0x5352424D)/UPLS(0x534C5055)/FBBF(0x46424246)——分区表/引导扇区魔数 |
| PECMD_GenerateTimeText（b3_remaining:8121–8778，即 rb 同名站点 8319/8358） | 1 | 2 | FBBF 结构体魔数复核（buffer+0xda），非动词 |
| PECMD_FormatTimeString（core_b3r_h4.c:958–1344） | 1 | 1 | 0x77777777('wwww') 为时间换算**哈希乘数假阳性**，排除 |

- 与权威口径对账：s12_verb_coverage_matrix.md §0 记 PSB FourCC 层全集 = **117 逻辑动词**（3 字母×3 [SED/DIR/SET] + 4 字母×113 + 6 字母×1 [THREAD]）。本普查 PSB 129 值 − 117 ≈ 12 个差额全部为 **Ghidra switch 降级伪影**：
  - 二分边界比较 `local_158 < 0x????????` 不是动词（抽样实锤：rb:6532 `<SEGI>`、rb:6458 `<DOME>`、rb:6697 `<MPOS>`、rb:6705 `<MOOP>`、rb:6820 `<MSTR边界>`）；
  - **+1 边界对**（真动词 X 与边界值 X+1 成对出现）：ITEM/JTEM(rb:6339–6340)、DOWN/EOWN、LOOP/MOOP、INIT/JNIT、LIST/MIST、SIZE/TIZE、SERV/TERV、NUMK/OUMK、SLID/TLID 共 **9 对**；
  - THREAD 特殊处理守卫 `!= THRE`(rb:6249) + 后缀 'D' 复核（audit §1 表 "THREA+D(0x606)"）。
  - ⚠️ 注意 EJEC(0x43454A45) 抽样为 `==` 直比（rb:6365）→ **疑似矩阵外真实动词**，登记待核（§6）。

### 2.2 「常量→动词名」对照表生成方案（建议脚本化）

**数据流**（零手工转录，全程机器可验证）：

```
restored_bodies.c (PSB 区间 rb:5821–7184)
   │  正则①：\(\s*local_158\s*==\s*0x([0-9a-f]{8})\s*\)   → 动词集合 V（== 直比才是动词）
   │  正则②：< / > 形态                                      → 边界伪影集合 B（丢弃，留档 diff 用）
   │  断言①：len(V) == 117 （对账 s12_verb_coverage_matrix §5；不等即 FAIL，人工看新增）
   │  小端解码 v → 'TEAM' 式 ASCII 名；冲突/不可打印 → 报错
   ▼
tools/gen_fourcc.py  ──▶  include/pecmd_fourcc.h
   enum {
     FOURCC_ENVI = 0x49564E45, /* ENVI */
     FOURCC_TEAM = 0x4D414554, /* TEAM */
     ...                  /* 117 项 + 3 个三字母(SED/DIR/SET 手工别名表) */
   };
   附 static inline void pecmd_fourcc_to_str(uint32_t v, wchar_t out[5]);
   附 // GENERATED-FROM: restored_bodies.c@<git-sha> — 再生成前禁止手改
```

- 三字母 SED/DIR/SET 与 THREAD 不落入 8-hex 口径（打包形式不同，audit §2 记 0x303/0x606 形态）→ 生成器内置 **4 项手工别名补丁表**，其余全自动。
- 交叉验证源：`tools/rename_map.json`（1594 符号）不含动词条目（s12 matrix §1.1 已勘误），故动词条权威源只有两处：① PSB `==` 比较集（本方案①）；② `build/msvc/s12_verb_coverage_matrix.md` §5 表格（Markdown 解析作第二意见）。两者 diff 必须为空或逐项归因。
- 落地方式（S14-a 波次）：`pecmd_fourcc.h` 全树 include 后，把 PSB 内 `local_158 == 0x49564E45` 机械替换为 `local_158 == FOURCC_ENVI`——纯宏等价替换，行为中性证明 = 双绿门 + 43 案 verdict 逐案不变。
- 边界伪影集合 B 不替换（保留 `< 0x…` 原样并加一行注释"switch 降级边界，勿改名"），避免误导后来者以为存在 SEGI/DOME/MPOS 动词。

### 2.3 全量 133 值清单（附录 A 有紧凑版）

完整 `hex→ASCII/hits/函数` 清单见附录 A；生成脚本落地后此附录即可废弃（以 pecmd_fourcc.h 为准）。

---

## 3. goto 普查与分类（任务 3）

### 3.1 总量与密度

- 全树 **2,287 条 goto**，分布于 **321/2100 个函数**（15.3%）；整体密度 ≈ 1.09 goto/函数、0.031 goto/百行级。
- **按条数 Top 5**：

| 函数 | 文件 | 行数 | goto | 其中 LAB_* 目标 | 密度(goto/行) |
|---|---|---|---|---|---|
| PECMD_ExecCmdDispatch | restored_bodies.c | 3961 | **140** | 139 | 0.035 |
| PECMD_DriverInstall | core_b3_remaining.c | 2905 | **117** | 116 | 0.040 |
| PECMD_EnviMemReadWrite | core_b7c.c | 1798 | **96** | 95 | 0.053 |
| PECMD_RegiEditRegistry | core_b7c.c | 1337 | 83 | 71 | 0.062 |
| PECMD_PartShowHideDrive | core_b7c.c | 2292 | 70 | 66 | 0.031 |

- **按密度 Top 3**（≥500 行函数中）：① ExpressionArithmetic **0.121**（67/553）② PatternMatch **0.082**（45/546）③ RegiEditRegistry 0.062（83/1337）。

### 3.2 伪影 vs 手写分类

| 类别 | goto 目标计数 | 标签定义计数 | 判定 |
|---|---|---|---|
| **G-LAB_raw**（`LAB_1400xxxxx:` 原样保留） | **1724（75.4%）** | 1031 个标签 | Ghidra 流水线伪影：直移产物，多目标汇聚 cleanup/error 链 |
| **H-semantic**（done/cleanup/L_err/L_data_ok 等语义名） | 563（24.6%） | 286 个标签 | 移植期手写/半手写（部分带 `/* LAB_xxx 入口 */` 对照注释，如 core_cmd4.c L_output） |
| **S-L_hexaddr**（`L_43a:` 式地址截断名） | （计入上类） | 27 个标签 | 半伪影：人改过名字但仍地址语义，集中在 ExpressionArithmetic（跳转表状态机） |

- 手写 goto 最密文件：core_b3_remaining.c(145)、core_b2e.c(73)、core_execline.c(43)、core_b8m.c(29)、core_b3r_i28e.c(29)、core_cmd4.c(26)。restored_bodies.c 自身仅 14 条命名 goto（其余全 LAB_*）。

### 3.3 可安全消除的模式分类（S14-c 只做前三类）

| 模式 | 典型站点 | 安全消除手法 | 风险 |
|---|---|---|---|
| **P1 cleanup/done 汇聚链**（多点 goto 单一出口释放） | ECD/DriverInstall/EnviMemReadWrite 内 LAB_ 密集段；RunScriptText convText 兜底(core_scriptrun.c:251) | 顺序化：提取每臂尾部重复序列为 helper，或将资源收进单一 exit 块（C 无 defer，保持单出口结构即可） | 低——纯控制流重排，diff 可证 |
| **P2 error→exit 短路** | RegiEditRegistry 12 条命名 err 标签；CodeConvertCommand L_err1/L_invalid | 提取参数校验前置 guard 子函数（早返回） | 低-中——需保证求值顺序副作用不变 |
| **P3 循环内 break/continue 替代** | EvalLoopCondition、PatternMatch 的 LAB_ 出多环 | while(true){…break} → 显式循环条件/标志位 | 中——循环边界条件改写须逐条对照 dc |
| **P4 跳转表状态机（保留不消）** | ExpressionArithmetic 67 个 L_hexaddr 互相跳；DispCommand 运算符分派 | **保留 do-while(state) 或显式 state 机**——这是反编译 switch 降级的忠实形态，强行结构化反而引入行为风险 | —（不动） |
| **P5 跨段长程跳转（保留不消）** | ECD boot 开关段跨千行 LAB_；PSB 通用回退 LAB_14004df3c | 仅加注释锚点（dc 行号对照），不做控制流手术；留给对应波次拆臂时自然消解 | —（不动） |

---

## 4. 三大巨函数行为文档骨架（任务 4）

> 用途：S14-d 波次逐函数填写模板。骨架中已按现有证据预填"输入/锚点/分支表"首列，空格处为重构时必填项。填写完成的文档放 `docs/behavior/<函数名>.md`。

### 4.1 PECMD_ExecCmdDispatch（ECD）@FUN_14000e26c

```markdown
# 行为文档：PECMD_ExecCmdDispatch（ECD）
## 0. 身份
- 原址 @0x14000e26c ｜ dc:7726- ｜ 移植体 restored_bodies.c:260–4220（3961 行）
- 角色：Layer-B 命令执行总兜底（s12 matrix §1.2）；EXEC 动词二次分发宿主
## 1. 输入契约（8 参）
- param_1: script ctx 指针 ｜ param_2: 行文本 ｜ param_3/4: 打包标志
  （⚠ D-05：rb:6297/6306-6310 第 4 参高 48 位栈垃圾历史缺陷，重构时必须保打包路径逐位一致）
- param_5: uint ｜ param_6/8: 出参指针 ｜ param_7:
- 【待填】各参数取值域与组合矩阵（从 dc:7726 起签名注释回填）
## 2. 读写的状态字段
- 【待填】DAT_/g_ 全局清单（建议用 audit_globals_in_function 类工具批量导出后人工归类）
## 3. 分支表（已证锚点 → 重构拆段草案）
| 段 | rb 锚点 | 触发条件 | 副作用 | 证据 |
|---|---|---|---|---|
| EXEC 重入 | rb:564 `"PECMD EXEC "` | 行前缀匹配 | 递归进脚本层 | s11_exec_launch_audit #1 |
| 优先级类 | rb:1268–1281 REALTIME/ABOVENORMAL/NORMAL/BELOWNORMAL | 参数字面量 | SetPriorityClass | 直移 |
| 桌面选择 | rb:2134 WinSta0\WinLogon ／ rb:2307 Default | 开关参数 | 服务桌面 | 直移 |
| Run 注册键 | rb:2335 ...\CurrentVersion\Run | boot 安装路径 | 注册表写 | 直移 |
| 进程创建 | rb:2611 cmd.exe ／ SeDebugPrivilege ×4 (2757/3121/3660/3671) | EXEC/SHEL 族 | CreateProcess/令牌 | s11_exec_launch_audit 十四变量对照 rb:2598–3617 |
| winpe 引导链 | rb:3265 winpeshl / 3271 wpeinit / 3282 SOFTWARE\PELOGON Shell.x / 3314 waitpeinit.pecmd | boot 期 | 启动 shell | 直移 |
| SystemStartOptions | rb:4080–4088 | 启动读 | 环境变量 PECMDHB=/WIMHB= | 直移 |
| 【待填】install/service/removeservice/-shel 开关段 | （dc 内联十六进制字符比较） | boot argv | 服务安装/移除 | s12 matrix §1.2 |
## 4. 副作用清单：【待填】（进程/注册表/文件/服务四类分列）
## 5. 返回契约：【待填】（uint64_t 各错误码语义 ↔ dc 对照）
## 6. 验收挂钩：EXEC/WRITE Layer-B 语料 005–007/044/045 + 新增 golden（现全红，先转绿再动刀）
```

### 4.2 PECMD_ProcessScriptBlock（PSB）@FUN_14004c0bc

```markdown
# 行为文档：PECMD_ProcessScriptBlock（PSB）
## 0. 身份
- 原址 @0x14004c0bc ｜ dc:43816–45163 ｜ 移植体 restored_bodies.c:5821–7184（≈1357 行）
- 角色：单行脚本处理（^转义/%变量再展开 + 关键字分派）。注意：行迭代主循环不在本函数，
  在 DispatchExpressionBlock(rb:7780–8002 ← FUN_1400b1724 dc:110300–110506)——s11_psb_loop_audit §1.1 勘误在案
## 1. 输入契约
- param_1 script ctx ｜ param_2 行游标 ｜ param_3 ｜ param_4 出参 local_res10 ｜ param_5
- ctx 依赖：ctx+0x48 XOR 键表（InitObfuscatedKeywords 初始化）、ctx+0x88 终止符键
## 2. 状态字段
- local_158：长度打包+大小写折叠后的 FourCC 比较值（rb:6157–6218 打包段）
- local_res18[0]=10：%变量展开次数上限（超限报错返回，rb:5988–6073）
- 【待填】其余 ~30 个局部槽位语义
## 3. 分支表（s11_psb_loop_audit §2 逐段对照结论直接继承）
| 段 | dc | rb | 内容 | 机械校验 |
|---|---|---|---|---|
| 入口/SkipLeadingControls/@前缀 | 43896–43916 | 5896–5923 | 空行防线(NUL 直接返回) | 一致 |
| `-` 前缀/变量展开/TEAM 日志位 | 43923–43975 | 5930–5982 | | 一致 |
| 内层 while(true) ^转义/%成对检测/重组 | 43981–44066 | 5988–6073 | 上限 10 次 | 一致 |
| `(`→`{` 归一、配对剥离 | 44067–44088 | 6074–6095 | | 一致 |
| #/空 → 清理返回；{/[ → XorEncode+DEB(|1) | 44092–44101 | 6099–6108 | | 一致 |
| FIND 直通否则展开进 local_160 | 44103–44111 | 6110–6118 | | 一致 |
| 引号/| 尾裁剪 → SkipIdentifier×2 → FourCC 打包 | 44112–44211 | 6119–6218 | 大小写折叠 | 一致 |
| SED/DIR/SET(0x303)、THREAD 特判 | 44212–44244 | 6219–6257 | 三字母+六字母 | 一致 |
| 通用回退 LAB_14004df3c（ECD 调用#1） | 44246–44312 | 6258–6325 | ⚠ D-05 缺陷甲遗址 | 结构一致 |
| EXEC(ECD 调用#2)/LOAD/READ/FIND… | 44313–44660 | 6327–6659 | | 一致 |
| 其余关键字 switch 尾 + TEAM/FOR/IFEX | 44661–45137 | 6660–7149 | 118 case 序列零差异 | 一致 |
| &&ERROR 等收尾 | 45138–45159 | 7150–7171 | wsprintfW B5 约定 | 一致 |
## 4. 副作用：【待填】（经 handler 间接；本函数自身仅局部缓冲）
## 5. 返回契约：【待填】
## 6. 拆臂指引（S14-b）：按 §2.2 生成的 117 枚举逐臂提独立函数 pecmd_verb_envi() 等；
    先拆 EXEC/LOAD/READ/FIND 高频臂；118-case 序列基线可用脚本回归（关键字 case 序列 LCS 对齐法）
## 7. 验收挂钩：全部 43 案均穿过本函数入口段；ENVI/CALC/LSTR/REGI/TEAM 臂已有绿案
```

### 4.3 PECMD_RunCommand @FUN_140031454

```markdown
# 行为文档：PECMD_RunCommand
## 0. 身份
- 原址 @0x140031454 ｜ dc:29635–30332（主展开段 LAB_140031887）｜ 移植体 src/lang/core_scriptrun.c:261–731（471 行）
- 角色：命令行脚本调度主流程；cmdline 就地解析（截断）
- 体量注记：471 行 < 800 行阈值，因属启动必经热路径仍列入 S14 试点首位（保守修正，见 §6）
## 1. 输入契约
- script: 脚本容器 ctx ｜ cmdline: WCHAR*（会被就地截断——调用方契约必须在文档中显式登记）
## 2. 状态字段（局部解析产物，rb/core_scriptrun.c:266–273）
- pp 游标 ｜ flags/flags2 ｜ m_flag/mem_flag ｜ sysinit_name/b_sysinit ｜ outbuf ｜ qkmode ｜ DVar13
## 3. 分支表
| 段 | 锚点 | 内容 | 待填 |
|---|---|---|---|
| 段1 前缀指令解析 | :280 SrParsePrefix（8 出参一次打包） | M/MEM/SYSINIT/QK 等前缀 | 各 flag 组合语义矩阵 |
| 段2 主展开路径 | :283 起（LAB_140031887, dc:29635–30332） | token 游标走查/tokenEnd/lVar32…约 14 个局部槽 | 展开/引号/管道规则 |
| 段3 done 清理 | :240–254 | FreeStrBuf(-8 头回拨契约！T4 缺陷丙 v3 归正注记 :241–247) | 双 buf(convText 中途 goto done) 回收路径 |
## 4. 副作用：【待填】（预计：脚本装载/环境设置/子例程注册）
## 5. 返回契约：【待填】int64_t
## 6. 验收挂钩：TEMP PROBE ×4 已在（enter/before-SrParsePrefix/…）；16 绿案启动链均经此函数
```

---

## 5. 回归风险评估与分批建议（任务 5）

### 5.1 保护网现状（T4 门禁基线）

- **verdict 实测（results/win_real，43 案）：16 PASS / 27 FAIL**。PASS：001/003 envi、015/034 ifex、018 find_file、022/023 sub、024 team、026/028/032/033 calc、029/040 lstr、030/042 regi。FAIL 27 案集中于 WRITE/READ/FILE 族与若干边界（002/004 envi、005–012 write/read/file、013/014 ifex_file、016/017/035/036 find、021 sub_call、025 team_with_if、027 div0、031 set、037/038 sub_nested/shadow、039 team_chain、041 mdir、043 envi_space、044 write_blank、045 roundtrip）。
- divergences 登记至 D-11（docs/divergences.md）；已知结构性风险组：R1 写回断链（SetVariable no-op 桩 87 调用面）、D-05 ECD 第 4 参高位垃圾（已修但属敏感区）。
- **探针网（TEMP PROBE）**：仅 16 函数带桩，且集中在启动链（RunStartupScript×12、MainW×3、RunCommand×4、RunScriptText×2、DispatchExpressionBlock×2…）。三大巨物中 ECD/PSB 各仅 1 处，冷区巨函数 0 处。
- 静态基线资产（可复用的"免费用例"）：s11_psb_loop_audit（PSB 全段逐行 + 118 case 序列零差异）、s11_exec_launch_audit（ECD EXEC 块 rb:2598–3617 十四变量对照）、s12_verb_coverage_matrix（117 动词×实现状态矩阵）。

### 5.2 巨函数 × 保护等级矩阵

| 巨函数 | 语料穿过? | 绿案证据 | 探针 | 静态基线 | 保护级 | 动刀裁定 |
|---|---|---|---|---|---|---|
| PSB | ✅ 全部 43 案 | 16 绿案全程经过 | 1 | ★★★ psb_loop_audit 全段 | **高** | 可拆（第 3 批） |
| ECD | ✅ Layer-B 路径案 | EXEC 族案当前全红 | 1 | ★★ exec_launch_audit(EXEC 块) | **中** | 先补绿再拆（第 4 批） |
| RunCommand | ✅ 启动必经 | 16 绿案经过 | 4 | ☆ 无专项 | 中 | **试点首选（第 2 批）** |
| RegiEditRegistry (REGI) | ✅ | 030/042 绿 | 0 | ☆ | 中 | 第 3 批随 PSB 后 |
| ExpressionArithmetic (CALC) | ✅ | 026/028/032/033 绿 | 0 | ☆ | 中 | 仅限 P4 保留策略，勿结构化 |
| EnviMemReadWrite / ReadTextLine / WriteFileEncoded | 在测但红 | 0（008–009/005–007 红） | 0 | ☆ | **低（红灯区）** | 冻结，T4 转绿前禁碰 |
| DriverInstall (DEVI) / PartShowHideDrive (SHOW) / RamdMount 族 / RegexReplace / Code / Disp | ❌ 零语料 | — | 0 | ☆ | **无保护** | 冷冻 + 补 golden 后再评估 |
| LoadObjectIcon / HelpDlgProc / FUN_1400d9818 / FUN_1400f6db0 / FUN_140100ac4（UI/INDIR 族） | ❌（harness 无 GUI 用例） | — | 0 | 部分 INDIR 注记 | 无（但冷区） | L1 冻结，标注"未重构区" |
| EvalLoopCondition (FOR/LOOP) | ❌（LOOP 案在 attic，方言未考证） | — | 0 | ☆ | 无 | 冷冻 |

### 5.3 「先建保护后动刀」分批建议

| 批次 | 内容 | 进入条件 | 行为中性证明 |
|---|---|---|---|
| **B0 保护网先行**（非 S14 本体，前置） | ① T4 27 红分诊收敛（R1 SetVariable 归一等既定批次）；② 把 16 处 TEMP PROBE 固化为可开关探针宏并补齐三大巨物段界探针；③ 冷区动词 golden 原版侧录制先行（与 msvc 侧解耦，ROADMAP §S12 时序修订 v2 已许可） | 即可启动 | 不改行为，只加观测 |
| **B1 = S14-a FourCC 可读化** | 按 §2.2 方案生成 pecmd_fourcc.h 并全树宏替换（PSB 129 值→117 枚举+4 别名）；顺带双重强转噪音清理 | B0 ② 完成 | 双绿门 + 43 案 verdict 逐案比对不变（纯宏等价） |
| **B2 = RunCommand 结构化试点** | 471 行最小巨物；P1/P3 goto 消除 + 段函数化（parse_prefix/main_expand/done_cleanup）；顺带产出第一份完整行为文档（§4.3 模板） | B1 合入且全绿 | 16 绿案回归 + memfail.log 探针序列不变 |
| **B3 = PSB 拆臂（S14-b 主战场之一）** | 按 verb 分臂：先 EXEC/LOAD/READ/FIND 高频臂，再 TEAM/FOR/IFEX 尾臂；118-case 序列 LCS 校验脚本随批运行；REGI(b7c) 臂同法跟进 | B2 交付 + EXEC 族语料转绿 | psb_loop_audit 全段对照复跑 + 全量 verdict |
| **B4 = ECD 拆段** | 先拆有审计子基线的 EXEC 启动块（rb:2598–3617），再 install/service/boot 开关段；D-05 敏感点(第 4 参打包)设只读断言注释禁改 | B3 合入 + boot/install 段探针与 golden 就绪 | exec_launch_audit 复跑 + 全量 verdict |
| **B5 = 冷区处置** | DriverInstall/SHOW/RAMD/UI 族：默认 L1 冻结 + 文件头"未重构区"注记（ROADMAP S14 优先级条款允许）；仅在语料扩容覆盖后再评估 | 长期 | — |
| **goto 专项（穿插 B2–B4）** | 仅 §3.3 P1/P2/P3 三模式；P4(ExpressionArithmetic/DispCommand 状态机)与 P5(ECD/PSB 跨段) 明确**保留**并在行为文档登记 | 随宿主函数所在批次 | 每消除一处附前后控制流等价说明 |

**排序理由**：FourCC 批（B1）是全树唯一"零控制流变更"的可机械验证批次，最先建立 S14 的流程信心；RunCommand（B2）体量小、探针最密、绿案穿透，是最低成本的拆分演练场；PSB（B3）拥有最强静态基线；ECD（B4）最大且含红灯 EXEC 族语料，故殿后。

---

## 6. SKIP 与保守注记登记

| # | 事项 | 处理 |
|---|---|---|
| 1 | token 数无真实 tokenizer | 以 chars/4 估算；">2000 token"阈值据此口径得 112 函数，**排序未采用该口径**（行数+Score 为主），后续若接入真 tokenizer 可复算 |
| 2 | RunCommand 不满足 >800 行（实测 471 行） | 保守解释：仍列入三大巨物清单（任务点名 + 启动热路径），但在 §1.1/§4.3 显式标注阈值不符，避免报告失真 |
| 3 | PSB/ECD 行号边界 ±10 行漂移 | 扫描器把前置注释归属差异所致；一律以"审计锚点 + 实测区间"双写呈现 |
| 4 | 3 字母 SED/DIR/SET 与 6 字母 THREAD 不在 8-hex census 口径 | 沿用 s12 matrix 数字（117 动词宇宙），生成方案内置 4 项手工别名 |
| 5 | 129 个 PSB 比较形态未逐一人工核验 ==/< | 抽样实锤 15 处（含 9 对 ±1）；全量机械化分类交由 gen_fourcc.py 断言完成（§2.2 断言①） |
| 6 | EJEC(0x43454A45) 为 `==` 直比但需确认在 117 宇宙内 | 登记：gen_fourcc.py 首跑时对账，若矩阵缺登则补录 s12 matrix |
| 7 | MBRS/UPLS/FBBF/wwww 非动词常量 | 已定性排除出枚举；FBBF 双站点（时间文本 + 分区扇区）建议后续在 triage_map 加一条"共享魔数"注记 |
| 8 | 未重开 decompiled.c 逐一核对 dc 行号 | dc 锚点引自 s11/s12 审计与 provenance.map（1594 行溯源表），本预研不重复取证 |
| 9 | verdict 统计为读取现存 results 快照 | 未运行任何 EXE；T4 实时状态以后续正式跑批为准 |
| 10 | 函数边界扫描器对 K&R/怪异宏定义理论漏检 | 本库为 Ghidra 直移风格（ANSI 定义），抽查 2100 样本未见漏检形态；风险接受 |

---

## 附录 A：全树可打印 8-hex 常量清单（133 值）

图例：`V`=PSB 真动词（== 直比，入枚举）｜`B`=switch 边界伪影（< 比较，不入枚举）｜`X`=非动词条目（数据签名/乘数）｜`?`=待 gen_fourcc.py 机械分类。括号内为 hits/宿主函数数。

**X 非动词（4）**：FBBF(3/2, 数据魔数)、MBRS(2/1, 引导扇区签名)、UPLS(1/1, 分区表标签)、wwww(1/1, 时间换算乘数假阳性)。另注：MSTR(2 hits/1 函数)=`<` 边界(rb:6820)+`==` 直比(rb:6860) 双站点，**属真动词**（s12 matrix ⛔ 整体桩项），其边界形态计入 B 类
**B 已证边界伪影（11）**：JTEM、EOWN、MOOP、JNIT、MIST、TIZE、TERV、OUMK、TLID（9 对 ±1）＋ SEGI、DOME、MPOS、THRE（抽样 `<`/守卫，余者同法机械判定）
**V 动词（== 直比，抽样确认）**：TEAM、ENVI、EXEC、CALC、ITEM、CHEK、DATE、FLNK、TABL、MEMO、HOTK、PUTF、NOTE、FDIR、EXIT、LOCK、CODE、HIDE、FORX、WALL、LOOP、FDRV、SERV、RECY、UPNP、EDIT、TIPS、TIME、FBWF、WAIT、IFEX、TEMP、RUNS、DOWN、DFMT、DISK、NAME、DISP、LOGO、CMPS、ENVI、PBAR、LOAD、FONT、PCIP、FNAM、SPIN、DEVI、THRD、MAIN、LPOS、TERV?(待机械判定)、GETF、LIST、SHUT、LSTR、INIT、SBAR、EJEC、DALL、CALL、LINK、LABE、IPAD、TREE、SWIN、BASE、FEXT、HASH、IMAG、PINT、RSTR、COME、REGI、STRL、FILE、WRIT、RAND、IIDE、GROU、HOME、NUMK、HELP、SLID、MESS、SIZE、SEND、RPOS、RAMD、SHOW、MOUN、FORM、USER、PAGE、NTPC、SITE、PART、KILL、BROW、READ、MDIR、SHEL、PATH、SOCK、TABS、LOGS、RADI、TEXT、SCRN、DTIM、HKEY、ADSL、MENU、HIVE、SUBJ、FIND、SSTR …（全集以 gen_fourcc.py 断言 len(V)==117 为准；本列为普查原始输出摘要，非权威枚举）

## 附录 B：本报告度量复现口径

```
# 函数边界：花括号深度扫描（注释/字符串消毒），96 TU → 2100 函数
#   产物（临时区，非仓库）：%TEMP%\s14_funcs.csv / s14_funcs_enriched.csv / s14_fourcc_census.csv
# FourCC：(?i)\b0x([0-9a-f]{8})\b 且小端 4 字节 ∈ [0x20,0x7E]
# goto：\bgoto\s+(LAB_[0-9A-Fa-f]+|[A-Za-z_]\w*) ；标签：^\s*(LAB_\w+|L_[0-9a-f]+|\w+)\s*:\s*$
# verdict：遍历 harness/results/win_real/*/verdict.json 取 verdict/status 字段
# Score = lines + 25×fourcc_distinct + 8×goto_total
```

（完）
