.model small
.386

.stack 0ffh

.code
start:
	; reset the old interruption
	cli
	xor		ax, ax
	mov		ds, ax

	mov		ax, ds:[60h * 4]
	mov		ds:[16h * 4], ax

	mov		ax, ds:[60h * 4 + 2]
	mov		ds:[16h * 4 + 2], ax
	sti

	mov		ah, 4ch
	int		21h
end start

