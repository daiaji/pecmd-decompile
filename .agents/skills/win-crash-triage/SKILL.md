---
name: win-crash-triage
description: 无调试器环境下定位 pecmd_msvc.exe 崩溃/挂起/OOM 的完整流程 (LocalDumps+minidump解析+探针法)
whenToUse: 被测进程 0xC0000005 崩溃、消息泵挂起、或弹窗循环时
---

# WIN 崩溃/挂起分诊手册

> **R20 更新**：本机现已装配 windbg MCP 且实测全链可用（attach 抓栈/断点/dump 解析）。
> 活体调试一律走 `vgate-live-debug` 技能的五道门；本文保留为"无调试器兜底路径"
> （LocalDumps 自解析 + GetThreadContext 直读 + 探针法），MCP 不可用时的完整替代。

环境无 cdb/windbg/procdump。以下方法全部实战验证过。

## 0. 通用准备

```powershell
Get-Process pecmd_msvc -ErrorAction SilentlyContinue | Stop-Process -Force   # 先清残留 (锁 exe)
```

WER LocalDumps 已配置 (HKLM\...\Windows Error Reporting\LocalDumps → C:\pectest\dumps, DumpType=2)。
未处理异常自动落 dump; **强杀/挂起不会落 dump**, 见 §2。

## 1. 崩溃 (0xC0000005 等) — dump 解析

工具: `tools/debug/parse_dump.py <dump路径>`。输出异常 RIP/rax/rcx → 模块偏移 → 栈回扫。

关键坑 (文档与实测不符处):
- MINIDUMP_EXCEPTION 的 ExceptionAddress 实测在 offset **24** (非文档的 16; offset 16 是 ExceptionRecord)
- ThreadContext 的 Rva **直接指向 CONTEXT 结构** (前面没有 MEMORY_DESCRIPTOR 头), DataSize=1232
- x64 CONTEXT: Rip=+0xF8, Rsp=+0x98, Rax=+0x78, Rcx=+0x80
- 模块表 entry 108 字节: `Base(8) Size(4) Cksum(4) Ts(4) NameRva(4)` + VS_FIXEDFILEINFO(52)+24
- Memory64List(stream 9): `count(8) baseRva(8)` + 每 range `start(8) size(8)`, 数据区顺序排列

RIP→符号: exe 内偏移 = rip - 实际基址 (dump 模块表里有); map 文件行格式
` seg:off   name   静态VA `, 静态基址 0x140000000, 二分找 ≤ 目标的最近符号。
**.map 被 gitignore, 用之前确认 build\msvc\pecmd_msvc.map 还在**。

判读经验:
- rip 在 ntdll 且 rax=0 → 典型 memcpy(dst, NULL)/间接调空指针; rcx 常是第一参数,
  减 exe 基址得偏移后查 map 的 DATA 符号可锁定全局变量 (CS 崩溃案即此法定案)
- 栈上出现的 IAT 槽地址 (exe+rdata 区) 不是返回地址, 别误读

## 2. 挂起 — GetThreadContext 直读

强杀不产生 dump。用 `tools/debug/thread_rip.py <pid>` (ctypes, Suspend+GetThreadContext):

- 主线程 rip 在 win32u.dll ≈ GetMessage 泵 (GUI 等待); 多个工作线程同址在 ntdll = 事件等待
- 结合模块枚举 (EnumProcessModulesEx+GetModuleInformation) 把 RIP 翻译成 模块+偏移

## 3. 弹窗循环 / 行为分叉 — 探针法

GUI 子系统无 stdout。fopen 追加写 `C:\pectest\memfail.log` 逐段插桩二分:

```c
{ /* TEMP PROBE */
    DWORD le__ = GetLastError();          /* V4 透明化: 必做! fopen("a") 成功置 LastError=183,
                                             曾污染退出码链(见 analysis/r19b_exit183_chain.md) */
    FILE *pf_ = fopen("C:\\pectest\\memfail.log", "a");
    if (pf_) { fprintf(pf_, "PROBE step-name\n"); fclose(pf_); }
    SetLastError(le__);
}
```

其余探针纪律: 手写 CRT extern 禁 `<stdio.h>`; 多进程禁共写同一日志;
定位完成后必须移除 (T5)。完整契约见仓库根 AGENTS.md「探针」。

现有探针网分布在 core_main/core_init/core_script2/core_scriptrun/core_string/core_var 六文件
(全局搜 `TEMP PROBE`)。**定位完成后必须移除** (纪律)。

弹窗内容识别: OOM 弹窗来自 FUN_1400630D0(mode) — mode=0 "内存错误", 其他 "内存不足!";
调用点 = PECMD_HeapRealloc/AllocWStringBuffer/StrDupAlloc 失败重试循环。

## 4. 原版行为对照 (别猜语义)

三重真源: decompiled.c 按地址 grep / 原版 EXE 二进制 (utf-16 字符串扫描判定命令存在性,
Python 一行 `data.find(name.encode('utf-16-le'))`) / 真机对照跑。
案例: WRITE 命令在此版本不存在 (exit=2=未知命令); TEXT 是屏幕显示不是写文件;
可用回捞通道 = EXEC cmd 重定向 或 REGI。
