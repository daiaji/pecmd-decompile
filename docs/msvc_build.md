# msvc_build.md — MSVC 构建管线（P0-③ 产出文档）

> 2026-08-24 交付。配套脚本：`tools\msvc_build.bat`（ASCII-only，规避 cmd 代码页坑）。
> 纯 WIN 化裁定见 PRODUCTION_ROADMAP §4.1：构建/验证全在原生 Windows，无 WSL/git-bash。

## 1. 前置

| 项 | 要求 |
|---|---|
| Visual Studio 2019 | **16.8+**，勾选"C++ 生成工具"（含 C 编译器；`_Static_assert`/`/std:c11` 需 16.8+） |
| 运行入口 | `x64 Native Tools Command Prompt for VS 2019`，或任意终端先 `call vcvarsall.bat x64` |

## 2. 命令

```bat
cd /d <仓库根>
tools\msvc_build.bat syntax     :: 语法门 (cl /Zs, 只查语法不产出) — WIN 端第一道门
tools\msvc_build.bat            :: 完整构建 → build\msvc\pecmd_msvc.exe
```

- 脚本自动收集 `src/**/*.c` + 桩四件套（restored_bodies/win32_api_stubs/unimplemented_stubs/crt_shims），经响应文件传给 cl（~99 TU 超出 cmd 8K 命令行上限）
- 编译选项：`/W4 /std:c11 /utf-8 /Zi`；链接库：advapi32/user32/gdi32/shell32/ole32/ws2_32/winmm（Win32 API 已桩化，多余库无害）

## 3. 当前预期状态（P0 未完成时首跑必红，属预期）

| 阶段 | 首跑现象 | 对应工作块 |
|---|---|---|
| P0-① 前 | ~33 处 `C2059/C2143/C2065`（`(LARGE_INTEGER){...}` 复合字面量） | 改写后逐处回归 syntax 门 |
| P0-② 前 | 链接期冲突：crt_shims 内的 CRT 机制桩/入口桩与系统 CRT 重定义（LNK2005/LNK1169 等） | 按 HANDOVER P0-② 清单删桩 |
| 全部完成后 | `[msvc_build] OK: build\msvc\pecmd_msvc.exe produced.` | 拷至 `C:\pectest\` 供 run_case.py 双跑 |

## 4. 故障速查

| 症状 | 原因 | 处理 |
|---|---|---|
| `cl.exe not in PATH` | 未用 VS 开发者命令行 | 见 §1 运行入口 |
| C4819 / C2001 中文注释附近 | 缺 `/utf-8`（脚本已内置，勿删） | 检查是否手改了 CFLAGS |
| LNK2019 unresolved external | 业务缺口桩缺真体 | FAIL 分诊流程（HANDOVER §6），补 unimplemented_stubs.c |
| LNK2005 / 入口点重复 | crt_shims 裁剪未完成 | 推进 P0-② |
| 响应文件报错 | 路径含空格未加引号 | 保持 rsp 内 `"%%f"` 引号格式 |
| 命令行超长 D2001 类错误 | 绕过了响应文件直传源文件 | 不要绕过 @sources.rsp |

## 5. 与验证闭环的衔接

```bat
copy build\msvc\pecmd_msvc.exe C:\pectest\
python harness\runners\run_case.py 001_envi_smoke          :: 双跑(原版+产物)
python harness\runners\diff_case.py 001_envi_smoke         :: verdict
```

门禁口径（双绿门 v4.1）：`syntax 门全绿` + `完整构建 exit 0`。
`build.sh` 为 Linux gcc 参考门，留档不作为 WIN 门禁。
