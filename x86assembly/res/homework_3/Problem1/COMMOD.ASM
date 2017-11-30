.386
include macrolib

public		name_to_index
public		printname

extrn		tab:byte
extrn		stu_max_num:abs

_stack segment use16 stack "stack"
_stack ends

data segment use16 public "data"
data ends

code segment use16 public "code"
	assume	cs:code, ds:data, ss:_stack, es:data
start:

; \para (2 byte) address of the name string in stack, string end with \0\0...\0
; \return (2byte) index of the table, in stack, return -1 if not found
name_to_index proc far
pushad
push	ds
push	es
mov		ax, data
mov		ds, ax
mov		es, ax
	mov		ecx, 0h
find_s:
	mov		si, [esp+28h]	; get the name address
	mov		eax, 0h
	mov		ax,	cx
	mov		bx, 14h
	mul		bx
	lea		di, tab[eax]

	push	cx				; store the index
	mov		cx, 0ah

	repe	cmpsb			; compare two string
	je		found			; if found

	pop		cx				; if not found this turn, pop the index
	inc		cx
	cmp		cx, stu_max_num
	jne		find_s			; loop till found/notfound

notfound:					; if not found in table
	mov		[esp+28h], word ptr -1h
	jmp		ending

found:
	pop		cx				; get the index
	mov		[esp+28h], cx

ending:
pop		es
pop		ds
popad
ret
name_to_index endp


; \brief print the name in the table, without the return
; \parameter index(2 bytes) in stack
; \return index(2 bytes) in stack, can NOT be ignored)
printname proc far
pushad
push	ds
push	es
mov		ax, data
mov		ds, ax
mov		es, ax
	mov		eax, 00h
	mov		ax, [esp+28h]	; get the index
	mov		bx, 14h
	mul		bx
	mov		ecx, eax

printchar:
	cmp		tab[ecx], byte ptr 0
	je		printend

	mov		dl, tab[ecx]
	mov		ah, 02h
	int		21h

	inc		ecx
	jmp		printchar

printend:
pop		es
pop		ds
popad
ret
printname endp

code ends
end start

