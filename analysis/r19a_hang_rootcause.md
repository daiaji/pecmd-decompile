# R19-A 簇挂起根因复审（002_envi_assign / 004_envi_global_scope / 021_sub_call / 037_sub_nested / 038_sub_param_shadow）

> 只读分析 · 权威语义源 `decompiled.c`(dc) · 我方转写体 `refactored/restored_bodies.c`(rb) + `refactored/src/**`
> 上游文档：`analysis/e26c_exec_wait_rootcause.md`（EXEC 259 根因）、`analysis/r19_triage_54fails.md`（A 簇初判）
> 新证据源：`C:\pectest\memfail.log` —— 构建内建 TEMP PROBE（`[PSB]/[WB]/[DEB]/[S8] ExpandVR/[CPW]` 等，
> 每条 fprintf 后立即 fclose，单线程区内顺序可靠；对应 2026-08-26 17:56–18:04 语料全量跑残留）。

---

## 0. 结论速览

**裁定：R19 分诊 A 簇初判「rb:4021 MsgWait 环对非 EXEC 路径缺 deadline」不成立（证伪）。**

1. **五案没有一个进入过 e26c / MsgWait 环。** 探针日志显示五案进程死亡时均处于
   **main.pecmd 主脚本的 ENVI/CALL/TEAM 行处理期**，远早于尾声 `EXEC =cmd.exe`；
   全程零 `[CPW]`（CreateProcess 包装）记录、零 `[ECD]` 记录、`vars.txt/done.txt` 双缺
   （若卡在环里，cmd.exe 子进程照样会写出 vars.txt——实际连它都没有）。
2. **反证（最强）**：每个用例的尾声都含与 064 同形的 `EXEC =cmd.exe ...`；001_envi_smoke
   走完全相同的 e26c EXEC→等待→取码链路并 exit=0（日志含 `[CPW] ret=1` 与完整 `[S7] tail-*` 收尾序列）。
   即我方构建中 e26c 等待环本身工作正常——它只是从未被这五案执行到。
3. **dc 侧结构本来就不让普通命令进环**（§2）：ENVI/CALL/SUB 在四字码分发表即被分流到
   专用处理器，只有四字码全未命中的行才落 LAB_14004df3c 兜底进 e26c；即便进了 e26c，
   快跳守卫 dc:11179 `(local_df4==0 && a98<1)` 也直接去清理返回。
   「SUB/GOTO 时 param_8/hProcess==NULL 如何出环」之问因此消解：那条路在 dc 根本不存在。
4. **真实死点（探针实证，见 §1 表）**落在三条互不相同但共享基础设施的路径上：
   - 021/037：CALL→子过程解析/调用机器（标签拷出后静默停摆）；
   - 038：CALL F,param 的子过程体从未执行、主线程随后阻塞；
   - 004：TEAM 内嵌行的 ProcessScriptBlock 入口后即停摆，主线程亦不再推进；
   - 002：ENVI 值展开前置区（PSB 级整行展开已返回、处理器级名侧展开未开始）。
   该三面正是 `analysis/sub_hang_expandvars_static_notes.md` 已登记的 H2/H3/H5 嫌疑面
   （展开器乒乓缓冲 / ctx 重配后扫描器失 NUL 停机 / TEAM 同步），与 e26c 无关。
5. **对 e26c 转写体不做任何修改**即为本轮正确处置；R19 的 DAT_14013a24f 宏别名修复
   （stubs_common.h:763）针对的是 EXEC 家族早退门语义且已被 064 通过验证，应保留。

置信度：结论 1/2/3 高（~95%，探针+双侧静态双证）；结论 4 的具体自旋语句定位为中
（死点区间已用探针夹逼到函数级，最终一条指令需活栈确认，本轮纪律禁止调试器/运行）。

---

## 1. 探针死点证据表（memfail.log 实测）

每行脚本的标准探针序列（通过例）：
`[DEB] line=` → `[PSB] line=` → `[S8] SCWin`+`StrCopyW`（rb:6096/dc:44086 整行拷贝）
→ `[S8] ExpandVR-in/out`×1（rb:6119/dc:44109 PSB 级整行展开）
→ `[WB] verb=%08x`（rb:7174，LAB_14004c525 汇合点=分发之后）
（含 %var% 行另有第二对 ExpandVR = 处理器内部名侧展开，如 ENVI→PECMD_ExpandSpecialDirs rb:5711 ↔ dc:26926）

| 案 | 最后处理的行 | 最后一条探针 | 缺失的预期探针 | 死点判定区间 |
|---|---|---|---|---|
| 002 | `ENVI C=%A%_%B%` | `ExpandVR-out … b=9`（log:220） | 第二对 ExpandVR-in、`[WB]` | rb:6120–6595（四字码链）＋PECMD_EvalSpecialToken 前奏＋PECMD_ExpandSpecialDirs 前奏至 rb:5711 之前（dc:26728+/27011+ 对应区）。该区内仅存分配点 AllocWStringBuffer(rb:5563)/AllocStrSlot(rb:5596) 与纯扫描 |
| 004 | `TEAM ENVI A=inner`（worker 内层） | 内层 `[PSB] line=ENVI A=inner`（log:421） | 内层 SCWin/StrCopyW 及其后全部；主线程此后也无任何新行探针 | 内层 PSB rb:5907–6095（RandSeedAdvance/SkipLeadingControls/AllocStrSlot(rb:5988)）；主线程疑似阻塞于 TEAM 等待 |
| 021 | `CALL F` | `SCWin`+`StrCopyW b=1`（log:2391-2392，从展开文本 +0xA 处拷出标签 "F"） | `[WB]`、后续任何行 | CALL 处理器（PECMD_DispatchCommand→ExecuteCommand @0x1400a4dcc 族）→ 子过程标签解析/调用机器内部 |
| 037 | `CALL OUTER` | `SCWin`+`StrCopyW b=5`（log:4381-4382，拷出 "OUTER"） | 同上 | 同上 |
| 038 | `CALL F,param` | `ExpandVR-out b=6`（log:4451）；随后主线程正常走到 `ENVI H=%a%` 完成（log:4453-4458）后整体静默 | CALL 行的 `[WB]`、子过程体 `_SUB F,a`/`TEAM ENVI G=%a%` 的任何执行痕迹、`ENVI T_CASE=` 及尾声全部 | CALL 分发返回异常路径（跳过 LAB_14004c525）＋子体调度缺失；主线程随后阻塞于某同步点 |

对照通过例（同一日志）：`ENVI B=%A%`（003）完整走完两对 ExpandVR+`[WB]`（log:277-285）；
001 尾声 `EXEC =cmd.exe` 有 `[CPW] … ret=1 gle=0` 且进程收尾干净。**e26c 环可达性与可用性均被通过例证明。**

---

## 2. 正面回答任务问题 1：dc 中「非 EXEC 普通 ENVI/SUB」与等待环的关系

### 2.1 普通 ENVI/SUB 根本不经过 FUN_14000e26c

dc 的单行动词分发器 FUN_14004c0bc（ProcessScriptBlock）四字码 if-链：

| 动词 | fourcc | dc 行 | 去向 |
|---|---|---|---|
| SET | 0x544553 | dc:44222-44227 | `goto LAB_14004c930` → **FUN_14002d33c**（cVar6='\x01'） |
| ENVI | 0x49564e45 | dc:44557-44561 | `goto LAB_14004c930` → **FUN_14002d33c**（cVar6='\0'） |
| CALL | 0x4c4c4143 | dc:44587-44590 | 非 '$' 前缀 → `LAB_14004e164`(dc:44304) → **FUN_140045c44**(dc:44310) |
| _SUB/_END 定义 | — | FUN_14004e488 行遍历器深度计数吸收（dc:45287/45372/45378） | 不分发 |

FUN_14002d33c（dc:27011-27138，969 字节）内部只走变量读写（FUN_14002cc30/FUN_1400c6324/
FUN_14007a224 等），**无任何句柄等待**。FUN_140045c44（dc:40493-40506，75 字节）按首字符
'$'/'@'/其它三分，同样无等待。**唯一把普通行送进 e26c 的是四字码全未命中时的兜底分支
LAB_14004df3c → dc:44294 `FUN_14000e26c(param_1, 展开行, param_1, local_150, 1, 0,0,0)`**
（且其前置条件 dc:44269 `(sVar26==0 && res8低16==0 && ParseCommandPath 返回<0)` 要求该行
不是 '@label' 引用）。ENVI/CALL 都在链上被截走，永不入兜底。

### 2.2 即便进入 e26c：快跳守卫

e26c 是 EXEC 家族执行器（入口特殊化 /installservice 等检查 dc:8024-8072）。函数尾部：

```c
11179: if ((local_df4 == 0) && ((longlong)local_a98 < 1)) {      // 无进程尝试 且 无 PID
11182:     if (*local_d58 == L'\0') goto LAB_140014f96;          // 无捕获输出变量 → 直达清理
           … FUN_1400629b8(param_1, local_d58, &DAT_14011c638);  // 有输出变量则写回后同样落到清理
       }
11187: else { … 追加 hProcess 进 local_970[]、进 LAB_14001512a 等待机制 … }
```

`LAB_140014f96`（dc:11525）＝关闭句柄族＋`return _Var39`，全程零等待。
**df4/a98 的取值由命令性质决定**：只有真正走过 CreateProcess/ShellExecuteEx 路径
（df4 写点 dc:10485/10552/10577/10770/10968/11051/11069…）或 '#PID' 语法解析
（a98 ← FUN_1400c1194 @dc:9127）才会离开「df4==0 ∧ a98==-1<1」状态。

### 2.3 等待环本体只为 EXEC 存在，出口共四个

环 dc:11480-11517（↔rb:4021-4059）。到达前提＝已入 else 支（有进程/PID）∧ `local_e00!=0`
（dc:11214，param_5 位/启动路径置位）∧ 非捕获或捕获完成。出口：

| # | 条件 | dc 行 | 语义 |
|---|---|---|---|
| E1 | `(DAT_14013a24f < '\x01') && ((longlong)local_d18 < 2)` | 11482 | 服务停止标志∧'='计数<2 早退门（正常运行 DAT≥1 恒假——上一轮已定） |
| E2 | `0 < local_c48` 且 `local_c28 <= GetTickCount()-local_db0` | 11484-11486 | 截止时间到 → 取码返回。**c48=(int)c28 仅当 EXEC 携带超时参数经 dc:8896 装入才 >0；无超时参数时无总预算** |
| E3 | `DVar11-1 == iVar52`（末句柄受信） | 11493-11495 | 进程句柄（最后追加者 dc:11189-11190）signal → 成功路径 |
| E4 | `local_e00 == 0xffffff9c` | 11477→11519 | 全函数无赋值点可产生该值（上轮 C 线核对 27 处），恒开门 |

**回答任务原文**：「哪个条件让原版（普通命令）走完一次就出去」——都不是环内条件，
而是**普通命令在 dc:11179 就 goto 清理、从未进环**；EXEC 则靠 E2/E3 出环。
「local_970[]/d08 在非进程命令下装什么」——装的是复位值 0（dc:9346/9360/9339），
因为非进程路径根本不会带计数进环。

### 2.4 任务问题 3：param_8 / hProcess 为 NULL 的分支

- `param_8` 仅一个使用点 dc:11526 `if (param_8 != 0) *param_8 = local_af8.hProcess;`
  （调用方要回传进程句柄的出参，如 dc:1642/12000/40100 处 `&DAT_14013c9c8`）。
  NULL 判空齐全，GOTO/SUB 类调用（param_8=NULL）无影响。
- `hProcess==NULL` 仍会追加进环的情形：else 支成立但 CreateProcess 失败而 '#PID'≥1
  （dc:11188-11190 先 `e00 &= -(hProcess!=0)`、`local_970[d08]=NULL; d08++`）。
  此时环内等待对象只剩可选读事件（dc:10435-10436 建 Event 时 d08=1），dc 依靠
  E2 截止预算（EXEC 显式/隐式超时）或 E1 出环；SUB/GOTO 不构造此状态。
- 结论：**dc 没有「SUB 跳转防进环/立即出环」的特殊机制，因为它压根不进 e26c。**

---

## 3. 任务问题 2：双侧局部量填充点配对（环相关全部赋值点）

dc 区间 7726-11674 全量 grep ↔ rb 对应体（rb 行号按同构转写核对）：

| 量 | dc 赋值点 | rb 对应 | 内容 | 配对 |
|---|---|---|---|---|
| local_c80 | 8074 / 8580 / 9650 | 570 / 1075 / 2141 | =0；'=' 前缀 ++ ×2 | ✓ |
| local_c28 | 8012 / 8896 | 508 / 1392 | =0；超时参数 .dwLowDateTime 装入 | ✓ |
| local_c48 | 8146 / 8348 / 8415 / 11479 | 641 / 853 / 920* / 2142* | NULL；解析装入；空串归零；环前 =(int)c28 | ✓ |
| local_db0 | 8020 / 9254 / 9262 / 9333 / 10697 / 10701 / 11256 / 11279 | 516 / 1748 / 1756 / 1826* / — / — / 3792 / 3815 | 0xd；uVar25；=0；计数器两处；起始 tick；捕获模式复用 | ✓（10697/10701 为 Ghidra 同槽复用的无关计数） |
| local_d08 | 8010 / 9346 / 10436 / 11190 | — / 1840* / 2941* / 3726 | 0x61（盘符比较同槽复用）；=0 复位；=1 读事件；++ | ✓ |
| local_970[] | 7974 / 9339 / 9360 / 11189 | 474 / 1833* / 1854* / 3725 | 声明[2]；[1]=0；[0]=0；[d08]=hProcess | ✓ |
| local_a98 | 8425 / 9127(填) | 921 / 1621 | =-1；'#(...' PID 解析填入 | ✓ |
| local_df4 | 9344 / 10485-11085 / 11279 | 1838 / 2991-3619 / 3815 | =0 复位；各进程创建结果；捕获复用 | ✓ |

\* rb 行号为上一轮文档登记值，本次抽查 rb:1838(df4=0)、rb:921(a98=-1)、rb:3725-3726、rb:4019-4020 逐一属实。

**裁定：环相关的初始化在我方无缺失、无错序；rb:4018-4059 与 dc:11477-11517 逐行同构。**
这从转写体侧再次否证「环初始化缺陷导致五案挂起」——五案根本没执行到这里。

---

## 4. 真实根因定位（按死点分解；待活栈收口项已标注）

五案共享的行为学：**静默停摆（无 AV、无 fastfail、exit=124 击杀）**，死点全部位于
「字符串/脚本缓冲扫描型循环或线程同步」家族。对应已知嫌疑面：

### 4.1 021 / 037（CALL→SUB 解析机器）
- 死点：标签（"F"/"OUTER"）从展开文本拷出后的下一位（§1 表）。
- 该位的调用链：PECMD_DispatchCommand(core_b3e.c:103 ↔ dc:40503) → PECMD_ExecuteCommand
  (@0x1400a4dcc) → ParseCommandPath/FindSubBlockNth(Scan)/InvokeSubRoutine
  （src/runtime/core_scriptdep.c:212 ↔ dc:29262 → PECMD_ParseScriptSegments core_script.c:56 ↔ dc:28874）。
- 头号机制（sub_hang 笔记 H3，本轮复核仍成立）：
  `InvokeSubRoutine` 用 `flags>>16` 重配局部 ctx 关键字块（core_scriptdep.c:231 ↔ dc:29285）。
  规范配置要求基分隔符 script[0x44]（字节 +0x88）=L'\0'，ParseScriptSegments 的段扫描
  `while (*t != s45 && *t != s48 && *t != s44) t++;`（core_script.c:81 ↔ dc:28929-28932）
  **完全依赖 s44/s45/s48 含 NUL 才能在缓冲尾停机**；一旦 c88≠0，扫描越过堆缓冲 NUL
  在堆上狂奔 → 静默挂起（页未触雷则无 AV）。dc 同构代码同样依赖该不变量，
  因此**忠实修复方向不是改扫描器，而是排查我方哪条 CALL 传参路径给了非零高位 flags
  或破坏了克隆体的关键字块**（dc 三处 InvokeSubRoutine 调用 dc:30187/30248/30319 中
  普通行路径 dc:30319 恒传 0；我方 core_scriptrun.c:540 同为 0，需核 0x1400a4dcc 族内部）。
- 待活栈收口：021 卡死的确切自旋环（FindSubBlockNthScan 两遍法数组扩容 vs 段扫描）。

### 4.2 038（CALL 分发返回异常 ＋ 子体未调度）
- CALL F,param 的处理在 ExpandVR-out 后**未出现 [WB]**却继续走了下一行 → 处理器走了
  绕过 LAB_14004c525 的出口（dc 同样存在 `goto LAB_14004c53b` 族出口），且 `_SUB F,a`
  体一行都没执行。随后主线程在 `ENVI H=%a%` 之后、`ENVI T_CASE=` 之前永久阻塞。
- 判定：CALL 的子过程调度在我方存在「吞体＋异步不同步」形态缺陷（TEAM 在体内未被触发
  即已丢失），与 4.1 同属 SUB 机器，但表现为丢执行而非自旋。

### 4.3 004（TEAM 内嵌行 + 主线程双停）
- TEAM worker 进入内层 ProcessScriptBlock 打印 `[PSB]` 后、第一条 SCWin 之前停摆；
  同时主线程不再产出任何新行探针（dc 的 TEAM 为后台线程、主流程照常前进——我方呈现
  主线程被 join/等待的形态）。候选：worker 在 AllocStrSlot(rb:5988) 一类分配点上
  与主线程堆锁/临界区互锁，或 TEAM 等待实现偏离 dc 的 fire-and-forget 语义。

### 4.4 002（ENVI 值展开前置区）
- PSB 级整行展开已返回（`%A%_%B%` 已成功替换），死点在四字码链→EvalSpecialToken→
  ExpandSpecialDirs 到达名侧展开(rb:5711)之前；区内无循环体依赖内容，唯二分配点 +
  纯扫描。单看本区不可自旋 → 与 004 同判：**堆/临界区层面的互锁最符合跨案分布**
  （死点漂移到「下一个需要进堆/锁的位置」），或存在尚未识别的无探针热路径。
- 待活栈收口：对 002 进程抓一次栈即可定案（禁令解除后第一优先）。

### 4.5 为什么此前会被误判为 MsgWait 环
R19 分诊在仅有 exit=124 面的情况下按「先验热点」标注了 rb:4021（原文自带『初判/待考』）。
本轮以探针日志+双侧配对复核将其证伪；e26c 相关的三处历史缺陷（对象分裂/初值/伪递减）
确曾真实存在且已被 R19 修复，修复本身正确（064 通过佐证），与本五案无关。

---

## 5. 最小忠实修复补丁（unified diff，**不应用**）

### 5.1 对 e26c / 等待环：**无补丁**（明确不动）

restored_bodies.c:4018-4059 与 decompiled.c:11477-11517 逐行同构（§3 配对表），
001/064 的通过证明其在真实 EXEC 路径行为正确。任何「补 deadline/补 WaitForSingleObject」
都属于行为注入（上一轮已否决过裸等待方案，此处再次否决）。

### 5.2 诊断探针补丁（下一轮活栈替代品；纯增量 fprintf，不改任何行为）

目的：把 §4 四个死点区间夹逼到最后一条语句，一次运行即可取代调试器。

```diff
*** a/refactored/src/runtime/core_scriptdep.c
--- b/refactored/src/runtime/core_scriptdep.c
@@ PECMD_InvokeSubRoutine (≈:212)
+#include <stdio.h>
+static void SG_Probe(const char *tag, long long v)
+{ FILE *f = fopen("C:\\pectest\\memfail.log","a"); if (f){ fprintf(f,"[SG] %s %llx\n",tag,(unsigned long long)v); fclose(f);} }
 uint32_t PECMD_InvokeSubRoutine(void *script, void *tmpl, uint32_t flags)
 {
     ...
+    SG_Probe("invoke-enter", (long long)(uintptr_t)flags);
     FUN_1400702B0(&nls, WSTR("\n"));
     pos = PECMD_StrChrOffset(*(const WCHAR **)script, (WCHAR)key);
+    SG_Probe("invoke-pos", (long long)pos);
     ...
     r = PECMD_ParseScriptSegments(script, 0, (int)pos, &nls, sub, flags);
+    SG_Probe("invoke-ret", (long long)r);
```

```diff
*** a/refactored/src/lang/core_script.c
--- b/refactored/src/lang/core_script.c
@@ PECMD_ParseScriptSegments 段扫描环 (≈:72-161)
     if (p < end) {
         do {
             WCHAR *t = p;
             ...
+            { extern int SG_P(const char*,long long); SG_P("seg-top",(long long)(uintptr_t)(p-base)); }
             while (*t != script[0x45] && *t != script[0x48] && *t != script[0x44])
                 t++;
+            { extern int SG_P(const char*,long long); SG_P("seg-scan-done",(long long)(uintptr_t)(t-p)); }
```

```diff
*** a/refactored/src/commands/core_b3e.c
--- b/refactored/src/commands/core_b3e.c
@@ PECMD_DispatchCommand (≈:92)
     else {
+        { /* TEMP PROBE: CALL 裸路径进出 */ extern void SG_Probe(const char*,long long);
+          SG_Probe("dispatch-exec", (long long)(uintptr_t)script); }
         PECMD_ExecuteCommand(script, cmd, (uint64_t)(uintptr_t)out, NULL, 0, 0, 0);
+        SG_Probe("dispatch-ret", 0);
         return 0;
     }
```

```diff
*** a/refactored/src/runtime/core_thread.c   （TEAM 工作体入口/出口；函数名以实际为准）
@@ TEAM worker fn
+    SG_Probe("team-worker-enter", (long long)(uintptr_t)param);
     ...既有体...
+    SG_Probe("team-worker-exit", 0);
```

以及主线程侧 TEAM 分发处对称打点。判读规则：最后一对不成对的 enter/exit 即死点所在函数。

### 5.3 若下一轮证实 H3（c88≠0 失 NUL 停机）——忠实修复模板（备用，本轮不应用）

修复原则：恢复 dc 不变量「普通脚本路径 InvokeSubRoutine 收到的 flags 高 16 位为 0」，
而不是给扫描器加边界（那会偏离 dc 行为）。示例形态（占位，实际调用点以 5.2 探针结论为准）：

```diff
*** a/refactored/src/commands/<CALL 路径文件>.c
--- b/refactored/src/commands/<CALL 路径文件>.c
@@ 传递 flags 给 InvokeSubRoutine/ParseCommandPath 链的点
-    InvokeSubXxx(script, tmpl, flags_used_here);
+    /* dc:30319 普通行恒传 0；高位 key 仅编码流资源脚本(dc:30187/30248)合法 */
+    InvokeSubXxx(script, tmpl, flags_used_here & 0xffffu);
```

---

## 6. 回归风险评估

| 处置 | 影响面 | 风险 |
|---|---|---|
| e26c/环：不改 | 9 个已 PASS 案 + 064 + 其余 | **零风险**（无代码变化） |
| R19 DAT 别名修复保留 | EXEC 家族早退门语义 | 064/001 已验证通过；保留无新增风险 |
| 5.2 探针补丁 | 全部案（I/O 增量） | 低：fprintf+fclose 每次即刷，多线程交错仅影响日志顺序不影响行为；落地后须重跑全语料对比（预期仅 memfail.log 变大，verdict 不变） |
| 5.3 备用 H3 修复 | CALL/SUB 全家（021/022/023/037/038…） | 中：掩码 `& 0xffff` 若误伤合法编码流路径（资源脚本 kf≠0 场景，core_scriptrun.c:666）会破坏 XOR 脚本——**必须以 5.2 探针确认违规传参的具体调用点后再精准施放**，禁止全链路盲改 |

---

## 7. 附：本轮核对过、可从排查面划掉的项

- e26c 环及全部局部量初始化（§3 配对表）——两侧零分歧；
- ProcessScriptBlock 四字码分发骨架（ENVI/SET/CALL 分流与 dc 一致，rb:5819-7201 vs dc:43816-45160 抽核）；
- PSB '^'/'%' 预处理环（rb:5991-6098 ↔ dc:43981-44066）终止条件一致；
- PECMD_SkipIdentifier（core_b1_remaining.c:8211 ↔ dc:16417）NUL 停机一致；
- BDA8 递归展开轮数上限（≤5+opt ≤3）与乒乓所有权（core_execline.c:68-113）终止性成立；
- OOM 重试环不构成静默自旋（PECMD_OomPrompt 恒返 0 ≠ 4，必 break，unimplemented_stubs.c:373）。

---
*生成：ox-alpha 根因定位子代理（R19-A 复审轮）；证据行号以当前工作区
decompiled.c(181421 行) / refactored 树 / C:\pectest\memfail.log 为准。*


## 【R20-A 活栈收口附录】(2026-08-26, V-Gate attach 流程实证)
021_sub_call 挂死现场(attach_process, 无需断点地址): 主线程自旋于 FUN_14001b23c+0x75
(RVA 0x1e7675) ← ParseAndExecuteLine+0xa2a ← ExecuteCommand ← DispatchCommand;
其余4线程均线程池等待位。
- dc:15775 体解密: 三停机字符取 script+0x88/0x8A/0x90, 循环无 NUL 兜底。
- 现场实测(param_1=0x1db32fca110): +0x88=栈指针0x1a4eefe700 碎片, 三字符=0xE700/0x4EEF/0x08B8 全垃圾;
  游标 *param_4=0x1a4eefe048 亦在栈上 —— 关键字块与文本区未初始化。
- 裁定升级: 非 H3 key 错配单因, 而是 CALL/SUB 构造路径对该脚本对象的初始化缺失
  (FUN_140017CDC/186BC 真体健在已排除双名; 下一步核对 ExecSubCommand(dc:44310)/
   ParseCommandPath 构造链与 dc 对 +0x88/8A/90 的全部写入点差异)。
- SG_Probe 探针网(scriptdep/script/b3e 三处)保留至修复验证后随 T5 统一拆除。
