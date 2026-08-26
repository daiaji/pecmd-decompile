

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
