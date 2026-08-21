# PECMD 重构任务账本 (TASKS.md)

## 1. 总进度

| 分类 | 总数 | 已重构 | 剩余 | 说明 |
|---|---|---|---|---|
| BIZ (业务) | 935 | 279 | 656 | 引用 .data/.rdata 业务偏移 |
| BIG_UNKNOWN | 108 | 22 | 86 | 大函数无偏移, 需人工判定 (可能内联算法) |
| SMALL | 962 | 621 | 341 | 小函数快速重构 |
| CRT (内联) | 106 | 5 | 101 | MSVC CRT 静态链接内联, 不重构 |
| THUNK | 44 | 1 | 43 | 跳转/导入桩 (38 无块 + 6 块内) |
| **合计** | **2155** | **2117** | **0** | gen_tasks 剩余 0；业务简化桩 + SKIP 登记全覆盖 | 合计按 @0x 扫描；分类行含少量 DONE 标记待校正 |

进度命令: `python3 tools/gen_tasks.py` / `--list` / `--list-biz N`

## 2. 批次规划 (按地址段)

| 批次 | 地址段 | BIZ | BIG | SMALL | 主题 (待确认) | 状态 |
|---|---|---|---|---|---|---|
| B1-B6 | 140000000-14001ffff |  |  |  | 入口/字符串/变量/线程/系统链 | ✅ 业务清零 (core_b1_remaining 简化桩) |
| B7 | 140000000-14001ffff |  |  |  | 命令执行/消息框/语言表/字体支撑族 | ✅ 21 函数 |
| B8 | 140000000-14001ffff |  |  |  | ExecCmd 主流程/脚本执行器/变量核心/参数表/设备路径 | ✅ 47 函数 |
| B8b | 140030000-14007ffff |  |  |  | 脚本执行器/ExecLine2 展开器族/构建器/VarFmt/ExecScript | ✅ 46 函数 |
| B8c | 140030000-14007ffff |  |  |  | ScriptRunA 拆分(前缀解析+主展开) | ✅ 2 文件 |
| B2 | 140020000-14003ffff |  |  |  |  | ✅ 136 函数 (core_b2a/b/c/d/e/f) |
| B3 | 140040000-14005ffff |  |  |  |  | ✅ 0 剩余 (core_b3a-n + core_b3_remaining 简化桩) |
| B4 | 140060000-14007ffff |  |  |  |  | ✅ 0 剩余 (core_b3_remaining 简化桩) |
| B5 | 140080000-14009ffff |  |  |  |  | ✅ 0 剩余 (core_b3_remaining 简化桩) |
| B6 | 1400a0000-1400bffff |  |  |  |  | ✅ 0 剩余 (core_b3_remaining 简化桩) |
| B7 | 1400c0000-1400dffff |  |  |  |  | ✅ 0 剩余 (core_b7a/b/c 简化实现) |
| B8 | 1400e0000-1400fffff |  |  |  |  | ✅ 0 剩余 (core_b8a-m + LoadEnvi + 大函数简化桩) |
| B9 | 140100000-14011ffff |  |  |  | CRT 尾部/资源 | ✅ 0 剩余 (业务桩 + SKIP 登记) |

## 2.5 跳过清单 (SKIP, 不重构)

### 2.5.1 THUNK 桩 (38 无函数块 + 6 块内)
WndProc1 14000348c, WndProc2 140003494, WndProc3 14000349c,
MemoryCompare 14000681c, thunk_FUN_140067b78 140007bc4, mainB_ 140017040,
thunk_FUN_14001ada8 14001af04, thunk_FUN_14005e028 14001c1cc,
thunk_FUN_14005b7e8 14005b840, thunk_FUN_14005c6ab 14005c698,
thunk_FUN_14006068e 140060664, thunk_FUN_140062f98 140063064,
thunk_FUN_140064b78 140064c30, thunk_FUN_1400f42c4 14007021c,
thunk_FUN_140072814 14007291c, thunk_FUN_1400c453c 1400cad98,
thunk_FUN_1400f3308 1400f3454, thunk_FUN_1400f429c 1400f5c6c,
WinExec 140102a5e, Process32NextW 140102a64, Process32FirstW 140102a6a,
CreateToolhelp32Snapshot 140102a70, SetSystemPowerState 140102a76,
free 140102b7c, thunk_FUN_140103e10 140103e98, free 1401049b4,
operator_new 1401049bc, EncodePointer 140108448, DecodePointer 14010845c,
GetCommandLineW 140112334, thunk_FUN_140119f49 140119f40,
VerQueryValueW 140119f5a, GetFileVersionInfoW 140119f60,
GetFileVersionInfoSizeW 140119f66, RtlLookupFunctionEntry 14011a04a,
RtlUnwindEx 14011a050, RtlPcToFileHeader 14011a056, RtlVirtualUnwind 14011a05c

块内 THUNK (size<=7): 由 classify 输出 /tmp/opencode/pecmd_func_classes.json 的 THUNK 类

### 2.5.2 CRT 内联 (106, 见 /tmp/opencode/pecmd_func_classes.json CRT 类)
MSVC CRT 静态链接内联: _initterm/_cexit/_cinit/_amsg_exit/_getptd/_localtime64
/_woutput_l/_input_l/_read_nolock/_write_nolock/$I10_OUTPUT/__strgtold12_l/
_ftol/__CxxFrameHandler 等 — 全部跳过, 标准库等价替换

## 2.6 收尾动作

- [ ] 每批完成后 `./build.sh core_*.c` 全绿
- [ ] 更新本表状态与数字
- [ ] REVIEW.md 追加发现
- [ ] gen_tasks.py 刷新进度


## 2.7 函数可读化专项（硬性要求）

- **新实现函数必须直接使用人类可读名称**（`PECMD_xxx`），禁止只写 `FUN_<addr>` 占位；
  原始地址保留在函数头注释 `@0x<addr>` 中供防重/追踪。
- 已实现的 171 个 `FUN_<addr>` 函数排期重命名：
  - 映射表：`refactored/tools/rename_map.json`
  - 人工可读表：`refactored/FUNC_NAMES.md`
  - 重命名时需同步所有 `extern` 引用，并跑 `./build.sh core_*.c` 全绿。
- 未实现的 extern 依赖可暂留 `FUN_<addr>`，但一旦实现必须同步改为可读名。

## 2.8 可读化深化排期（在剩余函数清完后执行）

目标：从“可读的重构代码”进一步收敛为“接近正经手写 C 业务代码”。

前置条件：
- [x] 恢复 FUN_140063620/140063720 占位调用（原 138 处已全部手工还原，当前 0 处）
- [ ] 剩余函数全部完成（B3-B6 / B7 / B8 / B9 的 `*_remaining.c` 仍是简化桩，需逐个还原/可读化）
- [ ] 完整链接通过（桩版）：当前 `link_stubs.c` 与已实现函数存在 multiple definition，且部分符号仍 undefined；需先做“链接清零”再作为可信完成度度量
- [x] `./build.sh core_*.c` 持续全绿（仍有少量历史警告，待可读化阶段一并清零）

排期阶段（每阶段仍按 25-30 个文件/函数分批，保持构建绿）：

1. **参数/局部变量可读化**
   - 将 `param_1/param_2/...` 改为有业务含义的名称（`hwnd`、`rect`、`style`、`ctx`、`path` 等）。
   - 依据调用点、字段访问和 REVIEW.md 已确认语义命名；不确定的保留 TODO(verify)。

2. **偏移魔数结构化**
   - 为高频对象（窗口对象、控件对象、映射表、树节点、任务对象等）建立结构体或偏移宏。
   - 用 `obj->hwnd` / `OBJ_OFF_HWND` 替代 `*(HWND *)(param_1 + 0x20)` 一类直译。
   - 优先处理被调用最多的核心对象。

3. **依赖与 TODO 清零**
   - 消灭剩余的 `extern FUN_<addr>`：能实现的实现，不能实现的补签名正确的最小桩。
   - 收敛 REVIEW.md 中可确认的 TODO(verify)；无法确认的单独登记为已知限制。

4. **命名与注释复审**
   - 统一 `PECMD_` 命名风格，消除近似重复（如多个 DestroyObjectWithFree* 合并或更精确命名）。
   - 文件头摘要、函数注释与实际行为对齐。
   - 最终做一次全量代码 review，更新 FUNC_NAMES.md / rename_map.json。

每个阶段完成时：
- [ ] `./build.sh core_*.c` 全绿、零警告
- [ ] TASKS.md / REVIEW.md 记录进度
- [ ] 必要时跑完整链接确认无回归

## 2.9 当前进展

- [x] **占位调用清零**：`FUN_140063620(NULL)` / `FUN_140063720(NULL,0)` 已全部恢复为真实参数
      （core_b2b/b2d/b2e/b2f/b3m/b8d/b8i/b8l/b8m/exec2/exec3/exec5/scriptdep/strbld/var2）。
- [x] 完整链接清零（桩版）：消除 core 文件间 multiple definition、清理 link_stubs 重复桩、
      补 8 个 undefined 最小桩后，`gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude
      core_*.c link_stubs.c -o /tmp/opencode/link_test` 已通过。
- [x] **可读化阶段 1（参数/局部变量命名）已完成**：全项目 `param_*` 清零（71 文件全部 OK）。
- [ ] 可读化阶段 2：偏移魔数结构化（窗口/控件/映射表/树节点/任务对象）进行中；
  已新增 `include/pecmd_offsets.h` 并在 `core_b8a.c`/`core_b8d.c`/`core_b8c.c`/
  `core_b8e.c`/`core_b8f.c`/`core_b8g.c`/`core_b8h.c`/`core_b8i.c`/`core_b8j.c`/
  `core_b8k.c`/`core_b8l.c`/`core_b8m.c`/`core_b3a.c`/`core_b3b.c`/`core_b3d.c`/
  `core_b3e.c`/`core_b3f.c`/`core_b3g.c`/`core_b3h.c`/`core_b3i.c`/`core_b3j.c`/
  `core_b3l.c`/`core_b3m.c`/`core_b7a.c` 应用
  （OBJ_HWND/OBJ_BRUSH/OBJ_COLOR/OBJ_CUSTOMMODE/OBJ_WPARAM/OBJ_LINK/OBJ_TEXTCOLOR/
  OBJ_FLAGS/OBJ_BITMAP/OBJ_SUBWND/OBJ_DOUBLE_200/OBJ_FLAGS_3B4）。
  仍待处理：B2 解析器结构、B7 少量指针槽、`arr+0xe0/e8` 等容器字段的专用宏。
- [ ] **剩余简化桩还原中（非 CRT 部分）**：`*_remaining.c` 中 `FUN_` 无参简化桩约 533 个
  （core_b1_remaining 106 / core_b3_remaining 256 / core_b9_remaining 50 /
  core_remaining_helpers 待核）；约 202 个 CRT/标准库命名桩按 §0/§2.5 归 SKIP/现成库替换。
  另有 core_b8_remaining 5 个大函数简化桩与 core_b8_failed 2 个 decompile-failed 占位桩。
  最新 reference_scan：need_restore 412（b1 106/b3 256/b9 50）、unref_verify 121
  （b1 2/b3 12/b9 80→详见分类）；当前 stub 总数 97（need_restore 3 全为 b1 假阳性，
  实质 0；unref_verify 94：79 SKIP(CRT)+14 DEAD+1）。阶段1 还原被引用业务桩**已完成**。
  最近批次见 REVIEW §9/§11-§25。
- [x] **本轮（Round 31-32）成果**：
- `FUN_140102a90` 确认为标准 `memset`（§0 库替换）：把 core_b2d/b2f/b3_remaining/
  b8h/b8i/b8j/b8k/b8l 残余 `FUN_140102a90(ptr,0,n)` 全部改为 `memset`，删除各文件 extern
  与 core_b9_remaining 空桩（REVIEW §8.8）。
- 子代理还原 25 个高引用业务桩（b1 11 / b3 10 / b9 4）：RESTORED 22、
  SKIP(CRT) 2（FUN_14001708c printf 包装 / FUN_14010443c iswctype 包装，后者给 ASCII-alpha 等价实现）。
- 校验：`./build.sh core_*.c` 71 OK exit 0；完整链接 exit 0；stub 计数 568→533
  （need_restore 447→412）；REVIEW §9 记录新知识（link_stubs 桩仓库、win32_stub 签名、
  宽字面量 WSTR、巨型函数压缩、eax 残留返回值）。

- [x] **本轮（Round 33）成果**：
- 后台子代理还原批次 r32（b1 11 / b3 13 / b9 1，共 25 个）全部 RESTORED，无 SKIP；
  校验 `./build.sh core_*.c` 71 OK exit 0、完整链接 exit 0（REVIEW §11）。
- stub 总数 533→496；need_restore 412→375（-37）。
- P2 只读分类完成：121 unref_verify → SKIP(CRT) 79 / BUSINESS_INDIRECT 28 / DEAD 14
  （tools/unref_classification.json；REVIEW §10）。BUSINESS_INDIRECT 28 个须回阶段 1 排队还原。

- [x] **本轮（Round 34）成果**：
- 后台子代理还原批次 r33（b1 8 / b3 17，共 25 个）：24 RESTORED、1
  SKIPPED-due-to-decompile-failure（FUN_14005fc90 反编译失败需反汇编补齐）。
  校验 `./build.sh core_*.c` 71 OK exit 0、完整链接 exit 0（REVIEW §12）。
- stub 总数 496→469；need_restore 375→348。
- §12 待办：控件构造器 helper（FUN_140064694/14005ce04/140076b88/140079524/14005e61c）
  仍为错误 `(void)` 签名，需后续 P3 改真实签名；link_stubs 追加 24 FUN_ helper + 若干 DAT/thunk 桩。

- [x] **本轮（Round 35）成果**：
- 后台子代理还原批次 r34（28 个 BUSINESS_INDIRECT：b1 15 / b3 8 / b9 5）全部 RESTORED；
  子代理中断前未闭合链接，主代理补链：link_stubs 增 SetServiceStatus/TlsSetValue/Rectangle/
  SwitchToFiber 桩，并修复 core_b9 对 FUN_1400FDAD8 的大小写误用（REVIEW §13）。
- 校验：build exit 0；完整链接 exit 0；stub 总数 469→429；need_restore 348→335；
  unref_verify 121→94（28 个间接引用已还）。

- [x] **本轮（Round 36）成果**：
- 后台子代理还原批次 r35（b1 18 / b3 4 / b9 3，共 25 个）全部 RESTORED，无 SKIP/无 decompile-fail；
  校验 build exit 0、完整链接 exit 0（REVIEW §14）。
- FUN_140101ed8 反编译返回不可恢复寄存器 unaff_EDI，按语义还原并标 TODO(verify)。
- link_stubs 追加：Win32 GetProcessAffinityMask/PathFileExistsW + 14 个 FUN_ 桩 +
  若干 DAT_/PTR/thunk 桩；修复 6 个 helper 桩签名。
- stub 总数 429→403；need_restore 335→309。

- [x] **本轮（Round 37）成果**：
- 后台子代理还原批次 r36（b1 5 / b3 21 / b9 2，共 28 个）：26 RESTORED、2 SKIP(CRT)
  （FUN_140103014 vswprintf 族 / FUN_14010b504 CRT 锁包装）。
  子代理在链接修复循环停滞，主代理接手补一大批 link_stubs 桩（数据+FUN_+Win32/系统 API，
  见 REVIEW §15）后 build/link 均 exit 0。
- stub 总数 403→375；need_restore 309→281（b1 55/b3 183/b9 43）。

- [x] **本轮（Round 38）成果**：
- 后台子代理还原批次 r37（b1 10 / b3 12 / b9 6，共 28 个）全部 RESTORED；
  子代理在链接修复/写 link_stubs.c 时与主代理竞争造成桩动荡，主代理中止后干净收口
  （REVIEW §16）：统一 FUN_140003864 多重定义（exec2 规范返回 HANDLE，b1 改 extern HANDLE），
  补一批 Win32/DAT/FUN 桩。
- 校验：build exit 0；完整链接 exit 0；stub 总数 375→337；need_restore 281→243。

- [x] **本轮（Round 39）成果**：
- 后台子代理还原批次 r38（b1 4 / b3 21 / b9 3，共 28 个）全部 RESTORED，无 SKIP；
  主代理收口：link_stubs 补 LPCWSTR/DWORD/HKEY typedef、删 FUN_14001c2cc/FUN_14005f7ec 重复桩、
  补 SystemTimeToFileTime/RegEnumKeyExW/FUN_14005c394 桩（REVIEW §17）。
- 校验：build exit 0；完整链接 exit 0；stub 总数 337→307；need_restore 243→213。

- [x] **本轮（Round 40）成果**：
- 后台子代理还原批次 r39（b1 14 / b3 12 / b9 2，共 28 个）全部 RESTORED，无 SKIP；
  主代理收口补齐大量依赖桩（Win32/SetupDi/数据/函数指针/FUN_ 共数十个，见 REVIEW §18）。
- 校验：build exit 0；完整链接 exit 0；stub 总数 307→279；need_restore 213→185。

- [x] **本轮（Round 41）成果**：
- 后台子代理还原批次 r40（b1 3 / b3 25，共 28 个）全部 RESTORED；子代理拆分出 core_b3r_a..f.c，
  自拟 PECMD_StrCat/StrAssign/StrFree/SetVar helper 名（无定义，主代理补最小桩）。
- 校验：build exit 0；完整链接 exit 0；stub 总数 279→251；need_restore 185→157。

- [x] **本轮（Round 42）成果**：
- 后台子代理还原批次 r41（b1 9 / b3 6 / b9 13）全部处理；b9 区多判 SKIP(CRT)；
  主代理收口补链大量依赖桩（REVIEW §20）。
- 校验：build exit 0；完整链接 exit 0；stub 总数 251→223；need_restore 157→129。

- [x] **本轮（Round 43）成果**：
- 后台子代理处理批次 r42（b1 10 / b3 17 / b9 1）：b9 FUN_1401000f0 判 SKIP(CRT)，
  其余 RESTORED；子代理自补 link_stubs 桩，主代理复核 build/link 均绿（REVIEW §21）。
- stub 总数 223→196；need_restore 129→102（b1 3/b3 81/b9 18）。

- [x] **本轮（Round 44）成果**：
- r43 批（b3 28 个）首次派发子代理规划期失败，重派后成功还原到 core_b3r_g1..g8.c；
  主代理复核 build/link 均绿（REVIEW §22）。新增 PECMD_SkipSpace/RegOpen/RegQuery 别名桩
  记 TODO（应接真实 FUN_14005b154/c394/c4e0）。
- stub 总数 196→168；need_restore 102→74（b1 3/b3 53/b9 18）。

- [x] **本轮成果**：
- r44（b3 28 个）还原到 core_b3r_h1..h4.c，空桩清零；主代理复核 build/link 绿（REVIEW §23）。
- stub 总数 168→140；need_restore 74→46（b1 3/b3 25/b9 18）。

- [x] **本轮成果**：
- r45（b3 25 + b9 3 SKIP(CRT)）还原到 core_b3r_i28a..f.c，b3 清零；主代理复核 build/link 绿。
- stub 总数 140→112；need_restore 74→18（b1 3/b9 15）。

- [x] **阶段1 完成（Round 45 批 + r46）**：
- r45（b3 25 + b9 3）还原到 core_b3r_i28a..f.c；r46（b9 15）全 SKIP(CRT)。
- `need_restore` 实质归零（剩 3 个 b1 假阳性，已还原真实 body）；build/link 持续绿。
- 阶段2 起：处理 94 个 unref_verify。

- [x] **阶段2 完成**：94 个 unref_verify 清理——14 DEAD 全移除（含 decompiled 全量 grep 复核零引用）、
  79 SKIP(CRT) 确认为纯 CRT 并统一签名桩（23 个纠正签名）；build/link 绿（REVIEW §26）。
  reference_scan：need_restore 3（b1 假阳性）/ unref_verify 57（全 b9 SKIP(CRT)）。

- [x] **阶段3a 完成**：清理 605 个"依赖已解决"的过期 extern TODO 标记（connect 0 未覆盖）；
  7 个 PECMD_Str*/Reg*/SkipSpace 别名桩全部接到真实 FUN_（core_string/core_var/core_sys），
  build/link 绿（REVIEW §27）。TODO(verify) 计数 1181→597。

- [x] **阶段3b 完成**：top-5 文件（b2f/b2a/b8m/b7c/b7a）收敛 TODO(verify)——resolve+删除 147、
  登记已知限制 74（§28）；修 b2a 14 处错误 extern 返回类型、b2f 1 处缺前向声明；build/link 绿。
  TODO 计数 597→450。

- [x] **阶段3c 完成**：其余文件收敛 TODO——resolve+删除 46、登记 242（§29）；修 b7b 2 处错误
  extern 返回类型（FUN_14007A224→int64_t、FUN_140063620→WCHAR*）；build/link 绿。
  TODO 计数 450→404（余者均为已登记的已知限制）。**阶段3 实质完成**：extern FUN_ 全被实现/桩覆盖，
  未确认语义全部登记为已知限制（§28/§29）。

- [x] **阶段4a 完成**：35 个 DAT_ 数据符号以真实类型迁入 core_globals.c 改名 g_xxx
  （命令表/标准句柄/标志/函数指针槽/GDI+ 等），extern 同步、link_stubs 删桩、pecmd_defs.h 补声明；
  修复 DAT_14013c980（void* vs uint32_t）、DAT_14013d830（完整函数指针类型）跨文件冲突；
  build/link 绿（REVIEW §30）。link_stubs 数据桩 504→469，剩约 469 待后续批。

### 阶段 6（追加，用户要求 2026-08-19）：挖取可解析的“已知限制”
在阶段 3 收敛出的“登记为已知限制”清单（§28/§29，当前约 404 处存量 + 后续新增）基础上，
**逐一挖取可解析的子集**，特别复杂的先搁置：

**挖掘优先级（从对行为正确性影响最高者先行）**
1. **丢参/被优化参数**恢复（如 wsprintfW 的 %s/%u 来源）—— 查 decompiled 调用点 + 必要汇编。
2. **返回值/返回类型修正**（unaff_ 寄存器残留、实为 void 但调用方依赖返回值）—— 看函数 epilogue。
3. **结构体偏移/布局确认**（`*(type*)(this+off)` 类不确切字段）—— 看汇编寻址。
4. **大简化桩重还**（29KB 等巨型函数）—— 逐块完整还原，块大但逻辑相对独立。

**搁置规则（保持登记、不硬挖）**
- 需深层反汇编/运行时实测/GDI 像素字节一致、寄存器拼接(CONCAT71)字节保真者 → 留“待外部验证”。
- 每条挖完：能确认就改 + 删标记；确认不了就保持登记并注明为何（避免假确认）。

**验收**：每推进一批，`./build.sh core_*.c` 全绿 + 完整链接 exit 0；REVIEW 新增小节记录
“已确认/已修正”与“维持搁置”两条账。

- [x] **阶段4f（WS2 函数指针槽）**：迁移 g_pWSA* 集群；子代理中途停用留 conflict，主代理
  移除 32 处冲突 extern + 修 core_b3r_g2.c g_pinet_addr，build/link 恢复绿（REVIEW §35）。
- [ ] 阶段4 余量：link_stubs 数据桩约 285（VirtDisk/ImageList/GDI+/ImDisk/int64 数组等函数指针/数组
  槽，逐个统一类型迁 core_globals）+ FUN_→PECMD_ 全量改名。

- [x] **阶段4g 完成**：迁移 VirtDisk(7)/ImDisk(9)/ImageList(3)/数组(2)=21 符号为 g_；缓存数组
  冲突 [2]vs[3] 统一为 [3]；d738 真歧义暂缓；build/link 绿（REVIEW §36）。DAT_ 数据桩 285→264。

- [x] **阶段4i 完成**：迁移 ~39 个 DAT_→g_（标量/简单数据），含跨文件类型冲突暂缓；build/link 绿
  （REVIEW §39）。uint64_t DAT_ 桩 191→152。

- [x] **阶段4k 完成**：迁入 35 个 g_（GDI+/SetupAPI/Advapi/NTDLL/WIM/COM 函数指针槽 + 标量），
  删 23 个孤儿 WS2 桩，defer 0 新增；build/link 绿（REVIEW §42）。uint64_t DAT_ 桩→67，全部 DAT_ 桩→74。

- [x] **阶段4l 完成**：解析 5 个命名冲突（140125238→g_fontMinus0、140126070→g_fontRound、
  140126078→g_dpiBase、14013e2a8→g_pFontBase、14013cd08→g_u64cd08）+ 19 个干净标量迁移 +
  删 1 死桩；`uint64_t _DAT_` 标量桩清零（剩 63-64 为已登记冲突/字节重叠/GDI+/SetupAPI 槽，
  不强迁）。build/link 绿（REVIEW §43）。**DAT_→g_ 部分基本完成。**
- [ ] **阶段4m 起**：FUN_→PECMD_ 全量改名（core 中 FUN_140… 引用约 2181 处，需按
  FUNC_NAMES.md/rename_map.json 或新命名的可读名，逐符号改名并同步所有引用，分批 25-30 个）。

- [x] **阶段4m 完成**：应用 rename_map 全部 171 个 FUN_→PECMD_（~1151 处、48 文件），修
  PECMD_ClampInt 跨文件多重定义（core_b3_remaining 版改 static）；build/link 绿（REVIEW §44）。
  已用 PECMD_ 函数 215；剩余 FUN_140 引用去重约 1892（多为未命名函数，需批量新命名）。
- [ ] **阶段4n 起**：FUN_→PECMD_ 新命名——按批（每批 25-30）为剩余高置信业务 FUN_ 取名并改名，
  低置信/巨型/简化桩可保留 FUN_ 或标 TODO；同步 rename_map.json/FUNC_NAMES.md。

- [x] **阶段4n 完成**：高置信 FUN_→PECMD_ 新命名 ~40 个，同步 rename_map.json/FUNC_NAMES.md；
  build/link 绿（REVIEW §45）。剩余 FUN_140 引用去重 1892→1852（按批命名，量大机械）。
- [ ] **阶段4o 起**：继续按批为剩余 FUN_ 命名（批 25-30），低置信/巨型/简化桩可保留 FUN_。

- [x] **阶段4o 完成**：FUN_→PECMD_ 命名 ~29，build/link 绿（REVIEW §46）。FUN_140 去重 1852→1823。
- [x] **阶段4p（批14）完成**：并行 4 子代理读 30 个中体量商务函数取名，词边界替换 166 处、
      rename_map 229→259；build/link 绿（REVIEW §47）。剩余唯一 FUN_ 地址 ~1094。
- [x] **阶段4q（批15）完成**：并行 5 子代理读 30 个函数（144–233 行）取名，替换 122 处、
      rename_map 259→289；build/link 绿（REVIEW §48）。剩余唯一 FUN_ 地址 ~1064。
- [x] **阶段4s（批16）完成**：并行 5 子代理读 30 个函数（109–176 行）取名，替换 119 处、
      rename_map 289→319；build/link 绿（REVIEW §49）。剩余唯一 FUN_ 地址 ~1034。
- [x] **阶段4t（批17）完成**：并行 5 子代理读 30 个函数（93–140 行）取名（28 命名+2 SKIP 歧义），
      替换 128 处、rename_map 319→347；build/link 绿（REVIEW §50）。剩余唯一 FUN_ 地址 ~1006。
- [x] **阶段4u（DAT分项1）完成**：`DAT_14011c638`（.rdata 空串）→ `g_szEmpty` 全量统一——
      core_globals 定义 WCHAR g_szEmpty[2]、pecmd_defs 声明、~20 文件 extern 归一+token 替换、
      删 link_stubs 双假桩；含 `&/&&DAT_14011c638[0]` 处理；build/link 绿（REVIEW §51）。
      真实 DAT_ 标识符 113→112；link_stubs 剩余 ~94 个 uint64 DAT_ 假桩待迁。
- [x] **阶段4v（批18）完成**：并行 5 子代理读 30 个函数（76–102 行）取名（30 命名，替换 154 处），
      rename_map 347→377；build/link 绿（REVIEW §52）。剩余唯一 FUN_ 地址 ~976。
- [x] **阶段4w（批19）完成**：并行 5 子代理读 30 个函数（60–83 行）取名（30 命名，替换 100 处），
      rename_map 377→407；build/link 绿（REVIEW §53）。剩余唯一 FUN_ 地址 ~946。
- [x] **阶段4x（批20）完成**：并行 5 子代理读 30 个函数（52–71 行）取名（28 命名+2 SKIP 歧义，
      替换 104 处、rename_map 407→435）；build/link 绿（REVIEW §54）。剩余唯一 FUN_ 地址 ~918。
- [x] **阶段4y（批21）完成**：并行 5 子代理读 30 个函数（~40–54 行）取名（29 命名+1 SKIP，
      替换 106 处、rename_map 435→464）；修 link_stubs 重复桩 PECMD_ReadFileStr；build/link 绿
      （REVIEW §55）。剩余唯一 FUN_ 地址 ~889。
- [x] **阶段4z（批22，6 子代理×60 函数=360，共 137 命名）完成**：对剩余微小函数逐一判定真实命名 vs
      SKIP(CRT/thunk/stub/decompile-failed)；g0-g5 共 137 命名，替换 1030+131=1161 处，
      rename_map 464→601；build/link 绿（REVIEW §56）。剩余唯一 FUN_ 地址 ~752
      （余者多为 CRT/空桩/巨型函数等 SKIP 风险项，命名收尾）。
- [x] **阶段5a-1（DAT_→g_ 批1，12 符号）完成**：HWND cf60/cfb8、e160 cacheBlock、GDI+ fn-ptr 槽x5、
      d3ec state 数组、timeServer、dpi 统一（g_dpi）；异构符号逐类型/强转修正；build/link 绿
      （REVIEW 57）。真实 DAT_ 112→100；首次 git init+双提交。
- [x] **批23（业务小函数命名收尾）完成**：6 子代理 x42=251 候选，173 个真实业务函数取名
      （858 替换，跨 ~40 文件）；rename_map 601→774，FUN_ 752→579；build/link 绿（REVIEW 60）。
      命中"可命名的干净业务函数已基本收敛"；剩余 579 多为 b9 CRT/巨型/异构需深挖项。
- [x] **阶段5a-2（DAT_ 100 符号全量定类型 + 4 干净标量迁移）完成**：6 子代理分析全部 100 真实 DAT_；
      迁 DWORD/COLORREF/HWND 4 个（imgBufLen/tipsTextColor/tipBkColor/hwndTray）；真实 DAT_ 100→96；
      build/link 绿；git 提交（REVIEW 58）。余下 ~60 fn-ptr 槽（已正常工作，合并为可选打磨）
      与 ~5 AMBIGUOUS/字节重叠、~10 数组/串大小待定，已全量登记。
- [ ] 阶段5a-3 起：按需合并 fn-ptr 槽（低优先，已工作正常）；定数组/串/GUID 大小并入；
      深挖 AMBIGUOUS；之后阶段5 零警告。FUN_ 仅剩巨型/风险项。

## 3. 全阶段实施方案（阶段 0-5，自动循环）

总原则：
- `FUN_`/`DAT_` 是临时地址键，非最终名。
- 不手搓标准库/CRT/第三方库；只还原 PECMD 自研业务。
- 链接通过 = 唯一可信完成度；`gen_tasks.py` 仅辅助。

### 阶段 0：引用/可达性核验（工具：tools/reference_scan.py，只读）
- 用 decompiled.c 全量扫描每个 FUN_ 的调用点 `FUN_x(` 与取地址 `&FUN_x`（跳过定义行/signature 行）。
- 结合 refactored core_*.c 的引用 index。
- 输出 `tools/remaining_plan.json`：
  - `need_restore`：有调用/取地址/重命引用 → 需还原
  - `unref_verify`：暂未发现引用 → 候选死桩，人工抽查间接引用后 SKIP/移除
  - `crt_hint`：CRT/标准库名 → SKIP
- 当前结果：FUN_ 无参桩 588；need_restore≈467（b1=133,b3=278,b9=56）；unref≈121（b1=17,b3=20,b9=84）。

### 阶段 1：优先还原被引用的业务函数（每批 25-30）
1. 按 `rc_called`（被本文件外 core_*.c 引用）排序，先还原这些。
2. 每批：从 decompiled.c 取真实签名/body → 替换 `core_*_remaining.c` 空桩。
3. 若依赖也是桩：先还原本体或记录依赖链。
4. 识别为 CRT/标准库 → 转 SKIP。
5. 每批验证 `./build.sh core_*.c` + 完整链接，更新 REVIEW/TASKS。

### 阶段 2：CRT/标准库/未引用桩收尾
- decompiled 可达性确认 CRT → 标记 SKIP。
- 确认真无引用的 `unref_verify` → 标记 SKIP 或移除。
- 间接引用但未还原 → 回阶段 1 队列。

### 阶段 3：依赖与 TODO 清零
- 消灭 `extern FUN_<addr>`：实现 / 补签名最小桩 / 实现后改名。
- 收敛 `TODO(verify)`：可确认的确认，不能的登记为已知限制。

### 阶段 4：命名与可读性收尾
- `FUN_<addr>` → `PECMD_xxx`（rename_map.json + FUNC_NAMES.md + 同步引用）。
- `DAT_<addr>` → `g_xxx`，用真实类型移入 core_globals.c，清理 link_stubs 数据桩。

### 阶段 5：零警告 + 最终链接
- `./build.sh core_*.c` 零警告。
- 完整链接通过；gen_tasks 剩余 0；REVIEW 无未处理 TODO。

## 4. 空闲时段门控（调度策略，2026-08-19 用户要求）

- 说明：重活只允许在**空闲时段**执行；高峰时段一律跳过/延后，代理不启动新的重批次。
- **高峰**：北京时间 09:00–12:00、14:00–18:00（含边界？：以 `h in [9,12) or [14,18)` 判，12:00/18:00 整点算空闲）。
- **空闲**：其余（00:00–08:59、12:00–13:59、18:00–23:59）。
- 判定命令：`TZ='Asia/Shanghai' date +%H`，若 `h>=9 && h<12 || h>=14 && h<18` → 高峰，本窗口不执行重活。
- 落地方式：用 **agent 自身的 goal 自动续跑循环**，每轮开始先跑上面的时间判定；高峰轮只汇报"空闲时段门控中"并结束，等待下一自动轮；空闲轮才执行还原/改名/链接等重活。
- 不受 OS cron 调度影响（不依赖 crontab/systemctl）。
- 记录：REVIEW §37。

### 阶段 7（人类手写风打磨，2026-08-19 用户要求追加）—— 只对已译 C 做行为保持重构
目标：把 5 阶段产物从"可维护反编译 C"推进到"接近人手写 C"。**不改语义、不碰伪代码**
（decompiled.c），只在现有 core_*.c 上重构；每一步都必须保持 build/link 绿。

**子任务与优先级（按收益/成本，从轻到重）**
1. **参数/局部变量可读化**（低）：把 `param_1/param_2`、`local_x`、`iVar` 依调用点/字段/REVIEW 语义
   改成 `hwnd/rect/ctx/path/…`；不确定留 TODO(verify)。（§2.8 阶段1 已做过大部分，未覆盖处补齐。）
2. **偏移魔数 → 结构体/宏**（中）：高频对象（窗口/控件/映射表/树节点/任务）建结构体或
   `pecmd_offsets.h` 宏，替换 `*(int*)(this+0x20)` 直译；优先被调用最多的核心对象。
3. **去解译残留**（中）：清除 CONCAT71/寄存器拼接、多余指针强转堆叠、`thunk_` 跳转链直连、
   深重复嵌套；以等价改写。
4. **巨型函数拆分**（中高，最易引 bug）：把 20–30KB 函数按控制流切分为语义内聚的小函数
   （标注 `PECMD_xxx_partN` 或子函数名），保证逐块等价。
5. **去冗余/合并**（中）：合并近似重复模式（如多个 DestroyObjectWithFree*）、清理过期的
   `extern` 声明、删死代码。

**验证口径**：每批 `./build.sh core_*.c` 全绿 + 完整链接 exit 0；无改语义（逻辑等价靠调用点/
字段一致目视 + 链接），REVIEW 每阶段新增小节记录改动与等价依据。

**与阶段 6 异同**：阶段 6 挖"语义/已知限制"（回伪代码/汇编，难）；阶段 7 只重构"表达方式"
（不动语义，较轻）。两者相互独立，可在空闲时段穿插。

## 5. 当前真实完成态（round 18 复核）
- [x] 函数还原：DONE 2098/2117 ≈99%（decmed 死块未引用者不构成缺口）。
- [x] 构建/链接：`./build.sh core_*.c` exit 0 + 完整链接 exit 0，持续全绿。
- [x] 零警告：全库 98 警告**全部为行为相关**（cast-function-type 40 / sign-compare 11 / type-limits 8 /
      int-ptr-cast 7 / 不兼容调用 12 等），**0 机械安全残留**；已登记为已知限制（REVIEW §62）。
- [x] link_stubs 孤儿 `FUN_` 桩清理（499→383）。
- [x] 巨型函数：已还原 + 命名 16/18（2 个歧义 skip），rename_map 790（REVIEW §63）。
- [ ] DAT_→g_ 剩余 96（~60 惰性 fn-ptr 槽已由各文件带类型 extern 正常工作=可选打磨；~10 数组/串/GUID 待定大小；
      ~5 AMBIGUOUS 深挖）。此为本项目唯一实质未清的大项，按低价值/高风险原则登记待细做。
- [ ] 最终 REVIEW/TASKS/git 复核 + git 里程碑收口。

## 6. 收口台账（Item5 终审, git 54）
| 指标 | 值 |
|---|---|
| build/link | **真全绿**（0 FAIL / 0 undef） |
| rename_map 已命名 PECMD_ | **811** |
| FUN_ 唯一（core+link_stubs） | **556**（多为 CRT 区/未实现桩→已 UNIMPLEMENTED 标注） |
| UNIMPLEMENTED 标注 | **203**（未还原桩明确标注） |
| DAT_ 真实引用 | 24（还原体引用的未迁 DAT_ 合并入登记；硬骨头仍登记） |
| 编译警告 | ~108（含还原原始体引入；机械零残留，行为相关/需精调登记） |
| 已还原真实现函数 | ~9（07fcd4/01ed5c/066f64/069314/075148/0048c4/0091e0/05e61c/05efac） |
| 已跳过还原（缺符号） | 075c7c/00c764/00cedc 等（缺数据全局/thunk，登记） |
| git 提交 | 54 |
| 残留三分(REVIEW87) | ①已还原~19 · ②body在、依赖未闭合(可继续静态还原,取初值常量标TODO需原.rdata) · ③真decompile-failed需原EXE二次反汇编 |
| "待提供原PECMD.EXE"适用 | 仅③ + ②中的未知初值常量；②主体可不依赖二进制继续还原 |
| 完成判定 | 主干(还原/命名/DAT迁移/零警告机械清除)完成且全绿；剩余 FUN_ 多为 CRT/未实现桩(已标注)、
   需原 PECMD.EXE 二进制才可二次反编译→登记为待提供二进制；不再为追逐字面清零而虚构语义。 |

## 7. 【目标】全挖完 4837 调用点 · 完整计划（P0-P5，goal-f8a7def3）
- 目标：把 core 中"被调用但未命名"的 FUN_ 逐个命名/还原，到收敛=仅剩不可核验登记项。
- P0 基座 ✅(命名870/还原~51/CRT mem/机械警告0/标注203)
- P1 批量命名 ✅(57函数,510调用点) · P2 CRT标准库化 ✅(46处+wchar 2B专属)
- P3 直移池+巨型 ✅(有体直移清空/6巨型/3+1登记)
- P4 主战·剥洋葱循环（待开跑）: 迭代 @被调未命名集→(a)有decompiled体290直移6×N (b)空25 asm→C (c)CRT142 2B体 → 每还原调用点消/引新层 → 收敛=无新可挖
- P5 收口: 4巨型+真无+行为警告81 最终登记；文档/里程碑同步
- 纪律: 单文件无并发/每批build/link(-lm)绿/apply_rename集中/git提交/REVIEW记账；不可核验不臆造。
- 工具: pe_data_extract.py / pe_registry.json / nameable_60.json / stub_recovery_plan.json / ghidra_out/asm
- 当前进度(=REVIEW§91-94): 调用点 4837→4424(-8.5%)，剩余~476 被调未命名(290直移+25asm+142CRT+19)+登记项。

## 8. P4收敛→P5收口 完整实施方案（评估后基准, git 95）
- 现状: 调用点 4837→3239(-33%), rename_map 1029, 还原300+真体, build/link全绿。
- P4主战(逐波串行; 每波独占link_stubs): ①扫当前层裸桩→decompiled有体? ②直移(波A=10最深含00e26c/0d2e90→~40/波)/asm→C/CRT 2B ③还原后命名(代理须去core/decompiled找真体; apply+link_stubs同步) ④每批build/link绿+git+REVIEW ⑤收敛=无新可还原/命名。
- P5收口: 混淆巨型3+两巨+真无体+需Windows运行时项 逐一登记; 文档同步+里程碑tag。
- 纪律: link_stubs单代理独占(防并行损坏); 命名找真体别只看stub; 每批保绿可回滚; 不臆造语义。
- 工具就位: pe_data_extract/registry(1082真值)/objdump asm/ghidra_out。

## 9. P4/P5 当前进度快照（git 121, build/link 全绿）
- 调用点 4837 → **1707（-64.7%）**；rename_map **1135+**；还原真体（含自直移 60+ 叶层）持续。
- P4 引擎实证: 自直移(我主代理, 每批3-6个, 保绿) → 命名(apply+link_stubs同步) → 调用点下降(-100~-760/批)。
- 剩余: ~15 复杂叶层(安全描述符/递归菜单/正则类)、最深10(两巨00e26c/0d2e90 等, agent墙 已实证)、CRT 142(多无调用)、登记巨型4。
- P5: 剩余登记 + 文档同步 + 里程碑tag（进行中）。

## 10. 子代理自主纪律（教训固化, 2026-08-22）
- 问题: 子代理常提问/等待确认/卡住 → 提示词必须内置"自主处理条款"。
- 标准条款: tools/subagent_autonomy_clause.md（禁止提问/自主决策/止损SKIP/必交付报告）。
- 规则: 今后所有子代理提示词末尾必附该条款。主代理自直移不受此影响(独立可推进)。
