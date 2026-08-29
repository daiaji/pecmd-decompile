# R25i · L1 语义化 C4 批次 1 — core_b7c.c 两函数（552 站点）

- 对象: `src/ui/core_b7c.c` 两函数（A 区精修文件，全文件 2411 处 local_xx）
  1. `PECMD_TablCreateControl @0x1400caf78`（6073-6392 行, dc `reference/decompiled.c` @125342 size=2215）
  2. `PECMD_CtlLoadPictureRgn @0x1400d5b48`（9153-9846 行, dc @131228 size=5185）
- 性质: 纯改名（Ghidra 占位名 → snake_case 语义名），零语句/类型/顺序改动；注释文本、
  函数头注释、dc 引注、`LAB_xxx` 标号、既有语义名（rc/bm/obj/bmpslot/flagbyte/stack_size/
  name_arg/resinfo/dst_w/scaled_w/colctx 等）一律不动
- 方法: python `\b旧名\b` 全字替换，块注释感知的行内 code/comment 切分（`/*...*/` 跨行块、
  `//`、字符串/字符字面量均跳过），严格限定两函数行界
- 与语料/控件路径关系: `TablCreateControl` = 'TABL' 命令项解析 → 委托
  `PECMD_AddDialogControl(FUN_1400cadec)` 建控件（ITEM/LIST 控件链入口, dc:125497）;
  `CtlLoadPictureRgn` = 控件图片装载+色键透明区域（控件位图/区域路径, 被 core_b7a/b7b 控件方调用）
- 密度普查法: 按函数头 `/* ========== NAME @0xADDR` 切函数统计（配方/派单口径）

## 选靶清单

| 函数 | 行界 | 行数 | local_xx 站点(`\blocal_[0-9a-f]+\b`) | Ghidra 全名站点 | 备注 |
|---|---|---|---|---|---|
| PECMD_TablCreateControl @0x1400caf78 | 6073-6392 | 320 | 126 | 357 | dc 逐语句 1:1 |
| PECMD_CtlLoadPictureRgn @0x1400d5b48 | 9153-9846 | 694 | 203 | 344 | dc 对齐, 局部重构点见观察项 |
| 合计 | — | 1014 | 329 (≤400 ✓) | 701 | — |

未选 `PECMD_TablCreateListCtrl @0x1400c9b9c`（5278-6033, 756 体行, local 156/全名 ~430）:
体行数 756 超出选靶标准②(300-700 行)，留批次 2；其 dc(@124641) 对齐度已核验为 1:1。

实际替换（Ghidra 全名口径, 配方第 2 步对象含 uVarN/WVarN 等）: 552 站点/63 名。
`local_xx` 严格口径替换: ~329 站点。

## 映射表 1 — PECMD_TablCreateControl（37 名 / 295 站点）

| 旧名 | 新名 | 证据（dc 行号/行为） | 站点 |
|---|---|---|---|
| local_res10 | parse_cur | 全程解析游标: dc:125398=param_2 跳命令词后起点, 125418=规格串游标, 125430/125468 选项/字段环游标 | 53 |
| local_res18 | dlg_saved | dc:125399 `local_res18 = param_3` 暂存, 125460/125563 循环内回灌 param_3 | 4 |
| local_res20 | coord_h | dc:125423 `[0]=0` 初值; 125469 作 ParseLtwhParams 第 4 出参(H); 125498 传 AddDialogControl | 4 |
| local_d8 | coord_l | dc:125420 `[0]=0`; 125469 第 1 出参(L); 125497 `local_d8[0]` 作 X | 4 |
| local_c8 | coord_t | dc:125421; 125469 第 2 出参(T) | 4 |
| local_c4 | coord_w | dc:125422; 125469 第 3 出参(W) | 4 |
| local_e8 | opt_sl_char | dc:125424 `='\0'`; 125527-125531 `-sl:` 取 pWVar16[4] | 4 |
| local_e6 | opt_sc_char | dc:125425 `='\t'`; 125535-125539 `-sc:` 取 pWVar16[4] | 4 |
| local_e4 | opt_sl_star | dc:125533 `-sl:*` 星标置 1 | 4 |
| local_e2 | opt_sc_star | dc:125541 `-sc:*` 星标置 1 | 4 |
| local_d0 | item_text | dc:125405 StrAssign 空串; 125462 SplitTokenTrimWs 项文本; 125464 展开; 125497 传 AddDialogControl | 6 |
| local_c0 | ctl_text | dc:125473 坐标后文本字段槽; 125498 传 AddDialogControl | 5 |
| local_b8 | def_text | dc:125483 ExtractTokenByDelim 默认项槽 | 5 |
| local_b0 | spec_text | dc:125412 规格首字段(逗号前)串槽; 125428 `*local_b0=='*'` | 7 |
| local_a8 | opt_cksz | dc:125551 `-cksz:` 值指针; 引号跳转 | 7 |
| local_a0 | opt_font | dc:125403 初值空串; 125517-125518 `-font:` 名 | 5 |
| local_98 | opt_hfont | dc:125417 初值空串; 125519-125526 `-hfont:` 名 | 5 |
| local_78 | opt_color | dc:125416 初值空串; 125521-125522 `-color:` 名 | 4 |
| local_90 | expand_slot | dc:125401 AllocStrSlot 变量展开上下文槽(xctx[0]); 125413/125468 作 SplitTokenAssignVar 槽 | 8 |
| local_88 | xctx_script | dc:125404 `local_88 = param_1`(xctx[1]=script) | 3 |
| local_80 | xctx_aux | dc:125402 `=0`(xctx[2]) | 3 |
| local_70 | spare_slot | dc:125411 `=0` → 125502 释放, 全程未用 | 3 |
| local_68 | spare_slot2 | dc:125409 AllocStrSlot → 125504 释放, 未用 | 3 |
| local_60 | rest_saved | dc:125415/125419 首字段后剩余行游标存档, 125460/125563 取回 | 4 |
| local_58 | paren_num | dc:125561 CalcEvalParenAtom 双精度出参(3 元素) | 2 |
| pWVar4 | spec_start | dc:125433 '*' 跳过后的规格串起点(尾空白裁剪基址) | 3 |
| pWVar5 | opt_zone_end | dc:125449 选项区终点(临时 NUL 截断位), 125459 `pWVar5<=pWVar16` 耗尽判定, 125462 恢复字符 | 4 |
| iVar6 | spec_len | dc:125434 `lstrlenW(local_res10)` 规格串长 | 3 |
| plVar7 | split_ctx | dc:125413/125468/125472/125482/125489 SplitTokenAssignVar 返回令牌槽游标, 供后续 Split/Parse 消费 | 11 |
| lVar9 | left_match | dc:125545-125548 `-left` 前缀匹配 → psVar13=0x80000000 | 3 |
| puVar12 | def_item | dc:125477-125490 默认项参数: `" ,"` 哨兵 1(125479)否则 =local_b8(125486), 125498 传 AddDialogControl | 6 |
| puVar14 | ret_no_ctl | dc:125410 `=1` 未建控件返回值; 125471 建控件前置 0; 125509/125644 返回 | 6 |
| pWVar16 | word_cur | 当前词游标: 选项环 dc:125512-125543 前缀匹配主变量, NUL 截断位(125445), 字段分支词头(125451) | 56 |
| pWVar17 | val_cur | 选项值游标: dc:125524 `-hfont:` 值, 125441/125447 引号跳转, 125455 回退扫描起点 | 11 |
| pWVar18 | rest_cur | dc:125415 剩余行游标, 字段分支坐标段起点 125466/125477 | 12 |
| WVar3 | look_char | 前瞻字符: dc:125452 词头判 `-`, 125553 `-cksz` 后首字符, 125559 `:` 环, 125464-125475 回退环当前字符 | 13 |
| psVar13 | opt_flagbits | 选项标志位累计(short* 位包): dc:125514 `-sps`\|0x100000000, 125491 解析值 OR, 125548 `-left`=0x80000000, 125499 传 AddDialogControl | 8 |

## 映射表 2 — PECMD_CtlLoadPictureRgn（27 名 / 257 站点）

| 旧名 | 新名 | 证据（dc 行号/行为） | 站点 |
|---|---|---|---|
| local_190 | name_cur | 图片名游标: dc:131329=in_stack_38 或 obj+0x260, 前缀逐段推进, 载入分派各处作路径实参(131501/131516/131529/131545) | 47 |
| local_180 | load_bmp | 载入位图结果: dc:131494 LoadBitmapW / 131501 FUN_14007c730 / 131545 LoadImageW / 131565 FUN_14006eaac, 131592 写输出槽 | 21 |
| WVar22 | scan_ch | 扫描字符: 131396 `'('` 常量, 131441 `'*'` 判定, 131387 `'>'` 暂存恢复, 131412-131421 hex 数字环 | 17 |
| local_138 | load_icon | 载入图标: dc:131529 FUN_14001f1d4 / 131567 =local_108, GDI+ 支路 131632 清 0 | 14 |
| bVar5 | star_mode | dc:131441 `bVar5 = WVar22=='*'` 星标模式, 贯穿两支路判定 | 13 |
| local_d0 | wic_obj | dc:131516-131521 FUN_14006e3a4 WIC 对象, 虚调用 0x18/0x10, 收尾释放 | 12 |
| local_148 | src_h | dc:131667 `=local_78`(GetObjectW bm.bmHeight) 源位图高; 131452 几何段清 0 为 Ghidra 槽复用噪声 | 11 |
| local_128v | src_w | dc:131668 `=local_7c`(bm.bmWidth) 源位图宽 | 8 |
| local_120 | outbmp_ptr | 输出位图槽指针: dc:131370 `=&local_a0`, 星标 131447 `=ppvVar1`(obj+600), 131592 写入 | 9 |
| local_168 | geo_w | dc:131474 `=(int)local_178` 几何第 1 值 w; GDI+ 131625 lVar25、重采样 131794 判定 | 10 |
| local_160v | geo_x | dc 几何第 3 值 x(v3): GDI+ 源 x 偏移 `local_160*local_128/sx`(131645) | 4 |
| local_198 | mode_flags | dc:131357 `=2`; 131439 `0x42`; 131444 `\|=0x10`; 131446 `\|=4`; 131660 `\|=0x20`; 131849 写回 obj+0x250 | 8 |
| local_118 | key_idx | dc:131402 `%索引%` / 131412 `FUN_1400649f4` hex; 131722 色键重排入 uVar23(keyrgb) | 8 |
| local_f0 | pic_rgn | dc:131365 清 0; 131715 CreateRectRgn; 131846 SetWindowRgn 应用并写 obj+0xf0 | 8 |
| local_a0 | tmpbmp_slot | dc:131368 清 0 备用位图槽(非星标输出), 收尾 131853 DeleteObject | 6 |
| local_140 | geo_y | dc 几何第 4 值 y(v4): GDI+ 源 y 偏移(131646), 色键扫描列起点(131710 `lVar25=(int)local_140`) | 7 |
| WVar2 | pct_char | dc:131400 `%` 后首字符: 判 `%%`(131401)与 hex 位域(131406-131407) | 7 |
| bVar4 | tried_once | dc:131330 `=false`; 131537/131569 首轮失败后置 true, 次轮直转 COM/收尾路(131509/131552) | 6 |
| local_158 | geo_sx | dc:131475 几何第 5 值 sx(v5): GDI+ x 缩放分母(131645/131647) | 6 |
| local_100 | ctl_w | dc:131367 `rc.right-rc.left` 控件宽; CreateRectRgn/dst_w 用 | 5 |
| local_f8 | ctl_h | dc:131369 `rc.bottom-rc.top` 控件高 | 5 |
| iVar6 | geo_h | dc:131608 `=local_184`(几何第 2 值 h); GDI+ 高向缩放 131646/131648。⚠还原件 9560 取 `stack_size`（见观察项①） | 5 |
| iVar8 | tgt_size | dc:131613 `=0x60` 缺省; 131616 `=in_stack_30`; local_150/尺寸乘算 | 5 |
| cVar26 | hex_count | dc:131420 hex 位计数, `<'\b'` 即 ≤8 位 | 4 |
| uVar19 | gdip_w | dc:131627 GDI+ ARGB 位图宽 `(lVar16>>4)-…`, 131635 传 GdipCreateBitmapFromScan0 | 4 |
| uVar20 | gdip_h | dc:131633 同式取高 | 4 |
| local_e8 | size_saved | dc:131328 `=in_stack_00000030` 尺寸参副本, 131845 `0<local_e8` 判失效重绘 | 3 |

## 保留清单（Ghidra 名残留, 共 154 站点, 均为多角色槽复用/伪影专名）

### TablCreateControl（5 名 / 62 站点）

| 旧名 | 站点 | 原因 |
|---|---|---|
| local_e0 | 24 | 双角色: 尾空白裁剪终点/选项区起点(dc:125436-125448) + 回退环扫描游标(125455) |
| uVar8 | 18 | 双角色: 选项前缀匹配结果(125512-125543) + 回退环 NUL 停止符 `uVar8=0`(125455-125466) |
| WVar1 | 9 | 双角色: 跳命令词环字符(125377) + 规格头字符星标判定(125428/125483) |
| WVar2 | 6 | 双角色: 选项区被 NUL 覆盖字符的暂存/恢复(125450/125462) + 字段词头字符(125463) |
| uVar11 | 5 | 双角色: 标志位解析值(125489-125491) + 回退环 `'-'` 停止符(125457, 还原件由 dc uVar15 合并) |

### CtlLoadPictureRgn（9 名 / 92 站点）

| 旧名 | 站点 | 原因 |
|---|---|---|
| lVar16 | 19 | 多角色: GDI+ 尺寸算式临时(131624/131631) + 源高×0x60 分子(131678) + 源宽(131786) + sw 实参(131812) |
| pWVar18 | 17 | 多角色: 游标快照(frm 名 131380, `'<'` 收尾位 131466) + DLL 资源名(131550-131580) + 尺寸值(131618) |
| iVar27 | 13 | 多角色: OLE init 结果(131505-131514) + GDI+ 步幅(131628) + 缺省几何(131796/131801) |
| local_150 | 11 | 三角色: DLL 资源名(131549) + 目标尺寸值(131619) + GDI+ Graphics 出参(131800/131825) |
| local_108 | 9 | 多角色: 图标出参(131565-131567) + 0x60 常量/宽分子(131610/131677) |
| iVar7 | 8 | 多角色: 样式标志源(131611-131615) + GetDIBits 返回(131689-131693) |
| lVar25 | 8 | 多角色: geo_w 快照(131625) + 源高快照/GetDIBits 行数(131666/131684) |
| lVar24 | 3 | 死占位(原件 `(void)lVar24` 镜像 dc 未用槽), 语义不明 |
| unaff_BL | 4 | Ghidra 未定值返回伪影专名, 函数头已注明「原体 unaff_BL 未定值路径」, 有意保留 |

注释内 dc 侧名（不在 code 域, 原样保留作证据）: TablCreateControl 无;
CtlLoadPictureRgn: in_stack_00000030/38, local_130, local_184, local_52, local_b0, local_238,
local_198(头注), pHVar13(头注), unaff_BL(头注) 等共 15 处。

## 纯度自检记录

1. **撞名预检**: 64 新名对 `src/`+`include/` 全树 `\b` 级 grep = 0 撞名; 无 Windows 宏词
   (min/max/near/far/TRUE/FALSE/ERROR/DELETE/COPY…均未涉及)。
2. **行界外**: 全文件 10358 行, 行界 [6073,6392]∪[9153,9846] 之外逐行字节一致（对 git HEAD
   独立复核: outside-bounds changed lines = NONE）。
3. **round-trip 反演**: 行界内每行 code 域应用逆映射后与 HEAD 原文逐字节一致; 正向
   (HEAD code 域施加映射 == 现文) 亦全通过; 注释域逐行字节一致。总行数 10358 不变,
   CRLF 行尾保持, 无 BOM（首字节 `2f2a`=`/*`）。
4. **残留计数**: code 域残留恰为保留清单 62+92=154 站点, 无一漏改/多改（脚本断言
   unexpected residuals = 空）。
5. **替换量**: 552 站点（TablCreateControl 295 / CtlLoadPictureRgn 257）, 63 名;
   `git diff --numstat` = 432 增/432 删对称, 仅 `src/ui/core_b7c.c`（工作区中
   `src/commands/core_b2f.c` 的改动系并行子代理所留, 本代理未触碰）。
6. 未构建、未提交（纪律: 主代理统一验证）。

## 观察项（仅登记, 未改码 — 零行为风险红线）

1. `iVar6`（geo_h）: dc:131608 `iVar6 = local_184`（几何第 2 值 h, 无 `'<` 段时=0）;
   还原件 9560 写作 `iVar6 = (uint32_t)stack_size`（注释自称 local_184）。两槽在 Ghidra
   帧模型中不同（in_stack_30 vs local_184）。疑点待版本核验，本轮只随名不改语义。
2. 几何第 6 值 sy(v6) 缺省: dc:131477 `local_178._0_4_ = local_184`（=已解析 h）;
   还原件 9376 `int v6 = 0`。若 ParseNumSkipChar 对短串不回写则两者有别, 同属上述待核验面。
3. `key_color`（v6=sy）既有名与头注「"<w,h,x,y,sx,sy>"」的第 6 语义一致保留, 未更名。

## 交付物

- `src/ui/core_b7c.c`（6073-6392、9153-9846 两函数 L1 语义化, 其余 10358-1449 行未动）
- `analysis/r25i_l1_b7c_batch1.md`（本文）
