.model small
.386

.stack 0ffh

.code
; the new int 16h interruption
	old		dw 0, 0
new16h:
	cmp		ah, 2h
	jz		skip

	int		60h
	; modify ah and al
	cmp		al, 61h
	jl		not_lower

	cmp		al, 7ah
	jg		not_lower
	sub		al, 20h

	iret

skip:
	int		60h
not_lower:
	iret

start:
	xor		ax, ax
	mov		ds, ax

	; move the interruption to another place
	mov		ax, ds:[16h * 4]
	mov		old, ax
	mov		ds:[60H * 4], ax

	mov		ax, ds:[16h * 4 + 2]
	mov		old+2, ax
	mov		ds:[60H * 4 + 2], ax

	; set the new interruption
	cli
	mov		word ptr ds:[16h * 4], offset new16h
	mov		ds:[16h * 4 + 2], cs
	sti

	mov		dx, offset start + 0fh
	mov		cl, 4
	shr		dx, cl
	add		dx, 10h

	mov		al, 0
	mov		ah, 31h
	int		21h
end start

