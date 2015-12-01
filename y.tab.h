/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     INTEGER = 258,
     VARIABLE = 259,
     PLUS = 260,
     MINUS = 261,
     TIMES = 262,
     DIVIDE = 263,
     GREAT_EQUAL = 264,
     LESS_EQUAL = 265,
     TWO_EQUAL = 266,
     NOT_EQUAL = 267,
     LESS_THAN = 268,
     GREATER_THAN = 269,
     OPEN_PAREN = 270,
     CLOSE_PAREN = 271,
     EQUAL = 272,
     END_LINE = 273,
     AND = 274,
     DO = 275,
     ELSE = 276,
     WHILE = 277,
     THEN = 278,
     END = 279,
     FOR = 280,
     IF = 281,
     VAR = 282,
     OR = 283,
     NOT = 284,
     PRINT = 285,
     EQUALVAR = 286,
     COMMAND = 287,
     IFX = 288,
     UMINUS = 289
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define VARIABLE 259
#define PLUS 260
#define MINUS 261
#define TIMES 262
#define DIVIDE 263
#define GREAT_EQUAL 264
#define LESS_EQUAL 265
#define TWO_EQUAL 266
#define NOT_EQUAL 267
#define LESS_THAN 268
#define GREATER_THAN 269
#define OPEN_PAREN 270
#define CLOSE_PAREN 271
#define EQUAL 272
#define END_LINE 273
#define AND 274
#define DO 275
#define ELSE 276
#define WHILE 277
#define THEN 278
#define END 279
#define FOR 280
#define IF 281
#define VAR 282
#define OR 283
#define NOT 284
#define PRINT 285
#define EQUALVAR 286
#define COMMAND 287
#define IFX 288
#define UMINUS 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 23 "Parser.y"
{
    int iValue;                 /* integer value */
    char sIndex[255];                /* symbol table index */
    nodeType *nPtr;             /* node pointer */
}
/* Line 1529 of yacc.c.  */
#line 123 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

