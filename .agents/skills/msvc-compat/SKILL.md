---
name: msvc-compat
description: GCC 风格反编译 C 代码移植到 MSVC 的症状->手法速查表 (9 类已定案坑)
whenToUse: cl 编译报 C2059/C2143/C2440/C2036/C7744/LNK2005/LNK2019 等错误时
---

# MSVC 兼容速查表

全部在本仓库实战定案过, 按症状直接套, 不要重新发明:

| 症状 | 根因 | 手法 |
|---|---|---|
| C2059/C2143 `{...}` 附近 | GCC 复合字面量 `(LARGE_INTEGER){.QuadPart=x}` | → `PECMD_LI(x)`; 辅助构造在 win32_stub.h 与 stubs_common.h 各一份 (两套头无 TU 同含, 不冲突) |
| C2440 标量→struct | 反编译器 cast 残留 `(LARGE_INTEGER)(int64_t)x` | → `PECMD_LI(x)` |
| C2059 "..." 前 / 函数指针 | 参数表裸 `(...)` 是 GCC 扩展 | → `()` |
| LNK2005 vs kernel32/user32/... | win32_api_stubs.c 定义了系统 DLL 导出同名函数 | 删定义让系统接管; 对照集 = `dumpbin /linkermember:1 <sdk_lib>` 全集。该文件现仅剩 13 个非系统符号 (*_exref/operator_new 等) |
| LNK2019 `_snwprintf`/`_vsnwprintf` | UCRT 无导入符号 (真 CRT 头内联) | 转发真体走 `__stdio_common_vswprintf` (见 win32_api_stubs.c); **该文件禁止 #include <stdio.h>** (会撞内联定义) |
| C2371 重定义/C2129 static 未定义 | 隐式 int 声明遇 void 定义 / 跨 TU static | 补前置声明 (调用点之前) / 去 static |
| C2036 void* 未知大小 | GNU void* 算术扩展 | 显式 `(char*)` 步长。注意 Ghidra `int* +N` = 字节 `+4N` — 曾据此把 `g_pMapBlk+6` 修正为 `+0x18` (顺带修了字段偏移 bug) |
| C7744 `\xXXXX` 越界 | `WSTR("A" "B")` 续行段丢 L 前缀 (宏只粘首段) | 合并为单一字面量; ASCII 续行段可容忍 (窄宽拼接是 MSVC/GCC 共有扩展) |
| C2143 label 后缺 ; | 标签后无语句 | 加空语句 `;` |
| C4235 `__int128` | MSVC C 无 128 位整型 | 取高 64 位 → `PECMD_MulHi64` (Hacker's Delight mulhs); `(a*b)/d` → `PECMD_MulDivU64`; `(u64)(u128积)` ≡ 普通乘法截断直接写 |

## 通用纪律

- 改写必须语义等价, 附 diff 证据; 无法核验语义时登记 divergences 而不是猜
- 每处改完跑 syntax 门回归 (`pecmd-build` skill)
- PowerShell 5.1 `Set-Content -Encoding UTF8` 会写 BOM — 批量改码后用 Python 清 BOM
