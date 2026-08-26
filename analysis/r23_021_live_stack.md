# R23: 021_sub_call 活体栈 — 补丁前版本死点复核

> 2026-08-26 R23 轮 · V-Gate 门3 attach（V1 symsnap 取址合规）
> 目标进程: C:\\pectest\\pecmd_msvc.exe (md5 dc353e0e = 7c8504e 时代构建, 不含 R20C 双补丁)
> 用例: 021_sub_call (main.pecmd: _SUB F / ENVI R=subok / _END / CALL F), LOAD run_all.pecmd, cwd=C:\\pectest
> 挂死确认: 启动 12s 后存活 (PID 13860, TID0 主线程), attach 于 ntdll breakin

## 主线程栈 (TID 0, k 40)

```
RIP   pecmd_msvc+0x1e7695        ← FUN_14001b23c (PECMD_ExtractTableSegment) +0x75
ret   pecmd_msvc+0x9f04a         ← PECMD_ParseAndExecuteLine +0xa2a
ret   pecmd_msvc+0xcedd2         ← PECMD_ExecuteCommand +0x812
ret   pecmd_msvc+0x49739         ← PECMD_DispatchCommand +0xd9
ret   pecmd_msvc+0x1a2e2a        ← PECMD_ProcessScriptBlock +0x18fa
ret   pecmd_msvc+0x1a6b11        ← PECMD_DispatchExpressionBlock +0xe61
ret   pecmd_msvc+0x1175dd        ← PECMD_RunScriptText +0xb6d
ret   pecmd_msvc+0x11e2cb        ← srx_ExecuteScriptFile +0x64b
ret   pecmd_msvc+0x11f58d        ← PECMD_RunCommand +0x124d
ret   pecmd_msvc+0x1a31a5        ← PECMD_ProcessScriptBlock +0x1c75
ret   pecmd_msvc+0x11d37f        ← PECMD_RunStartupScript +0x14cf
ret   pecmd_msvc+0x1e6f1c        ← PECMD_RunBootScriptInFiber +0x1c
ret   pecmd_msvc+0xf1a7          ← PECMD_MainW +0x5d7
ret   pecmd_msvc+0xf3b0          ← main +0xf0
      KERNEL32!BaseThreadInitThunk / ntdll!RtlUserThreadStart
```

## 结论

1. 死点与 r19a 附录活栈完全同构 (ExtractTableSegment+0x75 / ParseAndExecuteLine+0xa2a 偏移一致)
   → 021 在本轮抓栈时仍死于关键字块扫描自旋（三字符 0x88/8A/90 非零垃圾无 NUL 停机）。
2. 调用链确认裸 LOAD 入口: srx_ExecuteScriptFile → RunScriptText → DispatchExpressionBlock
   → ProcessScriptBlock → … → ExecuteCommand(CALL F) → ParseAndExecuteLine → ExtractTableSegment。
3. **版本前提**: 抓栈对象为 7c8504e 时代构建 (md5 dc353e0e, 构建于 20:44, 早于 R22 提交 20:56),
   **不含 R20C 双补丁**。本栈不能否定 R20C 补丁, 也不能确认其生效——必须重建部署后再抓。
4. 下一步: 用 HEAD 193b7d2 源码全量重建 → post_build 部署 → 复跑五案 → 仍 124 者再按
   V-Gate 门3 各自抓栈。若 021 修复, 则 R20C 补丁生效, 剩余死态各归其位。
