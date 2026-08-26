# DEBUGGER 活调试打通 — 交接文档

> 2026-08-25 会话结尾生成 | 给下一个 AGENT 继续
> 主题：把 Ghidra MCP 的 `debugger_*` 22 个工具（dbgeng 活调试）跑起来，用于 pecmd_msvc.exe 崩溃的活体定位。
>
> **⚠ R14 起 windbg 操作纪律权威版移至 `refactored\docs\WINDBG_MCP_ISSUES.md`**
> （缺陷 A-H 登记 + 受控实验台账 + SOP 12 条 + 探针优先判据）。本文 §5-§13 中与该专档
> 重复的 windbg 对策行仅作历史留痕，**以专档为准**；新增实战案例直接补录专档，不再在本文展开。

> **✅ 2026-08-25 更新：debugger 服务端已装好并在跑（本节取代 §1/§2 中"服务端未找到"的结论）**
> - 来源：fork **hacr-lab/ghidra-mcp**（上游拆分前的快照，完整保留 `debugger/` 包含 `d2/conventions.py`），经反代克隆到 **D:\tools\hacr-ghidra-mcp**
> - 依赖：Python 3.14.7 + `pip install -r requirements-debugger.txt`（pybag==2.2.16、comtypes==1.4.16，PyPI 直连可用）
> - 启动：`cd D:\tools\hacr-ghidra-mcp && python -m debugger --port 8099 --host 127.0.0.1`（dbgeng COM 初始化 OK，用 System32 的 dbgeng.dll）
> - 验证：`mcp__ghidra__debugger_status` → `{"state":"detached",...}`、`debugger_list_breakpoints` → "Not attached to any process"（业务语义错误），端到端已通；桥无需重启
> - 已知缺口：未传 `--exports-dir`（无 dll_exports 目录），`debugger_resolve_ordinal` 暂不可用；服务器进程由 DSH 会话后台任务托管，会话结束后需按上面命令手动重启

---

## 1. 当前结论（一句话）

**Ghidra 静态桥正常；debugger_* 工具已注册但代理目标（独立 dbgeng 服务端 :8099）未运行 ⇒ 一调就报 `Debugger server not running at http://127.0.0.1:8099`。服务端不在 pip 包里，在上游另一仓库，尚未找到可用的那份。**

## 2. 环境事实（已核实，勿重查）

| 项 | 值 |
|---|---|
| Ghidra 静态桥 | `bridge-mcp-ghidra.exe` (PID 14140) ↔ Ghidra 插件 (:8089, PID 5424)；项目 PECMD，程序 /PECMD.exe。静态工具（decompile/xref/set_xxx/dumpbt 配套）全部可用 |
| debugger 工具 | 22 个 `debugger_*` 已注册且 check_tools=callable，但代理到 `http://127.0.0.1:8099`（config.py:50 `GHIDRA_DEBUGGER_URL`，默认即 8099）——服务端不跑必报错 |
| 服务端形态 | 报错提示 `python -m debugger` / `uv run python -m debugger`；ghidra-mcp README:404 写明 2026-08-11 起该服务端**移到 "d2-game-exe" 仓库**（CLAUDE.md 提到其内部 `d2/conventions.py`，即带 D2 调用约定层）；flag：`--port 8099 --host 127.0.0.1 --exports-dir <dll_exports> --log-level` |
| 本机缺口 | 无 uv；无 cdb/windbg（SDK Debuggers 未装）；pip 包 `bridge_mcp_ghidra 6.0.0` 只含桥 |
| GitHub 直连 | TLS 握手失败（schannel）被墙；**用户提供反代可用：`https://rp.erojk.eu.org/https://github.com/<owner>/<repo>.git`** |

## 3. 已做的尝试（避免重复）

1. `git clone --depth 1` + 反代 → **D:\tools\ghidra-mcp**（bethington/ghidra-mcp，桥源码+src/ Java 插件；python/ 下只有桥，无 debugger 服务端；无 dll_exports 目录）。
2. 猜 `bethington/d2-game-exe` → 反代 404（git 报需用户名，即仓库不存在）。
3. web_search 确认实际仓库名 **bethington/game-exe** → 已 clone 到 **D:\tools\game-exe**（浅克隆，main=560024c）。
   它是 **C++ 版 D2 Game.exe 重实现**（Client/Game CMake 工程），主分支**没有** python debugger 包（无 debugger/ 目录、无 __main__.py、无 d2/conventions.py）；远程 `ls-remote --heads` 显示**只有 main 一个分支**。

## 4. 下一步（按序）

1. **定位真正的 debugger 服务端**（最关键，需想办法）：
   - 向用户询问 "d2-game-exe" / 带 `d2/conventions.py` + `python -m debugger` 服务端的仓库确切地址（可能私有或改名）；
   - 或：GitHub API 经反代搜 "d2-game-exe"/"ghidra debugger dbgeng"（`https://rp.erojk.eu.org/https://api.github.com/search/repositories?q=d2-game-exe`——若反代只转 https，可用 curl/Invoke-RestMethod 试）；
   - 或：`game-exe` 仓库历史分支/tag（`git ls-remote --tags` 经反代）与 `git log --all` 找曾存在的 `debugger/` 目录。
2. **拿到服务端后启动**（假定在 <repo>\debugger\ 或 <repo> 根含 `__main__.py`/pyproject）：
   ```bat
   cd <repo>
   pip install -r requirements.txt   (或 pip install -e .)
   python -m debugger --port 8099 --host 127.0.0.1 --exports-dir <dll_exports目录> --log-level INFO
   ```
   `--exports-dir` 用于 DLL ordinal→名称解析（对应工具 debugger_resolve_ordinal）；若无现成 dll_exports，可先不传该 flag。
3. **验证**（服务端起后，现有桥默认 URL=8099，通常无需重启桥）：
   - `mcp__ghidra__debugger_status` → 应返回服务端/跟踪信息；
   - 活体试验：`Start-Process C:\pectest\pecmd_msvc.exe -ArgumentList 'LOAD','C:\pectest\t2probe.pecmd'` 后 `debugger_attach 'pecmd_msvc'`；或直接 `debugger_launch(executable_path=C:\pectest\pecmd_msvc.exe, args='LOAD C:\pectest\t2probe.pecmd', cwd=C:\pectest)`。
4. **断点选址**（对应当前主线崩溃，map 在 `D:\repo\PECMD反编译\refactored\build\msvc\pecmd_msvc.map`；运行地址 = `debugger_modules` 里 pecmd_msvc base + map RVA；用 `debugger_set_breakpoint_2`(裸运行地址)）：
   - `PECMD_ProcessScriptBlock` 入口（RVA 查 map，restored_bodies.c:5726 定义）；
   - `PECMD_DispatchExpressionBlock`（restored_bodies.c:7682）；
   - `PECMD_ExecCmdDispatch`（restored_bodies.c:253）；
   - 当前活体崩溃点：**FUN_1400E7D58 内 memmove 巨长度（r8≈0x1AA3AC12）**，调用链（栈候选，现场为准）：RunCommand+0xc68 → FUN_14006F884+0x42 → PECMD_HeapRealloc+0x34。

## 5. 主线任务状态速览（S/E 系列，供断点/继续工作参考）

- 已提交：S7 启动分发直移(e727b25)、掩码极性归正(139fba1)、S8 行执行器+12 影子桩(ef1e9c2)、T4 首轮基线(a37b175)、文档 §130-§134/HANDOVER v6.5(bdab251 等)。
- 当前验收门 A 失败：t2probe 不产出 vars/done。最新活体证据（本轮探针）：
  - `[PSB]` 只收到一次 `line=LOAD C:\pectest\t2probe.pecmd` —— **ENVI/EXEC 行从未到达 ProcessScriptBlock**；随后在 RunCommand 变量路径崩溃（FUN_1400E7D58 巨长度 memmove）。
  - `[ECD]`（ExecCmdDispatch 入口探针）从未触发。
  ⇒ 查链方向：LOAD 动词后的内容怎么进入执行环（g_pNextCmd 主循环/嵌套 RunScriptText→DispatchExpressionBlock→…），以及 FUN_1400E7D58 巨长度来源。
- 未提交 WIP（故意保留，供继续调试）：`src/lang/core_script2.c` 无关；**`restored_bodies.c`（[PSB]/[ECD] 探针 + 手写 CRT extern fopen/fprintf/fclose）、`src/app/core_main.c`（[MAINLOOP] 探针）** 未提交。
- 工具文件：`C:\pectest\_mkt2.py`（生成 t2probe.pecmd）、`C:\pectest\_s8_ref.txt`（FUN_140003a20/1400b1724 原文）、`C:\pectest\_s9_ref.txt`（FUN_14006159c 原文）、dumps 在 `C:\pectest\dumps`。

## 6. 纪律提醒（沿用）

- 构建必须 `chcp 936` + 看 `[msvc_build] OK` 行（失败会删旧 exe，Test-Path 会骗人）。
- 不虚构语义；探针网（[S7]/[S8]/[PSB]/[ECD]/[MAINLOOP]）过门后统一拆（T5）。
- 子代理提示词必须附 `refactored\tools\subagent_autonomy_clause.md`；单编辑者纪律；每步保构建绿。

---

## 7. 【Round 5 更新】windbg MCP 已验证可用 + ResDecode 槽修复 + 下一步焦点

### windbg MCP 实测结论 → **权威版已并入 `refactored\docs\WINDBG_MCP_ISSUES.md`（A-H 缺陷+SOP），以下仅历史首证**
- ✅ 可用: launch(带初始断点)/set_breakpoint(module+RVA 表达式)/go 命中/registers/read_memory/open_dump/backtrace(**真实 unwind, 远胜栈扫描**)
- ⚠️ 不稳定: 多断点连续 go 偶发 0x8000FFFF→引擎 worker 半死(后续命令全 0x80040205)。~~对策: 每次追踪用全新 session, 单断点单放行~~ → 专档 A/E 修正: 毒剂是 execute 控制类命令与内嵌命令串断点, 非断点数量; dump 会话占 PDB→构建前 end_session(专档 C)
- 推荐工作流: 崩溃类问题直接 open_dump(WER dumps 在 C:\pectest\dumps)+backtrace; 活体追踪用 launch+单断点

### 活调试战果(已落代码, commit 48ff9c6)
1. 真实调用链: MainW→StartupScript→RunScriptText→DispatchExpressionBlock→ProcessScriptBlock→RunCommand(+0xc68)→FUN_14006F884→…→FUN_1400E7D58(memmove AV, r8≈0x1AA3AC12)
2. **根因修复**: RunCommand 变量路径调 ResDecode(FUN_1400E7D58) 前漏写长度槽——原文 dc:30309-30311 先存 local_208=lstrlenW*2 到槽相邻+8, ResDecode 读 param_1[1] 当字节数; 移植版裸指针导致读栈垃圾。已按原文补 resSlot[2]={ptr,len*2} 并回取指针
3. [PSB] 探针实证: ProcessScriptBlock 只收到 boot 行一次; **ENVI/EXEC 行从未到达**

### 下一步焦点(S9' 修订): 核对 ProcessScriptBlock 移植完整性
- 原文: dc:43816-45163 (1348 行, size=8717) —— 内含**内联动词级联**(ENVI/EXEC/...无字符串字面量, 靠 DispatchBuiltin 表 miss 后的 if 级联)
- 怀疑: restored_bodies.c:5726 起的移植版可能不完整(动词分支缺失 ⇒ DispatchBuiltin miss+ExecCmdDispatch 也无处可去)
- 方法: 逐段对照 dc:43816-45163 与 restored_bodies.c:5726-? 的分支数; 缺失动词按原文补
- 另: FUN_140003A20 大写三处错链(core_b3m/core_cmd4/core_b3i)仍待修


---

## 8. 【Round 6 决定性发现】动词级联 = PSB 内 4字符 DWORD switch（无字面量）

### 为什么全文搜不到 ENVI/EXEC
原版 ProcessScriptBlock(dc:43816-45163) 的动词分发是**首4字符打成 u32 的 if 级联**，
例(dc:44689-44724): `local_158==0x4e574f44(DOWN)/0x4d495444(DDTI)/0x4d524f46(FORM)/
0x4e49414d(MAIN)...` —— 源码里永远搜不到 "ENVI" 字面量。此前所有按名字 grep 的
排查全部失效，这就是多轮"找不到分发点"的根本原因。

### MAIN 分支(=boot "PECMD ..." 行的真正处理器)
```
if first4=='MAIN' (0x4e49414d):
    ExecLine(...) 可选日志
    跳过 '*' 前缀; 若参数非空:
        g_pNextCmd = "PECMD " + LVar11      # 排队给主循环 (dc:44717)
        script+200 = 2; goto 主循环重入
    else:
        FUN_140045c90(&g_Script, LVar11+8)  # ★脚本装载器吃参数尾 (dc:44724)
```

### 移植版差距
restored_bodies.c:5726 起的 PSB 移植体只含 DispatchBuiltin→ExecCmdDispatch 与
ON/OFF 少量分支，**未见该 4字符 switch 级联**(含 MAIN→FUN_140045c90 这条 boot
生命线)。这解释了 [PSB] 只收到 boot 行、ENVI 永不执行的完整因果链。

### S9'(修订版)任务书
1. 提取原文 dc:43816-45163 全文(1348 行)，与 restored_bodies.c:5726 起的移植体
   逐分支对照；
2. 补齐缺失的 4字符动词级联(至少覆盖语料所需: MAIN/LOAD/ENVI/EXEC/FIND/CALC/
   REGI/SUB/TEAM/IFEX/WRITE/FILE 族；各 case 的 DWORD 值由动词名小端拼出,
   如 LOAD=0x44414f4c)；每个动词的处理函数在工程内均有已还原真体(rename_map
   /FUNC_NAMES 可查)，禁止臆造；
3. 验收门不变: t2probe 产出 vars/done(内容 CASE=t2probe|A|B / OK)+t1 exit=0+
   corpus 28/28；过门后跑 T4 全量对拍记录 pass/fail。
4. 工具: windbg MCP 已可用(launch/bp/regs/read/backtrace 实测 OK；注意 map RVA
   必须取第三列 VA−0x140000000，第一列 offset 会差 0x1000 导致断点错位炸引擎)。

### Round 6 其他已提交
- ResDecode 长度槽修复(commit 48ff9c6): scriptrun 变量路径漏写 {ptr,len} 槽的
  len*2 → memmove 巨长度 AV(windbg dump 双实锤) —— 该 AV 已消除。


---

## 9. 【S10 落地 + 新崩点精确坐标】(Round 6 尾)

### 已修复提交 4b21ec2
1. S10: RunCommand 按 dc:29635-30332 重构(token 分类/裸路径→读文件执行/盘符探测/mem·map·资源归位) —— LOAD→读文件→逐行 PSB 链实证打通([DEB]/[PSB] 首次收到 ENVI/EXEC 行)
2. ExpandEnvVars %VAR 扫描死循环(core_execline.c:814 补 ch 重读)
3. DAT_14013e190 双符号分裂(restored_bodies 约50处锁零置桩 → 全部归一 g_csInit)

### 当前唯一崩点(windbg dump 实锤)
```
ECD(+0x71e3) → PECMD_StrBldCopyWide → PECMD_AppendWideStr
             → FUN_140063888(PECMD_StrCopyW 移植, unimplemented_stubs.c)
             → MemMoveSafe AV: src=0x4E3CF1BF(垃圾/截断指针), len=0x24
```
位置语义 = restored_bodies.c SEG6(cmd.exe 探测段 :2606-2612): StrBldCopyWide(&DAT_14013ca90, SystemDir) + AppendWideStr(&DAT_14013ca90, "\System32\cmd.exe") 构建命令处理器全路径。疑点: DAT_14013ca90 的声明类型(stubs_common.h)与用法(==0 判空/作为串槽)存在类型混淆; FUN_140063888 移植体本体已对照 dc:60952 核对无误——上游传入 src/b 即坏。
另注意 [ECD] line=(null): ECD 曾被以 NULL line 调用一次(调用方待查)。

### 下一步
1. 查 DAT_14013ca90 声明类型与 SEG6 用法是否匹配原文(dc:8054-8075 附近);
2. FUN_140063888 的调用方(AppendWideStr 内部)传参链;
3. ECD NULL-line 调用方定位(bp pecmd_msvc+0x1aae20 + rdx 检查);
4. 过门后 T4。


---

## 10. 【Round 8】FUN_140063888 垃圾源指针崩点 — 证据与最短路径

### 可靠事实（windbg dump: pecmd_msvc.exe.18308 前后同签名）
- AV 发生在 `MemMoveSafe`(memcpy) 内，src≈0x4E3CF1BF（非堆形态的垃圾/截断指针），len=0x24
- 栈候选链：`FUN_140063888+0x9f → PECMD_AppendWideStr+0xaf → PECMD_StrBldCopyWide+0x24 → ECD+0x71e3`
- 语义位置 = restored_bodies.c SEG6 cmd.exe 路径构建(:2606-2612)：
  `StrBldCopyWide(&DAT_14013ca90, SystemDir)` + `AppendWideStr(&DAT_14013ca90, "\System32\cmd.exe")`
- FUN_140063888 移植体(unimplemented_stubs.c, S8 直移 dc:60952-60968)本体已核对无误
- DAT_14013ca90 声明 = `extern uint64_t`（标量）——与槽用法(==0判空/&取址/值当串指针)自洽

### 教训（Round 8 两次误判记录）
1. map 第一列(section offset)≠RVA！真 RVA=第三列 VA−0x140000000。ECD 真入口 RVA=**0x1aae20**
   （第一列 001a9e20 会差 0x1000，断点下错位置→int3 打烂执行流→引擎"灾难性故障"假象）
2. 静态反汇编手算 RVA 同样易错——**活体断点+寄存器读数才是权威**

### 最短路径建议（下一代理）
不要再用 windbg 断点追（引擎不稳定）。~~改为**源码级探针**~~：
**【R19 裁定更正 2026-08-26】本条"引擎不稳定"系断点地址未随六轮重建更新所致误诊
（见 refactored\docs\WINDBG_MCP_ISSUES.md R19 裁定附录）；活体断点实测全部可用。
现行纪律=V-Gate 五道门（同文档 R20 版本锁定纪律）：地址只从当期 symsnap/map 派生、
下点三验证、阳性对照先行。探针仍适用于参数核验类问题（V4 探针透明化）。**
以下历史建议保留：
1. 在 FUN_140063888(unimplemented_stubs.c) 入口加 fprintf 记录 (a, b, c) 三参；
2. 在 restored_bodies.c SEG6 :2609/:2610 两处调用前各加一行探针打印所传 src 串内容；
3. 重跑 t2probe → memfail.log 对比哪一层先出现坏指针 ⇒ 定位是
   (a) StrBldCopyWide 返回后 DAT_14013ca90 槽即坏, 还是
   (b) AppendWideStr 的第二参数(src)本身错。
4. 修复方向提示：AppendWideStr≡FUN_14007DE70(core_exec4.c:261 "拼接*a+src")？
   注意 FUN_14007DE70 与 AppendWideStr 是否同一函数/签名是否一致——S8 报告曾提
   e7d58/07de70 双地址易混问题同类。

### 门A 当前状态
LOAD→读文件→逐行 PSB 分发已通([DEB]/[PSB] 收到 ENVI/EXEC 行)；%VAR 死循环已修；
CS 分裂已修。仅剩本崩点 → 过门A 指日可待。


---

## 11. 【Round 9】真实 unwind 拿到 — 崩点收敛至 AppendWideStr/HeapRealloc 交互

### windbg 真实 unwind（dump pecmd_msvc.exe.6664.dmp, base 0x7ff7d4680000）
```
11 MemMoveSafe+0xe2            ← AV
12 PECMD_AppendWideStr+0x102
13 PECMD_ExecCmdDispatch+0x71e3   ← SEG6 cmd.exe 路径构建(restored_bodies.c:2610)
14 ProcessScriptBlock+0x19a6
15 DispatchExpressionBlock+0xe61
16 RunScriptText+0xb6d
17 srx_ExecuteScriptFile+0x2e7    ← S10 新装载器 ✓ 工作
18 RunCommand+0x124f
19 ProcessScriptBlock+0x1b3d(boot行)
```
**意义**: LOAD→读文件→逐行分发→ENVI/EXEC 进 ECD 动词级联——全链已通到 ECD 深处;
崩在 ECD 内部 SEG6 的 cmd.exe 全路径构建(AppendWideStr 追加 "\System32\cmd.exe")。

### AWS 入口探针读数(崩溃该次)
slot=&DAT_14013ca90 ✓ / old=*slot=堆指针 ✓ / src="\System32\cmd.exe" ✓
⇒ 入参全有效; AV 发生在同调用内部: HeapRealloc(old,(n2+1+n1)*2) 之后
MemMoveSafe(p+n1*2, src, n2*2+2) 处。n2=17(len=36=0x24 与 dump r8 吻合)。

### 待查两点(Round 10)
1. **双定义 StrBldCopyWide**: restored_bodies.obj@0x19ec60 与 core_sys.obj@0x108b70
   各有一份(map 同名 f 标记)。ECD 实际链接到哪份? 若是 core_sys 版且行为不同
   (槽约定/分配器), 即为根因。查法: windbg `u pecmd_msvc+0x108b70 L20` vs
   `+0x19ec60` 对比; 或 x 命令看 ICF 折叠。
2. HeapRealloc(core_string.c) 对 ca90 槽旧值的处理: old 值若非本分配器家族
   的数据指针(如被 StrDupAssign/其他写坏), realloc(ptr-8) 读头即乱。

### 快速修复候选(若确认双定义问题): 删 core_sys.obj 冗余副本或统一改名,
让 ECD/SEG6 链路只经 restored_bodies 版(与 dc:60946 FUN_140063720 语义一致)。

---

## 12. 【S11 落地】门A崩点根因=隐式int原型截断; NextToken 空壳桩; 当前堵点前移至 CreateProcess 参数错乱

### 决定性根因(R11 实锤, 推翻 §4 "上游堆污染"假设)
windbg open_dump + .ecxr + `ub retaddr`: 崩点 `call PECMD_HeapRealloc` 后紧跟
**`cdqe`** —— restored_bodies.c 内 PECMD_HeapRealloc 无原型声明, MSVC 按
C4013 "extern returning int" 编译, 64 位堆指针在调用点被截断成 EAX 再符号扩展
(0x17e'd4c8'xxxx → 0xffffffff_d4c8'xxxx)。AWS 入参探针(slot/old/src 全有效)
证明非堆污染。[AWS] 探针+编译警告普查(503×C4013)双通道定位。

### S11 修复面(commit 3391af4)
- include/xproto.h: 267 定义派生原型 + dc 权威覆盖表 + WinAPI 真签名 31 条,
  挂载 stubs_common.h / pecmd_defs.h 尾部; S11_U64ToFT/FTToU64/U64ToLI/LIToU64/
  U64ToULI ABI 桥(x64 8字节结构按值=单 GP 寄存器)。
- 返回型归正: FUN_1400630D0→int(dc:60834)、RandSeedAdvance→uint64(dc:74802)、
  AllocStrSlot 返回槽地址(dc:60843)、PSB 捕获族 uint64 化、ParseNumberToken→int。
- 零参占位桩签名归正 ×13(体仍待移植, T4 分诊); core_sys.c StrBldCopyWide 双体删除;
  **PECMD_NextToken 空壳桩→FUN_140024c48 直移(dc:22133-22210)** — 活体断点
  pecmd_msvc+0x1b3373 实锤 cd0=NULL → StrCmpNIW(-8) AV, 该修复后此崩点消除。
- 工具链(C:\pectest): _s11_extract.py/_s11_gen_xproto.py/_s11_fix_locals.py 可复跑;
  报告: build\msvc\s11_warning_census.md(普查)/s11_dual_body_audit.md(双体裁决)/
  s11_t4_precheck.md(T4 预检)。

### 当前唯一堵点(门A 复跑后的下一个崩点)
EXEC 真启动 CreateProcessW 内 wcslen(0xe) AV。活体 bp pecmd_msvc!PECMD_CreateProcessW
入口实锤十参: rcx=NULL(appname)、rdx=完整命令行 ✓、r8/r9=0 ✓, 但栈槽
inherit=[rsp+0x20]=命令行指针复制品、flags=[rsp+0x28]=宽串"cmd "字节
(0x20646d63_00000000) —— 疑似移植块缺失 dc 上游对 inherit/flags 局部的赋值,
陈旧值入参。取证中: dc:10480-10960(EXEC 三路分发) vs restored_bodies.c ~2560-3560
逐行赋值审计(子代理执行中, 报告将落 build\msvc\s11_exec_launch_audit.md)。
注意: windbg k 的符号名有 ICF 折叠污染(ECD+0x9a43 行实际是 PSB 调 ECD 的 ret),
一切以 disasm+寄存器为准。

### 【R11 终局突破】三层连环根因全落 · 门A 半程达成(commit 0eb37ab)
1. **CreateProcess wcslen(0xe)**: FUN_140102a90 是 no-op 桩, 实为 CRT memset 内联。
   STARTUPINFO.lpTitle 未清零=陈旧栈值 0xe → 内核 wcslen AV。si.dump 探针实锤,
   已改真 memset → **CreateProcess ret=1 gle=0, probe_vars.txt='CASE=t2probe|A|B' 产出**!
   (此前"ECD 参数错乱"系误判: windbg 帧符号漂移 + shadow 槽/±8 偏移误读;
   CPW 十参探针证明包装器收参全净; PSB 审计(s11_psb_launch_audit.md)以 .pdata/
   ILT 字节级证据证伪 'PSB 内调 CreateProcessW'。)
2. **残余崩点**(新 dump): PSB 尾部 dc:45139-141 同款守卫下
   `(**)(param_1+0x50) = local_180` 因 **ctx+0x50==NULL** AV。
   初始化器 = FUN_1400186ac PECMD_InitListHead(+0x50→&this+0x58, dc:13328) **未移植**。
   构造三连 FUN_140017cdc→FUN_1400186bc→FUN_1400186ac 见 dc:82268-73。
3. 定位子代理进行中(报告将落 s11_init_listhead_locate.md): 找 LOAD 路径上
   PSB param_1 对象构造点 → 补三连初始化 → 预期门A 全通。

### 教训追加(v2) → windbg 相关条目已并入专档 D/G/K(见 `refactored\docs\WINDBG_MCP_ISSUES.md`)
- windbg k/ln 的符号标签在相邻大函数间会漂移(专档条目 D); 归属用 map 第三列 VA−base 或
  PDB 双源交叉; 栈参读取必须 dq @rsp L1 先锚返回地址(专档 SOP 11)。
- 探针优于断点猜读: 给包装器加 fprintf 十参+结构 dump 一轮就锁定真凶
  (memset 桩), 比三轮 windbg 会话更快更稳(专档"探针优先原则")。

## 13. 【Round 12】T4 缺陷甲乙丙连环修复 · case001 内容全 PASS · 唯一残余=退出期堆损坏

### 三缺陷定义与修复
1. **缺陷甲**(restored_bodies.c:6297): ECD arg4 打包区 local_150 只清 [0],
   [1..3] 陈旧栈垃圾入 ECD 第 4 参 → 入口门误判解引用野指针 → 28/28 C0000005。
   已四槽全清(dc:44284 意图为 8 字节清零)。构建#1 复跑: C0000005 消失 ✓。
2. **缺陷丙**(构建#1 新暴露): srx_ExecuteScriptFile done: 清理把 GrowByteBuffer
   裸块(HeapRealloc 直返, 无 8 字节头)交给 PECMD_FreeStrBuf(*ps-8) → 回拨 8 字节
   非法 HeapFree → 0xC0000374 堆损坏。此前主链总在到 done: 前 AV, 坏释放从未执行;
   甲打通后首次暴露。已改 `HeapFree(g_hHeap,0,buf)`。
3. **缺陷乙**(方案B 落地): srx_ExecuteScriptFile 原无条件 `rd>>1` UTF-16 视图,
   ANSI/LF 语料被读成乱码单行。按 build/msvc/s11_encoding_sniff_patch.md 实装:
   FE FF→PECMD_SwapBytePairs 整块换序 | FF FE→LE 跳 BOM | EF BB BF→跳 3 |
   其余 MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS) 失败回退 CP_ACP(0);
   convText 即拷即收 + done: 兜底回收。

### 结果证据(构建#2, memfail.log 全链)
chars=259 正确多行解析 → ENVI A/B/OUT/T_CASE 逐行 PSB ✓ →
EXEC#1 CreateProcessW **ret=1 gle=0**(echo CASE=…>out\vars.txt) ✓ →
EXEC#2 **ret=1 gle=0**(echo %T_DONE%>out\done.txt) ✓ → [RSTX] ret=0 → [REL] 干净 ✓
**out\vars.txt='CASE=001_envi_smoke|A|B|OUT' + out\done.txt='OK' 逐字节一致 = 内容全 PASS**
⚠ 产物路径是 `C:\pectest\out\`, 不在用例目录(手测勿再查错)。

### 唯一残余堵点(T4 新坐标)
进程**退出阶段**堆校验 0xC0000374(exit=-1073740940), 业务产物已全部正确。
下一步: LocalDumps(C:\pectest\dumps) 或活体 attach 取 0xC0000374 raise 点栈,
定位坏写者; 嫌疑面: mainW 收尾/ATEXIT 释放链、EXEC 包装器 pi 句柄清理、
日志中一次 `[ECD] line=(null)`(param_2==NULL 的 ECD 调用, 见日志倒数第 10 行)。

### 委派异常登记
本轮子代理连续 6 次 failed(no closing message): windbg 文档去重 ×2、R1 多体仲裁
×4(含重派)。疑环境性; 两任务均无产出, 待续(任务描述存本历史)。

---

## 14. 【Round 14】T4 堆损坏三连根因全落(丙v3/丁/戊) + S14 行为批次落地 · 024 回归清零

### 决定性方法转变: raise 点 ≠ 坏写者, 但反汇编返回址=源码行
WER dump 三枚(15364/15192/10492)逐一 backtrace+`.ecxr`+`ub retaddr` 反汇编,
把每个 c0000374 的 `call HeapFree/FreeStrBuf` 返回地址对回源码行 —— 三枚 dump
对应三个独立移植缺陷, 全部修复并验证:

| 缺陷 | 现场(dump) | 根因 | dc 权威证据 | 修复 |
|---|---|---|---|---|
| 丙 v2 归正 | 15364: srx+0x671 call HeapFree 返址 +0x11de71 | GrowByteBuffer→HeapRealloc(FUN_140063118) **带8字节头**返 hdr+8; v2"裸块无头"误判改 HeapFree(buf)=释块内指针 | dc:60560-60566 加头 / dc:60586+60773 家族释放=ptr-8 | core_scriptrun.c:240 恢复 FreeStrBuf(-8) |
| 丁 | 15192: PSB+0x1819 FreeStrBuf 返址 +0x1a2579 | rb:6319 `(WCHAR**)&ppWVar17` 二级地址 → HeapFree(栈地址-8); dc:44300 是传**值**(槽地址) | dc:44298-44300; 三入口 :6317/:6067/:7002 汇聚 LAB_14004c51b | rb:6319 改传 ppWVar17 值 |
| 戊 | 10492: 线程直入 SendMsgThreadProc+0x172←FreeStrBuf | ThreadMainLoop 尾部漏两次"置NULL"(dc:19158-163) → FreeStrBuf(&task) 与引用计数归零释放相撞 = double-free | dc:19130+19158-163 (cbref/local_res10 槽) | core_thread.c 尾部直移+cbref 槽 |

### 教训追加(v3)
1. **分配器家族契约表是硬约束**: FUN_140063118/1400633A8/140063694 三族全部带头
   {size,magic 0xaa55} 返 hdr+8, 释放一律 ptr-8。任何"HeapFree 裸指针"写法都先查家族。
2. **dc 里 `FUN_xxx(ppWVar)` 与 `FUN_xxx(&ppWVar)` 一字之差=值传递 vs 二级地址**,
   移植时逐字符对照形参层级。
3. **引用计数尾部纪律**: ReleaseRefCount 后必须置 NULL 再"保形 FreeStrBuf(no-op)",
   漏置 NULL 即 double-free。
4. **PDB 锁实战再现**: dump 会话(sess-5)未关即构建 → LNK1201 写 PDB 失败;
   end_session 后重链即过。构建前 end_session 纪律升级为 checklist 项。

### S14 行为批次三件套落地(补丁稿→应用→回归)
- 批次1 varargs×2(ScheduleSelfDelete/LoadFileMappingExec 补参+-1钳位还原)
- 批次2 PatchRemoteWinExec 删桩 extern 绑定 core_b2c.c:189 真体(D-02 收口)
- 批次3 ScriptInit/ScriptCopy 零参桩转发 core_exec5.c 真体(D-04 推进)
- **批次3 曾引入 024_team_multi 回归(PASS→FAIL)**: 内容产物仍全对, 但退出期
  c0000374 —— 正是缺陷戊的显形路径(TEAM 空桩时代广播链从未真实运行)。
  缺陷戊修后 024 复 PASS 且零新 dump。
- 产出: build/msvc/s14_behavior_batches_patchdraft.md(Ghidra 汇编级取证)

### 并行委派战果(4 子代理全部交付, 打破此前 6 连败)
- s13_r1_arbitration.md: 多体歧义 5 项全 RESTORED + 6 处空桩吞调用清单(stubs:332/
  392/398/546/547/777, 其中 stubs:547 高优先——073c58 清理流量走劣化副本)
- s13_r1_dualbody_batchA.md: batch-A [001..040]/134 完成(MATCH 26/DEV 14),
  崩溃级发现×3(b3i.c:203/690 CopyStrToSlot 漏 &、b2e.c:253 宽串传 ANSI 互斥槽);
  [041..134] 待 send_message 续派
- s15_corpus_expansion_plan.md + corpus_stage/(20 新案 046-065 待录制 golden)

### 当前 T4 记分板
- case001 干净退出 exit=0 ✓; 024 回归清零 ✓
- 全量 43 案基线 15 PASS(行为批次前)→ 本轮终态见 HANDOVER_PROGRESS §2
- 存量失败主体: SUB 族 021/037/038(c0000005)、REGI 030/042、WRITE/READ 文件族
  —— 下一轮按族收敛; batch-A 报告的 012/023 CopyStrToSlot 漏 & 是 SUB 链高嫌疑

### 【R14 续·第二波】返0桩族清剿 + SUB/MDIR 链推进 (commit 见 HANDOVER)

**全局返0桩三连**(S18 普查+dump 链定位, 全部落 stubs/win32_api_stubs):
1. operator_new 返NULL → 12+ 文件对象创建全静默失败(SUB 克隆块 _Memory=NULL 写 NULL+0xD9 AV) → 真体化(HeapAlloc+OOM 重试环)
2. PECMD_AsciiPrefixICmp(rb 115处)/小写 FUN_14005c788(rb 17处)/PECMD_AsciiWideICmp 无真体落返0桩 → 前缀/全串比较面整体失效 → 三转发 core_string/core_exec5 真体
3. FUN_14001b23c ExtractTableSegment 半吊子桩不写回游标出参 → FUN_14006e030 *pp=NULL AV → dc:15778-15819 真体直移(含 RTrim/注释截断两 helper)

**S18 四补丁**(NEEDS-C72c): core_thread.c:111 包装器内核(惠及~200下游动词位点)/core_main.c:155 "**u"/exec2.c:350+355 "_ENDFILE"成对 —— 全部换 TokPrefixICmp。

**S16 REGI 审计落地**: R1 三处 &script→script(同族 NULL+0xDA 崩源); R2 MULTI_SZ cp_sep 计数语义(普通字符复位/仅转义+1, cbData 短2字节修复)。R3/R4 轻微项与 OBS-1 存疑登记见 s16_regi_audit.md。

**对拍进展**: 15 → **18/43**。REGI 族清零(030/042 PASS)、024 保持 PASS。
**SUB 三案形态升级**: 崩溃 → exit=124 挂死(ExtractTableSegment 后链路在跑但死循环)——下轮入口。
**041 B簇崩点定位**: MDIR 分支(rb:6855)传 local_res8=NULL 进 CreateDirectoryTree(+0x18 首解引用)——原文 dc:44828 同传 local_res8, 说明前置参数展开段在该行型漏填; 下轮沿 PSB 参数准备段追。
