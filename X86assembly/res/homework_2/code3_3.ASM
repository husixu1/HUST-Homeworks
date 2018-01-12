ifndef	??version
?debug	macro
	endm
$comm	macro	name,dist,size,count
	comm	dist name:BYTE:count*size
	endm
	else
$comm	macro	name,dist,size,count
	comm	dist name[size]:BYTE:count
	endm
	endif
	?debug	S "stu_c.c"
	?debug	C E974797D4A077374755F632E63
	?debug	C E98D0A7D4A15433A5C54435C494E434C5544455C737464696F2E68
	?debug	C E98D0A7D4A15433A5C54435C494E434C5544455C5F646566732E68
	?debug	C E98D0A7D4A15433A5C54435C494E434C5544455C5F6E756C6C2E68
_TEXT	segment byte public 'CODE'
_TEXT	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
_BSS	ends
_DATA	segment word public 'DATA'
	db	10 dup (?)
_DATA	ends
_TEXT	segment byte public 'CODE'
   ;
   ;	int main(void){
   ;
	assume	cs:_TEXT
_main	proc	near
	push	bp
	mov	bp,sp
	sub	sp,10
   ;
   ;		char in_name[10]="";
   ;
	lea	ax,word ptr [bp-10]
	push	ss
	push	ax
	mov	ax,offset DGROUP:d@w+0
	push	ds
	push	ax
	mov	cx,10
	call	near ptr N_SCOPY@
@1@58:
   ;
   ;
   ;		do{
   ;			printf("Please input the student's name: ");
   ;
	mov	ax,offset DGROUP:s@
	push	ax
	call	near ptr _printf
	pop	cx
   ;
   ;			scanf("%[^\n]",in_name);
   ;
	lea	ax,word ptr [bp-10]
	push	ax
	mov	ax,offset DGROUP:s@+34
	push	ax
	call	near ptr _scanf
	pop	cx
	pop	cx
   ;
   ;			getchar();
   ;
	dec	word ptr DGROUP:__streams
	jl	short @1@114
	mov	bx,word ptr DGROUP:__streams+10
	inc	word ptr DGROUP:__streams+10
	mov	al,byte ptr [bx]
	mov	ah,0
	jmp	short @1@142
@1@114:
	mov	ax,offset DGROUP:__streams
	push	ax
	call	near ptr __fgetc
	pop	cx
@1@142:
   ;
   ;		}while(!in_name[0]);
   ;
	mov	al,byte ptr [bp-10]
	cbw
	or	ax,ax
	je	short @1@58
   ;
   ;
   ;		if(in_name[0]=='q' && !in_name[1])
   ;
	cmp	byte ptr [bp-10],113
	jne	short @1@254
	mov	al,byte ptr [bp-9]
	cbw
	or	ax,ax
	jne	short @1@254
@1@226:
   ;
   ;			return 0;
   ;
	xor	ax,ax
	jmp	short @1@282
@1@254:
   ;
   ;
   ;		printf("entering next procedure...\n");
   ;
	mov	ax,offset DGROUP:s@+40
	push	ax
	call	near ptr _printf
	pop	cx
	jmp	short @1@226
@1@282:
   ;
   ;		return 0;
   ;	}
   ;
	mov	sp,bp
	pop	bp
	ret
_main	endp
	?debug	C E9
_TEXT	ends
_DATA	segment word public 'DATA'
s@	label	byte
	db	'Please input the student'
	db	39
	db	's name: '
	db	0
	db	'%[^'
	db	10
	db	']'
	db	0
	db	'entering next procedure...'
	db	10
	db	0
_DATA	ends
_TEXT	segment byte public 'CODE'
_TEXT	ends
	extrn	N_SCOPY@:far
	public	_main
	extrn	__fgetc:near
	extrn	_scanf:near
	extrn	_printf:near
	extrn	__streams:word
_s@	equ	s@
	end

