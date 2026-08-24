# PECMD 反编译还原库

PECMD.EXE（WinPE 脚本工具）的 Ghidra 反编译忠实还原工程：把 `FUN_14xxxxxxx` 地址名函数
移植为语义命名的 C 真体。命名底账 1,594 符号（tools/rename_map.json），全部 FourCC 命令
真体落地（EXEC/ENVI/SHOW/REGI/RAMD/TABL/DISK/HASH/ListCtrl/PicRgn 等）。

> **定位声明**：本库为 PECMD 反编译**参考库**（v4 路线）——还原/命名/构建全战线清零，
> 待 WIN 端完成 MSVC 化 + 双二进制对拍验证后，交付"人类可读、行为验证过的参考源码"
> （PRODUCTION_ROADMAP v4）。金标准只录自真 Windows（原版 EXE）。

## 构建

```bash
bash build.sh                 # 全量语法验证 (src/**/*.c + 桩文件), 每文件 OK/FAIL
gcc -std=c99 -w -Wno-implicit-function-declaration -Iinclude \
    $(find src -name '*.c') restored_bodies.c win32_api_stubs.c \
    unimplemented_stubs.c crt_shims.c -lm -o pecmd_test   # 完整链接门
```

## 目录地图

| 目录 | 内容 |
|---|---|
| `src/app` | 进程入口与初始化 |
| `src/kernel` | 字符串/串构建器/全局槽 |
| `src/lang` | 词法/展开器/脚本解析/EXEC 引擎/资源解码 |
| `src/runtime` | 变量表/脚本对象/线程/子进程 |
| `src/commands` | FourCC 命令真体（b2/b3 族与长尾） |
| `src/ui` | 控件构造/Dtor/绘制引擎（SHOW/TABL/LIST） |
| `src/device` | 磁盘/卷/网络/驱动安装（DISK/DRVLOAD） |
| `src/misc` | 未归类 helper 与 SKIP 登记 |
| `include/` | pecmd_defs.h(类型) / pecmd_globals.h(g_ 全局) / win32_stub.h / stubs_common.h(桩共享声明) |
| `restored_bodies.c` 等 | 链接验证桩三件套 + crt_shims.c（**部署链接前必须移除后三者**，见各文件头横幅） |
| `harness/` | C-P0 行为对拍最小闭环：corpus 30 条核心命令用例 + runners 四脚本（run_case/diff_case/report/check_corpus） |
| `attic/` | 会话临时件与 ghidra_out 参考存档 |
| `docs/` | known_limits / triage_map / provenance.map / B1_pipeline_status / mingw 冒烟 |

## 关键文档

- `PRODUCTION_ROADMAP.md` — 参考库总方案书（v4：验证驱动补全 / WIN+MSVC）
- `docs/triage_map.md` — TODO(verify) 423 条分诊地图（STATIC 287 / BLACKBOX 46 / UNK 90）
- `docs/known_limits.md` — KNOWN-LIMIT 登记簿
- `docs/provenance.map` — 新符号 ↔ 原 Ghidra 地址对照（1,594 条）
- `docs/B1_pipeline_status.md` — 可读性推进（B1 语义化）配方
- `harness/README.md` — 验证闭环用法（双跑 + 判读 + 裁决纪律）
