# R24d: C4013 隐式 int 地雷清单 (CDQE 截断 AV 类)

> R24f-d live 取证 041: `PECMD_CreateDirectoryTree` AV — rax=0xfffffffff621cea2 (real
> 0x258f621cea2) = core_b2a.c 未声明 FUN_14001BE14 → C4013 隐式 int → CDQE 符号扩展截断。
> 全库扫雷: 构建日志 C4013 共 **93 个(void/uint64-等不同名)** 调用点。

## 已修 (本轮)
- core_b2a.c: 补 FUN_14001BE14 extern → **060_path_special 翻转 PASS** (49/63)
- 指针返回型 extern 补齐 ×5 (预防): core_b3a(AllocMagicString), device/core_b3r_g
  (GrowByteBuffer), commands/core_b3r_d(NextToken), ui/core_b8i(ReplaceStringSlot),
  device/core_b3r_g3(UnquoteString)

## 地雷清单 (指针/HANDLE 返回型, 未修者按需加法)
| 函数 | 返回 | 调用点 | 风险 |
|---|---|---|---|
| PECMD_AllocMagicString | longlong* | b3a:368 | 已修 |
| PECMD_AssignString | int64_t* | b3r_i28c:351 | 桩 (restored_bodies), 修桩需同改 |
| PECMD_GrowByteBuffer | void* | b3r_g:267 | 已修 |
| PECMD_NextToken | uint16_t* | b3r_d:395 | 已修 |
| PECMD_OpenFileHandle | HANDLE | b3j:657 | 返回未用 (OUT 参), 观察 |
| PECMD_ReplaceStringSlot | int64_t* | b8i:484 | 已修 |
| PECMD_UnquoteString | short* | b3r_g3:191 | 已修 |
| PECMD_VarLookup | void* | b3r_d:590 | xproto 声明 uint64 (同宽), 观察 |
| FUN_14007E34C / 07A224 / 5B184 / 63 060 / 0E57C0 等 | — | 多处 | 各自核返回型 (大部分 void/uint64 类) |

## 机制
- C89 隐式 int 声明: 返回 EAX 低 32 位, 调用点如赋 64 位指针 → CDQE 符号扩展
  (≥0x80000000 的堆地址 → 0xffffffffxxxxxxxx) → 下一条指针解引用 AV。
- 与 StrPBrkW (win32_stub.h BOOL) / 013-BE14 (b2f) 同族 — R24f 五连虫 #2 的家族。
