%{
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
int    ([+-]?[0-9]+)|(0[xX][0-9a-fA-F]+)|(0[0-7]+)
float  ([0-9]*\.[0-9]+)|([0-9]+\.)
CHAR \'([^'\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[Xx][0-9A-Fa-f]+|{UCN})+\'

%%
{int}        {yylval.type_int=strtol(yytext,NULL,0); return INT;}
{float}      {yylval.type_float=atof(yytext); return FLOAT;}
{CHAR}		 {yylval.type_char[0]=yytext[1];if(yytext[2] != '\'') {yylval.type_char[1]=yytext[2];yylval.type_char[2]='\0';} return CHAR;}

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
"/*" {BEGIN COMMENT;}
<COMMENT>"*/" {BEGIN INITIAL;}
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

