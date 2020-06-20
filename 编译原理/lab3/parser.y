/*LR分析法*/
%error-verbose/*指示bison生成详细的错误消息*/
%locations/*记录行号*/
//此处是声明部分
%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "def.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
int yylex();
void yyerror(const char* fmt, ...);
void display(struct ASTNode *,int);
%}
//辅助定义部分,union将各种类型统一起来
/*Bison中默认将所有的语义值都定义为int类型，可以通过定义宏YYSTYPE来改变值的类型。
如果有多个值类型，则需要通过在Bison声明中使用%union列举出所有的类型。*/
%union {
	int    type_int;
	float  type_float;
	char   type_id[32];
	char type_char[3];
	struct ASTNode *ptr;
};
/*
%type定义非终结符的语义值类型 %type <union 的成员名>  非终结符
%type  <ptr> program ExtDefList 
这表示非终结符 ExtDefList 属性值的类型对应联合中成员 ptr 的类型，在本实验中对应一个树结点的指针
*/
//  %type 定义非终结符的语义值类型
/*%token<type_id>ID表示的是识别出来的一个标识符后，标识符的字符串串值保存在成员type_id中*/
%type  <ptr> program ExtDefList ExtDef  Specifier ExtDecList FuncDec CompSt VarList VarDec ParamDec Stmt StmList DefList Def DecList Dec Exp Args Arraylist CaseType ForDec
//% token 定义终结符的语义值类型
%token <type_int> INT              /*指定INT的语义值是type_int，有词法分析得到的数值*/
%token <type_id> ID  RELOP TYPE    /*指定ID,RELOP 的语义值是type_id，有词法分析得到的标识符字符串*/
%token <type_char> CHAR
%token <type_float> FLOAT          /*指定ID的语义值是type_id，有词法分析得到的标识符字符串*/

%token DPLUS LP RP LC LB RB RC SEMI COMMA       /*用bison对该文件编译时，带参数-d，生成的.tab.h中给这些单词进行编码，可在lex.l中包含parser.tab.h使用这些单词种类码*/
//=assignop
%token PLUS MINUS STAR DIV MOD ASSIGNOP AND OR NOT IF ELSE WHILE RETURN FOR SWITCH CASE COLON DEFAULT COMADD COMSUB COMSTAR COMDIV COMMOD DOUBLEADD DOUBLESUB   
%token BREAK CONTINUE 
/*以下为接在上述token后依次编码的枚举常量，作为AST结点类型标记*/
%token EXT_DEF_LIST EXT_VAR_DEF FUNC_DEF FUNC_DEC EXT_DEC_LIST PARAM_LIST PARAM_DEC VAR_DEF DEC_LIST DEF_LIST COMP_STM STM_LIST EXP_STMT IF_THEN IF_THEN_ELSE ARRAY_LIST ARRAY_ID ARRAY_LAST BLANK    FOR_DEC
%token FUNC_CALL ARGS FUNCTION PARAM ARG CALL LABEL GOTO JLT JLE JGT JGE EQ NEQ 
%token SWITCH_STMT CASE_STMT DEFAULT_STMT EXP_ARRAY ARRAY_DEC DOUBLEADD_R DOUBLEADD_L DOUBLESUB_L DOUBLESUB_R

//这里是优先级定义
//left表示左结合，right表示右结合，前面符号的优先级低，后面的优先级高。
%left COMADD COMSUB COMSTAR COMDIV COMMOD //复合运算
%left ASSIGNOP 
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV MOD
%left DOUBLEADD DOUBLESUB
//优先级高的单目-符号UMINUS
%right UMINUS  NOT DPLUS
%right LB
%left RB
/*%nonassoc的含义是没有结合性,它一般与%prec结合使用表示该操作有同样的优先级*/
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

%%
//规则部分
/*
$$是一个bison定义的特殊标记，其意义是当前语法栈的规约元素。如果没有规约动作代码，缺省情况下赋予$$为NULL
$1表示这个产生式的第1个语法成分所在的语法栈中对应的值
 */
/*
Exp:  Exp ASSIGNOP Exp {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno); }  Exp →Exp =Exp
规则后面{}中的是当完成归约时要执行的语义动作。规则左部的 Exp 的属性值用$$表示，右部有 2 个 Exp，位置序号分别是 1 和 3，其属性值分别用$1 和$3表示
$$是子树的根结点指针，$1表示的是左值表达式的子树,$3表示的是右值表达式的子树，$2表示的是=号
yylineno表示赋值语句的行号
Stmt(statement) → error SEMI 表示对语句分析时，一旦有错，跳过分号（SEMI），继续进行语法分析
*/

program: ExtDefList    {display($1,0);semantic_Analysis0($1);}     //显示语法树,语义分析
         ; 
//作用：定义整个语法树
/*ExtDefList：外部定义列表，即是整个语法树*/
ExtDefList: {$$=NULL;}//语法树为空
          | ExtDef ExtDefList {$$=mknode(2,EXT_DEF_LIST,yylineno,$1,$2);}   //每一个EXTDEFLIST的结点，其第1棵子树对应一个外部变量声明或函数
          ;  
//作用：外部变量或函数声明
//例如：int a;
ExtDef:   Specifier ExtDecList SEMI   {$$=mknode(2,EXT_VAR_DEF,yylineno,$1,$2);}   //该结点对应一个外部变量声明
         |Specifier FuncDec CompSt    {$$=mknode(3,FUNC_DEF,yylineno,$1,$2,$3);}         //该结点对应一个函数定义
         | error SEMI   {$$=NULL;printf("missing SML（分号）\n");}
         ;
//作用：Specifier表示的是一个类型:int、float、char
Specifier:  TYPE    {$$=mknode(0,TYPE,yylineno);strcpy($$->type_id,$1);$$->type=!strcmp($1,"int")?INT:(!strcmp($1,"float")?FLOAT:CHAR);}   
           ; 
//变量名列表，由一个或多个变量组成，多个变量之间用逗号隔开    
ExtDecList:  VarDec      {$$=mknode(1,EXT_DEC_LIST,yylineno,$1);}       /*每一个EXT_DECLIST的结点，其第一棵子树对应一个变量名(ID类型(int、char、float)的结点),第二棵子树对应剩下的外部变量名*/
           | VarDec COMMA ExtDecList {$$=mknode(2,EXT_DEC_LIST,yylineno,$1,$3);}
           ;  
//变量名称，由一个ID组成
VarDec:  ID  {$$=mknode(0,ID,yylineno);strcpy($$->type_id,$1);}   //ID结点，标识符符号串存放结点的type_id
	      |ID Arraylist {$$=mknode(1,ARRAY_DEC,yylineno,$2);strcpy($$->type_id,$1);}
	     ;
//数组定义
Arraylist:LB Exp RB {$$=mknode(1,ARRAY_LAST,yylineno,$2);} 
      	|LB Exp RB Arraylist {$$=mknode(2,ARRAY_LIST,yylineno,$2,$4);}
	      |error RB {$$=NULL;printf("define array wrongly!\n");}
	       ; 
//作用：函数定义:函数名+参数定义
FuncDec: ID LP VarList RP   {$$=mknode(1,FUNC_DEC,yylineno,$3);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
		    |ID LP  RP   {$$=mknode(0,FUNC_DEC,yylineno);strcpy($$->type_id,$1);$$->ptr[0]=NULL;}//函数名存放在$$->type_id
		    |error RP {$$=NULL;printf("definr function wrong!\n");}
        ;  
//参数列表：有一个或者多个参数定义组成，用逗号隔开
VarList: ParamDec  {$$=mknode(1,PARAM_LIST,yylineno,$1);}
        | ParamDec COMMA  VarList  {$$=mknode(2,PARAM_LIST,yylineno,$1,$3);}
        ;
//参数的定义：固定有一个类型和一个变量组成
ParamDec: Specifier VarDec         {$$=mknode(2,PARAM_DEC,yylineno,$1,$2);}
         ;
//复合语句：左右分别用大括号括起来，中间有定义列表和语句列表
CompSt: LC DefList StmList RC    {$$=mknode(2,COMP_STM,yylineno,$2,$3);}
        ;
//语句列表：多个stmt语句构成
StmList: {$$=NULL; }  
        | Stmt StmList  {$$=mknode(2,STM_LIST,yylineno,$1,$2);}
        ;
//表示对语句分析时，有错误就会跳过分号，继续进行下去
/*语句，可能为表达式，复合语句，return语句，if语句，if-else语句，while语句，break、continue,空语句*/
Stmt:   Exp SEMI    {$$=mknode(1,EXP_STMT,yylineno,$1);}
      | CompSt      {$$=$1;}      //复合语句结点直接最为语句结点，不再生成新的结点
      | RETURN Exp SEMI   {$$=mknode(1,RETURN,yylineno,$2);}
      | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {$$=mknode(2,IF_THEN,yylineno,$3,$5);}
      | IF LP Exp RP Stmt ELSE Stmt   {$$=mknode(3,IF_THEN_ELSE,yylineno,$3,$5,$7);}
      | WHILE LP Exp RP Stmt {$$=mknode(2,WHILE,yylineno,$3,$5);}
      | FOR LP ForDec RP Stmt {$$=mknode(2,FOR,yylineno,$3,$5);}
      | BREAK SEMI		 {$$=mknode(0,BREAK,yylineno);}
      | CONTINUE SEMI   {$$=mknode(0,CONTINUE,yylineno);}
      | SWITCH LP Exp RP Stmt {$$=mknode(2,SWITCH_STMT,yylineno,$3,$5);}
      | CASE CaseType COLON StmList {$$=mknode(3, CASE_STMT, yylineno, $2, $4);}
      | DEFAULT COLON Stmt {$$=mknode(1, DEFAULT_STMT, yylineno,$3);} 
      ;
CaseType: INT {$$=mknode(0,INT,yylineno);$$->type_int=$1;$$->type=INT;}
        | CHAR {$$=mknode(0,CHAR,yylineno);strcpy($$->type_char, $1);$$->type=CHAR;}
        ;
ForDec: Exp SEMI Exp SEMI Exp {$$=mknode(3, FOR_DEC, yylineno, $1, $3, $5);}
        ;
//作用:定义列表，由0个或者多个定义语句组成
DefList: {$$=NULL; }
        | Def DefList {$$=mknode(2,DEF_LIST,yylineno,$1,$2);}
        | error SEMI   {$$=NULL;}
        ;
//定义一个或者多个语句语句，分号隔开
//例如：int a=1,b=1;
Def:    Specifier DecList SEMI {$$=mknode(2,VAR_DEF,yylineno,$1,$2);}
        ;
//语句列表，由一个或者多个语句组成，逗号隔开，最终都成一个表达式
DecList: Dec  {$$=mknode(1,DEC_LIST,yylineno,$1);}
       | Dec COMMA DecList  {$$=mknode(2,DEC_LIST,yylineno,$1,$3);}
	     ;
//语句：一个变量名称或者是一个赋值语句
Dec:     VarDec  {$$=$1;}
       | VarDec ASSIGNOP Exp  {$$=mknode(2,ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"ASSIGNOP");}
       ;
Exp:    Exp ASSIGNOP Exp {$$=mknode(2,ASSIGNOP,yylineno,$1,$3);strcpy($$->type_id,"ASSIGNOP");}//$$结点type_id空置未用，正好存放运算符
      | Exp COMADD Exp {$$=mknode(2,COMADD,yylineno,$1,$3);strcpy($$->type_id,"COMADD");}
      | Exp COMSUB Exp {$$=mknode(2,COMSUB,yylineno,$1,$3);strcpy($$->type_id,"COMSUB");}      
      | Exp COMSTAR Exp {$$=mknode(2,COMSTAR,yylineno,$1,$3);strcpy($$->type_id,"COMSTAR");}      
      | Exp COMDIV Exp {$$=mknode(2,COMDIV,yylineno,$1,$3);strcpy($$->type_id,"COMDIV");}     
      | Exp COMMOD Exp {$$=mknode(2,COMMOD,yylineno,$1,$3);strcpy($$->type_id,"COMMOD");}
      | Exp AND Exp   {$$=mknode(2,AND,yylineno,$1,$3);strcpy($$->type_id,"AND");}
      | Exp OR Exp    {$$=mknode(2,OR,yylineno,$1,$3);strcpy($$->type_id,"OR");}
      | Exp RELOP Exp {$$=mknode(2,RELOP,yylineno,$1,$3);strcpy($$->type_id,$2);}  //词法分析关系运算符号自身值保存在$2中
      | Exp PLUS Exp  {$$=mknode(2,PLUS,yylineno,$1,$3);strcpy($$->type_id,"PLUS");}
      | Exp MINUS Exp {$$=mknode(2,MINUS,yylineno,$1,$3);strcpy($$->type_id,"MINUS");}
      | Exp STAR Exp  {$$=mknode(2,STAR,yylineno,$1,$3);strcpy($$->type_id,"STAR");}
      | Exp DIV Exp   {$$=mknode(2,DIV,yylineno,$1,$3);strcpy($$->type_id,"DIV");}
      | Exp MOD Exp   {$$=mknode(2,MOD,yylineno,$1,$3);strcpy($$->type_id,"MOD");}
      | Exp DOUBLEADD  {$$=mknode(1,DOUBLEADD_R,yylineno,$1);strcpy($$->type_id,"DOUBLEADD");}
      | Exp DOUBLESUB {$$=mknode(1,DOUBLESUB_R,yylineno,$1);strcpy($$->type_id,"DOUBLESUB");}
      | DOUBLEADD Exp  {$$=mknode(1,DOUBLEADD_L,yylineno,$2);strcpy($$->type_id,"DOUBLEADD");}
      | DOUBLESUB Exp {$$=mknode(1,DOUBLESUB_L,yylineno,$2);strcpy($$->type_id,"DOUBLESUB");}
      | LP Exp RP     {$$=$2;}
      | MINUS Exp %prec UMINUS   {$$=mknode(1,UMINUS,yylineno,$2);strcpy($$->type_id,"UMINUS");}
      | NOT Exp       {$$=mknode(1,NOT,yylineno,$2);strcpy($$->type_id,"NOT");}
      | ID LP Args RP {$$=mknode(1,FUNC_CALL,yylineno,$3);strcpy($$->type_id,$1);}
      | ID LP RP      {$$=mknode(0,FUNC_CALL,yylineno);strcpy($$->type_id,$1);}
      | ID            {$$=mknode(0,ID,yylineno);strcpy($$->type_id,$1);}
      | INT           {$$=mknode(0,INT,yylineno);$$->type_int=$1;$$->type=INT;}
      | CHAR          {$$=mknode(0,CHAR,yylineno);strcpy($$->type_char, $1);$$->type=CHAR;}
      | FLOAT         {$$=mknode(0,FLOAT,yylineno);$$->type_float=$1;$$->type=FLOAT;}
      | ID Arraylist  {$$=mknode(1,EXP_ARRAY,yylineno,$2);strcpy($$->type_id,$1);}
      | {$$=NULL;}
      ;

//用逗号隔开参数
Args:    Exp COMMA Args    {$$=mknode(2,ARGS,yylineno,$1,$3);}
       | Exp               {$$=mknode(1,ARGS,yylineno,$1);}
       ;
       
%%

int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");
	if (!yyin) return -1;
	yylineno=1;
	yyparse();
	return 0;
	}


void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}

