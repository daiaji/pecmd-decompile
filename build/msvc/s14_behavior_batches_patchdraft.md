# S14 行为批次补丁稿（提案，不落源码）

> 状态：三批次全部 **READY**（证据齐备、old_string 唯一性已核）。
> 权威语义源：`decompiled.c`（dc:<行号>）。汇编实证取自本机 Ghidra 静态工程 `PECMD.exe`
> （仅静态反汇编，未动调试器）。本稿只出提案，不改任何 `src/**`；唯一落盘文件即本文。

## 总览

| 批次 | 目标 | 判定 | 关键结论 |
|---|---|---|---|
| 1 | varargs×2：`PECMD_ScheduleSelfDelete`(b1:1291) / `PECMD_LoadFileMappingExec`(b1:3937) | **READY** | 两处均为 wsprintfW 被 Ghidra 吞掉的可变参；汇编逐寄存器实证实参并还原（含 dc 未显示的 `-1` 钳位分支） |
| 2 | rb:252 注入桩 `PECMD_PatchRemoteWinExec` | **READY** | 方向定为 (a) 删静态空桩 + extern 绑定 `src/commands/core_b2c.c:189` 真体（dc 忠实直移已在）；直移复刻会造成重复符号 |
| 3 | `PECMD_ScriptInit/ScriptCopy` 零参空桩转发 | **READY** | 真体在 `src/runtime/core_exec5.c`（FUN_1400186BC / FUN_140017CDC），签名逐一对照 dc；须同步归正 `stubs_common.h:2815/3113` 旧零参声明 |

说明：任务表述"va_list/vswprintf 族"，经核实两处均为 `wsprintfW(..., fmt, ...)` 可变参被
Ghidra 按双参原型吞参的同型伪影（工程内已有同型还原先例：rb:10545、rb:7161、
`src/runtime/core_var.c:100`、`src/kernel/core_strbld.c:108`），按此最合理解释处理。

---

### 批次1 varargs×2：ScheduleSelfDelete / LoadFileMappingExec 的 wsprintfW 缺参

#### 【现状证据】

**站点A** `refactored/src/misc/core_b1_remaining.c:1273-1295`（函数 `PECMD_ScheduleSelfDelete`，注 `@0x14000481c size=166`）：

```c
1273: void PECMD_ScheduleSelfDelete(LARGE_INTEGER param_1, int param_2)
1274: {
1275:     /* @0x14000481c size=166 自删除/等待命令构造 */
1276:     int iVar1;
1277:     uint64_t uVar2;
1278:     WCHAR *local_res18[2];
...
1285:         if (param_2 == 0) {
1286:             GetCurrentProcessId();            // ← 返回值丢弃（喂给后续变参调用的典型伪影）
1287:         }
1288:         iVar1 = lstrlenW((LPCWSTR)param_1.QuadPart);
1289:         PECMD_AllocWStringBuffer(local_res18, (int64_t)iVar1 + 200);
1290:         /* TODO(verify): Ghidra 未显示 wsprintfW 的变参 */
1291:         wsprintfW(local_res18[0], WSTR("PECMD**pecmd-cmd* WAIT *%lu -del \"%s\""));   // ← 0 个变参
1292:         PECMD_RestartSelf(local_res18[0]);
```

**站点B** `refactored/src/misc/core_b1_remaining.c:3871-3971`（函数 `PECMD_LoadFileMappingExec`，注 `@0x1400091e0 size=833`）：

```c
3933:         *lpBaseAddress = (int64_t)(iVar2 * 2 + 2);
3934:         memcpy((uint8_t *)(lpBaseAddress + 1), (const uint8_t *)param_1, iVar2 * 2 + 2);
3935:         memset(...);
3936:         UnmapViewOfFile(lpBaseAddress);
3937:         wsprintfW(local_168, WSTR(" *map:0x%p:%lu "));      // ← 0 个变参
```

两处均为"格式串带 `%lu/%p/%s` 却零变参"的占位形态；`wsprintfW` 在 `include/stubs_common.h:1209`
声明为 `extern int wsprintfW(unsigned short *, const unsigned short *, ...)`（真变参），补参可直接编译。
`WSTR` 宏见 `include/win32_stub.h:982`。

#### 【dc 原文锚点 + 汇编实证】

**站点A** dc:1732-1757（`FUN_14000481c @14000481c size=166`，`void(LARGE_INTEGER,int)`）：
dc:1742-1755 关键逻辑与现状一一对应；dc:1752 `wsprintfW(local_res18[0],L"PECMD**pecmd-cmd* WAIT *%lu -del \"%s\"");` 同样吞参；
dc:1747-1749 仅显示 `if (param_2 == 0) { GetCurrentProcessId(); }`。

Ghidra 静态反汇编（权威，补足 dc 所缺）：

```asm
14000482d: MOV EBX,EDX                 ; EBX = param_2
140004855: CMP EBX,ESI                 ; ESI=0
140004857: JNZ 0x140004863
140004859: CALL qword ptr [0x14011b9b0]; GetCurrentProcessId()
14000485f: MOV EBX,EAX                 ; EBX = PID
140004861: JMP 0x140004869
140004863: CMP EBX,-0x1                ; ← dc 未显示的钳位：param_2 == -1 时
140004866: CMOVZ EBX,ESI               ;   EBX = 0
140004869: ...
140004886: MOV RCX,qword ptr [RSP+0x40]; buf
14000488b: LEA RDX,[0x14011c930]       ; fmt（内存字节级确认 = L"PECMD**pecmd-cmd* WAIT *%lu -del \"%s\""）
140004892: MOV R9,RDI                  ; 变参2(%s) = param_1（路径指针）
140004895: MOV R8D,EBX                 ; 变参1(%lu) = EBX
140004898: CALL qword ptr [0x14011c138]; wsprintfW
```

即忠实语义：`uArg = (param_2==0) ? GetCurrentProcessId() : ((param_2==-1)? 0 : (uint32_t)param_2)`，
随后 `wsprintfW(buf, fmt, uArg, pathPtr)`。佐证：dc:11569 调用方传 `local_af8.dwProcessId`
（真 PID）、dc:97671 传 0 —— `%lu` 实参随调用方变化，绝非可省略。

**站点B** dc:4990-5099（`FUN_1400091e0 @1400091e0 size=833`）；dc:5065 同样吞参：

```asm
1400092a0: LEA RAX,[RCX+RCX*1+0x2]     ; RAX = len*2+2（len=lstrlenW(param_1)，dc:5034）
1400092b0: MOV qword ptr [RSP+0x50],RAX; 存入栈槽（= 写入映射首 QWORD 的载荷长度）
1400092b5: ADD EAX,0x50                ; 映射总长 = len*2+2+0x50（对应 dc:5047）
...
140009368: MOV R15,qword ptr [RSP+0x50]; R15 = len*2+2
14000937d: MOV RCX,R13
140009380: CALL qword ptr [0x14011b920]; UnmapViewOfFile(lpBaseAddress)   (dc:5064)
140009389: MOV R8,qword ptr [RBX]      ; RBX = (param_4 ? param_4 : &local_res20)（140009289-297）
                                       ; → [RBX] = hFileMappingObject（1400092f5 写入）
14000938c: LEA RDX,[0x14011d2e8]       ; fmt（内存字节级确认 = L" *map:0x%p:%lu "）
140009393: LEA RCX,[RSP+0x100]         ; buf = local_168
14000939b: MOV R9D,R15D                ; 变参2(%lu) = len*2+2 低32位
14000939e: CALL qword ptr [0x14011c138]; wsprintfW
```

即 `%p` = **继承句柄 hFileMappingObject**（配合 `bInheritHandle=1` + CreateProcessW 继承，
子进程凭该句柄值打开共享段）、`%lu` = **载荷字节数 len*2+2**。

#### 【补丁】

**Hunk 1-A** `refactored/src/misc/core_b1_remaining.c`（声明区加局部变量；plate 注释保证唯一）

old_string:
```c
    /* @0x14000481c size=166 自删除/等待命令构造 */
    int iVar1;
    uint64_t uVar2;
    WCHAR *local_res18[2];
```

new_string:
```c
    /* @0x14000481c size=166 自删除/等待命令构造 */
    int iVar1;
    uint64_t uVar2;
    uint32_t uVar3;   /* WAIT 变参: asm@140004855-4866 实证 (s14 批次1) */
    WCHAR *local_res18[2];
```

**Hunk 1-B** `refactored/src/misc/core_b1_remaining.c`（还原条件取值 + 补两个变参）

old_string:
```c
        if (param_2 == 0) {
            GetCurrentProcessId();
        }
        iVar1 = lstrlenW((LPCWSTR)param_1.QuadPart);
        PECMD_AllocWStringBuffer(local_res18, (int64_t)iVar1 + 200);
        /* TODO(verify): Ghidra 未显示 wsprintfW 的变参 */
        wsprintfW(local_res18[0], WSTR("PECMD**pecmd-cmd* WAIT *%lu -del \"%s\""));
```

new_string:
```c
        /* asm@140004855-4866: param_2==0 → GetCurrentProcessId(); param_2==-1 → 0;
         * 其余取 param_2 原值 (dc:1747-1749 未显示 -1 钳位分支, 汇编补全)。 */
        if (param_2 == 0) {
            uVar3 = GetCurrentProcessId();
        }
        else {
            uVar3 = (param_2 == -1) ? 0u : (uint32_t)param_2;
        }
        iVar1 = lstrlenW((LPCWSTR)param_1.QuadPart);
        PECMD_AllocWStringBuffer(local_res18, (int64_t)iVar1 + 200);
        /* asm@140004886-4898 实证变参: R8D=uVar3(%lu), R9=param_1 路径指针(%s);
         * fmt 字节级确认 @0x14011c930。Ghidra 吞可变参, 已还原 (先例 rb:10545/rb:7161)。 */
        wsprintfW(local_res18[0], WSTR("PECMD**pecmd-cmd* WAIT *%lu -del \"%s\""),
                  uVar3, (LPCWSTR)param_1.QuadPart);
```

**Hunk 1-C** `refactored/src/misc/core_b1_remaining.c`（站点B 补两个变参）

old_string:
```c
        UnmapViewOfFile(lpBaseAddress);
        wsprintfW(local_168, WSTR(" *map:0x%p:%lu "));
```

new_string:
```c
        UnmapViewOfFile(lpBaseAddress);
        /* asm@140009389-939e 实证变参: %p=hFileMappingObject(可继承句柄, 取自
         * [param_4 ? param_4 : &local_res20]), %lu=R15D=载荷长度(lstrlenW*2+2);
         * fmt 字节级确认 @0x14011d2e8。Ghidra 吞可变参, 已还原。 */
        wsprintfW(local_168, WSTR(" *map:0x%p:%lu "),
                  (void *)hFileMappingObject, (uint32_t)(iVar2 * 2 + 2));
```

#### 【风险与验证建议】

- **偏离"保持原样"旧注**：`stubs_common.h:2119` 有"缺 vararg 保持原样"的旧策略注记；本批以其存在
  **汇编一级证据**为由主动还原（与 rb:10545/rb:7161/core_var.c:100 先例同一操作模式），应用时建议主代理
  在该注记旁追加交叉引用，避免后人回退。
- 站点A 的 `%lu` 在 dc:11569 调用方语境下承载的是 **dwProcessId**（而非秒数），`WAIT *<pid>` 的上层语义
  由原版自身决定，直移不做"纠正"；若运行验证发现等待行为异常，先核对 dc:11013/dc:11569/dc:97671 三处调用方。
- 站点B 中 `iVar2` 在 3937 之后会被 CreateProcessW 返回值复用（3954），补参表达式 `iVar2 * 2 + 2` 位于复用前，
  与 dc/asm 时序一致，无副作用。
- 验证：① 编译期——两处补参后 `wsprintfW` 走 stubs_common.h:1209 变参原型，应无新告警；
  ② 行为期——触发自删除/EXEC 映射路径，抓生成的命令串应为
  `PECMD**pecmd-cmd* WAIT *<num> -del "<path>"` / ` *map:0x<hex>:<len> `（非空占位）。

---

### 批次2 rb:252 注入桩删除 + PatchRemoteWinExec 绑定真体

#### 【现状证据】

定位：glob `**/restored_bodies.c` → `refactored/restored_bodies.c`。"注入桩"指 rb:247-252 这一小簇
EXEC 远程注入路径的占位体（上方 247-251 为 `PECMD_CreateInjectedProcess` 新增桩 @0x1400e7414，
保守失败 return 0；下方 252 即本批对象）：

```c
252: static void PECMD_PatchRemoteWinExec(void *h) { (void)h; }   /* DAT_14013a330/340/338 (core_globals.c) */
```

- **为何叫注入桩**：该函数原版职责就是"向目标进程远程写入一段补丁码，重定向 WinExec/ExitWindowsEx"
  （WriteProcessMemory 远程写码注入）；当前是 `static` 空壳，静默吞掉整个注入步骤，故名。
- **谁调用它**：仅一处——rb:3738（`PECMD_ExecCmdDispatch` 的 EXEC 补丁路径 ↔ dc:11205
  `FUN_1400229f8(pvVar32)`）：pvVar32 取自 `local_af8.hProcess` 或 `OpenProcess(0x1fffff,...)`
  句柄（rb:3730/3733），调用后按需 CloseHandle（rb:3739-3741）。
- **真体已存在**：`refactored/src/commands/core_b2c.c:184-204`，非静态
  `void PECMD_PatchRemoteWinExec(HANDLE hProcess)`，与 dc 直移一致（memcpy 0x3d 字节自 0x14001a7fc、
  WriteProcessMemory 至 `ExitWindowsEx_exref`）。`docs/divergences.md` D-02 已登记此"桩未随真体落地回收"分歧。
- `include/stubs_common.h` **无** 该名声明（grep 全树仅 FUNC_NAMES.md/provenance/rename_map/divergences/
  restored_bodies/core_b2c 命中），故删桩后需就地补 extern 声明，否则 rb:3738 变隐式声明。

#### 【dc 原文锚点】

dc:20762-20780 `FUN_1400229f8 @1400229f8 size=118`，signature `void __fastcall (HANDLE)`：

```c
20773:   nSize = 0x3d;
20774:   local_res10[0] = ((int)WinExec_exref - (int)ExitWindowsEx_exref) + -0x38;
20775:   FUN_14001d78c(local_108,FUN_14001a7fc,0x3d);                       // memcpy 补丁码体 61 字节
20776:   FUN_14001d78c(local_d4,(undefined1 *)local_res10,(int)nSize + -0x39); // memcpy 4 字节偏移修正量
20777:   WriteProcessMemory(param_1,(LPVOID)(int)ExitWindowsEx_exref, local_108,nSize,(SIZE_T *)0x0);
```

注意：dc 原体**只有 WriteProcessMemory，没有 VirtualProtectEx**（任务描述提及 VirtualProtectEx，
以 dc 为准登记差异）。core_b2c.c 真体（:189-204）与上述逐行对应，含同样的 `local_d4` 死存储（忠实保留）。

**方向判定**：(a) 删桩 + extern 绑定真体。理由：真体已存在于 src 树且非静态、签名与 dc 一致；
若选 (b) 将 dc 体直移进 restored_bodies.c，会与 core_b2c.c 的定义构成跨 TU 重复符号（除非再把它改
static 并令 src 真体变死码），劣于绑定。与项目既有 S8"同址别名转发"、D-02 登记的处理方向一致。

#### 【补丁】

**Hunk 2-A** `refactored/restored_bodies.c`

old_string:
```c
static void PECMD_PatchRemoteWinExec(void *h) { (void)h; }   /* DAT_14013a330/340/338 (core_globals.c) */
```

new_string:
```c
/* S14 批次2: 删除原静态空注入桩(no-op), 改经 extern 声明绑定真体 ——
 * @0x1400229f8 (dc:20765) 真体 src/commands/core_b2c.c:189 (WriteProcessMemory 远程 WinExec 补丁);
 * 调用点 rb:3738 (EXEC 补丁路径 ↔ dc:11205)。见 docs/divergences.md D-02。 */
extern void PECMD_PatchRemoteWinExec(HANDLE hProcess);
```

（old 串全文件唯一；`HANDLE` 经本文件 `#include "stubs_common.h"` 可用；rb:3738 实参 pvVar32 为
HANDLE 兼容类型，无需改动调用点。）

#### 【风险与验证建议】

- **行为实质化风险（最高）**：删桩后 EXEC 路径将对目标进程真实 WriteProcessMemory 0x3d 字节。
  但真体的源数据仍锚在重构版的占位符号上：`win32_api_stubs.c:9/16` 把 `ExitWindowsEx_exref/WinExec_exref`
  定义为"返回 0 的占位函数"（原版中它们是导入表槽位的地址标记），且补丁码体从绝对地址
  `0x14001a7fc` 直接 memcpy——重构版映像该地址内容并无保证。core_b2c.c:186-187 的 TODO(verify)
  已要求主代理统一 exref 数据定义；**建议在主代理完成 exref→真实槽位/码体迁移之前，将本 hunk 视为
  "接线正确但注入内容待校准"**，或先行加日志验证 hProcess/目标地址后再放开。
- 跨 TU 类型小疵（既有问题，不在本批扩大）：stubs_common.h:2800/3078 把两个 exref 声明为
  `uint64_t f(void)` 函数，而 core_b2c.c 函数内以 `extern unsigned char f[]` 使用；MSVC 不做跨 TU 校验，
  链接可过，留待主代理统一。
- 验证：① 编译/链接——确认 restored_bodies.obj 不再导出/内联同名静态符号，链接解析到 core_b2c.obj；
  ② 行为——EXEC 注入路径下断点/日志观察 WriteProcessMemory(hProcess, =ExitWindowsEx_exref 地址,
  0x3d)；③ 回归——不触 EXEC 注入的脚本路径不受影响。

---

### 批次3 ScriptInit/ScriptCopy 空壳桩转发 core_exec5 真体

#### 【现状证据】

定位：glob `**/unimplemented_stubs.c` → `refactored/unimplemented_stubs.c`（注意：在 `refactored/`
根目录，不在 src 下）。两个**零参占位桩**：

```c
 60: uint64_t PECMD_ScriptInit(void) { return 0; }
191: uint64_t PECMD_ScriptCopy(void) { return 0; }
```

配套旧声明（**必须一并归正**，否则 unimplemented_stubs.c 单 TU 内 decl/def 类型冲突）：

```c
include/stubs_common.h:2815: uint64_t PECMD_ScriptInit(void);
include/stubs_common.h:3113: uint64_t PECMD_ScriptCopy(void);
```

而全部真实调用点均为 **2 参成对调用** 且自带局部 extern（这些 TU 只含 win32_stub.h/pecmd_defs.h，
不含 stubs_common.h，改头无冲突面）：
- `src/commands/core_b3r_h4.c:136-137`（`extern void PECMD_ScriptCopy(uint64_t*,uint64_t*); extern void PECMD_ScriptInit(uint64_t*,int64_t);`），调用 ：863-864
- `src/commands/core_b3r_i28c.c:53-54`，调用 ：1191-1193
- `src/commands/core_b3_remaining.c:333-334`（ScriptCopy 局部声明为返回 `uint64_t*`），调用 ：1717-1718、20272-20273、20688-20689、20755-20756、20859-20860、21012-21013、24655-24656、24840-24841 等

真体（任务所指 core_exec5.c，已确认）：
- `refactored/src/runtime/core_exec5.c:28-103`：`void *FUN_140017CDC(void *dst, void *src)`，尾部 `return dst;`（:102）
- `refactored/src/runtime/core_exec5.c:105-…`：`void FUN_1400186BC(void *s, int64_t parent)`

REVIEW.md §99-100 亦认列二者为 core_exec5.c 的还原真体。

#### 【dc 原文锚点】

- **ScriptCopy = FUN_140017cdc**：dc:12764-12842，size=597，signature
  `undefined8 * __fastcall FUN_140017cdc(undefined8 *param_1, undefined8 *param_2)`；
  0xe0 结构逐字段深拷贝（+0x10 位合并清 bit0..bit6、+0x70 串复制走 FUN_140070310、
  +0xe0/+0xe8 收尾），尾部 dc:12841 `return param_1;`。
- **ScriptInit = FUN_1400186bc**：dc:13335-13377，size=161，signature
  `void __fastcall FUN_1400186bc(undefined8 *param_1, longlong param_2)`；
  parent!=0 时 `*(int*)(this+0x20) = *(int*)(parent+0x20)+1`（引用计数），清零字段集
  （+0x58/+0x40/+0x60/+0x68/+0xf/+0xe/+8/+0/+0xe0/+0xe8/+0xc8/+0xca/+0xd0/+0xd8/+0x12/+0xd9/+0x11 bit1），
  尾部继承 parent+0x50 与 parent+0x11 的 bit1。

**签名逐一对照（dc ↔ core_exec5 真体 ↔ 新转发桩）**

| 名称 | dc 签名 | 真体签名(core_exec5.c) | 新桩签名 | 对照结论 |
|---|---|---|---|---|
| PECMD_ScriptCopy ≡ FUN_140017cdc | `ptr (ptr, ptr)`，返回 dst | `void *(void *dst, void *src)` ret dst | `void *(void *, void *)` | ✅ 参数个数 2/2、返回 ptr 一致 |
| PECMD_ScriptInit ≡ FUN_1400186bc | `void (ptr, longlong)` | `void (void *s, int64_t parent)` | `void (void *, int64_t)` | ✅ 参数个数 2/2、返回 void 一致 |

现有零参桩（arity 0、返回 uint64_t）与调用点(2参)/dc 双重不符，属纯占位。

#### 【补丁】

**Hunk 3-A** `refactored/unimplemented_stubs.c`（:60 ScriptInit 转发）

old_string:
```c
uint64_t PECMD_ScriptInit(void) { return 0; }
```

new_string:
```c
/* S14 批次3: 同址别名归一 —— rename_map.json: FUN_1400186bc ≡ PECMD_ScriptInit
 * (dc:13335 size=161, 签名 void(undefined8*,longlong); 真体 src/runtime/core_exec5.c FUN_1400186BC)。
 * 原零参空桩 arity 与全部调用点(2参)不符且恒返 0 → 改为转发真体。 */
extern void FUN_1400186BC(void *s, int64_t parent);
void PECMD_ScriptInit(void *s, int64_t parent)
{
    FUN_1400186BC(s, parent);
}
```

**Hunk 3-B** `refactored/unimplemented_stubs.c`（:191 ScriptCopy 转发）

old_string:
```c
uint64_t PECMD_ScriptCopy(void) { return 0; }
```

new_string:
```c
/* S14 批次3: 同址别名归一 —— rename_map.json: FUN_140017cdc ≡ PECMD_ScriptCopy
 * (dc:12764 size=597, 签名 ptr(undefined8*,undefined8*) 返回 dst; 真体
 * src/runtime/core_exec5.c FUN_140017CDC)。原零参空桩 arity 不符且恒返 0 → 转发真体。 */
extern void *FUN_140017CDC(void *dst, void *src);
void *PECMD_ScriptCopy(void *dst, void *src)
{
    return FUN_140017CDC(dst, src);
}
```

**Hunk 3-C** `refactored/include/stubs_common.h`（:2815 声明归正）

old_string:
```c
uint64_t PECMD_ScriptInit(void);
```

new_string:
```c
void PECMD_ScriptInit(void *s, int64_t parent);   /* S14 批次3 归正(dc:13336 FUN_1400186bc), 真体 core_exec5.c */
```

**Hunk 3-D** `refactored/include/stubs_common.h`（:3113 声明归正）

old_string:
```c
uint64_t PECMD_ScriptCopy(void);
```

new_string:
```c
void *PECMD_ScriptCopy(void *dst, void *src);     /* S14 批次3 归正(dc:12765 FUN_140017cdc), 真体 core_exec5.c */
```

（四个 old 串均已 grep 确认各自文件内唯一。）

#### 【风险与验证建议】

- **返回类型取舍**：ScriptCopy 新签名为 `void *`（依 dc `undefined8*`）。现有调用方均以语句形式调用、
  不消费返回值（逐点核对上列行号），x64 ABI 下兼容；`core_b3_remaining.c:333` 局部 extern 写作
  `uint64_t *(uint64_t*,uint64_t*)`，与新定义跨 TU 形式不一致（MSVC 不校验、ABI 相容），建议主代理
  后续顺手统一为同型声明（可选清理，不阻塞本批）。
- **转发后语义激活**：ScriptCopy 会真正深拷贝 0xe0 结构、ScriptInit 会读写 parent+0x20/+0x50/+0x11——
  要求调用点传入有效模板指针。dc 全部 41 处调用均为"CDC 克隆 → Init"成对序（如 dc:29280-29281、
  dc:45800-45801），工程调用点同构；若存在单独调 Init 且 parent 悬空的隐藏点，将在首次运行暴露，
  建议门禁跑一遍 SUB/LOGS/进程链类脚本回归。
- ScriptInit 真体内 parent==0 分支的自锚默认值修正（core_exec5.c:137-140 S11 注记）依赖该真体本身，
  转发不改变其行为，反而让 `g_Script` 场景（app/core_init.c:92 走 FUN_1400186BC 直呼）与桩路径收敛到同一实现。
- 验证：① 编译全树无 C2371/C2084（重点看 unimplemented_stubs.c 与 stubs_common.h 一致性）；
  ② 链接期符号解析到 core_exec5.obj（不再有零参桩兜底）；③ 行为期对比克隆脚本的 argv/变量区
  （+0x70 串、+0x20 计数、+0x50 父锚）非零初始化。

---

## 附：证据索引

| 主题 | 文件:行 |
|---|---|
| 站点A 现状 | refactored/src/misc/core_b1_remaining.c:1273-1295 |
| 站点A dc | decompiled.c:1732-1757（吞参行 dc:1752） |
| 站点B 现状 | refactored/src/misc/core_b1_remaining.c:3871-3971（吞参行 :3937） |
| 站点B dc | decompiled.c:4990-5099（吞参行 dc:5065） |
| fmt 字节实证 | Ghidra 内存 @0x14011c930 / @0x14011d2e8（本稿批次1 引用的两条宽字符串） |
| 注入桩现状/调用点 | refactored/restored_bodies.c:247-252 / :3738 |
| PatchRemoteWinExec dc/真体 | decompiled.c:20762-20780 ／ refactored/src/commands/core_b2c.c:184-204 |
| D-02 登记 | refactored/docs/divergences.md:44,61-63 |
| ScriptInit/ScriptCopy 桩 | refactored/unimplemented_stubs.c:60,:191；include/stubs_common.h:2815,:3113 |
| 真体 dc | decompiled.c:12764-12842（ScriptCopy）；decompiled.c:13335-13377（ScriptInit） |
| 真体现状 | refactored/src/runtime/core_exec5.c:28-103 / :105 起 |
