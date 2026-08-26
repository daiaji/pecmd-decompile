# 053_hash_probe — HASH 文件哈希（方言探针）

## 断言要点
- `WRITE` 固定内容 `hash53` 后 `HASH H,%F%`：矩阵明记「方言待考」——算法与参数序均未考证。
- 设计取舍：不猜期望值、不设 ok/bad 分支，直接以 `%H%` 值捕获把真实输出字节锁进 golden；
  输入内容固定 ⇒ 无论算法是 CRC32/MD5/自研，同一实现重复运行值恒定，逐字节可比。

## 方言置信度：ASSUMED（低，纯探针）
若参数序相反（`HASH %F%,H`），H 将为空——golden 锁定空值，同样有效。

## 预期产物
- `vars.txt`: `CASE=053_hash_probe|H|<哈希串或空>|BODY_DONE`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
- 创建 `C:\pectest\out\s53_src.txt`（下轮清除）。

## 人工核对步骤
1. 核对 H 是否为十六进制/十进制摘要形态，长度可判别算法（4 hex=CRC32 类、32 hex=MD5 类），回填注记。
2. H 为空不判 FAIL（方言未证），但须在报告登记待考古项。
