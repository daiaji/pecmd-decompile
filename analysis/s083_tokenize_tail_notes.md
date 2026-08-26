# S083 — 轻量交叉验证报告（改道后任务）：FUN_1400a53e4 ↔ core_b3_remaining.c

> 任务书原前提（"TokenizeExpression 缺尾部约 690 行，证据区 dc:103884-104028"）已被证伪。
> 本笔记为改道后的交叉验证结论；原转录起草已存档于同目录
> `s083_tokenize_tail_draft.c`（顶部有"前提证伪存档"说明）。

## 一、范围核定（修正后的口径）

| 项 | 权威事实 |
|---|---|
| FUN_1400a53e4 = PECMD_TokenizeExpression | dc:103346(头注释)-**103731**(闭括号)，正文 dc:103349-103730 |
| 现有实现 | `src\commands\core_b3_remaining.c:24757-25171` |
| dc:103884-104028 实际归属 | **FUN_1400a600c = PECMD_CreateMenuItem**（dc 头注释 103826，体 103829-104037），已转录于 `src\commands\core_b3r_h4.c:724-950` |
| "~690 行"成因 | 103346→104028 跨度 683 行 ≈ "~690"，系把 TokenizeExpression 头一直数到 104028、跨过 FUN_1400a5d7c(dc:103735)/FUN_1400a5e6c(dc:103766) 两个完整小函数所致的边界错认 |

## 二、交叉验证方法与结果

方法：标签集合比对 + 全函数调用序列 1:1 映射比对 + 三处重点逐语句并排抽取比对。
工具：pwsh 文本切片（非构建、非测试，符合禁令）。

### 基线（全函数）
- 标签集：dc {LAB_1400a5424, LAB_1400a598c} ↔ ref 同集，无缺失。
- 调用序列 38 项次序完全一致，映射：FUN_140017cdc→PECMD_ScriptCopy、FUN_1400186bc→PECMD_ScriptInit、
  FUN_140063620→PECMD_AllocStrSlot、FUN_140063720→PECMD_AllocString、FUN_140073ccc→PECMD_ArgTokenize、
  FUN_14006156c×5→PECMD_MatchPattern、FUN_1400702d4→PECMD_StrBldCopyWideN、FUN_14006213c→PECMD_MatchClosingBracket、
  FUN_14001b23c→PECMD_ExtractTableSegment、FUN_14006e030→PECMD_CopyPathToken、FUN_14004c0bc→PECMD_ProcessScriptBlock、
  FUN_14005b0b8→PECMD_ZeroLenBuf、FUN_14005b104×4→PECMD_FreeStrBuf、FUN_14004eaa8→PECMD_ClearTaskTable、
  FUN_140066838→PECMD_ForwardCall_6838、FUN_140066f64→PECMD_ReleaseObjectSlots。
- 函数体内无 DECOMPILE FAILED 片段。

### 重点 1：两处递归点 —— 逐字对应 ✓
- **递归点 1**（dc:103517 ↔ ref:24932）：
  实参五元组完全一致：`(LVar13, *(u64*)(param_1+0x40), &local_88, 0, &DAT_14011c638)` ↔
  `(LVar13, *(int64_t*)(param_1+0x40), (int64_t*)&local_88, 0, &g_szEmpty[0])`；
  外层守卫 `if (cVar14=='\0') { if (*pWVar20 != L'\0') { local_88=pWVar19; …回写 pWVar20=local_88; } }` 结构一致。
- **递归点 2**（dc:103581 ↔ ref:25004）：
  `(local_80, *(u64*)(param_1+0x40), &local_88, 0, local_58)` ↔ 同构；守卫 `if (cVar14==(char)iVar15)` /
  `if (*pWVar20 != (WCHAR)iVar15)` / `local_88 = pWVar19 + 1` 一致。
- 第五参符号映射 `DAT_14011c638 → g_szEmpty` 为项目规范映射（include\pecmd_globals.h:407 注释
  "DAT_14011c638 .rdata 空串"；REVIEW.md:2688-2689 记录了全库归一规则）。

### 重点 2：param_4&1 分支 —— 对应 ✓
- 开分支条件：dc:103487 `if ((param_4 & 1) == 0) {` ↔ ref:24898 同句（大扫描 do-while 挂在 ==0 侧）。
- else 侧（参数替换模式）：dc:103693 `FUN_140025474(local_80.QuadPart,param_3,1,pWVar20); iVar15=(int)uVar10;
  LVar9=local_80;` ↔ ref:25132 `PECMD_ParseCommandBlock(local_80.QuadPart, param_3, 1, pWVar20); iVar15=(int)uVar10;
  LVar9=local_80;` —— 实参表、返回值处理、LVar9 刷新逐一一致。
- 分支内局部细节抽查（均一致）：local_28 选择逻辑(dc:103482-103486↔ref:24893-24897)、
  cVar14 置位(dc:103495↔ref:24906)、LAB_1400a598c 双路径汇入(ppWVar17=&local_58 / &local_68)、
  Ghidra 伪迹 `iVar15=0 后作字面 0 比较`(dc:103574-103577↔ref:24997-25000)被忠实保留。

### 重点 3：清理尾（dc:103719-103730 ↔ ref:25158-25170）—— 12 行逐语句对应 ✓
lVar8=(i64)iVar15 → FreeStrBuf(local_70) → FreeStrBuf(local_60) → EnterCS(g_csInit=DAT_14013e190) →
if(local_20){ForwardCall_6838(local_20,LVar9); ReleaseObjectSlots(lVar6,LVar9);} → LeaveCS → } → return lVar8。
其上游的状态合并/游标还原段（dc:103697-103718 ↔ ref:25136-25157）亦逐语句对应：
状态字 `(…|cVar2)&0x1ef` 合并、0xca→0xd0 定位槽回传、local_74!=0 时 0x24/0x28/0x70/0x60/0x68 快照还原、
克隆脚本 ClearTaskTable+FreeStrBuf(+0x70)+free。

## 三、发现的偏差清单（全部为良性，无真实缺失）

| # | 位置 | 偏差 | 判定 |
|---|---|---|---|
| D1 | ref:25169 | 多出 `(void)param_2;` | 未用形参告警抑制的空语句，无语义影响 |
| D2 | dc:103698 ↔ ref:25137 | `\|(short)cVar2` vs `\|(uint16_t)(char)cVar2` | 截断到 16 位后等价（cVar2=-1 时两侧均并入 0xFFFF）；类型书写形式差异 |
| D3 | 全函数 | FUN_*/DAT_* → PECMD_*/g_* 规范改名 | 项目既定改名映射，非偏差 |

**真实缺失/结构偏差：无。**

## 四、结论

**完整无缺。** FUN_1400a53e4（TokenizeExpression）在 refactored\src\commands\core_b3_remaining.c:24757-25171
的实现与 decompiled.c:103346-103731 的权威反编译在标签集、调用序列、两处递归点实参、param_4&1 分派、
清理尾逐语句五个层面全部一一对应，不存在任何缺失尾段或语义偏差；
原任务书所指 dc:103884-104028 属 PECMD_CreateMenuItem（亦已完整转录于 core_b3r_h4.c）。
S083 无需补齐动作，本项关闭。

## 五、遗留风险提示（不阻塞关闭）

1. `core_b2c.c:143` 与 `core_b2d.c:132` 对 PECMD_TokenizeExpression 的本地 extern 原型
   （int64_t ctx, ... 形式）与正式定义（LARGE_INTEGER, int64_t, ...）不一致——x64 ABI 下恰好兼容，
   属既有隐患，建议后续统一。
2. 行号会随 decompiled.c 再生成漂移，后续引用一律用地址锚定（@0x1400a53e4 / @0x1400a600c）。
3. 本验证未触碰 src 树、未构建、未运行测试（遵守禁令）；工作区 git 中 src 下两处改动
   （core_execline.c/core_b8m.c，时间戳早于本次会话写入）为他人/前序会话遗留，与本任务无关。
