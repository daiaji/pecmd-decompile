# PRODUCTION_ROADMAP.md — PECMD 反编译参考库总方案（v4）

> **版本：v4（2026-08-24 用户方向裁定后重写）｜上一版：v3（场景 B/C 框架，已废弃）**
> v3 的"场景 B（风格生产化）/ 场景 C（pecmd-next 绞杀者重写）"框架**整体废弃**——
> 用户裁定：**不继续开发 PECMD，另起炉灶；PECMD 源码层次研究用于新项目功能对齐**。
> v4 的单一目标 = 交付一份 **人类可读的、行为验证过的 PECMD 参考源码**。

---

## 1. 项目定位与目标（v4 核心）

### 1.1 一句话定位

把反编译成果打磨成"参考库"：**可读（人类能读懂并研究语义）+ 验证过（与原版二进制功能对齐有证据链）**，
作为用户新项目的**功能对齐基准**。

### 1.2 目标 / 非目标

| | 内容 |
|---|---|
| **目标** | ① 人类可读的源码（语义命名/注释/模块划分） ② 可运行的编译产物（pecmd_msvc.exe） ③ 与原版 EXE 的功能对齐验证证据链（verdict + divergences） |
| **非目标** | ✗ 继续开发/维护 PECMD ✗ pecmd-next 全树重写（v3 场景 C，废弃） ✗ C23/架构现代化重构（参考库要"忠实+可读"，不要"现代"） ✗ 风格生产化门禁（v3 场景 B，废弃——其产物作为资产继承） |
| **继承资产** | v3 期间完成的一切（见 §2.2）——B0 卫生 / 保真度修复 / domain 头 / 语料体系 / 分诊地图 全部保留 |

### 1.3 交付物（最终形态）

```
refactored/                       # 参考源码树（src/ 八域 + 桩文件）
pecmd_msvc.exe                    # MSVC 编译产物（可运行, 对拍主体）
harness/golden/win_real/<case>/   # 原版 EXE 行为金标准（真机录制）
harness/results/win_real/<case>/  # 编译产物行为（同机运行）
harness/results/**/verdict.json   # 对齐判定（PASS/FAIL/NO-GOLDEN）
docs/divergences.md               # 分歧登记（不可对齐项的完备清单）
```

---

## 2. 现状基线（v4 实测, 2026-08-24）

### 2.1 战线状态

| 战线 | 状态 | 证据 |
|---|---|---|
| 还原 | **清零**。全部 FourCC 命令真体落地（EXEC/ENVI/SHOW/REGI/RAMD/TABL/DISK/HASH/三中桩） | ROADMAP v3 §2.0（历史） |
| 命名 | rename_map **1594** 符号；biz 46 全处置 | tools/rename_map.json |
| 保真度 | 分诊 7 处静态可证问题：4 修 2 误报 1 注释升级（66c3d17） | docs/triage_map.md §6 |
| 构建 | bash build.sh **99 OK / 0 FAIL**（Linux gcc）；**MSVC 未验证**（v4 阶段 0 首项） | 附录 B M6 |
| 语料 | C-P0 首批 **30 条**核心命令用例（正/边界/错误例） | harness/corpus/cases/002-030 |
| 验证 | **零对拍记录**（语料就绪但未录制 golden；MSVC 产物未产生） | — |

### 2.2 继承资产（v3 期间产出, 全部保留）

| 资产 | 位置 | v4 用途 |
|---|---|---|
| src/ 八域布局 + 模块 README | src/ | 参考库的模块划分 |
| include/domain/pecmd_domain.h | include/ | 对象布局权威（VarNode/Script/Task 断言全过） |
| 保真度修复后的真体 | src/ | oracle 可信度（对拍前提） |
| 语料 30 条 + harness 四脚本 | harness/ | 验证闭环骨架（直接复用） |
| triage_map（STATIC 287/BLACKBOX 46/UNK 90） | docs/ | BLACKBOX→探针用例；STATIC→可读性批注素材 |
| provenance.map（1594 行） | docs/ | 新符号↔原 Ghidra 地址溯源 |
| B1 配方文档 | docs/B1_pipeline_status.md | 可读性推进（阶段 3） |
| known_limits（P10/P11/豁免登记） | docs/ | 分歧登记簿前身 |

### 2.3 反编译残留量化（与 v3 同口径, 供进度对照）

| 指标 | 实测 | 复验 |
|---|---|---|
| M1 FUN_ 引用 | 4,791（唯一拼写 737） | 附录 B M1 |
| M2 local_xx | 26,565 | M2 |
| M3 param_N | 13,888 | M3 |
| M4 DAT_/PTR_ | 2,632 | M4 |
| M5 TODO(verify) | 423（triage 已分诊） | M5 |

---

## 3. 工作路线：验证驱动补全（v4 核心方法论）

### 3.1 总循环

```
语料用例 ──▶ 双跑（原版 vs 编译产物）──▶ 4 维采集 ──▶ diff ──▶ verdict
   ▲                                                          │
   │                                                          ├─ PASS → 下一用例
   │                                                          └─ FAIL → 分诊：
   │                                                              ① 桩缺失 → 补真体
   │                                                              ② 反编译失真 → 点状重写
   │                                                              ③ 非确定性 → flaky 隔离
   └──────────── 重跑该用例 ◀────────────── 补全/修正 ◀───────────┘
```

**方法论铁律**：每一处补全/重写都由 diff 失败点驱动——"验证到哪里，补全到哪里，就交付到哪里"。
这是与 v3 场景 C"架构驱动的全树重写"的本质区别（后者无验证驱动，diff 无法归零，时间失控）。

### 3.2 阶段划分

| 阶段 | 内容 | 门禁 |
|---|---|---|
| **P0 MSVC 化** | ① 33 处复合字面量改写（(LARGE_INTEGER){…}→局部变量赋值） ② crt_shims.c 按 MSVC 裁剪（删 MSVC CRT 机制桩/入口桩，业务缺口桩保留） ③ 构建管线（**已交付**：`tools\msvc_build.bat` 原生驱动 cl.exe，见 docs/msvc_build.md） ④ cl 试编译 → **pecmd_msvc.exe 产出** | G0: pecmd_msvc.exe 可运行（至少冒烟用例能跑） |
| **P1 验证闭环打通** | ① 冒烟用例 001 双跑（原版 vs msvc 产物） ② 4 维采集（stdout/exit/vars/fs） ③ diff_case 判读 → verdict ④ golden 录制规范确立 | G1: 001 用例 verdict 闭环跑通（PASS/FAIL 均能判定且可归因） |
| **P2 核心命令对拍** | 30 条语料逐条双跑；FAIL 分诊补全（桩→真体 / 分歧修正）；**BLACKBOX 46 条转化为探针用例**补入语料 | G2: 核心命令集 verdict 全绿（或分歧均登记且归因） |
| **P3 可读性推进** | B1 语义化按配方推进（local_xx/param_N→语义名）；与验证循环并行（改后可重跑已绿用例回归） | G3: 精修区 A 区（b7c/b8m/b8h/b2f）local_xx≈0 |
| **P4 交付** | 证据链汇总（verdict 全量 + divergences.md 完备）+ 文档收口 + 归档 | G4: 参考库交付（可读+验证过） |

### 3.3 验证边界（务实收敛）

- **核心命令对拍绿**（30 条起步 → 控制流 → 按新项目需求扩长尾）
- 长尾登记 divergences.md（不追求全量清零）
- 验证记录 = 交付物的一部分（verdict.json 证据链）

---

## 4. 平台与工具链（v4：WIN 全包）

### 4.1 决策记录

| 决策 | 理由 |
|---|---|
| 剩余工作全在 WIN 端 | 用户裁定；验证闭环（编译→双跑→判读→补全）可全部在 WIN 完成 |
| **MSVC 为主编译器** | **原版 PECMD.EXE 即 MSVC 编译**——同 CRT/ABI/编译器行为，对拍假分歧最小；crt_shims 的 MSVC CRT 桩在 MSVC 下可删（系统 CRT 即原版所用） |
| mingw 弃用 | 交叉编译优势不再需要；其 7 处 CRT 冲突侦察结论（pthreadlocinfo/wcstok/GetCurrentThreadId）在 MSVC 下以"删桩"形式自然消解 |
| ~~WSL/git-bash 保留~~ → **管线纯 WIN 化（2026-08-24 v4.1 用户裁定）** | 判读层（diff_case/report/check_corpus）本就纯 stdlib Python，原生可跑零改动；唯一 POSIX 硬绑的 run_case.sh 移植为 run_case.py（双跑内置）；MSVC 构建交 `tools\msvc_build.bat` 原生驱动。环境收敛为 VS2019 + Windows Python，弃 WSL2/git-bash/GNU coreutils |

### 4.2 MSVC 化侦察结论（2026-08-24 实测）

| 项 | 数量 | 处理 |
|---|---|---|
| VLA | 0 | 无 |
| GCC 扩展 | 0 | 无 |
| 复合字面量 | 33 | 改写（P0-①），几乎全为 `(LARGE_INTEGER){...}` |
| 指定初始化器 | 30 | VS2019+ C 模式支持，不动 |
| `_Static_assert` | 19 | VS2019 16.8+ 支持，不动 |
| 空参表原型 | 10 | MSVC 老式语义（无参数检查），不动 |
| 入口点（crt_shims 内） | 3 | 删桩用系统 CRT（P0-②） |
| crt_shims.c 全文件 | 1,277 行 | 裁剪后预计保留 <50%（业务缺口桩） |

### 4.3 环境要求（WIN 端）

- Windows 10/11 + Visual Studio 2019 16.8+（含 C 编译器；免费 Community 版即可）
- Windows 原生 Python ≥3.8（验证闭环脚本，纯 stdlib；无需 WSL/git-bash/coreutils）
- 原版 `PECMD原始.EXE` 放 C:\pectest\（对拍真值）

---

## 5. 语料与验证体系（harness，直接继承 v3 设计）

- 目录约定/manifest 契约/尾声规范/4 维采集/diff 契约：沿用 v3 §4.C.1-0.1~0.5（设计有效，不再重复）
- **改动点**：
  1. 后端：`win_real` 唯一被测后端 + `win_real_orig` 原版参考后端（v3 的 wine/qemu 全部移除——WIN 全包下无意义）
  2. run_case.py 同时驱动**原版与编译产物**双跑（v3 只驱动被测方；golden 由原版结果经 `--record-golden` 晋升）
  3. diff_case.py 的 NO-GOLDEN 状态保留（先录 golden 再判读）
- 语料扩充：按需（新项目功能对齐需求驱动），BLACKBOX 46 条为探针用例现成来源

---

## 6. 流程纪律（全程有效）

1. 每阶段一个 git 提交点；破坏性实验开分支
2. **双绿门（v4.1）**：`tools\msvc_build.bat syntax` 全绿 + cl 编译通过 + 链接 exit 0
   （build.sh 为 Linux gcc 参考门，留档不作为 WIN 门禁）
3. 补全/重写必须携带"驱动它的 diff 失败证据"（记入 commit message）
4. 台账活页：本文档（方案）+ harness/results（事实）；REVIEW/TASKS 冻结为历史
5. 单编辑者纪律：桩文件并发修改禁止
6. 任何"已完成"声明必须携带当次 verdict/指标输出

---

## 7. 风险登记册（v4）

| # | 风险 | 触发信号 | 缓解 |
|---|---|---|---|
| R1 | MSVC 化超出预期（复合字面量改写引入转写错误） | P0 阶段 build 红且无法定位 | 每处改写后回归已绿用例；diff 证据驱动 |
| R2 | 对拍假分歧（原版环境差异/多进程干扰） | 同用例双跑不一致 | 用例沙箱化；flaky 隔离池；golden 重录 |
| R3 | 补全无底洞（长尾命令无穷尽） | 某命令域 3 轮未收敛 | 登记 divergences 合法放弃；UNK 上限规则 |
| R4 | 可读性推进破坏验证（B1 改名引入行为变化） | 已绿用例重跑 FAIL | 改名后全量回归已绿语料 |
| R5 | 文档再度失真 | 口径与实测不符 | §6.4 强制附输出 |
| R6 | MSVC 版本差异（WIN 端 VS 版本 <16.8） | _Static_assert 编译错 | 环境检查单强制 VS 2019 16.8+ |

---

## 附录 A：关键命令速查（WIN 端，v4.1 纯原生）

```bat
cd refactored
tools\msvc_build.bat syntax                          :: cl 语法门 (WIN 主门禁)
tools\msvc_build.bat                                 :: 完整构建 → build\msvc\pecmd_msvc.exe
python harness\runners\check_corpus.py               :: 语料结构自检
python harness\runners\run_case.py 001_envi_smoke    :: 双跑(原版+编译产物)
python harness\runners\run_case.py --all             :: 全语料双跑
python harness\runners\diff_case.py 001_envi_smoke   :: verdict
python harness\runners\report.py                     :: 汇总
```

注：`build.sh` 为 Linux gcc 参考门留档；构建细节见 docs/msvc_build.md。

## 附录 B：指标命令（M1-M10，Linux 侧历史口径，随 v3 冻结）

> 以下为 Linux 端已执行的度量配方（证据已固化于 tools/*.json），非 WIN 端日常命令。

```bash
M1: grep -rPno 'FUN_14[0-9a-fA-F]+' --include='*.c' src/ *.c | wc -l
M2: grep -rPno '\blocal_[0-9a-f]{2,}\b' --include='*.c' src/ *.c | wc -l
M3: grep -rPno '\bparam_\d+\b' --include='*.c' src/ *.c | wc -l
M4: grep -rPno '\bDAT_14|\bPTR_(FUN|s)?_' --include='*.c' --include='*.h' src/ *.c include/ | wc -l
M5: grep -rc 'TODO(verify' --include='*.c' src/ *.c | awk -F: '{s+=$2} END{print s}'
M6: bash build.sh; echo $?            # 99 OK / 0 FAIL
M7: awk '/#endif/{f=1;next} f&&NF' include/pecmd_defs.h | wc -l   # 0
M8: (警告账面, 见 tools/warning_census_b0.json)
M9: python3 -c "import json;print(len(json.load(open('tools/rename_map.json'))))"  # 1594
M10: git ls-files | grep -vcE '^src/|^include/|^tools/|^docs/|^attic/|^harness/|\.md$|\.sh$|^\.gitignore$|^\.clang-format$|^\.gitattributes$|^LICENSE|restored_|win32_api|unimplemented|crt_shims'
```

## 附录 C：历史沿革

- **v1-v3（2026-08-23~24）**：场景 B/C 框架（风格生产化 / pecmd-next 重写）——因用户方向裁定**废弃**；
  其执行产出作为资产继承（§2.2）。历史方案见 git 历史（72080e2 起）。
- **v4（2026-08-24）**：参考库 + 验证驱动补全 + WIN/MSVC 全包。
- **v4.1（2026-08-24）**：管线纯 WIN 化——run_case.sh→run_case.py、tools\msvc_build.bat
  交付、弃 WSL2/git-bash/coreutils（§4.1 决策记录）。
## S12 里程碑：脚手架层清除 —— 内嵌 CRT/弱声明 → 真 CRT/真头文件（2026-08-25 立项）

> 背景：S11 三层连环崩点（隐式 int 原型截断 / NextToken 空壳桩 / memset no-op 桩）
> 全部源自"手写声明层"历史包袱。本里程碑把 A 类脚手架替换为真 CRT/真声明，
> B 类行为本体（MemMoveSafe、0xaa55 分配器等对拍对象）永留不动。

### 分阶段验收门
| 阶段 | 触发条件 | 内容 | 验收 |
|---|---|---|---|
| S12-a | 门A 全通 | 低风险单点：LPSTARTUPINFOW/LPPROCESS_INFORMATION 等 void* 弱类型→真结构指针；fopen/fprintf 手写 extern→#include <stdio.h>（先解 win32_api_stubs _vsnwprintf 冲突）；FUN_140102a90 已完成(0eb37ab) | 双绿门 + t1/t2probe 回归 |
| S12-b | T4 首轮全量 PASS 落账 | umbrella header 统一头文件；逐类去重 typedef（UINT/WPARAM/LPARAM/FILETIME/HANDLE…约 20 个）；删除 crt_shims.c 中已有真 CRT 的条目 | C4013=0、C4311/12≈0、28 语料回归全 PASS |
| S12-c | T4 收敛后 | 清除 win32_api_stubs 与 win32_stub 的重复声明层；仅保留 B 类本体与确需的导入槽 | 全量回归 + divergences 无新增 |

### 已知阻塞清单（S12-b 前必须解）
1. win32_api_stubs 的 _vsnwprintf 内联 vs <stdio.h> 冲突（HANDOFF §6 老纪律的根）。
2. stubs_common.h 与 win32_stub.h 的 typedef 双定义（WPARAM/LPARAM 已对齐 4d577d1，
   其余待盘点——见 build\msvc\s12_crt_migration_checklist.md）。

## S13 里程碑：反编译坏味道清零 —— FUN_/DAT_/Ghidra类型/local变量 四层治理（2026-08-25 立项）

> 前提链：T5 拆探针 → S12-b 头文件统一(T2 类型层顺产) → T4 PASS 收敛。
> 铁律：纯机械改名=行为中性，每批必过 [msvc_build] OK + 28 语料回归全 PASS。

### 波次
| 波次 | 内容 | 度量 |
|---|---|---|
| S13-a | 残余 FUN_/DAT_/PTR_ 语义命名（证据驱动批次，管线=FUNC_NAMES+name_proposals+apply_rename.py） | M1/M4 趋零 |
| S13-b | local_xx/param_N/extraout 机械清扫（按 TU 分批外包；作用域内正则+编译门） | M2/M3 趋零 |
| S13-c | 伪码伪影清剿（CONCAT71/SUB164/extraout 双寄存器写法）——与语义修复同源，随 T4 分诊顺产 | 普查 P0/P2 清单归零 |

### 存量盘点
见 build\msvc\s13_smell_inventory.md（待产出）。
