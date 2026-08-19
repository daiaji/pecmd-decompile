#!/usr/bin/env python3
"""cleanup.py — 伪代码自动化清洗

处理项:
1. 删除 MSVC 0xCC 栈填充循环 (for + 0xcccccccc)
2. 删除 chkesp / local_8 = (T*)0x100xxxxx 尾部噪声（反编译瑕疵标记）
3. thunk 函数体仅剩 jmp 时折叠为注释
4. 0xCCCCCCCC 字面量替换为注释
用法:
  cleanup.py <decompiled.c> [-o out.c]
"""
import re
import sys
import os


CC_LOOP = re.compile(
    r'  (?:undefined4|int) \*puVar\d+ = local_\w+;\n'
    r'  for \(iVar\d+ = 0x[\da-fA-F]+; iVar\d+ != 0; iVar\d+ = iVar\d+ \+ -1\) \{\n'
    r'    \*puVar\d+ = 0xcccccccc;\n'
    r'    puVar\d+ = puVar\d+ \+ 1;\n'
    r'  \}\n',
)
CC_VAR_DECL = re.compile(
    r'  undefined4 local_\w+ \[\d+\];\n',
)


def clean(text):
    # 1. 删 0xCC 填充循环
    text = CC_LOOP.sub('', text)
    # 2. 删配套的数组声明
    text = CC_VAR_DECL.sub('', text)
    # 3. chkesp() 与尾部噪声
    text = re.sub(r'\n  chkesp\(\);\n', '\n', text)
    text = re.sub(r'\n  local_\w+ = \([A-Za-z_][\w ]*\)0x100[0-9a-fA-F]+;\n', '\n', text)
    # 4. thunk 折叠: 函数体只有 jmp 注释? Ghidra 不输出反汇编, 这里处理空体
    text = re.sub(r'\{\n\}\n', '{\n  /* thunk: 跳转到真实实现 */\n}\n', text)
    return text


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)
    src = sys.argv[1]
    out = sys.argv[sys.argv.index('-o') + 1] if '-o' in sys.argv else None
    with open(src, 'r', encoding='utf-8', errors='replace') as fh:
        text = fh.read()
    orig_lines = len(text.splitlines())
    cleaned = clean(text)
    new_lines = len(cleaned.splitlines())
    print(f"{src}: {orig_lines} -> {new_lines} lines (-{orig_lines-new_lines})")
    if out:
        os.makedirs(os.path.dirname(out), exist_ok=True)
        with open(out, 'w', encoding='utf-8') as fh:
            fh.write(cleaned)
        print(f"written: {out}")


if __name__ == '__main__':
    main()
