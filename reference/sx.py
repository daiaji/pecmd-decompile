#!/usr/bin/env python3
"""Extract function body from decompiled.c by address."""
import re,sys
with open('/home/daiaji/repo/PECMD反编译/decompiled.c') as f:
    text = f.read()
def extract(addr):
    idx = text.find('FUN_%s @' % addr)
    if idx<0: return None
    hs = text.rfind('/* =', 0, idx)
    ne = text.find('/* =====', idx)
    return text[hs:ne]
if __name__=='__main__':
    for a in sys.argv[1:]:
        print("##### %s #####" % a)
        print(extract(a))
        print()
