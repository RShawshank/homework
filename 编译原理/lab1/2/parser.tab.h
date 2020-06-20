/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    BREAK = 289,
    CONTINUE = 290,
    EXT_DEF_LIST = 291,
    EXT_VAR_DEF = 292,
    FUNC_DEF = 293,
    FUNC_DEC = 294,
    EXT_DEC_LIST = 295,
    PARAM_LIST = 296,
    PARAM_DEC = 297,
    VAR_DEF = 298,
    DEC_LIST = 299,
    DEF_LIST = 300,
    COMP_STM = 301,
    STM_LIST = 302,
    EXP_STMT = 303,
    IF_THEN = 304,
    IF_THEN_ELSE = 305,
    ARRAY_DEFINE = 306,
    ARRAY_CALL = 307,
    BLANK = 308,
    CONTINUE_ADD = 309,
    BREAK_ADD = 310,
    ARRAY_DEC = 311,
    FUNC_CALL = 312,
    ARGS = 313,
    FUNCTION = 314,
    PARAM = 315,
    ARG = 316,
    CALL = 317,
    LABEL = 318,
    GOTO = 319,
    JLT = 320,
    JLE = 321,
    JGT = 322,
    JGE = 323,
    EQ = 324,
    NEQ = 325,
    COMADD = 326,
    COMSUB = 327,
    COMSTAR = 328,
    COMDIV = 329,
    COMMOD = 330,
    MOD = 331,
    DOUBLEADD = 332,
    DOUBLESUB = 333,
    UMINUS = 334,
    DPLUS = 335,
    LOWER_THEN_ELSE = 336
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "parser.y" /* yacc.c:1909  */

	int    type_int;
	float  type_float;
	char   type_id[32];
	char type_char[3];
	struct ASTNode *ptr;

#line 144 "parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
