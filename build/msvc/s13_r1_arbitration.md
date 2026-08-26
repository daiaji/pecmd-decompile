# S13-R1 多体歧义仲裁报告（5 项）

- 仲裁角色: 只读裁决子代理；本文件为唯一写入产物，未修改任何 `src/**/*.c` / `restored_bodies.c` / 桩文件，未执行构建、git、调试器操作
- 语义权威源: `decompiled.c`（dc:<行号> 即其行号）；移植树: `refactored/src/**/*.c` + 根目录 `restored_bodies.c`（rb:<行号>）+ `unimplemented_stubs.c`（stubs:<行号>）
- 输入清单: `s13_r1_dualbody_scan.md` 附录「同 VA 多真体歧义登记」共 5 项（与任务书"5 项"一致，全部处理）；历史背景参照 `s11_dual_body_audit.md`
- 方法: 逐项定位各重复体 文件:行号 → 取 dc 原文全文 → 控制流/常量字面量/调用目标/返回值四维逐段对照 → 全树符号绑定核查（区分大小写：C 中大写/小写 FUN 名是不同链接符号）→ 裁决。所有别名等价（g_csInit≡DAT_14013e190、g_hHeap≡DAT_14013d328、g_pFontBase≡DAT_14013e2a8、PECMD_StrBldCopyWide≡FUN_1400703e4 等）均经 pecmd_defs.h:75、REVIEW.md:2231/:1825 及 S11 报告证实后才采信。

## 0. 结论摘要

| # | 对象 (VA) | 候选体 | 裁决 | 动作类型 |
|---|---|---|---|---|
| 1 | @0x1400e5890 GetControlFont | core_b8h.c:344 大写 / core_b7c.c:4560 小写直移 | 两体逐字节等价 ≡ dc，**合法分裂** | 登记映射；指定大写体为唯一权威体 |
| 2 | @0x14000531c SkipLeadingWhitespace 族 | core_proc.c:78 / b1_remaining.c:1689 / b1_remaining.c:2143(static) + 空桩 stubs:332 | 三真体等价 ≡ dc，**合法分裂**；空桩正在吞 rb:11643/11651 | 登记映射；空桩必须转发真体 |
| 3 | rb 直移副本 ×5 VA（07d0ac/07e01c/0633a8/0e6860/073c58） | 各 src/** 真体 vs rb 副本（+2 处小写空桩第三体） | 4 项**合法分裂**；073c58 **需合并**（rb 副本子调被空桩吞噬＝劣化副本）；07e01c rb 副本含两处实质缺陷 | 逐 VA 见 §3；登记映射 + 空桩转发 |
| 4 | PECMD_IsAlphaNumeric 双 VA 同名 | core_b3b.c:227(@140061204) / core_b3a.c:164(@14005bc28) | **非同一原版函数的双体**——两个不同 VA 被 rename_map 撞名；两体各自忠实 ≡ 各自 dc | 合法共存；rename_map 消歧登记；bc28 六处调用正被空桩吞噬（警示） |
| 5 | @0x140003a20 RunCommandLine 展开包装 | unimplemented_stubs.c:475(S8 直移全局) / core_b7c.c:6038(b7c_003a20 static) | 两体逐行等价 ≡ dc，**合法分裂** | 登记映射 |

五项全部 RESTORED（已裁决），无 UNRESOLVED。共性发现见 §6。

---

## 条目1：<FUN_1400E5890 / PECMD_GetControlFont> @0x1400e5890

### 【候选体清单】

| 体 | 位置 | 链接属性 | 调用点（全树实证） |
|---|---|---|---|
| A（大写真体） | `src/ui/core_b8h.c:341-352`（定义 :344，`LRESULT FUN_1400E5890(int64_t obj)`） | 外部 | core_b8m.c:1090/2858/3363、core_b8k.c:761/954、core_b8g.c:655/951、core_b3j.c:131 —— 共 8 处（extern 声明 b8m:68 / b8k:67 / b8g:82 / b3j:51） |
| B（小写直移副本） | `src/ui/core_b7c.c:4559-4567`（定义 :4560，`LRESULT FUN_1400e5890(longlong host)`） | 外部 | 仅 core_b7c.c:5531 一处（TU 内同名直接绑定） |

另存空桩 `unimplemented_stubs.c` 的 `PECMD_GetControlFont`（stubs:233，scan A组#61 记 22 处调用点）仍吞调用，其转发目标即本次仲裁对象。

### 【原文锚点 dc:xxxx】

`decompiled.c:140288-140301`（size=47）：

```c
140291  LRESULT FUN_1400e5890(longlong param_1)
140296    LVar1 = SendMessageW(*(HWND *)(param_1 + 0x20),0x31,0,0);   /* WM_GETFONT */
140297    if ((LVar1 == -1) || (LVar1 == 0)) {
140298      LVar1 = DAT_14013e2a8;
140300    return LVar1;
```

### 【逐段差异】

| 维度 | dc | 体A (b8h) | 体B (b7c) | 判定 |
|---|---|---|---|---|
| 控制流分支数 | 2 分支（== -1 或 == 0 回退） | 相同（:349 `r==-1 \|\| r==0`） | 相同（:4563） | ✓ 一致 |
| 常量字面量 | hwnd@+0x20、msg=0x31、回退 DAT_14013e2a8 | OBJ_HWND=0x20（pecmd_offsets.h:29）、0x31、g_pFontBase（b8h.c:94 注释 ≡ DAT_14013e2a8，REVIEW.md:2231） | 0x20 字面、0x31、g_pFontBase（b7c.c:3835 extern 同槽） | ✓ 一致 |
| 调用目标 | SendMessageW | SendMessageW | SendMessageW | ✓ 一致 |
| 返回值 | LRESULT（RAX 透传） | LRESULT | LRESULT | ✓ 一致 |

两体除形参拼写/局部变量名外逐语句相同，均忠实于 dc。

### 【裁决】

**合法分裂（两体各自覆盖不同调用点），登记映射**：
- `FUN_1400E5890`(大写) ≡ dc:140291 —— 权威体 = core_b8h.c:344；
- `FUN_1400e5890`(小写) ≡ dc:140291 —— core_b7c.c:4560 为同义冗余副本。
依据 REVIEW.md:892-894 项目约定（大小写双名并存、"两者均正常链接"为有意设计），现状不构成行为错误。

### 【建议动作】

1. 登记：`@0x1400e5890 → 权威体 core_b8h.c:344 FUN_1400E5890；别名体 core_b7c.c:4560 FUN_1400e5890（等价直移）`。
2. scan A组#61 的转发落地时，`PECMD_GetControlFont` 桩一律转发到**大写权威体**（勿接小写副本，避免权威体漂移）。
3. 后续收敛波次（可选）：将 b7c.c:5531 改绑大写名后删除小写副本；在此之前保持只读不动。

---

## 条目2：<SkipLeadingWhitespace 族> @0x14000531c

### 【候选体清单】

| 体 | 位置 | 链接属性 | 调用点（全树实证） |
|---|---|---|---|
| A | `src/runtime/core_proc.c:77-84` `WCHAR *FUN_14000531C(WCHAR *p)` | 外部 | core_proc.c:145；core_token.c:155/161（本地声明 core_init.c:47、core_token.c:32） |
| B | `src/misc/core_b1_remaining.c:1689-1696` `uint16_t *PECMD_SkipLeadingWhitespace(uint16_t *s)` | 外部 | 同 TU :1727(×2)、:4452(×2)、:4454(×2)、:4455、:4456 |
| C | `src/misc/core_b1_remaining.c:2143-2150` `static uint16_t *FUN_14000531c(uint16_t *param_1)` | static | 同 TU :3769（static 在 TU 内遮蔽外部小写符号） |
| 空桩 D | `unimplemented_stubs.c:332` `uint16_t *FUN_14000531c(uint16_t *s){(void)s;return s;}` | 外部(no-op) | **restored_bodies.c:11643、:11651 经 stubs_common.h:836 声明绑定到此 no-op** |

### 【原文锚点 dc:xxxx】

`decompiled.c:2261-2270`（size=38）：`for (; ((8 < *param_1 && (*param_1 < 0xe)) || (*param_1 == 0x20)); param_1 = param_1 + 1)` 即跳过 0x09–0x0D 与空格，返回推进后指针。

### 【逐段差异】

三真体的循环条件与 dc 完全同构（`*p > 8 && *p < 0xe` ⟺ `8 < *p && *p < 0xe`；`L' '`=0x20）；控制流单循环无分支差、无常量差、无调用、返回值均为推进指针。**三体两两语义相等且 ≡ dc**。唯一非等价项是空桩 D（恒等返回，跳空白失效）。

### 【裁决】

**合法分裂（三真体各自服务互斥调用点集），登记映射**；同时判定空桩 D 为**现行缺陷源**：rb:11643/11651 的跳空白被静默丢弃——这正是 scan A组#111「二批(多真体)须仲裁唯一体后转发」要解决的问题，本案仲裁结论：**不存在需要二选一的真体冲突，转发任意一个真体都正确，推荐 B**（与 D 同族命名空间、签名 uint16_t\* 一致、零转换透传）。

### 【建议动作】

1. 登记：`@0x14000531c → core_proc.c:78 FUN_14000531C ／ core_b1_remaining.c:1689 PECMD_SkipLeadingWhitespace ／ core_b1_remaining.c:2143(static) FUN_14000531c，三体等价`。
2. R1 转发补丁（属 scan 一批）：`unimplemented_stubs.c:332` 改为尾调 `return PECMD_Save... 即 PECMD_SkipLeadingWhitespace(s);`（需在该文件补 extern 声明或改用 core_proc.c 大写名，二者任选其一并注明 dc:2267 锚点）。修复后 rb:11643/11651 恢复跳空白语义。
3. 不删除任何真体（各有活跃调用点）；长期收敛可在 b1_remaining.c 内统一走 PECMD_SkipLeadingWhitespace 并退役 static C。

---

## 条目3：<restored_bodies.c 直移副本 ×5 VA>

扫描附录第 3 条把 5 个 VA 归为一项；本节逐一仲裁（每 VA 独立给【原文锚点】【逐段差异】【裁决】）。

### 3.1 @0x14007d0ac（SetVariableWithPrefix，&/&amp;&amp; 前缀变量赋值）

**候选体**：
- src 真体：`src/commands/core_b3l.c:992-1050` `void FUN_14007D0AC(int64_t*, LPCWSTR, LPCWSTR)`（大写）
- rb 副本：`restored_bodies.c:14628-14674` `void FUN_14007d0ac(longlong*, LPCWSTR, LPCWSTR)`（小写）

**【原文锚点】** `decompiled.c:79619-79669`（size=279）。

**【逐段差异】** 两体对 dc 的 #&#&# 前缀预处理(:79631-79634)、旁路条件 `*(u8*)(ctx+0xd)&0xf==0 && (ctx[8]==0 \|\| ***(short*)(ctx[8]+0x128)!=0x26)`(:79635-79636)、空名早退(:79637)、前缀拼串(:79657-79659)、`param_1[0x18]` 重定向(:79661-79663)、`EnterCS(DAT_14013e190)/FUN_14001e6bc(...,-1)/LeaveCS`(:79664-79666)、释放临时(:79667) 全部逐段对应。仅子调用目标不同：

| dc 子调 | src 体绑定 | rb 副本绑定 | 判定 |
|---|---|---|---|
| :79640 FUN_14005d534 | `FUN_14005D534` = core_var2.c:344-351（≡dc:55604-55619，script 参数忽略） | `PECMD_SetEnvIfNonEmpty` rb:14621-14626（≡dc:55612-55616） | ✓ 双双等价 |
| :79657 FUN_1400703e4 | `PECMD_StrBldCopyWide`（S11 目标2 已证 ≡ 槽契约） | 同左 rb:14663 | ✓ |
| :79658 FUN_14006375c | `FUN_14006375C` core_string.c:106-124（S-C 返回契约分歧，此处语句式调用无害） | `PECMD_AppendWideStr` rb:7443（≡dc:60917 槽契约） | ✓ 行为等价 |
| :79664 CS | g_csInit ≡ DAT_14013e190 | 同 | ✓ |

**调用点**：大写体 ≈21 处（b3l:1161/1176、b3m:1193、b3j:141/895、b3i:210/697、b3d:512、b8m:1914/2008、b8j:305、b8g:1055、b3_remaining:7571/7575/24097/24228/24283/24324/25208/25896/27623/27699/28015 等）；小写体 4 处（rb:14727、rb:16579、core_b7c.c:3896/3900，后者经本地 extern :3816 绑到 rb 符号）。两组调用点互斥、无交叉误绑。

**【裁决】合法分裂，登记映射**（两体均 ≡ dc，子调实现路径不同但行为一致）。

**【建议动作】** 登记 `@0x14007d0ac → 权威 core_b3l.c:995；别名 rb:14629（等价直移，服务 rb/b7c 四处调用）`；后续波次若收敛，把 b7c.c:3896/3900 与 rb 两处改绑大写名后退役 rb 副本。

### 3.2 @0x14007e01c（SetControlState）

**候选体**：
- src 真体：`src/commands/core_b3m.c:1140-1195` `FUN_14007E01C(int64_t,uint32_t,LPWSTR)`
- rb 副本：`restored_bodies.c:14676-14729` `FUN_14007e01c(longlong,uint,LPWSTR)`

**【原文锚点】** `decompiled.c:80358-80416`（size=318）。

**【逐段差异】** 主干（state≠0xf8000009 时写 +0x70(:80375)、`#编号:` 解析(:80382-80398)、标题赋值(:80399)、ShowWindow nCmdShow∈{0,5}(:80401-80404)、state<0x65 发 0x402(:80405-80407)、InvalidateRect(:80408-80410)、尾部变量回报(:80411-80414)）两体结构一致，但有两处**实质分歧**：

| 差异 | dc | src 体 | rb 副本 | 判定 |
|---|---|---|---|---|
| D1 编号解析子调 | :80389 `FUN_140074838(&local_res8,local_res10)` | :1169 `PECMD_ParseUIntValue`（≡dc，真体 core_scriptdep.c:458，scan#146 RM 证据） | :14703 `FUN_140074838` 小写 → 绑**空桩 stubs:392**（no-op） | ✗ rb 副本解析恒失败 |
| D2 wsprintfW 变参 | :80411 Ghidra 反编译丢失第三实参（工件） | :1191 补齐 `(WPARAM)(int)*(int *)(obj + 0x70)`（合理重构） | :14725 照抄缺参形态 → UB（读到寄存器残渣） | ✗ rb 副本未修复 |

**调用点现状**：两体当前**均为 0 引用**（大写体全树无调用；rb 副本仅被自身文件外声明 stubs_common.h:3434 引用为声明）；活跃流量走 `PECMD_SetControlState` 空桩 stubs:228 —— 实证调用点 core_b3_remaining.c:25296（xproto.h:261 存权威签名）。

**【裁决】需合并（KEEP src 体为唯一权威体）**：rb 副本在 D1/D2 两点上劣于 src 体，不应作为转发目标；src 体为可安全激活的实现。

**【建议动作】** ① 登记 `@0x14007e01c → 权威 core_b3m.c:1143 FUN_14007E01C；rb:14677 退役候选（D1/D2 缺陷注记）`；② scan A组#59 转发落地时 `PECMD_SetControlState(stubs:228)` 尾调 `FUN_14007E01C(a,b,c)`，使 core_b3_remaining.c:25296 恢复功能；③ 收敛波次删除 rb:14676-14729（其内部对 rb 版 07d0ac 的调用随 §3.1 映射一并迁移）。

### 3.3 @0x1400633a8（AllocStringSlot2，len+9 定长缓冲分配）

**候选体**（实际为**三体**）：
- src 真体：`src/runtime/core_thread.c:39-64` `FUN_1400633A8(void**, int64_t)`（大写）
- rb 改名体：`restored_bodies.c:7405-7422` `PECMD_AllocStringSlot2(void**, longlong)`（rename_map.json:1117 正名）
- 小写空桩第三体：`stubs:777 int FUN_1400633a8(){}` —— 吞掉 rb:17608 的调用（签名还是错的 `int()`）

**【原文锚点】** `decompiled.c:60681-60707`（size=122）：置空→`if(-1<len)` HeapAlloc(len+9) OOM 重试环(:60693-60697，`OomPrompt!=4 则退出`)→+4 写 0xaa55(:60699)→+0 写 len+1(:60700)→槽=头+8(:60701)→首字节清零(:60702-60704)。

**【逐段差异】**

| 维度 | src 体 | rb 改名体 | 判定 |
|---|---|---|---|
| 分配尺寸 len+9、magic 0xaa55、cap=len+1、首字节 0 | :48/:57/:58/:59-62 | :7412/:7417/:7418/:7419-7420 | ✓ 一致 |
| 写序（先魔数后尺寸 qword） | :57-58 且注释记录 T1c 崩溃实锤修复 | :7417-7418 | ✓ 双方均已按 dc 序 |
| OOM 放弃路径 | :47-52 忽略 `FUN_1400630D0(2)` 返回 → **无限重试** | :7414 `if (PECMD_OomPrompt(2) != 4) break;` → 退出后 NULL 解引用崩溃（忠实 dc:60697） | **分歧**：OOM 用户放弃时 src 挂起 vs dc/rb 崩溃 |

**调用点**：大写体 ≈36 处（b2f×6、b2e×4、b2d×3、b3l×2、b3m、b2b、b2c、thread×2、b8j、b8i×2、b8l、b8m×2、b8_failed×2、lang_exec、b1_remaining×7）；改名体 ≈40 处（b3_remaining×14、b7c×9、b3r_h3×5、rb 内×6、b3r_g4/g5/b3r_b/b3r_c/b3r_d/b3r_h4/i28c 等）——两组互斥。小写空桩吞 rb:17608 ≥1 处（分配永不发生，后续使用未初始化缓冲）。

**【裁决】合法分裂（两大写/改名真体各自覆盖互斥调用点），登记映射；第三体（小写空桩）必须转发**。OOM 放弃路径分歧如实登记：现网两体均不会在正常路径产生行为差，收敛时应显式决定保留哪一种（保守取向：向 dc 的「退出即崩」看齐，或统一改为无限重试并在两体同步——超出本仲裁权限，交主代理定夺）。

**【建议动作】** ① 登记 `@0x1400633a8 → core_thread.c:41 FUN_1400633A8 ＆ rb:7406 PECMD_AllocStringSlot2（等价分裂，OOM 路径分歧已注记）`；② R1 转发：stubs:777 删除，rb:17608 改绑 `PECMD_AllocStringSlot2`（同文件内即可，参数原序）；③ 收敛波次统一 OOM 语义。

### 3.4 @0x1400e6860（EndDialogDeferred，对话框关闭编排）

**候选体**：
- src 真体：`src/ui/core_b8f.c:223-253` `FUN_1400E6860(uint64_t*, int)`（大写，scan#195 记 0 引用）
- rb 改名体：`restored_bodies.c:10033-10053` `PECMD_EndDialogDeferred(uint64_t,int)` —— 调用点 core_b1_remaining.c:610/633（extern :330）
- 小写空桩第三体：`stubs:546 void FUN_1400e6860(WPARAM,int){}` —— 吞掉 rb:16423 的调用

**【原文锚点】** `decompiled.c:141058-141091`（size=127）：flags@+0x120；bit0=0 时（flags==0 直取 hwnd@+0x20，否则置 0x80 并 PostMessageW(0,msg,obj,result) 再取）EndDialog(hwnd,result)；bit0=1 时 hwnd≠0 则清 +0x20、IsWindow→DestroyWindow。

**【逐段差异】** 两体与 dc 三路对照：分支结构（bit0 判断、flags==0 二分、销毁路径三分支）、常量（+0x120/+0x20/消息 0/掩码 0x80）、API 目标（EndDialog/PostMessageW/IsWindow/DestroyWindow）、返回 void 全部一致。**双双忠实 ≡ dc，无差异项**。

**【裁决】合法分裂 + 第三体转发**。

**【建议动作】** ① 登记 `@0x1400e6860 → core_b8f.c:226 FUN_1400E6860 ＆ rb:10034 PECMD_EndDialogDeferred（等价）`；② R1 转发：stubs:546 改为尾调 `PECMD_EndDialogDeferred((uint64_t)a1,a2)`（或大写体，二者等价任选），恢复 rb:16423 的延迟关窗；③ 长期收敛二选一并退役另一体。

### 3.5 @0x140073c58（CleanupParser，解析器清理）

**候选体**：
- src 真体：`src/commands/core_b3h.c:523-539` `FUN_140073C58(int64_t parser)`（大写，scan#79 记 0 引用）
- rb 小写副本：`restored_bodies.c:17256-17271` `FUN_140073c58(longlong)` —— 调用点 rb:16382/16421

**【原文锚点】** `decompiled.c:73204-73224`（size=115）：`EnterCS(DAT_14013e190); FUN_14006703c(param_1); if(+400!=0) FUN_140066eac(param_1,0); if(+400!=0){HeapFree(DAT_14013d328,0,+400-8); 清 +400 与 +0x198;} LeaveCS;`（dc:73220-73221 的 jumptable WARNING 属反编译注记，不影响语义）。

**【逐段差异】** 结构两侧均按 dc 五步展开，但**两个子调用目标链接天差地别**：

| dc 子调 | src 体绑定 | rb 副本绑定 | 判定 |
|---|---|---|---|
| :73211 FUN_14006703c（释放窗口/钩子） | `PECMD_ReleaseWindowHooks` core_b3_remaining.c:13397-13399 —— **有完整实体**（:13402 起 ShowWindow/SendMessageW/UnhookWindowsHookEx…；:13401 的「UNIMPLEMENTED」注记为陈旧残留，实体代码在其后） | 小写 `FUN_14006703c` → **空桩 stubs:547** | ✗ rb 副本此步静默丢效 |
| :73213 FUN_140066eac（对象列表尾释放） | `PECMD_ReleaseObjectListTail` core_b3_remaining.c:13354-13387（完整实体） | 小写 `FUN_140066eac` → **空桩 stubs:784** | ✗ rb 副本此步静默丢效 |
| HeapFree/清零段 | :534-536 ≡ dc:73216-73218 | :17265-17267 ≡ dc | ✓ |

**调用点现状**：大写体 0 引用；rb 副本 2 处（rb:16382/16421）——**当前全部清理流量走的都是劣化副本**（只剩加锁+HeapFree，钩子与对象列表泄漏）；另有 `PECMD_CleanupParser` 空桩 stubs:270（scan#79 记 3 调用点）同样吞调用。

**【裁决】需合并——KEEP src 体（core_b3h.c:526）为唯一权威体**；rb 副本判定为**劣化副本**（两个释放步骤被空桩吞噬），不得作为转发目标。

**【建议动作】** ① 登记 `@0x140073c58 → 权威 core_b3h.c:526 FUN_140073C58；rb:17257 退役候选（子调空桩化缺陷）`；② R1 转发（优先级高于一般条目，属行为修复）：stubs:270 尾调 `FUN_140073C58(obj)`；rb:16382/16421 改绑大写权威体；③ 收敛波次删除 rb:17256-17271。

---

## 条目4：<PECMD_IsAlphaNumeric 双 VA 撞名>

### 【候选体清单】

| 体 | 位置 | 对应 VA | 链接属性 |
|---|---|---|---|
| 甲 | `src/commands/core_b3b.c:224-234` `uint64_t PECMD_IsAlphaNumeric(uint16_t ch)` | @0x140061204 | 外部 |
| 乙 | `src/commands/core_b3a.c:161-170` `static uint64_t PECMD_IsAlphaNumeric(uint16_t ch)` | @0x14005bc28 | static |

撞名证据：`tools/rename_map.json:33`（FUN_14005bc28→PECMD_IsAlphaNumeric）与 `:65`（FUN_140061204→PECMD_IsAlphaNumeric）；provenance.map:742-743 同样双记。

### 【原文锚点 dc:xxxx】

```c
58965  if ((((param_1 < 0x61) || (0x7a < param_1)) && ((param_1 < 0x41 || (0x5a < param_1)))) &&   /* FUN_140061204 size=43 */
58966     (9 < (ushort)(param_1 - 0x30))) { return 0; } return 1;      /* 接受 a-z / A-Z / 0-9 */
54316  if (((param_1 < 0x61) || (0x7a < param_1)) && (9 < (ushort)(param_1 - 0x30))) {              /* FUN_14005bc28 size=31 */
54317    return 0; } return 1;                                          /* 仅接受 a-z / 0-9（无大写支） */
```

### 【逐段差异】

甲体（b3b.c:229-230）含 `ch<0x41 || 0x5a<ch` 大写排除项，≡dc:58965；乙体（b3a.c:166）无此项，≡dc:54316。**两体各自与其 VA 的 dc 原文逐字符一致——这不是同一原版函数的重复体，而是两个不同原版函数被映射到了同一个改名结果**。size 差（43 vs 31）与多出的大写比较吻合，佐证无误。

**调用点现状**：
- @0x14005bc28：小写空桩 stubs:398 吞掉 core_b3_remaining.c:13066/13081/13118/13244/13256/13274 **共 6 处**调用（恒返回 0＝"非法字符"）；其真乙体是**他 TU 的 static**，现有转发模板无法直接按名转发（这正是附录第 4 条"勿混接"警告的实质风险点：若图省事把空桩转到大写全局体甲，会把 6 处调用从"拒绝大写"错改成"接受大写"，引入语义污染）。
- @0x140061204：全树无空桩亦无调用（零流量），全局甲体暂无人引用。

### 【裁决】

**合法共存（非双体冲突），登记消歧映射**；两体 KEEP 不动。附带裁定：@0x14005bc28 的 6 处被吞调用属 R1 转发范畴，但**转发目标必须是 bc28 自身语义**，禁止借用甲体。

### 【建议动作】

1. 登记映射：`@0x140061204 → core_b3b.c:227 PECMD_IsAlphaNumeric（含大写，≡dc:58965）；@0x14005bc28 → core_b3a.c:164 static PECMD_IsAlphaNumeric（无大写，≡dc:54316）；两 VA 不得互指`。
2. rename_map 消歧（后续波次，改表不改码亦可先行）：将 @0x14005bc28 的目标名改为独占名（建议 `PECMD_IsAlnumLowerDigit`），杜绝未来误接。
3. 若要在本轮修复 stubs:398 的 6 处吞调用：先把乙体以新独占名导出（或在 core_b3a.c 增加 `uint64_t PECMD_IsAlnumLowerDigit(uint16_t){…同乙体…}` 全局包装），再令 stubs:398 尾调该名——**不要**指向甲体。

---

## 条目5：<FUN_140003a20 / PECMD_RunCommandLine 展开包装>

### 【候选体清单】

| 体 | 位置 | 链接属性 | 调用点（全树实证） |
|---|---|---|---|
| A（S8 直移全局体） | `unimplemented_stubs.c:471-498` `longlong FUN_140003a20(longlong*,undefined8*,byte)`（带 P8_Probe 日志） | 外部 | restored_bodies.c:14228/15211；core_cmd4.c:772；core_b3i.c:227；core_b3m.c:1346；另有别名壳 `PECMD_RunCommandLine`（stubs:52-55，B 组✅已验证转发至此体）40 处级流量 |
| B（b7c 私有副本） | `src/ui/core_b7c.c:6036-6058` `static longlong b7c_003a20(longlong*, WCHAR**, uint8_t)`（前向声明 :121） | static | core_b7c.c:926/931/6204 |

### 【原文锚点 dc:xxxx】

`decompiled.c:1236-1261`（size=139）：备份 `*param_2`→置空输出槽(:1246-1247)；`(*(char*)(ctx+0xda)=='\0') && ((*(byte*)(ctx+0xd)&0xf)==0)` 走单遍 `FUN_14007bda8(...,flag)`(:1248-1251)；否则双遍 `FUN_14007a224(...,flag)`＋交换＋`FUN_14007a224(...,0)`(:1252-1258)；`FUN_14005b104(&saved)` 释放旧串(:1259)；返回 lVar2(:1260)。

### 【逐段差异】

| 维度 | 体A | 体B | 判定 |
|---|---|---|---|
| 分支条件与遍数 | :484-485 ≡dc:1248-1249，单/双遍同构 | :6043-6044 同 | ✓ |
| 单遍目标 | FUN_14007BDA8（真体 core_execline.c:68 @0x14007bda8，scan B 组✅） | PECMD_ExpandVarsRecursive（同址别名壳→同一真体） | ✓ |
| 双遍目标 | FUN_14007A224（真体 core_exepline.c:119 @0x14007a224，scan B 组✅） | PECMD_ExpandCommandLine（同上） | ✓ |
| 旧串释放 | PECMD_FreeStrBuf（≡dc:1259 FUN_14005b104） | 同 | ✓ |
| 返回值 | longlong lVar2 ≡dc:1260 | longlong r ≡dc | ✓ |
| 附加副作用 | P8_Probe ×1（仅日志） | 无 | 无行为差 |

### 【裁决】

**合法分裂，登记映射**。两体逐语句等价且均 ≡ dc；B 体注释自证其存在动机（规避历史上 link_stubs.c 同名 no-op 强符号冲突而 static 化），A 体承担全部跨 TU 流量。无行为错误，无需合并。

### 【建议动作】

1. 登记：`@0x140003a20 → 权威 unimplemented_stubs.c:475 FUN_140003a20（PECMD_RunCommandLine 别名壳已指向此体）；core_b7c.c:6038 b7c_003a20 为等价 static 副本`。
2. 可选收敛：待 link_stubs 历史 no-op 冲突解除后，core_b7c.c 三处改调全局名并删除 B 体（低收益，非必需）。

---

## 6. 共性发现与移交清单

1. **"多真体"大多不是冲突而是命名分层**：本项目以大小写双名（REVIEW.md:892-894 约定）与"改名体 vs 原名直移体"承载不同调用点集，凡各体均忠实 dc 且调用点互斥者，一律裁「合法分裂＋登记映射」，不做破坏性合并。
2. **真正的行为缺陷集中在"第三体＝小写空桩"上**：本轮实证 4 处空桩正在吞调用——`FUN_14000531c`(stubs:332→rb:11643/11651)、`FUN_1400633a8`(stubs:777→rb:17608)、`FUN_1400e6860`(stubs:546→rb:16423)、`FUN_14005bc28`(stubs:398→core_b3_remaining.c 六处)；另有间接劣化 2 处——rb 版 `FUN_140073c58`(:17257) 与 rb 版 `FUN_14007e01c`(:14677) 因内部小写子调落空桩而降级。上述 6 点应并入 S13-R1 转发批次执行（转发目标一律取本报告指定的权威体/独占名）。
3. **权威体速查**：@1400e5890→core_b8h.c:344；@14000531c→三体任一（推荐 PECMD_SkipLeadingWhitespace）；@14007d0ac→core_b3l.c:995；@14007e01c→core_b3m.c:1143；@1400633a8→core_thread.c:41 与 rb:7406 双权威（OOM 语义待统一）；@1400e6860→core_b8f.c:226 与 rb:10034 双权威；@140073c58→core_b3h.c:526；@14005bc28→core_b3a.c:164（static，导出需新独占名）；@140061204→core_b3b.c:227；@140003a20→unimplemented_stubs.c:475。
4. **取舍记录**：(i) 行号一律以本次实读为准（scan 报告所引 rb:14624/14672/7401/10029/17252 与实际定义行有 ±5 行偏差，已在各节更正为 14629/14677/7406/10034/17257）；(ii) P8_Probe/TEMP PROBE 类日志不计入语义；(iii) dc:80411 wsprintfW 缺参判为 Ghidra 反编译工件而非原文语义，采信 src 体重构（obj+0x70 状态值），未虚构其它实参可能；(iv) core_b3_remaining.c:13401 陈旧 "UNIMPLEMENTED" 注记与其实体代码并存，按实体代码采信；(v) OOM 放弃路径（§3.3）与 rename_map 撞名（条目4）涉及策略选择，按只读纪律仅登记建议，未改任何表/码。

*本报告为只读仲裁产物（唯一写入文件）；全部证据来自现场全文检索与逐行比对；未修改代码、未构建、未触碰 git。*
