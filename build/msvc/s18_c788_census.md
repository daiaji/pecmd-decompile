# S18 — FUN_14005C788 全调用点前缀匹配语义普查

- 日期: 2026-08 (S18 批次, 只读普查 + 本报告)
- 普查对象: `FUN_14005C788` / `PECMD_AsciiPrefixICmp` / 小写 `FUN_14005c788` 的全部调用表达式
- 触发背景: R14 将 `core_string.c:187` 的 FUN_14005C788 由"混装语义"(尾部要求 w[n] 为空白/NUL)恢复为 dc:54955-54976 的纯 n 字符前缀比较; 历史上在混装语义下调通的调用点可能出现过度匹配回归。
- 方法: **dc 镜像交叉验证法** —— 原二进制中 c788(纯前缀) 与 c72c(词边界) 是两个不同函数, decompiled.c 每个调用点已经"硬编码"了它真正需要的语义。将权威树(refactored/**)每个调用点映射回 dc 对应位点: 原 c788 → 纯语义正确(SAFE); 原 c72c 但重构写成纯前缀 → NEEDS-C72c。

---

## 0. 结论摘要

| 清单 | 计数 | 说明 |
|---|---|---|
| 权威树调用表达式总计 | **243** | `PECMD_AsciiPrefixICmp`×115 + 大写 `FUN_14005C788`×111 + 小写 `FUN_14005c788`(restored_bodies.c)×17 |
| **NEEDS-C72c** | **4 处补丁** | core_thread.c:111(包装器内核), core_main.c:155("**u"), core_exec2.c:350("_ENDFILE"), core_exec2.c:355("_ENDFILE-IMPORT") |
| SAFE(语义=原文) | 239 表达式 | 其 dc 对应位点本就调用纯前缀 c788, R14 后与原二进制行为一致 |
| ├ 其中 LINKAGE-RISK | 132 表达式 | 经 `PECMD_AsciiPrefixICmp`(115) 与小写 `FUN_14005c788`(17) 调用, 当前唯一"定义"是 unimplemented_stubs.c 的返 0 桩 → 语义正确但可能根本未被真实实现绑定(见 §4.1) |
| AMBIGUOUS(语义不可静态判定) | **0** | 所有位点均完成 dc 映射; 仅 1 条 SKIP 登记项(构建脚本缺失, 无法确认桩文件是否参与链接, 见 §5) |

补丁影响面: Patch#1(core_thread.c 包装器内核)一处修复即恢复其全部下游动词分发位点(~200+ 处 FUN_1400660AC 调用, 如 srparse.c:176/181 的 EnviMode/ncd 等)的词边界语义。HANDOVER_PROGRESS.md:43 预判的"混语义全局候选根因"即在此得到定位与收敛。

---

## 1. R14 实现一致性复核

`refactored/src/kernel/core_string.c:187-203` vs `decompiled.c:54955-54976`:
- 循环条件 `(c>0x40 || u==(WCHAR)(short)c) && (((short)c|0x20)==(u|0x20))` ≡ dc:54972-54974 ✓
- n 耗尽无条件返 1 ≡ dc:54963-54965 ✓
- 判定: **R14 恢复体与原文一致**, 纯前缀语义。✅

## 2. 任务④ TokPrefixICmp 体核对

`restored_bodies.c:7307-7319 PECMD_TokPrefixICmp` vs `decompiled.c:54922-54947 FUN_14005c72c`:
- 减计数循环、循环条件逐字符相同;
- 边界集合相同: `w[n] ∈ {0x09..0x0D} ∪ {0x20} ∪ {0x00}` → 返 1, 否则返 0 (rb:7313 ≡ dc:54933-54936);
- 失配返回值 `(ulonglong)(uVar3>>8)<<8` 相同 (rb:7319 ≡ dc:54947);
- 差异仅为 lVar4 初始化位置(循环前置 0 vs 短路路径内赋 0), 无语义差。
- 判定: **一致** ✅。(附带: rb:7320 行尾注释 "@0x14001c2cc EnablePrivilege 桩" 是误贴的装饰性注释, 建议主代理顺手清理。)

## 3. NEEDS-C72c — 精确替换补丁块(供主代理应用)

> 三份目标文件均包含 `pecmd_defs.h`, 而 `xproto.h` 未声明 PECMD_TokPrefixICmp(仅 stubs_common.h:1424 有),
> 故每个补丁附带一条本地 extern(int64_t 在 x64 与 longlong 兼容, 同签名重复 extern 合法)。

### Patch #1 — refactored/src/runtime/core_thread.c (包装器 FUN_1400660AC 内核, 最高优先级)
证据: dc:62800-62818 原体第 62811 行调用的是 **FUN_14005c72c**(词边界); 重构体现用纯前缀。
R14 前靠混装语义侥幸等效; R14 后丢失边界 → 下游全部动词分发位点可被 "sh"→"shel…"、"hide"→"hideX" 类输入穿透。

```diff
--- refactored/src/runtime/core_thread.c
@@ (line 23, 之后追加一行)
 extern WCHAR **FUN_14005B154(WCHAR **pp);               /* @0x14005b154 */
+extern int64_t PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n); /* @0x14005c72c */
@@ (line 111)
-    r = FUN_14005C788(word, p, n); /* PECMD_TokPrefixICmp 等价实现 */
+    r = (int)PECMD_TokPrefixICmp(word, p, n); /* dc:62811 原体经 FUN_14005c72c(词边界); 纯前缀是 R14 前混装语义残留 */
```

### Patch #2 — refactored/src/app/core_main.c ("**u", 镜像 dc:5537)
姊妹镜像 b1_remaining.c:4369(dc:5639)已正确使用 TokPrefixICmp, 仅此入口漏改。

```diff
--- refactored/src/app/core_main.c
@@ (line 35, 之后追加一行)
 extern WCHAR **FUN_14005B154(WCHAR **pp); /* @0x14005b154 */
+extern int64_t PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n); /* @0x14005c72c */
@@ (line 155)
-            if (!hasParam || FUN_14005C788("**u", pMain, 3) == 0) {
+            if (!hasParam || PECMD_TokPrefixICmp("**u", pMain, 3) == 0) { /* dc:5537 原体 c72c */
```

### Patch #3 + #4 — refactored/src/lang/core_exec2.c (_ENDFILE / _ENDFILE-IMPORT, dc:22012 / dc:22019)
关键回归场景: 行 "_ENDFILE-IMPORT xxx" 在纯语义下会被 :350 的 "_ENDFILE" 抢先命中而 break,
:355 的 IMPORT 掩码分支永远不可达(原二进制靠 '-'≠空白 让 c72c 在 :350 返 0)。两处必须成对替换。

```diff
--- refactored/src/lang/core_exec2.c
@@ (line 30, 之后追加一行)
 #include "pecmd_defs.h"
+extern int64_t PECMD_TokPrefixICmp(const char *a, const WCHAR *w, int n); /* @0x14005c72c */
@@ (line 350)
-        if (FUN_14005C788("_ENDFILE", tmp, 8) != 0)
+        if (PECMD_TokPrefixICmp("_ENDFILE", tmp, 8) != 0) /* dc:22012 原体 c72c */
@@ (line 355)
-        if (FUN_14005C788("_ENDFILE-IMPORT", start, 0xf) != 0) {
+        if (PECMD_TokPrefixICmp("_ENDFILE-IMPORT", start, 0xf) != 0) { /* dc:22019 原体 c72c */
```

越界说明: 两处读取 w[n] 均止于首个 NUL/失配, tmp[n]=0 已保证(rb 上文 :344), 无过读风险。

---

## 4. SAFE 清单(239 表达式, 按 dc 对应位点逐一映射)

判定基准: 模式尾部为结构性分隔符(`: = * < > ( ) { , . \\ / [`)时后随字符即载荷, 天然无边界问题; 尾部为字母/数字者, 以 dc 对应位点调用 c788 为准(原二进制即为纯语义, R14 后行为一致)。

### 4.1 LINKAGE-RISK 子组(132 处, 语义 SAFE 但绑定到返 0 桩名, 见 §6.1)
- `PECMD_AsciiPrefixICmp` ×115:
  - ui/core_b7c.c ×21: 689("0"),714("t:"),6268("-sps"),6273("-font:"),6276("-hfont:"),6278("-color:"),6286("-sl:"),6295("-sc:"),6303("-cksz:"),6643("#4"),6646("#1"),6650("#2"),6652("#22"),6654("#3"),6743/9913("-from:"),6764("-SKIP="),6775/9915("-skiptp:"),6818/9917("-skippt:") [dc:119792,119794,125512-125543,126709-126719,126806-126875,131964-131968]
  - restored_bodies.c ×34: 595("-shel"),841/858/860/862(cmd:/wd:/exe:/logs:),978(clone:),1013-1039(shel:/runrmenu:/poprmenu/pid:/hpid:/ppid:/exe:/wd:/logs:),1198-1202(uac/su/doc),1228(code:),1232(user:),1264/1266(passwd:/passwd*),3265(winpeshl),3271(wpeinit),8381-8395(-bs=/-ibs=/-obs=/-count=/-len=/-conv=/-skip=/-skipb=),8445(notrunc),12053(LOAD) [dc:8099,8345-8543,8703-8771,10749,10755,130000-130064,40827]
  - misc/core_b1_remaining.c ×1: 2826("\\?\volume{") [dc:3618]
  - device/core_b3r_h2.c ×6: 262("-del"),579(tmpl:),914("-display:"),1336(idlec:),1338(idle:),2119("-ssid:") [dc:97821,97565,95529,94178,94180,96723]
  - device/core_b3r_h1.c ×6: 249("-vcenter:"动态长),822("["),1402("-u:"),1407("-src"),1952("\\\\.\\\\"),1955("\Device\") [dc:83413/83675,85274,85820,85825,93762,93765]
  - device/core_b3r_g7.c ×1: 359("\\?\Volume") [dc:21595]
  - device/core_b3r_g.c ×2: 452(动态键),693("-vcenter:"动态长) [dc:81243,83413]
  - commands/core_b3_remaining.c ×12: 1276(p:),2163(listdev:),2263(*rescan:),2540-2565(*disable:*enable:*remove:*update:*status:*install:*restart:×7),19821("-sub") [dc:40409,41583,41680,41941-41953,28792/81535]
  - commands/core_b3r_i28f.c ×3: 253("-font:"),255("-color:"),432("-num:") [dc:117503-117508,117968]
  - commands/core_b3r_i28e.c ×3: 913("-b:"),917("-font:"),920("-color:") [dc:117503-117508 族]
  - commands/core_b3r_i28d.c ×1: 229("-vcenter:"动态长) [dc:83413]
  - commands/core_b3r_i28c.c ×4: 1029(tid:),1031(htid:),1134(st:),1396("-t:") [dc:109999-110001,110099,112073]
  - commands/core_b3r_i28b.c ×6: 202(=:),513(NL:),520(delims:),567(/s:),585(/size*:),591(/size:) [dc:107111,107930,107955,108007,108028,108030]
  - commands/core_b3r_i28a.c ×4: 113(Title),114(Tip),301/528("-scale:") [dc:105891,105893,106695,106919]
  - commands/core_b3r_h4.c ×8: 653(--wd:),1035-1056(utc:/gmt:/local0:/local:/sys0:/sys:),1524(.com*) [dc:104070,102153-102172,102948]
  - commands/core_b3r_h3.c ×1: 537("-delims:") [dc:100721/101532]
  - commands/core_b3r_b.c ×1: 177("-scale:",dc:72924)
  - ui/core_b8_remaining.c ×1: 784("--w:") [dc:143962]
- 小写 `FUN_14005c788` ×17 (均在 restored_bodies.c): 4968/5076(thread),5079("2"),5089(lock:),5115(ln=),5125(p),5130(np),5135(t=),5145(dbg=),5155(RAW=),5165(rt=),5176(ON=),6250(THREAD),6388(READ),13667(COMMAND),13669(NOTIFY),13964(KERNEL32) [dc:23166-23374,44234,44363,148392,148394,13095]

### 4.2 绑定真实现(FUN_14005C788→core_string.c)且 dc 对应为 c788 的位点(107 处)
- app/core_main.c ×2: 136(EXEC)+137 手工尾检, 150(MAIN)+151 手工尾检 ≡ dc:5512-5515 / 5527-5530 ✅
- misc/core_b1_remaining.c ×21: 4344(EXEC)+4346 手工尾检, 4358(MAIN)+4360 手工尾检 [dc:5615-5632], 5184(FORCELOCAL),5201(EnviMode),5205(EXPORTLOCAL),5227(__arg=),5235(logs_ln=),5243(logs_np=),5251(HelpColor=),5271(Arg=),5277(Clipboard=),5282(Clipboard?=),5292(QueryCmd=),5299(LoadPlugin=),5312(WndProc),5353(TextDelay=),5354(EnviBroad=),5355(memvar=),5365(zero=),5376(DisX64=),7674(Explorer) [dc:7272-7484,15393]
- ui/core_b7c.c ×50: 2236-2607 SED 开关链(-std/-env/-raw/-def/-tom/-tow/-copy/-cmp/-addr/-mkfixdummy/-mkdummy/-make/-swap/-zero/-ret/-get/-ex/-x/-[/*)与类型链(char/wchar/short/ptr/intptr/intpe/float/double/ldouble/int64/longlong/long/intsys/int ×2 组), 9277(frm<) [dc:122775-123061,68644-68669,131379] — 短旗标全链原二进制即纯匹配, 顺序消歧(-alignend 先于 -align 同理)
- commands/core_b3_remaining.c ×41: 961(-hlpdoc=), 12604-12838 数学函数链(lg/ln/deg/int/rad/not/div/max/min/mod/shl/shr/xor/cos/sin/abs/ctg/exp/tan/log/pow/frac/rand/lnot/ceil/sqrt/floor/round/pow10/hypot/arccos/arcctg/arcsin/arctan ×34), 17765/17767/17773(\Device\/\DosDevices\/\?\Volume{), 26702/26716/26749(<bmp/<ico/<img) [dc:37750,62061-62250,74897-74904,111723-113827]
- kernel/core_strbld.c ×10: 175-210 类型名链(char/wchar|short/long/int64/ptr/float/double/ldouble/int) [dc:68644-68669]
- lang/core_srparse.c ×1: 185(logs:, ends ':') [dc:29738]
- lang/core_scriptrun.c ×2: 334/410(*map:) [dc:29658,29831]
- lang/core_script2.c ×1: 386(MAIN) [dc:45622]
- lang/core_script.c ×1: 91(import) [dc:28944]
- lang/core_execscript.c ×1: 412(LOAD) [dc:40827]
- lang/core_exec.c ×2: 618(#code=),629(#str:) [dc:11726,11738]
- lang/core_exec2.c ×1: 361(FIND $1 = %&&__MAIN__%,, ends ',') [dc:22022]
- commands/core_b2f.c ×4: 2375(p:),7026(svr2),7437(scan:),7451(cur:) [dc:40409,35673,36927,36937]
- commands/core_b2c.c ×1: 915(-sub) [dc:28792]
- commands/core_b3f.c ×1: 321 PECMD_MatchAssignToken 内核 ≡ dc:62864-62883(FUN_140066188=c788+'='|':') ✅ 结构逐行同
- commands/core_b3d.c ×1: 412 PECMD_MatchAndAdvance ≡ dc:62890-62906(FUN_1400661e4=c788+推进) ✅
- ui/core_b8m.c ×6: 3142(COMMAND),3143(NOTIFY),3626-3629(HKCU\/HKEY_CURRENT_USER\/HKLM\/HKEY_LOCAL_MACHINE\) [dc:148392,148394,138948-138954]
- ui/core_b8g.c ×1: 980(<buddy>) [dc:67490]

### 4.3 已正确使用 PECMD_TokPrefixICmp 的原 c72c 位点(无需处理, 抽样核验)
b1_remaining:4369(**u), restored_bodies:6667(*svr)/6739(**u)/14312(--gui-), b2e:223(delall), h4:228(-case)/1061-1067(bsys/space0/space/us), h3:1003-1924(-UNI 族×2 组), d:499(*raw), b3_remaining:1496(-8), g/h1/i28a-i28f 各 -center/-right/-left/-rich/-nroc/-3D/-u2d/-scale/-vcenter/-trans/-w/-wx/-nf/-ncmd/-mod/-pcenter/-round/-nscope(f)/-nfocus/-def/-na/-b/-bupdate/-real/-smooth/-tab/-numicon(g)/-numbmp 链, h1:1417("-"), b7c:6306(-left) — 与 dc:21290,22012 外的全部 c72c 位点一一对应 ✅

---

## 5. AMBIGUOUS / SKIP 登记

| 项 | 类别 | 理由 |
|---|---|---|
| unimplemented_stubs.c 是否参与链接 | SKIP(无法验证) | build/msvc 下仅有 build_s11*.log 与语料, 未找到 .vcxproj/Makefile/build.bat; 无法静态确认 §4.1 的 132 个桩名调用点当前是否被返 0 桩满足。若参与链接, 这是比 R14 更大的现役缺陷(所有这些动词恒不匹配); 若不参与, 则存在未解析外部符号或另有真实定义未被本普查发现。登记待主代理以构建脚本核实。 |
| 语义层 AMBIGUOUS 位点 | — | **0 个**: 全部 243 个表达式均完成 dc 镜像映射, 无需猜测的动词上下文。 |

## 6. 附带发现(超出本普查修改权限, 仅上报)

1. **双名桩风险(§4.1)**: `PECMD_AsciiPrefixICmp` 全仓唯一定义为 unimplemented_stubs.c:115 `{...return 0;}`(xproto.h:115 亦标注 def 于该处); 小写 `FUN_14005c788` 定义于 unimplemented_stubs.c:529 `{...return 0;}`。与大写 `FUN_14005C788`(core_string.c:187 真体)构成三个符号。建议: 给两个桩名补真定义(转发 core_string.c 实现)或将调用点改名归一。
2. **@0x1400660ac 双实现并存**: core_thread.c:`FUN_1400660AC`(纯前缀, 错) 与 restored_bodies.c:7507 `PECMD_MatchTokenAdvance`(TokPrefixICmp, 对)。Patch#1 修复前者后两者语义一致; 中期建议合并为单一实现。
3. xproto.h 缺 `PECMD_TokPrefixICmp` 声明(stubs_common.h:1424 独有), 是本次三处补丁需带本地 extern 的原因; 建议主代理将该声明上收至 xproto.h 后可移除本地 extern。
4. HANDOVER_PROGRESS.md:43 所记 "FUN_14005C788 混语义(130 调用点)" 与实测口径差异: 权威树实际调用表达式 243 个(含镜像函数的重复实现), 原始唯一位点见 decompiled.c(c788≈494 行命中 / c72c≈113 行命中, split\ 与 big_funcs\ 为同一函数集的镜像拷贝)。
