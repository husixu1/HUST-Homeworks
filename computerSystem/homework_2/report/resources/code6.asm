08048dff <phase_6>:
 8048dff:	56                   	push   %esi
 8048e00:	53                   	push   %ebx
 8048e01:	83 ec 44             	sub    $0x44,%esp

///// parameter
 8048e04:	8d 44 24 10          	lea    0x10(%esp),%eax
 8048e08:	89 44 24 04          	mov    %eax,0x4(%esp)			// address

 8048e0c:	8b 44 24 50          	mov    0x50(%esp),%eax
 8048e10:	89 04 24             	mov    %eax,(%esp)				// input
/////
 8048e13:	e8 ec 04 00 00       	call   8049304 <read_six_numbers>
 8048e18:	be 00 00 00 00       	mov    $0x0,%esi				// esi = 0

L2:
 8048e1d:	8b 44 b4 10          	mov    0x10(%esp,%esi,4),%eax	// input [esi] -> eax
 8048e21:	83 e8 01             	sub    $0x1,%eax				// eax -= 1
 8048e24:	83 f8 05             	cmp    $0x5,%eax				// eax ? 5
 8048e27:	76 05                	jbe    8048e2e <phase_6+0x2f>	// eax <= 5,  					===> input[0] <= 6

 8048e29:	e8 97 04 00 00       	call   80492c5 <explode_bomb>

 8048e2e:	83 c6 01             	add    $0x1,%esi				// esi += 1
 8048e31:	83 fe 06             	cmp    $0x6,%esi				// esi ? 6
 8048e34:	74 1c                	je     8048e52 <phase_6+0x53>	// esi == 6,	jmp end1

																	// esi!=6 loop, starting with 1
 8048e36:	89 f3                	mov    %esi,%ebx				// esi -> ebx
L1:
 8048e38:	8b 44 9c 10          	mov    0x10(%esp,%ebx,4),%eax	// inpt[ebx] -> eax
 8048e3c:	39 44 b4 0c          	cmp    %eax,0xc(%esp,%esi,4)	// eax ? input[esi - 1]
 8048e40:	75 05                	jne    8048e47 <phase_6+0x48>	// 								===> input[m-1] != input[m-1]

 8048e42:	e8 7e 04 00 00       	call   80492c5 <explode_bomb>

 8048e47:	83 c3 01             	add    $0x1,%ebx				// ebx += 1
 8048e4a:	83 fb 05             	cmp    $0x5,%ebx				// ebx ? 5
 8048e4d:	7e e9                	jle    8048e38 <phase_6+0x39>	// ebx <= 5, jmp L1
 8048e4f:	90                   	nop
 8048e50:	eb cb                	jmp    8048e1d <phase_6+0x1e>	// ebx > 5 jmp L2				===> input[i] != input[j]

//=== part 2
end1:
 8048e52:	8d 44 24 10          	lea    0x10(%esp),%eax			// input	-> eax
 8048e56:	8d 5c 24 28          	lea    0x28(%esp),%ebx			// input + 6 -> ebx (over tail)
 8048e5a:	b9 07 00 00 00       	mov    $0x7,%ecx				// 7-> ecx

loop_S:
 8048e5f:	89 ca                	mov    %ecx,%edx				// ecx -> edx
 8048e61:	2b 10                	sub    (%eax),%edx				// edx -= *eax
 8048e63:	89 10                	mov    %edx,(%eax)				// edx -> *eax
 8048e65:	83 c0 04             	add    $0x4,%eax				// eax += sizeof(int)
 8048e68:	39 d8                	cmp    %ebx,%eax				// eax ? ebx
 8048e6a:	75 f3                	jne    8048e5f <phase_6+0x60>	// loop 						==> for index,i in tab: 7-i -> tab[index]


//=== part3
 8048e6c:	bb 00 00 00 00       	mov    $0x0,%ebx				// 0 -> ebx
 8048e71:	eb 1d                	jmp    8048e90 <phase_6+0x91>	// jmp L4

L7:																									//===> tab[i+6] = & structtab[i]
 8048e73:	8b 52 08             	mov    0x8(%edx),%edx			// *(edx+8) -> edx		edx(struct).next -> edx
 8048e76:	83 c0 01             	add    $0x1,%eax				// eax += 1 (eax = 2)	|
 8048e79:	39 c8                	cmp    %ecx,%eax				// ecx ? eax			|
 8048e7b:	75 f6                	jne    8048e73 <phase_6+0x74>	// !=, jmp L7			|--> add till eax = input[ebx]
 8048e7d:	eb 05                	jmp    8048e84 <phase_6+0x85>	// jmp L9

	L5:
	 8048e7f:	ba 54 c1 04 08       	mov    $0x804c154,%edx			// structtab(head pointer) -> edx
	L9:
	 8048e84:	89 54 b4 28          	mov    %edx,0x28(%esp,%esi,4)	// edx ->  input[6+esi]
	 8048e88:	83 c3 01             	add    $0x1,%ebx				// ebx += 1
	 8048e8b:	83 fb 06             	cmp    $0x6,%ebx				// ebx ? 6
	 8048e8e:	74 17                	je     8048ea7 <phase_6+0xa8>	// ==, end

	L4:
	 8048e90:	89 de                	mov    %ebx,%esi				// ebx -> esi
	 8048e92:	8b 4c 9c 10          	mov    0x10(%esp,%ebx,4),%ecx	// input[ebx] -> ecx
	 8048e96:	83 f9 01             	cmp    $0x1,%ecx				// ecx ? 1
	 8048e99:	7e e4                	jle    8048e7f <phase_6+0x80>	// <=, jmp L5		==> input [ebx] == 0,1 then loop L5

L6:
 8048e9b:	b8 01 00 00 00       	mov    $0x1,%eax				// 1->eax
 8048ea0:	ba 54 c1 04 08       	mov    $0x804c154,%edx			// structtab -> edx		==> 12 bytes per struct{id, val, *next}
 8048ea5:	eb cc                	jmp    8048e73 <phase_6+0x74>	// loop L7
end:

//== part 4
 8048ea7:	8b 5c 24 28          	mov    0x28(%esp),%ebx			// tab[6] -> ebx
 8048eab:	8d 44 24 2c          	lea    0x2c(%esp),%eax			// tab+7 -> eax
 8048eaf:	8d 74 24 40          	lea    0x40(%esp),%esi			// tab+12 -> esi (tail)

 8048eb3:	89 d9                	mov    %ebx,%ecx				// ebx->ecx
L1:
 8048eb5:	8b 10                	mov    (%eax),%edx				// *eax -> edx		|
 8048eb7:	89 51 08             	mov    %edx,0x8(%ecx)			// edx->*(ecx+8)	|
 8048eba:	83 c0 04             	add    $0x4,%eax				// eax = eax + 1	|
 8048ebd:	39 f0                	cmp    %esi,%eax				// eax ? esi		|
 8048ebf:	74 04                	je     8048ec5 <phase_6+0xc6>	// end				|
 8048ec1:	89 d1                	mov    %edx,%ecx				// edx -> ecx		|
 8048ec3:	eb f0                	jmp    8048eb5 <phase_6+0xb6>	// jmp L1			|--> re-link(tab[i-1]->next = tab[i] from 7 to 11)

//== part 5
 8048ec5:	c7 42 08 00 00 00 00 	movl   $0x0,0x8(%edx)
 8048ecc:	be 05 00 00 00       	mov    $0x5,%esi

 8048ed1:	8b 43 08             	mov    0x8(%ebx),%eax
 8048ed4:	8b 00                	mov    (%eax),%eax
 8048ed6:	39 03                	cmp    %eax,(%ebx)
 8048ed8:	7d 05                	jge    8048edf <phase_6+0xe0>	// eax >= (ebx)

 8048eda:	e8 e6 03 00 00       	call   80492c5 <explode_bomb>

 8048edf:	8b 5b 08             	mov    0x8(%ebx),%ebx
 8048ee2:	83 ee 01             	sub    $0x1,%esi
 8048ee5:	75 ea                	jne    8048ed1 <phase_6+0xd2>
 8048ee7:	83 c4 44             	add    $0x44,%esp

 8048eea:	5b                   	pop    %ebx
 8048eeb:	5e                   	pop    %esi
 8048eec:	c3                   	ret
