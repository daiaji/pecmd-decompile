# 059_link_shortcut — LINK 快捷方式创建

## 断言要点
- 先 WRITE 目标文件，再 `LINK <lnk路径>,<目标>` 创建快捷方式；`IFEX` 断言 .lnk 落盘。
- 副作用完全限制在 `C:\pectest\out\`（runner 每轮清空），不污染桌面/开始菜单。

## 方言置信度：ASSUMED（中等）
矩阵记「.lnk 创建→fs 对拍」（LinkCreateShortcut core_b2f）。参数序 `lnk,目标` 为假设；
若原版为 `目标,lnk` 序，可能把 .lnk 建到 s59_target.txt 同目录或失败——两种落点都确定可锁。

## 预期产物
- `vars.txt`: `CASE=059_link_shortcut|R|link_ok|BODY_DONE`
  - 替代合法落点: `R|link_none`
- `done.txt`: `OK`
- `exit.txt`: `exit=0`
- `stdout.txt`: 空

## fs 副作用
- 创建 `C:\pectest\out\s59_target.txt` 与 `C:\pectest\out\s59.lnk`（下轮清除）。
- 录制当场可人工双击校验 .lnk 指向 target（可选）。

## 人工核对步骤
1. R=link_ok 时顺手确认 s59.lnk 字节头为 `4C 00 00 00`（Shell Link 魔数），证明是真 .lnk 而非普通拷贝。
2. link_none 时登记方言差异（参数序疑点）。
