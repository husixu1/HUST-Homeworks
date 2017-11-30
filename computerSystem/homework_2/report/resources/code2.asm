08048bb4 <phase_2>:
 8048bb4:	56                   	push   %esi
 8048bb5:	53                   	push   %ebx
 8048bb6:	83 ec 34             	sub    $0x34,%esp

////// parameter
 8048bb9:	8d 44 24 18          	lea    0x18(%esp),%eax
 8048bbd:	89 44 24 04          	mov    %eax,0x4(%esp)

 8048bc1:	8b 44 24 40          	mov    0x40(%esp),%eax
 8048bc5:	89 04 24             	mov    %eax,(%esp)
//////
 8048bc8:	e8 37 07 00 00       	call   8049304 <read_six_numbers>
 8048bcd:	83 7c 24 18 01       	cmpl   $0x1,0x18(%esp)
 8048bd2:	74 1e                	je     8048bf2 <phase_2+0x3e> //je L1

 8048bd4:	e8 ec 06 00 00       	call   80492c5 <explode_bomb>
 8048bd9:	eb 17                	jmp    8048bf2 <phase_2+0x3e> // jmp L1

LOOP:
 8048bdb:	8b 43 fc             	mov    -0x4(%ebx),%eax
 8048bde:	01 c0                	add    %eax,%eax
 8048be0:	39 03                	cmp    %eax,(%ebx)
 8048be2:	74 05                	je     8048be9 <phase_2+0x35>
 8048be4:	e8 dc 06 00 00       	call   80492c5 <explode_bomb>
 8048be9:	83 c3 04             	add    $0x4,%ebx
 8048bec:	39 f3                	cmp    %esi,%ebx
 8048bee:	75 eb                	jne    8048bdb <phase_2+0x27> // LOOP
 8048bf0:	eb 0a                	jmp    8048bfc <phase_2+0x48> // LOOP end

L1:
 8048bf2:	8d 5c 24 1c          	lea    0x1c(%esp),%ebx
 8048bf6:	8d 74 24 30          	lea    0x30(%esp),%esi
 8048bfa:	eb df                	jmp    8048bdb <phase_2+0x27> // LOOP

 8048bfc:	83 c4 34             	add    $0x34,%esp
 8048bff:	5b                   	pop    %ebx
 8048c00:	5e                   	pop    %esi
 8048c01:	c3                   	ret
