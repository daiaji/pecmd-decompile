# HANDOVER_WIN.md — WIN 端作战手册 (v5, DSH 交接版)

> 2026-08-24 重写于 P0 完成后｜前身 v4 (参考库路线)。
> 本文是 **DSH 的行动手册**: 当前进度 → 已定案的技术档案 → 唯一堵点的完整证据链 → 下一步任务清单。
> **读法**: §1 看状态, §5 直接开工, §3/§4 遇到同类问题再回来查。

---

## 1. 状态一句话

**P0 (MSVC 化) 已完成并超额** — 双绿门达成 (`tools\msvc_build.bat syntax` 全绿 + 完整构建 exit 0),
`pecmd_msvc.exe` 产出且能运行到初始化深处。当前卡在 **P2 分诊的第一个运行时 bug**:
LOAD 冒烟时变量写入链触发 OOM 弹窗循环, 证据链已收窄到一个函数内 (§5-T1), 但未修复。

| 阶段 | 状态 |
|---|---|
| P0-① 复合字面量 | ✅ 完成 (56+1 处 → `PECMD_LI/PECMD_FT`, 另加 22 处标量强转、21 处裸 `(...)`) |
| P0-② crt_shims 裁剪 | ✅ 完成 (202→12, 190 个 CRT 桩删; win32_api_stubs 另裁 506 个系统同名桩) |
| P0-③ 构建管线 | ✅ 完成 (+ 补链 cfgmgr32/setupapi/shlwapi/version + GUI 子系统入口) |
| G0 门禁 | ⚠️ exe 可运行不崩, 但冒烟用例未闭环 (堵在 T1) |
| P1 验证闭环 | ⚠️ 半程: run_case.py 协议跑通; epilogue 的 WRITE 命令在原版不存在需换通道 (T2) |
| P2 对拍/分诊 | 🔴 进行中: 第一个 FAIL (OOM) 定位完毕待修 |
| P3/P4 | ⬜ 未动 |

## 2. 环境 (已就绪, 勿重装)

- VS 2022 Community + Windows Python 3.14。构建: 任意终端
  `cmd /c "call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 && tools\msvc_build.bat"`
- `C:\pectest\`: `PECMD.EXE` (=PECMD原始.EXE, 版本 **201201.88.05.94 mdyblog 修改版**) + `pecmd_msvc.exe`
- WER LocalDumps 已配置 (HKLM\...\LocalDumps → C:\pectest\dumps, DumpType=2 全量)
- git 身份已配 (repo-local: daiaji)
- ⚠️ 本机 8GB 内存, 编译期峰值吃 ~2GB, 勿并行多份构建; 构建产物 (*.obj/*.pdb/*.map/dump) 用完即删

## 3. 技术档案 — MSVC 化改动手法 (遇到同类报错直接套)

| 症状 | 根因 | 手法 |
|---|---|---|
| C2059/C2143 `{...}` | GCC 复合字面量 `(LARGE_INTEGER){.QuadPart=x}` | → `PECMD_LI(x)` (辅助构造函数在 win32_stub.h 与 stubs_common.h 各一份, 语义等价) |
| C2440 标量→struct | 反编译器 cast 残留 `(LARGE_INTEGER)(int64_t)x` | → `PECMD_LI(x)` |
| C2059 在 `(...)` 前 | 函数指针参数表裸 `(...)` 是 GCC 扩展 | → `()` |
| LNK2005 vs kernel32 等 | win32_api_stubs.c 定义了系统 DLL 导出同名函数 | 删定义让系统接管 (对照 dumpbin /linkermember:1 的导出全集); 该文件现在只剩 13 个非系统符号 |
| LNK2019 `_snwprintf` | UCRT 无此导入符号 (头文件内联实现) | 转发真体 → `__stdio_common_vswprintf` (win32_api_stubs.c 顶部), **勿 include <stdio.h> 进该文件** |
| C2371/C2129 static | 隐式声明/跨 TU static 不一致 | 补前置声明 / 去 static |
| C2036 void* 未知大小 | GNU void* 算术扩展 | 显式 `(char *)` 步长; 注意 Ghidra 的 `int* +N` = 字节 `+4N` (曾因此把 g_pMapBlk+6 修正为 +0x18) |
| C7744 \xXXXX 越界 | `WSTR("A" "B")` 续行段丢 L 前缀 | 合并为单一字面量 (宏只粘首段!) |
| C2143 label 后 | 标签后无语句 (C23 不允许) | 加 `;` |

## 4. WIN 调试工具箱 (本轮沉淀, 复用价值高)

无 cdb/windbg/procdump 的环境下的替代方案:

1. **崩溃**: WER LocalDumps 自动落 dump → Python 解析 minidump (纯 struct, 无依赖):
   异常流(6)/模块流(4)/Memory64List(9)。注意两个坑:
   - MINIDUMP_EXCEPTION 的 ExceptionAddress 实测在 offset 24 (非文档 16);
   - ThreadContext 的 Rva 直指 CONTEXT (无 MEMORY_DESCRIPTOR 头), x64 RIP 在 ctx[0xF8]。
   RIP → 模块偏移 → `build\msvc\pecmd_msvc.map` 第三列静态 VA (基址 0x140000000) 二分查符号。
   **构建时 map 会生成, 但已被 gitignore — 要用就临时去掉 /MAP 或本地保留**。
2. **挂起**: 不强杀! `GetThreadContext` 直读各线程 RIP (工具脚本思路见 commit e298ebd):
   - rip 在 win32u.dll ≈ GetMessage 泵挂起; ntdll 多线程同址 = 事件等待;
   - 配合 Memory64List 栈回扫可还原调用帧。
3. **探针法** (GUI 子系统无 stdout): fopen 追加写 `C:\pectest\memfail.log`, 逐段插桩二分定位。
   当前源码树里留有六处文件的 `TEMP PROBE` 探针网 (core_main/core_init/core_script2/
   core_scriptrun/core_string/core_var, 全部带注释标记), **T1 用完后全局搜 TEMP PROBE 移除**。
4. **原版行为对照**: 别猜命令语义! EXE 里 utf-16 字符串扫描可确认命令是否存在 (例:
   WRITE 命令在此版本不存在, exit=2=未知命令; TEXT 是屏幕显示不是写文件)。

## 5. DSH 任务清单 (按序执行)

### T1 — 修复 OOM 弹窗循环 (唯一硬堵点, 一步之遥)

**现象**: `pecmd_msvc.exe LOAD t1.pecmd` (内容仅一行 ENVI) 弹"内存不足!"对话框循环
(FUN_1400630D0 mode=2, 用户点掉即退出)。

**证据链** (探针日志 memfail.log):
```
PROBE init: qpc-done          ← InitEnvironmentVars 走到这里都正常
OOM str.c site=1 size=0x172de9bfee4 ptr=NULL    ← site=1 = PECMD_HeapRealloc(NULL分支)
```
size ≈ 当时堆指针值 → **某个未初始化/错位的值被当成分配长度**。

**锁定范围**: qpc-done 与 ver-vars-done 之间只有 core_init.c:93-99 (PECMDVER 变量设置),
即变量写入链 `FUN_1400629B8 → FUN_14001E6BC (@0x14001e6bc, core_var2.c:257) → PECMD_NewVarNode (core_var2.c:96)`。

**头号嫌疑** (未验证, 先看这里): `FUN_14001E6BC` 中 `uint64_t len;` 未初始化,
当 `caplen >= 0` 且 key 非 `::` 前缀时 **len 从未被赋值**, 而 else 分支 (node 存在时)
`PECMD_AllocString((WCHAR**)((u8*)node+8), len + 2)` (core_var2.c:327) 直接消费它。
次嫌疑: `PECMD_NewVarNode` 内部同类问题。另外注意 E6BC 大量 `node+偏移` 解引用前缺判空
(calloc 后 node 理论非空, 但 VarLookup 返回的 node 走 else 分支前没判 NULL 以外的东西)。

**修法纪律**: 对照 decompiled.c @14001e6bc 原文核对该路径 len 的真实来源后再补,
不要拍脑袋赋 0 (可能破坏截断语义)。修完跑 t1 → 期望 exit=0 且无弹窗。

### T2 — harness 回捞通道改造 (WRITE 命令不存在)

run_case.py 的 make_epilogue 生成的 `WRITE <file>,<content>` 在本版原版里是未知命令 (exit=2)。
已实测可用通道: `EXEC =C:\Windows\System32\cmd.exe /c echo <content>><file>` (exit=0, 文件落地)。
改造 make_epilogue (注意 cmd 元字符转义), masks.conf 可能要加行尾 \r\n 归一化。
改完跑 `python harness\runners\check_corpus.py` + 单条 run_case 验证双端产物一致。

### T3 — G0 收口 → P1 golden 录制

T1/T2 后: `python harness\runners\run_case.py 001_envi_smoke` 双跑 → diff_case 应 NO-GOLDEN →
`--exe orig --record-golden` 晋升 → 复跑判 PASS。G0 门禁才算真达成。
然后 `--all` 全量录 golden (30 条)。

### T4 — P2 全量对拍分诊

diff_case --all → report.py。FAIL 按 HANDOVER v4 循环分诊 (桩缺失→补真体/失真→点状重写/flaky→隔离)。
已知情报: 原版是 PE 环境程序, 部分 API 行为 (注册表/服务/网络) 在普通桌面 Windows 与 PE 有差,
差异先记 docs/divergences.md 不要硬凑。

### T5 — 移除全部 TEMP PROBE

`grep -r "TEMP PROBE"` 六个文件清干净, 回归 syntax 门。memfail.log 写文件逻辑若想保留成
常驻诊断, 统一收编成一个 PECMD_DiagLog(tag) helper 再说, 不要散着。

### T6 — BLACKBOX 46 条探针用例 (docs/triage_map.md 有现成清单) + P3/P4

照 v4 文档推进, 无新增情报。

## 6. 纪律红线 (沿用 v4, 血泪强化两条)

1. 每阶段 commit; 补全必须携带"驱动它的失败证据"
2. **双绿门**: syntax 全绿 + 构建 exit 0, 缺一不可 (本轮多次只绿一半就往下冲, 回头还债)
3. **别猜原版语义** — 我们手里有三重真源: decompiled.c 原文 / 原版 EXE 二进制 (字符串/资源/反汇编) /
   真机对照跑。CS 初始化缺失和 WRITE 不存在都是"对照原文/EXE"才定案的, 猜的方向全是错的
4. 不虚构语义: 无法核验 → 登记 divergences
5. 单编辑者纪律 (桩文件); 改动最小化 (每行 diff 能追溯到任务)

## 7. 关键文件地图 (新增部分)

```
tools/msvc_build.bat        # 构建入口 (/MAP 输出 build\msvc\pecmd_msvc.map, 已 gitignore)
tools/debug/parse_dump.py   # minidump 异常解析 (§4-1 的坑都已写进代码注释)
tools/debug/thread_rip.py   # 挂起进程线程 RIP 直读
compile_flags.txt           # clangd 配置 — 消除编辑器 'pecmd_defs.h' not found 误报
.agents/skills/             # DSH 项目级 skills ×4: pecmd-build / msvc-compat /
                            #   win-crash-triage / pecmd-semantics (即装即用, 热发现)
.dsh/cordis.yml             # MCP 插件配置 (memory server)。⚠️ 不自动发现:
                            #   启动时 dsh --config .dsh\cordis.yml 显式指定
include/win32_stub.h        # src/** 用头; PECMD_LI/FT + __debugbreak 垫片在此
include/stubs_common.h      # 桩四件套用头; PECMD_LI/FT 另一份
src/kernel/core_globals.c   # ★ .CRT$XCU CS 初始化器 (原版 FUN_14011a8b0 等价物)
src/app/core_main.c         # main 入口 (原版 @0x140017034); MainW 主循环
src/app/core_init.c         # InitEnvironmentVars (OOM 发生地, 探针密集区)
src/runtime/core_var2.c     # ★ T1 目标: FUN_14001E6BC / PECMD_NewVarNode / FUN_14005D534
unimplemented_stubs.c       # 空桩池; RunBootScriptInFiber 已改为转发真体
harness/runners/*.py        # 四脚本已 GBK 控制台兼容; epilogue 待改 (T2)
```

## 8. 给 DSH 的三句话

1. 你接手的不是一个"坏在半路的工程", 而是一个**编译链全通、运行链只差最后一个未初始化变量**的工程 —
   T1 就是临门一脚, 证据全在 §5-T1, 别重新排查。
2. 所有 MSVC 兼容坑都已在 §3 表格化, 再遇到先查表再动手。
3. 每次构建后记得 `Copy-Item build\msvc\pecmd_msvc.exe C:\pectest\ -Force`,
   并且先 `Stop-Process pecmd_msvc` — 它经常挂着消息泵占着 exe 锁。
