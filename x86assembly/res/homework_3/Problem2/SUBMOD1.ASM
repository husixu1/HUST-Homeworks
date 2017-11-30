.386
include macrolib

public		_registeration

extrn		tab:byte
extrn		stu_max_num:abs

_stack segment use16 stack "stack"
	db	0fffh dup(0)
_stack ends

_DATA segment use16 public "DATA"
	input_max_num	equ 10
	grade_max_num	equ 100

	info1	db	'Please input the index you want to edit: $'
	info2	db	'Please input the Name of the student: $'
	info3	db	'Please input the Chinese score: $'
	info4	db	'Please input the Math score   : $'
	info5	db	'Please input the English score: $'
	info6	db	'Student info registered.', 0dh, 0ah, '$'

	; the number should be equal to stu_max_num
	err1	db	'Index exceeded, range is 0-9', 0dh, 0ah, '$'
	err2	db	'Grade exceeded, max is 100', 0dh, 0ah, '$'

	input	db	input_max_num
			db	?
	buffer	db	input_max_num dup(0)

	; store the input info temporarily, for atomic operation
	stu_buf	db	20 dup(0)
_DATA ends

_TEXT segment use16 public "CODE"
	assume	cs:_TEXT, ds:_DATA, ss:_stack, es:_DATA
start:

_registeration proc
pushad
push	ds
push	es
mov		ax, _DATA
mov		ds, ax
mov		es, ax
reg_s:
	; get the index of the table
	puts	<offset info1>
	gets	<offset input>, 3

	; if doesn't get anything
	cmp		input+1h, 0
	jz		reg_s

	atoi	<offset buffer>, 0dh	; convert to num
	pop		eax

	cmp		eax, 0					; make sure the index does not exceeds
	jl		error1
	cmp		eax, stu_max_num
	jge		error1

	mov		ebx, 20					; get the real position of the student
	mul		ebx
	push	eax						; store the position
stuname:
	; get the name of the student
	puts	<offset info2>
	gets	<offset input>, 10

	cmp		input+1, byte ptr 00h	; make sure there's input
	je		stuname

	mov		cx, 0ah					; clear the buffer
	lea		eax, stu_buf
clear:
	mov		[eax], byte ptr 00h
	inc		eax
	loop	clear

	movzx	ecx, input+1h			; get the input length
	lea		esi, buffer
	lea		edi, stu_buf			; store the name temporarily
	rep		movsb

	; get the scores of the stuednt
chinese:
	puts	<offset info3>			; chinese
	gets	<offset input>,	4

	cmp		input+1, byte ptr 00h	; make sure there's input
	je		chinese

	atoi	<offset buffer>, 0dh
	pop		eax
	cmp		eax, 0					; make sure chinese score does not exceeds
	jl		error2
	cmp		eax, grade_max_num
	jg		error2
	mov		word ptr stu_buf+0ah, ax; store chinese score temporarily
math:
	puts	<offset info4>			; math
	gets	<offset input>,	4

	cmp		input+1, byte ptr 00h	; make sure there's input
	je		math

	atoi	<offset buffer>, 0dh
	pop		eax
	cmp		eax, 0					; make sure math score does not exceeds
	jl		error2
	cmp		eax, grade_max_num
	jg		error2
	mov		word ptr stu_buf+0ch, ax; store math score temporarily
english:
	puts	<offset info5>			; english
	gets	<offset input>,	4

	cmp		input+1, byte ptr 00h	; make sure there's input
	je		english

	atoi	<offset buffer>, 0dh
	pop		eax
	cmp		eax, 0					; make sure english score does not exceeds
	jl		error2
	cmp		eax, grade_max_num
	jg		error2
	mov		word ptr stu_buf+0eh, ax; store english score temporarily
pass:
	; if passed all test
	pop		eax						; get the position back
	mov		ecx, 14H				; transport all data (20bytes)

	; sent the name to correct position
	lea		esi, stu_buf
	lea		edi, tab[eax]
	rep		movsb
	puts	<offset info6>
	gets	<offset input>, 1
	jmp		ending

; index exceeds
error1:
	puts	<offset err1>
	gets	<offset input>,1		; wait for return
	jmp		ending

; grade exceeds
error2:
	pop		eax						; index not exceeds, but get an error, pop the temporarily stored index
	puts	<offset err2>
	gets	<offset input>,1		; wait for return
	jmp		ending

ending:
pop		es
pop		ds
popad
ret
_registeration endp

_TEXT ends
end start
