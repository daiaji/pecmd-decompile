# R25-i · L1 语义化配方（以双试点为活样例）

> 目标：ROADMAP G3 门——精修区 A 区（b2f/b7c/b8h）local_xx≈0；全库 M2+M3=40943 处，
> 前 10 文件占 82%。每批改动后全量 63/63 必须保持（硬门）。

## 活样例

- `analysis/r25i_l1_pilot_b2f_c06c.md`（C1：22 名/205 站点，保留 10 名多角色寄存器复用）
- `analysis/r25i_l1_pilot_b2d_pcb.md`（C2：16 名/174 站点，4 处 dc 引注内保留）
- 提交：096d554（构建+全量回归 63/63 零回归，md5=d5b769ee）

## 配方（七步）

1. **读双源**：msvc 函数全文 + dc 对应函数（`/* ========== NAME @0xADDR ==========*/`
   头取地址，grep 定位 decompiled.c），逐语句对齐。行界 = 函数头行到下一函数头前。
2. **建映射表**：每个 Ghidra 名（local_xx/uVarN/bVarN/cVarN/iVarN/lVarN/LVarN/DVarN/
   BVarN/WVarN/psVarN/pLVarN/param_N/…）→ snake_case 语义名，附 dc 行号或行为证据一句话。
   - **语义不明的保持原样**（宁缺毋滥）：Ghidra 寄存器/槽复用名（一个槽串多角色，如
     b2f 的 local_res20 同时当标志位/命令串/句柄出参）单名必误导其一，登记后保留。
   - 已语义化名（strb1/probe_ui/作者重写名）不动；注释文本与函数头不动。
3. **撞名预检**：新名先 grep 本文件 + include/ + src/ 全树，0 撞名才可用；禁用
   Windows 宏词（min/max/small/near/far/TRUE/FALSE/ERROR/DELETE/COPY/…）。
4. **应用**：python `\b旧名\b` 全字替换，**严格限定函数行界**（同名占位名跨函数普遍存在，
   越界即污染）；除标识符外零字符改动（空白/换行风格原样）。
5. **纯度自检**：① 行界外逐行字节一致断言；② 行界内残留 Ghidra 名计数（保留者逐个列
   原因）；③ `git diff` 反推复原 == 原文（round-trip 断言）。
6. **验证**（主代理）：双绿门构建 → post_build → 全量 63 案 diff（任何一 FAIL 即整批
   回滚排查）→ 抽查改名语义质量 → 提交。
7. **登记**：映射表落 analysis/，M2/M3 复测记入账本。

## 派单纪律

- 单文件独占：并行子代理各占一个文件；同一文件的多函数串行（或一人多函数）。
- 只改标识符：发现"改名需要改语句结构/类型/顺序"= 名字选错，换名而非改码。
- 构建禁止：子代理不构建不提交，主代理统一验证。

## 批次规划（按 语料覆盖真值路径 × A 区优先）

| 批 | 目标 | 密度 | 状态 |
|---|---|---|---|
| 试点 | b2f FUN_14003C06C / b2d ParseCommandBlock | 205+174 | ✅ 096d554 |
| 1 | b2f PECMD_EvalLoopCondition @0x140032dc4 | 412 | 进行中 |
| 1 | b7c 高密度函数 2-3 个 | ≤400 | 进行中 |
| 2 | b2f PECMD_DispCommand / HelpDlgProc / FUN_14003B540 | 444+321+215 | 待派 |
| 3 | core_b3_remaining.c（CALC/SED 体等，语料覆盖） | 12058 | 待派 |
| 4 | restored_bodies.c / core_b1_remaining.c | 9527/2907 | 待派 |
