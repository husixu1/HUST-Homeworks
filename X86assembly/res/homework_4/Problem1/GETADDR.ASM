.model small
.386
puts macro addr
push	dx
push	ax
	mov		dx, addr
	mov		ah, 09h
	int		21H
pop		ax
pop		dx
endm

; \brief convert an number (word) to a string, store it in addr ending with '$'
itoa macro addr, reg
local pushloop, poploop
push	reg
push	ax
push	bx
push	cx
push	dx
	mov		ax, reg
	mov		bx, 0ah
	mov		cx, 0
pushloop:
	mov		dx, 00h
	div		bx
	push	dx
	inc		cx
	cmp		ax, 0
	jnz		pushloop

	mov		bx, addr
poploop:
	pop		ax
	add		al, 30h
	mov		[bx], al
	inc		bx
	loop	poploop

	mov		[bx], byte ptr '$'		; add an ending char
pop		dx
pop		cx
pop		bx
pop		ax
pop		reg
endm

outreturn macro
push	ax
push	dx
	mov		dl, 0dh
	mov		ah, 02h
	int		21h
	mov		dl, 0ah
	mov		ah, 02h
	int		21h
pop		dx
pop		ax
endm

.stack 0ffh
.data
	buffer	db 10 dup(0)
.code
start:
	mov		ax, 00h
	mov		ds, ax

	; read directly
	mov		ecx, ds:[84h]
	itoa	<offset buffer>, cx
	puts	<offset buffer>
	outreturn
	shr		ecx, 16
	itoa	<offset buffer>, cx
	puts	<offset buffer>
	outreturn

	; use the system call
	mov		ah, 35h
	mov		al, 21h
	int		21h

	mov		cx, bx
	itoa	<offset buffer>, cx
	puts	<offset buffer>
	outreturn

	mov		cx, es
	itoa	<offset buffer>, cx
	puts	<offset buffer>
	outreturn

	cmp		ecx, eax
	mov		ah, 4ch
	int		21h
end start

