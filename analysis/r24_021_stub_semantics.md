# R24: 021 SUB 挂死根因定案 — FUN_14001b23c 直移副本扫描环语义分叉

> 2026-08-27 R24 轮 · V-Gate 门3 活体 attach · 构建身份: md5 62e5ddd3 = cd13046
> （DEPLOYED_BUILD.txt 00:07:48）；工作树 HEAD f8e7cbc（R23f 改名，无代码差异）
> 复现场景: `python harness/runners/run_case.py 021_sub_call --exe msvc --timeout 240`
> （harness 同构: cwd=C:\pectest, LOAD run_all.pecmd），12s 存活确认挂死 (PID 548) → attach

## 1. 取证摘要（一次性闭环）

| 项 | 证据 | 结论 |
|---|---|---|
| 死点 RIP | `pecmd_msvc+0x1e76fb` = 反汇编 `mov rcx,[rsp+60h]`（0x88 比较加载） | 位于 **unimplemented_stubs.obj FUN_14001b23c** @ RVA 0x1e7640 +0xbb 的扫描环 |
| 栈链 | `~0s` k 20: ExtractTableSegment←ParseAndExecuteLine(+0xa2a=0x5a33a)←ExecuteCommand←DispatchCommand←ProcessScriptBlock←DispatchExpressionBlock←RunScriptText←srx_ExecuteScriptFile←RunCommand←…←RunStartupScript←RunBootScriptInFiber←MainW←main | 与 R23 同构（裸 LOAD→CALL F 路径） |
| 文本流 | `du 24dd38a8338` = "ENVI R=subok._END.CALL F.ENVI T_CASE=021_sub_call.EXEC =C:\WINDOWS\System32\cmd."（`.`=LF） | **文本完全正常**，游标=+0x10（`_SUB F\n` 已消费） |
| 对象分隔符 | `dq obj+0x80`: +0x88=0x0000, +0x8a=0x000d(CR), +0x8c=0x00ef, +0x8e=0x0000, +0x90=0x000a(LF), +0x92=0x0020, +0x94=0x0009 | **分隔符集全初始化**（R20C srx 种子化生效） |
| 死因 | 桩体扫描环: ch 非 0x90/0x8a 且 *cur 非 0x88 时**不前进**（C: `if(ch==0x8a) cur+=2;` 否则无操作）→ 文本首字符 'E' 无匹配 → 原地自旋 | **语义分叉 vs dc** |

## 2. 语义对照（唯一真值）

dc:15781-15795（= 真体 core_b1_remaining.c:7969-7977）:
```c
while( true ) {
  sVar1 = *(short *)*param_4;
  if (sVar1 == *(short *)(param_1 + 0x90)) break;              // LF 终止
  if ((sVar1 == *(short *)(param_1 + 0x8a)) ||                 // CR 终止
     (psVar4 = (short *)*param_4 + 1, *param_4 = (longlong)psVar4,  // ★无条件 +1
      *psVar4 == *(short *)(param_1 + 0x88))) break;           // 下一字符==0 → 终止
}
```

unimplemented_stubs.c 旧副本（R14 直移，已删）:
```c
for (;;) {
  ch = *(short *)*(int64_t *)d;
  if (ch == *(short *)(a + 0x90)) break;
  if (ch == *(short *)(a + 0x8a)) { *(int64_t *)d += 2; }      // ✗ 写成 +2 跳过
  if (*(short *)*(int64_t *)d == *(short *)(a + 0x88)) break;  // ✗ 检查当前位置
}
```

- dc 语义: 每轮**无条件 +1 前进**；仅 ch==0x8a 时停在 0x8a；0x88 检查在**下一字符**。
- 旧副本语义: 无 0x90/0x8a/0x88 命中时**指针不动**；0x8a 命中 +2；0x88 检查当前位置。
- 021 文本首字符 'E'(0x45) 三值全不命中 → 旧副本死循环；dc 语义逐字符推进至 LF(0x90) 终止。

## 3. 与 R23 记录的偏差（勘误）

- R23 deadpoint_shift 假设"文本流未被种子化/来自再用缓冲"→ 本轮活体实测**文本流与分隔符均正常**，
  该假设不成立；实际病根是桩副本扫描环本身（R14 dump 16664 时代直移失真，属"恢复期语义漂移"而非运行期状态）。
- R23 记录 RIP "K=0x1e376da / FUN_14001b23c+0x9a" 与本次 +0xbb 的差异未影响结论：
  链接目标同为 unimplemented_stubs.obj 副本（真体 @0x140122950 / RVA 0x122950 未被该路径调用）。
- 符号双同名事实：map 中 PECMD_ExtractTableSegment@0x122950（core_b1_remaining.obj 真体）与
  FUN_14001b23c@0x1e7640（unimplemented_stubs.obj 副本）并存；4 个调用点中仅 core_b3m.c:624
  绑定真体，core_b3_remaining.c 三处 (4904/19938/20422) 绑定副本。

## 4. 落码（提交见 repo）

`unimplemented_stubs.c`: 删除 94 行错误副本（含 local_RTrim/local_StripInlineComments 两 helper 及
PECMD_SkipLeadingControls/PECMD_WideStrLen/g_charTableF extern，均仅该副本引用），替换为单行转发:

```c
LPCWSTR FUN_14001b23c(int64_t a, void *b, const uint16_t *c, void *d, char e)
{
    return PECMD_ExtractTableSegment(a, (uint64_t *)b, (uint16_t *)c, (int64_t *)d, e);
}
```

（保留 FUN_14001b23c 符号 = 代码库既有转发惯例，如 FUN_14005bc28→PECMD_IsAlnumLowerDigit；
PECMD_AllocString extern 保留供 FUN_140063720 转发使用。）

## 5. 预期影响面

- 021（SUB 族入口）确定性修复；037/038 同为 SUB 机路径，大概率同源同愈（待全量回归验证）。
- 其余 3 处调用点（ParseAndExecuteLine 词法/参数解析等）同步切换到 dc 语义，可能改善
  002/004 等展开/解析路径的次生死态（待回归观测），回归红线=零新FAIL。

## 6. 环境备忘

- 取证过程无探针、无 PDB 符号加载（全 RVA+map 对照），无构建身份漂移。
- run_case 超时行为复现挂死（exit=124），进程随超时自行终结，无残留。