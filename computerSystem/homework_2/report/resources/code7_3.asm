08048eed <fun7>:
 8048eed:	53                   	push   %ebx
 8048eee:	83 ec 18             	sub    $0x18,%esp
 8048ef1:	8b 54 24 20          	mov    0x20(%esp),%edx
 8048ef5:	8b 4c 24 24          	mov    0x24(%esp),%ecx
 8048ef9:	85 d2                	test   %edx,%edx
 8048efb:	74 37                	je     8048f34 <fun7+0x47>		// BOOM

 8048efd:	8b 1a                	mov    (%edx),%ebx
 8048eff:	39 cb                	cmp    %ecx,%ebx
 8048f01:	7e 13                	jle    8048f16 <fun7+0x29>

 8048f03:	89 4c 24 04          	mov    %ecx,0x4(%esp)
 8048f07:	8b 42 04             	mov    0x4(%edx),%eax
 8048f0a:	89 04 24             	mov    %eax,(%esp)
 8048f0d:	e8 db ff ff ff       	call   8048eed <fun7>

 8048f12:	01 c0                	add    %eax,%eax
 8048f14:	eb 23                	jmp    8048f39 <fun7+0x4c>

 8048f16:	b8 00 00 00 00       	mov    $0x0,%eax
 8048f1b:	39 cb                	cmp    %ecx,%ebx
 8048f1d:	74 1a                	je     8048f39 <fun7+0x4c>

 8048f1f:	89 4c 24 04          	mov    %ecx,0x4(%esp)
 8048f23:	8b 42 08             	mov    0x8(%edx),%eax
 8048f26:	89 04 24             	mov    %eax,(%esp)
 8048f29:	e8 bf ff ff ff       	call   8048eed <fun7>

 8048f2e:	8d 44 00 01          	lea    0x1(%eax,%eax,1),%eax
 8048f32:	eb 05                	jmp    8048f39 <fun7+0x4c>
 8048f34:	b8 ff ff ff ff       	mov    $0xffffffff,%eax

 8048f39:	83 c4 18             	add    $0x18,%esp
 8048f3c:	5b                   	pop    %ebx
 8048f3d:	c3                   	ret


