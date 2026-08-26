# S16 — PECMD_RegiEditRegistry (REGI) 全文逐段移植审计

| 项 | 值 |
|---|---|
| 审计对象 | `PECMD_RegiEditRegistry`（原版 `FUN_1400c13f8` @0x1400c13f8，REGI 注册表引擎） |
| 参考原文 | `decompiled.c` dc:119655–120918（size=8911 机器码行；C 体实际止于 120918，其后为 FUN_1400c36c8） |
| 移植体 | `refactored/src/ui/core_b7c.c:550–1887`（函数头注释 @0x1400c13f8） |
| 已知勿重报 | R14（s_70ctx 三元组，已修）；FUN_14007f6e4(SplitTokenAssignVar) 调用形态本身 |
| 方法 | 两遍通读原文建分支地图 → 与移植体逐分支对照（控制流/常量/API 序/栈槽/返回值）→ 差异定位与最小修复草案 |
| 约束遵守 | 只读源码；仅新建本报告；未改 src/**、未构建、未 git、无 windbg/Ghidra 会话 |

---

## 一、分支地图（原文，dc 锚点）

1. **序言/状态初始化** dc:119744–119759（local_res8=res10=res18 存参；s_70 槽分配；pass_cnt=−1 等）
2. **前置 `---initram` 检测** dc:119760–119762 → 命中即 FUN_14001b888(3)（dc:120768）
3. **每行循环头** dc:119763–119784：prefix 清零、want_type=0xFFFFFFF000000000、引导符 cVar6（`*`/行首`-`）、各选项标志清零
4. **选项词 while** dc:119785–119927：要求**成对引导符**（cursor[0]==cursor[1]==cVar6 且非 H/h/空白），从**第二引导符处**起比对选项词：
   `"0"`(死分支)→`"t:"`(死分支)→`"16","r10","byte","a","av","ak","name","v","v-","k","qk","init","su"`→缺省掠过整词
5. **类型前缀字符** dc:119935–119947：n/u/b/^/+/#/@/$/*/~/./? → prefix，消费 1 wchar
6. **二次标记 mark2** dc:119948–119955：`(prefix!=0 && *FUN_140063060(prefix)==*cursor) || *cursor=='$'`（FUN_140063060 为恒等桩 @dc:60483 ⇒ 实为**原始 prefix 字节**直接比较）；大写归一小写 dc:119956–119958
7. **UNC 跳过** dc:119959–119966：StrStrW(cursor,`\\\\`) 则 subp=命中+2
8. **分隔符扫描** dc:119967–120005：自 subp 找 `,` / `=` / 尾部 `!`；`,` 命中后先找其后 `=`（有则分隔符改为该 `=`），无则找尾 `!`
9. **命中分隔符** dc:120006–120013：终止串、a0=数据区、分隔符存 local_res10 低字节（移植体 delim_keep）；未命中 → LAB_1400c1c02 两遍重扫
10. **键路径拆分**
    - 分支 A（无任何 `\`，含首逗号字段展开重查）dc:120014–120048：SplitTokenAssignVar(s_70,',')→trim→f0；再按 `\` 拆 subkey/valname；aflags==0 取最后 `\`、aflags!=0 取串尾空段
    - 分支 B dc:120049–120062：最后一个 `\` 处断开；**ExpandVarDispatch(script, pHVar36[路径首], &f0)** 与 **(script, lastbs+1, &b8)** 双侧展开
11. **LAB_1400c1f01** dc:120063–120072：值名跳过前导 `\`；name 选项时 FUN_140079c80(script,&a0,&a8)
12. **`,` 查询侧** dc:120074–120087：拆变量名→FUN_140003a20 注册；prefix=='.'且续`,`→第二变量 e0；缺省名 "RegDat"
13. **`=` 写侧数据准备** dc:120088–120139：空数据×prefix($/*/~)/k_cnt 矩阵；StrAssign(s_50)+trim 视图(local_110)；pass_flag 分支 ExpandCommandLine/直赋；`""` 比较→DAT_14011c638；内嵌 `#`/`@` 重解析
14. **根键解析** dc:120140–120183：HKLM/HKCU/HKCR/HKU/HKCC 全名表 → 0x80000002/1/0/3/5；否则 FUN_140067cf4(root_slot) 数字句柄，失败→释放 a8→LAB_1400c1dd0
15. **无输出变量（写/删）路径** dc:120184–120208：删键 FUN_1400717f0 / scrub FUN_140071ae8(cnt_b0)；删值 FUN_14005c61c；RegCreateKeyExW(…,0x2001B,…)+Close；ERROR_FILE_NOT_FOUND(2)→0
16. **写值类型分派** dc:120210–120433：v/k 预建键(FUN_14005f750)及 v_cnt>1‖k_cnt 提前返回；`+`QWORD(byte/16hex/FUN_1400745c8 表达式)；`#`DWORD(dec/hex16/byte)；`b`BIG_ENDIAN(解析+4 字节翻转)；`*`MULTI_SZ(拷贝循环 LAB_1400c324c + 补 NUL LAB_1400c32f3)；`~`EXPAND_SZ(`\0` 转义拷贝)；`@`BINARY；`n`NONE；`^`LINK；`u`MUI；缺省 SZ；内嵌前缀(+,#,*,~,@)；byte 公共入口 LAB_1400c2f9a；E_INVALIDARG(LAB_1400c30bf)/retv=1(LAB_1400c33b0) 出口
17. **查询路径** dc:120435–120884：`.`/`?` 标记(bVar8/bVar4, flg_88)；缓冲分配(110=100、120=0x1000、d4=0xfff)、outsel 选择(f8↔110)；枚举分支(aflags)：RegOpenKeyExW(…,9,…)+双 RegQueryInfoKeyW+RegEnumValueW/RegEnumKeyExW+`\r\n`/`v `/`k ` 标签拼接；读值 FUN_1400690c0；NI/ERROR/su 错误分支；期望类型核对(DVar35)；各类型类型名输出+失配门(0x80070057)；QWORD/DWORD/BIG_ENDIAN 格式化(r10→%I64u/%lu)；EXPAND_SZ mark2 再展开；MULTI_SZ 还原循环(joined_r0x0001400c2b93)；二进制族 hexdump(out_off=1)；未知类型 typeid
18. **查询写回** dc:120863–120884：FUN_1400629b8(script,108,110+out_off*2) 与 (script,e0,*outsel)，返回 status_q
19. **写入循环 LAB_1400c34b3** dc:120832–120856：qk==0→FUN_14005c5a0(返回丢弃,DVar13=0)；qk→间接调用 DAT_14013d3f0(SHSetValueW 族)；==0x3FD 且 !v_cnt 时置 v_cnt=1 + FUN_14005f750(…,1) 重试
20. **两遍重扫/initram** dc:120744–120772：pass_cnt++>0→0x80070057；ExpandVarDispatch(script,param_2[原始行],98) 后重检 `---initram`
21. **收尾释放图**：LAB_1400c33b5(8 槽)→1cd1(98,70)；2847/错误/su(12 槽)；2d88(12 槽)；3517(10 槽)；1d02(5 槽)

---

## 二、发现清单

严重度定义：**AV** = 可致访问违例/崩溃；**行为错** = 不崩溃但写入/返回结果与原文背离；**轻微** = 边角输入下的行为差异或潜在隐患。

### R1【高 / AV】脚本上下文实参传址错误：`&script` 应为脚本对象本体

- 【dc 锚点】dc:120077 `FUN_140003a20((longlong *)pHVar30,&local_108,0);`、dc:120082（同型）、dc:120070 `FUN_140079c80((longlong *)pHVar30,…)` —— pHVar30 = local_res8 = **脚本对象指针按值传递**。佐证：FUN_140003a20 体（dc:1248–1249）直接解引用 `(longlong)param_1 + 0xda / + 0xd` 为对象字段。
- 【移植侧】`core_b7c.c:932`、`core_b7c.c:937`（活代码：凡 `,` 查询路径必经 932）；`core_b7c.c:925`（FUN_140079C80，同病，当前因 OBS-1 选项词不可达而处于死路径，仍须一并修复）。
- 【偏差描述】三处把**局部槽地址** `&script` 当作脚本身传入。`b7c_003a20`（core_b7c.c:6047，忠实直移 FUN_140003a20）读 `*(char*)((intptr_t)script + 0xda)` —— 传入 `&script` 时读到的是**本函数栈帧 +0xDA 处的随机字节**，据此误选单/双遍展开分支，并把 `&script` 继续传给 `PECMD_ExpandVarsRecursive/PECMD_ExpandCommandLine` 作 ctx；二者按对象布局取字段（如 +0x1A0 变量表指针），解引用栈垃圾 → AV 风险高（与已修 R14 同族：REGI 族崩点）。同文件 TABL 段 core_b7c.c:6213 的正确写法 `b7c_003a20((longlong *)a1,…)` 可为对照。
- 【触发面】任何带输出变量的 REGI 查询（`REGI HKLM\…\Val,Out`）—— 主路径。
- 【最小修复】（三处独立可匹配）

old（core_b7c.c:925，唯一）:
```c
                FUN_140079C80((int64_t *)(intptr_t)&script, (int64_t *)&a0, (int64_t *)&s_a8);
```
new:
```c
                FUN_140079C80((int64_t *)(intptr_t)script, (int64_t *)&a0,
                              (int64_t *)&s_a8); /* R1: 传脚本对象本体(dc:120070) */
```

old（core_b7c.c:932，唯一）:
```c
                b7c_003a20(&script, (WCHAR **)&s_108, 0);
```
new:
```c
                b7c_003a20((longlong *)(intptr_t)script, (WCHAR **)&s_108, 0);
```

old（core_b7c.c:937，唯一）:
```c
                    b7c_003a20(&script, (WCHAR **)&s_e0, 0);
```
new:
```c
                    b7c_003a20((longlong *)(intptr_t)script, (WCHAR **)&s_e0, 0);
```

### R2【行为错】写侧 MULTI_SZ 拷贝循环 cp_sep 计数语义不符 → 缺第二终止符、cbData 短 2 字节

- 【dc 锚点】dc:120792–120793（普通字符每轮 `pHVar30 = pHVar27` —— 计数复位）、dc:120784–120787/120804–120805（仅转义字符 `\0` 嵌入与 `$` 模式 CRLF→NUL 走 `LAB_1400c32c5: pHVar30++`）、dc:120807 `DVar13=(DWORD)pHVar30`、dc:120809–120827 收尾补 NUL（<1 补 1 个；len>0 且 <2 再补至 2 个；DVar24=pend−buf 覆盖**两个**计数 NUL）。pHVar27 全程不变量 0 ⇒ DVar13 实际取值：普通字符后=0，转义后=prev+1 ∈ {1,2}。
- 【移植侧】`core_b7c.c:1705–1779`（LAB_1400c324c … b7c_finish_pad）。
- 【偏差描述】移植体在 `b7c_c32c5: cp_sep = cp_sep + 1;` 处让**所有**路径汇入：`*`/无标记模式下每个普通字符都使 cp_sep++ ⇒ 非空数据结束时 cp_sep=字符数≥2，finish_pad 两个补齐分支全被跳过，仅剩 `*pend=0` 单终止符，DVar24 相应短 2 字节；`$` 模式虽跳过普通字符（:1746 goto b7c_copy_adv），但转义计数**累加不复位**，与原文"普通字符复位"语义不符。后果：写入注册表的 REG_MULTI_SZ 为单 NUL 结尾的非规范串（regedit/多数 API 期望双 NUL），cbData 与原版差 2 字节。空串路径（cp_sep=0）恰好一致，故仅非空数据出错。触发面：`REGI …=*abc` 类型前缀 `*` 或值首内嵌 `*`（均为活代码，不经选项词）。
- 【最小修复】四处 `goto b7c_c32c5;`（:1716/:1728/:1742/:1750）统一改 `goto b7c_esc;`；汇合点标签改为复位；新增 esc/c32c9 两标签。

old（replace_all，恰 4 处）:
```c
goto b7c_c32c5;
```
new:
```c
goto b7c_esc;
```

old（core_b7c.c:1746，唯一）:
```c
                        goto b7c_copy_adv;
```
new:
```c
                        goto b7c_c32c9; /* R2: '$' 普通字符亦须复位计数 */
```

old（core_b7c.c:1753–1755，标签定义处，唯一）:
```c
        b7c_c32c5:
            cp_sep = cp_sep + 1;
        b7c_copy_adv:
```
new:
```c
        b7c_esc: /* R2: 仅转义字符(嵌 NUL / CRLF→NUL)计数+1 (dc:120784-787,120804-805) */
            cp_sep = cp_sep + 1;
            goto b7c_copy_adv;
        b7c_c32c9: /* R2: 普通字符每轮复位, 直移 dc:120793 pHVar30=pHVar27(不变量0) */
            cp_sep = 0;
        b7c_copy_adv:
```

修复后与原文逐案对齐：末字符普通⇒cp_sep=0→补1+补1（计数 2 个 NUL）；末字符单转义⇒1→补1；连续双转义⇒2→不补；空串⇒0（循环未进入）→补1+跳过第二补齐。finish_pad 本身无需改动。

### R3【轻微】分隔符扫描：中途 `!` 后恢复点比原文早 1 wchar

- 【dc 锚点】dc:119994–120004：`!` 后余串非空时 `pHVar30 = local_res10`（裁剪后位置，经 `&local_res10[-1].unused + 2` 恒等回写），随后公共尾 `pHVar30 += 1 wchar` ⇒ 恢复点 = 裁剪后位置 **+1**（多跳过一个字符）。
- 【移植侧】`core_b7c.c:844–851`：`sp = after - 1;` + 公共尾 `sp = sp + 1;` ⇒ 恢复点 = 裁剪后位置（少跳一个）。
- 【偏差描述】仅当 `!` 后紧随的另一字符本身是 `,`/`=`/`!` 时可见：如 `V!=x` 原文错过该 `=`（继续扫到串尾→走两遍重扫/报错），移植体则以其为分隔符（删除名为 `V!` 的值）；`!!` 结尾同理分叉。常规值名不含 `!`，实际影响窄。
- 【备注】原文 `&local_res10[-1].unused + 2` 系 Ghidra 混型惯用式，存在反编译失真可能；本审计按保守原则以字面语义为准并在此注明。
- 【最小修复建议】

old（core_b7c.c:849，唯一）:
```c
                            sp = after - 1;
```
new:
```c
                            sp = after; /* R3: dc:119999 直移 —— 恢复点=裁剪后位置, 公共尾再 +1 */
```

### R4【轻微】分支 B 展开起点用 UNC 跳过点而非路径首

- 【dc 锚点】dc:120051 `FUN_14007bf44((longlong *)local_res8,(WCHAR *)pHVar36,&local_f0,0,1)` —— pHVar36 = **键路径首**（dc:119960 重置后的游标）；截断点 `*pWVar16=0` 在最后一个 `\`。
- 【移植侧】`core_b7c.c:900–904`：`PECMD_ExpandVarDispatch(…, subp, …)` —— subp 是 StrStrW(cursor,`\\\\`) 命中 +2 的跳过点。
- 【偏差描述】当键路径区段内出现连续双反斜杠时，原版把 `\\\\` 及其之前的内容一并纳入根名展开源串，移植体将其剔除；两者截断终点相同，仅起点不同。无 `\\\\` 的常规输入完全一致。
- 【最小修复建议】

old（core_b7c.c:901–903，唯一）:
```c
                WCHAR *lastbs = pWVar16; /* 最后一个 '\\' 处分界 */
                *lastbs = L'\0';
                PECMD_ExpandVarDispatch((void *)(uintptr_t)script, subp, (WCHAR **)&s_f0, 0, 1);
```
new:
```c
                WCHAR *lastbs = pWVar16; /* 最后一个 '\\' 处分界 */
                WCHAR *expbase = cursor; /* R4: dc:120051 展开起点=路径首(pHVar36), 非 UNC 跳过点 */
                *lastbs = L'\0';
                PECMD_ExpandVarDispatch((void *)(uintptr_t)script, expbase, (WCHAR **)&s_f0, 0, 1);
```

---

## 三、特别核对项结论（任务指定）

| 项 | 结论 |
|---|---|
| SHSetValueW/SHGetValueW 分支(qk) | 写侧 qk 经 `g_pSHSetValueW` 六参重构（:1791，TODO 变参还原合理）：root/sub/val/type/data/cb 与 dc:120839 `(*DAT_14013d3f0)()` 同参；0x3FD 重试+v_cnt=1+RegCreateKeyRetryWow64(:1795–1798) 对齐 dc:120841–120843 ✓。读侧 SHGetValue 未在本函数出现（读值走 FUN_1400690C0，签名六参与 dc:65272 一致）✓。qk 置位依赖选项词，见 OBS-1。 |
| 枚举分支(a/av/ak/name) | 结构对齐：access 9、双 RegQueryInfoKeyW、max(dmx1,dmx2)*4+0x20 缓冲、`\r`+`v `/`k ` 标签、双标志时名字写点后移、bit0=值/bit1=子键、成功才 AppendWideStr ✓。移植体将原版"标签 pos[4..5]+名字 pos[8]（间隙依赖零初始化）"压缩为连续 [2]='v',[3]=' ',[4]=写点，语义等价（OBS-4）。aflags/name 置位依赖选项词，见 OBS-1。 |
| '.'/'?' 输出 | flg_88 门控类型名、outsel 选择（'.'→s_f8 第二变量）、out_off=1 hexdump 偏移、双写回 108(+off) 与 e0(*outsel)、"RegDat" 缺省均一致 ✓ |
| ---initram 触发 | 前置检测与两遍重扫后检测均对齐 dc:120765/120768；命中 `PECMD_InitRamdataRegistry(3)` ✓；两遍超限 0x80070057、重扫源=原始行(param_2/local_78→s_78) ✓ |
| 根键句柄数字形式(FUN_140067cf4) | PECMD_ParseNumSkipWs(&rp,&numv)，失败或 0 → Free a8 → LAB_1400c1dd0；成功 root_key=numv ✓（原文后续 local_100=local_120 被 :1052/root_slot=root_key 等价覆盖，无行为差）✓ |

## 四、已核对一致的其余要点（抽样列举）

- 选项词表顺序与效果映射（16/r10/byte/a=3/av|=2/ak|=1/name/v±/k/qk/init/su/缺省掠词）逐一相符（尽管见 OBS-1 可达性存疑）；
- 类型前缀字符集、mark2 判定（FUN_140063060 为恒等桩 ⇒ 原始字节比较，移植体 `prefix == *cursor` 等价）、大写归一 `(byte)(x+0xbf)<0x1a`；
- 删除族 FUN_1400717f0/140071ae8/14005c61c、RegCreateKeyExW 0x2001B、2→0 映射；
- 写侧各类型 wtype/DVar24 组合（+:=0xB/8、#=4/4、b=5/4、*=want_type/计算长、~=遗留 2/计算长、@=3、n=0、^=6、u=0x15、缺省 SZ=1、byte 入口）与十六进制解析 0x 前缀剥离、hexdigit 门控、E_INVALIDARG 出口；
- `~` 写拷贝 `\0`→NUL（'0' 字符随后照抄——双方一致的原文怪癖）、长度 =(dst−buf)+2；
- 查询侧 MULTI_SZ 还原循环（`*` 转义、NUL→`\0`、CRLF、dst 步进 +1/+2 区分）逐步等价；
- 各出口释放槽集合（33b5/2847/错误/su/2d88/3517/1d02/1cd1）与返回值截断转换全部一致；
- g_szEmpty ↔ DAT_14011c638（全局空串）✓。

## 五、SKIP / 存疑登记

| 编号 | 内容 | 原因 |
|---|---|---|
| OBS-1 | **选项词在原文中即不可达**：while 条件强制 cursor[1]==cVar6('‑'/'*')，而选项比较恰始于 cursor[1]，故 "16"/"qk"/"a"/"name" 等永不命中（`--qk` 整词被缺省分支掠过）。移植体同构同位，双方一致——非移植缺陷；怀疑反编译层失真（真实二进制或经 FUN_140066148 式"模式+游标各跳 1"辅助匹配，见 dc:62844）。建议后续以动态跟踪核验选项语法，勿据本文单独改移植体。 | 反编译歧义，静态无法裁决 |
| OBS-2 | 死分支 `"0"`/`"t:"` 的比较基址移植体用 optp（第一引导符），原文用第二引导符；两处均不可达，无行为影响。 | 死代码，仅记录 |
| OBS-3 | wsprintfW 变参三处还原（DWORD/QWORD 值参、未知类型 typeid、hexdump 步进取 wsprintfW 返回宽度）——原文变参被 Ghidra 丢失/步进常量自相矛盾（+6 或 +18 字节均不合理），移植体 TODO 选择最合理解释。无法构建/调试验证。 | 需动态验证 |
| OBS-4 | 枚举缓冲布局重排（连续标签+写点 vs 原版跨 8 字节索引+零填充间隙），语义等价；依赖 AllocWStringBuffer 零初始化与否不影响移植体自身。 | 设计等价替代 |
| OBS-5 | 移植体在 '@'/'n'/'+'/'#' 分支额外同步 want_type（原文仅写 uVar31/wtype）；write 路径不读 want_type（'*' 收尾示 uVar31=local_90 处移植体亦显式 wtype=want_type=7 对齐），惰性冗余无害。 | 无行为差 |

## 六、统计

- **审计分支数**：54（分支地图第 1–21 大类细分，覆盖 dc:119655–120918 全部控制流汇点与两条收尾链）。
- **发现计数**：
  - 高 / AV：**1**（R1，含 2 处活代码 :932/:937 + 1 处死路径 :925）
  - 行为错：**1**（R2，写侧 MULTI_SZ 终止符/cbData）
  - 轻微：**2**（R3 分隔符 `!` 恢复点 off-by-one；R4 分支 B 展开起点）
  - 一致确认项：22 组；SKIP/存疑：5（OBS-1..5）
- 报告产物：本文件（唯一编辑物）。未改动任何源码/构建/git。
