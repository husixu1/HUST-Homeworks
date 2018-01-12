.386
.model flat, stdcall
option casemap :none   ; case sensitive

include windows.inc
include user32.inc
include kernel32.inc
include comctl32.inc
include gdi32.inc
include macrolib

includelib user32.lib
includelib kernel32.lib
includelib comctl32.lib
includelib gdi32.lib

; Local macros
szText MACRO Name, Text:VARARG
    LOCAL lbl
        jmp lbl
        Name db Text,0
        lbl:
ENDM

; move words in memory
movm MACRO M1, M2
    push M2
    pop  M1
endm

return MACRO arg
    mov eax, arg
    ret
ENDM

; prototypes
startMainWindow PROTO :DWORD,:DWORD,:DWORD,:DWORD
signalHandle    PROTO :DWORD,:DWORD,:DWORD,:DWORD
TopXY           PROTO :DWORD,:DWORD
Paint_Proc      PROTO :DWORD,:DWORD
Static          PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
ListBox         PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
EnmProc         PROTO :DWORD,:DWORD
RefreshAvg      PROTO
refresh_rank    PROTO

student struc
    naming  db 10 dup(0)
    chinese dw 0
    math    dw 0
    english dw 0
    average dw 0
    Rank    dw 0
student ends

.data
    szDisplayName db "SIMS - Student Information Management System",0
    CommandLine   dd 0
    MainWindow    dd 0
    hInstance     dd 0
    hStatus       dd 0
    hToolBar      dd 0
    hList         dd 0

    stu_max_num   equ 10
    tab           student <'alpha',95,96, 97,,>
                  student <'beta',80,90,100,,>
                  student <'gamma',100,95,100,,>
                  student <'delta',60,65,66,,>
                  student <'epsilon',50,97, 86,,>
                  student 4 dup(<>)
                  student <'husixu',98,99,88,,>

;-- code segment ---------------------------------------------------------------
.code
start:
    ; get the handle of the module
    invoke GetModuleHandle, NULL
    mov hInstance, eax
    ; get the handle of the commandline parameter
    invoke GetCommandLine
    mov CommandLine, eax
    ; start the main window process
    invoke startMainWindow, hInstance,NULL,CommandLine,SW_SHOWDEFAULT
    ;exit
    invoke ExitProcess,eax


;-- Main Process ---------------------------------------------------------------
startMainWindow proc hInst     :DWORD,
                     hPrevInst :DWORD,
                     CmdLine   :DWORD,
                     CmdShow   :DWORD
; local variables
LOCAL   windclass :WNDCLASSEX
LOCAL   msg       :MSG

LOCAL   wid        :DWORD
LOCAL   height     :DWORD
LOCAL   topx       :DWORD
LOCAL   topy       :DWORD

invoke InitCommonControls
; initialize the main window
mov windclass.cbSize,              sizeof WNDCLASSEX
mov windclass.style,               CS_HREDRAW or CS_VREDRAW or CS_BYTEALIGNWINDOW
mov windclass.lpfnWndProc,         offset signalHandle
mov windclass.cbClsExtra,          0
mov windclass.cbWndExtra,          0
movm windclass.hInstance,          hInst   ;<< NOTE: macro not mnemonic
mov windclass.hbrBackground,       COLOR_BTNFACE+1
mov windclass.lpszMenuName,        NULL
mov windclass.lpszClassName,       offset szClassName

invoke LoadIcon, hInst, 500    ; icon ID
mov windclass.hIcon,               eax

invoke LoadCursor, NULL, IDC_ARROW
mov windclass.hCursor,             eax
mov windclass.hIconSm,             0

invoke RegisterClassEx, ADDR windclass

; set width and height
mov wid, 640
mov height, 500

invoke GetSystemMetrics, SM_CXSCREEN
invoke TopXY, wid, eax
mov topx, eax
invoke GetSystemMetrics, SM_CYSCREEN
invoke TopXY, height, eax
mov topy, eax

szText szClassName,"Comctl_Class"

; create the main window
invoke CreateWindowEx, WS_EX_LEFT,
                      ADDR szClassName,
                      ADDR szDisplayName,
                      WS_OVERLAPPEDWINDOW,
                      topx, topy, wid, height,
                      NULL, NULL,
                      hInst, NULL
mov   MainWindow, eax

invoke LoadMenu, hInst, 600  ; menu ID
invoke SetMenu, MainWindow, eax
invoke ShowWindow, MainWindow, SW_SHOWNORMAL
invoke UpdateWindow, MainWindow

; message loop
looping:
    invoke GetMessage, ADDR msg, NULL, 0, 0
    cmp eax, 0
    je ending
    invoke TranslateMessage, ADDR msg
    invoke DispatchMessage,  ADDR msg
    jmp looping
ending:
return msg.wParam
startMainWindow endp


;-- message handling -----------------------------------------------------------
signalHandle proc hWin   :DWORD,
             uMsg        :DWORD,
             wParam      :DWORD,
             lParam      :DWORD

    LOCAL caW   :DWORD
    LOCAL caH   :DWORD
    LOCAL hDC   :DWORD
    LOCAL Rct   :RECT
    LOCAL tbb   :TBBUTTON
    LOCAL Tba   :TBADDBITMAP
    LOCAL Ps    :PAINTSTRUCT

    LOCAL rLeft   :DWORD
    LOCAL rTop    :DWORD
    LOCAL rRight  :DWORD
    LOCAL rBottom :DWORD

    szText tbSelect,"You have selected"

    ; window creation
    .if uMsg == WM_CREATE
            invoke ListBox, 20, 25, 590, 400, hWin, 600
            mov hList, eax
            jmp @F
                lbl1 db " Name",0
                lbl2 db " Chinese",0
                lbl3 db " English",0
                lbl4 db " Math",0
                lbl5 db " Average",0
                lbl6 db " Rank",0
            @@:

            invoke Static, ADDR lbl1, hWin,  20, 5, 90,  18, 500
            invoke Static, ADDR lbl2, hWin, 120, 5, 90,  18, 501
            invoke Static, ADDR lbl3, hWin, 220, 5, 90,  18, 502
            invoke Static, ADDR lbl4, hWin, 320, 5, 90,  18, 503
            invoke Static, ADDR lbl5, hWin, 420, 5, 90,  18, 504
            invoke Static, ADDR lbl6, hWin, 520, 5, 90,  18, 504

;-- Menu Events ----------------------------------------------------------------
    .elseif uMsg == WM_COMMAND
        .if wParam == 1000
            invoke SendMessage, hWin, WM_SYSCOMMAND, SC_CLOSE, NULL

        ; calculate the average score
        .elseif wParam == 1100
            ; TODO: refresh all scores
            szText TheMsg, "The Average Score and Rank has been refreshed",0
            invoke RefreshAvg
			invoke refresh_rank
            invoke MessageBox, hWin,ADDR TheMsg, ADDR szDisplayName, MB_OK

        ; show the whole list
        .elseif wParam == 1200
            invoke SendMessage, hList, LB_RESETCONTENT, 0, 0
            invoke EnumWindows, ADDR EnmProc, 0

        ; about
        .elseif wParam == 1300
            szText AboutMsg, "Author: Sixu Hu", 0dh, "Mail: husixu1@hotmail.com", 0
            invoke MessageBox, hWin, ADDR AboutMsg, ADDR szDisplayName, MB_OK

        .endif

;-- Resize Events --------------------------------------------------------------
    .elseif uMsg == WM_SIZE
        ; move the status bar
        invoke SendMessage,hToolBar,TB_AUTOSIZE,0,0
        movm caW, lParam[0]  ; client area width
        movm caH, lParam[2]  ; client area height
        invoke GetWindowRect,hStatus,ADDR Rct
        mov eax, Rct.bottom
        sub eax, Rct.top
        sub caH, eax
        invoke MoveWindow,hStatus,0,caH,caW,caH,TRUE

        .if hList != 0
            invoke GetClientRect, hWin, ADDR Rct
            movm rLeft, Rct.left
            add rLeft, 20
            movm rTop, Rct.top
            add rTop, 25
            movm rRight, Rct.right
            sub rRight, 40
            movm rBottom, Rct.bottom
            sub rBottom, 45
            invoke MoveWindow, hList, rLeft, rTop, rRight, rBottom, TRUE
        .endif

    .elseif uMsg == WM_PAINT
        invoke BeginPaint,hWin,ADDR Ps
          mov hDC, eax
          invoke Paint_Proc, hWin, hDC
        invoke EndPaint, hWin, ADDR Ps
        return 0

;-- closing the window ---------------------------------------------------------
    .elseif uMsg == WM_CLOSE
        szText TheText,"Do you really want to exit?", 0dh, "All changes will be lost."
        invoke MessageBox, hWin, ADDR TheText, ADDR szDisplayName, MB_YESNO
        .if eax == IDNO
            return 0
        .endif

    .elseif uMsg == WM_DESTROY
        invoke PostQuitMessage, NULL
        return 0

    .endif

    ; execute the pre-defined window processes
    invoke DefWindowProc, hWin, uMsg, wParam, lParam
    ret
signalHandle endp


;== sub processes ==============================================================
;===============================================================================

TopXY proc wDim:DWORD, sDim:DWORD
    shr sDim, 1      ; divide screen dimension by 2
    shr wDim, 1      ; divide window dimension by 2
    mov eax, wDim    ; copy window dimension into eax
    sub sDim, eax    ; sub half win dimension from half screen dimension
    return sDim
TopXY endp

;-------------------------------------------------------------------------------

Paint_Proc proc hWin:DWORD, hDC:DWORD
    LOCAL caW :DWORD
    LOCAL caH :DWORD
    LOCAL tbH :DWORD
    LOCAL sbH :DWORD
    LOCAL Rct :RECT

    invoke GetClientRect,hWin,ADDR Rct
    movm caW, Rct.right
    movm caH, Rct.bottom

    invoke GetWindowRect,hToolBar,ADDR Rct
    mov eax, Rct.bottom
    sub eax, Rct.top
    mov tbH, eax

    invoke GetWindowRect,hStatus,ADDR Rct
    mov eax, Rct.bottom
    sub eax, Rct.top
    mov sbH, eax

    mov eax, caH
    sub eax, sbH
    mov caH, eax

    mov Rct.left, 0
    movm Rct.top, tbH
    movm Rct.right, caW
    movm Rct.bottom, caH

    invoke DrawEdge,hDC,ADDR Rct,EDGE_SUNKEN,BF_RECT
    return 0
Paint_Proc endp

;-- list box creation ----------------------------------------------------------
ListBox proc a:DWORD, b:DWORD, wd:DWORD, ht:DWORD, hParent:DWORD, ID:DWORD
    LOCAL hFont :DWORD
    LOCAL hLst  :DWORD
    szText lstBox,"LISTBOX"
    invoke CreateWindowEx, WS_EX_CLIENTEDGE, ADDR lstBox, 0,
              WS_VISIBLE or WS_BORDER or WS_CHILD or \
              LBS_HASSTRINGS or LBS_NOINTEGRALHEIGHT or \
              LBS_DISABLENOSCROLL,
              a, b, wd, ht, hParent, ID, hInstance, NULL

    mov hLst, eax

    invoke GetStockObject, SYSTEM_FIXED_FONT      ; ANSI_FIXED_FONT
    mov hFont, eax
    invoke SendMessage,hLst,WM_SETFONT,hFont, 0

    mov eax, hLst

    ret

ListBox endp

;-- static item creation -------------------------------------------------------
Static proc lpText:DWORD,hParent:DWORD,
             a:DWORD,b:DWORD,wd:DWORD,ht:DWORD,ID:DWORD
LOCAL hStat :DWORD
LOCAL hFont :DWORD

szText statClass,"STATIC"

invoke CreateWindowEx,WS_EX_STATICEDGE,
        ADDR statClass,lpText,
        WS_CHILD or WS_VISIBLE or SS_LEFT,
        a,b,wd,ht,hParent,ID,
        hInstance,NULL

    mov hStat, eax

    invoke GetStockObject,ANSI_FIXED_FONT
    mov hFont, eax
    invoke SendMessage,hStat,WM_SETFONT,hFont, 0

    mov eax, hStat

    ret

Static endp

;-- List Refreshing ------------------------------------------------------------
EnmProc proc eHandle :DWORD, y :DWORD
    LOCAL Buffer[256]     :BYTE
    LOCAL clName[64]      :BYTE

    mov ecx, 00h
    szText ctlstr1,"%10s %7u %11u %11u"
    szText ctlstr2,"%s %11u %11u"
    szText BufferNone, '      ----     ---         ---         ---         ---         ---   '
    
	looping:
    mov eax, ecx
    mov ebx, 14h
    mul ebx
    push ecx
        ;TODO: convert the number to string in the tab
		
		.if tab[eax].naming[0] == 0
			invoke SendMessage, hList, LB_ADDSTRING, 0, ADDR BufferNone
		.else
			pushad
			xor ebx, ebx
			xor ecx, ecx
			xor edx, edx
			xor esi, esi
			xor edi, edi
			mov bx, tab[eax].chinese
			mov cx, tab[eax].math
			mov dx, tab[eax].english
			mov si, tab[eax].average
			mov di, tab[eax].Rank
			
			invoke wsprintf, ADDR Buffer, ADDR ctlstr1, ADDR tab[eax].naming, ebx, ecx, edx
			.if si!=0
				invoke wsprintf, ADDR Buffer, ADDR ctlstr2, ADDR Buffer, esi, edi
			.endif
			
			popad
			invoke SendMessage, hList, LB_ADDSTRING, 0, ADDR Buffer
		.endif
		
    pop ecx
    inc ecx
    cmp ecx, 0ah
    jne looping

    ;mov eax, eHandle
    mov eax, 0
    ret
EnmProc endp

;-- refresh average score ------------------------------------------------------

; \brief calculate a student's avgscore and store it in tab
; \para (2 byte) index of the student in stack
; \return (2 byte) avg score in stack (CAN NOT BE IGNORED)
cal_one_avg proc
push	eax
push	ebx
push	ecx
push	edx
	mov		eax, 00h
	mov		edx, 00h					; dx stores the avg score
	mov		ax, [esp+14h]				; get index (parameter)
	mov		ebx, 14h					; multiple by 20
	mul		ebx
	mov		ecx, eax					; ecx as the pointer
	mov		ax, 0						; ax as the total sum

	mov		dx, word ptr tab[ecx].chinese	; get chinese score
	shl		dx, 1
	add		ax,	dx

	mov		dx, word ptr tab[ecx].math		; get math score
	add		ax,	dx

	mov		dx, word ptr tab[ecx].english	; get english score
	shr		dx, 1
	add		ax,	dx

	mov		dx, 0h
	shl		ax, 1
	mov		bx, 07H						;s divide by 3.5
	div		bx

	mov		word ptr tab[ecx].average, ax	; store it to correct position
pop		edx
pop		ecx
pop		ebx
pop		eax
ret
cal_one_avg endp

;---------------------------------
RefreshAvg proc
	mov		cx, 00h
refresh_loop:
	mov		eax, 00h
	mov		ax, cx
	mov		bx, 14h
	mul		bx
								; if the not registered
	mov		bl, tab[eax].naming[0]
	cmp		bl, 00h
	jne		cal_this
	jmp		cal_this_end
	
cal_this:
	push	cx					; pass the parameter
	call    cal_one_avg
	pop		ax					; get the result (but do not use)

cal_this_end:
	inc		cx
	cmp		cx, stu_max_num
	jnz		refresh_loop
ret
RefreshAvg endp


;-- refresh ranking -----------------------------------------------------------


; \brief calculate a student's ranking and store it in tab
; \para (2 byte) index of the student in stack
; \return (2 byte) rank in stack (CAN NOT BE IGNORED)
cal_one_rank proc
push	eax
push	ebx
push	ecx
push	edx
push	esi
	mov		eax, 00h
	mov		esi, 00h					; esi store the rank
	mov		ax, [esp+18h]				; get index (parameter)
	mov		ebx, 14h					; multiple by 20
	mul		ebx
	mov		bx, word ptr tab[eax].average	; ax stores the current avgscore

	mov		cx, 00h
loop_rank:
	push	bx
	mov		eax, 00h
	mov		bx, 14h
	mov		ax, cx
	mul		bx

	cmp		tab[eax].naming[0], byte ptr 0
	je		not_reged

	pop		bx
	cmp		word ptr tab[eax].average, bx
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

	mov		ax, [esp+18h]				; get index (parameter)
	mov		ebx, 14h					; multiple by 20
	mul		ebx

	mov		word ptr tab[eax].Rank, si	; store it to correct position
pop		esi
pop		edx
pop		ecx
pop		ebx
pop		eax
ret
cal_one_rank endp

; \brief refresh all student's rank, but do NOT refresh avgscore automatically, use with care
refresh_rank proc
	mov		cx, 00h
refresh_loop:
	mov		eax, 00h
	mov		ax, cx
	mov		bx, 14h
	mul		bx
									; if the not registered
	cmp		tab[eax].naming[0], byte ptr 00h
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
ret
refresh_rank endp
; ########################################################################

end start
