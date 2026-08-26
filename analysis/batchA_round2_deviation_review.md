# batch-A 第二轮 DEVIATION 专项复核报告（#081 RAS 地址算术 / #083 TokenizeExpression 尾段）

- 复核人: 证据复核子代理（batch-A round 2）
- 权威语义源: `D:\repo\PECMD反编译\decompiled.c`（下称 **dc**，181421 行，ImageBase 0x140000000）
- 参考实现树: `refactored\src\`（另核对了 `refactored\restored_bodies.c`）
- 复核基线: 工作区当前内容（git HEAD `f6726c1`；注意 `52e3ffa`「R14c … 081 RAS 算术 …」已落库，本复核以当前工作区为准逐行重验）
- 方法约束: 只读核对，无构建、无测试、无源码改动、无调试会话。

---

## 0. 结论速览

| 项 | 原判定 | 本轮判定 | 要点 |
|---|---|---|---|
| #081 RAS 地址算术 | DEVIATION | **PASS（已修复并验证）** | 元素步进/首项偏移已按 dc 字节算术改正；分配器 +8 头契约全链一致；动词前提需勘误（实为 ADSL 非 'RAS'） |
| #081 附带发现 A | （新） | **DEVIATION（新增）** | `PECMD_RasDialEntry` 丢失 dc:144416–144420 的 `"HelloASL"` 缺省入口名回退分支 |
| #081 附带发现 B | （新） | **缺口登记** | `PECMD_WlanManage`(=@FUN_140099f18) 仅实现 RAS 半区；`-wlan` 半区 dc:96964–97471 约 508 行未实现 |
| #083 TokenizeExpression 尾段约 690 行缺失 | DEVIATION | **断言不成立，建议撤销（false positive）** | dc:103884–104028 不属于 TokenizeExpression；FUN_1400a53e4 全体仅 ~386 行且 refactored 已 1:1 覆盖 |

---

## 1. 复核项一：#081 RAS 地址算术

### 1.1 前提勘误：不存在 'RAS' 动词 FourCC

任务前提称 RAS 动词 FourCC 为 `0x00534152`（'R','A','S'）。全 dc 检索结果：

- `0x00534152` / `0x534152`：**0 处命中**；
- 字符串 `"RAS"` / `L"RAS"`：**0 处命中**（仅 `RasDialW`/`RASAPI32.DLL` 等 API 名字符串，dc:139777–139782）。

实际分发链路（动词打包 → 分发）：

| dc 行号 | 内容 |
|---|---|
| dc:44151–44210 | `FUN_14004c0bc` 主执行器将动词前至多 4 个字符小写转大写（`& 0xdf`）打包进 DWORD `local_158` |
| dc:44655–44657 | `else if (local_158 == 0x4c534441)`（LE 字节 41 44 53 4C = **"ADSL"**）→ `FUN_140099f18(param_1, LVar11.QuadPart + 8)` |

即网络拨号命令的动词是 **ADSL**；进入 `FUN_140099f18` 后由 `-wlan` 前缀词检测（dc:96697，`FUN_1400660ac("-wlan",…)`）分流：无 `-wlan` → **RAS 拨号半区**（dc:96869–96963），有 `-wlan` → WLAN 半区。refactored 侧分发镜像一致：`restored_bodies.c:6689–6690` 同为 `local_158 == 0x4c534441 → PECMD_WlanManage(...)`。

> 影响：#081 的定位描述应改写为「ADSL 动词处理器的 RAS 半区」；`core_b3r_h2.c` 文件头把 @0x140099f18 标注为「WLAN 管理命令 (-wlan)」亦与实际覆盖相反（见 1.6）。

### 1.2 RAS 集群实现定位（dc ↔ refactored）

| 功能 | dc 函数（行范围） | refactored 实现 |
|---|---|---|
| 动词分发（ADSL） | `FUN_14004c0bc` dc:43816 起，ADSL 分支 dc:44655–44657 | `restored_bodies.c:6689–6690` |
| ADSL/RAS+WLAN 处理器 | `FUN_140099f18`（size 7183）dc:96604–97471 | `src\device\core_b3r_h2.c:1965–2371`（仅 RAS 半区，见 1.6） |
| 懒加载 RASAPI32 | `FUN_1400e4d94` dc:139770 附近（调用点 dc:139777–139782） | `src\ui\core_b8i.c:87–106` |
| RAS 就绪查询 | `FUN_1400e4d84`（size 13）dc:139756 | `src\ui\core_b8h.c:289–292` |
| 查找/等待连接 | `FUN_1400e7664`（size 243）dc:141856–141905 | `src\ui\core_b8i.c:274–319` ← **#081 修复标记所在** |
| 枚举连接列表 | `FUN_1400e7758`（size 230）dc:141909–141953 | `src\ui\core_b8i.c:325–356` |
| 枚举电话簿条目 | `FUN_1400e7840`（size 338）dc:141957–142005 | `src\ui\core_b8j.c:141–184` |
| 挂断/等待 | `FUN_1400e75cc`（size 149）dc:141816–141852 | `src\ui\core_b8h.c:458–489` |
| 拨号入口 | `FUN_1400ebd30`（size 644）dc:144377–144460 | `src\ui\core_b8m.c:781–860` |
| 拨号状态回调 | `FUN_1400e4edc` dc:139824 | `src\ui\core_b8h.c:322+` |

### 1.3 核心算术逐条证据表（#081 主体）

Ghidra 类型要点：`FUN_1400e7664` 中 `local_res18` 声明为 `undefined4 *[2]`（dc:141868），`FUN_1400e7758` 中为 `undefined4 *`（dc:141923），故 dc 的 `p + k` 为 **dword 单位**；换算字节须 ×4。

| # | dc 行号 | dc 表达式（类型语义 → 字节偏移） | src 文件:行号 | src 表达式 | 结论 |
|---|---|---|---|---|---|
| A1 | dc:141892 | `FUN_1400e75cc(local_res18[0] + iVar3*0x15b + 1)`，元素 `undefined4*` → 字节 `base + i*0x56C + 4` | core_b8i.c:303 | `(uint8_t *)blk + (int64_t)i * 0x56c + 4`（blk 为 `uint32_t*`，显式字节算术） | **PASS**（含 R14b(batch-A #081) 修复注释） |
| A2 | dc:141896 | `lstrcmpiW(param_1, local_res18[0] + iVar3*0x15b + 3)` → 字节 `base + i*0x56C + 12` | core_b8i.c:307 | `lstrcmpiW(name, (LPCWSTR)(blk + i * 0x15b + 3))`（dword 单位，×4 后同为 +0x56C·i +12） | **PASS** |
| A3 | dc:141881–141885 | `FUN_1400633a8(…,0x21e30)`；`*blk=0x56c`；cb=0x21e30；`RasEnumConnectionsW(blk,&cb,&count)` | core_b8i.c:292–296 | 同序同值（0x21e30 = 恰好 100×0x56C） | **PASS** |
| A4 | dc:141899–141900 | 循环计数经 `uVar4=iVar3+1` 推进；命中后**不跳出**，继续扫描，`uVar2` 被后续命中覆盖（末次命中胜出） | core_b8i.c:298–315 | `do{…}while(i<count)`，命中后继续循环、result 覆盖 | **PASS**（语义等价，含 '*' 通配每项必中的行为） |
| A5 | dc:141893 | `uVar2 & 0xffffffff` | core_b8i.c:304/310 | `result &= 0xffffffffU` | **PASS** |
| B1 | dc:141934 | `lstrlenW(local_res18 + iVar6*0x15b + 3)`（`undefined4*` → 字节 +0x56C·i +12） | core_b8i.c:341 | `lstrlenW((LPCWSTR)(blk + i * 0x15b + 3))` | **PASS** |
| B2 | dc:141944 | `memcpy(dst, local_res18 + iVar6*0x15b + 3, (len+1)*2)` | core_b8i.c:350 | `memcpy(dst, blk + i*0x15b + 3, (len+1)*2)` | **PASS** |
| B3 | dc:141935–141947 | 扩容 `FUN_140063720(out,len+3+pos)`；`dst=*out+pos*2`；非首项写 `\r\n` 且 pos+=2；末尾 pos+=len | core_b8i.c:342–351 | 同构（`PECMD_AllocString(out,len+3+pos)`、`dst[0]=0xd;dst[1]=10`、pos/dst 步进一致） | **PASS** |
| C1 | dc:141991 | `lstrlenW(local_res18[0] + iVar6*0x106 + 1)`（stride 0x106 dword = 0x418 字节，名槽 dword+1=字节+4） | core_b8j.c:171 | `lstrlenW((LPCWSTR)(blk + i*0x106 + 1))` | **PASS** |
| C2 | dc:141996–141999 | `\r\n` 写入 + `memcpy(dst+2, 条目名, (len+1)*2)` + `total=len+total+2` | core_b8j.c:173–178 | 同构 | **PASS** |
| C3 | dc:141974–141987 | 探测分配 `operator_new(0x418)`→free→按回调后的 `cb` 二次 `FUN_1400633a8`；空表用 `&DAT_14011c638`，否则名槽 `+1`(dword) | core_b8j.c:151–165 | 同序（探测→free→`FUN_1400633A8(&blk,cb)`→`g_szEmpty`/`blk+1`） | **PASS** |

### 1.4 分配器契约（+8 头）专项核对

任务给定契约：「分配返回 hdr+8，头为 {size, magic 0xaa55}，释放 HeapFree(ptr-8)」。dc 实证：

| dc 行号 | 内容 | src 文件:行号 | 结论 |
|---|---|---|---|
| dc:60694 | `HeapAlloc(DAT_14013d328,0,param_2+9)`（失败走 `FUN_1400630d0(2)` 重试环） | core_thread.c:49–53 | **PASS** |
| dc:60699–60700 | **先**写魔数 dword(+4)=0xaa55，**后**写 qword size(+0)=len+1 ⇒ 魔数高半部被覆盖（死存储），最终头 qword 仅含尺寸 | core_thread.c:54–59（含 T1c 注释，刻意保持原写入顺序） | **PASS** |
| dc:60698/60701/60703 | 返回 `plVar3+1`＝hdr+8；首字节清零 | core_thread.c:60–63 | **PASS** |
| dc:53639 | `HeapFree(heap,0,*param_1 + -8)`，随后 `*param_1=0` | core_string.c:163–169（`PECMD_FreeStrBuf`） | **PASS** |
| dc:53610 | `FUN_140102a90(p,0,p[-1])`＝按头 qword 清零 | core_thread.c:69–74（`PECMD_ZeroLenBuf`，读 `((size_t*)p)[-1]`） | **PASS** |
| dc:60886–60889 | `FUN_140063720`＝宽串扩容 `realloc(len*2+2)`、`p[len]=0`（RAS 列表拼接用） | core_string.c:96–104（`PECMD_AllocString`） | **PASS** |

RAS 半区内**没有**裸 ±8 算术——±8 全部封装在上述四个原语中；RASCONN/RASENTRY 结构内的 +4/+12 是字段偏移而非分配器头。#081 所述「+8 头偏移」关注点经全链核对一致。

### 1.5 挂断与拨号的偏移核对

| dc 行号 | 内容 | src 文件:行号 | 结论 |
|---|---|---|---|
| dc:141830–141843 | `RasHangUpW(*state)`；成功(0)则 blk[0]=0x234、t0=GetTickCount、保存句柄后清 *state、轮询 `RasGetConnectStatusW` 至 6 或 t0+3000 超时（Sleep 50ms）、再次 `*state=0`、返 0 | core_b8h.c:465–482 | **PASS** |
| dc:141847–141849 | 失败分支：`*state=0; _DAT_14013e298=0; 返 1` | core_b8h.c:484–486（`g_syncState`=DAT_14013e298 映射一致，core_b8h.c:84） | **PASS** |
| dc:144394–144411 | 拨号前置：旧状态等待清理、ResetEvent/CreateEventW 两事件、失败返 0 | core_b8m.c:790–805 | **PASS** |
| dc:144414 | `*(u32*)(param_1+8)=0x83c`（param_1 为标量 longlong ⇒ 字节偏移 8） | core_b8m.c:807 | **PASS** |
| dc:144423/427/431/435/439 | `lstrcpyW` 至 `param_1+0xC / +0x20E / +0x412 / +0x614 / +0x816`（均为标量加法＝字节偏移） | core_b8m.c:814/817/820/823/826 | **PASS** |
| dc:144440–144443 | `RasDialW(0,0,param_1+8,0,FUN_1400e4edc,param_1+0xA78)`；成功则 `DAT_14013e2a0=*(param_1+0xA78)`、`_DAT_14013e298=param_1` | core_b8m.c:832–837 | **PASS** |
| dc:144444–144456 | `WaitForMultipleObjects(2,{E1F8,E200},0,50000)`；==0 返 1；==1/0x102 时 `RasGetErrorStringW(DAT_14013e208,err,0x100)`＋`Error: %d - `拼错误串 | core_b8m.c:838–854 | **PASS**（wsprintfW 在 dc:144452 即缺实参，src 补 `0` 占位，符合项目既定约定 core_b8i.c:111） |

### 1.6 本轮新发现（非原 #081 范围，随复核产出）

#### N-1（DEVIATION，新增）：`"HelloASL"` 缺省入口名回退分支丢失

| dc 行号 | dc 逻辑 | src 文件:行号 | src 逻辑 |
|---|---|---|---|
| dc:144416–144420 | 入口名为空时：枚举电话簿（`FUN_1400e7840(local_228,0)`）；**先**置 `lpString2 = L"HelloASL"`；若枚举结果为 NULL 则跳 `LAB_1400ebe23` 直接 `lstrcpyW(param_1+0xC, L"HelloASL")` —— **仍会以 "HelloASL" 发起拨号** | core_b8m.c:808–813 | `src==NULL → goto done` —— **整体放弃拨号直接返回 0**；字面量 `HelloASL` 在整个 refactored 实现树 0 命中（仅 attic 原始摘录与 tools 提案 JSON 有记载） |

行为差异：电话簿为空/枚举失败时，原版拨叫缺省条目 "HelloASL"（RasDialW 将返回相应错误码并走 dc:144449–144455 错误串路径）；现实现不拨号。属控制流偏差（非地址算术），建议随下一修复批补齐。

#### N-2（缺口登记）：`FUN_140099f18` 的 WLAN 半区未实现

- dc 结构：RAS 半区 dc:96695–96963（已实现）；**WLAN 半区 dc:96964–97471（约 508 行）在 refactored 无任何副本**。
- 未实现内容包括：wlanapi 九函数惰性加载块（dc:96989–97006）；接口枚举与索引定位（元素算术 `local_c20 + i*0x85 + 2`，dc:97063/97076/97089）；`queryall/list/liston/scan/start+/stop` 各分支（dc:97077–97111）；XML 配置文件生成（`WLANProfile`/`WPA2PSK`/`AES`/hex SSID，dc:97166–97243+）。
- 直接后果：`src\device\core_b3r_h2.c` 的 `PECMD_WlanManage` 在 `-wlan` 匹配时（`local_c27 != 0`）越过唯一实现的 RAS 分支直达函数尾（core_b3r_h2.c:2266→2370–2371），**返回未定义值**。
- 文件头自述「WLAN 管理命令 (-wlan)」（core_b3r_h2.c:8）与实际覆盖正好颠倒：现体只含 ADSL/RAS 半区。

#### N-3（文档 nit，P3）

core_b8i.c:307 注释「名字槽 dword 算术 dc:141889 一致」引用行号不准：名字比较 `lstrcmpiW` 在 **dc:141896**（dc:141889 是循环计数取值 `iVar3=(int)uVar4`）。语义无误，仅引注需更正。

---

## 2. 复核项二：#083 TokenizeExpression 尾段缺失交叉核对

### 2.1 待核对断言（round-1 原文要点）

> 「dc:103884–104028 区域是 TokenizeExpression 尾段约 690 行缺失的证据区」

### 2.2 函数边界实测（dc）

| dc 函数 | dc 行范围 | 体量 | 归属/说明 |
|---|---|---|---|
| `FUN_1400a53e4`（size 2456）＝ **TokenizeExpression** | 头注 103346；定义 103349；闭括号 **103731** | 正文 ~383 行 | 大表达式/令牌处理器（`{}`/引号/续行符/变量/子脚本递归） |
| `FUN_1400a5d7c`（size 240）＝ PECMD_BuildQuickCommand | 103735–103765 | ~31 行 | 快捷命令串构造 |
| `FUN_1400a5e6c`（size 415）＝ PECMD_ParseCommaParams | 103766–103825 | ~60 行 | `,` 分隔参数解析 |
| `FUN_1400a600c`（size 1566）＝ **PECMD_CreateMenuItem** | 103826–**104040** | ~215 行 | MENU/BAR 菜单项构造 |
| `FUN_1400a662c`（size 582） | 104041–104131 | — | — |

**判定一（起始/结束行号）**：TokenizeExpression 的 dc 范围是 103346–103731（次函数头 103735）。所引「证据区 103884–104028」**完全落在 `FUN_1400a600c`（CreateMenuItem）体内**，与 TokenizeExpression 相隔两个完整函数。

### 2.3 dc:103884–104028 的真实内容与逻辑形态

该窗口是 **CreateMenuItem 的菜单构建尾段**，逐段实据：

| dc 行号 | 内容 | 逻辑形态 |
|---|---|---|
| 103870–103878 | `FUN_14001b3a0` 兜底初始化、失败码 `-0x7ff8ffa9` | 错误路径 |
| 103879–103905 | `GetMenu`；`operator_new(0x48)` 项对象及 4 个串槽初始化；`FUN_140070398/07034c` 填充；标志字节合成（负值 `|-1`、`-`→bit2、`\|`→bit1） | 顺序构造 + 标志位分支 |
| 103915–103931 | 0x48 步长项表扩容（`FUN_140063424(pcVar13+8,(n+1)*0x48)`）、`.Enable` 变量写（`FUN_1400629b8`/`FUN_14007d0ac`）、项指针入表、计数 ++ | 顺序 + 内层条件 |
| 103932–103991 | 子菜单分支（cVar1/cVar2 标志）：`CreateMenu`、`FUN_14002fd88` 取标题、`operator_new(0x10)` 弹出节点、`operator_new(0xf0)` 子脚本＋`FUN_140017cdc/0186bc/073ccc` 初始化、`FUN_14004e488/066838/066f64/04eaa8` 注册清理、`CreatePopupMenu`＋`FUN_1400e5248` 建树、`FUN_1400e4f14` 挂菜单 | 嵌套 if/else 多分支 + 错误早退（返 1） |
| 103993–104016 | 顶层单/多项收尾：`FUN_14005b900`、`LAB_1400a6576/a657f` 分派 `FUN_1400e5120`、`SetMenu`＋`FUN_14005db98(…,3,…)` | switch 尾分支形态（两标志四象限） |
| 104017–104028 | else：`SetMenu(NULL)`、`thunk_FUN_140064b78`＋`free` 释放旧表、`param_1[3]=0` | 清除路径 |

即：**不是循环、不是 TokenizeExpression 的任何尾段**，而是菜单命令的构造/清除收尾（嵌套分支 + 错误路径 + 类 switch 的标志分派尾）。

### 2.4 refactored 现状与衔接

| 对应 dc 函数 | refactored 实现 | 与缺口关系 |
|---|---|---|
| `FUN_1400a53e4` TokenizeExpression | `src\commands\core_b3_remaining.c:24757–25171`（~415 行） | **完整覆盖**，见 2.5 对照 |
| `FUN_1400a5d7c` BuildQuickCommand | `core_b3_remaining.c:25173`（`@0x1400a5d7c size=240`） | 已实现 |
| `FUN_1400a5e6c` ParseCommaParams | `src\commands\core_b3r_h4.c:555` | 已实现 |
| `FUN_1400a600c` CreateMenuItem（含被引证据区全部内容） | `src\commands\core_b3r_h4.c:720` 起；实测含 `GetMenu`(≈773)、`operator_new(0x48)`、0x48 步长扩容、`.Enable`、`CreatePopupMenu`、`SetMenu`、`LAB_1400a6576/a657f/a65de/a65eb` 全部标记 | **被引「证据区」内容已在其正确归属函数中实现** |

### 2.5 TokenizeExpression 覆盖结构对照（dc ↔ src）

| 结构要素 | dc 行号 | src（core_b3_remaining.c） | 结论 |
|---|---|---|---|
| 前导跳过循环 + `LAB_1400a5424` | 103393–103405 | 24801–24814（label 24808） | PASS |
| 第二跳过循环；空指针早退 `return 1` | 103406–103417 | 24815–24828 | PASS |
| 标志 `&= 0x1ef`；`operator_new(0xf0)` 子脚本＋`FUN_140017cdc/0186bc`、`+0x40` 清零 | 103419–103433 | 24830–24844 | PASS |
| `local_80` 选择（本体或子脚本）；`+0x40` 传递；Enter/Leave CS | 103434–103441 | 24845–24852 | PASS |
| 参数5 → `FUN_14007a224`；现场保存 0x24/0x28/0xC8/0x60/0x68 | 103451–103459 | 24862–24870 | PASS |
| `FUN_140063620(&local_70)`；local_74∧bVar5 时换入 `+0x70` 槽＋`AllocString(0x10)` | 103460–103473 | 24871–24884 | PASS |
| bVar5 ? `FUN_140073ccc`（参数表构建） : 复制 0x68/0x60 | 103474–103480 | 24885–24891 | PASS |
| 引号字符(+0x96)分支：跳过、模式匹配 `FUN_14006156c`(0xaa/0x9a)、**递归调用①**（空参 `&DAT_14011c638`） | 103490–103527 | 24901–24943（递归 24932） | PASS |
| 闭引号(+0x98)分支：内层双 do-while、深度 --、`^0x21` 逃逸检查 | 103528–103544 | 24944–24961 | PASS |
| 续行对(+0xBA/+0xBC)分支：三段扫描、`FUN_1400702d4` 截串、递归②（携带 local_58）、`goto LAB_1400a598c` | 103545–103594 | 24962–25018（递归 25004，goto 25015） | PASS |
| 兜底扫描/推进（103595–103615） | 同左 | 25019–25040 | PASS |
| 变量令牌分支 `FUN_14006213c` | 103616–103623 | 25042–25050（映射 `PECMD_MatchClosingBracket`） | PASS |
| 表达式分支：`FUN_14001b23c` 抽取＋死调用 `lstrlenW`＋`FUN_14006e030`＋`FUN_14004c0bc` 执行＋`(p+0x11)&1` 清理 | 103624–103670 | 25051–25100（映射 ExtractTableSegment/CopyPathToken/ProcessScriptBlock/ZeroLenBuf） | PASS |
| `LAB_1400a598c`: 释放＋推进；主 do-while 条件 | 103671–103680 | 25101–25117（label 25110） | PASS |
| 收尾跳过循环＋`iVar15=0`；(param_4&1) 否则路径 `FUN_140025474` | 103682–103696 | 25119–25135（映射 `PECMD_ParseCommandBlock`） | PASS |
| 尾段：标志合并 `(|cVar2)&0x1ef`、`+0xCA→+0xD0` 透传、local_74 现场**恢复**、子脚本 `FUN_14004eaa8`+free、`local_20` 经 CS 保护 `FUN_140066838/066f64` 通知、`return` | 103697–103731 | 25136–25171 | PASS |

**判定二（缺的是什么逻辑形态）**：就 TokenizeExpression 而言**什么都不缺**——循环主体、两类递归、switch 尾分支、错误路径、状态恢复与清理尾全部在场。

### 2.6 「约 690 行」数字来源推断（供归档）

从 FUN_1400a53e4 头注（103346）连数到被引窗口末端（104028）≈ **683 行**；到 FUN_1400a600c 结束（104040）≈ **695 行**。round-1 很可能忽略了 103735/103766/103826 三个函数头，把 TokenizeExpression 之后三个独立函数的正文一并当作其「尾段」，从而得出「~690 行缺失」，并把其中一段（103884–104028）截作「证据区」。三个函数各自均有 refactored 实现（见 2.4），故该 DEVIATION 应予**撤销**。

---

## 3. 修复建议优先级

| 优先级 | 事项 | 位置 | 说明 |
|---|---|---|---|
| P1 | 补 `"HelloASL"` 缺省入口名回退（新发现 N-1） | `src\ui\core_b8m.c:808–813` | 按 dc:144416–144420：枚举 NULL 时以 `L"HelloASL"` 继续 `lstrcpyW(conn+0xC,…)` 并照常拨号；同时移除「枚举失败即 goto done」的提前退出 |
| P1 | 登记 WLAN 半区缺口（新发现 N-2） | `src\device\core_b3r_h2.c:1965–2371`（对应 dc:96964–97471） | 至少：函数头/TODO 注明「-wlan 半区未实现，现路径返回未定义值」；后续批次按 dc 补 wlanapi 加载块、接口枚举（`i*0x85+2` 字节=dword `i*0x214+2`？注意 `local_c20` 为 `int*`，`+i*0x85+2` 即字节 `i*0x214+8`，移植时按 dc 类型核算）及 XML profile 生成 |
| P2 | 撤销/关闭 #083 并归档本轮边界证据 | 分析档案 | 以本报告 §2 替换 round-1 断言；防止后续轮次重复排查 |
| P2 | 更正 @0x140099f18 的命名与描述（ADSL/RAS 而非 WLAN） | `core_b3r_h2.c` 文件头第 8 行、`FUNC_NAMES.md`/`REVIEW.md` 相应行 | 名称可保留但描述需改为「ADSL 动词处理器（RAS 拨号半区已实现；-wlan 半区未实现）」 |
| P3 | 修正引注 dc:141889 → dc:141896（N-3） | `src\ui\core_b8i.c:307` 注释 | 纯注释更正 |
| P3 | #081 关闭说明补记前提勘误 | batch-A 台账 | 原「FourCC 0x00534152 ('RAS')」定位描述改为「FourCC 0x4C534441 ('ADSL')，dc:44655→FUN_140099f18 RAS 半区」 |

---

## 4. 复核方法附注

- 全部行号为本次实读核验（read/grep/pwsh 只读），关键区间均整段读过：dc:96604–97471、103346–104040、141850–142005、141814–141855、144377–144460、60681–60707、53603–53676、60878–60917、44130–44210、44590–44709；src 侧 core_b8i.c 全文、core_b3r_h2.c 相关段、core_b8j.c/core_b8m.c/core_b8h.c/core_thread.c/core_string.c 相关段、core_b3_remaining.c:24690–25174、restored_bodies.c 定位点。
- 未发现任何 'RAS' FourCC/字符串存在于 dc（grep `0x534152`、`534152`、`L"RAS"` 均 0 命中），前提勘误据此成立。
