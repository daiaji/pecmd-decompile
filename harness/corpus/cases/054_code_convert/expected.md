# 054_code_convert — CODE 文本转码

## 断言要点
- `CODE %F%=>C:\pectest\out\s54_out.txt`：`=>` 目标文件语法镜像已实证的 FILE 复制（010）。
- `IFEX` 检查输出文件存在性派生标志。输入为纯 ASCII（`code54`），任何 UTF-8↔ANSI 转换都应字节等价。

## 方言置信度：ASSUMED（中等）
矩阵记「UTF8↔ANSI 小文本转码」；未带编码旗标（最简形式）。若原版要求显式编码参数，可能落入 `code_none`——锁定后人工回填真实旗标。

## 预期产物
- `vars.txt`: `CASE=054_code_convert|R|code_out|BODY_DONE`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
- 创建 `C:\pectest\out\s54_in.txt`、`s54_out.txt`（下轮清除；若需核对内容，录制时手工比对两文件均应为 `code54`+行尾）。

## 人工核对步骤
1. 录制当场用 `fc` 或字节查看确认 s54_out.txt 存在且内容与输入等价（ASCII 无损性检查）。
2. R=code_out 即验收；code_none 时登记方言差异。
