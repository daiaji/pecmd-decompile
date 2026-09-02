# R26-e FORX 真体化 — 已入账 (2026-09-02, 会话恢复完成)

> **本文原为在途存档, 现已完结入账** (HANDOVER R26-e, 64/64 零回归)。
> 与本文定案的差异均由 capstone 核验修正, 见 HANDOVER R26-e 登记与本文件尾「完结补记」;
> 作业流程已按原「待办」节执行: capstone 核验 → ParseSizeNumber 真体化 →
> 主体 part B 草拟 → 拼接替换 → 双门 → 64/64 回归 → HANDOVER 入账。


> 配套草稿件: `build/msvc/_r26e_forx_a.txt`(选项循环部分草稿, 已定稿可直接用)。
> 上位账本: `HANDOVER_PROGRESS.md`(R26-c/d 已提交; R26-e 未入账 — 本文即其前置)。

## 1. 作业目标与状态

- **目标**: `PECMD_ForCommand`(src/commands/core_b3r_i28b.c:424-619, 现"简化主体"桩)
  全文直移 dc `FUN_1400acd90`(reference/decompiled.c:107744-108445, size=6498, 712 行)。
- **进度**: dc 全文已通读并完成语义分析; 选项循环部分已草拟(`_r26e_forx_a.txt`, 含头部注释/extern
  需求/局部变量映射); **主体(LAB_1400ad61f 起 = dc:108081-108445)未草拟**; 尚未应用到源码、
  未构建、未回归。
- 替换范围: i28b.c 中自 `/* ================================================================\n * @0x1400acd90  FOR`
  起至 `return (ulonglong)(uint32_t)iVar8;\n}`(行 418-619, 替换前需按当前行号重定位)。
- 调用面(已核): restored_bodies.c:7133 ↔ dc:45123, 5 参
  `(script, cmd+8, param_3, param_4, param_5)`, 返回 longlong 承接。

## 2. 已定案的关键语义(转写依据)

1. **整体结构**: 选项循环(dc:107901-108062, do-while(true)) → 唯一 break 出口 =
   `*qu 且 opts 低32==1`(dc:107916-107919)→ 落入**引号扫描器**(dc:108063-108086)→
   `StrCopyW(&local_3c0, pWVar21, (cur-pWVar21)>>1)` + `ExpandVarDispatch(param_1, local_3c0,
   &local_3e0, 0, 1)` → ad61f 主体。其余路径经 ad429 内 `SplitTokenAssignVar` 后
   `goto ad61f`。
2. **SplitTokenAssignVar**(FUN_14007f6e4, dc:81271, 127B)真实语义: 参数一 = **3 槽 cell**
   `[0]`=ExpandVarDispatch 展开出参、`[1]`=script(展开的 ctx)、`[2]`=展开结果副本;
   返回 `&cell[2]`。dc 的 local_3d0(local_3e0 别名)与 local_3e8(script)未初始化读 =
   Ghidra 寄存器合并伪影 → 转写统一用 `int64_t fcell[3]`,`fcell[1]=script` 显式初始化,
   ad61f 的切分源取 `&fcell[0]`。
3. **选项旗寄存器合并**: dc uVar15/uVar22/uVar23/uVar25/local_4a8 = 同一寄存器(转写为
   `sFlag`);uVar34/uVar33 = `opts`。`uVar20 = sFlagLow | oFlag`(dc:107902)。
4. **选项语义**: NL→opts=(opts&~1)|6;NL:/delims:→opts=0xE+ParseStringToken+c490=*cur;
   `-`/`*`→opts&~1|1;v/**→opts|5;L→lFlag=opts&~1|0x11;*bf→sFlag=1(dc uVar23=uVar32=1);
   *ab/*qu/*qu~/*off→各旗; /s→sFlag=0x10(循环顶生效); /s:→+ParseUIntValue(&stepPair);
   /O:N→oFlag=0x20;/O:-N→0x60;*cur→curFlag=1;/size*:→n350|=0x10 后+1wchar;/size:→直入;
   /size 族解析 3 数(n370/n368/n358, FUN_14006a740); 未知 token 且非 `*`→ad429,
   `*`→ad02e 跳 token 续循环。
5. **NL 旗真值** = `opts bit2`(dc bVar4 被 "v" 测试复用, 与 dc:108087 `(uVar34&4)!=0` 判据
   一致)— **登记偏差**。体为 `L"*"` 时 dc bVar4 为未初始化残留 → 取真值 true(延迟体语义)。
6. **缺第 4 分隔符 → 返回 1**(dc uVar32 init=1; LAB_1400adddf)。
7. **uVar25 双角色**: 选项期 = sFlag 低 32 位;主体期 = delim(dc:108124 `uVar25=(uint)cVar35`)。
8. **tgtSlot(local_398)** = ProcessControlCommand 的出参槽(逐项写入当前项);`!bVar4` 时
   预填 = 变量名(s470);体为 `*` 时终值经 epilogue `SetVariable(s470, tgtSlot)`。
9. **三引擎分派**(dc:108207): `opts&2`(NL:/delims:)→ 行迭代(delim=c490, dc:108497+);
   否则 `opts&1`(/L)→ 数值迭代(dc:108455+, LAB_1400ade55);默认 → **文件通配**(dc:108210-108330,
   ProcessControlCommand 逐项)。默认 FORX = 文件通配(非空格分词);glob 分支恒
   `goto ae5ea`,ade55 内 `local_45c==0` 空格分词分支为死代码(Ghidra 共享模板伪影)。
10. **PCC 调用签名**(i28b:85-88, 已核): `uint64 PECMD_ProcessControlCommand(int64_t*,
    int64_t*, LPCWSTR, uint64_t, int64_t, uint32_t, int16_t*, int64_t*, int, uint64_t)`
    — 实参 `(script, &tgtSlot, item, res18, (longlong)blk428, sFlagLow|uVar20|mode458,
    s468, &stepNum, iVar36, param_5)`。
11. **逐迭代体执行**(dc:108521-108530): `n338==0`→`ProcessScriptBlock(script, s468, res18,
    NULL, param_5)`(体非空时);否则 `blk3e8 = blk428+1; TokenizeExpression(script, res18,
    &blk3e8, 0, g_szEmpty)`。每轮后 `script+0x19 & 0x100` 清位。
12. **script 旗访问**: `*(ushort *)(param_1 + 0x19)`(0x8b/0xef/0x100 位);break 旗 =
    `g_flagA24F`(DAT_14013a24f, pecmd_defs.h)。
13. **NL 模式变量源**(dc:108180-108196): `opts&4` 时 EnterCS(g_csInit=DAT_14013e190)→
    `VarLookup(script, s478, NULL, -1, NULL)` → `StrCopyW(&s418, node+8 值, node+0x18>>1 &
    0x1fffffffffffffff)` → 游标 = s418。

## 3. L 模式未决风险(主体草拟前必须解决)

- `WideStrToDouble`(FUN_140064694, dc 签名 `double(longlong* cursorCell, longlong* outEnd,
  byte* prec)`, 862B)的 **outEnd 语义**存疑: dc:108478 第二次解析写 `&local_3c8`(覆盖
  step=1 初值),而 `local_400 = local_3c8 + local_3f8`(指针+指针)语义不明。
- 建议: 转写前对原版 EXE FUN_1400acd90 的 dc:108462-108536 段做 capstone 反汇编核验,
  确定 local_3f8(当前值)/local_3c8(步长)/local_400(终值)的真实寄存器角色
  (工具链已验证可用: 手工 PE 解析 + capstone 5.0.7, 见 MOUN 轮 `_r26d` 脚本)。
- 其余主体段(glob 逐盘枚举 / 行迭代 / PCC 调用 / epilogue)语义已明确, 可直接转写。

## 4. 依赖核验(全部有 msvc 真体, 除注明)

ProcessControlCommand(i28b:85) / WideStrToDouble(i28b:69) / IsRemovableDrive(b2f:1937) /
ExpandPathAlloc2(b1_remaining:436) / AdvanceTokenPointer(b2c:147) / ProcessScriptBlock(b2c:145) /
TokenizeExpression(b2c:143) / FormatU64RetEnd(core_strbld.c:142) / FreeArray_ddf8(b3e:564) /
InitPtrTable / VectorSlotPtr / VectorAppendGen / CopyStrToSlot(b7c 内有 extern) /
StrCopyW(core_sys.c) / UnquoteString / AssignString / StrBldCopyWide(FUN_1400703e4) /
ParseStringToken / MatchPrefixAdvance / ParseUIntValue / g_flagA24F / g_csInit / g_szEmpty;
**桩依赖**: ExpandDrivePathAlloc(unimplemented_stubs.c:90)。
**连带真体化待做**: `PECMD_ParseSizeNumber` — restored_bodies.c:12 为清零桩
`(void)pp; *out = 0;`,仅 i28b:588/593 两调用点(FORX /size: 族)→ dc:66228(179B)直移,
签名改 `uint64_t(int64_t *pp, int64_t *out)`(i28b:73 extern 与 stubs_common.h:2714 同步),
其依赖 FUN_140067b78 已有真体(core_b7c.c:4392)。

## 5. 待办(恢复时按序)

1. (可选)capstone 核验 L 模式 local_3f8/3c8/400 真实角色(原版 EXE FUN_1400acd90)。
2. ParseSizeNumber 真体化(清零桩拆除 → dc:66228 直移; 两处声明同步)。
3. 草拟主体 part B(LAB_1400ad61f 起, dc:108081-108445):@/$/\! 前缀扫描 → 路径展开
   (ExpandDrivePathAlloc/AssignString)→ var/body/step 三段解析 → 三引擎 → epilogue
   (LAB_1400ae5ea: `script+0x19 &= 0x8b`; bVar4→SetVariable / flag498→AppendLongDecimal;
   7 槽释放; 返回 iVar8 / retCell)。
4. 拼接 `_r26e_forx_a.txt` + part B → 替换 i28b.c:418-619(先按当前行号重定位)→
   语法门 → 全量构建 → `run_corpus.sh` 64/64。
5. HANDOVER R26-e 登记(含本文所列偏差)+ git 提交 + compat `docs/reverse-map.md` FORX 行同步。

## 6. 会话总体队列状态(截至本存档)

- **已完成**(全部双绿门 + 64/64 零回归): D-01 注入桩遮蔽 / PUTF 双定义定案(D-23)/
  SITE 真体化(D-24 连带 ParseCommaNumbers)/ PAGE 直移缺陷归正(D-25)/ **MOUN 真体化**
  (WIM/VHD/UDM/SVR 四子引擎, 829 行)。
- **提交**: pecmd-decompile `2c5eb57`(R26-c)→ `ec3babe`(D-25)→ `f00039b`(R26-d MOUN);
  pecmd_compat `d1b0923`(reverse-map 入库+R26-c 同步)→ `0b3d4bb`(MOUN 行同步)。
- **未动**: GETF(dc:128565-129671, 1106 行)/ PART(dc:89789, 29895B, 最大件)/
  SHOW 10 处 TODO(verify) 收口 / 语料扩展(队列项 9, GETF/MOUN 均建议随件扩语料)。
- 部署现场: C:\pectest, 当前 md5=ddcd19b5(hash f00039b, 08:39)。
- 过程件: build/msvc/_r26c_*.py、_r26d_*(MOUN, 含 body 分段)、_r26e_forx_a.txt(本件草稿)。

---

## 完结补记 (2026-09-02, 恢复会话)

- **待办 1 (capstone)**: 已执行并对全文 (含 L 模式) 完成核验。**L 模式角色定案**:
  local_3f8=当前值 (整数迭代, AppendLongDecimal 值低 32 位, capstone `mov edx,ebx` 实证) /
  local_3c8=步长 (初值 1, 第二次解析覆盖) / local_400=终值 (start+step 兜底后被第三次解析
  覆盖 — WideStrToDouble 恒写 out, 兜底语义=第三次解析结果)。`local_400 = local_3c8+local_3f8`
  = 64 位整数加 (lea rcx,[rbx+rax] 实证), 非指针语义。
- **待办 2 (ParseSizeNumber)**: 已真体化 (dc:66228 直移), 连带 ParseSizeAndSkipWs 包装器按
  dc:66273 归正 (旧体 *param_2 恒未写)。
- **待办 3 (part B)**: 已草拟并落码 (dc:108087-108548 全文)。
- **待办 4**: 语法门 0 error → clean 全量构建 → **64/64 零回归**。注意: build/msvc 陈旧
  .obj 曾毒化构建 (SyncWorkingDirectory 产物与源码字节不符 → 全量 0xC0000005), 回归前
  **必须 rm build/msvc/*.obj**。
- **待办 5**: HANDOVER R26-e 已入账; 本文标记完结。
- **存档修正登记** (capstone 推翻原定案):
  1. §2.3 "选项旗寄存器合并 (uVar15/uVar22/uVar23/uVar25/local_4a8 同寄存器)" 作废 —
     local_4a8/uVar15 = /s 槽 (0x10), uVar22/uVar23 = *bf 寄存器 (1), 二者不同寄存器;
     uVar20 = sFlag|oFlag; local_3b8 = sFlag ? bfReg : 0。
  2. §2.9 "ade55 内 local_45c==0 空格分词分支为死代码" 作废 — capstone 0x1400adc84
     `je 0x1400ade42` 实证 */- 无 L 时落入 (活路径), 已全文转写。
  3. §2.2 "dc local_3e8(script) 未初始化读" 作废 — dc:107871 `local_3d8 = param_1` 为
     显式 cell[1] 赋值。
- **连带真体化** (WIP §4 标注桩): ExpandDrivePathAlloc (91B) + ExpandDrivePath (246B)
  由零参/空桩直移真体; stubs_common.h 2827/2865/2915 三处零参旧声明同步修正。
- **存量缺陷登记**: `WRIT` 命令真机崩溃 (原版 msvc 双侧一致), 非本轮引入, 归后续轮次。
