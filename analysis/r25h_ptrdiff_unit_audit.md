# R25-h · 指针差长度单位错误族全库静态审计报告

- 审计人：R25-h 静态审计子代理
- 日期：2026-08-29
- 方法基线：dc = `reference/decompiled.c`（唯一语义真值）；判定口径学习自 git 84dc772（R25-g 校准样例 5 处）
- 纪律：全程只读；未改 src、未构建、未 git 写操作、未用 windbg/Ghidra MCP
- 候选清单实际为 **22** 个站点（任务书写 23，经核对 grep 普查清单为 22；b8m 候选 2144 的同块兄弟行 2143 已作为补充站点单列）

---

## §1 统计摘要

| 判定 | 候选站点 | 补普站点 | 合计 |
|---|---|---|---|
| OK（msvc 与 dc 同构/等值） | 4 | 15（含批量 cast 形态核验） | 19 |
| BUG(byte-diff)：dc 字节差>>1，msvc 元素差>>1 → 折半 | 18 | 3 | 21 |
| BUG(byte-addr)：dc 字节地址±n，msvc 元素地址±n → 倍差 | 0 | 1 | 1 |
| 未对齐（登记原因，不强判） | 0 | 2 | 2 |

**合计实锤 BUG = 22 处**（21 处 byte-diff 折半 + 1 处 byte-addr 偏移倍差）。

关键判据（本审计确立的不变量）：
1. **dc 全库 0 处裸指针相减**（`grep -E "p[Wu][A-Za-z0-9]* - p[Wu][A-Za-z0-9]*"` 命中 0）。dc 一切指针差都是 `(longlong)a - (longlong)b`（字节域整数）。因此 msvc 侧凡出现裸 C 指针差 `a - b` 参与长度计算，必须逐点核对补偿形态；凡 `(int64_t)a - (int64_t)b` cast 形态必然与 dc 同构。
2. 补偿形态合法：msvc 裸元素差 ≡ dc 字节差>>1（如 execline.c:179 AllocString 参数、core_b7b.c:253），需逐点确认用途一致。

---

## §2 站点总表

### 2.1 候选 22 站点

修法一栏缩写：`FIX-PD` = `(int64_t)((intptr_t)a - (intptr_t)b) >> 1`。

| # | msvc file:line | dc 行号 | 判定 | 修法一句话 | 触发面（调用方→动词） |
|---|---|---|---|---|---|
| 1 | src/commands/core_b3m.c:644 (FUN_14006E030) | dc:68947 | OK | — | FIND/token 提取返回偏移；`limit-lVar4` 两侧均 int64 整数，同构 |
| 2 | src/commands/core_b3m.c:1119 (FUN_14007BF78) | dc:78855 | **BUG(byte-diff)** | FIX-PD（pWVar5,pWVar6） | 进程环境块导入脚本变量（dc 唯一调用方 FUN_1400b0bf8 异步脚本线程）；⚠ 见 §3 观察项 O1：msvc 当前为孤儿函数 |
| 3 | src/commands/core_b3m.c:1129 (FUN_14007BF78) | dc:78862 | **BUG(byte-diff)** | FIX-PD（pWVar5,pWVar6） | 同上 |
| 4 | src/commands/core_b2f.c:6247 (PECMD_DispCommand) | dc:33675 | OK | — | DISP；`>>1` 属魔法数 0x56ac…倒数除法（sum 纯整数），非指针差 |
| 5 | src/lang/core_scriptrun.c:313 (PECMD_RunCommand) | dc:29657 | **BUG(byte-diff)** | FIX-PD（pt2,p） | RUN 命令 `".#` 组合前缀（g_hInstance 模式，dc:29652-29658）；语料未见，极窄 |
| 6 | src/runtime/core_exec5.c:306 (FUN_140073CCC) | dc:73340 | OK | — | 参数表构建；已是 `((intptr_t)p-(intptr_t)*slot)>>1` 正确形态（v6.2 重写件）；dc 的 `&0xffffffff` 掩码在正差域等值 |
| 7 | src/lang/core_execline.c:306 (FUN_14007A224) | dc:77875 | **BUG(byte-diff)** | FIX-PD（pw16,inP） | **全变量展开主链**：未闭合 `%name` 段长度 → Reserve+逐字符拷贝；中段未闭合 % 的行（如 `TEXT 100% done`）输出腰斩；段尾 NUL 场景被拷贝终止掩盖 → 语料未暴露 |
| 8 | src/lang/core_execline.c:484 (FUN_14007A224) | dc:78112 | **BUG(byte-diff)** | FIX-PD（vp,val） | `%var~start,len%` 子串展开（负 start/len 钳制前的全长测量）；语料未见该语法 |
| 9 | src/lang/core_execline.c:508 (FUN_14007A224) | dc:78142 | **BUG(byte-diff)** | FIX-PD（vp,val） | 同上（joined_r 分支，`*vp=='\0'` 时 start=全长） |
| 10 | src/lang/core_execline.c:1027 (PECMD_ExpandEnvVars) | dc:78692 | **BUG(byte-diff)** | FIX-PD（vp,val） | ExpandEnvVars 主链（所有命令行展开）的 `%var~s,l%` 子串钳制 |
| 11 | src/lang/core_execline.c:1051 (PECMD_ExpandEnvVars) | dc:78723 | **BUG(byte-diff)** | FIX-PD（vp,val） | 同上 |
| 12 | src/ui/core_b8m.c:2144 (PECMD_FormatDoubleToStr) | dc:141233 | **BUG(byte-diff)** | FIX-PD（dot,pStr） | CALC 浮点结果格式化（calc_expr.c:751/779）；prec<0 且含 '.'，17+ 字符串截断定位错位；语料 026-033/062 未达该分支 |
| 13 | src/ui/core_b8m.c:3120 (FUN_1400F2384) | dc:148352 | **BUG(byte-diff)** | FIX-PD（p,text） | LIST/ITEM 控件 name 段拷贝（core_b3l.c:657、core_b8i.c:149）；name 含 `:` 前段腰斩；语料 057/059 部分覆盖 ITEM 路径 → 修复后需全量回归 |
| 14 | src/misc/core_b1_remaining.c:3011 (PECMD_EnumerateVolume) | dc:3796 | **BUG(byte-diff)** | FIX-PD（pWVar11,pWVar12） | 卷信息枚举/格式化（core_b3r_g5.c:453、core_b3r_h2.c:457 → 卷/设备类命令）；local_148 作为显示串长度 |
| 15 | src/misc/core_b1_remaining.c:3024 (PECMD_EnumerateVolume) | dc:3808 | **BUG(byte-diff)** | FIX-PD（pWVar12,pWVar11） | 同上；`==0` 判定在元素差域把「恰 1 字符」误判为空 → 误入分区枚举分支（dc 仅 0 字符才进） |
| 16 | src/misc/core_b1_remaining.c:3865 (PECMD_ScanDirectory) | dc:4901 | **BUG(byte-diff)** | FIX-PD（pWVar6,pWVar4） | 目录扫描前缀（`=`/`!`/`@`）剥离长度；唯一调用方=PECMD_RegisterFileAssociations（见 #17-20 同链） |
| 17 | src/misc/core_b1_remaining.c:4880 (PECMD_RegisterFileAssociations) | dc:6942 | **BUG(byte-diff)** | FIX-PD（pTok,lps） | 文件关联注册（启动/INIT 路径）：路径副本在**半程位置**写 NUL → 目录截断 → 后续 FindFirstFileW 全空；语料不覆盖（写注册表路径） |
| 18 | src/misc/core_b1_remaining.c:4890 | dc:6952 | **BUG(byte-diff)** | FIX-PD（pStr,pTok） | 同链：基名长度 iLen → ScanDirectory 的 param_4 起跳偏移 |
| 19 | src/misc/core_b1_remaining.c:4952 | dc:7013 | **BUG(byte-diff)** | FIX-PD（lps3,pTok） | 同链：INDATA 资源 `LOAD:` 行第一段长度 → 表2 命令词拷贝腰斩 |
| 20 | src/misc/core_b1_remaining.c:4960 | dc:7023 | **BUG(byte-diff)** | FIX-PD（lps3,pTok） | 同链：`#` 词长度与 `1 < iLen` 门；1 字符词误判（同 #15 形态） |
| 21 | src/misc/core_b1_remaining.c:9934 (PECMD_LoadIcon) | dc:18892 | **BUG(byte-diff)** | FIX-PD（pWVar8,lpString） | 图标加载 `lib#res|text` 语法资源名长度（core_b2d.c:1539、core_b3i.c:330、core_b2f.c:2568、core_b3r_c.c:834 → LOAD/FORM/ITEM 图标）；`|` 存在时名字腰斩 |
| 22 | src/commands/core_b3_remaining.c:17473 (FUN_14007403C) | dc:73542 | OK | — | argv 表重定位；`lVar2-lVar1` 均 int64 槽值（字节域），`(>>1)*2` 同构（与 dc:73354 同款） |

### 2.2 补普站点（自查新增）

| # | msvc file:line | dc 行号 | 判定 | 说明/修法 | 触发面 |
|---|---|---|---|---|---|
| S1 | src/lang/core_execline.c:183 (FUN_14007A224 出口) | dc:77758/77761 | **BUG(byte-diff)** | dc：`lVar11=字节差; return lVar11>>1`（元素数）；msvc：`lVar11=(cur-base)`（已是元素数）再 `>>1` → 返回值折半。最小修法 `return lVar11;`（或 FIX-PD 保形）。AllocString 参数 `(cur-base)+2` 已补偿、正确勿动 | 返回值经 FUN_14007BDA8（dc:78805 `return lVar3`）传播；**msvc 全部调用方按 void 丢弃** → 当前无行为差异，潜在 |
| S2 | src/lang/core_execline.c:700 (PECMD_ExpandEnvVars 出口) | dc:78320/78322 | **BUG(byte-diff)** | 同 S1（`lVar7=(cur-base); return lVar7>>1;`） | 同 S1 |
| S3 | src/ui/core_b8m.c:2143 (FormatDoubleToStr) | dc:141232 | **BUG(byte-diff)** | dc `(longlong)last + 2 - (longlong)pStr >> 1`（+2 字节）；msvc `last + 2 - pStr`（+2 元素=+4 字节）→ 判 17 门限错。修法 `(int)(((intptr_t)last + 2 - (intptr_t)pStr) >> 1)`。正确范例同库 core_b3j.c:541 | CALC 浮点 prec<0；门限通过条件由 len≥17 变 len≥34 → 17..34 长度漏截断；连动 #12 |
| S4 | src/kernel/core_strbld.c:246 (PECMD_FormatTypedMemValue) | dc:68740 | **BUG(byte-addr)** | dc `(longlong)pwVar1 + uVar7`（字节域：钳制对 dataLen 字节、'~' 乘 elmW 字节）；msvc `value + off`，value 为 `WCHAR*` → +2×off 字节，**读值偏移倍差**。修法 `memcpy(&val, (uint8_t *)value + off, elmW);` | 带 spec/width 的类型化取值（`%V:char2%`/`~` 宽度语法）；spec==NULL 早退不受影响 → 语料未见 |
| S5 | src/misc/core_b1_remaining.c:5095 | dc:7158 | OK | 已是 `((uintptr_t)pPre-(uintptr_t)local_res18)>>1` 正确形态 | RegisterFileAssociations CALL $ 日期段 |
| S6 | src/commands/core_b3_remaining.c:17435 | dc:73507 | OK | `(int64_t)pWVar15 - 槽值` cast 先绑定 = 字节差 | FUN_14007403C |
| S7 | src/commands/core_b3_remaining.c:4494 | dc:43567 (FUN_1400474a8) | OK | local_15b8 为 int64 出参槽（ExpandPathAlloc2 回填端址），字节差>>1 | 文件名前缀比较 |
| S8 | src/commands/core_b3_remaining.c:5412/5425 | dc:46821/46832 | OK | `param_2 + (元素数)` 两侧均元素域同构 | 表达式解析 0x1c/0x1e 分支 |
| S9 | src/ui/core_b7b.c:253 | dc:124621 | OK | 裸元素差 ≡ dc 字节差>>1（补偿形态，等值） | ITEM/LIST spec 解析 |
| S10 | src/ui/core_b7c.c:5536 / 5251 / 3734 / 3115-3121 | 各对齐处 | OK | 5536 cursor12 为 longlong；5251 `*pcursor` 槽值；3734 char* 差=字节；3115 组全整数槽（`>>1` 处有 `宽串: 字节长→字符长` 注记） | b7c 列/ITEM 解析 |
| S11 | src/commands/core_b3r_h3.c:738/772/851 | dc:100906/100936 | OK | dc 本身写 `(longlong)(pWVar16 + -1)` 元素域，msvc 逐字同构 | FIND/STR 位置计算（PECMD_SearchStringAndLocate @0x1400a0d38） |
| S12 | src/commands/core_b3r_h3.c:1339 | — | OK | uVar22 = 长度字段（字节）`>>1`→字符数，整数换算非指针差 | 同上族 |
| S13 | cast 形态批量核验 | — | OK | core_b3r_i28c.c:1056/1071/1076、core_b3r_i28e.c:432、core_b3r_d.c:377、core_b3r_h4.c:401/429/501/1139、core_b3_remaining.c:16868/20145/21261/23405/24735/24991/25629/28239、core_b2f.c:1231/1398/1745/1757/1775/3577/7078、core_b3j.c:103/104/112/115/541、core_strbld.c:78/103、core_script2.c:450/492/591、core_scriptdep.c:378 — 依不变量 1（dc 零裸差），cast 形态必然字节域同构 | — |
| S14 | src/lang/core_script2.c:164/179 | dc:56538/56553 | OK | lVar6 为整数（字节计），表达式逐字直移 | ` ;;`→换行压缩 |
| S15 | src/misc/core_b9_remaining.c:607（PECMD_WideStrLen） | dc:159144-159148 | OK | 有意重写（R14 #001 档案在案）：元素差=lstrlenW 语义 ≡ dc `(diff>>1)-1` | 广泛 |
| S16 | src/lang/core_exec4.c:343、core_exec2.c:335 | 未逐行对齐 | OK | 自洽重写体：元素数 alloc/copy/终止符内部一致 | 文本装载/参数切分 |
| S17 | src/device/core_b3r_g4.c:289/302、src/lang/core_exec2.c:426、src/misc/core_b1_remaining.c:2603、dc:6990 对应处 | — | OK | ReadFile 字节数→字符数整数换算 | PELOGON 配置读取 |
| S18 | src/commands/core_b3_remaining.c:1098/1180 (PECMD_ParseDateTimeSpec @0x1400408d0) | dc:37863+（20KB 大函数） | **未对齐** | 重写体（自洽元素域：malloc(len+1)×2、len 字符拷贝内部一致）；dc 对应语句未逐行核到（时间成本），不判 BUG | `*`/`-*` 日期规格解析（CALC 日期类） |
| S19 | src/commands/core_b3_remaining.c:9062 | dc:51183 | OK | 两侧指针类型与 `+ lVar18*2` 完全同构 | 设备 IOCTL 结果遍历 |

---

## §3 修复建议批次（按 影响面 × 置信度 排序）

> 统一修法：byte-diff 族 `(int64_t)((intptr_t)a - (intptr_t)b) >> 1`；byte-addr 族按 dc 字节域逐项换算（S4 用 `(uint8_t*)value + off`；参照 84dc772 对 dc:22653 的修法）。**每处修复须附 dc 行号注释**（沿用 R25-g 注释风格）。

- **批次 A（最高优先，核心展开链，修复后需全量回归）**：execline.c:306/484/508/1027/1051 + S1(183)/S2(700)。
  - 同文件同函数成组修复；全变量/环境展开是所有命令的公共路径，理论影响面最大。
  - 183/700 出口返回值当前所有调用方按 void 丢弃（已核 i28b/i28e/b2e/b3l/b3m/calc_expr），修复不改变现有行为，纯 dc 保真；306/484/508/1027/1051 影响未闭合 `%` 段与 `%var~s,l%` 子串两语法，现语料未暴露（段尾 NUL 掩盖/语法未用），属语料盲区。
- **批次 B（CALC 浮点，连带成对）**：b8m.c:2143+2144（S3+#12）成对修，另核 b3j.c:541 为已正确范例勿动。回归 026/027/028/032/033/062。
- **批次 C（LIST/ITEM name 段）**：b8m.c:3120（#13）。回归 057/059 及全部含 ITEM/LIST 的用例。
- **批次 D（卷枚举）**：b1_remaining.c:3011/3024（#14/#15）。3024 修复改变「恰 1 字符」分支走向，回归 058 及设备类。
- **批次 E（图标名）**：b1_remaining.c:9934（#21）。`lib#res|text` 语法，回归含图标资源用例。
- **批次 F（启动关联链，语料盲区）**：b1_remaining.c:3865 + 4880/4890/4952/4960（#16-#20 同一触发链：RegisterFileAssociations）。6 处一并修。
- **批次 G（窄触发）**：scriptrun.c:313（#5，RUN `".#` 前缀）。
- **批次 H（byte-addr 唯一实例）**：core_strbld.c:246（S4，FormatTypedMemValue）。
- **观察项 O1（非本族但必须随批处理）**：core_b3m.c FUN_14007BF78（#2/#3 所在真体）是**孤儿函数**——`tools/rename_map.json:922` 将 FUN_14007bf78 映射为 PECMD_ImportEnvironment，但 i28c.c:1235 实际解析到 `unimplemented_stubs.c:489` 的 0 参空壳 `PECMD_ImportEnvironment(){return 0;}`（i28c.c:57 的 3 参 extern 与之同名冲突）。修复 1119/1129 的同时应：删除/改名空壳，把 i28c:1235 接到真体（真体改名 PECMD_ImportEnvironment 并同步 3 参签名），否则两处修复无任何行为效果。此判定置信度高（符号名解析唯一性），建议主代理构建核验链接符号。

---

## §4 方法与局限

**方法**：
1. 学习 84dc772 校准 diff 的两条判定口径（byte-diff>>1 折半；byte-addr `+n*2` 字节域）。
2. 对候选站点逐个：读 msvc ±40 行 → 由 `/* ========== NAME @0xADDR ==========*/` 取函数地址 → grep/awk 定位 dc 函数 → 语句序对齐 dc 表达式 → 三态判定。
3. 补普三路：①全库 `>> 1` 清单（264 行，剔除 0x7fffffffffffffff 常量后按指针差特征过滤，剩余多为整数/寄存器域，抽样核验）；②拷贝调用点（StrCopyW/StrBldCopyWideN/StrDupA/FormatTypedMemValue）长度参数反向核对（裸差无 >>1 的 2 倍方向 + `/2`/`>>1` 形态）；③`+ x * 2` 字节/元素混算点抽核（b1:9235、b3r:7058/7063/9062、exec5:302 均两侧同构 OK）。
4. 确立并使用不变量「dc 全库零裸指针差」对 cast 形态站点做批量核验（S13），避免逐点展开 20+ 站点。

**局限**：
1. S13 批量核验依据不变量，未逐行展开 dc 对齐；若某 cast 站点的 dc 原文并非同形差值表达式（而是其它来源的等形巧合），可能漏判——概率低但非零。
2. S18（ParseDateTimeSpec 1098/1180）为 20KB 重写体，dc 逐行对齐未完成，按「自洽=不判」登记。
3. 触发面判定基于静态调用链 + 语料目录名推断；「语料未暴露」未逐一跑对拍验证（本代理无构建/运行权限）。
4. `>> 1` 全库清单中与指针无关的整数移位（颜色混合、文件属性位、消息参数拆包等）仅按上下文抽验，未 100% 逐行核 dc。
5.FUN_14007A224 出口（S1）dc:77761 与 execline.c:831 校准样例（dc:78350）同属 FUN_14007af60 族链的姊妹函数 FUN_14007a224；两函数结构镜像，若主代理修复时混淆姊妹函数站点，应以本表 dc 行号为准。
6. 任务书称候选 23 处，实际 grep 清单 22 处；差额疑似把 b8m:2143/2144 同块两行计为一处的口径差，本报告按 22 处候选 + 补普立案。
