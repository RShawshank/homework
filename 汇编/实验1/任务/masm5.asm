.386
STACK  SEGMENT  USE16  STACK
            DB  200  DUP(0)
STACK  ENDS

DATA            SEGMENT      USE16
BNAME           DB                'ZHANG SAN',0              ;老板姓名（必须是自己名字的拼音）此处0的ascll码为00（空字符
BPASS           DB                'test',0,0                    ;密码
N               EQU               30
SNAME           DB                'SHOP','$'                       ;网店名称，用0结束
GA1             DB                'PEN', 7 DUP(0) ,10    ;商品名称及折扣
                DW                35,56,70,25,0          ; 推荐度还未计算
GA2             DB                'BOOK', 6 DUP(0) ,9    ; 商品名称及折扣  一共21个字长度
                DW                12,30,25,5,0           ;推荐度还未计算
GAN             DB                N-2 DUP( 'Temp-Value',8,15,0,20,0,30,0,2,0,0,0) ;除了2个已经具体定义了的商品信息以外，其他商品信息暂时假定为一样的。
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
LOOPAFORPASSWORD:MOV               SI,1
				MOV                DI,-1
LOOPA:          LEA                BX,IN_PWD
                MOV                CL,4
				CMP                IN_PWD+1,4
		        JNE                START	
				INC                SI
				INC                DI
				CMP                BPASS[DI],'t'          ;当到达'0'时，跳出循环
				JE                 ADD_AUTH_ONE           ;auth赋值 1 ，并且跳到功能3
				POP                DI
				MOV                CL,[BX][SI]
				CMP                BPASS[DI],CL
				JE                 LOOPA
				JNE                START                  ;失败返回功能1
ADD_AUTH_ONE : 	
				push               DI
				MOV                DI,0
				MOV                AUTH[DI],1
THREE:          LEA                DX,BUF3              ;输出待查看的商品名提醒
			    MOV                AH,9
			    INT                21H
			    LEA                DX,IN_GOODS        ;输入商品名
		        MOV                AH,10
			    INT                21H
			    MOV                DI,2
			    CMP                IN_GOODS[DI],0DH      ;输入回车键
			    JE                 START                 ;返回功能1
			    LEA                BX,GA1                ;将GA1的基址作为子程序的参数传入
			    MOV                AL,N                  ;BL作为商品数量
;功能：判断输入的商品名和以BX作为基址的商品做比较	
	  
  
LOOPA_number: MOV                  SI,1
			  MOV                  DI,-1
              DEC                  AL
LOOPA_GOODS:  INC                  SI
			  INC                  DI
			  CMP                  BYTE PTR [BX+DI],0           ;商品名输入正确
			  JE                   CHECK_LOAD                   ;判断登录状态
			  MOV                  CL,IN_GOODS[SI]
			  CMP                  [BX][DI],CL
			  JE                   LOOPA_GOODS
			  ADD                  BX,21                  ;商品名输入不正确，检查下一个商品名
			  CMP                  AL,0
			  JE                   THREE                   ;未找到商品，返回功能三
			  JNE                  LOOPA_number            ;还有商品，继续对比下一个商品			  
CHECK_LOAD:   MOV                  DI,0
			  CMP                  AUTH[DI],1
			  JE                   SHOWGOODSNAME           ;登录正确，展示商品信息
			  JNE                  INTOFOUTH               ;计算推荐度，进入功能四			  
SHOWGOODSNAME:
              MOV                  BYTE PTR[BX+SI],'$' 
			  LEA                  DX,BUF5
			  MOV                  AH,9
			  INT                  21H
			  LEA                  DX,[BX]
			  MOV                  AH,9
			  INT                  21H
			  JMP                  START                 
INTOFOUTH:    
              MOV                  CX,0
			  MOV                  AX,0
			  MOV                  AX,WORD PTR [BX+11]   ;进货价格
			  MOV                  CX,WORD PTR [BX+13]   ;销售价格
			  MOV                  DX,128
			  IMUL                 AX,DX
			  MOV                  DX,0                  ;防止DX溢出
			  CMP                  CX,0
			  JE                   OVERFLOW
			  DIV                  CX
			  push                 AX
			  PUSH                 BX
			  MOV                  CX,0
			  MOV                  AX,0
			  POP                  BX
			  MOV                  AX,WORD PTR [BX+17]    ;已售数量
			  MOV                  CX,WORD PTR [BX+15]     ;进货数量
			  MOV                  DX,128
			  IMUL                 AX,DX 
			  MOV                  DX,0
			  CMP                  AX,0
			  JE                   OVERFLOW
			  DIV                  CX   
			  POP                  DX
			  ADD                  AX,DX
			  MOV                  WORD PTR [BX+19],AX
			  JMP                  FOUTH
OVERFLOW :    LEA                  DX,BUF4         ;输出价格为0
			  MOV                  AH,9
			  INT                  21H
			  MOV                  WORD PTR  [BX+19],0         ;推荐度为0
FOUTH:
              CMP                  AL,100
			  
			  JS                   L1
			  LEA                  DX,BUF5
			  MOV                  AH,9
			  INT                  21H
			  MOV                  DL,041H       ;推荐度大于100,输出A
			  MOV                  AH,2
			  INT                  21H
			  LEA                  DX,NEXT        ;让程序显示
		      MOV                  AH,10
			  INT                  21H
L1:           CMP                  AL,50
              JS                   L2
			  LEA                  DX,BUF5
			  MOV                  AH,9
			  INT                  21H
			  MOV                  DL,042H       ;推荐度大于50,输出B
			  MOV                  AH,2
			  INT                  21H
			  LEA                  DX,NEXT        ;让程序显示
		      MOV                  AH,10
			  INT                  21H
L2:           CMP                  AL,10
              JS                   L3
			  LEA                  DX,BUF5
			  MOV                  AH,9
			  INT                  21H
			  MOV                  DL,043H  ;推荐度大于10，输出C
			  MOV                  AH,2
			  INT                  21H
			  LEA                  DX,BUF5
			  MOV                  AH,9
			  INT                  21H
			  LEA                  DX,NEXT        ;让程序显示
		      MOV                  AH,10
			  INT                  21H
L3:           MOV                  DL,046H
              MOV                  AH,2
			  INT                  21H
			  LEA                  DX,NEXT        ;让程序显示
		      MOV                  AH,10
			  INT                  21H
ENDCODE:	  MOV                  AX, 4C00H
			  INT                  21H
		    CODE ENDS
			END START
