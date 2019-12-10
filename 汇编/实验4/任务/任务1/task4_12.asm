.386
STACK  SEGMENT  USE16  STACK
            DB  200  DUP(0)
STACK  ENDS
DATA SEGMENT USE16
buf1 DW 4 DUP(0)
buf2 DW 4 DUP(0) 
DATA ENDS
CODE            SEGMENT      USE16
                ASSUME       DS:DATA,SS:STACK,CS:CODE		 
START:
MOV                AX,STACK
MOV                SS,AX
MOV                SP,200
MOV                AX,DATA
MOV                DS,AX
PUSH               DS
XOR                AX,AX
XOR                bx,bx
MOV                DS,AX
MOV                BX,DS:[1H*4]
POP                DS
MOV                buf1[0],BX
PUSH               DS
mov                ds,AX
mov                bx,DS:[1h*4+2]
pop                DS
mov                buf1[2],BX
push               DS
MOV                DS,AX
MOV                BX,DS:[13H*4]
POP                DS
MOV                buf2[0],BX
push               DS
MOV                DS,AX
MOV                BX,DS:[13H*4+2]
POP                DS
MOV                buf2[2],BX
CODE ENDS
END START  