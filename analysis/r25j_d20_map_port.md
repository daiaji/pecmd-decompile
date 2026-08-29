# R25-j · D-20 `*map:` 执行体补全 — RUN 命令映射脚本执行路径移植报告

- 日期: 2026-08-29
- 交付: `src/lang/core_scriptrun.c`（本报告外的唯一代码文件）
- 语义真值: `reference/decompiled.c`（dc）+ `reference/PECMD原始.EXE` 反汇编定案
- 任务书: `tools/task_r25j_d20_map.md`

---

## 1. 反汇编工具与定案方法

任务书指定 objdump，本机 Git 安装无 objdump（`Git\usr\bin` 无此件，全盘 maxdepth-4 搜索无果）。
按任务书"工具不可用则换替代路径"纪律改用：

- **capstone 5.0.7**（python，环境自带）+ 手工 PE 解析（DOS→COFF→Optional→Section 表），
  内联脚本不落盘；IAT 表完整解析（574 项）标注 KERNEL32/SHLWAPI 导入调用点。
- ImageBase = 0x140000000（.text va=0x1000, raw=0x400），与 dc 标尺一致。

### 1.1 地址标尺校准（dc 标签 ↔ ASM 实测）

| dc 标签/行 | dc 行号 | ASM 地址 | 校准依据 |
|---|---|---|---|
| LAB_140031887 主展开路径 | 29635 | 0x140031887 | 标签即地址 |
| *map: 解析起点 (ParseUIntValue) | 29848 | 0x140031c73 | call 指纹 |
| MAP 读块第二映射 | 29857 | 0x140031cf3 | call MapViewOfFile |
| LAB_140031d26 分类入口 | 29871 | 0x140031d26 | 标签即地址（AllocEmpty@0x140031d2b = dc:29873 吻合） |
| StrRChrW '#' 分类 | 29963/29967 | 0x140031f5d/0x140031f77 | call 指纹 |
| mem_flag 门控 | 30017 | **0x140032081** | cmp byte[rsp+0x2f8] |
| LAB_140032341 mem 路径尾 | 30270 | 0x140032341 | call FreeStrBuf（标签=地址吻合） |
| map size 门控 | 30018 | **0x14003234b** | cmp rsi,r9/jle |
| cond_C 反斜杠门控 | 30019 | **0x14003253d** | cmp byte[rsp+0x40] |
| **MAP 执行块** | 30235-30272 | **0x14003235c-0x140032538** | AllocSmallObject@0x140032364 起 |
| mem 变量路径 | 30273-30332 | 0x140032097-0x140032334 | UnquoteToken@0x140032097 起 |
| 盘符探测 | 30020-30059 | 0x14003253d-0x1400326cc | lstrlenW/GetLogicalDriveStringsW |
| 失败尾 LAB_140032a2e/35 | 29863-29869 | 0x140032a2e/0x140032a35 | 标签即地址 |
| 成功尾 LAB_140032d20 | 30059/30066 | 0x140032d20 | 标签即地址 |

关键结构性定案：**dc 文本序 ≠ 地址序**。map 读块（dc:29844-29862）物理位于 0x140031c44-0x140031d26，
在分类代码（LAB_140031d26）**之前**；mem 路径（dc:30273+）物理在 0x140032097，在门控条件之后、
map 执行块之前。dc:30017-30019 三层 if 的物理布局为"全前向跳转"（见 §2.1）。

---

## 2. 关键定案（逐条 ASM 证据）

### 2.1 门控条件（任务书第 1 步核心问题）

dc:30016-30019 三层比较被 Ghidra 类型复用污染（`local_res20`/`ptVar40` 在此区间被复用为
指针/整数/布尔多角色）。反汇编定案：

| dc 字面 | ASM 实测 | 真实语义 |
|---|---|---|
| `if ((char)local_res20 == (char)ptVar40)` (30017) | `0x140032081 cmp byte ptr [rsp+0x2f8], r9b(0); je 0x14003234b` | **mem_flag == 0** → 大分派块；≠0 → 0x140032097 mem 变量路径。`[rsp+0x2f8]` = mem_flag（SrParsePrefix p_mem_flag = local_res20 低字节，core_srparse.c 头注在册） |
| `if ((longlong)local_178 <= (longlong)ptVar40)` (30018) | `0x14003234b mov rsi,[rsp+0x160]; cmp rsi,r9(0); jle 0x14003253d` | **map size（有符号 qword，[rsp+0x160]）> 0** → map 执行块；≤0 → 盘符/裸路径/资源分派。非 map 命令该槽保持 0，恒走分派块 |
| `if (local_298 != (char)ptVar40)` (30019) | `0x14003253d cmp byte ptr [rsp+0x40], r9b; je 0x1400326e6` | local_298 = 词首反斜杠标志 q1（既有 ASM 1400319c1 定案），[rsp+0x40] |

结论：map 形到达执行块的实条件 = `mem_flag==0 && size>0`；读取块已保证 size≥1
（0x140031cd9 `jle 0x140032a35`），故 map 形必然执行执行块。dc:30018 字面
`size <= ptVar40` 中的 `ptVar40` 即 r9=0（与 30017/30019 同一零寄存器比较），非指针比较。

**门控次序**：mem 判定（0x140032081）物理在 map size 判定（0x14003234b）之前——
`mem *map:N` 形态在原版走 **mem 变量路径**而非 map 路径。旧 msvc 代码 map 分支在前、
无 mem 守卫，属次序偏差，本次已加 `mem_flag == 0` 守卫修正（§4）。

### 2.2 映射读取块单位（关联修正）

| dc | ASM | 定案 |
|---|---|---|
| 29846 解析起点 | 0x140031c60 `lea rax,[rdi+0xa]` | mapStr+**5 字符**（旧码正确） |
| 29853 size 读取 | 0x140031cb5 `mov rax,qword [rax]` | **8 字节**有符号读（旧码正确） |
| 29856 size 校验 | 0x140031cd9 `cmp rax,rbx; jle` | **有符号** >0（旧码正确） |
| 29857 重映射大小 | 0x140031cdf `add rax,8` | **size + 8 字节**。dc 字面 `(SIZE_T)(local_230+2)` 为 int 型指针步进（+2×4）。**旧码 `(sz+2)` 系单位错，已修正为 `(size_t)mapSize + 8`** |
| 29859 payload | 0x140031d1a `add rax,8` | payload = 映射基址 + **8 字节**（&local_1a0->ismbcodepage 即 +8） |

### 2.3 执行块单位（任务书第 1 步指定定案点）

| dc | ASM | 定案 |
|---|---|---|
| 30238 `FUN_140063424(&local_228,(longlong)(piVar1+9))` | 0x14003236b `lea rdx,[rsi+0x24]` | **size + 0x24 = size+36 字节**（piVar1 为 int 型指针，+9 步进=+36）。非"size+9" |
| 30241 `FUN_14001d78c(local_228,local_230,(int)piVar1)` | 0x14003239c `mov r8,rsi` | memcpy 第三参 = **size 字节**（payload→buf） |
| 30242 `FUN_140102a90(local_228+piVar1,0,0x14)` | 0x1400323c2 `add rcx,rsi; lea r8d,[rdx+0x14]` | memset 目标 = buf + **size 字节**，长度 **0x14=20 字节**，值 0 |
| 30244 `FUN_1400e7d58(&local_228,1)` | 0x1400323d8-0x1400323e3 `lea rcx,[rsp+0xb0]; mov edx,r12d(1)` | ResDecode 传**槽地址**（[rsp+0xb0]），flags=1；字节长由邻接槽 [rsp+0xb8]（=size，dc:30240 local_220）提供——与 mem 路径 resSlot 同构，故 msvc 用 `int64_t mapSlot[3]` 复刻三槽邻接 |
| 30247 XorEncode(buf,WVar33,0) | 0x1400323ed `xor r8d,r8d` | len=0（XorEncode 自身语义处理，不臆造） |
| 30253-30260 分隔符写 | 0x140032441/0x14003245e/0x14003246d-0x1400324a1 `mov word ptr [基+r8*2+{0,2,4,6,8}]` | **WCHAR（2 字节）单位**，索引 iVar11（pos）为字符索引；0xd/0xa 变形 + 5 连 WVar33 位于 pos+2..pos+6 |
| 30261 XorEncode(buf,seed^WVar33,iVar11+6) | 0x1400324ad `lea eax,[r8+4]`(r8=pos+2) `xor edx,ebx` | key = seed(16位)^WVar33(16位)，len = pos+6（**字符数**，与资源路径 `(字节长>>1)+2` 同单位制） |
| 30262-30264 DAT_14013a248 | 0x1400324bd-0x1400324e7 `cmp dl,bl; cmovl eax,r12d` | **有符号字节** <0 则置 1；msvc = g_charTableF（core_globals.c:709，pecmd_globals.h:214 在册） |
| 30265 PrependEnviHeader | 0x1400324c4-0x1400324ef **无判空分支** | **无条件调用**（资源路径同名调用带 `*local_258 != 0` 守卫，map 路径没有——dc 文本亦无守卫，非 Ghidra 折叠） |

### 2.4 读取期截断/还原（新发现，读路径补齐）

- dc:29839-29841 (ASM 0x140031c11-0x140031c33)：`local_148 = line + restLen`（restLen =
  local_1a8 = dc:29824 余串 token 长度，`lea rax,[rsi+rbp*2]` 字符步进）；保存 `local_238 =
  line[restLen]` 后置 0。**截断发生在 map/非 map 判定之前、对两条路径都生效**（0x140031c28）。
- dc:29845（map 分支内）`mbulinfo[local_1a8-6]=0` = 0x140031c5c `mov word[rax+rbp*2],bx`
  同一位置的冗余重写。
- dc:29921（LAB_140031e1d 入口，ASM 0x140031e1d-0x140031e35）还原 `*local_148 = local_238`。
- 可观察性：仅当 restLen 落在句柄数字区间（5 < restLen < 5+位数）时影响 ParseUIntValue 结果；
  常见 `*map:<handle>`（无余串，restLen=0）截断 line[0]，不影响 +5 起解析。失败路径原版同样
  不还原（line 随即释放）。
- msvc 落点：截断在 map 分支入口（dc:29839 位），还原在执行块入口（dc:29921 位）。
  非 map 路径的截断窗口（仅约束星号 StrChrW 搜索上界）未复刻，见 §6 偏差表。

### 2.5 local_240 / local_190 构建（RunScriptText pCurFile 定案）

LAB_140031e1d (dc:29920-29933, ASM 0x140031e1d-0x140031eaf)：

```
0x140031e3a  movsx  rdi, bl              ; cVar6 = local_188 (".#" 形 '#' 偏移)
0x140031e40  lea    r8, [rdi+rbp+1]      ; len = cVar6 + lVar32(restLen) + 1
0x140031e4d  cmovne rsi, [rsp+0x148]     ; cVar6!=0 时 词 = local_190 旧值(dc:29656 词起点)
0x140031e56  lea    rdx, [rsi-2]         ; src = 词 - 2 字节（map 形词=line+2 → src=line）
0x140031e5a  call   StrNDup702d4         ; dc:29926 FUN_1400702d4(&local_240, src, len)
0x140031e6f  mov    word [rax+rdx*2+2],0 ; dc:29928 NUL[len]
0x140031e94  mov    [rsp+0x148], rsi     ; dc:29932 local_190 = local_240 + cVar6 + 1
```

- FUN_1400702d4 = dc:60955 FUN_140063888：`FUN_140063720(槽, n+1)` 分配 + `MemMoveForward(*槽,
  src, n*2)` + `NUL[n]` —— **len 为字符数**（原版 ASM 0x1400702d4 尾调 0x140063888 实锤）。
- map 形（line[0]=='*' 恒成立，星跳后词=line+2）：src = **line**，len = dotHash+restLen+1，
  **local_190 内容 = line[dotHash+1 .. dotHash+restLen]**；常见 restLen=0 时 = 空串。
- local_190 是 RunScriptText 第 4 参 pCurFile（0x1400324fc `mov r9,[rsp+0x148]`）。
- local_1f8（dc:29933 StrDup(local_190)）仅资源路径 RunScriptText 消费，map 路径死值，不复刻。
- dc:29921 截断还原先于 dc:29926 拷贝（src 读到还原后的 line）——msvc 同序。

### 2.6 RunScriptText 调用定案（dc:30268-30269 = ASM 0x1400324f4-0x140032529）

```
rcx = r14  = param_1 (script)
rdx = [rsp+0xb0] = local_228（PrependEnviHeader 回写后的流指针）
r8  = r15 = [rsp+0x128] = local_1b0 = line   (pName)
r9  = [rsp+0x148] = local_190                (pCurFile)
[rsp+0x20] = edi = (seed<<16) | r13d(uVar31) | 0x40
[rsp+0x28] = 0 (pFile), [rsp+0x30] = 0 (pPersist)
```

随后 0x14003252e-0x140032538：`lea rcx,[rsp+0xb0]; jmp 0x140032341`（FreeStrBuf(&local_228)
——dc:30267 已置 NULL，空操作）→ `jmp 0x140032d20` 成功尾。

---

## 3. dc → msvc 映射表（执行块 dc:30235-30272）

| dc 行 | 原文 | msvc（core_scriptrun.c map 分支内） | 备注 |
|---|---|---|---|
| 30235 | `FUN_140063344(&local_228)` | `PECMD_AllocSmallObject((void**)&mapSlot[0])` | 真体 core_exec.c:120 |
| 30236 | `local_220 = 0` | `mapSlot[1] = 0` | |
| 30237 | `local_218 = 0` | `mapSlot[2] = 0` | |
| 30238 | `FUN_140063424(&local_228, piVar1+9)` | `PECMD_GrowByteBuffer((void**)&mapSlot[0], mapSize+0x24)` | +36 字节（§2.3） |
| 30239 | `local_218 = piVar1` | `mapSlot[2] = mapSize` | |
| 30240 | `local_220 = piVar1` | `mapSlot[1] = mapSize` | ResDecode 槽[1] |
| 30241 | `FUN_14001d78c(local_228,local_230,(int)piVar1)` | `PECMD_MemMoveForward((uint8_t*)mbuf, mapPayload, (int)mapSize)` | 字节单位 |
| 30242 | `FUN_140102a90(local_228+piVar1,0,0x14)` | `memset((uint8_t*)mbuf+mapSize, 0, 0x14)` | CRT memset |
| 30243 | `UnmapViewOfFile(local_1a0)` | `UnmapViewOfFile(local_1a0)` | |
| 30244 | `FUN_1400e7d58(&local_228,1)` | `FUN_1400E7D58(mapSlot, 1)` | 槽[1]=字节长 |
| 30245 | `uVar9 = FUN_14001b510()` | `seed = PECMD_GenRandomSeed16()` | |
| 30246 | `WVar33 = uVar9<<8 \| uVar9&0xff` | `w33 = (uint16_t)((seed<<8)\|(seed&0xff))` | 16 位算术 |
| 30247 | `FUN_14001b5ac(local_228,WVar33,0)` | `FUN_14001B5AC(mbuf, w33, 0)` | |
| 30248 | `FUN_140030dcc(&local_228,param_1,WVar33)` | `PECMD_InvokeSubRoutine(mapSlot, script, w33)` | |
| 30251 | `FUN_14001b4f8(local_228,WVar33)` | `pos = PECMD_StrChrOffset(mbuf, w33)` | 字符索引 |
| 30253 | `pWVar22[iVar11] = WVar23^0xd` | `mbuf[pos] = w33^0xd` | WCHAR 单位 |
| 30254 | `local_228[iVar11+1] = WVar33^10` | `mbuf[pos+1] = w33^0xa` | |
| 30260-30256 | `local_228[lVar41..lVar41+4] = WVar33` | `mbuf[pos+2..pos+6] = w33` | 5 连填充 |
| 30261 | `FUN_14001b5ac(local_228, uVar9^WVar33, iVar11+6)` | `FUN_14001B5AC(mbuf, (uint16_t)(seed^w33), pos+6)` | |
| 30262-30264 | `DAT_14013a248<0 → 1` | `if ((int8_t)g_charTableF<0) g_charTableF=1;` | 有符号字节 |
| 30265 | `FUN_140024f20(uVar9,&local_228,local_258,local_294,0)` | `PECMD_PrependEnviHeader(seed, mapSlot, outbuf, flags2, 0)` | 无条件 |
| 30266 | `pWVar22 = local_228` | `pText = mapSlot[0]` | |
| 30267 | `local_228 = 0` | `mapSlot[0] = 0` | 所有权移交（T1e） |
| 30268-30269 | `FUN_1400b638c(param_1,pWVar22,ptVar15,local_190,flags,0,0)` | `PECMD_RunScriptText(script,pText,line,local_190,(seed<<16)\|flags\|0x40,NULL,NULL)` | |
| 30270-30271 | `ppWVar27 = &local_228` → LAB_140032341 尾释放 | `PECMD_FreeStrBuf((WCHAR**)mapSlot)`（NULL 空操作） | |
| （前置）29921 | `*local_148 = local_238` | `line[restLen] = mapSavedCh` | 截断还原 |
| （前置）29926-29932 | local_240/local_190 构建 | Alloc+memcpy+NUL / `local_190 = local_240+dotHash+1` | §2.5 |

`int64_t mapSlot[3]` 复刻原版栈槽 [rsp+0xb0]/[rsp+0xb8]/[rsp+0xc0]（local_228/local_220/
local_218）的**真实邻接**——ResDecode 经槽[1]读字节长（mem 路径同款契约），独立局部变量无法
保证邻接，故用数组。所有可能重分配的调用（GrowByteBuffer/ResDecode/InvokeSubRoutine/
PrependEnviHeader）之后均从槽[0]重读 mbuf，与原版寄存器重装一致。

## 3.1 参数对应表（任务书指定核对项）

| dc 名 | 物理位置 | 真实语义 | msvc 对应 |
|---|---|---|---|
| uVar31 / r13d | 寄存器 | 前缀 flag 累加（pwVar43/local_290 的寄存器形） | `flags`（SrParsePrefix p_flags；与 mem/资源路径既有映射一致） |
| local_294 / r9d 来源 [rsp+0x44] | 栈 | 第二 flag 累加（pwVar37 快照） | `flags2` |
| local_258 / r8 来源 [rsp+0x80] | 栈 | logs: 输出缓冲（dc:29749 FUN_14007a224 产出；SrParsePrefix p_outbuf = local_258，core_srparse.c:17 在册） | `outbuf` |
| local_190 / r9 [rsp+0x148] | 栈 | local_240+dotHash+1（词副本内部指针） | `local_190`（新增，指向 `local_240` 内部） |
| ptVar15 / r15 [rsp+0x128] | 栈 | local_1b0 = local_278 = 展开后完整命令行（dc:29827/29860） | `line` |
| local_178 / rsi [rsp+0x160] | 栈 | 映射 size（字节，有符号 qword） | `mapSize` |
| local_230 / rdx 来源 [rsp+0xa8] | 栈 | payload = 映射基址+8 | `mapPayload` |
| local_188 / bl [rsp+0x150] | 栈 | ".#" 形 '#' 词内偏移（字符） | `dotHash` |
| local_238 / cx [rsp+0xa0] | 栈 | 截断保存字符 | `mapSavedCh` |
| local_1a8 / rbp [rsp+0x130] | 栈 | 余串 token 长度（dc:29824） | `restLen`（自内层上提） |
| local_res20 [rsp+0x2f8] | 栈 | mem 前缀标志（低字节） | `mem_flag` |

## 3.2 依赖符号在册核对（任务书清单逐项）

| 符号 | @地址 | 真体位置 | 本文件引用 |
|---|---|---|---|
| PECMD_AllocSmallObject | 0x140063344 | src/lang/core_exec.c:120 `void *PECMD_AllocSmallObject(void **ps)` | 新增本地 extern（签名同真体） |
| PECMD_GrowByteBuffer | 0x140063424 | src/runtime/core_thread.c:30（回写 *ps ✓） | 既有 extern（L51） |
| PECMD_MemMoveForward | 0x14001d78c | restored_bodies.c:11567 `uint8_t *PECMD_MemMoveForward(uint8_t*,uint8_t*,int)`（stubs_common.h:841 同签名；FUN_14001d78c 为单行转发 restored_bodies.c:4663） | 新增本地 extern（签名与 stubs_common.h:841 一致，重声明安全） |
| memset (FUN_140102a90) | 0x140102a90 | CRT（既有惯例：srx_ExecuteScriptFile 同用） | string.h（L23 既有） |
| UnmapViewOfFile | IAT | win32_stub.h | 既有（读路径已用） |
| FUN_1400E7D58 (ResDecode) | 0x1400e7d58 | src/lang/core_resdecode.c:49 | 既有 extern（L74） |
| PECMD_GenRandomSeed16 | 0x14001b510 | src/lang/core_var3.c:80 | 既有 extern（L56） |
| FUN_14001B5AC (XorEncode) | 0x14001b5ac | src/lang/core_var3.c:103 | 既有 extern（L57） |
| PECMD_StrChrOffset | 0x14001b4f8 | src/lang/core_var3.c:120 | 既有 extern（L60） |
| PECMD_InvokeSubRoutine | 0x140030dcc | src/lang/core_scriptdep.c:204 | 既有 extern（L73） |
| PECMD_PrependEnviHeader | 0x140024f20 | src/lang/core_scriptdep.c | 既有 extern（L75） |
| PECMD_RunScriptText | 0x1400b638c | src/lang/core_execmain.c | 既有 extern（L82） |
| g_charTableF (DAT_14013a248) | 0x14013a248 | src/kernel/core_globals.c:709；声明 pecmd_globals.h:214（经 pecmd_defs.h:163 可见） | 直接引用（无需新 extern） |
| PECMD_AllocWStringBuffer | 0x140063694 | core_var.c | 既有 extern（L97），替代 dc:60964 FUN_140063720 的槽分配（同族 n 字符槽分配） |
| PECMD_FreeStrBuf | 0x14005b104 | core_string.c | 既有 extern（L98） |
| PECMD_ParseUIntValue | 0x140074838 | 既有 | L81 |

## 4. 对既有代码的修正（全部 ASM 背书，均在授权单文件内）

1. **重映射单位错**（原 L414）：`MapViewOfFile(..., (SIZE_T)(sz+2))` → `(SIZE_T)mapSize + 8`。
   ASM 0x140031cdf `add rax,8`。旧值少映射 6 字节，payload 按基址+8 取 size 字节时越界读。
2. **mem 前缀优先守卫**（原 L404）：map 分支条件补 `&& mem_flag == 0`。ASM 0x140032081：
   mem 判定先于 size 判定，`mem *map:N` 原版走 mem 变量路径。
3. **失败尾接回**（原 L422-423）：读取失败原版返回 -0x7ff8ffa9（0xFFFFFFFF80070057 =
   HRESULT E_INVALIDARG 符号扩展）并先释放 local_168(恒0)/local_278/local_258（dc:29863-29869）；
   旧码失败落入 `srx_tail` 返回 0。新码 `srx_mapfail` 标签 + 直接返回，另补 local_250 释放
   （dc:29829 原版已在主流程释放，msvc 释放点在收尾，失败路径直接返回须补齐防漏）。
4. **TODO(verify) 占位移除**（原 L417）：执行块已按 dc:30235-30272 补全。

## 5. RESTORED / SKIP 清单

- RESTORED: dc:30235-30272 `*map:` 执行块全序列（含 dc:29921 截断还原、dc:29926-29932
  local_240/local_190 前置构建、dc:30243 Unmap、dc:30262-30264 g_charTableF 门闩、
  dc:30270-30271 尾释放）；dc:29856-29858 失败路径 -0x7ff8ffa9 返回语义；§4 四项修正。
- SKIP（原因）:
  - dc:29834 `local_1a0 = ptVar15`（map 分支先把 local_1a0 置为 line）：死存储——读取块随
    即用映射基址覆盖（dc:29857），无可观察效果。
  - dc:29842 `local_168 = 0`：失败尾释放用的恒 NULL 槽，msvc 无对应局部，空操作。
  - dc:29845 `mbulinfo[local_1a8-6]=0`：与 dc:29841 同一位置的冗余重写（同一 ASM 槽
    0x140031c28/0x140031c5c），合并为一次截断。
  - dc:29933 `FUN_1400702b0(&local_1f8, ptVar30)`：local_1f8 仅被资源路径 RunScriptText
    第 4 参消费，map 形下为死值（map 路径经 early-exit 不再回到资源分支），不构建。
  - dc:29873 `FUN_140063620(&local_260)` 空槽分配：map 路径上 local_260 从未写入、仅收尾
    释放，msvc 以 NULL 槽等价。
  - 非 map 命令的截断窗口（dc:29839 对两路径生效，非 map 时仅约束 dc:29878 星号 StrChrW
    的搜索上界）：未复刻。msvc 星号分类走 wcls 全行副本（既有 S10 结构），改写将波及
    既有分派语义，超出本任务单点授权，登记 §6。

## 6. TODO(verify) / 登记偏差（未处置，留待后续轮次）

| # | 事项 | 证据状态 |
|---|---|---|
| 1 | 非 map 命令截断窗口未复刻（星号搜索上界差，仅当"有 rest 且 rest 之后还有 '*'/'#'"可观察） | dc/ASM 已定案，移植未做（§5 SKIP） |
| 2 | 旧码读路径"简版"曾无截断：本次已补 map 窗口截断+还原；但若原版截断导致句柄解析截短（restLen 落在数字区间内），现与原版一致 | ASM 定案，行为对拍待跑 |
| 3 | PECMD_ParseUIntValue 写 8 字节 vs 原版 local_1c0[0] 为 4 字节槽（0x140031c78 movsxd dword）：句柄值域 <2^31 时等价；Windows 句柄仅低 32 位有效，实践等价 | ASM 定案，登记不修 |
| 4 | msvc PECMD_StrBldCopyWideN（core_var2.c:68→StrCopyW）按**字节**拷贝，而原版 0x1400702d4→dc:60955 按**字符**（param_3*2）——真体单位偏差，非本任务文件，未修。本移植因此绕开该真体、按 dc:60964-60966 用 Alloc+memcpy(字符数)+NUL 直译 | dc:60955 + ASM 0x1400702d4 实锤 |
| 5 | msvc 资源路径 RunScriptText 第 4 参传 local_1f8=NULL，原版为 local_1f8=StrDup(local_190)（dc:29933/30228）——既有缺口，非本任务范围 | dc 对照，登记 |
| 6 | ".#" 形（dotHash≠0）与 map 形叠加的极端组合（词形 `".#*map:...`）按公式直译（src=rawWord-2），未经对拍 | dc:29647 公式在册，标注待验证 |
| 7 | 无边界护栏：`line[restLen]` 截断/还原、`local_240` 拷贝 len240、`mbuf[pos..pos+6]` 写均按原版无条件执行（restLen 与 line 长度的关系由上游 token 化保证），刻意不加兜底（禁止行为注入红线） | 红线遵从 |

## 7. 自检记录

- 注释/括号配平：状态机扫描 — 块注释 207 开/207 闭、行注释闭合、final state=code；
  brace 90/90、paren 427/427、bracket 46/46 配平。行内 208 个 `/*` 字面中 1 个为既有
  S11 注释内嵌示例文本（L100，`* /` 故意拆分），非真注释。
- 新代码路径符号均有在册真体 + 签名一致 extern（§3.2 表）。
- 未构建、未 git、未用 windbg/Ghidra MCP（反汇编经 capstone+手工 PE 解析完成）。
- 未改动 `src/lang/core_scriptrun.c` 与本报告之外的任何文件。

## 8. 结论

D-20 关闭：`RUN *map:<handle>` 形态从"只读映射后弃置"补全为 dc 真值执行路径——
payload 解码（ResDecode）→ 随机键双层 XOR 编码（XorEncode×2 + InvokeSubRoutine 子例程展开 +
key 分隔符/5 连填充）→ PrependEnviHeader 环境头前插 → RunScriptText 编码流执行，
全部单位（字节/字符/WCHAR）与门控条件（mem_flag 优先、size>0）经原版 EXE 反汇编定案，
无臆造语义。遗留偏差 7 项已登记（§6），其中 #4（StrBldCopyWideN 真体单位偏差）建议
后续轮次单独修复。
