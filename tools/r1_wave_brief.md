# R1 还原批简报（stub_only 小函数批，等 link_stubs.c 独占权释放后执行）

## 前置条件
- 00e26c SEG6-10 子代理已完成并回交，主代理验证绿+提交后，方可派发本批（link_stubs.c 单编辑者纪律）。

## 任务范围（第一批，全部小体量）
按 tools/stub_wave_triage.json + tools/nameable_located.json(stub_only) 执行：
| addr | dc_line | 体行 | 目标名(还原后由主代理统一改名, 本批保持 FUN_) |
|---|---|---|---|
| 14005b104 | 53632 | 4 | (PECMD_FreeStrBuf) 870 调用点 |
| 14005b0b8 | 53603 | 6 | (PECMD_ZeroLenBuf) |
| 140063694 | 60848 | 24 | (PECMD_AllocWStrSlot) |
| 140061c44 | 59529 | 20 | (PECMD_LoadOle32Apis) 注意 ls 已有 real@ls 则核对即可 |
| 14005e7dc | 56627 | 6 | (PECMD_InitNullDaclSD) |
| 140025f10 | 22907 | 139 | (PECMD_AppendLogMessage) 体源 /tmp/opencode/25f10_body.txt 若失取 decompiled |
| 1400e6d74 | 141345 | 5 | (PECMD_FormatU64Dec) |
| 1400e6d68 | 邻近 e6d74 | ~5 | (PECMD_FormatI64Dec) |
| 140101e70 | 158409 | 10 | 特殊：ls 现桩签名 uint64_t(void) 错误，须改 bool(LPCWSTR) 并同步 b3_remaining 的 extern |
| 1400702f0/1400702d4/1400703e4 | 邻近 702b0 区 | 各~8 | StrBld 拷贝族 |
| 1400669c4/1400668ec | 66xxx 区 | 各~10 | AppendLongDecimal/AppendFmtValue |

## 规则
1. 只编辑 link_stubs.c；不改 core_*.c/include/*；不 git commit；不改名。
2. 体源：`grep -n "========== FUN_<addr>" ../decompiled.c` 后 sed 提取。
3. 保持原 FUN_ 名与既有签名风格；调用点 arity 以库内现有 extern 为准（如冲突以 decompiled 真签名为准并在报告列出）。
4. DAT 真值：python3 tools/pe_data_extract.py ../PECMD原始.EXE <va> <n>；运行时槽置 0+判空。
5. 每并入 3-5 个函数跑一轮：
   bash build.sh core_*.c （95 OK 0 FAIL）
   gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude core_*.c link_stubs.c -lm -o /tmp/opencode/link_test （0 undef/multi/error）
6. 无法忠实还原的 → 保持空桩不动并登记原因（不虚构语义）。

## 报告格式
- 每项：RESTORED/SKIP+原因/已存在(real@X 无需动作)
- 新增依赖符号清单
- 最终 build/link 输出摘要
