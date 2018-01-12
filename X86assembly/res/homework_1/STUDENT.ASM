.386

_stack segment use16 stack
		db		100	dup(0)
_stack ends

; CAUTION: use with care, affects ah and dl
outreturn macro
		mov		ah, 02h		; output an \r
		mov		dl, 0dh
		int		21h
		mov		ah, 02h		; output an \n
		mov		dl, 0ah
		int		21h
endm

data segment use16
		n		equ 064h
		tab		db	'Ju Song',	0,	0,	0,	100, 100, 99                ,0
				db	'Jing Lu',	0,	0,	0,	100, 100, 100               ,0
				db	'Yin Ma',	0,	0,	0,	0,	40, 80, 60              ,0
				db	'Yi Ng',	0,	0,	0,	0,	0,	0,	0,	0           ,0
				db	'Sixu Hu',	0,	0,	0,	95,	90,	87                  ,0

				db	'Xiao Huan',0,	3,	62,	7                           ,0
				db	'Fu Lim',	0,	0,	0,	0,	3,	38,	54              ,0
				db	'Ru Chan',	0,	0,	0,	50,	59,	90                  ,0
				db	'Ya Hu',	0,	0,	0,	0,	0,	53,	11,	63          ,0
				db	'Bao Han',	0,	0,	0,	74,	69,	10                  ,0
				db	'Wen Zhu',	0,	0,	0,	64,	85,	0                   ,0
				db	'Min Li',	0,	0,	0,	0,	89,	11,	51              ,0
				db	'Mu Song',	0,	0,	0,	40,	49,	71                  ,0
				db	'Ya Ma',	0,	0,	0,	0,	0,	93,	49,	75          ,0
				db	'Xun Hu',	0,	0,	0,	0,	54,	34,	79              ,0
				db	'Xue Lu',	0,	0,	0,	0,	56,	80,	77              ,0
				db	'Jia Lee',	0,	0,	0,	100,	86,	43              ,0
				db	'Wen Man',	0,	0,	0,	23,	87,	44                  ,0
				db	'Ju Yu',	0,	0,	0,	0,	0,	1,	38,	1           ,0
				db	'Zhi Wen',	0,	0,	0,	27,	77,	29                  ,0
				db	'Ya Bai',	0,	0,	0,	0,	85,	84,	47              ,0
				db	'Ru Guo',	0,	0,	0,	0,	63,	28,	80              ,0
				db	'Mu Hsu',	0,	0,	0,	0,	13,	69,	96              ,0
				db	'Ru Jin',	0,	0,	0,	0,	30,	47,	42              ,0
				db	'Chun Wu',	0,	0,	0,	91,	58,	55                  ,0
				db	'Xue Li',	0,	0,	0,	0,	100,	69,	0           ,0
				db	'Lan Lau',	0,	0,	0,	69,	99,	42                  ,0
				db	'Yong Xu',	0,	0,	0,	24,	60,	3                   ,0
				db	'Lim Wu',	0,	0,	0,	0,	21,	59,	95              ,0
				db	'Fu Li',	0,	0,	0,	0,	0,	68,	11,	68          ,0
				db	'Xun Lim',	0,	0,	0,	7,	90,	76                  ,0
				db	'Ru Han',	0,	0,	0,	0,	97,	42,	98              ,0
				db	'Wu Ng',	0,	0,	0,	0,	0,	92,	6,	14          ,0
				db	'Ju Hou',	0,	0,	0,	0,	91,	100,	77          ,0
				db	'Ah Xun',	0,	0,	0,	0,	60,	82,	100             ,0
				db	'Min Yu',	0,	0,	0,	0,	19,	70,	91              ,0
				db	'Shu Hou',	0,	0,	0,	93,	19,	65                  ,0
				db	'Shi Liu',	0,	0,	0,	19,	9,	23                  ,0
				db	'Hua Wen',	0,	0,	0,	23,	3,	58                  ,0
				db	'Tu Chan',	0,	0,	0,	76,	96,	59                  ,0
				db	'Ya Tan',	0,	0,	0,	0,	4,	24,	62              ,0
				db	'Ru Ng',	0,	0,	0,	0,	0,	26,	77,	39          ,0
				db	'Xia Wen',	0,	0,	0,	96,	53,	81                  ,0
				db	'Da Lu',	0,	0,	0,	0,	0,	46,	86,	54          ,0
				db	'Hui Guo',	0,	0,	0,	47,	60,	97                  ,0
				db	'Na Tan',	0,	0,	0,	0,	51,	51,	41              ,0
				db	'Mu Liu',	0,	0,	0,	0,	31,	48,	1               ,0
				db	'Yang Lu',	0,	0,	0,	62,	7,	83                  ,0
				db	'Zan Sun',	0,	0,	0,	10,	0,	15                  ,0
				db	'Zan Chu',	0,	0,	0,	7,	34,	34                  ,0
				db	'Tai Hu',	0,	0,	0,	0,	66,	97,	56              ,0
				db	'Tu Yu',	0,	0,	0,	0,	0,	14,	69,	47          ,0
				db	'Ya Ruan',	0,	0,	0,	86,	49,	7                   ,0
				db	'Bai Li',	0,	0,	0,	0,	51,	75,	78              ,0
				db	'He Fan',	0,	0,	0,	0,	9,	76,	9               ,0
				db	'Fu Zhao',	0,	0,	0,	8,	47,	50                  ,0
				db	'Chao Hu',	0,	0,	0,	43,	70,	20                  ,0
				db	'Jin Bai',	0,	0,	0,	35,	19,	69                  ,0
				db	'Xiu Li',	0,	0,	0,	0,	4,	54,	89              ,0
				db	'Tao Guo',	0,	0,	0,	60,	53,	21                  ,0
				db	'Yun Yu',	0,	0,	0,	0,	61,	0,	67              ,0
				db	'Ru Chen',	0,	0,	0,	82,	69,	68                  ,0
				db	'Bao Wu',	0,	0,	0,	0,	87,	20,	94              ,0
				db	'Min Chu',	0,	0,	0,	17,	45,	13                  ,0
				db	'Jie Yu',	0,	0,	0,	0,	48,	82,	81              ,0
				db	'Zan Ng',	0,	0,	0,	0,	54,	69,	7               ,0
				db	'Na Guan',	0,	0,	0,	61,	49,	96                  ,0
				db	'Chao Xu',	0,	0,	0,	60,	27,	62                  ,0
				db	'Lim Yu',	0,	0,	0,	0,	74,	65,	63              ,0
				db	'Hong Yu',	0,	0,	0,	88,	86,	59                  ,0
				db	'Bai Wen',	0,	0,	0,	74,	94,	92                  ,0
				db	'Jin Lim',	0,	0,	0,	31,	44,	2                   ,0
				db	'Hui Pan',	0,	0,	0,	9,	61,	21                  ,0
				db	'Jin Xu',	0,	0,	0,	0,	86,	1,	7               ,0
				db	'Ru Wang',	0,	0,	0,	9,	25,	80                  ,0
				db	'Mu Tan',	0,	0,	0,	0,	73,	41,	97              ,0
				db	'Qing Yu',	0,	0,	0,	49,	47,	8                   ,0
				db	'Huan Xu',	0,	0,	0,	58,	36,	70                  ,0
				db	'Wei Yu',	0,	0,	0,	0,	24,	63,	17              ,0
				db	'Yin Xun',	0,	0,	0,	13,	28,	88                  ,0
				db	'Bao Man',	0,	0,	0,	11,	97,	41                  ,0
				db	'Jun Lin',	0,	0,	0,	77,	88,	71                  ,0
				db	'Ru Guan',	0,	0,	0,	55,	94,	6                   ,0
				db	'Da Sung',	0,	0,	0,	24,	63,	25                  ,0
				db	'Wen Man',	0,	0,	0,	38,	56,	56                  ,0
				db	'Wen Guo',	0,	0,	0,	84,	57,	90                  ,0
				db	'Lan Xun',	0,	0,	0,	80,	88,	28                  ,0
				db	'Hua Hsu',	0,	0,	0,	93,	66,	33                  ,0
				db	'Da Lim',	0,	0,	0,	0,	49,	95,	24              ,0
				db	'Tu Yu',	0,	0,	0,	0,	0,	8,	9,	88          ,0
				db	'Ming Wu',	0,	0,	0,	78,	77,	2                   ,0
				db	'Yun Yu',	0,	0,	0,	0,	3,	10,	73              ,0
				db	'Wen Hu',	0,	0,	0,	0,	35,	57,	56              ,0
				db	'Xia Lee',	0,	0,	0,	62,	58,	27                  ,0
				db	'Ah Sung',	0,	0,	0,	9,	26,	89                  ,0
				db	'Wu Wu',	0,	0,	0,	0,	0,	32,	49,	61          ,0
				db	'Lan Chu',	0,	0,	0,	22,	0,	65                  ,0
				db	'Min Lau',	0,	0,	0,	55,	60,	95                  ,0
				db	'Dong Ma',	0,	0,	0,	46,	74,	33                  ,0
				db	'Jia Bai',	0,	0,	0,	100,	1,	96              ,0
		in_name	db 10
				db ?
		in_buff	db 10 dup(0)
		output1	db 'Please input the name of the student: $'
		output2	db 'Student not found, please re-input ... ',0dh,0ah,'$'
		output3 db 'the grades of the student are: $'
data ends

code segment use16
assume ss:_stack, ds:data, cs:code
start:
		mov		ax, data
		mov		ds, ax
input:		; input the name of student
		mov		ah, 09h		; show input message
		lea		dx, output1
		int		21h

		mov		ah, 0ah		; input the name
		lea		dx, in_name
		int		21h

		outreturn

		cmp		in_buff, 0dh		; if input is \r
		jz		input

		cmp		in_buff, 71h		; test if quit
		jnz		find_s
		cmp		in_buff+01h, 0dh
		jnz		find_s			; q but not q\r
		mov		ah, 4ch
		int		21h

;-----------------------------------------------
find_s:
		mov		esi, 0		; if input is a name
		mov		edi, 0		; counter
		;TODO check the legitimacy of the name
find:		; find the student's name
		mov		ecx,0		; name length counter
		mov		eax, edi
		mov		bx, 0eh		; mov bx, 14
		mul		bx
		mov		esi, eax
if_equal:	; if the two string are equal
		cmp		in_buff[ecx], 0dh
		jnz		find_w		; if input not reach end
		cmp		byte ptr tab[esi][ecx], 0 ; if reach end in the same time
		jz		found		; if found
		cmp		ecx, 0ah	;
		jz		found		;
		jmp		next		; if not found in this row
find_w:		; if the two word are equal
		cmp		ecx, 0ah	; avoid length go over 10
		jz		next		; if the length is over 10

		mov		al, in_buff[ecx]
							; compare two character
		cmp		byte ptr tab[esi][ecx], al
		jne		next
		inc		ecx
		jmp		if_equal	;if this character matches
next:		; two string not equal, go next string
		inc		edi
		cmp		edi, n
		jne		find		; if length is not equal
			; if not found
		mov		ah, 09h
		lea		dx, output2
		int		21h
		jmp		input

;-----------------------------------------------
found:		; if found
		;TODO show detail grade of the student
		mov		bl, 02h		; calculate the average grade of the student
		mov		ecx, 0ah
		movzx	ax, tab[esi][ecx]
		mul		bx
		push	ax

		inc		ecx
		movzx	ax, tab[esi][ecx]
		push	ax

		inc		ecx
		movzx	ax, tab[esi][ecx]
		div		bl
		mov		ah, 0

		add		ax, [esp]
		add		esp, 02h	; perform a pop
		add		ax, [esp]
		add		esp, 02h	; perform a pop

		mov		bx, 02h
		mul		bx
		mov		bx, 07h
		div		bl

		inc		ecx			; store the average grade
		mov		tab[esi][ecx], al

;-----------------------------------------------
			;judge the Level
		movzx	cx, al
		mov		bx, 5ah		; mov bx, 90
		cmp		cx, bx
		js		b
		mov		dl, 'A'
		jmp		outgrade
b:
		mov		bx, 50h		; mov bx, 80
		cmp 	cx, bx
		js		c
		mov		dl, 'B'
		jmp		outgrade
c:
		mov		bx, 46h		; mov bx, 70
		cmp 	cx, bx
		js		d
		mov		dl, 'C'
		jmp		outgrade
d:
		mov		bx, 3ch		; mov bx, 60
		cmp 	cx, bx
		js		f
		mov		dl, 'D'
		jmp		outgrade
f:
		mov		dl, 'F'

outgrade:
		push	dx			; temporaryly store the Level

		mov		dx, offset output3 ; output the grade notification
		mov		ah, 09H
		int		21H

			; output the average grade
		mov		si, 0		; serve as a counter
hundrends:
		mov		ax, cx		; the grade
		mov		bl, 64H
		div		bl			; get the hundrends
		cmp		al, 0		; compare the quotient
		jz		tens		; if hundrends is zero, then not output
		inc		si
hundrends_out:
		add		al, 30H
		mov		dl, al
		mov		ah, 02H
		int		21H

tens:
		mov		ax, cx		; the grade
		mov		bl, 64H
		div		bl			; get the tens
		cmp		ah, 0
		mov		al, ah
		cbw
		mov		bl, 0ah
		div		bl
		cmp		al, 0
		jnz		tens_out	; if tens is zero and hundrends is zero, then not output
		cmp		si, 0
		jz		units
tens_out:
		add		al, 30H
		mov		dl, al
		mov		ah, 02H
		int		21H

units:
		mov		ax, cx		; the grade
		mov		bl, 0ah
		div		bl			; get the tens
		cmp		ah, 0		; compare remaings
units_out:
		add		ah, 30H
		mov		dl, ah
		mov		ah, 02H
		int		21H

		mov		dl,','
		mov		ah,02h
		int		21H

		pop		dx			; get the level back
		mov		ah,	02h
		int		21h
		outreturn

		jmp		start

stop:
code ends
end start
