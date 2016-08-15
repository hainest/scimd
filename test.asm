
test:     file format elf64-x86-64


Disassembly of section .text:

0000000000400b40 <_start>:
  400b40:	31 ed                	xor    ebp,ebp
  400b42:	49 89 d1             	mov    r9,rdx
  400b45:	5e                   	pop    rsi
  400b46:	48 89 e2             	mov    rdx,rsp
  400b49:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
  400b4d:	50                   	push   rax
  400b4e:	54                   	push   rsp
  400b4f:	49 c7 c0 90 11 40 00 	mov    r8,0x401190
  400b56:	48 c7 c1 20 11 40 00 	mov    rcx,0x401120
  400b5d:	48 c7 c7 70 0d 40 00 	mov    rdi,0x400d70
  400b64:	e8 e7 fe ff ff       	call   400a50 <__libc_start_main@plt>
  400b69:	f4                   	hlt    
  400b6a:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]

0000000000400b70 <__do_global_dtors_aux>:
  400b70:	55                   	push   rbp
  400b71:	48 89 e5             	mov    rbp,rsp
  400b74:	53                   	push   rbx
  400b75:	48 83 ec 08          	sub    rsp,0x8
  400b79:	80 3d a0 17 20 00 00 	cmp    BYTE PTR [rip+0x2017a0],0x0        # 602320 <completed.7508>
  400b80:	75 4b                	jne    400bcd <__do_global_dtors_aux+0x5d>
  400b82:	bb 08 1e 60 00       	mov    ebx,0x601e08
  400b87:	48 8b 05 9a 17 20 00 	mov    rax,QWORD PTR [rip+0x20179a]        # 602328 <dtor_idx.7510>
  400b8e:	48 81 eb 00 1e 60 00 	sub    rbx,0x601e00
  400b95:	48 c1 fb 03          	sar    rbx,0x3
  400b99:	48 83 eb 01          	sub    rbx,0x1
  400b9d:	48 39 d8             	cmp    rax,rbx
  400ba0:	73 24                	jae    400bc6 <__do_global_dtors_aux+0x56>
  400ba2:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]
  400ba8:	48 83 c0 01          	add    rax,0x1
  400bac:	48 89 05 75 17 20 00 	mov    QWORD PTR [rip+0x201775],rax        # 602328 <dtor_idx.7510>
  400bb3:	ff 14 c5 00 1e 60 00 	call   QWORD PTR [rax*8+0x601e00]
  400bba:	48 8b 05 67 17 20 00 	mov    rax,QWORD PTR [rip+0x201767]        # 602328 <dtor_idx.7510>
  400bc1:	48 39 d8             	cmp    rax,rbx
  400bc4:	72 e2                	jb     400ba8 <__do_global_dtors_aux+0x38>
  400bc6:	c6 05 53 17 20 00 01 	mov    BYTE PTR [rip+0x201753],0x1        # 602320 <completed.7508>
  400bcd:	48 83 c4 08          	add    rsp,0x8
  400bd1:	5b                   	pop    rbx
  400bd2:	c9                   	leave  
  400bd3:	c3                   	ret    
  400bd4:	66 66 66 2e 0f 1f 84 	data32 data32 nop WORD PTR cs:[rax+rax*1+0x0]
  400bdb:	00 00 00 00 00 

0000000000400be0 <frame_dummy>:
  400be0:	55                   	push   rbp
  400be1:	48 83 3d 27 12 20 00 	cmp    QWORD PTR [rip+0x201227],0x0        # 601e10 <__JCR_END__>
  400be8:	00 
  400be9:	48 89 e5             	mov    rbp,rsp
  400bec:	74 12                	je     400c00 <frame_dummy+0x20>
  400bee:	b8 00 00 00 00       	mov    eax,0x0
  400bf3:	48 85 c0             	test   rax,rax
  400bf6:	74 08                	je     400c00 <frame_dummy+0x20>
  400bf8:	bf 10 1e 60 00       	mov    edi,0x601e10
  400bfd:	c9                   	leave  
  400bfe:	ff e0                	jmp    rax
  400c00:	c9                   	leave  
  400c01:	c3                   	ret    
  400c02:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  400c09:	00 00 00 
  400c0c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

0000000000400c10 <global constructors keyed to detail::sqrt(float __vector, avxf_t)>:
  400c10:	48 83 ec 08          	sub    rsp,0x8
  400c14:	bf 30 23 60 00       	mov    edi,0x602330
  400c19:	e8 22 fe ff ff       	call   400a40 <std::ios_base::Init::Init()@plt>
  400c1e:	ba a8 20 60 00       	mov    edx,0x6020a8
  400c23:	be 30 23 60 00       	mov    esi,0x602330
  400c28:	bf 90 0a 40 00       	mov    edi,0x400a90
  400c2d:	48 83 c4 08          	add    rsp,0x8
  400c31:	e9 3a fe ff ff       	jmp    400a70 <__cxa_atexit@plt>
  400c36:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  400c3d:	00 00 00 

0000000000400c40 <detail::sqrt(double __vector, ssed_t)>:
  400c40:	c5 f9 51 c0          	vsqrtpd xmm0,xmm0
  400c44:	c3                   	ret    
  400c45:	66 66 2e 0f 1f 84 00 	data32 nop WORD PTR cs:[rax+rax*1+0x0]
  400c4c:	00 00 00 00 

0000000000400c50 <detail::sqrt(float __vector, ssef_t)>:
  400c50:	c5 f8 51 c0          	vsqrtps xmm0,xmm0
  400c54:	c3                   	ret    
  400c55:	66 66 2e 0f 1f 84 00 	data32 nop WORD PTR cs:[rax+rax*1+0x0]
  400c5c:	00 00 00 00 

0000000000400c60 <detail::sqrt(double __vector, avxd_t)>:
  400c60:	55                   	push   rbp
  400c61:	c5 fd 51 c0          	vsqrtpd ymm0,ymm0
  400c65:	48 89 e5             	mov    rbp,rsp
  400c68:	48 83 e4 e0          	and    rsp,0xffffffffffffffe0
  400c6c:	c9                   	leave  
  400c6d:	c3                   	ret    
  400c6e:	66 90                	xchg   ax,ax

0000000000400c70 <detail::sqrt(float __vector, avxf_t)>:
  400c70:	55                   	push   rbp
  400c71:	c5 fc 51 c0          	vsqrtps ymm0,ymm0
  400c75:	48 89 e5             	mov    rbp,rsp
  400c78:	48 83 e4 e0          	and    rsp,0xffffffffffffffe0
  400c7c:	c9                   	leave  
  400c7d:	c3                   	ret    
  400c7e:	66 90                	xchg   ax,ax

0000000000400c80 <operator<<(std::basic_ostream<char, std::char_traits<char> >&, SSEFloat)>:
  400c80:	41 54                	push   r12
  400c82:	ba 01 00 00 00       	mov    edx,0x1
  400c87:	55                   	push   rbp
  400c88:	53                   	push   rbx
  400c89:	48 89 fb             	mov    rbx,rdi
  400c8c:	48 83 ec 30          	sub    rsp,0x30
  400c90:	c5 f8 29 44 24 10    	vmovaps XMMWORD PTR [rsp+0x10],xmm0
  400c96:	48 8d 6c 24 2f       	lea    rbp,[rsp+0x2f]
  400c9b:	c5 fa 10 44 24 1c    	vmovss xmm0,DWORD PTR [rsp+0x1c]
  400ca1:	48 89 ee             	mov    rsi,rbp
  400ca4:	c5 fa 11 44 24 0c    	vmovss DWORD PTR [rsp+0xc],xmm0
  400caa:	c6 44 24 2f 7b       	mov    BYTE PTR [rsp+0x2f],0x7b
  400caf:	c5 fa 10 44 24 18    	vmovss xmm0,DWORD PTR [rsp+0x18]
  400cb5:	c5 fa 11 44 24 08    	vmovss DWORD PTR [rsp+0x8],xmm0
  400cbb:	c5 fa 10 44 24 14    	vmovss xmm0,DWORD PTR [rsp+0x14]
  400cc1:	c5 fa 11 44 24 04    	vmovss DWORD PTR [rsp+0x4],xmm0
  400cc7:	c5 fa 10 44 24 10    	vmovss xmm0,DWORD PTR [rsp+0x10]
  400ccd:	c5 fa 11 04 24       	vmovss DWORD PTR [rsp],xmm0
  400cd2:	e8 19 fe ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400cd7:	c5 fa 5a 04 24       	vcvtss2sd xmm0,xmm0,DWORD PTR [rsp]
  400cdc:	48 89 c7             	mov    rdi,rax
  400cdf:	e8 2c fe ff ff       	call   400b10 <std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)@plt>
  400ce4:	ba 02 00 00 00       	mov    edx,0x2
  400ce9:	49 89 c4             	mov    r12,rax
  400cec:	be f4 11 40 00       	mov    esi,0x4011f4
  400cf1:	48 89 c7             	mov    rdi,rax
  400cf4:	e8 f7 fd ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400cf9:	c5 fa 5a 44 24 04    	vcvtss2sd xmm0,xmm0,DWORD PTR [rsp+0x4]
  400cff:	4c 89 e7             	mov    rdi,r12
  400d02:	e8 09 fe ff ff       	call   400b10 <std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)@plt>
  400d07:	ba 02 00 00 00       	mov    edx,0x2
  400d0c:	49 89 c4             	mov    r12,rax
  400d0f:	be f4 11 40 00       	mov    esi,0x4011f4
  400d14:	48 89 c7             	mov    rdi,rax
  400d17:	e8 d4 fd ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400d1c:	c5 fa 5a 44 24 08    	vcvtss2sd xmm0,xmm0,DWORD PTR [rsp+0x8]
  400d22:	4c 89 e7             	mov    rdi,r12
  400d25:	e8 e6 fd ff ff       	call   400b10 <std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)@plt>
  400d2a:	ba 02 00 00 00       	mov    edx,0x2
  400d2f:	49 89 c4             	mov    r12,rax
  400d32:	be f4 11 40 00       	mov    esi,0x4011f4
  400d37:	48 89 c7             	mov    rdi,rax
  400d3a:	e8 b1 fd ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400d3f:	c5 fa 5a 44 24 0c    	vcvtss2sd xmm0,xmm0,DWORD PTR [rsp+0xc]
  400d45:	4c 89 e7             	mov    rdi,r12
  400d48:	e8 c3 fd ff ff       	call   400b10 <std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)@plt>
  400d4d:	c6 44 24 2f 7d       	mov    BYTE PTR [rsp+0x2f],0x7d
  400d52:	48 89 ee             	mov    rsi,rbp
  400d55:	ba 01 00 00 00       	mov    edx,0x1
  400d5a:	48 89 c7             	mov    rdi,rax
  400d5d:	e8 8e fd ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400d62:	48 83 c4 30          	add    rsp,0x30
  400d66:	48 89 d8             	mov    rax,rbx
  400d69:	5b                   	pop    rbx
  400d6a:	5d                   	pop    rbp
  400d6b:	41 5c                	pop    r12
  400d6d:	c3                   	ret    
  400d6e:	66 90                	xchg   ax,ax

0000000000400d70 <main>:
  400d70:	41 56                	push   r14
  400d72:	41 55                	push   r13
  400d74:	41 54                	push   r12
  400d76:	49 89 f4             	mov    r12,rsi
  400d79:	55                   	push   rbp
  400d7a:	53                   	push   rbx
  400d7b:	48 81 ec a0 00 00 00 	sub    rsp,0xa0
  400d82:	83 ff 01             	cmp    edi,0x1
  400d85:	0f 8e ad 02 00 00    	jle    401038 <stuff_end218+0x175>
  400d8b:	4c 8d ac 24 90 00 00 	lea    r13,[rsp+0x90]
  400d92:	00 
  400d93:	48 8d 94 24 9f 00 00 	lea    rdx,[rsp+0x9f]
  400d9a:	00 
  400d9b:	48 8b 76 08          	mov    rsi,QWORD PTR [rsi+0x8]
  400d9f:	4c 89 ef             	mov    rdi,r13
  400da2:	e8 29 fd ff ff       	call   400ad0 <std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(char const*, std::allocator<char> const&)@plt>
  400da7:	e8 54 fd ff ff       	call   400b00 <__errno_location@plt>
  400dac:	4c 8b b4 24 90 00 00 	mov    r14,QWORD PTR [rsp+0x90]
  400db3:	00 
  400db4:	48 89 c5             	mov    rbp,rax
  400db7:	c7 00 00 00 00 00    	mov    DWORD PTR [rax],0x0
  400dbd:	48 8d 5c 24 78       	lea    rbx,[rsp+0x78]
  400dc2:	4c 89 f7             	mov    rdi,r14
  400dc5:	48 89 de             	mov    rsi,rbx
  400dc8:	e8 13 fd ff ff       	call   400ae0 <strtof@plt>
  400dcd:	4c 3b 74 24 78       	cmp    r14,QWORD PTR [rsp+0x78]
  400dd2:	c5 fa 11 44 24 20    	vmovss DWORD PTR [rsp+0x20],xmm0
  400dd8:	0f 84 c8 02 00 00    	je     4010a6 <stuff_end218+0x1e3>
  400dde:	83 7d 00 22          	cmp    DWORD PTR [rbp+0x0],0x22
  400de2:	0f 84 b4 02 00 00    	je     40109c <stuff_end218+0x1d9>
  400de8:	48 8b bc 24 90 00 00 	mov    rdi,QWORD PTR [rsp+0x90]
  400def:	00 
  400df0:	48 83 ef 18          	sub    rdi,0x18
  400df4:	48 81 ff 00 23 60 00 	cmp    rdi,0x602300
  400dfb:	0f 85 af 02 00 00    	jne    4010b0 <stuff_end218+0x1ed>
  400e01:	49 8b 74 24 10       	mov    rsi,QWORD PTR [r12+0x10]
  400e06:	48 8d 94 24 9e 00 00 	lea    rdx,[rsp+0x9e]
  400e0d:	00 
  400e0e:	4c 8d a4 24 80 00 00 	lea    r12,[rsp+0x80]
  400e15:	00 
  400e16:	4c 89 e7             	mov    rdi,r12
  400e19:	e8 b2 fc ff ff       	call   400ad0 <std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(char const*, std::allocator<char> const&)@plt>
  400e1e:	4c 8b ac 24 80 00 00 	mov    r13,QWORD PTR [rsp+0x80]
  400e25:	00 
  400e26:	c7 45 00 00 00 00 00 	mov    DWORD PTR [rbp+0x0],0x0
  400e2d:	48 89 de             	mov    rsi,rbx
  400e30:	4c 89 ef             	mov    rdi,r13
  400e33:	e8 a8 fc ff ff       	call   400ae0 <strtof@plt>
  400e38:	4c 3b 6c 24 78       	cmp    r13,QWORD PTR [rsp+0x78]
  400e3d:	0f 84 4f 02 00 00    	je     401092 <stuff_end218+0x1cf>
  400e43:	83 7d 00 22          	cmp    DWORD PTR [rbp+0x0],0x22
  400e47:	0f 84 3b 02 00 00    	je     401088 <stuff_end218+0x1c5>
  400e4d:	48 8b bc 24 80 00 00 	mov    rdi,QWORD PTR [rsp+0x80]
  400e54:	00 
  400e55:	b8 00 23 60 00       	mov    eax,0x602300
  400e5a:	48 83 ef 18          	sub    rdi,0x18
  400e5e:	48 39 f8             	cmp    rax,rdi
  400e61:	0f 85 e7 01 00 00    	jne    40104e <stuff_end218+0x18b>
  400e67:	c5 fa 10 4c 24 20    	vmovss xmm1,DWORD PTR [rsp+0x20]
  400e6d:	c5 f0 c6 d1 00       	vshufps xmm2,xmm1,xmm1,0x0

0000000000400e72 <stuff_begin201>:
  400e72:	c5 f8 28 25 b6 03 00 	vmovaps xmm4,XMMWORD PTR [rip+0x3b6]        # 401230 <_IO_stdin_used+0x40>
  400e79:	00 
  400e7a:	c5 f8 c6 c8 00       	vshufps xmm1,xmm0,xmm0,0x0
  400e7f:	c5 e8 5c d9          	vsubps xmm3,xmm2,xmm1
  400e83:	c5 e8 58 c1          	vaddps xmm0,xmm2,xmm1
  400e87:	c5 f8 29 5c 24 20    	vmovaps XMMWORD PTR [rsp+0x20],xmm3
  400e8d:	c5 e8 59 d9          	vmulps xmm3,xmm2,xmm1
  400e91:	c5 e8 5e d1          	vdivps xmm2,xmm2,xmm1
  400e95:	c5 f8 29 5c 24 30    	vmovaps XMMWORD PTR [rsp+0x30],xmm3
  400e9b:	c5 f8 29 54 24 40    	vmovaps XMMWORD PTR [rsp+0x40],xmm2
  400ea1:	c5 f8 52 d1          	vrsqrtps xmm2,xmm1
  400ea5:	c5 f0 59 da          	vmulps xmm3,xmm1,xmm2
  400ea9:	c5 e0 59 da          	vmulps xmm3,xmm3,xmm2
  400ead:	c5 d8 5c db          	vsubps xmm3,xmm4,xmm3
  400eb1:	c5 e8 59 d3          	vmulps xmm2,xmm2,xmm3
  400eb5:	c5 e8 59 15 83 03 00 	vmulps xmm2,xmm2,XMMWORD PTR [rip+0x383]        # 401240 <_IO_stdin_used+0x50>
  400ebc:	00 
  400ebd:	c5 f8 29 54 24 50    	vmovaps XMMWORD PTR [rsp+0x50],xmm2

0000000000400ec3 <stuff_end218>:
  400ec3:	ba 06 00 00 00       	mov    edx,0x6
  400ec8:	c5 f8 51 c9          	vsqrtps xmm1,xmm1
  400ecc:	be 01 12 40 00       	mov    esi,0x401201
  400ed1:	c5 f8 29 4c 24 60    	vmovaps XMMWORD PTR [rsp+0x60],xmm1
  400ed7:	bf e0 21 60 00       	mov    edi,0x6021e0
  400edc:	c5 f8 29 04 24       	vmovaps XMMWORD PTR [rsp],xmm0
  400ee1:	e8 0a fc ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400ee6:	c5 f8 28 04 24       	vmovaps xmm0,XMMWORD PTR [rsp]
  400eeb:	bf e0 21 60 00       	mov    edi,0x6021e0
  400ef0:	e8 8b fd ff ff       	call   400c80 <operator<<(std::basic_ostream<char, std::char_traits<char> >&, SSEFloat)>
  400ef5:	ba 01 00 00 00       	mov    edx,0x1
  400efa:	48 89 de             	mov    rsi,rbx
  400efd:	48 89 c7             	mov    rdi,rax
  400f00:	c6 44 24 78 0a       	mov    BYTE PTR [rsp+0x78],0xa
  400f05:	e8 e6 fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400f0a:	ba 07 00 00 00       	mov    edx,0x7
  400f0f:	48 89 c5             	mov    rbp,rax
  400f12:	be 08 12 40 00       	mov    esi,0x401208
  400f17:	48 89 c7             	mov    rdi,rax
  400f1a:	e8 d1 fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400f1f:	c5 f8 28 44 24 20    	vmovaps xmm0,XMMWORD PTR [rsp+0x20]
  400f25:	48 89 ef             	mov    rdi,rbp
  400f28:	e8 53 fd ff ff       	call   400c80 <operator<<(std::basic_ostream<char, std::char_traits<char> >&, SSEFloat)>
  400f2d:	ba 01 00 00 00       	mov    edx,0x1
  400f32:	48 89 de             	mov    rsi,rbx
  400f35:	48 89 c7             	mov    rdi,rax
  400f38:	c6 44 24 78 0a       	mov    BYTE PTR [rsp+0x78],0xa
  400f3d:	e8 ae fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400f42:	ba 07 00 00 00       	mov    edx,0x7
  400f47:	48 89 c5             	mov    rbp,rax
  400f4a:	be 10 12 40 00       	mov    esi,0x401210
  400f4f:	48 89 c7             	mov    rdi,rax
  400f52:	e8 99 fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400f57:	c5 f8 28 44 24 30    	vmovaps xmm0,XMMWORD PTR [rsp+0x30]
  400f5d:	48 89 ef             	mov    rdi,rbp
  400f60:	e8 1b fd ff ff       	call   400c80 <operator<<(std::basic_ostream<char, std::char_traits<char> >&, SSEFloat)>
  400f65:	ba 01 00 00 00       	mov    edx,0x1
  400f6a:	48 89 de             	mov    rsi,rbx
  400f6d:	48 89 c7             	mov    rdi,rax
  400f70:	c6 44 24 78 0a       	mov    BYTE PTR [rsp+0x78],0xa
  400f75:	e8 76 fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400f7a:	ba 07 00 00 00       	mov    edx,0x7
  400f7f:	48 89 c5             	mov    rbp,rax
  400f82:	be 18 12 40 00       	mov    esi,0x401218
  400f87:	48 89 c7             	mov    rdi,rax
  400f8a:	e8 61 fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400f8f:	c5 f8 28 44 24 40    	vmovaps xmm0,XMMWORD PTR [rsp+0x40]
  400f95:	48 89 ef             	mov    rdi,rbp
  400f98:	e8 e3 fc ff ff       	call   400c80 <operator<<(std::basic_ostream<char, std::char_traits<char> >&, SSEFloat)>
  400f9d:	ba 01 00 00 00       	mov    edx,0x1
  400fa2:	48 89 de             	mov    rsi,rbx
  400fa5:	48 89 c7             	mov    rdi,rax
  400fa8:	c6 44 24 78 0a       	mov    BYTE PTR [rsp+0x78],0xa
  400fad:	e8 3e fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400fb2:	ba 07 00 00 00       	mov    edx,0x7
  400fb7:	48 89 c5             	mov    rbp,rax
  400fba:	be 20 12 40 00       	mov    esi,0x401220
  400fbf:	48 89 c7             	mov    rdi,rax
  400fc2:	e8 29 fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400fc7:	c5 f8 28 44 24 60    	vmovaps xmm0,XMMWORD PTR [rsp+0x60]
  400fcd:	48 89 ef             	mov    rdi,rbp
  400fd0:	e8 ab fc ff ff       	call   400c80 <operator<<(std::basic_ostream<char, std::char_traits<char> >&, SSEFloat)>
  400fd5:	ba 01 00 00 00       	mov    edx,0x1
  400fda:	48 89 de             	mov    rsi,rbx
  400fdd:	48 89 c7             	mov    rdi,rax
  400fe0:	c6 44 24 78 0a       	mov    BYTE PTR [rsp+0x78],0xa
  400fe5:	e8 06 fb ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400fea:	ba 07 00 00 00       	mov    edx,0x7
  400fef:	be 28 12 40 00       	mov    esi,0x401228
  400ff4:	48 89 c5             	mov    rbp,rax
  400ff7:	48 89 c7             	mov    rdi,rax
  400ffa:	e8 f1 fa ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  400fff:	c5 f8 28 44 24 50    	vmovaps xmm0,XMMWORD PTR [rsp+0x50]
  401005:	48 89 ef             	mov    rdi,rbp
  401008:	e8 73 fc ff ff       	call   400c80 <operator<<(std::basic_ostream<char, std::char_traits<char> >&, SSEFloat)>
  40100d:	c6 44 24 78 0a       	mov    BYTE PTR [rsp+0x78],0xa
  401012:	ba 01 00 00 00       	mov    edx,0x1
  401017:	48 89 de             	mov    rsi,rbx
  40101a:	48 89 c7             	mov    rdi,rax
  40101d:	e8 ce fa ff ff       	call   400af0 <std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long)@plt>
  401022:	31 c0                	xor    eax,eax
  401024:	48 81 c4 a0 00 00 00 	add    rsp,0xa0
  40102b:	5b                   	pop    rbx
  40102c:	5d                   	pop    rbp
  40102d:	41 5c                	pop    r12
  40102f:	41 5d                	pop    r13
  401031:	41 5e                	pop    r14
  401033:	c3                   	ret    
  401034:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]
  401038:	be f7 11 40 00       	mov    esi,0x4011f7
  40103d:	bf c0 20 60 00       	mov    edi,0x6020c0
  401042:	e8 59 fa ff ff       	call   400aa0 <std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)@plt>
  401047:	b8 ff ff ff ff       	mov    eax,0xffffffff
  40104c:	eb d6                	jmp    401024 <stuff_end218+0x161>
  40104e:	b8 00 00 00 00       	mov    eax,0x0
  401053:	48 85 c0             	test   rax,rax
  401056:	0f 84 a6 00 00 00    	je     401102 <stuff_end218+0x23f>
  40105c:	83 c8 ff             	or     eax,0xffffffff
  40105f:	f0 0f c1 47 10       	lock xadd DWORD PTR [rdi+0x10],eax
  401064:	85 c0                	test   eax,eax
  401066:	0f 8f fb fd ff ff    	jg     400e67 <main+0xf7>
  40106c:	c5 fa 11 04 24       	vmovss DWORD PTR [rsp],xmm0
  401071:	48 8d b4 24 9c 00 00 	lea    rsi,[rsp+0x9c]
  401078:	00 
  401079:	e8 42 fa ff ff       	call   400ac0 <std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Rep::_M_destroy(std::allocator<char> const&)@plt>
  40107e:	c5 fa 10 04 24       	vmovss xmm0,DWORD PTR [rsp]
  401083:	e9 df fd ff ff       	jmp    400e67 <main+0xf7>
  401088:	bf fc 11 40 00       	mov    edi,0x4011fc
  40108d:	e8 ee f9 ff ff       	call   400a80 <std::__throw_out_of_range(char const*)@plt>
  401092:	bf fc 11 40 00       	mov    edi,0x4011fc
  401097:	e8 c4 f9 ff ff       	call   400a60 <std::__throw_invalid_argument(char const*)@plt>
  40109c:	bf fc 11 40 00       	mov    edi,0x4011fc
  4010a1:	e8 da f9 ff ff       	call   400a80 <std::__throw_out_of_range(char const*)@plt>
  4010a6:	bf fc 11 40 00       	mov    edi,0x4011fc
  4010ab:	e8 b0 f9 ff ff       	call   400a60 <std::__throw_invalid_argument(char const*)@plt>
  4010b0:	b8 00 00 00 00       	mov    eax,0x0
  4010b5:	48 85 c0             	test   rax,rax
  4010b8:	74 35                	je     4010ef <stuff_end218+0x22c>
  4010ba:	83 c8 ff             	or     eax,0xffffffff
  4010bd:	f0 0f c1 47 10       	lock xadd DWORD PTR [rdi+0x10],eax
  4010c2:	85 c0                	test   eax,eax
  4010c4:	0f 8f 37 fd ff ff    	jg     400e01 <main+0x91>
  4010ca:	48 8d b4 24 9d 00 00 	lea    rsi,[rsp+0x9d]
  4010d1:	00 
  4010d2:	e8 e9 f9 ff ff       	call   400ac0 <std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Rep::_M_destroy(std::allocator<char> const&)@plt>
  4010d7:	e9 25 fd ff ff       	jmp    400e01 <main+0x91>
  4010dc:	48 89 c3             	mov    rbx,rax
  4010df:	4c 89 e7             	mov    rdi,r12
  4010e2:	e8 c9 f9 ff ff       	call   400ab0 <std::basic_string<char, std::char_traits<char>, std::allocator<char> >::~basic_string()@plt>
  4010e7:	48 89 df             	mov    rdi,rbx
  4010ea:	e8 41 fa ff ff       	call   400b30 <_Unwind_Resume@plt>
  4010ef:	8b 47 10             	mov    eax,DWORD PTR [rdi+0x10]
  4010f2:	8d 50 ff             	lea    edx,[rax-0x1]
  4010f5:	89 57 10             	mov    DWORD PTR [rdi+0x10],edx
  4010f8:	eb c8                	jmp    4010c2 <stuff_end218+0x1ff>
  4010fa:	48 89 c3             	mov    rbx,rax
  4010fd:	eb e8                	jmp    4010e7 <stuff_end218+0x224>
  4010ff:	90                   	nop
  401100:	eb f8                	jmp    4010fa <stuff_end218+0x237>
  401102:	8b 47 10             	mov    eax,DWORD PTR [rdi+0x10]
  401105:	8d 50 ff             	lea    edx,[rax-0x1]
  401108:	89 57 10             	mov    DWORD PTR [rdi+0x10],edx
  40110b:	e9 54 ff ff ff       	jmp    401064 <stuff_end218+0x1a1>
  401110:	48 89 c3             	mov    rbx,rax
  401113:	4c 89 ef             	mov    rdi,r13
  401116:	e8 95 f9 ff ff       	call   400ab0 <std::basic_string<char, std::char_traits<char>, std::allocator<char> >::~basic_string()@plt>
  40111b:	eb ca                	jmp    4010e7 <stuff_end218+0x224>
  40111d:	0f 1f 00             	nop    DWORD PTR [rax]

0000000000401120 <__libc_csu_init>:
  401120:	41 57                	push   r15
  401122:	41 89 ff             	mov    r15d,edi
  401125:	41 56                	push   r14
  401127:	49 89 f6             	mov    r14,rsi
  40112a:	41 55                	push   r13
  40112c:	49 89 d5             	mov    r13,rdx
  40112f:	41 54                	push   r12
  401131:	4c 8d 25 b0 0c 20 00 	lea    r12,[rip+0x200cb0]        # 601de8 <__init_array_start>
  401138:	55                   	push   rbp
  401139:	48 8d 2d b0 0c 20 00 	lea    rbp,[rip+0x200cb0]        # 601df0 <__init_array_end>
  401140:	53                   	push   rbx
  401141:	4c 29 e5             	sub    rbp,r12
  401144:	31 db                	xor    ebx,ebx
  401146:	48 c1 fd 03          	sar    rbp,0x3
  40114a:	48 83 ec 08          	sub    rsp,0x8
  40114e:	e8 9d f8 ff ff       	call   4009f0 <_init>
  401153:	48 85 ed             	test   rbp,rbp
  401156:	74 1e                	je     401176 <__libc_csu_init+0x56>
  401158:	0f 1f 84 00 00 00 00 	nop    DWORD PTR [rax+rax*1+0x0]
  40115f:	00 
  401160:	4c 89 ea             	mov    rdx,r13
  401163:	4c 89 f6             	mov    rsi,r14
  401166:	44 89 ff             	mov    edi,r15d
  401169:	41 ff 14 dc          	call   QWORD PTR [r12+rbx*8]
  40116d:	48 83 c3 01          	add    rbx,0x1
  401171:	48 39 eb             	cmp    rbx,rbp
  401174:	75 ea                	jne    401160 <__libc_csu_init+0x40>
  401176:	48 83 c4 08          	add    rsp,0x8
  40117a:	5b                   	pop    rbx
  40117b:	5d                   	pop    rbp
  40117c:	41 5c                	pop    r12
  40117e:	41 5d                	pop    r13
  401180:	41 5e                	pop    r14
  401182:	41 5f                	pop    r15
  401184:	c3                   	ret    
  401185:	66 66 2e 0f 1f 84 00 	data32 nop WORD PTR cs:[rax+rax*1+0x0]
  40118c:	00 00 00 00 

0000000000401190 <__libc_csu_fini>:
  401190:	f3 c3                	repz ret 
  401192:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  401199:	00 00 00 
  40119c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004011a0 <__do_global_ctors_aux>:
  4011a0:	55                   	push   rbp
  4011a1:	48 89 e5             	mov    rbp,rsp
  4011a4:	53                   	push   rbx
  4011a5:	48 83 ec 08          	sub    rsp,0x8
  4011a9:	48 8b 05 40 0c 20 00 	mov    rax,QWORD PTR [rip+0x200c40]        # 601df0 <__init_array_end>
  4011b0:	48 83 f8 ff          	cmp    rax,0xffffffffffffffff
  4011b4:	74 19                	je     4011cf <__do_global_ctors_aux+0x2f>
  4011b6:	bb f0 1d 60 00       	mov    ebx,0x601df0
  4011bb:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]
  4011c0:	48 83 eb 08          	sub    rbx,0x8
  4011c4:	ff d0                	call   rax
  4011c6:	48 8b 03             	mov    rax,QWORD PTR [rbx]
  4011c9:	48 83 f8 ff          	cmp    rax,0xffffffffffffffff
  4011cd:	75 f1                	jne    4011c0 <__do_global_ctors_aux+0x20>
  4011cf:	48 83 c4 08          	add    rsp,0x8
  4011d3:	5b                   	pop    rbx
  4011d4:	c9                   	leave  
  4011d5:	c3                   	ret    
