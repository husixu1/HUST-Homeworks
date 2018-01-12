data segment use16
		in_name	db 10
				db ?
		in_buff	db 10 dup(0)
		output1	db 'Please input the name of the student: $'
data ends

code segment use16
assume ss:_stack, ds:data, cs:code
start:
		mov		ax, data
		mov		ds, ax
input:		; input the name of student
mov		ah, 09h						; show input message
		lea		dx, output1
		int		21h

		mov		ah, 0ah				; input the name
		lea		dx, in_name
		int		21h

		outreturn

		cmp		in_buff, 0dh		; if input is \r
		jz		input

		cmp		in_buff, 71h		; test if quit
		jnz		find_s
		cmp		in_buff+01h, 0dh
		jnz		find_s				; q but not q\r
		mov		ah, 4ch
		int		21h
find_s:
	;enter next procedure
		mov		ah, 4ch
		int		21h
code ends
end start


