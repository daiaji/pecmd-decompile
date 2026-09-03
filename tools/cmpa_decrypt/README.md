# cmpa_decrypt — PECMD CMPS/CMPa 解密工具与样本

纯静态还原 PECMD `CMPS` 动词加密产物（魔数 `CMPa`/'a' 模式）。算法、证据、边界见
[`docs/cmpa_cryptography_notes.md`](../../docs/cmpa_cryptography_notes.md)。

## 用法

```bash
python cmpa_decrypt.py <in.cmp> <out.wcs> [mode_hex]   # mode 默认 0x14（'a' 模式）
# 产出 <out.wcs>（UTF-16LE 原始明文）+ <out.wcs.txt>（UTF-8-sig，可直接读）
```

## 样本与验证产物

| 文件 | 说明 |
|---|---|
| `zh_test.cmp` / `zh_test_out.wcs(.txt)` | 中文脚本加密→解码 回归样本（明文 `张三/北京/你好/计算结果`） |
| `drv101.bin` | DrvIndex SCRIPT 101（入口，LOAD #202/#203）→ `drv101.wcs.txt` |
| `drv202.bin` | DrvIndex SCRIPT 202 壳（内嵌 202 文件 hex）→ `drv202b.wcs.txt` |
| `drv202.cmp` | 203 壳内嵌的 CMPa（= 目录"203 文件"，66918B）→ `drv202.wcs.txt`（驱动索引主脚本 255KB） |
| `drv203.bin` | DrvIndex SCRIPT 203 壳（内嵌 203 文件 hex）→ `drv203_out.wcs.txt` |
| `fbfinfo.cmp` | 202 壳内嵌的 CMPa（= 目录"202 文件"，1723B）→ `fbfinfo.wcs.txt`（7-Zip/FBF 逻辑） |

来源：`/d/PECMD-解密-程序/x64/DrvIndex.unp.exe._SCRIPT__{101,202,203}_2052.bin` 与
`{202,203}` 文件（2026-09 取证，仅供格式验证）。

## 待验证

- 'S'(0x53) / 'M'(0x4d) 模式：mode 值已推导（0 / 0x18），无真实样本。
- 编码方向（对称 XOR 自反；LZ 压缩器未移植）。
