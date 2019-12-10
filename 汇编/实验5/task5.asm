  .386
.model   flat,stdcall
option   casemap:none

WinMain  proto :DWORD,:DWORD,:DWORD,:DWORD
WndProc  proto :DWORD,:DWORD,:DWORD,:DWORD
Display  proto :DWORD
Convert	 proto :BYTE,:DWORD
Calculate proto 

include      menuID.INC

include      windows.inc
include      user32.inc  ;MessageBoxA所在头文件
include      kernel32.inc ;ExitProcess所在头文件:退出程序返回操作系统的函数uexitcode为退出码，正常退出为0
include      gdi32.inc   
include      shell32.inc 

includelib   user32.lib;MessageBoxA所在的引入库
includelib   kernel32.lib;exitprocess所在的引入库
includelib   gdi32.lib
includelib   shell32.lib
shop	     struct
	    myname          db  10 dup(0)
	    discount  		db  0
	    p_price  db  0
	    s_price      db  0
	    p_amount     db  0
	    s_amount     db  0
		recommendation	db  0
shop      ends

.data
ClassName    db       'Class',0
AppName      db       'Shop Menu',0
MenuName     db       'MyMenu',0
DlgName	     db       'MyDialog',0
AboutMsg     db       'I am cs1703 raohongbin',0
CMsg         db       'The Calculation was finished!', 0
hInstance    dd       0
CommandLine  dd       0
buf	     shop  <>
	     shop  <'RULER1',10,35,46,70,25,0>
		 shop  <'RULER2',9,12,20,25,5,0>
		 shop  <'RULER3',4,16,60,90,2,0>
		 shop  <'RULER4',5,11,30,60,40,0>
		 shop  <'PENCIL',8,20,50,95,10,0>
	     shop  3 dup(<>)
msg_List             db		  'List', 0
msg_name             db       'Name    ', 0
msg_discount         db       'Discount',0
msg_p_price   db       'Purchase price',0
msg_s_price	     db       'Sale price',0
msg_p_amount  db       'Purchase amount',0
msg_s_amount      db       'Sale amount',0
msg_recommendation   db       'Recommendation',0

str_price    db       "00H", 20H, 20H, 0
str_len		 dw       0
menuItem     db       0  

.code
Start:	     invoke GetModuleHandle,NULL
	     mov    hInstance,eax
	     invoke GetCommandLine
	     mov    CommandLine,eax
	     invoke WinMain,hInstance,NULL,CommandLine,SW_SHOWDEFAULT
	     invoke ExitProcess,eax
	     ;;
WinMain      proc   hInst:DWORD,hPrevInst:DWORD,CmdLine:DWORD,CmdShow:DWORD
	     LOCAL  wc:WNDCLASSEX
	     LOCAL  msg:MSG
	     LOCAL  hWnd:HWND
             invoke RtlZeroMemory,addr wc,sizeof wc
	     mov    wc.cbSize,SIZEOF WNDCLASSEX
	     mov    wc.style, CS_HREDRAW or CS_VREDRAW
	     mov    wc.lpfnWndProc, offset WndProc
	     mov    wc.cbClsExtra,NULL
	     mov    wc.cbWndExtra,NULL
	     push   hInst
	     pop    wc.hInstance
	     mov    wc.hbrBackground,COLOR_WINDOW+1
	     mov    wc.lpszMenuName, offset MenuName
	     mov    wc.lpszClassName,offset ClassName
	     invoke LoadIcon,NULL,IDI_APPLICATION
	     mov    wc.hIcon,eax
	     mov    wc.hIconSm,0
	     invoke LoadCursor,NULL,IDC_ARROW
	     mov    wc.hCursor,eax
	     invoke RegisterClassEx, addr wc
	     INVOKE CreateWindowEx,NULL,addr ClassName,addr AppName,\
                    WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,\
                    CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,\
                    hInst,NULL
	     mov    hWnd,eax
	     INVOKE ShowWindow,hWnd,SW_SHOWNORMAL
	     INVOKE UpdateWindow,hWnd
	     ;;
MsgLoop:     INVOKE GetMessage,addr msg,NULL,0,0
             cmp    EAX,0
             je     ExitLoop
             INVOKE TranslateMessage,addr msg
             INVOKE DispatchMessage,addr msg
	     jmp    MsgLoop 
ExitLoop:    mov    eax,msg.wParam
	     ret
WinMain      endp

WndProc      proc   hWnd:DWORD,uMsg:DWORD,wParam:DWORD,lParam:DWORD
	     LOCAL  hdc:HDC
	     LOCAL  ps:PAINTSTRUCT
     .IF     uMsg == WM_DESTROY
	     invoke PostQuitMessage,NULL
     .ELSEIF uMsg == WM_KEYDOWN
	    .IF     wParam == VK_F1
             ;;your code
	    .ENDIF
     .ELSEIF uMsg == WM_COMMAND
	    .IF     wParam == IDM_FILE_EXIT
		    invoke SendMessage,hWnd,WM_CLOSE,0,0
	    ;  .ELSEIF wParam == IDM_FILE_CLEAR
		;    mov menuItem, 0
		;   invoke InvalidateRect,hWnd,0,1  ;;擦除整个客户区
		;  invoke UpdateWindow, hWnd
		.ELSEIF wParam == 10012
		    mov menuItem, 1
		    invoke InvalidateRect,hWnd,0,1  ;
		    invoke UpdateWindow, hWnd
		.ELSEIF wParam == 10011
		    mov menuItem, 2
		    invoke InvalidateRect,hWnd,0,1  ;
		    invoke UpdateWindow, hWnd
	    .ELSEIF wParam == IDM_HELP_ABOUT
		    invoke MessageBox,hWnd,addr AboutMsg,addr AppName,0
	    .ENDIF
     .ELSEIF uMsg == WM_PAINT
             invoke BeginPaint,hWnd, addr ps
             mov hdc,eax
	     .IF menuItem == 1
		 invoke Display,hdc ;选择1调用display
		 .ELSEIF menuItem == 2
		   ;选择2调用calculate
		   invoke Calculate
		 invoke MessageBox,hWnd,addr CMsg,addr AppName,0
	     .ENDIF
	     invoke EndPaint,hWnd,addr ps
     .ELSE
             invoke DefWindowProc,hWnd,uMsg,wParam,lParam
             ret
     .ENDIF
  	     xor    eax,eax
	     ret
WndProc      endp

Calculate	 proc	
			 mov si,16	
		cal:
			 movzx 	eax, buf[si].s_price
			 movzx  ebx, buf[si].discount
			 imul   ax, bx
			 mov 	bx, ax
			 xor    dx, dx
			 movzx	eax, buf[si].p_price
			 imul	ax, 1280
			 idiv	bx
			 mov 	bx, ax
			 xor    dx, dx
			 movzx	eax, buf[si].s_amount
			 imul	ax, 128
			 idiv	buf[si].p_amount
			 add 	bx, ax
			 mov	buf[si].recommendation, bl
			 cmp si,80
			 jz over
			 add si,16
			 jmp cal

		over:
			 ret
Calculate    endp


Display      proc   hdc:HDC
             XX     equ  15
             YY     equ  10
	     	 XX_GAP equ  125
	     	 YY_GAP equ  30
             invoke TextOut,hdc,XX+0*XX_GAP,YY+0*YY_GAP,offset msg_name,9
             invoke TextOut,hdc,XX+1*XX_GAP,YY+0*YY_GAP,offset msg_discount,8
             invoke TextOut,hdc,XX+2*XX_GAP,YY+0*YY_GAP,offset msg_p_price,14
             invoke TextOut,hdc,XX+3*XX_GAP,YY+0*YY_GAP,offset msg_s_price,10
             invoke TextOut,hdc,XX+4*XX_GAP,YY+0*YY_GAP,offset msg_p_amount,15
             invoke TextOut,hdc,XX+5*XX_GAP,YY+0*YY_GAP,offset msg_s_amount,11
			 invoke TextOut,hdc,XX+6*XX_GAP,YY+0*YY_GAP,offset msg_recommendation,14
             ;;
             invoke TextOut,hdc,XX+0*XX_GAP,YY+1*YY_GAP,offset buf[1*16].myname, 6
             invoke Convert,buf[1*16].discount ,addr str_price
			 invoke TextOut,hdc,XX+1*XX_GAP,YY+1*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[1*16].p_price ,addr str_price
             invoke TextOut,hdc,XX+2*XX_GAP,YY+1*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[1*16].s_price ,addr str_price
             invoke TextOut,hdc,XX+3*XX_GAP,YY+1*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[1*16].p_amount ,addr str_price
             invoke TextOut,hdc,XX+4*XX_GAP,YY+1*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[1*16].s_amount ,addr str_price
			 invoke TextOut,hdc,XX+5*XX_GAP,YY+1*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[1*16].recommendation ,addr str_price
             invoke TextOut,hdc,XX+6*XX_GAP,YY+1*YY_GAP,offset str_price, str_len

			 invoke TextOut,hdc,XX+0*XX_GAP,YY+2*YY_GAP,offset buf[2*16].myname, 6
             invoke Convert,buf[2*16].discount ,addr str_price
			 invoke TextOut,hdc,XX+1*XX_GAP,YY+2*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[2*16].p_price ,addr str_price
             invoke TextOut,hdc,XX+2*XX_GAP,YY+2*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[2*16].s_price ,addr str_price
             invoke TextOut,hdc,XX+3*XX_GAP,YY+2*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[2*16].p_amount ,addr str_price
             invoke TextOut,hdc,XX+4*XX_GAP,YY+2*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[2*16].s_amount ,addr str_price
			 invoke TextOut,hdc,XX+5*XX_GAP,YY+2*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[2*16].recommendation ,addr str_price
             invoke TextOut,hdc,XX+6*XX_GAP,YY+2*YY_GAP,offset str_price, str_len

			 invoke TextOut,hdc,XX+0*XX_GAP,YY+3*YY_GAP,offset buf[3*16].myname, 6
             invoke Convert,buf[3*16].discount ,addr str_price
			 invoke TextOut,hdc,XX+1*XX_GAP,YY+3*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[3*16].p_price ,addr str_price
             invoke TextOut,hdc,XX+2*XX_GAP,YY+3*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[3*16].s_price ,addr str_price
             invoke TextOut,hdc,XX+3*XX_GAP,YY+3*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[3*16].p_amount ,addr str_price
             invoke TextOut,hdc,XX+4*XX_GAP,YY+3*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[3*16].s_amount ,addr str_price
			 invoke TextOut,hdc,XX+5*XX_GAP,YY+3*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[3*16].recommendation ,addr str_price
             invoke TextOut,hdc,XX+6*XX_GAP,YY+3*YY_GAP,offset str_price, str_len

             invoke TextOut,hdc,XX+0*XX_GAP,YY+4*YY_GAP,offset buf[4*16].myname, 6
             invoke Convert,buf[4*16].discount ,addr str_price
			 invoke TextOut,hdc,XX+1*XX_GAP,YY+4*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[4*16].p_price ,addr str_price
             invoke TextOut,hdc,XX+2*XX_GAP,YY+4*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[4*16].s_price ,addr str_price
             invoke TextOut,hdc,XX+3*XX_GAP,YY+4*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[4*16].p_amount ,addr str_price
             invoke TextOut,hdc,XX+4*XX_GAP,YY+4*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[4*16].s_amount ,addr str_price
			 invoke TextOut,hdc,XX+5*XX_GAP,YY+4*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[4*16].recommendation ,addr str_price
             invoke TextOut,hdc,XX+6*XX_GAP,YY+4*YY_GAP,offset str_price, str_len

             invoke TextOut,hdc,XX+0*XX_GAP,YY+5*YY_GAP,offset buf[5*16].myname, 6
             invoke Convert,buf[5*16].discount ,addr str_price
			 invoke TextOut,hdc,XX+1*XX_GAP,YY+5*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[5*16].p_price ,addr str_price
             invoke TextOut,hdc,XX+2*XX_GAP,YY+5*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[5*16].s_price ,addr str_price
             invoke TextOut,hdc,XX+3*XX_GAP,YY+5*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[5*16].p_amount ,addr str_price
             invoke TextOut,hdc,XX+4*XX_GAP,YY+5*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[5*16].s_amount ,addr str_price
			 invoke TextOut,hdc,XX+5*XX_GAP,YY+5*YY_GAP,offset str_price, str_len
			 invoke Convert,buf[5*16].recommendation ,addr str_price
             invoke TextOut,hdc,XX+6*XX_GAP,YY+5*YY_GAP,offset str_price, str_len
             
             ret
Display      endp

Convert	 proc   price_char:BYTE, price_str:DWORD
			 mov 	ebx, 10
			 movzx	eax, price_char
			 mov 	esi, price_str
			 xor    cx, cx
LOP1:		 xor 	edx, edx
			 div 	ebx
			 push	dx
			 inc 	cx
			 or 	eax, eax
			 mov	str_len, cx
			 jnz    LOP1
LOP2:		 pop 	ax
			 cmp	al, 10
			 jb 	L1
			 add 	al, 7
L1: 		 add    al, 30H
			 mov 	[esi], al
			 inc    esi
			 dec    cx
			 cmp	cx, 0
			 jne    LOP2
			 ret
Convert	 endp

             end  Start
