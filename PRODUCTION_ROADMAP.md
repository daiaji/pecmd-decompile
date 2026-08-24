# PRODUCTION_ROADMAP.md — PECMD 重构产线总体方案（场景 B / C 完整版）

> 本文是后续工作的**总方案书**：覆盖场景定义、现状基线、场景 B 全部批次任务书、
> 场景 C 全部阶段任务书、合并执行时间表、WIN 端接管点裁定、流程纪律、风险登记册与全部模板。
> 前置文档：TASKS.md / REVIEW.md（还原阶段台账，本文大量引用其编号）。
>
> **版本：v3（p4-close 后审计修订版）｜基线日期：2026-08-24｜基线 commit：4b84546（tag `p4-close`）**
> 所有计数均为当次 grep/gcc 实测，复验命令见附录 B；与 v2 的差异见 §2 各表"v2 基线"列。
> v3 修订要点：①§2 基线全面刷新（还原/命名两战线已清零，残留构成剧变）；②§3 批次按现实重排
> （原批 1 的 FUN_ 改名已被 G1-G10 提前完成）；③新增 §8「WIN 端 DSH 接管点裁定」；
> ④平台策略定稿：场景 C 金标准只录自真 Windows，Wine 降级为可选开发冒烟。
> 评估输入材料索引另见 `docs/bc_evaluation_inputs.md`。

---

## 0. 文档地图与使用方法

| 章节 | 内容 | 谁在什么时候读 |
|---|---|---|
| §1 | 场景决策框架 | 立项/立项变更时 |
| §2 | 现状基线（实测+文件映射+问题清单） | 一切批次开工前 |
| §3 | 场景 B 任务书（批 0-5，重排版） | 执行 B 时逐批对照 |
| §4 | 场景 C 任务书（Phase 0-5） | 执行 C 时逐阶段对照 |
| §5 | B→C 合并时间表与门禁 | 排期与验收 |
| §6 | 流程纪律（git/台账/代理） | 全程 |
| §7 | 风险登记册 | 全程，每门禁复审一次 |
| §8 | WIN 端 DSH 接管点裁定（v3 新增） | 选定场景后、任何跨端迁移前 |
| 附录 A/B/C | 运行手册 / 指标命令 / 模板 | 随用随查 |

使用规则：任何批次开工前先抄录该批"验收"小节为 PR 描述；完工时附附录 B 当次输出。
禁止口头宣称清零/完成。

---

## 1. 决策框架：三个场景

| 场景 | 目标定义 | 正确性承诺 | 验收口径 | 成本量级 |
|---|---|---|---|---|
| S0 维持现状（已达成+超越） | 反编译参考库 | 无（仅可编译可链接） | build/link 绿 | 已沉没 |
| **S-B 风格生产化** | 代码外观达生产规范；组织、命名、类型、结构体化 | **无行为验证**；一切改动语义保持 | §3 各批验收 + 附录B 指标 | 单人 4-6 周；多代理流水线 2-3 周（构成已变，见 §3.B.7） |
| **S-C 生产行为等价** | 可维护、可继续开发的生产代码 | 黄金语料对拍绿 + 分歧登记完备 | §4 各 Phase 门禁 | 数月（详见 §4.C.7） |

选择规则：
- 只做参考/存档 → 停在 S0（实际已停在"S0+"：还原/命名清零且全绿）；
- 要门面（演示/交接）→ S-B；
- 要真货（继续开发/移植）→ S-C；
- **按 S-B 的预算期望 S-C 的结果是唯一错误选项**（阶段 7 的停滞已证明此路不通）。

---

## 2. 现状基线（2026-08-24 实测，commit 4b84546 / tag p4-close）

### 2.0 战线状态速览（v3 新增）

| 战线 | 状态 | 依据 |
|---|---|---|
| 还原 | **清零**。EXEC(3948dc行)/SHOW(1680)/ENVI(1848dc,真体1729)/REGI(1360)/RAMD(1047)/TABL/DISK/HASH/ListCtrl/PicRgn 及三中桩全部真体落地 | REVIEW §123/§128、commits 59c1736/a3269a3/7d40a6a/e895b5e 等 |
| 命名 | **收敛达成**。rename_map.json **1594** 符号；biz 未命名 46 全部处置落账（SKIP/INDIR 已移植/KEEP_LOW/待别名）；crt 147 永久豁免 | REVIEW §125-§128；TASKS §12 |
| 警告 | 机械四类在 core_*.c 内**清零**；行为相关 **120 条登记**（warning_census.json）。⚠️ 该终账口径=core_*.c 95 TU；link_stubs.c 另有存量 229 条未入账（新问题 P9） | REVIEW §129 + 本轮复测 |
| 符号性审计 | REAL_BUG **22 条全部闭合**（21 条此前已修 + 最后一条 A24F link_stubs 侧 `extern uint8_t→int8_t` 已随 aa79220 入库，本轮实测 ls:18108 确认） | tools/signedness_audit.json + 本轮复核 |
| 构建 | build.sh **95 OK / 0 FAIL**；gcc 完整链接 exit 0——双绿 | 本轮实测（附录 B M6） |
| 台账 | TASKS §11/§12 快照+收尾规划；REVIEW 至 §129；HANDOVER_WIN.md（父目录）已备 WIN 交接包 | — |

### 2.1 反编译残留量化（core_*.c + link_stubs.c 全量实测）

> 关键结论：**残留总量不降反升**。原因是还原波把 decompiled.c 的巨函数真体直移入库，
> 这些真体内含原始 `local_xx/param_N/FUN_` 拼写。v2 基线测量时它们还是空桩（无局部变量可数）。
> 因此 B 场景的"清理工作量"比 v2 书面估计更大、且高度集中（见 per-file 列）。

| 指标 | v2 基线 | **实测 (v3)** | 复验 | 主要集中文件 |
|---|---|---|---|---|
| `local_xx` Ghidra 局部变量 | 14,908 | **26,625** ↑ | M2 | link_stubs 6,435 / b3_remaining 6,009 / b2f 2,620 / b7c 2,405 |
| `param_N` 形参 | 7,674 | **13,989** ↑ | M3 | b3_remaining 6,061 / link_stubs 3,315 / b1_remaining 1,496 |
| `FUN_14xxxxx` 引用 | 7,521 | **4,967** ↓（唯一拼写 737 个） | M1 | link_stubs 748 / b7c 478 / b3_remaining 311 |
| CONCAT/unaff/extraout/in_stack/local_res 显式伪影 | 6,916* | **192** | —（正则见附录B注） | — |
| `DAT_/PTR_` 数据符号引用 | 1,012 | **2,417** ↑ | M4 | 随真体落地新增 |
| TODO(verify) / TODO(restore) | ~400 | **426 + 2** | M5 | 分散（诚实标注，C-P1 分诊输入） |
| PECMD_ 可读名引用（对照） | 7,854 | **16,100** ↑ | — | 命名战线的直接产出 |

\* v2 的 6,916 与本次 192 口径不可比（v2 正则更宽，疑似含 uVar/iVar 类短名）；v3 起
以附录 B 的显式伪影正则为准，历史数字不再引用。

### 2.1a 警告分账（v3 新增，本轮 gcc 15.2.0 实测）

| 范围 | 计数 | 构成 | 说明 |
|---|---|---|---|
| core_*.c（95 TU） | 104 条带旗标 | cast-function-type 58 / int-to-pointer-cast 22 / sign-compare 14 / nonnull 4 / builtin-decl-mismatch 2 / parentheses·unused-label·shift·ptr2int 各 1 | 与 census 行为类登记吻合；另有 incompatible-call(11)/type-limits(8)/conflicting-types(1) 为无旗标手工归类 → 合计 **120 条行为相关登记**，机械四类确认为 0 |
| link_stubs.c | **229 条** | parentheses 56 / set-but-used 42 / ptr-to-int 27 / discarded-qualifiers 22 / unused-var 19 / unused-param 14 / pointer-sign 13 / sign-compare 11 / int-to-ptr 9 / cast-func-type 5 / distinct-ptr-types 3 / 其它 8 | **不在 S5 终账口径内**（census files_scanned=95）。其中机械类 ~110 条属低风险清扫候选；`-Waddress`×2 与 `-Wtautological-compare`×1（ls:1666/13410/11986）是疑似转写失真的新线索（见 §2.2 P11） |

> ⚠️ 使用 warning_census.json 时必须知道：它是**修前快照**且**仅扫描 core_*.c**。

### 2.2 结构性问题清单（= B 批 0 输入，逐条本轮复核）

| # | 问题 | 本轮复核证据（2026-08-24） | 处置批次 |
|---|---|---|---|
| P1 | `#endif`(L153) 后 ~483 行 extern 在 guard 外 | pecmd_defs.h 现 649 行，guard 外 483 行（M7 实测） | B0 |
| P2 | 双 Win32 类型体系矛盾；HFONT(L30/L38)、SC_HANDLE(L54/L859) 重复 typedef；FindFirstVolumeW/Next 签名为 `(LPWSTR,DWORD,DWORD*)` 与真实 `(LPWSTR,DWORD)` 不符靠强转绕行 | win32_stub.h 本轮 grep 确认全部仍在 | B0 |
| P3 | link_stubs.c 三职责混装（Win32 桩/FUN_ 缺口桩/PECMD_* 别名转发）——现膨胀至 **20,781 行**（v2 时 16,255），因 EXEC/INDIR 巨函真体直移 | wc -l 实测 | B0（拆分范围裁决见批 0 第 8 项） |
| P4 | ~250 个 CRT 定义混在业务源码；strtol/perror/raise/iswctype/wcstol 同名强定义；`uint64_t f(void)` 型错误签名 | core_b9_remaining.c grep 命中确认 | B0（隔离）/C（真删） |
| P5 | 会话工作文件仍在 git 追踪：batch28*.txt 等 14 个 + ghidra_out/*.c 14 个 + tools/tmp_p4_batch/ 68 个（共 ~96 个非产物文件；仓库共 276 文件） | git ls-files 实测 | B0 |
| P6 | build.sh third_party/giflib 死配置仍存（L9/L16/L19）；（无参空循环已修于 95d3ea5） | grep 确认 | B0 |
| P7 | 文档口径：TASKS/REVIEW 已多轮刷新至 §129/§12，但"历史台账以本文档 §2.1 实测为准"声明仍未加；REVIEW 早期重复合并章节未复核 | 本轮阅读确认 | B0（半天文档项） |
| P8 | 过期标记：core_string.c:39 在**已实现**的 PECMD_HeapRealloc(@0x140063118, 有完整函数体) 内残留 "UNIMPLEMENTED…body 未还原"，且地址拼写畸形 `@0xFUN_140063118` | 本轮 sed 确认 | B4（顺带全量 grep 'UNIMPLEMENTED' 复核） |
| **P9** | **（v3 新增）link_stubs.c 警告存量 229 条游离在 S5 终账口径之外**；其中机械类约 110 条可低风险清扫，行为类比照 census 分类补登记 | §2.1a 实测 | B0 裁决归属 + B1 清扫 |
| **P10** | （v3 新增）REVIEW §129「保守未动三处」仍在：b3r_g1.c:140 与 core_b7c.c:2778 隐式 memset 内建不匹配（64 位截断风险，建议开票）、b3r:21205 parentheses、b7c:4930 unused-label | 本轮 -Wall 实测定位 | B1/C-P1 |
| **P11** | （v3 新增）link_stubs.c 三处疑似转写失真线索（只登记不改）：ls:11986 `(x & 0xffffffffff)==0x30003000300020` 恒假（掩码 40bit vs 常量 56bit）；ls:1666/13410 对数组符号 DAT_14013cb18 判 `==0` 恒假（疑把数据读取误成数组地址比较） | 本轮 -Waddress/-Wtautological 定位 | C-P1 语义规格期定案 |

### 2.3 现有源码文件 → 目标功能模块映射（B 批 2 初版映射，允许 ±一档微调；判据=文件头注释主语义）

| 目标模块 | 现有文件 |
|---|---|
| src/app | core_main.c, core_init.c |
| src/kernel | core_string.c, core_strbld.c, core_globals.c(暂整置，C 阶段拆分) |
| src/lang | core_token.c, core_execline.c, core_srparse.c, core_exec.c, core_exec2.c, core_exec3.c, core_exec4.c, core_execmain.c, core_script.c, core_script2.c, core_scriptrun.c, core_execscript.c, core_resdecode.c |
| src/runtime | core_var.c, core_var2.c, core_var3.c, core_exec5.c, core_scriptdep.c, core_thread.c, core_proc.c |
| src/commands | core_cmd4.c, core_b2a-f.c, core_b3a-n.c, core_b3_remaining.c, core_b3r_a…i28f.c 中非设备类 |
| src/ui | core_msgbox.c, core_b7a/b/c.c, core_b8a-m.c, core_b8_remaining.c, core_b8_failed.c |
| src/device | core_sys.c, core_b3r_g*.c, core_b3r_h*.c 中磁盘/卷/网络类 |
| src/misc(待消化) | core_b1_remaining.c, core_b9_remaining.c, core_remaining_helpers.c, core_skip_registry.c |
| (独立) | link_stubs.c → 拆分后不入 src/，见 B0-4 |

> 注 1：b3r_* 批次文件内部主题混杂，批 2 按"文件整体归入主导语义目录"执行；
> 函数级跨文件搬移留给 C 阶段。
> 注 2（v3）：core_b7c/b8 族因 SHOW/TABL/DISK/HASH/REGI 真体落地显著增大（b7c 单文件 local_xx
> 2,405 处），ui/device 两目录的 B1 工作量权重相应上调。

### 2.4 可复用资产盘点（v3 刷新）

| 资产 | 用途 | 现状 |
|---|---|---|
| rename_map.json（**1594** 符号）/ FUNC_NAMES.md | 改名底账 | 收敛态；biz 46 已处置（REVIEW §128） |
| apply_rename2.py（变体探测/占用检查/--sync-ls）/ g_pipeline.py | 改名执行器与六步管线 | 成熟，G5-G10 连环零修补验证 |
| reference_scan.py / gen_tasks.py / classify_funcs.py | 指标与分类 | gen_tasks 依赖 /tmp/opencode 易失路径（工具债，迁移前需固化输入） |
| pe_data_extract.py / pe_registry.json（**1082** 真值，本轮复验） | C-Phase1 数据初值规格 | 就绪 |
| warning_census.json（120 行为条目，**core-only 修前快照**）/ signedness_audit.json（67 条/REAL 22 全闭合） | 警告与符号性终账 | 就绪（口径注意见 §2.1a） |
| nameable_*.json / lowconf_review.json / dead_stub_evidence.json / c8a-c8c skips | 命名战线取证档案 | 冻结留档 |
| r3_partial_core_b7c.patch / fun_14000e26c_HANDOFF.md | 巨函移植配方档案 | 留档备查 |
| ghidra_out/(asm+伪码)、big_funcs/、decompiled.c | 硬骨头协议原始依据 | ghidra_out 尚被 git 追踪（P5） |
| tools/subagent_autonomy_clause.md / wine_smoke.sh（降级为可选冒烟） | 流程附件 | wine_smoke 仅 Linux 开发参考，永不进 verdict |
| HANDOVER_WIN.md（父目录）+ dsh_config_handover_*.tar.gz + pecmd_repo_p4close.bundle | WIN 端交接包 | 就绪，待 §8 裁定的接管点启用 |

---

## 3. 场景 B 任务书（语义保持的风格生产化）— v3 重排版

### 3.B 总则

- **红线**：一切改动语义保持。兜底手段 = 编译器类型检查 + `_Static_assert(offsetof)` +
  每批 build/link 双绿；**没有测试，所以凡编译器管不住的改动都在禁区（§3.B.8）**；
- 节奏：每批一个 git 提交点（tag `B<n>`），PR 描述预拷贝该批"验收"节；
- 进度看板只用附录 B 命令输出，禁止口头口径；
- **v3 背景变化**：原批 1 的"FUN_→PECMD_ 改名清零"已被还原期的 G1-G10/S4 命名批提前完成
  （rename_map 1157→1594，biz 未命名 46 全处置）；原批 0 卫生包经逐项复核**绝大多数仍有效**
  （P1/P2/P4/P5/P6/P8 全部在场）。因此 v3 的批 1 重心转移到 local_xx/param_N/DAT_ 语义化主体。

### 3.B.0 批 0：卫生与结构修复（1-2 天，零语义风险）

任务分解（对应 §2.2 问题编号，均为本轮复核仍在场的项目）：

1. **P1 头文件修复**：将 pecmd_defs.h 的 `#endif` 移到文件末尾；或（推荐）拆两文件：
   `pecmd_defs.h`（类型+函数声明）与 `pecmd_globals.h`（全部 extern g_），后者由前者 include；
   所有 `#include "pecmd_defs.h"` 处不改。验收：M7=0；build 绿。
2. **P5 仓库清洁**：
   ```bash
   printf 'tmp_*\nbatch28*\ngh?.txt\ng[ABCD].txt\nout[ABCD].txt\nwf28.js\ntools/tmp_p4_batch/\n' >> .gitignore
   mkdir -p attic/reference attic/session_2026-08
   git mv ghidra_out tools/tmp_p4_batch attic/reference/
   git rm --cached batch28*.txt g[0-9A-D]*.txt gh[0-9]*.txt   # 会话文件移 attic 后一并处理
   ```
   验收：`git ls-files` 中非产物文件≈0；仓库根只剩 *.md/*.sh/include/core/link/tools/attic/docs。
3. **P4 CRT 隔离**：新建 `crt_shims.c`，将 core_b9_remaining.c 中三类整体迁移：
   (a) MSVC CRT 内部机制族；(b) 与 libc 同名者；(c) 数学空桩。
   文件头横幅注明"仅链接验证用，部署链接前必须移除"。验收：build/link 绿；错误签名桩集中单文件。
4. **P3 link_stubs 拆分**（纯机械切分，不改内容）——**先做第 8 项裁决再动本项**：
   - Win32 API 形符号 → `win32_api_stubs.c`；FUN_/thunk_/DAT_/PTR_/数据 → `unimplemented_stubs.c`；
     PECMD_* 且函数体为转发真实体者 → 迁往真实体所在 TU 文件尾标注 `/* public alias */`；
   - 验收：link 绿；三文件合计 ≈ 原 20,781 行。
5. **P2 类型统一**：win32_stub.h 删 HFONT/SC_HANDLE 重复 typedef；修 FindFirstVolumeW/
   FindNextVolumeW 为真实签名 `(LPWSTR,DWORD)` 并同步显式转换调用点；新建
   `win32_stub_lite.h` 供桩 TU 收敛本地 typedef。验收：build/link 绿。
6. **P6/P7**：build.sh 删 third_party 分支；REVIEW/TASKS 文首加"历史台账，口径以
   PRODUCTION_ROADMAP §2.1/§2.1a 实测为准"声明。
7. **提交**：tag `B0`。
8. **（v3 新增）P9 归属裁决 + link_stubs 警告清扫**：
   - 先裁定 scope：若选 B，link_stubs.c 是否纳入精修区（推荐：**纳入机械类清扫**——
     ~110 条 unused-param/set-but-used/qualifiers/pointer-sign 属 `(void)x;` 沉底级低风险；
     行为类 119 条比照 census 分类补登记，不清零）；
   - 若该文件被裁定整体划入 C 重写区跳过深改，则在本文件头加 SCOPE 注记并在
     docs/known_limits.md 登记，B 门禁指标明确排除之；
   - P10 三处顺手开票（不修），P11 三处登记进 known_limits。
   验收：`-Wall -Wextra` 下 core+ls 合计警告 = 行为类登记数（每条有 census 编号）+ 明示豁免清单。

批级验收：build.sh 95 OK；完整链接 exit 0；M7 空；attic 外无会话文件；警告账面自洽（M8）。

### 3.B.1 批 1：残余改名与语义化（v3 重排：重心已换，2 周±，脚本化+子代理流水线）

前置输入：B0 完成；TODO 分诊结果（§3.B.5-5a 提前件）产出精修区/重写区地图
（改名预算只花在精修区；重写区仅保留编译所需）。

四条流水线的现状与残余：

1. **FUN_→PECMD_（≈已完成，转为登记管理）**：
   rename_map 1594 已应用；残余唯一拼写 737 个中——biz 46 已全部处置落账（REVIEW §128：
   纯桩 SKIP×5 / INDIR 已移植×3 / KEEP_LOW 待运行时钉死×3 / 零引用 vtable 候选 ~22 / 
   已还原待别名 25f10 等），crt 147 永久豁免，其余为 thunk/别名拼写噪声。
   **本项工作只剩**：25f10 别名小批 + KEEP_LOW 三处保持登记。禁止为凑零而给豁免类强加名字。
2. **param_N → 语义形参（主体，13,989 处）**：逐文件子代理批（每文件一代理）；保守字典
   `p/pp=游标, buf=缓冲, cnt/count=计数, flags=标志, ctx/hObj=对象指针, hXxx=句柄`；
   禁止臆造强语义名。优先序：b3_remaining(6,061) → link_stubs(3,315) → b1_remaining(1,496) → 其余。
3. **local_xx → 语义局部（主体，26,625 处）**：高频优先（link_stubs 6,435 / b3_remaining 6,009 /
   b2f 2,620 / b7c 2,405）；(void)死变量直接删除（编译器兜底）；与 P9 机械警告清扫同批合并执行，
   一把铲子两个坑。
4. **DAT_/PTR_ → g_ 迁移（2,417 处）**：按 REVIEW §4a-l 剧本；类型存疑者 void*+KNOWN-LIMIT。

验收：M1 代码命中仅剩豁免清单内地址（注释除外）；M2/M3 在精修区文件内≈0；
build/link 绿；rename_map 同步更新；警告账面仍自洽（M8）。

### 3.B.2 批 2：按功能重组文件（2-3 天，纯移动）

- 按 §2.3 映射执行 git mv；build.sh 通配改为显式 find 注入或 `src/**/*.c`；
- 根 README.md（是什么/构建方法/目录地图/**定位声明：风格生产级≠行为已验证；
  Wine 冒烟仅为开发参考，金标准录自真 Windows**）+ 每模块一句话 README；
- 不做函数级跨文件搬移。

验收：build/link 绿；每个新目录 README 就位。

### 3.B.3 批 3：Top 对象结构体化（1-2 周，判断密集）

范围限定：**仅布局已被 REVIEW 登记的对象**（PECMD_WndObj / PECMD_VarNode / PECMD_Script(0xe0) /
PECMD_Task，字段偏移表沿用 v2 版不变，见 git 历史 v2 §3.B.3 表格）：

实施步骤（每结构体一轮）：
1. `include/domain/<name>.h` 定义 struct + 每字段 `_Static_assert(offsetof(...)==…)`；
2. 选使用密度最高文件试点替换 `*(HWND*)(obj+0x20)` → `obj->hwnd`，逐 diff 人工过目；
3. build 绿后推广；pecmd_offsets.h 对应宏标 deprecated 并逐步清引用；
4. 未登记布局的对象不碰（发现新布局先登记再纳入下一轮）。

验收：试点对象宏引用清零；静态断言全通过；build/link 绿。

### 3.B.4 批 4：去重与格式统一（3-5 天）

1. 家族合并程序（DestroyObjectWithFree 族 / SendWindowMessage202/204 / InitXxxObjCore 族 /
   ReleaseObjectA…L 族）：diff 确认仅"被调清理函数+常量"不同 → 抽公共实现 → 原名薄包装 →
   调用点不动 → build 绿 → 下一族；
2. 删过期标记：P8（core_string.c:39）及全量 grep 'UNIMPLEMENTED' 复核；
3. 统一文件头模板与注释语言政策（中文为主，标识符英文）；
4. 提交 `.clang-format`（LLVM 基底，列宽 100，缩进 4）并全库格式化一次（独立提交）。

### 3.B.5 批 5：TODO 收容与门面收尾（2-3 天；分诊部分提前至批 1 前）

**5a. TODO 分诊（提前件）**：对 428 个 TODO(verify/restore) 逐条打标签：
`[RESOLVABLE-STATIC]`（asm/decompiled 可定案）/ `[RESOLVABLE-BLACKBOX]`（构造输入观察原版可定案，
供 C-P0 语料设计复用）/ `[UNK]`（转 KNOWN-LIMIT）。产出 `docs/triage_map.md`。
signedness_audit.json 的 FAITHFUL 45 条可直接销号一批；P11 三处恒假线索强制进 BLACKBOX/UNK。

**5b. 标签改造**：非 RESOLVED 者统一 KNOWN-LIMIT 格式（id/地址/现象/影响面/triage），
汇总 `docs/known_limits.md`。

**5c. provenance.sidecar**：`docs/provenance.map`（TSV：新符号 \t @0x地址 \t 原Ghidra名 \t 来源文件），
随后批量删除代码内 `@0x` 注释噪声（KNOWN-LIMIT 内地址保留）。tools/gen_provenance.py 入库。

**5d. 定位声明**：README 首屏固定一行：
`> 本库风格达生产规范，但行为未经系统对拍验证（见 PRODUCTION_ROADMAP 场景B边界）。`

### 3.B.6 批级验收与回滚

- 每批 PR 必附附录 B 输出前后对照；
- 任一批 build/link 破坏且 30 分钟内无法定位 → `git revert` 整批，缩小粒度重做；
- 批间允许穿插，tag 顺序固定 B0<B1<B2<B3<B4<B5。

### 3.B.7 工作量与产出物清单（v3 修订）

单人 4-6 周 / 多代理流水线 2-3 周（总量与 v2 持平，但构成已变：
FUN_ 改名清零红利 ≈ 抵消 local/param 直移增量；最大单项变为 local_xx 语义化 26.6k 处）。
产出：整洁源树（src/ 布局）、domain 结构体头、known_limits.md、triage_map.md、provenance.map、
README+定位声明、clang-format 配置、crt_shims/win32_api/unimplemented 三桩文件、
link_stubs 警告账面闭环（P9）。

### 3.B.8 明确禁区（越界即升级 S-C）

去 goto / 控制流重写；巨型函数拆分主体；解析器文法化重写；全局标志收敛 context 结构体；
错误处理策略改造；CRT 同名桩真删改链系统库；P11 恒假线索的"顺手修复"；任何"顺手修逻辑"的行为。

---

## 4. 场景 C 任务书（绞杀者重写 + 行为对拍）

### 4.C.0 核心策略

旧树停止打磨 → 参考实现 + 行为预言机；新建 `pecmd-next/` 干净树按模块重写；
每换一模块跑差分对拍。**唯一验收线：与原版行为一致，或在 divergences.md 显式登记。**

### 4.C.1 Phase 0：行为基线（1-2 周）

**0.1 目录约定**
```
harness/
  corpus/cases/<id>_<slug>/{setup/,setup.reg,manifest.json,main.pecmd,epilogue.pecmd}
  backends/win_real.sh qemu_win.sh qemu_winpe.sh [wine_dev_smoke.sh]
  runners/run_case.sh collect.py diff_case.py report.py
  results/<backend>/<case>/…
  golden/<backend>/<case>/…      # 原版录制
```

**0.2 用例 manifest.json**
```json
{ "id":"002_envi_scope", "cmds":["ENVI"], "vars":["A","B","OUT"],
  "timeout_s":60, "flaky":false, "gui":false, "notes":"" }
```
（v3：新增 `gui` 字段——弹窗类用例必须用户在场监督运行，见 0.6 与 §8 衔接协议。）

**0.3 尾声规范（runner 按 vars 自动生成 epilogue.pecmd）**
```ini
ENVI T_CASE=<id>
WRITE C:\pectest\out\vars.txt,CASE=<id>
WRITE -a C:\pectest\out\vars.txt,V_A=%A%
WRITE -a C:\pectest\out\vars.txt,V_B=%B%
ENVI T_DONE=OK
WRITE C:\pectest\out\done.txt,%T_DONE%
```
（WRITE 追加旗标以真机实测为准；若不支持 -a 则 runner 生成单行合并版。）

**0.4 四维采集**
| 维度 | 采集 | 实现 |
|---|---|---|
| stdout/exit | 进程直捕 | runner tee + exitcode |
| 变量表 | out/vars.txt | 见 0.3 |
| 文件系统 | 用例沙箱目录树 | `find -type f -exec sha256sum` 前后对比 |
| 注册表 | 涉事键导出 | `reg export` 前后（真机原生执行）|

**0.5 diff_case 输出契约（results/<case>/verdict.json）**
```json
{ "case":"002", "backend":"win_real",
  "stdout":{"same":true}, "exit":{"exp":0,"got":0},
  "vars":{"same":false,"diff":[{"k":"V_B","exp":"Win","got":"win"}]},
  "fs":{"same":true,"added":[],"removed":[],"changed":[]},
  "reg":{"same":true}, "verdict":"FAIL", "flaky_pool":false }
```
掩码规则集中在 `masks.conf`（时间戳/路径/随机值正则）。

**0.6 后端实现要点（v3 平台策略定稿）**

> **平台策略结论（用户已审定，纳入本方案作为约束）**：
> - 场景 B 全程 Linux 即可；
> - 场景 C 采用「Linux 开发(mingw 交叉) + 用户真机 Windows 测试」；用户可全程在真机上测试；
> - **Wine 降级为可选开发冒烟，永不进入 verdict**——实测其拉多进程+窗口消息过滤，
>   无人值守不可靠；且金标准必须录自真 Windows；
> - QEMU 可选补自动化，非必需。

- `win_real.sh`（金标准后端）：在用户真机 Windows 上录制 golden。由 WIN 端 DSH 或用户手动执行
  「部署用例包 → 运行原版 EXE → 回捞 out/ 目录」三步（衔接协议见 §8.4）；产物回 Linux 后统一跑 diff；
- `qemu_win.sh`（可选）：qcow2 backing 秒级还原；virtio-fs/SMB 投递；QMP 管控；仅作自动化补充，
  其结果同样**不替代真机 golden**，只用于回归加速；宿主在 Linux 侧；
- `wine_dev_smoke.sh`（可选，降级保留）：仅开发期快速冒烟（附录 A 手册），输出永不写入 verdict/golden；
- `qemu_winpe.sh`：WinPE ISO 注入 autorun，串口日志，仅发版冒烟集（可选）。

**0.7 三方对拍（杠杆）**
```bash
# 旧树编 PE（预期需 2-3 天调试；mingw 工具链可用性 = C-P0 第一检查项，至今未验证）
x86_64-w64-mingw32-gcc -std=c99 -w -Wno-implicit-function-declaration \
  -Iinclude <src_core_files...> win32_api_stubs.c unimplemented_stubs.c -o pecmd_old.exe
```
已知坑与对策（沿 v2）：(a) CRT 同名冲突 → objcopy 弱化 crt_shims 同名符号；
(b) 入口冲突 → `-nostartfiles` 自管入口或 `-Wl,--entry=mainW`；
(c) 未实现 FUN_ 桩运行期暴露 → verdict 差异自动生成补桩优先级清单。
三方矩阵：`golden(win_real)=原版EXE`、`old=pecmd_old.exe`、`new=pecmd_next.exe` 同语料互 diff。

**0.8 语料扩充顺序**：核心 30 命令 → 控制流（IFEX/FIND/LOOP/SUB/TEAM）→ 设备/服务长尾。
每命令至少：正例、边界例、错误输入例。TODO 分诊 BLACKBOX 类直接转化为探针用例。

**0.9 验收（Phase 0 门禁 G0）**
- run_case/diff/report 三工具闭环，一条命令出报告；
- ≥30 核心命令用例的 golden **全部录自真机 Windows**（wine/qemu 录制一律不算数）；
- flaky 隔离清单建立；gui=true 用例具备人工监督运行规程；
- （加分项，非门禁）旧树 exe 在 Wine 下 ENVI/FILE/WRITE 冒烟跑通。

### 4.C.2 Phase 1：语义规格恢复（2-3 周）

1. 文法抽取：素材=core_execline(展开器族)/core_strbld(6槽构建器)/core_srparse(分隔符配置)/
   REVIEW §5.3-5.4；产出 `docs/spec/grammar.ebnf` + 展开优先级表；
2. 结构体定稿：继承 B3 产物 + 补齐消息映射表(0x28/项)、内存块头(-8 cap/-4 magic)；
3. 模块语义卡 `docs/spec/<module>.md`：
   `职责/输入/输出/副作用/错误行为/关联命令/KNOWN-LIMIT 引用/开放问题`；
   P10 memset 内建不匹配两处、P11 三处恒假线索在此阶段以 dc/asm 对照定案；
4. TODO 终审：triage_map 的 RESOLVABLE 两类清账；UNK 全部落 divergences.md。
   **规则：未定案 TODO 阻塞所属模块 DoD。**

门禁 G1：grammar.ebnf + 全模块语义卡 + divergences.md 初版评审通过。

### 4.C.3 Phase 2：目标架构设计（~1 周）

```
pecmd-next/
  platform/   win32 薄封装（进程/文件/注册表/服务/GDI/网络 六面）
  kernel/     mem(标准堆+所有权) str容器 vec buf
  lang/       lexer expand parser exec-engine
  runtime/    vartable script-object task scheduler ctx
  commands/   cmd_file cmd_envi cmd_exec …（每命令一 TU）
  ui/         widget framework dialog msgbox
  app/        main init shutdown
```
- 上下文治理：`PECMD_Runtime`（进程级）/`PECMD_Session`（会话级）两级结构收容全局态；
- 错误策略：`typedef enum PECMD_ERR {...}` + 传播约定 + UI policy 回调；
- 风格指南三章（命名/所有权/错误处理）先行评审冻结；
- provenance.map 生成器沿用 B5c。

门禁 G2：架构 RFC 评审通过（重点审 platform 边界与 ctx 设计）。

### 4.C.4 Phase 3：逐模块重写循环（主体）

模块序（依赖深度优先）：kernel → runtime/vartable → lang/lexer+expand → lang/parser+engine →
commands(按语料频率分批) → ui → device/service/net 长尾。

每模块固定循环：
```
读语义卡 → 写接口头 → 实现 → 模块单测(语料切片) → 接入三方对拍 → diff 归零或入分歧表 → PR
```
DoD 四件套：**规格✓ 实现✓ 测试✓ 对拍绿✓**。
旧树用法：小包装/字符串族可"照抄式重写"；解析器/UI 一律新写，旧文件仅作输出对照器。
对拍的 golden 采集继续走 §8.4 真机通道（WIN 端执行部署/运行/回捞三步）。

门禁 G3：核心 30 命令对拍全绿（变量表/文件系统/注册表三维）。

### 4.C.5 Phase 4：硬骨头专项

| 类型 | 协议 |
|---|---|
| 巨型解析器(29KB/20KB 级) | 禁逐行翻；按 grammar.ebnf 新写；模糊怪异脚本差分；旧函数仅当 oracle |
| GDI 像素级绘制 | 先决策保真级别；像素级 → **真机截图**容差比对（Wine 截图不作数）；否则登记近似+语义重写 |
| decompile-failed | ghidra_out/asm 手工反汇编补规格，或黑盒探针反推 |
| UB 依赖点(wsprintfW 缺参等) | 以调用点语义定应然行为 → divergences.md |

### 4.C.6 Phase 5：硬化收口

- `-Wall -Wextra -Werror`；gcc+clang 双编译；ASan/UBSan 全语料零报告；
- parser 接 libFuzzer/afl++（语料种子=cases/）；崩溃样本转正式用例；
- CI 三道门：编译矩阵 / 黄金语料回归 / 覆盖率阈值；
- 交付 `docs/divergences.md` 终版；旧树移 attic/archive；台账封存进 git 历史。

门禁 G5（发布）：G3 全绿 + 长尾分歧表完备 + 硬化零红灯。

### 4.C.7 成本结构与风险量化

| Phase | 占比 | 单人粗估 |
|---|---|---|
| 0 行为基线 | ~15% | 1-2 周（含真机通道搭建 2-3 天） |
| 1 规格 | ~15% | 2-3 周 |
| 2 架构 | ~5% | 1 周 |
| 3 重写 | ~55% | 2-4 个月 |
| 4/5 硬化 | ~10% | 3-6 周 |

---

## 5. B→C 合并路线图（推荐执行序）

残值率：批0≈100%｜批3≈90%｜批5≈80%（triage→divergences）｜批2≈50%｜批1≈40%｜批4≈30%。
（v3 注：批 1 的 FUN_ 子项已在还原期完成，其成本已沉没于 P4；上表残值率指剩余子项。）

**四处顺序修正**（否则 B 是岔路；前三处沿 v2，第四处 v3 新增）：
1. 冒烟提前：mingw 试编译提到批 1 前（暴露空桩模块，防止给重写区精装修）；
   **mingw 工具链可用性至今未验证（冒烟实验曾中止），是启动任一场景前的第一检查项**；
2. 分诊提前：批 5a 的 TODO 分诊挪到批 1 之前（产出精修区/重写区地图）；
3. 结构体落位：批 3 直接写 `include/domain/`（C 原样继承）；
4. **ls 范围裁决提前**：link_stubs.c 是否纳入 B 精修区（P9 归属）在 B0 内一次裁定，
   避免批 1 执行到一半才发现口径分裂。

周计划与门禁（多代理流水线口径）：

| 周 | 内容 | 门禁 |
|---|---|---|
| W1 | B0 卫生(+ls 裁决) + mingw 冒烟 + 分诊地图 | G-B0：build 绿 + 地图评审 + 警告账面自洽 |
| W2-3 | B1 残余语义化(local/param/DAT，限精修区) ∥ B2 重组 | G-B1/2：附录B 指标达标 |
| W3-4 | B3 结构体(domain/) + B4 去重 | G-B3/4：静态断言+家族合并完成 |
| W4-5 | B5 收容 + README/定位声明 | **G-B：S-B 交付** |
| W5-7 | C-P0 语料+harness+真机通道+golden 录制（WIN 接管自此起，见 §8） | **G0** |
| W7-10 | C-P1 规格 + C-P2 架构 | **G1/G2** |
| W10~ | C-P3 循环 → P4/P5 | **G3 → G5** |

心理陷阱警示（常驻 README）：`风格生产级 ≠ 行为已验证`。B→C 之间隔的是裁判，不是打磨。

---

## 6. 流程纪律（全程有效）

1. 每批/每模块一个 git 提交点；破坏性实验一律开分支；
2. 台账只有两条活页：本文档（方案）+ results/reports（事实）；REVIEW/TASKS 冻结为历史；
3. 子代理提示词末尾必附 tools/subagent_autonomy_clause.md；命名类任务必附 REVIEW §7 证据审计规程；
4. 任何"已完成"声明必须携带附录 B 当次命令输出；
5. link_stubs/crt_shims 单代理独占修改（历史教训：并发改桩造成动荡）；
6. 高峰时段门控沿用 TASKS §4（北京时间 09-12/14-18 不排重活）；
7. **（v3 新增）单一裁判原则**：verdict 只认真机 Windows golden；Wine/QEMU 输出仅作开发参考，
   任何门禁不得引用；
8. **（v3 新增）跨端交接纪律**：git add -A 禁止（扫半成品教训）；bundle/tar 传输后必校 sha256；
   WIN 端仓库必须放英文路径、保持 LF 行尾。

---

## 7. 风险登记册（v3 修订）

| # | 风险 | 触发信号 | 缓解/回退 |
|---|---|---|---|
| R1 | TODO 分诊拖成无底洞 | 分诊两周未收敛 | divergences.md 提供"合法放弃"；UNK 上限 30%，超出强制裁决会 |
| R2 | ~~Wine 噪声污染 diff~~（v2）→ **golden 录制不可复现**（v3：真机环境差异/用户侧扰动） | 同用例双跑不一致 | 用例沙箱目录+前后快照；flaky 隔离池（>10% 停线排查）；QEMU 仅作交叉验证参考 |
| R3 | 台账再度失真 | 口径与 grep 不符 | §6.4 强制附输出；指标脚本随批执行；census 类 JSON 必须注明扫描范围 |
| R4 | mingw 三方对拍受阻 | 一周未通冒烟 | 降级为"新旧两方"(原版 vs 新树)；旧树仅作阅读参考 |
| R5 | B 期间产生完成幻觉 | 有人提议跳过 G0 | README 定位声明 + §5 心理陷阱条款 |
| R6 | 巨型解析器重写超支 | 单模块 >3 周未对拍绿 | Phase4 协议评审：降保真/切分子文法/登记近似 |
| R7 | 关键人不可用 | — | 全部状态在文档+git，无本地知识依赖 |
| R8 | **（v3）WIN 端迁移环境风险**：中文路径/WSL 缺失/CRLF 污染/凭据泄露 | clone 后 build 首跑红；git diff 出现整文件行尾变更；tar 校和不符 | 英文路径 clone；接管前置检查单（§8.3）逐项过；.gitattributes 建议 LF；凭据权限 600 |
| R9 | **（v3）core/ls 双口径分裂**：ls 警告存量未裁决导致门禁数字无法解释 | 任一批 M8 输出出现"未分类警告" | B0 第 8 项一次裁决；known_limits 登记豁免清单 |

---

## 8. WIN 端 DSH 接管点裁定（v3 新增；回应 TASKS §12-S6 / HANDOVER_WIN §2.5 法定职责）

### 8.1 裁决结论

> **接管点 = R-C0：场景 C 的 Phase 0「真机金标准录制」子阶段起，WIN 端 DSH 部分接管；
> 此前的一切（含整个场景 B 与 C-P0 的 harness 开发）留在 Linux 端。**
> 若用户最终只选场景 B：WIN 端全程不参与生产，B 完成、仓库重打包（bundle+tar 刷新）后再议。
> 若用户选场景 C：R-C0 之后形成稳定的「Linux 造脑、WIN 采证」双端分工，直至项目收口。

### 8.2 裁决依据：剩余阶段 × 环境依赖矩阵

| 阶段 | gcc/mingw 工具链 | bash/python 管线 | Wine | 真机 Windows | 裁定 |
|---|---|---|---|---|---|
| B0-B5 全部批次 | Linux gcc 门禁已实证 95 OK | 六步管线/改名器全在 Linux 验证 | 不需要 | 不需要（B 无行为验证） | **Linux** |
| C-P0 harness 开发（runners/diff/report/masks） | 否 | 是 | 否 | 否 | **Linux** |
| C-P0 mingw 旧树试编译 | 是（Linux 交叉） | 是 | 可选 | 否 | **Linux** |
| **C-P0 golden 录制** | 否 | 弱（投递/回捞脚本 WSL 可跑） | **禁入 verdict**（多进程+窗口消息过滤，无人值守不可靠） | ★必须 | **WIN ← 接管点 R-C0** |
| C-P1/P2 规格/架构 | 否（静态分析+文档） | 弱 | 否 | 否 | 默认 Linux（上下文连续性；WIN 端可并行做语料预演） |
| C-P3 重写循环对拍 | Linux mingw 交叉 | 是 | 可选 | 周期性采证 | Linux 为主；采证复用 R-C0 通道 |
| C-P4 GDI 保真比对 | — | — | 截图不可信 | ★必须 | WIN（走同一通道） |
| C-P5 CI/硬化 | CI 宿主默认 Linux | 是 | — | 可选回归 | Linux |

理由浓缩：
1. **依赖度**：B 场景与 C 的全部"造物"环节强绑定 Linux 上已验证的 gcc/bash/python 管线；
   WIN 端（即便有 WSL）需要重新证明整条链，纯属负资产；
2. **能力边界**：WIN 端 DSH 的价值恰好卡在"物理邻近真机"——golden 录制要求真 Windows，
   这是 Linux 端无论 Wine 还是 QEMU 都无法合规替代的环节（平台策略已定死 verdict 口径）；
3. **衔接方式**：真机录制天然需要"有人在场监督弹窗用例"，WIN 端 DSH + 用户同机协作是
   最短回路；diff/verdict 计算回 Linux 做，保证单一裁判（§6.7）；
4. **迁移成本最小化**：接管点推迟到 C-P0，意味着迁移前 B0 卫生包已做完、仓库体积与垃圾文件
   最小、rename/audit 账面已冻结——bundle/tar 一次性带走干净状态。现有交接包
   （HANDOVER_WIN.md + dsh_config tar + p4close bundle）已备齐，随时可启用。

### 8.3 WIN 端接管前置检查单（R-C0 启动前逐项过）

1. [ ] WSL2（首选）或 git-bash 可用；python3 ≥3.8；GNU coreutils 在 PATH；
2. [ ] 仓库 clone 到**纯英文路径**（当前仓库名含中文"PECMD反编译"，部分工具链敏感）；
3. [ ] `git bundle verify` + clone 后首跑 `bash build.sh`（95 OK）与完整链接门（exit 0）双绿；
4. [ ] dsh_config tar 包 sha256 校验通过；`.credentials.yaml` 存放权限收紧（600）；
5. [ ] 行尾核查：`git config core.autocrlf false`；抽查 .c 文件无 CRLF；
6. [ ] 真机试录一条冒烟用例（ENVI+WRITE），产物回 Linux 跑通 diff 闭环；
7. [ ] gen_tasks.py 的 `/tmp/opencode` 易失依赖固化（输入 JSON 入库 tools/）（工具债，迁移前处理）。

### 8.4 双端衔接协议（R-C0 生效后）

- Linux 端产出：`corpus/cases/*`（manifest+INI+epilogue）+ 回捞校验和清单 → 打包投递；
- WIN 端执行三步：**部署**（解包至 C:\pectest\<case>\）→ **运行**（gui=false 用例可无人值守批量跑；
  gui=true 用例必须用户在场点击监督）→ **回捞**（收集 out/ 目录 + reg 导出，按 manifest 校验完整性）；
- Linux 端消费：diff_case/report/verdict 全部在 Linux 计算；WIN 端不产生任何"结论性"判断；
- 异常处置：真机运行超时/崩溃/弹窗卡死 → WIN 端按 manifest 的 timeout_s 杀进程、标记
  `flaky_pool` 候选并回传现场截图/转储，不得擅自改用例。

---

## 附录 A：Wine / PE 运行手册（2026-08 实测版；v3 起定位=可选开发冒烟，永不进 verdict）

环境：wine-wow64-11.0(nix)。无 DISPLAY 时 `wineboot -i` 于 rundll32 InstallHinfSection
阶段死锁（ntsync 睡眠，注册表停滞 ~1.8MB）。已排除 /tmp 属主、32 位前缀误判（勿预置空 .reg）。

**手动初始化序列（推荐在有显示环境的终端执行）**：
```bash
pkill -9 -x wineserver; pkill -9 -x wineboot.exe; pkill -9 -x winedevice.exe; pkill -9 -x rundll32.exe
export WINEPREFIX=$HOME/pecmd-wine-prefix WINEARCH=win64
rm -rf "$WINEPREFIX"; mkdir -p "$WINEPREFIX"; wineboot -i
# 卡死>5min 则 Ctrl+C 继续（前缀已铺95%）：
wine reg add "HKCU\\Software\\Wine\\Drivers" /v Graphics /d null /f   # 无头保险
# 若上条也挂：date +%s > "$WINEPREFIX/.update-timestamp" 后重试

mkdir -p "$WINEPREFIX/drive_c/pectest"
cp "PECMD原始.EXE" "$WINEPREFIX/drive_c/pectest/PECMD.EXE"
cat > "$WINEPREFIX/drive_c/pectest/smoke.ini" <<'EOF'
ENVI V1=Hello
ENVI V2=Wine
WRITE C:\pectest\out.txt,V1=%V1%_V2=%V2%
EOF
cd "$WINEPREFIX/drive_c/pectest"
timeout 60 wine PECMD.EXE LOAD 'C:\pectest\smoke.ini'; echo exit=$?
cat out.txt 2>/dev/null || echo "(未产生 out.txt)"
```
判读：out.txt=`V1=Hello_V2=Wine` ⇒ 冒烟链路通（解析→展开→文件IO）。
备选调用形式：`wine PECMD.EXE 'C:\pectest\smoke.ini'`。
CI 替代路线：Docker 化 wine 镜像。**再次强调：以上一切仅用于开发期手感与快速回归，
golden/verdict 一律以真机 Windows 录制为准（§8）。**

## 附录 B：可复查指标命令（M1-M10；v3 扩充并修正 locale 陷阱）

```bash
cd refactored   # 或 pecmd-next
# 注意：警告类 grep 依赖英文报错文本，务必 LC_ALL=C
M1: grep -rPno 'FUN_14[0-9a-fA-F]+' --include='*.c' . | wc -l        # v3 目标=仅剩豁免清单内命中
M2: grep -rPno '\blocal_[0-9a-f]{2,}\b' --include='*.c' . | wc -l    # 目标: 精修区内≈0
M3: grep -rPno '\bparam_\d+\b' --include='*.c' . | wc -l             # 目标: 精修区内≈0
M4: grep -rPno '\bDAT_14|\bPTR_(FUN|s)?_' --include='*.c' --include='*.h' . | wc -l
M5: grep -rc 'TODO(verify' --include='*.c' . | awk -F: '{s+=$2} END{print s}'
M6: bash build.sh ; echo $?                 # 无参即全量 core_*.c（95d3ea5 起）; 全绿=0
M7: awk '/#endif/{f=1;next} f&&NF' include/pecmd_defs.h | wc -l      # guard外=0
M8: for f in core_*.c link_stubs.c; do LC_ALL=C gcc -std=c99 -Wall -Wextra \
      -Wno-implicit-function-declaration -Iinclude -fsyntax-only "$f"; done 2>&1 \
      | grep -c 'warning:'                    # 目标=行为类登记数+明示豁免数
    # 分范围口径: 把文件列表换成 core_*.c 即 core 口径（对表 §2.1a）
M9: python3 -c "import json;print(len(json.load(open('tools/rename_map.json'))))"
M10: git ls-files | grep -vcE '^core_|^include/|^tools/|^docs/|^attic/|\.md$|\.sh$|^\.gitignore$|^LICENSE'
    # 非产物追踪文件数（P5 进度）; 目标≈0
# 伪影计数（v3 口径，显式 token 正则）:
MA: grep -rPno 'CONCAT[0-9]*\(|\bunaff_|\bextraout_|in_stack_ffff|\blocal_res\b' \
      --include='*.c' --include='*.h' . | wc -l
```

## 附录 C：关键模板索引

| 模板 | 位置 |
|---|---|
| 用例 manifest.json（含 gui 字段） | §4.C.1-0.2 |
| epilogue.pecmd 生成规范 | §4.C.1-0.3 |
| verdict.json 契约 | §4.C.1-0.5 |
| KNOWN-LIMIT 标签格式 | §3.B.5-5b |
| provenance.map TSV 格式 | §3.B.5-5c |
| 模块语义卡 | §4.C.2-3 |
| DoD 四件套核对单 | §4.C.4 |
| WIN 接管前置检查单 / 双端衔接协议 | §8.3 / §8.4 |
| B/C 评估输入材料索引 | docs/bc_evaluation_inputs.md |

（完）


---

## 附录 D：B 执行状态登记（2026-08-24 会话，方案书本体不变）

| 批次 | 状态 | 证据 |
|---|---|---|
| B0 | ✅ tag `B0` (f79bec0) | P1-P11 全部落地; link_stubs 四桶拆分; crt_shims 202 块; M7=0 |
| 前置件 | ✅ | mingw 工具链确认 (GCC 15.2.0); TODO 分诊 423 条 (STATIC 287/BLACKBOX 46/UNK 90) |
| B1 | 🟨 ~8% | 机械清扫 36 条; 配方文档 docs/B1_pipeline_status.md; 主体 local/param/DAT_ 待续 |
| B2 | ✅ | src/ 八域重组 (95 TU) + 根/模块 README; build.sh find 注入 |
| B3 | 🟨 试点 | domain 头 (VarNode/Script/Task 断言全过 + WndObj 访问器); core_thread 10 处; OBJ_* 235 处推广未做 |
| B4 | 🟨 部分 | clang-format 全库; P8; @0xFUN_×147; 家族合并未做 |
| B5 | 🟨 部分 | 5a 分诊✓ 5b 入册136/423 5c provenance 1594 5d 定位声明✓; STATIC 287 待销号 |
| 保真度 | ✅ | 分诊 7 处静态可证问题: 4 修 (BOM/NTP sockaddr/消息块偏移/CONCAT71) 2 误报 1 注释升级 (66c3d17) |
| WIN 交接 | ✅ | §8.3 检查单 Linux 侧全就绪; harness 语料 30 条; bundle @ a9f0c32; HANDOVER_WIN.md 活页 |

**门禁 G-B 口径**：B 未达 100% (B1/B3 推广/B4 家族合并/B5b 收尾登记豁免，
见 docs/known_limits.md 附录「B 场景剩余工作登记」)。C 场景不依赖这些残项。
