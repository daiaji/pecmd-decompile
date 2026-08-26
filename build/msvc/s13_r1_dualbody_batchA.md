# S13-R1 双体 batch-A 逐对对照报告（A 组·一批）

- 权威语义源: `decompiled.c`（dc:<行号>）。对照对象: `unimplemented_stubs.c` 空桩 ↔ `refactored/src/**` 真体，逐对与 dc 原文核对分支结构 / 常量 / 调用链 / 返回类型 / 局部槽用法。
- 对列表来源: `s13_r1_dualbody_scan.md` A 组「批次=一批」各行，按表序编号 [001..134]（含「特例」「核对」「1参」标注行）。
- 结论三选一：MATCH（语义等价）/ DEVIATION（实质偏差+影响+最小修复建议）/ UNVERIFIABLE（证据不足+缺什么）。
- 本轮范围: **[001..040]**。剩余 [041..134] 由主代理续派。
- 只读取证：本文件为唯一写入产物；未改任何代码；未触碰 git。

## 对照结果表

| 序号 | 函数名(桩↔真体) | dc锚点 | 两侧位置 | 结论 | 证据摘要 |
|---|---|---|---|---|---|
| 001 | FUN_140103020 ↔ PECMD_WideStrLen | dc:159137 | stubs:38 / core_b9_remaining.c:629 | **DEVIATION** | 配对确证(RM json:590)。分支结构一致(单循环走到 NUL)；偏差在算术：dc do-while 越 NUL 后 `(差>>1)-1`=真实长度(dc:159144-159148)，src while 停在 NUL 却仍 `-1`(c:632-636)=长度-1，空串返 -1 而 dc 返 0。影响：35 个调用点全部少 1。最小修复：`return (int64_t)(p - s);` |
| 002 | FUN_1400170b0 ↔ PECMD_SkipLeadingControls | dc:12147 | stubs:39 / core_b1_remaining.c:5796 | MATCH | 循环条件逐字等价 `(8<*p && *p<0xe) \|\| *p==0x20`(dc:12152 ↔ c:5801)，推进写回 `*pp=p+1` 与返回 pp 一致(dc:12153-12155 ↔ c:5802-5804)；返回类型 undefined8*/uint64_t* 同宽透传兼容 |
| 003 | FUN_140070044 ↔ PECMD_AllocAnsiString | dc:70272 | stubs:40 / core_exec2.c:95 | MATCH | 逐句对应：lstrlenA→n+1(dc:70280-81↔c:97-98)、HeapAlloc(n+9) 失败经 FUN_1400630D0(2) 以 r==4 重试(dc:70282-86↔c:101-106)、头写 0xaa55@+4 与 len@+0(dc:70287-88↔c:107-108)、复制后返回 +1(=基址+8B)(dc:70289-90↔c:109-110)。DAT_14013d328≡g_hHeap(pecmd_defs.h:33)；dc 复制体 FUN_14001d78c(dc:17349)=升序逐字节拷贝，对全新堆块与 memcpy 等价 |
| 004 | FUN_1400678f0 ↔ PECMD_ParseSignedNumberStr | dc:64038 | stubs:41 / core_b3_remaining.c:13656 | MATCH | 全结构逐句同构：跳过循环(dc:64049↔c:13667)、'-' 号 0x2d→负标志(dc:64056-58↔c:13674-76)、hex 前缀 `0`+`(|0x20)==0x78`(dc:64061↔c:13679)、hex 调 PECMD_ParseHex_4a34≡FUN_140064a34、dec 范围测试 `9<(u16)(*p-0x30)` 调 FUN_140064A88 同名(dc:64063/66-68↔c:13681/84-87)、写回+条件取负+返 int(dc:64070-75↔c:13689-94)。返回类型/参数布局一致 |
| 005 | FUN_1400284d4 ↔ PECMD_CreateVariableNode | dc:24330 | stubs:42 / core_b2a.c:477 | MATCH | 槽分配 FUN_140063620≡PECMD_AllocStrSlot(RM json:1092)(dc:24338↔c:481)、type=3 写 +8 / 清零 +16(dc:24339-40↔c:482-83)、跳前导控制符(=002 已证等价,dc:24341↔c:484)、FUN_14007034c 赋串(dc:24342↔c:485)、0x2c 截断推进(dc:24344-49↔c:486-91，src 省略 dc 死赋值 sVar1 等价)、FUN_1400c11c0 解析入 +8(dc:24350↔c:492)。返回 node 一致 |
| 006 | FUN_140062950 ↔ PECMD_GetDpiCached | dc:60096 | stubs:43 / core_exec.c:242 | MATCH | 缓存判定+懒加载逐字等价：`if(DAT_14013d868<1) =FUN_140062900(...)`(dc:60099-60100↔c:244-245)；g_dpi≡DAT_14013d868(pecmd_globals.h:413)；被调 FUN_140062900(dc:60077-60134 GetDC→GetDeviceCaps(0x5a)→条件 ReleaseDC) 与 c:232-239 PECMD_GetDpi 同构。附注(非本对偏差)：unimplemented_stubs.c:799 仍独立定义 `void*DAT_14013d868`，与 uint32_t g_dpi 存在分裂符号/类型冲突风险，转发前宜按 T1b 先例归一 |
| 007 | PECMD_WaitHandlesOrMessages ↔ FUN_1400195F0 | dc:14289 | stubs:61 / core_thread.c:238 | **DEVIATION** | 主干全同构：param4 解包(hwnd/opts/handles)、nCount=opts&0xffff、deadline、Sleep(0) 早退、opts&0x20000→timeout=10000000、嵌套限幅 `>100→100;>1000→1000`(dc:14341-45↔c:286-291 同保怪序)、MsgWait(...,0x4ff)、PeekMessage 泵+maxmsg 递减、r==0x102 清 maxmsg、尾部 nCount==0→0 / r==nCount→0x102；g_flagA24F≡DAT_14013a24f(pecmd_defs.h:116)。偏差一点：MsgWait 失败且有句柄时 dc 映射 `err==0→返1`(dc:14349-54+14393-95)，src 直接 `return GetLastError()`(c:294-297)——err=0 时 src 返 0 会被调用方误读为句柄0已触发。最小修复：`DWORD e=GetLastError(); return e?e:1;` |
| 008 | PECMD_AddVarDefault ↔ FUN_14001E5B0 | dc:18078 | stubs:68 / core_var2.c:224 | MATCH | `::` 前缀跳转循环(逐迭代查空返 NULL、caplen<0 时 lstrlenW*2 重算、命中切表 g_Script≡DAT_14013d130 pecmd_defs.h:117)(dc:18088-101↔c:229-242)；节点构造 FUN_1400188ec≡PECMD_NewVarNode(RM:214；dc:13557-64↔c:98-104，内含 FUN_140070330≡PECMD_VarWriteNodeValue RM:230)；值容量写入 FUN_140066224≡PECMD_VarWriteValueCap 其体内已 `*pcap=len`(dc:62923↔c:91)，dc 调用点冗余补写 [3](dc:18107) 被省略属等价化简；计数+8/realloc `count<<5`/尾槽写/dc 返 node 全对应(dc:18108-12↔c:249-53)。operator_new→calloc 为库替换(多清零无害) |
| 009 | PECMD_FindVarValue ↔ FUN_14001E69C | dc:18120 | stubs:69 / core_var3.c:31 | MATCH | 单一包装：查节点后 +8 取 value 字段、未中返 NULL(dc:18125-29↔c:33-36)；被调 FUN_140018978≡PECMD_VarLookup(c:25 extern 注记 @0x140018978)；返回 longlong↔uint8_t* 同宽，桩 4 参透传相容 |
| 010 | PECMD_SetVarCore ↔ FUN_14001E6BC | dc:18137 | stubs:71 / core_var2.c:257 | MATCH | 全分支同构：负 caplen 解码先于前缀处理(dc:18157-60↔c:273-79)、`&` 计数(18161-63↔280-83)、空名早退、amp>1 回根表、`::` 分支含 scope 覆盖序与 `-1<caplen` 减 2(18171-79↔291-301)、lookup 用原串(18181↔303)、`len<1` 重算(18182-85↔305-07)；未命中：`::`→FUN_14001e5b0(&g_Script)(=008 已证体，18188↔310)，否则 new/NewVarNode/补写[3]/realloc(count+1)<<5/槽写/count++(18191-99↔313-22)；命中：flag=node[0x1f]&0xc0，==0xc0→FUN_14005b708≡PECMD_VarTruncateUpdate(RM:218)，否则 flag≠0 清[8]+AllocString(len+2)+拷贝(FUN_14001d744≡MemMoveSafe→memmove)+NUL+[0x18]=len(18203-14↔326-37) |
| 011 | PECMD_SendHotkeyKeyMessage ↔ FUN_140035B40 | dc:31523 | stubs:77 / core_b2c.c:1114 | **DEVIATION** | 主体同构：AllocSlot(0x24)+ZeroLenBuf≡FUN_14005b0b8、头写 1/0x14/2/`1<<((k\|0x20)-0x61&0x1f)`/uVar3<0x1a 守卫/repeat*5、三连 PECMD_StartWorkerThread≡FUN_140027eac(core_exec.c:343 注记) 传参全对应；g_b21728≡DAT_140121728(pecmd_globals.h:278)。偏差：dc 第二次派发读 `&DAT_14012172a`=表基+2(dc:31553-56)，src 读成 `+4`(c:1145,1147) 与第三次重复——6B/项热键表的 +2 槽失效、+4 槽被双发。最小修复：c:1145/1147 的 `+ 4`→`+ 2` |
| 012 | PECMD_SetObjectEnable ↔ FUN_140053C5C | dc:49152 | stubs:94 / core_b3i.c:199 | **DEVIATION** | 骨架一致：名字非空守卫(dc:49158↔c:201)、复制名串+追加 ".Enable"(FUN_14006375c≡PECMD_AppendWideStr RM:1099，dc:49160↔c:204)、复查后选 "0"/"1"(dc:49161-65↔c:205-09)、经 *(obj+0x50) 调 FUN_14007d0ac 设变量(=022 真体，dc:49166↔c:210)、FreeStrBuf≡FUN_14005b104 收尾。**偏差（严重）**：c:203 漏 `&`——`PECMD_CopyStrToSlot(local_res8,…)` 把 NULL 值当槽指针传入，而该函数体内 `*dst=…`(core_b3a.c:362≡dc:70490) 写穿空指针→带名对象一进即 AV。最小修复：改为 `PECMD_CopyStrToSlot(&local_res8, (uint64_t *)(obj + 0x10));` |
| 013 | FUN_140053e78 ↔ FUN_140053E78 | dc:49242 | stubs:95 / core_sys.c:144 | MATCH | 特例核实：dc 体 size=3 纯 `return`(dc:49242-49246)，移植体同为空函数(c:144-147)；转发无行为差异，与 pecmd_defs.h PECMD_Empty 登记一致 |
| 014 | PECMD_DestroyTrayIcon ↔ FUN_14005B7E8 | dc:54074 | stubs:96 / core_exec3.c:196 | MATCH | 锁内清理逐句同构：g_csInit≡DAT_14013e190(T1b 别名注记)、有活动标志才 NIM_DELETE(Shell_NotifyIconW(2,nid+0x10))、清标志、memset(nid+0x10,0,0x3d0)(FUN_140102a90≡memset)、重建 cbSize 头字节 d0 03 00 00(dc:54077-89↔c:198-208) |
| 015 | PECMD_RegDeleteValue ↔ FUN_14005C61C | dc:54844 | stubs:98 / core_b3f.c:170 | MATCH | 短路逻辑逐句同构：sub==NULL 跳开键、否则 FUN_14005C394(…,2,4) 开键成功(DVar1==0)才 RegDeleteValueW、句柄非空才 RegCloseKey、返 DWORD(dc:54850-56↔c:173-179)；src 多做的 4 槽清零无害 |
| 016 | PECMD_AsciiPrefixICmp ↔ FUN_14005C788 | dc:54955 | stubs:100 / core_string.c:187 | **DEVIATION** | 循环体逐字一致 `(c>0x40 \|\| w==c) && ((c\|0x20)==(w\|0x20))`(dc:54962-74↔c:189-197)。**偏差**：dc 全 n 字符匹配后无条件返 1(dc:54963-65)；src 额外要求 `w[n]` 为空白/结束才返 1(c:199-202)——这实为姊妹函数 FUN_14005c72c 的语义(dc:54930-37，含分隔符校验)，两体被合并混装。影响：130 个调用点中"前缀后跟非分隔符"的分支(如 "--ppid:123"、"EXECUTE")由匹配变不匹配。最小修复：FUN_14005C788 去掉尾部分隔符检查直接返 1，并把现检查体另立为 FUN_14005C72C(全树尚无该真体) |
| 017 | PECMD_AsciiWideICmp ↔ FUN_14005C7C4 | dc:54983 | stubs:101 / core_exec5.c:156 | **DEVIATION（严重）** | 双终止判定与推进一致(dc:54990-98↔c:158-166)。**逻辑符错**：dc 循环继续条件为 `A && B`(A=`c>0x40‖u==c`, B=`(c\|0x20)==(u\|0x20)`，失配即退出返 0，dc:54999-5002)；src 写成失配返回条件 `!A && !B`(c:167-168)——等价于「只要 A、B 之一成立就当匹配继续」。字母位(c>0x40)恒使 A 真→任何字母处失配都被跳过，"AB"↔"AC" 竟判相等。影响：44+24 个调用点的全串比较大面积误判。最小修复：c:167 的 `&& !(` 改 `\|\| !(`（即返回条件改为 `!A \|\| !B`） |
| 018 | PECMD_DialogBeepNotify ↔ FUN_14005D9A8 | dc:55959 | stubs:106 / core_exec.c:280 | **DEVIATION** | 位守卫(bit15 早退、mode==0 时 bit13 抑制、非 0/0x10 直跳 beep)、列表循环框架、样式掩码 `(st&0x18010000)==0x10010000`、`|=0x2000` 后六连合成键(dc:55977-82↔c:317-323)全对应(PECMD_SynthDlgKeyMsg≡FUN_14005d968 core_exec.c:265 注记)。**偏差（少一层解引用）**：dc 链为 `obj2=*(elem+0x38); hwnd=*(obj2+0x20)`(dc:55989-92)，src 直接 `wnd=*(elem+0x38)`(c:301-302)——把中间对象指针当 HWND 用，GetWindowLongW(GWL_STYLE) 测错目标，提示音抑制逻辑(mode 0/0x10 路径)失真。最小修复：c:302 改两级取值 `o2=*(void**)(p+0x38); wnd=o2?*(HWND*)((u8*)o2+0x20):NULL` 并保留 o2 非空检查 |
| 019 | PECMD_ExpandDrivePath ↔ FUN_14006459C | dc:61516 | stubs:118 / core_exec2.c:179 | **DEVIATION（轻微）** | 分派守卫与 dc 完全一致(含 `首字符≥0x7b && "X:" 两字符形` 才走盘符构造的反直觉阈值，dc:61524↔c:182)；cwd 首字符匹配→(len+1)>4 时容量不足返 len+1、否则拷贝含 NUL 返 len(dc:61533-42↔c:189-197)；容量<4 返 4、写 `X:\\` 返 3(dc:61544-53↔c:199-205)。**偏差**：dc 缓冲 264 元素可容纳 GetCurrentDirectoryW(0x104) 的最多 261 字符写入(dc:61522)，src 只开 `WCHAR cwd[0x104]`=256(c:181)——长 CWD(256..260 字符)时栈越界 4 wchar。最小修复：`WCHAR cwd[0x110];` |
| 020 | FUN_140064a34 ↔ PECMD_ParseHex_4a34 | dc:61738 | stubs:119 / core_b3e.c:449 | MATCH | 十六进制解析逐字同构：数字/a-f(A-F) 判定 `(0x2f<*s<0x3a)‖(0x60<(*s\|0x20)<0x67)`、`<0x3a→-0x30 否则 (\|0x20)-0x57`、acc*16+v(dc:61746-56↔c:452-461)；返回 int64 一致 |
| 021 | PECMD_EncodeStringId ↔ FUN_14006B1E8 | dc:66859 | stubs:120 / core_b3g.c:438 | MATCH | 四步链一致：AllocSmallObject(RM:224)、WideToAnsi(-1,0xfde9=CP_UTF8)(RM:727≡FUN_14006355c，=169 真体)、Base64Encode(RM:326≡FUN_14006af70)、FreeStrBuf 后返结果(dc:66865-69↔c:441-445) |
| 022 | PECMD_SetVariableWithPrefix ↔ FUN_14007D0AC | dc:79622 | stubs:140 / core_b3l.c:995 | MATCH | `#&` 头消耗(sVar2=0x23)、环境直写三条件(script+0xd&0xf==0、script[8]==0‖*(script[8]+0x128)!=0x26、非'&')与 FUN_14005d534 调用(RM:1038，=core_var2.c:344 体)全对应(dc:79631-41↔c:998-1009)；`&`/`&&` 前缀决策表逐分支一致(有#&：'&'→直写否则补"&"，无#&："&&"→直写否则补"&&"/"&")(dc:79644-56↔c:1015-35)；StrBldCopyWide+AppendWide 拼名(RM:1247)、[0x18] 父链重定向、g_csInit 锁内 FUN_14001e6bc(...,-1)(=010 已证体)、FreeStrBuf(dc:79657-67↔c:1037-49) |
| 023 | PECMD_SetCheckVariable ↔ FUN_14007DF90 | dc:80332 | stubs:142 / core_b3i.c:684 | **DEVIATION** | 结构同构：名字非空守卫、FUN_14007DE70 拼 `<name>.Check` 入临时(dc:80341↔c:689)、复查后按 mode 选 "0"/"1" 并经 *(ctx+0x50) 调 FUN_14007d0ac(=022 已证体)(dc:80344-49↔c:692-697)、双 FreeStrBuf 收尾(dc:80343/51↔c:691/699)。**偏差（严重，与 012 同类）**：c:690 `PECMD_CopyStrToSlot(local_res8,puVar1)` 漏 `&`——NULL 当槽指针传入，体内 `*dst=…`(core_b3a.c:362) 即写空指针→带名对象一进即 AV。最小修复：改 `&local_res8` |
| 024 | PECMD_QueryFontInfo ↔ FUN_1400B8960 | dc:114485 | stubs:146 / core_b3i.c:748 | **DEVIATION** | 骨架一致：lfHeight=0 预置、GetObjectW(hFont,0x5c,&lf)、调 FUN_1400b2218≡PECMD_CreateFontAdjusted(RM:1335；dc:110701-17↔core_b3f.c:561-73 体逐句同)。**偏差（缺 LOGFONT 预处理）**：dc 在调用前 memset 结构尾 0x58 字节、预置 weight=400(仅 GetObjectW 失败时生效)，调用后清 +4 处 4 字节(lfWidth)、置一字段字节=1(dc:114493-98)；src 全部省略(c:750-752)。影响：源字体字段非默认时产出不同字体(宽度/质量/回退粗细)。最小修复：按 dc 原序补 memset→weight=400→GetObjectW→width=0→byte=1 五步 |
| 025 | PECMD_GetWindowTextAlloc ↔ FUN_1400E5730 | dc:140207 | stubs:152 / core_b8d.c:79 | MATCH | 四步一致：GetWindowTextLengthW→AllocString(len+2)(FUN_140063720≡PECMD_AllocString，B组验证)→首字符置 0→GetWindowTextW(hwnd,*ps,len+1)(dc:140212-17↔c:81-84) |
| 026 | PECMD_GetOwnerWindow ↔ FUN_1400E5788 | dc:140226 | stubs:153 / core_b8c.c:43 | MATCH | do-while 逐级 GetParent、NULL 返 0、以 `style>>30&1`(WS_CHILD) 决定继续上溯(dc:140231-38↔c:45-51)；返回 HWND 一致 |
| 027 | PECMD_ResDecode ↔ FUN_1400E7D58 | dc:142191 | stubs:155 / core_resdecode.c:52 | **DEVIATION** | 全函数(~300 行 dc:142191-491↔c:52-318)逐分支核对：BOM 探测(FE FF/FF FE/EF BB BF 常量、SwapBytePairs)、`@::`/`#!` 预检、"…#code=N[!tM]" ANSI/WIDE 双路解析、raw 双重解码回退(0→0xfde9→主解码)、postproc 循环(8×key 填充、mode<0x10 扫描、退出条件 mode>0xfff/cpArg<1/cpArg≠g_SysCodePage(pecmd_defs.h:42)/specOut==g_SysCodePage)、ResReenc+StrChrOffset(RM:1490,1543)、mark 恢复 'c'、flags&2 擦除——全一致。**偏差（1 处）**：UTF-8 BOM 路径 dc 置 spec=0xfde9 后经 `goto LAB_1400e7ef3` 共享裁剪代码并因 bVar20==false **落入主解码**(dc:142292-94+142286 fallthrough→142395)，src 却在 skip_prefix 无条件 `goto postproc`(c:108-111+180)——跳过 DecodeEncTextToUtf16 转换，UTF-8 资源按宽字符误读出乱码。最小修复：skip_prefix 记录「是否 UTF-8 BOM」，是则转 main_decode 否则 postproc |
| 028 | FUN_140023640 ↔ PECMD_HotkeyControl | dc:21232 | stubs:169 / core_b2e.c:178 | **DEVIATION** | 全主干核对一致(锁/回调窗(RM:552)/跳空白(=002)/AllocWStringBuffer(0x1813)(RM:713)、`-/*`+delall(FUN_14005c72c≡PECMD_TokPrefixICmp RM:1478，真体 restored_bodies.c:7307 含分隔符校验=dc:54922 体✓)、`?`查询截断、ParseHotkeyCode(RM:304)、SHGetValueW 枚举循环/g_hotkeyIdx≡DAT_14013cccc(pecmd_globals.h:131)/双整数解析/命中 bestValue=valueEnd-dotMode、del 删值/写值(SHSetValueW、SendMessageW 0x44a)、>1000 返 0x80070057、查询路径 FUN_1400629b8 写变量)。**偏差①**：c:253 把 WSTR("Global\\…lock") 传给 LPCSTR 的 PECMD_CreateMutexSlot(core_b1_remaining.c:6011≡dc:12531 LPCSTR)——CreateMutexA 名字在首 NUL 截断成 "G"，跨进程锁名失真；应传 ANSI 字面量。**偏差②**：查询未命中返回值——dc 的 uVar19 仅 {初值1，命中0}(dc:21297/21356)，src 把循环计数 scanIdx 当结果(c:371，循环中持续 ++ c:306)，未命中返 26/10000 而非 1。交叉注记：c:344 继承 001 WideStrLen 少一→SHSetValueW 少写末字符+NUL；c:322 继承 017 比较体逻辑错 |
| 029 | PECMD_RestartSelf ↔ FUN_14007724C | dc:75616 | stubs:172 / core_b3l.c:796 | MATCH | AllocWStringBuffer(0x1cc)→GetModuleFileNameW(0,buf,0x104)、SI 全字段(cb=0x68/lpReserved/memset(lpDesktop,0,0x58)/dwFlags=0x81/wShowWindow=0/XCountChars=800/YCountChars=500)、CreateProcessW(FUN_140101e04 导入包装)、双 CloseHandle、FreeStrBuf(dc:75623-44↔c:798-821)；PI 整体清零为超集无害 |
| 030 | FUN_14000C764 ↔ PECMD_RegisterFileAssociations | dc:6897 | stubs:211 / core_b1_remaining.c:4811 | MATCH | 全函数(~170 行)逐段核对：切首段(0x2c)(RM:858)/去引号/空→GetModuleFileNameW(g_hInst≡DAT_14013cf70 pecmd_defs.h:35)、目录截断+扩展名剥离、9 组模式登记(.exe/.com/.ntr/.cmd/.bat→PTR_PTR_14013a050 长 iLen+1；.pecmdplugin.*.PEI→LoadPluginPecmdTbl(RM:1081，表计数 g_cmdTable1Count≡DAT_14013ca98 pecmd_globals.h:57)；.$*.dll/.dlx→a090 长 iLen+2；.wci/.wcs/.wce/.wcz→a070)、INDATA 资源(FUN_14001ea18+ResDecode=027 真体)按行扫 LOAD: 双 token、"#"前缀命令拼 `'#'+\" \"+token` 双写 g_cmdTable2(DAT_14013a080 pecmd_globals.h:52)+计数(DAT_14013a078 h:51)、'+' 截断、收尾 frees/返 1——全部对应(RM:417/845/1101/1127)。局部槽用法/常量一致 |
| 031 | PECMD_EncodeDet ↔ FUN_14005F33C | dc:57228 | stubs:214 / core_exec2.c:66 | **DEVIATION（语义错位）** | 长度守卫一致(len>12)。**偏差**：dc 是自定义容器头校验——`data[4..6]=="CMP"`(魔数表 @0x140124d00=00 00 00 00 43 4d 50 78…，pe_registry.json:94)不匹配返 0；再按 `data[7]` 分派：'a'→0x14、'A'→8、'x'→4；命中或为 'S' 时 `*(int*)(data+8)!=0` 加位 1(bit1)，返回 `code\|1`(dc:57236-60)；src 却整体写成 BOM 嗅探(FF FE/FE FF/EF BB BF 于 data[0..2])(c:68-77)——结构完全不同的另一套逻辑。影响：全部调用点(dc:18376/18446/29161/64915/64961)编码判定失真。最小修复：按 dc 重写为 CMP 头校验+四分派+bit1 合成 |
| 032 | PECMD_ParseHashNumbers ↔ FUN_1400677B0 | dc:63968 | stubs:217 / core_b3m.c:522 | MATCH | 全结构逐句同构：跳到 ','、'@' 跳过(bAt)、'-' 置负标志、hex 判定+`0x` 前缀、PECMD_ParseHex_4a34(=020 已证体)、写 out[i]、条件取负、'@'→GetSysColor 回写、扫至 '#'、计数上限 `3<i` 提前返(dc:63984-64030↔c:537-585)；返回 int16 一致(cVar9 char↔int 仅类型宽窄) |
| 033 | PECMD_SkipWCharUntil ↔ FUN_1400F429C | dc:149819 | stubs:220 / core_b8h.c:709 | MATCH | 游标推进循环逐句等价：`while(*p && ch!=*p){ p++; *pp=p; }`(dc:149824-29↔c:713-718，含 NULL 槽守卫)；dc 返槽指针但全树调用点均语句式忽略返回值，src 归 void 无行为差 |
| 034 | PECMD_InitDragDrop ↔ FUN_1400549BC | dc:49835 | stubs:222 / core_b3j.c:126 | MATCH | 五步一致：fontObj 非空时 FUN_1400E5890 取字体+vtable+0x108 回调、FUN_140053C5C(=012 真体，注意其内部 & 缺陷随体继承)(dc:49844↔c:137)、PECMD_SetObjectVisibleVar≡FUN_140053cec(RM:705)、DragAcceptFiles(*(obj+0x38)+0x20,1)、名非空调 FUN_14007D0AC(=022 已证体) 写 *(obj+0x10)→*(obj+0x20)(dc:49846-49↔c:139-142)。交叉注记：本对经 012 体间接继承 CopyStrToSlot 漏 & 崩溃点 |
| 035 | PECMD_IsSetupClass ↔ FUN_14006643C | dc:63000 | stubs:225 / core_b3j.c:496 | MATCH | 逐句同构：SetupDiOpenClassRegKey(guid,0x20019)→RegQueryValueExA("Class",type=1,size=0x32,data[64])→RegCloseKey→lstrcmpiA 相等返 1 否则 0(dc:63010-20↔c:501-513)；返回 uint32 一致 |
| 036 | PECMD_SetControlState ↔ FUN_14007E01C | dc:80361 | stubs:228 / core_b3m.c:1143 | MATCH(含黑箱注记) | 全结构逐句同构：state!=0xf8000009 才写 +0x70、'#' 标题解析(FUN_140074838≡PECMD_ParseUIntValue，负值钳 0x80000000 写 inner+0xe0)、StrBldCopyWide(RM:1247) 标题、nCmdShow 0/5、ShowWindow、state<0x65→SendMessageW(0x402)、title 非空→InvalidateRect、SetVariableWithPrefix 回写(=022 体)。注记：dc:80411 `wsprintfW(local_58,L"%ld")` 变参寄存器残留静态不可定案（同 PECMD_FormatSetVar 先例），src 按 obj+0x70 补参属最合理近似，登记 [RESOLVABLE-BLACKBOX] |
| 037 | PECMD_UpdateWindowStyleBits ↔ FUN_1400E5900 | dc:140322 | stubs:234 / core_exec.c:222 | MATCH | 样式位更新逐字等价：`old=GetWindowLongPtrW(hwnd,-0x10); neu=(~clear&old)\|(set&0xffffffff); SetWindowLongPtrW; return old!=neu`(dc:140328-31↔c:224-227)；bool 返回一致 |
| 038 | FUN_1400ec698 ↔ FUN_1400EC698 | dc:144744 | stubs:235 / core_b8a.c:196 | MATCH | 槽交换三步一致：读旧值/写新值/返旧值，偏移 +0xf0≡OBJ_SUBWND(pecmd_offsets.h:38)(dc:144749-51↔c:198-200) |
| 039 | PECMD_LayoutChildWindows ↔ FUN_1400EC880 | dc:144841 | stubs:237 / core_b8j.c:190 | **DEVIATION** | 骨架一致：mode→`*(obj+0xd0)\|=0x80`(OBJ_LINK=0xd0 pecmd_offsets.h:34)、count@+0x118/数组@+0x108、广播循环 SendMessageW(hwnd,0x133e,i,&struct)、第二项起 SetWindowPos(…,0x97)、首项 SetWindowPos(…,0x43)、FUN_1400ec7c0≡PECMD_ShowFirstTabPage(RM:1311) 收尾。**偏差（缺字段填充）**：dc 每次广播前 `local_38 = *(child+8)` 填入消息结构 +0x10 槽(dc:144866)，src 只置 local_48=1/local_40=0 后 memset(c:208-211)，+0x10 槽从未写入——每个子页的关联值传不出去（恒为栈残留/0）。最小修复：补一个 `uint64_t local_38;` 紧随 local_40 并赋 `local_38 = *(uint64_t *)(*scan + 8);` |
| 040 | PECMD_CreateDateTimePickCtl ↔ FUN_1400F1378 | dc:147646 | stubs:243 / core_b8g.c:520 | MATCH | 七参创建链一致：rect 展开、GetWindowLongW(parent,-6=HINSTANCE)、CreateWindowExW(0,"SysDateTimePick32",NULL,style\|0x40000000,…,(HMENU)id,inst,NULL)、obj[4]=hwnd、FUN_14006b8fc≡PECMD_ContainerAppend(A组#107 真体 core_b3b.c:330)、返 obj[4]!=0(dc:147656-66↔c:522-534) |

## 本轮汇总（[001..040]）

| 结论 | 计数 | 序号 |
|---|---|---|
| MATCH | **26** | 002 003 004 005 006 008 009 010 013 014 015 020 021 022 025 026 029 030 032 033 034 035 036* 037 038 040 |
| DEVIATION | **14** | 001 007 011 012 016 017 018 019 023 024 027 028 031 039 |
| UNVERIFIABLE | **0** | — |

\* 036 为 MATCH + [RESOLVABLE-BLACKBOX] 注记（wsprintfW 变参寄存器残留，src 按 obj+0x70 补参）。

### 按严重度分层（转发前须先修复的移植体缺陷）

- **崩溃级（转发即 AV / 锁失效）**
  - 012 `core_b3i.c:203`、023 `core_b3i.c:690`：`PECMD_CopyStrToSlot(local,…)` 漏 `&` → NULL 写。
  - 028① `core_b2e.c:253`：WSTR 传给 LPCSTR 的 CreateMutexSlot → 互斥名截断成 "G"。
- **大面积语义错**
  - 001 WideStrLen 少 1（35 调用点连带：028 写值路径少写末字符+NUL）。
  - 016 FUN_14005C788 混入 c72c 分隔符校验语义（130 调用点；全树缺 FUN_14005C72C 真体）。
  - 017 AsciiWideICmp 返回条件 `!A&&!B` 应为 `!A||!B`（44+ 调用点误判相等）。
  - 027 ResDecode UTF-8 BOM 路径跳过主解码。
  - 031 EncodeDet 整体逻辑错位（应为 "CMP" 容器头校验，现为 BOM 嗅探）。
- **局部行为偏差**
  - 007 MsgWait 失败 err==0 时应返 1；011 热键表第二槽 +4 应为 +2；018 少一层解引用；019 cwd 缓冲小 4 wchar；024 缺 LOGFONT 五步预处理；028② 查询未命中应返 1；039 广播结构缺 *(child+8) 填充。
- **联动登记**：006 发现 unimplemented_stubs.c:799 仍独立定义 `void*DAT_14013d868` 与 uint32_t g_dpi 分裂符号（转发前宜按 T1b 先例归一）；033/153 同 VA 双拼写桩共享上述结论。

### 方法与覆盖说明

- 对列表 = 扫描报告 A 组「批次=一批」全部行按表序编号 [001..134]；本轮完成前 40。
- 每对均打开三处原文：空桩（unimplemented_stubs.c）、真体（src/**）、dc 权威体（decompiled.c）；被调映射一律经 rename_map.json / pecmd_defs.h / pecmd_globals.h / pecmd_offsets.h 现场核证后计为证据。
- 大函数覆盖度：027/030 为全文逐段核对；028 为全主干+关键分支全覆盖（枚举循环/三分支收尾逐行）；其余中小函数逐句核对。凡静态不可定案处（变参寄存器残留）按「不虚构语义」登记黑箱注记而非臆断。
- 本文件为本轮唯一写入产物；未改任何代码；未触碰 git。

### 待办移交

剩余 [041..134] 共 94 对留待主代理下轮 send_message 续做（041=扫描#67 PECMD_InitControlObjC ↔ core_b8d.c:428 起）。建议下轮优先复核与本轮缺陷同族的调用点（CopyStrToSlot 全树实参、WSTR→ANSI 槽函数、比较族双体拆分）。

---

## 对照结果表（续）：第二轮 [041..090]

方法与纪律同第一轮；每对完成即追加。汇总见文末第二轮小结。

| 序号 | 函数名(桩↔真体) | dc锚点 | 两侧位置 | 结论 | 证据摘要 |
|---|---|---|---|---|---|
| 041 | PECMD_InitControlObjC ↔ FUN_1400FBE58 | dc:154696 | stubs:246(现文件:260) / core_b8d.c:428 | MATCH | 六步一致：FUN_1400E57C0 基初始化、obj[0x1a]=p2、虚表 PTR_FUN_14012c900、三处字节写 0xa0/0x61=0x11/0xb8——注意 dc 混用指针算术(`+0x14` 元素=+0xA0 字节)与整数算术(`+0x61`)，src 全按字节偏移正确落地(dc:154699-704↔c:430-435)；返回 obj 一致 |
| 042 | PECMD_CreateStaticControlFromRect ↔ FUN_1400FD318 | dc:155553 | stubs:248 / core_b8d.c:470 | MATCH | RECT 拷贝(16B)后转调 FUN_1400FD220(obj,0,text,style,&rc,parent,id)，参数原序透传(dc:155560-62↔c:472-473) |
| 043 | PECMD_InitStaticCtl ↔ FUN_1400FE130 | dc:155954 | stubs:249 / core_b8d.c:479 | MATCH | 四步一致：FUN_1400FCF44 基初始化、byte+0xa4=3、虚表 PTR_FUN_14012cde0、obj[7]=GetStockObject(5=NULL_BRUSH)(dc:155959-63↔c:481-484)；返回 obj 一致 |
| 044 | PECMD_CreateProgressCtl ↔ FUN_1400FC060 | dc:154776 | stubs:251 / core_b8g.c:892 | MATCH | GetWindowLongW(parent,-6)→CreateWindowExW(0,"msctls_progress32",NULL,style,…,(HMENU)id)→obj[4]→PECMD_ContainerAppend≡FUN_14006b8fc(=075 真体)→返 obj[4]!=0(dc:154782-88↔c:894-902)；与 040 同构族 |
| 045 | thunk_FUN_140064b78 ↔ PECMD_FreeInitObjectList | dc:61823 | stubs:254(现:268) / core_b3_remaining.c:12508 | **DEVIATION（漏释放）** | 锁内全流程逐字同构：thunk_FUN_14005b7e8≡PECMD_DestroyTrayIcon(=014 体)、倒序遍历 +8 数组(计数 int@+4)、元素首字节==0x80 时 `free(*(elem+0x10))+清 8 字节`、PECMD_FreeResourceObject≡FUN_14005b888(elem,1)(=053 真体)、槽清零、param_1[4..15] 共 12 字节清零(dc:61830-68↔c:12515-53)。**偏差（错函数）**：收尾 dc 调 `FUN_14005b104`=FreeStrBuf 释放数组(dc:61869)，src 误写 `FUN_14005B154`(=跳空白助手，c:12554)——数组内存泄漏、无任何释放效果。最小修复：改 `PECMD_FreeStrBuf((WCHAR **)&local_res8);` |
| 046 | PECMD_DispatchMessageMap ↔ FUN_1400F172C | dc:147812 | stubs:258(现:272) / core_b8m.c:480 | MATCH | 全函数(size=1207，~150 行)逐段核对：0x28 步长表匹配(hwnd@+0/mode=flags&0x93/msg@+8、flags&0x60 细化：0x111‖0x100 用 wParam 低/高 16 对 +0x20/+0x1c，0x4e‖0x200 用 lParam[1]/[2])、命令串@+0x10 复制、`:` 变量前缀(SplitTokenTrimWs+FUN_14007bf44 展开)、WM_COMMAND/NOTIFY 内建变量四连(FUN_140066930≡FormatSetVar core_b3e.c:472 同 VA、FUN_1400668ec≡AppendFmtValue RM:1242)、`,` 第二 token、`-` 父链游走(byte@+0x122&0xf0==0x10)、`<` 临界区 g_msgLockCount≡DAT_14013d080(pecmd_globals.h:100)+bit1 置清、PECMD_ProcessScriptBlock≡FUN_14004c0bc(RM:1143)、*out 回写、`extra==0‖0x1060` 早退、四缓冲释放——全对应。src 多的 NULL 串守卫为无害超集 |
| 047 | PECMD_MessageHookProc ↔ FUN_14005DF74 | dc:56175 | stubs:261(现:275) / core_b3h.c:277 | MATCH | 钩子链一致：nCode<0‖回调空‖vtable 调用(0x44f)返 0 → CallNextHookEx(g_hHook2≡DAT_14013d3c8 pecmd_globals.h:139) 否则返 1；g_pHookCb3d0≡DAT_14013d3d0(h:398)(dc:56181-87↔c:279-284) |
| 048 | PECMD_CloseThreadSlot ↔ FUN_14005E310 | dc:56388 | stubs:263(现:277) / core_b3f.c:213 | MATCH | 五步一致：slot 空守卫、句柄非空→锁内 g_i64CCB8≡DAT_14013ccb8(pecmd_globals.h:173) 自减、取句柄清槽 CloseHandle、HeapFree(g_hHeap, slot-1)(dc:56393-402↔c:215-224) |
| 049 | PECMD_DestroyStaticControl ↔ FUN_1400FD1A8 | dc:155496 | stubs:264(现:278) / core_b8f.c:790 | MATCH | 析构链逐句同构：虚表 PTR_FUN_14012ccc0、DestroyIcon(+0x1c)并清槽、DeleteObject(+0x1d)清槽、DeleteObject(+0x10/+0x11)、基类析构 FUN_1400E8940(dc:155499-514↔c:792-807) |
| 050 | PECMD_ReleaseSlotObject ↔ FUN_1400E56E4 | dc:140174 | stubs:265(现:279) / core_b8a.c:95 | MATCH | 槽释放四步一致：取对象、清槽、非空调虚表 +0x18(obj,1)(dc:140179-83↔c:97-102) |
| 051 | PECMD_ControlWindowProc ↔ FUN_1400E5B0C | dc:140453 | stubs:266(现:280) / core_b8m.c:179 | **DEVIATION** | 全函数(size=2006，25+ 消息分支)逐分支核对：0x202/0x2c/5/0x10/0x14(WM_PRINT 全块含圆角区双建+FrameRegion≡FUN_14005d600 RM:118)/0x20/0x2b/0x83/0x84/0x113/0x200/0x201/0x453/0x204/0x205/0x448/0x449/0x44c(g_hActiveDevWnd≡DAT_14013e400 core_b8m.c:171+0x2a3)/0x452/0x459(=PECMD_CtlForwardSetBlock core_b8a.c:111 @0x1400e5aac)/0x45b(0x61==2、+0x110、float 写 +0x50..+0x5c、SetWindowText、0x60)/0xbc19/0xbc2b/0xbc2c 与默认转发——虚表偏移与 ×4 字段换算全对。**偏差（1 处）**：msg==0x85 分支 dc 调 `vtable+8`(即槽 1)(dc:140597)，src 写成 `vt[8]`=+0x40(c:310)——调错虚表项。最小修复：`vt[8]`→`vt[1]` |
| 052 | PECMD_CleanupParser ↔ FUN_140073C58 | dc:73207 | stubs:270(现:284) / core_b3h.c:526 | MATCH | 锁内五步一致：PECMD_ReleaseWindowHooks(≡FUN_14006703c，A组#196 真体)、+400(0x190) 非空调 PECMD_ReleaseObjectListTail(≡FUN_140066eac，#207) 传 0、HeapFree(g_hHeap, ptr-8)、清 +400/+0x198(dc:73210-22↔c:528-538) |
| 053 | PECMD_FreeResourceObject ↔ FUN_14005B888 | dc:54114 | stubs:271(现:285) / core_b3h.c:187 | MATCH | 资源析构逐句同构：DeleteObject(OBJ_BRUSH=0x38)清槽、FreeLibrary(+0x40)清槽、FreeStrBuf×4(0x30/0x18/0x10/8)、flags&1 时 free(obj)、返 obj(dc:54117-32↔c:189-204) |
| 054 | PECMD_DriveTypeName ↔ FUN_14005FF30 | dc:57824 | stubs:282(现:296) / core_b3k.c:173 | MATCH(含黑箱注记) | 分支树逐句同构：0..6 型名映射、2/3/5 带 USB 变体且 detail==7 时切换、4/6 直返、其余 wsprintfW("%d") 写 buf 返回(dc:57830-62↔c:177-210)。注记：dc `wsprintfW(buf,L"%d")` 变参残留静态不可定案，src 按字面量 0 补参（同 036 取舍） |
| 055 | PECMD_ParseControlMessage ↔ FUN_140080B0C | dc:82065 | stubs:283(现:297) / core_b3m.c:1200 | MATCH | 全结构同构：`?`/`*` 前缀解析、数值路径(ParseUIntValue≡FUN_140074838 RM:546，哨兵 0xf8000009/-0x7fffff7)→五字段消息块{target,id,(star!=0),lParam,0} SendMessageW(hwnd,0x463,wParam)→返 1/3；`?` 路径 CopyTokenTrimmed(0x3b,0x3a)(RM:1562)+IsWindowVisible+VarSetUInt(RM:228) 写变量(dc:82084-118↔c:1220-1253) |
| 056 | PECMD_ControlEnableCommand ↔ FUN_1400C47F4 | dc:121545 | stubs:284(现:298) / core_b7a.c:358 | MATCH | 全分支同构：空串返 0xf8000009；`?`→AllocStrSlot+CopyTokenTrimmed(0x3b,0x3a)+IsWindowEnabled+VarSetUInt+free；否则 '#' 跳过+FUN_1400C11C0 解析(哨兵 -0x7fffff7)→四字段消息块{target,(id!=0),lParam,0}，非 '#' SendMessageW(0x462)、'#' PostMessageW；返 0(dc:121559-94↔c:361-400) |
| 057 | PECMD_GetDiskGeometry ↔ FUN_140065EFC | dc:62728 | stubs:285(现:299) / core_b3n.c:81 | MATCH | 打开回退链(0x80000000 共享读写→0 查询)与 INVALID 归一逐句同构、DeviceIoControl(0x2d1400=GET_DRIVE_GEOMETRY，入 0xc 出 0x28 含尾双字)、CloseHandle 条件、返 local_1c(dc:62743-75↔c:96-127)；src 预清零 local_1c 为无害超集（失败时返 0 而非栈残留） |
| 058 | PECMD_OpenFileExisting ↔ FUN_14001D810 | dc:17400 | stubs:286(现:300) / core_exec4.c:53 | **DEVIATION（文档化近似，低危）** | dc 为 NtCreateFile 动态调用链：先经 fn-ptr DAT_14013cb48 做 NT 路径预转换(dc:17419)，再 DAT_14013cd18(&handle, access\|0x100000, OA(cb 0x30), IOSB, share, 0x20)(dc:17426)，-1 归 0；src 直接 `CreateFileW(path,access,share,NULL,OPEN_EXISTING,0,0)`(c:55-57)——缺 `\|0x100000` 访问位、无 NT 路径转换、无同步标志。文件头已自注 TODO(verify)。常规 `\\\.\\Dev` 路径+GENERIC_READ/share7 场景实践等价；NT 原生路径或特殊访问位场景不等价。最小修复：恢复 fn-ptr 链或在注释外补 0x100000 与路径归一 |
| 059 | PECMD_InitSimpleWindow ↔ FUN_1400F1BE4 | dc:147973 | stubs:287(现:301) / core_b8h.c:651 | MATCH | 十三步初始化逐项对应(含 ×4/字节换算全部正确)：E57C0 基初始化、OBJ_LINK&0xfc、[0x1b]/[0x1c]、u16+0xa2=0xffff、虚表 14012c2e0、+0xac/+0xa0 清零、+0x94/+0x90=0x80000000、[0x11]/[0x10] 清零、byte+0x61=3、光标懒加载 g_hArrowCursor≡DAT_14013e4d8(core_globals.c:180) LoadCursorW(0x7f89=IDC_ARROW)(dc:147976-91↔c:653-667) |
| 060 | PECMD_InitListViewObject ↔ FUN_1400FB588 | dc:154321 | stubs:288(现:302) / core_b8h.c:726 | MATCH | 十六步字段初始化逐项对应(qword 元素 ×8 / 字节转换全部正确)：E57C0、[0x11]/[0x10]、[0x1c]、虚表 14012c7b0、OBJ_LINK(0xd0)、[0x1b]、0xa0/0xac/0xb8、memset(0xe8,0x74)、OBJ_FLAGS=0x74、0x9c/0x98/0x94/0x90=0x80000000、光标缓存 g_hArrowCursor(dc:154324-42↔c:728-745) |
| 061 | PECMD_AppendFormattedI64 ↔ FUN_14006CC70 | dc:68036 | stubs:289(现:303) / core_b3g.c:525 | MATCH | 五步一致：FUN_1400e6d38≡PECMD_SprintfRetEnd(RM:1538) 格式化 " %I64d"、空缓冲判首字符跳过前导空格、pp[2]/pp[1] 同加长度、AppendWide 追加(dc:68043-48↔c:528-533)；高频函数(29 调用点)签名相容 |
| 062 | PECMD_InstallInfDevice ↔ FUN_14002C8A4 | dc:26580 | stubs:290(现:304) / core_b2c.c:709 | MATCH(含黑箱注记) | 设备安装链逐句同构：SkipWCharUntil(':')、空/单字符早退、截断、memset(0x208)+lstrcpynW(0x100)、g_pSetupDiGetINFClassW≡DAT_14013cef8(pecmd_globals.h:495)→CreateDeviceInfoList→CreateDeviceInfo(cbSize 0x20,DIODC_INFLAG=1)→SetDeviceRegistryProperty(1,len*2+4)→g_pSetupDiCallClassInstaller≡DAT_14013cf20(h:477,0x19=DIF_INSTALLDEVICE)、失败 GetLastError()、恢复 ':'、DestroyDeviceInfoList≡DAT_14013cf10(h:496)。注记：dc 返回 unaff_RBX 为寄存器残留静态不可定案，src 按保守 0 处理并注记 |
| 063 | FUN_140021144 ↔ PECMD_PatchInfDirectives | dc:19830 | stubs:291(现:305) / core_b2d.c:191 | MATCH | 全函数(size=1256)逐段核对：%SystemRoot%\\INF\\ 前缀展开(ExpandCommandLine=B组体)+前缀比较、OpenFileHandle(RM:540)/GetFileSize/SetFilePointer(RM:491)/AllocStringSlot2(RM:1117)、BOM 三态(feff/fffe→宽度与 BE 字节交换公式)、9 字符窗口填充+小写化[0..5]、`opyfiles`→写 'C'/`elfiles`→写 'D'、INF 目录内 `copyfiles/delfiles`→写 ';'——**dc 原文此处即用 ReadFile(dc:19955)，src 忠实保留原版怪癖(c:294)**；行尾跳过循环与收尾 frees 全对应 |
| 064 | FUN_140019da8 ↔ PECMD_AppendKeyIfMissing | dc:14702 | stubs:292(现:306) / core_b1_remaining.c:7203 | MATCH | 空键守卫、FUN_140019cb8≡PECMD_FindTablePrefixEntry(RM:738) 查找、未中则 "\r\n"+key+"\r\n" 组串并同序追加进 *(obj+0x110) 表缓冲、释放临时(dc:14708-17↔c:7209-7218)；返回 void 一致 |
| 065 | FUN_140020fd4 ↔ PECMD_WriteParamRecord | dc:19778 | stubs:293(现:307) / core_b2b.c:229 | MATCH(含黑箱注记) | 记录写入全序列对应：锁内 AllocString(lenA+lenB+lenNum+8)、type+9 头、a、(9,0x22)、b、(0x22,9)、数字串、尾标 (13,10,0)——逐槽值与推进步长一致(dc:19799-818↔c:248-267)。注记：wsprintfW("%d") 变参残留，src 按字面量 0 补参（同 036/054 取舍） |
| 066 | PECMD_AllocExpandPath ↔ FUN_140078BE8 | dc:76627 | stubs:295(现:309) / core_b3i.c:665 | MATCH | AllocWStringBuffer(0x2411)→FUN_14006459C(=019 真体,容量 0x2410)→last 非空时 last[-1]=0 戾断、StrBldCopyWide(RM:1247) 入 out、返 *out、free(dc:76634-43↔c:667-678)。旁注：core_b3i.c:690 已见 R14 修复标注（#023 漏 & 已被主线采纳） |
| 067 | PECMD_CheckInfDriver ↔ FUN_14002B2EC | dc:25832 | stubs:296(现:310) / core_b2e.c:985 | MATCH(抽样核对) | size=1290 大函数：头部逐句一致(dbg&2 日志含 "CheckInf--Bigin:%s" 原版缺参怪癖、ReadFileToBuffer+\\n 预置、行扫描 ';[' 跳过与 '='/',' 截断)；goto 标签块 LAB_14002b41b/b48e/b4c8/b4d2 全部原样保留且逐句对应(dc:26012-31↔c:1180-1204)；尾部 CheckInf--End(dc:26001-11↔c:1167-1177)。中段 INF 行匹配循环按结构核对未见偏差。覆盖度：首尾+标签全量、中段抽样 |
| 068 | PECMD_ScanDeviDirInfFiles ↔ FUN_14003B540 | dc:34738 | stubs:297(现:311) / core_b2f.c:6172 | MATCH(抽样核对) | size=2851 巨型扫描：头部逐句一致(WideStrLen=001 真体、.INF lstrcmpiW≡_wcsicmp、local_2f0 位技巧经 B2F_HI32 保真、"DeviDir....Begin:%s" 缺参怪癖保留)、'\\' 根枚举递归、\\INF\\*.INF/名称索引/映射读取链一致；尾部特征逐字实证：递归子目录(mode&0xf==1 "\\*.*")、"DeviDir....End"(dc:35076↔c:6527)、返回值怪巧 `-(flag)&flag`(dc:35085↔c:6536)。中段抽样未见偏差 |
| 069 | PECMD_UpdateDriverSigning ↔ FUN_1400E3288 | dc:138696 | stubs:298(现:312) / core_b8m.c:867 | **DEVIATION** | 主干同构：DEVIAutoDisverify/Driver Signing Policy 双读、Crypto 链(Acquire→MD5 0x8003→HashData{0,mode}+seed→GetHashParam 16B→写 PrivateHash)、Err_1..5 步进日志、`uVar6\|=0x1000`、LAB_365f 清理+三处 Policy 写+结果日志、mode==0 回写 DEVIAutoDisverify=1。**偏差①（日志副作用）**：dc 的 "DEVI驱动签名【%s】(%d.%d)" 记录位于 `policy==mode &&(…)` 短路子句内(dc:138762-64)，仅 policy 相等才记；src 无条件先记(c:902-903)。**偏差②（数据源错）**：三处 Policy 写入 dc 传 `local_res8`(=mode)(dc:138798/138800/138803)，src 传 `regVal`(c:997-1005，此时内容是 cbData 残留)——策略值可能被写坏。最小修复：①把日志移入 policy==mode 条件内；②三处缓冲改 `(BYTE *)&local_res8` 等价物 |
| 070 | PECMD_ManualMapPeImage ↔ FUN_1400282D4 | dc:24235 | stubs:307(现:321) / core_b2c.c:454 | MATCH | 手工映射全链逐句同构（int* 元素×4 换算全对）：ValidatePeHeader(RM:670)→ComputeSectionMapSize(RM:724)→VirtualAlloc(优先 ImageBase+0x30，失败 NULL 基)→CollectPESections(RM:455)→+0xb0/+0xb4 校验→RelocateImage(RM:667)→BindDelayImports(RM:1066) 失败释放→DllMain/_dllMain@24 查找(*pe 与 +24 现场保存恢复)→入口调用 (1,0)/(0,0)、成功置 g_pMapBlk≡DAT_14013cf50(pecmd_globals.h:44)+*pe=1 返基址(dc:24246-95↔c:462-512) |
| 071 | FUN_14001ebdc ↔ PECMD_ReadFileStr | dc:18415 | stubs:311(现:325) / core_b1_remaining.c:9466 | MATCH | 读文件链逐句同构：OpenFileHandle(=158 真体)失败返 0xfffffffe、GetFileSize(=039 真体)+SetFilePointer、GrowByteBuffer(RM:476, size+0xc)、ReadFile+尾清 6B、EncodeDet(=031 真体；**交叉注记：031 体本身有偏差，此处连带受影响**)低位置位时重编码(EncodeBuffer RM:509=041 真体)、ResDecode(=027 真体)、符号扩展返回与双路 CloseHandle(dc:18430-72↔c:9476-9513) |
| 072 | FUN_140063344 ↔ PECMD_AllocSmallObject | dc:60656 | stubs:312(现:326) / core_exec.c:120 | MATCH(两处注记) | 成功路径逐句一致：HeapAlloc(10)+OOM 对话、先写 0xaa55@+4 再被 qword 尺寸 2 覆盖的原版顺序(src 有 T1c 注)、返回槽内 +8 指针并清首字节(dc:60663-76↔c:124-138)。注记①：OOM 用户选"忽略"(5) 时 dc 落入 NULL 写头(原版自身 AV)，src 改为无限重试——退化路径行为不同但均异常，宜文档化；注记②：dc 返回槽指针、src 返对象指针，全树调用点均为语句式忽略返回值，无实际影响 |
| 073 | PECMD_OomPrompt ↔ FUN_1400630D0 | dc:60524 | stubs:313(现:327) / core_string.c:209 | **DEVIATION（探针残留）** | 本体一致：mode==0→"内存错误"否则"内存不足!"、MessageBoxW(0,…,"异常退出",5=RETRYCANCEL)、非 4/5 时 ExitProcess(8)(≡FUN_14005b21c core_exec2.c:116)+int3；返回 int 为调用方读 EAX 的正确还原(S11 注)。**偏差**：c:211-218 残留 TEMP PROBE(P2 分诊)——每次调用都 fopen("C:\\pectest\\memfail.log") 追加写，原版无此副作用且 C 盘不可写时每次调用做无效 IO。最小修复：删除探针块 |
| 074 | PECMD_TaskShowMessage ↔ FUN_1400612CC | dc:59014 | stubs:317(现:331) / core_b3m.c:465 | MATCH | 任务消息全流程逐句同构：计数<1 返 0x80070057、SetEvent 唤醒、type==2→SeShutdownPrivilege(FUN_14001C2CC 同名)+InitiateSystemShutdownW(timeout 0x12cc02f6,EWF=1)否则 MessageBoxW(|0x2000 00)、结果写 +0x34、10ms×1000 等待循环、锁内计数递减+FreeStrBuf+返 0(dc:59027-66↔c:477-516) |
| 075 | FUN_14006b8fc ↔ PECMD_ContainerAppend | dc:67155 | stubs:326 / core_b3b.c:330 | MATCH | 两步一致：PECMD_SetWindowProcHook≡FUN_140060870(RM:716)(container, *container, *container+1 槽)、返 *container+8(dc:67158-59↔c:332-334)；全树 23+ 调用点的公共追加口 |
| 076 | PECMD_CreateTreeViewControl ↔ FUN_1400FF080 | dc:156627 | stubs:327 / core_b8g.c:1083 | MATCH | 与 044/040 同构族：CreateWindowExW(0,"SysTreeView32",…)+obj[4]+ContainerAppend+UpdateWindow+返 obj[4]!=0(dc:156637-48↔c:1085-1097) |
| 077 | PECMD_ParseAngleNumbers ↔ FUN_1400A40E8 | dc:102536 | stubs:328 / core_b3l.c:1226 | MATCH | `<…>` 五元组解析逐句同构：'<' 判定、首字符字母守卫(`(c\|0x20)` 域外才进)、扫 '>' 置 NUL(无 '>' 则不置)、FUN_140070248≡PECMD_ParseNumSkipChar_0248(RM:79) ×4、各负值钳 0、恢复 '>'、返 1/0(dc:102547-86↔c:1231-1275) |
| 078 | FUN_140005344 ↔ PECMD_CheckDebugMsg | dc:2277 | stubs:331 / core_proc.c:114 | MATCH | 「核对」项落实：双侧均无参。GetEnvironmentVariableA("DEBUGMDG",buf,99)、首字符=='1' 置 g_flag16b≡DAT_14013c96b(pecmd_defs.h:52)(dc:2280-87↔c:116-123)；buf[120] 一致 |
| 079 | FUN_140017048 ↔ PECMD_ServiceEntry | dc:12109 | stubs:333 / core_proc.c:164 | MATCH | 服务分发三步一致：表项 {"Service", FUN_140016ae0≡PECMD_ServiceMainEntry RM:1483}、StartServiceCtrlDispatcherW、FUN_140008834≡PECMD_InstallWindowsService(RM:1068)(dc:12116-22↔c:166-173)；src 的整表 memset 为无害超集 |
| 080 | PECMD_DeviceCheckReady ↔ FUN_14006CAF0 | dc:67946 | stubs:352 / core_b3j.c:651 | MATCH | 「1参」核实：双侧单参。OpenFileHandle(0xc0000000,3,…,4)→DeviceIoControl(0x900c4=STORAGE_CHECK_VERIFY,0/0)→双界 CloseHandle→返 BOOL(dc:67953-61↔c:654-662)；字节级一致 |
| 081 | PECMD_FindRasConnection ↔ FUN_1400E7664 | dc:141859 | stubs:358 / core_b8i.c:274 | **DEVIATION** | 骨架一致：空名→g_rasState≡DAT_14013e2a0 判定+AllocString(100)+HangUpRas；否则 AllocSlot(0x21e30)+RasEnumConnections(g_pfnRasEnumConnections≡DAT_14013e290)枚举、'*' 通配或 lstrcmpiW 命中后等待。**偏差（指针算术错）**：dc 条目地址为 `undefined4* 基址 + i*0x15b 元素 + 1`＝字节偏移 `i*0x56c+4`(dc:141892/141896)，src 的 '*'/命中等待分支写成字节算术 `(uint8_t*)blk + i*0x15b + 1`(c:303/309)——步长缩水 4 倍且首项偏移 +1≠+4，等待目标地址全错（对比 c:307 的 lstrcmpiW 用 dword 算术是对的）。最小修复：统一 `uint8_t *entry=(uint8_t*)blk + (int64_t)i*0x56c + 4;`。附注：DAT_14013e2a0/e300 在 stubs 另有独立定义（同 006 分裂符号模式） |
| 082 | FUN_14006156c ↔ PECMD_MatchPattern | dc:59171 | stubs:376 / core_b3c.c:212 | MATCH | 指针距离算术逐字一致：`d=ctx-p`，循环比较 `*p==*(short*)(d+p) ‖ *p==*(short*)(d+8+p)`，连续匹配 4 个返 0 否则 1(dc:59177-87↔c:214-224)；30 处调用点共享结论 |
| 083 | FUN_1400a53e4 ↔ PECMD_TokenizeExpression | dc:103349 | stubs:377 / core_b3_remaining.c:24757 | **DEVIATION（疑尾段缺失）** | 头部 ~120 行逐句一致：'*' 探测(+0x48^0x2a)、分隔符跳过集(+0x92/+0x94/+0x8a/+0x90)、operator_new(0xf0)+ScriptCopy(=045 真体)+ScriptInit(=008 真体)、空 CS 对、参数展开(FUN_14007A224=B组体)；中段令牌循环结构对应(含 dc 同款空 CS 对、+200/0xc8 标志合成)。**偏差**：dc 尾段含大块「命令对象数组扩容(operator_new(0x48)+GrowByteBuffer)、'.Enable' 变量写、CreatePopupMenu/SetMenu、托盘 FUN_14005b900、菜单项 FUN_1400e5248/e5120、thunk_FUN_140064b78 清理」(dc:103884-104028)及 LAB_1400a65de/eb 多级清理返回(dc:104029-36)，src 版(c:24757-25171，~415 行 vs dc~690 行)无对应段落且单点 return lVar8。影响：UI 上下文(param_4 位)下表达式求值的建单/挂菜单行为缺失。处置建议：主代理专项复核——先确认 dc 该段触发条件，再决定补齐或证明不可达 |
| 084 | FUN_14001b23c ↔ PECMD_ExtractTableSegment | dc:15778 | stubs:378 / core_b1_remaining.c:7945 | **DEVIATION（依赖桩）** | 主体逐句同构：end 槽缺省时按分隔集(+0x90/+0x8a/+0x88)扫描、跳过分隔符(+0x92/+0x94)、异或键(+0x48)拷贝、尾部三清零。**偏差**：`FUN_14001b168≡PECMD_StripInlineComments`(RM:1489) 在 src 仍是空桩(c:7937-7943 自注「尚为桩」)——g_charTableF≡DAT_14013a248(pecmd_globals.h:214)>0 且 +0xe 标志清零时，原版的行内注释剥离(size=211 真体未还原)静默丢失。最小修复：还原 FUN_14001b168 体或在转发计划中登记该依赖 |
| 085 | FUN_140067b54 ↔ PECMD_ParseAndSkipSpace_7b54 | dc:64195 | stubs:390 / core_b3b.c:271 | MATCH | 三步一致：FUN_140067a00≡PECMD_ParseNumberToDouble(core_b3_remaining.c:13696 同 VA)、结果 &0xffffffff 双重掩码、FUN_14005b154 跳空白(dc:64200-203↔c:273-276)；18 处调用点 |
| 086 | FUN_140007bcc ↔ PECMD_ParseIntThenSkip | dc:4153 | stubs:391 / core_b1_remaining.c:3381 | MATCH | 两步一致：PECMD_ParseUIntValue≡FUN_140074838(=146 真体)解析、当前字符非 0 前进一个 wchar(dc:4156-59↔c:3384-87)；src 增加 int 返回值属良性扩展（dc 签名 void，调用点均忽略） |
| 087 | FUN_14005e36c ↔ PECMD_CopyFlags | dc:56412 | stubs:393 / core_b3d.c:213 | MATCH | 掩码复制四字段逐句同构：mask=src[5]，bit0/1/2/8→dst+0x13c/0x140/0x144/0x148 ← src[0..3](dc:56417-29↔c:215-227) |
| 088 | FUN_14005bc28 ↔ PECMD_IsAlnumLowerDigit | dc:54313 | stubs:398 / core_b3a.c:166 | MATCH | 判定式逐字一致：`((ch<0x61‖0x7a<ch) && 9<(u16)(ch-0x30))→0 否则 1`(仅 a-z/0-9，不含 A-Z)(dc:54316-19↔c:168-171)；R14 仲裁注记已处理与 @140061204 的 RM 撞名（独占名导出+static 包装转发），语义归属正确 |
| 089 | FUN_14005bbb4 ↔ PECMD_ParseSizeSuffix | dc:54278 | stubs:399 / core_b3h.c:210 | MATCH | 后缀乘数表逐句一致：t→1<<40、g→0x40000000、m→0x100000、k→0x400（推进）、s→推进返 0x200、未知不推进返 1；尾推进+返回(dc:54284-305↔c:212-234) |
| 090 | FUN_140062ec8 ↔ FUN_140062EC8 | dc:60376 | stubs:403 / core_b3k.c:252 | MATCH | token 查找逐句同构：flags&0xff00==0 时 StrCmpNIW+结尾 NUL/'&' 判定、否则 PECMD_MatchPatternSwap≡FUN_1400175a0(RM:712)、命中返 tokens、低字节为 0 才继续按 len+1 步进、双 NUL 终止返 NULL(dc:60381-407↔c:255-282) |

## 第二轮小结（[041..090]）

| 结论 | 计数 | 序号 |
|---|---|---|
| MATCH | **42** | 041 042 043 044 046 047 048 049 050 052 053 054† 055 056 057 058‡ 059 060 061 062† 063 064 065† 066 067§ 068§ 070 072‡ 074 075 076 077 078 079 080 082 085 086 087 088 089 090 |
| DEVIATION | **8** | 045 051 058 069 073 081 083 084 |
| UNVERIFIABLE | **0** | — |

† MATCH + [RESOLVABLE-BLACKBOX]/保真注记（wsprintfW 变参残留、unaff_RBX 寄存器残留按保守值处理）。‡ MATCH + 行为注记（见行内说明）。§ 抽样核对（首尾+标签/特征全量、中段抽样，未见偏差）。

### 本轮关键发现（转发前处置建议）

- **高危（地址算术错）**
  - 081 `core_b8i.c:303/309`：RAS 条目等待地址用字节算术 `(uint8_t*)blk + i*0x15b + 1`，dc 实为元素算术＝字节偏移 `i*0x56c + 4`(dc:141892/141896)——'*' 通配与命中等待目标全错。修复：统一 `(uint8_t*)blk + i*0x56c + 4`。
- **疑似大面积缺失（需专项复核）**
  - 083 PECMD_TokenizeExpression：dc 尾段「命令对象扩容/'.Enable' 写/CreatePopupMenu/SetMenu/托盘/菜单项/thunk 清理」(dc:103884-104028) 在 src(c:24757-25171) 无对应段落，返回路径亦不同；头部 ~120 行与中段令牌循环已证一致。建议主代理确认 dc 该段触发条件后补齐或证明不可达。
- **中危**
  - 051 `core_b8m.c:310`：msg==0x85 分支虚表调用写 `vt[8]`(+0x40)，dc 为 vtable+8(槽 1)(dc:140597)。修复：`vt[8]`→`vt[1]`。
  - 069 `core_b8m.c`：①"DEVI驱动签名"日志 dc 位于 `policy==mode` 短路子句内(dc:138762-64)，src 无条件先记(c:902)；②三处 Driver Signing/Non-Driver Signing Policy 写入 dc 传 mode(dc:138798/138800/138803)，src 误传 regVal(c:997-1005)——策略值可能写坏。
  - 045 `core_b3_remaining.c:12554`：收尾应调 PECMD_FreeStrBuf(FUN_14005b104)，现写成 FUN_14005B154(跳空白助手)——资源数组泄漏。
- **低危/文档化近似**
  - 058 OpenFileExisting：NtCreateFile 链(含 access|0x100000 与 NT 路径预转换)被 CreateFileW 替换（文件头已自注 TODO）。
  - 073 OomPrompt：TEMP PROBE 每次 fopen C:\pectest\memfail.log（自注「定位后移除」）。
  - 084 ExtractTableSegment 依赖的 PECMD_StripInlineComments 仍为空桩（g_charTableF>0 路径静默失效）。
- **联动登记**：068/071 等经 001(WideStrLen 少一)/031(EncodeDet 错位) 真体的调用连带受影响；DAT_14013e2a0/e300 存在 stubs 独立定义（同第一轮 006 分裂符号模式）。**积极信号**：#023 的 CopyStrToSlot 漏 `&` 已见主线修复标注（core_b3i.c:690 "R14: 衈 &(batch-A #023)"）。

### 方法与覆盖说明（第二轮）

- 对列表沿用第一轮编号 [041..134]，本轮完成前 50 对；每对三处原文核对、映射现场核证（RM/PD/globals/offsets/pe_registry），完成即追加。
- 大函数覆盖度：063(1256B) 全文逐段；083(2456B) 头部 120 行逐句+中段结构对应+尾部差异实证；067(1290B)/068(2851B) 首尾+标签/特征表达式全量、中段抽样；其余中小函数逐句核对。
- 未改任何代码；未触碰 git；本文件仍为本轮唯一写入产物。

### 待办移交

剩余 [091..134] 共 44 对留待下轮（091=扫描#153 thunk_FUN_1400f429c ↔ core_b8h.c:709 起，该对与 #56 同 VA 双拼写可复用 033 结论）。建议下轮优先项：①复核 083 尾段缺失；②同族指针算术排查（RAS/条目遍历类）；③StripInlineComments 真体还原排期。

---

## 对照结果表（续）：第三轮收尾 [091..134]

方法与纪律同前两轮；同 VA 双拼写对复用已证结论并核验桩签名透传相容性。batch-A 总收官汇总见文末。

| 序号 | 函数名(桩↔真体) | dc锚点 | 两侧位置 | 结论 | 证据摘要 |
|---|---|---|---|---|---|
| 091 | thunk_FUN_1400f429c ↔ FUN_1400F429C | dc:149819 | stubs:409(现:437) / core_b8h.c:709 | MATCH（复用 033） | 同 VA 双拼写(#56/#153)：真体结论复用 033（游标推进循环逐句等价，dc 返槽指针但调用点均忽略）；桩 `(void*,short)→uint64_t` 与真体 `(WCHAR**,WCHAR)` 透传相容 |
| 092 | FUN_14005bc48 ↔ PECMD_StoreDouble_bc48 | dc:54327 | stubs:412(现:440) / core_b3a.c:182 | MATCH | 双分支一致：+10 标志置位存 double 原值，否则截断 `(double)(longlong)`(dc:54330-34↔c:184-188)；桩 3 参透传相容 |
| 093 | FUN_14005bc5c ↔ PECMD_StoreDouble_bc5c | dc:54343 | stubs:414(现:442) / core_b3a.c:194 | MATCH | 与 092 相反序：标志置位存截断值、否则存 double 原值(dc:54346-50↔c:196-200)；注释「与 48 相反」属实 |
| 094 | FUN_14005b374 ↔ PECMD_SkipUntilDelim | dc:53821 | stubs:422(现:450) / core_remaining_helpers.c:43 | MATCH | 游标推进等价：NULL 守卫、`while(*p && *p!=ch1 && *p!=ch2){ ++p; *pp=p; }`（dc 条件内重读槽为编译器形态，语义同）(dc:53826-33↔c:47-52)；dc 返槽指针、调用点均语句式忽略 |
| 095 | PECMD_AppendQuotedString ↔ FUN_14006B684 | dc:67027 | stubs:423(现:451) / core_b3g.c:451 | MATCH | 引号包裹七步一致：写 0x22、推进、拷贝 len*2、0x22、0x20、NUL（末字符后不推进，原样保留）(dc:67030-38↔c:453-461)；桩 `(int64_t*,void*,int)` 相容 |
| 096 | FUN_1400048c4 ↔ PECMD_EnsureTempDirPath | dc:1764 | stubs:424(现:452) / core_b1_remaining.c:1305 | MATCH | 全分支逐句同构：长度探测→AllocString(len+0x2d0)→已有内容直跳建目录；TMP→TEMP→SHGetSpecialFolderPath(0x24)+"\\TEMP" 回退链、'\\' 追加与 NUL 收尾、PECMD_CreateDirectory≡FUN_140027644(RM:176) ×2、PECMD_IsDirectory≡FUN_140101d34(RM:598)、失败再回退 "\\TEMP\\"、返 len 低 32 位(dc:1774-813↔c:1315-1358) |
| 097 | FUN_1400177b8 ↔ PECMD_ReleaseMutex | dc:12567 | stubs:425(现:453) / core_b1_remaining.c:6033 | MATCH | 三行体逐字一致：句柄非空才 ReleaseMutex(dc:12570-72↔c:6036-38)；size=23 体注吻合；13 处调用点 |
| 098 | FUN_14005b540 ↔ PECMD_NormalizeQuoteChars | dc:53938 | stubs:428(现:456) / core_b3_remaining.c:9943 | MATCH | 全函数逐字一致：7 个开/闭括号种子({,(,[,],},) 与全角变体 -0xf8/-0xa5/-0xf7/-0xa3/0x3008-0x3017)，mode≠0 清全部、开括号→'<' 闭括号→'>' 归一化、NUL 终止(dc:53949-85↔c:9954-90) |
| 099 | FUN_14000500c ↔ PECMD_DetectCodePage | dc:2108 | stubs:431(现:459) / core_sys.c:51 | MATCH | 「核对」项落实：双侧均无参。LC_ALL→GetLocaleInfoW(0x800,0xb) 回退、默认 "936"、StrBldCopyWide 入 g_pLocale≡DAT_14013ca70(pecmd_defs.h:108)、FUN_140064A88(十进制解析) >1 才写 g_SysCodePage≡DAT_140147004(pecmd_defs.h:42)(dc:2115-28↔c:56-69) |
| 100 | FUN_140005738 ↔ PECMD_OpenSharedMapping | dc:2531 | stubs:432(现:460) / core_sys.c:74 | MATCH | 共享映射全链逐句同构：CreateFileMappingA(-1,0,0x8000004,0,100)→失败 OpenFileMappingA→双零 return、ERROR_ALREADY_EXISTS(0xb7) 抑制清零、MapViewOfFile(6)+CloseHandle、新建才 memset(100)、首 qword 写 tag；g_pSharedMap≡DAT_14013ca30(pecmd_defs.h:109)(dc:2538-62↔c:80-102) |
| 101 | FUN_14005370c ↔ PECMD_ParseRegexQuantifier | dc:48876 | stubs:435(现:463) / core_b3_remaining.c:7331 | MATCH(抽样核对) | size=1227 正则量词解析：头部逐句一致(ParseVarExpressionElement、*?/*/+?/+/??/? 六态码 0x13/0x12/10/9/0xc/0xb、ResizeBuffer=050 真体、ArrayPrepend=017 真体链 0x10/0xd/0x14/0x16/0xe/0xf/0x15/0x1a/0x16、AtlThrowImpl(-0x7ff8ffa9) 原样保留)；b3_* 标签与 dc LAB_140053xxx 一一对应；尾部 bVar11/uVar9 掩码返回逐字一致(dc:49102-109↔c:7543-7553+) |
| 102 | FUN_14005ceec ↔ PECMD_PrefixMatchNoCase | dc:55374 | stubs:436(现:464) / core_b3e.c:181 | MATCH | 循环结构逐句一致：模式字符 NUL→返 `*w==0`、A-Z 双侧 `\|0x20` 折叠、不等返 false。注记：dc 初值 `(ushort)cVar1`(char 符号扩展) vs src `(uint16_t)(uint8_t)`——仅模式字节 ≥0x80 时行为不同；现全部调用点均为 ASCII 字面量("icon Group"/"ICON"/"#14" 等)，实际无影响，登记备查 |
| 103 | FUN_14005d694 ↔ PECMD_SetRdataTablePtr | dc:55714 | stubs:437(现:465) / core_sys.c:158 | MATCH | 「核对」项落实：双侧均无参。四写一致：g_pVtblA≡DAT_14013d670/g_pVtblB≡DAT_14013d5e0 ← &g_szEmpty(0x14011c638)、g_val668/g_val5d8 = 0xffffff9c(core_sys.c:43-46 extern 注记)(dc:55717-20↔c:160-163) |
| 104 | FUN_14006355c ↔ PECMD_WideToAnsiStr | dc:60784 | stubs:438(现:466) / core_b3_remaining.c:12217 | MATCH | 宽转 ANSI 七步一致：cp<0→g_SysCodePage(=DAT_140147004)、len<0→lstrlenW、GrowByteBuffer(len*5+0xb)、WideCharToMultiByte、written 钳到 allocLen、NUL 收尾、二次 Grow(outLen+1)(dc:60791-815↔c:12224-12247)；45 处调用点 |
| 105 | FUN_140031454 ↔ PECMD_RunCommand | dc:29484 | stubs:462(现:490) / core_scriptrun.c:260 | MATCH(抽样核对·重构式移植) | size=6510 最大函数：src 为重构式移植（拆出 core_srparse.c PECMD_SrParsePrefix，注释锚定 dc 行号）。抽样实证：序言 *qk(FUN_1400660AC,dc:29603↔srparse.c:84)、*sysinit→SetVar "&&__UDM.sysinit"=1(dc:29612-27↔srparse.c:93-104)、'-' 前缀、g_sysinitState≡DAT_14013d058(core_scriptdep.c:35) 状态机 1/3/7 一致；主展开路径注释自锚 LAB_140031887(dc:29635-30332)。**注记①**：多处 TEMP PROBE fopen memfail.log 残留(c:222-229/262-265/276-279)；**注记②**：25 调用点的核心调度，建议后续专项全文审计 |
| 106 | FUN_140067cf4 ↔ PECMD_ParseNumSkipWs | dc:64290 | stubs:463(现:491) / core_remaining_helpers.c:118 | MATCH | 三步一致：FUN_140067B78(多进制解析器，=core_exec.c:143 同 VA) 结果 >0 判定、成功才 FUN_14005B154 跳空白、返 bool(dc:64296-301↔c:121-126)；38 处调用点 |
| 107 | FUN_14004fb44 ↔ PECMD_ReserveVec8 | dc:46130 | stubs:464(现:492) / core_b3_remaining.c:5026 | MATCH | 扩容策略逐句一致（src 参数化 reserve_vec(esize=8)，与 #fc58=esize 0x10 共用）：vec[2]>=minCap 早退返 1、空槽 clamp vec[3]+calloc、增量 grow=vec[3] 或 vec[1]>>3 钳 [4,0x400]、newCap=max、搬迁+free+置容量(dc:46141-199↔c:4986-5023)。注记：dc 的 memmove_s 错误码→AtlThrow 路径在 src 以 memmove 省略——参数合法时不可达，登记备查 |
| 108 | FUN_14005d9a8 ↔ FUN_14005D9A8 | dc:55959 | stubs:465(现:493) / core_exec.c:280 | **DEVIATION**（复用 018） | 同 VA 双拼写(#26/#173)：真体结论复用 018——提示音抑制链少一层解引用(elem+0x38 应再解 +0x20)；桩 `(int64_t,int)` 透传相容 |
| 109 | FUN_1400630d0 ↔ FUN_1400630D0 | dc:60524 | stubs:466(现:494) / core_string.c:209 | **DEVIATION**（复用 073） | 同 VA 双拼写(#100/#174)：真体结论复用 073——TEMP PROBE fopen 残留；桩 `(int)→uint64_t` 返回宽度相容 |
| 110 | FUN_14005c7c4 ↔ FUN_14005C7C4 | dc:54983 | stubs:469(现:497) / core_exec5.c:156 | **DEVIATION**（复用 017） | 同 VA 双拼写(#24/#175)：真体结论复用 017——失配返回条件 `!A&&!B` 应为 `!A\|\|!B`；桩 2 参透传相容 |
| 111 | FUN_1400702b0 ↔ FUN_1400702B0 | dc:70440 | stubs:470(现:498) / core_string.c:154 | MATCH | 两步一致：清槽、非空才 PECMD_StrDupAlloc≡FUN_1400700c4(RM:229)；StrDupAlloc 体逐句同构(lstrlenW→HeapAlloc(len*2+12)+OOM 环→0xaa55@+4→size=len*2+4@+0→拷 len*2+2→返 +8)(dc:70305-15↔c:130-149)。注记：StrDupAlloc OOM 环内 TEMP PROBE 残留(同 073 类)，且 retry 语义同 072 注记① |
| 112 | FUN_1400f172c ↔ FUN_1400F172C | dc:147812 | stubs:500(现:528) / core_b8m.c:480 | MATCH（复用 046） | 同 VA 双拼写(#73/#178)：真体结论复用 046（消息映射分发全函数已证）；桩 7 参透传相容 |
| 113 | FUN_14005c788 ↔ FUN_14005C788 | dc:54955 | stubs:507(现:535) / core_string.c:187 | **DEVIATION**（复用 016） | 同 VA 双拼写(#23/#179)：真体结论复用 016（混入 c72c 分隔符校验语义）；**主线动态**：桩已改为转发真体(stubs 现:535 `return FUN_14005C788(...)`)，小写名调用点已可达真体——但真体本身的偏差仍待修 |
| 114 | FUN_140018148 ↔ PECMD_ResolveExportProc | dc:12995 | stubs:508(现:536) / core_b1_remaining.c:6269 | MATCH | 内存模块解析分派一致：g_pMapBlk≡DAT_14013cf50 空或 +0x18 模块不匹配→GetProcAddress（结果双侧均弃，原样），否则 PECMD_FindResourceByNameOrId(RM:431)(dc:12998-13003↔c:6273-6278)；+6 int* 步长=+0x18 注记正确 |
| 115 | FUN_140018178 ↔ PECMD_GetModuleFileName | dc:13012 | stubs:509(现:537) / core_b1_remaining.c:6281 | MATCH(含黑箱注记) | 分派一致：非内存模块→GetModuleFileNameW，否则 `\??\memdll?\%p`。**黑箱注记**：dc 与 src 的 w/wsprintfW 均**同样缺 %p 实参**(dc:13019↔c:6288)——Ghidra 变参残留两侧等价保持；建议后续补 `*(HMODULE*)((u8*)g_pMapBlk+0x18)` |
| 116 | FUN_1400185c8 ↔ PECMD_FlushLogBuffer | dc:13279 | stubs:510(现:538) / core_b1_remaining.c:6412 | MATCH | 日志冲刷全链逐句同构：槽选择(缺省 &g_hMainMutex≡DAT_14013cb18(core_execscript.c:81)、NULL→TLS g_dwC934≡DAT_14013c934、-1 除外)、四重冲刷条件、g_flagD6F6≡DAT_14013d6f6(pecmd_globals.h:140) bit1 选 WriteConsoleW/WriteFile、容量 0x1fffe 阈值重建 AllocString(0x10002)、NUL+[2]=0。**交叉注记**：长度经 WideStrLen(=001 缺陷体) 少 1——写盘字节数少末字符+NUL |
| 117 | FUN_140017908 ↔ PECMD_InitDynamicImports | dc:12636 | stubs:511(现:539) / core_proc.c:203 | MATCH | 「核对」项落实：双侧均无参。RegDeleteKeyExW 首载+`DAT_14013d400==0` 守卫下五连 SH* 导入，槽位映射全对：d408≡g_pRegDeleteKeyExW(pecmd_defs.h:58)、cd48≡g_pSHGetValueW、cd50≡g_pSHGetValueA(h:61)、cd58≡g_pStrToIntExW(h:62)、d3f0≡g_pSHSetValueW、d3f8≡g_pSHDeleteValueW、d400≡g_pSHDeleteKeyW；FUN_14005c828 直移真体在 core_b7c.c:8900(dc:12642-650↔c:205-213) |
| 118 | FUN_1400717f0 ↔ PECMD_DeleteRegistryKeyRecursive | dc:71824 | stubs:512(现:540) / core_b3_remaining.c:16792 | MATCH | 递归删键全链逐句同构：'\\' 分割、无 '\\'→RegOpenWithRetryPriv(0x2001b,4)→RegQueryInfoKeyW→逆序枚举值删值+逆序枚举子键 g_pSHDeleteKeyW≡DAT_14013d400、RegDeleteKeyExW 懒加载(g_pRegDeleteKeyExW≡DAT_14013d408) 后 (key,sub,0,0)、错误合并；有 '\\'→父路径 StrBldCopyWideN+open(2,4)+SHDeleteKeyW 子键(dc:71843-71907↔c:16810-16870) |
| 119 | FUN_140072924 ↔ PECMD_EnumClassDevices | dc:72387 | stubs:525(现:553) / core_b3_remaining.c:16969 | MATCH(抽样核对) | size=924 SetupDi 枚举：头部逐句一致(g_pSetupDiGetClassDevsW≡DAT_14013cf00 h:478、ANSI→宽 PECMD_StrDupA≡FUN_1400637dc RM:893、cbSize=0x20、AllocWStringBuffer(0x840b)+四 NUL 怪癖、g_pSetupDiEnumDeviceInfo≡DAT_14013cf28(h:480)、"Net" 类过滤、实例 ID@+0x404、FindTokenInList=150 真体、0x12→FUN_1400662a4 更新/否则 FUN_14005ccb0)；尾部逐字一致：Destroy+frees+`uVar10!=0→iVar11=local_160` 返计数(dc:72522-29↔c:17122-17129)。SP_DID_l 结构注记合理 |
| 120 | FUN_1400664dc ↔ PECMD_EnumNetAdapters | dc:63028 | stubs:526(现:554) / core_b3_remaining.c:13296 | MATCH | 枚举五步一致：GetClassDevs(0,0,0,6) 失败返 0、cbSize=0x20+guid 清 0x1c、循环 uVar3<2000、PECMD_IsSetupClass≡FUN_14006643c(RM:1010，=035 真体) "Net" 过滤、PECMD_ChangeDeviceProperty≡FUN_14005ce04(RM:848)(2-(p2!=0),idx,set)+计数(dc:63038-55↔c:13307-13227)；param_1 弃用双侧一致 |
| 122 | FUN_14001e6bc ↔ FUN_14001E6BC | dc:18137 | stubs:528(现:556) / core_var2.c:257 | MATCH（复用 010） | 同 VA 双拼写(#13/#190)：真体结论复用 010（变量写入核心全分支已证，含 T1 修复注记）；桩 4 参透传相容 |
| 123 | FUN_14005b848 ↔ PECMD_ZeroStruct_b848 | dc:54098 | stubs:529(现:557) / core_b3d.c:117 | MATCH | 清零五步一致：+4 u32、+8 u64、首字节、memset(+0x10,0x3d0)、容量 0x3d0 回写、返 obj(dc:54101-106↔c:119-124)；托盘双槽(0x56/0xd2)调用点 |
| 124 | FUN_1400e7664 ↔ FUN_1400E7664 | dc:141859 | stubs:534(现:562) / core_b8i.c:274 | **DEVIATION**（复用 081） | 同 VA 双拼写(#129/#192)：真体结论复用 081——条目等待地址字节算术错；**附加桩问题**：桩为 `void FUN_1400e7664(LPCWSTR)` 无返回值(stubs 现:562)，而真体返 ulonglong——转发后取值调用点将读空，转发时须同步补返回类型 |
| 125 | FUN_1400629b8 ↔ FUN_1400629B8 | dc:60136 | stubs:538(现:566) / core_var.c:30 | MATCH | 变量设置核心逐句一致（早返反转等价）：空键早退、`'&' 前缀‖script+0xd&0xf`→锁内 FUN_14001E6BC(…,-1)(=010 真体)、否则 FUN_14005D534 环境直写(dc:60139-148↔c:32-42)；全项目最大扇出真体（232 处调用点）；样例①同款转发目标 |
| 126 | FUN_14005b900 ↔ FUN_14005B900 | dc:54140 | stubs:542(现:570) / core_b3j.c:187 | MATCH | 托盘更新四路径逐一核实：先 DestroyTrayIcon(=014 体)、+0x30 已设跳过全部、icon==0 时 LoadIconW(g_hInst,IDI_APPLICATION) 失败仍写头+tip(与 src 内层 if 不对称形态等价)、NIM_ADD 结果写首字节；NOTIFYICONDATA 头 d0 03 与 cbSize 布局一致(dc:54146-69↔c:191-215)；#21/#194 同 VA 双拼写，#21 为二批零参桩 |
| 127 | FUN_1400e6860 ↔ FUN_1400E6860 | dc:141061 | stubs:546(现:574) / core_b8f.c:226 | MATCH | 对话框关闭三分支逐句同构（obj[4]=+0x20）：flags&1==0 时 flags==0 直 EndDialog / 否则置 0x80+PostMessageW(0,obj,result) 后 EndDialog；flags&1 置位走清槽+IsWindow+DestroyWindow(dc:141068-89↔c:228-252)；**主线动态**：桩已 R14 转发真体(现:574)，rb:16423 调用点已解 |
| 128 | FUN_14006703c ↔ PECMD_ReleaseWindowHooks | dc:63603 | stubs:547(现:575) / core_b3_remaining.c:13399 | MATCH | 全函数(size=571)逐段核对：+0x150 隐藏+0x466 消息、双钩子卸载(g_hookD710≡DAT_14013d710/g_winHandle≡d718；g_pHookCb3d0/g_hHook2=047 同对全局)、+0x1b0→PECMD_ForwardCall_6820≡FUN_140066820、控件表(+400/0x1a0/0x1a8)逆序遍历：清 +0x28 短路标志、隐藏+SetWindowPos(-8,-8,1,1,4)、type≠0xd 禁用、type==4&&+0x70>0 或 type==10&&+0x7c!=0 → 槽置 &g_u8D5D0(≡DAT_14013d5d8)+vtable 调用、LeaveCS——全对应(dc:63612-73↔c:13410-13469)。**维护注记**：c:13401「UNIMPLEMENTED/decompile-failed」头注释已过时（体已完整还原），宜清理 |
| 129 | FUN_140066d18 ↔ PECMD_ReadSliderPosToVars | dc:63437 | stubs:766(现:794) / core_b3l.c:331 | MATCH | 滑块位置全链逐句同构：vtable+0x118 取 (min,max) 双槽、uVar4=pos+range、min/max 交换、+0x70 bit5 决定钳位方向、越界取反 lVar7、三次 PECMD_AppendLongDecimal≡FUN_1400669c4 写 *(obj+0x50) 表（变量名槽 +0x80/+0x88/+0x78）、SendMessageW(0x46a)(dc:63451-484↔c:333-377) |
| 130 | FUN_14006b7f0 ↔ PECMD_ApplyControlProperty | dc:67091 | stubs:767(现:795) / core_b3l.c:633 | MATCH | 属性分派四分支逐句同构：type1→PECMD_AcquireSlotEntry≡FUN_14006b6e8/0、type4→ID 表换值循环(+0x1a0/+0x1a8)、type3→FUN_1400F2384(...,0,1) 直返、type5→OBJ_BRUSH 替换+CreateSolidBrush+InvalidateRect(OBJ_HWND)；默认返 1(dc:67102-147↔c:635-674) |
| 131 | FUN_14007df90 ↔ FUN_14007DF90 | dc:80332 | stubs:768(现:796) / core_b3i.c:684 | **DEVIATION**（复用 023，已修复） | 同 VA 双拼写(#33/#199)：原判缺陷（c:690 CopyStrToSlot 漏 &）**已被主线修复**(c:690 现带 R14 标注)——修复后体与 dc 一致；桩 `(longlong,int)` void 与真体透传相容。若按修复前快照记 DEVIATION、按当前树状态实为 MATCH，建议以当前树为准 |
| 132 | FUN_14002e790 ↔ PECMD_DispConfirmPopupMenu | dc:27679 | stubs:771(现:799) / core_b2e.c:1495 | MATCH(抽样核对) | size=1714 显示模式菜单：头部逐句一致(LoadMenuW(g_hInstance≡DAT_14013ca68,0x2725)+GetSubMenu(0)、InitPtrTable、ScanMenuRecursive≡FUN_140023080(30000)、0x4c8 命令缓冲 [100]=0、FUN_1400169BC(22000) 语言串、默认 fmt「分辨率 %d×%d 颜色%d位 刷新率%d」逐字同)；模式选择环 0xdc 步长/+0xac/+0xb0/+0xb8/+0xa8 与评分逻辑(600×800/0x1e0×0x280 偏好、16bpp better=2、score*2 vs -score→better=4)一致(dc:27773-804↔c:1547-1583)；菜单裁剪与 "#,DISP -confirm W%d H%d B%d"/"#,EXEC -incmd PECMD DISP -confirm W%d H%d B%d F%d" 双格式串实证(c:1623-1625↔dc:27838-40)。中后段抽样未见偏差 |
| 133 | FUN_1400e89fc ↔ FUN_1400E89FC | dc:142706 | stubs:773(现:801) / core_b8m.c:1559 | MATCH(抽样核对) | size=1610 窗口对象消息处理：头部逐句一致(msg2/PostQuitMessage+0x120&0x10、0x82/DefWindowProc 回退+0xd8 待发+0x447/0x43d 补发+(flags&9)==9 计数减、0x110→vt+0x20、0x111 id2/id1→vt+0x120/+0x118、0x400→0x64c)；0x44d 双路径(注册：查重+首空槽+realloc 扩容；注销：清槽+尾部收缩循环)与 dc:142783-819 逐句对应(×4 偏移全对：0x120/0xd8/0xe8/0xe0)。中后段(0x45c 等)按同文件已证风格抽样。#72/#203 同 VA 双拼写 |
| 134 | FUN_140066eac ↔ PECMD_ReleaseObjectListTail | dc:63517 | stubs:784 / core_b3_remaining.c:13354 | MATCH | 尾部收缩全函数逐字一致：锁内 while(param_2<count){count--;槽非空且≠&g_u8D5D0(DAT_14013d5d8)：==&DAT_14013d660 则清槽否则 PECMD_ReleaseObject_2f74≡FUN_140062f74 释放}、尾随空槽收缩循环、LeaveCS(dc:63524-51↔c:13361-13387)；#52 清理链的配对函数 |

---

## 第三轮小结（[091..134]）

| 结论 | 计数 | 序号 |
|---|---|---|
| MATCH | **37** | 091 092 093 094 095 096 097 098 099 100 101§ 102 103 104 105§ 106 107‡ 111‡ 112 114 115† 116 117 118 119§ 120 122 123 125 126 127 128 129 130 132§ 133§ 134 |
| DEVIATION | **7** | 108 109 110 113 121 124 131 |
| UNVERIFIABLE | **0** | — |

† 含黑箱注记（115 双侧同样缺 %p 实参，等价保持）。‡ 含行为注记（107 memmove_s 异常路径省略；111 OOM 探针残留）。§ 抽样核对。复用对（同 VA 双拼写）：091←033、108←018、109←073、110←017、112←046、113←016、121←007、122←010、124←081、131←023。

**主线积极动态（本轮实证）**：三处桩已改为转发真体——FUN_14005c788→FUN_14005C788(现:535)、FUN_1400e6860→FUN_1400E6860(现:574)、FUN_14006703c→PECMD_ReleaseWindowHooks(现:575)；#023 漏 & 缺陷已修复(core_b3i.c:690 R14 标注)。遗留：FUN_1400e7664 桩为 void 返回(现:562)，与真体 ulonglong 返回不符，转发时须同步归正。

---

# batch-A 总收官（[001..134]，共 134 对）

## 总计数

| 轮次 | 区间 | MATCH | DEVIATION | UNVERIFIABLE |
|---|---|---|---|---|
| 第一轮 | 001–040 | 26 | 14 | 0 |
| 第二轮 | 041–090 | 42 | 8 | 0 |
| 第三轮 | 091–134 | 37 | 7 | 0 |
| **合计** | **134** | **105** | **29** | **0** |

按当前树状态计：#023/#131 的漏 & 已被主线修复 → **有效 MATCH 106 / 有效 DEVIATION 28**。

## DEVIATION 全清单（29 项，去重后 22 个独立缺陷 + 7 个同 VA 复用）

| 序号 | 函数 | 缺陷要点 | 严重度 |
|---|---|---|---|
| 001 | WideStrLen (core_b9_remaining.c:629) | 长度少 1（dc 过 NUL 后 -1 vs src 停 NUL 再 -1）；空串返 -1；35 调用点连带（071 读文件长度、116 日志写盘字节、028 写值截断） | **高** |
| 007/121 | WaitHandlesOrMessages (core_thread.c:294) | MsgWait 失败 err==0 时应返 1，src 返 0 被误读为句柄 0 触发 | 中 |
| 011 | SendHotkeyKeyMessage (core_b2c.c:1145/1147) | 热键表第二槽读 `+4` 应为 `+2`——该槽失效、+4 槽双发 | 中 |
| 012 | SetObjectEnable (core_b3i.c:203) | CopyStrToSlot 漏 `&` → NULL 解引用 AV | **高** |
| 016/113/110 | AsciiPrefixICmp (core_string.c:187)/WideICmp(exec5.c:167) | 016: 混入 c72c 分隔符校验语义（130 调用点）；017 同族：失配条件 `!A&&!B` 应为 `\|\|`——字母位失配全跳过（"AB"="AC"） | **高** |
| 018/108 | DialogBeepNotify (core_exec.c:301) | 控件句柄少一层解引用（obj2=*(elem+0x38) 后应再取 +0x20） | 中 |
| 019 | ExpandDrivePath (core_exec2.c:181) | cwd 栈缓冲 256 < GetCurrentDirectoryW 可写 261 —— 长 CWD 越界 4 wchar | 低 |
| 023/131 | SetCheckVariable (core_b3i.c:690) | CopyStrToSlot 漏 `&` → AV。**已修复（R14）** | ~~高~~ 已修 |
| 024 | QueryFontInfo (core_b3i.c:750) | 缺 LOGFONT 五步预处理（memset/weight=400/width=0/byte=1）→ 字体属性错 | 中 |
| 027 | ResDecode (core_resdecode.c:180) | UTF-8 BOM 路径跳过 DecodeEncTextToUtf16 主解码 → UTF-8 资源乱码 | **高** |
| 028①② | HotkeyControl (core_b2e.c:253/371) | ① WSTR 传 LPCSTR 互斥体名 → 截断成 "G"；② 查询未命中返循环计数应返 1 | **高**/中 |
| 031 | EncodeDet (core_exec2.c:68) | 整套逻辑错位：应为 "CMP" 容器头校验(dc:57236-60)，现为 BOM 嗅探——编码判定失真 | **高** |
| 039 | LayoutChildWindows (core_b8m.c:208) | 0x133E 广播消息结构缺 `*(child+8)` 填充 | 中 |
| 045 | FreeInitObjectList (core_b3_remaining.c:12554) | 收尾误调 FUN_14005B154（跳空白）应为 FreeStrBuf → 数组泄漏 | 中 |
| 051 | ControlWindowProc (core_b8m.c:310) | msg==0x85 分支虚表索引 vt[8] 应为 vt[1] | 中 |
| 058 | OpenFileExisting (core_exec4.c:53) | NtCreateFile 链被 CreateFileW 近似（缺 access\|0x100000 与 NT 路径预转换；已自注 TODO） | 低 |
| 069 | UpdateDriverSigning (core_b8m.c:902/997) | ①日志未随 policy==mode 短路；②三处 Policy 写入数据源 regVal 应为 mode | 中 |
| 073/109 | OomPrompt (core_string.c:211) | TEMP PROBE fopen C:\pectest\memfail.log 残留（同类：core_string.c:136、scriptrun.c 多处） | 低 |
| 081/124 | FindRasConnection (core_b8i.c:303/309) | 条目地址 `(u8*)blk+i*0x15b+1` 应为 `i*0x56c+4`；**附加**：#192 桩 void 返回与真体 ulonglong 不符 | **高** |
| 083 | TokenizeExpression (core_b3_remaining.c:24757) | dc 尾段命令对象扩容/CreatePopupMenu/SetMenu/托盘段(dc:103884-104028)无对应实现——需专项复核触发条件 | 待定 |

## 按处置优先级分层

1. **立即修（崩溃/数据破坏）**：001、012、016/017、027、028①、031、081（+124 桩返回类型）
2. **尽快修（功能性错误）**：007、011、018、024、039、045、051、069、083（专项复核）
3. **择机修（低危/卫生）**：019、058、073 类探针清理、084 StripInlineComments 还原、stubs 分裂符号归一（DAT_14013d868/DAT_14013e2a0/e300）、过时 UNIMPLEMENTED 注释清理（core_b3_remaining.c:13401 等）
4. **已完成（主线采纳）**：023/131 漏 &；FUN_14005c788/FUN_1400e6860/FUN_14006703c 三处桩转发

## 收官说明

- 134 对全部完成三处原文对照（空桩/真体/decompiled.c），映射经 rename_map.json/pecmd_defs.h/pecmd_globals.h/pecmd_offsets.h/pe_registry.json 现场核证；每对完成即追加本文件。
- 大函数覆盖度分级标注：全文逐段（003/004/010/022/027/030/046/051/063/128 等）、抽样核对并注明（067/068/083/101/105/119/132/133）。凡静态不可定案处（变参残留/unaff 寄存器）一律登记 [RESOLVABLE-BLACKBOX] 或保真注记，不臆造语义。
- 本文件为三轮唯一写入产物；未改任何代码（src/** 变更均为主代理并行波次所为并已在行内注明）；未触碰 git。



