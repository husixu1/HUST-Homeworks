.386
assume cs:code, ds:data, ss:stack
n equ 3 ;学生总数

stack segment use16 stack
    db 200 dup (0)
stack ends

data segment use16
;在此处定义了程序主界面的内容 使用9号调用输出
head    db 10, 'Welcome to student scores query system.', 10, '$'
info1   db '1.Input the name of student', 10, '$'
info2   db '2.Query if the name input in 1 exists', 10, '$'
info3   db '3.Calculate the average score of all students', 10, '$'
info4   db '4.Evaluate the level of the student query in 2', 10, '$'
info5   db 'Please input the number, q to exit', 10, 10, '$'
;在此处定义了各种操作的结果 使用9号调用输出
passwdinfo  db 10, 'Please input the password:', '$'
inputinfo   db 10, 'Please input the name:','$'
unfoundinfo db 10, 'Student NOT found!', '$'
foundinfo   db 10, 'Student Found!', '$'
averginfo   db 10, 'Average Scores Calculated!', '$'
levelinfo   db 10, 'The level of the student query in 2 is:', '$'
wronginfo   db 10, 'Wrong password or debugging traced!',  '$'
;在此处定义了学生的成绩
student db 'Z' xor 'p', 'h' xor 'a', 'a' xor 's', 'n' xor 's', 'g' xor 'w', 'S' xor 'o', 0 xor 'r', 0 xor 'd', 0 xor '2', 0 xor '0', 100 xor 'a', 85 xor 'S', 80 xor 'm', 0
        db 'L' xor 'p', 'i' xor 'a', 'S' xor 's', 0 xor 's', 0 xor 'w', 0 xor 'o', 0 xor 'r', 0 xor 'd', 0 xor '2', 0 xor '0', 80 xor 'a', 100 xor 'S', 70 xor 'm', 0
        db 'C' xor 'p', 'h' xor 'a', 'e' xor 's', 'n' xor 's', 'Z' xor 'w', 'h' xor 'o', 0 xor 'r', 0 xor 'd', 0 xor '2', 0 xor '0', 85 xor 'a', 95 xor 'S', 100 xor 'm', 0
;在此处定义了程序中用到的缓冲区
poin    dw 0
buffer  db 10, 0
inname  db 10 dup (0)
buffer2 db 12, 0
inpw    db 12 dup (0)
p1      dw pass1
e1      dw wronga
p2      dw keyprocess
data ends

code segment use16
pwkey   db 'password204'
table   dw input, query, averg, level ;直接定址表
passwd  db 11 xor ' ', 'c' xor 'p','z' xor 'a','h' xor 's','1' xor 's','9' xor 'w','9' xor 'o','7' xor 'r','0' xor 'd','4' xor '2','0' xor '0','2' xor '4'
start:  mov ax, data
        mov ds, ax ;关联数据段寄存器
        mov ax, code
        mov es, ax
        mov ah, 9
        lea dx, passwdinfo
        int 21h
        lea dx, buffer2
        mov ah, 10
        int 21h
        cli
        mov ah, 2ch
        int 21h
        push dx
        mov al, buffer2 + 1
        xor al, ' '
        sub al, passwd
        movsx bx, al
        add bx, offset p1
        mov ah, 2ch
        int 21h
        sti
        cmp dx, [esp]
        pop dx
        jz ok1
        mov bx, offset e1
ok1:    mov bx, [bx]
        cmp word ptr cs:[bx], 0bb9h
        jz ok2
        jmp e1
ok2:    jmp bx
        db 'LaLaLa'
pass1:  mov cx, 11
        cli
        push p2
        mov si, 0
        pop ax
        mov bx, [esp-2]
        sti
        jmp bx
        db 'LueLueLue'
keyprocess:
        mov al, [inpw + si]
        xor al, cs:[pwkey + si]
        mov [inpw + si], al
        inc si
        loop keyprocess
        lea si, inpw
        lea di, passwd + 1
        sti
        mov cx, 11
        repz cmpsb
        jne wronga
main:   call print ;调用子程序 打印主界面
        mov ah, 1
        int 21h ;使用1号调用 读入字符
        cmp al, 113
        je mainend ;读取到了q 退出
        cmp al, 49
        jb main
        cmp al, 52
        ja main ;排除不合法的输入 重新跳转至开始
        mov bl, al
        sub bl, 49
        add bl, bl
        mov bh, 0
        call word ptr table[bx] ;根据直接定址表直接调用相应地子程序
        jmp main ;返回开头
mainend:
        mov ax, 4c00h ;程序返回
        int 21h
        db 'Dummy information here'
wronga:
        mov ah, 9
        lea dx, wronginfo
        int 21h
        mov ax, 4c01h
        int 21h
        db 'Trying to avoid this'
;说明 打印主界面
;参数 无
;结果 无
print:  push ax
        push dx ;参数压栈
        mov ah, 9
        lea dx, head
        int 21h
        lea dx, info1
        int 21h
        lea dx, info2
        int 21h
        lea dx, info3
        int 21h
        lea dx, info4
        int 21h
        lea dx, info5
        int 21h ;依次使用9号调用打印信息
        pop dx ;参数退栈
        pop ax
        ret

;说明 调用10号调用 将字符串读入inname中
;参数 无
;结果 无
input:  push ax
        push dx
        push cx
        push si ;参数压栈
inputstart:
        lea dx, inputinfo
        mov ah, 9
        int 21h ;使用9号调用 打印提示信息
        mov cx, 10
        mov si, 0
        mov al, 0
clearname:
        mov inname[si], al
        inc si
        loop clearname ;设置循环10次 用si寻址 al保存0 实现清空inname中的内容
        lea dx, buffer
        mov ah, 10
        int 21h ;使用10号调用 读入数据
        mov al, buffer + 1
        cmp al, 0
        je inputstart ;判断有效字符是否为0 如果是证明只读入了回车 重新读入
        mov ah, 0
        movsx si, al
        mov [inname + si], ah
        mov cx, 10
        mov si, 0
encryprocess:
        mov al, [inname + si]
        xor al, cs:[pwkey + si]
        mov [inname + si], al
        inc si
        loop encryprocess
        pop si ;参数退栈
        pop cx
        pop dx
        pop ax
        ret

;说明 根据inname中的内容查找所有学生中是否有名字和输入名字相同的学生 有则将成绩表的起始地址保存到poin中
;参数 无
;结果 无
query:  push ax
        push bx
        push cx
        push si ;参数压栈
        mov bx, offset student ;bx保存每个条目的起始地址
        mov cx, n ;cx保存循环次数
;外部循环 一次遍历所有学生
studentloop:
        push cx ;为了内部循环将cx压栈
        mov cx, 10 ;重设循环次数为姓名的长度
        mov si, 0 ;内部循环寻址寄存器
;内部循环 一次遍历一个学生的名字
nameloop:
        mov ah, [bx + si] ;从学生成绩表取得当前字符
        mov al, inname[si] ;从inname取得当前字符
        cmp al, 0dh ;比较如果inname已经结束 则认为查询成功
        je queryfound
        cmp ah, al ;比较两个字符是否相等
        jne nameloopend ;不相等则直接跳出循环
        inc si ;指向下一个字符
        loop nameloop
        jmp queryfound ;循环完成 认为查询成功
nameloopend:
        add bx, 14 ;指向下一个学生
        pop cx ;恢复cx
        loop studentloop
        mov ah, 9
        lea dx, unfoundinfo
        int 21h ;使用9号系统调用输出未找到信息
        jmp queryend ;结束子程序
queryfound:
        pop cx ;从循环中跳出 需要首先恢复cx 保证push和pop匹配
        mov poin, bx ;将地址保存到poin
        mov ah, 9
        lea dx, foundinfo
        int 21h ;使用9号调用输出找到信息
queryend:
        pop si ;参数退栈
        pop cx
        pop bx
        pop ax
        ret

;说明 计算所有学生的平均成绩
;参数 无
;结果 无
averg:
        push cx
        push bx
        push ax
        push dx ;参数入栈
        mov cx, n ;设定循环次数
        mov bx, offset student ;bx指向学生成绩表的第一条记录
avergloop:
        mov ah, 4
        mov al, [10 + bx]
        xor al, 'a'
        mul ah
        mov dx, ax ;计算语文成绩*4并且用dx暂存
        mov ah, 2
        mov al, [11 + bx]
        xor al, 'S'
        mul ah
        add dx, ax ;计算数学成绩*2并且和dx相加
        mov ah, 0
        mov al, [12 + bx]
        xor al, 'm'
        add dx, ax ;让英语成绩和dx相加
        mov ax, dx ;将dx返回ax
        mov dl, 7
        div dl ;除以7 现在al中保存了正确的结果
        mov [13 + bx], al ;保存结果
        add bx, 14
        loop avergloop
        mov ah, 9
        lea dx, averginfo
        int 21h ;使用9号调用输出信息
        pop dx ;参数退栈
        pop ax
        pop bx
        pop cx
        ret

;说明 评定2中学生的等级 并输出
;参数 无
;结果 无
level:  push ax
        push bx
        push dx ;参数入栈
        mov bx, poin ;从poin中得出相应的偏移量 bx现在指向2中查找到的记录
        mov bl, [bx + 13] ;取得平均成绩
        mov bh, 71 ;bh为'G' 根据不懂成绩依次自减即可得到结果
        cmp bl, 90
        jnb levelA
        cmp bl, 80
        jnb levelB
        cmp bl, 71
        jnb levelC
        cmp bl, 60
        jnb levelD
        jb levelF
levelA: dec bh
levelB: dec bh
levelC: dec bh
levelD: dec bh
        dec bh
levelF: dec bh
        lea dx, levelinfo
        mov ah, 9
        int 21h ;使用9号调用输出信息
        mov dl, bh
        mov ah, 2 ;使用2号调用输出等级
        int 21h
        pop dx ;参数退栈
        pop bx
        pop ax
        ret

code ends
end start
