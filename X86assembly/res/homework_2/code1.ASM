--- ../../homework_1/src/STUDENT.ASM	2017-03-22 16:20:17.023425342 +0800
+++ STUMOD1.ASM	2017-03-28 14:59:37.691770314 +0800
@@ -151,8 +151,13 @@
		mov		ah, 4ch
		int		21h

-;-----------------------------------------------
find_s:
+;--- Adding the big loop for time test ---------
+		mov		cx, 7530H	;loop time
+		call	far ptr disptime
+loop_big_s label far
+		push	cx			;protect cx
+;-----------------------------------------------
		mov		esi, 0		; if input is a name
		mov		edi, 0		; counter
		;TODO check the legitimacy of the name
@@ -255,7 +260,7 @@

		mov		dx, offset output3 ; output the grade notification
		mov		ah, 09H
-		int		21H
+;		int		21H

			; output the average grade
		mov		si, 0		; serve as a counter
@@ -270,7 +275,7 @@
		add		al, 30H
		mov		dl, al
		mov		ah, 02H
-		int		21H
+;		int		21H

tens:
		mov		ax, cx		; the grade
@@ -289,7 +294,7 @@
		add		al, 30H
		mov		dl, al
		mov		ah, 02H
-		int		21H
+;		int		21H

units:
		mov		ax, cx		; the grade
@@ -300,19 +305,80 @@
		add		ah, 30H
		mov		dl, ah
		mov		ah, 02H
-		int		21H
+;		int		21H

		mov		dl,','
		mov		ah,02h
-		int		21H
+;		int		21H

		pop		dx			; get the level back
		mov		ah,	02h
-		int		21h
-		outreturn
+;		int		21h
+;		outreturn

+;------- End of the time testing -------------------------
+		pop		cx
+		dec		cx
+		jnz		far ptr	loop_big_s
+		call	far ptr disptime
+;---------------------------------------------------------
		jmp		start

stop:
+		mov		ah, 4ch
+		int		21h
+
+;====== other functions =================================
+disptime proc far
+	local timestr[11]:byte		;m,m,\',s,s,\",c,c,\r,\n,'$'
+		push	cx
+		push	dx
+		push	ds
+		push	bx
+
+		push	ss
+		pop		ds
+
+		mov		ah,2ch
+		int		21h
+
+		mov		bl,10
+
+		xor		ax,ax				; store the minutes
+		mov		al,cl
+		div		bl
+		add		ax,3030h
+		mov		word ptr timestr, ax
+
+		mov		timestr+2,27H		;\'
+
+		xor		ax,ax				; store the seconds
+		mov		al,dh
+		div		bl
+		add		ax,3030h
+		mov		word ptr timestr+3,ax
+
+		mov		timestr+5,'"'
+
+		xor		ax,ax				; store the centiseconds
+		mov		al,dl
+		div		bl
+		add		ax,3030h
+		mov		word ptr timestr+6,ax
+
+		mov		word ptr timestr+8,0a0dh
+		mov		timestr+10,'$'
+
+		lea		dx,timestr
+		mov		ah,9
+		int		21h
+
+		pop		bx
+		pop		ds
+		pop		dx
+		pop		cx
+		ret
+disptime	endp
+;---------------------------------------------------------
code ends
end start

