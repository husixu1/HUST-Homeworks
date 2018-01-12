.386
include macrolib

; all submodule share the same table
public		tab, stu_max_num

extrn 		register:near
extrn		calcavg:near
extrn		calcrank:near
extrn		printall:near

_stack segment use16 stack "stack"
	db	0FFH DUP(0)
_stack ends

data segment use16 public "data"
	input_max_num	equ 10
	stu_max_num		equ 10
	half_max_num	equ 5
	menu	db	' Welcome to Student Manage Program %%%%%%%%', 0dh, 0ah
			db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah
			db	'    1) Record Student info                 ', 0dh, 0ah
			db	'    2) Calculate Average (not available)   ', 0dh, 0ah
			db	'    3) Calculate Ranks (not available)     ', 0dh, 0ah
			db	'    4) Show Grades and Ranks               ', 0dh, 0ah
			db	'    0) Exit                                ', 0dh, 0ah
			db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah
			db	'Please input your choice >>> $'

	info1	db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah
			db	' Thanks for using this system, bye %%%%%%%%', 0dh, 0ah
			db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah, '$'

	err1	db	'Unidentified choice, Please re-input ... ', 0dh, 0ah, '$'
	; modules do not share the input buffer
	input	db	input_max_num
			db	?
	buffer	db	input_max_num dup(0)

	encryptbuf db 14h dup(0)

	; 10 bytes name, 6 bytes score (3*2), 2 bytes average, 2 bytes rank
	tab		db	76h, 17h, 7bh, 0bh, 63h, 02h, 02h, 02h, 02h ,02h
			db	02h, 5dh, 5dh, 3dh, 3dh, 5ch, 5ch, 03h, 03h, 00h

			db	2dh, 4fh, 2ah, 5eh, 3fh, 3fh, 3fh, 3fh, 3fh, 3fh
			db	3fh, 6fh, 6fh, 35h, 35h, 51h, 51h, 04h, 04h, 00h

			db	5bh, 3ch, 5dh, 30h, 5dh, 3ch, 3ch, 3ch, 3ch, 3ch
			db	3ch, 58h, 58h, 07h, 07h, 63h, 63h, 01h, 01h, 00h

			db	7fh, 1bh, 7eh, 12h, 66h, 07h, 07h, 07h,	07h, 07h
			db	07h, 3bh, 3bh, 7ah, 7ah, 38h, 38h, 06h, 06h, 00h

			db	26h, 43h, 33h, 40h, 29h, 45h, 2ah, 44h, 44h, 44h
			db	44h, 76h, 76h, 17h, 17h, 41h, 41h, 05h, 05h, 00h

			db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
			db	00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh

			db	11h, 42h, 2bh, 53h, 26h, 06h, 4eh, 3bh, 3bh, 3bh
			db	3bh, 59h, 59h, 3ah, 3ah, 62h, 62h, 02h, 02h, 00h

			db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
			db	00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh

			db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
			db	00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh

			db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
			db	00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh

			db	00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
			db	00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh, 00h,0ffh

data ends

code segment use16 public "code"
	assume ss:_stack, cs:code, ds:data
start:
	mov		ax, data
	mov		ds, ax
	mov		es, ax
	mov		ax, 0504h			; set current page to 4
	int		10h

main:
	cls
	puts	<offset menu>
	gets	<offset input>, 2

case1:
	cmp		buffer, '1'
	jne		case4

	call 	register
	jmp		main

;case2:
;	cmp		buffer, '2'
;	jne		case3
;
;	call	calcavg
;	jmp		main
;
;case3:
;	cmp		buffer, '3'
;	jne		case4
;
;	call	calcrank
;	jmp		main

case4:
	cmp		buffer, '4'
	jne		case0

	call	printall
	jmp		main

case0:
	cmp		buffer, '0'
	jne		default

	puts	<offset info1>
	mov		ah, 4ch
	int		21h

default:
	puts	<offset err1>
	jmp		main

code ends
end start
