# R23: PATCH-1 / unaff_R13D 两笔 TODO(verify) Ghidra 定案档案

> R23 轮 · 主代理执行, 真源0=原版 /PECMD.exe 反汇编 (Ghidra MCP, project=PECMD)
> 构建身份: 定案基于 reference\PECMD.exe (0x140000000); 落码于 HEAD 193b7d2 工作树
> 结论性质: 两笔皆为"以反汇编实证推翻/确认既有假设", 含 skill 档案勘误

---

## 1. PATCH-1 (S-TEMP-1) — 0x140008110 PECMD_CreateTempMutexDir wsprintfW 实参

### 1.1 反汇编证据链 (0x140008110 size=687)

目录模式 (CreateDirectoryW 前, 0x1400081e0-0x14000821a):
```
CALL [0x14011ba88]  ; GetTickCount
MOV  EBX,EAX
CALL 0x14005e04c    ; FUN_14005e04c = QPC 线性同余种子推进 (PECMD_NextRandomSeed)
ADD  EBX,EAX        ; vararg3 = GetTickCount() + NextRandomSeed()  (u32 截断加)
CALL [0x14011b9b0]  ; GetCurrentProcessId
MOV  R8,[RSP+0xc8]  ; vararg1 = [入RSP+0x20] = 序言 R9 home = param_4
MOV  R9D,EAX        ; vararg2 = PID
LEA  RAX,[0x14011d108] ; ".tmp"
LEA  RDX,[0x14011d0d8] ; fmt
MOV  [RSP+0x28],RAX ; vararg4 = ".tmp"
MOV  RCX,RDI        ; buf
MOV  [RSP+0x20],EBX ; vararg3
CALL wsprintfW      ; = [0x14011c138]
```
文件模式 (LAB_1400082bf, 0x1400082c7-0x1400082f3):
```
CALL [0x14011b9b0]  ; GetCurrentProcessId
MOV  R8,[RSP+0xc8]  ; vararg1 = param_4
MOV  R9D,EAX        ; vararg2 = PID
MOV  RAX,[RSP+0xd0] ; vararg4 = param_5 (调用者栈参 [caller rsp+0x20])
LEA  RDX,[0x14011d0a8] ; fmt (无尾 %)
MOV  [RSP+0x28],RAX
MOV  ECX,RBX
MOV  [RSP+0x20],EDI ; vararg3 = EDI; EDI=ESI=0 (0x1400082ba MOV EDI,ESI, ESI 序言清零)
CALL wsprintfW
```

### 1.2 字面量实证 (read_memory)

| 地址 | 内容 | 用途 |
|---|---|---|
| 0x14011d0d8 | `~pecmd_%s.%lu.%lu~%s%` (**尾孤立 %** 真实存在) | 目录模式 fmt |
| 0x14011d0a8 | `~pecmd_%s.%lu.%lu~%s` (无尾 %) | 文件模式 fmt |
| 0x14011d108 | `.tmp` | 目录模式 vararg4 硬编码 |
| 0x14011d960 | `exedata` | e26c 调用点 param_4 (R9) |
| 0x140122e98 | `devi` | 0x140048d66 / 0x14004a9f1 调用点 param_4 |
| 0x140122ea8 | `.tmp.cab` | 0x140048d66 调用点 param_5 (文件模式) |
| 0x140122c68 | `tmp` | 0x14004a9f1 调用点 param_5 (目录模式) |
| 0x140128b10 | `mktmp` | 0x14009bf48 调用点 param_4 |

### 1.3 定案裁定 (替代 R19D 的 TODO(verify) 与 skill 旧记录)

| 项 | R19D/skill 旧记录 | 反汇编实证 | 处置 |
|---|---|---|---|
| fmt 目录模式 | 删尾 % | 带尾孤立 % (0x14011d0d8) | **保留尾 %**, 已落码 |
| fmt 文件模式 | 无尾 % | 无尾 % ✓ | 保持 |
| vararg1 | "tmp" (skill: tmp+PID+Tick+空串) | **param_4** (e26c="exedata") | 已落码为 param_4 |
| vararg2 | PID | PID ✓ | 保持 |
| vararg3 目录 | Tick | **Tick + NextRandomSeed()** | 已落码 |
| vararg3 文件 | Tick | **0** | 已落码 |
| vararg4 目录 | "" | **".tmp"** (硬编码) | 已落码 |
| vararg4 文件 | "" | **param_5** (第5参) | 函数签名扩为 5 参, 已落码 |
| 签名 | 4 参 | **5 参** ([RSP+0xd0]=caller [rsp+0x20]) | 4 调用点全补 |

### 1.4 调用点参数表 (反汇编实证 → 我方落码)

| 调用点 | 模式 | param_4 | param_5 | 我方落码 |
|---|---|---|---|---|
| 0x14001426b (e26c) | 目录 | "exedata" (原写 "exedat" 少1字符, 已修) | ".tmp"(CMOVNZ 缺省支) | restored_bodies.c:3369 |
| 0x140048d66 | 文件 | "devi" | ".tmp.cab" | core_b3_remaining.c:3003 |
| 0x14004a9f1 | 目录 | "devi" | "tmp" | core_b3_remaining.c:4095 |
| 0x14009bf48 | 目录 | "mktmp" | RAX 运行时值 (目录模式不读) | core_b3r_h2.c:682 (".tmp" 占位) |

### 1.5 残留登记 (divergence)

- e26c 的 param_5 在 RBX≠0 支取 [RSP+0x3d8](e26c 局部), 我方落 ".tmp"——目录模式不读 param_5,
  影响面为空; 若未来文件模式走 e26c 需回查。

---

## 2. unaff_R13D (S-TEAM-1) — 0x14004c0bc PECMD_ProcessScriptBlock 入口残留

### 2.1 反汇编证据 (0x14004c0bc)

- 序言: PUSH R13 保存 (0x14004c0cc), 后续 **全函数无任何 R13 读取** (search_instructions
  function=FUN_14004c0bc mnemonic in {mov,r13} operand r13 = 0 matches)。
- Ghidra 反编译 `local_170 = CONCAT44(local_170._4_4_,unaff_R13D) & ~0xff` 对应指令:
  - 0x14004c177 `MOV R13B, SIL`   (SIL = 0, 0x14004c0d9 XOR ESI,ESI)
  - 0x14004c18a `MOV [RSP+0x58], R13D`  (local_170 低 32 位)

### 2.2 裁定

**原版是"先清零再写"**: R13D 仅作为 0 的载体写入 local_170 低 32 位 →
`unaff_R13D ≡ 0` 恒真。我方 restored_bodies.c:5862 `undefined4 unaff_R13D = 0;`
(R19D S-TEAM-1) 是**精确还原**, 非近似。TODO(verify) 关闭。

---

## 3. 勘误回写

- `.agents/skills/pecmd-semantics/SKILL.md` 中 PATCH-1 定案记录 ("tmp + PID + Tick + 空串",
  "复核确认") 与反汇编实证冲突 → 已按 §1.3 表更正 (真源0 优先, 符合 skill 自身原则"冲突时以反汇编为准并回写勘误")。
- `analysis/r19d_fastfail_overrun.md` PATCH-1 补丁文本中的 "tmp/空串/删尾%" 为当时静态推断,
  本节后记登记: 以本档案为准。
