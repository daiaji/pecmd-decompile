# PRODUCTION_ROADMAP.md — PECMD 重构产线总体方案（场景 B / C 完整版）

> 本文是后续工作的**总方案书**：覆盖场景定义、现状基线、场景 B 全部批次任务书、
> 场景 C 全部阶段任务书、合并执行时间表、流程纪律、风险登记册与全部模板。
> 前置文档：TASKS.md / REVIEW.md（还原阶段台账，本文大量引用其编号）。
>
> 版本：v2（完整版）｜基线日期：2026-08｜所有计数均为当次 grep 实测，复验命令见附录 B。

---

## 0. 文档地图与使用方法

| 章节 | 内容 | 谁在什么时候读 |
|---|---|---|
| §1 | 场景决策框架 | 立项/立项变更时 |
| §2 | 现状基线（量化+文件映射） | 一切批次开工前 |
| §3 | 场景 B 任务书（批 0-5） | 执行 B 时逐批对照 |
| §4 | 场景 C 任务书（Phase 0-5） | 执行 C 时逐阶段对照 |
| §5 | B→C 合并时间表与门禁 | 排期与验收 |
| §6 | 流程纪律（git/台账/代理） | 全程 |
| §7 | 风险登记册 | 全程，每门禁复审一次 |
| 附录 A/B/C | 运行手册 / 指标命令 / 模板 | 随用随查 |

使用规则：任何批次开工前先抄录该批"验收"小节为 PR 描述；完工时附附录 B 当次输出。
禁止口头宣称清零/完成。

---

## 1. 决策框架：三个场景

| 场景 | 目标定义 | 正确性承诺 | 验收口径 | 成本量级 |
|---|---|---|---|---|
| S0 维持现状 | 反编译参考库 | 无（仅可编译可链接） | build/link 绿（已达成） | 已沉没 |
| **S-B 风格生产化** | 代码外观达生产规范；组织、命名、类型、结构体化 | **无行为验证**；一切改动语义保持 | §3 各批验收 + 附录B 指标 | 单人 4-6 周；多代理流水线 2-3 周 |
| **S-C 生产行为等价** | 可维护、可继续开发的生产代码 | 黄金语料对拍绿 + 分歧登记完备 | §4 各 Phase 门禁 | 数月（详见 §4.7） |

选择规则：
- 只做参考/存档 → 停在 S0，只做 §3 批 0 卫生包（半天）；
- 要门面（演示/交接）→ S-B；
- 要真货（继续开发/移植）→ S-C；
- **按 S-B 的预算期望 S-C 的结果是唯一错误选项**（阶段 7 的停滞已证明此路不通）。

---

## 2. 现状基线

### 2.1 反编译残留量化（core_*.c 全量实测）

| 指标 | 计数 | 复验命令（附录B 编号）|
|---|---|---|
| `local_xx` Ghidra 局部变量 | 14,908 | M2 |
| `param_N` 形参 | 7,674 | M3 |
| `FUN_140xxx` 引用 | 7,521 | M1 |
| CONCAT/unaff/extraout/in_stack/local_res 伪影 | 6,916 | — |
| `DAT_/PTR_` 数据符号引用 | 1,012 | M4 |
| TODO(verify)/TODO(restore) | ~400 | M5 |
| PECMD_ 可读名引用（对照） | 7,854 | — |

### 2.2 结构性问题清单（= B 批 0 输入，逐条可勾销）

| # | 问题 | 位置 | 处置批次 |
|---|---|---|---|
| P1 | `#endif`(L153) 后 ~500 行 extern 在 guard 外 | include/pecmd_defs.h | B0 |
| P2 | 双 Win32 类型体系互相矛盾（HANDLE/LPARAM 定义不一致）；HFONT(L30/38)、SC_HANDLE(L54/859) 重复 typedef；FindFirstVolumeW/Next 签名错误靠强转绕行 | win32_stub.h vs link_stubs.c | B0 |
| P3 | link_stubs.c 16,255 行三职责混装：Win32 API 桩 / FUN_ 业务缺口桩 / PECMD_* 别名真实转发 | link_stubs.c | B0 |
| P4 | ~250 个 CRT 定义在业务源码内；strtol/perror/raise/iswctype/wcstol/wcstoul 与 libc 同名强定义；大量错误签名 `uint64_t f(void)` | core_b9_remaining.c | B0（隔离）/C（真删） |
| P5 | ~20 个会话工作文件 + tools/tmp_p4_batch/ + ghidra_out/ 在源码树 | 仓库根/tools | B0 |
| P6 | build.sh 死配置 third_party/giflib | build.sh | B0 |
| P7 | 文档失真：REVIEW 重复合并（§21/§24）、TASKS"param_* 清零"等口径不实 | 两 md | B0 |
| P8 | 过期标记示例：core_string.c:39 对已实现函数标 UNIMPLEMENTED | 各处 | B4 |

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

> 注：b3r_* 批次文件内部主题混杂（同文件含 UI 与设备函数），批 2 允许"文件整体归入主导语义目录"，
> 函数级跨文件搬移留给 C 阶段随重写自然完成，避免 B 内做高风险手术。

### 2.4 可复用资产盘点

| 资产 | 用途 |
|---|---|
| rename_map.json(~1135 符号)/FUNC_NAMES.md | 批 1 改名底账 |
| apply_rename.py / apply_rename2.py | 批 1 执行器（含占用检查） |
| reference_scan.py / gen_tasks.py / classify_funcs.py | 指标与分类 |
| pe_data_extract.py / pe_registry.json(1082 真值) | C-Phase1 数据初值规格 |
| ghidra_out/(asm+伪码)、big_funcs/、decompiled.c | 硬骨头协议的原始依据 |
| tools/subagent_autonomy_clause.md | 子代理提示词必备附件 |

---

## 3. 场景 B 任务书（语义保持的风格生产化）

### 3.B 总则

- **红线**：一切改动语义保持。兜底手段 = 编译器类型检查 + `_Static_assert(offsetof)` +
  每批 build/link 双绿；**没有测试，所以凡编译器管不住的改动都在禁区（§3.B.8）**；
- 节奏：每批一个 git 提交点（tag `B<n>`），PR 描述预拷贝该批"验收"节；
- 进度看板只用附录 B 命令输出，禁止口头口径。

### 3.B.0 批 0：卫生与结构修复（1-2 天，零语义风险）

任务分解（对应 §2.2 问题编号）：

1. **P1 头文件修复**：将 pecmd_defs.h 的 `#endif` 移到文件末尾；或（推荐）拆两文件：
   `pecmd_defs.h`（类型+函数声明）与 `pecmd_globals.h`（全部 extern g_），
   后者由前者 include；所有 `#include "pecmd_defs.h"` 处不改（兼容 include 链）。
   验收：附录B M7 抽查为空；build 绿。
2. **P5 仓库清洁**：
   ```bash
   # .gitignore 追加
   printf 'tmp_*\nbatch28*\ngh?.txt\ng[ABCD].txt\nout[ABCD].txt\nwf28.js\ntools/tmp_p4_batch/\n' >> .gitignore
   mkdir attic && git mv ghidra_out tools/tmp_p4_batch attic/reference/ 2>/dev/null || mv ... attic/reference/
   ```
   会话工作文件移 `attic/session_2026-08/`。验收：仓库根只剩 *.md/*.sh/include/core/link/attic。
3. **P4 CRT 隔离**：新建 `crt_shims.c`；将 core_b9_remaining.c 中以下三类整体迁移：
   (a) MSVC CRT 内部机制（_onexit/_lock/_purecall/_woutput_l/_input_l/_XcptFilter/_mtinit/doexit/
   __tmainCRTStartup/entry/SEH/CxxEH 族/_cfltcvt 族/时区族/security_cookie 族…）；
   (b) 与 libc 同名者（strtol/wcstol/wcstoul/iswctype/perror/raise/wctomb_s…）；
   (c) 数学空桩（asin/acos/ceil/log10 等 double(double) 桩）。
   文件头横幅：`/* crt_shims.c — 仅链接验证用；同名符号会遮蔽系统库；部署链接前必须移除（见场景C） */`。
   core_b9_remaining.c 相应改为 extern 声明。验收：build/link 绿；`grep -c 'uint64_t (void)'` 型桩集中单文件。
4. **P3 link_stubs 拆分**（纯机械切分，不改内容）：
   - 分类规则：符号名为 Win32 API 形（win32_stub.h 出现过）→ `win32_api_stubs.c`；
     名为 FUN_/thunk_/DAT_/PTR_/数据 → `unimplemented_stubs.c`；
     名为 PECMD_* 且函数体是转发调用真实体 → 迁往该真实体所在 TU 文件尾，标注 `/* public alias */`；
   - 拆分后删除原 link_stubs.c，build.sh 调整为 `core_*.c win32_api_stubs.c unimplemented_stubs.c`。
   验收：link 绿；`wc -l` 三文件合计≈原文件。
5. **P2 类型统一**：win32_stub.h 删 HFONT/SC_HANDLE 重复 typedef；修 FindFirstVolumeW/
   FindNextVolumeW 为真实签名 `(LPWSTR,DWORD)` 并同步 core_b3*/b9 的显式函数指针转换调用点
   （删转换，直接调用；编译器验证参数个数）；link_stubs/crt_shims 的本地 typedef 收敛为
   `#include "win32_stub_lite.h"`（新建：仅基础类型，无函数声明，供桩 TU 使用）。
6. **P6/P7**：build.sh 删 third_party 分支；REVIEW 合并重复合并章节并在文首加
   "历史台账，口径以 PRODUCTION_ROADMAP §2.1 实测为准"声明。
7. **提交**：tag `B0`。

验收（批级）：build.sh core_*.c 全绿；完整链接 exit 0；M7 空；attic 外无会话文件。

### 3.B.1 批 1：机械改名清零（3-7 天，脚本化）

前置输入：B0 完成；TODO 分诊结果（§3.B.5 提前执行的部分）产出"精修区文件清单"
（改名预算只花在精修区；重写区文件跳过深改名，仅保留编译所需）。

四条流水线（均可并行，互不冲突时串行提交更稳）：

1. **FUN_→PECMD_**：
   - 已在 rename_map.json 的 1135 个：`apply_rename2.py --map rename_map.json` 直接跑；
   - 未命名的：沿用既有"子代理提名批"流程（25-30 个/批，提示词必附 subagent_autonomy_clause.md；
     命名须过 §7 命名证据审计：grep decompiled.c 全部引用点取证）；
     提名 JSON → 人工 10 分钟扫一遍 → apply → build；
   - 冲突处理：目标名已被占用时后缀 `_2`禁止，改用语义变体或挂起人工裁决清单。
2. **param_N → 语义形参**：逐文件子代理批（每文件一代理）；不确定形参用保守字典：
   `p/pp=游标, buf=缓冲, cnt/count=计数, flags=标志, ctx/hObj=对象指针, hXxx=句柄`；
   禁止臆造强语义名（如把 flags 命名 isDeleted）。
3. **local_xx → 语义局部**：高频文件优先（core_b2e/b2d/b1/b3_remaining/b8m…）；
   规则同上；(void)死变量若确认无用直接删除（比 `(void)x;` 更干净，编译器兜底）。
4. **DAT_/PTR_ → g_ 迁移**：按 REVIEW §4a-l 既定剧本；类型判定存疑者保持 void*+KNOWN-LIMIT 注释。

验收：M1 代码命中≈0（注释除外）；M2/M3 在精修区文件内≈0；build/link 绿；rename_map 同步更新。

### 3.B.2 批 2：按功能重组文件（2-3 天，纯移动）

- 按 §2.3 映射表执行 git mv；build.sh 通配不变（core_*.c 改为 `src/**/*.c` 或保持平铺软链皆可，
  推荐显式 find 注入）；
- 新增根 README.md（项目是什么/Wine 冒烟现状/构建方法/目录地图/定位声明）与
  `src/<module>/README.md` 每模块一句话；
- 不做函数级跨文件搬移（理由见 §2.3 注）。

验收：build/link 绿；每个新目录 README 就位。

### 3.B.3 批 3：Top 对象结构体化（1-2 周，判断密集）

范围限定：**仅布局已被 REVIEW 登记的对象**：

| 结构体 | 字段依据（REVIEW §3/§5.x） |
|---|---|
| PECMD_WndObj（窗口/控件通用族） | +0x20 hwnd, +0x38 brush/GDI, +0x40 color, +0x68 mode/font, +0x70 wParam, +0xd0 link, +0xe0 textColor, +0xe8 flags/bitmap, +0xf0 subwnd, +0x200 dbl, +0x3b4 flags |
| PECMD_VarNode | +0x00 name, +0x08 value, +0x10 rsv, +0x18 cap(bit7:6=0xc0 固定容量) |
| PECMD_Script(0xe0) | +0x00 varArray, +0x08 count, +0x38 parentTable, +0x50 tpl, +0xda/+0xd/+0x45/+0x48 分隔符组, +0xf/+0x10 参数区 |
| PECMD_Task | +0x00 refcnt, +0x08 refs, +0x18 msg, +0x20 wParam, +0x28 lParam, +0x30 flags, +0x38 timeout, +0x40 hwnd, +0x48 cb, +0x50 ctx |

实施步骤（每结构体一轮）：
1. 在 `include/domain/<name>.h` 定义 struct + 每字段 `_Static_assert(offsetof(...)==…)`；
2. 选定一个使用密度最高的文件做试点替换：`*(HWND*)(obj+0x20)` → `obj->hwnd`；
   替换脚本按"偏移字面量+强转"模式匹配，逐个 diff 人工过目；
3. build 绿后推广至同对象其余文件；pecmd_offsets.h 对应宏标记 deprecated 并逐步清引用；
4. 未登记布局的对象**不碰**（发现新布局→先登记 REVIEW 再纳入下一轮）。

验收：试点对象的宏引用清零；静态断言全数通过；build/link 绿。

### 3.B.4 批 4：去重与格式统一（3-5 天）

1. 家族合并程序（每族一轮）：DestroyObjectWithFree B/C/D…J、SendWindowMessage202/204、
   InitXxxObjCore 族、ReleaseObjectA…L 族——
   步骤：diff 各成员确认仅"被调清理函数+常量"不同 → 抽公共实现（参数化差异点）→
   原名保留为一行薄包装 → 调用点不动 → build 绿 → 下一族；
2. 删过期标记（UNIMPLEMENTED-on-implemented 类，grep 'UNIMPLEMENTED' 全量复核）；
3. 统一文件头模板（来源段/依赖段/状态行）与注释语言政策（中文为主，标识符英文）；
4. 提交 `.clang-format`（基于 LLVM，列宽 100，缩进 4）并全库格式化一次（独立提交便于 review）。

### 3.B.5 批 5：TODO 收容与门面收尾（2-3 天；其中"分诊"部分提前至批 1 前执行）

**5a. TODO 分诊（提前件，产出投资地图）**：对 ~400 个 TODO(verify) 逐条打标签：
- `[RESOLVABLE-STATIC]`：decompiled.c/asm 可定案 → 列清单排期；
- `[RESOLVABLE-BLACKBOX]`：构造输入观察原版可定案 → 列清单（供 C-Phase0 语料设计复用）；
- `[UNK]`：真不可知 → 转 KNOWN-LIMIT。
产出：`docs/triage_map.md`（精修区/重写区文件清单即由此派生）。

**5b. 标签改造**：非 RESOLVED 者统一改写：
```c
/* KNOWN-LIMIT @0x140066978(wsprintfW缺参): 原反编译变参被优化丢弃, 按""补位;
   证据不足待黑盒探测. triage=UNK #KL-0042 */
```
汇总 `docs/known_limits.md`（表格：id/地址/位置/现象/影响面/triage）。

**5c. provenance.sidecar**：生成 `docs/provenance.map`，TSV 格式
`新符号 \t 原@0x地址 \t 原Ghidra名 \t 来源文件`；随后批量删除代码内的 `@0x` 注释噪声
（保留 KNOWN-LIMIT 内地址）。生成脚本入库 tools/gen_provenance.py。

**5d. 定位声明**：README 首屏固定一行：
`> 本库风格达生产规范，但行为未经系统对拍验证（见 PRODUCTION_ROADMAP 场景B边界）。`

### 3.B.6 批级验收与回滚

- 每批 PR 必附附录 B 输出前后对照；
- 任一批 build/link 破坏且 30 分钟内无法定位 → `git revert` 整批，缩小粒度重做；
- 批间允许穿插（批 1 流水线与批 3 不同文件时可并行），但 tag 顺序固定 B0<B1<B2<B3<B4<B5。

### 3.B.7 工作量与产出物清单

单人 4-6 周 / 多代理流水线 2-3 周。
产出：整洁源树（src/ 布局）、domain 结构体头、known_limits.md、triage_map.md、
provenance.map、README、风格指南、clang-format 配置、crt_shims/win32_api/unimplemented 三桩文件。

### 3.B.8 明确禁区（越界即升级 S-C）

去 goto / 控制流重写；巨型函数拆分主体；解析器文法化重写；全局标志收敛 context 结构体；
错误处理策略改造；CRT 同名桩真删改链系统库；任何"顺手修逻辑"的行为。

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
  backends/wine.sh qemu_win.sh qemu_winpe.sh
  runners/run_case.sh collect.py diff_case.py report.py
  results/<backend>/<case>/…
  golden/<backend>/<case>/…      # 原版录制
```

**0.2 用例 manifest.json**
```json
{ "id":"002_envi_scope", "cmds":["ENVI"], "vars":["A","B","OUT"],
  "timeout_s":60, "flaky":false, "notes":"" }
```

**0.3 尾声规范（runner 按 vars 自动生成 epilogue.pecmd）**
```ini
ENVI T_CASE=<id>
WRITE C:\pectest\out\vars.txt,CASE=<id>
WRITE -a C:\pectest\out\vars.txt,V_A=%A%
WRITE -a C:\pectest\out\vars.txt,V_B=%B%
ENVI T_DONE=OK
WRITE C:\pectest\out\done.txt,%T_DONE%
```
（WRITE 追加旗标以实测为准；若不支持 -a 则 runner 生成单行合并版。）

**0.4 四维采集**
| 维度 | 采集 | 实现 |
|---|---|---|
| stdout/exit | 进程直捕 | runner tee + $? |
| 变量表 | out/vars.txt | 见 0.3 |
| 文件系统 | 用例沙箱目录树 | `find -type f -exec sha256sum` 前后对比 |
| 注册表 | 涉事键导出 | `reg export` 前后（wine: `wine reg export`）|

**0.5 diff_case 输出契约（results/<case>/verdict.json）**
```json
{ "case":"002", "backend":"wine",
  "stdout":{"same":true}, "exit":{"exp":0,"got":0},
  "vars":{"same":false,"diff":[{"k":"V_B","exp":"Wine","got":"wine"}]},
  "fs":{"same":true,"added":[],"removed":[],"changed":[]},
  "reg":{"same":true}, "verdict":"FAIL", "flaky_pool":false }
```
掩码规则集中在 `masks.conf`（时间戳/路径/随机值正则）。

**0.6 后端实现要点**
- wine.sh：每例独立 `WINEPREFIX=$HOME/.wine-<case>`（首例建模板后 tar 快照，秒级解包复位）；
  无显示环境用 null 图形驱动（注册表键 HKCU\Software\Wine\Drivers Graphics=null，
  经 `wine reg add` 设置或模板前缀内置）；
- qemu_win.sh：qcow2 backing 秒级还原；virtio-fs/SMB 投递用例；QMP 管控启停；产物回捞后跑同一 diff；
- qemu_winpe.sh：WinPE ISO 注入 PECMD.INI autorun；串口日志收集；仅发版冒烟集。

**0.7 三方对拍（本方案的杠杆）**
```bash
# 旧树编 PE（预期需 2-3 天调试，已知坑见下）
x86_64-w64-mingw32-gcc -std=c99 -w -Wno-implicit-function-declaration \
  -Iinclude src_core_files... win32_api_stubs.c unimplemented_stubs.c -o pecmd_old.exe
```
已知坑与对策：
(a) CRT 同名冲突（strtol/perror/raise 与 msvcrt）→ 用 `objcopy --weaken-symbol=strtol …`
    批量弱化 crt_shims.obj 中同名符号，让链接器回落系统库；
(b) 入口点：旧树自带 entry/__tmainCRTStartup 桩与 mingw 启动例程冲突 →
    `-nostartfiles` 自管入口，或链接期 `-Wl,--entry=mainW` 直指定；
(c) 未实现 FUN_ 桩运行期暴露 → verdict 差异自动生成"补桩优先级清单"。
三方矩阵：`golden(wine)=原版EXE`、`old=pecmd_old.exe`、`new=pecmd_next.exe` 同语料互 diff。

**0.8 语料扩充顺序**：核心 30 命令 → 控制流（IFEX/FIND/LOOP/SUB/TEAM）→ 设备/服务长尾。
每命令至少：正例、边界例、错误输入例。

**0.9 验收（Phase 0 门禁 G0）**
- run_case/diff/report 三工具闭环，一条命令出报告；
- ≥30 核心命令用例有 golden；flaky 隔离清单建立；
- （可选达成即加分）旧树 exe 在 Wine 下跑通 ENVI/FILE/WRITE 冒烟。

### 4.C.2 Phase 1：语义规格恢复（2-3 周）

1. 文法抽取：素材=core_execline(展开器族)/core_strbld(6槽构建器)/core_srparse(分隔符配置)/
   REVIEW §5.3-5.4；产出 `docs/spec/grammar.ebnf` + 展开优先级表；
2. 结构体定稿：继承 B3 产物 + 补齐消息映射表(0x28/项)、内存块头(-8 cap/-4 magic)；
3. 模块语义卡 `docs/spec/<module>.md` 模板：
   `职责 / 输入 / 输出 / 副作用 / 错误行为 / 关联命令 / KNOWN-LIMIT 引用 / 开放问题`；
4. TODO 终审：triage_map 的 RESOLVABLE 两类清账（STATIC 走 asm/decompiled；
   BLACKBOX 走 0.8 语料探针）；UNK 全部落 divergences.md。
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
- 上下文治理：`PECMD_Runtime`（进程级）/`PECMD_Session`（脚本会话级）两级结构收容全局态；
- 错误策略：`typedef enum PECMD_ERR {...}` + 传播约定 + UI policy 回调（交互弹窗/静默日志）；
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
DoD 四件套：**规格✓ 实现✓ 测试✓ 对拍绿✓**（缺一不算完成）。
旧树用法：小包装/字符串族可"照抄式重写"；解析器/UI 一律新写，旧文件仅作输出对照器。

门禁 G3：核心 30 命令对拍全绿（含变量表/文件系统/注册表三维）。

### 4.C.5 Phase 4：硬骨头专项

| 类型 | 协议 |
|---|---|
| 巨型解析器(29KB/20KB 级) | 禁逐行翻；按 grammar.ebnf 新写；模糊怪异脚本差分；旧函数仅当 oracle |
| GDI 像素级绘制 | 先决策保真级别；像素级→Wine 截图容差比对；否则登记近似+语义重写 |
| decompile-failed | ghidra_out/asm 手工反汇编补规格，或黑盒探针反推 |
| UB 依赖点(wsprintfW 缺参等) | 以调用点语义定应然行为 → divergences.md |

### 4.C.6 Phase 5：硬化收口

- `-Wall -Wextra -Werror`；gcc+clang 双编译；ASan/UBSan 全语料零报告；
- parser 接 libFuzzer/afl++（语料种子=cases/）；崩溃样本转正式用例；
- CI 三道门：编译矩阵 / 黄金语料回归 / 覆盖率阈值；
- 交付 `docs/divergences.md` 终版；旧树移 attic/archive 仓库；台账封存进 git 历史。

门禁 G5（发布）：G3 全绿 + 长尾分歧表完备 + 硬化零红灯。

### 4.C.7 成本结构与风险量化

| Phase | 占比 | 单人粗估 |
|---|---|---|
| 0 行为基线 | ~15% | 1-2 周 |
| 1 规格 | ~15% | 2-3 周 |
| 2 架构 | ~5% | 1 周 |
| 3 重写 | ~55% | 2-4 个月 |
| 4/5 硬化 | ~10% | 3-6 周 |

---

## 5. B→C 合并路线图（推荐执行序）

残值率：批0≈100%｜批3≈90%｜批5≈80%（triage→divergences）｜批2≈50%｜批1≈40%｜批4≈30%。

**三处顺序修正**（否则 B 是岔路）：
1. 冒烟提前：mingw/Wine 试编译提到批 1 前（暴露空桩模块，防止给重写区精装修）；
2. 分诊提前：批 5a 的 TODO 分诊挪到批 1 之前（产出精修区/重写区地图）；
3. 结构体落位：批 3 直接写 `include/domain/`（C 原样继承）。

周计划与门禁：

| 周 | 内容 | 门禁 |
|---|---|---|
| W1 | B0 卫生 + 冒烟试编译 + 分诊地图 | G-B0：build 绿 + 地图评审 |
| W2-3 | B1 改名(限精修区) + B2 重组 | G-B1/2：附录B 指标达标 |
| W3-4 | B3 结构体(domain/) + B4 去重 | G-B3/4：静态断言+家族合并完成 |
| W4-5 | B5 收容 + README/定位声明 | **G-B：S-B 交付** |
| W5-7 | C-P0 语料+三后端+三方对拍 | **G0** |
| W7-10 | C-P1 规格 + C-P2 架构 | **G1/G2** |
| W10~ | C-P3 循环 → P4/P5 | **G3 → G5** |

心理陷阱警示（常驻 README）：`风格生产级 ≠ 行为已验证`。B→C 之间隔的是裁判，不是打磨。

---

## 6. 流程纪律（全程有效）

1. 每批/每模块一个 git 提交点；破坏性实验一律开分支；
2. 台账只有两条活页：本文档（方案）+ results/reports（事实）；REVIEW/TASKS 冻结为历史；
3. 子代理提示词末尾必附 tools/subagent_autonomy_clause.md；命名类任务必附
   REVIEW §7 证据审计规程；
4. 任何"已完成"声明必须携带附录 B 当次命令输出；
5. link_stubs/crt_shims 单代理独占修改（历史教训：并发改桩造成动荡）；
6. 高峰时段门控沿用 TASKS §4（北京时间 09-12/14-18 不排重活）。

---

## 7. 风险登记册

| # | 风险 | 触发信号 | 缓解/回退 |
|---|---|---|---|
| R1 | TODO 分诊拖成无底洞 | 分诊两周未收敛 | divergences.md 提供"合法放弃"；UNK 上限 30%，超出强制裁决会 |
| R2 | Wine 噪声污染 diff | 同用例双跑不一致 | 夜间 QEMU-Windows 校准层；flaky 隔离池（占比>10% 即停线排查） |
| R3 | 台账再度失真 | 口径与 grep 不符 | §6.4 强制附输出；指标脚本进 CI |
| R4 | mingw 三方对拍受阻 | 一周未通冒烟 | 降级为"新旧两方"(原版vs新树)；旧树仅作阅读参考 |
| R5 | B 期间产生完成幻觉 | 有人提议跳过 G0 | README 定位声明 + §5 心理陷阱条款 |
| R6 | 巨型解析器重写超支 | 单模块 >3 周未对拍绿 | 触发 Phase4 协议评审：降保真/切分子文法/登记近似 |
| R7 | 关键人不可用 | — | 全部状态在文档+git，无本地知识依赖 |

---

## 附录 A：Wine / PE 运行手册（2026-08 实测版）

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
判读：out.txt=`V1=Hello_V2=Wine` ⇒ 全链路通（解析→展开→文件IO），即可开始录 golden。
备选调用形式：`wine PECMD.EXE 'C:\pectest\smoke.ini'`。
CI 替代路线：Docker 化 wine 镜像（绕开本机 nix 构建）。

## 附录 B：可复查指标命令（M1-M7）

```bash
cd refactored   # 或 pecmd-next
M1: grep -rPno 'FUN_14[0-9a-fA-F]+' --include='*.c' . | wc -l        # 目标≈0(注释外)
M2: grep -rPno '\blocal_[0-9a-f]{2,}\b' --include='*.c' . | wc -l
M3: grep -rPno '\bparam_\d+\b' --include='*.c' . | wc -l
M4: grep -rPno '\bDAT_14|\bPTR_(FUN|s)?_' --include='*.c' --include='*.h' . | wc -l
M5: grep -rc 'TODO(verify' --include='*.c' . | awk -F: '{s+=$2} END{print s}'
M6: ./build.sh $(find src -name '*.c') ; echo $?                     # 全绿=0
M7: awk '/#endif/{f=1;next} f&&NF' include/pecmd_defs.h | wc -l      # guard外=0
```

## 附录 C：关键模板索引

| 模板 | 位置 |
|---|---|
| 用例 manifest.json | §4.C.1-0.2 |
| epilogue.pecmd 生成规范 | §4.C.1-0.3 |
| verdict.json 契约 | §4.C.1-0.5 |
| KNOWN-LIMIT 标签格式 | §3.B.5-5b |
| provenance.map TSV 格式 | §3.B.5-5c |
| 模块语义卡 | §4.C.2-3 |
| DoD 四件套核对单 | §4.C.4 |

（完）
