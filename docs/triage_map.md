# triage_map.md — TODO(verify) 全量分诊地图（B5a 提前件 / S-B 交付物）

> 基线：tag B0 后实测 423 条 TODO(verify)。原始数据 tools/triage/triage_out_batch{1-4}.jsonl
> 规则：PRODUCTION_ROADMAP §3.B.5-5a。signedness_audit.json FAITHFUL 45 条已销号（见 §3）。

## 1. 总量与三类分布

| triage | 条数 | 含义 |
|---|---|---|
| RESOLVABLE-STATIC | 287 | 查 decompiled.c 即可定案（字段含义/参数个数/调用约定/简化桩补还原） |
| RESOLVABLE-BLACKBOX | 46 | 需运行原版 EXE 观察 → **C-P0 语料设计直接复用** |
| UNK → KNOWN-LIMIT | 90 | 静态无解且黑盒代价高；其中约 76 条经复核为文件头模板性说明（非单点疑问），随 B5b 统一转 KNOWN-LIMIT 格式 |
| 合计 | 423 | |

## 2. 精修区/重写区建议地图（B1 改名预算分配输入）

判据：TODO 密度 × 真体完整度。**重写区最终裁决权在 C-P0/P2 架构评审**，本表仅是 B 视角建议。

| 区 | 文件（TODO: S/B/U） | 建议 |
|---|---|---|
| 精修区-A（UI/命令真体，B1 重点投入） | core_b7c (46), core_b8m (29), core_b8h (16), core_b2f (20) | local_xx/param_N 语义化全做 |
| 精修区-B（脚本/依赖解析） | core_scriptdep (20), core_b2d/b2e/b2c (~30) | 语义化做；解析器本体 C 阶段可能文法化重写 |
| 精修区-C（长尾命令/helper） | 其余 core_*.c | 标准流水线处理 |
| 桩区（编译所需，浅处理） | restored/win32_api/unimplemented/crt_shims | 仅机械清扫+豁免登记，不做深度语义化 |

## 3. signedness_audit FAITHFUL 销号（45 条）

- core_b1_remaining.c:8401 — if ((param_2 == (LPCWSTR)0) || ((int)param_1 != -1)) {
- core_b3_remaining.c:1929 — if ((char)uVar19 != -1) { local_181f = (char)uVar19 + 1; }
- core_b3_remaining.c:6839 — (uVar1 = ((uint64_t (*)(void))g_pinet_addr)(), (int)uVar1 == -1)
- core_b3_remaining.c:21807 — (local_88 = FUN_14001b7f4(WSTR("PINT.FLAGS")), (int64_t)local_88 > -1)
- core_b3r_h3.c:1512 — if ((char)local_d0 == -1) { ... if ((uint8_t)(local_d0 >> 8) == (uint8
- core_b3r_h3.c:1520 — else if ((char)local_d0 == -2) { ... (uint8_t)(local_d0 >> 8) == (uint
- core_b7c.c:280 — if ((byte)cVar12 >= 0x80) { res_src = list_alloc; }   /* 大文件分块分支 */
- core_b7c.c:326 — if ((byte)cVar12 >= 0x80) { res_src = list_alloc; }   /* 公共尾 */
- core_b2d.c:964 — if (-1 < g_flagA249) { local_lf.lfItalic = 3; }
- core_b2f.c:4590 — if ((g_flagA249 < 0) && ((g_bitFlags & 2) == 0)) {
- …其余 35 条见 tools/signedness_audit.json audit[] verdict==FAITHFUL

## 4. 分诊过程中发现的"静态可证现存问题"（⚠ 超出场景B语义保持红线，只登记不修）

以下各条由分诊代理以 decompiled.c/.data 证据定案为**现源码疑似错误**，
按 §3.B.8 禁区纪律不在 B 阶段修改，全部移交 C-P1 语义规格期定案：

- core_resdecode.c L87/L97/L107：常量 @0x140124128/12c/130 实为 UTF-16LE/BE、UTF-8 BOM 字节（已从原 EXE .data 提取验证）；现源码 "\\x3a\\x3a"(::) 系误还原
- core_b3r_g1.c L134/L138：消息块 offset 12 疑应为 0x10（HWND 栈布局矛盾）
- core_b3e.c L461：FormatSetVar 缺失实参'按0补齐'疑错，dc 中 param_2 即被弃实参
- core_b3r_g2.c L186/L202：local_246 非'未使用'，实为 connect 的 sockaddr_in（dc:63151）
- core_b3r_g2.c L119：status/problem 实参顺序疑似对调（CM API 出参序 dc:62965）
- core_b3b.c L151：返回值近似丢失高位编码差异（dc:55031）
- core_b7c.c:128：dc 原文 cVar12 < '\x01' 表明 list(-1) 会进分块循环，注释断言疑似有误

## 5. 处置去向

- STATIC 287 条 → B5b 标签改造时逐条换 KNOWN-LIMIT 或直接销号（附 dc_hint）
- BLACKBOX 46 条 → C-P0 语料用例设计输入（探针用例来源）
- UNK 90 条 → B5b 转 KNOWN-LIMIT 格式入 docs/known_limits.md 附录

## 6. 分诊发现 7 处静态可证问题的复核结论（2026-08-24 会话）

| # | 位置 | 分诊断言 | 复核结论 | 处置 |
|---|---|---|---|---|
| ① | resdecode BOM×3 | UTF-16LE/BE/UTF-8 BOM | ✅ 确认（PE .data 提取铁证+dc 常量比较佐证） | 已修 (66c3d17) |
| ② | b3r_g1 消息块 | offset 12→0x10 | ✅ 确认，另发现第二处 0x2c→0x30 与 memset 起点 | 已修 (66c3d17) |
| ③ | b3e FormatSetVar | 按0补齐疑错 | ⚠️ 证据不足：dc 无 param_2 透传，r8 残留不可静态定案 | 注释升级 BLACKBOX |
| ④ | b3r_g2 NTP | local_246 实为 sockaddr | ✅ 确认且影响最大（NTP connect 恒失败） | 已修 (66c3d17) |
| ⑤ | b3r_g2 CM 实参对调 | 疑似对调 | ❌ 误报：与 dc:62965 逐字一致 | 不修 |
| ⑥ | b3b 返回值 | 高位编码丢失 | ✅ 确认（dc:55027），零调用方死代码 | 已修 (66c3d17) |
| ⑦ | b7c 分块注释 | 注释断言有误 | ❌ 误报：dc cVar12<1 与移植条件逻辑等价（-1=未知算法非 list） | 不修 |
