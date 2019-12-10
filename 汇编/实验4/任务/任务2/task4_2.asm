.386
CODE            SEGMENT      USE16
                ASSUME       SS:STACK,CS:CODE
old_int dw ?,?
NEW16H:

cmp ah,00h
je  NEW
cmp ah,10h
je  NEW
jne notmyjob
NEW:
pushf;对应的是call 中的iret
CALL dword ptr old_int
CMP AL,41H
JB quit
CMP AL,5AH
JA  QUIT
ADD al,20h
jmp quit
notmyjob:
jmp dword ptr old_int;这里的iret直接返回到中断前指令
quit:
iret
START:
xor   ax,ax
mov   ds,ax
mov   ax,ds:[16h*4];  取int 16h的中断矢量的偏移部分
mov   old_int,ax
mov   ax,ds:[16h*4+2]
mov   old_int+2,ax
CLI
mov       word ptr ds:[16h*4],offset NEW16H 
mov       ds:[16h*4+2],CS
STI
mov       dx,offset START+15
mov       cl,4
shr       dx,cl
add       dx,10h
mov       al,0
mov       ah,31h
int       21h
CODE ENDS
 STACK  SEGMENT    STACK USE16
            DB  200  DUP(0)
STACK  ENDS
			END START