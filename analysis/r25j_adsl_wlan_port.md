# R25-j — ADSL(PECMD_WlanManage) dc 直移补全报告

- 任务书: `tools/task_r25j_adsl_wlan.md`（D-18：ADSL 动词处理器为语义级重写且缺约 2/3 分支的「假真体」）
- 目标: `src/device/core_b3r_h2.c` 中 `PECMD_WlanManage`（=dc `FUN_140099f18 @0x140099f18 size=7183`）整体替换为 dc 直移体
- dc 源: `reference/decompiled.c` 行 96610–97468（97472 起 `FUN_14009bb28` 头之前）
- 本轮纪律: 只写 `src/device/core_b3r_h2.c` 与本报告；不构建、不 git、不用 windbg/Ghidra MCP

## 1. 结论速览

| 项 | 结果 |
|---|---|
| RESTORED | `PECMD_WlanManage` dc 直移体（dc:96610–97468 全量, RAS 半区 + WLAN 半区 + 接口/网络双清单格式化 + XML 配置文件构造） |
| 补最小件 | `FUN_1400e4e6c @0x1400e4e6c`(size=37, 工程未还原) → 本文件 static `PECMD_InitEventSlots`（dc 直移 5 语句, 见 §5.8） |
| SKIP | 无 |
| 纯度自检 | 行数 957 ≥ 859×0.9=773 ✓；if 105（=dc 104 + 1 处逗号表达式拆分, 逐条见 §4）≥ 95 ✓；调用点 205/205 逐名对表 ✓ |

调用方 `restored_bodies.c:6664`（ADSL tag 0x4c534441）签名未变:
`ulonglong PECMD_WlanManage(longlong *param_1, LPCWSTR param_2)`。

## 2. 变更点（唯一被改文件: src/device/core_b3r_h2.c）

1. 删除原 1966–2373 行的语义级重写残体（408 行, 仅覆盖 RAS 半区, WLAN 半区整段缺失）。
2. 换入 dc 直移体: 文件 1997–2988 行（函数头注 1997 起, 函数体 2032–2988, 体 957 行）。
3. 函数体前新增:
   - 10 个 dc 依赖的 extern（工程已有真体/桩, 见 §6）;
   - 5 个 `PECMD_FN_Wlan*` typedef（仅本函数调用点就地换型, 见 §5.2）;
   - `PTR_FUN_14012b1c0` / `g_hSyncEvtA` / `g_hSyncEvtB` 三个数据 extern + static `PECMD_InitEventSlots`（dc:FUN_1400e4e6c 直移）。
4. 函数外其余内容一律未动（文件头注、CONCAT 宏、既有 extern 块、其它函数均原样）。

## 3. 移植方法

dc 为唯一语义真值, 逐行直移、变量名沿用 Ghidra 名、逐段附 `dc:行号` 注释。
对 dc 中三类无法直译的伪影（变参丢弃 / unaff 残留 / 函数指针槽丢参），按 AGENTS.md
「回溯原版 EXE 反汇编定案」路径, 用 python+capstone 对 `reference/PECMD.exe`
（基址 0x140000000）做了点验级反汇编, 恢复被丢弃的实参/实参数后按二进制忠实移植。
全部恢复点 VA 在 §5 逐条列出, 未做任何无出处的语义补齐。

## 4. 分支核对表（dc 104 if → 新体 105 if）

自动按条件文本匹配 84 条; 20 条因换型/改名拼写差异人工核对同构; 另 1 条 msvc 侧
拆分（dc:97061 的 && 右操作数逗号表达式 `(uVar32 = 0, 0 < *local_c20)` 拆为独立
if, 赋值时序与判定结果等价）。合计 dc 104 → msvc 105, 语义 1:1。

| dc:if | dc 条件(截断) | msvc:if |
|---|---|---|
| 96726 | if ((char)uVar12 != '\0') { | 2151 |
| 96730 | if (((8 < (ushort)WVar1) && ((ushort)WVar1 < 0xe)) \|\| (WVar1 == L' ')) | 2158 |
| 96731 | if (*local_res10 != L'\0') { | 2159 |
| 96750 | if (cVar7 != '\0') { | 2178 |
| 96754 | if (*local_res10 == L',') { | 2182 |
| 96758 | if (*local_res10 == L',') { | 2186 |
| 96764 | if (*local_bc0 == L'?') { | 2193 |
| 96769 | if (*pwVar31 == L'*') { | 2198 |
| 96770 | if (pwVar31[1] == L'*') { | 2199 |
| 96775 | if (*pwVar31 == L'*') { | 2204 |
| 96780 | if (*pwVar31 == L'-') { | 2209 |
| 96784 | if (*pwVar31 == L'^') { | 2213 |
| 96789 | if (iVar9 == 0) { | 2218 |
| 96794 | if (iVar9 == 0) { | 2223 |
| 96799 | if (iVar9 != 0) { | 2228 |
| 96801 | if (iVar9 == 0) { | 2230 |
| 96806 | if (iVar9 != 0) { | 2235 |
| 96808 | if (iVar9 == 0) { | 2237 |
| 96814 | if (iVar9 == 0) { | 2243 |
| 96819 | if (iVar9 == 0) { | 2248 |
| 96824 | if (iVar9 == 0) { | 2253 |
| 96832 | if ((int)local_c48[0] < 1) { | 2263 |
| 96845 | if (local_c53 != '\0') { | 2276 |
| 96860 | if (*local_res10 == L',') { | 2291 |
| 96865 | if ((cVar22 != '\0') \|\| (bVar8)) { | 2296 |
| 96869 | if (local_c27 == '\0') { | 2301 |
| 96873 | if ((int)CONCAT71(extraout_var_00,bVar8) != 0) { | 2306 |
| 96874 | if (bVar5) { | 2307 |
| 96881 | if (cVar22 == '\0') { | 2314 |
| 96882 | if (bVar4) { | 2315 |
| 96896 | if ((lVar13 != 0) && | 2330 |
| 96905 | if ((int)uVar25 < 0) { | 2343 |
| 96931 | if (iVar10 != 0) { | 2372 |
| 96966 | if (((cVar22 != '\0') \|\| (bVar2)) \|\| ((char)local_c38 != '\0')) { | 2408 |
| 96975 | if (bVar3) { | 2417 |
| 96978 | if ((int)uVar32 < 0) { | 2420 |
| 96989 | if (DAT_14013d788 == (code *)0x0) { | 2432 |
| 97002 | if (DAT_14013d788 == (code *)0x0) { | 2453 |
| 97013 | if (iVar10 != 0) { | 2466 |
| 97016 | if (DVar11 == 0) { | 2469 |
| 97029 | if (local_b80 != 0) { | 2483 |
| 97032 | if (DVar11 == 0) { | 2486 |
| 97037 | if ((local_c20 == (int *)0x0) \|\| (*local_c20 == 0)) { | 2491 |
| 97038 | if (((char)local_c38 == '\0') \|\| (cVar22 != '\0')) { | 2492 |
| 97044 | if (*local_c20 <= (int)uVar32) { | 2498 |
| 97051 | if ((bVar4) && (*local_c30 == L'\0')) { | 2505 |
| 97054 | if (((int)uVar32 < 0) \|\| (*local_c20 <= (int)uVar32)) goto LAB_14009a7 | 2508 |
| 97056 | if (((local_c48[0] != 0) \|\| (bVar4)) && (*local_c30 != L'\0')) { | 2512 |
| 97060 | if ((((int)CONCAT71(extraout_var,bVar8) < 1) \|\| (*local_c50 != L'\0')) | 2517 |
| 97065 | if (iVar10 == 0) goto LAB_14009a8e9; | 2523 |
| 97068 | if (iVar10 == 0) goto LAB_14009a8e9; | 2526 |
| 97078 | if ((char)local_c38 != '\0') { | 2540 |
| 97084 | if (!bVar3) break; | 2555 |
| 97087 | if (*local_c20 <= (int)uVar25) break; | 2559 |
| 97095 | if ((char)local_res20 == '\0') { | 2567 |
| 97101 | if (bVar4) { | 2573 |
| 97105 | if (bVar2) { | 2577 |
| 97107 | if (iVar10 == 0) { | 2581 |
| 97113 | if (cVar22 == '\0') { | 2587 |
| 97124 | if ((*local_c38 == 0x23) && (lVar13 = FUN_14006ae00(local_c38,&local_r | 2599 |
| 97129 | if ((*local_be8 == 0x23) && (lVar13 = FUN_14006ae00(local_be8,&local_c | 2605 |
| 97139 | if (((char)*local_c08 == '\0') && | 2616 |
| 97142 | if (((char)local_bd0 != '\0') \|\| (local_c48[0] != 0)) { | 2619 |
| 97155 | if (uVar25 != 0) { | 2637 |
| 97158 | if (DVar11 == 0) { | 2640 |
| 97168 | if (*psVar24 != 0) { | 2650 |
| 97172 | if (*psVar24 != 0) { | 2654 |
| 97176 | if (*local_bc0 == L'>') { | 2658 |
| 97181 | if (wVar16 != L'\0') { | 2663 |
| 97183 | if (((8 < (ushort)wVar16) && ((ushort)wVar16 < 0xe)) \|\| (wVar16 == L' | 2665 |
| 97185 | if (*pwVar14 != L'\0') { | 2667 |
| 97198 | if (iVar10 == 0) { | 2680 |
| 97200 | if (*local_c50 != L'\0') { | 2682 |
| 97221 | if (*local_b98 != L'\0') { | 2703 |
| 97226 | if (0 < (longlong)iVar10) { | 2708 |
| 97236 | if (*psVar24 != 0) { | 2719 |
| 97244 | if (*pwVar31 == L'&') { | 2727 |
| 97252 | if (ppWVar15 != (LPWSTR *)0x0) { | 2741 |
| 97256 | if (DAT_14013ccb1 != '\0') { | 2749 |
| 97279 | if (iVar10 == 0) { | 2776 |
| 97282 | if (local_b88 != (LPCWSTR)0x0) { | 2779 |
| 97286 | if (iVar10 != 0) { | 2783 |
| 97289 | if (((int)cVar22 & 2U) != 0) { | 2786 |
| 97297 | if (0 < (longlong)pwVar31) { | 2794 |
| 97303 | if (local_c57 == '\x01') { | 2800 |
| 97305 | if (0 < iVar10) { | 2802 |
| 97311 | if (iVar9 == 0) goto LAB_14009ac4c; | 2808 |
| 97323 | if ((int)local_be8 == 0) { | 2821 |
| 97334 | if (iVar10 < 1) { | 2832 |
| 97336 | if ((char)local_c38 != '\0') goto LAB_14009aea7; | 2834 |
| 97343 | if ((int)local_c08 != 0) { | 2841 |
| 97351 | if (8 < iVar9) { | 2868 |
| 97355 | if (0 < lVar13) { | 2872 |
| 97370 | if (local_c57 == '\x02') { | 2888 |
| 97372 | if (-1 < (int)uVar25) { | 2890 |
| 97381 | if (((iVar9 == 0) && | 2899 |
| 97402 | if (local_c57 == '\x10') { | 2920 |
| 97404 | if (-1 < (int)uVar25) { | 2922 |
| 97413 | if (((iVar9 == 0) && (*(short *)((longlong)pWVar30 + lVar28 + 8) != 0) | 2931 |
| 97427 | if (iVar9 == iVar10) goto LAB_14009ad9f; | 2945 |
| 97447 | if (DVar11 == 0) { | 2965 |
| 97451 | if (local_ba0 != 0) { | 2969 |
| 97454 | if (local_b88 != (LPCWSTR)0x0) { | 2972 |
| 97457 | if (local_c20 != (int *)0x0) { | 2976 |

msvc 多出的 if 共 1 处 (dc:97061 的 && 右操作数逗号表达式 `(uVar32 = 0, 0 < *local_c20)` 拆为独立 if, 语义等价):

| dc | 条件 | msvc:if |
|---|---|---|
| 97061(拆分) | (uVar32 = 0, 0 < *local_c20) | 2519 |


## 5. Ghidra 伪影处理决策（全部有 dc 行号 + EXE VA 证据）

### 5.1 变参调用丢参（wsprintfW ×3）

- **dc:97081** `wsprintfW(local_bc0+local_bb8, "%s%d\t%s\t%u\t%s")` dc 零实参。
  EXE @0x14009a9b7: r8 = 行分隔前缀（r15d>0 ? DAT_1401210f8(L"\r\n") : DAT_14011c638(L"")）,
  r9d = esi = 当前接口下标(uVar32), [rsp+0x20] = &local_b08（GUID 串）,
  [rsp+0x28] = *(u32*)(entry+0x210)（State）, [rsp+0x30] = entry+0x10（Description）。
  行计数 r15d 初值 0 @0x14009a94e, 随下标 +1 @0x14009a9e3（新体引入 `uVarRow`,
  声明注记 msvc:2068）。
- **dc:97346** 16 实参仅剩 2。EXE @0x14009af57: arg3 = local_c50（[rsp+0x98],
  该槽与 FUN_140074838/FUN_140063620 的 &local_c50 三处调用点互证）, arg4 = local_be0,
  arg5 = 0x14011fa84 = L"\t", arg6..16（%u×11）= *(u32*)(r15±off), r15 = pWVar35
  （=entry+0x258）: +4→0x25C SignalQuality, +0x14→0x26C Flags, -0x34→0x224 BssType,
  -0x30→0x228 NumBssid, -0x2c→0x22C Connectable, -0x28→0x230 Reason, +8→0x260
  SecEnabled, +0xc→0x264 Auth, +0x10→0x268 Cipher, +0→0x258 MorePhyTypes,
  -0x24→0x234 NumPhyTypes（与 WLAN_AVAILABLE_NETWORK 布局逐字段吻合）。
- **dc:97248** 6 个 %s 仅剩 2。EXE @0x14009b4a5: 1,3 = pWVar21（模板 `<name>` 出现两次）,
  2 = local_b68（hex）, 4 = local_bc8（authentication）, 5 = pWVar20（encryption 行）,
  6 = local_ba8（sharedKey 块）; 格式串 = local_c50（0x140128530 模板, dc:97241 转换所得）。
- 认证/加密常量核验: 0x1401288d0=L"WPA2PSK", 0x14011daf0=L"open", 0x1401288b8=L"none",
  0x1401288c8=L"AES"（dc:97167-97173/97195-97206 同构）。

### 5.2 wlanapi 槽调用丢参（Ghidra 槽签名过窄; pecmd_globals.h 声明本 TU 已含且禁改, 按文件指针值就地换型调用）

| dc 行 | 槽 | dc 实参 | EXE 实调（VA） | 处理 |
|---|---|---|---|---|
| 97012 | DAT_14013d788 OpenHandle | 3 | 4: (2, NULL, &ver, &hClient) @0x14009a686 | typedef 换型 4 参 |
| 97106 | DAT_14013d7b0 Scan | 2 | 5: (h, guid, 0, 0, 0) @0x14009aaca | typedef 换型 5 参 |
| 97254 | DAT_14013d798 SetProfile | 4 | 8: (h, guid, 0, xml, NULL, 1, NULL, &reason) @0x14009b535 | typedef 换型 8 参 |
| 97278 | DAT_14013d7b8 GetAvailNetList | 4 | 5: (h, guid, 0, 0, &local_b88) @0x14009ab0c | typedef 换型 5 参 |
| 97455/97458 | DAT_14013d7c8 FreeMemory | 0 | 1: rcx = local_b88 / local_c20 @0x14009b6bc/6cf | typedef 换型 1 参 |
| 97102/97152/97452/97028 | Disconnect/Connect/CloseHandle/EnumInterfaces | 与实 ABI 一致 | @0x14009aab5/64e/6a9/— | 直接调用（实参换型） |

SetProfile 的 reason 出参落 `local_res10` 低半, 与 dc:97258 `local_res10 & 0xffffffff`
的调试读取自洽（EXE [rsp+0x38]=&[rsp+0xcf8]）。

### 5.3 unaff/未初始化残留

- dc:96701（local_be8 高半自读移位）、dc:96714、dc:96717（低字节清零残留）→ 按 0 定值,
  注释标明; 高半全文无读点。
- dc:97166/97232 `CONCAT44(uStack_b7c, local_b80)` 为同一 64 位槽的 Ghidra 拆分
  （local_b80 后被复用为宽密码指针）, 新体以 `uint64_t local_b80` 承载, 直写同值。

### 5.4 CONCAT 拼接

CONCAT31/44/71 共 16 处按本文件既有惯例展开为 `&0x…ff00ULL | n` 位运算（文件头注成文）。

### 5.5 dc:97060 良性解析路径的 uVar32 刷新（dc 数据流缺口, 二进制补证）

dc:97059 二次解析成功路径 dc 无显式 `uVar32` 刷新（落至 97076 时为 token1 首解析旧值）;
EXE @0x14009a8e2 `mov esi,[rsp+0xcf8]` 证得该路径取二次解析值（local_res10 低半）。
新体在 msvc:2536 补 `uVar32 = local_res10 & 0xffffffffULL;`（注释引用 dc+VA）。

### 5.6 WLAN_CONNECTION_PARAMETERS 参数块（dc 漏记 strProfile）

dc:97147-97151 将参数块写成 local_b60[2]+local_b50+local_b40+local_b3c 拼合; EXE
（[rsp+0x188] 起 0x28 字节）: +0 mode=0, +8 strProfile=local_c18（@0x14009b633, dc 漏记,
新体 msvc:2629 补行）, +0x10 pDot11Ssid=&local_b38, +0x18 NULL, +0x20 BssType=3,
+0x24 dwFlags=1。WlanConnect 第 3 参按块基址传递。

### 5.7 结构布局（按 dc 访存偏移还原, 未引入 wlanapi.h）

WLAN_INTERFACE_INFO(0x214: GUID@0, Description@0x10, State@0x210)、
WLAN_AVAILABLE_NETWORK(0x274, 12 字段映射见函数头注) — 与 dc:97063/97076（0x85 int
步进）、97320（SSID@+0x204）、97350（PhyTypes 计数@+0x234）等访问点互证。

### 5.8 FUN_1400e4e6c（工程未还原件）

dc body（size=37, 5 语句: 挂 PTR_FUN_14012b1c0 / 清 DAT_14013e1f8、DAT_14013e200 /
清 param_1[0x14f]) 以 static `PECMD_InitEventSlots` 落在 msvc:2004-2015, 调用点
dc:96926 → msvc:2364。数据符号工程已有（core_globals.c:167-168 = g_hSyncEvtA/B;
PTR_FUN_14012b1c0 见 core_b8h.c:67 同款用法）。select static 避免与后续批次还原
符号的链接冲突。

### 5.9 dc:97311/97427 跨块 goto 与标签

LAB_14009ad9f（打印体）、LAB_14009ac4c/LAB_14009ad07（排序去重）、LAB_14009b5a0
（连接块, 兼受 dc:97262 set-profile-then-connect 回跳）等标签均按 dc 落位; C 块内
标签 + 跨块 goto 与原控制流一致。

## 6. 外部依赖清单

**已有真体（直接引用, 零新增定义）**:
PECMD_FreeStrBuf(0x14005b104)×31、PECMD_FreeContainer(0x14005b134)×28、
PECMD_ZeroLenBuf(0x14005b0b8)×6、PECMD_SkipLeadingControlChars(0x14005b154)×3、
PECMD_GetApiProcCached(0x14005c828)×10、PECMD_AllocStrSlot(0x140063620)×12、
PECMD_AllocString(0x140063720)×4、PECMD_MatchTokenAdvance(0x1400660ac)×2、
PECMD_AsciiPrefixICmp(0x14005c788)×1、PECMD_SplitTokenTrimWs(0x1400675b8)×4、
PECMD_ParseShortStore(0x1400679b0)×1、PECMD_ParseUIntValue(0x140074838)×2、
PECMD_SetVariableWithPrefix(0x14007d0ac)×7、PECMD_WideToAnsiStr(0x14006355c)×8、
PECMD_AllocSmallObject(0x140063344)×2、PECMD_Base64Decode(0x14006ae00)×4、
PECMD_StrBldCopyAnsi(0x1400702f0)×4、PECMD_InitStrContainer(0x140070294)×1、
PECMD_GuidToString(0x14005fc90)×2、PECMD_GrowCapacityAligned(0x1400e6cf8)×2、
PECMD_AnsiStrNCompare(0x14005b184)×3、PECMD_MemMoveForward(0x14001d78c)×1、
PECMD_TlsLogWrite(0x140018d8c)×1、FUN_1400195F0(0x1400195f0)×1、
FUN_14001E69C(0x14001e69c)×1、FUN_1400637DC(0x1400637dc)×2、
PECMD_LoadRasApi/HasRasDialApi/DestroyEventSlots/FindRasConnection/
EnumRasConnections/EnumPhonebookEntries/RasDialEntry（RAS 族, 既有 extern）、
WLAN 槽 10 个（g_pWlanOpenHandle..g_pWlanFreeMemory, 本文件原有 extern + pecmd_globals.h）、
g_szEmpty(=DAT_14011c638)、g_Script(=DAT_14013d130)、g_u8CCB1(=DAT_14013ccb1)、
g_hSyncEvtA/g_hSyncEvtB/PTR_FUN_14012b1c0（本文件新增 extern, 定义已在工程）。

**extern 新增（本文件声明, 定义均在工程内）**: PECMD_GuidToString、
PECMD_GrowCapacityAligned、PECMD_AnsiStrNCompare、PECMD_MemMoveForward、
PECMD_TlsLogWrite、FUN_1400195F0、FUN_14001E69C、FUN_1400637DC、
PECMD_InitStrContainer、DAT_1401210f8、PTR_FUN_14012b1c0、g_hSyncEvtA、g_hSyncEvtB。

**TODO(verify) 登记**:
1. `DAT_1401210f8`（dc L"\r\n"）工程现挂零桩（unimplemented_stubs.c:460）— 行分隔前缀
   运行时为空串。与 g_wsz21014 等 .rdata 关键串同属全工程 data-stub 事项, 非本 TU 可修;
   建议主代理在 data 回填批次统一处理。
2. `FUN_1400637DC` 4 参调用形态沿用 core_b3l.c/core_main.c 既有写法; 链接落在
   core_exec5.c 的 3 参定义（第 4 参 codepage 被忽略, 等效 CP_ACP）— 全工程既有行为,
   非本轮引入; dc 97320 的 0xfde9(UTF-8) 语义受此影响, 与 core_b3l.c:891 同案。
3. `FUN_1400195F0`/`FUN_14001E69C` 沿用工程现行大小写拼写（核心定义处为准）。

## 7. 纯度自检数据

| 项 | dc | msvc 新体 | 判定 |
|---|---|---|---|
| 体行数 | 859（96610-97468） | 957（2032-2988） | ≥776(=859×0.9) ✓ |
| if | 104 | 105（104 同构 + 1 拆分, §4） | ≥95 ✓ |
| while | 10 | 10 | ✓ |
| for | 1 | 1 | ✓ |
| goto | 24 | 24 | ✓ |
| 调用点 | 205 | 205 全对表（§6 逐名计数; CONCAT×16 按惯例展开为位运算; WSTR×21 为宽字面量非调用; DWORD×2 为换型类型字） | ✓ |
| 花括号配平 | — | 深度 0, 无负 dive, 无未终止串/注释 | ✓ |
| 注释 `*/` 纯度 | — | 注释状态机扫描: 全部注释正常终止 | ✓ |

## 8. 风险与后续（交主代理）

1. 本轮未构建。请主代理跑 `tools/build_msvc.sh`（语法门先行）双绿后按 V-Gate 流程
   `make_symsnap` + 部署; 新增符号若有 LNK2019 应优先核对 §6 清单拼写。
2. D-18 复核: 本直移体分支数回到 dc 同构, 建议在 `docs/divergences.md` 注销 D-18
   并挂接本报告（该文件本轮未动）。
3. 行为对拍建议优先覆盖: `ADSL,,-wlan*`（RAS 半）与 `-wlan` query/queryall/list/list+
   （清单格式化、含两处恢复的变参格式）, stop/scan/connect/profile（WLAN 半）。
4. 构建身份绑定: 本报告基于 worktree 当前未提交状态; RVA/VA 证据均相对
   `reference/PECMD.exe` 原始基址 0x140000000, 与部署件无关。
