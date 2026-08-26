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
| **windbg MCP** | pecmd_msvc 活体断点/unwind/dump 分析 | ✅ 本轮 5 枚 dump 全靠 open_dump+backtrace+.ecxr+ub retaddr 破案；⚠️ 引擎脆弱：`s -dq` 栈搜索触发 0x80040205 半死，改用源码推理；**dump 会话不关就构建=LNK1201 PDB 锁（实战再现）** |
| memory 知识图谱 | — | 维持不用（§7 结论不变） |

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
