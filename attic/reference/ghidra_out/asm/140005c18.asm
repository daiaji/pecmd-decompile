
PECMD.exe：     文件格式 pei-x86-64


Disassembly of section .text:

0000000140005c18 <.text+0x4c18>:
   140005c18:	48 83 ec 38          	sub    rsp,0x38
   140005c1c:	41 0f b7 c0          	movzx  eax,r8w
   140005c20:	44 0f b7 ca          	movzx  r9d,dx
   140005c24:	41 c1 e8 10          	shr    r8d,0x10
   140005c28:	c1 ea 10             	shr    edx,0x10
   140005c2b:	89 44 24 28          	mov    DWORD PTR [rsp+0x28],eax
   140005c2f:	44 89 44 24 20       	mov    DWORD PTR [rsp+0x20],r8d
   140005c34:	44 8b c2             	mov    r8d,edx
   140005c37:	48 8d 15 d2 6f 11 00 	lea    rdx,[rip+0x116fd2]        # 0x14011cc10
   140005c3e:	ff 15 f4 64 11 00    	call   QWORD PTR [rip+0x1164f4]        # 0x14011c138
   140005c44:	48 83 c4 38          	add    rsp,0x38
   140005c48:	c3                   	ret
   140005c49:	cc                   	int3
   140005c4a:	cc                   	int3
   140005c4b:	cc                   	int3
   140005c4c:	48 89 5c 24 08       	mov    QWORD PTR [rsp+0x8],rbx
   140005c51:	55                   	push   rbp
   140005c52:	56                   	push   rsi
   140005c53:	57                   	push   rdi
   140005c54:	41 54                	push   r12
   140005c56:	41 55                	push   r13
   140005c58:	41 56                	push   r14
   140005c5a:	41 57                	push   r15
   140005c5c:	48 83 ec 60          	sub    rsp,0x60
   140005c60:	48 8b d9             	mov    rbx,rcx
   140005c63:	48 8b ea             	mov    rbp,rdx
   140005c66:	48 8d 4c 24 58       	lea    rcx,[rsp+0x58]
   140005c6b:	ba 69 01 00 00       	mov    edx,0x169
   140005c70:	4d 8b f1             	mov    r14,r9
   140005c73:	4d 8b e8             	mov    r13,r8
   140005c76:	e8 19 da 05 00       	call   0x140063694
   140005c7b:	4c 8b 64 24 58       	mov    r12,QWORD PTR [rsp+0x58]
   140005c80:	41 b8 04 01 00 00    	mov    r8d,0x104
   140005c86:	49 81 c4 c8 00 00 00 	add    r12,0xc8
   140005c8d:	48 8b d3             	mov    rdx,rbx
   140005c90:	49 8b cc             	mov    rcx,r12
   140005c93:	ff 15 87 5d 11 00    	call   QWORD PTR [rip+0x115d87]        # 0x14011ba20
   140005c99:	33 db                	xor    ebx,ebx
   140005c9b:	48 83 ce ff          	or     rsi,0xffffffffffffffff
   140005c9f:	8d 43 01             	lea    eax,[rbx+0x1]
   140005ca2:	48 8b fb             	mov    rdi,rbx
   140005ca5:	48 3b eb             	cmp    rbp,rbx
   140005ca8:	74 08                	je     0x140005cb2
   140005caa:	44 8b f8             	mov    r15d,eax
   140005cad:	48 3b ee             	cmp    rbp,rsi
   140005cb0:	75 03                	jne    0x140005cb5
   140005cb2:	44 8b fb             	mov    r15d,ebx
   140005cb5:	4c 3b eb             	cmp    r13,rbx
   140005cb8:	74 15                	je     0x140005ccf
   140005cba:	ba 64 00 00 00       	mov    edx,0x64
   140005cbf:	49 8b cd             	mov    rcx,r13
   140005cc2:	e8 59 da 05 00       	call   0x140063720
   140005cc7:	4d 8b 5d 00          	mov    r11,QWORD PTR [r13+0x0]
   140005ccb:	66 41 89 1b          	mov    WORD PTR [r11],bx
   140005ccf:	44 3b fb             	cmp    r15d,ebx
   140005cd2:	74 15                	je     0x140005ce9
   140005cd4:	ba 64 00 00 00       	mov    edx,0x64
   140005cd9:	48 8b cd             	mov    rcx,rbp
   140005cdc:	e8 3f da 05 00       	call   0x140063720
   140005ce1:	4c 8b 5d 00          	mov    r11,QWORD PTR [rbp+0x0]
   140005ce5:	66 41 89 1b          	mov    WORD PTR [r11],bx
   140005ce9:	48 8b 05 50 6d 13 00 	mov    rax,QWORD PTR [rip+0x136d50]        # 0x14013ca40
   140005cf0:	48 3b c3             	cmp    rax,rbx
   140005cf3:	75 7a                	jne    0x140005d6f
   140005cf5:	4c 8d 8c 24 b0 00 00 	lea    r9,[rsp+0xb0]
   140005cfc:	00 
   140005cfd:	4c 8d 05 34 6d 13 00 	lea    r8,[rip+0x136d34]        # 0x14013ca38
   140005d04:	48 8d 15 35 70 11 00 	lea    rdx,[rip+0x117035]        # 0x14011cd40
   140005d0b:	48 8d 0d 0e 70 11 00 	lea    rcx,[rip+0x11700e]        # 0x14011cd20
   140005d12:	48 89 9c 24 b0 00 00 	mov    QWORD PTR [rsp+0xb0],rbx
   140005d19:	00 
   140005d1a:	e8 09 6b 05 00       	call   0x14005c828
   140005d1f:	4c 8d 8c 24 b0 00 00 	lea    r9,[rsp+0xb0]
   140005d26:	00 
   140005d27:	4c 8d 05 12 6d 13 00 	lea    r8,[rip+0x136d12]        # 0x14013ca40
   140005d2e:	48 8d 15 0b 70 11 00 	lea    rdx,[rip+0x11700b]        # 0x14011cd40
   140005d35:	48 8d 0d cc 6f 11 00 	lea    rcx,[rip+0x116fcc]        # 0x14011cd08
   140005d3c:	e8 e7 6a 05 00       	call   0x14005c828
   140005d41:	4c 8b 1d f0 6c 13 00 	mov    r11,QWORD PTR [rip+0x136cf0]        # 0x14013ca38
   140005d48:	4c 3b db             	cmp    r11,rbx
   140005d4b:	74 0c                	je     0x140005d59
   140005d4d:	48 8b 05 ec 6c 13 00 	mov    rax,QWORD PTR [rip+0x136cec]        # 0x14013ca40
   140005d54:	48 3b c3             	cmp    rax,rbx
   140005d57:	75 1d                	jne    0x140005d76
   140005d59:	48 8b c6             	mov    rax,rsi
   140005d5c:	4c 8b de             	mov    r11,rsi
   140005d5f:	48 89 35 d2 6c 13 00 	mov    QWORD PTR [rip+0x136cd2],rsi        # 0x14013ca38
   140005d66:	48 89 05 d3 6c 13 00 	mov    QWORD PTR [rip+0x136cd3],rax        # 0x14013ca40
   140005d6d:	eb 07                	jmp    0x140005d76
   140005d6f:	4c 8b 1d c2 6c 13 00 	mov    r11,QWORD PTR [rip+0x136cc2]        # 0x14013ca38
   140005d76:	89 9c 24 a8 00 00 00 	mov    DWORD PTR [rsp+0xa8],ebx
   140005d7d:	48 3b c6             	cmp    rax,rsi
   140005d80:	75 19                	jne    0x140005d9b
   140005d82:	48 8d 94 24 a8 00 00 	lea    rdx,[rsp+0xa8]
   140005d89:	00 
   140005d8a:	49 8b cc             	mov    rcx,r12
   140005d8d:	e8 d4 41 11 00       	call   0x140119f66
   140005d92:	8b b4 24 c0 00 00 00 	mov    esi,DWORD PTR [rsp+0xc0]
   140005d99:	eb 33                	jmp    0x140005dce
   140005d9b:	8b b4 24 c0 00 00 00 	mov    esi,DWORD PTR [rsp+0xc0]
   140005da2:	4c 8d 84 24 a8 00 00 	lea    r8,[rsp+0xa8]
   140005da9:	00 
   140005daa:	49 8b d4             	mov    rdx,r12
   140005dad:	8b ce                	mov    ecx,esi
   140005daf:	41 ff d3             	call   r11
   140005db2:	8b d8                	mov    ebx,eax
   140005db4:	85 c0                	test   eax,eax
   140005db6:	75 18                	jne    0x140005dd0
   140005db8:	8d 70 02             	lea    esi,[rax+0x2]
   140005dbb:	4c 8d 84 24 a8 00 00 	lea    r8,[rsp+0xa8]
   140005dc2:	00 
   140005dc3:	49 8b d4             	mov    rdx,r12
   140005dc6:	8b ce                	mov    ecx,esi
   140005dc8:	ff 15 6a 6c 13 00    	call   QWORD PTR [rip+0x136c6a]        # 0x14013ca38
   140005dce:	8b d8                	mov    ebx,eax
   140005dd0:	ff 15 ea 5b 11 00    	call   QWORD PTR [rip+0x115bea]        # 0x14011b9c0
   140005dd6:	85 db                	test   ebx,ebx
   140005dd8:	75 19                	jne    0x140005df3
   140005dda:	4d 85 f6             	test   r14,r14
   140005ddd:	0f 84 af 02 00 00    	je     0x140006092
   140005de3:	8d 4b 01             	lea    ecx,[rbx+0x1]
   140005de6:	85 c0                	test   eax,eax
   140005de8:	0f 44 c1             	cmove  eax,ecx
   140005deb:	49 89 06             	mov    QWORD PTR [r14],rax
   140005dee:	e9 9f 02 00 00       	jmp    0x140006092
   140005df3:	21 bc 24 b0 00 00 00 	and    DWORD PTR [rsp+0xb0],edi
   140005dfa:	48 21 7c 24 38       	and    QWORD PTR [rsp+0x38],rdi
   140005dff:	8d 93 a0 01 00 00    	lea    edx,[rbx+0x1a0]
   140005e05:	48 8d 4c 24 50       	lea    rcx,[rsp+0x50]
   140005e0a:	e8 99 d5 05 00       	call   0x1400633a8
   140005e0f:	4c 8b 74 24 50       	mov    r14,QWORD PTR [rsp+0x50]
   140005e14:	48 8b 05 25 6c 13 00 	mov    rax,QWORD PTR [rip+0x136c25]        # 0x14013ca40
   140005e1b:	48 21 7c 24 30       	and    QWORD PTR [rsp+0x30],rdi
   140005e20:	4c 89 74 24 48       	mov    QWORD PTR [rsp+0x48],r14
   140005e25:	49 81 c6 90 01 00 00 	add    r14,0x190
   140005e2c:	48 83 f8 ff          	cmp    rax,0xffffffffffffffff
   140005e30:	75 17                	jne    0x140005e49
   140005e32:	8b 94 24 a8 00 00 00 	mov    edx,DWORD PTR [rsp+0xa8]
   140005e39:	4d 8b ce             	mov    r9,r14
   140005e3c:	44 8b c3             	mov    r8d,ebx
   140005e3f:	49 8b cc             	mov    rcx,r12
   140005e42:	e8 19 41 11 00       	call   0x140119f60
   140005e47:	eb 17                	jmp    0x140005e60
   140005e49:	44 8b 84 24 a8 00 00 	mov    r8d,DWORD PTR [rsp+0xa8]
   140005e50:	00 
   140005e51:	44 8b cb             	mov    r9d,ebx
   140005e54:	49 8b d4             	mov    rdx,r12
   140005e57:	8b ce                	mov    ecx,esi
   140005e59:	4c 89 74 24 20       	mov    QWORD PTR [rsp+0x20],r14
   140005e5e:	ff d0                	call   rax
   140005e60:	4c 8d 8c 24 b0 00 00 	lea    r9,[rsp+0xb0]
   140005e67:	00 
   140005e68:	4c 8d 44 24 38       	lea    r8,[rsp+0x38]
   140005e6d:	48 8d 15 90 6e 11 00 	lea    rdx,[rip+0x116e90]        # 0x14011cd04
   140005e74:	49 8b ce             	mov    rcx,r14
   140005e77:	e8 de 40 11 00       	call   0x140119f5a
   140005e7c:	48 8b 44 24 38       	mov    rax,QWORD PTR [rsp+0x38]
   140005e81:	48 85 ed             	test   rbp,rbp
   140005e84:	75 08                	jne    0x140005e8e
   140005e86:	8b 78 10             	mov    edi,DWORD PTR [rax+0x10]
   140005e89:	8b 40 14             	mov    eax,DWORD PTR [rax+0x14]
   140005e8c:	eb 06                	jmp    0x140005e94
   140005e8e:	8b 78 08             	mov    edi,DWORD PTR [rax+0x8]
   140005e91:	8b 40 0c             	mov    eax,DWORD PTR [rax+0xc]
   140005e94:	48 83 64 24 40 00    	and    QWORD PTR [rsp+0x40],0x0
   140005e9a:	4c 8d 8c 24 b0 00 00 	lea    r9,[rsp+0xb0]
   140005ea1:	00 
   140005ea2:	4c 8d 44 24 40       	lea    r8,[rsp+0x40]
   140005ea7:	48 8d 15 22 6e 11 00 	lea    rdx,[rip+0x116e22]        # 0x14011ccd0
   140005eae:	48 c1 e7 20          	shl    rdi,0x20
   140005eb2:	49 8b ce             	mov    rcx,r14
   140005eb5:	48 0b f8             	or     rdi,rax
   140005eb8:	33 f6                	xor    esi,esi
   140005eba:	e8 9b 40 11 00       	call   0x140119f5a
   140005ebf:	4c 8b 5c 24 40       	mov    r11,QWORD PTR [rsp+0x40]
   140005ec4:	45 33 c0             	xor    r8d,r8d
   140005ec7:	4d 3b d8             	cmp    r11,r8
   140005eca:	74 0e                	je     0x140005eda
   140005ecc:	41 0f b7 33          	movzx  esi,WORD PTR [r11]
   140005ed0:	41 0f b7 43 02       	movzx  eax,WORD PTR [r11+0x2]
   140005ed5:	c1 e6 10             	shl    esi,0x10
   140005ed8:	0b f0                	or     esi,eax
   140005eda:	4d 3b e8             	cmp    r13,r8
   140005edd:	75 09                	jne    0x140005ee8
   140005edf:	49 3b e8             	cmp    rbp,r8
   140005ee2:	0f 85 ce 00 00 00    	jne    0x140005fb6
   140005ee8:	4c 8b 64 24 48       	mov    r12,QWORD PTR [rsp+0x48]
   140005eed:	48 8d 15 8c 6d 11 00 	lea    rdx,[rip+0x116d8c]        # 0x14011cc80
   140005ef4:	44 8b c6             	mov    r8d,esi
   140005ef7:	49 8b cc             	mov    rcx,r12
   140005efa:	ff 15 38 62 11 00    	call   QWORD PTR [rip+0x116238]        # 0x14011c138
   140005f00:	4c 8d 8c 24 b0 00 00 	lea    r9,[rsp+0xb0]
   140005f07:	00 
   140005f08:	4c 8d 44 24 30       	lea    r8,[rsp+0x30]
   140005f0d:	49 8b d4             	mov    rdx,r12
   140005f10:	49 8b ce             	mov    rcx,r14
   140005f13:	e8 42 40 11 00       	call   0x140119f5a
   140005f18:	4c 8b 5c 24 30       	mov    r11,QWORD PTR [rsp+0x30]
   140005f1d:	45 33 c0             	xor    r8d,r8d
   140005f20:	49 8b db             	mov    rbx,r11
   140005f23:	4d 3b d8             	cmp    r11,r8
   140005f26:	74 19                	je     0x140005f41
   140005f28:	66 45 39 03          	cmp    WORD PTR [r11],r8w
   140005f2c:	74 13                	je     0x140005f41
   140005f2e:	49 8b cb             	mov    rcx,r11
   140005f31:	e8 26 fc ff ff       	call   0x140005b5c
   140005f36:	4c 8b 5c 24 30       	mov    r11,QWORD PTR [rsp+0x30]
   140005f3b:	45 33 c0             	xor    r8d,r8d
   140005f3e:	48 8b f8             	mov    rdi,rax
   140005f41:	4d 3b e8             	cmp    r13,r8
   140005f44:	74 75                	je     0x140005fbb
   140005f46:	4d 3b d8             	cmp    r11,r8
   140005f49:	74 51                	je     0x140005f9c
   140005f4b:	66 44 39 03          	cmp    WORD PTR [rbx],r8w
   140005f4f:	74 4b                	je     0x140005f9c
   140005f51:	49 8b 4d 00          	mov    rcx,QWORD PTR [r13+0x0]
   140005f55:	48 8d 93 c6 00 00 00 	lea    rdx,[rbx+0xc6]
   140005f5c:	41 bd 20 00 00 00    	mov    r13d,0x20
   140005f62:	48 3b da             	cmp    rbx,rdx
   140005f65:	73 2f                	jae    0x140005f96
   140005f67:	45 8d 4d e9          	lea    r9d,[r13-0x17]
   140005f6b:	0f b7 03             	movzx  eax,WORD PTR [rbx]
   140005f6e:	48 83 c3 02          	add    rbx,0x2
   140005f72:	66 89 01             	mov    WORD PTR [rcx],ax
   140005f75:	66 41 3b c0          	cmp    ax,r8w
   140005f79:	74 1b                	je     0x140005f96
   140005f7b:	66 44 3b c8          	cmp    r9w,ax
   140005f7f:	77 06                	ja     0x140005f87
   140005f81:	66 83 f8 0d          	cmp    ax,0xd
   140005f85:	76 0f                	jbe    0x140005f96
   140005f87:	66 44 3b e8          	cmp    r13w,ax
   140005f8b:	74 09                	je     0x140005f96
   140005f8d:	48 83 c1 02          	add    rcx,0x2
   140005f91:	48 3b da             	cmp    rbx,rdx
   140005f94:	72 d5                	jb     0x140005f6b
   140005f96:	66 44 89 01          	mov    WORD PTR [rcx],r8w
   140005f9a:	eb 25                	jmp    0x140005fc1
   140005f9c:	48 8b 44 24 38       	mov    rax,QWORD PTR [rsp+0x38]
   140005fa1:	49 8b 4d 00          	mov    rcx,QWORD PTR [r13+0x0]
   140005fa5:	44 8b 40 14          	mov    r8d,DWORD PTR [rax+0x14]
   140005fa9:	8b 50 10             	mov    edx,DWORD PTR [rax+0x10]
   140005fac:	e8 67 fc ff ff       	call   0x140005c18
   140005fb1:	45 33 c0             	xor    r8d,r8d
   140005fb4:	eb 05                	jmp    0x140005fbb
   140005fb6:	4c 8b 64 24 48       	mov    r12,QWORD PTR [rsp+0x48]
   140005fbb:	41 bd 20 00 00 00    	mov    r13d,0x20
   140005fc1:	45 3b f8             	cmp    r15d,r8d
   140005fc4:	75 09                	jne    0x140005fcf
   140005fc6:	49 3b e8             	cmp    rbp,r8
   140005fc9:	0f 84 b9 00 00 00    	je     0x140006088
   140005fcf:	48 8d 15 5a 6c 11 00 	lea    rdx,[rip+0x116c5a]        # 0x14011cc30
   140005fd6:	44 8b c6             	mov    r8d,esi
   140005fd9:	49 8b cc             	mov    rcx,r12
   140005fdc:	ff 15 56 61 11 00    	call   QWORD PTR [rip+0x116156]        # 0x14011c138
   140005fe2:	4c 8d 8c 24 b0 00 00 	lea    r9,[rsp+0xb0]
   140005fe9:	00 
   140005fea:	4c 8d 44 24 30       	lea    r8,[rsp+0x30]
   140005fef:	49 8b d4             	mov    rdx,r12
   140005ff2:	49 8b ce             	mov    rcx,r14
   140005ff5:	e8 60 3f 11 00       	call   0x140119f5a
   140005ffa:	4c 8b 5c 24 30       	mov    r11,QWORD PTR [rsp+0x30]
   140005fff:	33 f6                	xor    esi,esi
   140006001:	49 8b db             	mov    rbx,r11
   140006004:	4c 3b de             	cmp    r11,rsi
   140006007:	74 16                	je     0x14000601f
   140006009:	66 41 39 33          	cmp    WORD PTR [r11],si
   14000600d:	74 10                	je     0x14000601f
   14000600f:	49 8b cb             	mov    rcx,r11
   140006012:	e8 45 fb ff ff       	call   0x140005b5c
   140006017:	4c 8b 5c 24 30       	mov    r11,QWORD PTR [rsp+0x30]
   14000601c:	48 8b f8             	mov    rdi,rax
   14000601f:	44 3b fe             	cmp    r15d,esi
   140006022:	74 64                	je     0x140006088
   140006024:	4c 3b de             	cmp    r11,rsi
   140006027:	74 4a                	je     0x140006073
   140006029:	66 39 33             	cmp    WORD PTR [rbx],si
   14000602c:	74 45                	je     0x140006073
   14000602e:	48 8b 4d 00          	mov    rcx,QWORD PTR [rbp+0x0]
   140006032:	48 8d 93 c6 00 00 00 	lea    rdx,[rbx+0xc6]
   140006039:	48 3b da             	cmp    rbx,rdx
   14000603c:	73 30                	jae    0x14000606e
   14000603e:	41 b8 09 00 00 00    	mov    r8d,0x9
   140006044:	0f b7 03             	movzx  eax,WORD PTR [rbx]
   140006047:	48 83 c3 02          	add    rbx,0x2
   14000604b:	66 89 01             	mov    WORD PTR [rcx],ax
   14000604e:	66 3b c6             	cmp    ax,si
   140006051:	74 1b                	je     0x14000606e
   140006053:	66 44 3b c0          	cmp    r8w,ax
   140006057:	77 06                	ja     0x14000605f
   140006059:	66 83 f8 0d          	cmp    ax,0xd
   14000605d:	76 0f                	jbe    0x14000606e
   14000605f:	66 44 3b e8          	cmp    r13w,ax
   140006063:	74 09                	je     0x14000606e
   140006065:	48 83 c1 02          	add    rcx,0x2
   140006069:	48 3b da             	cmp    rbx,rdx
   14000606c:	72 d6                	jb     0x140006044
   14000606e:	66 89 31             	mov    WORD PTR [rcx],si
   140006071:	eb 15                	jmp    0x140006088
   140006073:	48 8b 44 24 38       	mov    rax,QWORD PTR [rsp+0x38]
   140006078:	48 8b 4d 00          	mov    rcx,QWORD PTR [rbp+0x0]
   14000607c:	44 8b 40 0c          	mov    r8d,DWORD PTR [rax+0xc]
   140006080:	8b 50 08             	mov    edx,DWORD PTR [rax+0x8]
   140006083:	e8 90 fb ff ff       	call   0x140005c18
   140006088:	48 8d 4c 24 50       	lea    rcx,[rsp+0x50]
   14000608d:	e8 72 50 05 00       	call   0x14005b104
   140006092:	48 8d 4c 24 58       	lea    rcx,[rsp+0x58]
   140006097:	e8 68 50 05 00       	call   0x14005b104
   14000609c:	48 8b 9c 24 a0 00 00 	mov    rbx,QWORD PTR [rsp+0xa0]
   1400060a3:	00 
   1400060a4:	48 8b c7             	mov    rax,rdi
   1400060a7:	48 83 c4 60          	add    rsp,0x60
   1400060ab:	41 5f                	pop    r15
   1400060ad:	41 5e                	pop    r14
   1400060af:	41 5d                	pop    r13
   1400060b1:	41 5c                	pop    r12
   1400060b3:	5f                   	pop    rdi
   1400060b4:	5e                   	pop    rsi
   1400060b5:	5d                   	pop    rbp
   1400060b6:	c3                   	ret
   1400060b7:	cc                   	int3
   1400060b8:	4c 8b dc             	mov    r11,rsp
   1400060bb:	49 89 5b 18          	mov    QWORD PTR [r11+0x18],rbx
   1400060bf:	49 89 4b 08          	mov    QWORD PTR [r11+0x8],rcx
   1400060c3:	57                   	push   rdi
   1400060c4:	48 83 ec 70          	sub    rsp,0x70
   1400060c8:	33 ff                	xor    edi,edi
   1400060ca:	33 c0                	xor    eax,eax
   1400060cc:	48 8b d9             	mov    rbx,rcx
   1400060cf:	49 89 7b 10          	mov    QWORD PTR [r11+0x10],rdi
   1400060d3:	49 89 7b e8          	mov    QWORD PTR [r11-0x18],rdi
   1400060d7:	49 89 43 f0          	mov    QWORD PTR [r11-0x10],rax
   1400060db:	48 3b cf             	cmp    rcx,rdi
   1400060de:	75 21                	jne    0x140006101
   1400060e0:	4d 8d 43 08          	lea    r8,[r11+0x8]
   1400060e4:	ba ff 01 0f 00       	mov    edx,0xf01ff
   1400060e9:	48 83 c9 ff          	or     rcx,0xffffffffffffffff
   1400060ed:	ff 15 25 4f 11 00    	call   QWORD PTR [rip+0x114f25]        # 0x14011b018
   1400060f3:	48 39 bc 24 80 00 00 	cmp    QWORD PTR [rsp+0x80],rdi
   1400060fa:	00 
   1400060fb:	0f 84 a8 00 00 00    	je     0x1400061a9
   140006101:	48 8d 44 24 60       	lea    rax,[rsp+0x60]
   140006106:	48 8d 0d fb 40 13 00 	lea    rcx,[rip+0x1340fb]        # 0x14013a208
   14000610d:	41 b9 20 02 00 00    	mov    r9d,0x220
   140006113:	48 89 44 24 50       	mov    QWORD PTR [rsp+0x50],rax
   140006118:	89 7c 24 48          	mov    DWORD PTR [rsp+0x48],edi
   14000611c:	89 7c 24 40          	mov    DWORD PTR [rsp+0x40],edi
   140006120:	89 7c 24 38          	mov    DWORD PTR [rsp+0x38],edi
   140006124:	89 7c 24 30          	mov    DWORD PTR [rsp+0x30],edi
   140006128:	41 b8 20 00 00 00    	mov    r8d,0x20
   14000612e:	b2 02                	mov    dl,0x2
   140006130:	89 7c 24 28          	mov    DWORD PTR [rsp+0x28],edi
   140006134:	89 7c 24 20          	mov    DWORD PTR [rsp+0x20],edi
   140006138:	ff 15 d2 4e 11 00    	call   QWORD PTR [rip+0x114ed2]        # 0x14011b010
   14000613e:	3b c7                	cmp    eax,edi
   140006140:	74 67                	je     0x1400061a9
   140006142:	48 8b 8c 24 80 00 00 	mov    rcx,QWORD PTR [rsp+0x80]
   140006149:	00 
   14000614a:	48 8d 84 24 88 00 00 	lea    rax,[rsp+0x88]
   140006151:	00 
   140006152:	33 d2                	xor    edx,edx
   140006154:	48 89 44 24 40       	mov    QWORD PTR [rsp+0x40],rax
   140006159:	48 89 7c 24 38       	mov    QWORD PTR [rsp+0x38],rdi
   14000615e:	48 8d 05 6b 3f 13 00 	lea    rax,[rip+0x133f6b]        # 0x14013a0d0
   140006165:	89 7c 24 30          	mov    DWORD PTR [rsp+0x30],edi
   140006169:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
   14000616e:	4c 8d 4c 24 60       	lea    r9,[rsp+0x60]
   140006173:	44 8d 42 01          	lea    r8d,[rdx+0x1]
   140006177:	c7 44 24 20 1a 00 00 	mov    DWORD PTR [rsp+0x20],0x1a
   14000617e:	00 
   14000617f:	89 7c 24 68          	mov    DWORD PTR [rsp+0x68],edi
   140006183:	ff 15 7f 4e 11 00    	call   QWORD PTR [rip+0x114e7f]        # 0x14011b008
   140006189:	f7 d8                	neg    eax
   14000618b:	48 1b c9             	sbb    rcx,rcx
   14000618e:	48 23 8c 24 88 00 00 	and    rcx,QWORD PTR [rsp+0x88]
   140006195:	00 
   140006196:	48 89 8c 24 88 00 00 	mov    QWORD PTR [rsp+0x88],rcx
   14000619d:	00 
   14000619e:	48 8b 4c 24 60       	mov    rcx,QWORD PTR [rsp+0x60]
   1400061a3:	ff 15 2f 50 11 00    	call   QWORD PTR [rip+0x11502f]        # 0x14011b1d8
   1400061a9:	48 3b df             	cmp    rbx,rdi
   1400061ac:	75 0e                	jne    0x1400061bc
   1400061ae:	48 8b 8c 24 80 00 00 	mov    rcx,QWORD PTR [rsp+0x80]
   1400061b5:	00 
   1400061b6:	ff 15 ac 58 11 00    	call   QWORD PTR [rip+0x1158ac]        # 0x14011ba68
   1400061bc:	48 8b 84 24 88 00 00 	mov    rax,QWORD PTR [rsp+0x88]
   1400061c3:	00 
   1400061c4:	48 8b 9c 24 90 00 00 	mov    rbx,QWORD PTR [rsp+0x90]
   1400061cb:	00 
   1400061cc:	48 83 c4 70          	add    rsp,0x70
   1400061d0:	5f                   	pop    rdi
   1400061d1:	c3                   	ret
   1400061d2:	cc                   	int3
   1400061d3:	cc                   	int3
   1400061d4:	4c 8b dc             	mov    r11,rsp
   1400061d7:	49 89 5b 08          	mov    QWORD PTR [r11+0x8],rbx
   1400061db:	49 89 6b 18          	mov    QWORD PTR [r11+0x18],rbp
   1400061df:	4d 89 4b 20          	mov    QWORD PTR [r11+0x20],r9
   1400061e3:	56                   	push   rsi
   1400061e4:	57                   	push   rdi
   1400061e5:	41 54                	push   r12
   1400061e7:	48 83 ec 40          	sub    rsp,0x40
   1400061eb:	45 33 e4             	xor    r12d,r12d
   1400061ee:	49 8b f8             	mov    rdi,r8
   1400061f1:	48 8b da             	mov    rbx,rdx
   1400061f4:	48 8b e9             	mov    rbp,rcx
   1400061f7:	49 3b d4             	cmp    rdx,r12
   1400061fa:	0f 84 d1 00 00 00    	je     0x1400062d1
   140006200:	41 8d 44 24 2a       	lea    eax,[r12+0x2a]
   140006205:	66 3b 02             	cmp    ax,WORD PTR [rdx]
   140006208:	0f 85 c3 00 00 00    	jne    0x1400062d1
   14000620e:	49 8d 4b e0          	lea    rcx,[r11-0x20]
   140006212:	ba 09 02 00 00       	mov    edx,0x209
   140006217:	e8 78 d4 05 00       	call   0x140063694
   14000621c:	48 8b 74 24 38       	mov    rsi,QWORD PTR [rsp+0x38]
   140006221:	49 3b fc             	cmp    rdi,r12
   140006224:	75 79                	jne    0x14000629f
   140006226:	4c 39 64 24 78       	cmp    QWORD PTR [rsp+0x78],r12
   14000622b:	0f 84 96 00 00 00    	je     0x1400062c7
   140006231:	48 8d 4c 24 78       	lea    rcx,[rsp+0x78]
   140006236:	e8 19 4f 05 00       	call   0x14005b154
   14000623b:	48 8d 54 24 68       	lea    rdx,[rsp+0x68]
   140006240:	48 8d 4c 24 78       	lea    rcx,[rsp+0x78]
   140006245:	45 33 c0             	xor    r8d,r8d
   140006248:	4c 89 64 24 68       	mov    QWORD PTR [rsp+0x68],r12
   14000624d:	e8 f6 e9 01 00       	call   0x140024c48
   140006252:	4c 8b 44 24 68       	mov    r8,QWORD PTR [rsp+0x68]
   140006257:	48 8b 54 24 78       	mov    rdx,QWORD PTR [rsp+0x78]
   14000625c:	48 8d 4c 24 30       	lea    rcx,[rsp+0x30]
   140006261:	e8 6e a0 06 00       	call   0x1400702d4
   140006266:	4c 39 64 24 68       	cmp    QWORD PTR [rsp+0x68],r12
   14000626b:	7e 7a                	jle    0x1400062e7
   14000626d:	48 8b 54 24 30       	mov    rdx,QWORD PTR [rsp+0x30]
   140006272:	41 b9 08 02 00 00    	mov    r9d,0x208
   140006278:	45 33 c0             	xor    r8d,r8d
   14000627b:	33 c9                	xor    ecx,ecx
   14000627d:	4c 89 64 24 28       	mov    QWORD PTR [rsp+0x28],r12
   140006282:	48 89 74 24 20       	mov    QWORD PTR [rsp+0x20],rsi
   140006287:	ff 15 73 56 11 00    	call   QWORD PTR [rip+0x115673]        # 0x14011b900
   14000628d:	41 3b c4             	cmp    eax,r12d
   140006290:	74 55                	je     0x1400062e7
   140006292:	48 8d 4c 24 30       	lea    rcx,[rsp+0x30]
   140006297:	48 8b fe             	mov    rdi,rsi
   14000629a:	e8 65 4e 05 00       	call   0x14005b104
   14000629f:	4c 8d 44 24 68       	lea    r8,[rsp+0x68]
   1400062a4:	48 8b d5             	mov    rdx,rbp
   1400062a7:	48 8b cf             	mov    rcx,rdi
   1400062aa:	4c 89 64 24 68       	mov    QWORD PTR [rsp+0x68],r12
   1400062af:	e8 20 da 0d 00       	call   0x1400e3cd4
   1400062b4:	4c 8b 5c 24 68       	mov    r11,QWORD PTR [rsp+0x68]
   1400062b9:	4d 3b dc             	cmp    r11,r12
   1400062bc:	74 09                	je     0x1400062c7
   1400062be:	66 45 89 63 fe       	mov    WORD PTR [r11-0x2],r12w
   1400062c3:	48 8b 5d 00          	mov    rbx,QWORD PTR [rbp+0x0]
   1400062c7:	48 8d 4c 24 38       	lea    rcx,[rsp+0x38]
   1400062cc:	e8 33 4e 05 00       	call   0x14005b104
   1400062d1:	48 8b 6c 24 70       	mov    rbp,QWORD PTR [rsp+0x70]
   1400062d6:	48 8b c3             	mov    rax,rbx
   1400062d9:	48 8b 5c 24 60       	mov    rbx,QWORD PTR [rsp+0x60]
   1400062de:	48 83 c4 40          	add    rsp,0x40
   1400062e2:	41 5c                	pop    r12
   1400062e4:	5f                   	pop    rdi
   1400062e5:	5e                   	pop    rsi
   1400062e6:	c3                   	ret
   1400062e7:	48 8d 4c 24 30       	lea    rcx,[rsp+0x30]
   1400062ec:	e8 13 4e 05 00       	call   0x14005b104
   1400062f1:	eb d4                	jmp    0x1400062c7
   1400062f3:	cc                   	int3
   1400062f4:	40 53                	rex push rbx
   1400062f6:	48 83 ec 20          	sub    rsp,0x20
   1400062fa:	ba 05 01 00 00       	mov    edx,0x105
   1400062ff:	48 8b d9             	mov    rbx,rcx
   140006302:	e8 19 d4 05 00       	call   0x140063720
   140006307:	48 8b 13             	mov    rdx,QWORD PTR [rbx]
   14000630a:	41 b8 04 01 00 00    	mov    r8d,0x104
   140006310:	33 c9                	xor    ecx,ecx
   140006312:	ff 15 e0 55 11 00    	call   QWORD PTR [rip+0x1155e0]        # 0x14011b8f8
   140006318:	48 8b 03             	mov    rax,QWORD PTR [rbx]
   14000631b:	48 83 c4 20          	add    rsp,0x20
   14000631f:	5b                   	pop    rbx
   140006320:	c3                   	ret
   140006321:	cc                   	int3
   140006322:	cc                   	int3
   140006323:	cc                   	int3
   140006324:	40 53                	rex push rbx
   140006326:	48 83 ec 20          	sub    rsp,0x20
   14000632a:	33 db                	xor    ebx,ebx
   14000632c:	48 3b cb             	cmp    rcx,rbx
   14000632f:	74 13                	je     0x140006344
   140006331:	66 39 19             	cmp    WORD PTR [rcx],bx
   140006334:	74 0e                	je     0x140006344
   140006336:	e8 f9 b9 0f 00       	call   0x140101d34
   14000633b:	3b c3                	cmp    eax,ebx
   14000633d:	0f 94 c3             	sete   bl
   140006340:	8b c3                	mov    eax,ebx
   140006342:	eb 02                	jmp    0x140006346
   140006344:	33 c0                	xor    eax,eax
   140006346:	48 83 c4 20          	add    rsp,0x20
   14000634a:	5b                   	pop    rbx
   14000634b:	c3                   	ret
   14000634c:	40 53                	rex push rbx
   14000634e:	48 83 ec 20          	sub    rsp,0x20
   140006352:	48 8b 05 ef 3c 13 00 	mov    rax,QWORD PTR [rip+0x133cef]        # 0x14013a048
   140006359:	33 db                	xor    ebx,ebx
   14000635b:	48 3b c3             	cmp    rax,rbx
   14000635e:	75 04                	jne    0x140006364
   140006360:	33 c0                	xor    eax,eax
   140006362:	eb 7b                	jmp    0x1400063df
   140006364:	48 83 f8 01          	cmp    rax,0x1
   140006368:	74 07                	je     0x140006371
   14000636a:	b8 01 00 00 00       	mov    eax,0x1
   14000636f:	eb 6e                	jmp    0x1400063df
   140006371:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   140006376:	4c 8d 05 d3 66 13 00 	lea    r8,[rip+0x1366d3]        # 0x14013ca50
   14000637d:	48 8d 15 2c 6a 11 00 	lea    rdx,[rip+0x116a2c]        # 0x14011cdb0
   140006384:	48 8d 0d 05 6a 11 00 	lea    rcx,[rip+0x116a05]        # 0x14011cd90
   14000638b:	48 89 5c 24 30       	mov    QWORD PTR [rsp+0x30],rbx
   140006390:	e8 93 64 05 00       	call   0x14005c828
   140006395:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   14000639a:	4c 8d 05 b7 66 13 00 	lea    r8,[rip+0x1366b7]        # 0x14013ca58
   1400063a1:	48 8d 15 08 6a 11 00 	lea    rdx,[rip+0x116a08]        # 0x14011cdb0
   1400063a8:	48 8d 0d c1 69 11 00 	lea    rcx,[rip+0x1169c1]        # 0x14011cd70
   1400063af:	e8 74 64 05 00       	call   0x14005c828
   1400063b4:	4c 8d 4c 24 30       	lea    r9,[rsp+0x30]
   1400063b9:	4c 8d 05 88 3c 13 00 	lea    r8,[rip+0x133c88]        # 0x14013a048
   1400063c0:	48 8d 15 e9 69 11 00 	lea    rdx,[rip+0x1169e9]        # 0x14011cdb0
   1400063c7:	48 8d 0d 7a 69 11 00 	lea    rcx,[rip+0x11697a]        # 0x14011cd48
   1400063ce:	e8 55 64 05 00       	call   0x14005c828
   1400063d3:	48 39 1d 6e 3c 13 00 	cmp    QWORD PTR [rip+0x133c6e],rbx        # 0x14013a048
   1400063da:	0f 95 c3             	setne  bl
   1400063dd:	8b c3                	mov    eax,ebx
   1400063df:	48 83 c4 20          	add    rsp,0x20
   1400063e3:	5b                   	pop    rbx
   1400063e4:	c3                   	ret
   1400063e5:	cc                   	int3
   1400063e6:	cc                   	int3
   1400063e7:	cc                   	int3
   1400063e8:	33 c0                	xor    eax,eax
   1400063ea:	48 89 01             	mov    QWORD PTR [rcx],rax
   1400063ed:	48 89 41 08          	mov    QWORD PTR [rcx+0x8],rax
   1400063f1:	48 89 41 10          	mov    QWORD PTR [rcx+0x10],rax
   1400063f5:	48 8b c1             	mov    rax,rcx
   1400063f8:	c3                   	ret
   1400063f9:	cc                   	int3
   1400063fa:	cc                   	int3
   1400063fb:	cc                   	int3
   1400063fc:	48 89 5c 24 10       	mov    QWORD PTR [rsp+0x10],rbx
   140006401:	48 89 74 24 18       	mov    QWORD PTR [rsp+0x18],rsi
   140006406:	57                   	push   rdi
   140006407:	48 83 ec 40          	sub    rsp,0x40
   14000640b:	0f b7 05 fa 7d 13 00 	movzx  eax,WORD PTR [rip+0x137dfa]        # 0x14013e20c
   140006412:	49 8b f0             	mov    rsi,r8
   140006415:	8b da                	mov    ebx,edx
   140006417:	48                   	rex.W
