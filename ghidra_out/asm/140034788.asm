
PECMD.exe：     文件格式 pei-x86-64


Disassembly of section .text:

0000000140034788 <.text+0x33788>:
   140034788:	48 8b c4             	mov    rax,rsp
   14003478b:	4c 89 48 20          	mov    QWORD PTR [rax+0x20],r9
   14003478f:	44 88 40 18          	mov    BYTE PTR [rax+0x18],r8b
   140034793:	48 89 50 10          	mov    QWORD PTR [rax+0x10],rdx
   140034797:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
   14003479b:	53                   	push   rbx
   14003479c:	55                   	push   rbp
   14003479d:	56                   	push   rsi
   14003479e:	57                   	push   rdi
   14003479f:	41 54                	push   r12
   1400347a1:	41 55                	push   r13
   1400347a3:	41 56                	push   r14
   1400347a5:	41 57                	push   r15
   1400347a7:	48 81 ec e8 00 00 00 	sub    rsp,0xe8
   1400347ae:	33 ff                	xor    edi,edi
   1400347b0:	4c 8b e1             	mov    r12,rcx
   1400347b3:	c6 44 24 46 21       	mov    BYTE PTR [rsp+0x46],0x21
   1400347b8:	44 8d 4f 23          	lea    r9d,[rdi+0x23]
   1400347bc:	44 8d 47 2d          	lea    r8d,[rdi+0x2d]
   1400347c0:	40 88 7c 24 47       	mov    BYTE PTR [rsp+0x47],dil
   1400347c5:	b3 2c                	mov    bl,0x2c
   1400347c7:	eb 66                	jmp    0x14003482f
   1400347c9:	b9 7c 00 00 00       	mov    ecx,0x7c
   1400347ce:	66 3b c8             	cmp    cx,ax
   1400347d1:	75 08                	jne    0x1400347db
   1400347d3:	8a 02                	mov    al,BYTE PTR [rdx]
   1400347d5:	88 44 24 47          	mov    BYTE PTR [rsp+0x47],al
   1400347d9:	eb 48                	jmp    0x140034823
   1400347db:	b9 5e 00 00 00       	mov    ecx,0x5e
   1400347e0:	66 3b c8             	cmp    cx,ax
   1400347e3:	74 38                	je     0x14003481d
   1400347e5:	66 44 3b c8          	cmp    r9w,ax
   1400347e9:	74 32                	je     0x14003481d
   1400347eb:	b9 7e 00 00 00       	mov    ecx,0x7e
   1400347f0:	66 3b c8             	cmp    cx,ax
   1400347f3:	74 28                	je     0x14003481d
   1400347f5:	b9 2b 00 00 00       	mov    ecx,0x2b
   1400347fa:	66 3b c8             	cmp    cx,ax
   1400347fd:	74 1e                	je     0x14003481d
   1400347ff:	66 44 3b c0          	cmp    r8w,ax
   140034803:	74 18                	je     0x14003481d
   140034805:	b9 3b 00 00 00       	mov    ecx,0x3b
   14003480a:	66 3b c8             	cmp    cx,ax
   14003480d:	74 0a                	je     0x140034819
   14003480f:	b9 3a 00 00 00       	mov    ecx,0x3a
   140034814:	66 3b c8             	cmp    cx,ax
   140034817:	75 1e                	jne    0x140034837
   140034819:	8a 1a                	mov    bl,BYTE PTR [rdx]
   14003481b:	eb 06                	jmp    0x140034823
   14003481d:	8a 02                	mov    al,BYTE PTR [rdx]
   14003481f:	88 44 24 46          	mov    BYTE PTR [rsp+0x46],al
   140034823:	48 83 c2 02          	add    rdx,0x2
   140034827:	48 89 94 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rdx
   14003482e:	00 
   14003482f:	0f b7 02             	movzx  eax,WORD PTR [rdx]
   140034832:	66 3b c7             	cmp    ax,di
   140034835:	75 92                	jne    0x1400347c9
   140034837:	b9 0d 00 00 00       	mov    ecx,0xd
   14003483c:	8b f7                	mov    esi,edi
   14003483e:	89 7c 24 68          	mov    DWORD PTR [rsp+0x68],edi
   140034842:	44 8d 71 1d          	lea    r14d,[rcx+0x1d]
   140034846:	8b ef                	mov    ebp,edi
   140034848:	89 7c 24 78          	mov    DWORD PTR [rsp+0x78],edi
   14003484c:	44 8d 69 fc          	lea    r13d,[rcx-0x4]
   140034850:	44 8d 79 13          	lea    r15d,[rcx+0x13]
   140034854:	66 44 3b 32          	cmp    r14w,WORD PTR [rdx]
   140034858:	0f 85 dc 00 00 00    	jne    0x14003493a
   14003485e:	44 8b e1             	mov    r12d,ecx
   140034861:	48 8d 0d b8 ce 0e 00 	lea    rcx,[rip+0xeceb8]        # 0x140121720
   140034868:	41 b8 02 00 00 00    	mov    r8d,0x2
   14003486e:	e8 15 7f 02 00       	call   0x14005c788
   140034873:	40 3a c7             	cmp    al,dil
   140034876:	74 06                	je     0x14003487e
   140034878:	0f ba ee 10          	bts    esi,0x10
   14003487c:	eb 48                	jmp    0x1400348c6
   14003487e:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   140034885:	00 
   140034886:	48 8d 0d 8f ce 0e 00 	lea    rcx,[rip+0xece8f]        # 0x14012171c
   14003488d:	41 b8 03 00 00 00    	mov    r8d,0x3
   140034893:	e8 f0 7e 02 00       	call   0x14005c788
   140034898:	40 3a c7             	cmp    al,dil
   14003489b:	74 06                	je     0x1400348a3
   14003489d:	0f ba ed 11          	bts    ebp,0x11
   1400348a1:	eb 23                	jmp    0x1400348c6
   1400348a3:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   1400348aa:	00 
   1400348ab:	48 8d 0d 62 ce 0e 00 	lea    rcx,[rip+0xece62]        # 0x140121714
   1400348b2:	41 b8 04 00 00 00    	mov    r8d,0x4
   1400348b8:	e8 cb 7e 02 00       	call   0x14005c788
   1400348bd:	40 3a c7             	cmp    al,dil
   1400348c0:	74 04                	je     0x1400348c6
   1400348c2:	0f ba ee 12          	bts    esi,0x12
   1400348c6:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   1400348cd:	00 
   1400348ce:	eb 17                	jmp    0x1400348e7
   1400348d0:	66 3b c7             	cmp    ax,di
   1400348d3:	74 27                	je     0x1400348fc
   1400348d5:	66 44 3b e8          	cmp    r13w,ax
   1400348d9:	77 06                	ja     0x1400348e1
   1400348db:	66 41 3b c4          	cmp    ax,r12w
   1400348df:	76 1b                	jbe    0x1400348fc
   1400348e1:	66 44 3b f8          	cmp    r15w,ax
   1400348e5:	74 15                	je     0x1400348fc
   1400348e7:	48 83 c2 02          	add    rdx,0x2
   1400348eb:	48 89 94 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rdx
   1400348f2:	00 
   1400348f3:	0f b7 02             	movzx  eax,WORD PTR [rdx]
   1400348f6:	66 44 3b f0          	cmp    r14w,ax
   1400348fa:	75 d4                	jne    0x1400348d0
   1400348fc:	66 44 3b 32          	cmp    r14w,WORD PTR [rdx]
   140034900:	0f 84 5b ff ff ff    	je     0x140034861
   140034906:	4c 8b a4 24 30 01 00 	mov    r12,QWORD PTR [rsp+0x130]
   14003490d:	00 
   14003490e:	89 6c 24 78          	mov    DWORD PTR [rsp+0x78],ebp
   140034912:	89 74 24 68          	mov    DWORD PTR [rsp+0x68],esi
   140034916:	b9 0d 00 00 00       	mov    ecx,0xd
   14003491b:	eb 1d                	jmp    0x14003493a
   14003491d:	66 44 3b e8          	cmp    r13w,ax
   140034921:	77 05                	ja     0x140034928
   140034923:	66 3b c1             	cmp    ax,cx
   140034926:	76 1a                	jbe    0x140034942
   140034928:	66 44 3b f8          	cmp    r15w,ax
   14003492c:	74 14                	je     0x140034942
   14003492e:	48 83 c2 02          	add    rdx,0x2
   140034932:	48 89 94 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rdx
   140034939:	00 
   14003493a:	0f b7 02             	movzx  eax,WORD PTR [rdx]
   14003493d:	66 3b c7             	cmp    ax,di
   140034940:	75 db                	jne    0x14003491d
   140034942:	48 8d 8c 24 38 01 00 	lea    rcx,[rsp+0x138]
   140034949:	00 
   14003494a:	e8 61 27 fe ff       	call   0x1400170b0
   14003494f:	48 8b 8c 24 38 01 00 	mov    rcx,QWORD PTR [rsp+0x138]
   140034956:	00 
   140034957:	66 39 39             	cmp    WORD PTR [rcx],di
   14003495a:	75 07                	jne    0x140034963
   14003495c:	33 c0                	xor    eax,eax
   14003495e:	e9 e9 0e 00 00       	jmp    0x14003584c
   140034963:	0f be c3             	movsx  eax,bl
   140034966:	44 8a ff             	mov    r15b,dil
   140034969:	8b f7                	mov    esi,edi
   14003496b:	0f b7 d0             	movzx  edx,ax
   14003496e:	44 8b ef             	mov    r13d,edi
   140034971:	44 8a f7             	mov    r14b,dil
   140034974:	44 89 7c 24 58       	mov    DWORD PTR [rsp+0x58],r15d
   140034979:	40 88 7c 24 45       	mov    BYTE PTR [rsp+0x45],dil
   14003497e:	40 88 7c 24 44       	mov    BYTE PTR [rsp+0x44],dil
   140034983:	48 89 bc 24 d8 00 00 	mov    QWORD PTR [rsp+0xd8],rdi
   14003498a:	00 
   14003498b:	89 7c 24 6c          	mov    DWORD PTR [rsp+0x6c],edi
   14003498f:	89 bc 24 b4 00 00 00 	mov    DWORD PTR [rsp+0xb4],edi
   140034996:	c7 44 24 48 00 00 00 	mov    DWORD PTR [rsp+0x48],0x80000000
   14003499d:	80 
   14003499e:	89 bc 24 b0 00 00 00 	mov    DWORD PTR [rsp+0xb0],edi
   1400349a5:	48 89 bc 24 90 00 00 	mov    QWORD PTR [rsp+0x90],rdi
   1400349ac:	00 
   1400349ad:	48 89 bc 24 a0 00 00 	mov    QWORD PTR [rsp+0xa0],rdi
   1400349b4:	00 
   1400349b5:	66 89 84 24 98 00 00 	mov    WORD PTR [rsp+0x98],ax
   1400349bc:	00 
   1400349bd:	ff 15 bd 72 0e 00    	call   QWORD PTR [rip+0xe72bd]        # 0x14011bc80
   1400349c3:	4c 8b d8             	mov    r11,rax
   1400349c6:	48 89 44 24 50       	mov    QWORD PTR [rsp+0x50],rax
   1400349cb:	48 3b c7             	cmp    rax,rdi
   1400349ce:	74 11                	je     0x1400349e1
   1400349d0:	66 89 38             	mov    WORD PTR [rax],di
   1400349d3:	4c 8b 5c 24 50       	mov    r11,QWORD PTR [rsp+0x50]
   1400349d8:	49 83 c3 02          	add    r11,0x2
   1400349dc:	4c 89 5c 24 50       	mov    QWORD PTR [rsp+0x50],r11
   1400349e1:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   1400349e8:	00 
   1400349e9:	bd 01 00 00 00       	mov    ebp,0x1
   1400349ee:	8b df                	mov    ebx,edi
   1400349f0:	8d 4d 2c             	lea    ecx,[rbp+0x2c]
   1400349f3:	48 89 bc 24 c0 00 00 	mov    QWORD PTR [rsp+0xc0],rdi
   1400349fa:	00 
   1400349fb:	89 5c 24 40          	mov    DWORD PTR [rsp+0x40],ebx
   1400349ff:	66 39 0a             	cmp    WORD PTR [rdx],cx
   140034a02:	0f 85 28 0a 00 00    	jne    0x140035430
   140034a08:	66 39 4a 02          	cmp    WORD PTR [rdx+0x2],cx
   140034a0c:	75 30                	jne    0x140034a3e
   140034a0e:	4c 8d 84 24 c0 00 00 	lea    r8,[rsp+0xc0]
   140034a15:	00 
   140034a16:	45 33 c9             	xor    r9d,r9d
   140034a19:	49 8b cc             	mov    rcx,r12
   140034a1c:	40 88 6c 24 20       	mov    BYTE PTR [rsp+0x20],bpl
   140034a21:	e8 1e 75 04 00       	call   0x14007bf44
   140034a26:	4c 8b 9c 24 c0 00 00 	mov    r11,QWORD PTR [rsp+0xc0]
   140034a2d:	00 
   140034a2e:	4c 89 9c 24 38 01 00 	mov    QWORD PTR [rsp+0x138],r11
   140034a35:	00 
   140034a36:	4c 8b 5c 24 50       	mov    r11,QWORD PTR [rsp+0x50]
   140034a3b:	8d 4d 2c             	lea    ecx,[rbp+0x2c]
   140034a3e:	48 8b 84 24 38 01 00 	mov    rax,QWORD PTR [rsp+0x138]
   140034a45:	00 
   140034a46:	66 39 08             	cmp    WORD PTR [rax],cx
   140034a49:	0f 85 e1 09 00 00    	jne    0x140035430
   140034a4f:	41 bc 57 00 07 80    	mov    r12d,0x80070057
   140034a55:	48 8b 84 24 38 01 00 	mov    rax,QWORD PTR [rsp+0x138]
   140034a5c:	00 
   140034a5d:	66 39 48 02          	cmp    WORD PTR [rax+0x2],cx
   140034a61:	0f 85 ff 05 00 00    	jne    0x140035066
   140034a67:	48 8d 94 24 38 01 00 	lea    rdx,[rsp+0x138]
   140034a6e:	00 
   140034a6f:	48 8d 0d 96 cc 0e 00 	lea    rcx,[rip+0xecc96]        # 0x14012170c
   140034a76:	41 b8 05 00 00 00    	mov    r8d,0x5
   140034a7c:	e8 2b 16 03 00       	call   0x1400660ac
   140034a81:	40 3a c7             	cmp    al,dil
   140034a84:	74 0a                	je     0x140034a90
   140034a86:	40 88 6c 24 45       	mov    BYTE PTR [rsp+0x45],bpl
   140034a8b:	e9 ba 05 00 00       	jmp    0x14003504a
   140034a90:	48 8d 94 24 38 01 00 	lea    rdx,[rsp+0x138]
   140034a97:	00 
   140034a98:	48 8d 0d 65 cc 0e 00 	lea    rcx,[rip+0xecc65]        # 0x140121704
   140034a9f:	41 b8 06 00 00 00    	mov    r8d,0x6
   140034aa5:	e8 02 16 03 00       	call   0x1400660ac
   140034aaa:	40 3a c7             	cmp    al,dil
   140034aad:	74 0e                	je     0x140034abd
   140034aaf:	bb 00 00 08 00       	mov    ebx,0x80000
   140034ab4:	89 5c 24 40          	mov    DWORD PTR [rsp+0x40],ebx
   140034ab8:	e9 91 05 00 00       	jmp    0x14003504e
   140034abd:	bb 09 00 00 00       	mov    ebx,0x9
   140034ac2:	48 8d 94 24 38 01 00 	lea    rdx,[rsp+0x138]
   140034ac9:	00 
   140034aca:	48 8d 0d 27 cc 0e 00 	lea    rcx,[rip+0xecc27]        # 0x1401216f8
   140034ad1:	44 8b c3             	mov    r8d,ebx
   140034ad4:	e8 d3 15 03 00       	call   0x1400660ac
   140034ad9:	40 3a c7             	cmp    al,dil
   140034adc:	74 0a                	je     0x140034ae8
   140034ade:	c6 44 24 44 06       	mov    BYTE PTR [rsp+0x44],0x6
   140034ae3:	e9 62 05 00 00       	jmp    0x14003504a
   140034ae8:	48 8d 94 24 38 01 00 	lea    rdx,[rsp+0x138]
   140034aef:	00 
   140034af0:	48 8d 0d f1 cb 0e 00 	lea    rcx,[rip+0xecbf1]        # 0x1401216e8
   140034af7:	41 b8 0b 00 00 00    	mov    r8d,0xb
   140034afd:	e8 aa 15 03 00       	call   0x1400660ac
   140034b02:	40 3a c7             	cmp    al,dil
   140034b05:	74 0a                	je     0x140034b11
   140034b07:	c6 44 24 44 04       	mov    BYTE PTR [rsp+0x44],0x4
   140034b0c:	e9 39 05 00 00       	jmp    0x14003504a
   140034b11:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   140034b18:	00 
   140034b19:	48 8d 0d b8 cb 0e 00 	lea    rcx,[rip+0xecbb8]        # 0x1401216d8
   140034b20:	41 b8 08 00 00 00    	mov    r8d,0x8
   140034b26:	e8 5d 7c 02 00       	call   0x14005c788
   140034b2b:	40 3a c7             	cmp    al,dil
   140034b2e:	74 4c                	je     0x140034b7c
   140034b30:	48 8b 84 24 38 01 00 	mov    rax,QWORD PTR [rsp+0x138]
   140034b37:	00 
   140034b38:	48 8d 94 24 80 00 00 	lea    rdx,[rsp+0x80]
   140034b3f:	00 
   140034b40:	48 8d 8c 24 a0 00 00 	lea    rcx,[rsp+0xa0]
   140034b47:	00 
   140034b48:	48 83 c0 10          	add    rax,0x10
   140034b4c:	41 b8 cd 01 00 00    	mov    r8d,0x1cd
   140034b52:	48 89 bc 24 80 00 00 	mov    QWORD PTR [rsp+0x80],rdi
   140034b59:	00 
   140034b5a:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034b61:	00 
   140034b62:	48 89 84 24 a0 00 00 	mov    QWORD PTR [rsp+0xa0],rax
   140034b69:	00 
   140034b6a:	e8 d9 00 ff ff       	call   0x140024c48
   140034b6f:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034b76:	00 
   140034b77:	e9 ce 04 00 00       	jmp    0x14003504a
   140034b7c:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   140034b83:	00 
   140034b84:	48 8d 0d 41 cb 0e 00 	lea    rcx,[rip+0xecb41]        # 0x1401216cc
   140034b8b:	41 b8 05 00 00 00    	mov    r8d,0x5
   140034b91:	e8 f2 7b 02 00       	call   0x14005c788
   140034b96:	40 3a c7             	cmp    al,dil
   140034b99:	0f 84 1a 01 00 00    	je     0x140034cb9
   140034b9f:	48 8b 84 24 38 01 00 	mov    rax,QWORD PTR [rsp+0x138]
   140034ba6:	00 
   140034ba7:	44 8b ed             	mov    r13d,ebp
   140034baa:	48 83 c0 0a          	add    rax,0xa
   140034bae:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034bb5:	00 
   140034bb6:	48 89 44 24 58       	mov    QWORD PTR [rsp+0x58],rax
   140034bbb:	4c 8b c0             	mov    r8,rax
   140034bbe:	0f b7 08             	movzx  ecx,WORD PTR [rax]
   140034bc1:	66 3b cf             	cmp    cx,di
   140034bc4:	74 33                	je     0x140034bf9
   140034bc6:	ba 20 00 00 00       	mov    edx,0x20
   140034bcb:	44 8d 62 ed          	lea    r12d,[rdx-0x13]
   140034bcf:	66 3b d9             	cmp    bx,cx
   140034bd2:	77 06                	ja     0x140034bda
   140034bd4:	66 41 3b cc          	cmp    cx,r12w
   140034bd8:	76 19                	jbe    0x140034bf3
   140034bda:	66 3b d1             	cmp    dx,cx
   140034bdd:	74 14                	je     0x140034bf3
   140034bdf:	48 83 c0 02          	add    rax,0x2
   140034be3:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034bea:	00 
   140034beb:	66 8b 08             	mov    cx,WORD PTR [rax]
   140034bee:	66 3b cf             	cmp    cx,di
   140034bf1:	75 dc                	jne    0x140034bcf
   140034bf3:	41 bc 57 00 07 80    	mov    r12d,0x80070057
   140034bf9:	48 8d 8c 24 38 01 00 	lea    rcx,[rsp+0x138]
   140034c00:	00 
   140034c01:	e8 aa 24 fe ff       	call   0x1400170b0
   140034c06:	b8 2a 00 00 00       	mov    eax,0x2a
   140034c0b:	66 41 39 00          	cmp    WORD PTR [r8],ax
   140034c0f:	75 0c                	jne    0x140034c1d
   140034c11:	44 0a f5             	or     r14b,bpl
   140034c14:	49 83 c0 02          	add    r8,0x2
   140034c18:	4c 89 44 24 58       	mov    QWORD PTR [rsp+0x58],r8
   140034c1d:	66 41 83 38 40       	cmp    WORD PTR [r8],0x40
   140034c22:	75 0c                	jne    0x140034c30
   140034c24:	45 8a 38             	mov    r15b,BYTE PTR [r8]
   140034c27:	49 83 c0 02          	add    r8,0x2
   140034c2b:	4c 89 44 24 58       	mov    QWORD PTR [rsp+0x58],r8
   140034c30:	66 41 83 38 2e       	cmp    WORD PTR [r8],0x2e
   140034c35:	75 0d                	jne    0x140034c44
   140034c37:	41 80 ce 02          	or     r14b,0x2
   140034c3b:	49 83 c0 02          	add    r8,0x2
   140034c3f:	4c 89 44 24 58       	mov    QWORD PTR [rsp+0x58],r8
   140034c44:	83 4c 24 70 ff       	or     DWORD PTR [rsp+0x70],0xffffffff
   140034c49:	b8 23 00 00 00       	mov    eax,0x23
   140034c4e:	40 8a df             	mov    bl,dil
   140034c51:	66 41 39 00          	cmp    WORD PTR [r8],ax
   140034c55:	75 0c                	jne    0x140034c63
   140034c57:	41 8a 18             	mov    bl,BYTE PTR [r8]
   140034c5a:	49 83 c0 02          	add    r8,0x2
   140034c5e:	4c 89 44 24 58       	mov    QWORD PTR [rsp+0x58],r8
   140034c63:	41 0f b7 00          	movzx  eax,WORD PTR [r8]
   140034c67:	b9 09 00 00 00       	mov    ecx,0x9
   140034c6c:	66 83 e8 30          	sub    ax,0x30
   140034c70:	66 3b c1             	cmp    ax,cx
   140034c73:	76 0d                	jbe    0x140034c82
   140034c75:	8d 41 1f             	lea    eax,[rcx+0x1f]
   140034c78:	66 41 3b 00          	cmp    ax,WORD PTR [r8]
   140034c7c:	0f 85 c8 03 00 00    	jne    0x14003504a
   140034c82:	48 8d 54 24 70       	lea    rdx,[rsp+0x70]
   140034c87:	48 8d 4c 24 58       	lea    rcx,[rsp+0x58]
   140034c8c:	e8 2f c5 08 00       	call   0x1400c11c0
   140034c91:	8b 44 24 70          	mov    eax,DWORD PTR [rsp+0x70]
   140034c95:	3b c7                	cmp    eax,edi
   140034c97:	41 0f 4e f4          	cmovle esi,r12d
   140034c9b:	40 3a df             	cmp    bl,dil
   140034c9e:	8b 5c 24 40          	mov    ebx,DWORD PTR [rsp+0x40]
   140034ca2:	74 0c                	je     0x140034cb0
   140034ca4:	89 84 24 b0 00 00 00 	mov    DWORD PTR [rsp+0xb0],eax
   140034cab:	e9 9e 03 00 00       	jmp    0x14003504e
   140034cb0:	89 44 24 6c          	mov    DWORD PTR [rsp+0x6c],eax
   140034cb4:	e9 95 03 00 00       	jmp    0x14003504e
   140034cb9:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   140034cc0:	00 
   140034cc1:	48 8d 0d fc c9 0e 00 	lea    rcx,[rip+0xec9fc]        # 0x1401216c4
   140034cc8:	41 b8 06 00 00 00    	mov    r8d,0x6
   140034cce:	e8 b5 7a 02 00       	call   0x14005c788
   140034cd3:	40 3a c7             	cmp    al,dil
   140034cd6:	0f 84 c1 00 00 00    	je     0x140034d9d
   140034cdc:	48 8b 84 24 38 01 00 	mov    rax,QWORD PTR [rsp+0x138]
   140034ce3:	00 
   140034ce4:	48 83 c0 0c          	add    rax,0xc
   140034ce8:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034cef:	00 
   140034cf0:	48 89 84 24 88 00 00 	mov    QWORD PTR [rsp+0x88],rax
   140034cf7:	00 
   140034cf8:	4c 8b c0             	mov    r8,rax
   140034cfb:	0f b7 08             	movzx  ecx,WORD PTR [rax]
   140034cfe:	66 3b cf             	cmp    cx,di
   140034d01:	74 33                	je     0x140034d36
   140034d03:	ba 20 00 00 00       	mov    edx,0x20
   140034d08:	44 8d 62 ed          	lea    r12d,[rdx-0x13]
   140034d0c:	66 3b d9             	cmp    bx,cx
   140034d0f:	77 06                	ja     0x140034d17
   140034d11:	66 41 3b cc          	cmp    cx,r12w
   140034d15:	76 19                	jbe    0x140034d30
   140034d17:	66 3b d1             	cmp    dx,cx
   140034d1a:	74 14                	je     0x140034d30
   140034d1c:	48 83 c0 02          	add    rax,0x2
   140034d20:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034d27:	00 
   140034d28:	66 8b 08             	mov    cx,WORD PTR [rax]
   140034d2b:	66 3b cf             	cmp    cx,di
   140034d2e:	75 dc                	jne    0x140034d0c
   140034d30:	41 bc 57 00 07 80    	mov    r12d,0x80070057
   140034d36:	48 8d 8c 24 38 01 00 	lea    rcx,[rsp+0x138]
   140034d3d:	00 
   140034d3e:	e8 6d 23 fe ff       	call   0x1400170b0
   140034d43:	b8 23 00 00 00       	mov    eax,0x23
   140034d48:	44 8d 68 df          	lea    r13d,[rax-0x21]
   140034d4c:	66 41 39 00          	cmp    WORD PTR [r8],ax
   140034d50:	0f 85 f4 02 00 00    	jne    0x14003504a
   140034d56:	4d 03 c5             	add    r8,r13
   140034d59:	48 8d 54 24 48       	lea    rdx,[rsp+0x48]
   140034d5e:	48 8d 8c 24 88 00 00 	lea    rcx,[rsp+0x88]
   140034d65:	00 
   140034d66:	4c 89 84 24 88 00 00 	mov    QWORD PTR [rsp+0x88],r8
   140034d6d:	00 
   140034d6e:	44 8d 40 09          	lea    r8d,[rax+0x9]
   140034d72:	89 7c 24 48          	mov    DWORD PTR [rsp+0x48],edi
   140034d76:	e8 35 2c 03 00       	call   0x1400679b0
   140034d7b:	8b 5c 24 40          	mov    ebx,DWORD PTR [rsp+0x40]
   140034d7f:	3b c7                	cmp    eax,edi
   140034d81:	7e 0e                	jle    0x140034d91
   140034d83:	0f bf 44 24 48       	movsx  eax,WORD PTR [rsp+0x48]
   140034d88:	89 44 24 48          	mov    DWORD PTR [rsp+0x48],eax
   140034d8c:	e9 bd 02 00 00       	jmp    0x14003504e
   140034d91:	44 8a fd             	mov    r15b,bpl
   140034d94:	89 6c 24 48          	mov    DWORD PTR [rsp+0x48],ebp
   140034d98:	e9 b1 02 00 00       	jmp    0x14003504e
   140034d9d:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   140034da4:	00 
   140034da5:	48 8d 0d 10 c9 0e 00 	lea    rcx,[rip+0xec910]        # 0x1401216bc
   140034dac:	41 b8 05 00 00 00    	mov    r8d,0x5
   140034db2:	e8 d1 79 02 00       	call   0x14005c788
   140034db7:	40 3a c7             	cmp    al,dil
   140034dba:	0f 84 16 01 00 00    	je     0x140034ed6
   140034dc0:	48 8b 84 24 38 01 00 	mov    rax,QWORD PTR [rsp+0x138]
   140034dc7:	00 
   140034dc8:	48 83 c0 0a          	add    rax,0xa
   140034dcc:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034dd3:	00 
   140034dd4:	48 89 84 24 c8 00 00 	mov    QWORD PTR [rsp+0xc8],rax
   140034ddb:	00 
   140034ddc:	4c 8b c0             	mov    r8,rax
   140034ddf:	0f b7 08             	movzx  ecx,WORD PTR [rax]
   140034de2:	66 3b cf             	cmp    cx,di
   140034de5:	74 33                	je     0x140034e1a
   140034de7:	ba 20 00 00 00       	mov    edx,0x20
   140034dec:	44 8d 62 ed          	lea    r12d,[rdx-0x13]
   140034df0:	66 3b d9             	cmp    bx,cx
   140034df3:	77 06                	ja     0x140034dfb
   140034df5:	66 41 3b cc          	cmp    cx,r12w
   140034df9:	76 19                	jbe    0x140034e14
   140034dfb:	66 3b d1             	cmp    dx,cx
   140034dfe:	74 14                	je     0x140034e14
   140034e00:	48 83 c0 02          	add    rax,0x2
   140034e04:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034e0b:	00 
   140034e0c:	66 8b 08             	mov    cx,WORD PTR [rax]
   140034e0f:	66 3b cf             	cmp    cx,di
   140034e12:	75 dc                	jne    0x140034df0
   140034e14:	41 bc 57 00 07 80    	mov    r12d,0x80070057
   140034e1a:	48 8d 8c 24 38 01 00 	lea    rcx,[rsp+0x138]
   140034e21:	00 
   140034e22:	e8 89 22 fe ff       	call   0x1400170b0
   140034e27:	89 bc 24 b8 00 00 00 	mov    DWORD PTR [rsp+0xb8],edi
   140034e2e:	41 0f b7 00          	movzx  eax,WORD PTR [r8]
   140034e32:	41 bd 10 00 00 00    	mov    r13d,0x10
   140034e38:	40 8a df             	mov    bl,dil
   140034e3b:	66 3b c7             	cmp    ax,di
   140034e3e:	74 46                	je     0x140034e86
   140034e40:	41 8d 4d 1a          	lea    ecx,[r13+0x1a]
   140034e44:	45 8d 65 13          	lea    r12d,[r13+0x13]
   140034e48:	66 3b c1             	cmp    ax,cx
   140034e4b:	75 05                	jne    0x140034e52
   140034e4d:	45 8a 30             	mov    r14b,BYTE PTR [r8]
   140034e50:	eb 14                	jmp    0x140034e66
   140034e52:	66 83 f8 40          	cmp    ax,0x40
   140034e56:	75 05                	jne    0x140034e5d
   140034e58:	45 8a 38             	mov    r15b,BYTE PTR [r8]
   140034e5b:	eb 09                	jmp    0x140034e66
   140034e5d:	66 41 3b c4          	cmp    ax,r12w
   140034e61:	75 18                	jne    0x140034e7b
   140034e63:	41 8a 18             	mov    bl,BYTE PTR [r8]
   140034e66:	49 83 c0 02          	add    r8,0x2
   140034e6a:	4c 89 84 24 c8 00 00 	mov    QWORD PTR [rsp+0xc8],r8
   140034e71:	00 
   140034e72:	66 41 8b 00          	mov    ax,WORD PTR [r8]
   140034e76:	66 3b c7             	cmp    ax,di
   140034e79:	75 cd                	jne    0x140034e48
   140034e7b:	44 89 7c 24 58       	mov    DWORD PTR [rsp+0x58],r15d
   140034e80:	41 bc 57 00 07 80    	mov    r12d,0x80070057
   140034e86:	48 8d 94 24 b8 00 00 	lea    rdx,[rsp+0xb8]
   140034e8d:	00 
   140034e8e:	48 8d 8c 24 c8 00 00 	lea    rcx,[rsp+0xc8]
   140034e95:	00 
   140034e96:	e8 9d f9 03 00       	call   0x140074838
   140034e9b:	48 63 8c 24 b8 00 00 	movsxd rcx,DWORD PTR [rsp+0xb8]
   140034ea2:	00 
   140034ea3:	3b c7                	cmp    eax,edi
   140034ea5:	7e 04                	jle    0x140034eab
   140034ea7:	3b cf                	cmp    ecx,edi
   140034ea9:	7e 09                	jle    0x140034eb4
   140034eab:	40 3a df             	cmp    bl,dil
   140034eae:	74 19                	je     0x140034ec9
   140034eb0:	3b c7                	cmp    eax,edi
   140034eb2:	7f 03                	jg     0x140034eb7
   140034eb4:	41 8b f4             	mov    esi,r12d
   140034eb7:	40 3a df             	cmp    bl,dil
   140034eba:	74 0d                	je     0x140034ec9
   140034ebc:	48 89 8c 24 d8 00 00 	mov    QWORD PTR [rsp+0xd8],rcx
   140034ec3:	00 
   140034ec4:	e9 81 01 00 00       	jmp    0x14003504a
   140034ec9:	48 89 8c 24 90 00 00 	mov    QWORD PTR [rsp+0x90],rcx
   140034ed0:	00 
   140034ed1:	e9 74 01 00 00       	jmp    0x14003504a
   140034ed6:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   140034edd:	00 
   140034ede:	48 8d 0d cb c7 0e 00 	lea    rcx,[rip+0xec7cb]        # 0x1401216b0
   140034ee5:	44 8b c3             	mov    r8d,ebx
   140034ee8:	e8 9b 78 02 00       	call   0x14005c788
   140034eed:	40 3a c7             	cmp    al,dil
   140034ef0:	74 7b                	je     0x140034f6d
   140034ef2:	48 8b 84 24 38 01 00 	mov    rax,QWORD PTR [rsp+0x138]
   140034ef9:	00 
   140034efa:	48 83 c0 12          	add    rax,0x12
   140034efe:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034f05:	00 
   140034f06:	48 89 84 24 80 00 00 	mov    QWORD PTR [rsp+0x80],rax
   140034f0d:	00 
   140034f0e:	0f b7 08             	movzx  ecx,WORD PTR [rax]
   140034f11:	66 3b cf             	cmp    cx,di
   140034f14:	74 33                	je     0x140034f49
   140034f16:	ba 20 00 00 00       	mov    edx,0x20
   140034f1b:	44 8d 62 ed          	lea    r12d,[rdx-0x13]
   140034f1f:	66 3b d9             	cmp    bx,cx
   140034f22:	77 06                	ja     0x140034f2a
   140034f24:	66 41 3b cc          	cmp    cx,r12w
   140034f28:	76 19                	jbe    0x140034f43
   140034f2a:	66 3b d1             	cmp    dx,cx
   140034f2d:	74 14                	je     0x140034f43
   140034f2f:	48 83 c0 02          	add    rax,0x2
   140034f33:	48 89 84 24 38 01 00 	mov    QWORD PTR [rsp+0x138],rax
   140034f3a:	00 
   140034f3b:	66 8b 08             	mov    cx,WORD PTR [rax]
   140034f3e:	66 3b cf             	cmp    cx,di
   140034f41:	75 dc                	jne    0x140034f1f
   140034f43:	41 bc 57 00 07 80    	mov    r12d,0x80070057
   140034f49:	48 8d 8c 24 38 01 00 	lea    rcx,[rsp+0x138]
   140034f50:	00 
   140034f51:	e8 5a 21 fe ff       	call   0x1400170b0
   140034f56:	48 8d 54 24 6c       	lea    rdx,[rsp+0x6c]
   140034f5b:	48 8d 8c 24 80 00 00 	lea    rcx,[rsp+0x80]
   140034f62:	00 
   140034f63:	e8 d0 f8 03 00       	call   0x140074838
   140034f68:	e9 dd 00 00 00       	jmp    0x14003504a
   140034f6d:	48 8b 94 24 38 01 00 	mov    rdx,QWORD PTR [rsp+0x138]
   140034f74:	00 
   140034f75:	48 8d 0d 24 c7 0e 00 	lea    rcx,[rip+0xec724]        # 0x1401216a0
   140034f7c:	44 8b c3             	mov    r8d,ebx
   140034f7f:	e8 04 78 02 00       	call   0x14005c788
   140034f84:	40 3a c7             	cmp    al,dil
   140034f87:	74                   	.byte 0x74
