# R24 · S083 语义核对裁决 — PECMD_TokenizeExpression（FUN_1400a53e4）尾段

> 产出：`analysis\r24_s083_verdict.md`（本轮唯一落盘文件，UTF-8）
> 核对对象：`analysis\s083_tokenize_tail_draft.c`（507 行草稿）· 真值 `reference\decompiled.c` dc:103346-103731 · 既有实现 `src\commands\core_b3_remaining.c:24757-25173`
> 方法：dc/草稿/既有实现三文本并排逐语句比对 + 括号深度骨架程序化核验（bash/python 只读切片，未构建未部署未改 src）
> 结论先行：**尾段（dc:103680-103730 ↔ core_b3_remaining.c:25119-25172）逐语句 1:1 一致，无真实缺失；草稿 PART A 为忠实二次转录，仅 1 处注释口径错误（draft:175-177，不影响既有实现）；PART B 为证据区归属（CreateMenuItem），其 2 处 TODO 中 1 处可关闭、1 处登记 divergence 移交。**

---

## 一、范围核定与口径说明

| 项 | 事实 | 证据 |
|---|---|---|
| FUN_1400a53e4 = PECMD_TokenizeExpression | dc:103346(头注)-103731(闭括号)，正文 dc:103349-103730，size=2456 | dc:103346-103731 |
| 既有实现 | `core_b3_remaining.c:24759-25173`（函数定义），尾段 25119-25172 | 已读 |
| 草稿 PART A（尾段二次转录） | draft:106-248，覆盖 dc:103680-103731 | 已读 |
| 草稿 PART B（证据区） | draft:264-499，覆盖 dc:103882-104037 = FUN_1400a600c(PECMD_CreateMenuItem) 中后段 | 已读；h4:724-950 已转录 |
| 任务书"~690 行起" | 草稿仅 507 行，无 690 行；"~690"系 103346→104028 跨度约数（683 行），对应**草稿 PART A 尾段**而非 PART B | 见 notes 三 |

**本轮新结构澄清（括号骨架核验，修正此前对 dc 的误读）**：dc:103406 的 `while( true ) { … }` 只是**前导空白/定界符跳过环**（103407-103414，对应既有实现 24817-24827），**并非包裹全函数的外层循环**。函数为单遍结构：
`puVar16=*param_3 → local_74 判定 → do{…}while(定界符) 首跳 → while(1) 空白跳过 → if(psVar12==NULL){lVar8=1} else { 克隆/展开/扫描/合并/清理 } → return lVar8`。
括号深度对账：103406 d1→2，103414 回到 d1，103418 else 开(→2)，103729 关(→1)，103730 return 在 d1 函数体级。既有实现骨架与之完全同构。

---

## 二、三栏对照表（草稿全部标注段：TODO / 未定 / 近似）

### PART A —— TokenizeExpression 尾段（任务主项）

| 编号 | dc 行号 | draft 行号 | 语义差异 / 一致 | 裁决 |
|---|---|---|---|---|
| S1 | 103680 `} while (*pWVar20 != *(WCHAR*)(param_1+0x88));` | 153-158（`/* } while…*/` + goto 模拟） | 草稿以 goto 模拟 do-while 回边，仅为片段语法自洽（draft:501-506 已声明包装壳）；退出条件文字与 dc 完全一致。落码形态 = 既有实现 25119 的真实 do-while | **OK**（片段形式；落码用 impl 形态） |
| S2 | 103681 `}`（闭合 `if(*pWVar20 != 0x88)` 块） | 160-161（`s083_scan_continue / s083_loop_exit` 标签壳） | 结构点一致；草稿标签为私有命名，不入正式树 | **OK** |
| S3 | 103682-103689 尾随定界符跳过环 | 162-172 | 循环条件（0x92/0x94/0x8a/0x90 四判）、`int16_t` 读取、`+1` 步进逐字一致 | **OK** |
| S4 | 103690 `iVar15 = 0;` | 173 | token 模式正常收尾清零返回码，一致 | **OK** |
| S5 | 103691-103696 `else { uVar10 = FUN_140025474(local_80.QuadPart,param_3,1,pWVar20); iVar15=(int)uVar10; LVar9=local_80; }` | 175-182 | ⚠ **草稿条件写成 `if (local_74 != 0)`，dc 为 `(param_4 & 1) != 0` 侧（即 `if((param_4&1)==0)` 的 else）**。local_74 是"参数模式"标志（dc:103395 由 `(*(ushort*)(p1+0x48)^0x2a)==*puVar16` 计算），与 param_4 位无关。实参表/返回处理/LVar9 刷新一致。**既有实现 25133-25137 已是正确 `else`，无需改动**；仅若以草稿为落码底本时须改正 | **需改**（仅 draft 文本；核心 25133 已正确） |
| S6 | 103697 `LVar13 = local_50;` | 184 | 一致 | **OK** |
| S7 | 103698-103699 状态字合并 `(*(ushort*)(LVar9+200) \| (short)cVar2) & 0x1ef` | 185-190（TODO 注 185-188） | dc 用 `(short)cVar2`（符号扩展）；草稿/既有实现用 `(uint16_t)(char)cVar2`。两者对任意 cVar2 在 16 位存储后**逐位等价**（均先经 int 符号扩展再 `&0x1ef`，掩码使高位即使有差也被裁掉）。等价性已推演：cVar2=-1→两侧均 0xFFFF; cVar2=0x80→两侧均 0xFF80→&0x1ef=0x180 | **OK**（TODO 关闭；无需改 impl 25139-25140） |
| S8 | 103700-103702 `if(*(char*)(LVar9+0xca)) *(u64*)(p1+0xd0)=*(u64*)(LVar9+0xd0)` | 191-194 | 一致（0xca 异常/跳转标志 → 0xd0 定位槽回传） | **OK** |
| S9 | 103703-103713 `if(local_74){ 0x24=local_44; 0x28=local_38; if(bVar5){0x70/0x60/0x68 换回 + local_70=lVar8} }` | 195-207 | 一致（**此处 dc 与草稿均正确用 local_74**，与 S5 的 param_4 分支是不同的两处，勿混） | **OK** |
| S10 | 103714-103718 `if(local_50){ FUN_14004eaa8(local_50,0); FUN_14005b104(LVar13+0x70); free(LVar13); }` | 208-213 | 克隆子脚本清理（ClearTaskTable + 槽释放 + free），一致 | **OK** |
| S11 | 103719 `lVar8 = (longlong)iVar15;` | 216 | 一致 | **OK** |
| S12 | 103720-103721 `FUN_14005b104(&local_70); FUN_14005b104((longlong*)&local_60);` | 217-218 | 一致（stub 签名为 `void PECMD_FreeStrBuf(void*)`，restored_bodies.c:7256；传 `(WCHAR**)&…` 只是调用点窗口类型，地址语义同） | **OK** |
| S13 | 103722-103728 `EnterCS(DAT_14013e190) / if(local_20){FUN_140066838(…);FUN_140066f64(…);} / LeaveCS` | 219-225 | 一致；符号映射经 `tools/rename_map.json:50/806` 核实（FUN_140066838→PECMD_ForwardCall_6838、FUN_140066f64→PECMD_ReleaseObjectSlots），g_csInit=DAT_14013e190 | **OK** |
| S14 | 103729-103730 `}` + `return lVar8;` | 246-248 | 一致（草稿 227-246 的 `(void)` 抑制与 D1 记录同，无语义影响） | **OK** |
| S15 | 103415-103417 错误路径 `if(psVar12==NULL){ lVar8=1; }`（尾段上游） | 未转录（前文占位，draft:150-151） | 既有实现 24828-24830 与 dc 逐字一致 —— 交叉印证尾段错误返回契约：**唯一"参数指针空"提前退出返回 1** | **OK**（交叉印证，非本轮尾段） |

### PART B —— 证据区（CreateMenuItem，草稿仅 2 处 TODO）

| 编号 | dc 行号 | draft 行号 | 语义差异 / 一致 | 裁决 |
|---|---|---|---|---|
| S16 | 103902-103904 `if ((int)uVar4 < 0) { uVar4 = -uVar4 \| 1; }` | 318-321（TODO 注 319） | dc 原文逐字如此；`core_b3r_h4.c:797` 已按此落码。草稿 TODO"语义待汇编核验"过分保守——转录本身无歧义（错误码保号去零），可关闭 | **OK**（TODO 关闭；h4:797 已对齐） |
| S17 | 104036 `return (byte *)LVar11.s;` | 496-498（TODO 注 496-497） | ⚠ **类型宽度差异**：dc 渲染为 8 字节结构成员 `.s`（LARGE_INTEGER 联合体，含 HighPart），错误路径 LVar11 可取 `-0x7ff8ffa9`（dc:103875，符号扩展 → RAX 全 64 位）；草稿与 h4:949 均为 `(uintptr_t)(uint32_t)LVar11.LowPart`（截 32 位 → 0x80070057）。属 PART B 归属线（CreateMenuItem）范畴，非本工单尾段；当前树内无 PECMD_CreateMenuItem 外部调用方，影响不可定案 | **保持 TODO(verify)**（登记 divergence，移交 CreateMenuItem 归属线；不臆改） |
| S18 | 103875 `LVar11.QuadPart = -0x7ff8ffa9;` | 未转录 | 佐证 S17：成功路径 LVar11=0（dc:103870）、-sub 解析失败路径 =1（dc:103944）；h4:764/769/845 忠实保留 —— 除返回宽度外 h4 与 dc 一致 | **OK**（佐证项） |

---

## 三、特别核对（任务点 2）

### 3.1 尾段循环 / 终止条件 / 错误返回路径

| 项 | dc | 既有实现 | 结果 |
|---|---|---|---|
| 扫描 do-while 终止条件 | 103680：`*pWVar20 != *(WCHAR*)(param_1+0x88)`（表达式首定界符，通常 '('） | 25119 同句 | 一致 |
| 尾随定界符跳过环 | 103682-103689：读 `(short*)*param_3` 与 0x92/0x94/0x8a/0x90 四字节比对，命中即 `+1` 步进，遇 NUL(0x0000) 自动停 | 25121-25130 同构 | 一致 |
| token 模式返回码 | 103690 `iVar15=0`（**无条件覆盖**扫描期 iVar15/local_78，故 token 模式恒返 0；local_78 仅用于递归门控） | 25131 同 | 一致 |
| 命令块模式返回码 | 103693-103694 `(int)FUN_140025474(...)` 强转截断 | 25134-25135 同 | 一致 |
| 错误路径 1（游标空） | 103415-103417 `psVar12==NULL → lVar8=1` | 24828-24830 同 | 一致 |
| 错误路径 2（克隆失败） | 103422-103426 `operator_new(0xf0)==NULL → LVar9=0, local_50=0`（退化为父脚本直用） | 24835-24839 同 | 一致 |
| 扫描内 break 汇合 | 103513/103540/103627 break 后经 103681/103682-103690 收口 iVar15=0 | 24929/24958/25056 + 25120-25131 同 | 一致 |
| 唯一出口 | 单 `return lVar8`（103730） | 25172 同 | 一致 |

### 3.2 变参处理

- dc 签名第 2 参 `undefined8 param_2` **函数体内从不读取**（Ghidra 变参/寄存器残留伪影）；既有实现以 `(void)param_2;`（25171，即 notes D1）抑制告警，语义=丢弃。两处递归点（dc:103517/103581）与全部外部调用方均传"占位值"（通常是 `*(p1+0x40)` 脚本链值），与 callee 忽略行为自洽。
- 第 4 参 `uint param_4` 仅测试 `& 1`：0=扫描/计数模式，1=命令块解析模式（dc:103487/103692）；既有实现 24900/25133 一致。
- 第 5 参 `WCHAR *param_5`：非空且首字符非 NUL 时置 `bVar5=true` 并走 FUN_14007a224(→PECMD_ArgTokenize) 参数展开路径（dc:103451-103454/103474-103476，impl 24864-24889 一致）；空串（g_szEmpty）即不展开。

### 3.3 调用方参数契约（dc 全部 12 处调用点 + msvc 侧）

| dc 调用点 | 行号 | param_1 | param_2(占位) | param_3(游标 入/出) | param_4 | param_5 | msvc 对应 |
|---|---|---|---|---|---|---|---|
| 22543 | (LARGE_INTEGER)param_1 | *(p1+0x40) | param_2 | 1 | &DAT_14011c638 | core_b2d.c:507（script, *(sc+0x40), pp, 1, g_szEmpty）✓ |
| 22598 | (LARGE_INTEGER)param_1 | *(p1+0x40) | &local_58 | 1 | local_40 | core_b2d.c:567（…, 1, local_40），569 `&0xffffffff` 掩码 ✓ |
| 31472 | (LARGE_INTEGER)param_1 | local_res18 | &local_48 | 0 | &DAT_14011c638 | core_b2c.c:1084（0, g_szEmpty）✓ |
| 45303 | (LARGE_INTEGER)param_1 | param_3 | &local_res10 | 0 | &DAT_14011c638 | core_b3_remaining.c:4832（0, g_szEmpty）✓ |
| 45339 | (LARGE_INTEGER)param_1 | param_3 | &local_res8 | 0 | local_50 | core_b3_remaining.c:4878（0, local_50）✓ |
| 103517/103581 | 递归 | LVar13/local_80 | *(p1+0x40) | &local_88 | 0 | g_szEmpty / local_58 | impl 24934-24936 / 25006-25008 ✓ |
| 107581 / 108471 / 108518 / 110370 / 110424 | 其它 TU 调用点 | — | — | — | 0（107581/108471/108518/110370/110424 均 0；无 1） | g_szEmpty×4 / pWVar12 | msvc 对应 TU 未纳入本次核对 → **SKIP（范围外，不影响尾段裁决）** |

契约要点：
1. **param_1**：脚本上下文指针（强转 LARGE_INTEGER），克隆子脚本时传 `local_80/LVar9/LVar13`；
2. **param_3**：`WCHAR**` 游标 in/out，函数推进至表达式尾；首字符定界时返回 1；
3. **param_4 bit0**：0=扫描，1=ParseCommandBlock；bit1+ 未见调用方使用；
4. **param_5**：参数展开前缀（空=g_szEmpty=不展开）；
5. **返回**：token 模式恒 0；命令块模式=(int)ParseCommandBlock 返回值；游标空=1。调用方以 0/非 0 判断，22598 侧另做 `&0xffffffff`。

**既有 extern 契约隐患（notes 遗留风险 1，复核确认，不阻塞）**：`core_b2c.c:143`（`int64_t ctx`…）、`core_b2d.c:132`（`int64_t a1, WCHAR **a3`…）、`core_b3_remaining.c:672`（`FUN_1400a53e4(int64_t,void*,void*,int,LPCWSTR)`）三个本地 extern 与正式定义 `PECMD_TokenizeExpression(LARGE_INTEGER, int64_t, int64_t*, uint32_t, WCHAR*)` 类型书写不一致；x64 ABI 下按值 8 字节 LARGE_INTEGER 单寄存器、指针同宽，**恰好兼容**，属应统一项而非错误。

---

## 四、可落码清单（供主代理执行）

### A. `src/commands/core_b3_remaining.c` —— **无需编辑（主项）**
- PECMD_TokenizeExpression 尾段 25119-25172 与 dc:103680-103730 逐语句一致；S5 分支在既有实现中已是正确 `else` 形态。**没有缺失、没有需改语句。**
- （可选，不建议）25139-25140 若要逐字贴近 dc:103698 可写 `| (short)cVar2`；现形 `(uint16_t)(char)cVar2` 16 位存储后等价且已过对拍，改它属无谓 churn。

### B. `analysis/s083_tokenize_tail_draft.c` —— 可选修正（存档资产，不影响构建）
1. draft:175-177：把 `if (local_74 != 0)` 的注释/条件订正为 `(param_4 & 1) != 0` 侧（dc:103691-103696），或直接注明"落码以既有实现 25133 else 为准"；
2. draft:319：关闭 TODO（dc:103903 逐字一致，h4:797 已落）；
3. draft:496-498：保留 TODO(verify)，追加 S17 divergence 注（dc:104036 `.s` 8 字节 vs 32 位截断；移交 CreateMenuItem 归属线）；
4. 文件头"前提证伪存档"说明可补充本轮结论指针（本报告）。

### C. 需要先恢复的输入符号 —— **无**
尾段全部依赖已在树中：
| 符号 | 位置 |
|---|---|
| PECMD_ParseCommandBlock | 定义 core_b2d.c:451；extern include\xproto.h:220 |
| PECMD_ClearTaskTable | extern include\stubs_common.h:999 |
| PECMD_ForwardCall_6838 | 定义 core_b3a.c:348 |
| PECMD_ReleaseObjectSlots | 定义 core_b3_remaining.c:13391 |
| PECMD_FreeStrBuf | 定义 restored_bodies.c:7256（stub，`void*(void*)`） |
| PECMD_AllocStrSlot / PECMD_AllocString | 定义 restored_bodies.c:7339 族（头部使用） |
| g_csInit（=DAT_14013e190） / g_szEmpty（=DAT_14011c638） | include\pecmd_globals.h:407 等 |
| FUN_14007a224、FUN_14006156c 等主体符号 | 既有实现的头部/中部已引用，非尾段新增 |

### D. 契约统一建议（非本轮强制）
统一 `core_b2c.c:143`、`core_b2d.c:132`、`core_b3_remaining.c:672` 三处本地 extern 首参为 `LARGE_INTEGER`（或全库统一为 `int64_t`+注释锚定），消除 notes 遗留风险 1。

---

## 五、目标状态汇总

| 目标 | 状态 |
|---|---|
| 尾段（dc:103680-103730 ↔ impl 25119-25172）逐语句核对 | **RESTORED**（1:1 一致，无缺失） |
| 草稿 PART A 全部标注段裁决（S1-S14） | **RESTORED**（13×OK + 1×需改[draft 文本仅]） |
| 循环/终止/错误路径/变参/调用方契约核对 | **RESTORED**（含骨架结构澄清） |
| 草稿 PART B TODO（S16/S17/S18） | S16 **RESTORED**（关闭）；S17 **保持 TODO(verify)**（dc:104036 返回值宽度 divergence，移交归属线）；S18 OK |
| dc:107581/108471/108518/110370/110424 调用点 msvc 对应 | **SKIP**（其它 TU 范围外，不影响本函数裁定） |
| src 树改动 / 构建 / 部署 / 测试 | 未触及（遵守禁令） |

**最终裁定：S083（TokenizeExpression 尾段）— 完整无缺，可关闭；无源码落码需求。** 唯一需主代理留意的动作为：若以 draft 为底本复用时先订正 draft:175-177 条件；PART B 的 dc:104036 返回值宽度项挂起移交 CreateMenuItem 归属线。