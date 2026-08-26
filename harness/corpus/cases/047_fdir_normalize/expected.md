# 047_fdir_normalize — FDIR 相对路径归一化

## 断言要点
- `ENVI REL=out\s47_probe` + `FDIR FULL,%REL%`：相对路径应展开为绝对路径。
- runner 以 `cwd=C:\pectest` 启动 EXE（run_case.py::run_exe），故期望 `C:\pectest\out\s47_probe`。
- `FIND $%FULL%=C:\pectest\out\s47_probe` 派生标志。

## 方言置信度：ASSUMED（中等）
矩阵记「相对路径归一正例」，处理函数 NormalizePath(core_b3r_h1)；参数序 `结果变量,源串` 沿用 LSTR(029) 实证风格。
若 FDIR 实际语义是「取目录部分」而非归一化，将落入 `fdir_other`——仍为有效锁定态。

## 预期产物
- `vars.txt`（主假设）: `CASE=047_fdir_normalize|FULL|C:\pectest\out\s47_probe|R|fdir_abs|BODY_DONE`
  - 替代合法落点: FULL=其他归一形态 + `R|fdir_other`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`（或既有 errorlevel 形态 2）
- `stdout.txt`: 空

## fs 副作用
无（纯字符串运算，不创建目录）。

## 人工核对步骤
1. 核对 FULL 捕获值是否为绝对路径形态（含 `C:\pectest\out\s47_probe` 或其变体）。
2. R 分支与 FULL 内容自洽即可验收；不一致时回填真实语义注记。
