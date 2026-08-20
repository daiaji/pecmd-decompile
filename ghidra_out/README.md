# Ghidra 12.1.3 对这 6 个 [DECOMPILE FAILED] 函数的二次反编译结论
- C 反编译器: 全部 6 个失败 — "Overlapping input varnodes" / "Unable to find unique hash for varnode"
  (Ghidra 对特定寻址手法的已知 C-decompiler 局限, 新版 12.1.3 未解决)
- 但 objdump 指令级反汇编完整可用: 见 asm/ 目录(每函数 2048B 窗口; 实际 size 按 header 大小)
- 地址/实际 size: 005c18(49) 034788(4312) 05fc90(233) 0682b0(397) 0e4864(980) 0fdad8(1025)
- 恢复路径: 汇编→手工重建 C (已在 refactored 中登记为"待 asm→C 重建")
- 这 6 个均被真实代码调用(core 引用 1-15 处), 非死代码
