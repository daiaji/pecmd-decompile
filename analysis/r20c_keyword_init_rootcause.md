# R20-C 关键字初始化缺失根因收口（CALL/SUB 族 exit=124 自旋）

> 只读分析子代理交付物 · 权威语义源 `D:\repo\PECMD反编译\decompiled.c`（下称 dc）
> 我方转写体 `refactored\src\**` + `refactored\restored_bodies.c`（下称 rb）
> 上游档案：`analysis/r19a_hang_rootcause.md`（含 R20-A 活栈附录）
> 构建身份：部署体 C:\pectest\pecmd_msvc.exe ≡ build\msvc（2026-08-26 19:27:28，md5 97A178C8…）；
> 工作区对 core_exec5.c / unimplemented_stubs.c / core_b3_remaining.c / core_scriptrun.c /
> core_scriptdep.c / core_execmain.c / restored_bodies.c 全部 clean（git status 空），
> 即**本报告全部行号与部署二进制一致**。

---

## 0. 裁定速览

1. **出生地**：挂死对象出生自 `src\commands\core_b3_remaining.c:24650` 的
   `operator_new(0xf0)`（PECMD_ExecuteCommand CALL 分支堆克隆）。活栈
   `FUN_14001b23c+0x75 ← ParseAndExecuteLine+0xa2a ← ExecuteCommand ← DispatchCommand`
   （r19a 附录）把 PAL 的调用者钉死在 ExecuteCommand，其内部仅两处 PAL 调用点：
   堆克隆支 rb:24669 与就地支 rb:24707；活体读数为堆地址 ⇒ 克隆支。
2. **dc 在该直接路径上不调 FUN_14006159c**：dc 全部 6 个调用点
   （dc:29285 / 29410 / 30186 / 40694 / 45805 / 110331）无一位于
   FUN_14004c0bc(PSB) / FUN_140045c44(DispatchCommand) / FUN_1400a4dcc(ExecuteCommand) /
   FUN_14004e488(PAL)。两侧的 CALL 路径关键字健康**完全靠继承**。
3. **真正的缺失点（我方遗漏的 dc 等价写入）在更上游的裸文件执行入口**：
   dc 把「裸词 → 执行脚本文件」路由进 FUN_140031068（ExecuteScriptBlock，
   dc:30061-30066），该函数**克隆脚本并以随机 key 种子化克隆的关键字块**
   （dc:29396-29410：GenRandomSeed16 + FUN_140017cdc + FUN_1400186bc×2 +
   FUN_1400629b8("&&CurDir") + **FUN_14006159c(clone, key)**）。
   我方对应支改为 `srx_ExecuteScriptFile`（core_scriptrun.c:110-234，文件头自述
   「读文件(UTF-16LE, 跳 BOM) → 明文流 → key=0 RunScriptText」），**整个 LOAD 链上
   该构造连同其关键字写入一并消失**；链上剩余的唯一覆盖是
   RunScriptText(core_execmain.c:356)→DispatchExpressionBlock 入口初始化
   （rb:7869，dc:110330-110331 同构），而活体读数证明运行对象并未被它覆盖到。
4. **排除项（本轮逐一证伪，见 §2 表）**：复制长度不足、双名桩未修复/桩竞争、
   链接错位、InitObfuscatedKeywords 本体失真、扫描器双侧差异——均不成立。
5. **补丁**：在 srx_ExecuteScriptFile 入口补回 dc 同位置的关键字写入
   （§5，形态取 dc:110331 的 `*(u16*)(obj+0x48)|0x10000` 变体以同时重建配对符表）。
   **严禁给 FUN_14001b23c 加 NUL 兜底**（dc:15789-15795 与 rb:7967-7975 逐字同构，
   加兜底＝行为注入）。

---

## 1. 自旋机制闭合（双侧同构确认）

dc:15775 FUN_14001b23c size=274 ↔ rb core_b1_remaining.c:7955 PECMD_ExtractTableSegment。

停机字符三件套：`*(short*)(param_1+0x88)` / `+0x8a` / `+0x90`
（dc:15788/15791/15792/15794 ↔ rb:7966/7969/7971/7973）。主扫描环：

```c
for (;;) {
    sVar1 = *(int16_t *)*param_4;
    if (sVar1 == kw90) break;
    if ((sVar1 == kw8a) || (++cursor, *cursor == kw88)) break;
}                    /* 无 NUL 兜底 —— dc/rb 一致 */
```

- 三字符全为**非零垃圾** ⇒ 文本终止符 NUL 不匹配 ⇒ 越过缓冲尾在堆上狂奔
  （页不触雷则无 AV）→ timeout 124。活测三字符 0xE700/0x4EEF/0x08B8 ∉ {0x0000,0x000D,0x000A} ✓。
- 三字符全为零（BSS 态）⇒ 首个 NUL 即命中 kw88/kw90 ⇒ 正常停机 ⇒ 不挂。
  **这解释了为何大量用例带零表也能 PASS**（XOR 键 +0x48=0 时动词四字码直比、
  _SUB/_END 直比均不受影响），也解释了此前 [DEB] end88=0 被
  误读为"已初始化"——**0 恰是 BSS 缺省值，与健康值低字节相同**。
- 结论：挂死当且仅当关键字块为**从未被任何写入方触碰过的 malloc 再用内存**
  （活体 +0x88 qword=0x1a4eefe700 为本线程栈址，正是再用块的典型残留——
  被释放的 pExec 克隆曾在 +0x78 存入 &bufRef 栈址 core_execmain.c:251，
  错位复用即落到新对象 +0x88）。

---

## 2. 双侧证据表（逐假设裁定）

| # | 假设/检查项 | dc 证据 | rb 证据 | 裁定 |
|---|---|---|---|---|
| 1 | ScriptCopy 是否漏拷关键字 | dc:12817(`+0x88` u16 经 `param_1+0x11`)、12818(`+0x8a`)、12820(`+0x90` 经 `param_1+0x12`)、12821-12824(0x92/94/96/98) | core_exec5.c:80/81/83/84-87 逐字段对应；函数尾至 +0xe8 与 dc:12839/12840 对齐 | **排除**：覆盖 ≥0x92，逐字段一致 |
| 2 | 双名桩竞争/恒返空桩 | rename_map.json: FUN_140017cdc≡PECMD_ScriptCopy | pecmd_msvc.map:1397 `FUN_140017CDC … core_exec5.obj`；:2259 `PECMD_ScriptCopy … unimplemented_stubs.obj`（转发体 ：248-251 → 真体）；调用方两名单一真体 | **排除**：链接正确，转发生效 |
| 3 | 克隆三连缺失（new 后不拷） | dc 12 处 new(0xf0)：82659/82721/82828/103242/103422/103957/105641/110151/113230/114369/114651/116029，脚本类均跟 ScriptCopy+ScriptInit | rb 11 处（src 10 + execmain calloc）：20686/20752/20856(malloc)/24650/24833/b3r_h4:860/i28c:1186/execmain:221 均紧跟三连；27331/27572/27835 为 UI 控件结构非脚本 | **排除**：无"裸 new 进 PAL"站点 |
| 4 | InitObfuscatedKeywords 本体失真 | dc:59195-59232（低字写 5 分隔符；高字非零才从 +0x48 派生全表） | core_exec2.c:291-322 完全一致 | **排除** |
| 5 | ExecuteCommand/PAL/PSB/DispatchCommand 直接写关键字 | 四函数体内无 FUN_14006159c、无 +0x88/8A/90 写 | 同（rb 全树 `\+ 0x88\) = ` 扫描仅 UI/窗口/EXEC-块换出块命中） | **成立**：健康只能靠继承 ⇒ 根因必在祖先链 |
| 6 | CALL 行是否走 InvokeSubRoutine（种子化模板支 dc:29285） | dc:29262-29295 | core_scriptdep.c:227-260（含 SG 探针 invoke-enter/pos/ret） | **排除为主因**：活测 invoke-enter 从未出现 |
| 7 | dc 裸 LOAD 的关键字保证 | dc:30061-30066 裸词 → **FUN_140031068**；其体 dc:29396-29410：随机 key + 克隆 + **FUN_14006159c(clone,key)** 后才装载/执行 | 我方同位置 = srx_ExecuteScriptFile（core_scriptrun.c:110-234），无克隆、无种子化，key=0 明文直跑 RunScriptText(script=原对象) | **★ 成立：我方在该入口丢失了 dc 的等价关键字写入** |
| 8 | 我方 LOAD 链剩余覆盖点 | dc:110330-110331（DispatchExpressionBlock 入口，守卫 bit0==0）；dc:40693-40694（FUN_140045c90 首跑块，仅 '\*' 形式 dc:45634 可达，纯 LOAD 走 dc:45653 RunCommand 不可达） | rb:7869（DEB 入口，同构）✓；首跑块在 rb:11768 ScriptMainEntry/core_execscript.c:271 两份恢复体内，LOAD 路径同样不可达；g_Script 在整条 LOAD 链上无任何种子化点 | **成立**：覆盖依赖单一入口且活体证明未达 |
| 9 | EXEC-块换出块（次嫌登记） | dc FUN_140031068 内 bVar13==9 支（save/restore 语义 Ghidra 未完整显示） | core_scriptdep.c:332-410：save90/swap90 **零初始化**（TODO(verify) 注释自认），bVar13==9 时会把宿主 +0x88/+0x90 区换出为零 | **登记不在本案路径**（EXEC 块专用，021 不经此），但属高危失真点需另开工单 |
| 10 | 忠实性小地雷 | dc:103227 `*(ushort *)(param_1 + 9)`＝**qword 索引 9＝字节 0x48** | rb:24634 `(uint16_t *)((uint8_t *)param_1 + 9)`＝**字节 9** | 仅当 +0x48==0（明文）时两者等价；key≠0 流将分歧。**随补丁顺带归正** |

---

## 3. 出生地说理（问题 1 正面回答）

- ExecuteCommand 家族内 PAL 的两个喂入对象：
  - 克隆支 rb:24650 `operator_new(0xf0)` → ScriptCopy(parent)+ScriptInit → rb:24669 PAL(clone)；
    dc 对应 dc:103242-103261 **逐字段同构，dc 同样不在此调 FUN_14006159c**。
  - 就地支 rb:24707 PAL(param_1)，param_1 为 PSB 链上传入对象，其远祖亦经
    RunScriptText 的 calloc 克隆（core_execmain.c:220-229，同样只拷不种）。
- 因此"精确缺失点"不在 ExecuteCommand 内部，而在**祖先对象的种子化责任链**上：
  dc 以「每个执行入口各自种自己的工作副本」保证不变量——
  裸文件执行入口 FUN_140031068（dc:29410）、编码流资源入口（dc:30186）、
  子程序模板入口（dc:29285）、表达式块入口（dc:110331）、主装载器首跑（dc:40694）。
  我方唯一结构性替换发生在第 1 个入口（§2#7）：srx 以"key=0 明文"语义替代了
  "随机 key + 种子化克隆"，却**没有把关键字写入留在任何等价位置**。
- 活体（关键字=栈址垃圾而非零）进一步把状态钉为"自原始分配后 +0x88/8A/90 无任何
  写入者"，即该对象的祖先链整体绕过了 rb:7869 的 DEB 入口初始化——
  与 §2#7/#8 的覆盖缺口一致（直接以 ProcessScriptBlock(&g_Script,…) 类入口进入行的
  处理路径在树中确实存在：core_b2e.c:1696、core_b3i.c:115、core_b2d.c:1361 等）。

## 4. 问题 2 / 问题 3 回答

- **问题 2（ScriptCopy 覆盖长度）**：≥0x92 成立（实测覆盖 0x00–0xEB 字段面），
  "复制长度不足"假说死亡（§2#1/#2/#3）。
- **问题 3（:24707 就地支上游裸对象交叉）**：全树 `operator_new(0xf0)` /
  `calloc/malloc(0xf0)` 共 11 处逐一核对，**不存在跳过三连的脚本对象出生点**；
  但就地支的上游对象可以来自不经 RunScriptText 的直接 PSB 入口（见 §3 末），
  这类对象在 LOAD 链上没有任何种子化覆盖——是"首次进入即裸对象"的真实形态
  （其字段值来自堆再用而非全零）。

---

## 5. 最小忠实修复补丁（**不应用**，文本交付）

原则：恢复 dc 在同一入口的等价写入（dc:29410 的位置语义 + dc:110331 的实参形态），
不给扫描器加兜底，不改克隆所有权/变量作用域。

```diff
*** a/refactored/src/lang/core_scriptrun.c
--- b/refactored/src/lang/core_scriptrun.c
@@ srx_ExecuteScriptFile（≈:110，函数入口、读文件之前）
 extern void FUN_140017CDC(void *dst, void *src);   /* 已有 :52 */
 extern void FUN_1400186BC(void *s, int64_t parent);/* 已有 */
 extern void PECMD_InitObfuscatedKeywords(void *script, uint64_t seed); /* 已有 :54 */
@@
 static int64_t srx_ExecuteScriptFile(void *script, LPCWSTR cmd, LPCWSTR a3, uint32_t flags,
                                      LPCWSTR outarg, WCHAR **outbuf)
 {
+    /* R20-C: 恢复 dc 裸文件执行入口的关键字不变量。
+     * dc 同位置 = FUN_140031068 入口段 dc:29396-29410（随机 key 种子化工作副本）；
+     * 我方为 key=0 明文架构，按 dc:110331 形态以 *(u16*)(script+0x48)|0x10000
+     * 原地重建（含高字置位派生 []/_SUB/_END 配对符全表），使后续一切克隆
+     * （RunScriptText:220 calloc 克隆 / ExecuteCommand:24650 new 克隆）继承健康表。 */
+    PECMD_InitObfuscatedKeywords(script,
+        (uint64_t)(*(uint16_t *)((char *)script + 0x48) | 0x10000));
+
     ...原有读文件/RunScriptText 流程不动...
 }
```

配套归正（同补丁批次，防 key≠0 流分歧）：

```diff
*** a/refactored/src/commands/core_b3_remaining.c
--- b/refactored/src/commands/core_b3_remaining.c
@@ PECMD_ExecuteCommand（rb:24634，对照 dc:103227）
-        if ((*(uint16_t *)((uint8_t *)param_1 + 9) ^ 0x2a) == *pWVar12) {
+        if ((*(uint16_t *)((int64_t *)param_1 + 9) ^ 0x2a) == *pWVar12) {  /* dc:103227 字节 0x48 */
```

验证探针建议（T5 前临时，随本补丁同批落地/拆除）：
`dispatch-exec` 处追加打印 `*(u16*)(script+0x88)/+0x8a/+0x90`，
预期修复后 021 序列出现 {0000,000D,000A} 且 dispatch-ret 回归。

### 5.1 明确不做

- ❌ FUN_14001b23c / ParseScriptSegments 段扫描加 NUL 兜底（dc 同位置无此逻辑）。
- ❌ 给 ExecuteCommand 克隆支补 FUN_14006159c（dc:103242-103261 同位置没有——
  会造成与原版不同的双重初始化语义）。
- ❌ 触碰 core_scriptdep.c:384-410 换出块（另立工单，须先逆向 dc 原义）。

---

## 6. 回归风险

| 处置 | 影响面 | 风险评估 |
|---|---|---|
| srx 入口种子化 | 所有 LOAD 文件执行案 | 低：对已健康对象为幂等重写（同 +0x48 派生）；对零表对象恰好完成 dc 保证的初始化。变量/作用域零改动（不动对象身份）。 |
| :24634 字节 9→0x48 归正 | 明文流下行为恒等（0^0x2a 同值） | 近零；仅编码流场景改变且为向 dc 对齐 |
| 探针增量 | 日志体积 | 低（V4 规范：GetLastError 保存恢复已有先例 SG_Probe） |

预期：021/037/038 及 002/004 中属同根因者退出自旋转正常返回；若 002/004 死点
独立（r19a §4.4 已示二者死点漂移），按各自工单继续。

---
*生成：R20-C 根因收口子代理 · 证据行号基于当前工作区（dc 181421 行 / 干净工作树 =
部署构建 2026-08-26 19:27:28）· 遵守只读纪律，未构建/未改源/未调试。*
