# R24f F 簇（0x80070057 参数族 ×7 案）取证分析

> 只读取证档案 · 本文件为工作档案（任务唯一允许的写盘文件）
> 取证主体：029/040(LSTR) 048(FEXT) 049(RSTR) 050(RPOS) 058(RECY) 062(STRL)
> 构建身份：**部署件 `C:\pectest\pecmd_msvc.exe` = DEPLOYED_BUILD hash=1c533d6 md5=304eea38（2026-08-27 03:04 部署）**，
> 含 R24c「C1 IFEX/FIND 去桩」提交（git log f8e7cbc..1c533d6 含 `ecfa8a1 R24c C1 de-stub`）。
> 纪律：未修改任何 src / restored_bodies.c / 桩文件 / build 产物；未构建；未 git 操作；
> 运行时取证仅 `python harness/runners/run_case.py <case> --exe msvc --timeout 240`（用户授权，只读运行现有 EXE，不部署新构建）。
> 探针证据源：`C:\pectest\memfail.log`（[PSB]/[WB] 探针，与 065 收尾日志同模块基址 7FF79E71A178 ⇒ 同 1c533d6 进程族）。

---

## 0. 结论速览（先看这里）

| 案 | 动词 | 输入（%VAR% 展开后） | 现行构建 exit | handler 返回 | 真分叉? | 根因定位 |
|---|---|---|---|---|---|---|
| 029_lstr_sub | LSTR | `R=abcdef,2,3` | **0x80070057 FAIL** | -2147024809 | **是** | `PECMD_SkipWCharUntil` 为 no-op 桩 |
| 040_lstr_overrun | LSTR | `R=abc,1,99` | **0x80070057 FAIL** | -2147024809 | **是** | 同上（同函数同桩） |
| 049_rstr_right | RSTR | `R=abcdef,2,3` | 2 PASS（掩蔽） | -2147024809 | **是** | 同上（同函数同桩） |
| 048_fext_change | FEXT | `M,s48pic.old,jpg` | 2 PASS（掩蔽） | -2147024809 | 否 | 用例无 `=`，**原版同样返哨兵** |
| 050_rpos_replace | RPOS | `T,abc123abc,123,X9` | 2 PASS（掩蔽） | -2147024809 | 否 | 用例无 `=`，**原版同样返哨兵** |
| 058_recy_volume | RECY | `V` | 2 PASS（掩蔽） | -2147024809 | 否 | 用例形态不合，**原版同样返哨兵** |
| 062_strl_len | STRL | `L,abcdef` | 2 PASS（掩蔽） | -2147024809 | 否 | 用例无 `=`，**原版同样返哨兵** |

**一句话根因**：F 簇 7 案中，**只有 LSTR/RSTR 三案是真实 msvc 分叉**，共享根因 = 分词前置 helper
`PECMD_SkipWCharUntil`（≡dc `FUN_1400f429c`，thunk 别名 `thunk_FUN_1400f429c`）在
`unimplemented_stubs.c:280` 为 **no-op 空桩（return 0，不进位指针）**，导致 LSTR/RSTR 的
「`=` 后扫描到 `,` 再判型」第二道闸永远失败 → 走 0xffffffff80070057 哨兵分支。
其余 4 案的 0x80070057 是**原版真实语义**（dc 同分支同返回），corpus 用例自身用了原版动词
不接受的参数形态（逗号/模式字母、无 `=`）；golden exit=2 完全来自尾行 FIND/IFEX 的「2 掩蔽」
（f8e7cbc 上因 FIND/IFEX 桩返 0 而显形为 FAIL，R24c 去桩后 exit 维已翻转，问题只是被盖住）。

> 每案一节的「dc 真值」均为 `reference/decompiled.c`（唯一语义真值）逐行对拍结论，行号见各节。

---

## 1. 公共前置：handler 收到的输入是什么（dispatch 层对拍）

六个 handler 的调用点（PSB 动词表）msvc ↔ dc 逐行同构，**发送给 handler 的尾部指针及内容完全一致**
（复核 rb 行号 vs dc 行号）：

| 动词 | dword(LE) | msvc restored_bodies.c | dc decompiled.c | 尾部槽 |
|---|---|---|---|---|
| LSTR | 0x5254534c | rb:6837 | dc:44796 | `LVar11+8`（%VAR% 展开副本） |
| RSTR | 0x52545352 | rb:6880 | dc:44839 | `LVar11+8` |
| STRL | 0x4c525453 | rb:6693 | dc:44652 | `LVar11+8` |
| RPOS | 0x534f5052 | rb:6923 | dc:44883 | `LVar25+8`（原行） |
| FEXT | 0x54584546 | rb:7110 | dc:45070 | `LVar11+8` |
| RECY | 0x59434552 | rb:7166 | dc:45127 | `LVar11+8` |

结论：**「变量槽层数 / 参数顺序分发」分叉假设（R24 triage 簇 F 猜测之一）被排除**——
探针 `[PSB]` 显示原始行、`[WB]` 显示展开行均与原版等价（见各案证据段）。

---

## 2. 逐案取证

### 2.1 029_lstr_sub —— LSTR `R=abcdef,2,3`（真分叉）

- **输入**（`main.pecmd`）：`ENVI S=abcdef` + `LSTR R=%S%,2,3`
- **msvc 行为**：exit=2147942487（0x80070057）；探针 `[WB] verb=5254534c l180=-2147024809 line=[LSTR R]`
  （`line=[LSTR R]` 的 NUL 落在 `=` 处——这是 dc 原文也在做的**就地置零**（dc:100331），本身不是分叉证据）
- **dc 真值**（FUN_1400a0644 @dc:100301，size=511，msvc 直移体 h3:274-348 逐行同构）：
  - dc:100327 `FUN_1400675b8`（SplitTokenTrimWs 按 `=`）→ `*src` 停在 `=`；
  - dc:100330-332 判 `=` → 就地置零、进位 → `abcdef,2,3`；
  - dc:100336 `thunk_FUN_1400f429c(&src, ',')`（**真实实现**，见 §3）→ `*src` 停在 `,`；
  - dc:100337 判 `,` → 通过 → 数值交换/ParseHexOrDecBool/clamp → `SetVariable` → 返回 **0**；
  - golden=0（029 无尾行查询动词，exit 即 LSTR 返回值）→ **原版成功、返 0** ✓
- **分叉点**：msvc h3:308 `PECMD_SkipWCharUntil` = **空桩**（不移指针）→ h3:309 判 `*src==','` 时
  `*src` 仍为 `'a'` → 第二道闸失败 → h3:343 `uVar6 = 0xffffffff80070057ULL`。
- **根因一句话**：参数解析第二闸依赖的分隔符推进 helper 未实现。
- **候选修法（最小改动点）**：在 `unimplemented_stubs.c:280` 把 `PECMD_SkipWCharUntil` 由 no-op 改为
  dc `FUN_1400f429c`（decompiled.c:149819，38 字节）的直移真体：
  `p=*pp; while(*p && *p!=ch) p++; *pp=p; return pp;`。
  回归面：LSTR/RSTR/029/040/049 三案 + 全库 ~30 处调用点（至少 h3:584 SearchStringAndLocate、
  h4:243 RPOS 键槽扫描、g 系/i28 系分隔符扫描）。

### 2.2 040_lstr_overrun —— LSTR `R=abc,1,99`（真分叉，同 029）

- **输入**：`ENVI S=abc` + `LSTR R=%S%,1,99`（开场即越界形态：start=1、len=99）
- **msvc 行为**：exit=0x80070057；`[WB] l180=-2147024809 line=[LSTR R]`（两次 LSTR 探针 = 029+040）
- **dc 真值**：与 2.1 同一函数同一路径——`=` 闸过、`,` 闸过（真 SkipWCharUntil）→ 成功、返 0；golden=0 ✓
- **分叉点/根因/修法**：与 029 完全相同（同函数、同桩）。无独立修法。

### 2.3 049_rstr_right —— RSTR `R=abcdef,2,3`（真分叉，同桩）

- **输入**：`ENVI S=abcdef` + `RSTR R=%S%,2,3` + 尾行 `FIND $%R%=def,...`
- **msvc 行为**：`[WB] verb=52545352 l180=-2147024809 line=[RSTR R]`；exit=2（**尾行 FIND 现返 2 掩蔽**）
- **dc 真值**（FUN_1400a0844 @dc:100382，msvc h3:354-427 同构）：
  dc:100416 `thunk_FUN_1400f429c` 后 dc:100417 判 `,` 通过 → `local_res10 += (iVar2 - len)`（取右段）→
  `SetVariable` → 返 **0**。原版成功。
- **分叉点**：msvc h3:387 `PECMD_SkipWCharUntil` 桩 → h3:388 判 `,` 失败 → h3:422 哨兵。
- **根因/修法**：同 2.1。注意本案 exit 维已被 C1 掩蔽为 PASS，**修复验收必须叠加「FIND 尾行撤销」或读探针**
  才可见；翻转 049 需先解除掩蔽（或接受 exit=2 已 PASS 的事实，仅修语义）。

### 2.4 048_fext_change —— FEXT `M,s48pic.old,jpg`（非分叉：用例形态与原版契约不符）

- **输入**：`FEXT M,%N%,jpg`（N=s48pic.old）→ 展开 `M,s48pic.old,jpg`
- **msvc 行为**：`[WB] verb=54584546 l180=-2147024809 line=[FEXT M,s48pic.old,jpg]`（**行完整，无就地截断**——
  因为全程没有任何 `=` 被置零，佐证"无 `=` 即死"路径）；exit=2（尾行 FIND 掩蔽）
- **dc 真值**（FUN_140096748 @dc:94732，size=312；msvc core_b3m.c:1326-1373 逐行同构）：
  - dc:94749 SplitTokenTrimWs 按 `=`：`M,s48pic.old,jpg` 无 `=` → 整串进 key，`*src` 停在 NUL；
  - dc:94751 `if (*local_res10 == 0x3d)` 假 → **跳过值槽填充**（local_18 保持 0x14 空缓冲区）;
  - dc:94756 `iVar4 = -0x7ff8ffa9`（=0xffffffff80070057）默认；
  - dc:94757 成功闸 `(*local_res20 != 0) && (*local_18 != 0)`——local_18 首字为空 ⇒ 假 → **返回哨兵**。
  ⇒ **原版对 `M,s48pic.old,jpg` 同样返回 0xffffffff80070057**，且把整串当作变量名写入
  空扩展（`FUN_1400629b8(key=整串, "")`）。
- **分叉点**：无（msvc ≡ dc）。golden exit=2 是 `FIND $%M%=s48pic.jpg…` 尾行的掩蔽产物
  （FIND 在 C1 去桩后返 2；f8e7cbc 时代因 FIND 桩返 0 而显形）。
- **根因一句话**：corpus 用例用了逗号三字段形态，而原版 FEXT 只认 `V=` 形态；哨兵是真实语义。
- **候选修法（最小改动点）**：改用例为原版合法形态（如 `FEXT E=%N%.jpg` 一类 `V=` 形式，按
  FUN_140096748 语义即"取路径扩展名写入变量 E"；M 模式在原版该函数中**根本不存在**）；
  **不改 C 源**。若坚持 comma 形态需要先在原版 PDB/反汇编确认存在别的 FEXT 预处理器（dc 未发现）。

### 2.5 050_rpos_replace —— RPOS `T,abc123abc,123,X9`（非分叉）

- **输入**：`RPOS T,%S%,123,X9`（S=abc123abc）→ `T,abc123abc,123,X9`（无 `=`）
- **msvc 行为**：`[WB] verb=534f5052 l180=-2147024809 line=[RPOS T,abc123abc,123,X9]`（行完整）；exit=2（FIND 掩蔽）
- **dc 真值**（FUN_1400a1938 @dc:101066，size=2645；msvc core_b3r_h4.c:165 起逐行同构）：
  - dc:101141 SplitTokenTrimWs 按 `=` → 整串为 key，`*src` 停 NUL；
  - dc:101188 `if (*local_res10 != L'=')` —— `*src` 为 NUL ≠ `=` → **goto LAB_1400a1bd9 →
    ppWVar16 = 0xffffffff80070057**（dc:101190）。
  ⇒ **原版同样返回哨兵**。
- **分叉点**：无（msvc h4:291-295 ≡ dc:101188-101191；即便 msvc h4:243 的 `PECMD_SkipWCharUntil` 是桩，
  也只是把 key 副本首字符置零——对本输入不影响 `=` 闸结果，两侧都死在哨兵）。
- **附加发现（潜在独立缺陷）**：**RPOS 的合法 `=` 形态（如 `T=abc123abc,123,X9`）在 msvc 会被桩毁掉**——
  h4:243 的 SkipWCharUntil 桩把 local_78（key 副本）留在首字符，h4:244 `*local_78 != 0` 为真 →
  h4:245 就地置零 key 首字符 → 键槽语义损坏。修桩后必须回归 RPOS `=` 形态。
- **候选修法**：用例改 `=` 形态；源侧仅需随 §3 修桩（该桩同时修复 RPOS `=` 形态的键槽损坏）。

### 2.6 058_recy_volume —— RECY `V`（非分叉）

- **输入**：`RECY V` + `IFEX %V%,...`
- **msvc 行为**：`[WB] verb=59434552 l180=-2147024809 line=[RECY V]`；exit=2（IFEX 掩蔽）
- **dc 真值**（FUN_14007cc1c @dc:79401，size=774；msvc core_b3r_g7.c:286-321 同构）：
  - dc:79433 `FUN_1400547bc(&src,&out, ',', 0)`（SplitNextToken，直移等价 msvc core_b3i.c:219+
    CopyTokenTrimmed core_b3j.c:519，已验证与 dc FUN_14006764c 同构）：`V` 无 `,` → out="V"、`*src` 停 NUL；
  - dc:79434 `if (*local_res10 == 0)` → **真** → dc:79438 `lVar12 = -0x7ff8ffa9`（哨兵）。
  ⇒ **原版对 `RECY V` 同样返回哨兵**（RECY 的 `,` 形态第二段是容量数值，`V` 单字段不构成合法参数）。
- **分叉点**：无。golden exit=2 = `IFEX %V%` 尾行掩蔽（%V% 未设置 → ELSE 分支 → IFEX 返 2）。
- **附加发现**：RECY 第二闸 `PECMD_ParseHexOrDec`（g7.c:318，dc FUN_1400c1194）在
  `unimplemented_stubs.c:200` 是 **0 参桩恒返 1**（形参 ABI 不符）——本输入被第一子句短路不受影响，
  但 `RECY C:` 类带容量参数形态的解析会错。建议随修。
- **候选修法**：源侧修桩（SkipWCharUntil 不涉及本案；可选把 ParseHexOrDec 桩归正为 2 参转发真体）；
  用例侧若想测"真 RECY"，需按原版契约写成 `RECY <盘符>` 或 `RECY V=…` 形态并接受环境依赖注记。

### 2.7 062_strl_len —— STRL `L,abcdef`（非分叉）

- **输入**：`ENVI S=abcdef` + `STRL L,%S%` → `L,abcdef`（无 `=`）
- **msvc 行为**：`[WB] verb=4c525453 l180=-2147024809 line=[STRL L,abcdef]`（行完整）；exit=2（FIND 掩蔽）
- **dc 真值**（FUN_1400a03ac @dc:100216，size=663；msvc h3:186-268 同构）：
  - dc:100244 `FUN_1400547bc(&src,&out, '=', 0)`（SplitNextToken）：`L,abcdef` 无 `=` →
    out="L,abcdef"、`*src` 停 NUL、返回指针指向 NUL；
  - dc:100245 `if ((*out == 0) || (*psVar5 != 0x3d))` → `*psVar5==0 ≠ '='` → **真** → dc:100246 哨兵。
  ⇒ **原版对 `L,abcdef` 同样返回 0xffffffff80070057**（STRL 只认 `V=` 形态，`V,串` 不合法）。
- **分叉点**：无。golden exit=2 = `FIND $%L%=6` 尾行掩蔽（%L% 未设置 → ELSE → FIND 返 2）。
- **候选修法**：用例改 `STRL L=%S%`（`=` 形态，dc 成功路径经 SplitNextToken：key="L"、值="abcdef"、
  WideCharToMultiByte 计数 → SetVariable(L, 长度)）；**不改 C 源**。

---

## 3. 共性根因（汇总）

1. **真分叉（029/040/049，3 个 FAIL-able 用例，同根）**：
   `PECMD_SkipWCharUntil` ≡ dc `FUN_1400f429c`（decompiled.c:149816，size=38；
   thunk 别名 `thunk_FUN_1400f429c`，本库 ~119 处 dc 调用点）在
   `unimplemented_stubs.c:280` 为 **直空桩**（`(void)pp;(void)ch; return 0;`）。
   dc 真体语义（dc:149824-149831）：`while (*p && *p != ch) p++;` 并把 `*pp` 推进到**停在分隔符上**；
   LSTR/RSTR 的 `=` 后第二闸（`*src==','`）正是吃这个"停在 `,`"的契约。
   桩不移指针 ⇒ 第二闸恒失败 ⇒ 走 `0xffffffff80070057` 哨兵分支。
   **这是 triage「共享参数解析前置 helper 缺 X」的精确 X。**
   影响面不止 3 案：凡依赖 thunk_FUN_1400f429c 的逗号/分隔符扫描（~30 个 msvc 调用点，含
   h3:584 SearchStringAndLocate、h4:243 RPOS、g/i28 系）目前全部空转或误判。

2. **非分叉（048/050/058/062，4 案）**：0x80070057 是**原版真实语义**（各 handler 的
   `-0x7ff8ffa9` 哨兵在 dc 原文无条件可达）；corpus 输入形态（逗号多字段/单字段模式字母、无 `=`）
   与原版 `V=` 参数契约不符。golden exit=2 是尾行 FIND/IFEX 的「2」掩蔽：
   - f8e7cbc（R24 triage 基线）上 FIND/IFEX=桩返 0 ⇒ 掩蔽失效 ⇒ 显形 FAIL；
   - 1c533d6（R24c，C1 去桩）起 FIND/IFEX 真体返 2 ⇒ exit 维已 PASS，handler 哨兵被盖住（探针仍可见）。
   → 修复方向是 **corpus 用例形态修正**（改 `=` 形态），而非 C 源；不修也只是"掩蔽态"，验收必须解除掩蔽。

3. **联动警示**：任何 LSTR/RSTR 语义回归（§3-1）都会让 RPOS 合法 `=` 形态的键槽损坏显形（h4:245 就地置零），
   且不能依赖 exit 维验收（049/062 等均被 FIND 掩蔽）——需配合探针/夹具改造（vars.txt 值回显，s17 §6-10）。
   另登记：`PECMD_ParseHexOrDec` 0 参桩（stubs:200）影响 RECY 带参形态。

4. **排除项**：dispatch 层（PSB 动词表、LVar11/LVar25 尾部槽、%VAR% 展开）msvc ≡ dc（逐行同构，
   探针 `[PSB]/[WB]` 实证），triage「变量槽层数/参数顺序分发分叉」假设**不成立**；
   SplitTokenTrimWs / SplitNextToken / CopyTokenTrimmed / SkipLeadingControlChars(FUN_14005b154) /
   MatchTokenAdvance / StrDupAssign(FUN_1400702b0) / RunCommandLine(FUN_140003a20) /
   AllocWStringBuffer(FUN_140063694，零初始化) 均已逐一对拍 dc，无差异。

---

## 4. 证据清单与身份绑定

| 证据 | 位置 | 绑定的构建 |
|---|---|---|
| DEPLOYED_BUILD 身份 | `C:\pectest\DEPLOYED_BUILD.txt` | 1c533d6 / 304eea38 / 03:04 |
| 探针 [PSB]/[WB]（6 动词 l180=-2147024809；%VAR% 展开行形态） | `C:\pectest\memfail.log` | 1c533d6（与 065 同基址 7FF79E71A178） |
| msvc 现行 exit 复跑（029/040=0x80070057；其余=2） | `harness/results/win_real/{029,040,048,049,050,058,062}`（run_case.py --exe msvc, 2026-08-27 复跑覆盖） | 1c533d6 |
| golden（原版录制） | `harness/golden/win_real/{029…062}/exit.txt` | 原版 PECMD.EXE（录制 08-25，见 §0 表） |
| dc 真值各函数 | `reference/decompiled.c` FUN_1400a0644/0844/03ac/1938/096748/07cc1c/0547bc/0675b8/06764c/0f429c | —（静态真值） |
| msvc handler 直移体 | `src/commands/core_b3r_h3.c`(186-427) `core_b3m.c`(1326-1373) `core_b3r_h4.c`(165-295) `src/device/core_b3r_g7.c`(286-321) | — |
| 空桩 | `unimplemented_stubs.c:280`（SkipWCharUntil）、`:200`（ParseHexOrDec） | — |

## 5. 下轮修复工单建议（最小集）

1. **P0**：`PECMD_SkipWCharUntil` 去桩——按 dc:149819 直移 38B 真体（唯一代码改动）。
2. **P0**：验收面：029/040 直接翻 0x80070057→0；049 需解掩蔽（暂挂尾行 FIND 或加值观测）后核对 R 值；
   RPOS/SSTR 族补 `=` 形态回归。
3. **P1**：corpus 4 案（048/050/058/062）改 `=`/合法形态（不改源）；若坚持原形态，判定登记为
   "PU（corpus 设计缺陷 + 原版哨兵语义）"而非 msvc 缺陷。
4. **P2（顺手）**：`PECMD_ParseHexOrDec` 0 参桩 → 2 参转发真体 FUN_1400c1194（RECY 带参形态）。