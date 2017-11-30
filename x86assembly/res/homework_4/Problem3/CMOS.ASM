.model small
.386

include MACROLIB

.stack 0ffh

.data
	info1	db 'please input the address: $'
	input	db 10h
			db 0
	buffer	db 10h dup(0)

.code
start:
	mov		ax, @DATA
	mov		ds, ax
again:
	puts	<offset info1>
	gets	<offset input>, 2
	cmp		input+1, 0			;if doesn't get input
	je		again

	atoi	<offset buffer>, 0dh
	pop		eax

	out		70h, al
	xor		eax, eax
	in		al, 71h

	itoa16	<offset buffer>, ax
	puts	<offset buffer>

	mov		ah, 4ch
	int		21h
end start

