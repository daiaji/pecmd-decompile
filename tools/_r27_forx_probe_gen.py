# -*- coding: utf-8 -*-
# FORX /L 与 NL 语法探测脚本生成器 (原版 PECMD.EXE 实测)
import os

D = r'C:\pectest\forx_probe'
os.makedirs(D, exist_ok=True)

# 探测脚本: 每个 FORX 变体循环体拼接 ACC 变量, 循环外 WRITE 落盘捕获
probe_lines = [
    "ENVI ACC1=start",
    "FORX /L W,1,5,2,ENVI ACC1=%ACC1%#%W%",
    "WRITE " + D + r"\out1.txt,%ACC1%",
    "ENVI ACC2=start",
    "FORX /L W,1,2,5,ENVI ACC2=%ACC2%#%W%",
    "WRITE " + D + r"\out2.txt,%ACC2%",
    "ENVI SRC=x#y#z",
    "ENVI ACC3=start",
    "FORX NL:# W,%SRC%,ENVI ACC3=%ACC3%#%W%",
    "WRITE " + D + r"\out3.txt,%ACC3%",
    "ENVI DONE=yes",
    "WRITE " + D + r"\out4.txt,%DONE%",
]
with open(os.path.join(D, 'probe.pecmd'), 'w', newline='\r\n') as f:
    f.write('\n'.join(probe_lines) + '\n')
print("probe.pecmd written:")
print('\n'.join(probe_lines))
