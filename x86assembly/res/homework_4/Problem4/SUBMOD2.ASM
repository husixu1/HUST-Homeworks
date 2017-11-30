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
	decryptbuf db 14h dup(0)
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

jmp		anti_trace_1_2
anti_trace_1_3:				mov		edx, 00h					; dx stores the avg score
jmp		anti_trace_1_4
anti_trace_1_5:				mov		ebx, 14h					; multiple by 20
jmp		anti_trace_1_6
anti_trace_1_15:
jmp		anti_trace_1_16
anti_trace_1_36:
jmp		anti_trace_1_37
anti_trace_1_2:				mov		eax, 00h
jmp		anti_trace_1_3
anti_trace_1_8:
jmp		anti_trace_1_9
anti_trace_1_6:				mul		ebx
jmp		anti_trace_1_7
anti_trace_1_20:				add		ax,	dx
jmp		anti_trace_1_21
anti_trace_1_10:				lea		di, decryptbuf
jmp		anti_trace_1_11
anti_trace_1_9:				lea		si, tab[ecx]				; move the student into thde decryptbuf
jmp		anti_trace_1_10
anti_trace_1_11:				push	ecx
jmp		anti_trace_1_12
anti_trace_1_21:
jmp		anti_trace_1_22
anti_trace_1_12:				mov		ecx, 14h
jmp		anti_trace_1_13
anti_trace_1_18:				mov		dx, word ptr decryptbuf+10	; get chinese score
jmp		anti_trace_1_19
anti_trace_1_4:				mov		ax, [esp+12h]				; get index (parameter)
jmp		anti_trace_1_5
anti_trace_1_13:				rep		movsb
jmp		anti_trace_1_14
anti_trace_1_22:				mov		dx, word ptr decryptbuf+12	; get math score
jmp		anti_trace_1_23
anti_trace_1_26:				shr		dx, 1
jmp		anti_trace_1_27
anti_trace_1_43:				mov		[esp+12h], ax				; return
jmp		anti_trace_1_44
anti_trace_1_7:				mov		ecx, eax
jmp		anti_trace_1_8
anti_trace_1_24:
jmp		anti_trace_1_25
anti_trace_1_16:				mov		ax, 0						; ax as the total sum
jmp		anti_trace_1_17
anti_trace_1_14:				decrypt <offset decryptbuf>, 14h
jmp		anti_trace_1_15
anti_trace_1_17:
jmp		anti_trace_1_18
anti_trace_1_19:				shl		dx, 1
jmp		anti_trace_1_20
anti_trace_1_23:				add		ax,	dx
jmp		anti_trace_1_24
anti_trace_1_25:				mov		dx, word ptr decryptbuf+14	; get english score
jmp		anti_trace_1_26
anti_trace_1_28:
jmp		anti_trace_1_29
anti_trace_1_37:				pop		ecx
jmp		anti_trace_1_38
anti_trace_1_30:				shl		ax, 1
jmp		anti_trace_1_31
anti_trace_1_42:
jmp		anti_trace_1_43
anti_trace_1_40:				mov		ecx, 14h
jmp		anti_trace_1_41
anti_trace_1_31:				mov		bx, 07H						; divide by 3.5
jmp		anti_trace_1_32
anti_trace_1_33:
jmp		anti_trace_1_34
anti_trace_1_32:				div		bx
jmp		anti_trace_1_33
anti_trace_1_34:				mov		word ptr decryptbuf+16, ax	; re-encrypt
jmp		anti_trace_1_35
anti_trace_1_35:				encrypt <offset decryptbuf>, 14h
jmp		anti_trace_1_36
anti_trace_1_38:				lea		di, tab[ecx]
jmp		anti_trace_1_39
anti_trace_1_41:				rep		movsb
jmp		anti_trace_1_42
anti_trace_1_27:				add		ax,	dx
jmp		anti_trace_1_28
anti_trace_1_29:				mov		dx, 0h
jmp		anti_trace_1_30
anti_trace_1_39:				lea		si, decryptbuf
jmp		anti_trace_1_40

anti_trace_1_44:
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
jmp		anti_trace_2_1

anti_trace_2_1:				mov		cx, 00h
jmp		anti_trace_2_2
anti_trace_2_4:				mov		ax, cx
jmp		anti_trace_2_5
anti_trace_2_17:			cal_this_end:
jmp		anti_trace_2_18
anti_trace_2_2:			refresh_loop:
jmp		anti_trace_2_3
anti_trace_2_7:											; if the not registered
jmp		anti_trace_2_8
anti_trace_2_20:				jnz		refresh_loop
jmp		anti_trace_2_21
anti_trace_2_15:				pop		ax					; get the result (but do not use)
jmp		anti_trace_2_16
anti_trace_2_14:				call cal_one_avg
jmp		anti_trace_2_15
anti_trace_2_18:				inc		cx
jmp		anti_trace_2_19
anti_trace_2_3:				mov		eax, 00h
jmp		anti_trace_2_4
anti_trace_2_6:				mul		bx
jmp		anti_trace_2_7
anti_trace_2_12:			cal_this:
jmp		anti_trace_2_13
anti_trace_2_8:				cmp		tab[eax], byte ptr 00h
jmp		anti_trace_2_9
anti_trace_2_9:				jne		cal_this
jmp		anti_trace_2_10
anti_trace_2_11:
jmp		anti_trace_2_12
anti_trace_2_5:				mov		bx, 14h
jmp		anti_trace_2_6
anti_trace_2_10:				jmp		cal_this_end
jmp		anti_trace_2_11
anti_trace_2_13:				push	cx					; pass the parameter
jmp		anti_trace_2_14
anti_trace_2_16:
jmp		anti_trace_2_17
anti_trace_2_19:				cmp		cx,	stu_max_num
jmp		anti_trace_2_20

anti_trace_2_21:
pop		es
pop		ds
popad
ret
refresh_avg endp

code ends
end start
