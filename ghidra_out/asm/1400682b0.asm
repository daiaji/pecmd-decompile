
PECMD.exe：     文件格式 pei-x86-64


Disassembly of section .text:

00000001400682b0 <.text+0x672b0>:
   1400682b0:	4c 8b dc             	mov    r11,rsp
   1400682b3:	49 89 5b 08          	mov    QWORD PTR [r11+0x8],rbx
   1400682b7:	49 89 6b 18          	mov    QWORD PTR [r11+0x18],rbp
   1400682bb:	49 89 73 20          	mov    QWORD PTR [r11+0x20],rsi
   1400682bf:	57                   	push   rdi
   1400682c0:	41 54                	push   r12
   1400682c2:	41 55                	push   r13
   1400682c4:	41 56                	push   r14
   1400682c6:	41 57                	push   r15
   1400682c8:	48 81 ec 30 01 00 00 	sub    rsp,0x130
   1400682cf:	41 8a f1             	mov    sil,r9b
   1400682d2:	41 8a c1             	mov    al,r9b
   1400682d5:	45 8a e9             	mov    r13b,r9b
   1400682d8:	45 33 f6             	xor    r14d,r14d
   1400682db:	24 02                	and    al,0x2
   1400682dd:	41 80 e5 0c          	and    r13b,0xc
   1400682e1:	40 80 e6 01          	and    sil,0x1
   1400682e5:	48 8b fa             	mov    rdi,rdx
   1400682e8:	4c 8b e1             	mov    r12,rcx
   1400682eb:	66 44 39 32          	cmp    WORD PTR [rdx],r14w
   1400682ef:	0f 84 27 01 00 00    	je     0x14006841c
   1400682f5:	33 c9                	xor    ecx,ecx
   1400682f7:	66 44 89 74 24 60    	mov    WORD PTR [rsp+0x60],r14w
   1400682fd:	66 44 89 74 24 50    	mov    WORD PTR [rsp+0x50],r14w
   140068303:	49 8b d8             	mov    rbx,r8
   140068306:	45 89 73 10          	mov    DWORD PTR [r11+0x10],r14d
   14006830a:	48 89 4c 24 52       	mov    QWORD PTR [rsp+0x52],rcx
   14006830f:	89 4c 24 5a          	mov    DWORD PTR [rsp+0x5a],ecx
   140068313:	66 89 4c 24 5e       	mov    WORD PTR [rsp+0x5e],cx
   140068318:	41 89 4b 14          	mov    DWORD PTR [r11+0x14],ecx
   14006831c:	41 3a c6             	cmp    al,r14b
   14006831f:	74 11                	je     0x140068332
   140068321:	49 8d 53 10          	lea    rdx,[r11+0x10]
   140068325:	49 8b c8             	mov    rcx,r8
   140068328:	49 8d 5b 10          	lea    rbx,[r11+0x10]
   14006832c:	ff 15 de 32 0b 00    	call   QWORD PTR [rip+0xb32de]        # 0x14011b610
   140068332:	48 0f be ee          	movsx  rbp,sil
   140068336:	41 bf 2a 00 00 00    	mov    r15d,0x2a
   14006833c:	66 44 3b 3c 6f       	cmp    r15w,WORD PTR [rdi+rbp*2]
   140068341:	74 15                	je     0x140068358
   140068343:	48 8d 54 24 50       	lea    rdx,[rsp+0x50]
   140068348:	48 8b cb             	mov    rcx,rbx
   14006834b:	ff 15 07 32 0b 00    	call   QWORD PTR [rip+0xb3207]        # 0x14011b558
   140068351:	66 44 3b 3c 6f       	cmp    r15w,WORD PTR [rdi+rbp*2]
   140068356:	75 23                	jne    0x14006837b
   140068358:	48 83 c7 02          	add    rdi,0x2
   14006835c:	41 3a f6             	cmp    sil,r14b
   14006835f:	74 08                	je     0x140068369
   140068361:	b8 26 00 00 00       	mov    eax,0x26
   140068366:	66 89 07             	mov    WORD PTR [rdi],ax
   140068369:	48 8b 13             	mov    rdx,QWORD PTR [rbx]
   14006836c:	48 8d 4c 24 60       	lea    rcx,[rsp+0x60]
   140068371:	e8 fe e9 07 00       	call   0x1400e6d74
   140068376:	e9 91 00 00 00       	jmp    0x14006840c
   14006837b:	0f b7 4c 24 5e       	movzx  ecx,WORD PTR [rsp+0x5e]
   140068380:	0f b7 54 24 5c       	movzx  edx,WORD PTR [rsp+0x5c]
   140068385:	0f b7 44 24 54       	movzx  eax,WORD PTR [rsp+0x54]
   14006838a:	44 0f b7 54 24 5a    	movzx  r10d,WORD PTR [rsp+0x5a]
   140068390:	44 0f b7 5c 24 58    	movzx  r11d,WORD PTR [rsp+0x58]
   140068396:	0f b7 5c 24 56       	movzx  ebx,WORD PTR [rsp+0x56]
   14006839b:	44 0f b7 4c 24 52    	movzx  r9d,WORD PTR [rsp+0x52]
   1400683a1:	44 0f b7 44 24 50    	movzx  r8d,WORD PTR [rsp+0x50]
   1400683a7:	89 44 24 48          	mov    DWORD PTR [rsp+0x48],eax
   1400683ab:	89 4c 24 40          	mov    DWORD PTR [rsp+0x40],ecx
   1400683af:	89 54 24 38          	mov    DWORD PTR [rsp+0x38],edx
   1400683b3:	44 89 54 24 30       	mov    DWORD PTR [rsp+0x30],r10d
   1400683b8:	44 89 5c 24 28       	mov    DWORD PTR [rsp+0x28],r11d
   1400683bd:	48 8d 15 2c e1 0b 00 	lea    rdx,[rip+0xbe12c]        # 0x1401264f0
   1400683c4:	48 8d 4c 24 60       	lea    rcx,[rsp+0x60]
   1400683c9:	89 5c 24 20          	mov    DWORD PTR [rsp+0x20],ebx
   1400683cd:	ff 15 65 3d 0b 00    	call   QWORD PTR [rip+0xb3d65]        # 0x14011c138
   1400683d3:	45 3a ee             	cmp    r13b,r14b
   1400683d6:	74 34                	je     0x14006840c
   1400683d8:	48 8d 4c 24 60       	lea    rcx,[rsp+0x60]
   1400683dd:	ff 15 c5 36 0b 00    	call   QWORD PTR [rip+0xb36c5]        # 0x14011baa8
   1400683e3:	41 0f be d5          	movsx  edx,r13b
   1400683e7:	48 8d 4c 24 50       	lea    rcx,[rsp+0x50]
   1400683ec:	83 e2 08             	and    edx,0x8
   1400683ef:	48 63 d8             	movsxd rbx,eax
   1400683f2:	e8 e9 8a ff ff       	call   0x140060ee0
   1400683f7:	48 8d 4c 5c 60       	lea    rcx,[rsp+rbx*2+0x60]
   1400683fc:	48 8d 15 dd e0 0b 00 	lea    rdx,[rip+0xbe0dd]        # 0x1401264e0
   140068403:	44 8b c0             	mov    r8d,eax
   140068406:	ff 15 2c 3d 0b 00    	call   QWORD PTR [rip+0xb3d2c]        # 0x14011c138
   14006840c:	4c 8d 44 24 60       	lea    r8,[rsp+0x60]
   140068411:	48 8b d7             	mov    rdx,rdi
   140068414:	49 8b cc             	mov    rcx,r12
   140068417:	e8 9c a5 ff ff       	call   0x1400629b8
   14006841c:	4c 8d 9c 24 30 01 00 	lea    r11,[rsp+0x130]
   140068423:	00 
   140068424:	49 8b 5b 30          	mov    rbx,QWORD PTR [r11+0x30]
   140068428:	49 8b 6b 40          	mov    rbp,QWORD PTR [r11+0x40]
   14006842c:	49 8b 73 48          	mov    rsi,QWORD PTR [r11+0x48]
   140068430:	49 8b e3             	mov    rsp,r11
   140068433:	41 5f                	pop    r15
   140068435:	41 5e                	pop    r14
   140068437:	41 5d                	pop    r13
   140068439:	41 5c                	pop    r12
   14006843b:	5f                   	pop    rdi
   14006843c:	c3                   	ret
   14006843d:	cc                   	int3
   14006843e:	cc                   	int3
   14006843f:	cc                   	int3
   140068440:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   140068445:	48 89 74 24 10       	mov    QWORD PTR [rsp+0x10],rsi
   14006844a:	57                   	push   rdi
   14006844b:	48 83 ec 20          	sub    rsp,0x20
   14006844f:	33 f6                	xor    esi,esi
   140068451:	48 83 fa ff          	cmp    rdx,0xffffffffffffffff
   140068455:	48 8b f9             	mov    rdi,rcx
   140068458:	48 0f 44 d6          	cmove  rdx,rsi
   14006845c:	48 89 11             	mov    QWORD PTR [rcx],rdx
   14006845f:	48 83 c1 08          	add    rcx,0x8
   140068463:	e8 dc ae ff ff       	call   0x140063344
   140068468:	48 89 77 10          	mov    QWORD PTR [rdi+0x10],rsi
   14006846c:	48 89 77 18          	mov    QWORD PTR [rdi+0x18],rsi
   140068470:	48 8d 4f 08          	lea    rcx,[rdi+0x8]
   140068474:	ba 00 10 00 00       	mov    edx,0x1000
   140068479:	c7 47 24 10 27 00 00 	mov    DWORD PTR [rdi+0x24],0x2710
   140068480:	40 88 77 20          	mov    BYTE PTR [rdi+0x20],sil
   140068484:	e8 9b af ff ff       	call   0x140063424
   140068489:	48 8b 5c 24 30       	mov    rbx,QWORD PTR [rsp+0x30]
   14006848e:	48 8b 74 24 38       	mov    rsi,QWORD PTR [rsp+0x38]
   140068493:	48 8b c7             	mov    rax,rdi
   140068496:	48 83 c4 20          	add    rsp,0x20
   14006849a:	5f                   	pop    rdi
   14006849b:	c3                   	ret
   14006849c:	40 53                	rex push rbx
   14006849e:	48 83 ec 30          	sub    rsp,0x30
   1400684a2:	80 79 20 00          	cmp    BYTE PTR [rcx+0x20],0x0
   1400684a6:	48 8b d9             	mov    rbx,rcx
   1400684a9:	74 23                	je     0x1400684ce
   1400684ab:	48 83 79 10 00       	cmp    QWORD PTR [rcx+0x10],0x0
   1400684b0:	7e 1c                	jle    0x1400684ce
   1400684b2:	44 8b 41 10          	mov    r8d,DWORD PTR [rcx+0x10]
   1400684b6:	48 8b 51 08          	mov    rdx,QWORD PTR [rcx+0x8]
   1400684ba:	48 8b 09             	mov    rcx,QWORD PTR [rcx]
   1400684bd:	48 83 64 24 20 00    	and    QWORD PTR [rsp+0x20],0x0
   1400684c3:	4c 8d 4c 24 40       	lea    r9,[rsp+0x40]
   1400684c8:	ff 15 5a 33 0b 00    	call   QWORD PTR [rip+0xb335a]        # 0x14011b828
   1400684ce:	48 83 63 10 00       	and    QWORD PTR [rbx+0x10],0x0
   1400684d3:	48 8d 4b 08          	lea    rcx,[rbx+0x8]
   1400684d7:	e8 28 2c ff ff       	call   0x14005b104
   1400684dc:	48 8b 0b             	mov    rcx,QWORD PTR [rbx]
   1400684df:	48 85 c9             	test   rcx,rcx
   1400684e2:	74 0c                	je     0x1400684f0
   1400684e4:	48 83 f9 ff          	cmp    rcx,0xffffffffffffffff
   1400684e8:	74 06                	je     0x1400684f0
   1400684ea:	ff 15 78 35 0b 00    	call   QWORD PTR [rip+0xb3578]        # 0x14011ba68
   1400684f0:	48 83 23 00          	and    QWORD PTR [rbx],0x0
   1400684f4:	48 83 c4 30          	add    rsp,0x30
   1400684f8:	5b                   	pop    rbx
   1400684f9:	c3                   	ret
   1400684fa:	cc                   	int3
   1400684fb:	cc                   	int3
   1400684fc:	48 83 ec 28          	sub    rsp,0x28
   140068500:	44 8b 51 34          	mov    r10d,DWORD PTR [rcx+0x34]
   140068504:	48 83 c1 08          	add    rcx,0x8
   140068508:	47 8d 4c 12 03       	lea    r9d,[r10+r10*1+0x3]
   14006850d:	44 0f af 49 28       	imul   r9d,DWORD PTR [rcx+0x28]
   140068512:	45 8b c1             	mov    r8d,r9d
   140068515:	41 8b c1             	mov    eax,r9d
   140068518:	41 c1 e8 15          	shr    r8d,0x15
   14006851c:	c1 e8 0e             	shr    eax,0xe
   14006851f:	41 80 e0 7e          	and    r8b,0x7e
   140068523:	44 32 c0             	xor    r8b,al
   140068526:	41 8b c1             	mov    eax,r9d
   140068529:	c1 e8 07             	shr    eax,0x7
   14006852c:	44 32 c0             	xor    r8b,al
   14006852f:	41 8d 42 01          	lea    eax,[r10+0x1]
   140068533:	45 32 c1             	xor    r8b,r9b
   140068536:	89 41 2c             	mov    DWORD PTR [rcx+0x2c],eax
   140068539:	41 32 d0             	xor    dl,r8b
   14006853c:	41 b8 01 00 00 00    	mov    r8d,0x1
   140068542:	88 54 24 38          	mov    BYTE PTR [rsp+0x38],dl
   140068546:	48 8d 54 24 38       	lea    rdx,[rsp+0x38]
   14006854b:	e8 08 6c ff ff       	call   0x14005f158
   140068550:	48 83 c4 28          	add    rsp,0x28
   140068554:	c3                   	ret
   140068555:	cc                   	int3
   140068556:	cc                   	int3
   140068557:	cc                   	int3
   140068558:	88 54 24 10          	mov    BYTE PTR [rsp+0x10],dl
   14006855c:	48 83 ec 28          	sub    rsp,0x28
   140068560:	48 8d 54 24 38       	lea    rdx,[rsp+0x38]
   140068565:	48 83 c1 08          	add    rcx,0x8
   140068569:	41 b8 01 00 00 00    	mov    r8d,0x1
   14006856f:	e8 e4 6b ff ff       	call   0x14005f158
   140068574:	48 83 c4 28          	add    rsp,0x28
   140068578:	c3                   	ret
   140068579:	cc                   	int3
   14006857a:	cc                   	int3
   14006857b:	cc                   	int3
   14006857c:	48 8b c4             	mov    rax,rsp
   14006857f:	53                   	push   rbx
   140068580:	48 83 ec 20          	sub    rsp,0x20
   140068584:	83 60 10 00          	and    DWORD PTR [rax+0x10],0x0
   140068588:	48 8b d9             	mov    rbx,rcx
   14006858b:	4c 8d 48 10          	lea    r9,[rax+0x10]
   14006858f:	48 8d 50 08          	lea    rdx,[rax+0x8]
   140068593:	48 83 c1 08          	add    rcx,0x8
   140068597:	41 b8 01 00 00 00    	mov    r8d,0x1
   14006859d:	c6 40 08 00          	mov    BYTE PTR [rax+0x8],0x0
   1400685a1:	e8 0a 6c ff ff       	call   0x14005f1b0
   1400685a6:	85 c0                	test   eax,eax
   1400685a8:	74 3e                	je     0x1400685e8
   1400685aa:	83 7c 24 38 01       	cmp    DWORD PTR [rsp+0x38],0x1
   1400685af:	72 37                	jb     0x1400685e8
   1400685b1:	44 8b 43 34          	mov    r8d,DWORD PTR [rbx+0x34]
   1400685b5:	43 8d 4c 00 03       	lea    ecx,[r8+r8*1+0x3]
   1400685ba:	0f af 4b 30          	imul   ecx,DWORD PTR [rbx+0x30]
   1400685be:	8b d1                	mov    edx,ecx
   1400685c0:	8b c1                	mov    eax,ecx
   1400685c2:	c1 e8 0e             	shr    eax,0xe
   1400685c5:	c1 ea 15             	shr    edx,0x15
   1400685c8:	80 e2 7e             	and    dl,0x7e
   1400685cb:	32 d0                	xor    dl,al
   1400685cd:	8b c1                	mov    eax,ecx
   1400685cf:	c1 e8 07             	shr    eax,0x7
   1400685d2:	32 d0                	xor    dl,al
   1400685d4:	41 8d 40 01          	lea    eax,[r8+0x1]
   1400685d8:	32 d1                	xor    dl,cl
   1400685da:	8a 4c 24 30          	mov    cl,BYTE PTR [rsp+0x30]
   1400685de:	89 43 34             	mov    DWORD PTR [rbx+0x34],eax
   1400685e1:	32 ca                	xor    cl,dl
   1400685e3:	0f b6 c1             	movzx  eax,cl
   1400685e6:	eb 03                	jmp    0x1400685eb
   1400685e8:	83 c8 ff             	or     eax,0xffffffff
   1400685eb:	48 83 c4 20          	add    rsp,0x20
   1400685ef:	5b                   	pop    rbx
   1400685f0:	c3                   	ret
   1400685f1:	cc                   	int3
   1400685f2:	cc                   	int3
   1400685f3:	cc                   	int3
   1400685f4:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   1400685f9:	48 89 6c 24 10       	mov    QWORD PTR [rsp+0x10],rbp
   1400685fe:	56                   	push   rsi
   1400685ff:	57                   	push   rdi
   140068600:	41 54                	push   r12
   140068602:	41 55                	push   r13
   140068604:	41 56                	push   r14
   140068606:	48 83 ec 30          	sub    rsp,0x30
   14006860a:	41 8b e8             	mov    ebp,r8d
   14006860d:	4c 8b ea             	mov    r13,rdx
   140068610:	4c 8b e1             	mov    r12,rcx
   140068613:	83 e5 01             	and    ebp,0x1
   140068616:	e8 21 e3 07 00       	call   0x1400e693c
   14006861b:	33 db                	xor    ebx,ebx
   14006861d:	3b c3                	cmp    eax,ebx
   14006861f:	48 8b f0             	mov    rsi,rax
   140068622:	0f 8e ec 01 00 00    	jle    0x140068814
   140068628:	83 c0 0c             	add    eax,0xc
   14006862b:	49 8b cd             	mov    rcx,r13
   14006862e:	48 63 d0             	movsxd rdx,eax
   140068631:	e8 ea b0 ff ff       	call   0x140063720
   140068636:	49 8b 7d 00          	mov    rdi,QWORD PTR [r13+0x0]
   14006863a:	83 4c 24 70 ff       	or     DWORD PTR [rsp+0x70],0xffffffff
   14006863f:	4c 8d 4c 24 70       	lea    r9,[rsp+0x70]
   140068644:	48 8b d7             	mov    rdx,rdi
   140068647:	44 8b c6             	mov    r8d,esi
   14006864a:	49 8b cc             	mov    rcx,r12
   14006864d:	48 89 5c 24 20       	mov    QWORD PTR [rsp+0x20],rbx
   140068652:	ff 15 00 34 0b 00    	call   QWORD PTR [rip+0xb3400]        # 0x14011ba58
   140068658:	48 63 74 24 70       	movsxd rsi,DWORD PTR [rsp+0x70]
   14006865d:	3b f3                	cmp    esi,ebx
   14006865f:	0f 8e af 01 00 00    	jle    0x140068814
   140068665:	3b c3                	cmp    eax,ebx
   140068667:	0f 84 a7 01 00 00    	je     0x140068814
   14006866d:	48 8d 0c 37          	lea    rcx,[rdi+rsi*1]
   140068671:	44 8d 43 0a          	lea    r8d,[rbx+0xa]
   140068675:	33 d2                	xor    edx,edx
   140068677:	e8 14 a4 09 00       	call   0x140102a90
   14006867c:	44 8d 66 0a          	lea    r12d,[rsi+0xa]
   140068680:	44 8d 73 02          	lea    r14d,[rbx+0x2]
   140068684:	48 8d 15 a1 ba 0b 00 	lea    rdx,[rip+0xbbaa1]        # 0x14012412c
   14006868b:	4d 8b c6             	mov    r8,r14
   14006868e:	48 8b cf             	mov    rcx,rdi
   140068691:	e8 ee 2a ff ff       	call   0x14005b184
   140068696:	3b c3                	cmp    eax,ebx
   140068698:	75 0a                	jne    0x1400686a4
   14006869a:	8b d6                	mov    edx,esi
   14006869c:	48 8b cf             	mov    rcx,rdi
   14006869f:	e8 d0 83 ff ff       	call   0x140060a74
   1400686a4:	48 8d 15 7d ba 0b 00 	lea    rdx,[rip+0xbba7d]        # 0x140124128
   1400686ab:	4d 8b c6             	mov    r8,r14
   1400686ae:	48 8b cf             	mov    rcx,rdi
   1400686b1:	e8 ce 2a ff ff       	call   0x14005b184
   1400686b6:	8b cb                	mov    ecx,ebx
   1400686b8:	3b c3                	cmp    eax,ebx
   1400686ba:	0f 94 c1             	sete   cl
   1400686bd:	38 1f                	cmp    BYTE PTR [rdi],bl
   1400686bf:	74 29                	je     0x1400686ea
   1400686c1:	38 5f 01             	cmp    BYTE PTR [rdi+0x1],bl
   1400686c4:	75 24                	jne    0x1400686ea
   1400686c6:	48 8d 4f 02          	lea    rcx,[rdi+0x2]
   1400686ca:	4d 63 c4             	movsxd r8,r12d
   1400686cd:	48 8b d7             	mov    rdx,rdi
   1400686d0:	e8 6f 50 fb ff       	call   0x14001d744
   1400686d5:	41 bb ff fe 00 00    	mov    r11d,0xfeff
   1400686db:	41 03 f6             	add    esi,r14d
   1400686de:	45 03 e6             	add    r12d,r14d
   1400686e1:	b9 01 00 00 00       	mov    ecx,0x1
   1400686e6:	66 44 89 1f          	mov    WORD PTR [rdi],r11w
   1400686ea:	3b cb                	cmp    ecx,ebx
   1400686ec:	0f 85 1a 01 00 00    	jne    0x14006880c
   1400686f2:	38 1f                	cmp    BYTE PTR [rdi],bl
   1400686f4:	75 29                	jne    0x14006871f
   1400686f6:	38 5f 01             	cmp    BYTE PTR [rdi+0x1],bl
   1400686f9:	74 24                	je     0x14006871f
   1400686fb:	48 8d 4f 02          	lea    rcx,[rdi+0x2]
   1400686ff:	4d 63 c4             	movsxd r8,r12d
   140068702:	48 8b d7             	mov    rdx,rdi
   140068705:	e8 3a 50 fb ff       	call   0x14001d744
   14006870a:	41 bb fe ff 00 00    	mov    r11d,0xfffe
   140068710:	41 03 f6             	add    esi,r14d
   140068713:	66 44 89 1f          	mov    WORD PTR [rdi],r11w
   140068717:	45 03 e6             	add    r12d,r14d
   14006871a:	e9 65 ff ff ff       	jmp    0x140068684
   14006871f:	49 8b 45 00          	mov    rax,QWORD PTR [r13+0x0]
   140068723:	48 63 d6             	movsxd rdx,esi
   140068726:	49 8b cd             	mov    rcx,r13
   140068729:	48 89 44 24 78       	mov    QWORD PTR [rsp+0x78],rax
   14006872e:	49 89 5d 00          	mov    QWORD PTR [r13+0x0],rbx
   140068732:	e8 e9 af ff ff       	call   0x140063720
   140068737:	0f b7 05 ea b9 0b 00 	movzx  eax,WORD PTR [rip+0xbb9ea]        # 0x140124128
   14006873e:	4d 8b 5d 00          	mov    r11,QWORD PTR [r13+0x0]
   140068742:	41 ba 03 00 00 00    	mov    r10d,0x3
   140068748:	48 8d 15 e1 b9 0b 00 	lea    rdx,[rip+0xbb9e1]        # 0x140124130
   14006874f:	4d 8b c2             	mov    r8,r10
   140068752:	48 8b cf             	mov    rcx,rdi
   140068755:	66 41 89 03          	mov    WORD PTR [r11],ax
   140068759:	e8 26 2a ff ff       	call   0x14005b184
   14006875e:	3b c3                	cmp    eax,ebx
   140068760:	74 07                	je     0x140068769
   140068762:	3b eb                	cmp    ebp,ebx
   140068764:	74 41                	je     0x1400687a7
   140068766:	44 8b d3             	mov    r10d,ebx
   140068769:	49 8b 4d 00          	mov    rcx,QWORD PTR [r13+0x0]
   14006876d:	8d 46 01             	lea    eax,[rsi+0x1]
   140068770:	44 8b ce             	mov    r9d,esi
   140068773:	49 03 ce             	add    rcx,r14
   140068776:	4d 63 c2             	movsxd r8,r10d
   140068779:	89 44 24 28          	mov    DWORD PTR [rsp+0x28],eax
   14006877d:	48 89 4c 24 20       	mov    QWORD PTR [rsp+0x20],rcx
   140068782:	45 2b ca             	sub    r9d,r10d
   140068785:	ba 08 00 00 00       	mov    edx,0x8
   14006878a:	b9 e9 fd 00 00       	mov    ecx,0xfde9
   14006878f:	4c 03 c7             	add    r8,rdi
   140068792:	ff 15 d8 41 0d 00    	call   QWORD PTR [rip+0xd41d8]        # 0x14013c970
   140068798:	4c 63 e0             	movsxd r12,eax
   14006879b:	3b eb                	cmp    ebp,ebx
   14006879d:	74 54                	je     0x1400687f3
   14006879f:	4c 3b e3             	cmp    r12,rbx
   1400687a2:	7f 4f                	jg     0x1400687f3
   1400687a4:	41 0b ee             	or     ebp,r14d
   1400687a7:	49 8b 4d 00          	mov    rcx,QWORD PTR [r13+0x0]
   1400687ab:	8d 46 01             	lea    eax,[rsi+0x1]
   1400687ae:	44 8b ce             	mov    r9d,esi
   1400687b1:	49 03 ce             	add    rcx,r14
   1400687b4:	89 44 24 28          	mov    DWORD PTR [rsp+0x28],eax
   1400687b8:	4c 8b c7             	mov    r8,rdi
   1400687bb:	48 89 4c 24 20       	mov    QWORD PTR [rsp+0x20],rcx
   1400687c0:	33 c9                	xor    ecx,ecx
   1400687c2:	ba 08 00 00 00       	mov    edx,0x8
   1400687c7:	ff 15 a3 41 0d 00    	call   QWORD PTR [rip+0xd41a3]        # 0x14013c970
   1400687cd:	44 8b d3             	mov    r10d,ebx
   1400687d0:	4c 63 e0             	movsxd r12,eax
   1400687d3:	4c 3b e3             	cmp    r12,rbx
   1400687d6:	7f 04                	jg     0x1400687dc
   1400687d8:	3b eb                	cmp    ebp,ebx
   1400687da:	74 8d                	je     0x140068769
   1400687dc:	48 8d 4c 24 78       	lea    rcx,[rsp+0x78]
   1400687e1:	e8 1e 29 ff ff       	call   0x14005b104
   1400687e6:	41 8d 4c 24 01       	lea    ecx,[r12+0x1]
   1400687eb:	4c 3b e3             	cmp    r12,rbx
   1400687ee:	0f 4f d9             	cmovg  ebx,ecx
   1400687f1:	eb 15                	jmp    0x140068808
   1400687f3:	48 8d 4c 24 78       	lea    rcx,[rsp+0x78]
   1400687f8:	e8 07 29 ff ff       	call   0x14005b104
   1400687fd:	41 8d 44 24 01       	lea    eax,[r12+0x1]
   140068802:	4c 3b e3             	cmp    r12,rbx
   140068805:	0f 4f d8             	cmovg  ebx,eax
   140068808:	8b c3                	mov    eax,ebx
   14006880a:	eb 0a                	jmp    0x140068816
   14006880c:	48 63 c6             	movsxd rax,esi
   14006880f:	48 d1 e8             	shr    rax,1
   140068812:	eb 02                	jmp    0x140068816
   140068814:	33 c0                	xor    eax,eax
   140068816:	48 8b 5c 24 60       	mov    rbx,QWORD PTR [rsp+0x60]
   14006881b:	48 8b 6c 24 68       	mov    rbp,QWORD PTR [rsp+0x68]
   140068820:	48 83 c4 30          	add    rsp,0x30
   140068824:	41 5e                	pop    r14
   140068826:	41 5d                	pop    r13
   140068828:	41 5c                	pop    r12
   14006882a:	5f                   	pop    rdi
   14006882b:	5e                   	pop    rsi
   14006882c:	c3                   	ret
   14006882d:	cc                   	int3
   14006882e:	cc                   	int3
   14006882f:	cc                   	int3
   140068830:	4c 8b 51 10          	mov    r10,QWORD PTR [rcx+0x10]
   140068834:	4c 8b c9             	mov    r9,rcx
   140068837:	4c 3b 51 18          	cmp    r10,QWORD PTR [rcx+0x18]
   14006883b:	7d 3c                	jge    0x140068879
   14006883d:	48 8b 41 08          	mov    rax,QWORD PTR [rcx+0x8]
   140068841:	47 8d 44 12 03       	lea    r8d,[r10+r10*1+0x3]
   140068846:	44 0f af 41 20       	imul   r8d,DWORD PTR [rcx+0x20]
   14006884b:	42 8a 0c 10          	mov    cl,BYTE PTR [rax+r10*1]
   14006884f:	41 8b c0             	mov    eax,r8d
   140068852:	41 8b d0             	mov    edx,r8d
   140068855:	c1 e8 0e             	shr    eax,0xe
   140068858:	c1 ea 15             	shr    edx,0x15
   14006885b:	80 e2 7e             	and    dl,0x7e
   14006885e:	32 ca                	xor    cl,dl
   140068860:	32 c8                	xor    cl,al
   140068862:	41 8b c0             	mov    eax,r8d
   140068865:	c1 e8 07             	shr    eax,0x7
   140068868:	32 c8                	xor    cl,al
   14006886a:	49 8d 42 01          	lea    rax,[r10+0x1]
   14006886e:	41 32 c8             	xor    cl,r8b
   140068871:	49 89 41 10          	mov    QWORD PTR [r9+0x10],rax
   140068875:	0f b6 c1             	movzx  eax,cl
   140068878:	c3                   	ret
   140068879:	83 c8 ff             	or     eax,0xffffffff
   14006887c:	c3                   	ret
   14006887d:	cc                   	int3
   14006887e:	cc                   	int3
   14006887f:	cc                   	int3
   140068880:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   140068885:	57                   	push   rdi
   140068886:	48 83 ec 20          	sub    rsp,0x20
   14006888a:	48 8b 41 10          	mov    rax,QWORD PTR [rcx+0x10]
   14006888e:	4d 63 c0             	movsxd r8,r8d
   140068891:	40 8a fa             	mov    dil,dl
   140068894:	48 8b 51 18          	mov    rdx,QWORD PTR [rcx+0x18]
   140068898:	4d 8d 44 00 03       	lea    r8,[r8+rax*1+0x3]
   14006889d:	48 8b d9             	mov    rbx,rcx
   1400688a0:	4c 3b c2             	cmp    r8,rdx
   1400688a3:	7c 18                	jl     0x1400688bd
   1400688a5:	48 81 c2 00 04 00 00 	add    rdx,0x400
   1400688ac:	48 83 c1 08          	add    rcx,0x8
   1400688b0:	48 89 51 10          	mov    QWORD PTR [rcx+0x10],rdx
   1400688b4:	48 83 c2 04          	add    rdx,0x4
   1400688b8:	e8 67 ab ff ff       	call   0x140063424
   1400688bd:	48 8b 43 10          	mov    rax,QWORD PTR [rbx+0x10]
   1400688c1:	8a 53 24             	mov    dl,BYTE PTR [rbx+0x24]
   1400688c4:	48 8b 4b 08          	mov    rcx,QWORD PTR [rbx+0x8]
   1400688c8:	40 32 d7             	xor    dl,dil
   1400688cb:	88 14 01             	mov    BYTE PTR [rcx+rax*1],dl
   1400688ce:	48 ff 43 10          	inc    QWORD PTR [rbx+0x10]
   1400688d2:	48 8b 5c 24 30       	mov    rbx,QWORD PTR [rsp+0x30]
   1400688d7:	33 c0                	xor    eax,eax
   1400688d9:	48 83 c4 20          	add    rsp,0x20
   1400688dd:	5f                   	pop    rdi
   1400688de:	c3                   	ret
   1400688df:	cc                   	int3
   1400688e0:	48 8b c4             	mov    rax,rsp
   1400688e3:	57                   	push   rdi
   1400688e4:	48 83 ec 60          	sub    rsp,0x60
   1400688e8:	33 ff                	xor    edi,edi
   1400688ea:	48 8b d1             	mov    rdx,rcx
   1400688ed:	48 8d 48 18          	lea    rcx,[rax+0x18]
   1400688f1:	48 89 78 d0          	mov    QWORD PTR [rax-0x30],rdi
   1400688f5:	89 78 c8             	mov    DWORD PTR [rax-0x38],edi
   1400688f8:	c7 40 c0 03 00 00 00 	mov    DWORD PTR [rax-0x40],0x3
   1400688ff:	44 8d 4f 07          	lea    r9d,[rdi+0x7]
   140068903:	41 b8 00 00 00 80    	mov    r8d,0x80000000
   140068909:	40 88 78 d8          	mov    BYTE PTR [rax-0x28],dil
   14006890d:	89 78 10             	mov    DWORD PTR [rax+0x10],edi
   140068910:	48 89 78 18          	mov    QWORD PTR [rax+0x18],rdi
   140068914:	48 89 78 b8          	mov    QWORD PTR [rax-0x48],rdi
   140068918:	e8 47 af f9 ff       	call   0x140003864
   14006891d:	48 39 bc 24 80 00 00 	cmp    QWORD PTR [rsp+0x80],rdi
   140068924:	00 
   140068925:	75 04                	jne    0x14006892b
   140068927:	32 c0                	xor    al,al
   140068929:	eb 53                	jmp    0x14006897e
   14006892b:	48 8b 8c 24 80 00 00 	mov    rcx,QWORD PTR [rsp+0x80]
   140068932:	00 
   140068933:	4c 8d 4c 24 78       	lea    r9,[rsp+0x78]
   140068938:	48 8d 54 24 40       	lea    rdx,[rsp+0x40]
   14006893d:	41 b8 20 00 00 00    	mov    r8d,0x20
   140068943:	48 89 7c 24 20       	mov    QWORD PTR [rsp+0x20],rdi
   140068948:	ff 15 0a 31 0b 00    	call   QWORD PTR [rip+0xb310a]        # 0x14011ba58
   14006894e:	8b 54 24 78          	mov    edx,DWORD PTR [rsp+0x78]
   140068952:	48 8d 4c 24 40       	lea    rcx,[rsp+0x40]
   140068957:	45 33 c0             	xor    r8d,r8d
   14006895a:	e8 dd 69 ff ff       	call   0x14005f33c
   14006895f:	48 83 bc 24 80 00 00 	cmp    QWORD PTR [rsp+0x80],0xffffffffffffffff
   140068966:	00 ff 
   140068968:	40 8a f8             	mov    dil,al
   14006896b:	74 0e                	je     0x14006897b
   14006896d:	48 8b 8c 24 80 00 00 	mov    rcx,QWORD PTR [rsp+0x80]
   140068974:	00 
   140068975:	ff 15 ed 30 0b 00    	call   QWORD PTR [rip+0xb30ed]        # 0x14011ba68
   14006897b:	40 8a c7             	mov    al,dil
   14006897e:	48 83 c4 60          	add    rsp,0x60
   140068982:	5f                   	pop    rdi
   140068983:	c3                   	ret
   140068984:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   140068989:	48 89 6c 24 10       	mov    QWORD PTR [rsp+0x10],rbp
   14006898e:	48 89 74 24 18       	mov    QWORD PTR [rsp+0x18],rsi
   140068993:	57                   	push   rdi
   140068994:	41 54                	push   r12
   140068996:	41 55                	push   r13
   140068998:	41 56                	push   r14
   14006899a:	41 57                	push   r15
   14006899c:	b8 c0 d4 00 00       	mov    eax,0xd4c0
   1400689a1:	e8 fa 16 0b 00       	call   0x14011a0a0
   1400689a6:	48 2b e0             	sub    rsp,rax
   1400689a9:	4c 8b ea             	mov    r13,rdx
   1400689ac:	8b 51 08             	mov    edx,DWORD PTR [rcx+0x8]
   1400689af:	48 8b d9             	mov    rbx,rcx
   1400689b2:	48 8b 09             	mov    rcx,QWORD PTR [rcx]
   1400689b5:	45 8b e0             	mov    r12d,r8d
   1400689b8:	45 33 c0             	xor    r8d,r8d
   1400689bb:	e8 7c 69 ff ff       	call   0x14005f33c
   1400689c0:	b9 08 00 00 00       	mov    ecx,0x8
   1400689c5:	44 8a c8             	mov    r9b,al
   1400689c8:	41 80 e1 fc          	and    r9b,0xfc
   1400689cc:	44 3a c9             	cmp    r9b,cl
   1400689cf:	b9 04 00 00 00       	mov    ecx,0x4
   1400689d4:	40 0f 94 c6          	sete   sil
   1400689d8:	44 3a c9             	cmp    r9b,cl
   1400689db:	40 0f 94 c5          	sete   bpl
   1400689df:	41 80 f9 14          	cmp    r9b,0x14
   1400689e3:	40 0f 94 c7          	sete   dil
   1400689e7:	84 c0                	test   al,al
   1400689e9:	75 08                	jne    0x1400689f3
   1400689eb:	8d 41 fd             	lea    eax,[rcx-0x3]
   1400689ee:	e9 ed 01 00 00       	jmp    0x140068be0
   1400689f3:	48 8d 4c 24 70       	lea    rcx,[rsp+0x70]
   1400689f8:	45 33 c0             	xor    r8d,r8d
   1400689fb:	33 d2                	xor    edx,edx
   1400689fd:	e8 96 32 ff ff       	call   0x14005bc98
   140068a02:	4c 8d 3d 67 cc 0b 00 	lea    r15,[rip+0xbcc67]        # 0x140125670
   140068a09:	48 8d 4c 24 28       	lea    rcx,[rsp+0x28]
   140068a0e:	4c 89 7c 24 20       	mov    QWORD PTR [rsp+0x20],r15
   140068a13:	e8 2c a9 ff ff       	call   0x140063344
   140068a18:	48 83 64 24 30 00    	and    QWORD PTR [rsp+0x30],0x0
   140068a1e:	48 83 64 24 38 00    	and    QWORD PTR [rsp+0x38],0x0
   140068a24:	83 64 24 40 00       	and    DWORD PTR [rsp+0x40],0x0
   140068a29:	4c 8d 35 50 db 0b 00 	lea    r14,[rip+0xbdb50]        # 0x140126580
   140068a30:	48 8d 4c 24 50       	lea    rcx,[rsp+0x50]
   140068a35:	4c 89 74 24 20       	mov    QWORD PTR [rsp+0x20],r14
   140068a3a:	c6 44 24 44 00       	mov    BYTE PTR [rsp+0x44],0x0
   140068a3f:	4c 89 7c 24 48       	mov    QWORD PTR [rsp+0x48],r15
   140068a44:	e8 fb a8 ff ff       	call   0x140063344
   140068a49:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
   140068a4e:	4c 8b 03             	mov    r8,QWORD PTR [rbx]
   140068a51:	48 8b 53 10          	mov    rdx,QWORD PTR [rbx+0x10]
   140068a55:	45 33 c9             	xor    r9d,r9d
   140068a58:	48 89 03             	mov    QWORD PTR [rbx],rax
   140068a5b:	48 63 44 24 30       	movsxd rax,DWORD PTR [rsp+0x30]
   140068a60:	45 8d 59 08          	lea    r11d,[r9+0x8]
   140068a64:	48 89 43 08          	mov    QWORD PTR [rbx+0x8],rax
   140068a68:	48 63 44 24 38       	movsxd rax,DWORD PTR [rsp+0x38]
   140068a6d:	4c 89 4c 24 58       	mov    QWORD PTR [rsp+0x58],r9
   140068a72:	48 89 43 10          	mov    QWORD PTR [rbx+0x10],rax
   140068a76:	48 8d 44 24 20       	lea    rax,[rsp+0x20]
   140068a7b:	4c 89 4c 24 60       	mov    QWORD PTR [rsp+0x60],r9
   140068a80:	48 89 84 24 a8 d4 00 	mov    QWORD PTR [rsp+0xd4a8],rax
   140068a87:	00 
   140068a88:	48 8d 44 24 48       	lea    rax,[rsp+0x48]
   140068a8d:	4c 89 74 24 48       	mov    QWORD PTR [rsp+0x48],r14
   140068a92:	48 89 84 24 b0 d4 00 	mov    QWORD PTR [rsp+0xd4b0],rax
   140068a99:	00 
   140068a9a:	49 8b c3             	mov    rax,r11
   140068a9d:	44 89 4c 24 68       	mov    DWORD PTR [rsp+0x68],r9d
   140068aa2:	4c 89 44 24 28       	mov    QWORD PTR [rsp+0x28],r8
   140068aa7:	48 89 54 24 38       	mov    QWORD PTR [rsp+0x38],rdx
   140068aac:	44                   	rex.R
   140068aad:	88                   	.byte 0x88
   140068aae:	64                   	fs
   140068aaf:	24                   	.byte 0x24
