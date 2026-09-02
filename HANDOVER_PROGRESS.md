# HANDOVER_PROGRESS.md — 交接总账（唯一权威）

> **接力导读（新代理从这里开始）**：
> 1. 先读仓库根 AGENTS.md —— 作业契约/硬纪律/命令速查
> 2. 本文件 = 全部轮次进展与当前态势（卷一=R14-R18 史前史，卷二=R19 起）
> 3. 专题深读按需：docs/WINDBG_MCP_ISSUES.md(windbg病理+SOP) / analysis\ 各轮报告 / .agents\skills\ 五技能
> 4. 布局说明：R21 起 git 根=项目根，reference\ 存逆向参考件；历史文档中 refactored\\X 路径按 根\\X 换算

---

# 卷一 · R14–R18 历史（自根目录旧账本原样归并）

# HANDOVER_PROGRESS.md — PECMD WIN 端交接进度快照

> 2026-08-26 Round14 更新 | 面向下一个接力者（DSH agent 或人类）
> 配套档案（按序读）：本文件 → `DEBUGGER_HANDOFF.md`（§1-§14 技术档案+踩坑）→ `refactored\HANDOVER_WIN.md`（v6.5 作战手册）→ `refactored\REVIEW.md` §130-§134

---

## 1. 项目一句话

把 PECMD（WinPE 命令解释器，x64）的 Ghidra 反编译产物还原为可编译 C，当前处于 **WIN 端行为对拍验证**：让 `pecmd_msvc.exe` 通过 harness 语料与原版 `PECMD.EXE` 逐条对拍 PASS 收敛，交付"经行为验证的参考源码"（北极星：Lua FFI 重写高保真对照基线）。

## 2. 里程碑矩阵（Round14 终态）

| 里程碑 | 状态 | 证据 |
|---|---|---|
| MSVC 构建（chcp 936 双绿门） | ✅ | `[msvc_build] OK` 每次构建 |
| 门A：t2probe/case001 产出 vars/done | ✅ | case001 内容 PASS + **干净退出 exit=0** |
| T4 全量对拍 | 🟡 15/43 | 三大根因(甲乙丙)+衍生(丁戊)全修；**退出期堆损坏问题类别已终结**；余 28 失败=SUB/REGI/WRITE 族存量语义缺口（非崩溃性回归） |
| S14 行为批次三件套 | ✅ 落地 | varargs×2 / PatchRemoteWinExec 绑定 / ScriptInit·ScriptCopy 转发（补丁稿+应用+回归验证） |
| R1 多体仲裁 | ✅ 5/5 | build\msvc\s13_r1_arbitration.md + 6 处空桩吞调用清单 |
| R1 batch-A 双体对照 | 🟡 40/134 | s13_r1_dualbody_batchA.md (MATCH 26/DEV 14, 崩溃级×3)；[041..134] 待续派 |
| 语料扩容提案 | ✅ 提案 | s15_corpus_expansion_plan.md + corpus_stage\ 20 案(046-065)待录 golden |

## 3. Round14 关键修复（全部 dump 反汇编返回址 ↔ dc 逐行定案）

| 缺陷 | 一句话 | 修复位置 | 详见 divergences.md |
|---|---|---|---|
| 丙 v3 归正 | GrowByteBuffer 带 8 字节头(dc:60560)，v2 的 HeapFree(buf)=释块内指针 | core_scriptrun.c:240 恢复 FreeStrBuf(-8) | D-09 |
| 丁 | PSB 清理段 `(WCHAR**)&ppWVar17` 二级地址 → HeapFree(栈地址-8) | restored_bodies.c LAB_14004c51b 改传值 | D-10 |
| 戊 | ThreadMainLoop 尾部漏置 NULL → task 块 double-free（TEAM 广播链激活后显形） | core_thread.c 尾部 dc:19158-163 直移 | D-11 |
| S14 批1 | wsprintfW 缺参 ×2 补齐 + (-1→0) 钳位汇编还原 | core_b1_remaining.c 两站点 | D-03 已修 |
| S14 批2 | PatchRemoteWinExec 删桩绑定 core_b2c.c:189 真体 | restored_bodies.c rb:252 | D-02 已修 |
| S14 批3 | ScriptInit/ScriptCopy 零参桩转发 FUN_1400186BC/FUN_140017CDC | unimplemented_stubs.c:60/:191 + stubs_common.h 两声明 | D-04 推进 |

## 4. 当前堵点与下一步（按优先级）

1. **SUB 族收敛**（021_sub_call/037_sub_nested/038_sub_param_shadow 均 c0000005）
   —— 高嫌疑=batch-A 报告的崩溃级发现：core_b3i.c:203/:690 `PECMD_CopyStrToSlot` 漏 `&`（NULL 写 AV）。
   先核这两处再上调试。
2. **REGI 族**（030/042）：dump 7600 已定位 ExpandVarDispatch(cursor[1]==NULL) 读 NULL+0xDA；
   cursor 槽填充链待查（RegiEditRegistry 内 SplitTokenAssignVar 前的初始化）。
3. **WRITE/READ/FIND 文件族**（005-012/016-017/041/043-045）：多为内容/编码级差异，
   batch-A DEVIATION 清单里 WideStrLen 少一(35 调用点)/FUN_14005C788 混语义(130 调用点)是全局候选根因。
4. **batch-A [041..134] 续派**（send_message 到 c7edc0df 或重派）。
5. **R1 转发批次**：仲裁报告 6 处空桩吞调用（stubs:332/392/398/546/547/777；547=073c58 清理流量走劣化副本高优先）。
6. **语料 golden 录制**：corpus_stage 20 案按 s15 报告 §6 流程（robocopy→check_corpus 65 案→逐案 --record-golden→对照 expected.md）。

## 5. MCP 使用手册（三通道全可用）

| MCP | 用途 | 状态/注意 |
|---|---|---|
| **Ghidra MCP 静态** | 原版 /PECMD.exe 的 xref/反编译/字节取证 | ✅ 一直可用 |
| **windbg MCP** | pecmd_msvc 活体断点/unwind/dump 分析 | ✅ 本轮 5 枚 dump 全靠 open_dump+backtrace+.ecxr+ub retaddr 破案；⚠️ 引擎脆弱结论已过时：R24d 上游修复验证（ffa39f7）后 T1/T3/T4 转为 `target exited, session ended` 明确报告、不再 0x80040205 半死，`s -dq` 面随修复关闭；**LNK1201 = 构建纪律（任何在途会话锁 PDB，非引擎故障）** |

破案工作流定型：**open_dump(WER) → backtrace(真实unwind) → 对 pecmd_msvc 返回址反汇编(`ub`) → 返回址↔map 第三列 RVA↔源码行 → dc 权威对照 → 最小修复 → 双绿门构建 → run_case 复验**。

## 6. 纪律速查（沿用，勿破坏）

- 构建必须 `chcp 936` + `[msvc_build] OK` 行为准（LNK1201=先查 windbg 会话是否未关）
- 每处修改携带证据（dump 反汇编返回址/dc 行号对照）后才能落代码
- 分配器家族契约：FUN_140063118/1400633A8/140063694 全部带头返 hdr+8，释放一律 ptr-8（D-09 血训）
- dc 中 `F(x)` vs `F(&x)` 一字之差=传值 vs 二级地址（D-10 血训）
- ReleaseRefCount 后必须置 NULL（D-11 血训）
- 单编辑者纪律；探针网保留至对应门通过后统一拆；禁在 src 内 `#include <stdio.h>`
- 子代理提示词必附 `refactored\tools\subagent_autonomy_clause.md`（本轮 4/4 交付，6 连败已破除）

## 7. 环境备忘

- pecmd_msvc.exe 在 C:\pectest\；WER LocalDumps → C:\pectest\dumps（每轮清理防误读）
- 原版真值 EXE：C:\pectest\PECMD.EXE；手测产物在 C:\pectest\out\（不在用例目录）
- harness 口径：run_case.py <case|--all> 双跑回捞；diff_case.py <case|--all> 出 verdict.json

---

## 【R14c-f 终态补记】(2026-08-26 凌晨, 提交 52e3ffa/f0d26f7/068fcd9/427f5e6)

### MCP 上游修复验证
- execute 控制单语句生效(进程运行至自然退出); 会话不再半死(.lastevent 全程可用)
- ba 数据断点路径仍 0x8000FFFF; s 大范围搜索仍报错(部分为合法不可读) —— 避走软 bp+源码推理

### 全量对拍: 15 → 17/43 (031 新进; 025 回归登记=TEAM-IFEX 展开链上游缺口显形, 不回退批次3)

### 本轮新增修复
- S18 四补丁(TokPrefixICmp: core_thread.c:111 包装器内核惠及~200下游/core_main.c:155/exec2.c:350+355 成对)
- S16 REGI R1(&script→script ×3 同族崩源) + R2(MULTI_SZ cp_sep 复位语义)
- batch-A 剩余六项: 081 RAS 算术/051 vt[1]/045 泄漏/069 数据源+日志短路/039 广播字段/124 返回型
- 比较族三别名转发(AsciiPrefixICmp rb115处/小写c788 rb17处/AsciiWideICmp) + operator_new 真体化 + ExtractTableSegment dc:15778 直移
- EnviMemReadWrite param_2 去多解一层(D-14)
- **退出码读侧修复**(GetExitCodeGlobal 沿 g_Script+0x50→+0x58 链, dc:54066/活体 dd 实锤)

### 退出码攻坚进展(17 案杠杆, 已推至最后一米)
机制全链破解(四步活体实证) → 读侧落地 → [WB]探针定案 WRITE 体返回 183!=2 → WRET 缩小至 SetFilePointer 链/local_88 赋值点(s20 §8-§8.1 工单)。

### 新档案
s16_regi_audit / s17_filecase_triage(17案=退出码单差异重大反转) / s18_c788_census(LINKAGE-RISK) / s19_s14_preflight(B0-B5) / s20_sub_allocav_triage(SUB/MDIR/025/WRITE 四工单)

### 下轮工单(按 ROI)
1. WRITE 返回值 183→2(s20 §8.1, 单函数收尾) → 直接翻转 005/006/007/044/045 等 A1 簇
2. READ 体 FUN_14009da54 同病同修(A2 簇)
3. FUN_140034788 去桩(IFEX/FIND 激活, A4/A5 簇) —— 与#1 叠加预计 43 案冲 30+
4. SUB 挂死/MDIR NULL/025 展开链(三案, 线索已备)
5. batch-A #083 TokenizeExpression 尾段(~690行)专项
6. golden 录制(corpus_stage 20 案)
7. HANDOVER_PROGRESS 终态刷新(R14c-f 未入)

---

## 【R15 终态补记】(提交 0f9a223/fb2751b/07504dd/e254532)

### 已完成
- **golden 录制 20 新案(046-065)全干净**：语料库 45→65 案(结构检查 63 目录含 attic 口径)，覆盖率断言口径 13→32/117；逐案 --exe orig --record-golden 零缺失、done=OK 全过(0f9a223)
- **IsPecmdScriptFile 真体落地**(dc:12709, stubs:95 恒0桩→真体)：boot LOAD 链恢复(srx 读文件 ✓)
- **退出码机制全链破解+读侧修复**：d180≡g_Script+0x50 同址、ScriptInit 自锚 +0x50→&this+0x58、PSB 尾写回非零 local_180、GetExitCodeGlobal 单层读——读侧已沿链(core_init.c)
- MCP 上游修复验证：execute 控制单语句✓、会话不再半死✓；ba 数据断点仍受限

### WRITE 族工单方向修正(s20 §9)
铁证(out.txt 未产出+入口 bp 未命中+[WIN] 探针缺失)⇒ WriteFileEncoded 从未被调，WRITE 行在 bare-path 判定即失败(ECD 兜底当外部程序)。此前"返回值 183 vs 2"考古作废。当前卡点=**bare-path 判定输入空串(e0=0)**，上溯 ExpandVarDispatch 输出链(s20 §11)。

### FUN_140034788 工单修正
dc 中该函数 [DECOMPILE FAILED](size=4312)——IFEX/FIND 共享主体无法 dc 直移，需 Ghidra 手工反汇编专项还原(S17 A4/A5 簇前置)。

### 下轮入口(按 ROI)
1. bare-path 输入空串：ExpandVarDispatch 输出链排查([BP] 探针已布 rb:6304)
2. ExpandVarsRecursive 内部 AV 免解引用二分
3. FUN_140034788 汇编级还原专项
4. #083 TokenizeExpression 尾段(~690行)
5. msvc 侧 63 案基线对拍结果落 divergences.md

---

## 【R16 工单面板】(Round 9-10, 提交 87f3c11/3ca855b)

### 核心突破: WRITE 族失败机制全链闭合(s20 §16-§17)
WRITE 行 → DispatchBuiltin(g_cmdTable1/2 **恒空**, 活体 dd 实锤) 未拦截 → bare-path 段 IsPecmdScriptFile("WRITE")=-1 → ECD 兜底当外部程序 → CreateProcessW gle=2 → 错误码写缓存。
同族: EXEC=259(STILL_ACTIVE, ECD 读码早于子进程结束); WRITE 体返 183(游标偏移, :2251 已修)。

### 下轮工单(按 ROI)
1. **g_cmdTable 注册链修复**(D-16): 核对 dc LoadPlugin=/启动触发时机(dc:7412)→补 msvc 初始化调用; 验证 INDATA 资源加载链(FUN_14001ea18+ResDecode)
2. **ECD EXEC 等待补齐**(dc:11260+ 深水段对照 rb:3700-3760)→解锁 EXEC=259
3. FUN_140034788 汇编级还原(IFEX/FIND 共享体, dc DECOMPILE FAILED size=4312)——A4/A5 簇前置
4. SUB 挂死 / ExpandVarsRecursive AV 免解引用二分
5. #083 TokenizeExpression 尾段(~690行)

### 已就绪资产
golden 20 新案(046-065)录制完成全干净; FourCC 头文件+生成器(tools/gen_fourcc.py, 98 动词值); 五份新档案 s16-s20; batch-A 134/134 收官报告含四级处置优先级分层。

---

# 卷二 · R19 起轮次明细

## R19 (2026-08-26) — 全案崩溃根因三连修+ECD四件套，基线 0/63 → 9/63
- 展开器 ExpandEnvVars(core_execline.c): H1 补 dc:78742 游标强制同步(inP=raw_src)；H2 b961 支路按 dc:78569-78571 自带 src/dst 装载(原双重递减+未初始化)；H3v2 未闭合名路径按 dc:78427-78428 直跳拷贝序言(v1 误经 b961 致自旋已纠正)。
- ECD EXEC=259 根因(analysis/e26c_exec_wait_rootcause.md): DAT_14013a24f 对象分裂→stubs_common.h 宏别名统一 g_flagA24F+删孤儿定义+初值=1；WaitTickCount(rb) 按 dc:59109-59144 直移为消息泵(原体错写全局标志递减)。旧工单裸 WFSO 方案正式废弃。
- N-1: RasDialEntry 缺省入口 HelloASL 回退补齐(dc:144416-144423)。
- 分析产物入库: FUN_140034788 汇编还原(385行)/FourCC 干跑普查(227行)/batch-A 二轮复核(175行,#083 虚警证伪撤销)/e26c 根因(254行)/SUB静态笔记(132行)/windbg MCP 上游复现文档。
- 已登记缺口: FUN_140099f18 -wlan 半区 ~508 行、PECMD_RunServiceCommand 桩、core_b2f.c:3829 占位误归属(IFEX/FIND 执行体)。


## R20 (2026-08-26) — V-Gate 纪律落地 + 探针透明化 + D线三修复, 基线 9/63 -> 17/63
- 误诊复盘: R18 断点定论撤销(地址未随重建更新), B簇 exit=183 实为探针 fopen 污染 LastError; WINDBG_MCP_ISSUES/DEBUGGER_HANDOFF 认知消毒完成, R20 版本锁定纪律五条门挂档。
- 探针透明化补丁 A/B(CPW/OFH 出口 SetLastError 恢复); D线修复 S-TEMP-1(wsprintfW 变参伪影双点)/S-TEAM-1(unaff_R13D 初始化)/S-HASH-1(双名分裂转发归一); P4 双槽恢复登记下批。
- 新基建: tools/make_symsnap.ps1(symsnap.txt=断点选址唯一合法来源) + C:/pectest/DEPLOYED_BUILD.txt 部署身份戳; 遗留冻结副本调试会话已清理。


## R21-R22 (2026-08-26) — 布局重构(副本式)+V-Gate实战+A线第一枪(未竟)
- ★布局: git根已上提为项目根(暂存区PECMD_restructured, 换入待执行); reference\ 子目录收纳逆向参考件; AGENTS.md 成为代理作业契约权威入口; docs/README.md 索引; 技能库新增 vgate-live-debug, pecmd-semantics 升级四重真源(Ghidra MCP=真源0, 桥在线双程序同开); 工具新增 post_build.ps1/run_corpus.ps1(PS5.1需BOM+CRLF已治)。
- V-Gate 首战: 021 活栈 attach 一发钉死自旋点 ExtractTableSegment(FUN_14001b23c), 关键字块栈垃圾实证——旧范式下此案需多轮探针构建。
- A线第一枪(R20C补丁: srx入口种子化 + rb:24634 字节偏移归正)已部署(md5 DC353E0E)但五案仍124——SG尾迹显示dispatch可完整往返, 说明五案死态异质(A线原分解: 021/037=SUB机自旋, 038=吞体, 004=TEAM双停, 002=堆互锁嫌疑), 单点种子化只覆盖其一。
- 下一步(ROI序): 对五案逐一attach抓栈(V-Gate门3, 每案2分钟)取得各自死点 → 分型施治; Ghidra MCP 定案 PATCH-1/unaff_R13D 两笔 TODO(verify); 之后回归全量。
- 环境备忘: 构建日志GBK按gbk读; PS5.1脚本必须 BOM+CRLF(python utf-8-sig 写入); analysis历史文档绝对路径指旧布局, 按 refactored\X⇒根\X 换算。


## R23 (2026-08-26/27) — 构建链UTF-8化+两笔TODO定案落码+五案死态取证, 基线 17/63 (持平)
- ★构建链 v3 (用户主导方向): ┃ tools/build_msvc.py(python 全托管, subprocess 调 cmd.exe) + bash 薄包装 build_msvc.sh;
  ┃ **零中文进 cmd 层**(sources.rsp/_msvc_cmd.bat 全相对路径, cwd=仓库根) + chcp 65001 全 UTF-8 链, 双绿门保留。
  ┃ 旧 msvc_build.bat 保留未删(84 行, 可作对照); 已废弃 cmd //c 绝对路径/GBK 方案(MSYS 转换坑+ACP 读乱)。
- ★make_symsnap.sh awk→内嵌 python (本机 awk 缺失根因: winuxsh 精简环境+DSH 会话快照未合并注册表);
  ┃ 注册表用户 PATH 已补 Git\bin+Git\usr\bin(R23 末重启 DSH/终端后验证 awk/gawk/perl 全部恢复, 无需注销)。
- ★PATCH-1 (S-TEMP-1) Ghidra 定案落码 (真源=原版 0x140008110 反汇编, 推翻 skill 旧记录"tmp+PID+Tick+空串"):
  ┃ 目录模式 fmt 带尾孤立%(0x14011d0d8 实字节)/vararg1=param_4(e26c="exedata")/vararg3=Tick+NextRandomSeed/
  ┃ vararg4=".tmp"(0x14011d108); 文件模式 vararg3=0/fmt 无尾%(0x14011d0a8)/vararg4=param_5 → **函数实为 5 参**。
  ┃ 落码: stubs_common.h+xproto.h 原型×2 + core_b1_remaining.c 函数体两处 wsprintfW + 4 调用点补第5参
  ┃ (e26c="exedata"+".tmp" 含 "exedat" 少字符笔误修正 / devi文件=".tmp.cab" / devi目录="tmp" / mktmp=".tmp" 占位)。
- ★unaff_R13D (S-TEAM-1) 定案: 0x14004c177 MOV R13B,SIL(序言清零) → 0x14004c18a 写 local_170 低32位,
  ┃ unaff_R13D ≡ 0 恒真, restored_bodies.c:5862 `= 0` 初始化即精确还原(非近似), TODO(verify) 关闭。
- 021 死点漂移实证(分析/r23_021_live_stack.md + r23_021_deadpoint_shift.md): 补丁前 attach=ExtractTableSegment
  (FUN_14001b23c)+0x75 ↔ r19a 同构; R20C+PATCH-1 版 attach=+0x9a(同环前移 0x25) → **R20C 种子化部分生效但未终结**;
  裸跑 main.pecmd(无 harness 尾声) = 0xC0000409 fastfail 快速退出 vs harness 场景挂死 → 未初始化堆异质终局佐证。
- 机制链更新(下轮关键): ExtractTableSegment 扫的是**文本流**找 0x88/8A/90 值或终止符; RunScriptText
  (core_execmain.c:218-229) pPersist==NULL 时 calloc(0xf0)+ScriptCopy(0x00-0xEB)+ScriptInit 生成克隆体;
  srx 种子化写的是对象 +0x88/8A/90, **从不写文本** → 需要活体读 *param_4 指向缓冲判定。
- 基线: 全量 63 案 17/63 PASS 零回归零改进; 提交 cd13046(48 files); 部署 md5=62e5ddd3 hash=cd13046。
- 档案: analysis/r23_ghidra_verdicts.md(PATCH-1/R13D 完整证据链) + r23_021_live_stack.md + r23_021_deadpoint_shift.md;
  r19d_fastfail_overrun.md 追加 R23 勘误后记; AGENTS.md 命令速查已改 bash 版。

### 下轮工单 (ROI 序)
1. **021 文本流取证**: attach 后读 (a) RIP 帧 *param_4 文本指针前 64 字节 (b) script+0x88/8A/90 值
   (c) bufRef/文本分配者 —— 判定"对象种子化不解决文本"后选治: 文本分配点补终止符(dc 对应位置须有证据)/修扫描等价位置。
2. **五案其余 4 案 (002/004/037/038) 逐一 attach 抓栈** (V-Gate 门3, 002=ENVI 展开前置区, 004=TEAM 内层 PSB, 037=SUB 机, 038=吞体) → 分型施治。
3. **exit-1 三案 (031/052/054) 共享退出码链根因** (stdout/vars/fs 全一致, 仅 exit 1 vs 0)。
4. 队列: g_cmdTable 注册链(D-16: dc:7412 触发时机)/ ECD EXEC 等待补齐(dc:11260+ 深水段)/ FUN_140034788 汇编还原(IFEX/FIND 共享体 4312B)/ #083 TokenizeExpression 尾段(~690行)/ ExpandVarsRecursive AV 二分。
5. 回归纪律: 每次落码后全量 63 案双跑, 零回归门.

### 环境备忘 (R23 更新)
- 构建: `bash tools/build_msvc.sh` (语法门加 syntax); 部署: `bash tools/post_build.sh . C:/pectest` (symsnap+拷贝+身份戳一站)。
- 本机 awk/gawk/perl 已随注册表修正恢复 (Git\usr\bin); 工具脚本仍以 python 内嵌为稳。
- 构建日志 UTF-8 解码 (chcp 65001 链); 旧 GBK 解码规则仅适用 build_*.log 历史件 (r20c 及以前)。
- windbg/Ghidra MCP 通道在线; symsnap.txt 已 python 化刷新 (14 符号, V1 法律来源有效)。
- 部署身份: C:/pectest/DEPLOYED_BUILD.txt → hash=cd13046 md5=62e5ddd3 (2026-08-27 00:07)。

- **改名安全性体检 (R23 末)**: 可执行/配置类零硬编码仓库绝对路径 (harness/runners、tools 全部
  cwd/脚本位派生; gen_tasks.py 仅一行注释文本提及; rename_to_ascii.sh 自身含替换常量属预期);
  活性文档仅 AGENTS.md/SKILL.md/WINDBG_MCP_ISSUES.md 三处, 由 rename_to_ascii.sh 白名单覆盖;
  历史档案原文保留按换算说明处理; 无 .dsh 配置引用。
### ★ 改名执行状态 (R23 末, 目录侧用户主导)
- **目录侧**: 根目录 `D:\repo\PECMD反编译` → `D:\repo\pecmd-decompile` 由用户执行
  (需无进程占用 + DSH 重配 workspace)。2026-08-27 检查时仓库物理路径仍为旧名。
- **代理侧 (已完成)**: 仓库内脚本/活性文档的全部 `D:\repo\PECMD反编译` 路径引用已切换为
  `D:\repo\pecmd-decompile` —— AGENTS.md 目录地图行 / pecmd-build SKILL 仓库行 /
  WINDBG_MCP_ISSUES 取材说明 / gen_tasks.py 注释 / rename_to_ascii.sh 替换目标。
- **为什么安全**: R23 构建链已零中文进 cmd 层 (rsp/bat 全相对路径), harness/tools 脚本
  零硬编码仓库路径 (全部 cwd/脚本位派生) → 路径切换**不需要改任何代码**。
- **目录侧完成后**: `bash tools/rename_to_ascii.sh` (幂等) → `bash tools/build_msvc.sh` 全量
  构建 → `bash tools/post_build.sh . C:/pectest` → 冒烟 001 + 全量基线复核 (期望 17/63 持平)。
- 历史文档换算: 「PECMD反编译 ⇒ pecmd-decompile」 (与 refactored\X ⇒ 根\X 同款纪律)。

---

## R24 (2026-08-27) — SUB/ENVI 双族挂死定案修复×2 + 全量分型 + 五线取证，基线 17/63 -> 22/63
- ★021/037 挂死同源定案+修复：活体 attach(PID548) 铁证死点=unimplemented_stubs.obj FUN_14001b23c
  桩副本(非真体@0x122950)，扫描环语义与 dc:15781-15795 分叉(缺无条件+1前进/0x88检查错位)，
  文本流与分隔符实测全部正常——R23"对象种子化不解决文本"假设证伪。落码=94行错误副本删改单行
  转发 ≡ PECMD_ExtractTableSegment(核心照抄 FUN_14005bc28 转发惯例)。档案 r24_021_stub_semantics.md。
- ★002/004/038 ENVI 展开器挂死定案+修复：子代理 D 三案活体 attach 全同根因=FUN_14007A224
  (core_execline.c:119) 主扫描环缺环头游标复载(%族分支只推进 line，非空 %var% 展开后 inP 永停'%'，
  StrBldGrowWide 每迭代+0x400 膨胀至 33-34MB 自旋)；修复=环头 `inP=line;`(dc:77750 单游标语义，
  模板照抄同文件 ExpandEnvVars H1 dc:78742)。触发条件=变量值非空(003 空值不挂)。档案 r24_hang_triage_002_004_038.md。
- ★全量分型(r24_fail_triage.md, 子代理A)：44 FAIL 分歧维度仅 exit(stdout/vars/reg 全 same)；
  退出码链=dc 忠实直移(rb:7187-7189≡dc:45139-45141) 无需修——FAIL 本质全是动词处理器返回值错误。
  11 簇: A挂死×3(已修) | B FILE契约×3(l180=87) | C1 IFEX/FIND桩×11(+3借道, 034788 4312B 未去桩)
  | D CALC垃圾码×6(exit=指针低32) | E TEAM×2(0xC0000374) | F 0x80070057×7 | G AV×5
  | H 堆损坏×2 | I 055 fastfail | J exit=1×3(031/052/054) | K 061=183。
- ★D-16 重大修正(r24_cmdtable_chain.md, 子代理B)：原版 g_cmdTable1/2 **无启动即注册路径**——
  唯一填充者 FUN_14000c764(LoadPlugin, dc:6894-7070) 是 ^/-mode/&&& 行首条件触发(dc:7412链)，
  R16"补初始化调用"前提不成立作废。缺口改写为: 同址别名归一转发
  (FUN_14000C764→PECMD_RegisterFileAssociations core_b1_remaining.c:4830 死代码 / PECMD_ResDecode→
  FUN_1400E7D58 core_resdecode.c:52) + INDATA 解码内核五件套仍为 stub(FUN_14005B184 恒0歪曲BOM判定)。
  原版资源实证: INDATA/4=18B "LOAD: HELLO1  #1  "、SCRIPTINIT/100=756B；msvc 无 .rc(D-17 成立)。
  98 动词 FourCC=硬编码直比级联，与 cmdTable 无数据流 → 表空零影响。
- ECD EXEC 工单闭合(r24_ecd_exec_wait.md, 子代理C)：**缺等待结论证伪**——等句柄环
  (MsgWaitForMultipleObjects 末位 hProcess, dc:11295↔rb:3837)、超时预算(dc:11483↔rb:4028)、
  退出码写回(dc:9687↔rb:2178) 全同构；7 条差异 D1-D7 全等价/命名级，D1 TODO(verify) resolve。
  P3 收尾: CPW 十参探针×2+R20 gle_restore 已拆(core_b9_remaining.c, 行为等价)。
- s083 关闭(r24_s083_verdict.md, 子代理E)：TokenizeExpression 尾段(dc:103680-103730 ↔
  core_b3_remaining.c:25119-25172) 逐语句 1:1，零落码；S16 dc:103903 TODO 关闭；S17 dc:104036
  8字节 vs 低32截断登记 divergence 移交 CreateMenuItem 归属线；draft 订正3处。
- 队列: U-1(原版"2写入者" s17§8) 未定位——对拍已钦定: IFEX/FIND 文件判定成功→2、
  条件/表达式路径→0(015/034 PASS 证 0==0)；031/061 golden=0 为 C1 反向风险案。下轮主工单
  =C1 034788 去桩(最多翻转 16 案, 前置 U-1), 其后 CALC 归正(6)/G dump(5)/F 参数族(7)/E TEAM(2)。
- 提交: abc0084(R24 桩转发) + b84da37(R24b A224同步+CPW拆+五档案)；部署 db9a3566
  (01:37:58)；基线 22/63 PASS 零回归；worktree 清理(dumps\ 有 5 枚历史 WER dump 待下轮清理)。
- 环境备忘: 子代理并发取证已验证可行(windbg MCP 多会话+run_case 独占纪律)；Ghidra MCP 会话对
  子代理不可用(子代理B于 R24 实测)——需 Ghidra 的取证由主代理或带说明派发。
- ★R24c C1 去桩(ecfa8a1/21e3be3)：PECMD_IfexFindExecutor @0x140034788 真体(902行草稿→合入
  core_b2f.c, 420条指令级还原, 旧误归属 PECMD_BuildImDiskMenu SKIP桩废弃) + core_b2a 包装器
  回传执行结果。U-1 语义：真→裸分支行执行回传(ENVI→0)；假→嵌套 PSB(act2 "ELSE <分支>"
  RDX=[0x60]) 该行返 2 ⇒ 013/014/016/017/035/036 直接翻转 + 借道 043/046/047/048/049/050/
  052/054/058/059/062/063/064 共 19 案。BE14 隐式 int 截断指针 AV 修复(013/014/061 崩解)。
  真值路径 dc 对齐二处(Ghidra 伪影误清 0: UVar19=extraout_EAX@dc:30938 / UVar35=UVar30@dc:30963)。
  基线 **22/63 -> 41/63 PASS 零回归**(015/018/034 表达式案保持)。
- U-1 工单闭环(r24_u1_exit2_writer.md, 子代理G+cdb 原生取证)：写入者=PECMD_ProcessScriptBlock
  尾 0x14004C533 mov [rax],rdx(guard 非零 local_180)；真实槽=0x14013D188(根脚本+0x58)而非
  0x14013caf0(从未写非零, s17 §8 找错对象)；值 2 三来源：WRITE/FILE 自带 2 / IFEX·FIND 假分支
  "ELSE <分支>" 行返 2 / 真分支=分支命令值 0；**FIND $X=Y 是等值比较非子串**(016/043 标签改写)。
- 遗留：031/061 真判假(exit=2 vs golden 0) — EvalLoopCondition 内 01E69C 直查/等值路径语义
  待与 dc 逐段甄别(两侧入参已铁证一致: 034788/032dc4 双入口 live 对照)；windbg MCP 半死复发结论
  已过时: R24d 上游修复验证(ffa39f7)后 T1/T3/T4 不再 0x80040205 半死, T2 残余 0x8000FFFF
  但伴随会话终结报告; 031/061 深水区下轮主工单(J 簇)。

---

## R24d/e (2026-08-27) — windbg MCP 故障专档化 + 上游修复验证 + 031 深水区取证推进，基线 41/63 持平
- ★windbg MCP 故障触发面专档化：`docs/WINDBG_MCP_FAULTS_REPRO.md`（独立文档，不污染
  AGENTS.md/ISSUES 职责）：T1 bp内嵌命令串 / T2 单条execute连发resume / T3 快退进程二次放行
  worker消亡 / T4 bp设不可达函数——每条含前置/精确步骤/判定标准/机制推断；§0.1 复现物料清单
  （路径+md5 已核验）；文末故障浮现速查卡 + 规避三律(execute单放行/禁bp内嵌串/热点用探针)。
- ★上游修复验证(R24d 实测, ffa39f7)：T1/T3/T4 全转为"目标退出→明确报告 target exited,
  session ended"健康终态；T2 仍报 0x8000FFFF 为一次的错但不再半死链。ISSUES 头部另有 R24e
  台账(外部编写, 已保留)：根因落定=目标于 resume/泵期间退出→引擎滞留运行态+无当前进程
  (GO_NOT_HANDLED / .lastevent 空)→settle 失败；上游 #242 已提交修复。
- 031/061 深水区当前结论(J 簇, 探针版部署体 md5 304eea38 含 [ELC] TEMP PROBE, T5 待拆)：
  ①两侧入参铁证一致(034788/032dc4 双入口 live, script+0xd=0/+0xda=0)；
  ②dc vs msvc EvalLoopCondition 归一化全 diff 同构(154 块全等价变换, 见 analysis/
  r24_032dc4_semdiff.txt)——差异不在函数体, 在**执行环境**；
  ③[ELC] 探针数据: 013(缺失=0)✓ 015(数值真=1)✓ 016(不等=0)✓ 但 031(等值=0✗ 期望1)、
  061(存在=0✗)；**031 未走到等值段**(cmp 探针仅 016 命中) → 早退路径待断;
  ④已修二处 Ghidra 伪影误清 0 (UVar19=extraout_EAX dc:30938 / UVar35=UVar30 dc:30963),
  031/061 未因此翻转 → 疑点收窄至 78b 决策前 UVar27 装配/01E69C 直查返回。
  下步: 78b 决策点探针(UVar27 内容+走 06F884/01E69C 哪支) 一次构建拿全。
- 工单队列(ROI 序): R24e#1 031/061 78b 决策探针→R24e#2 EVar27 装配对照→其后 B 同址归一
  转发+解码内核 / CALC 归正(6) / G dump(5) / F 参数族(7) / E TEAM(2) / 034788 T8 模式路径 /
  全套 TEMP PROBE T5 拆除(含 [ELC]×3+memfail 历史残留 8 处)。
- 提交链: ecfa8a1(C1) → 21e3be3(41/63 回归) → 1c533d6(交接) → 1f9d34c/3c2b7f4/d20417b/
  ffa39f7(R24d 文档) → 本批(031 取证产物+探针版)。

---

## R24f (2026-08-27) — CALC D簇归正 + 求值器链五连虫定案，基线 41/63 -> 46/63 零回归
- ★FUN_1400bf358 忠实移植落地 (core_calc_expr.c, 1007 行, dc:118318-119158 逐语句, 标号 21/21):
  替换 core_b3_remaining.c 旧"重构/简化版" (成功路径返堆指针 → exit=指针低32 的 D 簇根因)。
  子代理移植 + 主代理审查; 移植规格书 analysis/r24f_calc_port_spec.md。
- ★返回值契约 (原版活体对拍定案): CALC 成功返 0 / 除0 返 16 (pWVar6=0x10 via LAB_1400c0876
  赋值分支 pWVar5=pWVar6) / -err= 只影响错误文本不影响返回值; 值契约 11/13 逐位一致
  (7|4.5|17|21|1|-4|-5|2|4|7|2147483648|4294967296|0)。
- ★集成期五连虫 (windbg 硬件监视 + PageHeap 逐层伏击; 全部为既有移植体的潜伏缺陷,
  首次被 CALC 路径踩中):
  1) E34C 清字节 0x12 误置 (uint8/uint32 单位混用) → data1 指针第3字节归零 → 0xC0000005;
     原版清 0x28/0x48 (Ghidra 反汇编 @14007e36e/389 定案)。
  2) StrPBrkW 无原型/错原型 (win32_stub.h:888 BOOL) → C89 隐式 int → CDQE 截断 64 位返回 → 0xC0000409。
  3) 求值包装器按局部名释放向量 — MSVC 局部排布偏移 → 实释 op 栈 cap(=4) → HeapFree(-4) → 0xC0000374。
  4) MSVC 临时量 (uVar1) 恰好落在 ctx+0x10 (vec1-data 槽) → 结果值覆写向量指针;
     根治: 包装器 ctx 改堆分配 (calloc 0x60, 定案安全包络, PageHeap suffix 越写 0xF 也在此闭合)。
  5) repo FormatDoubleToStr 调 SafeVFormatW 仅传精度 p, 值不进 vararg → 成功值全部 "0"
     (解析侧 4294967296.0 已证; 静态判死 core_b8d.c:132)。
- 验收: 026/027/028/032/033 PASS (exit 0/16/0/0/0 全对金标), 015/018/034 (表达式案) 零回归,
  039 残留 exit=2 (TEAM 内层槽隔离, 归 E 簇); 全量 63 案 46/63 零回归。
- 非语料偏差登记 (forensics §7): `7%3` → msvc 7 vs 原版 1 (%-数字实参展开语义); `1.5+0.5` →
  -NAN(IND) vs 2 (ParseDoubleNumber 小数分支) — 两处待后续轮次。
- 附产: F 簇取证 (r24f_fcluster_forensics.md — 真分叉仅 029/040/049, 根因 SkipWCharUntil 空桩
  dc:149819 38B); T5 探针台账 (r24f_probe_audit.md — 61 标记/47 写盘全只读, [ELC]×3 保留);
  PRODUCTION_ROADMAP v4.2 (现代化裁定对齐 S14: L1 提前并行 / L2 归 S14+两项安全前置 / L3 非目标)。
- 提交 c9df979 (50 files, +1861/-239); 部署 md5=7d1b2bb9; 环境: PageHeap 演练后已撤;
  windbg T1 红线再犯一次 (bp 内嵌串) 已即时改正, 计入 windbg 纪律自查。

---

## R24f-b (2026-08-27) — F 簇工单闭合 + 浮点常量四联定案，基线 46/63 -> 48/63 零回归
- ★F 簇: PECMD_SkipWCharUntil/thunk_FUN_1400f429c 双桩按 dc FUN_1400f429c (dc:149819) 直移
  (unimplemented_stubs.c) → 029/040 翻转 PASS; 049 本体语义修复 (exit 仍被 FIND 掩蔽);
  048/050/058/062 用例形态案保持掩蔽 PASS; 全量 48/15 零回归 (F 簇工单闭合)。
- ★浮点常数四联 (Ghidra .rdata 定案): DAT_140126390=5.0 (旧 0.0 → 小数 0/0 → -NAN(IND)),
  DAT_140126398=(double)INT64_MIN, DAT_140121668=2^63, DAT_140124110=e。
  修复后 1.5 系由 -NAN → "整数部" (残留小数尾截断, 机理锁在箍环语义, 待原版汇编定谳 —
  非语料路径已登记, 不阻塞对拍)。7%3 %-数字展开差另案登记。
- 提�交 1b290f4 (29 files); 部署 md5=dcc512a7; 全量复跑确认 48/63。

---

## R24f-c (2026-08-27) — 夹具 U-2 升级第一步 (vars_val 变量值回捞) + B 簇档案，全量 48/63 稳
- ★夹具: make_epilogue 新增 vars_val.txt 工件 (manifest.vars_val=true 显式开关, 默认关 —
  实测批量 63 案下追加第三条 EXEC 会放大 EXEC 通道偶发哑火 (33/63), 故 opt-in)。
  形态 'CASE=<id>|A=5|B=3|R=gt!'; diff_case 不比对 ⇒ 金标/判据零扰动, 全量复跑 48/63 稳。
  首发证据: 015 IFEX 真臂 msvc R=gt = orig R=gt (分支语义 exit 单维外首次可观测);
  010 F 值双方一致; 026 CALC-%R%-echo 空值边缘已记录。
- ★B 簇档案 (forensics §9): 原版 FILE 契约活体定案 — 复制成功/删除成功/源缺失全返 2
  (pb1-pb4; 金标 2 = SHFileOperationW 缺文件错 2, 010 源在用例时序中恒缺); msvc 症状 =
  010/012=87 (SHFO 参数非法), 011=0; 现场 struct hwnd/wFunc=0x18/0x1a 垃圾 + pFrom/pTo
  悬垂 (栈覆写/释放时序, R24f 同族) — 症状级登记, 专项调试工单下轮。
- 提交 24549a0 (harness/runners/run_case.py + forensics §9 + 结果工件)。

---

## R24d (2026-08-27) — G簇首翻 (060) + C4013 隐式 int 地雷扫雷，基线 48/63 -> 49/63 零回归
- ★041 MDIR AV 定案 (live): PECMD_CreateDirectoryTree @rax=0xfffffffff621cea2 (real
  0x258f621cea2) — core_b2a.c 334/388 未声明 FUN_14001BE14 → C4013 隐式 int → CDQE
  截断 — 与 R24-013/b2f 同款 (b2f 曾修、b2a 漏) — 补 extern → 060_path_special 翻转 PASS;
  041 前进至 B簇87 (MDIR 通 → 其 FILE 行承接 B 症状)。
- ★C4013 全库扫雷: 构建日志 93 个不同名警告; 指针/HANDLE 返回型高危 7+ 列入
  analysis/r24d_c4013_minefield.md; 本轮补 extern ×6 (b2a/b3a/b3r_g/b3r_d/b8i/b3r_g3)
  — 预防性 (下一位触发者已预拆)。
- 全量 63 案 49/14 零回归; 提交 2cb7fca; 部署 md5=f9872ef7。
- 注: windbg 通道 T2/T3 故障面在本轮两次复发 (bp 链式自毁/快退 parse) — 已按
  WINDBG_MCP_FAULTS_REPRO 规避三律处置; 构建前会话清理纪律 (LNK1201) 复犯一次已即时收。

---

## R24d-b (2026-08-27) — B 簇三侧同构定论: dc ≡ 源码 ≡ 编译产物, 87 机制锁 live 层; G 簇 060+1
- ★构建: core_b2a.c 补 FUN_14001BE14 extern (C4013 隐式 int → CDQE 截断, 与 R24-013/b2f
  同款、b2f 曾修 b2a 漏) → **060_path_special 翻转 PASS** → 全量 49/63 零回归; 041 前进至
  B簇87 (MDIR 已通). C4013 扫雷 93 警告/指针返回型清单 → analysis/r24d_c4013_minefield.md,
  预防 extern ×6 (b3a/3r_g/3r_d/b8i/3r_g3 + b2a).
- ★B 簇终局: 当前构建导入 Ghidra 反编译 (auto 分析 8158 函数) — FUN_14003e710 与 dc
  逐段同构 (选项/wFunc/清零/双槽/GetLastError 链); 新证据点 local_68 恒 0 终止符写入三侧
  一致 ⟹ "清 pFrom" 理论排除; 87 唯一差异渠道 = SHFO 运行时实参内容 (live/堆对比), 下轮收口.
- 提交: 2cb7fca (R24d) + 03d1e13/… (对照结论); 部署 md5 f9872ef7; 全量 49/63.

---

## R24d-c (2026-08-27) — B 簇探针定案 (空路径 + garbage 结构实锤), 全量 49/63 稳
- ★TEMP PROBE (Win32 通道, 禁 stdio — V4 纪律): FUN_14003C06C 双 SHFO 调用点前
  打印结构字段 + 内容; 011 现场 = `hwnd=18 wFunc=1a pF=…F5F8 pT=…385A srcS=[] dstS=[]`
  — pFrom/pTo 内容为空串 + 结构首 8 字节为栈残留 (未初始化) ⟹ SHFO 87 (ERROR_INVALID_PARAMETER)
  的机制 = 空路径参数。
- dc-len 机制复原: FUN_1400216c4 向 {data,len,cap} 对象追加 (len 槽 = 尾部 local_68 别名,
  终止符 data[len]=0 落串尾); msvc 移植体 PECMD_AppendParamToken (b2b:272) 同样维护
  list[1]=len ⟹ 空内容 = token 切片阶段 (LVar21) 已空/追加被跳过 — 下轮在 append 调用点
  加第二层探针抓 token 到达实况。
- 全量 63 案 49/14 零回归 (探针构建); 提交 aC (R24d-c 探针定案); 部署 md5 dc543d7a;
  TEMP PROBE 已标记 T5 待拆。

---

## R24d-d (2026-08-27) — B 簇二级探针实验 + 回滚, 全量 49/63 稳
- 二级探针 (ENTRY/APP/SHFO): 011 入口参数正确 (len=25, str 完整) — 空路径不在传参层;
  探针构建间行为漂移实证 (87↔124 挂起) → V4 透明性违反记录, git checkout 回滚干净基线。
- 干净基线 md5 a18a8964: 010=87/011=0/012=87 (与 triage 一致), 全量 49/14 零回归。
- B 簇残余: 终止符 local_68 栈别名候选 — 下轮以不改代码的 windbg 单步/dh 堆对比收官。

---

## R24d-e (2026-08-27) — B 簇挂起实锤: MessageBox/HeapRealloc/AppendParamToken 链, 已回滚基线
- ★011 挂起根因实锤 (live 栈×2): USER32!MessageBoxW ← FUN_1400630D0 ← HeapRealloc ←
  AllocString ← **AppendParamToken** ← FUN_14003C06C — append 收集巨量 count → 分配失败
  弹"内存不足"阻塞 (124 = harness 超时杀)。
- 修复尝试 A (终止符显式 len) / B (+槽清零) 均未解 (同栈复现) ⟹ len 槽模型不完整;
  bp AllocString 轨迹 11 连击全 NewVarNode (ENVI 节点), append 帧未现。
- 已回滚 2cb7fca 干净基线 (md5 a8b828ac): 011=0/010=87; 下轮方向 = bp AppendParamToken
  入口读 list[1]/list[2] 实值, 或先令 HeapRealloc 失败静默化以暴露真实计数。
- 全量基线 49/63 (回滚后未复跑, 下轮首查); 提交 2456eba。

---

## R24e (2026-08-27) — ★B 簇收官: StrBld 对象结构体化, 010/011/012/041 全翻, 49→53/63 零回归
- ★根因定谳: dc FUN_14003c06c 以三邻接局部 (local_70/68/60, 58/50/48) 拟 StrBld 对象
  {data,len,cap}, 1400216c4-append 经 &local_70 以 list[1]/list[2] 写 len/cap — 邻接依赖
  编译器布局, MSVC 优化分离放置 ⟹ len 槽读栈残留: (a) 终止符 data[0]=0 清空 pFrom/pTo
  → SHFO 87 (010/012), (b) 残留巨大 → append AllocString 巨量 count → HeapRealloc 失败
  → MessageBoxW 弹窗挂起 124 — 三态同根 (四个多轮探针/活体证据链闭合: 探针 srcS=[]
  + MessageBox 栈 + 编译产物布局对照 + 三态波动)。
- ★修复: 两组三局部 → 单块结构体 `struct { WCHAR *data; int64_t len; uint64_t cap; }
  strb1/strb2` — 24B 连续布局由类型系统保证, 别名依赖彻底消除 (dc 语义等价)。
- ★验收: 010=2 ✓ 011=2 ✓ 012=2 ✓ 041 (MDIR+FILE 组合) = 2 ✓; 全量 63 案 53/10
  零回归 (10 FAIL = 纯已知队列 E×3/J×2/G×2/H×2/I×1); 提交 170bacf; 部署 md5 60e5b054。
- 下一站: 同款"三邻接局部拟 StrBld 对象"模式全库排查 (grep 相邻 LARGE_INTEGER+
  int64 声明组) — 同类潜在地雷; 队列队首转 G (051/057)。

---

## R24e-b (2026-08-27) — G 簇 051 翻转, 全量 53→54/63 零回归
- 051 live 定案: SplitNextToken 无声明 (S11 删声明未果, 文件不含 xproto) → C4013 →
  返回截断 (0x17096823d0c→0xffffffff96823d0c) → AV; 按定义签名补 extern → 051 PASS。
- 057 FORM: NULL-deref @ *pWVar10 (尾段应为 local_f8) — 待 bp 定向; b3r_h1 C4013 ×3
  同族登记。提交; 部署 md5 24219467; 全量 54/9。

---

## R24f-a (2026-08-27) — G 簇 051 翻转后收尾: 057 FORM 对照登记 + C4013 地雷区清零
- 051 SIZE: SplitNextToken extern 补齐 → 翻转 (53→54/63, 全量 54/9 零回归, md5 24219467)。
- 057 FORM: Ghidra 编译形态全流程对照 (两分支终点皆 local_f8 非空) — pWVar10=NULL
  疑 C 流/goto 结构与编译分歧, 需 windbg bp 定向 (本日通道故障 3 次) — 已登记 §13。
- C4013 minefield 终版: 90 名全核, 指针返回型 7 修/3 豁免 (家族 4 次命中记录)。

---

## R24f-b (2026-08-27) — 053 HASH 0xC0000374 首现场 + C4013 地雷区复核闭环
- 053 live: sxe c0000374 抓帧 — RtlFreeHeap 侧, pecmd 链: SynthDlgKeyMsg 体 →
  RegiEditRegistry (0x151920) → PSB; 053 脚本无 REGI 行 → REGI-EDIT 帧待 bp 确认;
  首写者待 PageHeap 点名 (下轮 IFEO 已备)。
- 057 FORM pWVar10-NULL 待 windbg 通道恢复 (登记 §13); C4013 地雷区终版 (7修/3豁免)。

---

## R24f-c (2026-08-27) — 053 PageHeap 点名: CRT vsprintf '%g' 参数槽守卫页命中
- PageHeap (IFEO, 用后即撤) + sxe av: 首违例 = __crt_stdio_output 内联链
  (fp_format_g → common_vsprintf → __stdio_common_vsprintf_s) 读堆区参数槽 — vsprintf
  收到的 '%g' 双精度参落在守卫页邻域; 用户帧 = 0xeab0-0xebc0 (b2a 小函数区, 符号名
  与 053 语义不符) — 下轮 bp vsprintf 入口打印格式串确认发起者。
- 全量基线 54/9 未动。

---

## R24f-d (2026-08-27) — T5 探针拆除完成 + 053 阴性
- 拆除 core_main×3 + core_init PROBE_STEP（memfail.log 探针 + 违规 stdio.h）;
  重建 md5 85f92a27; 053/056 仍 0xC0000374 = 探针非病因（阴性）。
- 053 下一招: 干净构建上 bp 0xEB60（swprintf 包装）直达真实格式串调用链。
- 全量基线 54/9。

---

## R24f-e (2026-08-27) — 053 三现场: FreeStrBuf@HASH 尾, 疑点收窄至 CryptoHashCompute
  契约 / AssignString 空桩
- 干净构建首帧符号化: RtlFreeHeap ← PECMD_FreeStrBuf+0x31 ← PECMD_HashCmdCompute+0x7b8;
  HASH 尾三释放中 wslot/ansi_slot/list_alloc 画像完成 — 下轮 bp 371-free 前查 wslot
  实值; PECMD_AssignString 空桩 (restored_bodies:7725) 契约缺口登记。
- 全量 54/9 基线。

---

## R24f-f (2026-08-27) — 053 四现场: wslot 头垃圾实锤, res_src 源头待探
- bp HASH+0x7b3: wslot=0x2177db0e460 (头无 StrBld 魔数, 内容" ") — 释放非法块成因
  落定; 0637DC 端口复核无恙 ⟹ 疑点前移至 StrBldCopyAnsi 的 res_src 入参 (bp 探针
  下一招)。全量 54/9。

---

## R24f-g (2026-08-27) — T5 核心拆解完成 + 053 探针干扰确认排除
- 拆: core_main×3/core_init/core_b2f×3/core_string×4 (OOM 系+ELC 系, 禁 stdio 恢复);
  构建 md5 1e1df48e, 全量 54/9 零回归。自动化正则误伤 11 文件 → git 还原 + 逐块
  read+edit 重做 (教训入档 §19)。
- 053: vsprintf 链 = 探针 printf (干扰实锤, 阴性互证); 病灶 = HASH 尾 wslot 释放
  (§18 四现场), 下轮首查 res_src/hash_out 源头。
- 存留探针清单: exec2/exec4/execmain/script2(S7 依赖)/scriptrun/scriptdep/var/b9/b3e/h3。

---

## R24g (2026-08-27) — 053 五现场: 摘要未产 (CryptAPI 链待首查)
- bp StrBldCopyAnsi@HASH+0x73b: res_src = 静态 hb_align 且仅" " — CryptoHashCompute 未
  写 hexbuf ⟹ CryptAcquireContextW 链失败/跳过 (下轮首探); wslot 头垃圾另列 ba 监视
  终局。全量 54/9 基线。

---

## R24g-b (2026-08-27) — 053 六现场: 值通道裁决 (H 两侧未置), 差异收窄至 wslot 槽覆写
- vars_val 双跑: orig H 空 = msvc-同 — acquire-0xF0000000 = dc 135 处原版同型 —
  均非差异点; 剩余 = wslot 槽二次写坏 → ba w8 槽监视终局 (或 ' '-execute 链验证)。
- 全量 54/9 基线。

---

## R24-最新现状 (2026-08-27, 部署 md5=1e1df48e, hash=cb9fcba, 全量 54/9)

### 53-H 簇 (053/056, 0xC0000374) — 六现场, 病灶收窄至 wslot 槽覆写

- 已排除 (值通道/静态/活体三轮裁决):
  1) T5 探针干扰 (core_string/core_main/core_init/core_b2f 的 printf 探针 = vsprintf 链真相,
     已拆除, 禁 stdio 红线恢复);
  2) CryptAPI 链 (map 证实为真 advapi32 链接, 非桩; dc 全库 135 处 0xf0000000 同型,
     acquire 失败为两侧共有 — digest 缺失非 msvc 独有);
  3) 传参层 (053 的 HASH 'H,%F%' 解析: varname='%F%' 展开路径, 原版 H 也同样未写入
     H 变量 — 值通道 vars_val 裁判 H 两侧皆空);
  4) res_src 源头 (StrBldCopyAnsi@HASH+0x73b 实读: rdx=模块静态 hb_align 区, 内容仅" "
     = hexbuf 初值 — 摘要从未写入, 但此与 CryptAPI 共有结论一致, 已被排除为差异点)。
- 现存差异唯一 = **wslot 槽 (HASH 尾局部) 被二次写坏**: wslot@rsp 槽含指针
  0x2177db0e460, 其 -8 块头 = {0x8e0001451caea860, 0x20} 无 StrBld 魔数/尺寸语义 =
  非分配器物; 而 StrBldCopyAnsi 端口 (restored_bodies:7721, '*a=0; 0637dc(a,b,c,~0)')
  语义正常 (AllocString→conv→缩容, 头由 HeapRealloc 维护) ⟹ 槽污染发生于复制之后、
  释放之前 — 即 SetVariable(varname='C:\...路径', wslot+2) / FUN_1400BEF64(" ")执行 之一。
- **终局手段 (已武装, 下一次 launch 即用)**: bp StrBldCopyAnsi(2 地址 bm) → 命中取
  rcx=&wslot → `ba w8 @rcx` 槽监视 → 继续 → **写者即停** (区分: 复制期自身写 vs
  槽覆写的第二写者). 候选写者: (a) FUN_1400BEF64(" ") 执行空行命令链; (b) SetVariable
  以 'C:\...' 为 key 的节点插入覆栈; (c) wslot+2 越界读后相邻局部写串扰。
- 053 用例 053_hash_probe 已开 vars_val (manifest, 值通道可用); golden=2 (尾部
  BODY_DONE 非零守卫, 非 HASH 行返回值 — 053 无尾 IFEX, exit 来源=结束时的槽值链)。

### 下一步 (H 簇本轮优先)
1. ba w8 wslot 槽监视抓第二写者 → 定根 → 修 → 053/056 双案, 期望全量 +2。
2. 若写者为 FUN_1400BEF64(" ") (空行执行) → 对照 dc 该分支 (空/纯空白 varname 的
   HASH 不应执行空行命令 — dc:119318/119358 调用点上下文) 定案。

### 其余队列 (未动)
- T5 存留清单: exec2/exec4/execmain/script2(S7-bisect 依赖, 注释明示)/scriptrun/
  scriptdep/var/b9/b3e/h3(R14b) — 逐块 read+edit (正则曾误伤 11 文件, 教训在 §19)。
- G/H 剩余: 057 FORM (pWVar10=NULL, Ghidra 编译形态对照§13, 需 bp), 056 TEMP (同 H)。
- P2: 051 已翻 (SplitNextToken extern); I 055 (fastfail); E TEAM (024/025/039);
  J 031/061 (深水); 非语料二项 (小数箍环/7%3); divergences 登记簿; 同类 StrBld
  邻接排查 (B簇结构体化模式推广); U-2 vars_val 普及。

---

## R25-a (2026-08-28) — T5 探针全网清零 (src 10 文件 + restored_bodies 8 处), 全量 54/63 零回归

### 起点复核
- 部署基线: hash=cb9fcba md5=1e1df48e (R24g-b), 与 R24 账本"全量 54/9"一致。
- HEAD 领先部署 2 笔纯文档提交 (3ca3c77/ebb4366: windbg MCP #242 修复后认知更新),
  源码零差异 ⟹ 账本现状可直接继承, 无需重建基线。

### 基线实测 (独立复核, 非采信账本)
- 全量 63 案 EXE=msvc 跑批 + diff_case: **54 PASS / 9 FAIL**, 与账本一致。
- 9 FAIL 全部落在 exit 单维 (stdout/vars/fs 三维全同) — 印证 U-2 (vars_val 值通道)
  普及的必要性: 现有裁判对"崩溃但产物恰好相同"的用例只有 exit 一维可辨。
- 失败清单: 024/025 (0xC0000374), 031/039/061 (exit=2), 053/056 (0xC0000374),
  055 (0xC0000409 fastfail), 057 (0xC0000005 AV)。

### T5 清理成果
- **拆净**: SG_Probe 定义 + 8 调用点 (core_scriptdep 定义本体 / core_script x2 /
  core_b3e x2); memfail.log fopen 探针块 (restored_bodies x8 / core_b3r_h3 x4 /
  core_scriptrun x5 / core_exec4 x2 / core_execmain x2 / core_exec2 x1 /
  core_var x1 / core_b9 x1); 死 `<stdio.h>` 包含 x4 (禁 stdio 红线全面恢复)。
- **方法**: python 大括号配对精确删除 — 沿用 R24f-g 教训 (§19: 正则曾误伤 11 文件,
  被迫 git 还原重做)。本次零误伤。
- **净减**: 224 行删除 / 20 行新增 (新增均为空行与 1 行拆除注记)。
- **构建**: 双绿门 (syntax + full) 全过; 部署 md5 **a63b0303** (hash 3ca3c77)
  → 后续 1b6a17a 提交后未重建, 源码等价 (仅空白), 身份戳沿用。
- **回归**: 全量重跑 **54/63 零回归** (两轮独立跑批确认: 探针拆除前后同为 54/9)。

### 关键副产物 (值通道裁决)
- 053 的 orig vars_val = `H=%H%|%H%=%H%|BODY_DONE=YES` — **H 变量原版侧同样未写入**,
  复证 R24g-b 六现场结论 (H 非差异点, 差异唯一在 exit: 0xC0000374 vs 2)。
- 055 的 orig exit = 2147942487 (0x80070057 = HRESULT_FROM_WIN32(ERROR_INVALID_PARAMETER))
  — **原版是优雅报错退出而非崩溃**, msvc 的 0xC0000409 是 /GS 栈保护触发;
  且 orig vars_val `G=%G%` 证明 SED 两侧同未赋值 ⟹ 目标 = 优雅退 0x80070057。
- 057 的 orig vars_val `R=%R%` 同上 (FORM 两侧同未产出) ⟹ 目标 = 正常 exit=2。
- ⟹ **三案的验收口径收窄为"退出码对齐", 不含功能产出** — 大幅降低修复面。

### 存留探针
- `src/lang/core_script2.c` (19 处) **按账本有意保留**: 文件头注释明示 S7 bisect
  依赖 (S7_BRIDGE_EXECUTOR / S7_MARK), 链路接通后一并移除。

### 已派分析线 (R25 并行子代理, 6 线覆盖 9 个失败案)
| 线 | 目标 | 子代理 |
|---|---|---|
| A | 061 LOGS 变量表损坏 (`BODY_DONE`→`BO` 截断征兆) | 8e4c9d68 |
| B | 031/039 SET-TEAM exit=2 共同根因 | af3d2f9b |
| C | H 簇 053/056 wslot 槽二次写坏 (六现场续) | ffae4613 |
| D | 055 SED fastfail (栈缓冲越界) | b2f479d1 |
| E | 057 FORM pWVar10 NULL-deref (§13 续) | 735e026e |
| F | E 簇 TEAM 024/025/039 + 与 H 簇同根性交叉验证 | 4b991222 |

### 下一轮首办
1. 收敛 6 线子代理交付物 (analysis/r25_*.md), 逐条核 dc 行号证据后合入源码。
2. 每合入一簇即重建 + 全量回归 (禁止批量合入后一次性验证 — 归因代价过高)。
3. 存留项: script2.c 探针 (S7 依赖); U-2 vars_val 普及; divergences 登记簿收口;
   同类 StrBld 邻接排查 (B簇结构体化模式推广); 非语料二项 (小数箍环/7%3)。

---

## R25-b (2026-08-28) — 工具 ROI 复盘 + 总进度量化 + 6 线战术校准

### 一、工具 ROI 复盘结论（windbg 前高后低）

基线推进曲线与各轮主要手段（账本实证提取）：

| 轮次 | 基线 | 增量 | 主要手段 |
|---|---|---|---|
| R19 | 0 → 9/63 | +9 | **dump 破案** (open_dump+backtrace+.ecxr+ub retaddr) |
| R20 | 9 → 17/63 | +8 | V-Gate 纪律 + 探针 |
| R23 | 17/63 | 0 | 持平 |
| R24 | 17 → 22/63 | +5 | **活体 attach 抓挂死栈** (021/037→FUN_14001b23c; 002/004/038→FUN_14007A224) |
| R24b | 22 → **41/63** | **+19** | 全量分型（最大跳跃，非调试器） |
| R24d | 41 → 49/63 | +8 | 静态 extern 补齐 / C4013 扫雷 |
| R24e-b | 53 → 54/63 | +1 | SplitNextToken extern（静态，一轮翻转） |

**结论**：windbg 不可替代的只有三类「运行时状态」问题 ——
① 崩溃栈回溯 (dump/AV/fastfail)；② 挂死进程抓栈；③ 堆损坏首写者 (PageHeap/`ba w`)。
**反例（拖后腿实证）**：057 FORM 挂两轮「待 bp 定向 → 通道故障 3 次 → 未定位」；
053 HASH **六轮活体取证至今未定根**（仅排除 4 假设），而同期静态的 051 一轮翻转。

**结构性原因**：剩余 9 FAIL 多为「还原代码与 dc 的控制流映射错误」（goto 目标/缓冲区尺寸/
签名契约）—— 这类读 dc 原文逐行对照即可定位，windbg 只能答"崩在哪"，答不了"该是什么"。
两条历史误诊亦印证：R18 断点定论撤销（**地址未随重建更新** = 活体取证的地址依赖软肋）；
B 簇 exit=183 实为**探针 fopen 污染 GetLastError**（观测污染被观测对象）。

**据此建立分诊表**：

| 用 windbg（运行时状态） | 用静态 dc 对照（更快） |
|---|---|
| 崩溃栈回溯 (dump/AV/fastfail) | 控制流映射错误 (goto/分支) |
| 挂死进程抓栈 | 签名/返回值契约 (extern) |
| 堆损坏首写者 (PageHeap/`ba w`) | 缓冲区尺寸/类型不匹配 |
| 探测未知运行时值 | C4013 隐式 int 扫雷 |

⟹ 9 FAIL 的分派：**4 案该用 windbg**（053/056/024/025 堆损坏）、**5 案该走静态**
（057/055/031/039/061）。

### 二、总进度量化（与 ROADMAP §2.3 同口径本轮实测）

| 指标 | 2026-08-24 基线 | R25 实测 | 消除 |
|---|---|---|---|
| M1 FUN_ 引用 | 4791（唯一 737） | 4675（唯一 759） | 2.4% |
| M2 local_xx | 26565 | 26931 | **−1.4%（倒退）** |
| M3 param_N | 13888 | 14039 | **−1.1%（倒退）** |
| M4 DAT_/PTR_ | 2632 | 1424 | **45.9%** ✅ |
| M5 TODO(verify) | 423 | 410 | 3.1% |

**警惕信号**：M2/M3 不降反增 ⟹ 过去 4 天投入**全在"修行为 + 清探针"，L1 语义化
（ROADMAP §P3）实际零推进**。且残留高度集中：**M2+M3 共 40970 处，前 10 文件占 82.1%**
（core_b3_remaining 12060 / restored_bodies 9520 / core_b2f 2596 / core_b7c 2459 /
core_b1_remaining 2907 / core_b3r_h1 1094 / core_b3r_h2 860 / core_b3r_h3 837 /
core_b3r_i28c 726 / core_b8_remaining 583）⟹ 若启动 L1，攻前 10 文件即可拿下 82%。

**功能覆盖（真正的缺口）**：FourCC 权威 98 动词（含 IPAD/EDIT 表外 2 则 95 可核），
语料覆盖 **28/95 = 29.5%**，**67 个动词零语料验证**（清单见下）。按 ROADMAP §3.3
「长尾登记 divergences、不追求全量清零」，此 67 个**可合法挂账**，但其还原代码目前
处于简化桩/未验证态 —— 需在 divergences.md 明示。

未覆盖 67 动词：ADSL BASE BROW CMPS COME DATE DEVI DFMT DISK DISP DOWN DTIM EJEC
EXIT FBWF FLNK FONT GROU HELP HIDE HOME HOTK IMAG INIT ITEM KILL LIST LOAD LOCK
LOGO LOOP LPOS MAIN MEMO MENU MESS MSTR NAME NOTE NTPC NUMK PAGE PBAR PCIP PINT
PUTF RAMD RAND SBAR SCRN SEND SERV SHOW SHUT SITE SLID SOCK SPIN SUBJ SWIN TABL
TABS TEXT THRD TIPS TREE USER

**已覆盖 34 命令的通过率**（按案统计）：100% 的有 ENVI/SUB/FILE/READ/LSTR/REGI/CALL/
MDIR/FORX/FDIR/FEXT/RSTR/RPOS/SIZE/DIR/CODE/RECY/LINK/PATH/STRL/SSTR/EXEC/GETF；
偏低：TEAM 25%(1/4)、IFEX 78.6%(11/14)、CALC 83.3%(5/6)、FIND 86.7%(13/15)、
WRITE 92.3%(12/13)；0%：SET/HASH/SED/TEMP/FORM/LOGS。

### 三、6 线战术校准（本轮已下发）

基于上述分诊表，对已派子代理做定向纠偏与情报补送：

| 线 | 目标 | 校准动作 |
|---|---|---|
| 057 FORM | 控制流映射错误 | **战术纠正**：从「待 bp 定向」切换为 **dc 逐行静态对照优先**，bp 降级为验证手段 |
| 055 SED | 栈缓冲越界 | **战术纠正**：同上改静态；并送达**验收口径收窄**（原版为优雅退 0x80070057，非崩溃） |
| 053/056 H | 堆损坏 | **维持 windbg**（PageHeap/`ba w` 唯一解）；送达 H 非差异点复证 + 空桩 AssignString 线索 |
| 024/025/039 E | TEAM 堆损坏+退出码 | **维持 windbg**；送达 025/039 原版真值锚点（TEAM 功能真实生效，须真修） |
| 031 SET | 退出码 | 聚焦 SET（039 归 E 簇线），送达 EvalSpecialToken mode=1 线索 |
| 061 LOGS | 内存覆写 | 送达**字节级损坏模式分析**（22 字节处断裂 + `BODY_DONE`→`BO` 变量名截断） |

**验收口径重大收窄（本轮实测，已全员下发）**：
055/057/053 三案的原版真值证明**这些命令在原版侧也未成功产出**
（`G=%G%` / `R=%R%` / `H=%H%`）⟹ 验收口径为**退出码对齐**，不含功能产出。
**但 025/039/031 相反** —— 原版真值 `A=1|B=ok`、`X=6|R=chain_ok`、`A=pecmd|R=setok`
证明 TEAM/SET 功能**真实生效**，这两簇**必须真修，不可只求退出码对齐**。

### 四、后续任务规划（按 ROI 排序）

**P0 — 9 FAIL 收敛（进行中，6 线在跑）**
1. 等 6 线交付 `analysis/r25_*.md`，逐条核 dc 行号证据（无证据者打回）。
2. **逐簇合入 + 单簇重建 + 全量回归** —— 严禁批量合入后一次性验证（归因代价过高）。
3. 期望 63/63；若某簇三轮未收敛，按 ROADMAP §3.3 登记 divergences 合法放弃。

**P1 — L1 语义化开线（本轮新立，因 M2/M3 倒退而提级）**
- 攻前 10 高浓度文件即可拿下 82% 残留（40970 处中 33600+）。
- 依据 ROADMAP §P3「与验证循环并行，不等待 S14 门链」，且 L1 属纯命名零行为风险，
  可与 P0 并行。**每改一批即全量回归已绿语料（防 R4 风险：改坏已绿用例）**。

**P2 — 67 未覆盖动词登记**
- 按 v4 定位合法挂账，但须在 `docs/divergences.md` 明示"简化桩/未验证"状态，
  不可让参考库读者误以为已对齐。

**P3 — 存留项**
- `src/lang/core_script2.c` 探针 19 处（S7 bisect 依赖，有意保留）
- U-2 vars_val 普及（现仅 1/63，有 EXEC 哑火风险，需谨慎灰度）
- 非语料二项（小数箍环 / 7%3）

---

## R25-c (2026-08-29) — 061 双 bug 定案 + SET 变量创建真体化, 基线 54→55/63 零回归

### 落地修复（全部 dc 行号证据锚定）
1. **061 LOGS=183 根因**: `FUN_140003864`(CreateFileW 包装, dc:1179-1196) 在
   unimplemented_stubs.c 是**恒 0 桩** → LOGS 开日志 `lVar21==0` 恒真 → GetLastError
   残留(183) 泄漏为退出码。**修复**: 桩改为转发 `PECMD_OpenFileHandle` 真体
   (core_exec2.c:173) 并返回句柄; 同步 stubs_common.h/xproto.h 声明 int→uint64_t。
2. **061 IFEX 命中已存在文件 AV(0xC0000005)**: core_b2f.c `PECMD_IfexFindExecutor`
   真分支 EvalLoopCondition 入口 `UVar30` 哨兵被 FindFileOrDir 覆写 → `*pWVar41(NULL)`
   解引用。**修复**: UVar30 拆分 probe_ui 独立变量 + 入口清理(探针版先导)。
3. **路径前导空格**(FUN_1400170B0): unimplemented_stubs.c **no-op 桩** → LOGS/IFEX
   等路径解析不跳前导空格 → CreateFileW 收到 `" C:\..."` → ERROR_INVALID_NAME(123)。
   **修复**: 桩转发 `PECMD_SkipLeadingControls` 真体(core_b1_remaining.c:5827)。
4. **SET 变量创建**(PECMD_AddVarDefault): unimplemented_stubs.c **恒 0 桩** → SET 创建
   变量返回 NULL → EnviMemReadWrite 返回值 = iVar20=1 (退出码污染) 且变量未创建。
   **修复**: 按 dc:18077-18113(FUN_14001e5b0 size=236) 直移真体(建节点→写值/容量
   →表计数+1→扩容→追加); 同步 stubs_common.h 声明 void*。

### 验收
- 最小化: `SET A=pecmd` exit 1→**0** ✓; p3/p4 IFEX 存在文件=0 ✓; p5 IFEX 不存在=2 ✓。
- **061_logs_smoke 翻转 PASS**(exit 0 = golden); 全量 63 案 **55/8** 零回归
  (FAIL 仍 = 024/025 E簇 + 031/039 J簇 + 053/056 H簇 + 055 I簇 + 057 G簇)。

### 031/039 深水定位证据 (J 簇, 下轮线索)
- p8 `SET A=pecmd`+`FIND $%A%=pecmd` → exit 2 (SET 创建变量 % 查不到)
- p13 `ENVI A=pecmd`+`FIND $%A%` → exit 0 (ENVI/环境变量路径正常)
- p18 `CALC X=6`+`FIND $%X%` → exit 0 (CALC 普通行正常)
- p19 `TEAM CALC X=2*3|FIND $%X%` → exit 2 (TEAM 内失败)
- 活体: AddVarDefault script=param_1(堆, count=10, +0x38=0, 表数组含 0/1 垃圾)
  ≠ g_Script(静态, count=1 仅 PECMDVER); IfexFindExecutor script == param_1;
  FUN_14001E69C 真体正常(core_var3.c:31)。**两向收窄**: ① SET 创建的表结构
  与 VarLookup 遍历不一致; ② TEAM 内 cmd2 的 %X% 展开时机(可能拆分时过早展开)。
- 下一步: bp PECMD_ExpandVarDispatch 全命中审计 %X% 展开顺序 / 对照 dc 原版
  EnviMemReadWrite param_1 语义(可能 msvc 传参对象类型错误)。

### 057 FORM AV 定案 + PECMD_AssignString 真体化 (G 簇)
- **057 FORM V,C: = 0xC0000005** 根因: `PECMD_AssignString`(restored_bodies.c:7670)
  是**空桩**(恒 NULL 不赋值) → FORM 参数解析 local_128 保持 NULL → `*local_128`
  解引用 AV。**修复**: 按 dc:70512-70528 (FUN_14007034c size=74) 直移真体
  (释放旧值 ptr-8 + PECMD_StrDupAlloc 复制 + 写回)。**057 翻转 PASS**(exit=2)。
- 注: dc 原文 FUN_14007034c 释放旧值, 与 FUN_1400702B0(StrDupAssign 不释放)语义不同,
  不可互相转发。

### 055 SED fastfail 定案 + GenerateTextContent 栈布局修复 (I 簇)
- **055 SED G,sed55seed = 0xC0000409** (FAST_FAIL_STACK_COOKIE_CHECK_FAILURE)
  根因: `PECMD_GenerateTextContent`(core_b3r_h1.c:669) 局部 `local_98[3]`(24B)
  但 `memset(local_98,0,0x58)`(88B) 按 dc:85202 直移 → msvc 布局下越界 64B
  覆盖 /GS cookie。dc 的相邻变量(local_80/78/70/68/60/58)在 msvc 重构中省略,
  使 memset 越界暴露。**修复**: local_98[3] → local_98[11](88B 容纳 memset,
  行为等价——被覆盖的相邻变量在 msvc 中无独立引用, local_58 由 :989 显式赋值)。
  **055 翻转 PASS**(exit=0x80070057, 与原版金标精确一致——优雅参数错误)。

### 全量 flaky 观察 (010/054)
- 全量跑批(63 案)期间 010/054 间歇 FAIL(0xC0000374/0xC0000005), 单跑连续 3 次
  全 PASS。疑为 run_case 批量跑批的用例间状态/时序(与 R24f-c EXEC 通道哑火
  同族), 非 AssignString/数组修复的确定性 bug。二次全量 010/054 均恢复 PASS。

### 053/056 H 簇六现场终结 + StrDupA 分配契约修复 (H 簇)
- **053 HASH = 0xC0000374** (R24g-b 六现场 wslot 头无魔数) 根因: `FUN_1400637DC`
  (core_exec5.c:180) 用 **calloc**（无 PECMD 头）+ **memcpy**（无 ANSI→宽转换）
  → HASH 尾 `PECMD_FreeStrBuf(&wslot)` 释放 `ptr-8` 非法块。**修复**: 按
  dc:60925-60948 (FUN_1400637dc size=171) 直移 —— `PECMD_AllocString`(带头 hdr+8)
  + `MultiByteToWideChar`(ANSI→宽, CP_ACP)。**053/056 双案翻转 PASS**。
- 活体验证链: SetVariable key=路径(dc 语义) → wslot 释放崩 → 块头无 {size,0xaa55}
  → StrDupA calloc 无头实锤。同族风险: 任何 StrDupA 结果用 FreeStrBuf 释放的调用点
  均已受益; 需审计残余 `free()` 释放 StrDupA 块的调用点。

### 当前基线 (R25-c 终)
- **59/63 PASS** (FAIL 4 = 024/025 E簇 TEAM + 031/039 J簇 %A%展开深水)。
- 本会话累计 +5: 061(LOGS+IFEX) / 057(AssignString) / 055(SED 栈布局) /
  053+056(StrDupA 分配契约)。
- flaky 池: 010/054 (全量间歇, 单跑稳定, 已记录)。

### 存留
- unimplemented_stubs.c 内仍有多处恒 0/no-op 桩待查(同类风险, 如 FUN_14005EA5C/
  PECMD_InstallKeyboardHook 等); 本次还原的 AddVarDefault 与 core_var2.c
  FUN_14001E5B0 为两份等价实现(应统一转发消重, 低优先)。

---

## R25-d (2026-08-29) — SET value 恒空定案(ApplyVarWriteModifiers 桩) + %A% 展开路径锚定

### 落地修复（windbg 活体 + dc 证据锚定）
1. **SET 变量 value 恒空根因**: `PECMD_ApplyVarWriteModifiers`(core_b3_remaining.c:21462,
   @0x140084a5c size=3133) 是 **SKIP 空桩** → EnviMemReadWrite:3280 调用后 `plen2
   (local_2d8)` 不写 → `v18=0` → `AddVarDefault` 第 5 参 flag=0 → `VarWriteValueCap
   (len=0)` → **SET 创建变量 value 恒空** → `%A%` 展开为空 → `FIND $%A%=pecmd` 判假。
2. **修复**: 按 dc:84512-84517 无修饰符路径(首空白即 break, 修饰符集合 `$ % = . # ~`
   未命中) 最小化 —— `*param_4 = lstrlenW(val)*2` (AddVarDefault 的 cap = val 字节
   长度); `*param_2` 保持不写(原版无修饰符时亦仅分配, 内容由 AddVarDefault 用 val
   重写)。修饰符展开路径($/%/=/.#/~)仍返回 NULL 保留原桩行为, TODO(verify) 完整移植。
3. **windbg 活体验收**: AddVarDefault flag `0→10` (rax=0xa) ✓; A 节点完整
   (`name="A"`, `value="pecmd"`, `cap=10`, 0x253312cb0c0) ✓; 但 031/039 **仍 exit=2**。

### %A% 展开路径定案（FIND 条件, J 簇深水）
- 链路: IfexFindExecutor(收到 `$%A%=pecmd` 未展开) → EvalLoopCondition → 
  PECMD_ExpandEnvVars var_b38c → **env_b55d**(优先 `GetEnvironmentVariableW`) →
  未找到(ERROR_ENVVAR_NOT_FOUND=0xcb) → 回退 **var_b6ee**(PECMD_VarLookup 脚本表)。
- **原版(reference PECMD.EXE 活体)**: SET 也走 AddVarDefault(脚本表堆地址), 
  `script+0xd=0`(与 msvc 相同) → env_b55d 查环境变量未找到(环境无 A) → 回退
  var_b6ee → 命中脚本表 A="pecmd" → `FIND` 判真 exit=0。
- **msvc(windbg 活体)**: env_b55d `GetEnvironmentVariableW("A")` **返回 1, 缓冲="1"**
  (环境变量 A=1 存在) → 展开 `"1"` → `$1=pecmd` 判假 exit=2。
- **未决**:
  - 环境变量 `A="1"` 来源未定 — `SetEnvironmentVariableW("A",...)` 全程追踪只命中
    系统变量(PECMDVER 等), 疑似 windbg 启动环境继承(IDE 服务环境可能带 A=1);
    bash 环境 `A` 为空, 但 bash 跑 p20 同样 exit=2 → 需区分"环境 A 干扰"与
    "var_b6ee 回退脚本表查找失败"两条支线。
  - 下一步: bp 无条件 `PECMD_VarLookup` 抓 `name="A"`(堆) 确认 var_b6ee 回退是否
    命中; 排除环境 A=1 干扰后再判脚本表结构/VarLookup 遍历是否还有问题。

### 基线
- 与 R25-c 持平 **59/63** (本修复消除 value 空, 但 %A% 展开未达终态, 031/039 仍 FAIL)。

### 存留
- PECMD_ApplyVarWriteModifiers 修饰符展开路径($/%/=/.#/~ 字节图案/hex/重复)待完整
  移植 dc:84514-84762; 若后续用例覆盖 ENVI `$`/`%` 赋值需补。

---

## R25-e (2026-08-29) — 031 PASS(var_b6ee spec + 环境隔离) + 039 根因实锤(script+0xd=1 删未命中%)

### 落地修复①: var_b6ee spec 条件化 → p23(SET B) 翻转 PASS
- **根因**: PECMD_ExpandEnvVars var_b6ee 主查找(core_execline.c:888-961) 原移植**无条件
  `p15 = p7`**(第二个%位置) → `%B%` 场景 spec="?" 非 NULL → PECMD_FormatTypedMemValue
  走 `0x%I64X` 格式化 → 展开 `"0x70"` 而非 `"pecmd"` → `$0x70=pecmd` 判假。
- **原版对照**(dc:78377-78426): 两种退出 — `pWVar13<=pWVar6` / nameStart 空 →
  `goto LAB_14007b91f`(**跳过** `pWVar15=pWVar6` → spec=NULL → 直接取字符串);
  仅 '?' 终止才走 `pWVar15=pWVar6`(spec 格式化)。
- **修复**: 加 `via_b91f` 标志 — `p13<=p7` 或 nameStart 空 → via_b91f=1 → 跳过
  扫描:/p15=p7/%d 特殊 → p15 保持 NULL; '?' 场景 via_b91f=0 原逻辑。
- **实测**: msvc p23(SET B=pecmd+FIND) exit **2→0** ✓ (与原版一致); p20(SET A)
  仍 2(环境 A=1 干扰, 原版亦 2); p24(ENVI B) 0 ✓。

### 落地修复②: run_case 环境隔离(A-Z) → 031 PASS
- **环境变量 A=1 来源定案**: cmd/注册表查证 — 注册表 HKCU\Environment 无 A, 但
  cmd/bash/python 全部继承 A=1 → **IDE 服务进程级环境自带 A=1**, 污染任何 `%A%`
  展开(原版实测 p20 亦 exit=2 → golden=0 失配)。
- **修复**: run_case.py subprocess.run 加 `env=clean_env`, 启动前清除 A-Z 单字符
  环境变量(仅 IDE 有 A=1, 清除安全; PECMD 运行期 ENVI/SET 设置不受影响)。
- **实测**: 031_set_assign diff **PASS**(exp=0 got=0); 环境隔离后原版/msvc p20
  均 exit=0(判真)。

### 039 根因实锤(未修复): ProcessScriptBlock 6101 整行展开删除 cmd2 的 %X%
- **复现**: p25(TEAM CALC X=2*3|FIND $%X%=6) 原版 exit=0, msvc exit=2。
- **关键观察**(msvc windbg): ProcessScriptBlock **restored_bodies.c:6100-6103**
  (非 "FIND " 行整行展开, 原版 Ghidra 同) → PECMD_ExpandVarDispatch 展开完整
  TEAM 行 → **展开结果 cmd2 的 `%X%` 被替换为空**(`FIND $=6`) → 判假。
- **根因链**: msvc 展开时 **script+0xd = 0x01**(LOAD 时实测 0, TEAM 行展开前变 1)
  → var_b38c `local_230 = xflag = script+0xd & 0x11 = 0x01`(非 0) → var_b6ee 未命中
  → **b961(core_execline.c:1056) `if (local_230 != 0) continue;` → 删除 %X%**。
  原版 script+0xd=0 → local_230=0 → b961 走 `local_268=1 → env_b55d → bab5`
  (**原样复制 `%X%`**) → 分段执行时 X 已创建 → 判真。
- **script+0xd 设置者未定**: msvc 启动**不调 PECMD_ParseEnvSwitches**(FORCELOCAL/
  EnviMode 分支均未命中); 显式 `ENVI EnviMode=1`(p26) 也不设置 script+0xda/0xd
  (msvc EnviMemReadWrite 无 EnviMode 分支) → script+0xd 变 1 是**其他路径**(疑
  LOAD/PECMD_PrependEnviHeader flags&0x10 ForceLocal 插入 `@ENVI^ FORCELOCAL=1`
  但 ParseEnvSwitches 未调 → 或脚本表创建时), 待 bp 写入点定位。

### 基线
- **60/63 PASS**(031 翻转为 1, 039 仍 FAIL), 全量回归待 R25-e 终跑确认零回归。

### 存留
- 039: script+0xd 写入点未定位(候选: LOAD 后/PrepEnviHeader ForceLocal 行/脚本表
  创建); 修复方向 = 使 TEAM 行展开时 script+0xd 保持 0(与原版一致)。
- 探针: p20-p26(SET/ENVI/TEAM 对照) 保留至 039 门通过后统一拆。

---

## R25-f (2026-08-29) — E簇修复入账 + 62/63 终跑确认 + 039 写入点静态收窄(4/5 排除) + 67 动词盘点, 基线 60→62/63 零回归

### 落地修复①(补记, 前会话完成未入账): E簇 024/025 TEAM 退出期 double-free
- core_thread.c `PECMD_EnumWindowsCallback`: dc:19100 成功路径置 task=NULL
  (dc:19103 FreeStrBuf(NULL) 不释放 task, 归线程侧 SendMsgThreadProc 清理);
  旧实现无条件释放 → 线程内二次释放 → 退出期 0xC0000374。
- **024/025 翻转 PASS**(02:34 全量首证, 本轮终跑复证)。

### 落地修复②: run_corpus.sh `SKIP_DIFF` 未定义在 `set -u` 下炸尾(全量跑完后 diff 不执行)
- 改 `${SKIP_DIFF:-0}`; 本轮全量双跑(EXE=both) + `diff_case.py --all` 出 verdict。

### 全量终跑(R25-e 悬置项闭合)
- **62/63 PASS 零回归**(63 案 orig+msvc 双跑, orig 侧真值复跑确认 golden 全部有效)。
- 唯一 FAIL = 039_team_chain (exp=0 got=2, 稳定复现非 flaky)。
- 部署 md5=fd7f957b(08-29 07:55 构建含 R25-d/e 修复); DEPLOYED_BUILD.txt 戳落后(517ee608), 活体取证前需 post_build 重刷。

### 子代理 A 交付: 039 script+0xd 写入点静态取证(analysis/r25f_039_script0xd_writers.md)
- **dc 全库写者仅 5 处**(ParseEnvSwitches dc:7284 / RunScriptText dc:113269+113383 /
  异步线程 dc:110197+110199 / ScriptCopy dc:12776 / 结构初始化 dc:18034+18047),
  msvc 9 赋值点一一对应同构, 无多余写者; 039 静态主链逐点推得两侧 +0xd≡0
  —— 与 R25-e 活体"展开前=1"矛盾, 写入者在静态未覆盖分叉或为非映射写。
- **写入者排序**: C1=core_execmain.c:254(RunScriptText flags&0x10→|=1, 中置信,
  禁直改——与 dc:113269 同构, 须修 flags 传递侧); **C1-a=core_scriptrun.c:650
  资源路径把随机种子低字节注入 flags**(kf&0xff 进位0-7, 位4 以种子值定态置位 →
  core_execmain.c:254 写 +0xd=1; dc:30219 无此注入, 源内自挂 TODO(verify))——
  唯一异构点, 若活体栈落 :650 即根因(修法=按 dc:30219 重写 flags 组合);
  C2 ParseEnvSwitches(无 FORCELOCAL 文本, 排除)/C3 THREAD(排除)/C4 ScriptCopy 传导/C5 非映射写兜底。
- **活体收口配方 §5.2**(V-Gate 合规): bp PECMD_ExpandEnvVars 首命中甄别脚本对象
  (rcx vs g_Script) → `ba w1 P+0xd` 抓写入 RIP → 回 map 反查; 阴性对照=原版同点 +0xd≡0。
- 伴生实锤(另行分诊, 勿混入本案): core_b2e.c:1386 `-mode` 判定 `(*p+1)` 应为
  `p[1]`(dc:27047, 恒死分支); `*map:` 子进程形态桩不执行脚本(core_scriptrun.c:393-410);
  PECMD_ExecuteScriptBlock 死代码与 srx 长期并存; run_case 双后端共享 out_dir 存在
  fs/vars 维度假阳性风险(exit 维度不受影响)。

### 子代理 B 交付: 67 未覆盖动词实现状态盘点(analysis/r25g_verb_coverage_register.md)
- **真体 59 / 简化桩 1(ADSL) / 恒0 桩 7(MESS MSTR SBAR SITE SOCK SPIN USER) / 缺失 0**
  —— R25-b "67 动词处于简化桩/未验证态"偏悲观, 88% 是 dc 直移真体, 零语料风险主体
  是直移缺陷(goto/缓冲区类)而非空桩。
- 7 桩已逐个排查 D-01 式"桩遮蔽真体"均无第二真体; MESS 桩另有 core_b3l.c:1385/
  core_calc_expr.c:983 两处内部调用同吃恒0桩(静默面最大)。
- Top5 真体化优先: MESS > MSTR > SOCK > ADSL > USER。登记入 divergences 待办。

### 下轮工单(ROI 序)
1. **039 活体收口**(§5.2 配方, 先 post_build 重刷身份戳): C1-a 证实→按 dc:30219 修
   core_scriptrun.c:650 flags 组合; 证实非 :650→C5 堆踩踏/E 簇范式排查。修后全量回归冲 63/63。
2. 67 动词登记并入 docs/divergences.md(按 r25g 报告)。
3. 伴生差异登记(-mode 恒死/*map: 桩/run_case 时序) + Top5 动词真体化(MESS 起)。
4. P1 L1 语义化开线(前 10 高浓度文件 local_xx/param_N, M2/M3 倒退待扭转)。
5. 存留: script2.c 探针(S7 依赖)/U-2 vars_val 普及/非语料二项(小数箍环/7%3)。

---

## R25-g (2026-08-29) — ★63/63 全量 PASS 首次完全收敛 + 039 真根因定案(移植单位错误族), 基线 62→63/63

### 039 活体收口: R25-e 的 script+0xd 假说正式证伪, 真根因 = 指针差单位错误(元素差再>>1=折半)
- 活体链(V-Gate 合规, 4 会话迭代, 硬件断点 ba w1 + 表枚举 + VarLookup 审计):
  ①TEAM 行展开时脚本对象 +0xd = 0(展开器入口 rcx+0xd 实测), R25-e "script+0xd=1 → b961 删 %X%"
  在当前构建不成立(该假说系探针构建时代的对象误认, C5-a 应验);
  ②修复 :831 后展开输出 %X% 存活, 但仍 exit=2 → 下游继续;
  ③枚举 VarLookup("X") 返回 rax=0 + 三张表(S2 9 变量/g_Script 38/S1 2)全无 X
  → CALC 根本没写 X; ④cmd1 整行展开输入 = "CALC "(值部丢失) → TEAM 分段腰斩实锤。

### 两处根因修复(均 dc 行号锚定, 字节差>>1 ≡ 元素数 的 dc 语义归正)
1. `core_execline.c:831`(dc:78350): `iVar10 = (int64_t)(p13 - p) >> 1` 元素差再>>1 折半
   → %X% 未命中段(bab5/H3 组合)只拷半截; 修 = `(intptr_t)p13 - (intptr_t)p) >> 1`。
2. `core_b2d.c` ParseCommandBlock(dc FUN_140025474) 同族四处:
   - :611 段拷贝(dc:22675): `(pWVar10 - *pp) >> 1` → TEAM 段 "CALC X=2*3" 拷成 "CALC "
     → CALC 无参不写 X → FIND $%X% miss → 判假 exit=2(039 直接根因);
   - :533 []块拷贝(dc:22565) / :555 }块拷贝(dc:22626) / :591 末字符指针(dc:22653 字节地址运算)。
- 修复后组合行为: TEAM 行整行展开保真 → CALC X=2*3 真实写入 → FIND 条件期 VarLookup("X")
  命中 → $6=6 判真 → chain_ok → exit 0(039 PASS, exit=0 = golden)。

### 全量回归
- **63/63 PASS**(修复部署 md5=fa90cb9c 后全量 EXE=msvc 重跑 + diff, 零回归)。
- 收敛曲线: R19 9 → R24b 41 → R24e 53 → R25a 54 → R25c 59 → R25f 62 → **R25g 63/63**。

### 新立工单: `>>1` 单位错误族全库审计(高优先)
- 同一移植族错误模式(dc `(longlong)a-(longlong)b>>1`=字节差>>1=元素数; msvc 误写 `(a-b)>>1`
  =元素差>>1=半数)已实锤 5 处(core_execline.c:831 + core_b2d.c×4); 修复只覆盖 039 路径,
  **其余同族站点未审计**(如 core_execline.c b81b 区 :1024/:1048 附近疑似同型, 语料未覆盖=静默)。
- 审计法: grep 全库 `(a - b) >> 1` 指针差模式 → 逐站点核 dc 对应行是否为
  `(longlong)差 >> 1`(字节差) → 是则归正。语料盲区必须逐点核, 不可凭"已 PASS"放过。

### 存留(沿 R25-f)
- 67 动词登记并入 divergences(真体59/简化1/恒0桩7, Top5=MESS>MSTR>SOCK>ADSL>USER)。
- 伴生差异: core_b2e.c:1386 -mode 恒死 / *map: 桩不执行 / run_case 双后端 out_dir 时序。
- script2.c 探针(S7 依赖) / P1 L1 语义化(M2/M3 倒退待扭转) / 非语料二项。

---

## R25-h (2026-08-29) — >>1 单位错误族全库审计 + 22 处归正 + ImportEnvironment 孤儿接线, 63/63 保持零回归

### 审计(analysis/r25h_ptrdiff_unit_audit.md, 子代理静态审计)
- 不变量判据: **dc 全库 0 处裸指针相减**——一切差值均为 (longlong)a-(longlong)b 字节域;
  msvc 凡 `(a-b)>>1`(元素差>>1=半数)必错, cast 形态必对。OK 19 / BUG 22(21 byte-diff 折半
  + 1 byte-addr 倍差) / 未对齐 2。
- 22 处分布: execline 双展开器 7(含两函数出口返回值双重复偿折半, 幸调用方全 void 丢弃)
  / b8m 浮点截断门限+LIST name 3 / b1_remaining 卷枚举+图标名+启动关联链 6
  / b2d TEAM 段拷贝 4(R25-g 已修) / scriptrun RUN 前缀 1 / strbld FormatTypedMemValue
  字节寻址倍差 1(全库唯一 byte-addr)。

### 落地修复(三组, 每组构建+全量回归 63/63 零回归: md5 dd5d5bdc→e6ecc49d→d05def76)
- 组1: execline.c 183/306/484/508/700/1027/1051(dc:77758/77875/78112/78142/78320/78692/78723)
  + core_strbld.c:246(dc:68740, `(uint8_t*)value+off` 字节寻址)。
- 组2: b8m.c 2143/2144(dc:141232-3)/3120(dc:148352) + b1_remaining.c
  3011/3024(dc:3796/3808)/3865(dc:4901)/4880/4890(dc:6942/6952)/4952/4960(dc:7013/7023)/
  9934(dc:18892) + scriptrun.c:313(dc:29657)。
- 组3(O1): unimplemented_stubs.c 恒0壳 PECMD_ImportEnvironment 删除 → core_b3m.c
  FUN_14007BF78 改名 PECMD_ImportEnvironment 真体接线(i28c:1235 异步线程 '&' 分支,
  rename_map.json:922 映射落地); 同步落 #2/#3(dc:78855/78862) 单位归正。

### 登记
- divergences.md D-18(67 动词登记, 真体59/简化1/恒0桩7, Top5=MESS>MSTR>SOCK>ADSL>USER)
  / D-19(-mode 恒死分支) / D-20(*map: 桩不执行) / D-21(run_case 双后端 out_dir 时序)。

### 下轮工单
1. P1 L1 语义化开线(前 10 高浓度文件 local_xx/param_N, M2/M3 倒退待扭转)。
2. D-19/D-20 待修(一行/一真体); Top5 动词真体化(MESS 起)。
3. 存留: script2.c 探针(S7 依赖) / 非语料二项(小数箍环/7%3) / S18 ParseDateTimeSpec 对齐。

### R25-h 追记(同日末批)
- D-19 落码: core_b2e.c:1386 `-mode` 判定 `(*p+1)`→`p[1]`(dc:27047), 构建回归 **63/63 零回归**
  (部署 md5=ffb900b7, hash=cf5daf7)。

---

## R25-i 规划段 (2026-08-29) — 63/63 收敛后的完整行动计划 (TODO 同步)

> 基线: 63/63 全绿 (部署 md5=ffb900b7→d5b769ee); L1 双试点已提交 (096d554, 379 站点);
> 配方已定稿 (analysis/r25i_l1_recipe.md)。

### P1 L1 语义化主轴 (体量最大, 40943 处占位名, 前 10 文件 82%)
- 批1: b2f EvalLoopCondition @0x140032dc4 (412处, 语料核心路径) + b7c 高密度函数 2-3 个
  (≤400处) —— 并行子代理单文件独占 → 构建+全量回归(63/63 硬门) → 提交。
- 批2: b2f DispCommand(444)/HelpDlgProc(321)/FUN_14003B540(215)。
- 批3: core_b3_remaining.c (12058处, 分子批, CALC/SED 语料覆盖体优先)。
- 批4: restored_bodies.c(9527)/b1_remaining.c(2907) → **A 区门检查: b2f/b7c/b8h local_xx≈0 (G3)**。
- 配方七步见 recipe 文档; 保留口径: Ghidra 槽复用名(多角色)保原样, 语义正确性是唯一 KPI。

### P2 语义还原收尾 (穿插执行)
- MESS 真体化(优先: 恒0桩+core_b3l.c:1385/core_calc_expr.c:983 两处内部调用点同步接线)
  → 每真体化 1 动词配 1-2 个 golden 语料案 → 全量回归。
- MSTR/SOCK/ADSL(假真体, 104→33)/USER 后续; D-20 *map: 真体(dc:379-390 段); S18 对齐。

### P3 语料扩容 (需求驱动)
- 前置: D-21 run_case 双后端 out_dir 时序修复 → U-2 vars_val 普及灰度。
- 真体化动词逐个录 golden; UI/WinPE 专用动词完成口径=真体落地+登记环境限制。
- 提级机制: Lua FFI 新项目动词清单 → 对应动词提级硬性排期。

### P4 交付沉淀
- Lua FFI 接口契约文档(签名→语义→行为验证状态); divergences/provenance/FUNC_NAMES 收口;
- M2/M3 里程碑复测入账本。

### 纪律(不变)
每处改动 dc 行号锚定; 逐批合入+每批全量回归(63/63 硬门, 掉即回滚排查);
并行子代理单文件独占, 构建/审查/提交收敛主代理; windbg 只用于运行时状态问题。

### R25-i 计划段（2026-08-29, 多轮路线图; TODO 同步跟踪）
- 阶段0 L1批1(进行中): C3=b2f EvalLoopCondition(412处) + C4=b7c 高密度≤400处, 并行派发后逐批回归。
- 阶段1 语义还原穿插: MESS 真体化(+core_b3l:1385/core_calc_expr:983 接线)→MSTR→SOCK→ADSL→USER,
  每项 dc 取证→真体→构建→1-2 golden 语料案→全量回归→提交; D-20 *map: 真体(dc:379-390)。
- 阶段2 L1规模化: 批2=b2f 剩余大函数(DispCommand 444/HelpDlgProc 321/B540 215/LinkCreateShortcut 212)
  →批3=core_b3_remaining(12058, CALC/SED 体优先)→批4=restored_bodies/b1_remaining; G3 门=A区 local_xx≈0。
- 阶段3 验证面: D-21 run_case 双后端 out_dir 时序修复→U-2 vars_val 灰度→真体化动词按需求录 golden。
- 阶段4 沉淀: M2/M3 复测+Lua FFI 接口契约文档(签名→语义→验证状态)+script2.c S7 探针(依赖解除后)。
- 口径: 67 零语料动词"全部完成"无承诺日期(ROADMAP §3.3 需求驱动长尾), Top5 即期;
  每批硬门=全量 63/63 零回归; 配方=analysis/r25i_l1_recipe.md。

---

## R25-i (2026-08-29) — L1 语义化开线: 配方+双试点+批次1, M2/M3 首次扭降 (40943→40342, A区 5060→4479)

### 配方 (analysis/r25i_l1_recipe.md, 七步)
读双源建映射表 → snake_case 语义名+dc 行号证据 → 全库撞名预检(禁 Windows 宏词) →
python \b 全字替换严格限函数行界 → 纯度自检(行界外字节一致+残留计数+diff round-trip 反演断言)
→ 主代理双绿门+全量 63 案回归(硬门) → 提交登记。活样例 = 两份 pilot 报告。

### 已完成三批 (每批独立构建+全量 63/63 零回归)
1. **双试点**(096d554, md5=d5b769ee): C1=b2f FUN_14003C06C 22名/205站点(保留10名多角色槽)
   + C2=b2d ParseCommandBlock 16名/174站点(TEAM 分段体, R25-g 锚点仅换名)。
2. **批次1**(fe5a20c, md5=87645985): C3=b2f EvalLoopCondition @0x140032dc4 48名/340站点
   (kind_marker_str/flt/int 三比较种类/lhs_buf/rhs_val/cond_neg_flag/env_gate_flag...,
   保留 40 名多角色槽逐个登记, probe_ui 未动) + C4=b7c TablCreateControl+CtlLoadPictureRgn
   63名/552站点(parse_cur/opt_flagbits/coord_l/name_cur/load_bmp/star_mode...);
   C4 登记 2 项待核观察(dc:131608 iVar6=local_184 vs 还原件 stack_size / 几何 sy 缺省 dc=h vs 还原件=0)。
3. **指标**: M2+M3 = 40943 → **40342**(-601, 首次扭降, 扭转 R25-b 倒退信号); A区(b2f/b7c/b8m/b8h)
   残留 5060 → **4479**。累计改名 ~1450 站点/150 名, 零行为差异(每批 63/63)。

### 在途状态 (未落码)
- 语义还原穿插批取证: MESS(dc:116238 4933B GUI 体, Ghidra 类型传播失败, 最难件; 且弹窗阻塞语料
  → 验证口径需设计) / USER(dc 入口 7B+FUN_14001ada8 346B 参数段解析执行) / MSTR(3966B)/
  SOCK(8287B)/ADSL(假真体 104→33 分支)。成本调整: USER/ADSL 先行, MESS 专项。
- 批次1 派发曾遭平台取消一次, 重发成功。

### 批次规划 (analysis/r25i_l1_recipe.md §批次)
批2=b2f 剩余大函数(DispCommand 444/HelpDlgProc 321/B540 215/LinkCreateShortcut 212)
→批3=core_b3_remaining(12058, CALC/SED 语料覆盖体优先)→批4=restored_bodies/b1_remaining;
b7c 后续: TablCreateListCtrl(756 行, dc 已预核)。

---

## R25-j (2026-08-29) — USER 真体化 + 语料案 066, 语料库 63→64 (64/64 全绿)

### 行为
1. **真体接线**: dc:15584 `FUN_14001af0c`(7B=`xor dl,dl; jmp` 尾调用) → 新增包装 `PECMD_UserCmdHandler`
   (core_b1_remaining.c, 调用既有真体 `PECMD_SetRegistryOwnerRun` @0x14001ada8 346B,
   该体早前已 dc:15528 直移: AllocStrSlot×2 → StrRChrW 找最后逗号 → 段展开
   (PECMD_ExpandVarDispatch, 首参 g_Script≡DAT_14013d130 已核 pecmd_defs.h:13) →
   param_2=0 写 HKLM RegisteredOwner/RegisteredOrganization / param_2=4 写 CurrentVersion\Run,
   RUNS tag 0x534e5552 已接)。恒0桩(unimplemented_stubs.c:172)拆除 + skip 登记移出。
2. **语料案 066_user_noseg**: 无逗号段早退路径(dc:15546 返回 1 不触达注册表写, 对拍安全);
   golden 录制 exit=1 (返回值经调度链同款传播到进程退出码, msvc 首跑即 PASS);
   %A% 在早退前不展开的口径写入 manifest notes。
3. **夹具修复**: run_case.py result_dir 缺 makedirs → 新案首跑 stdout.txt 直写崩溃(FileNotFoundError),
   补 `os.makedirs(result_dir, exist_ok=True)`。
4. **全量回归**: 构建 md5=dafd027b (515b927) → 部署 20:35 → run_corpus + diff_case --all
   = **64/64 全绿零回归**。

### 登记
- D-18 更新: 恒0桩 7→**6**(MESS MSTR SBAR SITE SOCK SPIN), 真体 59→60;
  USER 有逗号路径写真实 HKLM, 后续 golden 需注册表沙箱口径(Phase 3)。
- 下一步: Phase 1 不可用层压缩按序 ADSL 假真体补全 → MSTR → SOCK; MESS 专项(口径设计)。

---

## R25-k (2026-08-29) — GUI 校对延后(用户决定) + 三线并发回货: D-20 关闭 / ADSL 真体化 / L1 批2-b7c

### 规划修订（用户指示）
- **GUI 部分延后**：MESS 专项(4933B GUI 体+弹窗阻塞口径)与 SBAR 等涉及"与原版 PECMD GUI 行为校对"的
  项移出近程队列，挂 Phase 4 之后（校对口径复杂）。Phase 1 余量改为 **MSTR(3966B) → SOCK(8287B)**。

### 三线交付（全部独立构建+64/64 验证）
1. **D-20 `*map:` 执行体关闭**（core_scriptrun.c, +152 行）: dc:30235-30272 全序列直移
   (ResDecode→随机键双层XOR→InvokeSubRoutine→key 分隔/填充→PrependEnviHeader→RunScriptText);
   capstone 反汇编定案 0x140031bb0-0x140032560（门控 mem_flag==0 && size>0; 重映射 size+8 字节——
   旧码 sz+2 单位错一并修正）; 失败路径接回 dc -0x7ff8ffa9。偏差 7 项登记
   analysis/r25j_d20_map_port.md §6。**新实锤随案登记**: PECMD_StrBldCopyWideN(core_var2.c:68)
   按字节拷贝 vs 原版按字符（R25-h 家族, 报告 §6#4, 待审计调用面后修）。
2. **ADSL 真体化**（core_b3r_h2.c, 直移体 957 行）: FUN_140099f18 dc:96610-97468 全量直移,
   if 105/dc104 同构、调用点 205:205 对表; 3 处 wsprintfW 变参丢弃 + 5 处 wlanapi 槽丢参按
   原版 EXE 反汇编恢复; 补最小件 PECMD_InitEventSlots(1400e4e6c) 本文件 static。
   **简化桩 1→0, 真体 61**（D-18 更新）。analysis/r25j_adsl_wlan_port.md。
   登记: DAT_1401210f8(L"\r\n") 现挂零桩属全工程 data-stub 事项。
3. **L1 批2-b7c**（core_b7c.c TablCreateListCtrl @0x1400c9b9c, 756 体行）: 50 名/430 站点改名,
   保留 13 名/152 站点(多角色槽逐个登记); 纯度自检全绿(行界外字节一致+双向映射反演+撞名 0)。
   analysis/r25j_l1_b7c_listctrl.md。

### 构建/回归
- 最终构建 md5=5ae03726 (54ac6cb) 部署 22:38 → **64/64**。中途一次 011_file_del exit 2→0 假阳性:
  EXEC/FILE 通道偶发哑火 × 案间共享 `C:\pectest\out\` 状态链(005→010→011)级联, 复跑即过 →
  新登记 **D-22 工单**(案间状态自足化候选)。
- L1 批2 另一半(b2f DispCommand+HelpDlgProc)因平台并发上限被拒, 待重发。

## R26-a (2026-08-30) — 优先级重定: 消费方(pecmd_compat)基准供给升为近程主轴 (无代码变更)

### 规划修订（用户指示）
- 第一优先级从「自身二进制保真收敛」调整为「为 pecmd_compat（Lua 重写，非 GUI 对齐路线）提供对照基准」。
- R25-k 队列 **MSTR → SOCK 移出近程**：登记为「消费方决策暂缓」（compat 已正式否决旧语法/IPC 对象
  复刻，无消费方；未来若恢复按自身保真目标重排）。GUI 族维持 R25-k 延后决定。
- r25-g §3 Top5 按消费方视角重排：ADSL/USER 已完成；MESS/MSTR/SOCK 暂缓；
  **SITE 升为唯一近程真体化项**（compat 消费其文件属性/时间语义）。

### 消费方需求清单（近程队列，2026-08-30 按用户目标1 重排：功能对齐=排除 GUI 与旧语法的能力对齐，凡 compat matrix 有消费方且本仓 msvc 为桩的功能项排期拉高）
1. **D-01 修复**（EXEC 注入三路径桩遮蔽）——EXEC 是 compat 头号 partial 项，真值源被污染，最高优先。
2. **MOUN 真体化**（dc 完整 5907B @0x1400dfb14；compat MOUN=partial，功能对齐直接消费者；现 core_b7c.c:10344 恒0桩）。
3. **PART 真体化**（dc 巨体 29895B @0x14008cffc；compat PART=partial 分区布局消费者；现 core_b3_remaining.c:22666 桩）。
4. **GETF 真体化**（恒0桩 core_b7c.c:8793；compat GETF=needs_decision 下载面）。
5. **SITE 真体化**（dc 265L @0x1400d0468，core_b7c.c:489 桩——可与 b7c 控件桩批合并；compat SITE=partial）。
6. FORX msvc 修复（dc 完整 6498B，msvc 缩水为单次 SetVar；语法层现代替代=Lua，修复低成本）。
7. SHOW 10 处 TODO(verify) 收口（返回值契约点）；PAGE 疑似直移缺陷核验（core_b2e.c:844）。
8. PUTF 2 leaf 子桩——条件项（compat PUTF=needs_decision 落地"做"才排）。
9. 定向语料扩展（EXEC 变体/REGI 值类型/SEND/KILL/WAIT/SWIN/PCIP 等）+ **D-22 案间状态修复先行**（预言机可信度）。
MSTR/SOCK/MESS/SBAR/SPIN 维持「消费方决策暂缓」（无 compat 消费方）；GUI 族不排（compat 走 AHK 路线）。

### 消费方取证发现（供登记核对）
- **MOUN/PART 级联分支存在但未入 include/pecmd_fourcc.h 98 动词表**（gen_fourcc 口径缺口）；
  MOUN msvc 为恒0桩（core_b7c.c:10348，启发式名「文件时间命令」系误标）、PART msvc 为桩
  （core_b3_remaining.c:22666，自标 decompile-failed）——两者语义消费方已从 dc 抄录（其
  docs/re-notes/w1-storage.md）；msvc 真体化按自身保真目标排期，非消费方刚需。
- r25-g 登记表 USER 行已过期（R25-j 已真体化），引用该表时以本账本为准。

### 消费方已交付物（登记）
- pecmd_compat 新增 `docs/reverse-alignment.md`（对象级真值状态/可信度分级/名实勘误/暂缓划分/需求清单）
  与 `docs/re-notes/w1-{engine,cond-loop,calc-time,storage}.md` 四批取证（29 对象，绑定本仓 @63179b34
  快照）；chunks/matrix 已回填逆向核证块（TIME/DTIM 勘误改指 GUI 族、SCRN 标待核）。

### 补记（R26-a 同日，消费方取证收尾）
- GETF 恒0桩 + PUTF 同地址双定义疑点（core_b7c.c FUN_1400D2E90 桩 vs restored_bodies.c:8037 真体）——
  D-01 型风险，链接取舍待定案。
- RUNS 无独立动词（无 FOURCC/级联）；USER/PATH/RECY/FLNK 名实更正（细节见消费方
  docs/re-notes/w2-reg-env.md / w2-proc-power.md）。
- LOCK（PECMD_LoadTasksWait）与 WAIT（FUN_14003ED4C）已由消费方 V1 完整通读核验（2026-08-30，re-notes/v1-lock-wait.md）：
  LOCK=**具名锁/互斥体同步**（非工作站锁，matrix 行已更正）；WAIT=**多语义等待**（按键/进程/父进程`*`/`$`线程/`-del` 文件/`-cmd`/`-sys*` 主线程通道/thread 任务队列/scan 键状态/freemem），与官方文档一致。

## R26-b (2026-08-30) — 消费方目标1重排: 功能对齐(非GUI/非语法)的 msvc 真体化缺口升入近程 (无代码变更)

### 背景（用户三目标）
- 目标1: pecmd_compat 与 pecmd-decompile 完成**功能对齐**——排除 GUI（compat 侧按 AHK 路线重写）与排除旧语法
  （compat 是 Lua 重写），但旧命令式语法实现的功能须以 C 移植形态对齐。
- 目标2: 盘点反编译工作剩余的功能性(非 GUI)部分并拉高排期——本条目即该盘点与重排。
- 目标3: 两端文档同步——compat 侧 reverse-map.md 已交付并挂指针。

### 功能性(非 GUI)未移植/未还原清单（回答目标2）
**dc(Ghidra 反编译原文)侧对这些功能基本齐全——缺口在 msvc 移植/真体化层**（FUN_140034788 等少数
dc 反编译失败区已由汇编还原稿+活体裁定补齐，不在下例）：

| 动词 | msvc 形态 | dc 参考 | compat 消费方 |
|---|---|---|---|
| MOUN | 恒0桩 core_b7c.c:10344 | FUN_1400dfb14 @0x1400dfb14 (dc:136693, 5907B) | matrix MOUN=partial |
| PART | 桩 core_b3_remaining.c:22666 | FUN_14008cffc @0x14008cffc (dc:89789, 29895B) | matrix PART=partial |
| GETF | 恒0桩 core_b7c.c:8793 | @0x1400d0c6c | matrix GETF=needs_decision |
| SITE | 恒0桩 core_b7c.c:489 | @0x1400d0468 (dc:128226, 265L) | matrix SITE=partial |
| FORX | 缩水桩 core_b3r_i28b.c:424 | FUN_1400acd90 @0x1400acd90 (dc:107739, 6498B) | 语法层（Lua 替代，低成本） |
| PUTF | 真体+2 leaf 子桩 restored_bodies.c:8036-8037（另 core_b7c.c FUN_1400D2E90 同地址双定义疑点） | @0x1400d2e90 (dc:129678) | matrix PUTF=needs_decision(条件) |
| SHOW | 真体+10 处 TODO(verify) core_b7c.c:6496 | @0x1400cd3a8 (dc:126546) | matrix SHOW=partial |
| SERV | 真体+返回值 TODO core_b2f.c:174 | @0x140020018 (dc:19175) | matrix SERV=implemented |
| PAGE | 真体+疑似直移缺陷 core_b2e.c:844 | @0x14002a910 (dc:25444) | matrix PAGE=partial |
| DISP/FDRV/PCIP/REGI 等 | 真体但深水未逐行核 | — | 已可作参考，按需细读 |

### 排期重排（覆盖 R26-a 需求清单；按功能对齐价值）
近程队列：
1. **D-01 修复**（EXEC 注入三路径桩遮蔽）——EXEC 是 compat 头号 partial，真值源污染。
2. **MOUN 真体化**（dc 完整 5907B，compat MOUN=partial 直接消费）。
3. **PART 真体化**（dc 巨体 29895B，compat PART=partial 分区布局消费；工程量大可分段）。
4. **GETF 真体化**（本轮新确认的恒0桩）。
5. **SITE 真体化**（265L，可与 b7c 桩批合并）。
6. FORX msvc 修复（dc 完整，语法层但修复成本低）。
7. 真体收口批：SHOW 10 处 TODO + PAGE 疑似缺陷核验 + SERV 返回值 TODO。
8. PUTF 2 leaf 子桩（条件：compat PUTF 决策落地"做"）。
9. 定向语料扩展 + D-22 案间状态修复（预言机可信度，持续）。

暂缓维持：MSTR/SOCK/MESS/SBAR/SPIN（无 compat 消费方）+ GUI 族（compat 走 AHK 路线，不排）。

### 消费方交付（目标3 联动）
- pecmd_compat 新增 `docs/reverse-map.md`（逐对象 → dc 地址/行号 → msvc 位置 → 状态 → 证据笔记），
  matrix/chunks/missing-alignment 均已挂指针；本表与 reverse-map.md §F 一致。

## R26-c (2026-09-02) — 消费方队列首轮开工: D-01 修复 + PUTF 双定义定案 + SITE/ParseCommaNumbers 真体化, 64/64 零回归

### 落码清单 (每项独立构建 + 全量回归后入账)
1. **D-01 修复**（R26-b 队列头）: 删 restored_bodies.c:241-245 静态空注入桩 (return 0, 编译期抢绑 rb:3074/3562 两调用),
   改 extern 绑定 core_b8k.c:345 真体 (FUN_1400e7414 @0x1400e7414, dc:141754, size=392) —— 处置同 D-02。
   EXEC 注入三路径 (MEM PE blob / &变量 blob / EXEDATA) 不再静默失败。语料零暴露 = 潜伏分歧消除。
2. **D-23 新登记+已修 (PUTF 双定义定案, 即 R26-a 补记疑点)**: dc FUN_1400d2e90 (dc:129675, 11447B) 双符号分裂 ——
   真体 PECMD_DdCopyCommand (restored_bodies.c) vs 恒0桩 FUN_1400D2E90 (core_b7c.c); dc 四调用点中
   dc:35462 (core_b2f.c:7543, SHFILEOP -dd 复制路径) 误绑恒0桩。改绑真体 (QuadPart 承接, 同 dc:44518 形态) + 删桩
   + B2F_PART4_LOCAL extern 归正。详见 divergences.md D-23。
3. **SITE 真体化**（R26-b 队列项5）: FUN_1400d0468 (dc:128226, size=1731, 265L) 全文直移替换 core_b7c.c:485 恒0桩。
   语义 = 文件属性/时间设置: 属性分支 (+A/-A/+H/-H/+R/-R/+S/-S 循环 + SetFileAttributesW) 与 *touch 时间分支
   (':c'/':r' 目标选择 + *utc:/*local:/*local0:/*sys:/*sys0:/裸 8 段日期 + OpenFileHandle/SetFileTime +
   AppendLongDecimal 写 var)。返回 POINT 按调用面全 uint64 打包 (rb:6503 承接, 同 DdCopyCommand 模式)。
   dc 陷阱直移: 'c'/'r' 分支与 || 副作用殊途同归 → 游标一律 +2 wchar; S 块双不匹配 +1 wchar 前移靠槽零填充兜底。
4. **D-24 连带发现+已修**: SITE 时间分支依赖 FUN_140079cf8 (ParseCommaNumbers, dc:77418, 313B) 发现为恒0桩
   且 stubs_common.h:3340 零参错签, h4:1090/1101 (DATE 族日期段解析) 静默吃 0 → 真体直移落 core_b3r_h4.c
   (8 段 SYSTEMTIME 槽映射两 flag 形态) + 桩拆除 + 声明归正。详见 divergences.md D-24。
5. 依赖核验面: PECMD_EvalQueryValue (0x1400cb820, 7047B) 确认为 b7c 简化桩 (SITE '?' 递归路径依赖, 待后续批);
   GetFileAttributesExW/SetFileTime/LocalFileTimeToFileTime/StrCmpNIW 等声明面齐备 (win32_stub.h/xproto.h)。

### 构建/回归
- 分步构建 md5: b8e91d8e (D-01) → ee46fa6c (PUTF) → **2581139f (SITE+ParseCommaNumbers, 最终部署 04:27)**;
  每步 run_corpus 全量 = **64/64 零回归**。过程脚本 build/msvc/_r26c_*.py (一次性, 可清)。

### R26-b 近程队列状态
- 已完成: **1. D-01 ✅ / 5. SITE ✅ / 8. PUTF 双定义定案 ✅** (PUTF 2 leaf 子桩 PECMD_HandleDuplicateValid/
  PECMD_NormalizeDiskDevicePath 仍待, 条件项)。
- 未动: 2. MOUN (5907B) / 3. PART (29895B) / 4. GETF (8737B) / 6. FORX (6498B, 实测非"低成本"——dc 712 行
  类型传播警告体) / 7. SHOW/PAGE/SERV 收口 / 9. 语料扩展+D-22。
- 消费方通报: pecmd_compat 可同步 reverse-map.md 行状态 (PUTF/SITE/ParseCommaNumbers 真体化 + D-01/D-23 桩遮蔽消除);
  SITE 语义真值即 dc:128226 直移体, compat matrix SITE 行可引用。

### R26-c 追记 (同日末批): D-25 PAGE 直移缺陷实锤+已修
- R26-b 队列项 7 之"PAGE 疑似直移缺陷核验 (core_b2e.c:844)"**实锤**: dc:25537-25551 的 UNICODE_STRING 块
  (Length/MaximumLength/Buffer = local_60/local_5e/local_58) 被移植成孤立 `int16_t len16` 传址 → NtCreatePagingFile
  收垃圾 Buffer 指针。已按 dc 布局构造 struct 归正 (3 参调用契约保持 dc 原文), 64/64 零回归 (md5=8eaf0bee)。
  登记 divergences.md **D-25**。队列项 7 之"SHOW 10 处 TODO 收口"仍待。
- GETF 复核 (R26-b 队列项 4): dc 体实为 **1106 行** (dc:128565-129671, size=8737B), 非 8737B 直移量级口径;
  与 FORX (712 行) 同为专轮工程, 本轮不动。语料 065_getf_attr 仅比对变量名序列, 桩态 PASS 不构成真体化回归风险,
  但 GETF 返回值语义 (G 槽) 当前无对拍覆盖 → 真体化时应同步扩语料 (与队列项 9 合并)。

## R26-d (2026-09-02) — MOUN 真体化: FUN_1400dfb14 (dc:136698, 5907B/829行) 全文直移, 64/64 零回归

### 落码
- core_b7c.c 恒0桩 PECMD_MounMountWimImage 替换为 dc 全文直移体 (~830 行): WIM/VHD/UDM/SVR 四子引擎。
  分支面: -udm [upud]/listud (ExpandCommandLine+GenerateTimeText+SetVariable 日期掩码挂载);
  fallback "#22:INDATA mount ..."/"-svr" → RunCommand 委托; -vhd → LoadVhdApi+MounResolveDiskNo
  (c/cx 创建经 CaptureWimImage); 其余 → WIM DLL (g_pWIM* 槽) query 枚举/卸载/挂载 + '*' 临时目录 +
  PART update 联动。
- 调用面保持: rb:6759 全 uint64 (返回 8 字节状态) + core_b2b.c:874 void 面 (-udm OnlyApp 链)。

### 直移定案 (原版 EXE capstone 反汇编)
1. dc:137198 wsprintfW 缺变参 (D-03 族): 格式 "%I64d	%d	%s" 3 规格 2 可见参 —— @1400e09c0
   `mov [rsp+0x20], rdi` 实证第 5 参 = rdi = local_5f0 (lstrlenW 实参), 已补。
2. dc:137302 TlsLogWrite "FOUND WIM [%s]..." 4 规格 2 可见参 —— @1400e0efd-0f10 实证 6 参
   (script, fmt, _Var30, local_5f0, _Var15, iVar11), 已补。
3. dc:137289 "PART update %d " —— @1400e0f5f 实证 PART 实参 = wsprintfW 缓冲 +8 字节 (真机行为,
   非 Ghidra 帧伪影); 经 PECMD_ExecCommandLine (PART 桩) 2 参面照调 (第 3 参 r8=0 暂弃, 待 PART 真体化)。

### 登记偏差 (直移取舍)
- DAT_14013d878/DAT_14013dc98 两个未还原 CS (core_globals.c "不补" 注记) → g_csInit 替代
  (CS 同线程递归安全, 串行化语义保持)。
- dc 未初始化读 (local_5c0 初始化式 `high<<0x20`、local_res20 `& ~0xff` 基值) → C 层置 0。
- dc local_528[4] 实际承载 ≥14 wchar (PART update 串/FormatI64Dec) → C 层 [32]。
- dc local_618.dwLow = 0xffffffff 作 int32 (-1) 直移。

### 依赖面核验 (全部真体, 除注明)
MounResolveDiskNo (本文件 736) / RunCommand / ExpandCommandLine / GenerateTimeText / NextToken /
VarLookup / ResetScriptChain / WrapParamCall_02d8 / CreateWindowInSlot / DetachVirtualDisk /
LoadWimApi / QueryState_c95c / TlsLogWrite (变参) / EnumDeviceList / ImageCommitUnmount /
ApplyWimImage / DeleteDosDevice / AssignDriveLetter / CaptureWimImage / LoadVhdApi /
UnquoteString / AssignString / ExpandDrivePathAlloc(桩) / ExecCommandLine(PART 桩) / g_pWIM* 槽
(pecmd_globals.h, LoadWimApi 真体装载)。

### 构建/回归
- 部署 md5=ddcd19b5 (hash ec3babe) 08:39, run_corpus 全量 = **64/64 零回归**。
- 语料注: MOUN 无对拍案 (需真 WIM/VHD 语料, 归队列项 9); 真体化不构成现有案回归风险 (桩态 0 返回
  与真体路径在现有语料输入下同链)。过程件 build/msvc/_r26d_body_p*.txt + _r26d_moun_port.py。

### R26-b 队列状态
- 已完成: 1. D-01 ✅ / 2. MOUN ✅ / 5. SITE ✅ / 8. PUTF 双定义定案 ✅ (+ D-24/D-25 连带)。
- 未动: 3. PART (29895B) / 4. GETF (1106 行) / 6. FORX (712 行) / 7. SHOW TODO 收口 / 9. 语料扩展。

## R26-e (在途, 2026-09-02) — FORX 真体化中断存档(未落码)

- 队列项 6(FORX)开工: dc FUN_1400acd90(dc:107744-108445, 712 行)全文通读+语义分析完成;
  选项循环草稿已定稿(build/msvc/_r26e_forx_a.txt);主体(LAB_1400ad61f 起)未草拟;
  未应用源码/未构建/未回归。**完整作业档案(语义定案 13 条 + L 模式未决风险 + 依赖核验 +
  恢复待办)见 `analysis/r26e_forx_wip.md`** — 恢复作业以该文为准。
- 连带缺口已定案待落码: PECMD_ParseSizeNumber(restored_bodies.c:12 清零桩, dc:66228 179B 直移,
  i28b:73/stubs_common.h:2714 签名同步 uint64_t)。
- 本节仅为存档指针;R26-e 正式入账待 FORX 落码+64/64 后。
