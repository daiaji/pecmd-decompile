# R25-j 子代理任务书：D-20 `*map:` 执行体补全 — RUN 命令映射脚本执行路径

## 背景
- 仓库：D:\repo\pecmd-decompile。dc = reference/decompiled.c 唯一语义真值；msvc = 参考实现。
- D-20（docs/divergences.md）：RUN 命令的 `*map:` 形态在 msvc (src/lang/core_scriptrun.c
  PECMD_RunCommand @0x140031454 的 map 分支, 约 392-424 行) 只做 MapViewOfFile 读取
  (首 8 字节 size → 按 size+2 remap → local_1a0=映射基址) 然后 `TODO(verify)` 弃置
  (`branches |= 1; goto srx_tail;`)，映射文本从不被执行。
- dc 真值：映射成功后映射文本走「编码脚本流」协议最终由 FUN_1400b638c(RunScriptText) 执行
  （dc:30239-30272 拷贝/解码/封头 → dc:30268 执行）。

## 你的任务（单文件独占：只准写 src/lang/core_scriptrun.c）
### 第 1 步：反汇编定案（家规：Ghidra 类型汤必须回溯原版 EXE 反汇编）
dc:30016-30240 的控制流被 Ghidra 类型复用污染，关键比较
`if ((longlong)local_178 <= (longlong)ptVar40)`（dc:30018，决定「普通路径」vs「map 执行块」）
不可直接采信。要求：
- 用 objdump -d --start-address=... --stop-address=... 对 reference/PECMD原始.EXE 或
  reference/PECMD.exe（ImageBase 0x140000000）反汇编 0x1400320a0-0x140032120 附近
  （dc:30016-30240 对应区间，自行用 dc 行内地址标尺 LAB_140032341/LAB_140032a2e 校准），
  确认该比较的真实寄存器/立即数语义（本文件已有先例注释：「ASM 1400319c1/9cf/9d9 核实」，
  沿用该风格把你的 ASM 证据写入注释）。
- 特别核对：map 成功路径（dc:29871 `goto LAB_140031d26`）之后到达 dc:30240 块的实条件；
  piVar1(=local_178=映射 size) 在 dc:30244 `FUN_140063424(&local_228,(longlong)(piVar1 + 9))`
  与 dc:30246 memcpy 里的真实单位（字节 vs 字符）。
- 若反汇编工具不可用，改用 reference/ghidra_out/ 下产物交叉核对；仍不可得则在报告中
  登记「条件未定案」并按最保守直译（dc 字面 `size <= (longlong)ptVar40`）实现+TODO(verify)。

### 第 2 步：移植 map 执行块（dc:30239-30272）
msvc 全部依赖真体已核实在册（勿重复定义，按文件既有 extern 惯例引用）：
- PECMD_AllocSmallObject @0x140063344 / PECMD_GrowByteBuffer @0x140063424 /
  PECMD_MemMoveForward @0x14001d78c / memset(CRT, dc:FUN_140102a90=零填充 0x14 字节) /
  UnmapViewOfFile(Kernel32) / FUN_1400E7D58=PECMD_ResDecode @0x1400e7d58(真体 core_resdecode.c:49) /
  PECMD_GenRandomSeed16 @0x14001b510(core_var3.c:80) / FUN_14001B5AC=XorEncode @0x14001b5ac
  (core_var3.c:103) / PECMD_StrChrOffset @0x14001b4f8(core_var3.c:120) /
  PECMD_InvokeSubRoutine @0x140030dcc(core_scriptdep.c:204) /
  PECMD_PrependEnviHeader @0x140024f20(core_scriptdep.c) /
  FUN_1400b638c=PECMD_RunScriptText(已有真体, 查 src/lang/core_execmain.c 或 rename_map)。
- dc 序列逐句直移（dc:30239-30272），每句附 dc 行号注释：
  AllocSmallObject(&local_228) → GrowByteBuffer(&local_228, size+9) → MemMoveForward(local_228, 映射基址, size)
  → memset(local_228+size, 0, 0x14) → UnmapViewOfFile(local_1a0) → ResDecode(&local_228, 1)
  → seed=GenRandomSeed16() → WVar33=(seed<<8)|(seed&0xff) → XorEncode(local_228, WVar33, 0)
  → InvokeSubRoutine(&local_228, script, WVar33)（dc:30260，参数次序以 msvc 真体签名为准核对）
  → iVar11=StrChrOffset(local_228, WVar33) → local_228[iVar11]=WVar33^0xd; [iVar11+1]=WVar33^0xa
  → lVar41=iVar11+2; [lVar41..lVar41+4]=WVar33（5 个填充）→ XorEncode(local_228, seed^WVar33, iVar11+6)
  → DAT_14013a248<0 则置 1 → PrependEnviHeader(seed, &local_228, local_258, local_294, 0)
  → RunScriptText(script, local_228, ptVar15, local_1f8, (seed<<0x10)|uVar31|0x40, NULL, NULL)
    （dc:30268 的 uVar31/local_258/local_294/local_190 与 msvc RunCommand 现有局部量的对应关系
    由你核对 dc 与本文件上下文后定，写入报告）。
- 完成后删除原 `TODO(verify): 反编译 379-390 行映射数据拷贝与解码` 注释并接回主流程
  （map 路径返回 RunScriptText 结果；失败路径保持 dc:29858-29874 的 -0x7ff8ffa9 返回语义）。
- 禁止改动本文件之外的任何文件；禁止构建/git/MCP。

## 交付物（先落盘后回话）
1. src/lang/core_scriptrun.c — map 分支补全后的 PECMD_RunCommand。
2. analysis/r25j_d20_map_port.md — 报告：ASM 定案证据（比较条件/单位）、dc→msvc 映射表、
   参数对应表（uVar31/local_258/local_294/local_190/ptVar15 → msvc 局部量）、TODO(verify) 清单。
3. 结构化摘要：RESTORED/SKIP(原因) + 定案结论。

【自主处理纪律 - 禁止提问/卡住】
- 你是一个自主执行体：遇到任何歧义、缺失信息、目标含糊、与预期不符的情况，不要提问、不要等待确认、不要卡住。
- 一律按以下方式自行决策并推进：
  1) 歧义 → 采用最合理/最保守的解释（不臆造语义、以证据为准），并在报告中注明你的取舍；
  2) 缺失符号/类型/依赖 → 自行补最小桩（与项目约定一致），或按文件既有模式处理；
  3) 无法验证 → 按"不虚构语义"原则登记 SKIP(具体原因) 并继续下一个目标；
  4) 遇到工具/环境错误 → 先重试一次；仍失败则记录原因并切换到可推进的替代路径；
  5) 永远不要以"需要用户决定"结束回合；总是以可交付物（代码/报告/文件）结束。
- 完成后给出结构化报告：每个目标 RESTORED/SKIP(原因)/未触及。

## 附加纪律
- 单文件独占：你只能写 src/lang/core_scriptrun.c 与 analysis/r25j_d20_map_port.md。
- 不虚构语义：无法忠实移植的按 TODO(verify)+登记处理，不得臆造。
- 不提交 git（主代理统一提交）。中文注释按仓库惯例；注释内不得出现非终止用途的 */ 序列。
