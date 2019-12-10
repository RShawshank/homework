public       _GA1,_THREE1,_THREE2,_RECOMMEND_RANK,_RECOMMEND,_RANK,_INFORMATIONS
.386
ASSUME       DS:DATA,SS:STACK,CS:CODE 
STACK  SEGMENT  USE16  STACK
DB  200  DUP(0)
STACK  ENDS
DATA            SEGMENT      USE16
BNAME           DB                'raohongbin',0              ;老板姓名（必须是自己名字的拼音）此处0的ascll码为00（空字符
BPASS           DB                'test',0,0                    ;密码
N               EQU               3
;m               EQU               1
;a               EQU               0
SNAME           DB                'SHOP','$'                       ;网店名称，用0结束
;商品名称（10个字节，数据段中定义时，名称不足部分补0），折扣：取值0~10；0表示免费赠送，10表示不打折，1~9为折扣率；实际销售价格=销售价*折扣/10）
;进货价,销售价，进货总数，已售数量，推荐度【=（进货价/实际销售价格+已售数量/进货数量）*128】。
_GA1             DB                'PEN', 6 DUP(0),'$' ,10    ;商品名称及折扣
DW                35,56,70,25,0          ; 推荐度还未计算
GA2             DB                'BOOK', 5 DUP(0) ,'$',9    ; 商品名称及折扣  一共21个字长度
DW                12,30,25,5,0           ;推荐度还未计算
GAN             DB                N-2 DUP( 'TempValue','$',8,15,0,20,0,30,0,2,0,0,0) ;除了2个已经具体定义了的商品信息以外，其他商品信息暂时假定为一样的。
;GA3             DB                'bag',7 DUP(0),9      ;商品名称为10个字节，折扣，进货价，销售价，进货总数，已售数量，推荐度
;DW                10,10,m,a,0            ;m表示是进货总数，a表示的是已售数量
BUF1            DB                0DH,0AH,'please input the name?','$'
BUF2            DB                0DH,0AH,'please input the password?','$'
_BUF3            DB                0DH,0AH,'please input what do you want to search?','$'
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
;首先显示一个功能菜单（格式自行定义。若是未登录状态，只显示菜单“1”和“6”）:
;1=查询商品信息，2=修改商品信息，3=计算推荐度，
;4=计算推荐度排名，5=输出全部商品信息，6=程序退出。
;     输入1-6的数字进入对应的功能。
menu1           DB  0DH,0AH,'              menu'
DB  0DH,0AH,'            1.check for goods information',9 DUP(0)  ;每行的长度为50个字节
DB  0DH,0AH,'            2.change the goods information',8 DUP(0)
DB  0DH,0AH,'            3.calculate the recommendation',8 DUP(0)
DB  0DH,0AH,'            4.calculate the recommendation rank',3 DUP(0)
DB  0DH,0AH,'            5.output all goods information',8 DUP(0)
DB  0DH,0AH,'            6.exit'
DB  0DH,0AH,'            please choose one of 1-6','$'
menu2           DB  0DH,0AH,'              menu'
DB  0DH,0AH,'            1.check for goods information',9 DUP(0)  ;每行的长度为50个字节
DB  0DH,0AH,'            6.exit'
DB  0DH,0AH,'             please choose one of 1 or 6','$'
BUF6            DB               0DH,0AH,'please input which goods that you want to change its information?','$'   
BUF7            DB               0DH,0AH,'input error','$'
change_goods    DB      10            ;存储修改商品信息的商品名
                DB      0
                DB      10 DUP(0)    
change_information_  DB       10           ;存储修改商品的信息
				   DB       0
				   DB       10 DUP(0)
DATA  ENDS

CODE           SEGMENT         USE16       ;问题1：为什么要命名为text，问题二：为什么不能加USE16（加了的话会显示段属性错误的重定义）
START:        
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
JE                 EXIT_THIS
JMP				   CONTINUE	
EXIT_THIS:
CALL               ENDCODE               ;程序退出
CONTINUE :	
CMP                IN_NAME[DI],0DH
JE                 ADD_AUTH                ;输入回车键，则将auth赋值为0
JUDGE : 
MOV                DI,2
CMP                IN_NAME[DI],0DH         ;判断是否输入的是回车键
JE                 INTOTHREE               ;进入功能三，跳过功能二
JMP                CONTINUE1
INTOTHREE:
CALL               THREE
CONTINUE1:
LEA                DX,BUF2                 ;提醒输入密码
MOV                AH,9
INT                21H
LEA                DX,IN_PWD               ;输入密码
MOV                AH,10
INT                21H
JMP                SECOND
ADD_AUTH:      
MOV                DI,0
MOV                AUTH[DI],0
JMP                JUDGE
SECOND	:       
MOV                SI,1
MOV                DI,-1
LOOPAFORNAME:   
LEA                BX,IN_NAME
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
LOOPAFORPASSWORD:
MOV                SI,1                   ;开始判断密码是否输入正确
MOV                DI,-1
LOOPA:          
LEA                BX,IN_PWD
MOV                CL,4
CMP                IN_PWD+1,4
JNE                START	
INC                SI
INC                DI
CMP                BPASS[DI],'t'          ;当到达't'时，跳出循环,即密码正确
JE                 ADD_AUTH_ONE           ;auth赋值 1 ，并且跳到功能3
MOV                CL,[BX][SI]
CMP                BPASS[DI],CL
JE                 LOOPA
JNE                START                  ;失败返回功能1
ADD_AUTH_ONE : 	
XOR                DI,DI
MOV                AUTH[DI],1   
CALL               THREE      

THREE PROC
cmp              change_goods[10],1
je               disp2
MOV              DI,0
CMP              AUTH[DI],1
JE               showmenu1
JNE              showmenu2
showmenu1:
LEA             DX,menu1
mov             AH,9
int             21H
JMP             NEXT_THREE1
showmenu2:
LEA             DX,menu2
mov             AH,9
int             21H
NEXT_THREE1:
mov                dl,0Ah
mov                ah,2
int                21H 
MOV                AH,1    ;从键盘中读入字符，并保存在AL中
INT                21H
CMP             AL,'1'
JE              disp1
CMP             AL,'2'
JE              disp2
CMP             AL,'3'
JE              disp3
CMP             AL,'4'
JE              disp4
CMP             AL,'5'
JE              disp5
CMP             AL,'6'
JE              disp6
disp1:
CALL  _THREE1   
jmp   THREE       
disp2:
CALL  _THREE2   
jmp   THREE 
disp3:
CALL            _Recommend
JMP             THREE
disp4:
CALL            _RANK
JMP             THREE
disp5:
CALL            _INFORMATIONS
JMP             THREE
disp6:
CALL            ENDCODE
RET
THREE ENDP
_THREE1  PROC     near  
pusha
push ds
mov si,DATA
MOV DS,SI
qqq:
LEA                DX,_BUF3    ;提醒输入商品名称
MOV                AH,9
INT                21H
LEA                DX,IN_GOODS          ;输入商品名
MOV                AH,10
INT                21H
LEA                BX,_GA1                ;将GA1的基址作为子程序的参数传入
MOV                AX,N                  ;AX作为商品数量
LOOPA_number:
MOV                  SI,1
MOV                  DI,-1
DEC                  AX
LOOPA_GOODS:  
INC                  SI
INC                  DI
CMP                  BYTE PTR [BX+DI],0           ;商品名输入正确     
JE                   intoshow_information         ;“商品名称，折扣，销售价，进货总数，已售数量，推荐度”顺序显示该商品的信息。
MOV                  CL,IN_GOODS[SI]
CMP                  IN_GOODS[2],0DH                ;输入回车键，退出子程序
JE                   NEXT3
CMP                  [BX][DI],CL
JE                   LOOPA_GOODS
ADD                  BX,21                        ;商品名输入不正确，检查下一个商品名
CMP                  AX,0
JE                   qqq                          ;未找到商品，重新输入
JNE                  LOOPA_number                 ;还有商品，继续对比下一个商品	
intoshow_information:
mov              dl,0Ah
mov              ah,2
int              21h
call            show_information
NEXT3:
POP DS
popa
RETf
_THREE1 ENDP

;入口参数bx---商品的地址
;作用：展示商品的信息
;返回参数无
show_information PROC
pusha
mov              DI,-1
loopa1:
inc              DI
cmp              BYTE PTR[BX+DI],0    ;表示商品名称输出完毕
je               next4
mov              al,[BX][DI]   
MOV              DL,AL              ;输出字符
MOV              AH,2
INT              21H
jmp              loopa1
next4:                  ;输出后面的数字
mov              dl,00h
mov              ah,2
int              21h
mov              dl,[bx][10];输出折扣
xor              Dh,Dh              
call             change_into_ascii
mov              dl,00h
mov              ah,2
int              21h
mov              si,13             ;不输出进货价
loopa2:
cmp              si,21
je               w1;退出
mov              dx, word PTR[bx][si]
call             change_into_ascii
mov              dl,00h
mov              ah,2
int              21h
add              si,2
jmp              loopa2
w1:
popa
RET                                              ;在子程序中调用其他的子程序会不会跳过ret？
show_information ENDP
_THREE2 PROC  NEAR
MOV                change_goods[10],0
pusha
push               ds
MOV                SI, DATA
MOV                DS,SI
LEA                DX,BUF6                  ;提醒输入需要修改信息的商品名称
MOV                AH,9
INT                21H
LEA                DX,change_goods          ;输入商品名
MOV                AH,10
INT                21H
MOV                DI,2
CMP                change_goods[DI],0DH         ;判断是否输入的是回车键4
JE                 W3                           ;退出
LEA                BX,_GA1                       ;将GA1的基址作为子程序的参数传入
MOV                AX,N                         ;AX作为商品数量
LOOPA_number1:
MOV                  SI,1
MOV                  DI,-1
DEC                  AX
LOOPA_GOODS1:  
INC                  SI
INC                  DI
CMP                  BYTE PTR [BX+DI],0           ;商品名输入正确     
JE                   intoshow_information1         ;“商品名称，折扣，进货价，销售价，进货总数”顺序显示该商品的信息。
MOV                  CL,change_goods[SI]
CMP                  [BX][DI],CL
JE                   LOOPA_GOODS1
ADD                  BX,21                        ;商品名输入不正确，检查下一个商品名
CMP                  AX,0
JE                   NEXT31                        ;未找到商品，返回功能三（2）
JNE                  LOOPA_number1                 ;还有商品，继续对比下一个商品	
NEXT31:
MOV                  change_goods[10],1           ;表示商品名称输入错误
jmp                  W3
intoshow_information1:
mov              dl,0Ah
mov              ah,2
int              21h
call            show_information1
CALL           T2_change_information1 ;修改商品信息
W3:
pop        ds
popa
RETf
_THREE2 ENDP  
;入口参数bx---商品的地址
;作用：展示商品的信息---折扣、进货价、销售价、进货总数
;返回参数无
show_information1 PROC  
pusha
mov              DI,-1
loopa11:
inc              DI
cmp              BYTE PTR[BX+DI],0    ;表示商品名称输出完毕
je               next41
mov              al,[BX][DI]   
MOV              DL,AL              ;输出字符
MOV              AH,2
INT              21H
jmp              loopa11
next41:                  ;输出后面的数字
mov              dl,00h
mov              ah,2
int              21h
mov              dl,[bx][10];输出折扣
XOR              DH,DH
call             change_into_ascii
mov              dl,00h
mov              ah,2
int              21h
mov              si,11             ;不输出进货价
loopa21:
cmp              si,17
je               w11;退出
mov              dx, word PTR[bx][si]
call             change_into_ascii
mov              dl,00h
mov              ah,2
int              21h
add              si,2
jmp              loopa21
w11:
popa
RET                                              ;在子程序中调用其他的子程序会不会跳过ret？
show_information1 ENDP
;功能：修改商品的信息
;入口参数bx---商品的地址
;
T2_change_information1 PROC 
pusha
MOV                CL,6
loopa4:
mov                dl,0Ah
mov                ah,2
int                21H
DEC                CL
qq:
CMP                CL,1
JE                 W4                               ;全部的信息修改完了
cmp                CL,5
je                 q5        ;修改折扣
cmp                CL,4
je                 q4        ;修改进货价
cmp                CL,3
je                 q3        ;修销售价
cmp                CL,2
je                 q2        ;修改进货数量
ee3:
LEA                DX,change_information_             ;输入修改信息
mov                ah,10            
int                21H
CMP                change_information_[2],0DH         ;判断是否输入的是回车键
JE                 loopa4         
call               change_into_number
cmp                change_information_[9],1
je                 qq
jne                loopa4
q5:
mov              si,10
mov              dl,[BX][si]
XOR              DH,DH
call             change_into_ascii
mov              dl,2fh
MOV              AH,2
INT              21H
JMP              ee3
q4:
mov              si,11
mov              dx,WORD PTR[BX][si]
call             change_into_ascii
mov              dl,2fh
MOV              AH,2
INT              21H
JMP              ee3
q3:
mov              si,13
mov              dx,WORD PTR[BX][si]
call             change_into_ascii
mov              dl,2fh
MOV              AH,2
INT              21H
JMP              ee3
q2:
mov              si,15
mov              dx,WORD PTR[BX][si]
call             change_into_ascii
mov              dl,2fh
MOV              AH,2
INT              21H
JMP              ee3
W4:
popa
RET
T2_change_information1 ENDP
;功能：将ascii码值转化成二进制数值
;入口参数bx---商品的地址，si----商品信息地址
;出口参数：change_information[9]-----0表示输入无误，1表示输入有误
change_into_number PROC 
mov                change_information_[9],0
mov     di,si
pusha
xor     cx,cx                ;存储位数
xor     dx,dx                ;运算结果在dx
mov     si,1
mov     CL,change_information_[si]
loopa5:
cmp     CL,0
je      w2                     ;运算结束
INC     SI                   
cmp     change_information_[si],030h
jb      w12                           ;输入有误
cmp     change_information_[si],039h
ja      w12
mov     AL,change_information_[si]
push    cx
sub     AL,48
ll:
dec     CL
CMP     CL,0         ;当个位数时
JE      QQQQ
mov     ah,10
imul    ah
cmp     CL,1
jne     ll
QQQQ:
pop     cx
dec     CL
add     DX,AX
jmp     loopa5
w2:
cmp      di,10
je       add1
mov    WORD PTR [BX][DI],0000H          ;小问题：是低地址作为字的首地址吗？
mov    word ptr [bx][di],dx
jmp     e1
add1:
mov      [bx][10],dl
jmp      e1
w12:
LEA                DX,BUF7    ;提醒输入信息错误
MOV                AH,9
INT                21H
mov                dl,0AH
MOV                AH,2
INT                21H
mov                change_information_[9],1   ;作为输入有误的标志位
e1:
popa
RET
change_into_number ENDP
change_into_ascii PROC
pusha
mov ax,dx
xor dx,dx
mov bx,10
mov cx,0
a1:
cmp ax,10
jb ok
div bx
add dl,30h
push dx
xor dx,dx
inc cx
jmp a1
ok:
add al,30h
push ax
inc cx
b:
pop dx
mov ah,2
int 21h
loop b
popa
RET
change_into_ascii ENDP
ENDCODE PROC near
MOV                  AX, 4C00H
INT                  21H
RET
ENDCODE ENDP
;此部分由吴迎楠写
SUBDATA SEGMENT  USE16
_RECOMMEND_RANK DW N DUP(0)
TIPS1   DB   0AH,0DH,'Discount:','$'
TIPS2   DB   0AH,0DH,'Prime-Price:','$'
TIPS3   DB   '  Sale-Price:','$'
TIPS4   DB   0AH,0DH,'Total:','$'
TIPS5   DB   '  The number sold:','$'
TIPS6   DB   0AH,0DH,'Recommend Degree:','$'
TIPS7   DB   '  Recommend Degree Rank:','$'
TIPS8   DB   0AH,0DH,0AH,0DH,'$'
SUBDATA ENDS
;功能三：计算所有商品的推荐度
_RECOMMEND proc  near
			push ds
			pusha
			mov si,data
			mov ds,si
           MOV CX,N
		   LEA BX,_GA1		   
CALCULATE: MOV SI,10;此处为折扣所在的偏移地址！！！！！！！！！！！！！！！！
		   MOV BYTE PTR DL,DS:[BX+SI];DX中存的是折扣
		   MOV BYTE PTR DH,0
		   ADD SI,1
		   MOV WORD PTR AX,DS:[BX+SI];AX中存的是进货价
		   PUSH AX;暂存一下进货价
		   ADD SI,2
		   MOV WORD PTR AX,DS:[BX+SI];AX中存的是销售价
		   MUL DX
		   MOV DL,10
		   DIV DL
		   MOV DI,AX
		   POP AX
		   MOV BP,128
		   MUL BP
		   DIV DI;计算进货价*128/销售价
		   PUSH AX;保存商
		   ADD SI,4
		   MOV WORD PTR AX,DS:[BX+SI]
		   SUB SI,2
		   MOV WORD PTR DI,DS:[BX+SI]
		   MUL BP
		   DIV DI;计算已售数量*128/进货总数
		   POP BP
		   ADD AX,BP
		   ADD SI,4
		   MOV DS:[BX+SI],AX;保存推荐度
		   ADD BX,21
		   LOOP CALCULATE
		  popa
		  pop ds
		   retf
_RECOMMEND endp			   
		   ;功能四：计算推荐度排名		   
_RANK     proc  near
			push ds
		 	PUSHa
		    MOV SI,data
            mov ds,si
            mov si,0		   
		   LEA BP,_GA1
		   ADD BP,21;第一个商品的推荐度所在地址
           MOV CX,N;外层循环
OUTCMP:	   MOV AX,1;AX用来存放排名
		   MOV DI,DS:[BP];DI存放要获得排名的商品的推荐度
		   PUSH CX;保存外层cx
		   LEA BX,_GA1
		   ADD BX,21
		   MOV CX,N;设置比较循环次数，通过比较来获得排名
INCMP:	   CMP DI,DS:[BX];比较外层推荐度和内层推荐度的大小
		   JB  BELOW;
CONTINUE11:  ADD BX,21	   
           LOOP INCMP
           PUSH DS
		   MOV DX,SUBDATA
		   MOV DS,DX
		   MOV DS:[SI],AX
		   ADD SI,2
		   POP DS
		   POP CX
		   ADD BP,21;外层下一个商品
		   LOOP OUTCMP
		   popa
		   pop ds
		   RETF
BELOW:     INC AX 
           JMP CONTINUE11 
_RANK endp			   
           ;功能五：输出所有商品信息
_INFORMATIONS proc near
               PUSH DS
               pusha		   
		       mov si,DATA
		       MOV DS,SI
               LEA BP,_GA1
			   MOV CX,N
SHOW:		   PUSH CX
               LEA DX,[BP];]输出商品名称
               MOV AH,9
			   INT 21H
			   ADD BP,10;此处为折扣所在的偏移地址！！！！！！！！！！！！！！！！
               MOV AL,DS:[BP];al存折扣
               ADD BP,1
               MOV BX,DS:[BP];bx存进货价
               ADD BP,2
               MOV CX,DS:[BP];cx存销售价
			   ADD BP,2
               MOV DX,DS:[BP];dx存进货总数 		
               ADD BP,2
               MOV SI,DS:[BP];si存已售数量
               ADD BP,2
               MOV DI,DS:[BP];DI存推荐度			   
			   PUSH BP
               PUSH DS	
			   PUSH DX;DX进栈
               MOV	BP,SUBDATA
               MOV DS,BP
			   LEA DX,TIPS1;分别显示商品的所有信息
			   MOV AH,9
			   INT 21H
			   MOV DL,AL
			   MOV DH,0
			   CALL change_into_ascii
			   LEA DX,TIPS2
			   MOV AH,9
			   INT 21H
			   MOV DX,BX
			   CALL change_into_ascii
			   LEA DX,TIPS3
               MOV AH,9
			   INT 21H
			   MOV DX,CX
			   CALL change_into_ascii
			   LEA DX,TIPS4
               MOV AH,9
			   INT 21H
			   POP DX
			   CALL change_into_ascii
			   LEA DX,TIPS5
			   MOV AH,9
			   INT 21H
			   MOV DX,SI
			   CALL change_into_ascii
			   LEA DX,TIPS6
			   MOV AH,9
			   INT 21H
			   MOV DX,DI
			   CALL change_into_ascii
			   LEA DX,TIPS7
			   MOV AH,9
			   INT 21H
			   POP DI;DI暂存DS
			   POP BP;
			   POP CX;循环
			   MOV AX,CX
			   MOV BX,2
			   MUL BX
			   MOV BX,N*2
			   SUB BX,AX
			   MOV DX,DS:[BX]
			   CALL change_into_ascii
			   LEA DX,TIPS8
			   MOV AH,9
			   INT 21H
			   MOV DS,DI
			   ADD BP,2
			   SUB CX,1			   
			   CMP CX,0
			   JNE SHOW
		       popa
			   pop ds
		       RETf	   
_INFORMATIONS endp		   
			   			   			   			   
CODE ENDS
end start