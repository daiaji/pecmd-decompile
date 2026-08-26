# SUB 挂死 / ExpandVarsRecursive AV — 静态分析弹药（无动态轮）

- 权威语义源: `decompiled.c`（下称 dc，base 0x140000000）
- 参考实现: `refactored\src\`（下称以 `src/文件:行` 引用）
- 本轮纯静态对读，未构建、未运行、未改任何源码。
- 结论速览:
  - ExpandVarsRecursive = FUN_14007bda8（dc:78754），唯一引擎为 FUN_14007af60（dc:78234，src 对应 `src/lang/core_execline.c` 的 PECMD_ExpandEnvVars）。
  - SUB 子过程关键字不是 FourCC 直比：`_SUB/_sub`、`_END/_end` 以 ctx+0x48 为 key 异或混淆存于 ctx+0x9a("_SUB_sub") / ctx+0xaa("_END_end")。dc 中 0x4a425553("SUBJ") 是盘符映射命令(FUN_140076144, dc:74936, DefineDosDeviceW)，与子过程无关——任务书假设的 'SUB\'=0x00425553 直比在 dc 不存在，已排除。
  - 发现两个 refactored 展开器结构性分歧（H1/H2，见 §3），均可造成零进度自旋或缓冲无限增长；SUB 的每次标签解析都经过该展开器（dc:28589），是"SUB 语料挂死"的头号静态嫌疑。

---

## 1. Fault A：ExpandVarsRecursive

### 1.1 调用链图（文本）

```
PECMD_ProcessScriptBlock FUN_14004c0bc (dc:43816, size=8717)  脚本块执行主入口
├─ '@'/CALL 标签引用路径 (lowconf_review.json 实证 L44268)
│   └─ FUN_14002fd88 ParseCommandPath (dc:28559)
│       ├─ dc:28589 → FUN_14007bda8 ExpandVarsRecursive   ★ 两故障交汇点
│       └─ → FUN_140024350 GetNextCommandArg (dc:21736)
│              ├─ param_5==NULL → FUN_1400240c0 FindSubBlockNth    (dc:21638)
│              └─ param_5!=NULL → FUN_140023c48 FindSubBlockNthScan(dc:21465, 两遍法)
├─ 行遍历器 FUN_14004e488 (dc:45231)  _SUB/_END 嵌套计数 + 逐行分发
│   └─ 深度0 → FUN_14004c0bc 递归执行单行
├─ 动词级联 fourcc if-链 (~dc:44xxx)
│   ├─ "SUBJ"(0x4a425553, dc:44563) → FUN_140076144 盘符映射【与子过程无关】
│   └─ SUB 控件动词 → FUN_1400ab724 SubCommand (dc:107017)
│       ├─ dc:107084 → FUN_140003a20 RunCommandLine (dc:1236)
│       │     └─ dc:1250 → FUN_14007bda8 ExpandVarsRecursive
│       ├─ dc:107126 → FUN_14007bda8（-sub 分支，展开参数文本）
│       └─ 拼 "-sub@name:param value" → FUN_140030144 ExecSubCommand (dc:28720)
│             └─ dc:28842 → FUN_14002fd88 ParseCommandPath（再次进入标签解析）
└─ 其余 BDA8 直调点: dc:26926(FUN_14002cc30) 28589(见上) 83898(FUN_140083a48)
    85053(FUN_14008569c, mode=-1,opt=0) 102476(FUN_1400a3f08) 105524(FUN_1400a8a30)
    115670(FUN_1400ba35c) 118553/118644(FUN_1400bf358) 126521(FUN_1400cb820)
    135102(FUN_1400dc9fc)

分发器 FUN_14007bf44 (dc:78810): script+0xda==0 且 script+0xd&0xf==0 → BDA8，否则 A224(全量展开)

FUN_14007bda8 ExpandVarsRecursive (dc:78757, src core_execline.c:68)
└─ 全程持全局临界区 DAT_14013e190(src g_csInit)
   └─ 循环调用 FUN_14007af60 = PECMD_ExpandEnvVars (dc:78240, size=3654,
      src core_execline.c:627) —— af60 的唯一调用者就是 BDA8
      └─ 依赖: FUN_14006d7e8 StrBldInitWide(6槽builder) / 14006d880 Reserve /
         14006d92c Grow / 140063694 Alloc / 140063720 Realloc / 14005b104 Free /
         140018978 VarLookup / 14006d9d0 FormatTypedMemValue /
         GetEnvironmentVariableW / lstrcmpW / StrCmpNIW / StrChrW
```

### 1.2 递归深度控制 与 输出缓冲增长

- 深度上限（两版一致，已逐行核对）：
  - 第一阶段：循环外首调 1 次 + do 体至多 5 次（dc:78775 计数置 4，底测 `-1 < cnt`；
    src:81 `cnt=4`，底测 `cnt>=0`）→ 至多 6 次 af60。
  - opt!=0 第二阶段：再 +1 次前置 + do 体至多 2 次（iVar1 从 1 起）→ 总计至多 9 次。
  - 提前退出判据：flag 置位（环境值含 `%`，或本遍未发生环境展开——dc:78315-78317，
    注意初值恒为 0：dc:78301 `unaff_R13D & 0xffffff00` 低字节必为 0，src:668 的
    TODO(verify) 可销号）或输出与输入 lstrcmpW 相等（dc:78778/78793）。
  - 无显式深度栈；防振荡完全靠 5 轮计数 + 稳定比较，周期 2 的振荡串会耗满额度后终止。
- 缓冲增长：
  - af60 内部 6 槽 builder（StrBldInitWide），每段拷贝前 Reserve(count+2) 按需扩容，
    Realloc 由 FUN_140063720 承担；每轮 af60 新分配整串输出。
  - 环境缓冲初始 0x50（dc:78299/78302），返回值>0 且首字符为 NUL 时按 rlen+100 重分配重查
    （dc:78602-78609，src:1070-1076 一致）。
  - BDA8 用双指针乒乓（param_2 ↔ *param_3）轮换输入/输出所有权，结束释放旧链
    （dc:78802-78803，src:109-110）。所有权前提："每次 af60 必产出新串"，见 E4。

### 1.3 解引用候选清单（上游可能 NULL/悬垂）

| # | dc 行 | src 文件:行 | 点位 | 怀疑理由 |
|---|-------|-------------|------|----------|
| E1 | 78778 / 78793 | core_execline.c:84 / :100 | `lstrcmpW(*out, in)` | *out 无 NULL 守卫。首轮若 af60 分配失败/早退未写 out 槽（调用方槽还可能是未初始化栈），lstrcmpW(NULL/wild) 直接 AV |
| E2 | 78779-78780 | core_execline.c:86-87 | 轮间 `*out = chain`(首次=NULL) | 两轮之间 *out 被故意置 NULL；若下一遍 af60 存在任何不写 *out 的返回路径，则下一遍 lstrcmpW(*out=NULL) AV。af60 全部出口需逐一核清（本轮未发现明显早退，但无结构性保证） |
| E3 | 78786→78313 | core_execline.c:93→:684 | opt 阶段以 `*out` 作输入 | 承接 E2：NULL 输入进入 af60 头部 `*p == L'\0'` 读即 AV@0x0 |
| E4 | 78802-78803 | core_execline.c:109-110 | 终局 free(pWVar4) | 乒乓所有权假设"每次调用必新串"。若 af60 任何路径把输出别名到输入（零长优化/失败透传），此处双重释放或 UAF，且 *out 悬垂给调用方 |
| E5 | 78456-78464 (`%#`→argv[idx+2])<br>78471/78473 (`%*`→argv[idx])<br>78521/78523 (`%@`→argv[idx+1]) | core_execline.c:211(:726)<br>:228-229(:740)<br>:235(:754) | 参数表越界读 | 三者只查表指针非空，不查下标边界；对比数字型 %N 有 `uVar14 < count` 检查（dc:78497，src:779/250）。idx 取自 sp[0xc] 本身时命中表尾 OOB，读出垃圾 WCHAR* 后 lstrlenW/拷贝环即 AV。**若测试 harness 按 count 精确长度布 argv（无哨兵槽），此类必踩** ← 当前 c0000005 的头号嫌疑族 |
| E6 | 78302 + 78576/78600 | core_execline.c:676/:1069 | envBuf/lpBuffer 写入 | FUN_140063694/PECMD_AllocWStringBuffer 返回值全程无失败检查，写 NULL 即 AV（低概率，但属同一"无守卫解引用"面） |
| E7 | 78672-78673 | core_execline.c:975-977 | 变量节点 `*(node+8)`/`*(node+0x18)` | 节点来自 VarLookup；若同遍内变量被删/重建（临界区内应序列化，但 SUB 链存在嵌套进出的二次加锁点 dc:29287），悬垂节点字段即 UAF |
| E8 | 78381/78396/78416 | core_execline.c:893/911/932 | 引擎向**输入缓冲**写 NUL（`:~`/`?spec` 截取） | 首遍输入是调用方原始行时被原地改写；只读/共享/复用缓冲 → 数据损坏与悬垂别名温床（非直接 AV，列作伴生风险） |

---

## 2. Fault B：SUB 动词结构与嵌套/跳转逻辑

### 2.1 关键字与分隔符的真实编码（排障前提）

- 配置器 FUN_14006159c（dc:59195）：低字 uVar2 为基字符 →
  ctx+0x88=uVar2、+0x92=uVar2^0x20(' ')、+0x94=uVar2^9('\t')、+0x8a=uVar2^0xd('\r')、
  +0x90=uVar2^10('\n')。主初始化传 0x10000（dc:40694）⇒ 规范配置
  **c88='\0', c92=' ', c94='\t', c8a='\r', c90='\n'**。
- 仅当入参高 16 位非 0 时才写关键字块（dc:59207）：以 ctx+0x48 为 key 异或生成
  "_SUB_sub"（ctx+0x9a..）与 "_END_end"（ctx+0xaa..）、'{'/'}'(0x96/0x98)、'['/']'(0xba/0xbc)。
- 匹配器 FUN_14006156c（dc:59168）：同时比对 keyword[i] 与 keyword[i+4]（大小写双变体），
  4 字符全中返回 0。
- ⚠ InvokeSubRoutine（dc:29285，src runtime/core_scriptdep.c:231）用 `flags>>16` 同时充当
  行定位字符与**分隔符基字符**重配局部副本——任何非零高位 flags 都会把 c88 变成非 0，
  使下游扫描器的"遇 NUL 停"保证失效（见 H3/H5）。src 与 dc 此处一致，差异只会来自
  harness 侧传 flags 的取值。

### 2.2 跳转/嵌套计数链

```
定义: 脚本文本 "_SUB 名" ... "_END"
行遍历 FUN_14004e488 (dc:45231; src commands/core_b3_remaining.c:4755)
  循环顶 break 条件: *cur==c88(规范=NUL) 或 flags&0xc3 (dc:45287 ↔ src:4811-4813)
  深度 iVar7: 命中 "_SUB"→++(dc:45378↔src:4919)；"_END"→ iVar7<1 则整体 break，
              否则 --(dc:45372-45373 ↔ src:4912-4914)；深度>0 时块内行不分发
  方向核对: 两版一致，无"计数方向反了"类差异。

引用: '@name' / 'name::n'
ParseCommandPath FUN_14002fd88 (dc:28559 ↔ src commands/core_b2e.c:1706)
  ├─ 先 ExpandVarsRecursive 展开 label 表达式 (dc:28589)   ← H1/H2 入口
  ├─ "::" 前缀 → 直接定位; 否则 "名:n" 语法拆冒号
  ├─ 带 n → GetNextCommandArg(param_5!=NULL) → FindSubBlockNthScan 两遍法:
  │    第一遍 (dc:21505-21548) 收集全部 _SUB 位置数组(初始0xe8,+8步进扩容)+深度;
  │    第二遍 (dc:21569-21631) 回溯匹配第 N 个同名出现
  └─ 无 n → 多段冒号循环逐次 FindSubBlockNth 单遍深度匹配 (dc:28654-28707)
执行: InvokeSubRoutine FUN_140030dcc (dc:29262 ↔ src core_scriptdep.c:212)
  克隆 ctx(FUN_140017cdc dc:12764) + 注入 &&CurDir + 重配分隔符(flags>>16) → FUN_140030420
SUB 控件动词: SubCommand (dc:107017 ↔ src commands/core_b3r_i28b.c:113)
  "-sub" 分支拼 "-sub@名:参 值" → ExecSubCommand → 又走 ParseCommandPath
```

### 2.3 死循环候选清单（结构差异/失稳点）

| # | dc 行 | src 文件:行 | 点位 | 怀疑理由 |
|---|-------|-------------|------|----------|
| H1 ★ | 78733-78747（LAB_14007bc3c 尾部拷贝环） | core_execline.c:1136-1151（copy_raw）；入口 :843（未闭合%）与 :1037 | **copy_raw 不推进 inP** | dc 版 while 头逗号表达式**每次强制** `param_2 = pWVar13`（即使拷贝 0 字符也赋值）→ 游标必然推进到闭合%之后/终结符。refactored 把推进寄望于入口前设好的 inP，而 copy_raw 本体只动 cur/src/dst：(a) 未闭合%入口(:843) inP 仍指向开% ⇒ 同一 '%' 反复解析，0 输出 0 进度自旋；(b) 同文件 FUN_14007A224 的对应分支 :331 有 `line = pw16;`，ExpandEnvVars 版丢了等价赋值——移植丢失实锤级旁证 |
| H2 ★ | 78567-78571 + 78735-78741（b961 重试→bc3c） | core_execline.c:1036 + :1137-1144 | **b961 回退支路差一（双减）** | 环境未命中确认后的重试路径：dc 检查 `*p13=='%'`（必真）⇒ 恒 +1 扩展、游标落在闭合%**之后**；refactored 先 `inP -= 1`(:1036)，copy_raw 再 `src = inP - 1` ⇒ 检查的是 p13-1（名字末字符）⇒ 永不扩展、少拷 1 字符、游标停在闭合%**之上**。后果："%UNDEF%"结尾语料 → 悬空%重解析成 `(p13==p11, count=0)` 的 copy_raw → 0 进度死循环；带尾文 → 每圈吞尾段、输出缓冲持续增长直至 OOM/卡死。SUB 测试 021/037/038 的 c0000005 与"早期挂死"均可用此机制解释 |
| H3 | 21538-21541 / 21542-21547 / 21622-21629 | core_b2e.c:461-471 / :555-565 | 扫描器底部跳过环只停 c88/c8a/c90 | 规范配置 c88=NUL 时安全停机；一旦 ctx 被 InvokeSubRoutine 用非零 flags>>16 重配（c88≠0），跳过环越过 NUL 在堆上狂奔 → 挂死/AV。两版代码一致，触发条件在 harness 的 flags 取值 |
| H4 | 28654-28707 | core_b2e.c:1797-1861 | ParseCommandPath 多段冒号环 | 终止依赖 GetNextCommandArg 每轮推进 + 最终 NULL。若 label 展开结果因 H1/H2 混入残留 '%'/错名，匹配长期失败但 argBuf 仍沿 ':' 推进至 NUL，理论上会终止；列为低优先观察项 |
| H5 | 45285-45402 | core_b3_remaining.c:4809+ | 行遍历器 EOF 终止 | break 依据 `*cur==c88`；末行缺换行且 c88 被重配为非 0（同 H3 根因）→ 失控。规范配置下安全 |
| H6 | — | core_execline.c:819 | 已修复项备案 | 注释记载的"%VAR% 行死循环(windbg 实锤)"已修复（名字扫描环逐字符重读），勿重复排查 |

已核对无差异项（可从排查面划掉）：FindSubBlockNthScan/Nth/GetNextCommandArg 三函数主体
逐段与 dc 一致（含第一遍数组 0xe8/+8 扩容、第二遍回溯、c48^0x2c 等）；SubCommand 主体、
InvokeSubRoutine、PrependCallSubLine、ExecSubCommand 骨架一致；BDA8 计数与轮次两版等价
（≤6 / opt 阶段 ≤3 次 af60）。

---

## 3. 最小代价二分方案（各一次单案运行）

### 3.1 Fault A（ExpandVarsRecursive AV）

**探针位置：只改一处——`src/lang/core_execline.c:83-91`（FUN_14007BDA8 的 do 环）。**
在每次调 PECMD_ExpandEnvVars 前后插入三断言（带轮次号打印）：

```c
/* 调用前 */ assert(in == NULL || *in != 0xDCDC);   /* 输入非毒 */
/*            */ assert(*out != NULL);               /* A1: out 槽非 NULL */
/* 调用后 */ assert(*out != NULL && *out != in);     /* A2: 新串且不别名输入 */
```

- 断言 A1/A2 任一命中 → 故障在**乒乓所有权族**（E1-E4），崩溃点应在 lstrcmpW/free；
- 断言全程不命中而仍 AV → 故障在**引擎内部族**（E5-E8）。再看异常地址：
  紧邻脚本 ctx/argv 堆页 ⇒ E5（argv 越界读特征强烈）；否则查 E6/E7。
- 零代码辅助判别（可与探针并列）：harness 给 argv 表尾追加 2 个 NULL 哨兵槽再跑一次——
  AV 消失即坐实 E5 族（%#/%*/%@ 越界）。

### 3.2 Fault B（SUB 挂死）

**探针位置：只改一处——`src/lang/core_execline.c:681`（PECMD_ExpandEnvVars 主环顶部）。**

```c
static const WCHAR *prevP = NULL; /* 或随调用传入的迭代上下文 */
iter++;
printf("EV iter=%d off=%td grow=%td\n", iter, inP - line, cur - base);
assert(inP > prevP || *inP == L'\0');  /* 游标必须单调前进 */
prevP = inP;
```

理由：SUB 每一次标签解析都先过这里（dc:28589），且 H1/H2 的自旋都表现为
"inP 不前进 / cur 无界增长"。
- 探针触发（off 停滞或 grow 单调暴涨）⇒ 展开器自旋，直接落证 H1/H2，
  并可用打印的 off 值区分：off==0 不动 → H1 型；off 在两值间振荡 → H2 型；
- 探针不触发而挂死仍在 ⇒ 整体嫌疑面移交扫描器/行遍历（H3-H5），
  下一步单查 harness 调 InvokeSubRoutine 时 flags 高位是否非零（H3 根因）。
- 免跑替代（不改码）：语料脚本缓冲尾部统一补 "\r\n"+NUL 再对拍——挂死消失 ⇒ EOF/
  终结符类（H2@EOF、H3、H5）；仍挂 ⇒ 逻辑类（H1、H2 带尾文、H4）。

### 3.3 优先序建议

先跑 §3.2 探针（一个断言同时覆盖两故障的交汇路径 dc:28589），再跑 §3.1；
E5 的 argv 哨兵槽试验成本最低，可与任一并行。

---
*生成: ox-alpha 静态分析子代理；证据行号以当前工作区 decompiled.c (181421 行) 与
refactored/src 为准。*
