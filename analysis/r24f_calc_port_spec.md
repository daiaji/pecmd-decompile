# R24f 工单: PECMD_CalcExpression (FUN_1400bf358) 忠实移植 — 移植规格书

> 本文件是子代理作业的唯一规格。先读本文，再读模板函数与 dc 源，最后写代码。
> 产物: `src/commands/core_calc_expr.c`（新文件）。禁止修改任何其他文件。

## 0. 任务一句话

将 dc `FUN_1400bf358`（CALC 表达式主驱动器）从 118318 行处开始（约 118318..119158，
即 `reference/big_funcs/1400bf358_FUN_1400bf358.c` 全文）以**逐语句忠实移植**（非重构/非简化）
写入新文件 `src/commands/core_calc_expr.c`，替换现存的简化版 `PECMD_CalcExpression`
（位于 `src/commands/core_b3_remaining.c:28361`，标注"重构/简化版"——该版本语义失真，
成功路径返回堆指针导致 exit=指针低32，本轮必须根治）。

## 1. 目标签名（保持现签名，勿改）

```c
LPWSTR PECMD_CalcExpression(int64_t *param_1, WCHAR *param_2, uint64_t *param_3)
```

- param_1 = 脚本上下文（g_Script）
- param_2 = 命令行文本（PSB 传 LVar25+8）
- param_3 = 输出槽（PSB 传 NULL；其他调用方传 &value）

## 2. 风格模板（必须先读）

`src/commands/core_b3_remaining.c:19277` 起的 `PECMD_ParseExpression`
（FUN_14007E3A4 的既有移植体）—— 用它的全部 GIMPLE 手法惯例：
- 局部变量同名同型（LPWSTR/int64_t/...）；`pthreadmbcinfo` 系指针噪声一律按
  `WCHAR *`/`LPWSTR` 化；
- `ptr->refcount` 相关 = 指针首 WCHAR 读写（`*(uint16_t*)p`/`p[0]` 语义；
  对 WCHAR* 直接 `*p`）；
- `ptr->mbulinfo[k]` = WCHAR 数组下标访问；`ptr->mbulinfo[k + -6]` = `p[k]`（对齐 -6 是
  Ghidra 的槽头伪差，模板函数里已示范如何化掉）；
- `CONCAT62/44/71`、`SUB84/81` 等 = 按模板函数的 carrier 手法（定义局部载体变量）或
  直接写成等价 64 位运算，与模板一致即可；
- `thunk_FUN_1400f429c`、`LAB_1400xxxxx` 标号命名照 dc 原样保留。

## 3. 依赖 helper 映射（全部已存在，签名以各自定义处为准，在本文件顶部 extern 声明）

| dc 函数 | msvc 符号 | 定义位置（签名要点） |
|---|---|---|
| FUN_14005b154 | FUN_14005B154 | src/kernel/core_string.c:173 `WCHAR **FUN_14005B154(WCHAR **pp)` |
| FUN_1400702b0 | FUN_1400702B0 | core_string.c:154 `void FUN_1400702B0(WCHAR **ps, LPCWSTR src)`（赋值不释放） |
| FUN_14006375c | FUN_14006375C | core_string.c:108 `WCHAR *FUN_14006375C(WCHAR **ps, LPCWSTR src)`（追加） |
| FUN_140063720 | PECMD_AllocString | core_string.c `WCHAR *PECMD_AllocString(WCHAR **ps, int64_t count)`（保证容量） |
| FUN_140063694 | PECMD_AllocWStringBuffer | src/runtime/core_var.c `void PECMD_AllocWStringBuffer(WCHAR **ps, int64_t count)` |
| FUN_140063620 | PECMD_AllocStrSlot | restored_bodies.c `uint16_t *PECMD_AllocStrSlot(uint16_t **out)` |
| FUN_14005b104 | PECMD_FreeStrBuf | core_string.c `void PECMD_FreeStrBuf(WCHAR **ps)` |
| FUN_140018b70 | PECMD_StripTrailingSpaces | src/misc/core_b1_remaining.c `LPCWSTR PECMD_StripTrailingSpaces(LPCWSTR s)`（去尾空白） |
| FUN_1400661e4 | PECMD_MatchAndAdvance | src/commands/core_b3d.c `bool PECMD_MatchAndAdvance(char *key, int64_t *pp, int len)` — 注意第二个参数是 `int64_t *pp`（指向指针的指针），dc 传 `(longlong *)&local_res10` |
| FUN_1400675b8 | PECMD_SplitTokenTrimWs | src/misc/core_remaining_helpers.c:64 `void PECMD_SplitTokenTrimWs(WCHAR **src, WCHAR **dst, int16_t delim)`（按分隔符切分，dst 收左半） |
| FUN_140074838 | PECMD_ParseUIntValue | src/commands/core_b2f.c `int PECMD_ParseUIntValue(WCHAR **pp, int *out)` |
| FUN_1400745c8 | PECMD_EvalParenStripped | restored_bodies.c `ulonglong PECMD_EvalParenStripped(longlong *param_1, ulonglong *param_2)` |
| FUN_140067b54 | PECMD_ParseAndSkipSpace_7b54 | src/commands/core_b3b.c `uint64_t PECMD_ParseAndSkipSpace_7b54(int64_t *pp, double *out)` |
| FUN_1400a9a84 | PECMD_EvalExprSkipOneChar | src/misc/core_remaining_helpers.c `uint64_t PECMD_EvalExprSkipOneChar(int64_t *pp, uint64_t *out)` |
| FUN_1400e69ac | PECMD_FormatDoubleToStr | src/ui/core_b8m.c `WCHAR **PECMD_FormatDoubleToStr(WCHAR **out, double value, LPCWSTR fmt, uint32_t prec, char trim, char mode)` |
| FUN_1400e6d38 | PECMD_SprintfRetEnd | src/kernel/core_strbld.c `WCHAR *PECMD_SprintfRetEnd(WCHAR *dst, uint64_t v, LPCWSTR fmt)` |
| FUN_14001d78c | PECMD_MemMoveForward | restored_bodies.c `uint8_t *PECMD_MemMoveForward(uint8_t *a, uint8_t *b, int n)`（前向 memmove） |
| FUN_1400bb718 | PECMD_MessShowMsgBox | src/commands/core_b3_remaining.c `uint64_t PECMD_MessShowMsgBox(int64_t *param_1, WCHAR *param_2, int64_t *param_3)` |
| FUN_140018978 | PECMD_VarLookup | src/runtime/core_var2.c `void *PECMD_VarLookup(void *script, LPCWSTR name, void *scope, int namelen, void **found)` |
| FUN_14005eefc | PECMD_ExpandPercentD | restored_bodies.c `uint16_t *PECMD_ExpandPercentD(uint16_t *param_1, WCHAR *param_2)` |
| FUN_14005efac | PECMD_ExpandStringMarkers | core_b3_remaining.c `LPWSTR PECMD_ExpandStringMarkers(WCHAR *param_1, WCHAR *param_2)` |
| FUN_140070398 | PECMD_ReplaceStringSlot | src/commands/core_b3e.c `int64_t *PECMD_ReplaceStringSlot(int64_t *ps, uint64_t *src)` |
| FUN_14007034c | FUN_14007034C | 既有（core_b8m.c 等调用），`void FUN_14007034C(void *ps, LPCWSTR src)`，extern 声明即可 |
| FUN_140070310 | PECMD_CopyStrToSlot | src/commands/core_b3a.c `uint64_t *PECMD_CopyStrToSlot(uint64_t *dst, uint64_t *src)` |
| FUN_140060b24 | PECMD_IsIdentifierChar | src/commands/core_b3d.c:346 `uint64_t PECMD_IsIdentifierChar(uint16_t ch)` |
| FUN_14005bc74 | PECMD_NegateDouble | src/commands/core_b3b.c:120 `void PECMD_NegateDouble(int64_t obj, double *value)` |
| FUN_14007bda8 | FUN_14007BDA8 | 既有（core_b2d.c 等调用）`extern` 声明，签名 `int64_t FUN_14007BDA8(void *script, WCHAR *text, WCHAR **out, int c, uint8_t d)` 按调用点对 |
| FUN_14007a224 | FUN_14007A224 | 既有（core_b2e.c 等调用）同上签名族 |
| FUN_140065140 | PECMD_LexMathExpression | core_b3_remaining.c:12561 `uint64_t PECMD_LexMathExpression(uint16_t *param_1)` |
| FUN_14007efa4 | PECMD_ParsePathRecord2 | src/commands/core_b3h.c:601 `uint64_t PECMD_ParsePathRecord2(LPWSTR path, uint8_t *flags)` |
| FUN_14007f018 | PECMD_ParsePathRecord | src/commands/core_b3f.c:522 `uint64_t PECMD_ParsePathRecord(LPWSTR path, uint8_t *flags)` |
| FUN_1400703e4 | PECMD_StrBldCopyWide | 既有，`void *PECMD_StrBldCopyWide(void *a, const WCHAR *b)`，extern |
| FUN_140067d20 | PECMD_ParseNumTryWriteback | src/misc/core_remaining_helpers.c `int PECMD_ParseNumTryWriteback(long long *pp, int *out)` |
| FUN_140067cf4 | PECMD_ParseNumSkipWs | src/misc/core_remaining_helpers.c:118 `bool PECMD_ParseNumSkipWs(WCHAR **pp, uint64_t *out)` |
| thunk_FUN_1400f429c | thunk_FUN_1400f429c | 既有 `uint64_t thunk_FUN_1400f429c(void *a, short)`，extern |
| FUN_1400c11c0 | PECMD_ParseHexOrDecBool | `extern bool PECMD_ParseHexOrDecBool(WCHAR **pp, int *out)`（core_remaining_helpers.c 已extern） |

全局常量（dc → msvc）：
- DAT_14011c638 → `g_szEmpty`（src/kernel/core_globals.c:1114 `WCHAR g_szEmpty[2]`）
- DAT_14013e190 → `g_csInit`（core_globals.c:104 `CRITICAL_SECTION g_csInit`）
- DAT_140125238 → `g_fontMinus0`（-0.0，core_globals.c，extern double）
- DAT_140129848 → 本文件内局部静态 `WCHAR dt_129848[] = L"+txt+icon1 0";`
  （实测原版 .rdata：+0 起 "+txt\0"，+8 起 "+icon1 0"；`local_88 = dt_129848 + (4 - local_17c)`）
- DAT_140129810 → 局部静态 `WCHAR dt_129810[] = L"+cb";`（实测 6 字节 "+cb\0"）

## 4. 关键语义注记（主代理已用原版 PECMD.EXE 活体对拍 + dc 逐段甄别定案，照此实现）

1. **返回值契约（exit 链）**：PSB 的 CALC 分支 `local_180 = PECMD_CalcExpression(script, txt, 0)`
   把返回值写入槽（非零才写，U-1 机制）；退出码 = 槽值。活体实测原版：
   `CALC R=1+2*3`→0、`CALC R=1/0`→16、`CALC R=9/2`→0、`CALC R=0x10+1`→0、
   `CALC R=(1+2)*(3+4)`→0。
2. **pWVar5/pWVar6 载体**：每轮语句循环启动处 `pWVar5=(LPWSTR)0`；`LAB_1400c0876`:
   当 `local_168`（语句文本游标）refcount==0 且 `ptVar30`（扩展后变量名）非空 →
   `FUN_1400629b8(param_1, ptVar30, puVar14)`（赋变量）后 **`pWVar5 = pWVar6`**。
   pWVar6 在求值分派处 = `(char)local_res20`（求值器标志字节：成功 0 / 除0 0x10 / 其他错误位）。
   **除 0 时 pWVar6=0x10 → 返回 16（实测原版 exit=16 的铁证路径）**；成功 pWVar6=0 → 返回 0。
3. **语句循环结构**：`do { ... } while(true)` 以 LAB_1400c0948 为迭代头（local_d8 语句计数
   递减；`lVar31 < 1` 则 goto LAB_1400c0a08 → 收尾返回 pWVar5）；函数唯一收口
   `LAB_1400c0a79`（free local_90/local_80/local_138/local_f0 → return pWVar5）。
4. **求值分派**：`uVar8 = PECMD_LexMathExpression(ptVar17)`；`uVar33 = (ushort)uVar10`；
   - `<0`（子代理注意：LexMath 正常收尾返 0，负数路径原样保留）：pWVar6=0xffffffff80070057；
     uVar33!=0 → LAB_1400c0241（pWVar5=local_b0 → LAB_1400c0251）；否则 LAB_1400c02bd。
   - 否则 `uVar33 < 0x23` → **数值路径**：`uVar8 = PECMD_ParsePathRecord2(ptVar17, &res20)`；
     `uVar37=(u32)uVar8; uVar38=(u32)(uVar8>>32)`；`(char)res20!=0` → LAB_1400c02bd，否则
     LAB_1400c02c1。**注意：此路径的 uVar37/uVar38 是求值返回值本身（含除0后的 0），
     不是 local_150**（与 uVar8<0 路径的 uVar39/uVar40 区分开！）。
   - `uVar33 >= 0x23` → **表达式路径**：`pWVar5 = (LPWSTR)PECMD_ParsePathRecord(...)`；
     `pWVar6 = (LPWSTR)(int64_t)(char)res20`；标志非 0 → LAB_1400c0241；否则 LAB_1400c0251。
5. **格式化**：
   - 数值路径/错误值：`FUN_1400e69ac(&local_a0, dVar36, 0, local_160[0], cVar15, cVar19)`
     （= PECMD_FormatDoubleToStr，double %g 类格式化——实测原版 `9/2` → `%R%`="4.5"）；
     然后 `FUN_140070398(&local_188, plVar9)`（= PECMD_ReplaceStringSlot）。
   - 表达式路径：`FUN_1400e6d38(local_188, local_118, "%I64d"/"%I64u")`（uVar33>0x23 用 %I64u）。
   - `1 < local_170`（-base= 且 base>1）→ 逐位数字转换路径（pWVar5 累加），原样移植。
   - base 2/8/16 输出路径（pWVar29 = L"0b"/L"0o"/L"0x"）原样移植。
6. **-err=N 语义**：仅影响错误文本（local_b0 解析值），**不影响返回值**（实测原版
   `-err=17 R=1/0` → exit 仍 16）。
7. **?N 前缀路径**（变量名以 '?' 开头，dc LAB_1400c0382 内）：原样移植（用
   PECMD_ParseNumTryWriteback/PECMD_ParseNumSkipWs/PECMD_VarLookup/PECMD_MemMoveForward/
   PECMD_StrBldCopyWide 等）。
8. **+txt / -cb**：local_17c=4 或 local_140=3；前缀串按第 3 节常量；`local_88` 表偏移按
   `(4 - local_17c)`。
9. **临界区**：EnterCriticalSection/LeaveCriticalSection(&g_csInit) 围住求值→赋值整段
   （照 dc 位置）。
10. **收尾**：所有临时串槽（local_138/80/90/f0/150/148/158/108/188/178/120/a8/c8/a0 等）
    按 dc 每处 PECMD_FreeStrBuf 时机释放；`local_80[8]` 只初始化/释放第 0 槽（照 dc）。

## 5. 编码约束

- 文件头注明 `/* ========== PECMD_CalcExpression @0x1400bf358 忠实移植 (dc:118318-119158) ========== */`
- 全部注释中文，ASCII 代码；不得引入任何虚构分支；dc 没有的分支一个都不要加。
- 不用 `#include <stdio.h>`；需要的外部符号在本文件顶部 extern 声明（少数已有 xproto.h
  集中原型，以定义处为准自行 extern 亦可，参照同目录 core_b3x.c 的做法）。
- WSTR() 宏可用（pecmd_defs.h / stubs_common.h 提供，看同目录文件怎么引头）。
- 循环/标号结构必须与 dc 一一对应；可复用模板函数 PECMD_ParseExpression 的载体手法。

## 6. 完成自查

- [ ] 函数内语句数与 dc 对齐（允许 goto 重排但分支集相同）
- [ ] 所有 helper 均为映射表符号，无新增 stub
- [ ] 不修改除本新文件以外的任何文件
- [ ] 用 `gcc -fsyntax-only` 或至少肉眼复核括号/标号配对（仓库不依赖 gcc，语法门由主代理跑）
- 交付后回复：文件路径 + 行数 + 任何需要主代理注意的偏差（如 dc 中某个分支翻译时有不明处）。