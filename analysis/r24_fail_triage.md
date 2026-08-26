# R24 全量 FAIL 案分类盘点（2026-08-27 基线 · 19 PASS / 44 FAIL）

> 取证子代理产出 · 单文件独占：本报告
> 构建身份：HEAD hash=f8e7cbc，md5=f9166420，已部署 `C:\pectest`（与 win_real 结果、`C:\pectest\memfail.log` 探针同构建期）
> 输入：`harness\results\win_real\<case>\verdict.json` + `exit.txt`（win_real vs win_real_orig）+ `corpus\cases\*\main.pecmd` + 参考档案
> （`analysis\r23_021_deadpoint_shift.md`、`build\msvc\s17_filecase_triage.md`、`build\msvc\s20_sub_allocav_triage.md`、`docs\divergences.md`、`build\msvc\s13_r1_dualbody_batchA.md`）
> 纪律：只读分析；未改任何 src/restored_bodies.c/unimplemented_stubs.c/include 文件；未构建/未部署/未 run_case；未 git commit；未用 pwsh（本会话 shell 后端即 bash 语法）。

## 0. 总体观察（先于逐案）

1. **44 案全部只有 exit 维度分歧**：`stdout/vars/reg` 均 `same:true`，`fs` 除 `done.txt` 缺失外无差异。vars.txt 是变量**名字**模板回显（run_case.py 尾声字面回显，不带值），故 R/S/V 等变量取值在本夹具中**不可观测**（s17 §2/U-2 沿用）。
2. **11 案 done.txt 缺失**（崩/挂在尾声 EXEC 之前）：002, 004, 038（挂）+ 041, 051, 053, 055, 056, 057, 059, 060（崩溃类）。其余 33 案进程完整跑完、仅退出码不同。
3. **退出码链机制已实证正确（不是本次缺陷所在）**：
   - PSB 尾 `LAB_14004c525`（rb `restored_bodies.c:7177`，写回 `if (local_180.QuadPart != 0) 写 *(script+0x50)` 在 rb:7187-7189）与 dc `decompiled.c:45139-45141` 逐句同构——**只写非零动词返回值**。
   - 探针 `[WB]`（rb:7178-7186 临时探针，写 `C:\pectest\memfail.log`，保留 GetLastError）实测当前构建各动词返回值，与 verdict exit 精确互证（如 012 FILE l180=87=exit、040 LSTR l180=-2147024809→exit 0x80070057、026 CALC l180=指针→exit 低 32 位）。
   - 槽身份细节两个档案有分歧（S17 主张 =g_Script+0x50；S20 §19 更晚实测定案 `DAT_14013d180` 为独立全局指针、指向堆缓存），**不影响本分诊**（两侧结构一致，值一致）。
   - 推论：**44 案的 FAIL 本质 = 动词处理器的返回值错误（含崩溃），不是退出码传播链断裂**。写回链本身已经是"对拍一致"的忠实直移。
4. 原版 golden 的「2」是多数文件/查询族动词的**成功返回值**（WRITE 已实证：dc FUN_14009f070 尾 uVar6 成功路径=2，见 s20 §8；本基线 005-009/044/045 已 PASS exit=2 印证），"成功也=2"是 PECMD 非直观锚点（s17 §5.1）。**但"到底谁把 2 写进缓存"仍存在未闭合问题（见 §4 特别标注-3），部分簇的翻转估值以此为条件**。

---

## 1. 全表（44 案逐行）

退出码字典（先查表）：

| hex | dec | 含义 |
|---|---|---|
| 0x00000000 | 0 | 正常退出（golden 大多数） |
| 0x00000002 | 2 | ERROR_FILE_NOT_FOUND（PECMD 文件/查询族**成功返回值**的 golden 形态） |
| 0x00000001 | 1 | ERROR_INVALID_FUNCTION（msvc 侧 SET/DIR/CODE 返回值） |
| 0x00000010 | 16 | ERROR_BAD_UNIT（golden 027 除 0 错误码；PECMD 自定除 0 码） |
| 0x00000057 | 87 | ERROR_INVALID_PARAMETER（msvc FILE 拷贝失败返回值） |
| 0x000000B7 | 183 | ERROR_ALREADY_EXISTS（msvc LOGS 返回值） |
| 0x0000007C | 124 | **harness 超时约定值**（run_case.py `TimeoutExpired→124`），非 NTSTATUS |
| 0xC0000005 | 3221225477 | STATUS_ACCESS_VIOLATION（数据段/野指针读） |
| 0xC0000374 | 3221226356 | STATUS_HEAP_CORRUPTION（堆校验失败） |
| 0xC0000409 | 3221226505 | STATUS_STACK_BUFFER_OVERRUN / fastfail（GS cookie 上报，果非因） |
| 0x80070057 | 2147942487 | **HRESULT_FROM_WIN32(87)=E_INVALIDARG=STRSAFE_E_INVALID_PARAMETER**（调用方按 64 位返回 0xffffffff80070057，进程退出码取低 32 位） |

| 案 | 脚本要点 | exp | got | done.txt | 探针 l180（WB，现行构建） | 簇 |
|---|---|---|---|---|---|---|
| 002_envi_assign | ENVI A=Hello/B=World/C=%A%_%B% | 0 | 124 (0x7C 超时) | ✗ | A/B 行 l180=0，C 行未到 WB | A |
| 004_envi_global_scope | ENVI+TEAM ENVI A=inner+ENVI B=%A% | 0 | 124 | ✗ | 尾日志停在 `ENVI B=%A%` 展开中（S8 ExpandVR b=6） | A |
| 038_sub_param_shadow | _SUB F,a / TEAM ENVI G=%a% / CALL F,param / ENVI H=%a% | 0 | 124 | ✗ | 日志停在 `ENVI a=global` 后展开段 | A |
| 010_file_copy | FILE %F%=>copy.txt | 2 | 87 (0x57) | ✓ | — | B |
| 011_file_del | FILE %F%<> | 2 | 0 | ✓ | — | B |
| 012_file_missing_src | FILE nofile=>x2.txt | 2 | 87 | ✓ | **FILE l180=87** ✓ | B |
| 013_ifex_file | IFEX <存在>→ENVI R！ELSE | 2 | 0 | ✓ | IFEX l180=0 | C1 |
| 014_ifex_not | IFEX <不存在>+ELSE | 2 | 0 | ✓ | IFEX l180=0 | C1 |
| 016_find_str | FIND $命中 +ELSE | 2 | 0 | ✓ | — | C1 |
| 017_find_not | FIND $不命中 +ELSE | 2 | 0 | ✓ | — | C1 |
| 024_team_multi | TEAM ENVI A=1\|ENVI B=2 | 0 | 0xC0000374 堆损坏 | ✓（收尾期崩） | — | E |
| 025_team_with_if | TEAM ENVI A=1\|IFEX $%A%=1,Envi B | 0 | 0xC0000374 | ✓（收尾期崩） | — | E |
| 026_calc_arith | CALC R=1+2*3 | 0 | 3619699008=0xD7C03940 | ✓ | **l180=0x218D7C03940，低32=0xD7C03940 精确=exit** | D |
| 027_calc_div0 | CALC R=1/0 | 16 (0x10) | 963255408=0x396A1C70 | ✓ | — | D |
| 028_calc_hex | CALC R=0x10+1 | 0 | 261344304=0x0F93CC30 | ✓ | **l180=0x21C0F93CC30，低32=0x0F93CC30 精确=exit** | D |
| 029_lstr_sub | LSTR R=%S%,2,3 | 0 | 0x80070057 | ✓ | LSTR l180=-2147024809（=0xffffffff80070057） | F |
| 031_set_assign | SET A=pecmd + FIND | 0 | 1 | ✓ | **SET l180=1**（verb=00544553） | J |
| 032_calc_paren | CALC R=(1+2)*(3+4) | 0 | 3615197520=0xD77B8950 | ✓ | l180=指针（另一运行 0x24527EAF070） | D |
| 033_calc_div_floor | CALC R=9/2 | 0 | 2167334864=0x812EE7D0 | ✓ | l180=指针（另一运行 0x146877F7CD0） | D |
| 035_find_case | FIND $ 大小写探针 | 2 | 0 | ✓ | — | C1 |
| 036_find_missing_file | FIND --s <不存在> +ELSE | 2 | 0 | ✓ | FIND l180=0（--s 缺失） | C1 |
| 039_team_chain | TEAM CALC X=2*3\|FIND | 0 | 1624954784=0x60DAD7A0 | ✓ | **CALC l180=0x17660DAD7A0，低32=0x60DAD7A0 精确=exit**；FIND-in-TEAM l180=0 | D+E |
| 040_lstr_overrun | LSTR R,%S%,1,99 | 0 | 0x80070057 | ✓ | LSTR l180=-2147024809（越界参数同样返回） | F |
| 041_mdir_file_combo | WRITE+MDIR+FILE=> | 2 | 0xC0000005 AV | ✗ | WRITE l180=2 正常，MDIR/FILE 段崩（未到 WB） | G |
| 043_envi_space_value | FIND $%S%=world again（空格值） | 2 | 0 | ✓ | FIND l180=0 | C1 |
| 046_forx_enum | FORX W,a,b,c + FIND %LAST%=c | 2 | 0 | ✓ | FORX l180=0；FIND l180=0 | C1 借道 |
| 047_fdir_normalize | FDIR FULL,%REL% + FIND | 2 | 0 | ✓ | FDIR l180=0；FIND l180=0 | C1 借道 |
| 048_fext_change | FEXT M,%N%,jpg + FIND | 2 | 0x80070057 | ✓ | FEXT l180=-2147024809 | F |
| 049_rstr_right | RSTR R=%S%,2,3 + FIND | 2 | 0x80070057 | ✓ | RSTR l180=-2147024809 | F |
| 050_rpos_replace | RPOS T,%S%,123,X9 + FIND | 2 | 0x80070057 | ✓ | RPOS l180=-2147024809 | F |
| 051_size_known | WRITE+SIZE Z,%F% + IFEX $ | 2 | 0xC0000005 AV | ✗ | SIZE 行未达 WB（处理体内崩） | G |
| 052_dir_wildcard | WRITE×3 + DIR N,%D%\*.txt + IFEX $ | 2 | 1 | ✓ | — | J |
| 053_hash_probe | WRITE+HASH H,%F% | 2 | 0xC0000374 堆损坏 | ✗ | WRITE l180=2，HASH 行未达 WB | H |
| 054_code_convert | WRITE+CODE %F%=>out + IFEX 文件 | 2 | 1 | ✓ | — | J |
| 055_sed_generate | SED G,sed55seed | 0x80070057（**原版即异常**） | 0xC0000409 fastfail | ✗ | SED 行未达 WB | I |
| 056_temp_exist | TEMP T + IFEX %T% | 2 | 0xC0000374 堆损坏 | ✗ | — | H |
| 057_form_positive | FORM V,C: + IFEX $ | 2 | 0xC0000005 AV | ✗ | FORM 行未达 WB | G |
| 058_recy_volume | RECY V + IFEX %V% | 2 | 0x80070057 | ✓ | RECY l180=-2147024809 | F |
| 059_link_shortcut | WRITE+LINK lnk + IFEX 文件 | 2 | 0xC0000005 AV | ✗ | LINK 行未达 WB | G |
| 060_path_special | PATH P | 0 | 0xC0000005 AV | ✗ | PATH 行未达 WB | G |
| 061_logs_smoke | LOGS log + IFEX 文件 | 0 | 183 (0xB7) | ✓ | **LOGS l180=183**（verb=53474f4c） | K |
| 062_strl_len | STRL L,%S% + FIND %L%=6 | 2 | 0x80070057 | ✓ | STRL l180=-2147024809 | F |
| 063_sstr_search | SSTR P,%S%,cd + FIND %P%=3 | 2 | 0 | ✓ | SSTR l180=0；FIND l180=0 | C1 借道 |
| 064_exec_explicit | EXEC =cmd /c echo>file + IFEX 文件 | 2 | 0 | ✓ | EXEC l180=0；IFEX-file l180=0 | C1 借道 |

> 探针列空白=该段未在本日志窗口截获或命令崩溃未达尾探针；`-2147024809` = 0xffffffff80070057（sign 64），进程退出码取低 32 位 = 0x80070057。

---

## 2. 同源簇分组（含根因假设与候选函数）

### 簇 A — ENVI「%var% 引用展开/文本流扫描」挂死 ×3（002, 004, 038）

- **证据**：三案 exit=124、done.txt 缺失、stdout 同。探针尾日志显示 002/004/038 均挂在 ENVI 引用展开阶段（004 停于 `ENVI B=%A%` 的 S8 ExpandVR 段）；001/003（无 `%var%` 引用）PASS ⇒ 触发面 = **脚本变量引用展开后的文本/分隔符扫描**。
- **假设（沿用 r23）**：`PECMD_ExtractTableSegment`（dc:15775，rb `core_b1_remaining.c` 域，真体 @0x140122950）扫描**文本流**找 0x88/0x8A/0x90 分隔符的自旋死循环；r23 已证死点在扫描环内（0x75→0x9a 漂移），R20C 种子化"只种对象不种文本流"，未终结自旋。004=TEAM 内层展开、038=SUB 克隆链 + 参数遮蔽（`ScriptCopy(FUN_140017CDC)→ScriptInit(FUN_1400186bc dc:13328)` 深拷贝后重入）复合。
- **候选函数**：PECMD_ExtractTableSegment（dc:15775）、PECMD_InitObfuscatedKeywords（core_exec2.c:291-322）、RunScriptText（core_execmain.c:218-229）、ENVI/SET 共用 `PECMD_EvalSpecialToken`（@0x14002d33c，core_b2e.c:1358）、SUB 克隆三连（dc:82268-73）。
- **注**：r23 §4 原计划对 002/004 抓活体栈，未见后续档案完成 ⇒ SKIP(现行构建死点未取证)。

### 簇 B — FILE 动词返回值契约 ×3（010, 011, 012）

- **证据**：010/012 `FILE =>` 失败路径 got=87（0x57 ERROR_INVALID_PARAMETER）vs golden 2；011 `FILE <>` 删除成功 got=0 vs golden 2；探针 012 **l180=87**。
- **假设**：FILE 处理器 `FUN_14003C06C`（psb 直接调用；rb `core_b2f.c:6552`，s17 误名 IndataCopyDirs）在`=>`复制路径上于 **CopyFileW 之前或之后**取到 87 型错误码作为返回值；删除路径成功返 0。原版语义 = 复制/删除成功均返 2、源缺失返 2（s17 双面对拍锚 007/009/012 失败分支与 005/008/010 成功分支）。
- **SKIP**：87 的赋值点未静态定位（需对照 dc:44465 FILE 分支 → FUN_14003c06c 尾部 uVar 链，或按 s17 §6-7 双面对拍设计）。

### 簇 C1 — IFEX/FIND 共享主体 `PECMD_BuildImDiskMenu @0x140034788` 为 SKIP 桩（直接 7 案 + 借道 4 案）

- **直接成员**：013, 014, 016, 017, 035, 036, 043（IFEX/FIND 行）。**探针统一 l180=0**，golden=2。
- **借道成员**：046（尾行 FIND）、047（尾行 FIND）、063（尾行 FIND）、064（IFEX-file 行）——脚本末位查询动词若返回 2 则整案 exit=2。
- **根因（结构性，已坐实）**：PSB 分支 `IFEX=0x58454649→PECMD_IfexCmdHandler`（core_b2a.c:140）与 `FIND=0x444e4946→PECMD_FindCmdHandler`（core_b2a.c:151）皆为 thunk，调用共享主体 `PECMD_BuildImDiskMenu @0x140034788`（dc:31379，4312B，IFEX/FIND/文件探测总闸），而该函数在 `core_b2f.c:3829-3850` 是 **`return (HMENU)0;` 的 SKIP 桩**（注记 [DECOMPILE FAILED]，s17 A4/A5 已登记）。
- **联动警示**：桩返 0 意味着 **IFEX/FIND 的条件分支（ENVI R=… 等）根本没有执行**。当前夹具 vars 不可观测使该缺陷被 exit 单维掩盖（U-2）——修复验收只能覆盖 exit，分支语义正确性需夹具改造（s17 §6-10）才能验证。

### 簇 D — CALC 返回「容器/堆指针」垃圾码 ×5+1（026, 027, 028, 032, 033 + 039 借道 TEAM）

- **证据（探针+exit 双重精确匹配）**：026 l180=0x218D7C03940→exit 低32 0xD7C03940；028 l180=0x21C0F93CC30→0x0F93CC30；039 l180=0x17660DAD7A0→0x60DAD7A0。即**退出码=返回值指针的低 32 位**。
- **根因假设**：`PECMD_CalcExpression`（`core_b3_remaining.c:28361`，dc @0x1400bf358 size=6015）标为**「重构/简化版」**——非忠实直移。成功路径返回格式化结果串指针 `pWVar5` → PSB 尾按非零写入 → exit=指针低 32。原版 dc 返回值语义需逐句对拍（dc pWVar5 赋值点 :90/:190/:546/:577/:588/:607/:610/:647/:720/:752/:755/:819/:825；PSB 两侧 CALC 分支形态一致 dc:44349-44354 ≡ rb:6390-6394，故不是分发差异）。
- **027 除 0**：golden=16（0x10，PECMD 自定除 0 错误码）；msvc got=0x396A1C70 仍为垃圾指针形态 —— 说明我们的除 0 分支也没走"返 16"。
- **039 借道**：TEAM 分支 `PECMD_ParseValueCommand`（rb:6701）直接 `goto LAB_14004c53b` 不写槽；垃圾来自 **TEAM 内层子行 CALC 经内层 PSB 尾写同一 +0x50 槽**（原始为子脚本私有槽，见簇 E）。

### 簇 E — TEAM 子执行上下文（024, 025 崩；039 槽隔离缺失；004 挂兼属 A）

- **证据**：024/025 got=0xC0000374 且 done.txt 存在（**收尾期**堆校验）；039 无崩但内层返回值穿透到根槽（见 D）。
- **根因假设（多源待复核）**：
  - r19d **S-TEAM-1**：`PECMD_ProcessScriptBlock`（rb:5829，dc:43816）把 dc 入口寄存器残留 `unaff_R13D` 声明为未初始化局部直接读取（每次命令执行都经过）——未初始化读。
  - s20 §5：025 曾现 `ExpandEnvVars+0x1a9c 读 NULL+0x18`（TEAM 内 IFEX 展开链，批次3 激活后才暴露）；D-11（ThreadMainLoop 漏置 NULL double-free，**已修**，024 曾是载体）。
  - **槽隔离缺失（本簇新增结构观察）**：S17 §5.1 对照指出 TEAM 内 FIND+ELSE 的 golden=0（子上下文抑制"2"效应）；原版子脚本私有出口槽 `FUN_1400186ac: child+0x50=&child+0x58`（dc:82273 调用）。msvc 039 实证内层 CALC 返回值直接落根槽 ⇒ **TEAM 子执行上下文出口槽隔离未忠实**。
- **候选函数**：PECMD_ProcessScriptBlock（rb:5829/restored_bodies.c TEAM 分支 6701）、PECMD_ParseValueCommand（=TEAM 分发）、PECMD_ExecCmdDispatch、ExpandEnvVars、嵌套执行器 FUN_1400a53e4（dc:103346，含 child→parent errorlevel 回传 dc:103263-269）。
- **SKIP**：现行构建 024/025 崩溃落点未复核（既有 dump 属 17:56 构建期，早于本次 f8e7cbc 基线）。

### 簇 F — 0x80070057（E_INVALIDARG/STRSAFE 参数校验）族 ×7（029, 040, 048, 049, 050, 058, 062）

- **证据**：7 案 exit=2147942487=0x80070057；探针 LSTR/RSTR/RPOS/FEXT/RECY/STRL 六类 **l180 全部 = -2147024809（0xffffffff80070057）**——返回值本身即该 HRESULT，几乎不可能是巧合 ⇒ 共享"参数段/前缀解析"路径的校验失败。
- **根因假设**：各 handler 参数解析后走「非法参数」分支返回 `{Lo=0x80070057, Hi=0xffffffff}`（源码中该常量大量出现：core_b3_remaining.c:1100/1106/1192、core_b3r_h3.c:1497、core_b2d.c:1340、core_b2f.c:5695/7823、core_b3m.c:478、core_b3r_h4.c:689 等；且 r19d 提到 055 SED 原版 golden 同为 0x80070057 ⇒ **原版 dc 原文也有该哨兵语义**）。即实现体在**输入解析前置**就与 DC 分叉（如参数顺序/前缀/分隔符/变量槽层数），合法输入被判非法。
- **handler 映射**：LSTR→`PECMD_WriteVarTruncated`（rb:6836）、RSTR→`PECMD_SetVarRange`（rb:6879）、RPOS→`PECMD_VarSearchReplace`（rb:6901 区）、STRL→`PECMD_WriteVarEncoded`（rb:6692）、FEXT→`PECMD_SetFileNameExtension`（rb:7109）、RECY→`PECMD_QueryRecycleBinVolume`（rb:7165）。
- **SKIP**：7 个 handler 各自在哪个解析点判非法未逐一定位（需 dc 逐函数对拍各自参数段）。

### 簇 G — AV（0xC0000005）崩溃 ×5（041, 051, 057, 059, 060）

- **证据**：全部 done.txt 缺失、处理体未达尾探针。
  - 041：WRITE l180=2 正常后崩（MDIR 或 FILE 段）。s17/s20 历史：MDIR 分支曾传 `local_res8=NULL` → `PECMD_CreateDirectory`（rb:6873，dc:44827-44828）→ CreateDirectoryTree 首解引用 AV（dump 17448）。
  - 051：`SIZE→PECMD_GetPathSizeToVar`（rb:6367-6370）。
  - 057：`FORM→PECMD_GetDiskFreeSpace`（rb:6741-6744）；r19d 有「GetDiskFreeSpace NULL 解引用」dump（未命名归属，疑即本案）。
  - 059：`LINK→PECMD_LinkCreateShortcut`（rb:6634-6636）。
  - 060：`PATH→PECMD_DispatchSpecialDirective`（rb:6576-6579）。
- **假设**：各处理体内野指针解引用（未初始化局部/参数槽、路径/对象构造差一层）——与 D-05 类缺陷同形态但落点各异；逐一需要现行构建 dump 定位。
- **SKIP**：现行构建 5 案 AV 落点均未复核（旧 dump 时代差）。

### 簇 H — 堆损坏（0xC0000374）×2（053 HASH, 056 TEMP）

- 053：`HASH→PECMD_HashCmdCompute`（rb:6573）；S-HASH-1（哈希算法名匹配 FUN_14005c7c4 双名分裂连 0 桩）**已修**（unimplemented_stubs.c:539-543 转发真体 core_exec5.c:156），但 053 仍崩 ⇒ 崩溃点转移/另在他处，未复核。
- 056：`TEMP→PECMD_TempPathCommand`（core_b3r_h2.c:519）；r19d §1.2：`TempPathCommand+0xd5b` HeapFree 串槽崩；S-TEMP-1（PECMD_CreateTempMutexDir wsprintfW 变参丢弃）已由 R23 PATCH-1 修复（src/misc/core_b1_remaining.c:3621-3635 五参实证）——**固定后的崩溃属性待复核**。

### 簇 I — 055 SED fastfail（0xC0000409）×1

- r19d §1.1 完整解剖：第一现场 `FUN_140003a20+0x31` 读 `script->0xDA` 时 AV（script 已为垃圾指针）→ 该帧 GS cookie 已被踩 → `__report_gsfailure(int 29h)` 兜底上报。即 **c0000409 是先有指针踩踏、cookie 失败是果**。上游 = `PECMD_GenerateTextContent`（SED 处理器，core_b3r_h1.c）+ `PECMD_RunCommandLine` 链。
- 注意 golden=0x80070057：**原版 SED G 也"异常"退出**（参数非法 HRESULT），msvc 目标应是 0x80070057 而非 0——修复判据是"不崩且退 0x80070057"。

### 簇 J — exit=1 ×3（031 SET, 052 DIR, 054 CODE）

- 031：PSB `SET=0x544553` 分支（rb:6239-6246 ≡ dc:44222-44228）进 `PECMD_EvalSpecialToken(mode=1)`，**探针 l180=1**；mode=1（SET）路径返回 1 而 golden 0。
- 052：`DIR→PECMD_DirWildcardExpand`（rb:6234-6236）。
- 054：`CODE→PECMD_CodeConvertCommand`（core_cmd4.c:93）；文件头自注返回语义 **1=参数/读文件错误** ⇒ 054 实为 `CODE %F%=>…` 输入/参数解析失败（可能 `=>` 目标语法未解析），对应 golden 2。
- 三者返回值恰巧同为 ERROR_INVALID_FUNCTION(1)，但**handler 互不相干**（解答见 §4-1）。

### 簇 K — 061 LOGS exit=183 ×1

- **探针铁证：LOGS l180=183**（verb=53474f4c；rb 分发 rb:6895-6898 → `PECMD_ScriptInitParse` 名疑误标）。golden=0。
- 历史 r19b 的"183=探针 fopen 污染 LastError"结论**已不适用于本构建**（WRITE 返回已归正为 2，探针侧同一 fopen 存在却未再产出 183）——现 183 直接从 LOGS 处理器返回。
- **疑点**：061 的 golden=0 与 013/014（IFEX-file 返回 2）形态冲突（见 §4-3），修复判据需先解"原版 061 为何 0"。

---

## 3. ROI 排序（修复预期翻转案数 × 难度 → 下轮工单序）

| 序 | 工单 | 预期翻转 | 难度 | 依据/风险 |
|---|---|---|---|---|
| **0** | **前置研究：活体取证"原版把 2 写进退出缓存"的指令**（s17 §6-1：对原版 005/013/016/031/061 各跑一次，硬件写断点缓存槽） | 解锁 C1/B/D/F 全部估值 | 低（只读取证） | U-1 未闭合；决定 C1 去桩后返回值应写 2 还是写 0 |
| 1 | C1：`PECMD_BuildImDiskMenu @0x140034788`（core_b2f.c:3829）4312B 去桩/双体对译（s13 方法），**返回值契约=2** | **11~16**（直接 7 + 借道 4 + F 簇中带 FIND/IFEX 尾行 5 案掩蔽 048/049/050/058/062） | 高（大函数） | 翻转依赖工单0 的"2 从哪来"结论；掩盖 5 案底层动词参数错误（F 簇） |
| 2 | D：`PECMD_CalcExpression`（core_b3_remaining.c:28361）返回值语义对拍 dc FUN_1400bf358（成功=0/串指针归位，除 0=16） | 6（026,027,028,032,033,039） | 中高 | "重构/简化版"需重做忠实直移或至少归正返回值 |
| 3 | G：041/051/057/059/060 现行构建 dump 定位 5 个 AV 落点 | 5 | 中高 | 需允许运行 EXE/开 dump；每案独立 |
| 4 | H：053/056 现行构建复核堆损坏落点（校验 R23 修复是否已在被测产物生效） | 2 | 中高 | 同 3 |
| 5 | F：029/040/048/049/050/058/062 参数解析前置对拍（每 handler 一个解析点） | 2（029,040 无尾行查询动词，不可被工单1掩蔽）~7 | 高（7 函数） | 048/049/050/058/062 可先被工单1掩蔽翻转 |
| 6 | E：024/025 TEAM 收尾堆损坏 + TEAM 子上下文出口槽隔离（039 佐证） | 2 | 高 | 需活体/新 dump |
| 7 | A：002/004/038 现行构建挂死点取证（r23 §4 遗留动作） | 3 | 高 | 需活体 attach；r23 死点在前一构建 |
| 8 | J+K：031（EvalSpecialToken mode=1）、052（DirWildcardExpand）、054（CodeConvertCommand 参数解析）、061（LOGS 返回 183） | 4 | 中 | 逐一 dc 对拍返回值 |
| 9 | I：055 SED→GenerateTextContent 链指针踩踏 | 1 | 高 | golden 本身=0x80070057，"不崩"即可翻转 |

> 难度口径：低=静态对照可修；中=单函数对拍；高=大函数恢复或需活体/dump 取证。
> 严格执行"翻转=exit 与 golden 一致"，done.txt 缺失的崩案必须进程能跑完才计翻。

---

## 4. 特别标注（任务指定两问 + 一个必须声明的未闭合点）

### 4-1 exit=1 族（031/052/054）与 061(183) 是否同一"动词返回值链"根因？

**答：链条同源，缺陷不同源——不存在一个共享补丁能同时翻转 4 案。**
- 链条侧：四条都经同一条**已验证忠实**的通道（PSB 尾非零写回 rb:7177-7180 ≡ dc:45139-45141；探针直接量到 l180=1/1/1/183），通道无需修。
- 缺陷侧：4 个互不相干的处理器各自返回错误常量——`PECMD_EvalSpecialToken`（SET, mode=1）返 1、`PECMD_DirWildcardExpand`（DIR）返 1、`PECMD_CodeConvertCommand`（CODE，头注 1=参数/读文件错误）返 1、LOGS 处理器返 183。修复必须逐个对拍 dc 尾部返回语义。
- 附加耦合：031 脚本含 FIND 行，FIND 桩当前返 0；若工单1 使 FIND 返 2，031 的 exit 会变 2——**golden=0**，会引入新 FAIL（除非按工单0 澄清原版该处为何 0）。031/061 是 C1 修复的"反向风险案"，必须纳入工单1 回归面。

### 4-2 CALC / TEAM / LSTR 三"崩溃簇"是否同源（如 HeapFree 契约/未初始化读）？

**答：三簇互不同源，且其中两簇并非真正崩溃。**
| 簇 | 形态 | 铁证 | 根因指向 |
|---|---|---|---|
| CALC（026/027/028/032/033/039） | **非崩溃**，进程完整跑完，exit=返回值指针低 32 | 探针 l180=0x218D7C03940…，exit 低 32 逐位吻合 | `PECMD_CalcExpression` "重构/简化版"返回值语义错（成功返结果串指针） |
| LSTR 族（029/040/048/049/050/058/062） | **非崩溃**，exit=0x80070057 | 探针 l180=-2147024809 | 参数/前缀解析前置分叉 → 非法参数 HRESULT 哨兵（dc 亦有此哨兵） |
| TEAM（024/025） | 真崩溃 0xC0000374（收尾期堆校验） | done.txt 存在；r19d/s20 多源 | 未初始化读（S-TEAM-1）/ ExpandEnvVars 展开链 / 广播线程竞态（D-11 已修项）——现行落点未复核 |
- 与 **HeapFree 契约（D-09 GrowByteBuffer 头契约，已修 v3）** 无现行同源证据；三簇的唯一共同点只是"全部经非零写回通道显形"。结论：**分簇分治，勿按单一根因处理**。

### 4-3 必须声明的未闭合点（U-1 状态）：原版 golden「2」的来源指令仍未定位

- S17 静态穷尽（.text 全量 RIP-rel/imm64 扫描）只找到 init/读侧 3 处直接引用；写侧必经寄存器中转。S20 §19 活体只观测到「缓存值=2(WRITE 成功码)」的**结果**，未拿到写指令归属。
- 本基线新增实证反而**收窄**了模型：探针量到 WRITE l180=2（=dc:100374 uVar6 锚）且 005-009/044/045 已 PASS ⇒ "2"在 msvc 侧已能由 WRITE 产生；而 **IFEX/FIND/FORX/FDIR/SSTR 在 msvc 全返 0、原版 golden 却=2（013/014/016/017/035/036/043/046/047/063/064）**，且 TEAM 内同形态返 0（039）——"2 的写入者"在原版中到底是谁仍未闭合。
- **031/061 的反例**（同含 IFEX/FIND 形但 golden=0）进一步说明不是"IFEX/FIND 恒返 2"这么简单。**任何 C1/F 簇的翻转估值都以工单0 为前提**，本报告估值已按该前提标注。

---

## 5. SKIP / 未验证登记（不虚构语义）

| # | 事项 | 原因 | 解锁 |
|---|---|---|---|
| U-1 | 原版"2"写入指令归属 | 静态穷尽仍留运行时寄存器中转盲区（S17 §8） | 工单0 动态断点 |
| U-2 | R/S/V 等变量取值双方是否一致 | 夹具 vars.txt 为名字模板回显（run_case.py:82-111） | s17 §6-10 夹具改造 |
| U-3 | A/G/H/E 簇现行构建（f8e7cbc）的崩/挂落点 | 允许操作禁止运行 EXE/attach；既有 dump 为 17:56 构建期（r19d），早于本基线 | 解除运行令后逐案 dump |
| U-4 | F 簇 7 案各自参数解析分歧点 | 未逐函数对拍 dc | 工单5 |
| U-5 | 061 LOGS golden=0 与 013/014（IFEX-file=2）的语义冲突 | 静态不可判定；需原版活体同点观测 LOGS/IFEX 返回 | 工单0 扩展 |
| U-6 | 052/054 `DIR`/`CODE` 行未截获 WB 探针（052 可能走 ECD 兜底路径，054 有头注语义） | 日志窗口截获不全 | run 时探针复查 |

---

## 6. 交付结论（结构化简报）

**簇清单（44 案 → 11 簇）**：
- 挂死 A×3（002,004,038）· FILE 契约 B×3（010,011,012）· IFEX/FIND 桩 C1×11（013,014,016,017,035,036,043 直接 + 046,047,063,064 借道）· CALC 返回值 D×6（026,027,028,032,033,039）· TEAM 上下文 E×2（024,025）· 0x80070057 参数族 F×7（029,040,048,049,050,058,062）· AV 崩溃 G×5（041,051,057,059,060）· 堆损坏 H×2（053,056）· SED fastfail I×1（055）· exit=1 J×3（031,052,054）· LOGS-183 K×1（061）

**推荐下轮工单序（ROI 排序）**：
**工单0（前置，低难度高杠杆）→ 工单1 C1 去桩（11~16 案）→ 工单2 CALC 归正（6 案）→ 工单3 G 崩溃 dump（5 案）→ 工单4 H 复核（2 案）→ 工单5 F 参数族（2~7 案）→ 工单6 E TEAM（2 案）→ 工单7 A 挂死取证（3 案）→ 工单8 J+K（4 案）→ 工单9 I（1 案）**

**核心一句话**：44 案 FAIL 无一例是退出码传播链断裂（链为忠实直移且探针逐案实证）——全部是**动词处理器返回值错误（含 15 个崩/挂案）**；最高杠杆单点是 IFEX/FIND 共享主体 4312B 桩（C1，最多翻转 16 案），但其翻转以"原版 2 的写入者"（U-1/工单0）为前置条件；CALC/TEAM/LSTR 三簇已铁证互不同源（两簇非崩溃），exit=1 族与 061 同链不同因。