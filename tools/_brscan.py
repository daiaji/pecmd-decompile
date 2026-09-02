import sys

path = sys.argv[1] if len(sys.argv) > 1 else 'src/commands/core_b3r_i28g.c'
lines = open(path, encoding='utf-8').read().split('\n')
depth = 0
report = []
for i, ln in enumerate(lines, 1):
    j = 0
    n = len(ln)
    in_str = False
    str_ch = ''
    in_block = False
    opens = 0
    while j < n:
        c = ln[j]
        if in_block:
            if c == '*' and j + 1 < n and ln[j + 1] == '/':
                in_block = False
                j += 2
                continue
            j += 1
            continue
        if in_str:
            if c == chr(92):
                j += 2
                continue
            if c == str_ch:
                in_str = False
            j += 1
            continue
        if c == '/' and j + 1 < n and ln[j + 1] == '/':
            break
        if c == '/' and j + 1 < n and ln[j + 1] == '*':
            in_block = True
            j += 2
            continue
        if c == '"' or c == "'":
            in_str = True
            str_ch = c
            j += 1
            continue
        if c == '{':
            opens += 1
        elif c == '}':
            opens -= 1
        j += 1
    if opens != 0:
        depth += opens
        report.append((i, depth, opens, ln.strip()[:90]))
print("final depth:", depth)
for r in report:
    print(r[0], "depth->", r[1], "delta", r[2], "|", r[3])
