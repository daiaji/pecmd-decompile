# WINDBG-MCP 上游缺陷复现报告：断点命中不停止（bp/ba hit → no stop）

> 编纂：2026-08-26 | 用途：直接提交 windbg-mcp 上游 | 配套病理档案：`docs/WINDBG_MCP_ISSUES.md`

## EN Quick Summary (for upstream paste)

Software breakpoints (`set_breakpoint` tool or `execute("bp ...")`) register successfully
(visible via `bl`) but the **hit event never stops the target** — the process runs to
natural exit. Hardware/data breakpoints (`ba`) detonate `0x8000FFFF` on first `go`.
Meanwhile: free-run via `execute("g")` (no breakpoints) works perfectly with full event
observability, single-command failures no longer poison the session, and dump-session
read-only analysis is unaffected. Conclusion: the **breakpoint-hit → stop-event dispatch
path** in the worker is broken in the current build. It worked before the latest fix
round (see timeline below) — likely a regression introduced by the stop-wait state
machine change.

---

## 一、环境

| 项 | 值 |
|---|---|
| 宿主 OS | Windows Version 26100 (24H2)，x64 |
| 引擎 | dbgeng（windbg-mcp Rust 主服务 + python worker 组） |
| windbg-mcp 版本 | 【上报前请填：git hash / 版本号】 |
| 会话类型 | launch（活体），非 dump |
| 复现目标 | 任意用户态程序皆可；下例为本项目受控二进制 pecmd_msvc.exe |

## 二、主复现：软件断点命中不停止

1. `launch <target.exe>` → 正常停初始 int3。
2. `set_breakpoint(expression="<必经地址>")`
   （通用替代：任何必然调用的 API 入口，如 `kernel32!ExitProcess`；
   工具返回成功，事后 `bl` 确认断点在册且状态 enabled）。
3. `go`（专用放行工具）。
   - **预期**：命中断点停止，`registers` 可读、rip 落在断点处。
   - **实际**：目标一路运行至自然退出；`go` 返回的是**进程退出事件**。
4. 佐证：`.lastevent` 显示 `EXIT_PROCESS`（退出码事件），全程无 BREAKPOINT 事件；
   断点如同不存在。
5. **对照组（关键，用于上游二分）**：同一构建、同一会话模式下，
   不设断点直接 `execute {"command": "g"}` 自由运行 → 运行至退出，
   全程 `.lastevent`/输出回传可观测 ✅。

   ⇒ 说明 resume 通路与 stop-wait 基本盘是好的，
   缺陷特定于**断点命中事件的捕获/分发**环节。

本项目实测实例（供参考）：`launch C:\pectest\pecmd_msvc.exe LOAD C:\pectest\t2probe.pecmd`，
`set_breakpoint("pecmd_msvc+0x1a09a0")`（脚本块分发入口，脚本加载后必经），
`go` → 进程跑完整个脚本期退出，断点零触发。多轮多日稳定复现。

## 三、次要复现：数据断点引爆会话

1. `launch` 目标，停初始 int3。
2. 设置 `ba r/w <宽度> <地址>`（数据访问断点）→ 设置本身成功。
3. 首次 `go` → 立即返回灾难性错误 `0x8000FFFF`，此后该会话一切控制类命令
   `0x80040205`（worker 半死态，唯一出路 end_session）。
   —— 与历史病理条目 A 同族错误码。

## 四、行为漂移时间线（回归定位线索）

| 时点 | 软断点 bp+go 命中停止 | execute("g") 自由运行 | 单命令失败隔离 | 备注 |
|---|---|---|---|---|
| 2026-08-25（第一次修复前） | ✅ 正常（S14/S16/S17 三轮受控实验，n=3） | ❌ 吞命令→会话半死 | ❌ 污染 | 毒剂=execute 控制类语句、带自动命令串的 bp |
| 第一次上游修复后 | ✅ 仍正常 | ✅ 修复 | ✅ 修复 | 我方 R14c 实测通过 |
| 第二次上游修复后（当前，2026-08-26） | ❌ **命中完全不停止** | ✅ 保持正常 | ✅ 保持正常 | **本次上报的回归点** |

推断：第二次修复为修 `execute("g")` 动了 stop-wait 状态机，
副作用使**断点命中事件被吞或未映射为 stop**（事件分发路径断裂）。
建议上游在该次变更的 diff 内排查 breakpoint hit event → engine callback 的挂接逻辑。

## 五、给上游的取证抓手

- 复现时开 `RUST_LOG=debug` 起 server，用 `server_log`（level=trace/debug）抓取：
  断点注册请求 → 引擎 callback 注册回执 → 命中时刻的事件流（预期此处出现断层）。
- 健康基线对照：同会话先验证 `execute("g")` 自由运行可观测（排除环境因素），
  再设 bp 复现不停止。
- 完整历史病理档案（含 0x8000FFFF/0x80040205 错误码家族、S13–S17 受控实验台账、
  end_session 纪律等）：本仓库 `docs/WINDBG_MCP_ISSUES.md`。

---
*本文档事实全部来自本项目 2026-08-25/26 两日受控实验与实战记录，无推测混入；
推断性结论均已标注"推断"。*
