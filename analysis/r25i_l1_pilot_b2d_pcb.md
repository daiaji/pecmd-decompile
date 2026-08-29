# R25i · L1 语义化试点 C2 — PECMD_ParseCommandBlock @0x140025474

- 对象: `src/commands/core_b2d.c` 447-639 行（dc 对应 `reference/decompiled.c` FUN_140025474 @dc:22474 size=1290）
- 性质: 纯改名（Ghidra 占位名 → 语义名），零语句/类型/顺序改动；函数头注释与 dc 引注注释原文保留
- 方法: python `\b旧名\b` 严格限定 447-639 行替换；逐行 round-trip 反向映射校验（反演后与原文逐字节一致）+ 行界外逐字节比对 + `git diff --stat` 确认仅本文件 116/116 行对称变更（总行数 2317 不变，CRLF/无 BOM 保持）

## 映射表（旧名 | 新名 | 证据 | 站点数）

| 旧名 | 新名 | 证据（dc 行号/行为） | 站点数 |
|---|---|---|---|
| uVar4 | last_result | 返回值累积器：:506/:570 TokenizeExpression、:625 ProcessScriptBlock LowPart 赋值，:493/:498/:581/:629 以 `(int)` 截断返回（dc:22498 uVar4=0 起） | 10 |
| bVar2 | f_lt_jump | flags&2 扫描中见 `'<'` 置 true（:474,dc:22507-22508）；决定走 TrimTrailingSeparator（:585,dc:22612）且尾段不 +1 前进（:632,dc:22646）——`<` 直跳模式标志 | 5 |
| WVar12 | sep_char | 自定义分隔符，默认 `L'|'`（:455,dc:22500），由 17 个可选字符之一覆盖（:483,dc:22520），贯穿 RemoveDuplicateChar/TrimTrailingSeparator/[] 扫描 | 8 |
| sVar1 | lead_char16 | 分隔符声明扫描的续行条件：`*(int16_t*)*pp` 当前首字符 16 位视图（:469/:486,dc:22502/:22523） | 4 |
| pWVar5 | scan_ptr | 万用工作游标：扫描环中 `*pp` 快照（:472,:521），或 RemoveDuplicateChar/TrimTrailingSeparator 返回的分隔符命中位=下一段锚点（:509,:543,:586,:589,:627-:633） | 55 |
| pWVar9 | seg_start | 当前段/块文本起点：主环快照（:496,:500,dc:22535）、`[]` 内容起点（:517,dc:22556）、内层段起点（:538,dc:22570），拷贝源与回扫基线（:533,:551,:555） | 20 |
| pWVar10 | seg_end | 段拷贝终点指针：无分隔符时为末字符字节地址（:595,dc:22621）、否则命中位-1（:598,dc:22624），回扫控制字符后作 StrCopyW 长度端点（:619,dc:22636，R25-g 三锚点之一） | 15 |
| pWVar11 | body_ptr | `[]` 内嵌 `{...}` 体指针：`{`后首址存入 body_text（:535/:539,dc:22567/:22571），复用为回扫 `'}'` 游标=体终点（:550-:558,dc:22582-:22586） | 14 |
| WVar8 | cur_char | 扫描环当前/待匹配 WCHAR：`'{'` 字面量匹配（:491,:500,dc:22528/:22537）、`[]` 前扫（:520-:523）与内层环（:541-:564）的当前字符 | 9 |
| local_res10 | seg_slot | 当前段文本槽：memset+AllocStrSlot 分配（:583-:584,dc:22611），QuadPart 装 WCHAR* 传 ProcessScriptBlock（:624,dc:22638），FreeStrBuf 释放（:628/:635） | 9 |
| local_58 | body_text | 体文本指针三态：`{`后体起点（:539,dc:22571）→ StrCopyW 拷贝源（:557-:558,dc:22586，R25-g 锚点）→ 别名 blk_text 后作 TokenizeExpression 游标（:566,:571,dc:22594/:22598） | 6 |
| local_48 | body_check_ptr | blk_text 别名，SkipLeadingControls 推进后做体空判（:567-:569,dc:22595-:22597） | 4 |
| local_40 | blk_prefix | `[]` 与内嵌 `{` 之间的文本缓冲：StrBldCopyWideN 收集（:533,dc:22566），作 TokenizeExpression 第 5 参（:571,dc:22599；`{` 分支传 g_szEmpty），FreeStrBuf 释放 | 4 |
| local_50 | blk_text | StrBld 缓冲收集内嵌 `{...}` 体文本：AllocWStringBuffer(:540,dc:22572)→StrCopyW(:557)→别名给 body_text/body_check_ptr→FreeStrBuf(:574,dc:22602) | 6 |
| iVar3 | text_len | `lstrlenW(*pp)` 余文长度，用于末字符字节地址运算 `*pp-2+len*2`（:593-:595,dc:22620-:22621，R25-g 锚点） | 2 |
| LVar6 | proc_ret | PECMD_ProcessScriptBlock 返回的 LARGE_INTEGER，取 LowPart 符号截断入 last_result（:622-:625,dc:22638-:22640） | 3 |

合计 174 处标识符替换；R25-g 三处修复锚点（:533/:555(实际长度式 :557)/:611(:619) 及 :591(:595) 字节地址末字符）仅换名、语义零改动，`/* dc:xxxx */` 注释原样保留。

## 保留清单（行界内 Ghidra 名残留，共 4 处，均在 dc 引注注释内）

| 行 | 残留 | 原因 |
|---|---|---|
| :556 | pWVar11, local_58 | dc:22626 引注注释原文（`注释文本不动`纪律），引用的是 dc 侧变量名，作为 dc 行号证据保留 |
| :594 | iVar3 | dc:22653 引注注释原文，同上 |
| :615 | pWVar10 | dc:22675 引注注释原文，同上 |

其余 12 个旧名行界内残留计数为 0；新名在 `src/`+`include/` 全库 `\b` 级冲突预检均为 0。

## 过程备注

- 第一遍替换的注释判定误把行首 `*`（解引用表达式，如 `*pp = pWVar5 + 1;`）当注释行跳过；第二遍以修正判定（仅 `/*`、`//`、`* `、`*/` 开头为注释）补齐 18 行、+28 处（scan_ptr+24 / seg_start+3 / body_ptr+1），逐行 round-trip 校验通过后落盘。
- 校验三件套：① 行界内每行反向映射复原==替换前逐字节一致；② 行界外（1-446、640-2317）逐字节一致；③ `git diff --stat` 仅 `src/commands/core_b2d.c`，116 增/116 删对称。

## 交付物

- `src/commands/core_b2d.c`（本文件，447-639 行 L1 语义化）
- `analysis/r25i_l1_pilot_b2d_pcb.md`（本文）
