# S17 — WRITE/READ/FIND/FILE 族失败用例分诊 (18 案)

- 角色: 执行子代理 (单文件独占: 本报告)。约束遵守: 未改任何代码 / 未跑任何 EXE / 未构建 / 未提交 git。
- 日期: 会话时钟 2026-08。工作目录 `D:\repo\PECMD反编译`。
- 输入: `refactored\harness\results\win_real_orig\<case>\`(golden 真值)、`...\results\win_real\<case>\`(msvc 实测 + verdict.json)、`C:\pectest\<case>\{main,run_all}.pecmd`、`decompiled.c`(dc 引用)、`refactored\src\**` + `restored_bodies.c`(还原源码)、Ghidra 打开的 `PECMD.exe`(@base 0x140000000, 2726 函数)。

---

## 1. 执行摘要

| 项 | 结论 |
|---|---|
| 聚类数 | **2 个主簇**: A=退出码传播缺失(17 案), B=崩溃 AV(1 案); A 内按触发命令族分 5 子簇 A1–A5 |
| 判定 | **18 案无一存在 vars/done/stdout 内容差异**; 17 案唯一差异 = 进程退出码(golden `exit=2` vs msvc `exit=0`), 1 案(041)= msvc 侧 0xC0000005 崩溃且无产物 |
| 根因定位(m Proven) | 进程退出码链路已静态打通: `main→mainW→FUN_14005b7dc` 读 `*(*(DAT_14013d180))`, 而 `DAT_14013d180 ≡ g_Script+0x50`(初始化指向退出码缓存 `0x14013caf0`)。全 `.text` 字节级扫描证明**直接引用仅 3 处**(初始化清零/LEA/读取); 原版中唯一的非零写入路径是分发器 `FUN_14004c0bc` 的 **EXIT 动词分支(dc:44918–45030)** 经 `*(script+0x50)` 解引用写缓存 |
| ms v c 侧缺陷 | 还原源码中 `g_exitCode/g_exitCodeCache`(core_globals.c:68-69,273)**从未被任何路径写非零值** → 还原版恒退 0(除崩溃/超时)。这与 18 案实测完全吻合 |
| 工作假设(未闭合) | "原版文件族命令(WRITE/READ/FILE/IFEX 文件探测/顶层 FIND)执行后把 errorlevel=2 留在根脚本并最终传播进进程退出码" —— 与 18 案及全部对照案(§5.2)一致, 但写入 `0x14013caf0` 的具体指令未定位(见 §8 UNVERIFIABLE-1) |

---

## 2. 方法与口径

- 逐案比对 `win_real_orig`(=golden 来源) vs `win_real` 四产物 + verdict.json 的 `exit/vars/fs` 字段; 脚本读 `C:\pectest\<case>\main.pecmd`(run_all = main + 固定尾声, 见 `run_case.py::make_epilogue`)。
- 静态分析三层交叉: dc 反编译文本(`decompiled.c`, 2117/2726 函数有函数体)、Ghidra xref/disasm、以及**对磁盘 PE 的自绘 RIP-相对 disp32 / imm64 全量字节扫描**(覆盖反编译盲区)。
- 重要口径发现: 尾声 `vars.txt` 内容 = `CASE=<id>|<manifest.vars 变量名列表>` 的**字面模板回显**(run_case.py:94-103), 不携带 `R/S/V` 等变量的实际求值; `done.txt` 恒为 `OK`。⇒ 本批用例当前**唯一有区分度的观测维度就是退出码**(外加崩溃与否), 见 §7 语料观察。

---

## 3. 逐案对照 (任务项 1)

格式: 期望(golden/orig) vs 实测(msvc)。除注明外: stdout 双方均为空, vars.txt/done.txt 字节级一致(CRLF 归一后), fs 无 added/changed。

| 案例 | 脚本要点 (main.pecmd) | 期望 exit | 实测 exit | 差异要点 |
|---|---|---|---|---|
| 005_write_file | `WRITE %F%,line1` (out\demo.txt) | 2 | 0 | 仅 exit; vars=`CASE=…\|F\|C` 一致 |
| 006_write_crlf | `WRITE lineA` + `WRITE lineB` 同文件 | 2 | 0 | 仅 exit |
| 007_write_missing_dir | `WRITE C:\pectest\nopath\x.txt,data` | 2 | 0 | 仅 exit(目标目录不存在, 双方均未产出文件) |
| 008_read_file | `READ %F%,V`(存在的 demo.txt) | 2 | 0 | 仅 exit |
| 009_read_missing | `READ C:\pectest\missing_file.txt,V` | 2 | 0 | 仅 exit |
| 010_file_copy | `FILE %F%=>out\copy.txt` | 2 | 0 | 仅 exit |
| 011_file_del | `FILE %F%<>`(删 copy.txt) | 2 | 0 | 仅 exit |
| 012_file_missing_src | `FILE C:\pectest\nofile.txt=>out\x2.txt` | 2 | 0 | 仅 exit |
| 013_ifex_file | `IFEX <存在路径>, ENVI R=exists! ELSE …` | 2 | 0 | 仅 exit |
| 014_ifex_not | `IFEX <不存在路径>, … ELSE …` | 2 | 0 | 仅 exit |
| 016_find_str | `FIND $%S%=world, …! ELSE …`(命中) | 2 | 0 | 仅 exit |
| 017_find_not | `FIND $%S%=xyz, …! ELSE …`(不命中) | 2 | 0 | 仅 exit |
| 035_find_case | `FIND $%S%=world`(大小写探针) | 2 | 0 | 仅 exit |
| 036_find_missing_file | `FIND --s <不存在>, …! ELSE …` | 2 | 0 | 仅 exit |
| 041_mdir_file_combo | `WRITE data41` + `MDIR sub41` + `FILE =>sub41\dst41.txt` | 2 | **3221225477 (0xC0000005)** | msvc 崩溃: done.txt/vars.txt 均未产出(verdict fs.removed=[done.txt]); 历史 run_notes 显示该案 **6 次** msvc 运行全部产物缺失 |
| 043_envi_space_value | `FIND $%S%=world again, …! ELSE …` | 2 | 0 | 仅 exit |
| 044_write_blank_line | `WRITE first` + `WRITE ,`(空行) + `WRITE last` | 2 | 0 | 仅 exit |
| 045_read_write_roundtrip | `WRITE lineA/B` + `READ %F%,V` | 2 | 0 | 仅 exit |

旁证(历史 flakiness): 上述 17 案的 `win_real\<case>\run_notes.txt` 均留有多轮 `[msvc] 产物缺失 vars.txt/done.txt` 记录(041 多达 6 组) —— 即早期构建在同族命令上曾于尾声前就崩溃/挂起; 当前构建已完成到尾声, 残余差距收窄为"退出码"(041 除外)。此记录与 S11–S13 修复批次的时间线相符。

---

## 4. 退出码机制 — 已证事实链 (本报告核心增量)

1. **进程退出码来源**: CRT 入口 `entry@0x1401081ec → __tmainCRTStartup → main@0x140017034`(字节核实 `mov rdx,rcx; xor ecx,ecx; jmp mainB`) `→ mainB@0x140016ad0 → mainW@0x140009ccc`; mainW 收尾(LAB_14000a100, 字节核实 `CALL 0x14005b7dc`)返回 `FUN_14005b7dc()` 作为 CRT 退出码。
2. **GetExitCodeGlobal 语义**: `0x14005b7dc: mov rax,[0x14013d180]; mov rax,[rax]; ret` —— 即 `*(*(0x14013d180))`。
3. **指针槽身份**: 初始化 `PECMD_InitProcessGlobals@0x1400051b4`(dc:2220-2221): `AND qword[0x14013caf0],0; lea rax,[0x14013caf0]; mov [0x14013d180],rax`。而 `0x14013caf0 − 0x14013d130(g_Script) = 0x50` ⇒ **`DAT_14013d180 就是根脚本的 script+0x50 出口槽**, 初值指向退出码缓存 `0x14013caf0`(= 还原码 `g_exitCodeCache`, core_globals.c:273)。
4. **全量字节扫描(权威, 覆盖 609 个未反编译函数)**: 对整个 `.text`(raw 0x400–0x119E00)扫描所有 RIP 相对 disp32 与 imm64: 指向 `0x14013caf0` 的引用 **仅 1 处**(init LEA), 指向 `0x14013d180` 的引用 **仅 2 处**(init 写 + 0x14005b7dc 读)。⇒ 不存在任何指令把这些地址作为立即数或其它编码访问。
5. **唯一的非零写入路径**: 分发器 `FUN_14004c0bc`(PSB) 的 **EXIT 动词分支**(tag `0x54495845`, dc:44918-45030):
   - `EXIT =<value>`: `**(script+0x50) = value`(dc:44937) 且 `script+0xd0 = value`(errorlevel, dc:44938)、置退出标志 `script+0xca = 1`;
   - `EXIT [,mode]`: `local_118[0] ← *(int)(script+0xd0)`(dc:44948) 后同样 `**(script+0x50) = …`(dc:45027)。
   - 子脚本的 +0x50 继承父值(`FUN_1400186bc` dc:13328 附近: 新建时拷贝 `parent+0x50`); SUB 调用路径改为私有槽(`FUN_1400186ac@0x1400186ac: child+0x50=&child+0x58`, 由 `FUN_140080cfc` dc:82273 调用)。
6. **errorlevel 的另一条回流(未接通到进程码)**: LOAD 运行器 `FUN_1400b638c`(RunScriptText) 在脚本体执行完返回 `*(int)(script+0xd0)`(dc:113379→113458); 该值经 fiber 存入 `DAT_14013cae0`(`FUN_1400050c8`)后被 mainW/MainEntryPoint **忽略**(dc:5551 / rb core_b1_remaining.c:4388)。嵌套执行器 `FUN_1400a53e4` 有 child→parent 的 errorlevel 回传(dc:103263-103269, 条件 `child+0xca != 0`)。
7. **还原侧现状**: `core_main.c:190 ret = PECMD_GetExitCodeGlobal();` 与原版结构一致, 但 `g_exitCode/g_exitCodeCache` 在全部还原源码中只有清零(core_b1_remaining.c:1618), **没有任何写入者** ⇒ 还原版对正常结束恒退 0。18 案中 17 案 `exit=0` 与此完全自洽。

> 推论: golden 里的 `2` 必然经 `*(g_Script.slot50)` 这条唯一通道进入缓存; 由于 18 案均无显式 `EXIT` 动词, 必定存在**文件族命令执行路径上的隐式等价写入**(寄存器中转, 静态不可直寻)。候选落点与排查动作见 §5/§6、缺口登记 §8。

---

## 5. 失败模式聚类 (任务项 2)

### 5.1 主簇 A — 退出码传播缺失 (17 案; msvc 侧单一缺陷, 原版侧触发族分 5 子簇)

**统一症状**: vars/done/stdout 全同, `exit exp=2 got=0`。
**msvc 侧候选根因**: §4-7 所述全局缺口 —— 还原代码没有实现"命令 errorlevel → `*(g_pExitCode)`"的传播; 修复必须先回答 §8 UNVERIFIABLE-1(原版谁写 2), 否则只能猜语义。

| 子簇 | 案例 | 触发族 | 原版候选函数 (地址权威; 括号内为 provenance 自动命名, 注意误名) | 还原现状 |
|---|---|---|---|---|
| A1 WRITE | 005, 006, 007, 044, 045 | `WRITE` | `FUN_14009f070` PECMD_WriteFileEncoded @0x14009f070 | rb core_b3r_h3.c:1821 (已还原) |
| A2 READ | 008, 009, 045 | `READ` | `FUN_14009da54` PECMD_ReadTextLine @0x14009da54 | rb core_b3r_h3.c:917 (已还原) |
| A3 FILE | 010, 011, 012 | `FILE =>/<>` | `FUN_14003c06c` @0x14003c06c (IndataCopyDirs) | rb core_b2f.c:6552 (已还原) |
| A4 IFEX 文件探测 | 013, 014 | `IFEX <path>,…! ELSE…` | 链 `FUN_140035b08`(IfexCmdHandler, thunk) → **`FUN_140034788`(共享主体, 4312B)** | thunk 已还原(core_b2a.c:136); **主体在还原码里是 SKIP 桩**(core_b2f.c:3829, 被自动误名 "PECMD_BuildImDiskMenu (stub)") |
| A5 FIND(带 ELSE) | 016, 017, 035, 036, 043 | `FIND $…=…` / `FIND --s` | 链 `FUN_140035b24`(FindCmdHandler, thunk) → 同一 `FUN_140034788`; 子串搜索 `FUN_14001b4f8` PECMD_StrChrOffset | thunk 已还原(core_b2a.c:147); **同一主体桩**; StrChrOffset 已还原(kernel/core_string.c 域, rb 内有实现) |

判别性对照(同族不同型, 决定"2"的触发面):
- `015_ifex_expr`(`IFEX $%A%>3,…`) golden=0 → 表达式型 IFEX 不产生 2, 文件探测型产生;
- `018_find_file`(`FIND --s <存在>, …` **无 ELSE**) golden=0 → 该形态不产生;
- `039_team_chain`(TEAM 管道内的 FIND+ELSE) golden=0 → **TEAM 子执行上下文抑制**该效应(候选: TEAM 走 `FUN_14002fd88/FUN_140030144`/`FUN_1400a53e4` 子脚本路径, SUB 化的私有出口槽或上下文标志差异);
- `027_calc_div0` golden=**16** → 除0路径以另一常量走同类通道(佐证"命令完成态→退出码"通道存在, 且值可≠2)。

### 5.2 对照集 (本轮复核为 PASS, 用于圈定回归面)

001/003/004(ENVI)、015(IFEX 表达式)、018(FIND --s 无ELSE)、022/023/024(SUB/TEAM)、026/028/029/030/032/033/034/040/042(CALC/LSTR/REGI/SET/IFEX&&/REGI) 均 PASS(exit 双方一致)。⇒ 回归面精确限定在 **文件族命令副作用 + 退出码传播** 两点, ENVI/CALC/SUB/TEAM 主干未受影响。

### 5.3 主簇 B — 041_mdir_file_combo 崩溃 (1 案)

- 症状: msvc `exit=0xC0000005`, vars/done 双缺(崩溃发生在 main 三行之内: WRITE→MDIR→FILE)。
- 涉事处理函数: `WRITE`(A1) → `MDIR = FUN_140027644` PECMD_CreateDirectory(rb core_b2a.c:382) → `FILE = FUN_14003c06c`(rb core_b2f.c:6552)。
- 高度吻合已知缺陷形态 **D-05 缺陷甲**(divergences.md:88): PSB 回退路径 `restored_bodies.c:6297` 曾仅清 `local_150[0]`,`[1..3]` 栈垃圾被打包进 `PECMD_ExecCmdDispatch` 第 4 参 → 入口门误判 → `PECMD_SkipLeadingControlChars` 解引用野指针 AV。当前源码 rb:6300-6305 已带四槽清零补丁、rb:6326 带"缺陷丁"HeapFree 二级地址修正 —— 但**当前 results 仍是崩溃态**, 说明: (a) 补丁后未重跑该案, 或 (b) 崩溃点另在 MDIR/FILE/WRITE 具体处理体内(如 `FILE =>` 目标目录刚由 MDIR 创建的竞态/路径拼接越界)。倾向 (a)+(b) 并存待复测。

---

## 6. 每簇下一步排查动作 (任务项 3; 全部为建议, 本轮未执行)

**A 簇公共前置(最高优先)** — 定位"谁把 2 写进 `0x14013caf0`":
1. 动态(解除"不跑 EXE"限制后): x64dbg/WinDbg 附着原版 PECMD.EXE 跑 `LOAD C:\pectest\005_write_file\run_all.pecmd`, 下硬件写断点 `bpa 0x14013caf0 w 4`(或 Ghidra debugger_watch_memory write), 记录首次非零写入的 RIP → 反查所属函数; 同法跑 016(FIND)与 013(IFEX)各一次确认是否同一条指令。
2. 静态补充(无需运行): 对 `.text` 做**指令级模式扫描**——凡 `mov r64,[reg+0x50]`(48 8B 4? 50 / 4C 8B 4? 50 等)后短窗口内跟 `mov [r64],imm/reg` 的站点清单化, 再人工核对站点所在函数是否文件族调用链(重点: FUN_14004c0bc 各动词尾、FUN_1400a53e4、FUN_14004e488、三个文件族 handler 尾部)。Ghidra 可用 `search_instructions mnemonic=mov operand_pattern="[reg + 0x50]"` 类查询加速。
3. 盲区核查: 609 个未反编译函数清单(functions.csv vs decompiled.c 头计数 2117/2726), 优先反编译地址落在文件操作簇(0x14003xxxx / 0x14009xxxx / 0x1400axxxx)者。

**A4+A5 (IFEX/FIND, 4+5 案)**:
4. **去桩 `FUN_140034788`**(4312B, dc:31379 起): 它同时是 IFEX 与 FIND 的共享主体、也是 A4/A5 的行为总闸; 按 s13_r1_dualbody 方法做双体对译, 特别注意其返回值/对 `script+0xd0`、`+0xca` 的副作用。桩期间两族用例不可能语义通过。
5. 复核 `PECMD_StrChrOffset(FUN_14001b4f8)` 在 FIND `$…=` 参数展开中的调用位(rb 侧已有实现), 以及 `--s` 开关解析(dc:44409 `0x7c='|'` 分支附近)。

**A1/A2/A3 (WRITE/READ/FILE)**:
6. 逐一比对新还原体与 dc 尾部: `FUN_14009f070`(dc:99499+)、`FUN_14009da54`(dc:98642+)、`FUN_14003c06c`(dc:44465 分支)的**返回值语义**(是否存在常量 2 / errorlevel 写); 交叉检查 dc:103263-103269(child errorlevel 回传)与 dc:44937-45030(EXIT 通道)能否构成"命令完成→2"闭环。
7. 用 007/009/012(失败分支)与 005/008/010(成功分支)做双面对拍设计, 修复后应同时复现 golden 的"成功也=2"这一非直觉锚点。

**B 簇 (041)**:
8. 当前构建下重跑 041(允许运行后): 若仍 AV, WinDbg 附着取 dump, 看 `PECMD_SkipLeadingControlChars`/ECD 入口的 param_4 是否仍是野指针(缺陷甲复发位)或落在 MDIR/FILE 体内; 同时检查 `C:\pectest\memfail.log` TEMP PROBE 输出确定推进到的阶段(core_main/core_init 内埋了探针)。
9. 若 AV 消失则按 A 簇出口码路径继续(041 golden 也是 2)。

**语料/夹具改进(提请决策, 非本轮改动)**:
10. 让 `R/S/V` 值可观测: 尾声改用 `ENVI T_R=%R%` + echo `%T_R%` 通道(或 manifest 增加 value-echo 行), 否则 IFEX/FIND 族永远无法从产物判定真假分支, WideStrLen 等嫌疑也无从验证(见 §7)。
11. fs 产物(src41/sub41/w44/r45 等)不入 golden 的现状(s12_golden_expansion §4.4)维持, 但建议 verdict.json 增加 `fs_manifest` 可选维度以便将来启用。

---

## 7. 全局嫌疑函数验证状态 (任务书提示项)

| 嫌疑 | 本轮证据 | 状态 |
|---|---|---|
| `PECMD_WideStrLen` 长度少1(R14 已修, 待验效果) | 18 案 vars/done/stdout/fs 零内容差异; 但 vars 是模板回显(§2), **值维度不可观** | **无反证亦无正证** — 受夹具表达力所限不可判定; 待 §6-10 改造后用含中文/空格边界串的 READ/WRITE 回环案验证 |
| `FUN_14005C788` 前缀比较恢复纯语义 (dc:54955) | 该函数用于 EXEC/MAIN 前缀识别(mainW/dc:5512,5527)与 READ 动词复核(dc:44363); 18 案均正确走到尾声(EXIT/EXEC 链正常)且无 verb 误派迹象 | **暂无反证**; 其纯语义化未破坏本批用例 |
| D-05 缺陷甲(local_150 栈垃圾→ECD 野指针) | rb:6300-6305 补丁在源; 041 现结果仍崩 → 待复测定 (a)/(b) | **未关闭** |
| D-05 缺陷丁(HeapFree 二级地址) | rb:6326 已按 dc:44300 归正; 17 案干净退出佐证生效 | **初步有效** |

---

## 8. UNVERIFIABLE 登记与保守解释

| # | 事项 | 原因 | 解锁条件 |
|---|---|---|---|
| U-1 | 原版中将 `2` 写入 `0x14013caf0` 的具体指令/函数 | 静态手段已穷尽直接寻址(RIP-rel disp32 + imm64 全量字节扫描, 仅 3 处合法引用); 写入必经运行时寄存器中转([reg+0x50] 型), 反编译文本(覆盖率 2117/2726)与模式扫描未能唯一定位 | §6-1 动态断点 或 §6-2 指令级扫描+人工核 |
| U-2 | 18 案中 R/S/V 变量双方实际取值是否一致 | 尾声产物为变量名字面回显(run_case.py:82-111), 不携带值 | §6-10 夹具改造 |
| U-3 | 041 崩溃现行复现性与确切落点 | 任务禁止运行 EXE/构建; 现有 results 为旧构建产物, 源码补丁(甲/丁)晚于部分记录 | §6-8 复测 |

保守解释声明: 歧义处一律取"不改代码、不扩大断言"一侧; §4 所有结论均给出可复核的字节/行号证据, 工作假设(§1)明确标注为假设而非结论。

## 9. 附: 关键地址速查

| 名称 | 地址 | 备注 |
|---|---|---|
| main / mainB / mainW | 0x140017034 / 0x140016ad0 / 0x140009ccc | 入口链(字节核实) |
| PECMD_GetExitCodeGlobal | 0x14005b7dc | `mov rax,[0x14013d180]; mov rax,[rax]` |
| InitProcessGlobals(清缓存+设槽) | 0x1400051b4 (dc:2202) | `AND qword[0x14013caf0],0` @0x14000521a |
| g_Script / 出口槽 / 缓存 | DAT_14013d130 / 0x14013d180(=+0x50) / 0x14013caf0 | 还原名: g_Script / g_pExitCode / g_exitCodeCache |
| 分发器 PSB | FUN_14004c0bc (dc:43816) | EXIT 分支 dc:44918-45030; WRITE dc:44913; READ dc:44362; FIND dc:44403; FILE dc:44465; MDIR dc:44827 |
| LOAD 执行器 / 脚本运行器 | FUN_14004eb34 (dc:~21000 前) / FUN_1400b638c (dc:113098) | 后者返回 `*(script+0xd0)` |
| 解释循环 / 嵌套执行器 | FUN_1400b1724 (dc:110300) / FUN_1400a53e4 (dc:103346) | 循环忽略命令返回; 嵌套器有 errorlevel 回传 dc:103263-269 |
| IFEX/FIND thunk→共享主体 | 0x140035b08 / 0x140035b24 → FUN_140034788 (dc:31379) | 主体在还原码为桩(core_b2f.c:3829, 自动误名 BuildImDiskMenu) |
| WRITE / READ / FILE / MDIR | 0x14009f070 / 0x14009da54 / 0x14003c06c / 0x140027644 | rb: core_b3r_h3.c:1821 / :917 / core_b2f.c:6552 / core_b2a.c:382 |
