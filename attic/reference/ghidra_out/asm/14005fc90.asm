
PECMD.exe：     文件格式 pei-x86-64


Disassembly of section .text:

000000014005fc90 <.text+0x5ec90>:
   14005fc90:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   14005fc95:	48 89 6c 24 10       	mov    QWORD PTR [rsp+0x10],rbp
   14005fc9a:	48 89 74 24 18       	mov    QWORD PTR [rsp+0x18],rsi
   14005fc9f:	57                   	push   rdi
   14005fca0:	41 54                	push   r12
   14005fca2:	41 55                	push   r13
   14005fca4:	41 56                	push   r14
   14005fca6:	41 57                	push   r15
   14005fca8:	48 81 ec 80 00 00 00 	sub    rsp,0x80
   14005fcaf:	44 0f b6 4a 0e       	movzx  r9d,BYTE PTR [rdx+0xe]
   14005fcb4:	44 0f b6 52 0d       	movzx  r10d,BYTE PTR [rdx+0xd]
   14005fcb9:	44 0f b6 5a 0c       	movzx  r11d,BYTE PTR [rdx+0xc]
   14005fcbe:	4c 8b f2             	mov    r14,rdx
   14005fcc1:	45 85 c0             	test   r8d,r8d
   14005fcc4:	45 8b e0             	mov    r12d,r8d
   14005fcc7:	45 0f b6 46 09       	movzx  r8d,BYTE PTR [r14+0x9]
   14005fccc:	41 0f b6 5e 08       	movzx  ebx,BYTE PTR [r14+0x8]
   14005fcd1:	41 0f b7 7e 06       	movzx  edi,WORD PTR [r14+0x6]
   14005fcd6:	41 0f b7 76 04       	movzx  esi,WORD PTR [r14+0x4]
   14005fcdb:	48 8d 05 7e 5d 0c 00 	lea    rax,[rip+0xc5d7e]        # 0x140125a60
   14005fce2:	4c 8b f9             	mov    r15,rcx
   14005fce5:	0f b6 4a 0b          	movzx  ecx,BYTE PTR [rdx+0xb]
   14005fce9:	0f b6 52 0a          	movzx  edx,BYTE PTR [rdx+0xa]
   14005fced:	4c 8d 2d 44 c9 0b 00 	lea    r13,[rip+0xbc944]        # 0x14011c638
   14005fcf4:	49 8b ed             	mov    rbp,r13
   14005fcf7:	48 0f 45 e8          	cmovne rbp,rax
   14005fcfb:	41 0f b6 46 0f       	movzx  eax,BYTE PTR [r14+0xf]
   14005fd00:	45 85 e4             	test   r12d,r12d
   14005fd03:	48 89 6c 24 70       	mov    QWORD PTR [rsp+0x70],rbp
   14005fd08:	89 44 24 68          	mov    DWORD PTR [rsp+0x68],eax
   14005fd0c:	44 89 4c 24 60       	mov    DWORD PTR [rsp+0x60],r9d
   14005fd11:	45 8b 0e             	mov    r9d,DWORD PTR [r14]
   14005fd14:	44 89 54 24 58       	mov    DWORD PTR [rsp+0x58],r10d
   14005fd19:	44 89 5c 24 50       	mov    DWORD PTR [rsp+0x50],r11d
   14005fd1e:	89 4c 24 48          	mov    DWORD PTR [rsp+0x48],ecx
   14005fd22:	89 54 24 40          	mov    DWORD PTR [rsp+0x40],edx
   14005fd26:	44 89 44 24 38       	mov    DWORD PTR [rsp+0x38],r8d
   14005fd2b:	89 5c 24 30          	mov    DWORD PTR [rsp+0x30],ebx
   14005fd2f:	4c 8d 25 26 5d 0c 00 	lea    r12,[rip+0xc5d26]        # 0x140125a5c
   14005fd36:	48 8d 15 b3 5c 0c 00 	lea    rdx,[rip+0xc5cb3]        # 0x1401259f0
   14005fd3d:	4d 0f 45 ec          	cmovne r13,r12
   14005fd41:	49 8b cf             	mov    rcx,r15
   14005fd44:	89 7c 24 28          	mov    DWORD PTR [rsp+0x28],edi
   14005fd48:	4d 8b c5             	mov    r8,r13
   14005fd4b:	89 74 24 20          	mov    DWORD PTR [rsp+0x20],esi
   14005fd4f:	ff 15 e3 c3 0b 00    	call   QWORD PTR [rip+0xbc3e3]        # 0x14011c138
   14005fd55:	4c 8d 9c 24 80 00 00 	lea    r11,[rsp+0x80]
   14005fd5c:	00 
   14005fd5d:	49 8b c7             	mov    rax,r15
   14005fd60:	49 8b 5b 30          	mov    rbx,QWORD PTR [r11+0x30]
   14005fd64:	49 8b 6b 38          	mov    rbp,QWORD PTR [r11+0x38]
   14005fd68:	49 8b 73 40          	mov    rsi,QWORD PTR [r11+0x40]
   14005fd6c:	49 8b e3             	mov    rsp,r11
   14005fd6f:	41 5f                	pop    r15
   14005fd71:	41 5e                	pop    r14
   14005fd73:	41 5d                	pop    r13
   14005fd75:	41 5c                	pop    r12
   14005fd77:	5f                   	pop    rdi
   14005fd78:	c3                   	ret
   14005fd79:	cc                   	int3
   14005fd7a:	cc                   	int3
   14005fd7b:	cc                   	int3
   14005fd7c:	48 8b c4             	mov    rax,rsp
   14005fd7f:	48 89 58 08          	mov    QWORD PTR [rax+0x8],rbx
   14005fd83:	48 89 68 10          	mov    QWORD PTR [rax+0x10],rbp
   14005fd87:	48 89 70 18          	mov    QWORD PTR [rax+0x18],rsi
   14005fd8b:	48 89 78 20          	mov    QWORD PTR [rax+0x20],rdi
   14005fd8f:	33 ed                	xor    ebp,ebp
   14005fd91:	4c 63 da             	movsxd r11,edx
   14005fd94:	49 63 d8             	movsxd rbx,r8d
   14005fd97:	4c 3b db             	cmp    r11,rbx
   14005fd9a:	48 8b f1             	mov    rsi,rcx
   14005fd9d:	49 8b f9             	mov    rdi,r9
   14005fda0:	44 8b d5             	mov    r10d,ebp
   14005fda3:	7f 69                	jg     0x14005fe0e
   14005fda5:	48 8b d5             	mov    rdx,rbp
   14005fda8:	42 8a 4c 5e 01       	mov    cl,BYTE PTR [rsi+r11*2+0x1]
   14005fdad:	46 8a 04 5e          	mov    r8b,BYTE PTR [rsi+r11*2]
   14005fdb1:	40 3a cd             	cmp    cl,bpl
   14005fdb4:	74 24                	je     0x14005fdda
   14005fdb6:	44 3b d5             	cmp    r10d,ebp
   14005fdb9:	75 05                	jne    0x14005fdc0
   14005fdbb:	80 f9 20             	cmp    cl,0x20
   14005fdbe:	74 1a                	je     0x14005fdda
   14005fdc0:	0f b6 c1             	movzx  eax,cl
   14005fdc3:	48 83 c2 02          	add    rdx,0x2
   14005fdc7:	66 41 89 01          	mov    WORD PTR [r9],ax
   14005fdcb:	49 83 c1 02          	add    r9,0x2
   14005fdcf:	80 f9 20             	cmp    cl,0x20
   14005fdd2:	74 06                	je     0x14005fdda
   14005fdd4:	4c 8b d2             	mov    r10,rdx
   14005fdd7:	49 d1 fa             	sar    r10,1
   14005fdda:	44 3a c5             	cmp    r8b,bpl
   14005fddd:	74 27                	je     0x14005fe06
   14005fddf:	44 3b d5             	cmp    r10d,ebp
   14005fde2:	75 06                	jne    0x14005fdea
   14005fde4:	41 80 f8 20          	cmp    r8b,0x20
   14005fde8:	74 1c                	je     0x14005fe06
   14005fdea:	41 0f b6 c0          	movzx  eax,r8b
   14005fdee:	48 83 c2 02          	add    rdx,0x2
   14005fdf2:	66 41 89 01          	mov    WORD PTR [r9],ax
   14005fdf6:	49 83 c1 02          	add    r9,0x2
   14005fdfa:	41 80 f8 20          	cmp    r8b,0x20
   14005fdfe:	74 06                	je     0x14005fe06
   14005fe00:	4c 8b d2             	mov    r10,rdx
   14005fe03:	49 d1 fa             	sar    r10,1
   14005fe06:	49 ff c3             	inc    r11
   14005fe09:	4c 3b db             	cmp    r11,rbx
   14005fe0c:	7e 9a                	jle    0x14005fda8
   14005fe0e:	48 8b 5c 24 08       	mov    rbx,QWORD PTR [rsp+0x8]
   14005fe13:	48 8b 74 24 18       	mov    rsi,QWORD PTR [rsp+0x18]
   14005fe18:	66 41 89 29          	mov    WORD PTR [r9],bp
   14005fe1c:	49 63 c2             	movsxd rax,r10d
   14005fe1f:	48 8d 04 47          	lea    rax,[rdi+rax*2]
   14005fe23:	48 8b 7c 24 20       	mov    rdi,QWORD PTR [rsp+0x20]
   14005fe28:	66 89 28             	mov    WORD PTR [rax],bp
   14005fe2b:	48 8b 6c 24 10       	mov    rbp,QWORD PTR [rsp+0x10]
   14005fe30:	c3                   	ret
   14005fe31:	cc                   	int3
   14005fe32:	cc                   	int3
   14005fe33:	cc                   	int3
   14005fe34:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   14005fe39:	4c 8b 19             	mov    r11,QWORD PTR [rcx]
   14005fe3c:	4a 8d 44 02 ff       	lea    rax,[rdx+r8*1-0x1]
   14005fe41:	33 db                	xor    ebx,ebx
   14005fe43:	4c 8b ca             	mov    r9,rdx
   14005fe46:	4c 8b d1             	mov    r10,rcx
   14005fe49:	4c 8b c2             	mov    r8,rdx
   14005fe4c:	48 3b d0             	cmp    rdx,rax
   14005fe4f:	77 0d                	ja     0x14005fe5e
   14005fe51:	41 38 18             	cmp    BYTE PTR [r8],bl
   14005fe54:	74 08                	je     0x14005fe5e
   14005fe56:	49 ff c0             	inc    r8
   14005fe59:	4c 3b c0             	cmp    r8,rax
   14005fe5c:	76 f3                	jbe    0x14005fe51
   14005fe5e:	49 ff c8             	dec    r8
   14005fe61:	eb 09                	jmp    0x14005fe6c
   14005fe63:	41 80 38 20          	cmp    BYTE PTR [r8],0x20
   14005fe67:	75 08                	jne    0x14005fe71
   14005fe69:	49 ff c8             	dec    r8
   14005fe6c:	49 3b d0             	cmp    rdx,r8
   14005fe6f:	76 f2                	jbe    0x14005fe63
   14005fe71:	49 3b d0             	cmp    rdx,r8
   14005fe74:	77 26                	ja     0x14005fe9c
   14005fe76:	41 80 39 20          	cmp    BYTE PTR [r9],0x20
   14005fe7a:	75 1b                	jne    0x14005fe97
   14005fe7c:	49 ff c1             	inc    r9
   14005fe7f:	4d 3b c8             	cmp    r9,r8
   14005fe82:	76 f2                	jbe    0x14005fe76
   14005fe84:	eb 11                	jmp    0x14005fe97
   14005fe86:	41 0f be 11          	movsx  edx,BYTE PTR [r9]
   14005fe8a:	49 8b 0a             	mov    rcx,QWORD PTR [r10]
   14005fe8d:	66 89 11             	mov    WORD PTR [rcx],dx
   14005fe90:	49 83 02 02          	add    QWORD PTR [r10],0x2
   14005fe94:	49 ff c1             	inc    r9
   14005fe97:	4d 3b c8             	cmp    r9,r8
   14005fe9a:	76 ea                	jbe    0x14005fe86
   14005fe9c:	49 8b 0a             	mov    rcx,QWORD PTR [r10]
   14005fe9f:	49 8b c3             	mov    rax,r11
   14005fea2:	66 89 19             	mov    WORD PTR [rcx],bx
   14005fea5:	48 8b 5c 24 08       	mov    rbx,QWORD PTR [rsp+0x8]
   14005feaa:	c3                   	ret
   14005feab:	cc                   	int3
   14005feac:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   14005feb1:	48 89 74 24 18       	mov    QWORD PTR [rsp+0x18],rsi
   14005feb6:	57                   	push   rdi
   14005feb7:	48 83 ec 40          	sub    rsp,0x40
   14005febb:	41 83 20 00          	and    DWORD PTR [r8],0x0
   14005febf:	48 8b fa             	mov    rdi,rdx
   14005fec2:	49 8b f0             	mov    rsi,r8
   14005fec5:	48 8b d9             	mov    rbx,rcx
   14005fec8:	33 d2                	xor    edx,edx
   14005feca:	41 b8 c0 00 00 00    	mov    r8d,0xc0
   14005fed0:	48 8b cf             	mov    rcx,rdi
   14005fed3:	e8 b8 2b 0a 00       	call   0x140102a90
   14005fed8:	48 83 64 24 38 00    	and    QWORD PTR [rsp+0x38],0x0
   14005fede:	83 64 24 58 00       	and    DWORD PTR [rsp+0x58],0x0
   14005fee3:	48 8d 44 24 58       	lea    rax,[rsp+0x58]
   14005fee8:	45 33 c9             	xor    r9d,r9d
   14005feeb:	45 33 c0             	xor    r8d,r8d
   14005feee:	48 89 44 24 30       	mov    QWORD PTR [rsp+0x30],rax
   14005fef3:	ba 50 00 07 00       	mov    edx,0x70050
   14005fef8:	48 8b cb             	mov    rcx,rbx
   14005fefb:	c7 44 24 28 00 00 10 	mov    DWORD PTR [rsp+0x28],0x100000
   14005ff02:	00 
   14005ff03:	48 89 7c 24 20       	mov    QWORD PTR [rsp+0x20],rdi
   14005ff08:	ff 15 c2 b9 0b 00    	call   QWORD PTR [rip+0xbb9c2]        # 0x14011b8d0
   14005ff0e:	85 c0                	test   eax,eax
   14005ff10:	75 04                	jne    0x14005ff16
   14005ff12:	33 c0                	xor    eax,eax
   14005ff14:	eb 08                	jmp    0x14005ff1e
   14005ff16:	8b 47 04             	mov    eax,DWORD PTR [rdi+0x4]
   14005ff19:	89 06                	mov    DWORD PTR [rsi],eax
   14005ff1b:	48 8b c7             	mov    rax,rdi
   14005ff1e:	48 8b 5c 24 50       	mov    rbx,QWORD PTR [rsp+0x50]
   14005ff23:	48 8b 74 24 60       	mov    rsi,QWORD PTR [rsp+0x60]
   14005ff28:	48 83 c4 40          	add    rsp,0x40
   14005ff2c:	5f                   	pop    rdi
   14005ff2d:	c3                   	ret
   14005ff2e:	cc                   	int3
   14005ff2f:	cc                   	int3
   14005ff30:	40 53                	rex push rbx
   14005ff32:	48 83 ec 20          	sub    rsp,0x20
   14005ff36:	48 8b da             	mov    rbx,rdx
   14005ff39:	44 8b c9             	mov    r9d,ecx
   14005ff3c:	85 c9                	test   ecx,ecx
   14005ff3e:	0f 84 8e 00 00 00    	je     0x14005ffd2
   14005ff44:	b8 01 00 00 00       	mov    eax,0x1
   14005ff49:	44 2b c8             	sub    r9d,eax
   14005ff4c:	74 7b                	je     0x14005ffc9
   14005ff4e:	44 2b c8             	sub    r9d,eax
   14005ff51:	74 5e                	je     0x14005ffb1
   14005ff53:	44 2b c8             	sub    r9d,eax
   14005ff56:	74 49                	je     0x14005ffa1
   14005ff58:	44 2b c8             	sub    r9d,eax
   14005ff5b:	74 3b                	je     0x14005ff98
   14005ff5d:	44 2b c8             	sub    r9d,eax
   14005ff60:	74 26                	je     0x14005ff88
   14005ff62:	44 3b c8             	cmp    r9d,eax
   14005ff65:	74 18                	je     0x14005ff7f
   14005ff67:	44 8b c1             	mov    r8d,ecx
   14005ff6a:	48 8d 15 f7 d0 0b 00 	lea    rdx,[rip+0xbd0f7]        # 0x14011d068
   14005ff71:	48 8b cb             	mov    rcx,rbx
   14005ff74:	ff 15 be c1 0b 00    	call   QWORD PTR [rip+0xbc1be]        # 0x14011c138
   14005ff7a:	48 8b c3             	mov    rax,rbx
   14005ff7d:	eb 5a                	jmp    0x14005ffd9
   14005ff7f:	48 8d 05 fa 5b 0c 00 	lea    rax,[rip+0xc5bfa]        # 0x140125b80
   14005ff86:	eb 51                	jmp    0x14005ffd9
   14005ff88:	48 8d 0d d1 5b 0c 00 	lea    rcx,[rip+0xc5bd1]        # 0x140125b60
   14005ff8f:	48 8d 05 b2 5b 0c 00 	lea    rax,[rip+0xc5bb2]        # 0x140125b48
   14005ff96:	eb 27                	jmp    0x14005ffbf
   14005ff98:	48 8d 05 89 5b 0c 00 	lea    rax,[rip+0xc5b89]        # 0x140125b28
   14005ff9f:	eb 38                	jmp    0x14005ffd9
   14005ffa1:	48 8d 0d 60 5b 0c 00 	lea    rcx,[rip+0xc5b60]        # 0x140125b08
   14005ffa8:	48 8d 05 41 5b 0c 00 	lea    rax,[rip+0xc5b41]        # 0x140125af0
   14005ffaf:	eb 0e                	jmp    0x14005ffbf
   14005ffb1:	48 8d 0d 18 5b 0c 00 	lea    rcx,[rip+0xc5b18]        # 0x140125ad0
   14005ffb8:	48 8d 05 f1 5a 0c 00 	lea    rax,[rip+0xc5af1]        # 0x140125ab0
   14005ffbf:	41 83 f8 07          	cmp    r8d,0x7
   14005ffc3:	48 0f 44 c1          	cmove  rax,rcx
   14005ffc7:	eb 10                	jmp    0x14005ffd9
   14005ffc9:	48 8d 05 b8 5a 0c 00 	lea    rax,[rip+0xc5ab8]        # 0x140125a88
   14005ffd0:	eb 07                	jmp    0x14005ffd9
   14005ffd2:	48 8d 05 8f 5a 0c 00 	lea    rax,[rip+0xc5a8f]        # 0x140125a68
   14005ffd9:	48 83 c4 20          	add    rsp,0x20
   14005ffdd:	5b                   	pop    rbx
   14005ffde:	c3                   	ret
   14005ffdf:	cc                   	int3
   14005ffe0:	33 c0                	xor    eax,eax
   14005ffe2:	3b d0                	cmp    edx,eax
   14005ffe4:	7e 16                	jle    0x14005fffc
   14005ffe6:	44 8b c2             	mov    r8d,edx
   14005ffe9:	0f b6 11             	movzx  edx,BYTE PTR [rcx]
   14005ffec:	48 c1 e0 08          	shl    rax,0x8
   14005fff0:	48 ff c1             	inc    rcx
   14005fff3:	48 0b c2             	or     rax,rdx
   14005fff6:	49 83 e8 01          	sub    r8,0x1
   14005fffa:	75 ed                	jne    0x14005ffe9
   14005fffc:	f3 c3                	repz ret
   14005fffe:	cc                   	int3
   14005ffff:	cc                   	int3
   140060000:	40 53                	rex push rbx
   140060002:	48 83 ec 40          	sub    rsp,0x40
   140060006:	48 8b 05 93 5b 0c 00 	mov    rax,QWORD PTR [rip+0xc5b93]        # 0x140125ba0
   14006000d:	48 8b da             	mov    rbx,rdx
   140060010:	48 8d 54 24 20       	lea    rdx,[rsp+0x20]
   140060015:	48 89 02             	mov    QWORD PTR [rdx],rax
   140060018:	48 8b 05 89 5b 0c 00 	mov    rax,QWORD PTR [rip+0xc5b89]        # 0x140125ba8
   14006001f:	4c 8b cb             	mov    r9,rbx
   140060022:	48 89 42 08          	mov    QWORD PTR [rdx+0x8],rax
   140060026:	48 8b 05 83 5b 0c 00 	mov    rax,QWORD PTR [rip+0xc5b83]        # 0x140125bb0
   14006002d:	45 33 c0             	xor    r8d,r8d
   140060030:	48 89 42 10          	mov    QWORD PTR [rdx+0x10],rax
   140060034:	8b 05 7e 5b 0c 00    	mov    eax,DWORD PTR [rip+0xc5b7e]        # 0x140125bb8
   14006003a:	89 42 18             	mov    DWORD PTR [rdx+0x18],eax
   14006003d:	0f b7 05 78 5b 0c 00 	movzx  eax,WORD PTR [rip+0xc5b78]        # 0x140125bbc
   140060044:	66 89 42 1c          	mov    WORD PTR [rdx+0x1c],ax
   140060048:	48 8b 03             	mov    rax,QWORD PTR [rbx]
   14006004b:	66 89 4c 24 38       	mov    WORD PTR [rsp+0x38],cx
   140060050:	33 c9                	xor    ecx,ecx
   140060052:	33 d2                	xor    edx,edx
   140060054:	66 89 08             	mov    WORD PTR [rax],cx
   140060057:	48 8d 4c 24 20       	lea    rcx,[rsp+0x20]
   14006005c:	e8 67 d8 fb ff       	call   0x14001d8c8
   140060061:	48 8b 03             	mov    rax,QWORD PTR [rbx]
   140060064:	48 83 c4 40          	add    rsp,0x40
   140060068:	5b                   	pop    rbx
   140060069:	c3                   	ret
   14006006a:	cc                   	int3
   14006006b:	cc                   	int3
   14006006c:	4c 8b c2             	mov    r8,rdx
   14006006f:	48 8b c1             	mov    rax,rcx
   140060072:	48 99                	cqo
   140060074:	49 f7 f8             	idiv   r8
   140060077:	48 85 d2             	test   rdx,rdx
   14006007a:	74 06                	je     0x140060082
   14006007c:	4c 2b c2             	sub    r8,rdx
   14006007f:	49 8b d0             	mov    rdx,r8
   140060082:	48 8b c2             	mov    rax,rdx
   140060085:	c3                   	ret
   140060086:	cc                   	int3
   140060087:	cc                   	int3
   140060088:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   14006008d:	48 89 7c 24 10       	mov    QWORD PTR [rsp+0x10],rdi
   140060092:	8b 7c 24 28          	mov    edi,DWORD PTR [rsp+0x28]
   140060096:	4c 8b da             	mov    r11,rdx
   140060099:	33 d2                	xor    edx,edx
   14006009b:	48 8b c1             	mov    rax,rcx
   14006009e:	49 8b d8             	mov    rbx,r8
   1400600a1:	44 8b 44 24 30       	mov    r8d,DWORD PTR [rsp+0x30]
   1400600a6:	48 f7 f7             	div    rdi
   1400600a9:	fe c2                	inc    dl
   1400600ab:	41 88 11             	mov    BYTE PTR [r9],dl
   1400600ae:	33 d2                	xor    edx,edx
   1400600b0:	49 f7 f0             	div    r8
   1400600b3:	48 8b c8             	mov    rcx,rax
   1400600b6:	b8 ff 03 00 00       	mov    eax,0x3ff
   1400600bb:	88 13                	mov    BYTE PTR [rbx],dl
   1400600bd:	48 3b c8             	cmp    rcx,rax
   1400600c0:	76 1c                	jbe    0x1400600de
   1400600c2:	f7 5c 24 38          	neg    DWORD PTR [rsp+0x38]
   1400600c6:	48 1b c9             	sbb    rcx,rcx
   1400600c9:	48 03 c8             	add    rcx,rax
   1400600cc:	80 3d 2e 6f 0e 00 00 	cmp    BYTE PTR [rip+0xe6f2e],0x0        # 0x140147001
   1400600d3:	74 09                	je     0x1400600de
   1400600d5:	41 fe c8             	dec    r8b
   1400600d8:	41 88 39             	mov    BYTE PTR [r9],dil
   1400600db:	44 88 03             	mov    BYTE PTR [rbx],r8b
   1400600de:	48 8b 5c 24 08       	mov    rbx,QWORD PTR [rsp+0x8]
   1400600e3:	48 8b 7c 24 10       	mov    rdi,QWORD PTR [rsp+0x10]
   1400600e8:	48 8b c1             	mov    rax,rcx
   1400600eb:	48 c1 e8 02          	shr    rax,0x2
   1400600ef:	24 c0                	and    al,0xc0
   1400600f1:	41 08 01             	or     BYTE PTR [r9],al
   1400600f4:	41 88 0b             	mov    BYTE PTR [r11],cl
   1400600f7:	c3                   	ret
   1400600f8:	40 53                	rex push rbx
   1400600fa:	48 83 ec 20          	sub    rsp,0x20
   1400600fe:	48 8b d9             	mov    rbx,rcx
   140060101:	ff 15 b9 b7 0b 00    	call   QWORD PTR [rip+0xbb7b9]        # 0x14011b8c0
   140060107:	48 8b cb             	mov    rcx,rbx
   14006010a:	ff 15 b0 b7 0b 00    	call   QWORD PTR [rip+0xbb7b0]        # 0x14011b8c0
   140060110:	48 8b cb             	mov    rcx,rbx
   140060113:	48 83 c4 20          	add    rsp,0x20
   140060117:	5b                   	pop    rbx
   140060118:	48 ff 25 a1 b7 0b 00 	rex.W jmp QWORD PTR [rip+0xbb7a1]        # 0x14011b8c0
   14006011f:	cc                   	int3
   140060120:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   140060125:	57                   	push   rdi
   140060126:	48 83 ec 40          	sub    rsp,0x40
   14006012a:	48 8b d9             	mov    rbx,rcx
   14006012d:	e8 c6 ff ff ff       	call   0x1400600f8
   140060132:	33 ff                	xor    edi,edi
   140060134:	48 89 7c 24 38       	mov    QWORD PTR [rsp+0x38],rdi
   140060139:	48 8d 44 24 58       	lea    rax,[rsp+0x58]
   14006013e:	45 33 c9             	xor    r9d,r9d
   140060141:	48 89 44 24 30       	mov    QWORD PTR [rsp+0x30],rax
   140060146:	45 33 c0             	xor    r8d,r8d
   140060149:	ba 40 01 07 00       	mov    edx,0x70140
   14006014e:	48 8b cb             	mov    rcx,rbx
   140060151:	89 7c 24 28          	mov    DWORD PTR [rsp+0x28],edi
   140060155:	89 7c 24 58          	mov    DWORD PTR [rsp+0x58],edi
   140060159:	48 89 7c 24 20       	mov    QWORD PTR [rsp+0x20],rdi
   14006015e:	ff 15 6c b7 0b 00    	call   QWORD PTR [rip+0xbb76c]        # 0x14011b8d0
   140060164:	3b c7                	cmp    eax,edi
   140060166:	75 08                	jne    0x140060170
   140060168:	ff 15 52 b8 0b 00    	call   QWORD PTR [rip+0xbb852]        # 0x14011b9c0
   14006016e:	8b f8                	mov    edi,eax
   140060170:	8b c7                	mov    eax,edi
   140060172:	48 8b 5c 24 50       	mov    rbx,QWORD PTR [rsp+0x50]
   140060177:	48 83 c4 40          	add    rsp,0x40
   14006017b:	5f                   	pop    rdi
   14006017c:	c3                   	ret
   14006017d:	cc                   	int3
   14006017e:	cc                   	int3
   14006017f:	cc                   	int3
   140060180:	40 53                	rex push rbx
   140060182:	48 83 ec 20          	sub    rsp,0x20
   140060186:	8b 81 b8 01 00 00    	mov    eax,DWORD PTR [rcx+0x1b8]
   14006018c:	48 8b d9             	mov    rbx,rcx
   14006018f:	85 c0                	test   eax,eax
   140060191:	74 0a                	je     0x14006019d
   140060193:	83 f8 ff             	cmp    eax,0xffffffff
   140060196:	74 05                	je     0x14006019d
   140060198:	83 f8 01             	cmp    eax,0x1
   14006019b:	75 13                	jne    0x1400601b0
   14006019d:	e8 aa de ff ff       	call   0x14005e04c
   1400601a2:	25 4d ff ff ff       	and    eax,0xffffff4d
   1400601a7:	83 c8 4d             	or     eax,0x4d
   1400601aa:	89 83 b8 01 00 00    	mov    DWORD PTR [rbx+0x1b8],eax
   1400601b0:	48 83 c4 20          	add    rsp,0x20
   1400601b4:	5b                   	pop    rbx
   1400601b5:	c3                   	ret
   1400601b6:	cc                   	int3
   1400601b7:	cc                   	int3
   1400601b8:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   1400601bd:	57                   	push   rdi
   1400601be:	48 83 ec 30          	sub    rsp,0x30
   1400601c2:	f6 01 11             	test   BYTE PTR [rcx],0x11
   1400601c5:	48 8b 5c 24 68       	mov    rbx,QWORD PTR [rsp+0x68]
   1400601ca:	49 8b c1             	mov    rax,r9
   1400601cd:	4d 8b d0             	mov    r10,r8
   1400601d0:	44 8b da             	mov    r11d,edx
   1400601d3:	48 8b f9             	mov    rdi,rcx
   1400601d6:	74 05                	je     0x1400601dd
   1400601d8:	48 85 db             	test   rbx,rbx
   1400601db:	74 22                	je     0x1400601ff
   1400601dd:	4c 8b 4c 24 60       	mov    r9,QWORD PTR [rsp+0x60]
   1400601e2:	4c 8b c0             	mov    r8,rax
   1400601e5:	49 8b d2             	mov    rdx,r10
   1400601e8:	41 8b cb             	mov    ecx,r11d
   1400601eb:	48 89 5c 24 20       	mov    QWORD PTR [rsp+0x20],rbx
   1400601f0:	e8 7b 74 fa ff       	call   0x140007670
   1400601f5:	48 85 db             	test   rbx,rbx
   1400601f8:	0f 95 c0             	setne  al
   1400601fb:	fe c0                	inc    al
   1400601fd:	08 07                	or     BYTE PTR [rdi],al
   1400601ff:	48 8b 5c 24 40       	mov    rbx,QWORD PTR [rsp+0x40]
   140060204:	48 83 c4 30          	add    rsp,0x30
   140060208:	5f                   	pop    rdi
   140060209:	c3                   	ret
   14006020a:	cc                   	int3
   14006020b:	cc                   	int3
   14006020c:	40 53                	rex push rbx
   14006020e:	48 83 ec 30          	sub    rsp,0x30
   140060212:	f6 01 0f             	test   BYTE PTR [rcx],0xf
   140060215:	48 8b d9             	mov    rbx,rcx
   140060218:	74 24                	je     0x14006023e
   14006021a:	48 83 64 24 20 00    	and    QWORD PTR [rsp+0x20],0x0
   140060220:	48 83 c8 ff          	or     rax,0xffffffffffffffff
   140060224:	45 33 c9             	xor    r9d,r9d
   140060227:	4c 8b c0             	mov    r8,rax
   14006022a:	48 8b d0             	mov    rdx,rax
   14006022d:	8b c8                	mov    ecx,eax
   14006022f:	e8 3c 74 fa ff       	call   0x140007670
   140060234:	c6 03 00             	mov    BYTE PTR [rbx],0x0
   140060237:	c6 05 b7 d4 0d 00 00 	mov    BYTE PTR [rip+0xdd4b7],0x0        # 0x14013d6f5
   14006023e:	48 83 c4 30          	add    rsp,0x30
   140060242:	5b                   	pop    rbx
   140060243:	c3                   	ret
   140060244:	44 8d 41 bf          	lea    r8d,[rcx-0x41]
   140060248:	41 b9 1a 00 00 00    	mov    r9d,0x1a
   14006024e:	66 45 3b c1          	cmp    r8w,r9w
   140060252:	7d 37                	jge    0x14006028b
   140060254:	49 0f bf c0          	movsx  rax,r8w
   140060258:	44 0f b7 d1          	movzx  r10d,cx
   14006025c:	48 69 c0 20 02 00 00 	imul   rax,rax,0x220
   140060263:	66 45 2b c8          	sub    r9w,r8w
   140060267:	48 8d 4c 10 10       	lea    rcx,[rax+rdx*1+0x10]
   14006026c:	41 0f b7 d1          	movzx  edx,r9w
   140060270:	8b 01                	mov    eax,DWORD PTR [rcx]
   140060272:	44 3b d0             	cmp    r10d,eax
   140060275:	75 07                	jne    0x14006027e
   140060277:	0d 00 90 00 00       	or     eax,0x9000
   14006027c:	89 01                	mov    DWORD PTR [rcx],eax
   14006027e:	48 81 c1 20 02 00 00 	add    rcx,0x220
   140060285:	48 83 ea 01          	sub    rdx,0x1
   140060289:	75 e5                	jne    0x140060270
   14006028b:	f3 c3                	repz ret
   14006028d:	cc                   	int3
   14006028e:	cc                   	int3
   14006028f:	cc                   	int3
   140060290:	48 83 ec 28          	sub    rsp,0x28
   140060294:	45 33 db             	xor    r11d,r11d
   140060297:	44 0f b7 d1          	movzx  r10d,cx
   14006029b:	45 0f b7 c3          	movzx  r8d,r11w
   14006029f:	44 38 1a             	cmp    BYTE PTR [rdx],r11b
   1400602a2:	74 44                	je     0x1400602e8
   1400602a4:	45 8d 4b 1a          	lea    r9d,[r11+0x1a]
   1400602a8:	66 45 3b c1          	cmp    r8w,r9w
   1400602ac:	7d 3a                	jge    0x1400602e8
   1400602ae:	49 0f bf c0          	movsx  rax,r8w
   1400602b2:	0f be 0c 10          	movsx  ecx,BYTE PTR [rax+rdx*1]
   1400602b6:	41 0f b7 c2          	movzx  eax,r10w
   1400602ba:	3b c1                	cmp    eax,ecx
   1400602bc:	74 10                	je     0x1400602ce
   1400602be:	66 41 ff c0          	inc    r8w
   1400602c2:	49 0f bf c0          	movsx  rax,r8w
   1400602c6:	44 38 1c 10          	cmp    BYTE PTR [rax+rdx*1],r11b
   1400602ca:	75 dc                	jne    0x1400602a8
   1400602cc:	eb 1a                	jmp    0x1400602e8
   1400602ce:	49 0f bf c8          	movsx  rcx,r8w
   1400602d2:	41 0f bf c0          	movsx  eax,r8w
   1400602d6:	48 03 ca             	add    rcx,rdx
   1400602d9:	44 2b c8             	sub    r9d,eax
   1400602dc:	48 8d 51 01          	lea    rdx,[rcx+0x1]
   1400602e0:	4d 63 c1             	movsxd r8,r9d
   1400602e3:	e8 5c d4 fb ff       	call   0x14001d744
   1400602e8:	48 83 c4 28          	add    rsp,0x28
   1400602ec:	c3                   	ret
   1400602ed:	cc                   	int3
   1400602ee:	cc                   	int3
   1400602ef:	cc                   	int3
   1400602f0:	48 8b c4             	mov    rax,rsp
   1400602f3:	48 89 58 08          	mov    QWORD PTR [rax+0x8],rbx
   1400602f7:	48 89 68 10          	mov    QWORD PTR [rax+0x10],rbp
   1400602fb:	48 89 70 18          	mov    QWORD PTR [rax+0x18],rsi
   1400602ff:	48 89 78 20          	mov    QWORD PTR [rax+0x20],rdi
   140060303:	41 54                	push   r12
   140060305:	41 55                	push   r13
   140060307:	4c 8b 5c 24 38       	mov    r11,QWORD PTR [rsp+0x38]
   14006030c:	45 33 e4             	xor    r12d,r12d
   14006030f:	41 0f b7 f1          	movzx  esi,r9w
   140060313:	49 8b e8             	mov    rbp,r8
   140060316:	4c 8b ca             	mov    r9,rdx
   140060319:	45 8d 6c 24 01       	lea    r13d,[r12+0x1]
   14006031e:	66 45 8b d4          	mov    r10w,r12w
   140060322:	66 83 f9 5a          	cmp    cx,0x5a
   140060326:	0f 87 9f 00 00 00    	ja     0x1400603cb
   14006032c:	49 8b 1b             	mov    rbx,QWORD PTR [r11]
   14006032f:	44 38 23             	cmp    BYTE PTR [rbx],r12b
   140060332:	0f 84 93 00 00 00    	je     0x1400603cb
   140060338:	66 44 3b e6          	cmp    r12w,si
   14006033c:	41 0f b7 d4          	movzx  edx,r12w
   140060340:	7d 4e                	jge    0x140060390
   140060342:	4d 0f bf c2          	movsx  r8,r10w
   140060346:	4d 69 c0 20 02 00 00 	imul   r8,r8,0x220
   14006034d:	48 0f bf fa          	movsx  rdi,dx
   140060351:	0f bf 44 bd 00       	movsx  eax,WORD PTR [rbp+rdi*4+0x0]
   140060356:	43 39 44 08 04       	cmp    DWORD PTR [r8+r9*1+0x4],eax
   14006035b:	75 0b                	jne    0x140060368
   14006035d:	0f bf 44 bd 02       	movsx  eax,WORD PTR [rbp+rdi*4+0x2]
   140060362:	43 39 04 08          	cmp    DWORD PTR [r8+r9*1],eax
   140060366:	74 0b                	je     0x140060373
   140060368:	66 41 03 d5          	add    dx,r13w
   14006036c:	66 3b d6             	cmp    dx,si
   14006036f:	7d 1f                	jge    0x140060390
   140060371:	eb da                	jmp    0x14006034d
   140060373:	43 0f b6 44 08 10    	movzx  eax,BYTE PTR [r8+r9*1+0x10]
   140060379:	66 3b c8             	cmp    cx,ax
   14006037c:	75 0d                	jne    0x14006038b
   14006037e:	48 8d 43 01          	lea    rax,[rbx+0x1]
   140060382:	45 8b d4             	mov    r10d,r12d
   140060385:	49 89 03             	mov    QWORD PTR [r11],rax
   140060388:	0f be 08             	movsx  ecx,BYTE PTR [rax]
   14006038b:	66 3b d6             	cmp    dx,si
   14006038e:	7c 2c                	jl     0x1400603bc
   140060390:	49 0f bf d2          	movsx  rdx,r10w
   140060394:	48 69 d2 20 02 00 00 	imul   rdx,rdx,0x220
   14006039b:	42 0f b6 44 0a 10    	movzx  eax,BYTE PTR [rdx+r9*1+0x10]
   1400603a1:	66 3b c8             	cmp    cx,ax
   1400603a4:	75 16                	jne    0x1400603bc
   1400603a6:	42 0f ba 64 0a 10 0e 	bt     DWORD PTR [rdx+r9*1+0x10],0xe
   1400603ad:	73 0d                	jae    0x1400603bc
   1400603af:	4d 01 2b             	add    QWORD PTR [r11],r13
   1400603b2:	49 8b 03             	mov    rax,QWORD PTR [r11]
   1400603b5:	66 45 8b d4          	mov    r10w,r12w
   1400603b9:	0f be 08             	movsx  ecx,BYTE PTR [rax]
   1400603bc:	66 45 03 d5          	add    r10w,r13w
   1400603c0:	66 41 83 fa 1a       	cmp    r10w,0x1a
   1400603c5:	0f 8c 57 ff ff ff    	jl     0x140060322
   1400603cb:	48 8b 5c 24 18       	mov    rbx,QWORD PTR [rsp+0x18]
   1400603d0:	48 8b 6c 24 20       	mov    rbp,QWORD PTR [rsp+0x20]
   1400603d5:	48 8b 74 24 28       	mov    rsi,QWORD PTR [rsp+0x28]
   1400603da:	48 8b 7c 24 30       	mov    rdi,QWORD PTR [rsp+0x30]
   1400603df:	66 8b c1             	mov    ax,cx
   1400603e2:	41 5d                	pop    r13
   1400603e4:	41 5c                	pop    r12
   1400603e6:	c3                   	ret
   1400603e7:	cc                   	int3
   1400603e8:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   1400603ed:	57                   	push   rdi
   1400603ee:	48 83 ec 20          	sub    rsp,0x20
   1400603f2:	48 8b fa             	mov    rdi,rdx
   1400603f5:	33 d2                	xor    edx,edx
   1400603f7:	0f b7 d9             	movzx  ebx,cx
   1400603fa:	44 8d 42 1b          	lea    r8d,[rdx+0x1b]
   1400603fe:	48 8b cf             	mov    rcx,rdi
   140060401:	e8 8a 26 0a 00       	call   0x140102a90
   140060406:	66 83 fb 5a          	cmp    bx,0x5a
   14006040a:	77 13                	ja     0x14006041f
   14006040c:	b8 01 00 00 00       	mov    eax,0x1
   140060411:	88 1f                	mov    BYTE PTR [rdi],bl
   140060413:	66 03 d8             	add    bx,ax
   140060416:	48 03 f8             	add    rdi,rax
   140060419:	66 83 fb 5a          	cmp    bx,0x5a
   14006041d:	76 f2                	jbe    0x140060411
   14006041f:	48 8b 5c 24 30       	mov    rbx,QWORD PTR [rsp+0x30]
   140060424:	48 83 c4 20          	add    rsp,0x20
   140060428:	5f                   	pop    rdi
   140060429:	c3                   	ret
   14006042a:	cc                   	int3
   14006042b:	cc                   	int3
   14006042c:	48 81 ec 38 02 00 00 	sub    rsp,0x238
   140060433:	33 c0                	xor    eax,eax
   140060435:	48 8d 54 24 20       	lea    rdx,[rsp+0x20]
   14006043a:	45 33 c9             	xor    r9d,r9d
   14006043d:	44 8d 40 24          	lea    r8d,[rax+0x24]
   140060441:	33 c9                	xor    ecx,ecx
   140060443:	66 89 44 24 20       	mov    WORD PTR [rsp+0x20],ax
   140060448:	ff 15 6a b7 0b 00    	call   QWORD PTR [rip+0xbb76a]        # 0x14011bbb8
   14006044e:	66 8b 44 24 20       	mov    ax,WORD PTR [rsp+0x20]
   140060453:	48 81 c4 38 02 00 00 	add    rsp,0x238
   14006045a:	c3                   	ret
   14006045b:	cc                   	int3
   14006045c:	45 33 c9             	xor    r9d,r9d
   14006045f:	49 8b d1             	mov    rdx,r9
   140060462:	66 44 39 09          	cmp    WORD PTR [rcx],r9w
   140060466:	74 41                	je     0x1400604a9
   140060468:	66 83 39 3a          	cmp    WORD PTR [rcx],0x3a
   14006046c:	74 32                	je     0x1400604a0
   14006046e:	48 8d 41 02          	lea    rax,[rcx+0x2]
   140060472:	66 83 38 3a          	cmp    WORD PTR [rax],0x3a
   140060476:	75 28                	jne    0x1400604a0
   140060478:	48 8b d0             	mov    rdx,rax
   14006047b:	48 8d 48 02          	lea    rcx,[rax+0x2]
   14006047f:	eb 1f                	jmp    0x1400604a0
   140060481:	41 b8 5c 00 00 00    	mov    r8d,0x5c
   140060487:	66 44 3b c0          	cmp    r8w,ax
   14006048b:	74 0c                	je     0x140060499
   14006048d:	41                   	rex.B
   14006048e:	b8                   	.byte 0xb8
   14006048f:	2f                   	(bad)
