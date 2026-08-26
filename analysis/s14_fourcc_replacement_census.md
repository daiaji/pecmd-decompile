# S14 FourCC 宏替换干跑普查（施工图 · 未动一行源码）

- 普查对象：PSB 动词 FourCC 裸十六进制现场 ↔ `include/pecmd_fourcc.h`（S14-B1 生成，98 动词宏）
- 工作区：`D:\repo\PECMD反编译\refactored`
- 方法：干跑三重扫描（全值交替扫描 / 分发形态扫描 / 可打印-FourCC 形状扫描），逐命中读上下文甄别；本文件为唯一产出，未修改任何源码/header/构建。
- 权威映射源：`include/pecmd_fourcc.h`（生成器 `tools/gen_fourcc.py`，口径＝只收 `local_158 == 0x…` 直比，去重后 98 互异动词值）

## 0. 结论速览（关键事实反转）

任务书假设"refactored\src 下大量动词分发现场"。实测：

1. **`refactored\src` 下动词 FourCC 直比命中数为 0**。src 中所有 `local_158` 均是无关函数的局部变量（字符串缓冲、尺寸、句柄等），无一处参与动词分发（证据：全树 98 值交替扫描 0 命中 + `local_158` 全量核对）。
2. **全部施工集中在一个文件**：仓库根目录 `restored_bodies.c`（build.sh 已纳入编译且 `-Iinclude` 就绪），函数 `PECMD_ProcessScriptBlock @ 0x14004c0bc`（即 header 注释所指 dc:44xxx if-级联，行 6347–7158）。该级联是 **Ghidra 二分查找 if 树**，不是 switch/case——全仓不存在任何 `case 0x…:` 动词标签。
3. 现场构成：**98 处直比（类 a，可机械换）＋ 1 处以 MSTR 值作二分枢轴的比较（类 b，换但需注释）＋ 14 处结构枢轴（类 c，不换）＋ 2 处表外动词比较 IPAD/EDIT（本轮不动，建议扩表决策）**。
4. 另有 28 处纯文档/JSON 提及（分析笔记与命名提案的 evidence 字符串），不属于替换面。

## 1. 数值→宏名 映射表摘要

完整权威定义见 `include/pecmd_fourcc.h`（98 条，含 `"XXXX"` ASCII 注释）。速查（宏名去掉 `FOURCC_` 前缀，小端 DWORD，值按 header 大写形态给出；源码现场一律为小写无 `u` 后缀形态）：

```text
ADSL=0x4C534441  BASE=0x45534142  BROW=0x574F5242  CALC=0x434C4143  CALL=0x4C4C4143
CMPS=0x53504D43  CODE=0x45444F43  COME=0x454D4F43  DATE=0x45544144  DEVI=0x49564544
DFMT=0x544D4644  DISK=0x4B534944  DISP=0x50534944  DOWN=0x4E574F44  DTIM=0x4D495444
EJEC=0x43454A45  ENVI=0x49564E45  EXEC=0x43455845  EXIT=0x54495845  FBWF=0x46574246
FDIR=0x52494446  FEXT=0x54584546  FILE=0x454C4946  FIND=0x444E4946  FLNK=0x4B4E4C46
FONT=0x544E4F46  FORM=0x4D524F46  FORX=0x58524F46  GETF=0x46544547  GROU=0x554F5247
HASH=0x48534148  HELP=0x504C4548  HIDE=0x45444948  HOME=0x454D4F48  HOTK=0x4B544F48
IFEX=0x58454649  IMAG=0x47414D49  INIT=0x54494E49  ITEM=0x4D455449  KILL=0x4C4C494B
LINK=0x4B4E494C  LIST=0x5453494C  LOAD=0x44414F4C  LOCK=0x4B434F4C  LOGO=0x4F474F4C
LOGS=0x53474F4C  LOOP=0x504F4F4C  LPOS=0x534F504C  LSTR=0x5254534C  MAIN=0x4E49414D
MEMO=0x4F4D454D  MENU=0x554E454D  MESS=0x5353454D  MSTR=0x5254534D  NAME=0x454D414E
NOTE=0x45544F4E  NTPC=0x4350544E  NUMK=0x4B4D554E  PAGE=0x45474150  PATH=0x48544150
PBAR=0x52414250  PCIP=0x50494350  PINT=0x544E4950   PUTF=0x46545550  RAMD=0x444D4152
RAND=0x444E4152  READ=0x44414552  RECY=0x59434552  REGI=0x49474552  RPOS=0x534F5052
RSTR=0x52545352  SBAR=0x52414253  SCRN=0x4E524353  SEND=0x444E4553  SERV=0x56524553
SHOW=0x574F4853  SHUT=0x54554853  SITE=0x45544953  SIZE=0x455A4953  SLID=0x44494C53
SOCK=0x4B434F53  SPIN=0x4E495053  SSTR=0x52545353  STRL=0x4C525453  SUBJ=0x4A425553
SWIN=0x4E495753  TABL=0x4C424154  TABS=0x53424154  TEAM=0x4D414554  TEMP=0x504D4554
TEXT=0x54584554  THRD=0x44524854  TIPS=0x53504954  TREE=0x45455254  USER=0x52455355
WAIT=0x54494157  WALL=0x4C4C4157  WRIT=0x54495257
```

## 2. 扫描口径与覆盖性论证

三条扫描（ripgrep，大小写不敏感，范围＝整个 refactored 树含根文件、src、tools、analysis、attic）：

| # | 正则 | 目的 |
|---|------|------|
| S1 | 98 个值的字面交替 `0x(v1\|v2\|…)` | 表内值全量定位（不限比较形态） |
| S2 | `case 0x{8}` 、`== 0x{8}` / `0x{8} ==`、`[<>!]=? 0x{8}`（对表值再过滤） | 任务书点名的 case/相等/区间形态 |
| S3 | `0x([2-7][0-9a-f]){4}`（每字节可打印 ASCII 的 8 位十六进制形状） | 兜底捕获表外 FourCC 形状常量 |

**总账对账**：S1 全树共 225 处命中 = `include/pecmd_fourcc.h` 定义本体 98 ＋ `restored_bodies.c` 代码现场 99 ＋ 文档/JSON 提及 28。分毫不差，无遗漏入口。

**十进制形态风险**：反编译体例下 32 位魔数一律十六进制书写；S3 形状扫描亦未发现可疑十进制等值（未逐值穷举 98 个十进制串，风险评级低，见 §6 注意事项）。

## 3. 逐文件施工清单

### 3.1 `restored_bodies.c` —— 唯一代码施工文件（115 处字面现场）

- 所属函数：`PECMD_ProcessScriptBlock` @ `0x14004c0bc`（行 5819 起），`local_158` 为当前 PSB 行解析出的动词 tag；级联区行 6347–7158。
- **前置步骤（1 行）**：在行 2 `#include "stubs_common.h"` 之后插入 `#include "pecmd_fourcc.h"`（build.sh 已带 `-Iinclude`，无需改构建脚本；当前全仓尚无任何文件包含此头）。
- 源内字面形态统一：**全小写 hex、8 位、无 `u` 后缀、运算符两侧有空格**（如 `local_158 == 0x4d455449`），利于机械化。

#### 类别 a —— 可直接换宏（98 处直比，每动词恰好 1 处）

| 行号 | 现值(源内小写) | 替换为 |
|-----:|----------------|--------|
| 6348 | 0x4d455449 | FOURCC_ITEM |
| 6357 | 0x455a4953 | FOURCC_SIZE |
| 6362 | 0x45444948 | FOURCC_HIDE |
| 6367 | 0x44494c53 | FOURCC_SLID |
| 6373 | 0x43454a45 | FOURCC_EJEC |
| 6377 | 0x43455845 | FOURCC_EXEC |
| 6383 | 0x434c4143 | FOURCC_CALC |
| 6390 | 0x4350544e | FOURCC_NTPC |
| 6395 | 0x44414552 | FOURCC_READ |
| 6405 | 0x44414f4c | FOURCC_LOAD |
| 6424 | 0x444d4152 | FOURCC_RAMD |
| 6428 | 0x444e4152 | FOURCC_RAND |
| 6432 | 0x444e4553 | FOURCC_SEND |
| 6436 | 0x444e4946 | FOURCC_FIND |
| 6449 | 0x44524854 | FOURCC_THRD |
| 6467 | 0x454d4f43 | FOURCC_COME |
| 6484 | 0x45444f43 | FOURCC_CODE |
| 6488 | 0x45455254 | FOURCC_TREE |
| 6493 | 0x45474150 | FOURCC_PAGE |
| 6497 | 0x454c4946 | FOURCC_FILE |
| 6502 | 0x454d414e | FOURCC_NAME |
| 6512 | 0x454d4f48 | FOURCC_HOME |
| 6516 | 0x45534142 | FOURCC_BASE |
| 6520 | 0x45544144 | FOURCC_DATE |
| 6524 | 0x45544953 | FOURCC_SITE |
| 6529 | 0x45544f4e | FOURCC_NOTE |
| 6536 | 0x4b4d554e | FOURCC_NUMK |
| 6541 | 0x49474552 | FOURCC_REGI |
| 6545 | 0x46544547 | FOURCC_GETF |
| 6549 | 0x46545550 | FOURCC_PUTF |
| 6553 | 0x46574246 | FOURCC_FBWF |
| 6557 | 0x47414d49 | FOURCC_IMAG |
| 6564 | 0x48534148 | FOURCC_HASH |
| 6569 | 0x48544150 | FOURCC_PATH |
| 6584 | 0x49564544 | FOURCC_DEVI |
| 6589 | 0x49564e45 | FOURCC_ENVI |
| 6597 | 0x4a425553 | FOURCC_SUBJ |
| 6601 | 0x4b434f4c | FOURCC_LOCK |
| 6605 | 0x4b434f53 | FOURCC_SOCK |
| 6621 | 0x4c4c4143 | FOURCC_CALL |
| 6627 | 0x4b4e494c | FOURCC_LINK |
| 6631 | 0x4b4e4c46 | FOURCC_FLNK |
| 6635 | 0x4b534944 | FOURCC_DISK |
| 6639 | 0x4b544f48 | FOURCC_HOTK |
| 6643 | 0x4c424154 | FOURCC_TABL |
| 6654 | 0x4c4c4157 | FOURCC_WALL |
| 6658 | 0x4c4c494b | FOURCC_KILL |
| 6685 | 0x4c525453 | FOURCC_STRL |
| 6689 | 0x4c534441 | FOURCC_ADSL |
| 6694 | 0x4d414554 | FOURCC_TEAM |
| 6706 | 0x534f504c | FOURCC_LPOS |
| 6714 | 0x504f4f4c | FOURCC_LOOP |
| 6724 | 0x4e574f44 | FOURCC_DOWN |
| 6728 | 0x4d495444 | FOURCC_DTIM |
| 6734 | 0x4d524f46 | FOURCC_FORM |
| 6739 | 0x4e49414d | FOURCC_MAIN |
| 6761 | 0x4e495053 | FOURCC_SPIN |
| 6767 | 0x4e495753 | FOURCC_SWIN |
| 6774 | 0x4e524353 | FOURCC_SCRN |
| 6786 | 0x4f474f4c | FOURCC_LOGO |
| 6790 | 0x4f4d454d | FOURCC_MEMO |
| 6798 | 0x50494350 | FOURCC_PCIP |
| 6803 | 0x504c4548 | FOURCC_HELP |
| 6813 | 0x504d4554 | FOURCC_TEMP |
| 6829 | 0x5254534c | FOURCC_LSTR |
| 6834 | 0x50534944 | FOURCC_DISP |
| 6839 | 0x52414250 | FOURCC_PBAR |
| 6846 | 0x52414253 | FOURCC_SBAR |
| 6852 | 0x52455355 | FOURCC_USER |
| 6856 | 0x52494446 | FOURCC_FDIR |
| 6868 | 0x5254534d | FOURCC_MSTR |
| 6872 | 0x52545352 | FOURCC_RSTR |
| 6877 | 0x52545353 | FOURCC_SSTR |
| 6881 | 0x53424154 | FOURCC_TABS |
| 6888 | 0x53474f4c | FOURCC_LOGS |
| 6902 | 0x5453494c | FOURCC_LIST |
| 6911 | 0x54494e49 | FOURCC_INIT |
| 6915 | 0x534f5052 | FOURCC_RPOS |
| 6919 | 0x53504954 | FOURCC_TIPS |
| 6924 | 0x53504d43 | FOURCC_CMPS |
| 6928 | 0x5353454d | FOURCC_MESS |
| 6932 | 0x54494157 | FOURCC_WAIT |
| 6946 | 0x54495257 | FOURCC_WRIT |
| 6951 | 0x54495845 | FOURCC_EXIT |
| 7073 | 0x544d4644 | FOURCC_DFMT |
| 7077 | 0x544e4950 | FOURCC_PINT |
| 7081 | 0x544e4f46 | FOURCC_FONT |
| 7093 | 0x56524553 | FOURCC_SERV |
| 7098 | 0x54554853 | FOURCC_SHUT |
| 7102 | 0x54584546 | FOURCC_FEXT |
| 7106 | 0x54584554 | FOURCC_TEXT |
| 7110 | 0x554e454d | FOURCC_MENU |
| 7114 | 0x554f5247 | FOURCC_GROU |
| 7127 | 0x574f4853 | FOURCC_SHOW |
| 7132 | 0x574f5242 | FOURCC_BROW |
| 7138 | 0x58454649 | FOURCC_IFEX |
| 7149 | 0x58524f46 | FOURCC_FORX |
| 7158 | 0x59434552 | FOURCC_RECY |

注：其中两处直比后随 `goto`（6502 NAME→LAB_14004d206、6529 NOTE→LAB_14004ce16），不影响替换。

#### 类别 b —— 可换但必须附注释（1 处）

| 行号 | 现场 | 处置 |
|-----:|------|------|
| 6828 | `else if (local_158 < 0x5254534d)` | 改为 `< FOURCC_MSTR` 并加行尾注释。此处 MSTR 值兼任二分枢轴：既分隔下方子树（LSTR/DISP/PBAR/SBAR/USER/FDIR 组），又在其后 6868 行作为自身分支的等值判定出现。裸读会误以为是普通排序比较，必须注明"枢轴恰为 MSTR 键值本身"。建议注释文案：`/* 二分枢轴==MSTR键值: 分隔 LSTR..FDIR 子树, 下方 6868 为 MSTR 自身分支 */` |

#### 表外动词比较 —— 本轮不动，建议扩表决策（2 处）

| 行号 | 现场 | 语义 | 说明 |
|-----:|------|------|------|
| 6416 | `if (local_158 != 0x44415049) goto LAB_14004df3c;` | IPAD（IP 地址控件→PECMD_MsgDispatch） | 真动词 FourCC，但生成器口径只收 `==` 直比，故不在 98 宏表内，无宏可换。若扩表（`FOURCC_IPAD 0x44415049u`）需同步改 `tools/gen_fourcc.py` 口径（header 标注 DO NOT EDIT BY HAND） |
| 6937 | `if (local_158 != 0x54494445) goto LAB_14004df3c;` | EDIT（文本控件→PECMD_CreateTextControl） | 同上，候选 `FOURCC_EDIT 0x54494445u` |

#### 类别 c —— 不该换（14 处二分结构枢轴，逐行豁免理由）

这 14 行均为 Ghidra 把原 `tag <= X` 改写成的排他上界 `< X+1`，其数值＝某动词值＋1，**不是任何动词语义**；换成宏会得到错误表达式（`< FOURCC_XXX` 语义变为"< XXX"，恒假于边界键），或根本无对应宏。故全部豁免：

| 行号 | 枢轴值 | ＝动词值+1 推导 | 豁免理由编号 |
|-----:|--------|-----------------|--------------|
| 6347 | 0x4d45544a | ITEM(0x4d455449)+1 | C1 |
| 6356 | 0x455a4954 | SIZE(0x455a4953)+1 | C1 |
| 6361 | 0x45444949 | HIDE(0x45444948)+1 | C1 |
| 6366 | 0x44494c54 | SLID(0x44494c53)+1 | C1 |
| 6466 | 0x454d4f44 | COME(0x454d4f43)+1 | C1 |
| 6535 | 0x4b4d554f | NUMK(0x4b4d554e)+1 | C1 |
| 6540 | 0x49474553 | REGI(0x49474552)+1 | C1 |
| 6620 | 0x4c4c4144 | CALL(0x4c4c4143)+1 | C1 |
| 6705 | 0x534f504d | LPOS(0x534f504c)+1 | C1 |
| 6713 | 0x504f4f4d | LOOP(0x504f4f4c)+1 | C1 |
| 6723 | 0x4e574f45 | DOWN(0x4e574f44)+1 | C1 |
| 6901 | 0x5453494d | LIST(0x5453494c)+1 | C1 |
| 6910 | 0x54494e4a | INIT(0x54494e49)+1 | C1 |
| 7092 | 0x56524554 | SERV(0x56524553)+1 | C1 |

豁免理由正文：
- **C1（上表 14 行）**：数值是"动词值+1"的排他上界，属反编译器二分查找的结构产物，非动词语义的同值碰撞。直接套宏会产生语义错误（差 1）；可选的后续重构是把 `< X+1` 改写回 `<= FOURCC_X`（改运算符，超出本轮"纯宏替换"范畴，不建议捆绑施工）。
- （对照）6828 行虽同为 `<` 比较，但其值恰等于真实键 MSTR，属类 b 而非 C1——这是全级联唯一的"枢轴与键重合"点。

### 3.2 `refactored\src` —— 零命中的证明与形状扫描副产物

- 表内 98 值在 src 下 **0 命中**（S1 扫描）；`case 0x…:` 形态全树 0 命中；`==0x…` 形态在 src 下的 40 处命中全部为 0xffffffff/0x80000000/0x12345001 等哨兵与内部魔数，无一在映射表内。
- src 内各文件的 `local_158`（core_b7c/core_b2f/core_b3r_h1/core_b3r_i28c/core_b3r_i28f/core_b3_remaining 等）经核对全是无关函数的局部变量，与动词分发无关。
- S3 形状扫描副产物（表外、非动词，**全部不换**）：

| 文件:行 | 字面量 | 判定 |
|---------|--------|------|
| src\commands\core_b3_remaining.c:8319, 8358, 18717 | 0x46424246 ("FBBF") | 镜像/磁盘格式探测魔数（读偏移 0xDA/0x1B4 后比对），非 PSB 动词，无宏 |
| src\commands\core_b3_remaining.c:18717 | 0x534c5055 ("UPLS") | 同上，格式识别姊妹值 |
| src\commands\core_b3_remaining.c:18727, 18733 | 0x5352424d ("MBRS") | 分区表签名魔数（偏移 0x1FE），非动词 |
| src\commands\core_b3r_h4.c:1314, 1324 | 0x77777777、-0x29406b2a1a85bd43 | 算术魔法数（乘法逆元/定点系数），仅形状误报 |

### 3.3 非代码提及 —— 不改（28 处）

均为分析笔记与命名提案 JSON 的 evidence 字符串，属文档性叙述而非可编译现场；替换反而破坏其对原始 dc 的引用保真。清单备查：

| 文件 | 行号 | 处数 |
|------|------|-----:|
| analysis\sub_hang_expandvars_static_notes.md | 8, 28 | 2 |
| tools\name_proposals.json | 15, 29, 36, 43, 64, 78, 92, 99, 106, 113, 120, 127 | 12 |
| tools\lowconf_review.json | 6, 171, 195, 275 | 4 |
| tools\name_proposals_c8a.json | 74, 130, 138, 282 | 4 |
| tools\name_proposals_c8b.json | 79, 90 | 2 |
| tools\shell_review.json | 16, 34, 53, 69 | 4 |

另：`include\pecmd_fourcc.h` 自身 98 处为定义本体， obviously 不换。（注：shell_review.json 所引 `link_stubs.c` 已不存在于树中，系陈旧引用，佐证这些 JSON 无需回改。）

## 4. 统计汇总与工作量预估

| 文件 | 字面现场 | 类 a | 类 b | 类 c | 表外待扩表 | 非代码提及 |
|------|-------:|----:|----:|----:|----------:|----------:|
| restored_bodies.c | 115 | 98 | 1 | 14 | 2 | – |
| include\pecmd_fourcc.h | 98（定义本体，不动） | – | – | – | – | – |
| refactored\src（103 个 .c） | 0（另有 9 行形状误报，均判不换） | 0 | 0 | 0 | 0 | – |
| analysis/*.md + tools/*.json | – | – | – | – | – | 28 |
| **合计** | **213+28** | **98** | **1** | **14** | **2** | **28** |

工作量预估（仅类 a+b 施工）：
- 机械替换 98 处直比：单文件单函数内正则可一次脚本化（形态高度规整），约 15 分钟含自检；纯手工约 40 分钟。
- 前置 `#include "pecmd_fourcc.h"` 插入：1 行。
- 类 b 注释：1 处。
- 验证：`./build.sh`（gcc `-fsyntax-only`，已覆盖 restored_bodies.c 与全部 src）一遍即可回归。
- **合计 ≈ 0.5–1 人时，零结构性风险**（等值替换，无逻辑改动）。
- 可选后续（独立决策项）：扩表 IPAD/EDIT 两宏并改 gen_fourcc.py 口径；把 14 处 `X+1` 枢轴改写为 `<= FOURCC_X`。

## 5. 施工注意事项（给执行者的安全口径）

1. 只替换精确形态 `local_158 == 0x<hex8>`（hex 小写、8 位、无后缀）；**严禁触碰 15 处 `<` 比较行**，除非按 §3.1 类 b 单独处理 6828 一行。
2. 98 个值在本文件内各自恰好出现一次于直比位，替换无歧义；不要做全局 sed 式"见值就换"（会把枢轴行打坏）。
3. IPAD(0x44415049)/EDIT(0x54494445) 两处 `!=` 比较本轮保持原样（无宏可用）；扩表前不得用近似宏顶替。
4. 替换完成后以 `./build.sh restored_bodies.c` 快速验证语法，再跑全量 build.sh。
5. 若后续发现任何十进制形态的动词值（本次扫描未发现），须先并入映射表再施工。
