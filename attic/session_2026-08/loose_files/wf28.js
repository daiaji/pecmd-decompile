const groups = [
  {file:'gA.txt', out:'outA.txt', label:'group A (7 funcs)'},
  {file:'gB.txt', out:'outB.txt', label:'group B (8 funcs)'},
  {file:'gC.txt', out:'outC.txt', label:'group C (7 funcs)'},
  {file:'gD.txt', out:'outD.txt', label:'group D (6 funcs)'},
];
const results = await parallel(groups.map(g => () => agent(
`你在 /home/daiaji/repo/PECMD反编译/refactored 工作。

先完整阅读 /home/daiaji/repo/PECMD反编译/refactored/agent_instructions.md（批次28 函数重构子代理指令），然后：
1. 读取 /home/daiaji/repo/PECMD反编译/refactored/${g.file}（含你负责的函数的反编译原始体）。
2. 按指令把每个函数重构为可编译 C 代码，写入 /home/daiaji/repo/PECMD反编译/refactored/${g.out}（只写函数体，带 /* @0x<addr> */ 头注释）。
3. 参考已重构文件 /home/daiaji/repo/PECMD反编译/refactored/core_b3r_a.c 的调用风格。
最后用 report 简要汇报每个函数状态 + 疑似未定义符号清单。`,
{label: g.label}
)));
return results;
