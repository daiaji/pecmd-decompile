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
  待与 dc 逐段甄别(两侧入参已铁证一致: 034788/032dc4 双入口 live 对照)；windbg MCP 每 2-3
  次操作后 0x80040205 半死复发(命令串 bp 触发, 病理已登记—exec0 快跑进程勿用 bp-after-launch
  查末段)；031/061 深水区下轮主工单(J 簇)。

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
