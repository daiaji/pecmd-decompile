# WINDBG_MCP_ISSUES.md — windbg MCP 鲁棒性问题专档

> **2026-08-26 状态更新（R14c）：上游已修复，实测通过。**
> ① `execute("g")` 控制通路真实生效（launch→bp→g 进程运行至自然退出，全程 .lastevent/registers 可响应，不再吞命令、不再半死）；② 单条命令失败不再污染会话（错误后 .lastevent 立即可用）。
> 仍需注意：dump 会话对不可读栈区/错误宽度模式的 `s` 搜索报 0x80040205 属合法失败（如 `-dq` 需 8 字节模式），会话可继续使用。
> 以下 A-H 为历史病理档案，保留作鉴别诊断参考；SOP 第 4 条放宽为「控制类优先专用工具，execute 单语句已可用」。

> 2026-08-25 编纂 | 文档子代理产出 | 定位：windbg-mcp（Rust 主服务 + python worker 组）在
> pecmd_msvc.exe 活调试中的已知缺陷登记、受控复现实录与安全操作规程。
> 上游纪律文档：`refactored\DEBUGGER_HANDOFF.md` §7/§10/§11/§12、`HANDOVER_PROGRESS.md` §5/§8。
>
> **编纂注记（信息源偏差，保守处理）**
> 1. 任务书所列 `refactored\DEBUGGER_HANDOFF.md` 实际位于仓库根目录
>    `D:\repo\PECMD反编译\DEBUGGER_HANDOFF.md`（refactored\ 下无此文件），本文按实际位置取材；
> 2. `refactored\REVIEW.md` 全文**无字面 "windbg" 条目**（grep 实证），最接近的相关内容为
>    §130 战果链表（dump/探针证据栏）、§130 教训 5（dumpbt.py 三件套）与文末 WER LocalDumps 备注，
>    本文以这些条目替代；
> 3. git log（仓库根=`refactored\`）搜 windbg/PDB/LNK1201：windbg 相关 3 条
>    （48ff9c6 / 3391af4 / 0eb37ab，见各条目引证）；PDB 仅 476997d（gitignore 排除 *.pdb）；
>    **LNK1201 零命中**——该问题只存在于会话经验与 §7 纪律行，无提交留痕。

---

## 0. 验证方法与本轮实验台账

信息源三类：①历史档案（DEBUGGER_HANDOFF / HANDOVER_PROGRESS / REVIEW / git log，只读）；
②受控复现实验（对 `C:\pectest\pecmd_msvc.exe LOAD C:\pectest\t2probe.pecmd` 发起，全程
launch/open_dump/set_breakpoint/go/registers/execute 等 MCP 工具，每会话结束即 end_session，
未运行任何构建）；③server_log 服务端侧佐证。

| # | 会话 id 尾号 | 类型 | 操作序列 | 结果 |
|---|---|---|---|---|
| S13 | …fc4-13 | open_dump (pecmd_msvc.exe.13132.dmp, base 0x7ff721b10000) | set_breakpoint → backtrace(24帧) → registers ×2 → end_session | B 复现；D 未复现（逐帧对 map 自洽）；G 正常态采样正常 |
| S14 | …f6c-14 | launch | set_breakpoint ×2 → go(tool) ×3（全部正常命中 PSB）→ registers ✓ → **execute("bp …; g")** → registers 空 ×2 → threads/bl/disasm 仍活 → go/g 均 0x80040205 → debug_batch eval ✓ / resume ✗ → end_session | E 复现（bp 生效、g 被丢）；A 的"半死"形态复现 |
| S15 | …82c-15 | launch | **bp 内嵌自动命令串**(`bp … ".printf \"…\\n\"; gc"`) → .bpcmds 回读完好 → go(tool) 第一次即 **0x8000FFFF** → registers/bl/batch 全 0x80040205 → end_session | A 复现（0x8000FFFF 主症状）；F 病理修正 |
| S16 | …960-16 | launch（对照组） | bp(tool) ✓ → go(tool) ✓ 命中 → registers ✓ → execute("bc 0") ✓ → **execute("g") 被吞** → \| 可用、interrupt "nothing running"、registers 空 ×2 → .lastevent 显示仍停旧事件 → go(tool) 0x80040205 → end_session | E 单语句形态复现；G=损伤伴生形态 |
| S17 | …6b8-17 | launch（隔离对照） | bp(tool) → go(tool) 命中 → **仅插入 execute("g") 一个变量** → go(tool) 立即 0x80040205 → end_session | E→A 因果链 n=3 隔离实证 |

SOP 对照组结论：纯净流程（launch → bp 工具单断点 → go 工具放行 → registers → bc → 收尾）
在 S14/S16/S17 三轮中断点命中、寄存器读取全部正常——**缺陷集中在 execute 复杂/控制类命令路径，
不在断点与放行的基本盘**。

---

## A. 执行控制失效连锁：0x8000FFFF 灾难错误 → worker 半死（后续命令全 0x80040205）

> **R14 补充触发面（2026-08-26 实战）**：dump 会话中对深栈执行 `s -dq <range> <pattern>`
> 栈模式搜索亦报 `0x80040205`——大范围栈扫描类读取命令同样可能引爆/伴随引擎异常态。
> 处置同本条：弃会话改源码级推理（该案即因此放弃内存级取证转 dc 对照破案，
> 见 DEBUGGER_HANDOFF §14 缺陷戊）。范围搜索命令慎用；确需内存取证优先小窗口 read_memory。

- **严重度：高**
- **症状**：一次 go 放行返回 `灾难性故障 (0x8000FFFF)`；此后该会话一切命令
  （registers/bl/batch/execute）统一报 `提出了一个意外的意外现象 (0x80040205)`，
  引擎 worker 进入不可恢复的半死态。较轻的变体：不经 0x8000FFFF，直接从某条命令起
  全部控制类请求 0x80040205（S14/S16/S17 均为此形态）。
- **复现配方**（2026-08-25 受控实证，n=3 隔离）：
  1. `launch` pecmd_msvc.exe → 停初始 int3；
  2. `set_breakpoint` 工具下单断点（如 `pecmd_msvc+0x1a09a0`，PSB 入口）；
  3. `go` 工具放行 → 正常命中；
  4. **经 `execute` 下发任意执行控制语句**（单独 `"g"` 或组合 `"bp X; g"`）——该调用静默吞掉、无报错；
  5. 再调 `go` → 0x80040205，会话报废。
  另一触发面（S15）：`bp <addr> ".printf \"…\\n\"; gc"` 这类**内嵌自动命令串的断点**，
  设置成功后第一次 go 即 0x8000FFFF。
- **根因猜测**：MCP worker 对"执行控制类"命令需要配套的 stop-wait 状态机处理；经 execute
  通路下发的 resume 类语句（以及带内嵌命令串的 bp）使 worker 状态机错位——命令被提交但
  等待侧未挂接，后续控制请求全部撞死锁化的引擎会话（0x80040205 是 dbgeng 的通用失败透传）。
  历史案例另有**自伤成分**：map 第一列(section offset)≠RVA，第一列算出的地址差 0x1000，
  int3 下进指令流中间打烂执行流，同样呈现"灾难性故障"假象（DEBUGGER_HANDOFF §10 教训 1）。
- **对策（已验证）**：
  - 执行控制一律用专用工具：`go` / `run_to_address` / `step_*`；**execute 只用于读取类单语句**
    （bl / .bpcmds / .lastevent / disasm 类），禁发 `g`/`p`/`t`，禁任何含 `;` 的组合命令；
  - 禁用带自动命令串的断点（见 F）；
  - 断点地址必须 map 第三列 `VA − 0x140000000`，下点后先 bl + registers 核对 rip 落点再放行；
  - 半死态无解药：debug_batch 的 eval 读类尚可作为"遗言取证"（S14 实测 `? @rip` 成功、
    resume 步骤同样失败），执行控制不可恢复，唯一出路 end_session 重建。
- **历史对照**：§7 记载的"多断点连续 go 偶发 0x8000FFFF"，本轮 S14 用 2 个断点连放 3 次
  **全部正常**——单纯"多断点+连续 go"不是充分触发条件，真正的毒剂是 execute 控制类命令与
  内嵌命令串断点（以及历史上的错误 RVA 自伤）。原对策"每次追踪用全新 session、单断点单放行"
  仍然有效但可以放宽表述：断点数量本身不是雷，命令通路才是。

## B. dump 会话下 bp 必报 0x80040205（静态快照无活进程）

- **严重度：低**（行为确定、无附带损伤；记录价值在鉴别诊断——与 A 的半死态错误码相同）
- **症状**：open_dump 打开 WER dump 后，set_breakpoint 必然
  `Debug command failed: 提出了一个意外的意外现象 (0x80040205)`。
- **复现配方**（已复现，S13）：`open_dump C:\pectest\dumps\<n>.dmp` →
  `set_breakpoint("pecmd_msvc+0x1a09a0")` → 上述错误，稳定必现。
- **根因**：dump 是崩溃瞬间的静态快照，无可注入 int3 的活线程；dbgeng 拒绝下断点并透传
  HRESULT。这不是 MCP 的 bug，是语义边界，但错误码与 A 的半死态撞码，容易误诊为"worker 又坏了"。
- **对策（已验证）**：dump 会话只做只读分析（backtrace 真实 unwind / registers /
  read_memory / disassemble）；需要断点必须 launch 新活体会话。dump 会话同样要 end_session
  （见 C——它会占住 PDB）。

## C. 会话未 end_session → LNK1201 PDB 占用（阻塞构建）

- **严重度：高**（直接卡死构建回归环）
- **症状**：windbg 会话存活期间执行 MSVC 构建，link 阶段报 LNK1201"无法写入程序数据库文件"
  （pecmd_msvc.pdb 被占用）。交接记录称上一会话累计发生 3+ 次。
- **复现配方**：~~本轮未复现（SKIP）~~ → **R14 实战复现（2026-08-26）**：dump 会话
  （pecmd_msvc.exe.10492.dmp 分析，sess-…fc9-5）未 end_session 即发起完整构建 →
  `LINK : fatal error LNK1201: 写入程序数据库 pecmd_msvc.pdb 时出错`；end_session 后
  原样重跑构建立即通过。机理与处方不变。
- **根因**：dbgeng 加载 pecmd_msvc.exe 符号后 worker 进程持有 `.pdb` 文件句柄；
  MSVC link 需要独占写该 PDB，冲突即 LNK1201。
- **对策（end_session 释放已验证；LNK1201 消除属历史经验+R14 实战闭环）**：
  - **构建前强制清点并 end_session 所有 windbg 会话**（HANDOVER_PROGRESS §8 已立规；
    本轮 5 个实验会话全部即时关闭，server_log 显示每个 worker pid 都随 end_session shutdown）；
  - server_log 可审计当日会话生命周期（本轮实录：一天内 14+ 个 worker 全部及时回收）；
  - 构建报 LNK1201 时先查有无残留 windbg 会话/worker 进程，勿盲目重跑构建。

## D. k/ln 符号标签在相邻大函数间漂移（ECD/PSB 互串）

- **严重度：中**（历史造成过两轮实质误判，见下）
- **症状**：栈回溯的**符号名**列把帧归属到错误的相邻大函数（如 ECD+0x9a43 行实际是
  PSB 调 ECD 的 ret；"ECD 参数错乱"假案即由此 + shadow 槽 ±8 误读共同构成，后被
  0eb37ab 提交的十参探针证伪）。
- **复现配方**：本轮 **未复现（条件性缺陷）**。S13 对 dump 会话 backtrace 的 24 帧逐一与
  map 第三列核对（SLCC/ECD/PSB/DEB/RST/srx/RunCommand/RSU/MainW 各帧 symbol+offset
  与 map 入口 RVA 全部自洽）。历史案例发生在存在 ICF 折叠/同名双体的旧构建上
  （如 StrBldCopyWide 曾有 restored_bodies/core_sys 双定义，3391af4 已删）。
- **根因猜测**：PDB 符号解析对 `/OPT:ICF` 折叠的等价函数体只保留单一符号锚点，回溯器按
  最近锚点归属帧，相邻大函数间即串名；双定义链接布局加剧之。
- **对策（已验证）**：
  - 一切以 `[module+RVA]` 地址列为准、符号名仅当提示；本轮实测 MCP backtrace 工具每帧自带
    `[pecmd_msvc+RVA]` 第二列，天然抗漂移（优于裸 k 输出）；
  - 归属判定用 map 第三列 `VA − 0x140000000`（第一列 section offset 差 0x1000，勿用）
    或 PDB 双源交叉；栈参读取先 `dq @rsp L1` 锚定返回地址再谈偏移；
  - 遇疑似漂移：对可疑帧地址 `disassemble` 看指令流落点，与候选函数的 map 区间比对。

## E. execute 组合/控制类命令静默吞语句、吞输出

- **严重度：高**（静默失败 + 状态损伤双重危害，且是 A 的主要触发源）
- **症状**：三种形态，均已受控复现：
  1. **第二语句整体丢弃**：`execute("bp pecmd_msvc+0x11dba0; g")` 只回显命令行文本；
     事后 bl 证实 bp 部分生效、`g` 部分根本没执行（rip 未动，S14）；
  2. **单条控制语句被吞**：`execute("g")` 单独发送同样只回显不执行（S16/S17）；
  3. **复杂语句输出捕获丢失**：`bp … ".printf …; gc"`（双层引号嵌套）设置命令的自身输出
     为空，但 `.bpcmds` 回读证实命令串完好落盘（S15）。
- **复现配方**：见 §0 台账 S14/S16/S17。最小隔离配方 = S17：launch → bp(工具) →
  go(工具) 命中 → execute("g") → go(工具) 即 0x80040205。n=3。
- **根因猜测**：worker 对 execute 通路的命令走了与专用工具不同的提交/等待分支；
  resume 类语句需要 stop-wait 配套，该分支存在状态机缺陷——命令提交后等待侧未挂接
  （表现为"吞执行"），残留状态污染后续所有控制请求（表现为 0x80040205 连锁）。
  读取类命令不走该分支故幸存（threads/bl/.bpcmds/.lastevent/disasm 在半死态仍可用）。
  服务端 RUST_LOG=debug 级日志可进一步定位（本轮 info 级无细节）。
- **对策（已验证）**：
  - 执行控制只用专用工具（go / run_to_address / step_*）；execute 白名单化为只读单语句；
  - 需要连续动作（设点+放行）就拆成多次独立工具调用——拆分后每步都有独立回报，
    吞命令立即显形（这正是"需拆分发送"的历史经验的机理化）；
  - 每次放行后用 registers 核 rip 是否落在预期断点，作为"放行真的发生了"的闭环校验。

## F. .printf / 断点自动命令串的可靠性问题（病理修正）

- **严重度：中**
- **症状（历史记载）**：".printf 多层转义经 MCP 传递后失真，复杂脚本命令不可靠"。
- **本轮实测（修正病理）**：
  - 单层 `.printf "hello-1\n"`、`.printf "a\tb\"q\"=%x\n", @eax`（含转义引号、\t、格式参数）
    经 MCP **完全正常**；
  - 双层转义 `bp <addr> ".printf \"PSB hit\\n\"; gc"`：`.bpcmds` 回读显示
    `bp0 … ".printf \"PSB hit\\n\"; gc";` **逐字符完好**——传输层不失真；
  - 真正的问题在别处：①该类设置命令自身输出被吞（E 形态 3）；②带自动命令串的断点
    第一次放行即引爆 0x8000FFFF（A 触发面，S15 实测）。历史观察到的"打印内容不对"
    很可能是"输出丢失/会话报废"被误读成"转义失真"。
- **复现配方**：S15（内嵌命令串断点 → go → 0x8000FFFF）；S15（.bpcmds 回读对照）。
- **根因猜测**：内嵌命令串使断点命中路径进入 worker 未覆盖的处理分支（命中时要代跑
  命令串再继续），该分支与 0x8000FFFF 强相关；与 E 同源，属 worker 命令通路缺陷家族。
- **对策（已验证）**：不用断点自动命令串做打印；定点观测改为
  `run_to_address`（到址即停）+ 事后 registers/read_memory 读取；确需保留自动命令串时，
  设置后必须 `.bpcmds` 回读验证，且预期可能引爆会话、提前做好重建准备。

## G. 偶发空响应（registers 返回空）

- **严重度：低**（本身无害，但有重要诊断价值）
- **症状**：`registers` / `r` 只回显命令名，无寄存器内容。
- **复现配方**：本轮复现 4 次，但**全部为损伤伴生形态**，非健康会话的孤立抖动：
  S14（组合命令后半死早期）×2、S16（execute("g") 被吞后）×2。健康状态下采样
  （S13 dump ×2、S14 前段、S16 对照组前段）从未出现空响应。
  另注意区分第三种情形：目标运行中/引擎忙时的空响应（S16 中 `|` 能列出进程、
  interrupt 报 "nothing was running"，说明当时处于运行/悬置边界态）。
- **根因猜测**：空响应 = worker 提交了查询但未收到/未转发引擎输出，出现在
  (a) 引擎忙于运行态、(b) worker 状态机已损伤（E/A 前兆）两类场景。
- **对策（修订）**：历史口径"重试即恢复"**本轮未获支持**——两次空响应重试均未恢复，
  都演进为半死态。现行处置：空响应出现时先判断会话健康度——补一发轻量读取
  （`.lastevent` 或 threads）：能正常返回则目标可能在运行，用 interrupt/wait 处理；
  也异常或随后 go 报 0x80040205 ⇒ 按 A 处置，立即 end_session 弃会话，不做无效重试。

## H. sxe av; g 之后的异常状态语义混乱（first-chance/second-chance 与预期不符）

- **严重度：中（历史定性）；本轮 SKIP**
- **症状（历史记载，未经本轮复核）**：配置 second-chance 断 AV 后放行，停止时机/
  上下文与预期不符（first-chance 即停或根本不停，寄存器上下文指向存疑）。
- **本轮未复现原因（SKIP）**：
  ① 当前构建 t2probe 无稳定 AV 路径（ab14263 门A全通；最新已知 AV=
  SkipLeadingControlChars 属 T4 corpus 场景，需全套 harness 调用才能确定性触达）；
  ② 复现所需的 `sxe av` 与放行组合恰属本轮实证的高危命令形态（E/F 家族），
  在半死态上观察异常语义得不到可信数据。
- **保守建议**：在缺陷 A/E 修复前，AV 捕获类工作流一律让 WER LocalDumps 出 dump、
  走 open_dump 事后分析（本项目已有成熟通道：C:\pectest\dumps + backtrace 真实 unwind），
  不依赖活体 sxe av。

---

## 【安全操作规程 SOP】— windbg MCP 现行纪律汇总

以下每条均有本轮实验或历史档案背书：

1. **全新会话纪律**：每轮追踪开新 launch/open_dump；会话用完（或报废）立即 end_session。
   本轮 5 个实验会话 + server_log 当日 14+ worker 全部即时回收，零残留。
2. **构建前 end_session 清点**：worker 进程持 PDB 句柄，残留会话 = LNK1201（条目 C）。
   报 LNK1201 先查会话/worker 残留，勿盲目重试构建。
3. **dump 只读**：open_dump 会话禁下断点（必然 0x80040205，条目 B），只做
   backtrace/registers/disassemble/read_memory；崩溃类问题首选 dump 事后分析。
4. **执行控制只用专用工具**：go / run_to_address / step_*；**execute 白名单=只读单语句**
   （bl / .bpcmds / .lastevent / r 类查询）；禁发 g/p/t、禁 `;` 组合命令（条目 A/E，
   n=3 隔离实证）。
5. **禁用断点自动命令串**：`bp addr "…; gc"` 形态会引爆 0x8000FFFF；定点观测改
   run_to_address + 事后读取（条目 F）。
6. **断点选址铁律**：RVA 取 map **第三列 VA − 0x140000000**（第一列 section offset 差
   0x1000，int3 下错位会打烂执行流伪装成引擎故障）；用 `module+RVA` 表达式下点；
   下点后 bl + registers 核 rip 落点，再放行。
7. **符号归属双源锚定**：backtrace 以 `[module+RVA]` 地址列为权威、符号名仅提示；
   与 map 第三列交叉核对；疑漂移时 disassemble 该帧地址比对候选函数区间（条目 D）。
8. **放行闭环校验**：每次 go 后 registers 核 rip 是否停在预期断点——既防"吞放行"
   （E），也顺带采集寄存器现场；单断点单放行保持归因清晰（断点数量本身不致灾，
   但小断点集利于定位毒剂命令）。
9. **空响应处置树**：空响应 → 补一发轻量读取探健康 → 正常＝运行态（interrupt/等待）；
   异常或 go 报 0x80040205 ＝半死，直接 end_session 重建，不做无效重试（条目 G）。
10. **半死态无解药**：唯一出路 end_session；报废前可用 debug_batch eval 做"遗言取证"
    （读 rip/内存），执行控制不可救（条目 A）。
11. **栈参读取规范**：读栈槽前先 `dq @rsp L1` 锚返回地址，防 shadow 槽/±8 偏移误读
    （历史"ECD 参数错乱"误判教训之一）。
12. **探针优先原则**：能用源码级探针回答的问题不用活体断点猜读（见下节）。

### 哪些操作建议改用源码级 fprintf 探针替代

**判据：问题能在源码层表达（函数参数/返回值/结构体字段/调用序）⇒ 探针；必须看
无源码帧、寄存器级 ABI、或拿不到新构建 ⇒ windbg。**

优先改用探针的场景（历史战例背书）：
- **包装器/系统调用的参数核验**：CreateProcessW 十参探针一轮锁定 memset 空壳桩真相
  （0eb37ab），比三轮 windbg 会话更快更稳——windbg 帧漂移还曾把该案误导成"ECD 参数错乱"；
- **入口/中间值追踪**：[AWS] 入参探针证伪"上游堆污染"假设，把根因收敛到隐式 int 原型截断
  （3391af4）；si.dump 结构体 dump 实锤 STARTUPINFO.lpTitle 陈旧栈值；
- **调用链/执行流验证**：[PSB]/[ECD]/[DEB]/[MAINLOOP] 探针网实证脚本行走链路
  （ENVI/EXEC 行是否到达分发器），这类"到没到/带什么参数"的问题探针完胜断点；
- **所有权/生命周期取证**：[FREE]/[_ReturnAddress()] 定位 double-free 调用点（REVIEW §130 T1e）。
- **探针工程纪律**：TEMP PROBE 统一命名、手写 CRT extern `fopen/fprintf/fclose`
  并**禁 `#include <stdio.h>`**（撞 win32_api_stubs 的 _vsnwprintf 内联）；探针网过门后
  T5 统一拆除；每步保构建绿（chcp 936 + `[msvc_build] OK` 行为准）。

windbg 仍不可替代的场景：
- 崩溃**事后**分析（WER dump + 真实 unwind 远胜栈扫描，ResDecode/NextToken 两案实锤）；
- CRT/ntdll/kernel32 等无源码帧内的崩点定位与寄存器级 ABI 取证；
- 拿不到重新构建窗口时的纯现场分析（配合 tools/debug/dumpbt.py 三件套）。

---

*本文由文档子代理基于只读史料 + 2026-08-25 五轮受控实验（会话尾号 13–17，全部即时
end_session，零构建）编纂；实验原始观察已内嵌各条目，未改动任何其他文件。*


---

## 【R18 最终状态更新】(Round13, 上游二次修复后实测)

**已修复并验证**:
- execute("g") 无断点运行→自然退出: 全程可观测(.lastevent/输出回传) ✓
- 单命令失败不再污染会话(.lastevent 错误后立即可用) ✓
- dump 会话 fopen 类源码探针不受影响 ✓

**仍不可用(实测)**:
- 软件断点(bp)命中后停止: set_breakpoint 工具与 execute("bp...") 设置均不触发停止, 进程直接跑完退出
- ba 数据断点: 同族 0x8000FFFF
- s 大范围搜索: 部分场景 0x80040205

**现行可行调试手段(穷举)**:
1. WER dump 事后分析(open_dump+backtrace+.ecxr+disassemble) —— R14 三案全靠此破
2. 源码级 fprintf 探针(禁 stdio.h, 手写 CRT extern)
3. Ghidra msvc 工程静态分析(已导入, 3813 函数)
4. run_case/diff_case 行为对拍(黑盒)

**结论**: ~~断点类活体调试在当前 MCP 版本完全不可用; 调试策略=dump 事后+源码探针双轨。上游需修复 worker 的断点命中-停止事件分发机制。~~
**【R19 裁定已撤销本结论】**——系断点地址取自旧构建(六轮重建后 RVA 漂移)所致误诊，见下方 R19 裁定附录与撤回声明；现行有效纪律见文末【R20 版本锁定纪律】。

> **复现配方移交版 (2026-08-26)**: 可直接发上游的最小复现步骤+行为漂移时间线+取证抓手,
> 见 `docs/WINDBG_MCP_REPRO_UPSTREAM.md`。

---

## 【R19 裁定附录】(Round19 独立复核, 2026-08-26) — R18 结论撤销: 无上游缺陷, 系断点地址与二进制版本不符

**裁定: 本文档 R18 "断点类活体调试在当前 MCP 版本完全不可用 / 上游需修复 worker 断点命中-停止事件分发机制" 的结论不成立, 予以撤销。** 配套
`docs/WINDBG_MCP_REPRO_UPSTREAM.md` 已加撤回声明, 请勿发上游。

### A. 证据链 (全部实测, 同一 windbg-mcp 二进制 = Release 0.12.0/fec77eb 构建, sha256 5F69AB02…; 同一冻结 pecmd_msvc.exe 快照)

| # | 实验 | 结果 |
|---|---|---|
| 1 | 冻结 exe + `set_breakpoint("pecmd_msvc+0xf2b0")`(=main 入口) → `go` | ✅ `Breakpoint 0 hit`, rip=入口精确落点 |
| 2 | 续放 → `+0xebc0`(PECMD_MainW) | ✅ `Breakpoint 1 hit`, rip 精确落点 |
| 3 | 同 exe + `kernel32!ExitProcess` + `execute("g")` | ✅ `Breakpoint 0 hit` + moved note |
| 4 | R18 原配方逐字复现 (`+0x1a09a0` + run_all.pecmd + `execute("g")`) | ⚠ 完整复现 R18 症状(只回显 g → registers 0x80040205) —— 因为该地址未被脚本路径执行 |
| 5 | ping 对照(符号 bp / 模块+偏移 bp / `ba w4` / `bc*;bp X;g` 组合) | ✅ 全部正常命中 |

### B. 根因: 断点地址取自旧构建, 被测 exe 已被连续重建

- `pecmd_msvc+0x1a09a0` 在 8-25 S13-S17 时期确为 PSB 入口(当时命中正常); **8-26 白天 PECMD 六轮重建**(45c7793→27fce88→52e3ffa→…→997ff7a)后函数重排:
  - 当前 17:56 构建 map: `0x1a09a0` = `PECMD_ParseResourceStringRef+0x1e0`(**函数内部**, ENVI/EXEC 脚本从不执行);
  - 13:09 构建 `pecmd_old.exe` 实测反汇编: 该地址已是 `xor [rax-7Dh],cl / c4 ??? / int3` **非指令边界的填充垃圾**;
  - 会话日志 turn 7 还自证过 PSB 实为 `+0x1a1060`, turn 26 换用后**仍失败**(该轮又重建, map 显示 `0x1a1060`=另一个函数内部) —— 地址始终没有与当时的 exe 绑定核验。
- 症状全过程 = 断点设在从未执行的代码上 → 进程跑完脚本自然退出 → `execute("g")` 忠实回传退出 → 无 debuggee 后 registers 报 0x80040205(合法失败)。
  `go` 报 0x8000FFFF 同理 = 上游已记录"等待期间目标退出"(FOLLOWUPS item 48), 非 worker 半死。

### C. 工作流缺陷与修正纪律(取代 R18 的"MCP 不可用"结论)

1. **地址-二进制绑定**: 断点 RVA 只能从**当前会话所连 exe 的当前 map/反汇编**解析; 构建即换地址, 绝不沿用旧文档/旧会话的 RVA。
2. **下点三验证**: `set_breakpoint` 后必做 `bl`(确认解析地址) + `u <addr>`(确认指令边界) + 确认目标在脚本路径上, 再放行。
3. **引擎健康对照**: 每个活体会话先设一个必中断点(如 `kernel32!ExitProcess`)验证引擎本底正常, 再谈业务断点。
4. **exe 快照冻结**: 调试目标 exe 复制冻结(本裁定用 `.frozen-pecmd\` 快照, sha256 2F9C0B7D…), 防构建循环污染实验。
5. **实测通行**: 模块内 bp / 符号 bp / 模块+偏移 bp / 多断点连续放行 / `execute("g")` / `ba` 数据断点在本版本全部可用 —— R18 的"断点类活体调试不可用"与"ba 引爆会话"均未获支持。
6. 原 A-H 档案中**真实仍成立**的条目(与地址无关者): B(dump 禁断点, 语义边界)、C(PDB 占用 LNK1201, 纪律规避)、D(符号漂移, 条件性)、`s` 大范围搜索不可读区失败(合法); A/E/F/G/H 中与"execute 控制类命令"相关的**吞命令/半死**问题已在 #226 settle 修复中解决(R14c 已证), 剩余观察均可用本裁定 B 节机理解释。

---

## 【R20 版本锁定纪律】(2026-08-26 双误诊复盘新增)

> 背景：R18"断点不可用"误诊与 B 簇 exit=183 探针污染（analysis/r19b_exit183_chain.md）
> 同属一类事故——观测装置/过期工件污染被当成产品缺陷。断点调试任务编排=V-Gate 五道门，
> 以下五条为硬性纪律：
>
> - **V1 断点前版本核验(P0)**：任何 bp/ba 之前必须对部署目标取哈希并与当期构建产物比对，
>   并从【当期】pecmd_msvc.map 第三列重推目标 RVA；禁止复用历史会话笔记中的 module+offset。
>   构建后运行 tools\make_symsnap.sh 生成 build\msvc\symsnap.txt——断点选址唯一合法来源。
> - **V2 落点自证**：下点后 disassemble 验证指令边界，首次命中 registers 核对 rip 落在预期函数
>   map 区间内，不符立即弃点重推。"从未命中"的结论只有 V1+V2 均满足后才有资格成立。
> - **V3 部署身份标记**：每次部署同步写 C:\pectest\DEPLOYED_BUILD.txt(git hash+MD5+时间)；
>   分析任何日志/dump 前先核对其与当期构建一致；禁止两份构建并发写同一日志/dump 目录。
> - **V4 探针透明化**：探针块进出保存/恢复 GetLastError（补丁 A/B 已落地 core_b9_remaining.c /
>   core_exec2.c）。凡"API 失败→探针 I/O→GetLastError() 消费"窗口一律按污染嫌疑排查。
> - **V5 否定性结论门禁**："某工具/路径坏了"类定论归档或上报上游前必须三关全过：
>   当期构建复现、已知良好对照实验（如 bp kernel32!ExitProcess）、第二代理证伪尝试。
>
> 调度附则(V6)：启动构建前除清点 windbg 会话外，还必须确认无在途子代理持有 dump 会话
> （R20 实训：D 线取证子代理会话未关即构建 → LNK1201）。
