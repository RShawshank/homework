
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
     DPLUS = 264,
     LP = 265,
     RP = 266,
     LC = 267,
     LB = 268,
     RB = 269,
     RC = 270,
     SEMI = 271,
     COMMA = 272,
     PLUS = 273,
     MINUS = 274,
     STAR = 275,
     DIV = 276,
     MOD = 277,
     ASSIGNOP = 278,
     AND = 279,
     OR = 280,
     NOT = 281,
     IF = 282,
     ELSE = 283,
     WHILE = 284,
     RETURN = 285,
     FOR = 286,
     SWITCH = 287,
     CASE = 288,
     COLON = 289,
     DEFAULT = 290,
     COMADD = 291,
     COMSUB = 292,
     COMSTAR = 293,
     COMDIV = 294,
     COMMOD = 295,
     DOUBLEADD = 296,
     DOUBLESUB = 297,
     BREAK = 298,
     CONTINUE = 299,
     EXT_DEF_LIST = 300,
     EXT_VAR_DEF = 301,
     FUNC_DEF = 302,
     FUNC_DEC = 303,
     EXT_DEC_LIST = 304,
     PARAM_LIST = 305,
     PARAM_DEC = 306,
     VAR_DEF = 307,
     DEC_LIST = 308,
     DEF_LIST = 309,
     COMP_STM = 310,
     STM_LIST = 311,
     EXP_STMT = 312,
     IF_THEN = 313,
     IF_THEN_ELSE = 314,
     ARRAY_LIST = 315,
     ARRAY_ID = 316,
     ARRAY_LAST = 317,
     BLANK = 318,
     FOR_DEC = 319,
     FUNC_CALL = 320,
     ARGS = 321,
     FUNCTION = 322,
     PARAM = 323,
     ARG = 324,
     CALL = 325,
     LABEL = 326,
     GOTO = 327,
     JLT = 328,
     JLE = 329,
     JGT = 330,
     JGE = 331,
     EQ = 332,
     NEQ = 333,
     SWITCH_STMT = 334,
     CASE_STMT = 335,
     DEFAULT_STMT = 336,
     EXP_ARRAY = 337,
     ARRAY_DEC = 338,
     DOUBLEADD_R = 339,
     DOUBLEADD_L = 340,
     DOUBLESUB_L = 341,
     DOUBLESUB_R = 342,
     UMINUS = 343,
     LOWER_THEN_ELSE = 344
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
#line 151 "parser.tab.h"
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

