
PECMD.exe：     文件格式 pei-x86-64


Disassembly of section .text:

00000001400e4864 <.text+0xe3864>:
   1400e4864:	48 8b c4             	mov    rax,rsp
   1400e4867:	48 89 58 10          	mov    QWORD PTR [rax+0x10],rbx
   1400e486b:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
   1400e486f:	55                   	push   rbp
   1400e4870:	56                   	push   rsi
   1400e4871:	57                   	push   rdi
   1400e4872:	41 54                	push   r12
   1400e4874:	41 55                	push   r13
   1400e4876:	41 56                	push   r14
   1400e4878:	41 57                	push   r15
   1400e487a:	48 81 ec 80 00 00 00 	sub    rsp,0x80
   1400e4881:	33 c9                	xor    ecx,ecx
   1400e4883:	45 8a e9             	mov    r13b,r9b
   1400e4886:	48 8d 70 b0          	lea    rsi,[rax-0x50]
   1400e488a:	41 80 e5 07          	and    r13b,0x7
   1400e488e:	48 8d 58 b8          	lea    rbx,[rax-0x48]
   1400e4892:	48 89 48 b0          	mov    QWORD PTR [rax-0x50],rcx
   1400e4896:	48 89 48 b8          	mov    QWORD PTR [rax-0x48],rcx
   1400e489a:	4d 8b e1             	mov    r12,r9
   1400e489d:	49 0f be c5          	movsx  rax,r13b
   1400e48a1:	4c 2b e0             	sub    r12,rax
   1400e48a4:	45 8b f0             	mov    r14d,r8d
   1400e48a7:	4c 8b fa             	mov    r15,rdx
   1400e48aa:	8b e9                	mov    ebp,ecx
   1400e48ac:	8d 79 01             	lea    edi,[rcx+0x1]
   1400e48af:	44 3a e9             	cmp    r13b,cl
   1400e48b2:	74 12                	je     0x1400e48c6
   1400e48b4:	49 8d 74 24 e8       	lea    rsi,[r12-0x18]
   1400e48b9:	48 8d 5e 08          	lea    rbx,[rsi+0x8]
   1400e48bd:	48 39 0b             	cmp    QWORD PTR [rbx],rcx
   1400e48c0:	0f 85 48 02 00 00    	jne    0x1400e4b0e
   1400e48c6:	41 81 f8 c7 cf ff ff 	cmp    r8d,0xffffcfc7
   1400e48cd:	0f 85 e4 01 00 00    	jne    0x1400e4ab7
   1400e48d3:	48 8d 15 52 68 04 00 	lea    rdx,[rip+0x46852]        # 0x14012b12c
   1400e48da:	49 83 c8 ff          	or     r8,0xffffffffffffffff
   1400e48de:	49 8b cc             	mov    rcx,r12
   1400e48e1:	e8 ee eb f7 ff       	call   0x1400634d4
   1400e48e6:	48 8d 4c 24 50       	lea    rcx,[rsp+0x50]
   1400e48eb:	ba 00 1c 00 00       	mov    edx,0x1c00
   1400e48f0:	33 db                	xor    ebx,ebx
   1400e48f2:	c7 44 24 44 00 04 00 	mov    DWORD PTR [rsp+0x44],0x400
   1400e48f9:	00 
   1400e48fa:	44 8b ef             	mov    r13d,edi
   1400e48fd:	8b f7                	mov    esi,edi
   1400e48ff:	e8 a4 ea f7 ff       	call   0x1400633a8
   1400e4904:	4c 8b 5c 24 50       	mov    r11,QWORD PTR [rsp+0x50]
   1400e4909:	48 8d 8c 24 d8 00 00 	lea    rcx,[rsp+0xd8]
   1400e4910:	00 
   1400e4911:	49 8d 83 00 18 00 00 	lea    rax,[r11+0x1800]
   1400e4918:	49 8d ab 00 10 00 00 	lea    rbp,[r11+0x1000]
   1400e491f:	48 89 4c 24 28       	mov    QWORD PTR [rsp+0x28],rcx
   1400e4924:	48 21 5c 24 20       	and    QWORD PTR [rsp+0x20],rbx
   1400e4929:	48 89 44 24 58       	mov    QWORD PTR [rsp+0x58],rax
   1400e492e:	4d 8d bb 00 14 00 00 	lea    r15,[r11+0x1400]
   1400e4935:	33 c9                	xor    ecx,ecx
   1400e4937:	eb 39                	jmp    0x1400e4972
   1400e4939:	48 8d 84 24 d8 00 00 	lea    rax,[rsp+0xd8]
   1400e4940:	00 
   1400e4941:	4c 8d 4c 24 40       	lea    r9,[rsp+0x40]
   1400e4946:	45 33 c0             	xor    r8d,r8d
   1400e4949:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   1400e494e:	8b cb                	mov    ecx,ebx
   1400e4950:	48 89 6c 24 20       	mov    QWORD PTR [rsp+0x20],rbp
   1400e4955:	ff 15 65 68 03 00    	call   QWORD PTR [rip+0x36865]        # 0x14011b1c0
   1400e495b:	03 df                	add    ebx,edi
   1400e495d:	48 8d 84 24 d8 00 00 	lea    rax,[rsp+0xd8]
   1400e4964:	00 
   1400e4965:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   1400e496a:	48 83 64 24 20 00    	and    QWORD PTR [rsp+0x20],0x0
   1400e4970:	8b cb                	mov    ecx,ebx
   1400e4972:	4c 8d 4c 24 40       	lea    r9,[rsp+0x40]
   1400e4977:	45 33 c0             	xor    r8d,r8d
   1400e497a:	33 d2                	xor    edx,edx
   1400e497c:	ff 15 3e 68 03 00    	call   QWORD PTR [rip+0x3683e]        # 0x14011b1c0
   1400e4982:	33 d2                	xor    edx,edx
   1400e4984:	85 c0                	test   eax,eax
   1400e4986:	75 b1                	jne    0x1400e4939
   1400e4988:	21 44 24 20          	and    DWORD PTR [rsp+0x20],eax
   1400e498c:	48 8d 4c 24 60       	lea    rcx,[rsp+0x60]
   1400e4991:	44 8b cf             	mov    r9d,edi
   1400e4994:	45 33 c0             	xor    r8d,r8d
   1400e4997:	ff 15 13 68 03 00    	call   QWORD PTR [rip+0x36813]        # 0x14011b1b0
   1400e499d:	85 c0                	test   eax,eax
   1400e499f:	0f 84 01 01 00 00    	je     0x1400e4aa6
   1400e49a5:	48 8b 4c 24 60       	mov    rcx,QWORD PTR [rsp+0x60]
   1400e49aa:	4c 8d 4c 24 44       	lea    r9,[rsp+0x44]
   1400e49af:	4d 8b c7             	mov    r8,r15
   1400e49b2:	8b d7                	mov    edx,edi
   1400e49b4:	44 89 6c 24 20       	mov    DWORD PTR [rsp+0x20],r13d
   1400e49b9:	ff 15 f9 67 03 00    	call   QWORD PTR [rip+0x367f9]        # 0x14011b1b8
   1400e49bf:	85 c0                	test   eax,eax
   1400e49c1:	0f 84 c6 00 00 00    	je     0x1400e4a8d
   1400e49c7:	83 64 24 48 00       	and    DWORD PTR [rsp+0x48],0x0
   1400e49cc:	48 8b 4c 24 58       	mov    rcx,QWORD PTR [rsp+0x58]
   1400e49d1:	45 8b 2f             	mov    r13d,DWORD PTR [r15]
   1400e49d4:	49 8d 57 04          	lea    rdx,[r15+0x4]
   1400e49d8:	b8 63 00 00 00       	mov    eax,0x63
   1400e49dd:	8b 6a 04             	mov    ebp,DWORD PTR [rdx+0x4]
   1400e49e0:	44 8b 32             	mov    r14d,DWORD PTR [rdx]
   1400e49e3:	3b e8                	cmp    ebp,eax
   1400e49e5:	0f 42 c5             	cmovb  eax,ebp
   1400e49e8:	48 83 c2 08          	add    rdx,0x8
   1400e49ec:	44 8b c0             	mov    r8d,eax
   1400e49ef:	8b d8                	mov    ebx,eax
   1400e49f1:	e8 ea 2d 02 00       	call   0x1401077e0
   1400e49f6:	48 8b 54 24 58       	mov    rdx,QWORD PTR [rsp+0x58]
   1400e49fb:	41 8b cd             	mov    ecx,r13d
   1400e49fe:	81 e1 00 e0 00 00    	and    ecx,0xe000
   1400e4a04:	c6 04 13 00          	mov    BYTE PTR [rbx+rdx*1],0x0
   1400e4a08:	81 f9 00 20 00 00    	cmp    ecx,0x2000
   1400e4a0e:	74 3c                	je     0x1400e4a4c
   1400e4a10:	81 f9 00 60 00 00    	cmp    ecx,0x6000
   1400e4a16:	74 2b                	je     0x1400e4a43
   1400e4a18:	81 f9 00 80 00 00    	cmp    ecx,0x8000
   1400e4a1e:	74 1a                	je     0x1400e4a3a
   1400e4a20:	48 8d 05 fd 66 04 00 	lea    rax,[rip+0x466fd]        # 0x14012b124
   1400e4a27:	4c 8d 05 ee 66 04 00 	lea    r8,[rip+0x466ee]        # 0x14012b11c
   1400e4a2e:	81 f9 00 a0 00 00    	cmp    ecx,0xa000
   1400e4a34:	49 0f 44 c0          	cmove  rax,r8
   1400e4a38:	eb 19                	jmp    0x1400e4a53
   1400e4a3a:	48 8d 05 d3 66 04 00 	lea    rax,[rip+0x466d3]        # 0x14012b114
   1400e4a41:	eb 10                	jmp    0x1400e4a53
   1400e4a43:	48 8d 05 c2 66 04 00 	lea    rax,[rip+0x466c2]        # 0x14012b10c
   1400e4a4a:	eb 07                	jmp    0x1400e4a53
   1400e4a4c:	48 8d 05 b1 66 04 00 	lea    rax,[rip+0x466b1]        # 0x14012b104
   1400e4a53:	48 8b 4c 24 50       	mov    rcx,QWORD PTR [rsp+0x50]
   1400e4a58:	48 89 54 24 30       	mov    QWORD PTR [rsp+0x30],rdx
   1400e4a5d:	48 8d 15 84 66 04 00 	lea    rdx,[rip+0x46684]        # 0x14012b0e8
   1400e4a64:	45 8b ce             	mov    r9d,r14d
   1400e4a67:	45 8b c5             	mov    r8d,r13d
   1400e4a6a:	89 6c 24 28          	mov    DWORD PTR [rsp+0x28],ebp
   1400e4a6e:	48 89 44 24 20       	mov    QWORD PTR [rsp+0x20],rax
   1400e4a73:	ff 15 3f 76 03 00    	call   QWORD PTR [rip+0x3763f]        # 0x14011c0b8
   1400e4a79:	48 8b 54 24 50       	mov    rdx,QWORD PTR [rsp+0x50]
   1400e4a7e:	49 8b cc             	mov    rcx,r12
   1400e4a81:	e8 d6 e9 f7 ff       	call   0x14006345c
   1400e4a86:	44 8b 6c 24 48       	mov    r13d,DWORD PTR [rsp+0x48]
   1400e4a8b:	eb 02                	jmp    0x1400e4a8f
   1400e4a8d:	33 f6                	xor    esi,esi
   1400e4a8f:	85 f6                	test   esi,esi
   1400e4a91:	0f 85 0e ff ff ff    	jne    0x1400e49a5
   1400e4a97:	48 8b 4c 24 60       	mov    rcx,QWORD PTR [rsp+0x60]
   1400e4a9c:	33 d2                	xor    edx,edx
   1400e4a9e:	ff 15 ec 66 03 00    	call   QWORD PTR [rip+0x366ec]        # 0x14011b190
   1400e4aa4:	33 ff                	xor    edi,edi
   1400e4aa6:	48 8d 4c 24 50       	lea    rcx,[rsp+0x50]
   1400e4aab:	e8 54 66 f7 ff       	call   0x14005b104
   1400e4ab0:	8b c7                	mov    eax,edi
   1400e4ab2:	e9 66 01 00 00       	jmp    0x1400e4c1d
   1400e4ab7:	4c 8d 05 d2 65 04 00 	lea    r8,[rip+0x465d2]        # 0x14012b090
   1400e4abe:	44 8b cf             	mov    r9d,edi
   1400e4ac1:	33 d2                	xor    edx,edx
   1400e4ac3:	48 8b ce             	mov    rcx,rsi
   1400e4ac6:	c7 44 24 20 00 00 00 	mov    DWORD PTR [rsp+0x20],0xf0000000
   1400e4acd:	f0 
   1400e4ace:	ff 15 dc 66 03 00    	call   QWORD PTR [rip+0x366dc]        # 0x14011b1b0
   1400e4ad4:	85 c0                	test   eax,eax
   1400e4ad6:	75 0b                	jne    0x1400e4ae3
   1400e4ad8:	ff 15 e2 6e 03 00    	call   QWORD PTR [rip+0x36ee2]        # 0x14011b9c0
   1400e4ade:	e9 3a 01 00 00       	jmp    0x1400e4c1d
   1400e4ae3:	48 8b 0e             	mov    rcx,QWORD PTR [rsi]
   1400e4ae6:	45 33 c9             	xor    r9d,r9d
   1400e4ae9:	45 33 c0             	xor    r8d,r8d
   1400e4aec:	41 8b d6             	mov    edx,r14d
   1400e4aef:	48 89 5c 24 20       	mov    QWORD PTR [rsp+0x20],rbx
   1400e4af4:	ff 15 ae 66 03 00    	call   QWORD PTR [rip+0x366ae]        # 0x14011b1a8
   1400e4afa:	85 c0                	test   eax,eax
   1400e4afc:	75 10                	jne    0x1400e4b0e
   1400e4afe:	ff 15 bc 6e 03 00    	call   QWORD PTR [rip+0x36ebc]        # 0x14011b9c0
   1400e4b04:	45 33 ff             	xor    r15d,r15d
   1400e4b07:	8b e8                	mov    ebp,eax
   1400e4b09:	e9 e9 00 00 00       	jmp    0x1400e4bf7
   1400e4b0e:	48 8b 94 24 c0 00 00 	mov    rdx,QWORD PTR [rsp+0xc0]
   1400e4b15:	00 
   1400e4b16:	48 8b 0b             	mov    rcx,QWORD PTR [rbx]
   1400e4b19:	45 33 c9             	xor    r9d,r9d
   1400e4b1c:	45 8b c7             	mov    r8d,r15d
   1400e4b1f:	ff 15 7b 66 03 00    	call   QWORD PTR [rip+0x3667b]        # 0x14011b1a0
   1400e4b25:	45 33 ff             	xor    r15d,r15d
   1400e4b28:	41 3b c7             	cmp    eax,r15d
   1400e4b2b:	75 0d                	jne    0x1400e4b3a
   1400e4b2d:	ff 15 8d 6e 03 00    	call   QWORD PTR [rip+0x36e8d]        # 0x14011b9c0
   1400e4b33:	8b e8                	mov    ebp,eax
   1400e4b35:	e9 bd 00 00 00       	jmp    0x1400e4bf7
   1400e4b3a:	41 f6 c5 02          	test   r13b,0x2
   1400e4b3e:	74 07                	je     0x1400e4b47
   1400e4b40:	33 c0                	xor    eax,eax
   1400e4b42:	e9 d6 00 00 00       	jmp    0x1400e4c1d
   1400e4b47:	48 8b 0b             	mov    rcx,QWORD PTR [rbx]
   1400e4b4a:	4c 8d 8c 24 d8 00 00 	lea    r9,[rsp+0xd8]
   1400e4b51:	00 
   1400e4b52:	4c 8d 44 24 40       	lea    r8,[rsp+0x40]
   1400e4b57:	ba 04 00 00 00       	mov    edx,0x4
   1400e4b5c:	44 89 7c 24 40       	mov    DWORD PTR [rsp+0x40],r15d
   1400e4b61:	c7 84 24 d8 00 00 00 	mov    DWORD PTR [rsp+0xd8],0x4
   1400e4b68:	04 00 00 00 
   1400e4b6c:	44 89 7c 24 20       	mov    DWORD PTR [rsp+0x20],r15d
   1400e4b71:	ff 15 21 66 03 00    	call   QWORD PTR [rip+0x36621]        # 0x14011b198
   1400e4b77:	8b 54 24 40          	mov    edx,DWORD PTR [rsp+0x40]
   1400e4b7b:	48 8d 4c 24 60       	lea    rcx,[rsp+0x60]
   1400e4b80:	e8 23 e8 f7 ff       	call   0x1400633a8
   1400e4b85:	4c 8b 6c 24 60       	mov    r13,QWORD PTR [rsp+0x60]
   1400e4b8a:	8b 44 24 40          	mov    eax,DWORD PTR [rsp+0x40]
   1400e4b8e:	48 8b 0b             	mov    rcx,QWORD PTR [rbx]
   1400e4b91:	4c 8d 8c 24 d8 00 00 	lea    r9,[rsp+0xd8]
   1400e4b98:	00 
   1400e4b99:	4d 8b c5             	mov    r8,r13
   1400e4b9c:	ba 02 00 00 00       	mov    edx,0x2
   1400e4ba1:	89 84 24 d8 00 00 00 	mov    DWORD PTR [rsp+0xd8],eax
   1400e4ba8:	44 89 7c 24 20       	mov    DWORD PTR [rsp+0x20],r15d
   1400e4bad:	ff 15 e5 65 03 00    	call   QWORD PTR [rip+0x365e5]        # 0x14011b198
   1400e4bb3:	45 88 3c 24          	mov    BYTE PTR [r12],r15b
   1400e4bb7:	45 8b f7             	mov    r14d,r15d
   1400e4bba:	44 39 bc 24 d8 00 00 	cmp    DWORD PTR [rsp+0xd8],r15d
   1400e4bc1:	00 
   1400e4bc2:	76 29                	jbe    0x1400e4bed
   1400e4bc4:	45 0f b6 45 00       	movzx  r8d,BYTE PTR [r13+0x0]
   1400e4bc9:	48 8d 15 88 28 04 00 	lea    rdx,[rip+0x42888]        # 0x140127458
   1400e4bd0:	49 8b cc             	mov    rcx,r12
   1400e4bd3:	ff 15 df 74 03 00    	call   QWORD PTR [rip+0x374df]        # 0x14011c0b8
   1400e4bd9:	44 03 f7             	add    r14d,edi
   1400e4bdc:	49 83 c4 02          	add    r12,0x2
   1400e4be0:	4c 03 ef             	add    r13,rdi
   1400e4be3:	44 3b b4 24 d8 00 00 	cmp    r14d,DWORD PTR [rsp+0xd8]
   1400e4bea:	00 
   1400e4beb:	72 d7                	jb     0x1400e4bc4
   1400e4bed:	48 8d 4c 24 60       	lea    rcx,[rsp+0x60]
   1400e4bf2:	e8 0d 65 f7 ff       	call   0x14005b104
   1400e4bf7:	48 8b 0b             	mov    rcx,QWORD PTR [rbx]
   1400e4bfa:	49 3b cf             	cmp    rcx,r15
   1400e4bfd:	74 09                	je     0x1400e4c08
   1400e4bff:	ff 15 83 65 03 00    	call   QWORD PTR [rip+0x36583]        # 0x14011b188
   1400e4c05:	4c 89 3b             	mov    QWORD PTR [rbx],r15
   1400e4c08:	48 8b 0e             	mov    rcx,QWORD PTR [rsi]
   1400e4c0b:	49 3b cf             	cmp    rcx,r15
   1400e4c0e:	74 0b                	je     0x1400e4c1b
   1400e4c10:	33 d2                	xor    edx,edx
   1400e4c12:	ff 15 78 65 03 00    	call   QWORD PTR [rip+0x36578]        # 0x14011b190
   1400e4c18:	4c 89 3e             	mov    QWORD PTR [rsi],r15
   1400e4c1b:	8b c5                	mov    eax,ebp
   1400e4c1d:	48 8b 9c 24 c8 00 00 	mov    rbx,QWORD PTR [rsp+0xc8]
   1400e4c24:	00 
   1400e4c25:	48 81 c4 80 00 00 00 	add    rsp,0x80
   1400e4c2c:	41 5f                	pop    r15
   1400e4c2e:	41 5e                	pop    r14
   1400e4c30:	41 5d                	pop    r13
   1400e4c32:	41 5c                	pop    r12
   1400e4c34:	5f                   	pop    rdi
   1400e4c35:	5e                   	pop    rsi
   1400e4c36:	5d                   	pop    rbp
   1400e4c37:	c3                   	ret
   1400e4c38:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   1400e4c3d:	57                   	push   rdi
   1400e4c3e:	48 83 ec 20          	sub    rsp,0x20
   1400e4c42:	33 db                	xor    ebx,ebx
   1400e4c44:	48 8b 0d dd 86 05 00 	mov    rcx,QWORD PTR [rip+0x586dd]        # 0x14013d328
   1400e4c4b:	33 d2                	xor    edx,edx
   1400e4c4d:	41 b8 08 04 00 00    	mov    r8d,0x408
   1400e4c53:	ff 15 b7 6d 03 00    	call   QWORD PTR [rip+0x36db7]        # 0x14011ba10
   1400e4c59:	48 8b f8             	mov    rdi,rax
   1400e4c5c:	48 3b c3             	cmp    rax,rbx
   1400e4c5f:	75 0f                	jne    0x1400e4c70
   1400e4c61:	b9 02 00 00 00       	mov    ecx,0x2
   1400e4c66:	e8 65 e4 f7 ff       	call   0x1400630d0
   1400e4c6b:	83 f8 04             	cmp    eax,0x4
   1400e4c6e:	74 d4                	je     0x1400e4c44
   1400e4c70:	48 8d 4f 08          	lea    rcx,[rdi+0x8]
   1400e4c74:	c7 47 04 55 aa 00 00 	mov    DWORD PTR [rdi+0x4],0xaa55
   1400e4c7b:	48 c7 07 00 04 00 00 	mov    QWORD PTR [rdi],0x400
   1400e4c82:	48 89 0d d7 95 05 00 	mov    QWORD PTR [rip+0x595d7],rcx        # 0x14013e260
   1400e4c89:	8b c3                	mov    eax,ebx
   1400e4c8b:	ba 08 00 00 00       	mov    edx,0x8
   1400e4c90:	a8 01                	test   al,0x1
   1400e4c92:	74 09                	je     0x1400e4c9d
   1400e4c94:	d1 e8                	shr    eax,1
   1400e4c96:	35 20 83 b8 ed       	xor    eax,0xedb88320
   1400e4c9b:	eb 02                	jmp    0x1400e4c9f
   1400e4c9d:	d1 e8                	shr    eax,1
   1400e4c9f:	48 83 ea 01          	sub    rdx,0x1
   1400e4ca3:	75 eb                	jne    0x1400e4c90
   1400e4ca5:	ff c3                	inc    ebx
   1400e4ca7:	89 01                	mov    DWORD PTR [rcx],eax
   1400e4ca9:	48 83 c1 04          	add    rcx,0x4
   1400e4cad:	81 fb 00 01 00 00    	cmp    ebx,0x100
   1400e4cb3:	7c d4                	jl     0x1400e4c89
   1400e4cb5:	48 8b 5c 24 30       	mov    rbx,QWORD PTR [rsp+0x30]
   1400e4cba:	48 83 c4 20          	add    rsp,0x20
   1400e4cbe:	5f                   	pop    rdi
   1400e4cbf:	c3                   	ret
   1400e4cc0:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   1400e4cc5:	48 89 6c 24 10       	mov    QWORD PTR [rsp+0x10],rbp
   1400e4cca:	48 89 74 24 18       	mov    QWORD PTR [rsp+0x18],rsi
   1400e4ccf:	57                   	push   rdi
   1400e4cd0:	48 83 ec 20          	sub    rsp,0x20
   1400e4cd4:	4c 8b 15 85 95 05 00 	mov    r10,QWORD PTR [rip+0x59585]        # 0x14013e260
   1400e4cdb:	49 8b f8             	mov    rdi,r8
   1400e4cde:	48 8b f2             	mov    rsi,rdx
   1400e4ce1:	48 8b e9             	mov    rbp,rcx
   1400e4ce4:	4d 85 d2             	test   r10,r10
   1400e4ce7:	75 0c                	jne    0x1400e4cf5
   1400e4ce9:	e8 4a ff ff ff       	call   0x1400e4c38
   1400e4cee:	4c 8b 15 6b 95 05 00 	mov    r10,QWORD PTR [rip+0x5956b]        # 0x14013e260
   1400e4cf5:	44 8a cf             	mov    r9b,dil
   1400e4cf8:	83 cb ff             	or     ebx,0xffffffff
   1400e4cfb:	45 33 c0             	xor    r8d,r8d
   1400e4cfe:	41 80 e1 07          	and    r9b,0x7
   1400e4d02:	74 15                	je     0x1400e4d19
   1400e4d04:	49 0f be c1          	movsx  rax,r9b
   1400e4d08:	48 2b f8             	sub    rdi,rax
   1400e4d0b:	4c 8d 47 e8          	lea    r8,[rdi-0x18]
   1400e4d0f:	49 83 38 00          	cmp    QWORD PTR [r8],0x0
   1400e4d13:	74 04                	je     0x1400e4d19
   1400e4d15:	41 8b 58 08          	mov    ebx,DWORD PTR [r8+0x8]
   1400e4d19:	48 85 f6             	test   rsi,rsi
   1400e4d1c:	74 1e                	je     0x1400e4d3c
   1400e4d1e:	0f b6 45 00          	movzx  eax,BYTE PTR [rbp+0x0]
   1400e4d22:	0f b6 cb             	movzx  ecx,bl
   1400e4d25:	48 ff c5             	inc    rbp
   1400e4d28:	48 33 c8             	xor    rcx,rax
   1400e4d2b:	8b c3                	mov    eax,ebx
   1400e4d2d:	41 8b 1c 8a          	mov    ebx,DWORD PTR [r10+rcx*4]
   1400e4d31:	c1 e8 08             	shr    eax,0x8
   1400e4d34:	33 d8                	xor    ebx,eax
   1400e4d36:	48 83 ee 01          	sub    rsi,0x1
   1400e4d3a:	75 e2                	jne    0x1400e4d1e
   1400e4d3c:	41 f6 c1 02          	test   r9b,0x2
   1400e4d40:	74 0f                	je     0x1400e4d51
   1400e4d42:	8b c3                	mov    eax,ebx
   1400e4d44:	49 c7 00 01 00 00 00 	mov    QWORD PTR [r8],0x1
   1400e4d4b:	49 89 40 08          	mov    QWORD PTR [r8+0x8],rax
   1400e4d4f:	eb 1a                	jmp    0x1400e4d6b
   1400e4d51:	f7 d3                	not    ebx
   1400e4d53:	48 85 ff             	test   rdi,rdi
   1400e4d56:	74 13                	je     0x1400e4d6b
   1400e4d58:	48 8d 15 d1 63 04 00 	lea    rdx,[rip+0x463d1]        # 0x14012b130
   1400e4d5f:	44 8b c3             	mov    r8d,ebx
   1400e4d62:	48 8b cf             	mov    rcx,rdi
   1400e4d65:	ff 15 4d 73 03 00    	call   QWORD PTR [rip+0x3734d]        # 0x14011c0b8
   1400e4d6b:	48 8b 6c 24 38       	mov    rbp,QWORD PTR [rsp+0x38]
   1400e4d70:	48 8b 74 24 40       	mov    rsi,QWORD PTR [rsp+0x40]
   1400e4d75:	8b c3                	mov    eax,ebx
   1400e4d77:	48 8b 5c 24 30       	mov    rbx,QWORD PTR [rsp+0x30]
   1400e4d7c:	48 83 c4 20          	add    rsp,0x20
   1400e4d80:	5f                   	pop    rdi
   1400e4d81:	c3                   	ret
   1400e4d82:	cc                   	int3
   1400e4d83:	cc                   	int3
   1400e4d84:	33 c0                	xor    eax,eax
   1400e4d86:	48 39 05 db 94 05 00 	cmp    QWORD PTR [rip+0x594db],rax        # 0x14013e268
   1400e4d8d:	0f 95 c0             	setne  al
   1400e4d90:	c3                   	ret
   1400e4d91:	cc                   	int3
   1400e4d92:	cc                   	int3
   1400e4d93:	cc                   	int3
   1400e4d94:	48 89 5c 24 10       	mov    QWORD PTR [rsp+0x10],rbx
   1400e4d99:	57                   	push   rdi
   1400e4d9a:	48 83 ec 20          	sub    rsp,0x20
   1400e4d9e:	48 83 3d c2 94 05 00 	cmp    QWORD PTR [rip+0x594c2],0x0        # 0x14013e268
   1400e4da5:	00 
   1400e4da6:	48 8b d9             	mov    rbx,rcx
   1400e4da9:	0f 85 af 00 00 00    	jne    0x1400e4e5e
   1400e4daf:	48 83 64 24 30 00    	and    QWORD PTR [rsp+0x30],0x0
   1400e4db5:	48 8d 3d f4 63 04 00 	lea    rdi,[rip+0x463f4]        # 0x14012b1b0
   1400e4dbc:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   1400e4dc1:	4c 8d 05 a0 94 05 00 	lea    r8,[rip+0x594a0]        # 0x14013e268
   1400e4dc8:	48 8d 0d d1 63 04 00 	lea    rcx,[rip+0x463d1]        # 0x14012b1a0
   1400e4dcf:	48 8b d7             	mov    rdx,rdi
   1400e4dd2:	e8 51 7a f7 ff       	call   0x14005c828
   1400e4dd7:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   1400e4ddc:	4c 8d 05 8d 94 05 00 	lea    r8,[rip+0x5948d]        # 0x14013e270
   1400e4de3:	48 8d 0d a6 63 04 00 	lea    rcx,[rip+0x463a6]        # 0x14012b190
   1400e4dea:	48 8b d7             	mov    rdx,rdi
   1400e4ded:	e8 36 7a f7 ff       	call   0x14005c828
   1400e4df2:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   1400e4df7:	4c 8d 05 7a 94 05 00 	lea    r8,[rip+0x5947a]        # 0x14013e278
   1400e4dfe:	48 8d 0d 73 63 04 00 	lea    rcx,[rip+0x46373]        # 0x14012b178
   1400e4e05:	48 8b d7             	mov    rdx,rdi
   1400e4e08:	e8 1b 7a f7 ff       	call   0x14005c828
   1400e4e0d:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   1400e4e12:	4c 8d 05 67 94 05 00 	lea    r8,[rip+0x59467]        # 0x14013e280
   1400e4e19:	48 8d 0d 40 63 04 00 	lea    rcx,[rip+0x46340]        # 0x14012b160
   1400e4e20:	48 8b d7             	mov    rdx,rdi
   1400e4e23:	e8 00 7a f7 ff       	call   0x14005c828
   1400e4e28:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   1400e4e2d:	4c 8d 05 54 94 05 00 	lea    r8,[rip+0x59454]        # 0x14013e288
   1400e4e34:	48 8d 0d 15 63 04 00 	lea    rcx,[rip+0x46315]        # 0x14012b150
   1400e4e3b:	48 8b d7             	mov    rdx,rdi
   1400e4e3e:	e8 e5 79 f7 ff       	call   0x14005c828
   1400e4e43:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   1400e4e48:	4c 8d 05 41 94 05 00 	lea    r8,[rip+0x59441]        # 0x14013e290
   1400e4e4f:	48 8d 0d e2 62 04 00 	lea    rcx,[rip+0x462e2]        # 0x14012b138
   1400e4e56:	48 8b d7             	mov    rdx,rdi
   1400e4e59:	e8 ca 79 f7 ff       	call   0x14005c828
   1400e4e5e:	48 8b c3             	mov    rax,rbx
   1400e4e61:	48 8b 5c 24 38       	mov    rbx,QWORD PTR [rsp+0x38]
   1400e4e66:	48 83 c4 20          	add    rsp,0x20
   1400e4e6a:	5f                   	pop    rdi
   1400e4e6b:	c3                   	ret
   1400e4e6c:	48 8d 05 4d 63 04 00 	lea    rax,[rip+0x4634d]        # 0x14012b1c0
   1400e4e73:	48 89 01             	mov    QWORD PTR [rcx],rax
   1400e4e76:	33 c0                	xor    eax,eax
   1400e4e78:	48 89 05 79 93 05 00 	mov    QWORD PTR [rip+0x59379],rax        # 0x14013e1f8
   1400e4e7f:	48 89 05 7a 93 05 00 	mov    QWORD PTR [rip+0x5937a],rax        # 0x14013e200
   1400e4e86:	48 89 81 78 0a 00 00 	mov    QWORD PTR [rcx+0xa78],rax
   1400e4e8d:	48 8b c1             	mov    rax,rcx
   1400e4e90:	c3                   	ret
   1400e4e91:	cc                   	int3
   1400e4e92:	cc                   	int3
   1400e4e93:	cc                   	int3
   1400e4e94:	48 83 ec 28          	sub    rsp,0x28
   1400e4e98:	48 8d 05 21 63 04 00 	lea    rax,[rip+0x46321]        # 0x14012b1c0
   1400e4e9f:	48 89 01             	mov    QWORD PTR [rcx],rax
   1400e4ea2:	48 8b 0d 4f 93 05 00 	mov    rcx,QWORD PTR [rip+0x5934f]        # 0x14013e1f8
   1400e4ea9:	48 85 c9             	test   rcx,rcx
   1400e4eac:	74 0e                	je     0x1400e4ebc
   1400e4eae:	ff 15 b4 6b 03 00    	call   QWORD PTR [rip+0x36bb4]        # 0x14011ba68
   1400e4eb4:	48 83 25 3c 93 05 00 	and    QWORD PTR [rip+0x5933c],0x0        # 0x14013e1f8
   1400e4ebb:	00 
   1400e4ebc:	48 8b 0d 3d 93 05 00 	mov    rcx,QWORD PTR [rip+0x5933d]        # 0x14013e200
   1400e4ec3:	48 85 c9             	test   rcx,rcx
   1400e4ec6:	74 0e                	je     0x1400e4ed6
   1400e4ec8:	ff 15 9a 6b 03 00    	call   QWORD PTR [rip+0x36b9a]        # 0x14011ba68
   1400e4ece:	48 83 25 2a 93 05 00 	and    QWORD PTR [rip+0x5932a],0x0        # 0x14013e200
   1400e4ed5:	00 
   1400e4ed6:	48 83 c4 28          	add    rsp,0x28
   1400e4eda:	c3                   	ret
   1400e4edb:	cc                   	int3
   1400e4edc:	48 83 ec 28          	sub    rsp,0x28
   1400e4ee0:	45 85 c0             	test   r8d,r8d
   1400e4ee3:	74 10                	je     0x1400e4ef5
   1400e4ee5:	48 8b 0d 14 93 05 00 	mov    rcx,QWORD PTR [rip+0x59314]        # 0x14013e200
   1400e4eec:	44 89 05 15 93 05 00 	mov    DWORD PTR [rip+0x59315],r8d        # 0x14013e208
   1400e4ef3:	eb 14                	jmp    0x1400e4f09
   1400e4ef5:	81 ea 00 20 00 00    	sub    edx,0x2000
   1400e4efb:	74 05                	je     0x1400e4f02
   1400e4efd:	83 fa 01             	cmp    edx,0x1
   1400e4f00:	75 0d                	jne    0x1400e4f0f
   1400e4f02:	48 8b 0d ef 92 05 00 	mov    rcx,QWORD PTR [rip+0x592ef]        # 0x14013e1f8
   1400e4f09:	ff 15 89 6b 03 00    	call   QWORD PTR [rip+0x36b89]        # 0x14011ba98
   1400e4f0f:	48 83 c4 28          	add    rsp,0x28
   1400e4f13:	c3                   	ret
   1400e4f14:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   1400e4f19:	48 89 6c 24 10       	mov    QWORD PTR [rsp+0x10],rbp
   1400e4f1e:	56                   	push   rsi
   1400e4f1f:	57                   	push   rdi
   1400e4f20:	41 54                	push   r12
   1400e4f22:	41 55                	push   r13
   1400e4f24:	41 56                	push   r14
   1400e4f26:	48 83 ec 60          	sub    rsp,0x60
   1400e4f2a:	41 f6 01 08          	test   BYTE PTR [r9],0x8
   1400e4f2e:	49 8b f1             	mov    rsi,r9
   1400e4f31:	49 8b e8             	mov    rbp,r8
   1400e4f34:	4c 8b e1             	mov    r12,rcx
   1400e4f37:	0f 85 c8 01 00 00    	jne    0x1400e5105
   1400e4f3d:	4d 8b 49 08          	mov    r9,QWORD PTR [r9+0x8]
   1400e4f41:	ff 15 79 71 03 00    	call   QWORD PTR [rip+0x37179]        # 0x14011c0c0
   1400e4f47:	48 8b 4e 30          	mov    rcx,QWORD PTR [rsi+0x30]
   1400e4f4b:	45 33 ed             	xor    r13d,r13d
   1400e4f4e:	66 44 39 29          	cmp    WORD PTR [rcx],r13w
   1400e4f52:	0f 84 ad 01 00 00    	je     0x1400e5105
   1400e4f58:	41 8b c5             	mov    eax,r13d
   1400e4f5b:	4c 89 6c 24 50       	mov    QWORD PTR [rsp+0x50],r13
   1400e4f60:	41 8b fd             	mov    edi,r13d
   1400e4f63:	44 89 6c 24 40       	mov    DWORD PTR [rsp+0x40],r13d
   1400e4f68:	41 8d 55 3f          	lea    edx,[r13+0x3f]
   1400e4f6c:	89 84 24 a8 00 00 00 	mov    DWORD PTR [rsp+0xa8],eax
   1400e4f73:	eb 06                	jmp    0x1400e4f7b
   1400e4f75:	ff c7                	inc    edi
   1400e4f77:	48 83 c1 02          	add    rcx,0x2
   1400e4f7b:	48 89 4c 24 48       	mov    QWORD PTR [rsp+0x48],rcx
   1400e4f80:	66 3b 11             	cmp    dx,WORD PTR [rcx]
   1400e4f83:	74 f0                	je     0x1400e4f75
   1400e4f85:	41 be 2a 00 00 00    	mov    r14d,0x2a
   1400e4f8b:	66 44 3b 31          	cmp    r14w,WORD PTR [rcx]
   1400e4f8f:	75 76                	jne    0x1400e5007
   1400e4f91:	eb 02                	jmp    0x1400e4f95
   1400e4f93:	ff c7                	inc    edi
   1400e4f95:	48 83 c1 02          	add    rcx,0x2
   1400e4f99:	48 89 4c 24 48       	mov    QWORD PTR [rsp+0x48],rcx
   1400e4f9e:	66 3b 11             	cmp    dx,WORD PTR [rcx]
   1400e4fa1:	74 f0                	je     0x1400e4f93
   1400e4fa3:	48 8d 54 24 40       	lea    rdx,[rsp+0x40]
   1400e4fa8:	48 8d 4c 24 48       	lea    rcx,[rsp+0x48]
   1400e4fad:	e8 6e 2d f8 ff       	call   0x140067d20
   1400e4fb2:	8b 44 24 40          	mov    eax,DWORD PTR [rsp+0x40]
   1400e4fb6:	48 8b 4c 24 48       	mov    rcx,QWORD PTR [rsp+0x48]
   1400e4fbb:	bb 3a 00 00 00       	mov    ebx,0x3a
   1400e4fc0:	89 84 24 a8 00 00 00 	mov    DWORD PTR [rsp+0xa8],eax
   1400e4fc7:	66 3b 19             	cmp    bx,WORD PTR [rcx]
   1400e4fca:	75 27                	jne    0x1400e4ff3
   1400e4fcc:	48 83 c1 02          	add    rcx,0x2
   1400e4fd0:	48 8d 94 24 a8 00 00 	lea    rdx,[rsp+0xa8]
   1400e4fd7:	00 
   1400e4fd8:	48 89 4c 24 48       	mov    QWORD PTR [rsp+0x48],rcx
   1400e4fdd:	48 8d 4c 24 48       	lea    rcx,[rsp+0x48]
   1400e4fe2:	e8 39 2d f8 ff       	call   0x140067d20
   1400e4fe7:	48 8b 4c 24 48       	mov    rcx,QWORD PTR [rsp+0x48]
   1400e4fec:	8b 84 24 a8 00 00 00 	mov    eax,DWORD PTR [rsp+0xa8]
   1400e4ff3:	66 44 3b 31          	cmp    r14w,WORD PTR [rcx]
   1400e4ff7:	74 05                	je     0x1400e4ffe
   1400e4ff9:	66 3b 19             	cmp    bx,WORD PTR [rcx]
   1400e4ffc:	75 09                	jne    0x1400e5007
   1400e4ffe:	48 83 c1 02          	add    rcx,0x2
   1400e5002:	48 89 4c 24 48       	mov    QWORD PTR [rsp+0x48],rcx
   1400e5007:	41 3b fd             	cmp    edi,r13d
   1400e500a:	0f 84 ab 00 00 00    	je     0x1400e50bb
   1400e5010:	e8 47 9a f7 ff       	call   0x14005ea5c
   1400e5015:	48 c1 e8 10          	shr    rax,0x10
   1400e5019:	3d 00 00 06 00       	cmp    eax,0x60000
   1400e501e:	72 62                	jb     0x1400e5082
   1400e5020:	48 8b 0d 11 92 05 00 	mov    rcx,QWORD PTR [rip+0x59211]        # 0x14013e238
   1400e5027:	49 3b cd             	cmp    rcx,r13
   1400e502a:	75 24                	jne    0x1400e5050
   1400e502c:	4c 8d 05 05 92 05 00 	lea    r8,[rip+0x59205]        # 0x14013e238
   1400e5033:	48 8d 15 36 14 04 00 	lea    rdx,[rip+0x41436]        # 0x140126470
   1400e503a:	48 8d 0d 87 61 04 00 	lea    rcx,[rip+0x46187]        # 0x14012b1c8
   1400e5041:	45 33 c9             	xor    r9d,r9d
   1400e5044:	e8 4f 78 f7 ff       	call   0x14005c898
   1400e5049:	48 8b 0d e8 91 05 00 	mov    rcx,QWORD PTR [rip+0x591e8]        # 0x14013e238
   1400e5050:	41 8b dd             	mov    ebx,r13d
   1400e5053:	48 8d 41 ff          	lea    rax,[rcx-0x1]
   1400e5057:	48 83 f8 fe          	cmp    rax,0xfffffffffffffffe
   1400e505b:	0f 92 c3             	setb   bl
   1400e505e:	41 3b dd             	cmp    ebx,r13d
   1400e5061:	74 0b                	je     0x1400e506e
   1400e5063:	ff                   	.byte 0xff
