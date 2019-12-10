;在数据段定义一个待读取的CMOS内部单元的地址编号。再使用IN/OUT指令，读取CMOS内的指定单元的信息。
;将读取的信息用16进制的形式显示在屏幕上。若是时间信息，可以人工判断一下是否与操作系统显示的时间一致。
.386
STACK  SEGMENT    STACK USE16
            DB  200  DUP(0)
STACK  ENDS

CODE            SEGMENT      USE16
ASSUME       DS:CODE,SS:STACK,CS:CODE
;新的int 8h使用的变量
count        db 18
month        db ?,?,':'
day          db ?,?,','
hour         db ?,?,':'
min          db ?,?,':'
sec          db  ?,?
buf_ien       =$-month  ;计算显示的信息长度，“$”表示当前这条指令在代码段中的偏移量。
cursor       dw ?       ;原光标的位置
old_int      dw ?,?       
;新的int 8h的代码
new08h  proc far
        pushf  ;本指令可以把标志寄存器的内容保存到堆栈中去
        call  dword ptr cs:old_int
        dec   cs:count
        jz    disp
        iret
disp:   mov cs:count,18
		sti
		pusha
		push DS
		push es
		mov ax,cs
		mov ds,AX
		mov es,AX
		call GET_TIME
		mov bh,0
		mov ah,3
		int 10h
		mov cursor,dx
		mov bp,offset month
		mov bh,0
		mov dh,0
		mov dl,80-buf_ien
		mov bl,07h
		mov cx,buf_ien
		mov al,0
		mov ah,13h
		int 10h
		mov bh,0
		mov dx,cursor
		mov ah,2
		int 10h   ;还原光标位置的目的是为了保证主程序的光标位置不受影响
		pop es
		pop DS
		popa
		iret
new08h endp
GET_TIME proc
mov al,8
out 70h,al
jmp $+2
in al,71h
mov ah,al
and al,0fh
shr ah,4
add ax,3030h
xchg ah,al
mov word ptr month,AX
mov al,7
out 70h,al
jmp $+2
in al,71h
mov ah,al
and al,0fh
shr ah,4
add ax,3030h
xchg ah,al
mov word ptr day,AX
mov al,4
out 70h,al
jmp $+2
in al,71h
mov ah,al
and al,0fh
shr ah,4
add ax,3030h
xchg ah,al
mov word ptr hour,AX
mov al,2
out 70h,al
jmp $+2
in al,71h
mov ah,al
and al,0fh
shr ah,4
add ax,3030h
xchg ah,al
mov word ptr min,AX
mov al,0
out 70h,al
jmp $+2
in al,71h
mov ah,al
and al,0fh
shr ah,4
add ax,3030h
xchg ah,al
mov word ptr sec,AX
ret
GET_TIME endp

START:
push cs
pop DS
mov ax,3508h
int 21h        ;取中断信息，al=中断类型号 ex【bx】=入口地址
mov old_int,bx
mov old_int+2,es
mov dx,offset new08h
mov ax,2508h
int 21h
next:
mov ah,0
int 16h
cmp al,"q"
jne next
lds dx,dword ptr old_int  ;恢复原中断
mov ax,2508h;al=中断类型号 ds:[dx]=入口地址
int 21h
mov ah,4ch
int 21h
CODE ENDS
END START
