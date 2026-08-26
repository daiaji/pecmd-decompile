---
name: vgate-live-debug
description: V-Gate 五道门活体调试范式（attach优先/symsnap取址/版本绑定/探针透明化），R20 双误诊复盘后确立
whenToUse: 需要对 pecmd_msvc.exe 做活体断点、attach 抓栈、内存取证，或评估"某工具坏了"类结论时
---

# V-Gate 活体调试五道门

> 背景：R18"断点不可用"与 B 簇 exit=183 两起事故同源——观测装置/过期工件污染被当成产品缺陷。
> 本技能取代 win-crash-triage 中"环境无 windbg"的旧前提（windbg MCP 现已实测全链可用，
> 见 docs/WINDBG_MCP_ISSUES.md R19 裁定附录 + R20 纪律）。

## 门 0 会话初始化（任何调试调用之前）

1. 读 `C:\pectest\DEPLOYED_BUILD.txt`，与本地 `build\msvc\pecmd_msvc.exe` 的 MD5 比对；
   不一致 → 先重新部署，禁止在身份不明的二进制上调试。
2. 所有断点地址从当期 `build\msvc\symsnap.txt` 取（构建后由 `tools\make_symsnap.sh` 生成）。
   **禁止复用历史会话笔记/docs 里的 module+offset**（六轮重建即可让旧 RVA 指进无关函数）。

## 门 1 下点自证

set_breakpoint 后：`bl` 确认解析地址 → `u <addr>` 确认指令边界与预期函数序言 →
首次命中后 registers 核 rip 落在该函数 map 区间内。"从未命中"只有门 0+1 都满足后才配成立。

## 门 2 阳性对照

任何"X 不工作"假设先打一发必中断点（`kernel32!ExitProcess`）。对照不中=环境问题；
对照中+目标不中才是真异常。R18 误诊若当时做过此步十分钟即翻案。

## 门 3 attach 优先（挂死类首选）

进程活着不退（挂死/自旋）时：`attach_process <pid>` → `~* k 30` 全线程栈 →
从指令流反解对象指针 → `read_memory` 直接看运行时数据。**全程零断点地址依赖，天然免疫漂移。**
实例（R20b）：021_sub_call 自旋点 FUN_14001b23c+0x75 一发钉死，关键字块垃圾值直接读出。

## 门 4 探针透明化（配合门 3 用）

探针块进出保存/恢复 GetLastError（fopen("a") 成功路径置 183，曾污染退出码链）；
手写 CRT extern 禁 stdio.h；多进程禁止共写同一日志。

## 门 5 结论门禁

否定性高影响结论（"工具/路径坏了"）归档或上报上游前三关全过：
当期构建复现、阳性对照、第二代理证伪尝试。

## 收尾

每次会话（含 dump 只读会话）用完立即 `end_session`——worker 持 PDB 句柄，
残留 = LNK1201 构建阻塞；调度构建前还要确认无在途子代理持有取证会话（V6）。
