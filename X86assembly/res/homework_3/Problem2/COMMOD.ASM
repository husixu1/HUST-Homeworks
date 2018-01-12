.386
include macrolib

public		name_to_index
public		printname
public		tab
public		stu_max_num

_stack segment use16 stack "stack"
	db	0ffh dup(0)
_stack ends

_DATA segment use16 public "DATA"
	stu_max_num		equ 10
	tab		dw	stu_max_num	dup(5 dup(0), -1h, -1h ,-1h, -1h, -1h)
	clsstr 	db 'cls',0
_DATA ends

_TEXT segment use16 public "CODE"
	assume	cs:_TEXT, ds:_DATA, ss:_stack, es:_DATA
start:

; \para (2 byte) address of the name string in stack, string end with \0\0...\0
; \return (2byte) index of the table, in stack, return -1 if not found
name_to_index proc far
pushad
push	ds
push	es
mov		ax, _DATA
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
mov		ax, _DATA
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

	mov		dl,  byte ptr tab[ecx]
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

;clear proc
;extrn	_system:near
;pushad
;	mov		ax, offset clsstr
;	push	ax
;	call	_system
;	pop		ax
;popad
;ret
;clear endp

_TEXT ends
end start

