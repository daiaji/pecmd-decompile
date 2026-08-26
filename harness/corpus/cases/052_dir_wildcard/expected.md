# 052_dir_wildcard — DIR 通配展开计数

## 断言要点
- 自建恰好 3 个匹配 `s52_*.txt` 的文件（a/b/c），通配计数应恒为 3。
- `IFEX $%N%=3` 派生标志；N 值确定可对拍。

## 方言置信度：ASSUMED
矩阵记「`DIR <wild>,<var>` 计数对拍」（3 字母分支，DirWildcardExpand）；本案采用库内主流的 `结果变量,参数` 序。若原版是「通配在前」，落入 `dir_other` 并锁定真实行为。

## 预期产物
- `vars.txt`（主假设）: `CASE=052_dir_wildcard|N|3|R|dir_cnt3|BODY_DONE`
  - 替代合法落点: N=首文件名/空 + `R|dir_other`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
- 创建 `C:\pectest\out\s52_{a,b,c}.txt`（下轮清除，不入 golden）。

## 人工核对步骤
1. 核对 N 捕获值是否为计数形态（数字 3 或记录实际语义）。
2. R 与 N 自洽即可验收。
