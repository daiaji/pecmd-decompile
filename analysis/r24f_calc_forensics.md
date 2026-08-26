# R24f: CALC 簇取证定案 (FUN_1400bf358 返回值契约 + 原版活体对拍)

> 本轮取证为 R24f 工单「D 簇 CALC 归正 ×6 (026/027/028/032/033/039)」提供真值。
> 方法: dc 全函数逐段阅读 + 原版 PECMD.EXE 活体对拍 (LOAD 单行脚本测 exit, EXEC 通道
> 捞 %R%), Ghidra MCP 复核 .rdata 常量。构建身份: 未动构建 (基线仍 1c533d6)。

## 1. 结构现状: 求值器链已整体移植完毕, 只缺主驱动器

| dc 函数 | 状态 | msvc 符号 |
|---|---|---|
| FUN_140065140 (1827B) | ✅ 已移植 | PECMD_LexMathExpression (core_b3_remaining.c:12561) |
| FUN_14006587c (902B) | ✅ 已移植 | PECMD_ParseNumericValue (core_b3_remaining.c:12991) |
| FUN_140065c04 (760B) | ✅ 已移植 | PECMD_ParseDoubleNumber (core_b3_remaining.c:13139) |
| FUN_140070710 (1685B) | ✅ 已移植 | PECMD_MathCheckOperatorToken (core_b3_remaining.c:16130 区) |
| FUN_140070da8 (2630B) | ✅ 已移植 | PECMD_ExpressionArithmetic (core_b3_remaining.c:16235 区) |
| FUN_140065864 (21B) | ✅ 已移植 | PECMD_GrowArrayWrap/FUN_140065864 (restored_bodies.c:12986, 真实逻辑) |
| FUN_14007e34c (87B) | ✅ 已移植 | FUN_14007E34C (core_b3f.c) |
| FUN_14007e3a4 (3070B) | ✅ 已移植 | PECMD_ParseExpression (core_b3_remaining.c:19277) |
| FUN_14007efa4 (113B) | ✅ 已移植 | PECMD_ParsePathRecord2 (core_b3h.c:601) |
| FUN_14007f018 (95B) | ✅ 已移植 | PECMD_ParsePathRecord (core_b3f.c:522) |
| FUN_1400e69ac | ✅ 已移植 | PECMD_FormatDoubleToStr (core_b8m.c, %g 类格式化) |
| **FUN_1400bf358 (6015B)** | ❌ 简化版失真 | PECMD_CalcExpression (core_b3_remaining.c:28361) ← **本工单** |

其余小 helper (strbuf/切分/前缀比较/求值包装) 均已在库内 (映射表见 r24f_calc_port_spec.md §3)。
结论: D 簇根因 = **主驱动器被替换为「重构/简化版」**, 求值器链本体从来都在。

## 2. 返回值契约 (dc:118318-119158 + 原版活体对拍定案)

- PSB CALC 分支 `local_180 = PECMD_CalcExpression(script, txt, 0)`; 槽写入带
  「非零才写」guard (U-1), 退出码 = 槽值 (低 32)。
- dc 每轮语句循环启动处 `pWVar5 = 0`; 语句处理收口 `LAB_1400c0876` (语句文本游标
  local_168 指到 NUL 且变量名非空): 赋变量后 **`pWVar5 = pWVar6`**。
- pWVar6 = 求值器标志字节 `(char)local_res20`: 成功 0 / **除0 = 0x10** / 其他错误位。
- ⇒ **成功返 0, 除 0 返 16**。活体实测逐一吻合:
  `CALC R=1+2*3`→0, `CALC R=1/0`→16, `CALC R=9/2`→0, `CALC R=0x10+1`→0,
  `CALC R=(1+2)*(3+4)`→0; `-err=17 R=1/0`→16 (err= 只影响错误文本不影响返回值)。
- 除 0 的 dc 落点: FUN_140070da8 除号分支 `dVar13==0.0 → *(byte*)(param_1+8) |= 0x10`
  (dc:71352-71355 区) → E3A4 收口置结果 0 + 标志 → F018/EFA4 的 *param_2 标志 = 0x10
  → bf358 pWVar6=16 → LAB_1400c0876 赋值分支 pWVar5=16 → return 16。
  (之前 R24 主担心的「exit=指针低32」只发生在 msvc 简化版, 原版无此现象。)

## 3. 原版求值活体对拍 (单脚本多变量一次捞取, p_all.pecmd, exit=16)

```
R=1+2*3 → "7"       A=9/2 → "4.5"   (double 除法, %g 格式化)
B=0x10+1 → "17"     C=(1+2)*(3+4) → "21"
D=7%3 → "1"         E=5-9 → "-4"    F=0-5 → "-5"
G=1.5+0.5 → "2"     H=8/2 → "4"
I=%X%*3+1 (X=2) → "7"   (变量展开后求值)
J=2147483647+1 → "2147483648"  (double 精确, 无溢出截断)
K=0xFFFFFFFF+1 → "4294967296"  (hex 字面量按无符号→double)
L=1/0 → "0"        (除 0 仍赋值 R="0" 并返 16!)
```
格式化通道: 数值路径 E69AC (double, %g 类); 表达式路径 ('#' 前缀) E6D38 "%I64d"/"%I64u"。

## 4. '#' 前缀 = 表达式模式开关 (新发现, dc 内证)

bf358 函数级 `if (local_res10->refcount == 0x23) { uVar33 |= 0x23; ...}` — 语句首 token
为 '#' (ASCII 0x23; 全角 '＃' 经归一化) 时 uVar33 ≥ 0x23 → 求值走 **FUN_14007f018
(表达式路径, E6D38 格式化)**; 否则 uVar33 < 0x23 → **FUN_14007efa4 (数值路径, E69AC
格式化)**。core_b7a.c 的 PECMD_EvalParenthesizedExpr 把 '(' 改写成 '#' 再交求值器
(dc:0x1400c10c0 移植体) — 互证此约定。corpus 六个用例都是 uVar33=0 数值路径。

## 5. .rdata 常量实测 (Ghidra MCP 读原版)

- DAT_140129848 = L"+txt" L"+icon1 0" (local_88 = + (4-local_17c) 偏移表)
- DAT_140129810 = "+cb\0" (6 字节)
- DAT_14011C638 → g_szEmpty; DAT_14013E190 → g_csInit; DAT_140125238 → g_fontMinus0

## 6. 验收目标 (移植体上线后逐项核对)

1. run_case 026/027/028/032/033/039: exit 0/16/0/0/0/0, 六案全 pass
2. p_all.pecmd 跑 msvc: 输出 `7|4.5|17|21|1|-4|-5|2|4|7|2147483648|4294967296|0`, exit=16
3. 全量 63 案 ≥41 PASS 零回归 (D 簇 +6 → 期望 47)
## 7. 集成期五连虫定案 (windbg 现场 + PageHeap 逐层伏击, 部署体 md5 7d1b2bb9)

| # | 症状 | 根因 | 定案证据 | 修复 |
|---|---|---|---|---|
| 1 | 全 6 案 0xC0000005 | E34C 移植体 `(uint8_t*)ctx + 0x12` 误清 byte 0x12(=data1 指针第3字节); 原版清 byte 0x28/0x48 (uint32 单位) | 硬件写监视当场抓获 rcx=槽值+2; Ghidra 反汇编 @14007e36e/389 | core_b3f.c E34C 两行改 0x28/0x48 |
| 2 | 0xC0000409 | StrPBrkW 无原型 → C89 隐式 int → CDQE 截断 64 位返回 (0x23a86b4e1ba→0xffffffff86b4e1ba) | 反汇编 call 后 `cdqe`; win32_stub.h:888 错声明 BOOL | win32_stub.h + stubs_common.h 改 LPWSTR 原型 |
| 3 | 0xC0000374 | 包装器按局部名释向量 — MSVC 局部排布偏移 8B → 实释 op 栈 cap(=4) → HeapFree(-4) | PageHeap Verifier 0xC "Heap block: -4"; 槽值=4 | 按 dc 语义改释 ctx+0x10/0x30 |
| 4 | 0xC0000374 残留 | MSVC 把包装器临时量 uVar1/puVar2 排在 ctx+0x10 区 — 结果值覆写 vec1-data 槽 | 写监视抓获 ParsePathRecord2+0x98 `mov [rsp+38h],rax`(7.0) 正落在槽上 | 包装器 ctx 改堆分配 (calloc), 布局冲突结构上排除 |
| 5 | 值全 0 (exit 契约已对) | repo FormatDoubleToStr 调 SafeVFormatW 只传精度 p, 值不进格式串 vararg → %.*lf 垃圾输出 | 静态判死: core_b8d.c SafeVFormatW 仅 1 参; setvar 实证 value="0" 而解析值=4294967296.0 | core_b8m.c 改 swprintf(..., p, value) |
| 6 | ctx 尾部越写 (PageHeap suffix @+0x48) | 解析器/0706B4 尾区宽访越过 calloc(0x48) — 定位为访问包络需 >0x48 | Verifier 0xF corrupted suffix, block=calloc 块 | calloc 0x48→0x60 (定案安全包络) |

验收: 026/027/028/032/033 PASS (exit 0/16/0/0/0 全对金标), 015/018/034 零回归, 039 残留 exit=2 (TEAM 槽隔离, E 簇工单)。
值契约: 11/13 与原版逐位一致; 2 项非语料偏差登记:
- `7%3` → msvc 7 vs 原版 1 (%-数字实参展开语义差, PSB 侧)
- `1.5+0.5` → msvc -NAN(IND) vs 原版 2 (ParseDoubleNumber 小数分支)

## 8. R24f-b 续: F 簇桩直移 + 浮点常量定案 (部署体 md5 dcc512a7)

- F 簇: PECMD_SkipWCharUntil + thunk_FUN_1400f429c 双桩按 dc FUN_1400f429c (dc:149819) 直移
  (unimplemented_stubs.c) → 029/040 翻转 PASS (0x80070057→0), 049 语义修复(exit 仍被 FIND 掩蔽);
  048/050/058/062 "用例形态"案保持掩蔽 PASS; 全量 **48/63 零回归** (F 簇工单闭合)。
- 浮点常数四联 (Ghidra .rdata 定案): DAT_140126390=5.0 (旧 0.0 → 小数分母 0 → -NAN(IND)),
  DAT_140126398=(double)INT64_MIN (箍环), DAT_140121668=2^63 (回绕修正), DAT_140124110=e。
  修复后 1.5 系由 -NAN 转 "仅整数部分" — 残留 = 小数位截断, 机理锁定在箍环
  (dVar7<dVar13 恒真 + 2^63 上推 → cvttsd2si 饱和) — 需原版汇编逐指令对照后方可定谳,
  登记为待办 (非语料路径, 不阻塞对拍)。

## 9. R24f-c B 簇档案 (FILE 契约×3, 症状级定案, 待专项调试)

- 活体契约定案: 原版 FILE 复制成功/删除成功/源缺失 全返 2 (pb1-pb4 探针);
  EXEC-echo 返 0 (pb2) → 金标 2 全来自 FILE 行自身, 由 SHFileOperationW 路径
  (bVar25==0) 返回缺文件错 2 达成 (010 源 C:\\pectest\\out\\demo.txt 在用例时序中不存在 —
  run_case 每案清空 out!, 源缺失是固定前提)。
- msvc 症状: 010/012 = 87 (SHFileOperationW 参数非法), 011 = 0 (删"成功").
- 现场 (windbg @shell32!SHFileOperationW 首击): struct@栈 — hwnd=0x18 / wFunc=0x1a (垃圾,
  应为 0/2!) / fFlags=0x614 (真实) / pFrom=0x20d1b2d1538 pTo=0x20d1b2bf578 (du 不可读=悬垂).
  ⟹ msvc 的 local_b0 结构在 token 循环后首 8 字节被覆写、pFrom/pTo 指向已释放块 —
  与 R24f 五连虫同族 (栈/生命周期), 需下一次专项调试 (bp wFunc-写点/FreeStrBuf 计时)。
  登记至 B 簇工单 (010/011/012), 不阻塞他簇。

## 10. R24d-b B 簇编译产物对照 (Ghidra 当前构建反编译)

- 当前构建 (md5 dab02b3f 代) 的 FUN_14003C06C 由 Ghidra 完整反编译 (import+auto 分析):
  结构逐段与 dc 同构 — 选项扫描/wFunc 1/2/3/4 赋值/'=' 前驱判定/memset(0x30)/
  pFrom=pTo 槽拷贝/SHFileOperationW 分支/GetLastError→uVar 返回链 全部一致。
  ⟹ 87 非代码结构缺陷, 锁定数据层: SHFO 结构 (hwnd/wFunc 区 = 0x18/0x1a 垃圾,
  pFrom/pTo 悬垂) — 疑似局部结构未被清零的另一个入口 (typedef fFlags=WORD 小异不解释 87),
  需下一轮: ba 写监视 local_b0 wFunc/hwnd 区 + pFrom/pTo 内容指针的释放时序 (FreeStrBuf 计时).
- 附带: rCX@SHFO = &uStack_70, 栈上结构首 8 字节稳定出现 0x18/0x1a (跨两次会话) —
  指向"首个写入者"为该 executor 的确定性代码 (非随机垃圾)。

## 10b. B 簇三侧同构 + 新证据点 (local_68)

- 三侧逐字节同构: dc (decompiled.c:35252/35363) ≡ msvc C 源 (b2f:7207+) ≡ msvc 编译产物
  (Ghidra 反编译 FUN_14003e710): 选项扫描/wFunc 赋值/'=>' 前驱判定/memset(0x30)/
  StrBld 双槽拷贝/SHFO 分支/GetLastError 链。
- 新证据点: `local_68 = 0` 仅一次初始化, 唯一使用 = 尾部终止符写入
  `*(WCHAR*)(local_70.QuadPart + local_68*2) = 0` 恒写 [0] — 三侧完全一致,
  dc 亦同 (decompiled.c:35363)。⟹ "[0]=0 清 pFrom" 理论不成立 (原版同样行为且工作)。
- 结论: 87 机制不在语法/结构层 — 唯一差异渠道 = SHFO 运行时实参内容/悬垂 (live 观测),
  下轮以 ba 写监视 pFrom/pTo 槽内容指针 + 释放时序收口 (windbg 通道 T2/T3 故障面期间
  改用 dump 对比: 原版 SHFO 前后堆内容 vs msvc)。

## 10c. B 簇探针定案 (R24d-c, TEMP PROBE @SHFO 双调用点, md5 dc543d7a)

- 011 现场 (Win32 通道探针): `SHFO hwnd=18 wFunc=1a pF=...F5F8 pT=...385A srcS=[] dstS=[]`
  — 结构首 8 字节 = 栈残留 (未初始化) + pFrom/pTo 内容为空串。
- dc-len 机制复原: FUN_1400216c4 = 向 {data,len,cap} 对象追加 — 调用 `&pLVar16->QuadPart`
  (pLVar16=&local_70) 时 len 槽 = local_68 (栈续接别名!) — 尾部 `data[local_68]=0` = 串尾
  终止符。msvc 移植体 PECMD_AppendParamToken (b2b:272) 同样维护 list[1]=len ✓ —
  ⟹ 空内容 = token 切片阶段 (LVar21 内容) 已空/追加被跳过 (append 对空 token 直接 return)。
- 待下轮: append 调用点探针抓 token 实况 (切片位置 vs in-place 指针) — SHFO 87 = 空路径
  参数的必然结果 (ERROR_INVALID_PARAMETER); 空切片疑点 = Func_140011da0/063620 槽初始化
  或 LVar21 指向被扩展开销后的副本区。

## 10d. B 簇二级探针实验 (R24d-d, 已回滚 — 记录全部数据)

- 二级探针 (ENTRY + APP-SEMI + APP-FINAL + SHFO) 实测 011:
  `ENTRY len=25 str=[C:\\pectest\\out\\copy.txt<>]` — **PSB 传参完全正确** (空路径不在传参层);
  且不同探针构建间行为漂移: dc543d7a 011=87(SHFO 探针触发, wFunc=0x1a garbage),
  75b3e0b2 011=87(仅 ENTRY, SHFO 未触发), 显式 len 终止符版 011/010/012 全部 124 挂起 —
  ⟹ 探针/改动会改变该函数编译形态与运行时行为 (V4 透明性违反的实证), 已 git checkout
  2cb7fca 回滚干净基线 (md5 a18a8964, 010=87/011=0/012=87 与 triage 一致, 全量 49/63 稳)。
- 候选根因残余: 终止符写 `data[local_68]=0` 的 local_68 依赖与 StrBld 对象 len 槽的栈别名
  (dc 同构依赖) — 显式别名读取版触发挂起, 机制未明 — 下次以"不改代码的 windbg 单步观察
  011 的 local_b0 段"收官 (或 dh 堆对比原版/msvc 的 pFrom 内容)。

## 10e. B 簇挂起实锤 (R24d-e, live 栈×2 + bp 轨迹, 已回滚)

- 011 挂起 (124) 实锤: 两次 live 栈 = USER32!MessageBoxW ← FUN_1400630D0 (错误弹窗)
  ← PECMD_HeapRealloc+0x177 ← PECMD_AllocString+0x28 ← **PECMD_AppendParamToken+0x14b**
  ← FUN_14003C06C+0x7c0 — 即 Append 内 AllocString 收到巨量 count → HeapRealloc 失败 →
  弹"内存不足"框阻塞。
- 尝试修复 A (终止符显式 len 读 &obj+8): 011 仍挂 (同栈) — 去掉探针纯改动版同样。
- 尝试修复 B (+ 槽显式清零 &obj+8/+0x10): 仍挂 (同栈) — ⟹ len 槽模型不完整 —
  append 的 list[1] 巨量读另有来源 (list 实参/槽区域 与 C 变量布局的深层错位)。
- bp PECMD_AllocString 轨迹: 11 连击全为 NewVarNode/StrCopy 帧 (ENVI 变量节点创建),
  AppendParamToken 帧从未出现于 bp 命中 — FILE 行的 append 在调试器慢速下未及执行即
  …(或 011 路径先弹窗)。011 的 87/0 与挂起三态随构建波动 (07:05-07:14 五次构建观察)。
- 结论: B 簇 = AppendParamToken 的 list 对象布局错位 (与 C 变量分离) + HeapRealloc
  失败弹窗双重; 已回滚 2cb7fca 干净基线 (md5 a8b828ac, 011=0/010=87); 下轮方向:
  bp PECMD_AppendParamToken 入口读 list[1]/list[2] 实值 (或先修 HeapRealloc 失败弹窗
  为静默返回以暴露真实计数)。

## 11. B 簇收官 (R24e): StrBld 对象结构体化 — 010/011/012 + 041 全翻转, 49→53/63

- 根因最终定谳: dc 的 FUN_14003c06c 用 local_70/68/60 (及 58/50/48) 三邻接局部充当
  StrBld 对象 {data,len,cap} — 1400216c4-append 经 &local_70 以 list[1]/list[2] 写 len/cap。
  该邻接依赖编译器布局: MSVC 优化下 C 变量被单独放置 → len 槽 (对象区+8) 读栈残留
  → (a) 终止符 data[local_68=0]=0 清空 pFrom/pTo → SHFO 87; (b) 残留巨大时 append 的
  AllocString 巨量 count → HeapRealloc 失败 → MessageBoxW 弹窗挂起 (124) — 三态同根。
- 修复: 将两组三局部替换为单块结构体 `struct { WCHAR *data; int64_t len; uint64_t cap; }
  strb1/strb2` — 24B 连续布局由类型系统保证, 别名依赖彻底消除 (dc 语义等价)。
- 验收: 010=2 ✓ 011=2 ✓ 012=2 ✓ (55 天内 B 簇 ×3 首翻); 041 (MDIR+FILE 组合) 连带
  2 ✓; 全量 63 案 53/10 零回归 (10 FAIL = 已知队列 E×3/J×2/G×2/H×2/I×1)。
- 部署 md5 60e5b054; 下一步登记: 同款"三邻接局部拟 StrBld 对象"模式全库排查。
