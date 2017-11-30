.386
include macrolib

public		register

extrn		tab:byte
extrn		stu_max_num:abs

_stack segment use16 stack "stack"
_stack ends

data segment use16 public "data"
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
data ends

code segment use16 public "code"
	assume	cs:code, ds:data, ss:_stack, es:data
start:

register proc
pushad
push	ds
push	es
mov		ax, data
mov		ds, ax
mov		es, ax
reg_s:
	jmp far ptr anti_track1
anti_track71 label far

	jmp far ptr anti_track72
anti_track19 label far
			stuname:
	jmp far ptr anti_track20
anti_track66 label far
				jg		error2
	jmp far ptr anti_track67
anti_track88 label far
				encrypt	<offset stu_buf>, 14h
	jmp far ptr anti_track89
anti_track103 label far

	jmp far ptr anti_track104
anti_track42 label far
				gets	<offset input>,	4
	jmp far ptr anti_track43
anti_track45 label far
				je		chinese
	jmp far ptr anti_track46
anti_track17 label far
				mul		ebx
	jmp far ptr anti_track18
anti_track32 label far
				dec		cx
				cmp		cx, 0
				jne		far ptr clear
	jmp far ptr anti_track33
anti_track85 label far
				mov		ecx, 14H				; transport all data (20bytes)
	jmp far ptr anti_track86
anti_track34 label far
				movzx	ecx, input+1h			; get the input length
	jmp far ptr anti_track35
anti_track4 label far
				; if doesn't get anything
	jmp far ptr anti_track5
anti_track94 label far
				puts	<offset info6>
	jmp far ptr anti_track95
anti_track81 label far
				mov		word ptr stu_buf+0eh, ax; store english score temporarily
	jmp far ptr anti_track82
anti_track37 label far
				rep		movsb
	jmp far ptr anti_track38
anti_track106 label far
				pop		eax						; index not exceeds, but get an error, pop the temporarily stored index
	jmp far ptr anti_track107
anti_track73 label far
				je		english
	jmp far ptr anti_track74
anti_track109 label far
				jmp		ending
	jmp far ptr anti_track110
anti_track51 label far
				cmp		eax, grade_max_num
	jmp far ptr anti_track52
anti_track65 label far
				cmp		eax, grade_max_num
	jmp far ptr anti_track66
anti_track44 label far
				cmp		input+1, byte ptr 00h	; make sure there's input
	jmp far ptr anti_track45
anti_track14 label far
				jge		error1
	jmp far ptr anti_track15
anti_track23 label far

	jmp far ptr anti_track24
anti_track22 label far
				gets	<offset input>, 10
	jmp far ptr anti_track23
anti_track55 label far
				puts	<offset info4>			; math
	jmp far ptr anti_track56
anti_track80 label far
				jg		error2
	jmp far ptr anti_track81
anti_track64 label far
				jl		error2
	jmp far ptr anti_track65
anti_track25 label far
				je		stuname
	jmp far ptr anti_track26
anti_track69 label far
				puts	<offset info5>			; english
	jmp far ptr anti_track70
anti_track77 label far
				cmp		eax, 0					; make sure english score does not exceeds
	jmp far ptr anti_track78
anti_track41 label far
				puts	<offset info3>			; chinese
	jmp far ptr anti_track42
anti_track102 label far
				jmp		ending
	jmp far ptr anti_track103
anti_track28 label far
				lea		eax, stu_buf
	jmp far ptr anti_track29
anti_track82 label far
			pass:
	jmp far ptr anti_track83
anti_track90 label far
				; sent the name to correct position
	jmp far ptr anti_track91
anti_track21 label far
				puts	<offset info2>
	jmp far ptr anti_track22
anti_track8 label far
				atoi	<offset buffer>, 0dh	; convert to num
	jmp far ptr anti_track9
anti_track9 label far
				pop		eax
	jmp far ptr anti_track10
anti_track91 label far
				lea		esi, stu_buf
	jmp far ptr anti_track92
anti_track60 label far

	jmp far ptr anti_track61
anti_track40 label far
			chinese:
	jmp far ptr anti_track41
anti_track11 label far
				cmp		eax, 0					; make sure the index does not exceeds
	jmp far ptr anti_track12
anti_track35 label far
				lea		esi, buffer
	jmp far ptr anti_track36
anti_track75 label far
				atoi	<offset buffer>, 0dh
	jmp far ptr anti_track76
anti_track78 label far
				jl		error2
	jmp far ptr anti_track79
anti_track63 label far
				cmp		eax, 0					; make sure math score does not exceeds
	jmp far ptr anti_track64
anti_track53 label far
				mov		word ptr stu_buf+0ah, ax; store chinese score temporarily
	jmp far ptr anti_track54
anti_track10 label far

	jmp far ptr anti_track11
anti_track58 label far
				cmp		input+1, byte ptr 00h	; make sure there's input
	jmp far ptr anti_track59
anti_track99 label far
			error1:
	jmp far ptr anti_track100
anti_track57 label far

	jmp far ptr anti_track58
anti_track31 label far
				inc		eax
	jmp far ptr anti_track32
anti_track108 label far
				gets	<offset input>,1		; wait for return
	jmp far ptr anti_track109
anti_track54 label far
			math:
	jmp far ptr anti_track55
anti_track105 label far
			error2:
	jmp far ptr anti_track106
anti_track96 label far
				jmp		ending
	jmp far ptr anti_track97
anti_track36 label far
				lea		edi, stu_buf			; store the name temporarily
	jmp far ptr anti_track37
anti_track18 label far
				push	eax						; store the position
	jmp far ptr anti_track19
anti_track100 label far
				puts	<offset err1>
	jmp far ptr anti_track101
anti_track98 label far
			; index exceeds
	jmp far ptr anti_track99
anti_track89 label far

	jmp far ptr anti_track90
anti_track12 label far
				jl		error1
	jmp far ptr anti_track13
anti_track26 label far

	jmp far ptr anti_track27
anti_track49 label far
				cmp		eax, 0					; make sure chinese score does not exceeds
	jmp far ptr anti_track50
anti_track104 label far
			; grade exceeds
	jmp far ptr anti_track105
anti_track86 label far

	jmp far ptr anti_track87
anti_track93 label far
				rep		movsb
	jmp far ptr anti_track94
anti_track62 label far
				pop		eax
	jmp far ptr anti_track63
anti_track97 label far

	jmp far ptr anti_track98
anti_track74 label far

	jmp far ptr anti_track75
anti_track5 label far
				cmp		input+1h, 0
	jmp far ptr anti_track6
anti_track7 label far

	jmp far ptr anti_track8
anti_track52 label far
				jg		error2
	jmp far ptr anti_track53
anti_track2 label far
				gets	<offset input>, 3
	jmp far ptr anti_track3
anti_track6 label far
				jz		reg_s
	jmp far ptr anti_track7
anti_track13 label far
				cmp		eax, stu_max_num
	jmp far ptr anti_track14
anti_track87 label far
				; encrypt
	jmp far ptr anti_track88
anti_track101 label far
				gets	<offset input>,1		; wait for return
	jmp far ptr anti_track102
anti_track83 label far
				; if passed all test
	jmp far ptr anti_track84
anti_track68 label far
			english:
	jmp far ptr anti_track69
anti_track39 label far
				; get the scores of the stuednt
	jmp far ptr anti_track40
anti_track48 label far
				pop		eax
	jmp far ptr anti_track49
anti_track50 label far
				jl		error2
	jmp far ptr anti_track51
anti_track84 label far
				pop		eax						; get the position back
	jmp far ptr anti_track85
anti_track38 label far

	jmp far ptr anti_track39
anti_track46 label far

	jmp far ptr anti_track47
anti_track107 label far
				puts	<offset err2>
	jmp far ptr anti_track108
anti_track47 label far
				atoi	<offset buffer>, 0dh
	jmp far ptr anti_track48
anti_track56 label far
				gets	<offset input>,	4
	jmp far ptr anti_track57
anti_track27 label far
				mov		cx, 0ah					; clear the buffer
	jmp far ptr anti_track28
anti_track30 label far
				mov		[eax], byte ptr 00h
	jmp far ptr anti_track31
anti_track61 label far
				atoi	<offset buffer>, 0dh
	jmp far ptr anti_track62
anti_track24 label far
				cmp		input+1, byte ptr 00h	; make sure there's input
	jmp far ptr anti_track25
anti_track3 label far

	jmp far ptr anti_track4
anti_track43 label far

	jmp far ptr anti_track44
anti_track92 label far
				lea		edi, tab[eax]
	jmp far ptr anti_track93
anti_track72 label far
				cmp		input+1, byte ptr 00h	; make sure there's input
	jmp far ptr anti_track73
anti_track15 label far

	jmp far ptr anti_track16
anti_track76 label far
				pop		eax
	jmp far ptr anti_track77
anti_track67 label far
				mov		word ptr stu_buf+0ch, ax; store math score temporarily
	jmp far ptr anti_track68
anti_track59 label far
				je		math
	jmp far ptr anti_track60
anti_track33 label far

	jmp far ptr anti_track34
anti_track70 label far
				gets	<offset input>,	4
	jmp far ptr anti_track71
anti_track29 label far
			clear label far
	jmp far ptr anti_track30
anti_track79 label far
				cmp		eax, grade_max_num
	jmp far ptr anti_track80
anti_track16 label far
				mov		ebx, 20					; get the real position of the student
	jmp far ptr anti_track17
anti_track95 label far
				gets	<offset input>, 1
	jmp far ptr anti_track96
anti_track1 label far
				puts	<offset info1>
	jmp far ptr anti_track2
anti_track20 label far
				; get the name of the student
	jmp far ptr anti_track21
anti_track110 label far


ending:
pop		es
pop		ds
popad
ret
register endp

code ends
end start
