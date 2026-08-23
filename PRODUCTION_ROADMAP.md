# PRODUCTION_ROADMAP.md — 场景 B / C 生产化路线图

> 定位：TASKS.md/REVIEW.md 记录的是"反编译还原"阶段；本文档规划其后的两条路线——
> **场景 B**（风格生产化，语义保持）与 **场景 C**（绞杀者重写 + 行为对拍）。
> 两场景可分可合，推荐按 §4 的合并路线执行。
>
> 状态基线日期：2026-08（本文档创建时的实测数据）

---

## 0. 决策框架

| 场景 | 目标 | 验收口径 | 大致成本 |
|---|---|---|---|
| 维持现状 | 反编译参考库 | build/link 绿（已达成） | 0 |
| **B** | "看起来像正经工程"：风格达生产规范，**不承诺行为正确** | §2.3 硬指标 + 每批 build/link 绿 | 单人 4-6 周 / 多代理流水线 2-3 周 |
| **C** | 可维护、可继续开发的生产代码 | 黄金语料对拍绿 + 分歧登记表完备 | 数月（含已投入量参照 §3.7） |

**核心约束差异**：B 的每一步必须语义保持（semantics-preserving），靠编译器与
静态断言兜底；C 引入行为裁判（对拍），才允许动语义。

---

## 1. 现状基线（实测，2026-08）

### 1.1 反编译残留量化（core_*.c 全量 grep）

| 指标 | 计数 |
|---|---|
| `local_xx` Ghidra 局部变量 | 14,908 |
| `param_N` 形参 | 7,674 |
| `FUN_140xxx` 引用 | 7,521 |
| CONCAT/unaff/extraout/in_stack/local_res 伪影 | 6,916 |
| `DAT_/PTR_` 数据符号引用 | 1,012 |
| TODO(verify)/TODO(restore) | ~400 |
| PECMD_ 可读名引用（对照） | 7,854 |

### 1.2 结构性问题清单（B 批 0 的输入）

1. `include/pecmd_defs.h`：`#endif` 在 L153，其后 ~500 行 extern 在 include guard 之外；
2. `win32_stub.h` 与 `link_stubs.c` 两套互相矛盾的 Win32 类型体系
   （HANDLE=uintptr_t vs void*，LPARAM=intptr_t vs long）；HFONT(L30/L38)、SC_HANDLE(L54/L859)
   重复 typedef；FindFirstVolumeW/FindNextVolumeW 签名已知错误（REVIEW §9.3）靠调用点强转绕行；
3. `link_stubs.c`（16,255 行）三种职责混装：Win32 API 桩 / 未实现 FUN_ 业务桩 /
   PECMD_* 别名真实转发（公共 API 定义落在补丁文件 = 架构倒挂）；
4. `core_b9_remaining.c` 内 ~250 个 CRT 定义，其中 strtol/perror/raise/iswctype/wcstol/wcstoul
   与 libc 同名强定义，真链接时会静默遮蔽系统库；大量错误签名 `uint64_t f(void)`；
5. 源码根目录 ~20 个会话工作文件（batch28*.txt、gh1-4.txt、gA-gD.txt、outA-D.txt、
   tmp_b3*_next.txt、wf28.js）；tools/tmp_p4_batch/（60+ 中间产物）、ghidra_out/ 位于源码树内；
6. `build.sh` 引用不存在的 `third_party/giflib`（死配置）；
7. 文档失真：REVIEW.md 章节号重复（两个 §21、§24）、TASKS.md "param_* 清零"等口径与实测不符；
8. 过期标记示例：core_string.c:39 对已实现函数标 UNIMPLEMENTED。

---

## 2. 场景 B：语义保持的风格生产化

### 2.0 总原则

- 一切改动可被编译器判定或静态断言钉死，不依赖"看起来没问题"；
- 每批一个 git 提交点，`./build.sh core_*.c` 全绿 + 完整链接 exit 0 双门槛；
- 进度看板用 §附录B 的 grep 命令实测，禁止口头宣称清零；
- README 最终明示定位："风格生产级 ≠ 行为已验证"。

### 2.1 批次任务

**批 0：卫生与结构修复（1-2 天，零风险）**

1. 修 pecmd_defs.h：guard 移至文件尾，或拆分为 pecmd_defs.h + pecmd_globals.h；
2. .gitignore 扩充（`tmp_*`、`batch28*`、`gh?.txt`、`g[ABCD].txt`、`out[ABCD].txt`、
   `wf28.js`、`tools/tmp_p4_batch/`），存量移入 `attic/`；ghidra_out/ 移出源码根或入 attic/reference/；
3. core_b9_remaining.c 的 ~250 个 CRT 桩拆至独立 `crt_shims.c`，文件头显著标注
   "仅链接验证用，禁止真实部署链接"（B 不真删，删除留给 C）；
4. link_stubs.c 拆分：`win32_api_stubs.c` / `unimplemented_stubs.c`；
   PECMD_* 别名转发迁回真实实现所在 TU；
5. win32_stub.h：删重复 typedef；修 FindFirstVolumeW/FindNextVolumeW 签名并同步
   强转调用点（编译器兜底）；统一与 crt_shims/link_stubs 的类型来源；
6. build.sh 删除死配置 third_party/giflib 分支；
7. 文档纠偏：REVIEW 重复合并、"清零"类失真声明改为实测数字（引用 §1.1 表）。

**批 1：机械改名清零（3-7 天，脚本化）**

- 复用既有工具链：apply_rename2.py + rename_map.json（已覆盖 1135 符号）+
  nameable_next.json 子代理提名流水线；
- 目标与手段：
  - FUN_140xxx 引用 → PECMD_ 名（未命名的走提名批）；
  - param_N → 语义形参名（逐文件子代理批处理）；
  - local_xx → 语义局部名（高频文件优先；拿不准用保守名 cursor/bufLen 等）；
  - DAT_/PTR_ → 按 REVIEW §4a-l 剧本迁 core_globals.c 配真实类型；
- 硬指标：`grep -P 'FUN_14' core_*.c` 仅剩注释命中（见附录B）。

**批 2：按功能重组文件（2-3 天，纯移动）**

```
src/
  kernel/    内存、字符串容器（core_string/core_strbld…）
  lang/      词法、变量展开、解析（core_execline/srparse/token…）
  runtime/   变量表、脚本对象、任务（core_var*/exec5/scriptdep…）
  commands/  各业务命令（core_b2*/cmd4…）
  ui/        窗口/控件框架（core_b8*/msgbox…）
  device/    设备/磁盘/服务（core_b3r_g*/sys…）
  app/       main/init/script 入口链
```

- git mv + build.sh 通配调整；补根 README（构建方法 + 目录地图）与每模块一句话说明。

**批 3：Top 对象结构体化（1-2 周，唯一判断密集批次）**

- 只做布局已被 REVIEW 登记的对象：控件/窗口对象族（OBJ_* 宏对应字段）、
  VarNode(0x20)、Script(0xe0)、task 结构、消息映射表项(0x28)；
- **安全技巧（本批的底气）**：

```c
typedef struct { HWND hwnd; HBRUSH brush; /* ... */ } PECMD_WndObj;
_Static_assert(offsetof(PECMD_WndObj, hwnd)  == 0x20, "layout must match binary");
_Static_assert(offsetof(PECMD_WndObj, brush) == 0x38, "layout must match binary");
```

  每字段一条静态断言钉死布局后，偏移直译→字段访问的替换不可能悄悄改变语义；
- 结构体放 `include/domain/*.h`（为 C 复用预留，见 §4 修正三）；
- 未登记布局的对象一律不碰。

**批 4：去重与格式统一（3-5 天）**

- DestroyObjectWithFree B~J 等家族：抽公共实现 + 原名一行薄包装
  （调用点与溯源保留，重复消失）；逐族人工确认等价后再合；
- 删过期标记（如 core_string.c:39 UNIMPLEMENTED）；统一注释语言政策与文件头格式；
- 全库过 `.clang-format`（提交配置文件）。

**批 5：TODO 收容与门面收尾（2-3 天）**

- ~400 个 TODO(verify) 二次分类：能以 decompiled.c/asm 证据定案者删标记；
  其余改写为显式标签 `/* KNOWN-LIMIT @0x…: 原因 */` 并汇总 `docs/known_limits.md`；
- 生成 `docs/provenance.map`（新符号 ⇄ 原 @0x 地址 sidecar 映射，脚本从头注释提取），
  把溯源从代码注释挪出，生产代码不再携带 @0x 噪声；
- 根 README 写入定位声明（见 §2.0）。

### 2.2 明确禁区（越界即升级为 C）

去 goto / 控制流重写；巨型函数拆分主体；解析器文法化重写；全局标志收敛为 context
结构体；错误处理策略改造；CRT 同名桩真删除改链系统库。——这些都会动语义，无对拍兜底即赌博。

### 2.3 验收口径（全部可复查）

| 指标 | 目标 |
|---|---|
| build.sh + 完整链接 | 每批全绿 |
| `FUN_14` 代码引用（非注释） | ≈0 |
| `param_[0-9]` / `local_[0-9a-f]{2,}` | ≈0（保守名允许保留少量） |
| `DAT_14|PTR_FUN_|PTR__` | ≈0（仅 KNOWN-LIMIT 注释内允许） |
| 重复 typedef / guard 外代码 | 0 |
| clang-format 干净、无过期标记 | 是 |

---

## 3. 场景 C：绞杀者重写 + 行为对拍

### 3.0 核心策略

现有 `refactored/` 树停止打磨，降级为"参考实现 + 行为预言机"；新建干净目标树，
按模块以生产风格重写，每替换一模块跑一次差分对拍。不做大爆炸重写。
唯一验收线：**与原版行为一致，或在分歧表中显式声明**。

### 3.1 Phase 0：行为基线（~1-2 周，一切的前置）

1. **黄金语料库**：覆盖 PECMD 命令矩阵的脚本集（FILE/ENVI/EXEC/REGI/SUB/MOUN/DISP…，
   按使用频率先覆盖核心 30 命令）。每用例 =
   `setup/`（前置文件+注册表导出）+ `main.pecmd` + `epilogue.pecmd`（用 PECMD 自身
   WRITE 命令把变量表快照落盘——测试驱动语言与被测语言同源，天然公平）；
2. **录制预言机**：原版 PECMD.EXE 录制四维输出（见下）；
3. **冻结现状**：git tag，旧树只读；
4. **定目标平台**：仅 Windows（Win32 直调）还是跨平台（加 OS 抽象层）——决定 Phase 2 平台层形态。

**可观测面（diff 四维度）**：① stdout/退出码；② 变量表转储文本；③ 文件系统前后
树哈希+内容；④ 注册表前后导出。GUI 类走截图容差比或逻辑探测降级。

**执行后端三层金字塔**：

| 后端 | 保真度 | 单例耗时 | 用途 |
|---|---|---|---|
| Wine(Linux) | 中高 | 秒级 | CI 主力，全量每次提交 |
| QEMU+Windows | 高 | 分钟级 | 夜间校准，识别 Wine 假差异 |
| QEMU+WinPE | 完全 | 更慢 | 发版冒烟（X: 盘/minint/pelogon 等 PE 特有路径）|

真实 Windows 不必每例都上，但必须保留周期性真值校准通道。

**关键杠杆——三方对拍**：旧树可用 mingw-w64 编成真 PE 放进 Wine 运行
（Wine 即 Win32 兼容实现）。于是 原版 EXE / 旧树二进制 / 新树二进制 同输入三方可 diff：
既验重写质量，也审计旧树还原质量，还自动暴露哪些空桩在运行时真正要命（指导补桩优先级）。

**不确定性驯服**：时间戳/随机值→正则掩码或格式断言；SOCK→本地回环 echo 对端；
盘符/SystemRoot→受控 prefix+规范化；GUI 抖动→容差比较+"已知不稳定隔离清单"（控制不增长）。

**无头环境注意**：本机实测 wineboot -i 会死锁（见附录A），手动带显示环境初始化成功前，
CI 后端先用 Docker 化 wine 镜像替代亦可。

### 3.2 Phase 1：语义规格恢复（~2-3 周）

1. 从引擎代码抽 PECMD 脚本语言完整 EBNF（token 规则、%var%/%~/%?% 展开、
   &/&&/:: 作用域、分隔符配置、注释规则；素材=REVIEW §5.x 批记录+core_execline/strbld）;
2. 领域结构体还原（直接继承 B 批 3 产物，补齐未覆盖对象）；
3. 模块语义卡：每域一页纸（输入/输出/副作用/错误行为/不确定点↔KNOWN-LIMIT 编号）；
4. **TODO 终审分诊**（承接 B 批 5）：① asm/decompiled 可定案→排期；② 黑盒探测可定案
   （构造输入看原版行为）→排期；③ 真不可知→写入 `docs/divergences.md` 并自选合理语义。
   **未定案 TODO 阻塞所属模块 DoD**。

产出：`docs/spec/`（文法+结构体+模块卡+分歧表）+ `include/domain/*.h`。

### 3.3 Phase 2：目标架构设计（~1 周）

- 功能目录（同 B 批 2 骨架，但为新树独立布局）；分层：platform←kernel←lang←runtime←commands←ui←app；
- 全局状态治理：数百散装标志收敛为分层上下文（PECMD_Runtime/PECMD_Session），明确所有权与并发假设；
- 错误策略：错误码枚举+传播约定；弹窗隔离为可插拔 UI policy（生产静默日志/交互弹窗）；
- 杀掉 0xaa55 魔数分配器，换标准堆+所有权规则；
- 溯源机制：provenance.map sidecar（沿用 B 批 5 生成器），产品代码零 @0x 注释；
- 先写风格指南（命名/所有权/错误处理三章即可），PR 对照审查。

### 3.4 Phase 3：逐模块重写循环（主体工程）

DoD 四件套缺一不可：**规格 ✓ 实现 ✓ 模块测试 ✓ 对拍绿 ✓**

| 序 | 模块 | 重写要点 | 对拍方式 |
|---|---|---|---|
| 1 | kernel 串容器/内存 | malloc 封装+所有权文档 | 单测 |
| 2 | runtime 变量表 | VarNode 结构体化，作用域照规格 | 变量表快照 diff |
| 3 | lang 词法+展开 | 递归下降替掉游标扫描 | 展开结果字符串 diff |
| 4 | lang 解析+执行 | 文法驱动，零 goto/LAB_ | 黄金语料全量 |
| 5 | commands 按频率分批 | 每命令独立 TU，helper 上提 | 单命令用例 |
| 6 | ui 控件框架 | 真 struct+vtable，合并重复家族 | Wine 截图/消息序列容差对比 |
| 7 | device/service/net 长尾 | 同模式 | 专项用例（可声明"按规格实现未对拍"）|

节奏：一批=一个模块或大命令族；旧树全程保留对照。旧树中语义干净的函数允许
"看着旧行写新码"式抄写——保证风格质变的关键细节。

### 3.5 Phase 4：硬骨头专项协议

- **巨型解析器**（29KB/20KB 级）：禁逐行翻。按文法写新解析器，模糊生成的怪异脚本做差分；原函数仅当输出对照器；
- **GDI 像素级代码**：先决策是否像素保真。是→Wine 像素 diff 容差比对；否→登记近似，按语义重写；
- **decompile-failed 函数**：读 ghidra_out/asm 手工补规格，或黑盒反推；
- **依赖 UB 的点**（wsprintfW 缺参类）：以调用点语义定"应然行为"，入分歧表。原版的偶然行为不值得复刻。

### 3.6 Phase 5：生产硬化与收口

- `-Wall -Wextra -Werror` + clang 并列构建；ASan/UBSan 全语料零报告；
- 解析器过 libFuzzer/afl（PECMD 解析半可信输入，真实攻击面）；
- CI 三道门：编译矩阵 / 语料回归 / 覆盖率阈值；
- 交付物含 `docs/divergences.md`（与原版全部已知行为差异）；
- 旧树移 attic/ 或归档仓库；REVIEW/TASKS/rename_map 随 git 历史封存。

### 3.7 成本结构与风险

| 环节 | 占比 | 备注 |
|---|---|---|
| Phase 0 对拍基建 | ~15% | 一次性投入，任何含"正确"的目标都逃不掉 |
| Phase 1 规格恢复 | ~15% | 七成事实已在 REVIEW，属整理非挖掘 |
| Phase 3 重写主体 | ~55% | 循环高度重复，适配本项目已验证的多代理流水线 |
| Phase 4/5 尾部 | ~15% | 经典长尾 |

单人全职粗估：Phase 0-2 约 4-6 周；Phase 3 约 2-4 个月；Phase 4/5 再 3-6 周。

风险登记：
1. TODO 分诊拖成无底洞 → 分歧登记表提供"合法放弃"出口，不求全定案；
2. Wine 噪声污染 diff → 夜间 Windows 校准层 + 隔离清单；
3. 台账再度失真 → 所有进度指标强制走附录B可复查命令；
4. "差不多完成"幻觉 → DoD 四件套 + 唯一验收线（对拍/分歧表）。

---

## 4. B→C 合并路线图（推荐执行序）

**可行性**：B 六成工作量对 C 有残值（批0≈100%、批3≈90%、批5≈80%、批1≈40%、批2≈50%、批4≈30%）。
前提是按以下三处修正执行，否则 B 是岔路：

- **修正一**：mingw/Wine 冒烟（原 C Phase 0，压缩版）提到改名之前——先暴露空桩模块，
  避免给注定重写的文件精装修；
- **修正二**：TODO 分诊从 B 批 5 提到第 1 批——分诊结果决定投资地图
  （语义已定案→精修/直移区；语义黑洞→重写区，跳过深改名）；
- **修正三**：批 3 结构体直接落位 `include/domain/`——B 产物被 C 原样继承。

时间线：

```
第1周     批0 卫生修复 + mingw/Wine 冒烟（桩缺口暴露）+ TODO 分诊出投资地图
第2-3周   批1 改名 + 批2 重组 —— 仅对地图上"精修区"文件深处理
第3-4周   批3 结构体入 include/domain/ + 批4 去重
          ↘ B 完结：可读参考实现 + 行为冒烟通过
第5周起   C 主循环：补关键桩使旧树 Wine 下跑通核心命令 → 录制黄金语料
          → 绞杀者模式逐模块重写（旧树此时为最佳对照物）
```

**心理陷阱警示**：B 完成时会产生"已经很像生产代码"的错觉。README 必须常驻定位声明
"风格生产级 ≠ 行为已验证"。B 与 C 之间隔的不是更多打磨，而是对拍裁判。

---

## 附录 A：Wine 无头运行实录（2026-08，供 Phase 0 参考）

- 环境：wine-wow64-11.0（nix），无 DISPLAY、无 Xvfb、nix 无 channel 无法补装 xvfb_run；
- 现象：`wineboot -i` 于 rundll32 setupapi InstallHinfSection 阶段死锁
  （注册表写入停滞于 system.reg≈1.8MB，全部进程睡在 ntsync_schedule）；
- 已排除：/tmp 属主问题（改家目录）；32 位前缀误判（预置空 .reg 所致，勿预置）；
- 已试旁路：伪造 `.update-timestamp` 后 `wine reg query` 仍挂起（自动更新循环或前缀状态损坏）；
- 结论/下一步：
  1. 优先在有显示环境的交互终端手动初始化（DISPLAY 存在时大概率直接通过）；
  2. 或改用 Docker 化标准 wine 镜像完成 Phase 0 后端（绕开本机 nix 构建的特殊性）;
  3. 手动操作序列见会话记录（pkill -x 清理 → WINEARCH=win64 家目录前缀 →
     reg add Graphics=null → drive_c/pectest 摆料 → `wine PECMD.EXE LOAD C:\pectest\smoke.ini`）。

## 附录 B：验收用可复查指标命令

```bash
cd refactored
# FUN_ 地址名残留（应仅剩注释命中）
grep -rPno 'FUN_14[0-9a-fA-F]+' --include='core_*.c' . | wc -l
# Ghidra 形参/局部残留
grep -rPno '\bparam_\d+\b' --include='core_*.c' . | wc -l
grep -rPno '\blocal_[0-9a-f]{2,}\b' --include='core_*.c' . | wc -l
# 数据符号残留
grep -rPno '\bDAT_14|\bPTR_(FUN|s)?_' --include='*.c' --include='*.h' . | wc -l
# 待验证标签
grep -rc 'TODO(verify' --include='*.c' . | awk -F: '{s+=$2} END{print s}'
# guard 外代码抽查（pecmd_globals.h 拆分后应为空）
awk '/#endif/{f=1;next} f&&NF' include/pecmd_defs.h | head
```

> 纪律：任何"已清零"的声明必须附上述命令的当次输出。
