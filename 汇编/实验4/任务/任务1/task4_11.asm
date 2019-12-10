.386
STACK  SEGMENT  USE16  STACK
            DB  200  DUP(0)
STACK  ENDS
DATA SEGMENT  USE16 
DATA ENDS
CODE            SEGMENT      USE16
                ASSUME       DS:DATA,SS:STACK,CS:CODE		 
START:
MOV                AX,STACK
MOV                SS,AX
MOV                SP,200
MOV                AX,0
MOV                DS,AX
mov                al,1h
mov                ah,35h
int                21h
mov                al,13h     ;bx=中断号是n的中断处理程序偏移地址  es=中断号是n的中断处理程序段地址
mov                ah,35h
int                21h
CODE ENDS
END START  