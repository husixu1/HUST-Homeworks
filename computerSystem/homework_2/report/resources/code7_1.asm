0804944e <phase_defused>:
 804944e:	81 ec 8c 00 00 00    	sub    $0x8c,%esp
 8049454:	65 a1 14 00 00 00    	mov    %gs:0x14,%eax
 804945a:	89 44 24 7c          	mov    %eax,0x7c(%esp)
 804945e:	31 c0                	xor    %eax,%eax
 8049460:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
 8049467:	e8 4b fd ff ff       	call   80491b7 <send_msg>
 804946c:	83 3d e8 c7 04 08 06 	cmpl   $0x6,0x804c7e8
 8049473:	75 7e                	jne    80494f3 <phase_defused+0xa5>

///// parameter
 8049475:	8d 44 24 2c          	lea    0x2c(%esp),%eax
 8049479:	89 44 24 10          	mov    %eax,0x10(%esp)

 804947d:	8d 44 24 28          	lea    0x28(%esp),%eax
 8049481:	89 44 24 0c          	mov    %eax,0xc(%esp)

 8049485:	8d 44 24 24          	lea    0x24(%esp),%eax
 8049489:	89 44 24 08          	mov    %eax,0x8(%esp)

 804948d:	c7 44 24 04 0b a6 04 	movl   $0x804a60b,0x4(%esp)

 8049494:	08
 8049495:	c7 04 24 f0 c8 04 08 	movl   $0x804c8f0,(%esp)
/////
 804949c:	e8 bf f3 ff ff       	call   8048860 <__isoc99_sscanf@plt>
 80494a1:	83 f8 03             	cmp    $0x3,%eax
 80494a4:	75 35                	jne    80494db <phase_defused+0x8d>

///// parameter
 80494a6:	c7 44 24 04 14 a6 04 	movl   $0x804a614,0x4(%esp)
 80494ad:	08
 80494ae:	8d 44 24 2c          	lea    0x2c(%esp),%eax
 80494b2:	89 04 24             	mov    %eax,(%esp)
/////
 80494b5:	e8 90 fb ff ff       	call   804904a <strings_not_equal>
 80494ba:	85 c0                	test   %eax,%eax
 80494bc:	75 1d                	jne    80494db <phase_defused+0x8d>

 80494be:	c7 04 24 60 a4 04 08 	movl   $0x804a460,(%esp)
 80494c5:	e8 26 f3 ff ff       	call   80487f0 <puts@plt>
 80494ca:	c7 04 24 88 a4 04 08 	movl   $0x804a488,(%esp)
 80494d1:	e8 1a f3 ff ff       	call   80487f0 <puts@plt>
 80494d6:	e8 63 fa ff ff       	call   8048f3e <secret_phase>

 80494db:	c7 04 24 c0 a4 04 08 	movl   $0x804a4c0,(%esp)
 80494e2:	e8 09 f3 ff ff       	call   80487f0 <puts@plt>
 80494e7:	c7 04 24 ec a4 04 08 	movl   $0x804a4ec,(%esp)
 80494ee:	e8 fd f2 ff ff       	call   80487f0 <puts@plt>

