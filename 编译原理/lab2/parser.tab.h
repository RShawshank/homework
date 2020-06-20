
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     ID = 259,
     RELOP = 260,
     TYPE = 261,
     CHAR = 262,
     FLOAT = 263,
     LP = 264,
     RP = 265,
     LC = 266,
     LB = 267,
     RB = 268,
     RC = 269,
     SEMI = 270,
     COMMA = 271,
     PLUS = 272,
     MINUS = 273,
     STAR = 274,
     DIV = 275,
     ASSIGNOP = 276,
     AND = 277,
     OR = 278,
     NOT = 279,
     IF = 280,
     ELSE = 281,
     WHILE = 282,
     RETURN = 283,
     FOR = 284,
     SWITCH = 285,
     CASE = 286,
     COLON = 287,
     DEFAULT = 288,
     COMADD = 289,
     COMSUB = 290,
     COMSTAR = 291,
     COMDIV = 292,
     COMMOD = 293,
     DOUBLEADD = 294,
     DOUBLESUB = 295,
     BREAK = 296,
     CONTINUE = 297,
     EXT_DEF_LIST = 298,
     EXT_VAR_DEF = 299,
     FUNC_DEF = 300,
     FUNC_DEC = 301,
     EXT_DEC_LIST = 302,
     PARAM_LIST = 303,
     PARAM_DEC = 304,
     VAR_DEF = 305,
     DEC_LIST = 306,
     DEF_LIST = 307,
     COMP_STM = 308,
     STM_LIST = 309,
     EXP_STMT = 310,
     IF_THEN = 311,
     IF_THEN_ELSE = 312,
     ARRAY_DEFINE = 313,
     ARRAY_CALL = 314,
     BLANK = 315,
     CONTINUE_ADD = 316,
     BREAK_ADD = 317,
     ARRAY_DEC = 318,
     FUNC_CALL = 319,
     ARGS = 320,
     FUNCTION = 321,
     PARAM = 322,
     ARG = 323,
     CALL = 324,
     LABEL = 325,
     GOTO = 326,
     JLT = 327,
     JLE = 328,
     JGT = 329,
     JGE = 330,
     EQ = 331,
     NEQ = 332,
     MOD = 333,
     DPLUS = 334,
     UMINUS = 335,
     LOWER_THEN_ELSE = 336
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 20 "parser.y"

	int    type_int;
	float  type_float;
	char   type_id[32];
	char type_char[3];
	struct ASTNode *ptr;



/* Line 1676 of yacc.c  */
#line 143 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;

