# S15 语料扩容计划 — 全动词覆盖缺口分析与新语料提案（046–065）

- 角色: 规划子代理（S15）。本文件为**唯一报告产出**；配套暂存区 `refactored/build/msvc/corpus_stage/`（本代理新建，唯一编辑者）。
- 基线: 工作区当前状态只读取证。**未运行任何 EXE**（golden 录制留给主代理）、未改动 harness/corpus 现有文件、未动 src/**、未构建、未 git、无 debugger 会话。
- 日期: 会话时钟（承接 S12/S13 序列）。

---

## 0. 执行摘要

| 指标 | 数值 | 说明 |
|---|---|---|
| 动词全集 | **117**（Layer A FourCC）/ +Layer B 开放集 | 沿用 s12 矩阵口径；Layer B 以 WRITE 为已实证代表 |
| 覆盖率·S12 基线（28 案） | 11/117 ≈ **9.4%** | 矩阵 §0 自报口径 |
| 覆盖率·当前（45 案，031–045 已落地并录完 golden） | 断言覆盖 13/117 ≈ 11.1%；计入 EXEC 尾声 incidental 用法 14/117 ≈ **12.0%** | 新增 SET(031)/MDIR(041)；EXEC 每案尾声都在跑但无失败分支可辨 |
| 本批提案 | **20 条新用例（046–065），覆盖 19 个零覆盖动词 + EXEC 显式断言升级** | 已全部写入 `corpus_stage\`，结构自检通过 |
| 覆盖率·本批落地后（65 案） | 断言覆盖 32/117 ≈ 27.4%；含 EXEC 显式 33/117 ≈ **28.2%**；总口径（+Layer B WRITE）≈ 29.1% | 相对基线 **×3** |
| 剩余零覆盖 | 84 个 | 分类登记见 §2.3/§8，多为 GUI 族/危险池/网络设备类/桩阻塞 |

**核心方法论升级（相对 S12 批次）**：现库尾声只回捞变量**名**（`CASE=<id>|<var名列表>`），值从不落盘，且 masks.conf 为空——大量既有案的 PASS 仅证明"跑完没崩"。本批利用 `make_epilogue` 对 `%` 不转义、PECMD 先于 cmd 展开变量的实证行为（done.txt 通道），把 `%VAR%` 值引用写进 `manifest.vars`，使 vars.txt 变为**值级证据**；配合 IFEX/FIND 派生 ok/bad 标志，实现"产物逐字节可比"的任务要求。详见 §4。

---

## 1. 证据与方法

### 1.1 输入证据（本轮实读）
| 来源 | 用途 |
|---|---|
| `build/msvc/s12_verb_coverage_matrix.md` | 117 动词全集、P0/P1/P2 分层、桩态清单（§4.1–4.3）、首批 15 条草案 |
| `harness/corpus/cases/*`（45 案逐一核对存在性）+ 抽样精读 001/005/008/010/013/015/016/018/022/026/029/042 的 main.pecmd/run_all.pecmd | 现库方言一手样例：`ENVI/FIND $%S%=x/IFEX path 与 $%A%>3/WRITE %F%,t/READ %F%,V/FILE =>/_SUB F,a,b…_END/CALL F,X,Y` |
| `harness/runners/run_case.py` + `diff_case.py` + `check_corpus.py` + `masks.conf` + `harness/README.md` | 部署→尾声生成→回捞→比对全链路契约；masks.conf 实测仅注释行（无生效掩码） |
| `harness/golden/win_real/<case>/{vars,done,exit,stdout}.txt`（抽样 031/041） | golden 四产物形态：vars.txt=变量名字面量、done.txt=`OK`、exit.txt=`exit=N`、stdout.txt=空 |
| `docs/divergences.md`（全文） | R1 已修证据（D-04 注记：stubs :108–115 S11 R1 修正案已转发真体）；D-05 ANSI 启动 AV 在修（msvc 侧对拍前置门槛）；attic 教训引用 |
| `build/msvc/s12_golden_expansion.md` | 031–045 录制先例：exit=2 属 errorlevel 正常形态、fs 副作用不入 golden、UTF-8/LF 编码裁定 |

### 1.2 关键事实修正（对任务书假设）
1. 任务书称"现有 28+15 条语料"——核实为 **45 案已全部落地**（cases/ 下 001–018、021–045），且 031–045 的原版 golden 已录制晋升（s12_golden_expansion.md）。故本批从 **046** 起编号。
2. 任务书提到 golden 文件在各 case 目录内——实际 golden 位于 `harness/golden/win_real/<id>/`（四产物），case 目录只有 main.pecmd + manifest.json；C:\pectest\<case>\ 是运行时部署目录（main.pecmd + 运行生成的 run_all.pecmd）。
3. 矩阵 §5 表字面枚举 108 个唯一动词，与 117 差 9 个。经 §4.2 写回组宿主清单交叉印证补齐 **RSTR、STRL、SSTR、TEXT、WRIT、LPOS** 共 6 个；余 3 个（**MESS、HOTK、LOAD**）依矩阵 §6/§7 文字提及推断补齐——标 **ASSUMED(表外)**，依据为矩阵自身文本而非独立物证。
4. 矩阵自称"UI 控件值族 23 词"，但其行内字面枚举（ITEM…BROW）实际为 **24** 词——按字面枚举计，出入在此登记。

---

## 2. 覆盖盘点

### 2.1 已覆盖动词清单（当前 45 案）

| # | 动词 | 语料 | 断言形态 |
|---|---|---|---|
| 1 | ENVI | 001–004、043 | 值不落盘，靠完成性 |
| 2 | SET | 031 | 同上 |
| 3 | READ | 008–009、045 | 同上 |
| 4 | FILE | 010–012、041 | fs 副作用不入 golden，靠完成性 |
| 5 | IFEX | 013–015、034 | 分支结果变量未被值捕获（名而已） |
| 6 | FIND | 016–018、035–036 | 同上 |
| 7 | SWIN(_SUB/_END) | 021–023、037–038 | 完成性 |
| 8 | CALL | 021–023、037–038 | 完成性 |
| 9 | TEAM | 024–025、038–039 | 完成性 |
| 10 | CALC | 026–028、032–033、039 | 完成性 |
| 11 | LSTR | 029、040 | 完成性（R1 修复后 msvc 可比前提已具备） |
| 12 | REGI | 030、042 | 042 含真实 HKCU 写读 roundtrip（当次验证后清理） |
| 13 | MDIR | 041 | 完成性 |
| 14 | EXEC | 每案尾声 | **incidental**：字节对拍生效（vars/done 就是它写的）但无独立判定分支 → 本批 064 升级 |
| + | WRITE(Layer B) | 005–007、044 | 完成性 |

### 2.2 覆盖率计算

| 时点 | Layer A 断言覆盖 | 含 EXEC incidental/explicit | 百分比（Layer A 口径） |
|---|---|---|---|
| S12 基线（28 案） | 11 | 11（EXEC 未计） | 9.4% |
| 当前（45 案） | 13 | 14 | **12.0%** |
| 本批落地后（65 案） | 32 | 33 | **28.2%** |

注：矩阵基线把 WRITE(Layer B) 计入"11 族"表述；上表统一按 Layer A 117 分母口径另列，避免双分子。

### 2.3 零覆盖全集（103 个 → 本批消化 19+1 → 余 84）

分类明细（每类逐词列出，供后续批次直接取用；完整排除理由见 §8）：

| 类别 | 数量 | 动词 |
|---|---|---|
| **本批消化** | 19 | FORX FDIR FEXT RSTR RPOS SIZE DIR HASH CODE SED TEMP FORM RECY LINK PATH LOGS STRL SSTR GETF |
| UI 控件值族（gui=true 专项波次） | 24 | ITEM LABE EDIT CHEK PBAR RADI TABL LIST TREE GROU SPIN SBAR TABS SLID HKEY IMAG BASE DTIM IPAD MEMO TIPS PINT MENU BROW |
| 破坏/系统副作用池（禁常规池） | 15 | SHUT DOWN DFMT PART DEVI MOUN DISK HIVE RAMD PAGE SHEL SEND NUMK DISP SHOW |
| 网络/设备/物理依赖 | 11 | NTPC TIME PCIP ADSL FDRV LOCK SCRN NAME FNAM COME NOTE |
| 整体桩阻塞（msvc 必 FAIL） | 2 | MSTR USER |
| 有条件缓录（前置条件见表） | 26 | EXIT KILL LOOP WAIT RAND MAIN SERV SUBJ EJEC UPNP SOCK PUTF HIDE RUNS INIT HELP THRD THREAD WALL HOME FONT SITE FBWF DATE CMPS FLNK |
| 矩阵表外/证据不足（ASSUMED 存在） | 6 | TEXT WRIT LPOS MESS HOTK LOAD |
| **合计** | **103** | 19+24+15+11+2+26+6 = 103 ✓（与 117−14 收口） |

---

## 3. T4 可自动化验证优先级排序

排序准则：**文件 / 环境变量 / FIND / CALC / _SUB / TEAM 类易断言优先；网络 / 驱动 / 交互类降级并注明原因**。逐字节可比是硬约束（masks.conf 为空且本代理禁改）。

| 档 | 判定特征 | 动词 | 入选 |
|---|---|---|---|
| **T1 确定性值捕获** | 纯函数式：输入固定 ⇒ 输出恒定，无论方言正误都可字节锁定 | FORX FDIR FEXT RSTR RPOS STRL SSTR SIZE DIR HASH CODE | 046–054（HASH/CODE/SED 兼探针） |
| **T2 环境值标志化** | 原值随机器/时间漂移 ⇒ 只以 IFEX 数值比较/存在性派生确定标志，原值不进 vars | FORM GETF（时间戳/自由空间）；LINK（fs 副作用 confinement 到 out\） | 057 065 059 |
| **T2′ 半稳定值直捕** | 同机跨运行实践稳定（用户 TEMP、回收站卷符），值捕获可对拍，附漂移注记 | TEMP RECY | 056 058 |
| **T3 行为锁定探针** | 语义完全未证 ⇒ 不猜期望值，值捕获本身即真相，配 BODY_DONE 完成证明 | PATH SED LOGS（LOGS 另避 S7 AV 形态） | 055 060 061 |
| **T4a 覆盖升级** | 已 incidental 覆盖但无独立判定 → 补显式断言 | EXEC（= 前缀通道，T2 三变体已验，非 ASSUMED） | 064 |
| 缓录 | 见 §8 逐词原因（伴生进程/方言考古/masks 规则先行/runner 支持/reg 副作用等） | KILL LOOP EXIT WAIT RAND DATE … | 不入本批 |
| 禁常规池 | 关机/格式化/分区/驱动/shell 接管/输入注入 | SHUT DOWN DFMT PART DEVI MOUN DISK HIVE RAMD PAGE SHEL SEND NUMK DISP | 永久排除 |
| gui=true | 需人工在场监督，UI 回捞专项波次整体处理 | UI 24 族 | 专项批次 |

---

## 4. 值级对拍机制设计（本批新约定的取舍说明）

1. **`%VAR%` 值引用进 manifest.vars**：`make_epilogue` 把 vars 以 `|` 连接拼进 echo 行且**不对 `%` 转义**（源码注释明言 PECMD 先做变量展开）——done.txt 的 `%T_DONE%`→`OK` 即实证。因此 `"vars": ["LAST","%LAST%",...]` 会让 vars.txt 变成 `CASE=id|LAST|<值>|...`。这是对现库"只回捞名"惯例的**有意扩展**：若主代理希望保持旧惯例，删除 vars 中带 `%` 的条目即可退化为名捕获，脚本无需改动。
2. **cmd 元字符安全约束**：`_cmd_meta_escape` 只转义静态段，展开值直接进 cmd 命令行 ⇒ 凡被 `%VAR%` 捕获的值必须不含 `^ & | < > %`。本批全部捕获值为路径/数字/短标识符，满足约束；此约束已写入各案 expected.md 并作为后续扩容红线。
3. **BODY_DONE 尾标记**：每案正文末行 `ENVI BODY_DONE=YES` 且列入 vars——若不确定方言的动词行导致原版中止/挂起，vars.txt 整体缺失或 BODY_DONE 缺席，录制者可立即定位中段夭折，而不是对着空产物猜测。
4. **漂移值一律标志化**：FORM 自由字节、GETF 时间戳、DATE 类输出绝不进 vars（masks.conf 空）；用 `IFEX $%V%>0` 数值派生或存在性检查得到跨时间恒定的 ok/bad 标志。TEMP/RECY 按"同机稳定"直捕并附漂移注记。
5. **输入自建**：全部 scratch 文件落在 `C:\pectest\out\` 且带案号前缀（runner 每轮 rmtree out），无跨案依赖（修正存量 008 读他案产物的历史缺陷）。
6. **编码**：main.pecmd 全 ASCII + LF（check_corpus 拒 CRLF；D-05 说明非 UTF-16LE 在 msvc 侧启动必崩属已知项，不影响 orig 侧 golden 录制）。

---

## 5. 新用例索引表（20 条，详见各目录 expected.md）

预期 vars/done 内容按"主假设"书写；每个 expected.md 同时给出替代合法落点（方言不符时的锁定态）。置信度：★实证 / ◆中等 ASSUMED / ▽低 ASSUMED 探针。

| 目录（corpus_stage\） | 动词 | 档 | 断言要点 | 预期 vars.txt（主假设） | done | 置信度 |
|---|---|---|---|---|---|---|
| 046_forx_enum | FORX | T1 | 固定列表 a,b,c 枚举赋值，FIND 校验末值 c | `CASE=…\|LAST\|c\|R\|forx_last_c\|BODY_DONE` | OK | ◆ |
| 047_fdir_normalize | FDIR | T1 | 相对路径归一为绝对路径（cwd=C:\pectest） | `…\|FULL\|C:\pectest\out\s47_probe\|R\|fdir_abs\|BODY_DONE` | OK | ◆ |
| 048_fext_change | FEXT | T1 | 扩展名替换 s48pic.old→s48pic.jpg | `…\|M\|s48pic.jpg\|R\|fext_jpg\|BODY_DONE` | OK | ◆ |
| 049_rstr_right | RSTR | T1 | 右截取镜像 LSTR 形参布局，期望 def | `…\|R\|def\|F\|rstr_def\|BODY_DONE` | OK | ▽ |
| 050_rpos_replace | RPOS | T1 | 变量内串替换 123→X9，期望 abcX9abc | `…\|T\|abcX9abc\|F\|rpos_hit\|BODY_DONE` | OK | ▽ |
| 051_size_known | SIZE | T1 | 自建 data51 文件取尺寸；Z∈{6,8}（CRLF 形态二选一，录制锁定） | `…\|Z\|6\|R\|size_pos\|BODY_DONE` | OK | ◆ |
| 052_dir_wildcard | DIR | T1 | 自建恰 3 个 s52_*.txt 通配计数 | `…\|N\|3\|R\|dir_cnt3\|BODY_DONE` | OK | ◆ |
| 053_hash_probe | HASH | T1/T3 | 固定内容哈希→变量；算法未知故值捕获即锁定 | `…\|H\|<摘要串或空>\|BODY_DONE` | OK | ▽ |
| 054_code_convert | CODE | T1 | `=>` 输出文件存在性 + ASCII 无损人工核对 | `…\|R\|code_out\|BODY_DONE` | OK | ◆ |
| 055_sed_generate | SED | T3 | 文本生成最小探针，值捕获锁行为 | `…\|G\|<生成串或空>\|BODY_DONE` | OK | ▽ |
| 056_temp_exist | TEMP | T2′ | 临时目录回捞 + IFEX 存在断言 | `…\|T\|<temp 绝对路径>\|R\|temp_exist\|BODY_DONE` | OK | ◆ |
| 057_form_positive | FORM | T2 | 盘剩余空间>0 派生标志（**原值不进 vars**） | `…\|R\|form_pos\|BODY_DONE` | OK | ◆ |
| 058_recy_volume | RECY | T2′ | 回收站卷查询 + 存在断言（半稳定注记） | `…\|V\|<卷路径>\|R\|recy_exist\|BODY_DONE` | OK | ◆ |
| 059_link_shortcut | LINK | T2 | .lnk 创建到 out 内 + IFEX 存在断言（副作用隔离） | `…\|R\|link_ok\|BODY_DONE` | OK | ◆ |
| 060_path_special | PATH | T3 | 特殊路径指令最小探针 | `…\|P\|<路径或空>\|BODY_DONE` | OK | ▽ |
| 061_logs_smoke | LOGS | T3 | 合法路径开日志 + 正常退出（避开 S7 AV 形态） | `…\|R\|logs_open\|BODY_DONE` | OK | ◆ |
| 062_strl_len | STRL | T1 | 字符串长度 abcdef→6（表外动词，ASSUMED 登记） | `…\|L\|6\|R\|len6\|BODY_DONE` | OK | ▽ |
| 063_sstr_search | SSTR | T1 | 子串搜索位置 cd→3（表外动词） | `…\|P\|3\|R\|pos3\|BODY_DONE` | OK | ▽ |
| 064_exec_explicit | EXEC | T4a | = 前缀通道回声文件 + IFEX 显式判定（最敏感冒烟） | `…\|R\|exec_ok\|BODY_DONE` | OK | ★ |
| 065_getf_attr | GETF | T2 | 文件属性→数值性派生标志（**原值含时间戳不进 vars**） | `…\|R\|getf_num\|BODY_DONE` | OK | ◆ |

通用预期：`stdout.txt`=空、`exit.txt` 主假设 `exit=0`（出现 `exit=2` 属既有 errorlevel 正常形态，见 s12_golden_expansion.md §4.3，golden 锁定即可）。

---

## 6. 录制与验收流程清单（供主代理照做）

```bat
:: ① 逐案复制进正式语料库（expected.md 随行，不影响 check_corpus）
robocopy refactored\build\msvc\corpus_stage\046_forx_enum refactored\harness\corpus\cases\046_forx_enum /E
:: … 047–065 同式 …

:: ② 结构自检（预期：检查 65 个用例目录 / 全部通过 ✓）
python harness\runners\check_corpus.py

:: ③ 逐案原版录制 + 晋升 golden（单一裁判纪律；timeout 放宽到 120s 与 S12 先例一致）
python harness\runners\run_case.py 046_forx_enum --exe orig --record-golden --timeout 120
```

④ **每案即时核对**（录制当场，勿攒批）：
1. `results\win_real_orig\<id>\` 无 `run_notes.txt`（vars/done 回捞零缺失）；
2. 对照该案 `expected.md` 核对 `golden\win_real\<id>\vars.txt`：
   - 落主假设分支 → 通过；
   - 落替代分支 → **golden 有效不重录**，把真实方言回填 expected.md 与本报告 §7 注记；
3. `done.txt` 必须为 `OK`；
4. 超时/缺产物/崩溃 → 该案判 **SKIP(dialect)** 登记原因，同一案最多重试一次，禁止反复盲试（防挂起堆积，attic 019/020 教训）。

⑤ **顺序建议**：先录 ★/◆ 档（047 048 051 052 054 056 057 058 059 061 064 065），建立信心后再录 ▽ 探针档（046 049 050 053 055 060 062 063）；061(LOGS) 若复现收尾 AV 立即停录登记。

⑥ **msvc 侧对拍（后置门槛，不在本轮）**：D-05（ANSI 脚本启动 AV）修复前，msvc 后端对新旧语料全量 FAIL 属预期状态，不构成回归噪声；修复后统一 `run_case.py --all` + `diff_case.py --all`。R1（PECMD_SetVariable no-op 桩）已经 S11 修正案转发真体（divergences.md D-04 注记），故本批写回组新动词（RSTR/RPOS/STRL/SSTR/FDIR 等）在 msvc 侧具备端到端可比前提。

⑦ git 提交由主代理统一执行（本代理未动 git）。

---

## 7. 取舍与 ASSUMED 登记

| # | 事项 | 取舍/依据 |
|---|---|---|
| A1 | 任务书"corpus_stage\NNN_verbname\.pecmd"单文件表述 | 落地为三件套 main.pecmd+manifest.json+expected.md：与现库结构对齐保证 check_corpus 直接过，expected.md 承载任务要求的"人工可核对的期望值说明"；另置 README.md 索引 |
| A2 | "脚本内 echo 关键结果到 out\vars.txt/done.txt" | 经 runner 链路实现而非手写在 main 里：手写会被 make_epilogue 的 `>` 覆盖（源码实证），故采用 §4.1 的 manifest.vars `%VAR%` 引用让**尾声 echo 携带值**；done.txt 由尾声固有逻辑产出。此为对现有管线的最保守兼容解 |
| A3 | 矩阵表外 9 动词（RSTR STRL SSTR TEXT WRIT LPOS MESS HOTK LOAD） | 前 6 个有 §4.2 宿主清单物证；后 3 个仅矩阵 §6/§7 文字提及，标 ASSUMED(表外)。均不在本批 20 内（除 RSTR/STRL/SSTR 有同族形参布局可循） |
| A4 | UI 族计数 23 vs 字面 24 | 按字面枚举 24 计，出入登记 |
| A5 | EXEC 覆盖口径 | 当前计 incidental；064 落地后计 explicit。两口径均在 §0/§2.2 双列 |
| A6 | FORX/FEXT/FDIR/RPOS/HASH/CODE/SED/PATH/TEMP/FORM/RECY/LINK/GETF 参数序 | 全部 ASSUMED，逐案标注置信度；设计保证"猜错也是有效锁定态"（§4.3 BODY_DONE + 替代分支），golden 以原版为唯一裁判 |
| A7 | LOGS 安全形态 | 仅避开 REVIEW §S7 明示的 `LOGS\0\n\n` 畸形输入；正常路径形式为常识假设，录制红线已写入 expected.md |

---

## 8. 缓录/禁入动词登记表（84 个剩余零覆盖的处理依据）

| 动词 | 原因（一句话） | 解锁条件 |
|---|---|---|
| EXIT | PSB 内联特殊路径，置于 main 中段会截断 runner 尾声（vars/done 无法回捞） | runner 支持 EXIT 后续捞 |
| KILL | 杀自身启动的子进程对拍难，需伴生进程方案 | 伴生进程脚手架 |
| LOOP | attic 019/020 方言误判致原版不终止（60s 超时教训） | 方言考古定案 |
| WAIT | 死等风险，时长方言未证 | 同上 + timeout 收紧 |
| RAND | 输出随机不可直接对拍 | masks.conf 增随机值规则（禁改解除后） |
| DATE | 输出含当前时刻 | 同上（时间戳掩码） |
| MSTR USER | 整体桩 no-op，加了必 FAIL（msvc 侧） | 先补桩移植 |
| HELP | ParseHashNumbers 占位 ◑ + 交互形态嫌疑 | S11 占位清零 + 交互评估 |
| RUNS HIDE | Run 键/PELOGON 注册表真实写入（任务约束避免注册表副作用） | 专用清理约定批次（参照 042 先例） |
| SERV INIT SUBJ EJEC SHOW FBWF SITE WALL HOME FONT | 服务/系统开关/盘符/分区显隐/外观等系统副作用 | 隔离环境专项 |
| PUTF | 剪贴板副作用 | 用户在场监督批次 |
| SOCK UPNP | IPC socket / INDATA 执行 exe 方案复杂 | 专项设计 |
| THRD THREAD | 异步对拍难 | 异步同步化方案 |
| MAIN | 递归入口，Global\PECMD:main 排他易互斥冲突/爆栈 | 专用方案 |
| CMPS | 合法编码样本制作方法未知（需原版编码器取证） | 取证后 |
| NTPC PCIP TIME ADSL | 网络/NTP/系统时钟依赖 | 隔离网络环境 |
| FDRV LOCK SCRN NAME FNAM SEND NUMK DISP COME NOTE | 设备/全局键态/显示切换/串口物理依赖 | 物理监督批次 |
| SHUT DOWN DFMT PART DEVI MOUN DISK HIVE RAMD PAGE SHEL | 关机/格式化/分区/驱动安装/hive/页面文件/ramdisk/shell 接管——破坏性 | **永久禁入常规池** |
| UI 24 族 | gui=true，控件值→变量回捞多数同时踩 R1 历史+占位桩 | "UI 回捞专项波次"整体处理（矩阵建议） |
| TEXT WRIT LPOS MESS HOTK LOAD | 矩阵表外，语法零证据 | 先考证再立项 |

---

## 9. 纪律遵守声明

- ✅ 只写 `build/msvc/s15_corpus_expansion_plan.md`（本文件）与 `build/msvc/corpus_stage/**` 新文件；
- ✅ 未修改 harness/corpus 现有文件、src/**、构建产物；未运行任何 EXE；未 git；无 debugger 会话；
- ✅ 全部 20 案通过机械自检：目录名=id 正则、manifest 六必填、JSON 可解析、main.pecmd 纯 ASCII/LF；
- ✅ 歧义处保守处理并逐条登记（§7），无法验证项标 SKIP/ASSUMED 而非虚构语义。
