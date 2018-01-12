08048d8b <phase_5>:
 8048d8b:	83 ec 2c             	sub    $0x2c,%esp

///// parameter
 8048d8e:	8d 44 24 1c          	lea    0x1c(%esp),%eax
 8048d92:	89 44 24 0c          	mov    %eax,0xc(%esp)

 8048d96:	8d 44 24 18          	lea    0x18(%esp),%eax
 8048d9a:	89 44 24 08          	mov    %eax,0x8(%esp)

 8048d9e:	c7 44 24 04 b1 a5 04 	movl   $0x804a5b1,0x4(%esp)
 8048da5:	08

 8048da6:	8b 44 24 30          	mov    0x30(%esp),%eax
 8048daa:	89 04 24             	mov    %eax,(%esp)
/////
 8048dad:	e8 ae fa ff ff       	call   8048860 <__isoc99_sscanf@plt>
 8048db2:	83 f8 01             	cmp    $0x1,%eax
 8048db5:	7f 05                	jg     8048dbc <phase_5+0x31>
 8048db7:	e8 09 05 00 00       	call   80492c5 <explode_bomb>


 8048dbc:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048dc0:	83 e0 0f             	and    $0xf,%eax
 8048dc3:	89 44 24 18          	mov    %eax,0x18(%esp)
 8048dc7:	83 f8 0f             	cmp    $0xf,%eax
 8048dca:	74 2a                	je     8048df6 <phase_5+0x6b>   // BOOM!

 8048dcc:	b9 00 00 00 00       	mov    $0x0,%ecx
 8048dd1:	ba 00 00 00 00       	mov    $0x0,%edx

 8048dd6:	83 c2 01             	add    $0x1,%edx				// L1
 8048dd9:	8b 04 85 a0 a3 04 08 	mov    0x804a3a0(,%eax,4),%eax

 8048de0:	01 c1                	add    %eax,%ecx
 8048de2:	83 f8 0f             	cmp    $0xf,%eax
 8048de5:	75 ef                	jne    8048dd6 <phase_5+0x4b>	// jne L1 (loop)

 8048de7:	89 44 24 18          	mov    %eax,0x18(%esp)
 8048deb:	83 fa 0f             	cmp    $0xf,%edx
 8048dee:	75 06                	jne    8048df6 <phase_5+0x6b>	// BOOM!

 8048df0:	3b 4c 24 1c          	cmp    0x1c(%esp),%ecx
 8048df4:	74 05                	je     8048dfb <phase_5+0x70>

 8048df6:	e8 ca 04 00 00       	call   80492c5 <explode_bomb>

 8048dfb:	83 c4 2c             	add    $0x2c,%esp
