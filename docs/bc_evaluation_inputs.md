# docs/bc_evaluation_inputs.md — 场景 B/C 评估输入材料索引与审计结论摘要

> 编制：B/C 方案审计与更新子代理（TASKS §12-S6 派发）｜日期：2026-08-24
> 基线：refactored @ 4b84546（tag `p4-close`），build 95 OK / link exit 0 双绿实测
> 用途：用户本人评估场景 B / C 时的**一站式材料入口**。方案正文见 `../PRODUCTION_ROADMAP.md`（v3）。
> 纪律：本文件与 PRODUCTION_ROADMAP v3 为本轮仅有的两份写入物；未改任何 .c/.h/link_stubs.c/
> rename_map/TASKS/REVIEW；未 git commit。

---

## 1. 一句话现状

还原战线与命名战线均已收口（tag p4-close）：全部 FourCC 命令真体落地、rename_map 1594、
biz 未命名 46 全处置、REAL_BUG 22 全闭合；仓库处于"高保真参考库"稳态。
剩余工作 = 用户在 S-B（风格生产化）/ S-C（行为等价重写）之间的选择及其执行。

## 2. 材料索引（按用途分组）

### 2.1 决策主文档
| 材料 | 路径 | 说明 |
|---|---|---|
| 方案书 v3 | refactored/PRODUCTION_ROADMAP.md | 本轮全面更新：§2 实测基线、§3 批次重排、§8 WIN 接管点裁定 |
| 收尾规划（冻结） | refactored/TASKS.md §11/§12 | 会话快照、暂停点边界、S2-S5/S6 职责定义——S6 即本轮 |
| 还原战报（冻结） | refactored/REVIEW.md §123–§129 | EXEC/命名批/ls-dirty 回滚教训/G 批管线/46 处置终表/警告终账 |
| WIN 交接总纲 | ../HANDOVER_WIN.md | 交接包清单、WIN 能力边界、纪律红线、接管点待裁事项（已由 ROADMAP §8 裁定） |

### 2.2 终账数据（评估定量依据）
| 材料 | 路径 | 关键读数 |
|---|---|---|
| 警告终账 | tools/warning_census.json | 行为相关 120 条登记（101 唯一位置）；机械四类 core 内清零。⚠️ **修前快照且仅扫 core_*.c（95 TU）**，link_stubs.c 另有 229 条存量（ROADMAP §2.1a/P9） |
| 符号性终账 | tools/signedness_audit.json | 67 条审计 / REAL_BUG **22 全闭合**（末条 A24F ls:18108 `extern int8_t` 已随 aa79220 入库）；FAITHFUL 45；global/sentinel/field 三路扫描均"类别干净" |
| 改名底账 | tools/rename_map.json | **1594** 符号（1157→1594 由 G1-G10 完成）；biz 未命名 46 处置见 REVIEW §128 |
| 数据真值库 | tools/pe_registry.json | 1082 条 PE .data 初值（C-P1 规格直接复用） |

### 2.3 执行工具与配方档案
apply_rename2.py + g_pipeline.py（六步改名管线）、reference_scan.py / gen_tasks.py（指标，
后者有 /tmp/opencode 易失依赖待固化）、nameable_*.json / lowconf_review.json /
dead_stub_evidence.json（命名取证档案）、r3_partial_core_b7c.patch 与
fun_14000e26c_HANDOFF.md（巨函移植配方留档）、tools/subagent_autonomy_clause.md
（子代理提示词必备附件）、wine_smoke.sh（降级为可选开发冒烟）。

### 2.4 WIN 交接包（父目录）
dsh_config_handover_20260824.tar.gz（~/.dsh 全量快照，含凭据——传输后必校 sha256、权限收紧）、
pecmd_repo_p4close.bundle（git bundle，clone 后首验 build/link 双绿）、PECMD原始.EXE
（对拍真值来源，md5 见 HANDOVER_WIN §2）、decompiled.c（Ghidra 反编译全文）。

## 3. 本轮审计实测基线（2026-08-24，复验命令=ROADMAP 附录 B）

### 3.1 指标快照（v2 → v3 实测）
| 指标 | v2 书面基线 | v3 实测 | 备注 |
|---|---|---|---|
| local_xx | 14,908 | **26,625** | 直移真体带入原始拼写；top: ls 6,435 / b3r 6,009 / b2f 2,620 / b7c 2,405 |
| param_N | 7,674 | **13,989** | top: b3r 6,061 / ls 3,315 / b1r 1,496 |
| FUN_ 引用 | 7,521 | **4,967**（唯一拼写 737） | 命名批红利；余量为豁免类+thunk 噪声 |
| DAT_/PTR_ | 1,012 | **2,417** | 随真体落地新增 |
| TODO(verify)/TODO(restore) | ~400 | **426+2** | 诚实标注，C 分诊输入 |
| PECMD_ 可读名引用 | 7,854 | **16,100** | 命名战线产出 |
| 显式伪影(CONCAT/unaff/…) | (6,916*) | **192** | *v2 口径不可比，v3 起以显式 token 正则为准 |
| pecmd_defs.h guard 外行数 | ~500 | **483** | P1 在场 |
| 构建/链接 | 绿 | **95 OK / 0 FAIL；link exit 0** | 本轮实测 |

### 3.2 警告分账（gcc 15.2.0 本轮实测）
- core_*.c：104 条带旗标（cast-func-type 58 / int-to-ptr 22 / sign-compare 14 / nonnull 4 /
  builtin-mismatch 2 / parentheses·unused-label·shift·ptr2int 各 1）＋无旗标归类
  （incompatible-call 11 / type-limits 8 / conflicting-types 1）＝ census 的 120 条行为登记；
  机械四类确认清零 ✔
- link_stubs.c：**229 条**（parentheses 56 / set-but-used 42 / ptr-to-int 27 / qualifiers 22 /
  unused-var 19 / unused-param 14 / pointer-sign 13 / sign-compare 11 / 其它 25）——
  游离于 S5 终账口径之外，其中 ~110 条机械类属低风险清扫候选（ROADMAP 批 0 第 8 项裁决归属）。

### 3.3 结构性问题清单复核结论
P1（guard 外 extern）、P2（重复 typedef + FindFirstVolumeW 错签名）、P3（ls 三职责混装，
现 20,781 行）、P4（CRT 混装 b9）、P5（~96 个会话/临时文件仍被 git 追踪）、P6（third_party 死配置）、
P7（口径声明未加）、P8（core_string.c:39 过期 UNIMPLEMENTED）——**全部仍在场**；
新增 P9（ls 警告口径分裂）/ P10（REVIEW §129 保守三处仍在：b3r_g1.c:140 与 core_b7c.c:2778
memset 内建不匹配、b3r:21205 parentheses、b7c:4930 unused-label）/
P11（ls 三处疑似转写失真恒假线索）。逐条证据与处置批次见 ROADMAP §2.2。

## 4. 平台策略结论（用户已审定，评估时作为既定约束）

1. **场景 B：全程 Linux 即可。**
2. **场景 C：「Linux 开发(mingw 交叉) + 用户真机 Windows 测试」。**
   用户可全程在真机 Windows 上测试；
3. **Wine 降级为可选开发冒烟，永不进入 verdict**——实测拉多进程＋窗口消息过滤，
   无人值守不可靠；金标准必须录自真 Windows；
4. QEMU 可选补自动化，非必需（宿主放 Linux，结果不替代真机 golden）。

## 5. WIN 端 DSH 接管点裁定（摘要，全文见 ROADMAP §8）

**接管点 = R-C0：C-P0「真机金标准录制」子阶段起 WIN 端部分接管；此前一切留 Linux。
只选 B 则 WIN 端全程不参与生产。**
理由：①B 与 C 造物环节强绑定 Linux 已实证的 gcc/bash 管线；②WIN 端不可替代的价值恰是
"物理邻近真机"的采证环节；③diff/verdict 回 Linux 计算，保持单一裁判；④推迟接管使迁移发生在
B0 卫生之后，bundle/tar 一次带走最干净状态。前置检查单与双端衔接协议见 ROADMAP §8.3/§8.4。

## 6. 源码级遗留问题清单（本轮发现，**均未修改任何源码**）

| # | 位置 | 问题 | 建议 |
|---|---|---|---|
| L1 | link_stubs.c（全文件） | 229 条警告存量游离于 S5 终账口径外（P9） | B0 第 8 项裁决归属；机械类随 B1 清扫或整体豁免登记 |
| L2 | link_stubs.c:11986 | `(x & 0xffffffffff) == 0x30003000300020` 恒假（40bit 掩码 vs 56bit 常量），疑似转写失真（P11） | C-P1 以 dc/asm 对照定案，禁 B 期顺手修 |
| L3 | link_stubs.c:1666 / :13410 | 对数组符号 DAT_14013cb18 写 `== 0` 判空恒假，疑把数据读取误成地址比较（P11） | 同上 |
| L4 | core_b3r_g1.c:140、core_b7c.c:2778 | 隐式声明 memset 与内建不匹配（64 位截断风险；REVIEW §129 曾记一处，实为两处）（P10） | 开票，C-P1 定案 |
| L5 | core_b3_remaining.c:21205、core_b7c.c:4930 | parentheses 提示 / unused-label 'hit_out'（P10 其余两处，保守未动） | 低危，随 B1 格式化批顺带处理 |
| L6 | core_string.c:39 | 已实现函数 PECMD_HeapRealloc(@0x140063118) 体内残留过期 "UNIMPLEMENTED…body 未还原"，且地址拼写畸形 `@0xFUN_140063118`（P8） | B4 过期标记清扫批 |
| L7 | link_stubs.c:10361/:15003/:17207 | `-Wcompare-distinct-pointer-types` ×3，census 分类学中无此类目 | 补登记后归入行为类台账 |
| L8 | REVIEW §123 留档 | DAT_14013a24f 文件真值 0x01 vs 早期头文件定义 0 的历史差异（已按真值修正并留档 TODO） | 保持留档，C-P1 复核 |
| L9 | FUN_140025f10 | R1 已还原但 AppendLogMessage 名被占，别名未定（REVIEW §128 待别名项） | B1 第 1 条残余小批 |
| L10 | tools/gen_tasks.py | 依赖易失路径 /tmp/opencode/pecmd_func_classes.json（工具债非源码债） | 迁移/B0 前把输入 JSON 固化入库 |

## 7. 快速复验命令（详见 ROADMAP 附录 B）

```bash
cd refactored
bash build.sh ; echo $?                                   # M6 期望 0
grep -rPno '\blocal_[0-9a-f]{2,}\b' --include='*.c' . | wc -l   # M2 = 26625
python3 -c "import json;print(len(json.load(open('tools/rename_map.json'))))"  # = 1594
for f in core_*.c link_stubs.c; do LC_ALL=C gcc -std=c99 -Wall -Wextra \
  -Wno-implicit-function-declaration -Iinclude -fsyntax-only "$f"; done 2>&1 \
  | grep -c 'warning:'                                    # M8 = 333 (core104+ls229)
```

（完）
