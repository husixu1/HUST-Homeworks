08048c02 <phase_3>:
 8048c02:	83 ec 2c             	sub    $0x2c,%esp

// ===
 8048c05:	8d 44 24 1c          	lea    0x1c(%esp),%eax			// a pointer on stack
 8048c09:	89 44 24 0c          	mov    %eax,0xc(%esp)

 8048c0d:	8d 44 24 18          	lea    0x18(%esp),%eax			// a pointer on stack
 8048c11:	89 44 24 08          	mov    %eax,0x8(%esp)

 8048c15:	c7 44 24 04 b1 a5 04 	movl   $0x804a5b1,0x4(%esp)
 8048c1d:	8b 44 24 30          	mov    0x30(%esp),%eax			// input parameter

 8048c21:	89 04 24             	mov    %eax,(%esp)
// ===
 8048c24:	e8 37 fc ff ff       	call   8048860 <__isoc99_sscanf@plt>
 8048c29:	83 f8 01             	cmp    $0x1,%eax
 8048c2c:	7f 05                	jg     8048c33 <phase_3+0x31>
 8048c2e:	e8 92 06 00 00       	call   80492c5 <explode_bomb>

 8048c33:	83 7c 24 18 07       	cmpl   $0x7,0x18(%esp)
 8048c38:	77 66                	ja     8048ca0 <phase_3+0x9e>    boom

 8048c3a:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048c3e:	ff 24 85 80 a3 04 08 	jmp    *0x804a380(,%eax,4)
 8048c45:	b8 00 00 00 00       	mov    $0x0,%eax
 8048c4a:	eb 05                	jmp    8048c51 <phase_3+0x4f>
 8048c4c:	b8 c1 03 00 00       	mov    $0x3c1,%eax
 8048c51:	2d 3c 02 00 00       	sub    $0x23c,%eax
 8048c56:	eb 05                	jmp    8048c5d <phase_3+0x5b>
 8048c58:	b8 00 00 00 00       	mov    $0x0,%eax
 8048c5d:	05 a4 01 00 00       	add    $0x1a4,%eax
 8048c62:	eb 05                	jmp    8048c69 <phase_3+0x67>
 8048c64:	b8 00 00 00 00       	mov    $0x0,%eax
 8048c69:	2d 41 01 00 00       	sub    $0x141,%eax
 8048c6e:	eb 05                	jmp    8048c75 <phase_3+0x73>
 8048c70:	b8 00 00 00 00       	mov    $0x0,%eax
 8048c75:	05 41 01 00 00       	add    $0x141,%eax
 8048c7a:	eb 05                	jmp    8048c81 <phase_3+0x7f>
 8048c7c:	b8 00 00 00 00       	mov    $0x0,%eax
 8048c81:	2d 41 01 00 00       	sub    $0x141,%eax
 8048c86:	eb 05                	jmp    8048c8d <phase_3+0x8b>
 8048c88:	b8 00 00 00 00       	mov    $0x0,%eax
 8048c8d:	05 41 01 00 00       	add    $0x141,%eax
 8048c92:	eb 05                	jmp    8048c99 <phase_3+0x97>
 8048c94:	b8 00 00 00 00       	mov    $0x0,%eax
 8048c99:	2d 41 01 00 00       	sub    $0x141,%eax
 8048c9e:	eb 0a                	jmp    8048caa <phase_3+0xa8>
