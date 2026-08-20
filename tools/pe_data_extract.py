#!/usr/bin/env python3
# PE 数据段取值提取器：从原 PECMD 原始.EXE 按绝对地址取字节
import struct,sys
def load(path):
    d=open(path,'rb').read()
    e=struct.unpack('<I',d[0x3c:0x40])[0]
    num=struct.unpack('<H',d[e+6:e+8])[0]
    optsz=struct.unpack('<H',d[e+20:e+22])[0]
    ib=struct.unpack('<Q',d[e+24+24:e+24+32])[0]
    secs=[]; st=e+24+optsz
    for i in range(num):
        off=st+i*40
        name=d[off:off+8].rstrip(b'\x00').decode('latin1')
        vsize,va,rsize,raw=struct.unpack('<IIII',d[off+8:off+24])
        secs.append((name,va,raw,vsize))
    return d,ib,secs
def va2off(d,ib,secs,addr):
    rva=addr-ib
    for name,va,raw,vsize in secs:
        if 0<=rva-va<vsize: return raw+(rva-va)
    return None
def dump(path,addr,n):
    d,ib,secs=load(path); off=va2off(d,ib,secs,addr)
    return d[off:off+n] if off is not None else None
if __name__=='__main__':
    print(dump(sys.argv[1],int(sys.argv[2],16),int(sys.argv[3])).hex(' '))
