.386
include macrolib

public		printall

extrn		tab:byte
extrn		stu_max_num:abs
extrn		refresh_avg:far
extrn		refresh_rank:far

_stack segment use16 stack "stack"
_stack ends

data segment use16 public "data"
	info1	db	'+------------+-----+-----+-----+-----+------+', 0dh, 0ah
			db	'|    Name    | CHN | MAT | ENG | AVG | RANK |', 0dh, 0ah
	info2	db	'+------------+-----+-----+-----+-----+------+', 0dh, 0ah, '$'
	line	db	'|            |     |     |     |     |      |', 0dh, 0ah, '$'
	info3	db	'|     --     |  -  |  -  |  -  |  -  |  --  |', 0dh, 0ah, '$'

	buffer	db	80 dup(0)
	numbuff	db	10 dup(0)

data ends

code segment use16 public "code"
	assume	cs:code, ds:data, ss:_stack, es:data
start:
printall proc
pushad
push	ds
push	es
mov		ax, data
mov		ds, ax
mov		es, ax
	call	far ptr refresh_avg
	call	far ptr refresh_rank

	cls
	puts	<offset info1>

	mov		cx, 00h
print_loop:
	; clear the buffer
	push	cx
	mov		cx, 50h
	lea		si, line
	lea		di, buffer
	rep		movsb
	pop		cx

	; initilize index
	mov		eax, 00h
	mov		ax, cx
	mov		bx,	14h
	mul		bx

	cmp		tab[eax], byte ptr 0
	je		not_reged

	; fill the name
	lea		di, buffer+2
	lea		si, tab[eax]
name_s:
	movsb
	cmp		[si], byte ptr 0
	jne		name_s

	; fill the chinese grade
	mov		dx, word ptr tab+10[eax]
	itoa	<offset numbuff>, dx
	lea		si, numbuff
	lea		di, buffer+0fh
chinese_s:
	movsb
	cmp		[si], byte ptr '$'
	jne		chinese_s

	; fill the math grade
	mov		dx, word ptr tab+12[eax]
	itoa	<offset numbuff>, dx
	lea		si, numbuff
	lea		di, buffer+15h
math_s:
	movsb
	cmp		[si], byte ptr '$'
	jne		math_s

	; fill the english grade
	mov		dx, word ptr tab+14[eax]
	itoa	<offset numbuff>, dx
	lea		si, numbuff
	lea		di, buffer+1bh
english_s:
	movsb
	cmp		[si], byte ptr '$'
	jne		english_s

	; fill the avreage grade
	mov		dx, word ptr tab+16[eax]
	itoa	<offset numbuff>, dx
	lea		si, numbuff
	lea		di, buffer+21h
average_s:
	movsb
	cmp		[si], byte ptr '$'
	jne		average_s

	; fill the rank
	mov		dx, word ptr tab+18[eax]
	itoa	<offset numbuff>, dx
	lea		si, numbuff
	lea		di, buffer+27h
rank_s:
	movsb
	cmp		[si], byte ptr '$'
	jne		rank_s

	puts	<offset buffer>
	jmp		print_loop_tail

not_reged:
	puts	<offset info3>

print_loop_tail:
	inc		cx
	cmp		cx, stu_max_num
	jne		print_loop

	puts	<offset info2>
	gets	<offset numbuff>, 1

pop		es
pop		ds
popad
ret
printall endp

code ends
end start
