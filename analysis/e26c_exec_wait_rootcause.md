# FUN_14000e26c — EXEC 动词等待环被绕过（退出码 259 STILL_ACTIVE）根因定位

> 只读分析 · 权威语义源 `decompiled.c`(下称 dc) · 我方转写体 `refactored/restored_bodies.c`(下称 rb)
> 函数体对应：FUN_14000e26c @dc:7726(size=33864, 7726–11670 行) ↔ rb:267–4210 一带；
> 尾段稳定偏移 dc≈rb+7459（如 dc:11482 ↔ rb:4023）。已通读 `tools/fun_14000e26c_HANDOFF.md`。
> 旧工单（rb:2292 前补裸 WaitForSingleObject）再次否决：dc 该路径不存在任何 WaitForSingleObject，
> 等待完全由 MsgWaitForMultipleObjects 多句柄环完成（dc:11494），补裸等待属行为注入。

---

## 1. 根因裁定

**裁定：A 线成立，但根因载体不是 `local_c80`，而是早退门条件的另一半——全局运行标志 `DAT_14013a24f`。**
B（句柄数组）、C（流程可达性）两线排除。

一句话机制链：

```
普通 EXEC（无 '=' 前缀） ⇒ local_c80 = 0 ⇒ 早退门第二支 ((longlong)local_d18 < 2) 恒真；
原版 DAT_14013a24f 初值字节 = 0x01 且命令循环反复置 1（dc:5558/5665），仅服务停止等收尾路径写 ≤0
  ⇒ 第一支 (DAT_14013a24f < '\x01') 恒假 ⇒ 早退门死路 ⇒ 必进 MsgWait 环直至进程句柄受信
    （dc:11493–11495）⇒ dc:9687 GetExitCodeProcess 取到真实退出码。
我方三缺陷叠加使第一支恒真：
  (a) 对象分裂：e26c 转写体读到的是 unimplemented_stubs.c:981 独立定义的 int64_t DAT_14013a24f = 0，
      而全库所有置 1/哨兵写入者写在另一对象 int8_t g_flagA24F（core_globals.c:63）上，二者无任何桥接；
  (b) 孤立对象初值 0 ≠ PE .data 真值 0x01（handoff 登记项②、REVIEW.md:3549）；
  (c) 同对象上的 PECMD_WaitTickCount 误转写体（rb:7386–7395）把 dc 的"局部预算递减"
      错写成对全局标志的递减（rb:7393），而 EXEC 主流程恰会调用它（core_execmain.c:427）
      ——即使只修 (b)，一次调用即归零。
⇒ rb:4023 早退门恒真 → 直接 goto LAB_140015c69 → 整个等待环被跳过 →
  GetExitCodeProcess（dc:9687 / rb:4061 后）在子进程仍存活时执行 → 259 STILL_ACTIVE。
```

---

## 2. 三条嫌疑线裁定明细

### A 线（早退门数据源）— **成立，载体为 DAT_14013a24f，非 local_c80**

任务原设问"若 EXEC 下 dc 的 local_c80≥2 而我方为 0/1 即为根因"——**该具体形态被证伪**：
`local_c80` 仅有一个增量点（前缀解析环中遇 `'='`，dc:9648–9650 ↔ rb 无差），普通 EXEC 下两侧恒为 0，
赋值点逐一配对等价（见 §3 表）。真正分歧在门的另一操作数：

- dc:11481–11482（rb:4022–4023 逐字同构）：
  `local_e07 = DAT_14013a24f; if ((DAT_14013a24f < '\x01') && ((longlong)local_d18 < 2)) goto LAB_140015c69;`
- `local_d18 = (_FILETIME)(longlong)(int)local_c80`（dc:11478 ↔ rb:4019）→ EXEC 下 = 0，第二支恒真；
- 于是一切取决于 `DAT_14013a24f`。它是**运行时状态全局**而非常量，全二进制写入点共 10 处：
  | dc 行 | 写值 | 宿主/语境 |
  |---|---|---|
  | 2522 | 0xff(-1) | WM_CLOSE 强关窗口路径（signedness_audit §引 dc@234） |
  | **4765** | **0** | **FUN_14000e26c 外：服务控制处理器 FUN_140008c5c，仅 param_1==1（服务停止）时清零** |
  | **5558** | **1** | 启动/命令分发 do-while 每轮（有命令行才到达） |
  | **5665** | **1** | FUN_14000a14c（WinMain 族命令行分发，LAB_14000a49a 前） |
  | 6563 / 6580 | 2 / 0xfe | TEAM 类等待 / 服务主循环 FUN_14000befc 退出分支 |
  | 45829 | 0xff(-1) | 脚本收尾（有后台任务时） |
  | 81992 / 133466 / 133746 | -1 | 线程/UI 收尾哨兵 |

  即**正常运行期 EXEC 执行时刻其值 ≥1**（初值 1 + 命令循环每轮重assert 1），第一支恒假。
- 我方侧：`DAT_14013a24f` 名下的读点全部位于 rb（:3747/:3821/:3822/:4005/:4022/:4023/:4053/:7391/:7393），
  绑定到 `unimplemented_stubs.c:981 int64_t DAT_14013a24f = 0;`；而上述 dc 写入者的我方对应体
  （core_main.c:183↔dc:5558、core_b1_remaining.c:4397↔dc:5665、:4471↔dc:6563、core_script2.c:648↔dc:45829 等）
  **全部写 `g_flagA24F`**（core_globals.c:63 定义 int8_t）。全树 grep 无任何
  `#define DAT_14013a24f …` 桥接（stubs_common.h:760 还是 `extern int64_t` 独立声明）。
  ⇒ 两个独立存储对象，启动侧永远喂不饱 e26c 的读点；孤立对象又初值 0；再叠加 (c) 主动清零。

### B 线（句柄数组填充顺序/计数）— **排除**

- 数组 `HANDLE local_970[2]`（dc:7974 ↔ rb:474）；解析期复位 `[1]=0/d08=0/[0]=0`
  （dc:9339/9346/9360 ↔ rb:1833/1840/1854）。
- 可选读事件占 [0] 并计数 1：`local_970[0]=CreateEventW(...); local_d08=1`
  （dc:10435–10436 ↔ rb:2941–2942）。
- **进程句柄最后追加**：`local_970[(int)local_d08]=local_af8.hProcess; local_d08++`
  （dc:11189–11190 ↔ rb:3725–3726）——与"DVar11-1 命中即进程受信"语义吻合
  （dc:11495 ↔ rb:4037；首环变体 dc:11401 ↔ rb:3942）。
- MsgWait 调用两侧等价：dc:11295/11494 经 FUN_14001bbac（handoff 登记按包装体内联展开），
  rb:3833/4036 直接 `MsgWaitForMultipleObjects(local_d08,local_970,0,超时,0x4ff)`，结果照常回写
  local_9a0/iVar52。顺序、计数、命中判定全对齐。非根因。

### C 线（EXEC 路径到 :4018 的可达性）— **排除**

- 快速跳过守卫：`if ((local_df4==0) && ((longlong)local_a98<1))`（dc:11179 ↔ rb:3715）——EXEC 等待场景
  （df4≠0 或 PID≥1）走 else 分支；
- 进程句柄追加后 `LAB_14001512a:` 快照 `local_e07`（dc:11210–11211 ↔ rb:3746–3747），
  `if (local_e00 != 0)`（dc:11214 ↔ rb:3750）放行（EXEC 下 param_5 位/启动路径 |=1 保证非零，
  dc:8077/9649/10418/10791）；
- 非捕获模式（无 `$`/输出重定向，dc:11257 `(local_ddc&8)` 不成立）落到入口门
  `if (local_e00 != 0xffffff9c)`（dc:11477 ↔ rb:4018）——可达。
- 附注：dc 中没有任何赋值点能产生 0xffffff9c（全函数 27 处 local_e00 引用核对），故该门实际恒开，
  两侧一致，非分歧点。症状"静默跳过等待"正是从 :4018 进入后在 :4023 早退所致，反证可达性无误。

---

## 3. 双侧证据表

### 3.1 A 线相关局部量赋值点配对（全部等价）

| 语义 | dc 行 | rb 行 | 内容 |
|---|---|---|---|
| 超时预算清零 | 8012 | 508 | `local_c28 = 0;` |
| 起始槽位哨兵 | 8020 | 516 | `local_db0 = 0xd;` |
| '=' 计数清零 | 8074 | 570 | `local_c80 = 0;` |
| '=' 计数 ++（前缀环） | 8580 | 1075 | `local_c80 = local_c80 + 1;` |
| 超时参数装入 | 8896 | 1392 | `local_c28 = local_c10.dwLowDateTime(.ft);` |
| 解析期复用 db0 | 9254/9262 | 1748/1756 | `local_db0 = (DWORD)uVar25;` |
| '=' 计数 ++（'=' 前缀） | 9650 | 2141 | `local_c80 = local_c80 + 1;`（dc:9648–9651 `'=' → e00|=1`） |
| 起始 tick | 11256 | 3792 | `local_db0 = local_df8;`（df8=GetTickCount @11249） |
| 首环早退门 | 11286 | 3822 | `if((DAT<1)&&((int)c80<2)) goto a5e;` |
| d18/c48 装载 | 11478/11479 | 4019/4020 | `d18=(int)c80; c48=(int)c28;` |
| 二环早退门 | 11482 | 4023 | `if((DAT<1)&&(d18<2)) goto c69;` |

结论：EXEC 下两侧 `local_c80` 同为 0、`local_c28/local_db0` 数据流同构——**局部量层面零分歧**。

### 3.2 DAT_14013a24f 对象分裂实证

| 位置 | 角色 | 内容 |
|---|---|---|
| dc:5558/5665 | 原版置 1 | 命令循环每轮 reassert |
| dc:4765 | 原版清零 | 服务停止专用（FUN_140008c5c param_1==1） |
| handoff 登记项② / REVIEW.md:3549 / docs/bc_evaluation_inputs.md:107 | 真值留档 | `.data` 字节真值 **0x01** vs 早期定义 0 |
| unimplemented_stubs.c:981 | **对象甲定义** | `int64_t DAT_14013a24f = 0;` |
| stubs_common.h:760 | 对象甲声明 | `extern int64_t DAT_14013a24f;`（rb:2 包含本头 ⇒ e26c 九处读点全绑对象甲） |
| core_globals.c:62–63 | **对象乙定义** | `int8_t g_flagA24F;`（零初始化） |
| pecmd_defs.h:116 | 对象乙声明 | `extern int8_t g_flagA24F;` |
| core_main.c:183 ↔ dc:5558 | 乙上置 1 | `g_flagA24F = 1;`（主循环每轮） |
| core_b1_remaining.c:4397/4471/4492/1836 ↔ dc:5665/6563/6580/2522 | 乙上写 1/2/0xfe/0xff | — |
| core_script2.c:648 ↔ dc:45829 | 乙上写 -1 | — |
| rb:7393 | 甲上错误递减 | `DAT_14013a24f = DAT_14013a24f - 1;` |

### 3.3 PECMD_WaitTickCount 误转写实证（dc:59106–59144 ↔ rb:7385–7395）

dc 真体（FUN_140061470）：10ms 窗口内 PeekMessageW(PM_REMOVE)→Translate→Dispatch 泵，
**递减的是局部预算 `iVar4`（初值 9），从不写 DAT_14013a24f**（dc:59121/59134–59141）。
rb 版把泵体整个替换成 `DAT_14013a24f -= 1`，且调用点就在 EXEC 主流程收尾
（src/lang/core_execmain.c:427；另见 commands/core_b3_remaining.c:20360/21080）。

### 3.4 等待环本体（佐证逐行一致）

dc:11480–11517 ↔ rb:4021–4059：门→1000ms/截止预算（dc:11483–11492 ↔ rb:4024–4033）→
MsgWait（dc:11494 ↔ rb:4036）→ 末句柄命中出口（dc:11495 ↔ rb:4037）→ 管道 ReadFile（dc:11503 ↔ rb:4045）→
PeekMessage 泵（dc:11511–11516 ↔ rb:4053–4058）→ 回环头。汇合点 LAB_140015a5e/LAB_140015c69
（dc:9665/9667）→ 终止守卫（dc:9673–9685，`local_e07>0` 才考虑杀树）→ **GetExitCodeProcess**
（dc:9687）→ 结果写 `*param_6`（dc:9688）。

---

## 4. 最小忠实修复补丁（unified diff 风格，**不应用**）

修复原则：不触碰 e26c 转写体本身（它与 dc 逐行一致）；让"同一个原版全局"回到单一存储对象，
并消除对该对象的唯一伪写点。补丁①②为核心根因修复，③为同路径必需伴随修复，④可选加固。

```diff
--- a/refactored/include/stubs_common.h
+++ b/refactored/include/stubs_common.h
@@ -757,11 +757,16 @@
 extern int64_t (*DAT_14013ce30)(void);
 extern void *DAT_14013d3b8;
 ;
 ;
-extern int64_t DAT_14013a24f;
+/* DAT_14013a24f(@0x14013a24f) 与 g_flagA24F 是同一原版全局, 此前裂为两个独立存储对象:
+ * 本头旧 extern int64_t 声明 + unimplemented_stubs.c 的 int64_t=0 定义, 使 restored_bodies.c
+ * (仅包含本头) 的 9 处读点永远看不到 core_main.c:183 等启动侧写入(它们写 core_globals.c 的
+ * int8_t g_flagA24F)。按 unimplemented_stubs.c T1b(g_hHeap) 先例改宏别名统一对象;
+ * 类型取 int8_t = dc 有符号 char 语义(signedness_audit #1), .data 真值字节 0x01。 */
+#define DAT_14013a24f g_flagA24F
 ;
 ;
 extern int64_t DAT_14013a24c;
```

```diff
--- a/refactored/unimplemented_stubs.c
+++ b/refactored/unimplemented_stubs.c
@@ -978,7 +978,8 @@
 void *DAT_14013d3b8 = 0;
-/* [已删除] int64_t DAT_14013a24f = 0;
- * 重复定义且初值违背 PE 真值(0x01, REVIEW.md §3549/handoff 登记项②):
- * 它使 FUN_14000e26c 的 EXEC 等待环早退门(rb:4023)恒真 → 子进程退出码读得 259。
- * 现统一绑定 core_globals.c 的 int8_t g_flagA24F(stubs_common.h 宏别名)。 */
 int64_t DAT_14013a24c = 0;
```

```diff
--- a/refactored/restored_bodies.c
+++ b/refactored/restored_bodies.c
@@ -7385,17 +7385,36 @@
-/* @0x140061470 size=…10ms 対标志 超时循环(直移) */
+/* @0x140061470 size=149 — dc:59109-59144 直移: 10ms 窗口内消息泵
+ * (PeekMessageW PM_REMOVE → Translate → Dispatch, 至多派发 9 条);
+ * dc 递减的是局部预算 iVar4, 从不写 DAT_14013a24f。旧体把它错写为
+ * 全局标志递减, EXEC 流程(core_execmain.c:427)一次调用即把等待门清零。 */
 void PECMD_WaitTickCount(void)
 {
-  uint32_t t0 = GetTickCount();
-  for (;;) {
-    if (t0 + 10 == GetTickCount()) return;
-    if (DAT_14013a24f < 1) return;
-    if ((int)((t0 + 10) - GetTickCount()) < 1) return;
-    DAT_14013a24f = DAT_14013a24f - 1;
-  }
-}   /* CoFreeUnusedLibrariesEx */
+  DWORD tickBase;
+  DWORD tickNow;
+  BOOL  bPumped;
+  int   pumpBudget = 9;              /* dc:59121 iVar4 = 9 */
+  tagMSG msg;
+  ZeroMemory(&msg, sizeof(msg));     /* dc:59118-59119 hwnd=0 且整块清零(栈上含填充) */
+  tickBase = GetTickCount();         /* dc:59120 DVar1 */
+  for (;;) {
+    tickNow = GetTickCount();        /* dc:59123 */
+    if (tickBase + 10 == tickNow) return;          /* dc:59124-59125 */
+    if (DAT_14013a24f < '\x01') return;            /* dc:59127-59128 */
+    tickNow = GetTickCount();        /* dc:59130 */
+    if ((int)((tickBase + 10) - tickNow) < 1) return; /* dc:59131-59132 */
+    bPumped = PeekMessageW(&msg, (HWND)0, 0, 0, 1); /* dc:59134, PM_REMOVE */
+    if (bPumped == 0) return;        /* dc:59135 break → 59143 return */
+    TranslateMessage(&msg);          /* dc:59136 */
+    DispatchMessageW(&msg);          /* dc:59137 */
+    pumpBudget--;                    /* dc:59138 递减局部预算, 非全局标志 */
+    if (pumpBudget < 0) return;      /* dc:59139-59140 */
+  }
+}
```

```diff
--- a/refactored/src/kernel/core_globals.c   【可选加固】
+++ b/refactored/src/kernel/core_globals.c
@@ -62,3 +62,3 @@
-int8_t
-    g_flagA24F; /* DAT_14013a24f 运行标志(dc 写入-1/0xff/0xfe 哨兵, 读侧有符号序比较; 审计#1根因) */
+int8_t g_flagA24F = 1; /* DAT_14013a24f 运行标志; PE .data 真值 0x01(pe_data_extract 提取,
+                       REVIEW.md §3549 留档); 运行期 dc:5558/5665 每轮命令循环重 assert 1,
+                       仅服务停止(dc:4765)/收尾哨兵写 <=0。审计#1 已定有符号 char 语义。 */
```

说明：
- ①②落地后，rb 的九处 `DAT_14013a24f` 读点自动绑定到启动侧维护的 int8_t 对象，负哨兵
  （-1/0xff/0xfe）跨 TU 可见，恢复 dc 的有符号序比较语义（审计#1 的既定方向）。
- ④为可选：正常入口下 core_main.c:183 已每轮置 1，不加亦能修复本 bug；加上则与静态映像真值严格
  一致，覆盖非常规入口（如直接以服务态进入 EXEC 的边角路径，dc 初值同为 1）。
- 不采纳方案：在 rb:2292 前插裸 WaitForSingleObject（dc 无此调用，且治标掩盖门条件失真）；
  或仅改 unimplemented_stubs.c:981 的初值为 1（不修对象分裂时启动侧写入依旧不可达，且
  rb:7393 一次递减即归零——见 3.3）。

## 5. 置信度与残留风险

- **根因裁定置信度：高（~92%）**。证据闭环：早退门双支结构静态可判定；EXEC 下 c80=0 两侧一致；
  原版标志 ≥1 有初值真值提取 + 10 处写入点语境支撑；我方对象分裂/初值 0/伪递减三点均有精确行号。
- B/C 排除置信度：高（填充序、计数、命中判定、入口守卫逐点配对）。
- 残留风险：① `param_5`/`local_e00` 位语义未全量展开（dc:8077/9649/10418/10791），不影响本根因，
  但若后续 EXEC 变体（`=`/`!` 组合）出现等待行为差异应优先复查此处；② `local_dc0`（终止守卫
  dc:9673–9685 的杀树开关，dc:8015/8238/8898）在我方的对应实现值得单独复核；③ 补丁应用后需按
  build.sh + gcc link 流程回归（本次纪律禁止构建，未验证编译通过）；④ g_flagA24F 其余 ~45 处
  使用点在对象统一后行为整体向 dc 收敛，建议按 docs/bc_evaluation_inputs.md L8 登记 C-P1 复核。
