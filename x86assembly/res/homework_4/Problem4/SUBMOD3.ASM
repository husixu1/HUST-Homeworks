.386
include macrolib

public		calcrank
public		refresh_rank

extrn		tab:byte
extrn		stu_max_num:abs
extrn		name_to_index:far
extrn		printname:far
extrn		refresh_avg:far

_stack segment use16 stack "stack"
_stack ends

data segment use16 public "data"
	input_max_num	equ 10
	info1	db	' Submenu - Ranking Calculate %%%%%%%%%%%%%%', 0dh, 0ah
			db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah
			db	'    1) Calculate/Refresh all students      ', 0dh, 0ah
			db	'    2) Specify a student (by index)        ', 0dh, 0ah
			db	'    3) Specify a student (by name)         ', 0dh, 0ah
			db	'    0) Exit                                ', 0dh, 0ah
			db	'%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%', 0dh, 0ah
			db	'Please input your choice >>> $'

	info2	db	'Index $'
	info3	db	': Rank is $'
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

calcrank proc
pushad
push	ds
push	es
mov		ax, data
mov		ds, ax
mov		es, ax
cal_s:
	call	far ptr refresh_avg		; refresh the avreage score before calculating rank
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
	push	cx						; pass the parameter
		call cal_one_rank
	pop		ax						; get the result (but do not use)

	puts	<offset info2>			; print index
	itoa	<offset buffer>, cx
	puts	<offset buffer>

	puts	<offset info4>
	push	cx						; print name
	call	far ptr printname
	pop		cx

	puts	<offset info3>			; print rank
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
		call cal_one_rank
	pop		ax						; get the result (but do not use)

	puts	<offset info2>			; print index
	itoa	<offset buffer>, cx
	puts	<offset buffer>

	puts	<offset info4>
	push	cx						; print name
	call	far ptr printname
	pop		cx

	puts	<offset info3>			; print rank
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

	call	cal_one_rank			; the parameter is already in stack
	pop		ax

	puts	<offset info2>			; print index
	itoa	<offset buffer>, cx
	puts	<offset buffer>

	puts	<offset info4>
	push	cx						; print name
	call	far ptr printname
	pop		cx

	puts	<offset info3>			; print rank
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
calcrank endp

; \brief calculate a student's ranking and store it in tab
; \para (2 byte) index of the student in stack
; \return (2 byte) rank in stack (CAN NOT BE IGNORED)
cal_one_rank proc
push	eax
push	ebx
push	ecx
push	edx
push	esi
push	edi
	mov		eax, 00h
	mov		esi, 00h					; esi store the rank
	mov		ax, [esp+1ah]				; get index (parameter)
	mov		ebx, 14h					; multiple by 20
	mul		ebx

	push	esi							; get the current score
	push	edi
	push	ecx
		lea		si, tab[eax]
		lea		di, decryptbuf
		mov		ecx, 14h
		rep		movsb
		decrypt	<offset decryptbuf>, 14h
	pop	ecx
	pop	edi
	pop	esi
	mov		bx, word ptr decryptbuf+16	; bx stores the current avgscore

	mov		cx, 00h
loop_rank:
	push	bx
	mov		eax, 00h
	mov		bx, 14h
	mov		ax, cx
	mul		bx

	cmp		tab[eax], byte ptr 0
	je		not_reged

	push	esi							; decrypt and compare
	push	edi
	push	ecx
		lea		si, tab[eax]
		lea		di, decryptbuf
		mov		ecx, 14h
		rep		movsb
		decrypt	<offset decryptbuf>, 14h
	pop	ecx
	pop	edi
	pop	esi

	pop		bx
	cmp		word ptr decryptbuf+16, bx
	jg		add_one
	jmp		loop_rank_tail

add_one:
	inc		si
	jmp		loop_rank_tail

not_reged:
	pop		bx

loop_rank_tail:
	inc		cx
	cmp		cx, stu_max_num
	jne		loop_rank

loop_end:
	inc		si

	mov		ax, [esp+1ah]				; get index (parameter)
	mov		ebx, 14h					; multiple by 20
	mul		ebx

	push	esi							; load, decrypt and re-encrypt
	push	edi
	push	ecx
		lea		si, tab[eax]
		lea		di, decryptbuf
		mov		ecx, 14h
		rep		movsb
		decrypt	<offset decryptbuf>, 14h
	pop	ecx
	pop	edi
	pop	esi

	mov		word ptr decryptbuf+18, si	; store it to correct position
	encrypt <offset decryptbuf>, 14h
	mov		[esp+1ah], si				; return value

	lea		di, tab[eax]				; send into tab
	lea		si, decryptbuf
	mov		ecx, 14h
	rep		movsb
pop		edi
pop		esi
pop		edx
pop		ecx
pop		ebx
pop		eax
ret
cal_one_rank endp

; \brief refresh all student's rank, but do NOT refresh avgscore automatically, use with care
refresh_rank proc far
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
	push	cx						; pass the parameter
	call cal_one_rank
	pop		ax						; get the result (but do not use)

cal_this_end:
	inc		cx
	cmp		cx,	stu_max_num
	jnz		refresh_loop
pop		es
pop		ds
popad
ret
refresh_rank endp

code ends
end start
