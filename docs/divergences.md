# 已知分歧登记（Divergences Registry）

- 文件：`refactored/docs/divergences.md`（唯一登记入口；追加条目只改本文件）
- 建立：2026-08-25 ｜ 维护纪律：**只读取证**（不运行任何 EXE、不动实现代码、不 git）；歧义处保守注记，不虚构语义；无法取证的项目明确 SKIP 并注明原因。
- 结论基准：移植体（`refactored/` 工作树）vs 原文 `decompiled.c`（dc）/原版 `PECMD.EXE` 的行为分歧。行号对应当前工作树快照，重链/重构后须复核。

## 0. 登记规范

每条分歧一个 `D-nn` 编号，四个必填字段：

| 字段 | 要求 |
|------|------|
| **根因** | 可指认的成因，必须落到 `文件:行号` 或明确机制描述；推断性内容单独标注「推断」并给出依据边界 |
| **证据链** | 可回溯序列：审计报告 → 章节/结论编号 → 一手物证（源码行 / map 行 / 汇编地址）。至少一条一手物证，不接受仅转述 |
| **影响面** | 触发条件 + 受影响的语法/动词/语料 + 是否污染现有验收门（harness 28 例、T4 对拍） |
| **状态** | 取值词表：`未修` ／ `在修` ／ `待复证` ／ `低风险挂账` ／ `已归一(残留)`。状态变更须附日期与依据 |

补充规则：

1. 每条末尾可带 `备注/SKIP`：口径注记、前提偏差、未做事项及原因。
2. 「×N」计数的条目（如缺变参 ×2）按**站点数**计数，不按报告数。
3. 引用审计报告一律写相对本仓库根的路径 `build/msvc/s11_*.md` + 章节号。
4. 本文件不复制报告正文，只登记结论与指针；裁定细节以报告原文为准。

### 审计报告索引（结论已被本文引用）

| # | 报告 | 范围一句话 | 本文引用条目 |
|---|------|-----------|--------------|
| 1 | `build/msvc/s11_exec_launch_audit.md` | ECD EXEC 启动块十四变量逐行赋值对照（dc:10109–11087 ↔ rb:2598–3617） | D-01/02/03/06/08 |
| 2 | `build/msvc/s11_psb_launch_audit.md` | PSB 启动点归属二进制级重定位（0x1A2068 = PSB→ECD 分发，非 CreateProcessW） | D-05/06/08 |
| 3 | `build/msvc/s11_inject_shadow_audit.md` | 注入桩遮蔽专案（static 桩 vs core_b8k 真体 vs dc FUN_1400e7414） | D-01/02 |
| 4 | `build/msvc/s11_psb_loop_audit.md` | PSB 主循环/ECD 入口实参加对照与崩溃分诊（exit=0xC0000005 复现） | D-05 |
| 5 | `build/msvc/s11_varargs_recovery.md` | wsprintfW 两站点变参恢复取证（IAT+格式串字节级验证） | D-03 |
| 6 | `build/msvc/s11_dual_body_audit.md` | 字符串家族双体裁决（difflib 归一化 diff + map 物证） | D-07 |
| 7 | `build/msvc/s11_t4_precheck.md` | T4 开跑前只读预检（构建红/陈旧二进制风险清单） | D-08 |

> SKIP 注记：`s11_init_listhead_locate.md`（定位记录）、`s11_warning_census.md`/`_v2.md`（告警普查）非分歧裁定文档，不入索引；若后续从中提炼出行为分歧，按 §2 模板另行登记。

## 1. 已知条目（预填，2026-08-25 只读取证）

| ID | 标题 | 一句话根因 | 状态 |
|----|------|-----------|------|
| D-01 | 注入式 EXEC 三路径静默失败（static 桩遮蔽真实现） | rb:247 桩编译期抢绑两处调用，core_b8k 真体零调用方 | 未修 |
| D-02 | 同族第二实例：PECMD_PatchRemoteWinExec 空桩 no-op | rb:252 空桩被 rb:3738 调用，core_b2c 真体未绑定 | **已修(S14批2)** |
| D-03 | wsprintfW 缺变参 ×2（Ghidra 变参丢弃被忠实直移） | dc 原文即缺参，移植忠实继承；x64 下读寄存器/栈残留 | **已修(S14批1)** |
| D-04 | 零参占位桩族（签名归正但体待移植） | unimplemented_stubs.c 大量 `(void){return 0;}` 占位 | 进行中(S14批3修2) |
| D-05 | PSB「幻影行」崩溃（机制已更名，双缺陷叠加） | ECD 第 4 参高 48 位栈垃圾 AV × ANSI 按 UTF-16LE 误读触发链 | **已修(甲乙丙丁)** |
| D-06 | FUN_140101e04 双体疑点（契约+归属残留） | dc void 契约 vs 调用方消费 RAX；归属注释冲突 | 已归一(残留) |
| D-07 | 字符串家族双体返回值契约分歧（FUN_140063888 族） | 同址双体并存致 dc=槽/stubs=void/var2=*ps 三方不一 | 低风险挂账 |
| D-08 | EXEC 启动活体观测矛盾（未决） | 静态实参表与调试观测不可调和；读槽偏移/间接调用面假设 | 待复证 |
| D-09 | 缺陷丙v2：GrowByteBuffer 头契约误判致块内指针释放 | HeapRealloc(=FUN_140063118) 带8字节头返回 hdr+8(dc:60560)，v2 改 HeapFree(buf) 释内指针 | **已修(v3归正)** |
| D-10 | 缺陷丁：PSB LAB_14004c51b 二级地址释放 | dc:44300 传 ppWVar17 的值(槽地址)，rb:6319 写成 &ppWVar17 → HeapFree(栈地址-8) | **已修** |
| D-11 | 缺陷戊：ThreadMainLoop 尾部漏置 NULL 致 double-free | dc:19158-163 两路 ReleaseRefCount 后置 NULL；rb 版直接 FreeStrBuf(&task) 与归零释放相撞 | **已修** |
| D-12 | operator_new 返 0 桩 → 全局对象创建静默失败 | win32_api_stubs.c:41 `void*operator_new{ return 0; }`；12+ 文件控件/对象 new 全 NULL | **已修(真体化)** |
| D-13 | 比较族三别名落返 0 桩 → 前缀/全串匹配面整体失效 | PECMD_AsciiPrefixICmp(rb 115处)/小写 FUN_14005c788(rb 17处)/PECMD_AsciiWideICmp 无真体定义, 链接到 stubs 返0桩 | **已修(转发真体)** |
| D-14 | EnviMemReadWrite param_2 多解一层 → 串文本当游标 | dc:122671 local_288=param_2 值拷贝；v0 写 *param_2 → "A=pe" 文本被当指针(SUB dump 2200) | **已修** |
| D-15 | FUN_14005C788 混装 c72c 词边界语义(R14 恢复纯前缀后暴露) | 4 个调用点原文实为 c72c(core_thread.c:111 包装器内核/core_main.c:155/exec2.c:350+355), 纯语义下穿透或 IMPORT 分支不可达 | **已修(S18 四补丁)** |

---

### D-01 注入式 EXEC 三路径静默失败（static 桩遮蔽真实现）｜未修

- **根因**：`restored_bodies.c:247-251` 定义文件级 `static int64_t PECMD_CreateInjectedProcess(...){…; return 0;}`（打桩注记「未还原 helper, 失败保守」），同 TU 内 rb:3080、rb:3564 两处调用按 C 内部链接作用域**编译期绑定到桩**；真实现 `src/ui/core_b8k.c:345`（↔ dc `FUN_1400e7414 @0x1400e7414`，逐行一致）全仓 0 个调用方，成镜像内死代码。桩出处 `tools/fun_14000e26c_HANDOFF.md:64`——打桩时真体尚未落地，其后真体完成而桩未回收。
- **证据链**：`build/msvc/s11_inject_shadow_audit.md` §0-F1/F2、§1、§3（map 双符号共存：真体 `0x14017f490`(core_b8k.obj) vs 桩 `0x1401d55a0`(restored_bodies.obj)，无 ICF 折叠）；`build/msvc/s11_exec_launch_audit.md` §0-C5、§7-A。本轮复核 rb:247-251/3080/3564 与现源一致。
- **影响面**：三类注入式 EXEC 全部**静默失败**（返回失败而非崩溃）：① `EXEC MEM <非磁盘 PE 目标>`；② 目标以 `&变量` 开头（变量 blob 为 PE 映像）；③ EXEDATA 资源 / LoadFileToSlot 内存映像启动。t2probe、t1、harness 28 语料**零暴露**（均走 `=` 前缀 ShellExecuteExW 或无 EXEC）→ 潜伏分歧，不污染现有验收门 A/T4；未来语料踩中即与原版分叉。
- **状态**：未修（处方＝删桩绑定真体，inject 报告 §5-R1；同族处置见 D-02）。

### D-02 同族第二实例：PECMD_PatchRemoteWinExec 空桩 no-op｜未修

- **根因**：`restored_bodies.c:252` `static void PECMD_PatchRemoteWinExec(void *h){(void)h;}` 被 rb:3738（EXEC 补丁路径 ↔ dc big_funcs:3480）调用；真体 `src/device/core_b2c.c:189`（dc `FUN_1400229f8 @0x1400229f8`，size=118）未被绑定——与 D-01 相同的「桩未随真体落地回收」模式。
- **证据链**：`build/msvc/s11_inject_shadow_audit.md` §0-F4、§5-R3（triage_out_batch3.jsonl:39 RESOLVABLE-STATIC；map L146/L10643 双符号）。本轮复核 rb:252/3738 与现源一致。
- **影响面**：EXEC 远程补丁路径恒 no-op；普通 EXEC 不经此路径。
- **状态**：已修（2026-08-26 S14 批次2：删桩+extern 绑定 core_b2c.c:189 真体；补丁稿 build/msvc/s14_behavior_batches_patchdraft.md。残留：exref 占位符号与绝对地址码体未统一，注入内容待校准——见补丁稿风险节）。

### D-03 wsprintfW 缺变参 ×2（Ghidra 变参丢弃被忠实直移）｜未修

- **根因**：dc 原文即缺变参（Ghidra 未显示），移植体忠实直移。**站点 A**：`src/misc/core_b1_remaining.c:3937`（`PECMD_LoadFileMappingExec` ↔ dc:5065 `FUN_1400091e0 @0x1400091e0`），格式串 `L" *map:0x%p:%lu "` 缺 2 参——汇编实证 %p=CreateFileMappingW 句柄（INVALID→NULL 归一后）、%lu=`lstrlenW(param_1)*2+2`。**站点 B**：`core_b1_remaining.c:1291`（`PECMD_ScheduleSelfDelete` ↔ dc:1752 `FUN_14000481c @0x14000481c`），格式串 `L"PECMD**pecmd-cmd* WAIT *%lu -del \"%s\""` 缺 2 参——%lu=目标 PID（param_2==0 时本进程 PID）、%s=param_1；汇编另有 `-1→0` 钳制（@0x140004863-66 CMOVZ）dc 未呈现。x64 下 wsprintfW 将 R8/R9 及栈残留当变参读取 → `%s` 解引用任意寄存器内容可 AV，`%p/%lu` 打印垃圾。
- **证据链**：`build/msvc/s11_varargs_recovery.md` §0/§2/§3（IAT 槽位→导入名、格式串 UTF-16LE 字节流双重验证；恢复形态见 §2.4/§3.4）；`build/msvc/s11_exec_launch_audit.md` §0-C6、§7-B。本轮复核 :1291/:3937 现文仍缺参（:1290 有 TODO(verify) 标记）。
- **影响面**：站点 A 污染 LOAD 映射链拼出的 `PECMD LOAD …` 命令行（格式化寄存器垃圾）；站点 B 影响 self-delete/WAIT 重启链命令串。波及面提示：varargs 报告 §4-5 列出同类缺参多处（core_b1_remaining.c:1192/2667/2689/3620、app/core_init.c:103 等，部分有 TODO(verify)），尚未逐处取证，不在本条计数内。
- **状态**：已修（2026-08-26 S14 批次1：两站点各补 2 参 + 还原 -1→0 钳位分支（汇编 @140004863 CMOVZ 补全 dc 所缺）；证据与 hunk 见 s14_behavior_batches_patchdraft.md §批次1；stubs_common.h:2119 旧"缺参保持原样"注记已被本批以汇编一级证据偏离，建议追加交叉引用防回退）。

### D-04 零参占位桩族（签名归正但体待移植）｜进行中

- **根因**：`unimplemented_stubs.c` 存在大量零参占位桩 `uint64_t X(void){return 0;}`（grep 口径：`(void)` 定义形 73 处，其中 67 处为 `return 0;` 直返式；口径注记：含少量带注释体的显式 no-op）。部分已经 S11 签名归正但函数体仍为占位，真实语义待对照 dc 移植（例：`:70-72` PECMD_SetVarCore 四参空壳 ↔ dc FUN_14001e6bc；`:136`、`:215`、`:310` 三处归正注记）。历史教训：自动桩零参签名曾致 `PECMD_SetVariable` no-op 使 b3r 家族 87 处调用静默丢变量写回（stubs `:108-115` S11 R1 修正案已转发真体）。
- **证据链**：`unimplemented_stubs.c` 全文（一手物证）；`REVIEW.md` §9-6（*_remaining.c 自动桩签名为错的再印证）；`PRODUCTION_ROADMAP.md:286`（R1 类双体分裂与占位桩清零是覆盖前置）。
- **影响面**：各桩暴露面取决于调用方对「恒返 0/空操作」的解释，逐桩与原版的语义分歧未逐一取证（保守注记，不断言哪些动词已受影响）；凡触达占位体的分支均为潜在分歧点。具体暴露取证后按 §2 模板拆条登记。
- **状态**：进行中（签名归正陆续落地，函数体移植未开始系统性清点）。

### D-05 PSB「幻影行」崩溃（机制已更名，双缺陷叠加）｜在修

- **根因**：双缺陷叠加，且「幽灵空行」原始假设已被审计证伪。
  - **缺陷甲（崩溃机制）**：`restored_bodies.c:6297` 仅清 `local_150[0]`，`[1..3]` 全函数从未赋值；rb:6306-6310 将四段 uint16 打包为 `PECMD_ExecCmdDispatch` 第 4 实参 → 高 48 位为陈旧栈垃圾 → ECD 入口（rb:514-519 ↔ dc:8021-8026）`param_4 != 0` 门通过 → `PECMD_SkipLeadingControlChars` 解引用野指针 → AV（exit=0xC0000005）。dc:44284 同形（原版同属 UB 但该栈槽恰为良性值）。本轮复核 rb:6297 仍仅 `[0]=0`，修复甲未落地。
  - **缺陷乙（触发链）**：`src/lang/core_scriptrun.c:141-168` 无编码嗅探、按 UTF-16LE `rd>>1` 读入脚本（本轮复核 :147 现状如此）；ANSI 文件整篇变单行乱码 → 命中 PSB 通用回退分支（缺陷甲唯一现实触发路径）。原版对应能力：编码嗅探 + ANSI→宽字符转换（dc:29009-29255，尤其 29161-29190）。
  - 机制更名：崩溃指针来自 PSB 栈帧打包参数而非文本缓冲；core_scriptrun.c:152-168 已加的 0x10 槽补零缓解对该崩溃无效（psb_loop 报告 §3 统一解释表）。
- **证据链**：`build/msvc/s11_psb_loop_audit.md` §0/§3/§5（复现日志）/§6 处方甲乙；触发定位另见 `build/msvc/s11_psb_launch_audit.md` §0-K1/K2（返回地址 0x1A2068 属 PSB→ECD 正常分发，非 CreateProcessW）。
- **影响面**：LOAD 非 UTF-16LE（ANSI/无 BOM）脚本必崩（001_envi_smoke 语料稳定复现）；UTF-16LE+BOM 语料不受影响。现有 harness golden 以原版录制，此缺口使 msvc 后端在该类输入上必然 FAIL。
- **状态**：已修（甲=rb:6297 四槽清零 / 乙=编码嗅探方案B / 丙=释放契约 v3 归正见 D-09 / 丁=ECD 回退清理二级地址见 D-10。case001 内容全 PASS 且干净退出 exit=0；后续衍生缺陷戊=D-11）。

### D-06 FUN_140101e04（≡PECMD_CreateProcessW）双体疑点｜已归一(残留)

- **根因**（按可证物证保守登记）：① **返回值契约分裂**——dc:158395-158405 将 `@0x140101e04`（size=103，CreateProcessW 业务薄壳）反编译声明为 `void`，而 dc:5082/10770/10968/139469 各调用方均消费其返回值（RAX=成功标志）；移植体按 REVIEW §9-7 归一为 `int` 返回成功标志。② **归属注释冲突**——`include/stubs_common.h:1048` 注记「@0x140101e04 core_b1_remaining.c」，但 core_b1_remaining.c:19-22 仅 extern 声明，实际唯一定义在 `src/misc/core_b9_remaining.c:194`。③ **链接层现状单符号**（map：PECMD_CreateProcessW @0x13FE50 core_b9_remaining.obj；psb_launch K3 全 .text 直连扫描穷尽 4 个调用点）。
- **歧义注记**：任务简报称「双体」；本轮只读取证**未见现行 C 源中存在第二函数体**，如实登记为契约+归属残留分歧，不虚构第二体语义。若历史曾存在第二体，请后续轮次以 map/diff 物证补充本条。
- **证据链**：`REVIEW.md` §9 判定结果(:728)、关键观察 7(:751)；`tools/rename_map.json:798`、`FUNC_NAMES.md:805`、`docs/provenance.map:244`；`build/msvc/s11_exec_launch_audit.md` §7-C；`build/msvc/s11_psb_launch_audit.md` §1.3/K3。
- **影响面**：现无运行时行为差（单定义）；风险为陈旧注释误导后续还原与原型漂移（void vs int 类契约错误正是 D-07 家族的成因模式）。
- **状态**：已归一(残留)（待办：清理 stubs_common.h:1048 陈旧归属注释——属其他文件改动，本登记轮不动）。

### D-07 字符串家族双体返回值契约分歧（FUN_140063888 族）｜低风险挂账

- **根因**：同址双体并存导致返回值契约三方不一。主案 `FUN_140063888`（↔ `PECMD_StrCopyW`，rename_map.json:225）：dc=槽地址（dc:60967 `return param_1`）/ stubs 直移体=void（unimplemented_stubs.c:429）/ 改名体=*ps 数据指针（core_var2.c:45）。主体逻辑三方语义等价，实质分歧仅返回值契约 + stubs 体死代码空守卫 + 探针日志。同族补充发现：**S-A** `PECMD_StrBldCopyWideN` static(ret 槽 ≡dc)/全局(void) 双定义并存（restored_bodies.c:7581 vs core_var2.c:68）；**S-B** `FUN_140063720` 别名壳丢 dc 返回值（stubs:486-489）；**S-C** `FUN_14006375C` 第二体 ret \*ps（core_string.c:106-124）vs `PECMD_AppendWideStr` ret 槽（restored_bodies.c:7359 ≡ dc:60917）。
- **证据链**：`build/msvc/s11_dual_body_audit.md` §0 裁决表、§1（difflib 归一化 diff + 差异清单 D1-D7）、§6 关系图、§7 补充发现表；map 双符号物证（map:2146/1442 等）。
- **影响面**：当前全树调用点均为语句式或 void 强转（dual_body §1.4 grep 零命中赋值型消费）→ **现编译产物行为一致，低风险**；未来新增消费返回值的调用点将踩契约分歧（只有「槽地址」契约与 dc 运行时语义一致）。
- **状态**：低风险挂账（是否归一由后续轮次决定，dual_body §8；本审计只读不改）。

### D-08 EXEC 启动活体观测矛盾（未决）｜待复证

- **根因**：调试侧曾在 `PECMD_CreateProcessW` 入口观测到的栈槽值（arg5≈命令行指针复制品、arg6≈0x20646d63 形态）与静态可证的 4 个直连调用点实参表**不可调和**（exec_launch C4：flags 值域为小整数集合，数学上不可能等于 0x20646d63）。psb_launch 后案修正两点：① 前案读槽偏移漏加 8 字节返回地址——真栈参自 `@rsp+0x28` 起（H1）；② arg5/arg6 为 32 位存储（`mov [rsp+20h/28h],eax`），整 qword 观测必混入调用方栈面陈旧高位（H2/K5）；残余解释空间为非常规间接调用面（H3：函数指针表/.ilk 残留旧码/TEAM 自重启的另一进程另一构建）。另 `build/msvc/s11_t4_precheck.md` R1 记录构建曾红（xproto.h 三类错误）且 C:\pectest 曾存陈旧二进制——一切动态结论必须先确认被测产物新鲜度（只认 `[msvc_build] OK` 行）。
- **证据链**：`build/msvc/s11_exec_launch_audit.md` §0-C4、§6 排查步骤；`build/msvc/s11_psb_launch_audit.md` §0-K3/K4/K5、§1.4 启动点装载字节、§4-H1-H3、§5-R2 规程；`build/msvc/s11_t4_precheck.md` §0/R1/R2。
- **影响面**：不影响源码忠实性（rb:3285/3491 ↔ dc:10770/10968 十参已字节级验证一致，psb_launch §1.4）；影响的是崩溃归因结论有效性与 T4 回归判读（测旧二进制则 pass/fail 无效）。
- **状态**：待复证（硬约束：在拿到 `dq @rsp L1; ln poi(@rsp)` 返回地址归属前，**禁止**据活体观测改写 rb:3285/3491 实参表——两侧严格一致，改动即制造新偏差，exec_launch R2 / psb_launch R1；复测前删 `.ilk` 全量重链并校验单进程单镜像）。

## 2. 追加条目模板（空表，后续登记在此追加）

> 用法：新条目先在上节加详条（四字段齐备），再在下表补一行索引；ID 沿用 `D-nn` 顺延。证据不足的观察请先挂「待复证」，勿直接定性为分歧。

| ID | 登记日期 | 标题 | 根因（一句话） | 证据链（报告§/文件:行） | 影响面 | 状态 | 备注/SKIP |
|----|----------|------|----------------|--------------------------|--------|------|-----------|
| D-09 | 2026-08-26 | 缺陷丙v2：GrowByteBuffer 头契约误判致块内指针释放 | FUN_140063118 带 8 字节头{size,magic}返回 hdr+8（dc:60560-60566），家族释放=ptr-8（dc:60586/60773）；3e8536a v2 误判"裸块无头"改 HeapFree(g_hHeap,0,buf) 释块内指针 | dump pecmd_msvc.exe.15364 栈帧 srx+0x671 反汇编 call HeapFree 返回址 +0x11de71；全仓 31 处 HeapFree 唯一缺 -8 处 | case001 内容 PASS 但退出必 c0000374；LOAD 链每次触发 | 已修(v3归正 core_scriptrun.c:240 FreeStrBuf) | v1(-8) 本是对的被误改；历史#1 c0000374 归因存疑不追溯 |
| D-10 | 2026-08-26 | 缺陷丁：PSB LAB_14004c51b 二级地址释放 | dc:44300 `FUN_14005b104(ppWVar17)` 传值(槽地址)；rb:6319 写成 `(WCHAR**)&ppWVar17` → HeapFree(栈地址-8) | dump pecmd_msvc.exe.15192 帧 PSB+0x1819；三入口 rb:6317/:6067/:7002 汇聚同一 label | LOAD 不存在文件(PSB→ECD 回退路径)即崩 c0000374；missing-file 类语料前置崩溃 | 已修(rb:6319 改传 ppWVar17 值) | 手测 LOAD case.pecmd 不存在文件复现 |
| D-11 | 2026-08-26 | 缺陷戊：ThreadMainLoop 尾部漏置 NULL 致 double-free | dc:19158-163 两路 ReleaseRefCount 后各置 NULL 再保形 FreeStrBuf(no-op)；rb 版 :230-232 漏置 NULL 且第二路直接 FreeStrBuf(&task) 与引用计数归零释放相撞 | dump pecmd_msvc.exe.10492 帧 SendMsgThreadProc+0x172←FreeStrBuf(BaseThreadInitThunk 直入线程)；dc 对照逐语句 | S14批3(ScriptInit/Copy 转发)激活 TEAM 广播链(BroadcastEnvChange→EnumWindows→SendMsgThreadProc)后退出期必现 c0000374；024_team_multi 回归载体 | 已修(core_thread.c ThreadMainLoop 尾部直移+cbref 槽) | 批次3 前 TEAM 空桩使广播链从未真实运行故基线不炸 |

<!-- 单行示例（复制后去注释填写）：
| D-09 | 2026-08-26 | 示例标题 | 机制一句话（文件:行号） | 报告名 §n + 一手物证行号 | 触发条件/受影响语料/验收门影响 | 未修 | 口径注记或 SKIP 原因 |
-->


### D-16 g_cmdTable1/2 命令注册表恒空 → DispatchBuiltin 全动词拦截失效｜工单已开

- **根因**：msvc 侧 `PECMD_RegisterFileAssociations`(core_b1_remaining.c:4811 ↔ dc:6897 FUN_14000c764)**零调用者**(死代码)，原版由 LoadPlugin= 配置行(dc:7412)与初始化链触发；g_cmdTable1(静态扩展名模式)/g_cmdTable2(INDATA 资源 LOAD: 扫描)恒空 → DispatchBuiltin 对全部内置动词匹配失败。
- **证据链**：活体 dd 实锤 g_cmdTable1Count/g_cmdTable2Count=0(map <common> 段 1402c7af0/1402c7b20)；[WB]/[BP] 探针序列实锤 WRITE 行掉 bare-path→ECD 兜底(s20 §16-§17)。
- **影响面**：所有依赖内置表分发的动词行(WRITE/FIND 等)掉 ECD 被当外部程序执行失败；错误码污染退出码缓存(EXEC=259 同家族)。
- **状态**：工单已开 —— ① 补初始化调用(需先核对 dc 触发时机: LoadPlugin= 行 vs 启动序列) ② INDATA 资源加载链(FUN_14001ea18+ResDecode)在 msvc 的对应执行验证。

