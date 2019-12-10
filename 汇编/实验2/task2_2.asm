.386
STACK  SEGMENT  USE16  STACK
            DB  2000  DUP(0)
STACK  ENDS

DATA            SEGMENT      USE16
BNAME           DB                'ZHANG SAN',0              ;老板姓名（必须是自己名字的拼音）此处0的ascll码为00（空字符
BPASS           DB                'test',0,0                    ;密码
N               EQU               3000
m               EQU               10
a               EQU               0
SNAME           DB                'SHOP','$'                       ;网店名称，用0结束
;商品名称（10个字节，数据段中定义时，名称不足部分补0），折扣：取值0~10；0表示免费赠送，10表示不打折，1~9为折扣率；实际销售价格=销售价*折扣/10）
;进货价,销售价，进货总数，已售数量，推荐度【=（进货价/实际销售价格+已售数量/进货数量）*128】。
GA1             DB                'PEN', 7 DUP(0) ,10    ;商品名称及折扣
                DW                35,56,70,25,0          ; 推荐度还未计算
GA2             DB                'BOOK', 6 DUP(0) ,9    ; 商品名称及折扣  一共21个字长度
                DW                12,30,25,5,0           ;推荐度还未计算
GAN             DB                N-3 DUP( 'Temp-Value',8,15,0,20,0,30,0,2,0,0,0) ;除了2个已经具体定义了的商品信息以外，其他商品信息暂时假定为一样的。
GA3             DB                'bag',7 DUP(0),9      ;商品名称为10个字节，折扣，进货价，销售价，进货总数，已售数量，推荐度
                DW                10,10,m,a,0            ;m表示是进货总数，a表示的是已售数量
BUF1            DB                0DH,0AH,'please input the name?','$'
BUF2            DB                0DH,0AH,'please input the password?','$'
BUF3            DB                0DH,0AH,'please input what do you want to search?','$'
BUF4            DB                0DH,0AH,'the place is zero.','$'
BUF5            DB                0DH,0AH,'$'
IN_NAME         DB                10   
                DB                0
		        DB                10   DUP(0)
IN_PWD		  	DB                6
				DB                0 
				DB                6    DUP(0)
IN_GOODS        DB                10
				DB                0
				DB                10   DUP(0)
NEXT            DB                10
                DB                0
				DB                10   DUP(0)
AUTH            DB                0
GOODS           DB                0;     记录商品编号 
number_m        DW                N
DIV_            DD                0
DATA  ENDS

CODE            SEGMENT      USE16
                ASSUME       DS:DATA,SS:STACK,CS:CODE
START  :        
				MOV                AX,STACK
				MOV                SS,AX
				MOV                SP,200
				MOV                AX, DATA
				MOV                DS,AX
				MOV                DI,2
				LEA                DX,SNAME+0 ;输出网店名字
				MOV                AH,9
				INT                21H
				LEA                DX,BUF1    ;提醒输入姓名
				MOV                AH,9
				INT                21H
				LEA                DX,IN_NAME   ;输入姓名
				MOV                AH,10
				INT                21H	
				CMP                IN_NAME[DI],'q'         ;判断是否输入的是q
				JE                 END_THIS                ;程序退出
				JMP				   CONTINUE				
END_THIS:	
				JMP                ENDCODE
CONTINUE:		CMP                IN_NAME[DI],0DH
				JE                 ADD_AUTH                ;输入回车键，则将auth赋值为0
JUDGE:          pop                DI
				CMP                IN_NAME[DI],0DH         ;判断是否输入的是回车键
				JE                 THREE                   ;进入功能三，跳过功能二
				LEA                DX,BUF2                 ;提醒输入密码
				MOV                AH,9
				INT                21H
				LEA                DX,IN_PWD               ;输入密码
				MOV                AH,10
				INT                21H
				JMP                SECOND
ADD_AUTH:       PUSH               DI   
				MOV                DI,0
				MOV                AUTH[DI],0
			    JMP                JUDGE
SECOND	:       MOV                SI,1
				MOV                DI,-1
LOOPAFORNAME:   LEA                BX,IN_NAME
				MOV                CL,9
                CMP                IN_NAME+1,CL
				JNE                START
				INC                SI
				INC                DI
				CMP                BNAME[DI],'N'          ;当到达'N'时，跳出循环
				JE                 LOOPAFORPASSWORD       ;开始判断密码是否输入正确
				MOV                CL,[BX+SI]
				CMP                BNAME[DI],CL	
				JE                 LOOPAFORNAME           ;继续比较姓名的字符
				JNE                START                  ;失败返回功能1
LOOPAFORPASSWORD:MOV               SI,1                   ;开始判断密码是否输入正确
				MOV                DI,-1
LOOPA:          LEA                BX,IN_PWD
                MOV                CL,4
				CMP                IN_PWD+1,4
		        JNE                START	
				INC                SI
				INC                DI
				CMP                BPASS[DI],'t'          ;当到达't'时，跳出循环,即密码正确
				JE                 ADD_AUTH_ONE           ;auth赋值 1 ，并且跳到功能3
				POP                DI
				MOV                CL,[BX][SI]
				CMP                BPASS[DI],CL
				JE                 LOOPA
				JNE                START                  ;失败返回功能1
ADD_AUTH_ONE : 	
				push               DI
				XOR                DI,DI
				MOV                AUTH[DI],1             
				
				
				
				
THREE:          MOV                number_m,N
				LEA                DX,BUF3              ;输出待查看的商品名提醒
			    MOV                AH,9
			    INT                21H
			    LEA                DX,IN_GOODS          ;输入商品名
		        MOV                AH,10
			    INT                21H
			    MOV                DI,2
				call               disptime               ;计时开始
next3:		    XOR                AX,AX
				call               function_recommend    ;计算推荐度
				CMP                number_m,AX
				JE                 end_time              ;循环结束，结束计时
				CMP                IN_GOODS[DI],0DH      ;输入回车键
			    JE                 START                 ;返回功能1
			    LEA                BX,GA1                ;将GA1的基址作为子程序的参数传入
			    MOV                AX,N                  ;AX作为商品数量
;功能：判断输入的商品名和以BX作为基址的商品做比较	
;两次循环，第一次循环其数量，第二次循环判断其商品名是否正确
LOOPA_number: MOV                  SI,1
			  MOV                  DI,-1
			  DEC                  AX
LOOPA_GOODS:  INC                  SI
			  INC                  DI
			  CMP                  BYTE PTR [BX+DI],0           ;商品名输入正确
			  JE                   calculate_number        
			  MOV                  CL,IN_GOODS[SI]
			  CMP                  [BX][DI],CL
			  JE                   LOOPA_GOODS
			  ADD                  BX,21                        ;商品名输入不正确，检查下一个商品名
			  CMP                  AX,0
			  JE                   THREE                        ;未找到商品，返回功能三
			  JNE                  LOOPA_number                 ;还有商品，继续对比下一个商品	   
end_time:    
              call                 disptime
			  JMP                  THREE
;有商品时就卖掉       
calculate_number:
			  PUSH                AX
			  mov                 ax,number_m
			  dec                 ax
			  mov                 number_m,ax
			  pop                 ax
			  MOV                 AX,WORD PTR[BX+17]
			  cmp                 AL,[BX+15]
			  JGE                 next3        ;下一个人
			  INC                 AL
			  MOV                 [BX+17],AL                
			  JMP                  next3			  
;功能：计算shop中的所有商品的推荐度
;传入：商品数量、第一个商品的基址	
;需要注意的是，当N值较大时，BX可能会溢出。需要使用EBX		  
function_recommend proc
			  push                 EAX
			  push                 EBX
			  push                 ECX
			  push                 DX
			  LEA                  BX,GA1   ;基址
			  sub                  BX,21
			  MOV                  AX,N
loopa1:   	  
			  PUSH                 AX
			  add                  BX,21        ;下一个商品
			  MOVZX                EAX,WORD PTR[BX+11]   ;进货价格
			  MOVZX                ECX,WORD PTR[BX+13]   ;销售价格
			  SAL                  EAX,7
			  XOR                  EDX,EDX
			  CMP                  ECX,0
			  JE                   OVERFLOW
			  DIV                  ECX
			  PUSH                 EAX
			  XOR                  ECX,ECX
			  XOR                  EAX,EAX
			  MOVZX                EAX,WORD PTR[BX+17]    ;已售数量
			  MOVZX                ECX,WORD PTR[BX+15]     ;进货数量
			  SAL                  EAX,7
			  CMP                  ECX,0
			  JE                   OVERFLOW
			  DIV                  ECX
			  POP                  EDX
			  ADD                  EAX,EDX
			  MOV                  WORD PTR[BX+19],AX  
			  POP                  AX
			  DEC                  AX
			  cmp                  AX,0
              JE                   next1;返回
			  JMP                  loopa1
OVERFLOW :    MOV                  BYTE PTR[BX+19],0;推荐度为0	
next1:		  POP                  DX
			  POP                  ECX
			  POP                  EBX
			  POP                  EAX
			  RET
function_recommend endp			  
;判断该商品已售数量是否大于等于进货总数，若是，则回到功能一（1），否则将已售数量加1。
;传入参数：BX--商品的基址

disptime proc        ;显示秒和百分秒，精度为55ms。(未保护ax寄存器)
    local timestr[8]:byte     ;0,0,'"',0,0,0dh,0ah,'$'
		 push ax
         push cx
         push dx         
         push ds
         push ss
         pop  ds
         mov  ah,2ch 
         int  21h
         xor  ax,ax
         mov  al,dh
         mov  cl,10
         div  cl
         add  ax,3030h
         mov  word ptr timestr,ax
         mov  timestr+2,'"'
         xor  ax,ax
         mov  al,dl
         div  cl
         add  ax,3030h
         mov  word ptr timestr+3,ax
         mov  word ptr timestr+5,0a0dh
         mov  timestr+7,'$'    
         lea  dx,timestr  
         mov  ah,9
         int  21h    
         pop  ds 
         pop  dx
         pop  cx
		 pop  ax
         ret
disptime	endp

ENDCODE:	  MOV                  AX, 4C00H
			  INT                  21H
		    CODE ENDS
			END START
