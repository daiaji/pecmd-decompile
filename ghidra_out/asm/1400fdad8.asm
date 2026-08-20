
PECMD.exe：     文件格式 pei-x86-64


Disassembly of section .text:

00000001400fdad8 <.text+0xfcad8>:
   1400fdad8:	48 8b c4             	mov    rax,rsp
   1400fdadb:	48 89 58 08          	mov    QWORD PTR [rax+0x8],rbx
   1400fdadf:	48 89 68 18          	mov    QWORD PTR [rax+0x18],rbp
   1400fdae3:	48 89 70 20          	mov    QWORD PTR [rax+0x20],rsi
   1400fdae7:	57                   	push   rdi
   1400fdae8:	41 54                	push   r12
   1400fdaea:	41 55                	push   r13
   1400fdaec:	41 56                	push   r14
   1400fdaee:	41 57                	push   r15
   1400fdaf0:	48 83 ec 60          	sub    rsp,0x60
   1400fdaf4:	4d 8b f1             	mov    r14,r9
   1400fdaf7:	49 8b e8             	mov    rbp,r8
   1400fdafa:	44 8b ea             	mov    r13d,edx
   1400fdafd:	48 8b f9             	mov    rdi,rcx
   1400fdb00:	81 fa 4f 04 00 00    	cmp    edx,0x44f
   1400fdb06:	0f 85 a2 01 00 00    	jne    0x1400fdcae
   1400fdb0c:	48 8d 48 b8          	lea    rcx,[rax-0x48]
   1400fdb10:	ff 15 82 e5 01 00    	call   QWORD PTR [rip+0x1e582]        # 0x14011c098
   1400fdb16:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdb1a:	33 f6                	xor    esi,esi
   1400fdb1c:	48 8d 54 24 48       	lea    rdx,[rsp+0x48]
   1400fdb21:	89 74 24 48          	mov    DWORD PTR [rsp+0x48],esi
   1400fdb25:	89 74 24 4c          	mov    DWORD PTR [rsp+0x4c],esi
   1400fdb29:	89 74 24 50          	mov    DWORD PTR [rsp+0x50],esi
   1400fdb2d:	89 74 24 54          	mov    DWORD PTR [rsp+0x54],esi
   1400fdb31:	ff 15 c9 e6 01 00    	call   QWORD PTR [rip+0x1e6c9]        # 0x14011c200
   1400fdb37:	8d 5e 01             	lea    ebx,[rsi+0x1]
   1400fdb3a:	4c 3b f6             	cmp    r14,rsi
   1400fdb3d:	74 06                	je     0x1400fdb45
   1400fdb3f:	41 0f b6 ee          	movzx  ebp,r14b
   1400fdb43:	eb 11                	jmp    0x1400fdb56
   1400fdb45:	8b cb                	mov    ecx,ebx
   1400fdb47:	ff 15 cb e4 01 00    	call   QWORD PTR [rip+0x1e4cb]        # 0x14011c018
   1400fdb4d:	66 8b e8             	mov    bp,ax
   1400fdb50:	81 e5 00 80 00 00    	and    ebp,0x8000
   1400fdb56:	48 8b 54 24 40       	mov    rdx,QWORD PTR [rsp+0x40]
   1400fdb5b:	48 8d 4c 24 48       	lea    rcx,[rsp+0x48]
   1400fdb60:	ff 15 82 e2 01 00    	call   QWORD PTR [rip+0x1e282]        # 0x14011bde8
   1400fdb66:	3b c6                	cmp    eax,esi
   1400fdb68:	75 51                	jne    0x1400fdbbb
   1400fdb6a:	8a 87 ad 00 00 00    	mov    al,BYTE PTR [rdi+0xad]
   1400fdb70:	40 3a c6             	cmp    al,sil
   1400fdb73:	7e 1b                	jle    0x1400fdb90
   1400fdb75:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdb79:	44 8b c3             	mov    r8d,ebx
   1400fdb7c:	33 d2                	xor    edx,edx
   1400fdb7e:	40 88 b7 ad 00 00 00 	mov    BYTE PTR [rdi+0xad],sil
   1400fdb85:	ff 15 c5 e3 01 00    	call   QWORD PTR [rip+0x1e3c5]        # 0x14011bf50
   1400fdb8b:	e9 17 01 00 00       	jmp    0x1400fdca7
   1400fdb90:	2a c3                	sub    al,bl
   1400fdb92:	3c 9c                	cmp    al,0x9c
   1400fdb94:	88 87 ad 00 00 00    	mov    BYTE PTR [rdi+0xad],al
   1400fdb9a:	0f 8d 07 01 00 00    	jge    0x1400fdca7
   1400fdba0:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdba4:	ba 13 27 00 00       	mov    edx,0x2713
   1400fdba9:	40 88 b7 ad 00 00 00 	mov    BYTE PTR [rdi+0xad],sil
   1400fdbb0:	ff 15 32 e4 01 00    	call   QWORD PTR [rip+0x1e432]        # 0x14011bfe8
   1400fdbb6:	e9 ec 00 00 00       	jmp    0x1400fdca7
   1400fdbbb:	48 3b ee             	cmp    rbp,rsi
   1400fdbbe:	74 3f                	je     0x1400fdbff
   1400fdbc0:	40 38 b7 ad 00 00 00 	cmp    BYTE PTR [rdi+0xad],sil
   1400fdbc7:	0f 8f da 00 00 00    	jg     0x1400fdca7
   1400fdbcd:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdbd1:	44 8b c3             	mov    r8d,ebx
   1400fdbd4:	33 d2                	xor    edx,edx
   1400fdbd6:	88 9f ad 00 00 00    	mov    BYTE PTR [rdi+0xad],bl
   1400fdbdc:	ff 15 6e e3 01 00    	call   QWORD PTR [rip+0x1e36e]        # 0x14011bf50
   1400fdbe2:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdbe6:	45 33 c9             	xor    r9d,r9d
   1400fdbe9:	ba 13 27 00 00       	mov    edx,0x2713
   1400fdbee:	45 8d 41 0a          	lea    r8d,[r9+0xa]
   1400fdbf2:	ff 15 e8 e3 01 00    	call   QWORD PTR [rip+0x1e3e8]        # 0x14011bfe0
   1400fdbf8:	bd 01 02 00 00       	mov    ebp,0x201
   1400fdbfd:	eb 7c                	jmp    0x1400fdc7b
   1400fdbff:	40 38 b7 ad 00 00 00 	cmp    BYTE PTR [rdi+0xad],sil
   1400fdc06:	0f 8e 9b 00 00 00    	jle    0x1400fdca7
   1400fdc0c:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdc10:	44 8b c3             	mov    r8d,ebx
   1400fdc13:	33 d2                	xor    edx,edx
   1400fdc15:	40 88 b7 ad 00 00 00 	mov    BYTE PTR [rdi+0xad],sil
   1400fdc1c:	ff 15 2e e3 01 00    	call   QWORD PTR [rip+0x1e32e]        # 0x14011bf50
   1400fdc22:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdc26:	ba 13 27 00 00       	mov    edx,0x2713
   1400fdc2b:	ff 15 b7 e3 01 00    	call   QWORD PTR [rip+0x1e3b7]        # 0x14011bfe8
   1400fdc31:	f6 87 d1 00 00 00 40 	test   BYTE PTR [rdi+0xd1],0x40
   1400fdc38:	75 6d                	jne    0x1400fdca7
   1400fdc3a:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdc3e:	ff 15 6c e5 01 00    	call   QWORD PTR [rip+0x1e56c]        # 0x14011c1b0
   1400fdc44:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdc48:	bd 02 02 00 00       	mov    ebp,0x202
   1400fdc4d:	ff 15 5d e5 01 00    	call   QWORD PTR [rip+0x1e55d]        # 0x14011c1b0
   1400fdc53:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdc57:	0f b7 d8             	movzx  ebx,ax
   1400fdc5a:	48 81 cb 00 00 02 02 	or     rbx,0x2020000
   1400fdc61:	ff 15 51 e5 01 00    	call   QWORD PTR [rip+0x1e551]        # 0x14011c1b8
   1400fdc67:	4c 8b cd             	mov    r9,rbp
   1400fdc6a:	4c 8b c3             	mov    r8,rbx
   1400fdc6d:	ba 41 04 00 00       	mov    edx,0x441
   1400fdc72:	48 8b c8             	mov    rcx,rax
   1400fdc75:	ff 15 55 e3 01 00    	call   QWORD PTR [rip+0x1e355]        # 0x14011bfd0
   1400fdc7b:	e8 84 05 f6 ff       	call   0x14005e204
   1400fdc80:	48 8b 47 20          	mov    rax,QWORD PTR [rdi+0x20]
   1400fdc84:	48 8b 8f d8 00 00 00 	mov    rcx,QWORD PTR [rdi+0xd8]
   1400fdc8b:	48 89 74 24 30       	mov    QWORD PTR [rsp+0x30],rsi
   1400fdc90:	45 33 c9             	xor    r9d,r9d
   1400fdc93:	45 33 c0             	xor    r8d,r8d
   1400fdc96:	8b d5                	mov    edx,ebp
   1400fdc98:	c6 44 24 28 80       	mov    BYTE PTR [rsp+0x28],0x80
   1400fdc9d:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1400fdca2:	e8 85 3a ff ff       	call   0x1400f172c
   1400fdca7:	33 c0                	xor    eax,eax
   1400fdca9:	e9 0d 02 00 00       	jmp    0x1400fdebb
   1400fdcae:	81 fa 4e 04 00 00    	cmp    edx,0x44e
   1400fdcb4:	75 13                	jne    0x1400fdcc9
   1400fdcb6:	48 8b 01             	mov    rax,QWORD PTR [rcx]
   1400fdcb9:	49 8b d0             	mov    rdx,r8
   1400fdcbc:	ff 90 e0 00 00 00    	call   QWORD PTR [rax+0xe0]
   1400fdcc2:	48 98                	cdqe
   1400fdcc4:	e9 f2 01 00 00       	jmp    0x1400fdebb
   1400fdcc9:	81 fa 62 04 00 00    	cmp    edx,0x462
   1400fdccf:	0f 85 c2 00 00 00    	jne    0x1400fdd97
   1400fdcd5:	4c 3b c9             	cmp    r9,rcx
   1400fdcd8:	75 cd                	jne    0x1400fdca7
   1400fdcda:	b8 3f 00 00 00       	mov    eax,0x3f
   1400fdcdf:	4c 89 44 24 40       	mov    QWORD PTR [rsp+0x40],r8
   1400fdce4:	66 41 3b 00          	cmp    ax,WORD PTR [r8]
   1400fdce8:	75 1d                	jne    0x1400fdd07
   1400fdcea:	8a 81 d1 00 00 00    	mov    al,BYTE PTR [rcx+0xd1]
   1400fdcf0:	bb 01 00 00 00       	mov    ebx,0x1
   1400fdcf5:	c0 e8 06             	shr    al,0x6
   1400fdcf8:	66 f7 d0             	not    ax
   1400fdcfb:	66 23 c3             	and    ax,bx
   1400fdcfe:	66 41 89 00          	mov    WORD PTR [r8],ax
   1400fdd02:	e9 b1 01 00 00       	jmp    0x1400fdeb8
   1400fdd07:	33 f6                	xor    esi,esi
   1400fdd09:	89 b4 24 98 00 00 00 	mov    DWORD PTR [rsp+0x98],esi
   1400fdd10:	66 41 83 38 23       	cmp    WORD PTR [r8],0x23
   1400fdd15:	75 09                	jne    0x1400fdd20
   1400fdd17:	49 8d 40 02          	lea    rax,[r8+0x2]
   1400fdd1b:	48 89 44 24 40       	mov    QWORD PTR [rsp+0x40],rax
   1400fdd20:	48 8d 94 24 98 00 00 	lea    rdx,[rsp+0x98]
   1400fdd27:	00 
   1400fdd28:	48 8d 4c 24 40       	lea    rcx,[rsp+0x40]
   1400fdd2d:	e8 ee 9f f6 ff       	call   0x140067d20
   1400fdd32:	44 8b 9c 24 98 00 00 	mov    r11d,DWORD PTR [rsp+0x98]
   1400fdd39:	00 
   1400fdd3a:	44 3b de             	cmp    r11d,esi
   1400fdd3d:	7e 09                	jle    0x1400fdd48
   1400fdd3f:	80 a7 d1 00 00 00 bf 	and    BYTE PTR [rdi+0xd1],0xbf
   1400fdd46:	eb 25                	jmp    0x1400fdd6d
   1400fdd48:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdd4c:	ba 13 27 00 00       	mov    edx,0x2713
   1400fdd51:	ff 15 91 e2 01 00    	call   QWORD PTR [rip+0x1e291]        # 0x14011bfe8
   1400fdd57:	80 8f d1 00 00 00 40 	or     BYTE PTR [rdi+0xd1],0x40
   1400fdd5e:	44 8b 9c 24 98 00 00 	mov    r11d,DWORD PTR [rsp+0x98]
   1400fdd65:	00 
   1400fdd66:	40 88 b7 ad 00 00 00 	mov    BYTE PTR [rdi+0xad],sil
   1400fdd6d:	44 3b de             	cmp    r11d,esi
   1400fdd70:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdd74:	bb 01 00 00 00       	mov    ebx,0x1
   1400fdd79:	7f 10                	jg     0x1400fdd8b
   1400fdd7b:	44 8b c3             	mov    r8d,ebx
   1400fdd7e:	33 d2                	xor    edx,edx
   1400fdd80:	ff 15 ca e1 01 00    	call   QWORD PTR [rip+0x1e1ca]        # 0x14011bf50
   1400fdd86:	e9 2d 01 00 00       	jmp    0x1400fdeb8
   1400fdd8b:	8b d3                	mov    edx,ebx
   1400fdd8d:	e8 a6 f7 ff ff       	call   0x1400fd538
   1400fdd92:	e9 21 01 00 00       	jmp    0x1400fdeb8
   1400fdd97:	81 fa 5a 04 00 00    	cmp    edx,0x45a
   1400fdd9d:	75 30                	jne    0x1400fddcf
   1400fdd9f:	8a 81 a4 00 00 00    	mov    al,BYTE PTR [rcx+0xa4]
   1400fdda5:	bb 01 00 00 00       	mov    ebx,0x1
   1400fddaa:	a8 10                	test   al,0x10
   1400fddac:	75 08                	jne    0x1400fddb6
   1400fddae:	a8 03                	test   al,0x3
   1400fddb0:	74 04                	je     0x1400fddb6
   1400fddb2:	8b f3                	mov    esi,ebx
   1400fddb4:	eb 02                	jmp    0x1400fddb8
   1400fddb6:	33 f6                	xor    esi,esi
   1400fddb8:	48 8b 49 20          	mov    rcx,QWORD PTR [rcx+0x20]
   1400fddbc:	44 8b ce             	mov    r9d,esi
   1400fddbf:	4d 8b c6             	mov    r8,r14
   1400fddc2:	48 8b d5             	mov    rdx,rbp
   1400fddc5:	e8 92 ab fa ff       	call   0x1400a895c
   1400fddca:	e9 e9 00 00 00       	jmp    0x1400fdeb8
   1400fddcf:	48 8b 81 d8 00 00 00 	mov    rax,QWORD PTR [rcx+0xd8]
   1400fddd6:	33 f6                	xor    esi,esi
   1400fddd8:	44 8b 78 10          	mov    r15d,DWORD PTR [rax+0x10]
   1400fdddc:	44 3b fe             	cmp    r15d,esi
   1400fdddf:	7e 0e                	jle    0x1400fddef
   1400fdde1:	48 8b 49 20          	mov    rcx,QWORD PTR [rcx+0x20]
   1400fdde5:	e8 9e 79 fe ff       	call   0x1400e5788
   1400fddea:	4c 8b e0             	mov    r12,rax
   1400fdded:	eb 03                	jmp    0x1400fddf2
   1400fddef:	4c 8b e6             	mov    r12,rsi
   1400fddf2:	4c 3b e6             	cmp    r12,rsi
   1400fddf5:	74 1a                	je     0x1400fde11
   1400fddf7:	ba 50 04 00 00       	mov    edx,0x450
   1400fddfc:	41 b9 aa 55 a5 5a    	mov    r9d,0x5aa555aa
   1400fde02:	41 b8 04 00 00 00    	mov    r8d,0x4
   1400fde08:	49 8b cc             	mov    rcx,r12
   1400fde0b:	ff 15 c7 e1 01 00    	call   QWORD PTR [rip+0x1e1c7]        # 0x14011bfd8
   1400fde11:	44 3b fe             	cmp    r15d,esi
   1400fde14:	7e 41                	jle    0x1400fde57
   1400fde16:	48 8b 8f d8 00 00 00 	mov    rcx,QWORD PTR [rdi+0xd8]
   1400fde1d:	48 8d 44 24 40       	lea    rax,[rsp+0x40]
   1400fde22:	bb 01 00 00 00       	mov    ebx,0x1
   1400fde27:	48 89 44 24 30       	mov    QWORD PTR [rsp+0x30],rax
   1400fde2c:	48 8b 47 20          	mov    rax,QWORD PTR [rdi+0x20]
   1400fde30:	4d 8b ce             	mov    r9,r14
   1400fde33:	4c 8b c5             	mov    r8,rbp
   1400fde36:	41 8b d5             	mov    edx,r13d
   1400fde39:	88 5c 24 28          	mov    BYTE PTR [rsp+0x28],bl
   1400fde3d:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1400fde42:	48 89 74 24 40       	mov    QWORD PTR [rsp+0x40],rsi
   1400fde47:	e8 e0 38 ff ff       	call   0x1400f172c
   1400fde4c:	a8 04                	test   al,0x4
   1400fde4e:	74 07                	je     0x1400fde57
   1400fde50:	48 8b 5c 24 40       	mov    rbx,QWORD PTR [rsp+0x40]
   1400fde55:	eb 41                	jmp    0x1400fde98
   1400fde57:	4d 8b ce             	mov    r9,r14
   1400fde5a:	4c 8b c5             	mov    r8,rbp
   1400fde5d:	41 8b d5             	mov    edx,r13d
   1400fde60:	48 8b cf             	mov    rcx,rdi
   1400fde63:	e8 a4 7c fe ff       	call   0x1400e5b0c
   1400fde68:	44 3b fe             	cmp    r15d,esi
   1400fde6b:	48 8b d8             	mov    rbx,rax
   1400fde6e:	7e 28                	jle    0x1400fde98
   1400fde70:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fde74:	48 89 74 24 30       	mov    QWORD PTR [rsp+0x30],rsi
   1400fde79:	c6 44 24 28 02       	mov    BYTE PTR [rsp+0x28],0x2
   1400fde7e:	48 89 4c 24 20       	mov    QWORD PTR [rsp+0x20],rcx
   1400fde83:	48 8b 8f d8 00 00 00 	mov    rcx,QWORD PTR [rdi+0xd8]
   1400fde8a:	4d 8b ce             	mov    r9,r14
   1400fde8d:	4c 8b c5             	mov    r8,rbp
   1400fde90:	41 8b d5             	mov    edx,r13d
   1400fde93:	e8 94 38 ff ff       	call   0x1400f172c
   1400fde98:	4c 3b e6             	cmp    r12,rsi
   1400fde9b:	74 1b                	je     0x1400fdeb8
   1400fde9d:	49 c7 c1 a5 5a 55 aa 	mov    r9,0xffffffffaa555aa5
   1400fdea4:	41 b8 04 02 00 00    	mov    r8d,0x204
   1400fdeaa:	ba 51 04 00 00       	mov    edx,0x451
   1400fdeaf:	49 8b cc             	mov    rcx,r12
   1400fdeb2:	ff 15 18 e1 01 00    	call   QWORD PTR [rip+0x1e118]        # 0x14011bfd0
   1400fdeb8:	48 8b c3             	mov    rax,rbx
   1400fdebb:	4c 8d 5c 24 60       	lea    r11,[rsp+0x60]
   1400fdec0:	49 8b 5b 30          	mov    rbx,QWORD PTR [r11+0x30]
   1400fdec4:	49 8b 6b 40          	mov    rbp,QWORD PTR [r11+0x40]
   1400fdec8:	49 8b 73 48          	mov    rsi,QWORD PTR [r11+0x48]
   1400fdecc:	49 8b e3             	mov    rsp,r11
   1400fdecf:	41 5f                	pop    r15
   1400fded1:	41 5e                	pop    r14
   1400fded3:	41 5d                	pop    r13
   1400fded5:	41 5c                	pop    r12
   1400fded7:	5f                   	pop    rdi
   1400fded8:	c3                   	ret
   1400fded9:	cc                   	int3
   1400fdeda:	cc                   	int3
   1400fdedb:	cc                   	int3
   1400fdedc:	48 89 5c 24 10       	mov    QWORD PTR [rsp+0x10],rbx
   1400fdee1:	48 89 6c 24 18       	mov    QWORD PTR [rsp+0x18],rbp
   1400fdee6:	48 89 74 24 20       	mov    QWORD PTR [rsp+0x20],rsi
   1400fdeeb:	57                   	push   rdi
   1400fdeec:	41 54                	push   r12
   1400fdeee:	41 55                	push   r13
   1400fdef0:	48 83 ec 50          	sub    rsp,0x50
   1400fdef4:	48 8b 05 5d c4 03 00 	mov    rax,QWORD PTR [rip+0x3c45d]        # 0x14013a358
   1400fdefb:	44 8b 6a 08          	mov    r13d,DWORD PTR [rdx+0x8]
   1400fdeff:	4c 8b 62 10          	mov    r12,QWORD PTR [rdx+0x10]
   1400fdf03:	48 6b c0 0d          	imul   rax,rax,0xd
   1400fdf07:	48 8b 6a 18          	mov    rbp,QWORD PTR [rdx+0x18]
   1400fdf0b:	48 89 05 46 c4 03 00 	mov    QWORD PTR [rip+0x3c446],rax        # 0x14013a358
   1400fdf12:	80 b9 d0 00 00 00 00 	cmp    BYTE PTR [rcx+0xd0],0x0
   1400fdf19:	48 8b f2             	mov    rsi,rdx
   1400fdf1c:	48 8b f9             	mov    rdi,rcx
   1400fdf1f:	0f 84 c2 01 00 00    	je     0x1400fe0e7
   1400fdf25:	f6 81 d1 00 00 00 40 	test   BYTE PTR [rcx+0xd1],0x40
   1400fdf2c:	0f 85 b5 01 00 00    	jne    0x1400fe0e7
   1400fdf32:	41 81 fd 02 02 00 00 	cmp    r13d,0x202
   1400fdf39:	75 53                	jne    0x1400fdf8e
   1400fdf3b:	33 d2                	xor    edx,edx
   1400fdf3d:	c6 81 ad 00 00 00 00 	mov    BYTE PTR [rcx+0xad],0x0
   1400fdf44:	48 8b 49 20          	mov    rcx,QWORD PTR [rcx+0x20]
   1400fdf48:	44 8d 42 01          	lea    r8d,[rdx+0x1]
   1400fdf4c:	ff 15 fe df 01 00    	call   QWORD PTR [rip+0x1dffe]        # 0x14011bf50
   1400fdf52:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdf56:	ff 15 54 e2 01 00    	call   QWORD PTR [rip+0x1e254]        # 0x14011c1b0
   1400fdf5c:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdf60:	ff 15 4a e2 01 00    	call   QWORD PTR [rip+0x1e24a]        # 0x14011c1b0
   1400fdf66:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdf6a:	0f b7 d8             	movzx  ebx,ax
   1400fdf6d:	48 81 cb 00 00 02 02 	or     rbx,0x2020000
   1400fdf74:	ff 15 3e e2 01 00    	call   QWORD PTR [rip+0x1e23e]        # 0x14011c1b8
   1400fdf7a:	45 8b cd             	mov    r9d,r13d
   1400fdf7d:	4c 8b c3             	mov    r8,rbx
   1400fdf80:	ba 41 04 00 00       	mov    edx,0x441
   1400fdf85:	48 8b c8             	mov    rcx,rax
   1400fdf88:	ff 15 4a e0 01 00    	call   QWORD PTR [rip+0x1e04a]        # 0x14011bfd8
   1400fdf8e:	41 81 fd 00 02 00 00 	cmp    r13d,0x200
   1400fdf95:	0f 85 98 00 00 00    	jne    0x1400fe033
   1400fdf9b:	8a 87 ad 00 00 00    	mov    al,BYTE PTR [rdi+0xad]
   1400fdfa1:	41 83 e4 01          	and    r12d,0x1
   1400fdfa5:	84 c0                	test   al,al
   1400fdfa7:	7e 09                	jle    0x1400fdfb2
   1400fdfa9:	4d 85 e4             	test   r12,r12
   1400fdfac:	74 6e                	je     0x1400fe01c
   1400fdfae:	84 c0                	test   al,al
   1400fdfb0:	7f 09                	jg     0x1400fdfbb
   1400fdfb2:	4d 85 e4             	test   r12,r12
   1400fdfb5:	0f 85 ff 00 00 00    	jne    0x1400fe0ba
   1400fdfbb:	80 bf d0 00 00 00 02 	cmp    BYTE PTR [rdi+0xd0],0x2
   1400fdfc2:	75 1c                	jne    0x1400fdfe0
   1400fdfc4:	48 8d 4c 24 70       	lea    rcx,[rsp+0x70]
   1400fdfc9:	ff 15 c9 e0 01 00    	call   QWORD PTR [rip+0x1e0c9]        # 0x14011c098
   1400fdfcf:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdfd3:	48 8d 54 24 40       	lea    rdx,[rsp+0x40]
   1400fdfd8:	ff 15 22 e2 01 00    	call   QWORD PTR [rip+0x1e222]        # 0x14011c200
   1400fdfde:	eb 24                	jmp    0x1400fe004
   1400fdfe0:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fdfe4:	0f b7 c5             	movzx  eax,bp
   1400fdfe7:	48 8d 54 24 40       	lea    rdx,[rsp+0x40]
   1400fdfec:	89 44 24 70          	mov    DWORD PTR [rsp+0x70],eax
   1400fdff0:	48 8b c5             	mov    rax,rbp
   1400fdff3:	48 c1 e8 10          	shr    rax,0x10
   1400fdff7:	0f b7 c0             	movzx  eax,ax
   1400fdffa:	89 44 24 74          	mov    DWORD PTR [rsp+0x74],eax
   1400fdffe:	ff 15 2c e1 01 00    	call   QWORD PTR [rip+0x1e12c]        # 0x14011c130
   1400fe004:	48 8b 54 24 70       	mov    rdx,QWORD PTR [rsp+0x70]
   1400fe009:	48 8d 4c 24 40       	lea    rcx,[rsp+0x40]
   1400fe00e:	ff 15 d4 dd 01 00    	call   QWORD PTR [rip+0x1ddd4]        # 0x14011bde8
   1400fe014:	85 c0                	test   eax,eax
   1400fe016:	0f 85 cb 00 00 00    	jne    0x1400fe0e7
   1400fe01c:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fe020:	33 d2                	xor    edx,edx
   1400fe022:	c6 87 ad 00 00 00 00 	mov    BYTE PTR [rdi+0xad],0x0
   1400fe029:	44 8d 42 01          	lea    r8d,[rdx+0x1]
   1400fe02d:	ff 15 1d df 01 00    	call   QWORD PTR [rip+0x1df1d]        # 0x14011bf50
   1400fe033:	41 81 fd 01 02 00 00 	cmp    r13d,0x201
   1400fe03a:	0f 85 a7 00 00 00    	jne    0x1400fe0e7
   1400fe040:	80 bf ad 00 00 00 00 	cmp    BYTE PTR [rdi+0xad],0x0
   1400fe047:	7e 17                	jle    0x1400fe060
   1400fe049:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fe04d:	33 d2                	xor    edx,edx
   1400fe04f:	c6 87 ad 00 00 00 00 	mov    BYTE PTR [rdi+0xad],0x0
   1400fe056:	44 8d 42 01          	lea    r8d,[rdx+0x1]
   1400fe05a:	ff 15 f0 de 01 00    	call   QWORD PTR [rip+0x1def0]        # 0x14011bf50
   1400fe060:	80 bf d0 00 00 00 02 	cmp    BYTE PTR [rdi+0xd0],0x2
   1400fe067:	75 1c                	jne    0x1400fe085
   1400fe069:	48 8d 4c 24 70       	lea    rcx,[rsp+0x70]
   1400fe06e:	ff 15 24 e0 01 00    	call   QWORD PTR [rip+0x1e024]        # 0x14011c098
   1400fe074:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fe078:	48 8d 54 24 40       	lea    rdx,[rsp+0x40]
   1400fe07d:	ff 15 7d e1 01 00    	call   QWORD PTR [rip+0x1e17d]        # 0x14011c200
   1400fe083:	eb 21                	jmp    0x1400fe0a6
   1400fe085:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fe089:	0f b7 c5             	movzx  eax,bp
   1400fe08c:	48 c1 ed 10          	shr    rbp,0x10
   1400fe090:	89 44 24 70          	mov    DWORD PTR [rsp+0x70],eax
   1400fe094:	0f b7 c5             	movzx  eax,bp
   1400fe097:	48 8d 54 24 40       	lea    rdx,[rsp+0x40]
   1400fe09c:	89 44 24 74          	mov    DWORD PTR [rsp+0x74],eax
   1400fe0a0:	ff 15 8a e0 01 00    	call   QWORD PTR [rip+0x1e08a]        # 0x14011c130
   1400fe0a6:	48 8b 54 24 70       	mov    rdx,QWORD PTR [rsp+0x70]
   1400fe0ab:	48 8d 4c 24 40       	lea    rcx,[rsp+0x40]
   1400fe0b0:	ff 15 32 dd 01 00    	call   QWORD PTR [rip+0x1dd32]        # 0x14011bde8
   1400fe0b6:	85 c0                	test   eax,eax
   1400fe0b8:	74 2d                	je     0x1400fe0e7
   1400fe0ba:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fe0be:	45 33 c9             	xor    r9d,r9d
   1400fe0c1:	ba 13 27 00 00       	mov    edx,0x2713
   1400fe0c6:	45 8d 41 0a          	lea    r8d,[r9+0xa]
   1400fe0ca:	ff 15 10 df 01 00    	call   QWORD PTR [rip+0x1df10]        # 0x14011bfe0
   1400fe0d0:	48 8b 4f 20          	mov    rcx,QWORD PTR [rdi+0x20]
   1400fe0d4:	33 d2                	xor    edx,edx
   1400fe0d6:	44 8d 42 01          	lea    r8d,[rdx+0x1]
   1400fe0da:	c6 87 ad 00 00 00 01 	mov    BYTE PTR [rdi+0xad],0x1
   1400fe0e1:	ff 15 69 de 01 00    	call   QWORD PTR [rip+0x1de69]        # 0x14011bf50
   1400fe0e7:	48 8b 47 20          	mov    rax,QWORD PTR [rdi+0x20]
   1400fe0eb:	48 83 64 24 30 00    	and    QWORD PTR [rsp+0x30],0x0
   1400fe0f1:	4c 8b 4e 18          	mov    r9,QWORD PTR [rsi+0x18]
   1400fe0f5:	4c 8b 46 10          	mov    r8,QWORD PTR [rsi+0x10]
   1400fe0f9:	8b 56 08             	mov    edx,DWORD PTR [rsi+0x8]
   1400fe0fc:	48 8b 8f d8 00 00 00 	mov    rcx,QWORD PTR [rdi+0xd8]
   1400fe103:	c6 44 24 28 80       	mov    BYTE PTR [rsp+0x28],0x80
   1400fe108:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1400fe10d:	e8 1a 36 ff ff       	call   0x1400f172c
   1400fe112:	4c 8d 5c 24 50       	lea    r11,[rsp+0x50]
   1400fe117:	33 c0                	xor    eax,eax
   1400fe119:	49 8b 5b 28          	mov    rbx,QWORD PTR [r11+0x28]
   1400fe11d:	49 8b 6b 30          	mov    rbp,QWORD PTR [r11+0x30]
   1400fe121:	49 8b 73 38          	mov    rsi,QWORD PTR [r11+0x38]
   1400fe125:	49 8b e3             	mov    rsp,r11
   1400fe128:	41 5d                	pop    r13
   1400fe12a:	41 5c                	pop    r12
   1400fe12c:	5f                   	pop    rdi
   1400fe12d:	c3                   	ret
   1400fe12e:	cc                   	int3
   1400fe12f:	cc                   	int3
   1400fe130:	40 53                	rex push rbx
   1400fe132:	48 83 ec 20          	sub    rsp,0x20
   1400fe136:	48 8b d9             	mov    rbx,rcx
   1400fe139:	e8 06 ee ff ff       	call   0x1400fcf44
   1400fe13e:	4c 8d 1d 9b ec 02 00 	lea    r11,[rip+0x2ec9b]        # 0x14012cde0
   1400fe145:	b9 05 00 00 00       	mov    ecx,0x5
   1400fe14a:	c6 83 a4 00 00 00 03 	mov    BYTE PTR [rbx+0xa4],0x3
   1400fe151:	4c 89 1b             	mov    QWORD PTR [rbx],r11
   1400fe154:	ff 15 c6 d0 01 00    	call   QWORD PTR [rip+0x1d0c6]        # 0x14011b220
   1400fe15a:	48 89 43 38          	mov    QWORD PTR [rbx+0x38],rax
   1400fe15e:	48 8b c3             	mov    rax,rbx
   1400fe161:	48 83 c4 20          	add    rsp,0x20
   1400fe165:	5b                   	pop    rbx
   1400fe166:	c3                   	ret
   1400fe167:	cc                   	int3
   1400fe168:	48 89 4c 24 08       	mov    QWORD PTR [rsp+0x8],rcx
   1400fe16d:	53                   	push   rbx
   1400fe16e:	55                   	push   rbp
   1400fe16f:	56                   	push   rsi
   1400fe170:	57                   	push   rdi
   1400fe171:	41 54                	push   r12
   1400fe173:	41 55                	push   r13
   1400fe175:	41 56                	push   r14
   1400fe177:	41 57                	push   r15
   1400fe179:	48 81 ec 98 00 00 00 	sub    rsp,0x98
   1400fe180:	44 8b b4 24 00 01 00 	mov    r14d,DWORD PTR [rsp+0x100]
   1400fe187:	00 
   1400fe188:	8b b4 24 00 01 00 00 	mov    esi,DWORD PTR [rsp+0x100]
   1400fe18f:	49 8b f9             	mov    rdi,r9
   1400fe192:	8b 47 08             	mov    eax,DWORD PTR [rdi+0x8]
   1400fe195:	45 33 c9             	xor    r9d,r9d
   1400fe198:	41 83 e6 da          	and    r14d,0xffffffda
   1400fe19c:	2b 07                	sub    eax,DWORD PTR [rdi]
   1400fe19e:	83 e6 04             	and    esi,0x4
   1400fe1a1:	4d 3b c1             	cmp    r8,r9
   1400fe1a4:	48 98                	cdqe
   1400fe1a6:	49 8b d8             	mov    rbx,r8
   1400fe1a9:	4c 8b e2             	mov    r12,rdx
   1400fe1ac:	44 89 b4 24 e8 00 00 	mov    DWORD PTR [rsp+0xe8],r14d
   1400fe1b3:	00 
   1400fe1b4:	44 89 8c 24 f0 00 00 	mov    DWORD PTR [rsp+0xf0],r9d
   1400fe1bb:	00 
   1400fe1bc:	48 89 44 24 50       	mov    QWORD PTR [rsp+0x50],rax
   1400fe1c1:	7d 0f                	jge    0x1400fe1d2
   1400fe1c3:	48 8b ca             	mov    rcx,rdx
   1400fe1c6:	ff 15 dc d8 01 00    	call   QWORD PTR [rip+0x1d8dc]        # 0x14011baa8
   1400fe1cc:	45 33 c9             	xor    r9d,r9d
   1400fe1cf:	48 63 d8             	movsxd rbx,eax
   1400fe1d2:	48 89 5c 24 58       	mov    QWORD PTR [rsp+0x58],rbx
   1400fe1d7:	49 8b ec             	mov    rbp,r12
   1400fe1da:	4c 89 64 24 70       	mov    QWORD PTR [rsp+0x70],r12
   1400fe1df:	4d 8b e9             	mov    r13,r9
   1400fe1e2:	4c 89 4c 24 40       	mov    QWORD PTR [rsp+0x40],r9
   1400fe1e7:	4c 89 4c 24 48       	mov    QWORD PTR [rsp+0x48],r9
   1400fe1ec:	4d 8b f9             	mov    r15,r9
   1400fe1ef:	4c 89 8c 24 f8 00 00 	mov    QWORD PTR [rsp+0xf8],r9
   1400fe1f6:	00 
   1400fe1f7:	41 3b f1             	cmp    esi,r9d
   1400fe1fa:	0f 84 74 02 00 00    	je     0x1400fe474
   1400fe200:	48 63 47 04          	movsxd rax,DWORD PTR [rdi+0x4]
   1400fe204:	48 8b d3             	mov    rdx,rbx
   1400fe207:	49 8b f1             	mov    rsi,r9
   1400fe20a:	48 89 44 24 78       	mov    QWORD PTR [rsp+0x78],rax
   1400fe20f:	8b 47 0c             	mov    eax,DWORD PTR [rdi+0xc]
   1400fe212:	48 c1 e2 03          	shl    rdx,0x3
   1400fe216:	2b 47 04             	sub    eax,DWORD PTR [rdi+0x4]
   1400fe219:	4c 89 4c 24 60       	mov    QWORD PTR [rsp+0x60],r9
   1400fe21e:	48 63 c8             	movsxd rcx,eax
   1400fe221:	48 89 8c 24 80 00 00 	mov    QWORD PTR [rsp+0x80],rcx
   1400fe228:	00 
   1400fe229:	48 8d 4c 24 48       	lea    rcx,[rsp+0x48]
   1400fe22e:	e8 f1 51 f6 ff       	call   0x140063424
   1400fe233:	4c 8b 7c 24 48       	mov    r15,QWORD PTR [rsp+0x48]
   1400fe238:	45 33 c9             	xor    r9d,r9d
   1400fe23b:	49 3b d9             	cmp    rbx,r9
   1400fe23e:	49 8b cf             	mov    rcx,r15
   1400fe241:	4c 89 bc 24 f8 00 00 	mov    QWORD PTR [rsp+0xf8],r15
   1400fe248:	00 
   1400fe249:	48 89 4c 24 68       	mov    QWORD PTR [rsp+0x68],rcx
   1400fe24e:	0f 8e dd 00 00 00    	jle    0x1400fe331
   1400fe254:	4c 8b 7c 24 50       	mov    r15,QWORD PTR [rsp+0x50]
   1400fe259:	8b 47 0c             	mov    eax,DWORD PTR [rdi+0xc]
   1400fe25c:	39 47 04             	cmp    DWORD PTR [rdi+0x4],eax
   1400fe25f:	0f 8d bf 00 00 00    	jge    0x1400fe324
   1400fe265:	f3 0f 6f 07          	movdqu xmm0,XMMWORD PTR [rdi]
   1400fe269:	b8 01 00 00 00       	mov    eax,0x1
   1400fe26e:	48 3b c3             	cmp    rax,rbx
   1400fe271:	48 8b f0             	mov    rsi,rax
   1400fe274:	48 8b e8             	mov    rbp,rax
   1400fe277:	f3 0f 7f 44 24 30    	movdqu XMMWORD PTR [rsp+0x30],xmm0
   1400fe27d:	7f 67                	jg     0x1400fe2e6
   1400fe27f:	45 8b ee             	mov    r13d,r14d
   1400fe282:	4c 8b b4 24 e0 00 00 	mov    r14,QWORD PTR [rsp+0xe0]
   1400fe289:	00 
   1400fe28a:	41 0f ba ed 0a       	bts    r13d,0xa
   1400fe28f:	f3 0f 6f 07          	movdqu xmm0,XMMWORD PTR [rdi]
   1400fe293:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   1400fe298:	44 8b c6             	mov    r8d,esi
   1400fe29b:	49 8b d4             	mov    rdx,r12
   1400fe29e:	49 8b ce             	mov    rcx,r14
   1400fe2a1:	44 89 6c 24 20       	mov    DWORD PTR [rsp+0x20],r13d
   1400fe2a6:	f3 0f 7f 44 24 30    	movdqu XMMWORD PTR [rsp+0x30],xmm0
   1400fe2ac:	ff 15 8e de 01 00    	call   QWORD PTR [rip+0x1de8e]        # 0x14011c140
   1400fe2b2:	8b 44 24 38          	mov    eax,DWORD PTR [rsp+0x38]
   1400fe2b6:	2b 44 24 30          	sub    eax,DWORD PTR [rsp+0x30]
   1400fe2ba:	48 98                	cdqe
   1400fe2bc:	49 3b c7             	cmp    rax,r15
   1400fe2bf:	7f 10                	jg     0x1400fe2d1
   1400fe2c1:	48 8b ee             	mov    rbp,rsi
   1400fe2c4:	48 ff c6             	inc    rsi
   1400fe2c7:	49 3b c7             	cmp    rax,r15
   1400fe2ca:	74 05                	je     0x1400fe2d1
   1400fe2cc:	48 3b f3             	cmp    rsi,rbx
   1400fe2cf:	7e be                	jle    0x1400fe28f
   1400fe2d1:	44                   	rex.R
   1400fe2d2:	8b                   	.byte 0x8b
   1400fe2d3:	b4 24                	mov    ah,0x24
   1400fe2d5:	e8                   	.byte 0xe8
	...
