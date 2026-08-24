# HANDOVER_WIN.md — WIN 端作战手册 (v6.2, DSH 中途交接版)

> 2026-08-25 更新｜v6.1 → v6.2: **T1b 闭环 + 连环修复 T1c/T1d/T1e 全部落地并验证**,
> t1 冒烟 exit=0x0 全程跑通 (LOAD→解析→ENVI→退出), G0 实质达成。详见 REVIEW §130。
> **读法**: §1 看状态 → §5 从 T2 开工; §3/§4 遇同类问题再回来查。

---

## 1. 状态一句话

**运行时连环坑 T1→T1e 已全部闭环** — t1 冒烟 exit=0x0 全程跑通:
堆句柄双符号归一(T1b) → 分配器头写序 6 处归正(T1c) → 引用计数块两函数按原文重写(T1d)
→ ArgTokenize 直移重写(T1d2) → RunScriptText 所有权移交防 double-free(T1e)。
全部有 dump 反汇编/探针/原版字节级证据, 明细见 REVIEW §130。

| 阶段 | 状态 |
|---|---|
| P0 MSVC 化 | ✅ (v5 已达成) |
| G0 门禁 | 🟢 **冒烟 exit=0 跑通** (t1 单命令级); 全量 30 例待 T3/T4 |
| P1 验证闭环 | ⚠️ 半程: run_case.py 协议通; epilogue WRITE 通道待换 (T2) |
| P2 对拍/分诊 | 🔴 运行时 bug 连环修复中 (本轮 4 连修, 见 §3) |
| P3/P4 | ⬜ 未动 |

### 本轮 (v6) 战果清单 — 全部对照 decompiled.c 原文定案后修改

| # | 文件 | 缺陷 | 根因/证据 | 结果 |
|---|---|---|---|---|
| 1 | src/runtime/core_var2.c | FUN_14001E6BC 五处失真: len 未初始化; 负值编码在 caplen=-1 覆盖后才解码且只放 :: 分支; 重算守卫误为 caplen<0; :: 与 amp>1 的 scope 优先级反 | 原文 @18137-18219: uVar7 初值 -1, `if(param_4<-0xf)` 最先解码, 守卫是 `(longlong)uVar7<1` | 按原文逐行还原 |
| 2 | src/runtime/core_var.c, src/commands/core_b3l.c | FUN_14001E6BC 第 4 参声明为 `int`, 定义是 `int64_t` | x64 MSVC 按 int 传参清零 r9 高半位 → 被调方读到 caplen=0xFFFFFFFF(>0) → 跳过重算消费野 len → OOM 弹窗 (memfail.log site=1 size≈堆指针值实锤) | 统一 int64_t |
| 3 | src/runtime/core_proc.c | PECMD_GetParentProcessId 缓冲 uint64_t[5] 却 memset/查询 0x30 字节 | 踩 /GS cookie → __report_gsfailure (0xC0000409); 原文 @3436 是 local_38[5]+local_10 共 6 qword, 取值 +0x28 InheritedFromUniqueProcessId (旧码误取 info[4]) | 改 uint64_t[6] 取 [5] |
| 4 | src/app/core_main.c | main() 把 `(WCHAR*)argc` 当 cmdline 透传 | 原版 @0x17034 字节 `48 8B D1(mov rdx,rcx) 33 C9(xor ecx,ecx) E9(jmp mainB)` = mdyblog 改版 CRT 启动器把**宽命令行参数尾经 RCX** 传入 main; 原生 UCRT 传的是 argc! lstrlenW((WCHAR*)2) 内部 SEH 吞错返回垃圾长度 → memmove AV | 自行从 GetCommandLineW 引号敏感地取参数尾; hInstance 用 GetModuleHandleW(NULL) |
| 5 | src/lang/core_script2.c | RunStartupScript 头部写作 `PECMD_GrowByteBuffer(NULL,0)` — ps=NULL 必炸 | 原文 @45530: `local_res18=FUN_140024c48(&local_res18,0,5)` 取首 token + `FUN_1400170b0(&local_res18)` 跳空白 (与 FUN_14005b154 同体) | 按原文还原头部 |

**冒烟证据** (C:\pectest\memfail.log, 同一条 `LOAD t1.pecmd`):
```
v5 之前: qpc-done → OOM str.c site=1 size=0x172de9bfee4 → "内存不足!" 弹窗循环
v6 现在: heap-init → InitEnvVars 全通过 (ver-vars/shellfolders-done)
        → StartupScript enter → RunCommand enter → before SrParsePrefix → (T1b 崩点)
```

---

## 2. 环境 (已就绪, 勿重装)

- VS 2022 Community + Windows Python 3.14。构建:
  ```bat
  cmd /c "chcp 936 >nul & call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 & tools\msvc_build.bat"
  ```
  **⚠️ 必须 chcp 936**: msvc_build.bat 用 cmd echo 写 sources.rsp, 编码随控制台代码页;
  pwsh 默认 UTF-8 下 rsp 里中文路径变 UTF-8 字节, cl 按 GBK 读 → C1083 找不到源文件
  (报错形如 `PECMD鍙嶇紪璇慭`)。GBK 控制台则无此问题。syntax 门同理。
- `C:\pectest\`: PECMD.EXE (=PECMD原始.EXE) + pecmd_msvc.exe + t1.pecmd (`ENVI T1VAR=hello`)
- 构建后部署: `Stop-Process pecmd_msvc` 先杀 (常挂消息泵占 exe 锁) → Copy 到 C:\pectest\
- WER LocalDumps → C:\pectest\dumps (DumpType=2 全量); **dumps 会积累, 定期清理**
- git 身份 repo-local: daiaji
- **MCP 状态 (v6.1 实测)**: 会话中曾成功使用 GhidraMCP (项目 `PECMD`, 程序 `/PECMD.exe`
  = 原版, x64 base 0x140000000, 2726 函数已分析) 完成 xref/decompile 取证; **重启 DSH 后
  ghidra-mcp 从工具列表消失**, windbg-mcp 一直未挂载。下任若需继续字节级取证, 先确认
  MCP 配置 (参考 .dsh/cordis.yml) 与 Ghidra bridge 是否重新可见, 不可见时仍可回落
  decompiled.c 原文 + 自研 dumpbt.py (§4) 完成同类工作。

## 3. 技术档案 — 运行时连环坑 (v6 新增, 遇同症直接套)

| 症状 | 根因 | 手法 |
|---|---|---|
| OOM 弹窗循环, size≈堆指针值 | E6BC 野 len (表#1/#2) | 对照原文还原 + 原型统一 |
| 退出码 0xC0000409 (__report_gsfailure) | 栈缓冲比 memset/查询长度小 8 字节, 踩 GS cookie (GetParentProcessId 例) | 数组尺寸按原文变量合计; Ghidra 相邻局部可能是同一结构被拆开 |
| 一启动就 AV 在 memmove/lstrlenW | 原版改版 CRT 经 RCX 传宽 cmdline 尾, 原生 UCRT 传 argc | 见 §3 表#4; 凡"反编译显示 arg 复用/寄存器残留"的入口都要用字节级核对 |
| ps=NULL 必炸的辅助函数被显式传 NULL | 还原者臆造的调用 (GrowByteBuffer(NULL,0)) | 回原文找该位置真实调用序列 |
| ntdll 内 AV, rcx=小整数, `[rcx+0x10] cmp 0xddeeddee` | 无效堆 HANDLE 传入分配 API (RtlAllocateHeap 校验 _HEAP+0x10 魔数) | 当前堵点 T1b, 见 §5 |

## 4. WIN 调试工具箱 (v6 增补)

1. **dumpbt.py (新, 已入库 tools/debug/, 权威版本)**: minidump 异常流+模块+Memory64List
   栈回扫+map 符号一站式:
   ```bat
   python tools\debug\dumpbt.py C:\pectest\dumps\<最新>.dmp
   ```
   输出异常码/fastfail 子码/RIP 符号化/寄存器/栈上 pecmd_msvc 返回地址候选。
   坑位备注: MINIDUMP_EXCEPTION.ExceptionAddress 在流内 offset 24; ThreadContext Rva
   直指 CONTEXT, RIP 在 ctx+0xF8; MSVC map 第三列是裸 hex (无 h 后缀)。
   ⚠️ 旧 parse_dump.py 曾出现"文件被回退成旧版"的现象, 以 dumpbt.py 为准。
2. **ntdll/kern32 崩点定性**: 从 dump Memory64List 直接读 RIP 前后字节 capstone 反汇编;
   再拿 C:\Windows\System32\ntdll.dll 导出表二分定位函数名 (脚本见 git 历史/本轮会话)。
3. **map 符号 ↔ 运行地址换算**: 模块基址(从 dump ModuleList 取) + map RVA;
   全局变量当前值可直接用 Memory64List 读出 (本轮读 DAT_14013d328/g_hHeap 实锤用此法)。
4. **探针法**: memfail.log 探针网仍在 (core_main/core_init/core_script2/core_scriptrun/
   core_string/core_var, 带 TEMP PROBE 注释)。**T1b 未闭环前不要拆** — 本轮全靠它收窄。
5. WER LocalDumps / GetThreadContext 直读 RIP / 原版 EXE utf-16 字符串扫描: 同 v5 §4。

## 5. DSH 任务清单 (按序执行)

### ✅ T1b/T1c/T1d/T1e — 已全部闭环 (2026-08-25, 见 REVIEW §130)

历史档案: T1b 曾为"堆句柄双符号"堵点; 本轮连环修掉 T1c(分配器头写序×6)/
T1d(AdoptRefCountedString+RefCountRelease 重写)/T1d2(ArgTokenize 直移重写)/
T1e(RunScriptText 所有权 double-free)。以下原始记录保留供考古。

#### 原始记录 — T1b — 堆句柄双符号归一

**现象**: t1 冒烟崩在 SrParsePrefix 入口附近, RIP 在 ntdll 分配路径内
(RtlAllocateMemoryBlockLookaside 导出区间 +0x2e), 指令 `cmp dword ptr [rdi+0x10], 0xddeeddee`,
rdi=rcx=**0x13** → 分配 API 收到无效堆 HANDLE。

**已实锤** (dump 读全局, 方法见 §4-3, dump: pecmd_msvc.exe.12168.dmp):
```
DAT_14013d328 (map: 0x1402b8fb8, unimplemented_stubs.obj) = 0x0    ← 从未被赋值!
g_hHeap      (map: 0x1402c1850, <common>)             = 0x250fc340000 ← 正确进程堆
```
原版只有一个全局 DAT_14013d328。还原工程裂成两个符号:
- core_globals.c 定义并初始化 `g_hHeap` (pecmd_defs.h 注释明说 "DAT_14013d328 -> g_hHeap")
- unimplemented_stubs.c 又定义了独立的 `void *DAT_14013d328 = 0;`,
  stubs_common.h extern 它 → **restored_bodies.c 约 12 处 HeapAlloc/HeapFree**
  (AllocStrSlot@7318 / FreeStrBuf@7119 / AllocWStringBuffer 族) 全部用未初始化句柄!

**原版 Ghidra 交叉引用增补 (v6.1 新证据, 更强定案)**:
DAT_14013d328 在原版恰有 **2 个写入点**:
- `mainW @0x140009d12` → `GetProcessHeap()` (启动后)
- `FUN_1400051b4 @0x140005213` → `if (DAT==0) DAT=GetProcessHeap()`, 由
  `dllMain_Name@0x6a24` 在 **DLL attach 阶段 (main 之前!)** 调用 — 空守卫提前初始化,
  保证所有分配器从进程一起跑就有合法句柄。约 50 处读取点全为分配器。
**启发**: 还原项目里"唯一性工作" (单例句柄/标志) 值不值得用 Ghidra 查一遍 xref 全集
来判断有没有同型分裂 — 本轮双符号就是靠 xref 才 100% 定案, 不再停留在"基本锁定"。

**修复实施状态 (v6.1: 2/3 已完成, 未验证构建)**:
| 步骤 | 状态 |
|---|---|
| 1. stubs_common.h:553 改为 `#define DAT_14013d328 ((void *)g_hHeap)` + `extern HANDLE g_hHeap;` | ✅ 已改 (工作区) |
| 2. unimplemented_stubs.c 删除 `void *DAT_14013d328 = 0;` 独立定义 | ✅ 已改 (工作区) |
| 3. core_globals.c `PECMD_InitCriticalSections` 开头补 `if(!g_hHeap) g_hHeap=GetProcessHeap();` (对应 attach 守卫) | ⏸️ **未做** (可选加固: 当前 MainW/InitEnvVars 也会设, 但非 main 前) |
**接手第一步**: `git status` 确认两文件改动在 → 重建 (§2, chcp 936) → 跑 t1 冒烟。
若绿: 补步骤 3 加固后再提交; 若仍崩: 重跑 dumpbt 看新 RIP — rcx 若仍是小整数,
用 §4-3 读调用点 (rsp+0x8 返回地址) 所属 pecmd_msvc 符号再定位; 0x13 精确来源
(疑为 HeapAlloc 收到 0 句柄后 Aslr/fastpath 传参混淆) 尚未闭环, 不必预设。

**验收**: t1.pecmd exit=0 无弹窗; memfail.log 出现 SrParsePrefix 之后的进度标记。

### T2 — harness 回捞通道改造 (WRITE 命令不存在)
同 v5: make_epilogue 改 `EXEC =C:\Windows\System32\cmd.exe /c echo <content>><file>`,
注意 cmd 元字符转义 + masks.conf 行尾归一; check_corpus + 单条 run_case 验证。

### T3 — G0 收口 → P1 golden 录制
001 双跑 → NO-GOLDEN → `--exe orig --record-golden` → PASS; 然后 --all 录 30 条。

### T4 — P2 全量对拍分诊
diff_case --all → report.py; FAIL 循环分诊; PE 环境差异先记 docs/divergences.md。

### T5 — 移除全部 TEMP PROBE
**等 P2 稳定后再做**; 本轮证明探针网是命脉。若保留 memfail.log 作常驻诊断,
收编成 PECMD_DiagLog(tag) helper, 不要散着。

### T6 — BLACKBOX 46 条探针用例 + P3/P4
照 v4 文档推进。

## 6. 纪律红线 (沿用 + 本轮强化)

1. 每阶段 commit; 补全必须携带"驱动它的失败证据"
2. 双绿门: syntax 全绿 + 构建 exit 0
3. **别猜原版语义 — 三重真源**: decompiled.c 原文 / 原版 EXE 字节与资源 / 真机对照跑。
   本轮 4 连修全部靠"先读原文再动手", 入口契约甚至要读到字节级 (@17034 反汇编)。
4. 不虚构语义: 无法核验 → 登记 divergences
5. 单编辑者纪律 (桩文件); 改动最小化
6. **跨 TU 原型必须一致**: MSVC 无跨 TU 类型检查, int vs int64_t 这类分歧不报编译错,
   只在运行时爆 (表#2)。新增 extern 时 grep 全树既有声明核对签名。

## 7. 关键文件地图 (v6 增删)

```
tools/msvc_build.bat        # 构建入口 (⚠️ chcp 936, 见 §2)
tools/debug/dumpbt.py       # ★ minidump 一站式定位 (权威版, 已入库)
src/runtime/core_var2.c     # FUN_14001E6BC 已按原文还原 (T1 核心)
src/runtime/core_var.c      # 62a2c/629b8 包装族 (原型已修正)
src/runtime/core_proc.c     # GetParentProcessId 已修 ([6] qword, 取[5])
src/app/core_main.c         # main() 入口契约已按字节级证据重写
src/lang/core_script2.c     # RunStartupScript 头部已按原文还原
restored_bodies.c           # ★ T1b 主战场: ~12 处 HeapAlloc/Free 用 DAT_14013d328
unimplemented_stubs.c:452   # ★ T1b 元凶之一: 多余的 DAT_14013d328 定义
include/stubs_common.h:553  # ★ T1b: extern DAT_14013d328 的传播源头
include/pecmd_defs.h        # 注释明确 DAT_14013d328 -> g_hHeap 应为同一变量
harness/runners/*.py        # 四脚本; epilogue 待改 (T2)
HANDOVER_WIN.md             # 本文
```

## 8. 给下任 DSH 的三句话

1. 你接手的工程运行链已经打通到**脚本命令解析门口** — 初始化全绿, 只剩 restored_bodies
   里那批用错堆句柄的分配器 (T1b), 修法已给到行号, 别重新排查。
2. 所有新症状先查 §3 表格; 动手前必读 decompiled.c 对应函数原文, 入口类还要看字节。
3. 构建/部署/调试三件套命令都在 §2/§4, 探针别拆, dumps 记得清。


---

## 9. 【v6.3 追记, 2026-08-25】Round130 结论修正 + S7 插队

- **推翻"G0 实质达成"**: t1 exit=0 系空转(动词从未分发)。证据与影响面见 REVIEW §131。
- EXEC 语法矩阵已实测(原版 OK / msvc 静默) —— T2 改造方案本身可行, 但被 S7 阻塞。
- **S7(新, 最高优先)**: 直移 FUN_14004eb34 完整分支树(重点 LAB_14004efda 启动分发),
  替换 core_script2.c 简化版; 验收=t1 的 ENVI 真实执行并经 EXEC 通道回捞到 %T1VAR%。
- S7 绿后按序恢复 T2→T3→T4。
