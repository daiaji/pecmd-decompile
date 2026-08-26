# s20 — SUB 族崩溃取证档案（dump 7712 系）

> 2026-08-26 Round14 续 | 主代理逐案排查 SUB 用例(021_sub_call/037_sub_nested/038_sub_param_shadow)
> 关联：batch-A 报告(s13_r1_dualbody_batchA.md) #012/#023 漏&已修；本档登记**修复后仍存**的崩点。

## 1. 现场：pecmd_msvc.exe.7712.dmp（2026-08-26 07:08:25, base 0x7ff7158b0000）

异常（.exr -1）：
```
ExceptionCode: c0000005 (read AV)
ExceptionAddress: ntdll!RtlProtectHeap+0x1a77
指令: mov rcx, qword ptr [rax]   ; rax=0 → 读地址 0
```

调用链（backtrace 真实 unwind）：
```
13 ntdll!RtlAllocateHeap+0xad4        ← 分配器内部炸
14 pecmd_msvc!PECMD_HeapRealloc+0x34     [pecmd_msvc+0x10b954]
15 pecmd_msvc!PECMD_AllocString+0x28     [+0x10be88]
16 pecmd_msvc!FUN_140063720+0x1d         [+0x1e4e4d]  (=PECMD_AllocString 别名壳)
17 pecmd_msvc!FUN_140063888+0x61         [+0x1e4ac1]  (StrCopyW 族)
18 pecmd_msvc!PECMD_StrBldCopyWide+0x24  [+0x19f7f4]
19 pecmd_msvc!PECMD_ProcessScriptBlock+0xa62  [+0x1a1aa2]
20-29 DispatchExpressionBlock→RunScriptText→srx_ExecuteScriptFile→RunCommand→PSB→…→MainW
```

寄存器（.ecxr）：rax=0 rbx=堆基址段 rdx=0x1c00(请求尺寸相关?) rsi/rdi/r12-r15=堆内部结构指针。

## 2. 初步分析

- AV 点在 **HeapAlloc 内部遍历堆结构时解引用空指针**——不是请求巨尺寸（rdx=0x1c00 合理），
  而是**堆元数据已被此前的写操作破坏**（free list/segment 链上有 NULL 条目被当作合法节点）。
- 即：崩点是**受害者**（检测者），坏写者在更早的执行窗口。
- 触发行 = PSB+0xa62 处的 StrBldCopyWide（动词级联里构建串槽），属 SUB 行处理高频路径——
  任何先于此的堆破坏都会在此显形，位置不特异。
- 嫌疑面（按 batch-A 报告与 R14 变更收敛）：
  a) **SUB 克隆链**：批次3 转发的 ScriptCopy(FUN_140017CDC) 深拷贝 0xe0 结构 + ScriptInit 清零字段集
     ——若移植版结构布局与 dc 不一致（如 +0x70 串槽偏移），深拷贝会写穿；
  b) batch-A #001 WideStrLen 已修(+1)——但依赖其返回值的分配尺寸调用点是否全部同步？
     若某处用 `len+1` 分配而写入方仍按旧语义写 len+2 字节 → 尾部越界写；
  c) 031 EncodeDet 重写后 core_script.c:269 的 enc 判定路径变化（非 CMP 数据返 0 → XOR 解码分支）
     ——LOAD 子脚本若误入 XOR 分支会污染串内容但不该破堆；低嫌疑；
  d) FUN_14005C788 恢复纯前缀语义后，SUB 参数解析可能走进原未达分支（新暴露的上游缺陷）。

## 3. 待办（下一轮）

1. 其余 4 枚 dump（5076/13760/1920/2200）逐一 backtrace——确认同签名还是多点分布；
   同签名=单根因；异签名=按族分治。
2. 对 PSB+0xa62 反汇编定位源码行（SUB 分支的哪一步），确认 StrBldCopyWide 的 src/dst 槽。
3. 对照 dc 的 SUB 处理链（dc:44xxx SUB 分支 ↔ rb 移植体）核对克隆三连
   （ScriptCopy→ScriptInit→InitListHead dc:82268-73）在 SUB 路径上的完整性与字段偏移。
4. 若静态对照无果，上 [SCC]/[SINIT] 探针（源码级，禁 stdio.h）跟踪克隆块分配尺寸 vs 写入尺寸。
5. HeapRealloc 入口探针打印 (ptr,size) 序列，找 size 异常或 ptr 非本家族头的调用点。

## 4. 纪律提醒

- dump 会话只读（专档 B）；构建前 end_session（专档 C，R14 实战 LNK1201）。
- 栈搜索类命令(s -dq)会引爆引擎（专档 A 补充触发面）——改源码推理。

---

## 5. R14b 补充现场(六项 DEVIATION 修复后全量复验)

- **SUB 三案**: 崩溃已消除 → **exit=124 挂死**(死循环, ExtractTableSegment 真体后链路推进但循环不退出)。下一嫌疑: ParseCommandPath/EnviMemReadWrite/SUB 参数替换的游标推进。
- **041 B簇**: MDIR 分支(rb:6855)传 local_res8=NULL → CreateDirectoryTree+0x18 首解引用 AV(dump 17448)。dc:44828 同传 local_res8 ⇒ PSB 前置参数准备段对该行型漏填, 待追。
- **025_team_with_if 回归登记**(18→17): 内容全对(done=OK), exit=0xC0000374。TEAM 内 `IFEX $%A%=1` 行经批次3 激活后走 ExpandEnvVars 展开链:
  - dump 15532: **ExpandEnvVars+0x1a9c 读 NULL+0x18**(AV, 变量槽/串槽未初始化) ← ExpandVarsRecursive ← ExpandVarDispatch ← PSB+0xbca
  - dump 16752: **gdi32full 线程 LdrShutdownThread 退出路径堆校验爆**(D-11 同族线程竞态变体)
  - 判定: 批次3 激活 IFEX-in-TEAM 展开链后暴露的上游未修缺陷(非批次3 本身错); 024/031 净增收益保留, 不回退。
- 下轮入口优先级: ① ExpandEnvVars NULL+0x18 槽初始化链(025) ② SUB 挂死循环点 ③ MDIR local_res8 填充 ④ 退出码传播(S17 UNVERIFIABLE, 17 案杠杆) ⑤ FUN_140034788 去桩(IFEX/FIND 激活)。
## 6. 退出码机制全链破解(R14c, MCP 修复后活体取证)

**原版链(活体 dd 实锤)**:
1. CRT init(dc:2220-21): DAT_14013d180=&DAT_14013caf0
2. ScriptInit(g_Script,0)(dc:13348/13368 parent==0 分支): *(this+0x50)=this+0x58 -- 覆盖 CRT 别名, 自指 &g_Script+0x58(即 14013d188)
3. PSB 尾部每行(14004c533 mov [rax],rdx): 非零动词返回值写 *(script+0x50) => 写 g_Script+0x58
4. GetExitCodeGlobal(dc:54066 单层解引用) -> mainW return -> 进程 exit code

**实测**: boot "LOAD xxx" 行写回 rdx=2(=WRITE 返回值逐层传播) => 进程 exit=2 吻合

**msvc 现状**: 读侧已修(core_init.c GetExitCodeGlobal 沿 g_Script+0x50 链), 自锚正常(槽=&this+0x58), 但缓存值=183(0xB7) != 2 -- msvc 的 LOAD 执行链(srx_ExecuteScriptFile->RunScriptText->PSB)某层把返回值算成 183。
**下轮断点**: msvc 版 PSB 写回指令(map 内 LAB_14004c525 对应处)bp 序列读 rdx; 或对照 dc RUNSCRIPT/srx 返回路径 dc:30060-30066 + FUN_140031068 尾部。

注: MCP 上游修复验证 -- execute 控制单语句生效、会话不再半死(.lastevent 全程可用); ba 数据断点路径仍 0x8000FFFF(未覆盖, 避走软 bp+组合命令拆分)。
## 7. 183 真相与真根因定案(R14c 末)

[RSTX] 实测: boot 行与 LOAD 文件行两轮 RunScriptText ret=0 => msvc 的 PSB 对 "LOAD xxx" 行 local_180=0, 写回条件(非零)不满足; g_Script+0x58 的 183 是其他子系统复用残留(非退出码)。
**真根因**: 原版 boot LOAD 行经 PSB LOAD 分支把 srx 执行链结果(=末命令返回值 2)接进 local_180 并写回; msvc 的对应分支(或 MAIN 排队路径)丢弃了该结果。
**下轮切入点**: 对照 dc:44xxx PSB 的 LOAD/bare-path 分支如何把 RunCommand/srx 返回值装入 local_180(dc:44821-44831 邻域及 LAB_14004c53b 出口), 与 rb:6305-6321(srx 已返 local_180 但上层是否继续传播)逐层核对; 重点验证 MAIN 排队->主循环->RunCommand 返回值是否回流。

## 8. WRITE 返回值工单(R14c 收敛点, 下轮首攻)
[WB] verb=54495257(WRIT) l180=183 => 写回执行、缓存=183、exit=183 全链自洽; 唯一缺口=PECMD_WriteFileEncoded(core_b3r_h3.c:1825, dc:99499 size=4921)成功返回 183 != 原版 2。
取证: 尾部 return LVar31(:2633); LVar31 来源 SetFilePointer×6/local_res10/local_88/LVar30(:2628)。dc 尾部 LAB_1400a0829 return uVar6(:100374)。
下轮动作: 对照 dc uVar6 全部赋值(成功路径应=2)与 msvc LVar31 链, 定位差一层的位置; 同族 READ 体 FUN_14009da54 预计同病(READ golden 也=2)。

## 8.1 WRET 补充(R14e 末): 探针@2628 未打印 => 183 不经 LVar31=LVar30 汇合点, 来自其余赋值点(SetFilePointer×6 @2376-2481 / :2247 local_res10 / :2345 local_88)。注意 183=0xB7=ERROR_ALREADY_EXISTS 形态。下轮: 在 :2247/:2345/:2376-2481 各点加探针, 或直接对照 dc:99605/99643(LVar31=1 出口)与 dc 成功流终值。

## 9. WRITE 工单方向修正(R14g 末, 铁证)
- WriteFileEncoded 入口软 bp 未命中(进程完整跑完) + [WIN] 入口探针未打印 + **out.txt 根本未产出**
- => WRITE 行在进入写入体之前即失败; l180=183(0xB7=ERROR_ALREADY_EXISTS 形态)是失败路径错误码, 非成功返回值
- 此前 §8 "返回值 183 vs 2" 方向作废; 新工单: 定位 WRITE 行的前置失败门(ECD 入口守卫/参数展开/文件名构造), 方法=CreateFileW 序列对照(原版 vs msvc)+ECD 入口探针
- [WB] 的 line=%.24ls 打印出上轮日志残留 => LVar11 尾部读数不可信, verb 槽亦可能残留, 以 API 序列为准

## 10. IsPecmdScriptFile 真体落地后的下一层(R15a 末)
- 桩→真体(dc:12709)后: boot 行 LOAD→srx 链恢复(OFH main.pecmd ×2 ✓); 但内层 WRITE 行仍失败:
- **[OFH] h=0 path="[ECD] line=WRITE..."** —— bare-path 段(rb:6280-6302)构建的 local_f8 内容=堆上残留的旧日志文本 ⇒ 源游标(LVar15/参数准备)错位
- 下轮: rb:6280-6302 ↔ dc:44255-44290 逐行对照(local_res8/LVar33/local_f8 构建序), 定位错位的源指针; 同时核对 IsPecmdScriptFile 对该场景应返什么(dc 语义下 out.txt hello 非脚本应 -1/0 → ECD 兜底, 原版 exit=2 的 2 来自 ECD 兜底执行失败码? 或另有路径 —— 需原版活体同点观测)

## 11. bare-path 判定输入实锤(R15b 末)
[BP] e0=0 f8=[] —— IsPecmdScriptFile 收到空串(原版同点应收到 WRITE 行首 token "WRITE"→返0走ECD? 或整段语义另考)。
上溯: :6292 ExpandVarDispatch(script, cursor, &local_160,0,1) 展开结果未落 local_160 → :6293-6299 提取 token 长 0。
下轮: 对照 dc:78813-78824(FUN_14007bf44 分派)+FUN_14007bda8/14007a224 体, 查 msvc rb 版 ExpandVarDispatch(restored_bodies.obj @14019ff70)的 param_3 写出链; 同时复核 D-14 修复(param_2 层级)是否误伤本路径。

## 12. R15 末新形态登记(Round4, 提交 07504dd 后)
IsPecmdScriptFile 真体生效后 001 出现 exit=259(0x103) 新形态:
- 活体 dd 实锤: **g_Script+0x50 == NULL**(应为由 ScriptInit(parent==0) 自锚写入 &this+0x58)
- GetExitCodeGlobal 读 *(NULL) 触发 first-chance AV(SEH 吞掉)→ 返回值不定 → 259 形态
- 工单: ① 核对 core_init.c:92 FUN_1400186BC(&g_Script,0) 与 core_exec5.c:143 自锚写是否实际执行(疑后续清零覆盖或双份 g_Script COMMON 合并问题, map 显示 g_Script 在 <common> 段) ② 259 数值来源追溯
- 注: 本轮曾发生"回滚源码未重建部署"事故导致 63 案 0/63 假基线, 已重建纠正; 教训入 SOP: 回滚源码后必须立即重建部署再测。

## 13. EXEC=259 定位(Round5)
[WB] 实锤: verb=EXEC(0x43455845) l180=259(STILL_ACTIVE) ×2行 => ECD 的 EXEC 路径在子进程未结束时读取了退出码(缺 WaitForSingleObject 或读码时机早于等待)。
对照: 原版同场景 EXEC 成功后进程 exit=0(golden) => 原版 ECD 正确等待并传播子进程退出码 0。
修复方向: 对照 dc FUN_14000e26c 的 EXEC 分支(CreateProcessW→等待→GetExitCodeProcess 序列)与 rb 版对应段, 补齐/修正等待。
另: [WB] line 探针曾打印日志残留 => LVar11 尾部读数不可信(槽复用), 以 API 序列与 verb/l180 为准。

## 14. R15 收官补充(Round5)
- dc FUN_14000e26c 的 :11165-11209 段与 rb:3700-3760 **逐行同构确认**(WaitForInputIdle/WriteSysAck/句柄数组/WaitHandlesOrMessages/PatchRemoteWinExec 全对应)
- EXEC=259 的差异点收敛至 dc:11260 之后的"等待子进程+GetExitCodeProcess 写变量"深水段(local_ddc&8 分支后) —— 下轮以该段为锚逐行对照 rb 对应区
- [WB] line 探针的日志残留读数问题已定位(LVar11 尾部槽复用), 后续探针改用局部快照

## 15. EXEC=259 复现确认(Round6)
全量重跑(健康 exe): EXEC 行 l180=259 写回缓存 => 001 exit=259。
机制定案: ECD 的 EXEC 主路径(CreateProcessW 成功后)读取子进程退出码时机早于子进程实际结束(cmd /c echo 需数百 ms), 读到 STILL_ACTIVE(259) 并作为动词返回值传播至 PSB 写回缓存。
原版对照: 同场景 golden exit=0 => 原版 EXEC 成功返回 0(或等待完成后读码)。
修复锚点: rb ECD 内 EXEC 分支(CreateProcessW :3288/:3494 之后)补 dc 对应的等待语义(dc:11260+ 段为管道流, EXEC 等待在更前); 或最小修复=EXEC 成功路径显式 WaitForSingleObject(hProcess, timeout) 后再取码。
影响面: 001/024 等 EXEC 用例的退出码污染; 内容产物不受影响。

复方向(Round6-7)
[WB] 实锤: verb=EXEC(0x43455845) l180=259(STILL_ACTIVE) => ECD 的 EXEC 路径读子进程退出码时机早于结束。
- slot50 反转确认: g_Script+0x50 自锚正常(ScriptInit dc:13368 parent==0 分支), 此前活体读 NULL 系地址换算错误(每 launch base 变化, 手算易错 -- 教训: 用 dd poi() 相对寻址或源码探针)
- EXEC=259 写回缓存 => GetExitCodeGlobal 读出 259 => exit=259
- 原版对照: 同场景 golden exit=0
修复锚点: rb ECD 内 EXEC 分支(CreateProcessW :3288/:3494 之后)补 dc 对应的等待语义(dc:11260+ 段为管道流, EXEC 等待在更前); 或最小修复=EXEC 成功路径显式 WaitForSingleObject(hProcess, timeout) 后再取码。
影响面: 001/024 等 EXEC 用例的退出码污染; 内容产物不受影响。

## 16. WRITE 行掉 bare-path 的机制定位(Round8)
[WB]+[BP] 联合读数: WRITE 行到达 bare-path 段(:6284-6310)且 f8=空 => **DispatchBuiltin 未拦截 WRITE**。
DispatchBuiltin 真体健康(core_b1_remaining.c:5614), 其匹配依赖 g_cmdTable1/g_cmdTable2 运行时注册表:
- g_cmdTable1: 静态注册(RegisterFileAssociations dc:6897 启动段)
- g_cmdTable2: LOAD: 资源行扫描填充(dc:100374 同函数)
下轮工单: ① 活体验证 g_cmdTable1/2 的 count 值(预期非零) ② 若为零, 排查 RegisterFileAssociations 在 msvc 的执行与 INDATA 资源(FUN_14001ea18+ResDecode)加载链 ③ WRITE/ENVI 等 verb 的表项归属确认

## 17. WRITE 行全链闭合(Round9)
dc:44254-44291 与 rb:6270-6310 逐行同构确认。WRITE 行实际流:
ExpandVarDispatch(展开变量) → FUN_140024C48 提取首 token="WRITE" → local_f8="WRITE"
→ IsPecmdScriptFile("WRITE"): FUN_140101E70 打开失败 → 返 -1 ≠ 1
→ ECD 兜底: CreateProcessW("WRITE C:\...out.txt hello") gle=2 → 错误码写缓存 → l180=183
**缺失环节**: 原版在 bare-path 之前应有命令表拦截(WRITE 是内置命令), msvc 的 DispatchBuiltin 表(g_cmdTable1/2 count=0 活体实锤)为空 → 拦截失效。
工单: 追查 g_cmdTable1/2 注册内容来源(dc RegisterFileAssociations 的 INDATA 资源扫描+静态模式段在 msvc 的对应执行), 以及原版 WRITE 动词在 PSB 级联(0x54495257 @rb:6938)与 DispatchBuiltin 表两条路的先后与覆盖关系。

## 16. EXEC=259 干净复现定案(Round12, 污染排除后)
彻底清洁测试(log 删除验证)铁证:
- slot50(g_Script+0x50)=00007FF6745F7F58 **全程稳定非 NULL**(前轮读 NULL 系地址换算错误, 已纠正方法论)
- ENVI 行 l180=0(不写缓存); **EXEC 行 l180=259(STILL_ACTIVE)写回缓存**
- => 进程 exit=259 的直接原因=EXEC 动词处理器返回值
- 原版 golden exit=0 => 原版 EXEC 成功返 0(等待完成)或返 0 不触发写回
工单: 定位 EXEC 行的实际处理器(rb ECD 内 0x43455845 分支), 补 dc 对应等待逻辑(dc FUN_14000e26c EXEC 段), 使成功 EXEC 返 0。


## 16. EXEC=259 clean repro (Round12)
- slot50 (g_Script+0x50) stable non-NULL through run: self-anchor OK
- ENVI lines l180=0 (no cache write); EXEC lines l180=259 STILL_ACTIVE written to cache
- process exit = 259 directly caused by EXEC handler return value
- original golden exit=0: EXEC success returns 0 (after wait) or no write-back
- fix direction: locate EXEC handler in rb ECD (0x43455845 branch), add dc-matching wait logic before exit-code read


## 17. R16 认知边界登记(Round13 末)
- 干净复现: WRITE 单行脚本 exit=0xb7(183), out.txt 未产出(与既往一致)
- 探针网现状: [WIN]入口/[WRET2]出口均未打印 => WriteFileEncoded 主体未执行或早退点在其第一条语句前
- [BP]/[WB] 确认行到达 PSB 尾部且 local_158=WRIT/local_180=183
- 矛盾点: 分支体首探针未打印但尾部有返回值 => 需核对 rb:6939 编译后的实际调用目标(ILT thunk 解析), 排除同名双体/链接错位
- 下轮首动作: windbg 反汇编 rb:6939 调用点的实际 call 目标地址, 与 map PECMD_WriteFileEncoded(1400700b0) 比对


## 18. R17 认知边界终登记(Round13 末)
- Ghidra 导入 msvc exe 分析: WriteFileEncoded(1400700b0) 唯一引用=DATA(函数指针表条目 1402ca7f4), 无直接 CALL
- 全指令扫描: 无任何 cmp 0x54495257 直比指令 => PSB 级联对 WRITE 的分发经函数指针表间接进行, [WB] 的 verb 槽读数为残留值不可信
- 可靠事实链保持: exit=0xb7(183)/out.txt 未产出/WriteFileEncoded 未被调([WIN] 探针+bp 双证)
- 下轮首动作: 定位函数指针表 1402ca7f4 所在数组结构与其填充者(RegisterFileAssociations 静态模式段?), 从表内容反推 WRITE 行的真实处理器; 或在 DispatchBuiltin 内部加免解引用探针打印每次比较的表项名


## 19. R18 关键对照: 原版 vs msvc 的命令表与退出码槽(Round15)
**原版活体 dd(GetExitCodeGlobal 断点命中时)**:
- 14013ca98(cmdTable1Count?)=0 / 14013a078=0 / 14013a080=0 —— **原版计数同样为 0, D-16/D-17 表空假设被推翻**
- 14013a050=0x4011d4b8(非空堆指针)
- **14013d180=0x4013d188(堆缓存指针), 该缓存处值=2(WRITE 成功码)**

**结论修正**:
1. DAT_14013d180 是独立全局指针变量(非 g_Script 字段! S17 的"g_Script+0x50 等价"系地址相邻误判)
2. DispatchBuiltin 表空在两边一致 => WRITE 行不走 DispatchBuiltin, 走 PSB local_158 级联(rb:6939 WriteFileEncoded) => D-16/D-17 工单方向作废
3. msvc 的 GetExitCodeGlobal 读侧修复(g_Script+0x50 链)是**错的**, 应恢复读 *g_pExitCode(g_exitCodeCache)
4. 真正缺口=**谁往 g_exitCodeCache 写入最终退出码**(msvc 无写入者恒 0; 原版经某路径写入 2)
下轮: 全树搜 g_exitCodeCache 的写入者; 若无, 对照 dc 找 DAT_14013caf0(=g_exitCodeCache 对应物)的写入函数并补移植。


## 17. R18 部署纠偏与现状固化(Round13)
- 部署事故根因: 残留 GUI 进程锁文件, Stop-Process 后需 taskkill+Move 绕过; 已建 "杀进程-重命名-复制" 部署序列
- 干净部署后行为固化复现: exit=0xb7(183)/out.txt 未产出/WriteFileEncoded 入口探针未触发(双证: 探针缺失+[BP]未命中)
- [WB] verb=WRIT 为 local_158 打包值(可信); l180=183 的来源处理器**不是** WriteFileEncoded(rb:6939 分支未进入)
- 下轮首动作: 用 Ghidra msvc 工程反编译 PSB 的 0x54495257 分支体(Ghidra 地址需按 map 重算), 核对编译后的真实 call 序列; 同时以 DispatchBuiltin 内部探针打印每次表项比较, 确定 "WRITE" 是否到达表匹配


## 18. EXEC=259 最终工单(Round13, 上下文极限收尾)

**现象**: 所有含 EXEC 行的用例 exit=STILL_ACTIVE(259)。内容产物正确(done/vars 一致)但退出码污染。

**根因链(多轮活体+探针实锤)**:
1. EXEC 行进 PSB 级联 → 匹配 EXEC 分支(rb:6362-6366) → 调 ExecCmdDispatch
2. ECD 内部 CreateProcessW 成功(cmd.exe 启动, echo 正确执行, vars.txt 内容一致)
3. 但 ECD 返回 259 —— 某处 GetExitCodeProcess 在子进程结束前读取了 STILL_ACTIVE
4. PSB 尾部写回: 259 ≠ 0 → 写入 g_Script+0x58 缓存 → GetExitCodeGlobal 读出 259 → exit=259

**原版对照**: 同场景 golden exit=0 => 原版 ECD 对成功 EXEC 返 0(等待后取码或直接返 0)

**修复方向**(下轮首动作)
A. 快速修: 在 ECD 内 EXEC 成功路径(CreateProcessW ret!=0 后)显式补 WaitForSingleObject(hProcess, timeout)+GetExitCodeProcess, 替代当前的异步句柄收集模式
B. 正确修: 对照 dc FUN_14000e26c 中 EXEC 分支的完整返回值组装逻辑(dc:11260+ 深水段), 找到 msvc 缺失的等待步骤

**影响面**: 001/024 等 EXEC 用例退出码异常(内容不受影响)。与 WRITE 体返 183(s20 §8.1)同属"动词处理器返回值语义错误"家族。

**当前基线**: 17/43 PASS。EXEC=259 影响约 10 案(001/024 及其他含 EXEC 行的用例的退出码维度)。

## 19.1 EXEC=259 精确修复点锁定(Round16)
rb:2292 `local_df8 = local_db4` —— local_db4 来自 GetExitCodeProcess(hProcess,&local_db4) 读到 STILL_ACTIVE(259)。
**修复**: 在 rb:2178 GetExitCodeProcess 调用前补 WaitForSingleObject(local_af8.hProcess, timeout), 使子进程完成后才读退出码。
需对照 dc 原文确认原版的等待时机与超时值(可能为 INFINITE 或与 -wait/-timeout 修饰符关联的超时)。