08048f3e <secret_phase>:
 8048f3e:	53                   	push   %ebx
 8048f3f:	83 ec 18             	sub    $0x18,%esp
 8048f42:	e8 0d 04 00 00       	call   8049354 <read_line>

///// parameter
 8048f47:	c7 44 24 08 0a 00 00 	movl   $0xa,0x8(%esp)
 8048f4e:	00
 8048f4f:	c7 44 24 04 00 00 00 	movl   $0x0,0x4(%esp)
 8048f56:	00
 8048f57:	89 04 24             	mov    %eax,(%esp)
/////
 8048f5a:	e8 71 f9 ff ff       	call   80488d0 <strtol@plt>

 8048f5f:	89 c3                	mov    %eax,%ebx
 8048f61:	8d 40 ff             	lea    -0x1(%eax),%eax				// eax -= 1
 8048f64:	3d e8 03 00 00       	cmp    $0x3e8,%eax					// eax ? 0x3e8
 8048f69:	76 05                	jbe    8048f70 <secret_phase+0x32>	// ===> para1 <= 0x3e9

 8048f6b:	e8 55 03 00 00       	call   80492c5 <explode_bomb>

///// parameter
 8048f70:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048f74:	c7 04 24 a0 c0 04 08 	movl   $0x804c0a0,(%esp)
/////
 8048f7b:	e8 6d ff ff ff       	call   8048eed <fun7>

 8048f80:	85 c0                	test   %eax,%eax
 8048f82:	74 05                	je     8048f89 <secret_phase+0x4b>
 8048f84:	e8 3c 03 00 00       	call   80492c5 <explode_bomb>

 8048f89:	c7 04 24 58 a3 04 08 	movl   $0x804a358,(%esp)
 8048f90:	e8 5b f8 ff ff       	call   80487f0 <puts@plt>
 8048f95:	e8 b4 04 00 00       	call   804944e <phase_defused>
 8048f9a:	83 c4 18             	add    $0x18,%esp
 8048f9d:	5b                   	pop    %ebx
 8048f9e:	c3                   	ret
 8048f9f:	90                   	nop


