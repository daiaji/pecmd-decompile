# r25i — L1 语义化试点 C1：FUN_14003C06C（core_b2f.c 7180–7571）

- 对象：`src/commands/core_b2f.c` 内 `FUN_14003C06C @0x14003c06c`（FILE 动词行解析+执行体，
  SHFILEOPSTRUCTW 调用方；`-` 开关族 → `>`/`>>`/`->` 定 wFunc → 源/目标 StrBld 收集 → SHFileOperationW/单文件 API）。
- 语义真值对齐：`reference/decompiled.c:35103` 起 `FUN_14003c06c`（dc 全文逐语句核对）。
- 纯改名：未改任何语句结构/类型/顺序/空白；行界外（含 7180 函数头注释、7571 下一函数头）零触碰。
- 已语义化名（strb1/strb2、形参 script/cmd/flags）与全部 PECMD_*/FUN_*/B2F_LO8 调用名未动。

## 一、改名映射表（旧名 → 新名 | 证据 | 站点数）

| 旧名 | 新名 | 证据（行号为 core_b2f.c 当前行号） | 站点数 |
|---|---|---|---|
| uVar1 | skip_char | 7263–7270 无开关命中分支里逐字符前进跳过未匹配裸词时读到的当前宽字符 | 7 |
| bVar3 | dest_flag_prev | 7353 置 false；7422 每轮末快照 bVar2；7427 循环后据它补计目标分段 | 4 |
| bVar4 | f_delay | 7277 "-delay" 命中置位；7445 走 MoveFileExW(…,6)=DELAY_UNTIL_REBOOT\|REPLACE_EXISTING | 4 |
| bVar5 | f_rd | 7283 "-rd" 命中置位（op_code=2）；7491 走 RemoveDirectoryW | 4 |
| bVar6 | f_su | 7298 "-su" 命中置位（op_code\|=0x10）；7495 删除前试开句柄、7537 走 dd 原样拷贝串 | 5 |
| uVar8 | mflag_cached | 7356/7396/7412 缓存 B2F_LO8(local_res20)（"-m" 开关 7305 置位）；7358 判 ';' 分段开关 | 5 |
| iVar9 | text_len | 7331 lstrlenW(游标处剩余行文本)，用于行副本分配/拷贝长度 | 4 |
| DVar11 | open_lasterr | 7500/7520 PECMD_OpenFileHandle 后 GetLastError() 值（==2 判 FILE_NOT_FOUND） | 5 |
| BVar12 | move_ok | 7476/7477 MoveFileW 返回值（!=1 进错误码链） | 3 |
| LVar14 | rtrim_cursor | 7370–7379 命中 '>' 后自 tok_cursor-4 向 tok_start 回退清空尾随空白/空格的游标 | 10 |
| psVar15 | last_sep | 7464/7470 PECMD_LastPathSeparator 结果（取目录/文件名分界） | 7 |
| LVar17 | tok_cursor | 7340 自 scan_cursor 接管，主扫描循环当前字符游标（7420 每轮 +2） | 21 |
| LVar19 | dest_count_li | 7355 置 0；7381/7414–7416 目标侧 ';' 分段计数（LARGE_INTEGER 槽，HighPart 显式清零） | 9 |
| uVar20 | fflags_acc | 7222 基值 4；"-progress"→0（7313）；"-simpleprogress"→0x100（7317）；7341 fFlags=acc\|0x610 | 5 |
| LVar21 | tok_start | 7352/7395/7411 当前 token 起始游标，遇 '>'/';'/结尾整段交给 AppendParamToken | 12 |
| WVar24 | cur_char | 7351/7421 主扫描循环当前宽字符（判 0/';'/'>'） | 7 |
| bVar25 | op_code | 7223 清零；"-file"→1（7309）；"-rd"→2（7282）；"-su"\|=0x10（7299）；7403 判 0；7453 分派 | 8 |
| local_res10 | scan_cursor | brief 锚点：当前扫描游标（WCHAR* 装 LARGE_INTEGER 槽）；7230=cmd，7335 起指向行私有副本 | 32 |
| local_c5 | progress_flag | brief 锚点：进度 UI 标志；"-q" 置 0x10（7293）；7441 传入 PECMD_DeleteDirectoryTree | 4 |
| local_b8 | dest_count | 7344 置 0；7381/7416 与 dest_count_li.LowPart 互为驻留镜像（跨 '>' 保持计数） | 4 |
| local_b0 | shfo | brief 锚点：_SHFILEOPSTRUCTW（.hwnd/.wFunc/.fFlags/.pFrom/.pTo），7438/7459 传 SHFileOperationW | 39 |
| local_78 | line_copy_buf | brief 锚点：行文本拷贝缓冲；7332 分配 (len+1)*2+4，7567 PECMD_FreeStrBuf 释放 | 6 |

合计改名 205 站点，22 个名字。命名冲突核查：22 个新名在本文件、include/、src/ 全树 `grep -rn "\b名\b"` 均 0 命中
（core_b2d.c 的 `cur_char`/`text_len` 为彼 TU 局部名，不同编译单元不冲突，且与本试点 snake_case 风格一致）。

## 二、保留原名清单及原因（Ghidra 复用槽，单名必误导其一；宁缺毋滥）

| 保留名 | 原因 | 站点数 |
|---|---|---|
| bVar2 | 双角色：7321 前为 "-delme"→INDATA 标志（brief 锚点，7288 置位、7326 早退）；7345 复位后在扫描循环复用为「已过 '>' 进入目标侧」标志（7393/7403/7413/7422） | 9 |
| cVar7 | 双角色：7235–7262 开关匹配结果；7403–7407 复用为 AppendParamToken 第三参（按 bVar2/op_code 计算，该参语义未定案） | 28 |
| uVar10 | 双角色：`flags` 形参缓存（7231–7300 各分支回写、7437/7438 判调用方标志位）；7478–7483/7557–7562 复用为 GetLastError 草稿 | 19 |
| iVar18 | 双角色：目标侧分段计数副本（7419/7428/7433/7456，驱动 pTo 赋值与 FOF_MULTIDESTFILES）；7438 起+各单文件 API 返回值（7459/7492/7534/7547/7552/7556） | 17 |
| lVar13 | 三角色：PECMD_RunCommand 结果（7324/7326）；行副本字节长（7333–7336）；PECMD_OpenFileHandle 句柄（7497/7501/7517/7521） | 10 |
| LVar22 | 双角色：主结果载具（多数站点，7568 返回）；7465–7474 复用为改名路径中 pTo 文件名游标 | 21 |
| LVar23 | 双角色：0 常量初值（7219/7355/7439/7448/7555）；7482–7486 复用为改名路径错误码暂存（Ghidra 寄存器对复用伪影） | 9 |
| pLVar16 | 双角色：当前 token 构建器指针（7348=&strb1 → 7394=&strb2）；7485/7544 复用为 LAB_14003c8ca 待释放缓冲指针（&local_c0 / &local_res20） | 9 |
| local_c0 | 双角色：分隔符后下一 token 游标中转（7385–7411）；7463–7476 复用为改名目标合成路径 StrBld（FUN_1400702B0+FUN_14006375C） | 15 |
| local_res20 | 多角色：低字节="-m" 标志位（B2F_LO8）；整体复用为 "#23:INDATA" 命令串缓冲（7322–7325）、PECMD_OpenFileHandle 句柄出参槽（7497–7531）、dd 命令串缓冲（7538–7544） | — |

## 三、行界内残留 Ghidra 名计数（全部属上表保留项，无遗漏）

```
LVar22:21  LVar23:9  bVar2:9  cVar7:28  iVar18:17  lVar13:10  local_c0:15  pLVar16:9  uVar10:19
合计残留 137（= 保留项站点总数；local_res20 的整体使用不计入正则 local_[0-9a-f]+ 模式，因其含下标字母）
```
另：`local_res20` 在界内出现 24 次（含 B2F_LO8 站点），已列入保留清单；界内已无非保留类 Ghidra 名。

## 四、验证记录

1. 改写脚本：python `\bold\b` 正则，仅作用于 7180–7571 行（0 基 7179..7570）；行界外逐行断言逐字节一致。
2. `git diff -U0 -- src/commands/core_b2f.c`：84 hunk / 174 行对，行号跨度 7185–7567（均在界内）；
   对每对 -/+ 行施加映射反推，174/174 完全复原 → 零非改名差异。
3. `git status`：本任务仅 `M src/commands/core_b2f.c`；`M src/commands/core_b2d.c` 与
   `?? analysis/r25i_l1_pilot_b2d_pcb.md` 为并行子代理（C2/b2d）工作区改动，与本次无关。
4. 未构建、未提交（守纪律）；改名后函数体经 Read 复核（声明区/开关循环/扫描循环/收尾释放）语义读感一致。
