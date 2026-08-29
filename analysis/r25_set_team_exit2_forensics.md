# R25 · SET / TEAM / LOGS 三案 exit=2 根因取证

**取证代理**: 分析子代理（只读，未改任何源码）
**构建身份**: `C:\pectest\DEPLOYED_BUILD.txt` → `hash=cb9fcba md5=1e1df48e665aba1542f3ea02d68b60ec time=2026-08-27 07:57:08`
（现场 `pecmd_msvc.exe` md5 复核 = `1e1df48e665aba1542f3ea02d68b60ec` ✅ 一致；`PECMD.EXE` md5 = `4b97f08dd4b8391619d0ee7620c48a9c`）
**真值源**: `reference/decompiled.c`（唯一语义真值）

---

## 0. 结论摘要（先给答案）

**这三个用例不是同一个根因。** 账本"共同根因"的前提不成立。实测拆解：

| 用例 | msvc 实测 | 真正的失配点 | 与 SET/TEAM 是否相关 |
|---|---|---|---|
| 031_set_assign | exit=2 | `ENVI &…=` / `SET` 返回 1，与后续 FIND 的 2 叠加 | 间接（SET 用了同一条 mode 链） |
| 039_team_chain | exit=2 | **TEAM 内部 `ENVI R=chain_ok!` 的 `!` 值** → 2 | 是（TEAM 槽隔离） |
| 061_logs_smoke | exit=2 | **LOGS 返回 183 (0xB7)** + **IFEX 命中已存在文件 → 崩溃 0xC0000005** | **否**（与 LOGS/TEAM 无关） |

另有独立发现：**退出码机制本身是"最后非零动词返回值"**（dc:45139-141），
且 **`ENVI &NAME=value`（前导 `&` 且带 `=`）在 msvc 下恒定返回 1**，是一个比 SET 更普遍的缺陷。
`SET A=pecmd` 之所以返回 1，正是因为它走的是 `&` 加深链（mode=1 + `&` 计数）的同一条路径。

---

## 1. 真值采集（步骤 1 输出）

复跑原版（exit 均为 0）：

```
python harness/runners/run_case.py 031_set_assign --exe orig --timeout 120  → exit=0
python harness/runners/run_case.py 039_team_chain --exe orig --timeout 120  → exit=0
python harness/runners/run_case.py 061_logs_smoke --exe orig --timeout 120  → exit=0
```

| 文件 | orig | msvc |
|---|---|---|
| 031 `vars_val.txt` | `CASE=031_set_assign\|A=pecmd\|R=setok` | ⚠ 缺失（**陈旧件**，见 §1.1） |
| 039 `vars_val.txt` | `CASE=039_team_chain\|X=6\|R=chain_ok` | ⚠ 缺失（陈旧件） |
| 061 `vars_val.txt` | `CASE=061_logs_smoke\|R=logs_open\|logs_open=%logs_open%\|BODY_DONE=YES` | `CASE=061_logs_smoke\|R=%=??\|BO=YES` ⚠ 陈旧件 |

### 1.1 ⚠ 证据污染警告（必须记录）

`harness/results/win_real/{031,039,061}/vars_val.txt` 的 mtime 均为 **2026-08-27 06:03**，
而本次 exit.txt/stdout.txt 为 **2026-08-28 22:45**。
`run_case.py:run_exe()` 只 `shutil.rmtree(out_dir)`，**从不清理 `result_dir`**（见 `run_case.py` 第 130-135 行）。
且这三案 manifest **均无 `"vars_val": true`**（全语料仅 `053_hash_probe` 有）。

⇒ **那些 `vars_val.txt` 是 8/27 的陈旧残留，不是本次证据。**
061 那个 `R=%=??|BO=YES` 的"损坏值"是旧构建产物，**不能作为 msvc 变量展开损坏的证据**。
建议顺手修 `run_case.py`（清理 result_dir 或按 manifest 门控回捞），否则会持续误判。

### 1.2 稳定性

`C:\pectest\031_set_assign\run_all.pecmd` 连跑 5 次，**5/5 exit=2** → 稳定复现，非偶发。✅

---

## 2. 退出码机制（dc 铁证）——这是理解三案的钥匙

退出码不是"脚本末尾守卫值"，而是 **PSB 尾部把最后非零动词返回值写进 `script+0x50`**：

- **dc:45139-141**（原文）：
```c
LAB_14004c525:
  if (local_180.QuadPart != 0) {
    (*(LARGE_INTEGER **)(param_1.QuadPart + 0x50))->QuadPart = (LONGLONG)local_180;
  }
```
- 还原对应：`restored_bodies.c:6883-6886`（`LAB_14004c525:` 块）
- 读出端：`src/app/core_init.c:69-72` `PECMD_GetExitCodeGlobal()` → `*(uint32_t*)*(uint64_t*)(g_Script+0x50)`
- 主函数返回：`src/app/core_main.c:174` `ret = PECMD_GetExitCodeGlobal();`

**关键性质：只有非零返回值才覆写。** 所以"最后一次非零的动词返回值"= 进程退出码。

这条直接解释了账本里的"BODY_DONE 守卫值"记载——那其实是 061 里 `ENVI BODY_DONE=YES` 之前的
LOGS(183)/IFEX(崩溃) 留下的值，而非 IFEX 分支本身。

---

## 3. SET / ENVI / TEAM 的源码与 dc 定位（步骤 2、3）

### 3.1 命令分派级联（动词 → FourCC）

- 动词构造：`restored_bodies.c:6153-6212`（`local_158` 拼装 + `uVar21 = uVar21 << 8 | uVar29`）
  - dc 对应：**dc:44212**（同一 `uVar21 = uVar21 << 8 | uVar29;`）
- **SET 分派**：`restored_bodies.c:6222-6236`（`local_158 != 0x544553` → fallthrough，`cVar6='\x01'`）
  - dc 对应：**dc:44223-44227**
- **ENVI 分派**：`restored_bodies.c:6573-6582`（`local_158 == 0x49564e45`，`cVar6='\0'`）
  - dc 对应：**dc:44557**
- **TEAM 分派**：`restored_bodies.c:6678-6682`（`local_158 == 0x4d414554` → `PECMD_ParseValueCommand`）
  - dc 对应：**dc:44660-44661**（`FUN_140025980`）
- **LOGS 分派**：`restored_bodies.c:6872-6878`（`local_158 == 0x53474f4c`）
  - dc 对应：**dc:44854**（`FUN_140026338`）
- 未识别动词落点：`restored_bodies.c:6253` `LAB_14004df3c:`（dc 同名标签）

**✅ 结论：分派级联本身与 dc 逐行一致（SET/ENVI/TEAM/LOGS 的 FourCC 与 mode 常量全部对上）。**

### 3.2 SET 与 ENVI 确实同走 `EvalSpecialToken`，但 mode 不同

- SET：`restored_bodies.c:6227` `cVar6 = '\x01';` → 落到 `LAB_14004c930`
- ENVI：`restored_bodies.c:6576` `cVar6 = '\0';` → 落到同一 `LAB_14004c930`
- 共同调用：`restored_bodies.c:6229-6234` / dc:**dc:44227**

```
dc:44227:  local_180 = FUN_14002d33c(param_1,_Var22,cVar6,_Var28,(longlong)param_3);
```
`param_3` 即 mode。函数定义 **dc:27011**（`FUN_14002d33c @ 14002d33c`），
还原在 `src/commands/core_b2e.c:1358` `PECMD_EvalSpecialToken(..., char mode, ...)`。

**✅ 假设"SET 与 ENVI 同走 EvalSpecialToken mode=1 分支"——部分成立：**
同走一个函数，但 **SET 传 mode=1，ENVI 传 mode=0**，在 `param_3 == '\0'` 的判据上分道：
- dc:27100 `if ((cVar1 != '\0') || ((param_3 == '\0' && (*(ushort *)local_res10[0] != 0x26))))`
- dc:27122 `if ((param_3 == '\0') && (*(short *)local_res10[0] != 0x26)) {` → `LAB_14002d695` 直返
- dc:27133 `LAB_14002d6c7:` → `FUN_1400c6324(..., param_3)`（mode 透传给 EnviMemReadWrite）
- 还原对应：`src/commands/core_b2e.c:1451`、`1469`、`1485-1486`

### 3.3 TEAM 的 `|` 解析与子命令分发

- 还原：`src/commands/core_b2a.c:94-101` `PECMD_ParseValueCommand`
```c
uint64_t PECMD_ParseValueCommand(void *script, void *value, uint64_t flags)
{
    void *arr[3];
    arr[0] = value;
    PECMD_ParseCommandBlock(script, arr, 2, flags);
    return 0;
}
```
- dc 原文：**dc:22656-22667**
```c
void FUN_140025980(longlong param_1,longlong param_2,undefined8 param_3)
{
  longlong local_res10 [3];
  local_res10[0] = param_2;
  FUN_140025474(param_1,local_res10,2,param_3);
  return;
}
```

**⚠ 注意**：`arr[1]`/`arr[2]` 未初始化是 **dc 原文的忠实直移**（Ghidra 未显示初始化），
且 dc 标注 `void` 返回、size=33。**这属于 Ghidra 伪影，不是还原引入的 bug** ——
按 AGENTS.md 纪律，**不得擅自"补初始化"**（禁止行为注入）。`|` 的真正切分在
`FUN_140025474`（`PECMD_ParseCommandBlock`）内部，本次未深入。

- TEAM 相关辅助：`restored_bodies.c:4375` `PECMD_LineIsTeamExecLoad`（dc 直移）

### 3.4 LOGS 实现位置

- 分派到 `FUN_140026338`，dc 定义 **dc:23049**（`size=4875`）。
- 还原侧：`restored_bodies.c:6872-6878` 调用 `PECMD_LogOpenSafe`（具体符号名待核）。

---

## 4. 三案根因（步骤 4）

### 4.1 031_set_assign

**最小化复现（bisect）：**

| 脚本 | msvc | orig |
|---|---|---|
| `SET A=pecmd` | **1** | 0 |
| `ENVI A=pecmd` | 0 | 0 |
| `SET A=pecmd` + `FIND …ENVI R=setok! ELSE…` | **2** | 0 |
| `ENVI A=pecmd` + `FIND …ENVI R=setok! ELSE…` | 0 | 0 |
| `SET A=pecmd` + `ENVI B=1` | **1** | 0 |
| `SET A=pecmd` + `CALC B=1+1` | **1** | 0 |

**根因假设（高置信）**：`SET` 走 mode=1 → `EnviMemReadWrite(..., mode=1)` →
`src/ui/core_b7c.c:2147` `local_317 = a5;` 为 1（非 0）。
该值在 **`src/ui/core_b7c.c:3656`** 的门控里被直接判真：

```c
iVar5 = 0;
if (((local_218 == 0) || (local_317 != '\0')) ||          /* ← mode!=0 即短路为真 */
    (((*(uint8_t *)((char *)(intptr_t)pWVar12 + 0xd) & 0xf) != 0) || bVar34 ||
     (-1 < local_278) || ((uint8_t)local_res10 != 0))) {
```
dc 同源：**dc:124237-124238**，`iVar5 = 0;` 后紧跟 `local_317 != '\0''` 判据。
⇒ mode≠0 时进入 `PECMD_VarLookup` 分支（`FUN_140018978`，dc:2661 区域），
该路径最终把 **1** 写入 `r28`（`src/ui/core_b7c.c:3775` `r28 = (int64_t)iVar6;`），
返回后被 dc:45139 写入 `script+0x50` ⇒ exit=1。
后续的 `FIND … ELSE` 又贡献一个非零值 ⇒ 末值为 **2**。

**变量赋值本身是对的**（`A=[pecmd] R=[setok]` 与 orig 一致），**只有返回码错了**。

**更普遍的缺陷（本次新发现，比 SET 更重要）**：
```
ENVI &B=2      → msvc=1  orig=0     ← 前导 & 且带 '=' 
ENVI &B        → msvc=0  orig=0
ENVI B&=2      → msvc=0  orig=0
ENVI &&&A=pecmd→ msvc=1  orig=0
```
`src/ui/core_b7c.c:2148-2153`（dc:122677-122682）里每个前导 `&` 都让 `local_317 += 1`。
所以 **`&NAME=value` 这类"写父作用域变量"的常规写法在 msvc 下恒定返回 1**，
会污染所有后续退出码。SET 只是同样的 mode≠0 触发面的一个特例。
⇒ **建议把根因表述为"`local_317`(mode) 门控失真"，SET 为其子集。**

### 4.2 039_team_chain

**最小化复现：**

| 脚本 | msvc | orig |
|---|---|---|
| `TEAM CALC X=2*3\|FIND $%X%=6, ENVI R=chain_ok! ELSE ENVI R=chain_bad` | **2** | 0 |
| `TEAM CALC X=2*3\|FIND $%X%=6, ENVI R=ok`（去掉 `!`） | 0 | 0 |
| `TEAM ENVI Q=6\|FIND $%Q%=6, ENVI R=ok!` | **2** | 0 |
| `TEAM ENVI R=a!b` | 0 | 0 |
| `FIND $6=6, ENVI R=a!b`（**不在 TEAM 内**） | 0 | 0 |
| `TEAM CALC X=2*3` / `TEAM ENVI A=1\|ENVI B=2` | 0 | 0 |

**根因假设**：TEAM 内部执行 `FIND … , ENVI R=<含 `!` 的值>` 时，
`!` 在值里被当作分隔符参与切分（`src/ui/core_b7c.c:817-864` 的 `,` / `=` / `!` 扫描段，
其中 `:828` `StrChrW(sp+1, L'!')` 与 `:844` `if (*sp == L'!')`），
在 TEAM 的槽上下文中产生了非零返回值 2，写入 `script+0x50`。
**`!` 只在 TEAM 内触发、不在顶层触发** ⇒ 指向 TEAM 子命令的槽隔离/缓冲所有权问题。

**⚠ 未完全定案**：`PECMD_ParseValueCommand` 的 `arr[3]` 未初始化（dc:22659-22664 原文如此），
是否参与该 2 的生成，本次未证实。**登记为待核验，不要据此改代码。**

### 4.3 061_logs_smoke —— **与 SET/TEAM 无关，是另两个独立 bug**

**两行拆开各自就炸：**

| 脚本 | msvc | orig |
|---|---|---|
| `LOGS C:\pectest\_bz\s.log` | **183** (0xB7 = ERROR_ALREADY_EXISTS) | 0 |
| `IFEX C:\pectest\_bz\s.log, ENVI R=logs_open`（文件**存在**） | **0xC0000005** (AV) | 0 |
| `IFEX C:\pectest\_bz\nofile.txt, ENVI R=yes`（文件**不存在**） | 0 | 0 |
| `ENVI BODY_DONE=YES` | 0 | 0 |

**（a）LOGS 泄漏 GetLastError**：183 = `ERROR_ALREADY_EXISTS`。
LOGS 打开文件（`CreateFileW` 带 `CREATE_ALWAYS`/`CREATE_NEW`）后，
Win32 成功置 last-error=183，**还原侧未保存/恢复 GetLastError**，
该值经动词返回值 → `script+0x50` → 退出码。
> AGENTS.md 探针纪律 V4 已明确记载此坑（"fopen("a") 成功会置 183，曾污染退出码链"），
> 此处是同一病理在生产代码里的实例。

**（b）IFEX 命中已存在文件即 AV（0xC0000005）**——**这是本次最严重的发现**：
- `IFEX <存在的文件>, ENVI R=x` → msvc 崩溃；`IFEX <不存在的文件>, …` → 正常。
- 且 **013_ifex_file 全语料是 orig=2 / msvc=2 双绿**，因为 013 的文件 `C:\pectest\out\demo.txt`
  在裁判环境下**不存在**，恰好绕开了崩溃路径。
- 061 因为 `LOGS` 先**创建**了 `s61.log`，使 `IFEX` 命中"存在"分支 ⇒ 崩。
  这解释了两案 exit 的耦合假象。

⇒ **061 应拆成两个独立缺陷单，且 (b) 优先级最高（崩溃 > 退出码）**。

---

## 5. 建议修复方案（按优先级，均需先过 dc 核验）

1. **【P0·崩溃】IFEX 命中已存在文件 → 0xC0000005**
   - 定位 `PECMD_IfexFindExecutor`（`src/commands/core_b2a.c:139` 区）在"文件存在"分支的取址/解引用。
   - 建议：先在 `IFEX <存在文件>` 路径上 `attach_process` 抓栈（`~* k 30`），
     按 `vgate-live-debug` 技能取址（唯一来源：`build/msvc/symsnap.txt`）。
   - ⚠ 该路径影响面远大于这 3 案（任何 `IFEX <存在的东西>` 都可能崩）。

2. **【P1·退出码污染】`local_317`(mode) 门控失真 —— SET 与所有 `&NAME=value`**
   - 位置：`src/ui/core_b7c.c:3656`（dc:124238）。
   - 现行代码把 `local_317 != '\0'` 当作"进入 VarLookup 分支"的正向条件；
     需回原版 EXE 反汇编确认 mode≠0 时 `r28` 的真实来源
     （是 `iVar5` 还是 `iVar20`/其他），**不要直接删条件**（禁止行为注入）。
   - 修好这一处，SET 与 `&` 系列一并收敛（覆盖 031）。

3. **【P1·退出码污染】LOGS 的 GetLastError 泄漏**
   - 在 LOGS 打开文件的 **调用点前后保存/恢复 GetLastError**（与 AGENTS.md V4 探针纪律同手法）。
   - 覆盖 061 的 183 分量。

4. **【P2】TEAM 内 `!` 值返回 2**
   - 先确认 `PECMD_ParseCommandBlock` 里 `|` 切分后子命令的返回值传播规则，
   再决定是否动 `arr[3]` 初始化（**dc 原文未初始化，须先在原版 EXE 反汇编定案**）。

5. **【P2·工具链】`run_case.py` 陈旧产物污染**
   - `run_exe()` 增加 `shutil.rmtree(result_dir)`（或对 `ARTIFACTS` 逐个 `os.remove`），
   或按 `manifest["vars_val"]` 门控回捞，避免 8/27 残留件被误读为当次证据（§1.1）。

---

## 6. 未能定案 / 需登记 divergences 的事项

| 项 | 状态 |
|---|---|
| `PECMD_ParseValueCommand` 的 `arr[1]/arr[2]` 未初始化是否致病 | **未证实**，dc:22659-22664 原文如此，禁止擅自补齐 |
| TEAM 内 `!` 产生 2 的确切写入点 | **未定位到具体行**，仅复现到现象 |
| `FUN_140025474`（`PECMD_ParseCommandBlock`）的 `|` 切分细节 | 本次未展开 |
| LOGS 具体是哪个 Win32 调用置了 183 | 推断为 CreateFile，未反汇编确认 |
| IFEX 崩溃的精确指令 | 需活体 `attach_process` 抓栈（本代理未构建/未调调试器） |

---

## 7. 附：本次使用的关键行号索引

| 内容 | 位置 |
|---|---|
| 退出码写入（dc 真值） | **dc:45139-141** |
| 退出码写入（还原） | `restored_bodies.c:6883-6886` |
| 退出码读出 | `src/app/core_init.c:69-72`；`src/app/core_main.c:174` |
| 动词构造 `uVar21<<8\|uVar29` | dc:44212 / `restored_bodies.c:6212` |
| SET 分派（mode=1） | dc:44223-44227 / `restored_bodies.c:6222-6236` |
| ENVI 分派（mode=0） | dc:44557 / `restored_bodies.c:6573-6582` |
| TEAM 分派 | dc:44660 / `restored_bodies.c:6678-6682` |
| LOGS 分派 | dc:44854 / `restored_bodies.c:6872-6878` |
| EvalSpecialToken 定义 | dc:27011 / `src/commands/core_b2e.c:1358` |
| mode 分道判据 | dc:27100, 27122, 27133 / `src/commands/core_b2e.c:1451,1469,1485` |
| TEAM 处理函数 | dc:22656-22667 / `src/commands/core_b2a.c:94-101` |
| LOGS 处理函数 | dc:23049 |
| EnviMemReadWrite | dc:122536 / `src/ui/core_b7c.c:2011` |
| **`local_317` mode 门控（根因核心）** | **dc:124237-124238 / `src/ui/core_b7c.c:3656`** |
| `&` 加深 mode | dc:122677-122682 / `src/ui/core_b7c.c:2147-2153` |
| `!` 分隔符扫描 | `src/ui/core_b7c.c:817-864` |
| 未识别动词落点 | `restored_bodies.c:6253` `LAB_14004df3c:` |
