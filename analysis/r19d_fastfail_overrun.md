# R19-D fastfail/内存越界取证报告（TEAM/HASH/TEMP 族 + 039 观察）

- 取证子代理轮次: R19 D 簇专项
- 工作区: `D:\repo\PECMD反编译\refactored`；权威语义源: `D:\repo\PECMD反编译\decompiled.c`（下称 dc）
- 构建: `build\msvc\pecmd_msvc.{exe,map}`（map 时间戳 2026-08-26 17:56:25）
- 手段纪律: 仅 `open_dump` 只读分析（backtrace/.ecxr/.exr/disassemble/read_memory），未用任何调试控制操作；dump 会话均已 `end_session`（含复检时发现被引擎重启的 6724.dmp 会话，已二次关闭）。未构建、未测试、未改任何源码。
- **构建身份绑定（V-Gate）**: 本报告全部 msvc 崩点 RVA/函数归属的版本锚 = `build\msvc\pecmd_msvc.map` **时间戳 2026-08-26 17:56:25**（`DEPLOYED_BUILD.txt` 不存在于仓库树，故以 map 时间戳为唯一可用身份锚）；核验方式 = 各 dump 模块基址 `0x00007ff79bbd0000` 与该 map 第三列 VA−0x140000000=RVA 逐点换算一致（§1），且 dump 调试时间 18:03:19–43 晚于 map 构建时间、属同一构建轮次。**若主代理此后重建（map 时间戳变化），本报告所有 msvc RVA 结论一律降级为"待版本核验"并须按新 map 重算。** 文中 dc/rb 的 `@0x140xxxxx` 为原始二进制（PECMD原始.EXE, ImageBase=0x140000000）函数标识符，仅作源级索引，不属于本节"崩点归属"范畴，不受重建影响。

---

## 0. 结论速览

1. **D 簇 4 案（024/025/053/056）本轮没有对应 WER dump**——`C:\pectest\dumps\` 下仅 5 份 dump，经命令行（PEB→ProcessParameters→CommandLine）逐一定位，分属 **055_sed_generate / 056_temp_exist / 059_link_shortcut / 060_path_special / 未知(GetDiskFreeSpace NULL 解引用)**。
2. **056_temp_exist 的 dump 与 R19 分类不符**：实际异常是 **c0000374（堆损坏）**，崩点在 `PECMD_TempPathCommand+0xd5b`（RVA 0x10532B）的字符串槽 `HeapFree`，不是栈 cookie fastfail。堆块在更早阶段已被破坏或被二次释放。
3. 唯一一份 GS 失败样本（**055_sed_generate**，非 D 簇）完整展示了本构建 0xC0000409 的产生机制：
   `异常上下文 RIP = pecmd_msvc!FUN_140003a20+0x31`（读 `script->0xDA` 时 AV，script 指针已是垃圾值）→ 展开 SEH 时该帧的 `__GSHandlerCheck` 校验 cookie 失败 → `__report_gsfailure` → `int 29h`，`.exr -1` 参数 `FAST_FAIL_STACK_COOKIE_CHECK_FAILURE(2)`。
   即：**本构建的 c0000409 = "先有指针/内存踩踏，后由 GS 兜底上报"**，cookie 失败是果不是因。
4. 静态逐体核对结论：TEAM/HASH/TEMP 三族直移体本身**未发现"Ghidra 抄小数组/丢钳位"类收缩偏差**（详见 §2 各表）；真正可编译出"野写入"的实锤缺陷有两处：
   - **S-TEMP-1（实锤，潜伏雷）**：`PECMD_CreateTempMutexDir` 把 dc 中 Ghidra 丢失变参的 `wsprintfW` 伪影**原样编译**（0~1 个实参喂 4 个占位符，且一处格式串尾带孤立 `%`）→ `%s` 拉寄存器/栈上垃圾当指针，输出长度失控可写爆 0x26c 字符槽。
   - **S-TEAM-1（UB 级）**：`PECMD_ProcessScriptBlock` 把 dc 的入口寄存器残留 `unaff_R13D` 声明成**未初始化局部变量**直接读取（每次命令执行都经过）。
   - **S-HASH-1（行为偏差，非越界）**：`FUN_14005c7c4`（HASH 算法名匹配）与真体 `FUN_14005C7C4`（core_exec5.c:156，同址 0x14005c7c4）双名分裂，链接到恒 0 桩 → SHA*/CRC32/list 全部失效、永远走 MD5 缺省。

---

## 1. Dump 取证（只读）

**版本绑定声明**：本节全部崩点 RVA/符号偏移均绑定上文构建身份锚（map 2026-08-26 17:56:25 + 模块基址 0x7ff79bbd0000 换算核验）；换 map 即待版本核验。

模块基址 5 份 dump 一致：`pecmd_msvc` base = `0x00007ff79bbd0000`。
RVA 换算与 map 第三列核验：`VA − 0x140000000 = RVA`，例证：
- `PECMD_TempPathCommand` map 行 `0001:001035d0 … 00000001401045d0` → RVA 0x1045D0；dump 帧 `+0xd5b` ⇒ 绝对 `base+0x10532B`，与 RetAddr `0x7ff79bcd532b` 反算一致 ✅
- `FUN_140003a20` map 行 `0001:001e4650 … 00000001401e5650` → RVA 0x1E5650；异常 RIP `base+0x1E5681 = +0x31` ✅（反汇编确认 0x1E5650 正是该函数序言起点）

### 1.1 pecmd_msvc.exe.18616.dmp — 055_sed_generate（F 簇样本，GS 机制解剖）

```
.lastevent : c0000409 Security check failure or stack buffer overrun
.exr -1    : ExceptionAddress = pecmd_msvc!__report_gsfailure+0x1d (int 29h)
             Parameter[0]=2 → FAST_FAIL_STACK_COOKIE_CHECK_FAILURE
k:
  00 __report_gsfailure+0x1d        [pecmd_msvc+0x1e973d]
  01 __GSHandlerCheck+0x13          [pecmd_msvc+0x1e960f]
  02..04 ntdll 异常分发链 (_chkstk/RtlDispatch*/KiUserExceptionDispatcher)
  05 pecmd_msvc!FUN_140003a20+0x31  [pecmd_msvc+0x1e5681]   ← 第一现场上下文
  06 pecmd_msvc!PECMD_RunCommandLine+0x28  [unimplemented_stubs.obj]
  07 pecmd_msvc!PECMD_GenerateTextContent+0x9d6 [core_b3r_h1.obj, RVA 0xFB2A0]
第一现场指令 (+0x31):
  mov rax,[rsp+60h]          ; param_1 (script)
  movsx eax,byte ptr [rax+0DAh]   ← AV：script 指针已为垃圾值
机制还原: 第一异常在本帧内 AV → 分发时 __GSHandlerCheck 先验 cookie → 已被踩 →
主动 __report_gsfailure(int29)。即 cookie 破坏发生在进入 FUN_140003a20 之前/期间，
FUN_140003a20 本体（unimplemented_stubs.c:628-651 直移体）与 dc:1239-1261 逐句一致，非根因。
```

### 1.2 pecmd_msvc.exe.6028.dmp — **056_temp_exist（D 簇）**

```
.exr -1 : c0000374 (堆损坏) @ ntdll 提升点
k（关键段）:
  ntdll!RtlFreeHeap+0x2ba            ← RtlFreeHeap 检测到坏块并 raise
  pecmd_msvc!PECMD_FreeStrBuf+0x31   [core_string.c 真体, HeapFree(ptr-8)]
  pecmd_msvc!PECMD_TempPathCommand+0xd5b   ← RVA 0x10532B（core_b3r_h2.obj）
  pecmd_msvc!PECMD_ProcessScriptBlock+0x2f8f
  ... → srx_ExecuteScriptFile → RunCommand（LOAD 重入）→ main
命令行: C:\pectest\pecmd_msvc.exe LOAD C:\pectest\056_temp_exist\run_all.pecmd
```
解读：TEMP 处理器尾部的字符串槽释放动作只是"受害者"，坏块在更早的执行流中形成
（上游堆越界写或同块双放）。§2.3 给出的 TEMP 族嫌疑点按此排序。

### 1.3 其余三份（非 D 簇，佐证"垃圾指针"共性）

| dump | 用例 | 异常 | 现场 |
|---|---|---|---|
| 18576 | 059_link_shortcut | c0000005 读 0x4b26d338 | `PECMD_LinkCreateShortcut+0x656`（RVA 0x2ADA0+0x656）|
| 10296 | 060_path_special | c0000005 读 0x33122782 | `PECMD_CreateDirectoryTree+0x18` ← `PECMD_CreateDirectory+0x29` ← `PECMD_DispatchSpecialDirective+0x95` |
| 6724 | （未知，FOR/SIZE 族形态）**用例归属待版本核验/待复跑确认** | c0000005 读 0x0 | `PECMD_GetDiskFreeSpace+0x3a5`（RVA 绑定同上 map）|

0x4b26d338 / 0x33122782 这类"小数值垃圾指针"与 §1.1 的垃圾 script 指针同形，
提示存在共性的**未初始化数据被当指针使用**问题（与 S-TEAM-1 的 unaff_R13D 同类家族）。

---

## 2. 四动词族越界嫌疑清单（dc 行号 ↔ rb/src 行号 ↔ 缓冲声明 vs 写入跨度）

> rb = refactored 树；"✔ 忠实" 表示逐句核对未见尺寸/钳位偏差。

### 2.1 TEAM 族 —— 024_team_multi / 025_team_with_if（+039_team_chain 观察对象）

调用链：`restored_bodies.c`(PECMD_ProcessScriptBlock @0x14004c0bc) → dispatch `0x4D414554`
→ `PECMD_ParseValueCommand`(core_b2a.c) → `PECMD_ParseCommandBlock`(core_b2d.c)
→ 逐段 `PECMD_ProcessScriptBlock`（'|' 分段）→ ENVI/CALC/FIND/IFEX 各处理器。

| # | dc | rb/src | 核对项 | 结论 |
|---|----|--------|--------|------|
| T-a | dc:44660-44663（dispatch）| restored_bodies.c:6694-6698 | TEAM→ParseValueCommand(line+8,param_5) | ✔ 忠实 |
| T-b | dc:22656-22667 `longlong local_res10[3]` 仅 [0] 赋值 | core_b2a.c:87-98 `void *arr[3]` 仅 arr[0] | ParseCommandBlock 只解引用 pp[0]，arr[1..2] 未初始化但两侧同样不触 | ✔ 等价（非 OOB） |
| T-c | dc:22474-22652 ParseCommandBlock | core_b2d.c:451-630 | 全函数无固定数组（local_res10 为槽）；'{}'/'[]' 块分支逐句一致 | ✔ 忠实 |
| T-d | dc:43816-43894 ProcessScriptBlock 局部 | restored_bodies.c:5825-5897 | `WCHAR local_c8[68]` ↔ 同名同尺寸；唯一写点 `wsprintfW(local_c8,L"0x%p",…)`（dc:45144/restored_bodies.c:7191 附近）≤19 字符 < 68 | ✔ 忠实 |
| **T-1** | **dc:43921 `local_170 = … unaff_R13D & ~0xff`（Ghidra：入口 R13 寄存器残留）** | **restored_bodies.c:5862 `undefined4 unaff_R13D;`（未初始化局部）+ :5931** | **声明 vs 使用：读了从未赋值的自动变量（MSVC 下为栈垃圾）。污染 `local_170` 低字节，影响 `(char)local_170 != 0` 类分支（LOAD/FORX/FIND 前置展开门）。属 UB + 保真洞，每条命令执行都经过** | ⚠ 嫌疑（行为层） |

039（C 形态垃圾退出码 815713520）：R19 已判 HRESULT 形态泄入 g_exitCode；本次静态复核
TEMP 同型返回 `(LARGE_INTEGER*)0xffffffff80070057`（core_b3r_h2.c:729 ≙ dc:97703）证实
该族返回值通道确实携带 E_INVALIDARG 形态值，支持 C 簇结论；无新增内存不安全点。

**小结：TEAM 链本身没有发现更小的缓冲或丢失的钳位；其 fastfail 若发生，应按 §1.1 机制到
共享层（展开器/槽管理/上游踩踏）找因，而非 TEAM 处理器本体。**

### 2.2 HASH —— 053_hash_probe

调用链：dispatch `0x48534148`（restored_bodies.c:6564 ≙ dc:44532-44535，a3=NULL 一致）
→ `PECMD_HashCmdCompute`（core_b7c.c:156-382 ≙ dc FUN_1400c0ad8 @119159-119391）。

| # | dc | rb/src | 缓冲声明 vs 写入跨度 | 结论 |
|---|----|--------|------------------------|------|
| H-a | dc:119198 `CHAR local_e1[161]`（栈）| core_b7c.c:177-178 `static uint64_t hb_align[(24+176)/8]`，`hexbuf=hb+0x17`，`hash_out=hexbuf+1` | 写入方=PECMD_CryptoHashCompute 路径 B：`wsprintfA(p,"%02X",…)` 每 byte 写 2 字符+NUL，最大 SHA512=64B ⇒ 跨度 2*64+1=129B；可用容量 dc=160B / rb=177B | ✔ 均≥129，无收缩；rb 改 static 仅移除 GS 保护，不构成越界 |
| H-b | dc:119313-119327 >4GB 分块 extra=(out+3/1) | core_b7c.c:290-319 cf='\x03'/‘\x01’ | 句柄槽 out-0x18/out-0x10 语义一致（rb 显式清零 hb_align[0..1]）| ✔ 忠实 |
| H-c | dc:119365 CRC32→FUN_1400e4cc0(out) | core_b7c.c:347 | 输出 `%08X`+NUL=9B ≤ 容量 | ✔ |
| **H-1** | dc:119239-119253 算法名匹配 FUN_14005c7c4（真体）| **unimplemented_stubs.c:622 `uint64_t FUN_14005c7c4(...){return 0;}` 恒 0 桩**；真体在同址双名 `FUN_14005C7C4`（core_exec5.c:153-156，stubs_common.h:1324/core_cmd4.c:32 注明"1=相等"） | 小写/大写两个链接符号分裂：HASH 的 SHA1/SHA256/SHA384/SHA512/CRC32/list 匹配全部失配 → 永远 CALG_MD5 缺省 | ⚠ 行为缺陷（非越界；REVIEW.md §132 另记真体"标志倒置"争议，接线时需一并裁决） |
| H-2 | dc:119350 `$`串模式 wide→ANSI FUN_14006355c | core_b7c.c:336 | 槽分配制（FUN_1400633A8 len+9 头）| ✔ |

053 用例 `HASH H,%F%` 实际走"文件打开失败→结果=' '→SetVariable(文件路径,' ')"路径
（解析规则：单逗号=尾字段即变量名，dc:119220-119230），两侧一致，无越界面。
**若 053 呈 c0000409，按 §1.1 机制应查上游踩踏；HASH 本体静态无越界点。**

### 2.3 TEMP —— 056_temp_exist（dump 实测为堆损坏，见 §1.2）

调用链：dispatch `0x504D4554`（restored_bodies.c:6813 ≙ dc:44779-44782）
→ `PECMD_TempPathCommand`（core_b3r_h2.c:519-827 ≙ dc FUN_14009bbbc @97499-97790）。

| # | dc | rb/src | 缓冲声明 vs 写入跨度 | 结论 |
|---|----|--------|------------------------|------|
| P-a | dc:97540-97546 七个 `WCHAR xxx[264]` | core_b3r_h2.c:553-559 同七数组 [264] | 唯一写方 `PECMD_LoadLocalizedString(hInst,id,buf,0x104)`，请求 260 ≤ 264 | ✔ 尺寸一致（实现侧 LoadStringW 语义受 nBufferMax 约束） |
| P-b | dc:97685-97697 TEMP/TMP 注册表读 | core_b3r_h2.c:710-723 | 槽经 `PECMD_RegReadWrap`→`FUN_1400690C0`（core_b3n.c:133-172）：首请 0xffa/alloc 0x1000；MORE_DATA 后 size+10/alloc size+6；收尾写偏移 ≤ size+4 < alloc；末次 Grow(size+5) | ✔ 无越界（逐边界核算） |
| **P-1** | **dc:4409 `wsprintfW(dst,L"~pecmd_%s.%lu.%lu~%s%")` / dc:4436 `…(~%s",local_res20)`（Ghidra 变参丢弃伪影，原文如此）** | **core_b1_remaining.c:3628（0 个实参喂 4 占位符+格式串尾孤立 '%'）/ :3656（1 个实参喂 4 占位符）** | `PECMD_CreateTempMutexDir`（@0x140008110，TEMP *tmpdir/*tmpfile/tmpl 及 MKTMP 内部路径）：wsprintfW 按 stdcall varargs 从 r8/r9/[rsp+0x28…] 拉垃圾当 `%s` 指针（野读）且拼接长度不受控，目标为 `AllocString(plVar8,0x26c)` 槽 → 可写爆该堆块 | 🔴 **实锤缺陷（编译期把伪影固化）**。plain `TEMP T` 不经过此路径，但 *tmpdir/*tmpfile 一触即发；亦是本族最可能的"堆块被写坏"源头之一 |
| **P-2** | dc:97683(reg 槽=local_res10)+97740(browse 槽=local_ec0) 两独立槽；尾部 97782-97788 释放 res10 | core_b3r_h2.c:708(注册表槽误用 local_ec0)→:769-771 Setting/browse 分支又把 local_ec0 清零重用 → :813 释一次 → 尾部 :820 再释 | 双槽合一 ⇒ 第一次分配的注册表槽泄漏；第二次 free 因 `PECMD_FreeStrBuf` 置空槽（core_string.c:163-169）而降级为 no-op | ⚠ dc 偏差（泄漏+别名），非双放崩溃源；建议仍按 dc 恢复双槽 |
| P-3 | dc:97713 ExpandCommandLine(&local_eb0) 后 eb0↔LVar7 指针交换 | core_b3r_h2.c:740-743 | 交换后尾部释放集合与 dc 等价 | ✔ |

056 dump 的堆坏块并非上述任一点在 plain `TEMP T` 路径上的直接产物 → 指向 LOAD 前置流程
的共享层（与 §1.1/§1.3 的垃圾指针现象同源候选：S-T1 unaff_R13D 家族的未初始化数据、以及
树内已知"双名分裂"桩群中任何**带写出参的错误 arity 桩**）。

### 2.4 公共路径复核（四案共用，排除项）

| 对象 | dc | rb/src | 结论 |
|---|----|--------|------|
| `FUN_140003a20` 展开包装 | dc:1239-1261 | unimplemented_stubs.c:628-651 | ✔ 忠实（dump 中的 AV 是其入参被害） |
| `FUN_14007BDA8/FUN_14007A224/PECMD_ExpandEnvVars` | dc:78754/77684/78234 | src/lang/core_execline.c:68/119/627 | ✔ 缓冲等价：`a8[81]`↔`local_1a8[81]+local_106`、`f8[96]`↔`local_f8[96]`、envBuf 0x50 起 + (rlen+100) 重试、`:~` 截取钳位齐全（含 joined_r2 负索引钳制）；H1/H2/H3 修复注释与 dc 对应 |
| `PECMD_ParseValueCommand/ParseCommandBlock` | 见 §2.1 | — | ✔ |
| 槽基建 `AllocString/FreeStrBuf/GrowByteBuffer/HeapRealloc` | dc:53617/… | core_string.c:96-169、core_thread.c:30-65 | ✔ FreeStrBuf 置空槽；Grow 走 realloc；头布局死存储问题已有 T1c 结论注释 |

---

## 3. 最小修复补丁文本（仅文本，不应用）

### PATCH-1（S-TEMP-1，实锤）— `src/misc/core_b1_remaining.c`

```c
--- a/src/misc/core_b1_remaining.c   (PECMD_CreateTempMutexDir, @0x140008110)
@@ 目录模式随机名拼装（现 :3625-3628）
     GetTickCount();
     PECMD_NextRandomSeed();
     GetCurrentProcessId();
-    wsprintfW((LPWSTR)(lVar1 + (int64_t)iVar4 * 2), WSTR("~pecmd_%s.%lu.%lu~%s%"));
+    /* FIX(R19D): 原 line 编译了 dc:4409 的 Ghidra 变参丢弃伪影——0 个实参喂
+     * 4 个占位符且格式串尾带孤立 '%', %s 将拉取寄存器/栈垃圾作指针(野读写),
+     * 拼接长度亦不可控, 可写爆 AllocString(0x26c) 目标槽。
+     * 补齐实参; 字面量顺序 TODO(verify): 以原始 EXE 该 call 现场寄存器/栈为准 */
+    wsprintfW((LPWSTR)(lVar1 + (int64_t)iVar4 * 2), WSTR("~pecmd_%s.%lu.%lu~%s"),
+              WSTR("tmp"), (unsigned long)GetCurrentProcessId(),
+              (unsigned long)GetTickCount(), (LPCWSTR)local_res20);
@@ 文件模式循环（现 :3656）
-    wsprintfW((LPWSTR)(lVar1 + (int64_t)iVar4 * 2), WSTR("~pecmd_%s.%lu.%lu~%s"), local_res20);
+    wsprintfW((LPWSTR)(lVar1 + (int64_t)iVar4 * 2), WSTR("~pecmd_%s.%lu.%lu~%s"),
+              (LPCWSTR)local_res20, (unsigned long)GetCurrentProcessId(),
+              (unsigned long)GetTickCount(), WSTR(""));
```

### PATCH-2（S-TEAM-1，UB 消除）— `restored_bodies.c`

```diff
--- a/restored_bodies.c  (PECMD_ProcessScriptBlock 局部声明区, 现 :5862)
-  undefined4 unaff_R13D;
+  undefined4 unaff_R13D = 0;   /* FIX(R19D): dc:43921 语义为"入口 R13 寄存器残留"
+                                  (Ghidra unaff_)。C 直移不得读未初始化自动变量;
+                                  恒 0 与 execline 同族观测一致(core_execline.c envHit)。
+                                  TODO(verify): 如需精确保真, 反汇编各 caller 进入
+                                  0x14004c0bc 时的 r13 取值再定常量 */
```

### PATCH-3（S-HASH-1，双名分裂归一）— `unimplemented_stubs.c`

```diff
--- a/unimplemented_stubs.c  (现 :622)
-uint64_t FUN_14005c7c4(const char *a, const uint16_t *b) { (void)a;(void)b; return 0; }
+/* FIX(R19D): 小写 FUN_14005c7c4 与真体 FUN_14005C7C4(core_exec5.c:156) 同对应
+ * @0x14005c7c4。原恒 0 桩令 HASH 的 SHA1/SHA256/SHA384/SHA512/CRC32/list 选择
+ * 全部失配(恒落 MD5 缺省)。转发真体归一; 返回语义(相等=1)以 REVIEW.md §132
+ * "标志倒置"复核结论为准后再合入。 */
+uint64_t FUN_14005c7c4(const char *a, const uint16_t *b)
+    { return (uint64_t)FUN_14005C7C4(a, b); }
```

### PATCH-4（S-TEMP-2，恢复 dc 双槽结构）— `src/device/core_b3r_h2.c`

```diff
--- a/src/device/core_b3r_h2.c  (PECMD_TempPathCommand)
@@ 局部声明区（现 :550 附近）
     WCHAR *local_ec0;
+    WCHAR *local_regslot = NULL;   /* ≡ dc local_res10：TEMP/TMP 注册表读取专用槽 */
@@ 平铺路径半区（现 :706-711）
-    local_ec0 = (WCHAR *)0x0;
     ...
-    PECMD_AllocStrSlot(&local_ec0);
+    PECMD_AllocStrSlot(&local_regslot);      /* 注册表读入独立槽, 不再复用 ec0 */
@@ 三处 RegReadWrap / SetVariable / LVar7 取值（现 :710-726）
-    ..., (longlong *)&local_ec0, ...
+    ..., (longlong *)&local_regslot, ...
-    LVar7 = PECMD_LI((intptr_t)local_ec0);
-    if (*local_ed0 != L'\0') { PECMD_SetVariable(param_1, local_ed0, (LPCWSTR)local_ec0); }
+    LVar7 = PECMD_LI((intptr_t)local_regslot);
+    if (*local_ed0 != L'\0') { PECMD_SetVariable(param_1, local_ed0, (LPCWSTR)local_regslot); }
@@ 尾部释放（现 :819-820）
     PECMD_FreeStrBuf(&local_eb0);
-    PECMD_FreeStrBuf(&local_ec0);
+    PECMD_FreeStrBuf(&local_regslot);
```
（效果：消除注册表槽泄漏与 ec0 别名重用；虽被 FreeStrBuf 置空兜底非崩溃源，但恢复 dc 结构。）

---

## 4. 后续取证建议（不在本轮执行）

0. **V-Gate 提示**：主代理任何一次成功重建后，须先用新 map 时间戳重验 §1 各 RVA 归属，再引用本报告结论；未重验前按"待版本核验"对待。
1. **补采 dump**：对 024/025/039/053 单独重跑并开启 LocalDumps; 若再现 c0000409，重点看
   `.exr -1` Parameter[0] 与第一现场是否落在 §1.1 同款机制（某共享函数帧内 AV→GS 兜底）。
2. 056 复跑时先 `!heap` 元数据或在 `PECMD_FreeStrBuf` 加探针记录 `(ptr-8)` 头部 magic/size，
   定位坏块的形成者（现有 memfail.log 探针网可直接扩展）。
3. 排查树内其余"双名分裂"恒 0 桩（grep `unimplemented_stubs.c` 中带写出参却 `(void)` 吞参的定义),
   特别是位于 LOAD/脚本装载路径上、可能向调用方栈帧/槽写坏的 arity 错误桩。
