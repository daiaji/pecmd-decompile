

## R19 (2026-08-26) — 全案崩溃根因三连修+ECD四件套，基线 0/63 → 9/63
- 展开器 ExpandEnvVars(core_execline.c): H1 补 dc:78742 游标强制同步(inP=raw_src)；H2 b961 支路按 dc:78569-78571 自带 src/dst 装载(原双重递减+未初始化)；H3v2 未闭合名路径按 dc:78427-78428 直跳拷贝序言(v1 误经 b961 致自旋已纠正)。
- ECD EXEC=259 根因(analysis/e26c_exec_wait_rootcause.md): DAT_14013a24f 对象分裂→stubs_common.h 宏别名统一 g_flagA24F+删孤儿定义+初值=1；WaitTickCount(rb) 按 dc:59109-59144 直移为消息泵(原体错写全局标志递减)。旧工单裸 WFSO 方案正式废弃。
- N-1: RasDialEntry 缺省入口 HelloASL 回退补齐(dc:144416-144423)。
- 分析产物入库: FUN_140034788 汇编还原(385行)/FourCC 干跑普查(227行)/batch-A 二轮复核(175行,#083 虚警证伪撤销)/e26c 根因(254行)/SUB静态笔记(132行)/windbg MCP 上游复现文档。
- 已登记缺口: FUN_140099f18 -wlan 半区 ~508 行、PECMD_RunServiceCommand 桩、core_b2f.c:3829 占位误归属(IFEX/FIND 执行体)。
