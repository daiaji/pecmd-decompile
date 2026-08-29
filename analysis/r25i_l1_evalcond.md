# R25-i · L1 语义化批次 1-C3 — PECMD_EvalLoopCondition @0x140032dc4

- 对象: `src/commands/core_b2f.c` 2659–3746 行（函数头注释 2659–2663 + 函数体 2664–3746；下一函数头 3748）。
  dc 对应 = `reference/decompiled.c` FUN_140032dc4（dc:30362–31375，size=6596，IFEX/FIND/TEAM 共享条件求值体）。
- 性质: 纯改名（Ghidra 占位名 → snake_case 语义名），零语句/类型/顺序/空白改动；函数头注释、
  R24/R25 定案引注注释、dc 引注内旧名全部原样保留（本函数注释仅提及保留名 UVar30/UVar19/UVar35/
  pWVar41/uVar20/probe_ui，与改名集零交集）。
- 方法: python `\bold\b` 全字替换严格限 2659–3746 行；全行注释（stripped 以 `/*`、`//`、`* `、`*/`
  开头）跳过；48 个新名在 src/+include/ 全树 `\b` 预检 0 撞名（无 Windows 宏词）。
- 行数不变（8649 行）；git diff 281/281 对称（其中 2 对为 GNU diff 并 hunk 时的空行同文对
  L2714/L3099，内容逐字节相同，实际差异 279 行，全部落 2667–3744 界内）。
- C1 完工区 FUN_14003C06C（7180–7571）零触碰。

## 一、改名映射表（旧名 → 新名 | 证据 | 站点数；行号 = core_b2f.c 当前行号）

### 1a. 模式/标记旗（prologue 2721–2830）

| 旧名 | 新名 | 证据 | 站点 |
|---|---|---|---|
| uVar39 | mode1_flag | 2731 `(uVar3 == 1)`（flags&0xff==1 求值模式旗）；2734/2751 分支；2739 存入 local_148 | 5 |
| local_148 | mode1_cached | 2739 =mode1_flag 缓存；2857/2896/2913/3191 判 mode1；3373→bVar21（MEM 查询种类）；3487 GetDiskSpaceInfo 旗 | 8 |
| local_d0 | mode2_cached | 2823 `(uVar3 == 2)` 缓存（WCHAR* 视图）；3171 取出；3184/3312 判 mode2 | 4 |
| local_a0 | mode0_cached | 2825 `(uVar22)`（mode0 旗）缓存；3320 mode0 && 引号 → kind=str 判定 | 3 |
| local_c8 | f_case_sensitive | 2827 =uVar26(=flags&0x10000)；3592 !=0 走 StrCmpNW（区分大小写），==0 走 StrCmpNIW | 3 |
| local_c0 | flags_20000 | 2828 =uVar37(=flags&0x20000)；3247 condA 门（0 → 允许文件探测分支） | 3 |
| local_128 | env_form_marker | 2725 flags&0x40000；2773 头部 `*` 时置 0x2a；2832 →uVar26（直查 env 路径门 3071/3114） | 4 |
| local_1a4 | kind_marker_flt | 模式相关 kind 标记（0x24/0x7c/0x100003）；3326 `==uVar37` → PECMD_ParseParenthesizedExpression double 比较 | 10 |
| local_180 | kind_marker_int | 模式相关 kind 标记（'#'）；3290 →uVar26；3350 `==uVar37` → PECMD_EvalParenStripped 整数比较 | 7 |
| local_168 | kind_marker_str | 模式相关 kind 标记（0x24/0x7c）；`uVar37==local_168` → 3568 起字符串比较路径 | 11 |
| local_158 | head_prefix_char | 条件头标记字符宽值：2758（mode2 '|'）、2774（'*' 情形）、2782 =头字符；2836/2841/3058 转发 | 9 |
| local_1a7 | head_prefix_byte | 同上字节视图：2772/2783 置位；3312 `==0` 判「无头标记」 | 5 |
| local_100 | cond_neg_flag | 2744 =序言 FUN_1400660AC("!") 跳过结果；3728 !=0 → 对 CalcExpression 结果取反 | 3 |
| local_104 | clause_neg_flag | 2838 =子句级 '!' 跳过结果（LAB_1400330ae 每轮）；3684 取反分派 | 3 |
| local_108 | char_eq | 2822 =0x3d '='；2925 左词扫描 op 字符判定 | 3 |
| local_fc | char_amp | 2824 =0x26 '&'；2930 同上 | 3 |
| local_f8 | char_pipe | 2748/2824 链 =0x7c '|'；2930 同上 | 3 |
| local_e0 | char_at | 2826 =0x40 '@'；2931 同上 | 3 |

### 1b. 词条/值/缓冲槽

| 旧名 | 新名 | 证据 | 站点 |
|---|---|---|---|
| local_190 | lhs_buf | 构建/展开后的 LHS 文本缓冲：3075/3090 env 值追加、3094–3116 FUN_14007BF44/FUN_14007A224 展开、3145 AllocString、3161 StripTrailingSpaces、各出口 FreeStrBuf（3360/3563/3735/3744） | 25 |
| local_130 | op_tail_buf | 运算符位起的尾串副本（2865 StrBldCopyWide 自 op 字符；3042 StrCopyW UVar35→local_170），原文本随后在 op 位写 NUL | 12 |
| local_138 | rhs_exp_buf | RHS 侧展开临时缓冲：3100/3123 FUN_14007BF44 展开、3106/3124 追加进 lhs_buf、3359/3562/3734/3743 释放 | 14 |
| local_a8 | name_exp_saved | 3096 =lhs_buf 首遍展开结果快照（local_190 复位前暂存）；3111 FreeStrBuf | 3 |
| local_b0 | tail_exp_saved | 3102 =rhs_exp_buf 快照；3109 FreeStrBuf | 3 |
| local_118 | rhs_num_cursor | 3380–3384 RHS 数值解析游标（PECMD_ParseAndSkipSpace_7f90/FUN_14006A7F4/判 '.'）；3508 判 '.' | 7 |
| local_120 | rhs_dbl | 3381 ParseAndSkipSpace 解析出的 RHS double；3408–3412/3511–3513 双精度比较 | 10 |
| dVar45 | lhs_dbl | 3404/3507 LHS 换算 double（MEM `>>0x14` MB + g_dbl21630 小数）；3408–3513 与 rhs_dbl 比较 | 13 |
| bVar42 | dbl_nan_flag | 3409/3512 `isnan(lhs_dbl)||isnan(rhs_dbl)`；3412/3514 NaN → 结果 -1 | 4 |
| bVar44 | cmp_bool | 3410/3513 double 相等；3598 字符串相等；3685/3688 取反后子句布尔；3690 写入真值 | 9 |
| local_140 | lhs_val | LHS int64 值槽：3333/3351 表达式值、3395 QueryPhysicalMemory、3544 GetAsyncKeyState 派生、3608 串比较差值；3612–3679 终判左操作数 | 19 |
| local_178 | rhs_val | RHS int64 值槽：3334/3352/3383/3385 解析、3536 VK 码、3609 探测路径清 0；3620–3678 终判右操作数 | 27 |
| local_160 | op_pos | 运算符位置快照：2866/3045 =op_tail_buf 起点、3108/3157/3169 =lhs_buf+len、3127–3134 op 字符数统计；3480–3483 盘路径段按该位字符保存/恢复 | 23 |
| pWVar11 | resolved_var_ptr | 3139 PECMD_ResolveScriptVariable 返回值；!=NULL → pWVar41=解析值 | 4 |
| pWVar18 | env_value_str | 3085 g_szEmpty / 3088 `*(LPCWSTR*)pWVar41`（E69C 直查值）；3090 追加进 lhs_buf | 4 |
| local_res20 | out_var_name | 2722 =text 形参；3298 SkipLeadingControls（'?' 回写目标名）；3519 PECMD_WriteNumberToScriptVar、3551 FUN_1400629B8 的目标变量名 | 5 |
| local_110 | out_write_cursor | 子句结果写出游标（2740/2815 =text-2 起；3691 写真值数字；3694/3710 随 pWVar41 同步） | 7 |
| local_b8 | calc_buf_base | 2741 =text-2 写出缓冲基址；3722–3724 在 [-3..-1] 写 "#&="、3726 PECMD_CalcExpression(script, calc_buf_base-1) | 6 |

### 1c. 扫描/杂用

| 旧名 | 新名 | 证据 | 站点 |
|---|---|---|---|
| iVar23 | rhs_trim_len | 3586 RHS 尾随空白回扫后长度 `(puVar24+2 - 起点距)>>1`；3588/3589 参与 min 比较长度 | 5 |
| lVar40 | lhs_word_len | 3180 LHS 首词长（3173–3177 非空白推进、op_pos/终点为界）；3355 →iVar7（<3 出错/探测）；3434 关键词第 4 字符基址 `0x3d+len` | 4 |
| uVar34 | quoted_flag | 2894 清 0 / 2902 带引号置 1；2914/2921/2972 →bVar33（左词条带引号旗，不参与计数自增） | 6 |
| cVar31 | quote_cnt | 2922 =(char)uVar12 引号计数；2940/2966 `'\x01' < quote_cnt` 判已入引号值；2968 自增 | 5 |
| uVar17 | kw_char5 | 3430 关键词第 5 字符（&0xffdf 大写化）；3446 'U'→kind 1 / 3451 'A' 判定 | 4 |
| bVar36 | scaled_unit_flag | 3375 置 1（默认 MB 刻度）；3378 '*' 后缀清 0（原始字节）；3496 ==0 → 64 位原值比较分支 | 4 |
| local_d8 | env_gate_flag | 2821 =`(script[0xda]!=0 \|\| script[0xd]!=0)`（b961 族门）；3070/3113 选 FUN_14007BF44 扩展链 vs E69C 直查；2999 % 配对保护 | 4 |
| puVar24 | sp_trim_cursor | 尾随空白回扫游标：2793–2798 括号体收尾（含 ']' 剥离）、3581–3586 RHS 收尾 | 18 |
| local_f0 | root_str_0 | 3473–3486 栈上合成 "X:\" 根路径串 5 连槽之一（盘符字符）；3479 '\\' 判定 | 4 |
| local_ee | root_str_1 | 同上槽 2 =0x3a ':'（3474） | 3 |
| local_ec | root_str_2 | 同上槽 3 =0x5c '\\'（3476） | 3 |
| local_ea | root_str_3 | 同上槽 4 =NUL（3485 =WVar2） | 3 |
| local_e8 | root_str_4 | 同上槽 5 =0（3475，收尾 NUL） | 3 |
| local_96 | keystr_nul | 3546 =WVar2（NUL），keystr 2 字符栈缓冲的终止槽（local_98 后一字） | 3 |

合计改名 48 名 / 340 站点 / 279 行。

## 二、保留原名清单及原因（Ghidra 复用槽，单名必误导其一；宁缺毋滥）

| 保留名 | 原因 | 站点 |
|---|---|---|
| bVar1 | 双角色: 2834–2838 = 子句 '!' 跳过结果（→clause_neg_flag 槽）；2844 起复用为运算符种类旗（0 无/1 单字符 op/3 非 op 特殊字符/5 双写 op，2955 并入 local_res18） | 23 |
| bVar16 | 双角色: 恒 0 零寄存器（UVar30.LowPart 字节视图，`x == bVar16` ≡ `x == 0`）；3232–3261 探测块内复用为 FindFileOrDir 目录旗标 1（`(int)(char)bVar16 \| 0x10` = 0x11） | 32 |
| bVar20 | 双态: MEM 查询 MB 换算旗（3393 起 `>>0x14` 判 0）；3389/3392 与 bVar21 寄存器搬移互换（Ghidra 伪影，单名必误导搬移站点） | 8 |
| bVar21 | 同上对偶: MEM 查询种类（初值=mode1_cached；3447 'BU'→1；3395 传 QueryPhysicalMemory），与 bVar20 互换搬移 | 7 |
| bVar33 | 双角色: 左词条带引号旗（2914–2976，源 quoted_flag）；KEY 路径按键名形式旗（3368–3534: 0=VK 名/2=数字十六进制/1='#' 后） | 9 |
| WVar2 | 双角色: 恒 NUL 终止符（2842 起，词条截断/字符串终结主用）；3697–3741 尾扫描与 WVar29 角色互换（WVar2=当前字符） | 50 |
| WVar29 | 双角色: 主扫描当前字符（引号/词条/值/标记扫描，2859–3302）；3698–3741 尾扫描复用为 NUL（与 WVar2 互换，Ghidra 寄存器分配伪影） | 69 |
| uVar3 | 多角色: 低 8 位=求值模式（2728–2757 模式 0/1/2 分派）；2856/3058 kind 拷贝、3030 引号/'*' 旗、3292 探测真值、3303–3308 字符码、3682 终判 0 基准 | 20 |
| uVar4 | 即写即拷 Ghidra 临时量: 头标记字符选择（2749 '$'/2754 '\|'），2768 →uVar5 | 4 |
| uVar5 | 同上带 0x100000 标签副本（2745 =0x100001；2772/2774 →head_prefix_byte/char） | 5 |
| uVar22 | 双角色: mode0 旗（2732–2750，→mode0_cached）；3303–3308 复用为当前字符码（kind 标记命中改写） | 9 |
| uVar26 | 高复用槽: case-flag（2726=flags&0x10000→local_res18/local_c8）→env_form（2832 起）→int-kind 拷贝（3290）→VK 码（3535）→子结果（3628）→XOR（3680）→终值（3685/3688） | 26 |
| uVar37 | 双角色: 比较种类选择器（3057 起 =head_prefix_char 系，3301–3325 按 kind_marker 三标记分派）；2789/2814/2828 初值复用为 flags&0x20000（→flags_20000） | 23 |
| iVar6 | 双角色: 括号体文本长（2792–2793 尾扫基址）；字符串比较 min 长度（3587–3596 StrCmpN(I)W 参） | 8 |
| iVar7 | 高复用长度/旗标槽: env_gate 旗源（2819）→lhs 构建长（3105–3107）→解析 var 长（3144–3146）→lhs 长（3163–3168）→lhs_word_len 拷贝（3355）→关键词长分派 3/4/5（3387–3524）→lhs trim 长（3577） | 26 |
| DVar8 | 双角色: 恒 0 零寄存器（UVar30.LowPart 视图，`== DVar8` ≡ `== 0`）+ 子句比较结果 int32 载具（3195/3340/3344/3416/3497–3503/3593–3605 →lhs_val） | 42 |
| DVar9 | 高复用: mode1 旗（2896）→kind 双旗（3026–3033）→路径长（3205）→尾斜杠旗（3243–3254）→MEM/KEY 前缀比较（3388/3525）→串比较结果（3593–3599） | 23 |
| DVar10 | GetLogicalDrives 位掩码暂存（3214–3215）+ 0xfffffff8 哨兵（3210），与 DVar38 搬移伪影（3213/3224） | 6 |
| DVar38 | 双角色: mode1 旗缓存（2913–2929）；盘符校验态（3206–3250: 0xfffffff8=未验/1=已验，condB 门） | 10 |
| pWVar32 | 双角色: mode1 旗指针视图（2915–2929）；op 字符前缀字符数（3134–3151 memcpy 长度/偏移） | 8 |
| pWVar41 | 高复用: 写出缓冲基址（2737–2741）/头标记字符码（2836/2983–2985）/mode2 旗（3171）/解析 var 值（3135–3152）/尾 trim 游标（3571–3577）/写出活游标（3692/3708/3725） | 38 |
| uVar12 | 三角色: 引号计数（2895–2968，与 quote_cnt 同源自增）；PECMD_IsRemovableDrive 结果（3222/3252）；RHS 数值（3381/3401/3492） | 12 |
| uVar14 | 双角色: PECMD_WriteNumberToScriptVar 结果（3519–3520）；'@' XOR 对偶 lhs 布尔（3679–3680） | 5 |
| uVar25 | '@' XOR 对偶 rhs 布尔（3678/3680，与 uVar14 成对，3 站点不拆对） | 3 |
| lVar13 | 三角色: lhs 构建长（3121–3151 memcpy/分配）；lhs 终点指针（3165/3581 回扫基址）；尾斜杠 trim 下标（3233–3241） | 16 |
| uVar28 | 三角色: 盘符大写字符（3207–3221 GetLogicalDrives 位选）；关键词第 4 字符（3431–3437）；GetAsyncKeyState 值（3542–3543） | 15 |
| bVar43 | 双角色: '[' 开头旗（2788–3069，选括号/非括号解析分支）；3729 复用为 CalcExpression 结果==0 旗（取反中转） | 8 |
| local_res10 | 万用游标槽（dc 同名同槽）: 条件文本游标（主）→构建后 lhs 串（3161）→展开后路径（3203，盘符/探测段） | 86 |
| UVar15 | 高复用: 词条终点/op 尾标记（2845 清 0/2869/3047 =op_tail_buf）→RHS 展开源（3100–3148）→尾 trim 界（3571–3573）→终判 op 字符读取（3612–3676）→最终结果载具（3693/3726–3737 返回） | 66 |
| UVar19 | 真值积累槽（dc:30938/30963 定案）+ 游标/计数杂用: lhs 引号扫描（2854–2885）、op 后缀游标（2954–2963）、% 配对计数（2995–3013）、env 展开临时（3095–3098）、var 名（3140）、ws 游标（3172–3177） | 78 |
| UVar27 | 多角色: LHS 词条指针（主，2855–3309）+ 数值载具（MEM/盘空间 3396–3519）+ rhs_val 别名（3611/3631） | 57 |
| UVar30 | NULL 哨兵恒 0（R25 定案，2685 注释）+ 尾段错误/返回值载具（3493 -1、3520 回写值、3559 0x80070057、3565 返回；dc 同槽） | 56 |
| UVar35 | 三角色: op 字符位快照（2918/2948，3042–3044 尾串拷贝源）；RHS 起始游标（3282–3296/3579）；3289 短暂承载 probe_ui 真值（R24/R25 注释锚点，不折不扣保留） | 18 |
| local_1a0 | 高复用: 扫描游标（2893–3036）→解析 var 出参+释放（3136–3153）→lhs double 值槽（3327–3339）→关键词文本指针（3388–3480）→rhs 游标（3568–3596） | 37 |
| local_188 | 双载体: RHS 游标（主，3279–3297/3331–3535）+ 低 32 位旗标/标记寄存（2730 flags&0x20000、2841/3057/3064/3154 head_prefix_char/kind 标记转发） | 33 |
| local_170 | 双角色: 当前词条扫描游标（2790–3036）+ 词条终点/op 位标记（3042–3053 截断写 NUL）+ 尾扫描游标（3697–3740） | 35 |
| local_150 | 双角色: 值扫描游标（2837–2992）+ RHS double 值槽（3328–3339 ParseParenthesizedExpression 出参） | 33 |
| local_res18 | 双载体: 低字节=运算符种类（1/3/5，2864/2955）+ 高位 case-flag 缓存（2738）；3335 低字节<2 → double 判定门；3133 回读 | 11 |
| local_98 | 双角色: KEY 按键串 2 字符栈缓冲首槽（3545–3554）；3726 复用为 PECMD_CalcExpression 出参 | 6 |
| local_198 | '?' 分支使能旗（2733–2735 mode0/1 置位；3295 消费）——单角色，但 '?' 回写分支整体语义未定案（仅形态证据），保守保留 | 4 |

（probe_ui 为 R25 已语义化名，8 站点，未动。）

## 三、行界内残留 Ghidra 名计数（全部属上表保留项，无遗漏）

```
local_res10:86  UVar19:78  WVar29:69  UVar15:66  WVar2:50  UVar27:57  UVar30:56  DVar8:42
pWVar41:38  local_1a0:37  local_170:35  local_188:33  local_150:33  bVar16:32  uVar26:26
iVar7:26  uVar37:23  bVar1:23  DVar9:23  uVar3:20  lVar13:16  uVar28:15  uVar12:12
UVar35:18  local_res18:11  bVar33:9  bVar20:8  bVar21:7  iVar6:8  pWVar32:8  bVar43:8
DVar38:10  uVar22:9  DVar10:6  local_98:6  uVar4:4  uVar5:5  uVar14:5  local_198:4  uVar25:3
```
合计保留 1025 站点（40 个 Ghidra 名）。改名集 48 名在界内非注释行残留 = 0（脚本断言）。
23 行全行注释跳过未动，注释内出现名（UVar30/UVar19/UVar35/pWVar41/uVar20/probe_ui）与改名集零交集（脚本断言通过）。

## 四、纯度自检记录

1. 替换脚本: python `\bold\b` 全字 subn，仅作用 2659–3746 行（0 基 2658..3745）；全行注释
   （stripped `/*`、`//`、`* `、`*/` 开头）跳过；界外逐行字节一致断言通过。
2. round-trip 反演: 对 279 条实际差异行施加逆映射 → 与原文逐字节一致（281/281 git -/+ 对
   双向反演亦全部通过；其中 2 对为 GNU diff 并 hunk 空行同文对 L2714/L3099，old==new）。
3. git 核对: `git status` 仅 `M src/commands/core_b2f.c`（另 `M src/ui/core_b7c.c` 与
   `?? analysis/tmp_c4_*` 为并行子代理工作区改动，与本次无关）；diff 行号跨度 2667–3744，
   均在界内；总行数 8649 不变、LF 保持、无 BOM。
4. 撞名预检: 48 个新名在 src/ + include/ 全树 `\b` 级 grep 均 0 命中；无 Windows 宏词。
5. 未构建、未提交（守纪律）；C1 完工区 FUN_14003C06C（7180–7571）零触碰。
6. 语义复核: 声明区（2667–2713）、模式标记分配（2743–2769）、终判分派（3607–3690，
   lhs_val/rhs_val 读感与 dc:31253–31323 逐语句对应）、写出/收尾（3691–3745）经 Read 复核。

## 交付物

- `src/commands/core_b2f.c`（2659–3746 行 L1 语义化，48 名/340 站点）
- `analysis/r25i_l1_evalcond.md`（本文）
