# CMPa/CMPS 加密格式逆向笔记（2026-09-03 实证）

> 目标：纯静态还原 PECMD CMPS 加密（`CMPS` 动词产物），不依赖原版 EXE 运行（动态钩子路线另案废弃）。
> 状态：**CMPa（'a' 模式）全链路解密已实现并三重验证**；'S'/'M' 变体结构已推导、无真实样本未验证。
> 交付：`tools/cmpa_decrypt.py`（独立解码器）+ 本文档；工作样本见 `cmps_samples/`。

## 命名澄清（易混点）

- **CMPS** = PECMD **动词/命令名**（FOURCC_CMPS=0x53504D43 → `PECMD_ProcessEncodedFile @0x140086944`，r25g 登记真体）。文件内**不含** "CMPS" 字样。
- **CMPa / CMPS / CMPM** = 文件**魔数**（偏移 4 起 `CMP`+模式字节）：默认→`'a'`(0x61)、`-udm`→`'M'`(0x4d)、其余（`-u`/`-m` 等）→`'S'`(0x53)。模式字节即文件第 8 字节。
- 文件结构是 **XOR 包 LZ**，不是"壳套壳"；DrvIndex 里的嵌套是**应用层**（脚本 `SET$#` 嵌另一 CMPa 的 hex 再写盘 LOAD），与格式本身无关。

## 文件格式（CMPa 'a' 模式，实测）

```
偏移 0x00  8 字节   00000000 + "CMPa"        （签名区，前 4 字节零 + 魔数）
偏移 0x08  4 字节   种子 seed（小端，明文）    ← 密钥信息在文件头，无口令
偏移 0x0C  4 字节   保留（全零）
偏移 0x10  数据     XOR(LZ(明文))             ← 加密包着压缩
明文 = UTF-16LE（可含中文）
```

## 算法（三层）

### 1. 种子 → 密钥 X
```
K   = 0x5aa59669                     （固定常数，非口令）
X   = ((seed>>16)<<16 | ((seed & 0xffff) ^ mode)) ^ K
mode = 0x14 ('a' 默认) / 0x18 ('M') / 0（'S' 待验）
mul = X（全 32 位；动态实测对象 +0x20 = X）
```
编码方向：`X ^ K` → 低16 ^ mode 拼 seed，故解码反推即上式。

### 2. XOR 逐字节变换（每字节一个密钥字节）
```
n 从 0x10 起逐字节递增（对象 +0x10，64 位计数，初值 16）
t   = (2n + 3) * X    （32 位截断）
k   = ((t>>21)&0x7E) ^ ((t>>14)&0xFF) ^ ((t>>7)&0xFF) ^ (t&0xFF)
out = in ^ k
```
证据：解码读函数 `0x140068830`（对象 +0x8 数据 / +0x10 计数 / +0x18 长 / +0x20 mul；`imul [rcx+20h]`、`lea r8d,[r10+r10+3]`、四次 xor 折叠、`n+1` 回写）。加密写 `0x1400684fc` 同式（XOR 自反，加解密同族）。

### 3. LZ 解压（LZ77 变体，FUN_14005c260 @0x14005c260 直移，306B）
```
位流：16 位缓冲 uVar2；每消费 1 位先判 (uVar2>>9)&1，
      为 0 则读入新字节 b 并 uVar2 = b | 0xff00（bit9=1 哨兵=低 8 位是新数据）
      ★ 判空须用移位前的 bit9（先存 flag 再 >>1），否则错位
位=1 → 字面量：读 1 字节直写
位=0 → 匹配：a=read8; b2=read8; off = a | ((b2&0xf0)<<4)（12 位）;
      len = (b2&0xf) + 3; 窗内从 off 起顺序拷 len 字节
窗：4096B 环形；初始化预填 0x20 × 0xfee 字节；写指针 wpos 从 0xfee 起，写后 (wpos+1)&0xfff
```
★ 两个易错点（实测踩坑）：位流判空顺序（先 flag 后移位）；窗预填 0x20 且 wpos=0xfee（不从 0 起、不预填 0 都会导致匹配解出 0x20/0x00 填充）。

## 验证证据（三重）

1. **自产回归**：`PECMD CMPS zh_test.wcs zh_test.cmp`（含中文脚本）→ 解码 → 明文逐字节一致（UTF-16LE 中文 OK）。
2. **动态对拍**：WinDbg 断 `0x140068830`（解码读）实测对象 mul/n0；断 `0x1400684fc`（编码写）现场 rax=0x517ecd7b 佐证 X 推导；LZ 解压 `FUN_14005c260` 命中于 `LOAD` CMPa 时。
3. **真实样本**：`DrvIndex.unp.exe` 的 SCRIPT 101/202/203 全链解出（见下）。

## DrvIndex 应用层嵌套实证

```
DrvIndex.unp.exe
├ SCRIPT 101 (434B)      → LOAD #202 或 #203（-s 参数 / DPI 感知判断）
├ SCRIPT 202 (3434B)     → SET$# &FBFInfo=<hex> → PUTF#* %Temp% → LOAD -del
│   └ 内嵌 = 202 文件 (CMPa 1723B) → 明文 5376B（7-Zip/FBF 解压逻辑，Sevenz.dll）
└ SCRIPT 203 (117666B)   → SET$# &infodrv=<hex> → PUTF#* %Temp% → LOAD -del
    └ 内嵌 = 203 文件 (CMPa 66918B) → 明文 255KB（驱动索引主脚本，131 _SUB/257 行中文 UI）
```
注：资源号 ≠ 目录文件号（资源 202 内嵌的是"202 文件"……）。`SET$# &var=<hex>` = 设二进制变量；`PUTF#* 路径,0,&var` = 写文件；`LOAD -del` = 执行后自删。

## 工具与用法

```bash
python tools/cmpa_decrypt.py <in.cmp> <out.wcs> [mode_hex]   # mode 默认 0x14
# 产出 out.wcs（UTF-16LE 原始）+ out.wcs.txt（UTF-8-sig，可直接读）
```
- 控制台自动转 UTF-8（Windows GBK 管道会乱码，脚本内 io.TextIOWrapper 处理）。
- 中文明文：UTF-16LE → 逐字节正确；预览/落盘 UTF-8 无乱码。

## 边界与未决

- **'S'（0x53）/ 'M'（0x4d）模式未用真实样本验证**：mode 值按 dc 选项映射推导（' '/`-udm`），XOR/LZ 层结构应同，待样本。
- 编码方向未写成工具（无需求）；如需可对称实现（XOR 自反 + LZ 压缩器需另写）。
- `ProcessEncodedFile` 另含 `-src` 源码预处理（CleanScriptText 删注释/压空行）、`-bin`、`-utf8`、`-f`(头 4 字节清零)、摘要(DigestUpdate)等面，本次未全验。
- 本能力对应 matrix `CMPS` = deprecated（旧格式默认不做）；如需正式纳入（旧脚本/资源兼容）需翻状态 + 补对拍测试。

## 相关地址速查

| 符号 | 地址 | 说明 |
|---|---|---|
| PECMD_ProcessEncodedFile | 0x140086944 | CMPS 动词主函数（dc 85711，3401B） |
| 流对象 vtable | 0x140126540 / 560 / 580 | 变换流 / 裸流 / 解码流（.rdata 数据指针） |
| 解码读字节 | 0x140068830 | XOR 变换读（对象 +0x20=mul, +0x10=n） |
| 编码写字节 | 0x1400684fc | XOR 变换写（同式） |
| LZ 解压 | 0x14005c260 | LZ77 4KB 窗（dc 54688，306B） |
| 头签名区 | 0x140124d00 | `00 00 00 00 43 4d 50 78`（'x' 占位→运行时写模式字节） |
| 随机种子 | g_randSeedAccum 区 | `time+tick+PRNG` 混入，约束循环（低 2 位=3、各 nibble 非 0 等） |
