.386
include macrolib

public		calcavg
public		refresh_avg

extrn		tab:byte
extrn		stu_max_num:abs
extrn		name_to_index:far
extrn		printname:far

_stack segment use16 stack "stack"
_stack ends

data segment use16 public "data"
	input_max_num	equ 10
	info1	db	' Submenu - Average Score Calculate %%%%%%%%', 0dh, 0ah
			db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah
			db	'    1) Calculate/Refresh all students      ', 0dh, 0ah
			db	'    2) Specify a student (by index)        ', 0dh, 0ah
			db	'    3) Specify a student (by name)         ', 0dh, 0ah
			db	'    0) Exit                                ', 0dh, 0ah
			db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah
			db	'Please input your choice >>> $'

	info2	db	'Index $'
	info3	db	': average Score is $'
	info4	db	': Name is $'

	info5	db	'Please input the name: $'
	info6	db	': Student not registered $'
	info7	db	'Please input the index: $'

	err1	db	'Unidentified choice, Please re-input ... ', 0dh, 0ah, '$'
	err2	db	'Student not found ... ', 0dh, 0ah, '$'
	err3	db	'Index exceeded, range is 0-9', 0dh, 0ah, '$'

	input	db	input_max_num
			db	?
	buffer	db	input_max_num dup(0)
data ends

code segment use16 public "code"
	assume	cs:code, ds:data, ss:_stack, es:data
start:

calcavg proc
pushad
push	ds
push	es
mov		ax, data
mov		ds, ax
mov		es, ax
cal_s:
	cls
	puts	<offset info1>
	gets	<offset input>, 2

case1:
	cmp		buffer, '1'
	jne		case2

	mov		cx, 00h
loop_a	label far
	mov		eax, 00h
	mov		ax, cx
	mov		bx, 14h
	mul		bx
								; if the not registered
	cmp		tab[eax], byte ptr 00h
	jne		case1_cal_this_one
	puts	<offset info2>
	itoa	<offset buffer>, cx
	puts	<offset buffer>
	puts	<offset info6>
	outreturn
	jmp		far ptr cal_one_end

case1_cal_this_one:
	push	cx					; pass the parameter
		call cal_one_avg
	pop		ax					; get the result (but do not use)

	puts	<offset info2>
	itoa	<offset buffer>, cx
	puts	<offset buffer>

	puts	<offset info4>
	push	cx						; print name
	call	far ptr printname
	pop		cx

	puts	<offset info3>
	itoa	<offset buffer>, ax
	puts	<offset buffer>
	outreturn

cal_one_end label far
	inc		cx
	cmp		cx,	stu_max_num
	jnz		far ptr loop_a

	jmp		far ptr looping

case2:
	cmp		buffer,	'2'
	jne		case3

case2_input:
	puts	<offset info7>			; get the index
	gets	<offset input>, 3

	cmp		input+1h, 0				; if doesn't get anything
	jz		case2_input

	atoi	<offset buffer>, 0dh	; convert to num
	pop		ecx

	cmp		ecx, 0					; make sure the index does not exceeds
	jl		error3
	cmp		ecx, stu_max_num
	jge		error3

	mov		eax, 00h
	mov		ax, cx
	mov		bx, 14h
	mul		bx

	cmp		tab[eax], byte ptr 00h	; if not registered
	jne		case2_cal_this_one
	puts	<offset info2>
	itoa	<offset buffer>, cx
	puts	<offset buffer>
	puts	<offset info6>
	outreturn
	jmp		far ptr looping

case2_cal_this_one:
	push	cx						; pass the parameter
		call cal_one_avg
	pop		ax						; get the result (but do not use)

	puts	<offset info2>
	itoa	<offset buffer>, cx
	puts	<offset buffer>

	puts	<offset info4>
	push	cx						; print name
	call	far ptr printname
	pop		cx

	puts	<offset info3>
	itoa	<offset buffer>, ax
	puts	<offset buffer>
	outreturn

	jmp		far ptr looping

case3:
	cmp		buffer, '3'
	jne		case0

case3_input:
	puts	<offset info5>
	gets	<offset input>, 10

	cmp		input+1, byte ptr 00h	; make sure there's input
	je		case3_input

	mov		si, offset buffer		; make the buffer end with \0...\0
	mov		cx, 0ah
a0:
	cmp		ds:[si], byte ptr 0dh
	je		b0
	inc		si
	loop	a0
b0:
	mov		ds:[si], byte ptr 0
	inc		si
	loop	b0

	mov		si, offset buffer
	push	si
	call	far ptr name_to_index

	pop		cx						; retreive the index into cx
	cmp		cx, -1h					; if not found
	je		error2
	push	cx

	call	cal_one_avg				; the parameter is already in stack
	pop		ax

	puts	<offset info2>			; show the grade
	itoa	<offset buffer>, cx
	puts	<offset buffer>

	puts	<offset info4>
	push	cx						; print name
	call	far ptr printname
	pop		cx

	puts	<offset info3>
	itoa	<offset buffer>, ax
	puts	<offset buffer>
	outreturn

	jmp		far ptr looping

case0:
	cmp		buffer, '0'
	jne		default
	jmp		ending

default:
	puts	<offset err1>
	jmp		far ptr looping

error2:
	puts	<offset err2>
	jmp		far ptr looping

error3:
	puts	<offset err3>
	jmp		far ptr looping

looping	label far
	gets	<offset input>, 1	;wait for an enter
	jmp		cal_s

ending:
pop		es
pop		ds
popad
ret
calcavg endp

; \brief calculate a student's avgscore and store it in tab
; \para (2 byte) index of the student in stack
; \return (2 byte) avg score in stack (CAN NOT BE IGNORED)
cal_one_avg proc
push	eax
push	ebx
push	ecx
push	edx
	mov		eax, 00h
	mov		edx, 00h					; dx stores the avg score
	mov		ax, [esp+12h]				; get index (parameter)
	mov		ebx, 14h					; multiple by 20
	mul		ebx
	mov		ecx, eax					; ecx as the pointer
	mov		ax, 0						; ax as the total sum

	mov		dx, word ptr tab+10[ecx]	; get chinese score
	shl		dx, 1
	add		ax,	dx

	mov		dx, word ptr tab+12[ecx]	; get math score
	add		ax,	dx

	mov		dx, word ptr tab+14[ecx]	; get english score
	shr		dx, 1
	add		ax,	dx

	mov		dx, 0h
	shl		ax, 1
	mov		bx, 07H						; divide by 3.5
	div		bx

	mov		word ptr tab+16[ecx], ax	; store it to correct position
	mov		[esp+12h], ax				; return
pop		edx
pop		ecx
pop		ebx
pop		eax
ret
cal_one_avg endp

; \brief refresh the avreage score in table, designed for other modules
refresh_avg proc far
pushad
push	ds
push	es
mov		ax, data
mov		ds, ax
mov		es, ax
	mov		cx, 00h
refresh_loop:
	mov		eax, 00h
	mov		ax, cx
	mov		bx, 14h
	mul		bx
								; if the not registered
	cmp		tab[eax], byte ptr 00h
	jne		cal_this
	jmp		cal_this_end

cal_this:
	push	cx					; pass the parameter
	call cal_one_avg
	pop		ax					; get the result (but do not use)

cal_this_end:
	inc		cx
	cmp		cx,	stu_max_num
	jnz		refresh_loop
pop		es
pop		ds
popad
ret
refresh_avg endp

code ends
end start
