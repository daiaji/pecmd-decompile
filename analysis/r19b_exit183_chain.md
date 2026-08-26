# R19-B 簇 exit=183 根因链分析（WRITE 族 + 061 + 065，msvc=183 vs 原版=2）

数据源：`C:\pectest\memfail.log`（本轮探针日志；065 段 = 行 7895–8062）。
对照权威语义：`D:\repo\PECMD反编译\decompiled.c`（下称 dc）；我方还原 `refactored/`（下称 rb）。

---

## 0. 结论（TL;DR）

**183 不是语义链错误，也不是原版有额外收尾步骤；是 `[CPW]` 探针自身的副作用污染了
Win32 LastError。**

- 失败链真实值是 **gle=2**（ERROR_FILE_NOT_FOUND，CreateProcessW("WRITE …") 找不到程序）。
- 我方 `PECMD_CreateProcessW` 包装器的**出口侧探针**在读完 gle 之后执行
  `fopen("C:\\pectest\\memfail.log","a")`——对**已存在文件**以 OPEN_ALWAYS 打开，
  成功时 Win32 约定置 LastError = **183（ERROR_ALREADY_EXISTS）**。
- 随后 ExecCmdDispatch 在 LAB_140014c72 处 `local_df8 = GetLastError()` 读到的是被探针
  改写后的 183（dc 同位置读到 2），该值成为函数返回值 → 写入结果槽 → 进程退出码。
- 原版无探针 → LastError 保持 2 → 返回 2 → 退出码 2 = golden。与 core_init.c 中
  R14b 触发面表「WRITE→2」完全吻合。
- 065/061 之所以同为 183：PSB 尾部只写**非零**动词返回值（dc:45139-141），WRITE 先把
  槽写成 183，后续 GETF/IFEX/ENVI/EXEC 全部返回 0 不覆盖 → 收尾读槽仍 183。

任务给的两个候选判定：(a) 把中间状态误当退出码 —— **否**（槽语义与 dc 一致且已由
R14b 活体 dd 实锤）；(b) 原版额外收尾被我方遗漏 —— **否**。真因是 **(c) 探针 I/O 污染
LastError**，属"观测装置改变了被观测值"。

---

## 1. 065_getf_attr 轨迹解码表（最后一轮完整用例）

脚本体（log:7923–7930）：`WRITE %F%,attr65` → `GETF G,%F%` → `IFEX $%G%>0,…` → ENVI×3 → EXEC×2。

| log 行 | 标签 | 含义 / 现场 |
|---|---|---|
| 7895 | `PROBE StartupScript enter cmd=C:\pectest\pecmd_msvc.exeLOAD C:\pectest\065_getf_attr\run_all.pecmd` | mainW 启动脚本链入口（core_main.c:76 探针）。注意 exe 名与 LOAD 参数无空格拼接是原版命令行形态 |
| 7896–7897 | `[S7] mode=0 autoFirst… br-mode0` | core_script2.c S7 二分标记：脚本解析模式 0、无自动段 |
| 7898 | `[OFH] h=…260 disp=3 fl=00000000 path=C:\pectest\065_getf_attr\run_all.pecmd` | PECMD_OpenFileHandle(rb core_exec2.c:178=@140003864) 以 OPEN_ALWAYS 打开脚本文件成功 |
| 7899–7906 | `[S8] SCWin/StrCopyW`、`[RSTI]`、`[ADOPT]`、`[DEB] ent head=LOAD …` | 字符串构建/脚本块重排（RSTI=ResetTextInit，ADOPT=缓冲块认领），外层 LOAD 行解析 |
| 7908–7913 | `[DEB]/[PSB]/[S8] ExpandVR-out b=15` | 内层行 "LOAD …" 进入行分发器；ExpandVR=变量展开（出参长度 0x15） |
| 7914–7917 | `PROBE RunCommand enter / before SrParsePrefix`、`[OFH] h=…268 path=…run_all.pecmd`、`[S10] LOAD chars=359` | RunCommand 重入读内层脚本；S10=LOAD 取词 |
| 7918–7931 | `[RSTI]/[ADOPT]/[DEB] ent head=ENVI F=…` + 原文回显 | 内层脚本文本装配完成，首行 ENVI F=… |
| 7933–7941 | `[DEB]/[PSB]/SCWin/StrCopyW(b=22)/ExpandVR×2/[WB] verb=49564e45 l180=0` | 行 `ENVI F=C:\pectest\out\s65_attr.txt`：ENVI('ENVI'=0x49564e45) 分支执行成功返回 0 |
| 7943–7944 | `[DEB] line=WRITE %F%,attr65` / `[PSB] line=WRITE %F%,attr65 slot50=00007FF79BE97F98` | 行进入大分发器；slot50 = *(script+0x50) 结果槽指针（即退出码槽，见 §2.5） |
| 7945–7954 | SCWin/StrCopyW(b=10)/ExpandVR(out b=14)/SCWin/StrCopyW(b=5)/ExpandVR(b=14)/SCWin/StrCopyW(b=0) | 展开变量得 `C:\pectest\out\s65_attr.txt`、内容 `attr65`(5字符)；b=0 的 StrCopyW 为空串拷贝 |
| 7955 | `[BP] e0=0 f8=[]` | **默认路径取证点**（rb:6300–6306，位于 LAB_14004df3c 内）：bare-path 判定输入为空 ⇒ WRITE 未命中 FourCC 表，走 default 分支（详见 §2.1） |
| 7956 | `[OFH] h=0 disp=3 fl=0 path=` ＋（跨进程交错合并的）`[ECD] line=WRITE C:\pectest\out\s65_attr.txt,attr65` | OFH：某次 OPEN_ALWAYS 打开失败/空路径（h=0）；ECD=PECMD_ExecCmdDispatch 入口探针（rb:261–267），参数为**展开后的整行** ⇒ 已进入"当作外部命令执行"的兜底路径。此行两条日志来自并发进程写同一日志的交错（本轮同时跑了 C:\pectest 与 D:\repo\windbg-mcp\.frozen-pecmd 两份 msvc 构建），行界失真不影响结论 |
| 7957–7963 | SCWin/StrCopyW(b=28)/SCWin(静态串)/StrCopyW(b=15)/SCWin(AWS槽)/StrCopyW(b=10)/`[AWS] src=\System32\cmd.exe` | EXEC 兜底共用 machinery 构建启动环境（AWS=AppendWideToSlot，往全局槽追加 "\System32\cmd.exe"，此处仅为共享代码路径痕迹） |
| 7964–7965 | `[CPW] cmd=NULL buf=… si/pi≠NULL flg=0` + `buf.str=WRITE C:\pectest\out\s65_attr.txt,attr65` + si.dump | PECMD_CreateProcessW(rb core_b9_remaining.c:194=@140101e04) 十参现场：lpApplicationName=NULL、命令行=整行 WRITE 文本 ⇒ 尝试把 "WRITE …" 当可执行文件启动 |
| 7966 | `[CPW] ret=0 gle=2` | **CreateProcessW 失败，ERROR_FILE_NOT_FOUND(2)** —— 这就是原版退出码 2 的来源值 |
| 7967 | `[WB] verb=54495257 l180=183 line=[WRITE C:\pectest\out\s65_att]` | 大分发器尾部探针（rb:7171–7179，对应 dc:45138 LAB_14004c525）：verb FourCC='WRIT'（local_158 只装前 4 字符），**l180 = local_180 = 动词返回值 = 183**（已被污染，真值应为 2，见 §2.4） |
| 7969–7975 | `[WB] verb=46544547 l180=0 line=[GETF G,C:\pectest\out\s65_at]` | GETF('GETF') 四字母动词正常命中 FourCC 表，返回 0；因 G 未取到数值，后续 IFEX 展开 `%G%`→`%?`（log:7983 显示 `$%?>0`） |
| 7977–8003 | `[WB] verb=58454649(IFEX) l180=0`、ENVI BODY_DONE/T_CASE `l180=0` | IFEX/ENVI 均返回 0 —— **都不覆盖槽里的 183**（尾部只在非零时写槽） |
| 8005–8047 | `[ECD] line=(null)`、EXEC 两连发 `[CPW] ret=1 gle=0`、`[WB] verb=43455845(EXEC) l180=0` | 两个 cmd.exe echo 重定向成功（vars/done 内容面与 golden 一致的原因）；EXEC 返回 0 |
| 8048–8061 | `[RSTX]/[REL]/[RCCLEAN]` ×2、`[S7] tail-0a…tail-7 hook`、`PROBE StartupScript done r=0x0`、`[MAINLOOP] next=NULL` | 脚本块释放、收尾钩子序列跑完；next=NULL ⇒ 主循环跳出 → after_main → `return PECMD_GetExitCodeGlobal()`（core_main.c:191）→ **进程退出码 = 槽值 = 183** |

---

## 2. 183 来源链（逐步，含两侧代码引证）

### 2.1 WRITE 为什么没走 WriteFileEncoded（[WIN] 从未出现）
- 全日志 grep `[WIN]` = 0 命中 ⇒ `PECMD_WriteFileEncoded`(@14009f070, rb core_b3r_h3.c:1829)
  在本轮从未执行。
- 结构性原因：FourCC 分发表被 `uVar21 == 0x404` 门控（rb:6261；dc 同构），uVar21 低字节 =
  动词标识符长度。"WRITE" 长 **5** ⇒ uVar21=0x505 ⇒ 直接落 LAB_14004df3c 默认分支。
  local_158 仍被打包成前四字符 'WRIT'，所以 [WB] 打印 verb=54495257（rb:6162–6221 打包逻辑）。
  dc:44913 的 WRIT 分支同理只服务 4 字母 token，普通 WRITE 在 dc 也走默认分支。
- 默认分支：`PECMD_DispatchBuiltin`(@140016bc8, rb core_b1_remaining.c:5614) 查 5 张运行期命令表
  （SUB/插件注册，本场景全空）→ 未命中返回 0 → 触发 ECD 兜底：
  `PECMD_ExecCmdDispatch(param_1, 展开后整行, …)`（rb:6289–6326；[BP]/[ECD]/[CPW] 即此路径产物）。

### 2.2 兜底把 WRITE 行当外部命令启动并失败
- ExecCmdDispatch 最终走到 rb:3288 `PECMD_CreateProcessW(NULL, "WRITE C:\…,attr65", …)`，
  日志实证 ret=0 gle=2（ERROR_FILE_NOT_FOUND）。fs 内容仍一致是因为 vars/done 由后续
  EXEC 正常产出，golden 对比面不含 demo.txt/s65_attr.txt 的缺失差异（或由用例自身容差覆盖）。

### 2.3 探针在失败与 GetLastError 捕获点之间改写了 LastError（根因）
- rb core_b9_remaining.c:217–227（包装器出口侧）：
```c
int r = CreateProcessW(...);
DWORD gle = GetLastError();          // ← 此处读到正确的 2 并打印
{ /* TEMP PROBE(S11): 返回现场 */
    pf2_ = fopen("C:\\pectest\\memfail.log", "a");   // ← 文件已存在!
    fprintf(pf2_, "[CPW] ret=%d gle=%lu\n", ...);
    fclose(pf2_);
}                                    // ← LastError 现在是 183
return r != 0;
```
- Win32 语义：`CreateFileW(OPEN_ALWAYS)` 打开**已存在**文件成功返回时
  LastError = ERROR_ALREADY_EXISTS(**183**)。UCRT `fopen(name,"a")` 底层即
  OPEN_ALWAYS|_O_APPEND。memfail.log 自首个探针起恒存在 ⇒ 每次 [CPW] 出口探针
  都把 LastError 置成 183。
- 回到调用方后，ExecCmdDispatch 在共享收尾标签处捕获：
```c
LAB_140014c72:
  local_df8 = 0;
  if (local_df4 == 0) local_df8 = GetLastError();   // ← 读到 183（dc 无探针读到 2）
  *(uint32_t *)local_990 = local_df8;               // 错误码出参
```
  （rb:2999–3003 为 ShellExecuteEx 分支同构体；rb:3616 `goto LAB_140014c72`；
  dc 对应体含同一 `local_df8 = GetLastError()` 捕获，dc 清单 10492–10497 区段可见同型。）
- 该错误码随后汇入函数返回值低位（rb:4157 `_Var39.ft.dwLowDateTime = local_df8;`，
  SEG10 注释"真实返回"）。
- 中间还有一处 `DVar21 = GetLastError()`（rb:3319，仅比对 0xc1==ERROR_BAD_EXE_FORMAT），
  读到 183/2 都不改变控制流 —— 但它证明污染发生在包装器返回之前即刻生效。

### 2.4 返回值 → 结果槽 → &&ERROR/ERROR → 退出码
- 大分发器尾部（rb:7170–7182 ≡ dc:45138–45141）：
```c
if (local_180.QuadPart != 0)
    (*(LARGE_INTEGER **)(param_1+0x50))->QuadPart = local_180;   // 最后一个非零动词返回值入槽
```
- 同尾设置 ERROR/&&ERROR 变量 = "183"、DAT_14013ccca=1（rb:7184–7194 ≡ dc:45143–153）。
- mainW 收尾（core_main.c:187–193 ≡ dc:5541–5547 / 5644–5649）：
```c
after_main:
    if (g_afterMain != 0) PECMD_BroadcastEnvChange();
    ret = PECMD_GetExitCodeGlobal();      // @14005b7dc
    return ret;                           // GUI 入口链经 ExitProcess(ret) 或 return
```
- `PECMD_GetExitCodeGlobal`（core_init.c:70–73，R14b 活体 dd 实锤）：原版 DAT_14013d180 与
  g_Script+0x50 同址，读 `*(uint32*)(*(uint64*)(g_Script+0x50))` —— 即上面那个槽。
- 因此：**槽里最后写入的非零动词返回值 = 进程退出码**。WRITE 槽入 183（dc 语义下入 2）
  ⇒ msvc 退出 183，原版退出 2。与 core_init.c:69 触发面表「WRITE→2 / ENVI,CALC,SUB,TEAM→0」吻合。

### 2.5 为什么 GETF(065)/logs_smoke(061) 也是 183
- 它们脚本体内都含一条 WRITE（065: `WRITE %F%,attr65`，log:7923）。WRITE 把槽置 183 后，
  其余动词全部返回 0，槽不再被覆盖（非零才写）；GETF 本身走 FourCC 表正常返回 0
  （log:7975），**不是** GETF 自身的错 —— 共用的是"槽+收尾链"，病灶仍在 WRITE 兜底的
  LastError 污染。

### 2.6 判定
- (a) 把中间状态误当退出码？否 —— 槽语义/收尾链与 dc 逐行一致，且 R14b 已活体验证。
- (b) 原版有额外收尾覆盖？否 —— dc 收尾就是 BroadcastEnvChange(条件)+GetExitCodeGlobal，
  我方 core_main.c 已具备。
- 真因 (c)：**探针 fopen("a") 的 OPEN_ALREADY_EXISTS 副作用污染了语义载重的 GetLastError**。
  凡"失败 API → (探针 I/O) → GetLastError()"的窗口都会中招；当前暴露面是 [CPW]
  （[OFH] 包装器同型风险，见补丁 B）。

---

## 3. 最小修复补丁（文本，未应用）

原则：不动任何语义代码，只让探针"透明化"——进出探针块保存/恢复 LastError。
两处均为 TEMP PROBE，后续拆除时自然消失。

### 补丁 A（必需）：core_b9_remaining.c — PECMD_CreateProcessW 出口探针

```diff
--- refactored/src/misc/core_b9_remaining.c
+++ refactored/src/misc/core_b9_remaining.c
@@ -217,11 +217,14 @@
     {
         int r = CreateProcessW(cmd, buf, sa, da, inherit, flags, env, cwd, si, pi);
         DWORD gle = GetLastError();
+        DWORD gle_restore = gle; /* R19b: 探针 fopen("a") 会把 LastError 改成 183(ERROR_ALREADY_EXISTS),
+                                    污染调用方 ExecCmdDispatch@LAB_140014c72 的 GetLastError() 捕获,
+                                    导致 WRITE 族/061/065 退出码 183 (原版=2)。退出前恢复。 */
         { /* TEMP PROBE(S11): 返回现场 */
             void *pf2_ = fopen("C:\\pectest\\memfail.log", "a");
             if (pf2_) {
                 fprintf(pf2_, "[CPW] ret=%d gle=%lu\n", r, (unsigned long)gle);
                 fclose(pf2_);
             }
         }
+        SetLastError(gle_restore); /* R19b: 还原探针副作用, 保持 LastError 透明 */
         return r != 0;
     }
```

### 补丁 B（同型防御，建议同批）：core_exec2.c — PECMD_OpenFileHandle 探针

OpenFileHandle 失败(h=0)同样可能被下游按 LastError 分诊（如管道/重定向打开失败路径），
探针同样会把它改成 183：

```diff
--- refactored/src/lang/core_exec2.c
+++ refactored/src/lang/core_exec2.c
@@ -188,11 +188,15 @@
     if (h == (HANDLE)-1)
         h = 0;
     *out = h;
     { /* TEMP PROBE R15(文件打开序列取证) */
+        DWORD le_restore__ = GetLastError(); /* R19b: 保护下游 GetLastError 消费者 */
         void *pf_ = fopen("C:\\pectest\\memfail.log", "a");
         if (pf_) {
             fprintf(pf_, "[OFH] h=%p disp=%u fl=%08x path=%ls\n", (void *)h,
                     (unsigned)disp, (unsigned)flags, path ? path : L"(null)");
             fclose(pf_);
         }
+        SetLastError(le_restore__);
     }
     return h;
 }
```

### 不需要改动的地方（明确排除）
- `PECMD_GetExitCodeGlobal` / g_exitCodeCache 相关：R14b 方案（读 g_Script+0x50 槽）正确，勿回退。
- 大分发器槽写入 `if (l180 != 0)` 条件：与 dc:45139 一致。
- WriteFileEncoded/FourCC 门控 uVar21==0x404：与 dc 同构，WRITE 走兜底属原版行为。

---

## 4. 修复后验证预期
1. 005/006/007/044/045/061/065 七案 msvc exit 由 183 → **2**，与 golden 对齐；stdout/vars/fs 不变。
2. memfail.log 中 `[CPW] ret=0 gle=2` 打印值不变（探针读取点在恢复点之前）。
3. 反向冒烟：任选一条会**成功**的 EXEC，其 `[CPW] ret=1 gle=0` 亦不受影响。
4. 残余风险审计项：其余落在"API 失败与其 GetLastError() 消费者之间"的探针块
   （[WB]/[PSB]/[BP]/[AWS] 等均在返回值已定之后打印，理论安全；如后续出现同类
   "差一位错误码"分簇，优先按本模式排查）。
