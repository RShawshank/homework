%{
/*Flex 读入给定的输入文件，如果没有给定文件名的话，则从标准输入读取，*/
/*从而获得一个关于需要生成的扫描器的描述。*/
/*此描述叫做 规则，由正则表达式和 C代码对组成。*/
/*Flex 的输出是一个 C 代码文件——lex.yy.c——其中定义了yylex() 函数。*/
/*编译输出文件并且和 -lfl 库链接生成一个可执行文件。*/
/*当运行可执行文件的时候，它分析输入文件，为每一个正则表达式寻找匹配。*/
/*当发现一个匹配时，它执行与此正则表达式相关的C代码。*/
#include "parser.tab.h"
#include "string.h"
#include "def.h"
int yycolumn=1;
#define YY_USER_ACTION    	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;	yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
typedef union {
	char type_char[3];
	int type_int;
	float type_float;
	char type_id[32];
	struct node *ptr;
} YYLVAL;
#define YYSTYPE YYLVAL

%}
%option yylineno
%x COMMENT
/*通用字符名*/
UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
/*识别16进制等*/
id   [A-Za-z][A-Za-z0-9]*  
errorid [0-9][0-9]*[A-Za-z]*
int    ([+-]?[0-9]+)|(0[xX][0-9a-fA-F]+)|(0[0-7]+)
float  ([0-9]*\.[0-9]+)|([0-9]+\.)
CHAR \'([^'\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9A-Fa-f]+|{UCN})+\'
/*转换规则：通过在声明部分的正则变量，识别输入流中的单词，并可以执行一些动作*/
/*词法分析器识别出一个单词后，将该单词对应的字符串保存在 yytext中，其长度为yyleng*/
/* 通过yylval.type_int=strtol(yytext,NULL,0);将整常数的值保存在yylval的成员type_in中，这里 yylval 是一个 Flex 和 Bison 共用的内部变量，类型为 YYLVAL，按这样的方式，在 Flex 中通过 yylval 的成员保存单词属性值，在 Bison 中就可以通过yylval 的成员取出属性值，实现了数据的传递*/
/*规则=模式+动作：分析器识别相应模式（在定义中定义的正则表达式）后对相应模式进行处理的C语言动作*/
%%
{int}        {yylval.type_int=strtol(yytext,NULL,0); return INT;}
{float}      {yylval.type_float=atof(yytext); return FLOAT;}
{CHAR}		 {yylval.type_char[0]=yytext[1];if(yytext[2] != '\'') {yylval.type_char[1]=yytext[2];yylval.type_char[2]='\0';} return CHAR;}
{errorid}    {printf("非法定义int型数据 \"%s\"\n\t at Line %d\n",yytext,yylineno);}
"int"        {strcpy(yylval.type_id,  yytext);return TYPE;}
"float"      {strcpy(yylval.type_id,  yytext);return TYPE;}
"char" 	     {strcpy(yylval.type_id,  yytext);return TYPE;}

"return"     {return RETURN;}
"if"         {return IF;}
"else"       {return ELSE;}
"while"      {return WHILE;}
"for" 		 {return FOR;}
"break"		 {return BREAK;}
"continue"	 {return CONTINUE;}
{id}       {strcpy(yylval.type_id,  yytext); return ID;/*由于关键字的形式也符合标识符的规则，所以把关键字的处理全部放在标识符的前面，优先识别*/}
";"			{return SEMI;}
","			{return COMMA;}
">"|"<"|">="|"<="|"=="|"!=" {strcpy(yylval.type_id, yytext);;return RELOP;}
"="			{return ASSIGNOP;}
"+"			{return PLUS;}
"-"			{return MINUS;}
"*"			{return STAR;}
"/"			{return DIV;}
"%"			{return MOD;}
"++" 		        {return DOUBLEADD;}
"--" 		        {return DOUBLESUB;}
"+=" 		        {return COMADD;}
"-=" 		        {return COMSUB;}
"*="     	        {return COMSTAR;}
"/="     	        {return COMDIV;}
"%="     	        {return COMMOD;}
"&&"         {return AND;}
"||"            {return OR;}
"!"			{return NOT;}
"("			{return LP;}
")"			{return RP;}
"{"			{return LC;}
"}"			{return RC;}
"[" 		        {return LB;}
"]" 		        {return RB;}
[\n]           {yycolumn=1;}   
[ \r\t]          {}   
.			{printf("Error type A :Mysterious character \"%s\"\n\t at Line %d\n",yytext,yylineno);}

"//".* { }                                             /*跳过单行注释*/
"/*" {BEGIN COMMENT;}/*"/*"这表示开头应为/* BEFIN COMMENT为条件模式，COMMENT为条件名*/
<COMMENT>"*/" {BEGIN INITIAL;}/*BEGIN INITIAL将休眠所有的条件模式，如果匹配到 退出整个条件模式*/
<COMMENT>([^*]|\n)+|.     /*跳过多行注释*/
%%

/* 和bison联用时，不需要这部分
void main()
{
yylex();
return 0;
}

*/
int yywrap()
{
return 1;
}

