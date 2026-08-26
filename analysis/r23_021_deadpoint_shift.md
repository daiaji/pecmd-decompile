# R23: 021 SUB 死点漂移实证 — R20C 种子化部分生效 (+0x75 → +0x9a)

> 2026-08-26 R23 轮 · V-Gate 门3 attach · 构建身份: md5 62e5ddd3 (HEAD 193b7d2,
> 含 R20C 双补丁 + R23 PATCH-1 修正), DEPLOYED_BUILD.txt 时间 23:48:15
> 方法: C:\\pectest 后台启动 `pecmd_msvc.exe LOAD 021_sub_call/run_all.pecmd` (harness
> 同构场景), 挂死确认(10s 存活, PID 3428), attach → ~0s → k 30

## 1. 活体栈 (R20C+PATCH-1 版)

```
RIP   FUN_14001b23c+0x9a   (K=0x1e376da; PDB 命名; map PE 符号 FUN_14001b23c@0x1401e7640
      = unimplemented_stubs.obj 转发桩, 真体 PECMD_ExtractTableSegment @0x140122950
      core_b1_remaining.obj —— 死点位于真体扫描环, 符号双同名已登记)
      ParseAndExecuteLine+0xa2a ← ExecuteCommand+0x812 ← DispatchCommand+0xd9
      ← ProcessScriptBlock+0x18fa ← DispatchExpressionBlock+0xe61 ← RunScriptText+0xb6d
      ← srx_ExecuteScriptFile+0x64b ← RunCommand+0x124d ← ProcessScriptBlock+0x1c75
      ← DispatchExpressionBlock ← RunScriptText ← RunStartupScript+0x14cf
      ← RunBootScriptInFiber+0x1c ← MainW+0x5d7 ← main+0xf0
```

## 2. 死点比较

| 版本 | 死点 | 调用链 |
|---|---|---|
| 补丁前 (dc353e0e, R23 首抓) | FUN_14001b23c+0x75 | 与本次完全同构 |
| R20C+PATCH-1 (62e5ddd3) | FUN_14001b23c+0x9a | 同上 |

- 同一扫描环内偏移前移 0x25 → **R20C 的 srx 入口种子化改变了对象/文本状态但未终结自旋**，
  与 r20c 档案"单点种子化只覆盖其一, 死态异质"结论一致。
- 附带发现: 同一裸 main.pecmd 手动运行(无 harness 尾声)为 0xC0000409 fastfail 快速退出
  (2s 内), 而 harness 场景挂死——同一源码在堆垃圾不同分布下呈现异质终局, 进一步佐证
  未初始化读取的可变性。

## 3. 机制链更新 (r20c §1 修订)

- ExtractTableSegment (dc:15775/rb:7955) 扫描**文本流**找 0x88/8A/90 分隔符值或终止符;
  InitObfuscatedKeywords (core_exec2.c:291-322) 只在脚本对象上写分隔符值, 不改文本。
- RunScriptText (core_execmain.c:218-229): pPersist==NULL 时 calloc(0xf0)+ScriptCopy
  (FUN_140017CDC 覆盖 0x00-0xEB)+ScriptInit → pExec=克隆体; srx 种的 +0x88/8A/90 会被
  拷贝, 但**文本流本身从未被种子化**——若扫描文本来自未初始化/再用缓冲, 种子化对象不解决。
- → 关键疑问(下轮活体取证): 扫描指针 *param_4 指向的文本缓冲内容是什么、有无 NUL。
  r20c "关键字块"叙述部分需要按"文本流"重新对照 dc:15775 语义。

## 4. 下轮动作

1. 021 再抓一次, attach 后直接读: (a) RIP 帧 *param_4 文本指针前 64 字节
   (b) script+0x88/8A/90 值 (c) 若 param_4 指向 bufRef 则查谁分配它
2. 002/004 抓栈(与 021/037/038 非同机——002=展开前置区, 004=TEAM 内层)
3. 判定后选择: 对文本分配点补种子/终止符(须 dc 对应位置有证据) 或 修扫描器等价位置
