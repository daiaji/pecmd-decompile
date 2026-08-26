# WINDBG_MCP_FAULTS_REPRO.md — windbg MCP 故障触发面与完整复现方法（独立专档）

> R24d 建立（2026-08-27）。姊妹档：`docs/WINDBG_MCP_ISSUES.md`（病理 A–H 历史/上游修复史）。
> 本档只回答一个问题：**故障怎么一步步复现**——每条触发面给出可在本机执行的精确命令序列、
> 预期观察与判定标准。文末附「故障浮现速查卡」供取证/调试时即时对照。

---

## 0. 复现环境（固定不变的前提）

| 项 | 值 |
|---|---|
| windbg-mcp 服务 | 本会话接入的 windbg MCP（engine worker 每会话独立进程） |
| 目标 A（快退型） | `C:\pectest\pecmd_msvc.exe`（部署体，见 DEPLOYED_BUILD.txt） |
| 目标 B（快退型） | `C:\pectest\PECMD.EXE`（原版，无 ASLR，基址固定 0x140000000） |
| 用例 | `C:\pectest\031_set_assign\run_all.pecmd`（运行 ~1-2s 内 exit） |
| 复现载体 | MCP 工具序列：`launch` / `execute` / `session_status` / `end_session` |
| 纪律 | 每个触发面用独立会话；复现后必须 `end_session` 再开下一个 |

## 0.1 复现物料清单（R24d 当日核验，全部在位）

| 物料 | 位置 | 身份/备注 |
|---|---|---|
| msvc 部署体（目标 A） | `C:\pectest\pecmd_msvc.exe` | md5 依 `C:\pectest\DEPLOYED_BUILD.txt`（hash+md5+time + 身份核对规则）；**当前部署体可能携带 [ELC] TEMP PROBE**（memfail.log 有记录）——复现 T2-T4 前先看身份戳，探针版不影响触发 |
| 原版无 ASLR 副本（目标 B） | `C:\pectest\PECMD.EXE` | md5 `4b97f08dd4b8391619d0ee7620c48a9c`，基址固定 0x140000000 |
| 原版原始件 | `D:\repo\pecmd-decompile\reference\PECMD.exe` / `reference\PECMD原始.EXE` | 只读参考；Ghidra 程序源 |
| 复现用例 | `C:\pectest\031_set_assign\run_all.pecmd`（+ `main.pecmd`） | 由 run_case.py 部署再生；语料源 `harness\corpus\cases\031_set_assign\` |
| 探针日志（[ELC] 数据） | `C:\pectest\memfail.log` | 追加式；复现前可清空以隔离噪音 |
| WER 崩溃 dump | `C:\pectest\dumps\` | 每轮取证后清理防误读 |
| 构建产物/身份旁证 | `D:\repo\pecmd-decompile\build\msvc\pecmd_msvc.exe` + `pecmd_msvc.map` + `symsnap.txt` | map 第 3 列 VA 才是断点选址依据（§ISSUES R19 教训） |

> 核对规则：任何分析/复现前先 `cat C:/pectest/DEPLOYED_BUILD.txt` 确认与当前
> 工作树 git HEAD 对应；dumps\ 清理纪律同 R23（每轮取证前清空防误读）。

> 复现 T1-T4 均不需要特殊语料：任何"启动后 1-2 秒内自然退出"的进程即可。
> 想要更慢的窗口，可把脚本改成 `TEAM WAIT 5000` 之类延长运行，但**快退本身就是
> T3/T4 的触发条件**，勿为方便而改变。

---

## T-1 bp 内嵌自动命令串（含 g）→ 会话半死 0x80040205

### 前置
- 目标 B（原版 PECMD.EXE，无 ASLR，地址固定）。

### 精确步骤
1. `launch` 目标 B：`C:\pectest\PECMD.EXE LOAD C:\pectest\031_set_assign\run_all.pecmd`
   → 预期：停在 ntdll 初始断点，返回 session_id。
2. `execute` 设置带内嵌串的断点并放行：
   ```
   bc *; bp 140034788 "du @rdx L40; .echo ====; g"; g
   ```
   （0x140034788 = 原版 IFEX/FIND 共享执行体入口，用例必然命中多次）
3. 观察这一步的返回：**可能立即无输出**（g 被吞），也可能返回一次命中断点信息。
4. 紧接着随便发一条查询：`execute "k 3"` 或 `registers`。

### 判定
- 步骤 4 若返回 `Debug command failed: 提出了一个意外的意外现象 (0x80040205)`
  → **复现成功**。该会话已半死（一切命令 0x80040205，包括 session 内任何工具），
  只能 `end_session`。
- 若步骤 4 正常 → 未复现（换 T-2 的连发 g 配方，或重试一次；半死窗口与命中时序相关）。

### 机制（实测推断）
- 断点回调上下文内执行 `g`（resume 需要引擎等待新事件）违反 dbgeng 回调约束，
  worker 的 stop-wait 状态机失去一致性；后续所有请求撞死锁。
- 同一会话内 `bp … "; gc"`（§ISSUES F 变体）同样命中，首次 go 即 0x8000FFFF。

---

## T-2 单条 execute 连发多个 resume（`g; g; g; g; g`）→ 0x8000FFFF 灾难

### 前置
- 目标 A（pecmd_msvc.exe，带符号：`PECMD_EvalLoopCondition` 可解析）。

### 精确步骤
1. `launch` 目标 A：`C:\pectest\pecmd_msvc.exe LOAD C:\pectest\031_set_assign\run_all.pecmd`
2. `execute` 下普通断点：`bp pecmd_msvc!PECMD_EvalLoopCondition; g`
   → 预期：`Breakpoint 0 hit`，目标停止。
3. `execute` 放行一次（单条）：`g` → 预期命中或继续运行。
4. （关键步骤，二选一）
   a. `execute` 发送 `g; g; g; g; g`（一条调用内 5 个 resume）；
   b. 或连续 3 次单条 `g`（每次返回后立即再发，不再检查输出）。

### 判定
- 步骤 4 后下一次命令若返回 `灾难性故障 (0x8000FFFF)`，随后一切命令 `0x80040205`
  → **复现成功**。`end_session` 收尸。
- 对照组（证明是"连发"而非"放行"本身）：改为每条 `g` 后都先读一次 `r` 或
  `bl` 再发下一条 → 全程稳定（当日实证 6 次单放行零故障）。

### 机制（实测推断）
- 目标在第一个 resume 后停在断点；队列里第二个 resume 使引擎在"已停止"状态下
  再次请求 continue → dbgeng 返回 E_UNEXPECTED，worker 未做状态回滚 → 死锁。

---

## T-3 快退进程 launch 后"二次放行" → worker 进程消亡

### 前置
- 目标 A（pecmd_msvc.exe）。此触发面**对目标 A 稳定复现，对目标 B 未复现**
  （B 的断点命中密度低、回调快，进程退出跑在放行窗口之外）。

### 精确步骤
1. `launch` 目标 A（031 用例）。
2. `execute`：`bp pecmd_msvc!PECMD_EvalLoopCondition; g`
   → 预期 `Breakpoint 0 hit`（该函数在启动流程中被高频调用，必命中）。
3. 读一次寄存器确认目标仍在（`r`）。
4. 再发 `g`（第二次放行）。

### 判定
- 步骤 4 若返回错误：
   `the engine worker process holding session <id> is gone — it exited, crashed, or was terminated`
   → **复现成功**（worker 进程级崩溃；目标丢失；该 session_id 永不可再用）。
- 这是三种终态里最重的一种：不是"会话半死"而是"worker 死了"。`end_session`
  对已死会话是 no-op，直接开新会话即可。

### 机制（实测推断）
- msvc 构建断点命中密集（EvalLoopCondition 在多处被调），断点回调与调试事件
  处理在目标快速退出（ExitProcess）窗口内竞争 → worker 的 WaitForDebugEvent
  循环在进程清理路径上崩溃。B（原版）回调轻且命中少，窗口外退出，不崩。

---

## T-4 bp 设在不被调用的函数 + g 静默放行 → 命令 0x80040205

### 前置
- 目标 A；选择"该路径其实不会调用的函数"作断点目标。
- 当日实例：`FUN_14006F884`（环境变量查询）——031 的 EvalLoopCondition 分派
  实际走 `FUN_14001E69C`（变量直查），06F884 全程零调用。

### 精确步骤
1. `launch` 目标 A（031 用例）。
2. `execute`：`bp pecmd_msvc!FUN_14006F884; g`
   → 预期：**g 无任何命中输出**（目标跑完自行退出）。
3. 等 1-2 秒后发任意命令：`execute "k 3"` 或 `.echo hit`。

### 判定
- 步骤 3 返回 `0x80040205` → **复现成功**（bp 从未命中 + 目标已退出后的状态）。
- 若步骤 2 恰好命中（未来实现变化），则不会复现——这正是该配方的自校验性：
  **"从未命中"必须先静态确认目标函数必然可达**（R19 教训：bp 不中≠引擎坏，
  先怀疑函数是否真的会被调）。

### 机制（实测推断）
- g 放行后无新事件产生，worker 停在"等待事件"态；此时客户端发命令，
  引擎侧事件队列与请求队列错位 → 0x80040205。

---

## 对照实验（证明故障是上述操作触发的，而非随机）

| 实验 | 操作 | 结果（当日） |
|---|---|---|
| C1 | 目标 B：launch → bp 0x140032dc4 → **逐条单 g**（每条之间读 du/db/r）×6 | 全部正常（031 标志取证完成） |
| C2 | 目标 A（挂死复现如 021）：attach_process → ~*k → read_memory → end_session | 全程零故障（R24 子代理 D 三案实测） |
| C3 | dump 会话（open_dump）：backtrace/registers/read_memory | 查询类命令稳定（ISSUES §0） |

结论：**查询类命令 + attach 流程是健康路径；故障全部集中在 execute 通路对
resume/控制类命令的处理。** 这也是"规避三律"的依据。

---

## 故障浮现速查卡（取证遇异常即时对照）

| 症状原文 | 判定 | 立即处置 |
|---|---|---|
| `0x80040205` 提出的一个意外的意外现象 | 会话半死（恢复无望） | `end_session`；别在同一会话里再试任何命令 |
| `0x8000FFFF` 灾难性故障 | 连发 resume / bp 内嵌串所致 | `end_session`；后续每条 execute 只放行一次 |
| `engine worker process is gone` | worker 进程已死 | 弃用旧 session_id，重开；复发改探针 |
| 命令正常但 bp 从不命中 | 先静态确认目标函数必达 | 不赖 bp；换 fprintf 探针验证 |
| attach 挂死进程一切正常 | 健康路径 | 挂死/慢目标一律 attach 优先 |

规避三律：
1. **execute 每条只含一条 resume**（`g`/`p`/`t`），放行后先看输出再决定下一步；
2. **禁 bp 内嵌自动命令串**（`bp X "…; gc"` 一律不用）；
3. **必达热点/快退进程取证直接上源码级 fprintf 探针**（一次构建拿全量数据，省 windbg 轮次）。

---

## 与姊妹档的分工

| 文档 | 内容 |
|---|---|
| `docs/WINDBG_MCP_ISSUES.md` | 病理 A–H 全史 + 上游修复记录 + R20 版本锁定纪律（历史真相） |
| `docs/WINDBG_MCP_FAULTS_REPRO.md`（本档） | 触发面 T1-T4 复现配方 + 判定标准 + 速查卡（行动指南） |

如遇新触发面：在本档追加"Tx 复现配方"，同时把终态现象一行追加到 ISSUES 的台账，
两档职责不重叠。