# mingw 冒烟结果（B0 前置件 / C-P0 第一检查项）

日期：2026-08-24 ｜ 工具链：x86_64-w64-mingw32-gcc (GCC) 15.2.0（nix）

## 结论

**工具链可用性 = ✅ 确认**（v3 方案书悬置项就此关闭）。旧树全量编译可运行，
阻塞面收敛为 **7 处 MSVC CRT 类型冲突**，全部属方案书 §4.C.1-0.7 已知坑 (a)
「CRT 同名冲突」的预期范畴，留待 C-P0 正式期以 objcopy 弱化/条件化解决。

## 已在 B0 内顺手根治（无副作用，保留）

1. gen 区原型生成去重（304 条冗余重复声明消除）——Linux/mingw 双受益；
2. `pthreadlocinfo/pthreadmbcinfo` 项目内 typedef 加 `#ifdef __MINGW32__` 守卫
   （mingw corecrt.h 提供真实类型）。

## C-P0 待办清单（冒烟实测，非猜测）

| # | 冲突 | 说明 |
|---|---|---|
| 1 | pthreadmbcinfo ×5 | 项目内散布 char*/void*/uint64_t 三种伪定义 vs mingw 真实 typedef |
| 2 | wcstok ×1 | WCHAR* 版强定义与 mingw CRT 原型冲突 |
| 3 | GetCurrentThreadId ×1 | uint64_t 版残留声明（gen 区去重漏网，DWORD≡unsigned long 在 Linux 巧合等价） |

## 验证基线

- Linux gcc 完整链接：exit 0（未受影响）
- build.sh：96 OK / 0 FAIL（未受影响）
- mingw 编译：进入 TU 编译阶段后于类型检查收敛失败，**代码生成前错误**，
  无链接器介入——符合"先语法/类型、后链接"的排障次序。
