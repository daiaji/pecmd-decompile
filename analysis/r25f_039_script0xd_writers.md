# R25-f · 039_team_chain 根因线：script+0xd 位0 写入点静态取证报告

- 线别: R25-f 静态子代理 A（只读取证；未改 src / 未构建 / 未用 windbg/Ghidra MCP）
- 证据基线: dc = `reference/decompiled.c`（唯一语义真值，基址 0x140000000）；
  msvc 源 = 工作树（HEAD=1b6a17a + R25-b..e 未提交改动）；
  部署构建 = `C:\pectest\DEPLOYED_BUILD.txt` hash=1b6a17a md5=517ee608… time=2026-08-29 02:50:07，
  与 `build/msvc/symsnap.txt`（build=1b6a17a time=02:49:55）一致。
  git diff 1b6a17a→工作树仅触及 `restored_bodies.c(PECMD_AssignString)` /
  `core_exec5.c(FUN_1400637DC)` / `core_execline.c(var_b6ee spec)`，**均不涉及 +0xd 写者**，
  故 R25-e 的活体测量（+0xd=1）对当前源码结论仍然适用。
- 案件: 039 `TEAM CALC X=2*3|FIND $%X%=6, ENVI R=chain_ok! ELSE ENVI R=chain_bad`
  golden exit=0 / msvc exit=2。run_case 实际喂给 EXE 的命令行 =
  `pecmd_msvc.exe LOAD C:\pectest\039_team_chain\run_all.pecmd`（main + 4 行尾声）。

---

## §1 结论摘要（写入者排序列表 + 置信度）

**script 结构字节 +0xd 的"位0=1"写入者，dc 全量清点只有 5 处（§2）；msvc 侧一一对应同构（§3）。
在 039 的静态执行路径（启动→RunStartupScript→RunScriptText(call A)→LOAD 分发→srx_ExecuteScriptFile→
RunScriptText(call B)→ProcessScriptBlock→TEAM 行整行展开）上，逐点核对结论：
5 个写者无一满足触发条件 —— dc 侧全程 +0xd≡0（与 golden exit=0 自洽）。**

因此 msvc 的 0→1 写入者按可能性排序（均需活体一次定位收口，见 §5 配方）：

| # | 候选写入者 | msvc 证据位 | 触发前提 | 静态可排除度 | 置信度 |
|---|---|---|---|---|---|
| C1 | `PECMD_RunScriptText` flags&0x10 → `pExec+0xd \|= 1`（core_execmain.c:254），且 **pPersist!=NULL 且 persistMode!=1 时收尾不恢复（core_execmain.c:371-377）→ 位0 持久留存** | dc:113269/113383 | 存在一个 flags 含 0x10 且 pPersist 非空的调用点 | 039 主路径两个调用点（core_script2.c:636 flags=bit0\|bit1\|0x80、core_scriptrun.c:232 flags=0）均已静态排除；**但"某个 pPersist!=NULL 的调用点"未枚举穷尽**（mega 分发器 FUN_14000e26c 尚未逐行核对） | **中** |
| C2 | `PECMD_ParseEnvSwitches` FORCELOCAL=<digit> → `script+0xd = 数字&1`（core_b1_remaining.c:5212，mode='&' 时计数预置 3 ⇒ `2<count` 恒真），入口=行首 `^`/`-mode`/`&&&`（core_b2e.c:1388/1406）；若 msvc 某处插入了 `@ENVI^ ForceLocal=1` 行（PrependEnviHeader 族），展开该行即置位 | dc:7284 | 一行含 FORCELOCAL=digit 的特殊行经过 EvalSpecialToken | 039 脚本与 LOAD 命令行均无该 token；msvc 的 PrependEnviHeader 唯一宿主 PECMD_ExecuteScriptBlock(core_scriptdep.c:296) **零调用者（死代码）**，LOAD 走 srx_ExecuteScriptFile 不插行 —— 静态排除度高 | **中低** |
| C3 | `PECMD_LaunchAsyncScriptThread`（THREAD 族）克隆体 `=1`（core_b3r_i28c.c:1244，'&' 开关） | dc:110197/110199 | THREAD/& 命令 | 039 无 THREAD；唯一调用点 restored_bodies.c:6240 = PSB 0x606 分支 —— **已排除** | 低（仅列表完整性） |
| C4 | 继承链 ScriptCopy/FUN_1400186BC 把已置位的父对象拷进克隆（core_exec5.c:40） | dc:12776 | 父对象 +0xd 已是 1 | 是"传导者"而非"源"：若活体证实克隆体=1 而父=0，则本条排除 | 传导项 |
| C5 | 非映射写入（堆越界踩踏 clone2+0xd，E 簇 TEAM 堆损坏同族）或测量对象误认 | — | — | 静态无法证伪 | **兜底** |

**核心静态事实**：msvc 源码里对 script 字节 +0xd 的全部写点 = C1/C2/C3/C4 四族 + ScriptInit 归零体
（core_b1_remaining.c:9360-9383，先清 0 后继承父值），**与 dc 完全同构，没有第 5 个"多余写者"**。

> **R25-f 第二轮静态取证增补（同日，见文末「增补节」）**：RunScriptText 全部 7 个调用点两侧已枚举完毕
> （msvc 7 / dc 7，逐一 flags 定案，**mem 执行器 dc:33498≡msvc:6058 同构**）；新增一个 C1 的具体子嫌疑
> **C1-a = core_scriptrun.c:650 资源路径把随机种子低字节（含位4）注入 RunScriptText flags**（dc:30219 无此注入），
> 以及 `*map:` 桩、vars/done 产物跨后端污染两项伴生发现。C1 的"flags 0x10 来源未明"由此收窄为两个具体落点。
所以本报告的交付价值 = ①排除清单（活体 bp 面大幅收窄）②传导链定案 ③精确 watchpoint 配方；
"msvc 侧 0→1 的最终指令地址"按 V-Gate 纪律须由一次活体 ba w1 收口（§5），不在静态线上臆测定案。

---

## §2 dc 侧 script+0xd 写者全量清单（byte 语义，含误报甄别）

方法：`grep "+ 0xd)"` 全量 55 处 + `|=`/`&=`/`+=` 变体 + `param[0xd]=` 字索引复核；
按指针类型区分 **byte+0xd**（真字段）与 **qword 索引 [0xd]（=字节 0x68，argv 表指针槽）**。

### 2.1 真写者（byte +0xd，脚本结构 0xf0）

| dc 行 | 函数 | 写入 | 条件 | 值语义 |
|---|---|---|---|---|
| 7284 | FUN_14000d2a0 ParseEnvSwitches（7217，size 3609） | `*(byte*)(param_2+0xd) = *p & 1` | 行内 token `FORCELOCAL`+`=`+数字，且 `2 < '&'计数`（param_3=='&' 时计数**预置 3 ⇒ 恒真**） | '1'→1，'0'→0（ForceLocal 开关） |
| 113269 | FUN_1400b638c PECMD_RunScriptText（113098，size 2709） | `LVar15+0xd \|= 1` | `param_5(flags) & 0x10`（LOAD `*local` 开关族） | 置位 ForceLocal；入口已存 savedD（113253） |
| 113383 | 同上（收尾） | `LVar15+0xd = uVar3`（恢复） | `param_7(pPersist)==NULL \|\| uVar11==1`；uVar11 = (flags&0x200)?(pPersist==param_1?1:0x200):0 ⇒ **pPersist!=NULL 且非 self 时持久不恢复** | 还原/持久 |
| 110197/110199 | FUN_1400b0bf8 异步脚本线程启动（109812） | 新克隆 `+0xd = 父+0xd`；`uVar21!=0 → =1` | `'&'` 前缀开关（local_e8） | THREAD 族克隆体 ForceLocal 继承/置位 |
| 12776 | FUN_140017cdc PECMD_ScriptCopy（12764） | `param_1+0xd = param_2+0xd` | 无条件（结构复制） | 传导（非源） |
| 18034/18047 | FUN_14001e4dc 脚本结构初始化（18009） | `+0xd = 0`；父非空→`= 父+0xd` | 无条件清零 + 条件继承 | 归零/继承 |

### 2.2 甄别为"非脚本 +0xd"的命中（防后继线误踩）

- dc:49233/49278（FUN_140053dc8/FUN_140053e7c）`*(undefined4*)(param_1+0xd)=0`：param_1 是
  undefined8*，实为**字节 0x68** 的 4 字节清零（对话框项对象，vtable=PTR_FUN_1401234f0）。
- dc:76236（FUN_1400780fc）：GUID 解析器，param_2 是 16 字节 GUID 出参（0xd 字节 = GUID 第 5 段）。
- dc:109713/110217（FUN_1400b0820/FUN_1400b0bf8）：线程参数块 `lpParameter[0xd]`（qword 槽）。
- dc:126304（FUN_1400cb820）：**读** param_1+0xd 写入局部 local_res20，非结构写。
- 其余 ~40 处均为读侧（`&0xf`/`&0x11`/`==0`/`!=0`），代表读点：
  **68793**（FUN_14006dd74 取值器）、**77738**（FUN_14007a224 完整展开器）、**78290**
  （FUN_14007af60 PECMD_ExpandEnvVars，即 R25-e 的 var_b38c=xflag 读点）、129577（&0x11）。

### 2.3 dc 侧 +0xd 全程为 0 的路径证明（039 形态）

1. 全局脚本 DAT_14013d130：BSS 零初始化；启动 FUN_140027690（23734）仅以
   `FUN_1400186bc(&DAT_14013d130,0)`（23752）初始化，该函数**不写 byte 0xd**（13338 区：
   `param_1[0xd]` 是 qword@0x68；byte 侧只清 0xe/0xf/0x12）。
2. 启动顶层 RunScriptText（FUN_14004eb34 尾 45821）flags =
   `bVar31&1 | uVar25 | 0x80`，uVar25 低半 = `(旧|uVar8)&2` ⇒ **位4(0x10)=0** ⇒ 113269 不触发。
3. LOAD 行分发（FUN_140031454，29481）的 flags=`uVar31|local_294`：位4 仅由
   `*local`/`-local` 开关置位（29759/29764）。039 的 LOAD 行 = `LOAD <绝对路径>`，无开关 ⇒ 0。
4. 装载器 FUN_140031068（29349）→ `FUN_1400b638c(param_1,…, uVar7&2|uVar14<<0x10|param_4&0xfffffff7,…)`
   —— 高半 seed<<16 不进位4，低半 bit1；**位4 仍 0**。pPersist=plVar12=0（无 0x200）⇒ 走新克隆。
5. 文件正文（TEAM 行）展开时脚本 = 该克隆，+0xd = 克隆自 param_1 链的 0 ⇒ **var_b38c=0 →
   b961（LAB_14007b961, 78559-78574）`local_230!=0` 不成立 → local_268=1 → b55d → bab5
   原样复制 %X%** —— 与 golden exit=0 一致。∎

---

## §3 msvc 对照表（同构性核对）

| dc 写者 | msvc 位置 | 代码 | 同构判定 |
|---|---|---|---|
| 7284 ParseEnvSwitches | `src/misc/core_b1_remaining.c:5212`（函数 5158） | `if (2 < v10) *(uint8_t*)((char*)param_2+0xd) = *p & 1;`，mode='&' 时 `v7=3` 预置（5186） | **同构**（含计数预置怪癖）；EnviMode 命中分支对称写 +0xda（:5527 处 `param_2+0xda = *p & 1`） |
| 113269 RunScriptText 置位 | `src/lang/core_execmain.c:254` | `if ((flags & 0x10) != 0) *(uint8_t*)((char*)pExec+0xd) \|= 1;` | **同构** |
| 113383 RunScriptText 恢复 | `src/lang/core_execmain.c:371-377` | `if (pPersist == NULL \|\| persistMode == 1) { … = savedD; }`，persistMode 定义与 dc uVar11 一致（:118-122） | **同构**（含"pPersist 非空非 self ⇒ 不恢复"语义） |
| 110197/199 异步线程 | `src/commands/core_b3r_i28c.c:1241-1244`（PECMD_LaunchAsyncScriptThread） | 先拷父值，`uVar21(local_e8,'&')!=0 → =1` | **同构** |
| 12776 ScriptCopy | `src/runtime/core_exec5.c:40`（FUN_140017CDC） | `d[0xd] = s[0xd];` | **同构** |
| 18034/18047 初始化体 | `src/misc/core_b1_remaining.c:9360-9383` | qword[0xd]=0（0x68）、byte 0xd 清零、父非空继承 | **同构** |
| 读侧 b961 闸 | `src/lang/core_execline.c:656`(xflag 读)/`:829`(local_230)/`:1059-1060`(`if (local_230 != 0) continue;`) | 与 dc:78290/78348/78561 对齐 | **同构**（另 FUN_14007A224 读点 :138） |
| 误报族 | `core_b3k.c:62`/`core_b7c.c:3886`/`restored_bodies.c:9807` 等 | 均 qword 索引或他对象 | 同为误报，无新增写者 |

**msvc 全源 byte+0xd 写点完备清单**（双模式 grep，含 `/*` 行不过滤复核）：
core_execmain.c:254/375、core_b1_remaining.c:5212/9370/9383、core_b3r_i28c.c:1241/1244、
core_exec5.c:40 —— **没有超出 dc 写者集的第 5 族写点**。

### 3.1 039 主路径逐点排除（msvc）

| 路径点 | 文件:行 | flags/对象推演 | 位4/位0 |
|---|---|---|---|
| 启动顶层 RunScriptText | core_script2.c:636-637 | flags=`(bVar31&1)\|seedFlags\|0x80`，seedFlags=`(uVar29<<0x10)\|((uVar25run\|(uVar8&0xffff))&2)` ⇒ 位4=0；pPersist=NULL→clone1 | 不触发 |
| LOAD 裸路径分发 | core_scriptrun.c:607 | flags\|flags2=0（SrParsePrefix 对 `LOAD…` 首字符 'L'≠'*'/'-' 直接 break，core_srparse.c:171） | 0 |
| srx 装载 → RunScriptText | core_scriptrun.c:232 | `flags & 0xfffffff7` = 0；pPersist=NULL→clone2（core_execmain.c:212-229 calloc(0xf0)+ScriptCopy+FUN_1400186BC） | 不触发 |
| clone2 构造到展开之间 | core_execmain.c:230-330 | 写点仅 +0x10/+0x48/+0x78/+0x80/+0x8c/+0x24/+0xe/+0xd8/+0xda 与 flags&0x10/0x20 门控项；ResetScriptChain/73CCC/SetCurFileVariables/&__MAIN__ 均无 +0xd 写（dc 侧同步核对亦无） | 无写入 |
| TEAM 行整行展开 | restored_bodies.c:6099-6103 → FUN_14007A224/PECMD_ExpandEnvVars | 读 clone2+0xd | **R25-e 实测=1（矛盾点）** |

---

## §4 四项特别核查记录

### (a) PECMD_PrependEnviHeader / ForceLocal 位与 +0xd 的关系
- dc: FUN_140024f20（22214，size 412）：`flags&0x10` → 插入行 `@ENVI^ ForceLocal=1\n`（dc:22240），
  `flags&0x20` → `@ENVI^ EnviMode=1\n`（dc:22245）。调用点 = LOAD 资源脚本分支
  （dc:30199/30265/30322，flags=local_294=flags2，**位4 仅由 `*local` 开关置入**，29759/29764）。
- 该插入行随后经 `^` 前缀 → FUN_14002d33c（27011）→ ParseEnvSwitches(·,'&') → +0xd=1 ——
  这是**原版"局部环境脚本"的合法置位通道**。
- msvc: `src/runtime/core_scriptdep.c:127`（PECMD_PrependEnviHeader）同构复刻；唯一宿主
  `PECMD_ExecuteScriptBlock`（core_scriptdep.c:296，对应 dc FUN_140031068）**在全工程零调用**
  （grep 证实）；039 的 LOAD 裸路径走 `srx_ExecuteScriptFile`（core_scriptrun.c:109，S10 替代体），
  其内部**不调用** PrependEnviHeader。⇒ 该通道在 039 msvc 路径静态死亡。TODO(verify)：
  活体确认死代码判断（map 中 PECMD_ExecuteScriptBlock 是否被链接进某个未枚举调用点）。

### (b) ParseEnvSwitches 的调用者与写位
- dc 调用者唯二：FUN_14002d33c:27052（mode='&'，入口 `^`/`-mode`）与 :27068（mode=0，入口 `&&&`）；
  FUN_14002d33c 的上游 = PSB 的 SET 命令分支（dc:44227，`SET -mode …`）及 `?`/`@`/`*` 特殊行。
- msvc 调用者唯二：`core_b2e.c:1388`（mode=0x26）/`:1406`（mode=0），同构。
- 写位：FORCELOCAL=digit → **+0xd**；EnviMode=digit → **+0xda**（core_b1_remaining.c:5527 区）；
  EXPORTLOCAL/__arg=/logs_*/HelpColor= 写 +0xc/+0x4a/+0x11 等。R25-e 活体"未命中 FORCELOCAL/
  EnviMode 分支"与本静态结论一致。
- 附带发现（不改语义，登记）：msvc EvalSpecialToken 的 `-mode` 判定
  `((uint16_t)(*p + 1) | 0x20) == 0x6d`（core_b2e.c:1386）把 dc 的 `p[1]|0x20=='m'`
  （dc:27025）写成了 `(*p)+1`（'-'=0x2d → 0x2e，恒≠0x6d）⇒ **msvc 的 `-mode` 分支恒死**。
  与 039 无关，但属读侧失真，建议另行登记 divergence（TODO(verify)：dc 原文 27022-27027 复核）。

### (c) ScriptInit / 0xf0 克隆清零覆盖
- calloc(1,0xf0)（core_execmain.c:212）全零；ScriptCopy 逐字段含 `d[0xd]=s[0xd]`；
  **FUN_1400186BC 不触碰 byte 0xd**（dc:13338 区与 core_exec5.c:108-156 一一比对成立），
  故 clone+0xd ≡ 父+0xd，链根 g_Script 为 BSS 零 ⇒ 正常路径克隆体恒 0。
- 传导面注意：RUN/`{}`块/SUB 等每层克隆都会把上一层 +0xd 原样带下去（C4 传导项）；
  FUN_1400186BC 内 msvc 将"父引用"写 +0x58 而 dc 写 param_1[7]=+0x38（core_exec5.c:112-118，
  作者注 S11 汇编@1400186cc 实证 +0x58 自锚）——字节 0xd 不受影响，此差异**不构成**本案根因，
  仅在案备查（TODO(verify) 已有作者汇编注，不重复立案）。

### (d) TEAM / ProcessScriptBlock 的临时置位-复位
- dc TEAM = FUN_140025980（22656，size 33）→ FUN_140025474（22474，size 1290）：按 `|` 分段后
  **以同一 script 重入 PSB（FUN_14004c0bc，dc:43816）**，全程无 +0xd 写点；FUN_1400b638c 的
  "存-置位-恢复"对（113253/113269/113383）是**唯一**的临时置位机制，且只属于 RunScriptText。
- msvc: `PECMD_ParseValueCommand`（core_b2a.c:94）→ `PECMD_ParseCommandBlock`
  （core_b2d.c:447，尾部 `PECMD_ProcessScriptBlock(script,…)` :632 同形重入）→
  restored_bodies.c PSB 直移体（6099-6103 整行展开门）——同样无 +0xd 写。
- 结论：**TEAM 执行本身在两侧都不改 +0xd**；R25-e"TEAM 行展开前变 1"的写者必然位于
  LOAD→展开窗口内（§3.1 表）或为非映射写入（C5）。

---

## §5 修复建议与活体收口配方

### 5.1 修复原则（纪律约束）
- **禁止行为注入**：`b961 if (local_230 != 0) continue;` 与 xflag 读法均为 dc 同构真值，
  **不得**在展开器加"039 特判/位掩码兜底"。
- 修复点必须在写者侧：若 C1 证实 ⇒ 修正调用点 flags 或补齐 pPersist 恢复条件使之与 dc 一致；
  若 C2 证实 ⇒ 摘除非法的 `@ENVI^ ForceLocal=1` 插入或其触发面；若 C5 证实 ⇒ 走堆越界排查
  （E 簇 TEAM 堆损坏范式，pageheap/哨兵）。在活体收口前，本线**不落地任何 src 改动**，
  并建议在 HANDOVER_PROGRESS 登记为 "039 写入点待活体定案（本报告已排除 4/5 静态候选）"。

### 5.2 活体一次定位配方（V-Gate 合规，选址=build/msvc/symsnap.txt，build=1b6a17a=部署版）
1. **对象甄别**（区分 clone1/clone2/g_Script，防测量对象误认=C5-a）：
   `bp pecmd_msvc+0x117d90`（PECMD_ExpandEnvVars）→ 首次命中 `r @rcx`（script 形参）+
   `db @rcx+0xd L1`；同时 `x pecmd_msvc!g_Script` 取全局地址比对。若 rcx==g_Script ⇒
   推翻"克隆体"模型，直接转入对 g_Script 的写入者狩猎。
2. **写者捕获**：从上一步拿到的展开期脚本对象 P，`ba w1 /w 1 P+0xd`（硬件写断点，零地址依赖
   变体见 vgate-live-debug 技能）→ `g` → 命中即得写入 RIP，回 map 反查符号/行。
3. **阳性预置断点**（缩短狩猎面，地址经 `u` 反汇编窗口内扫描 `or byte ptr [r??+0Dh],1` 确认指令边界）：
   - `u pecmd_msvc+0x11a8a0 L400` 定位 RunScriptText 内 `|=1`（源 core_execmain.c:254）与
     收尾恢复（:375）两条指令；
   - ParseEnvSwitches/LaunchAsyncScriptThread 不在 symsnap 14 项内，按 V1 须先
     `tools/make_symsnap.sh` 扩充或从 `build/msvc/pecmd_msvc.map` 取址（V1 合规来源）。
4. **阴性对照**：原版 EXE 同点观察 +0xd≡0（金标对照），排除"测量本身含误"。
5. 三关验收（当期构建复现 / 阳性对照 / 第二代理证伪）后再落修复补丁。

### 5.3 建议登记的伴生差异（非本案根因，另行分诊）
- `core_b2e.c:1386` EvalSpecialToken `-mode` 判定 `(*p + 1)` 应为 `p[1]`（恒死分支，见 §4b）。
- 死代码：`PECMD_ExecuteScriptBlock`（core_scriptdep.c:296，dc FUN_140031068 忠实体）零调用者，
  与 srx_ExecuteScriptFile（S10 替代体）长期并存，后续二选一收敛（登记 SKIP(工程收敛)）。

---

## 附：证据文件/行号速查

- dc 写者: 7284 / 12776 / 18034-18047 / 113253-113269 / 113383 / 110197-110199
- dc 读锚: 68793 / 77738 / 78290 / 78559-78574(b961) / 78730 / 129577
- dc 路径锚: 23752(全局 init) / 45821(顶层 RunScriptText) / 29759-29764(LOAD *local 位4) /
  30046-30064(LOAD→140031068) / 29349(FUN_140031068) / 22240(PrependEnviHeader 插行) /
  27052-27068(EvalSpecialToken→ParseEnvSwitches) / 44227(SET→EvalSpecialToken) /
  22656+22474(TEAM→PSB 重入) / 44660(TEAM 分派) / 43816(PSB)
- msvc: core_execmain.c:212-229/238/254/375、core_script2.c:615/636-637、core_scriptrun.c:109/232/580/607、
  core_srparse.c:64-171、core_execline.c:138/656/829/1059-1060、core_exec5.c:40/108-156/212-288、
  core_b1_remaining.c:5158-5212-5290/9360-9383、core_b3r_i28c.c:846/1241-1244、
  core_scriptdep.c:127-170/296/377、core_b2a.c:94、core_b2d.c:447/632、restored_bodies.c:6099-6103/6240/7792、
  core_b2e.c:1358/1386/1388/1406
- 身份: C:\pectest\DEPLOYED_BUILD.txt(1b6a17a) ≡ build/msvc/symsnap.txt(1b6a17a) ≡ git HEAD

---

## 增补节 · R25-f 第二轮静态取证（同日子代理 A 续跑，独立复核 + 新增证据）

本节为对上文报告的独立复核与增量。复核结论：§2/§3 写者清单与同构判定**全部复核成立**（独立 grep
`\+ ?0xd\)\s*[|^&]?=` 全仓 regex 闭环，9 处赋值 = 上文清单，无一多无一漏）；039 主链 flags 推演
（§2.3/§3.1）复核成立。以下为**新增**证据与修正。

### A1. RunScriptText 调用点双侧枚举收口（补上文 C1 "未枚举穷尽"缺口）

msvc 全部 7 个 `PECMD_RunScriptText` 调用点（grep 全仓，含 restored_bodies.c 零命中）与 dc 对应：

| msvc 调用点 | dc 对照 | flags 组合（位4 视角） | 判定 |
|---|---|---|---|
| core_script2.c:636（启动顶层） | dc:45821-45823 | `(bVar31&1)\|seedFlags\|0x80`，seedFlags 低16被 `&2u` 截断 | 位4=0，同构 |
| core_scriptrun.c:232（srx 装载） | dc:29455（经 FUN_140031068） | `flags&0xfffffff7`；dc=`uVar7&2\|uVar14<<0x10\|param_4&~8` | 位4=0（param_4=0），同构 |
| core_scriptrun.c:536（**mem** 变量路径） | dc:30326 | `(seed<<16)\|flags\|0x40`，flags=RunCommand 前缀累积 | 位4 仅 `*local` 前缀（SrParsePrefix core_srparse.c:155-160），039 无前缀 ⇒ 0，同构 |
| core_scriptrun.c:650（**资源**路径） | **dc:30219-30222** | msvc=`(uSeed<<16)\|kf\|0x40`，**kf 低字节=种子低8位**；dc=`uVar35<<0x10\|(uVar31\|uVar24)&0xffff\|local_290\|0x40`（uVar24=InvokeSubRoutine 返回值 dc:30189） | **不同构（见 A2）** |
| core_b2f.c:6058（**mem** 内联文本执行器） | dc:33498 | 两侧均 `uVar11<<0x10\|0x40`（uVar11=随机种子，仅进位16+） | 位4=0，**同构（本轮实核对闭）** |
| core_scriptdep.c:381（ExecuteScriptBlock） | dc:29455 同体 | `(r&2)\|(key<<16)\|flags&~8` | 位4=0；**该函数 msvc 零调用者（死代码）**，不触发 |
| core_b2d.c:851（文本窗口 0x44d） | dc:24508 | flags=0 | 位4=0，同构 |

⇒ C1 的"某个调用点 flags 含 0x10"在**静态可达面上收窄为唯一异构点：core_scriptrun.c:650（A2）**；
其余 6 点全部位4=0。若活体调用栈不落在 :650，则 C1-a 排除、C5（非映射写/对象误认）升级为首选。

### A2. C1-a：资源路径把随机种子低字节注入 RunScriptText flags（新增具体根因候选）

- `src/lang/core_scriptrun.c:637-655`：`kf = (uint64_t)(uint16_t)uSeed; kf = (kf<<16) | (kf & 0xff);`
  随后 `RunScriptText(…, ((uint64_t)uSeed<<16) | kf | 0x40, …)` —— **flags bit0-bit7 = 种子低 8 位**，
  其中位4(0x10) 以 1/16 概率置位 → 一旦置位即经 core_execmain.c:254 写 clone+0xd 位0=1（且该调用
  pPersist=NULL 走克隆、收尾恢复，位0 只影响本次执行——行为与 dc FORCELOCAL 同形但**触发随机**）。
- dc 对照（30219-30222）：`uVar35<<0x10 | (uVar31|uVar24)&0xffff | local_290 | 0x40`，
  uVar24 = **FUN_140030dcc(InvokeSubRoutine) 返回值**（dc:30189）——种子低字节从不进入 flags 低 16 位。
- 源内自标 `TODO(verify): 反编译 707-721 的详细 flags 组合` —— 与本发现互证。
- 039 主链（裸路径）静态不走该分支；**若 §5.2 活体调用栈落点 = core_scriptrun.c:650，则它即根因**，
  修法 = 按 dc:30219 组合重写该处 flags（证据充分，可直接修+全量对拍）。

### A3. `*map:` 子进程形态：msvc 桩不执行脚本（伴生发现）

- `PECMD_LoadFileMappingExec`（core_b1_remaining.c:3898=dc FUN_1400091e0 同构）与
  `PECMD_ExecLoadCommand`（core_b1_remaining.c:9606=dc FUN_14001eecc 同构）都会 spawn 子进程
  `PECMD LOAD *map:0x…`（PECMD_CreateProcessW=core_b9_remaining.c:190，真 CreateProcessW 包装）。
- 但 msvc RunCommand 的 `*map:` 分支（core_scriptrun.c:393-410）**只 MapViewOfFile 即 `goto srx_tail`
  不执行**（源内 `TODO(verify): 反编译 379-390 行映射数据拷贝与解码`）；dc 对应分支（dc:29831-29870
  → dc:30268 RunScriptText）会执行映射文本。
- 后果：msvc 下 TEXT 窗口/`--exe:`/拖放等 `*map:` 子进程形态的脚本**静默不执行**。039 主链
  （父进程内裸路径执行）不受影响，但这是 exit/产物语义的潜在差异源，建议另行立案。

### A4. 对拍证据污染警示（039 verdict 的 fs/vars 维度）

- run_case 对同一 case 双后端共享同一 out_dir（`C:\pectest\039_team_chain\`）：本轮产物时间戳
  vars.txt=12:40:09.**051**、done.txt=.**639**、exit.txt(msvc)=.**653** —— orig 后端先跑并写入的
  vars/done 可能被 msvc 轮次回捞（run_case.py 已修 result_dir 陈旧件，但 case 内跨后端共享 out_dir
  的时序未隔离）。verdict.json 的 `fs.same/vars.same=true` 存在假阳性可能。
- 建议：039 复跑用 `--exe msvc` 单后端并核对产物 mtime > 进程启动时刻；exit 维度（0 vs 2）不受此影响，
  仍是本案唯一可信判据。

### A5. 复核确认（对上文结论的独立验证记录）

- dc 写者 5 处、msvc 同构 9 赋值点：独立 regex 重扫一致。
- dc:29455（ExecuteScriptBlock→RunScriptText flags）与 dc:45821（顶层 flags）逐符号复核 = 上文 §2.3。
- `S7_BRIDGE_EXECUTOR = 0`（core_script2.c:63）——启动走 RunScriptText 原文路径，桥接分支未启用。
- `PECMD_DispatchExpressionBlock` 已是真体（restored_bodies.c:7792，非 no-op 桩），行循环以
  `PECMD_ProcessScriptBlock(param_1,…)`（:7981）重入，script 实参=RunScriptText 的 pExec，与 dc 一致。
- `-mode` 判定缺陷（上文 §4b/core_b2e.c:1386）：本轮独立复核成立（dc:27047 为 `p[1]`，msvc 误写 `*p+1`）。
- dc:33498 ≡ msvc core_b2f.c:6058（mem 执行器 flags）：**同构**（上文未核项，本轮闭合）。
- dc 全局脚本初始化：FUN_140027690/23752 与 core_init.c:90 `FUN_1400186BC(&g_Script,0)` 语义等价
  （msvc 少一步 InitObjectWithParent 包装，但 byte 0xd 结果同 = BSS 0）。

