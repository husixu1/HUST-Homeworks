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
	info4	db	'Please input the passwd: $'
	info5	db	'Wrong passwd, exiting ...$'

	input	db	80
			db	0
	buffer	db	80 dup(0)

	loadbuf	db	80 dup(0)
	numbuff	db	10 dup(0)

	encryped_passwd	db	1dh, 06h, 1ah, 11h, 0dh, 44h, 59h, 1dh, 06h, 73h

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

cli
push	ax
push	offset anti_track2
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track2:	pop		ax
				call	far ptr refresh_avg
cli
push	ax
push	offset anti_track3
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track3:	pop		ax
				call	far ptr refresh_rank
cli
push	ax
push	offset anti_track4
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track4:	pop		ax
			;=== input the key =====================
cli
push	ax
push	offset anti_track5
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track5:	pop		ax
				puts	<offset info4>
cli
push	ax
push	offset anti_track6
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track6:	pop		ax
				gets	<offset input>, 9
cli
push	ax
push	offset anti_track7
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track7:	pop		ax

cli
push	ax
push	offset anti_track8
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track8:	pop		ax
				xor		ebx, ebx
cli
push	ax
push	offset anti_track9
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track9:	pop		ax
				mov		bl, input+1
cli
push	ax
push	offset anti_track10
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track10:	pop		ax
				mov		ecx, 0h
cli
push	ax
push	offset anti_track11
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track11:	pop		ax

cli
push	ax
push	offset anti_track12
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track12:	pop		ax
			append:
cli
push	ax
push	offset anti_track13
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track13:	pop		ax
				; append the key to 10 bytes
cli
push	ax
push	offset anti_track14
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track14:	pop		ax
				mov		al, buffer[ecx]
cli
push	ax
push	offset anti_track15
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track15:	pop		ax
				mov		buffer[ebx], al
cli
push	ax
push	offset anti_track16
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track16:	pop		ax
				inc		cx
cli
push	ax
push	offset anti_track17
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track17:	pop		ax
				inc		bx
cli
push	ax
push	offset anti_track18
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track18:	pop		ax
				cmp		bx, 0ah
cli
push	ax
push	offset anti_track19
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track19:	pop		ax
				jnz		append
cli
push	ax
push	offset anti_track20
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track20:	pop		ax

cli
push	ax
push	offset anti_track21
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track21:	pop		ax
				; change the key format
cli
push	ax
push	offset anti_track22
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track22:	pop		ax
				mov		ecx, 00h
cli
push	ax
push	offset anti_track23
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track23:	pop		ax
			change:
cli
push	ax
push	offset anti_track24
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track24:	pop		ax
				mov		al, buffer[ecx]
cli
push	ax
push	offset anti_track25
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track25:	pop		ax
				xor		al, buffer[ecx+1]
cli
push	ax
push	offset anti_track26
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track26:	pop		ax
				mov		buffer[ecx], al
cli
push	ax
push	offset anti_track27
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track27:	pop		ax
				inc		ecx
cli
push	ax
push	offset anti_track28
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track28:	pop		ax
				cmp		ecx, 09h
cli
push	ax
push	offset anti_track29
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track29:	pop		ax
				jnz		change
cli
push	ax
push	offset anti_track30
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track30:	pop		ax

cli
push	ax
push	offset anti_track31
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track31:	pop		ax
				; compare the passwd
cli
push	ax
push	offset anti_track32
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track32:	pop		ax
				mov	ecx, 00h
cli
push	ax
push	offset anti_track33
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track33:	pop		ax
			compare:
cli
push	ax
push	offset anti_track34
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track34:	pop		ax
				mov		al, encryped_passwd[ecx]
cli
push	ax
push	offset anti_track35
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track35:	pop		ax
				cmp		byte ptr buffer[ecx], al
cli
push	ax
push	offset anti_track36
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track36:	pop		ax
				jnz		wrongpasswd
cli
push	ax
push	offset anti_track37
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track37:	pop		ax
				inc		ecx
cli
push	ax
push	offset anti_track38
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track38:	pop		ax
				cmp		ecx, 0ah
cli
push	ax
push	offset anti_track39
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track39:	pop		ax
				jne		compare
cli
push	ax
push	offset anti_track40
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track40:	pop		ax
				jmp		correctpasswd
cli
push	ax
push	offset anti_track41
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track41:	pop		ax

cli
push	ax
push	offset anti_track42
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track42:	pop		ax
			wrongpasswd:
cli
push	ax
push	offset anti_track43
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track43:	pop		ax
				puts	<offset info5>
cli
push	ax
push	offset anti_track44
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track44:	pop		ax
				mov		ah, 4ch
cli
push	ax
push	offset anti_track45
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track45:	pop		ax
				int		21h
cli
push	ax
push	offset anti_track46
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track46:	pop		ax

cli
push	ax
push	offset anti_track47
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track47:	pop		ax
			correctpasswd:
cli
push	ax
push	offset anti_track48
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track48:	pop		ax
			;=======================================
cli
push	ax
push	offset anti_track49
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track49:	pop		ax
				cls
cli
push	ax
push	offset anti_track50
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track50:	pop		ax
				puts	<offset info1>
cli
push	ax
push	offset anti_track51
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track51:	pop		ax

cli
push	ax
push	offset anti_track52
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track52:	pop		ax
				mov		cx, 00h
cli
push	ax
push	offset anti_track53
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track53:	pop		ax
			print_loop:
cli
push	ax
push	offset anti_track54
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track54:	pop		ax
				; clear the buffer
cli
push	ax
push	offset anti_track55
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track55:	pop		ax
				push	cx
cli
push	ax
push	offset anti_track56
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track56:	pop		ax
				mov		cx, 50h
cli
push	ax
push	offset anti_track57
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track57:	pop		ax
				lea		si, line
cli
push	ax
push	offset anti_track58
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track58:	pop		ax
				lea		di, buffer
cli
push	ax
push	offset anti_track59
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track59:	pop		ax
				rep		movsb
cli
push	ax
push	offset anti_track60
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track60:	pop		ax
				pop		cx
cli
push	ax
push	offset anti_track61
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track61:	pop		ax

cli
push	ax
push	offset anti_track62
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track62:	pop		ax
				; initilize index
cli
push	ax
push	offset anti_track63
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track63:	pop		ax
				mov		eax, 00h
cli
push	ax
push	offset anti_track64
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track64:	pop		ax
				mov		ax, cx
cli
push	ax
push	offset anti_track65
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track65:	pop		ax
				mov		bx,	14h
cli
push	ax
push	offset anti_track66
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track66:	pop		ax
				mul		bx
cli
push	ax
push	offset anti_track67
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track67:	pop		ax

cli
push	ax
push	offset anti_track68
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track68:	pop		ax
				; perform a decrypt
cli
push	ax
push	offset anti_track69
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track69:	pop		ax
				push	cx
cli
push	ax
push	offset anti_track70
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track70:	pop		ax
				mov		cx, 14h
cli
push	ax
push	offset anti_track71
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track71:	pop		ax
				lea		di, loadbuf
cli
push	ax
push	offset anti_track72
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track72:	pop		ax
				lea		si, tab[eax]
cli
push	ax
push	offset anti_track73
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track73:	pop		ax
				rep		movsb
cli
push	ax
push	offset anti_track74
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track74:	pop		ax
				decrypt	<offset loadbuf>, 14h
cli
push	ax
push	offset anti_track75
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track75:	pop		ax
				pop		cx
cli
push	ax
push	offset anti_track76
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track76:	pop		ax

cli
push	ax
push	offset anti_track77
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track77:	pop		ax
				; if student not registered
cli
push	ax
push	offset anti_track78
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track78:	pop		ax
				cmp		loadbuf, byte ptr 0
cli
push	ax
push	offset anti_track79
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track79:	pop		ax
				je		not_reged
cli
push	ax
push	offset anti_track80
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track80:	pop		ax

cli
push	ax
push	offset anti_track81
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track81:	pop		ax
				; fill the name
cli
push	ax
push	offset anti_track82
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track82:	pop		ax
				lea		di, buffer+2
cli
push	ax
push	offset anti_track83
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track83:	pop		ax
				lea		si, loadbuf
cli
push	ax
push	offset anti_track84
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track84:	pop		ax
			name_s:
cli
push	ax
push	offset anti_track85
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track85:	pop		ax
				movsb
cli
push	ax
push	offset anti_track86
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track86:	pop		ax
				cmp		[si], byte ptr 0
cli
push	ax
push	offset anti_track87
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track87:	pop		ax
				jne		name_s
cli
push	ax
push	offset anti_track88
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track88:	pop		ax

cli
push	ax
push	offset anti_track89
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track89:	pop		ax
				; fill the chinese grade
cli
push	ax
push	offset anti_track90
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track90:	pop		ax
				mov		dx, word ptr loadbuf+10
cli
push	ax
push	offset anti_track91
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track91:	pop		ax
				itoa	<offset numbuff>, dx
cli
push	ax
push	offset anti_track92
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track92:	pop		ax
				lea		si, numbuff
cli
push	ax
push	offset anti_track93
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track93:	pop		ax
				lea		di, buffer+0fh
cli
push	ax
push	offset anti_track94
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track94:	pop		ax
			chinese_s:
cli
push	ax
push	offset anti_track95
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track95:	pop		ax
				movsb
cli
push	ax
push	offset anti_track96
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track96:	pop		ax
				cmp		[si], byte ptr '$'
cli
push	ax
push	offset anti_track97
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track97:	pop		ax
				jne		chinese_s
cli
push	ax
push	offset anti_track98
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track98:	pop		ax

cli
push	ax
push	offset anti_track99
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track99:	pop		ax
				; fill the math grade
cli
push	ax
push	offset anti_track100
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track100:	pop		ax
				mov		dx, word ptr loadbuf+12
cli
push	ax
push	offset anti_track101
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track101:	pop		ax
				itoa	<offset numbuff>, dx
cli
push	ax
push	offset anti_track102
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track102:	pop		ax
				lea		si, numbuff
cli
push	ax
push	offset anti_track103
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track103:	pop		ax
				lea		di, buffer+15h
cli
push	ax
push	offset anti_track104
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track104:	pop		ax
			math_s:
cli
push	ax
push	offset anti_track105
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track105:	pop		ax
				movsb
cli
push	ax
push	offset anti_track106
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track106:	pop		ax
				cmp		[si], byte ptr '$'
cli
push	ax
push	offset anti_track107
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track107:	pop		ax
				jne		math_s
cli
push	ax
push	offset anti_track108
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track108:	pop		ax

cli
push	ax
push	offset anti_track109
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track109:	pop		ax
				; fill the english grade
cli
push	ax
push	offset anti_track110
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track110:	pop		ax
				mov		dx, word ptr loadbuf+14
cli
push	ax
push	offset anti_track111
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track111:	pop		ax
				itoa	<offset numbuff>, dx
cli
push	ax
push	offset anti_track112
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track112:	pop		ax
				lea		si, numbuff
cli
push	ax
push	offset anti_track113
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track113:	pop		ax
				lea		di, buffer+1bh
cli
push	ax
push	offset anti_track114
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track114:	pop		ax
			english_s:
cli
push	ax
push	offset anti_track115
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track115:	pop		ax
				movsb
cli
push	ax
push	offset anti_track116
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track116:	pop		ax
				cmp		[si], byte ptr '$'
cli
push	ax
push	offset anti_track117
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track117:	pop		ax
				jne		english_s
cli
push	ax
push	offset anti_track118
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track118:	pop		ax

cli
push	ax
push	offset anti_track119
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track119:	pop		ax
				; fill the avreage grade
cli
push	ax
push	offset anti_track120
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track120:	pop		ax
				mov		dx, word ptr loadbuf+16
cli
push	ax
push	offset anti_track121
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track121:	pop		ax
				itoa	<offset numbuff>, dx
cli
push	ax
push	offset anti_track122
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track122:	pop		ax
				lea		si, numbuff
cli
push	ax
push	offset anti_track123
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track123:	pop		ax
				lea		di, buffer+21h
cli
push	ax
push	offset anti_track124
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track124:	pop		ax
			average_s:
cli
push	ax
push	offset anti_track125
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track125:	pop		ax
				movsb
cli
push	ax
push	offset anti_track126
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track126:	pop		ax
				cmp		[si], byte ptr '$'
cli
push	ax
push	offset anti_track127
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track127:	pop		ax
				jne		average_s
cli
push	ax
push	offset anti_track128
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track128:	pop		ax

cli
push	ax
push	offset anti_track129
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track129:	pop		ax
				; fill the rank
cli
push	ax
push	offset anti_track130
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track130:	pop		ax
				mov		dx, word ptr loadbuf+18
cli
push	ax
push	offset anti_track131
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track131:	pop		ax
				itoa	<offset numbuff>, dx
cli
push	ax
push	offset anti_track132
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track132:	pop		ax
				lea		si, numbuff
cli
push	ax
push	offset anti_track133
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track133:	pop		ax
				lea		di, buffer+27h
cli
push	ax
push	offset anti_track134
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track134:	pop		ax
			rank_s:
cli
push	ax
push	offset anti_track135
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track135:	pop		ax
				movsb
cli
push	ax
push	offset anti_track136
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track136:	pop		ax
				cmp		[si], byte ptr '$'
cli
push	ax
push	offset anti_track137
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track137:	pop		ax
				jne		rank_s
cli
push	ax
push	offset anti_track138
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track138:	pop		ax

cli
push	ax
push	offset anti_track139
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track139:	pop		ax
				puts	<offset buffer>
cli
push	ax
push	offset anti_track140
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track140:	pop		ax
				jmp		print_loop_tail
cli
push	ax
push	offset anti_track141
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track141:	pop		ax

cli
push	ax
push	offset anti_track142
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track142:	pop		ax
			not_reged:
cli
push	ax
push	offset anti_track143
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track143:	pop		ax
				puts	<offset info3>
cli
push	ax
push	offset anti_track144
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track144:	pop		ax

cli
push	ax
push	offset anti_track145
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track145:	pop		ax
			print_loop_tail:
cli
push	ax
push	offset anti_track146
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track146:	pop		ax
				inc		cx
cli
push	ax
push	offset anti_track147
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track147:	pop		ax
				cmp		cx, stu_max_num
cli
push	ax
push	offset anti_track148
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track148:	pop		ax
				jne		print_loop
cli
push	ax
push	offset anti_track149
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track149:	pop		ax

cli
push	ax
push	offset anti_track150
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track150:	pop		ax
				puts	<offset info2>
cli
push	ax
push	offset anti_track151
pop		ax
mov		ax, [esp-2]
jmp		ax
sti
anti_track151:	pop		ax
				gets	<offset numbuff>, 1

pop		es
pop		ds
popad
ret
printall endp

code ends
end start
