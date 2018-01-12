08048d22 <phase_4>:
 8048d22:	83 ec 2c             	sub    $0x2c,%esp

///// parameter
 8048d25:	8d 44 24 1c          	lea    0x1c(%esp),%eax
 8048d29:	89 44 24 0c          	mov    %eax,0xc(%esp)

 8048d2d:	8d 44 24 18          	lea    0x18(%esp),%eax
 8048d31:	89 44 24 08          	mov    %eax,0x8(%esp)

 8048d35:	c7 44 24 04 b1 a5 04 	movl   $0x804a5b1,0x4(%esp)
 8048d3c:	08

 8048d3d:	8b 44 24 30          	mov    0x30(%esp),%eax			// parameter
 8048d41:	89 04 24             	mov    %eax,(%esp)
/////
 8048d44:	e8 17 fb ff ff       	call   8048860 <__isoc99_sscanf@plt>

 8048d49:	83 f8 02             	cmp    $0x2,%eax
 8048d4c:	75 07                	jne    8048d55 <phase_4+0x33>

 8048d4e:	83 7c 24 18 0e       	cmpl   $0xe,0x18(%esp)
 8048d53:	76 05                	jbe    8048d5a <phase_4+0x38>	// a less than or equal to 0xe
 8048d55:	e8 6b 05 00 00       	call   80492c5 <explode_bomb>

///// parameter
 8048d5a:	c7 44 24 08 0e 00 00 	movl   $0xe,0x8(%esp)
 8048d61:	00
 8048d62:	c7 44 24 04 00 00 00 	movl   $0x0,0x4(%esp)
 8048d69:	00
 8048d6a:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048d6e:	89 04 24             	mov    %eax,(%esp)
/////
 8048d71:	e8 4b ff ff ff       	call   8048cc1 <func4>

 8048d76:	83 f8 06             	cmp    $0x6,%eax
 8048d79:	75 07                	jne    8048d82 <phase_4+0x60>

 8048d7b:	83 7c 24 1c 06       	cmpl   $0x6,0x1c(%esp)
 8048d80:	74 05                	je     8048d87 <phase_4+0x65>

 8048d82:	e8 3e 05 00 00       	call   80492c5 <explode_bomb>

 8048d87:	83 c4 2c             	add    $0x2c,%esp
 8048d8a:	c3                   	ret


