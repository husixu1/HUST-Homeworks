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
	menu	db	' Welcome to Student Manage Program %%%%%%%%', 0dh, 0ah
			db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah
			db	'    1) Record Student info                 ', 0dh, 0ah
			db	'    2) Calculate Average Score             ', 0dh, 0ah
			db	'    3) Calculate Ranks                     ', 0dh, 0ah
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

	; 10 bytes name, 6 bytes score (3*2), 2 bytes average, 2 bytes rank
	tab		dw	stu_max_num	dup(5 dup(0), -1h, -1h ,-1h, -1h, -1h)
data ends

code segment use16 public "code"
	assume ss:_stack, cs:code, ds:data
start:
	mov		ax, data
	mov		ds, ax
	mov		ax, 0504h			; set current page to 4
	int		10h

main:
	cls
	puts	<offset menu>
	gets	<offset input>, 2

case1:
	cmp		buffer, '1'
	jne		case2

	call 	register
	jmp		main

case2:
	cmp		buffer, '2'
	jne		case3

	call	calcavg
	jmp		main

case3:
	cmp		buffer, '3'
	jne		case4

	call	calcrank
	jmp		main

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
