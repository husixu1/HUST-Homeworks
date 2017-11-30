--- STUMOD1.ASM	2017-03-28 14:59:37.691770314 +0800
+++ STUTUNE.ASM	2017-03-28 20:28:09.054451103 +0800
@@ -159,21 +159,13 @@
		push	cx			;protect cx
 ;-----------------------------------------------
		mov		esi, 0		; if input is a name
-		mov		edi, 0		; counter
 find:		; find the student's name
		mov		ecx,0		; name length counter
-		mov		eax, edi
-		mov		bx, 0eh		; mov bx, 14
-		mul		bx
-		mov		esi, eax
 if_equal:	; if the two string are equal
		cmp		in_buff[ecx], 0dh
		jnz		find_w		; if input not reach end
		cmp		byte ptr tab[esi][ecx], 0 ; if reach end in the same time
		jz		found		; if found
-		cmp		ecx, 0ah	;
-		jz		found		;
		jmp		next		; if not found in this row
 find_w:		; if the two word are equal
		cmp		ecx, 0ah	; avoid length go over 10
@@ -186,8 +178,8 @@
		inc		ecx
		jmp		if_equal	;if this character matches
 next:		; two string not equal, go next string
-		inc		edi
-		cmp		edi, n
+		add		esi, 0eh
+		cmp		esi, 0578h
		jne		find		; if length is not equal
			; if not found
		mov		ah, 09h

