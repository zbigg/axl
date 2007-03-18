/* A Bison parser, made by GNU Bison 1.875a.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INCLUDE = 258,
     SAFE = 259,
     FOR = 260,
     WHILE = 261,
     DO = 262,
     EWHILE = 263,
     EFOR = 264,
     RETURN = 265,
     EQ = 266,
     NEQ = 267,
     GE = 268,
     LE = 269,
     NF = 270,
     INSTR_IN = 271,
     NIN = 272,
     OROR = 273,
     ANDAND = 274,
     XORXOR = 275,
     IF = 276,
     ELSE = 277,
     SWITCH = 278,
     CASE = 279,
     DEFAULT = 280,
     WITH = 281,
     GLOBAL = 282,
     LOCAL = 283,
     NIL = 284,
     CTX_OP = 285,
     TOK_ARRAY = 286,
     SET_ADD = 287,
     SET_SUB = 288,
     SET_MUL = 289,
     SET_DIV = 290,
     SET_ASK = 291,
     XCHG = 292,
     CHOOSE = 293,
     BEGIN_TEXT = 294,
     END_TEXT = 295,
     BEGIN_BLOCK = 296,
     END_BLOCK = 297,
     ECHO = 298,
     FUNC = 299,
     EFUNC = 300,
     BUILTIN = 301,
     SET = 302,
     SEQ = 303,
     METHOD = 304,
     RANGE = 305,
     TWO_DOT = 306,
     NEW = 307,
     INSTR_DELETE = 308,
     COPY = 309,
     TOK_NEW_VAR = 310,
     ARROW_R = 311,
     ARROW_L = 312,
     TOK_BOGUS = 313,
     PLUS_PLUS = 314,
     MINUS_MINUS = 315,
     TOK_FLOAT = 316,
     TOK_BAD_FLOAT = 317,
     TOK_INTEGER = 318,
     TOK_BAD_INTEGER = 319,
     TOK_CHAR = 320,
     TOK_BAD_CHAR = 321,
     TOK_GARBAGE = 322,
     TOK_STRING = 323,
     TOK_BAD_STRING = 324,
     TOK_IDENTIFIER = 325,
     TOK_BAD_IDENTIFIER = 326,
     TOK_SYMBOL = 327,
     TOK_SYMBOL_ARRAY = 328,
     NUMER = 329,
     TOK_REAL = 330,
     UNARY = 331,
     NORMAL_OPER = 332
   };
#endif
#define INCLUDE 258
#define SAFE 259
#define FOR 260
#define WHILE 261
#define DO 262
#define EWHILE 263
#define EFOR 264
#define RETURN 265
#define EQ 266
#define NEQ 267
#define GE 268
#define LE 269
#define NF 270
#define INSTR_IN 271
#define NIN 272
#define OROR 273
#define ANDAND 274
#define XORXOR 275
#define IF 276
#define ELSE 277
#define SWITCH 278
#define CASE 279
#define DEFAULT 280
#define WITH 281
#define GLOBAL 282
#define LOCAL 283
#define NIL 284
#define CTX_OP 285
#define TOK_ARRAY 286
#define SET_ADD 287
#define SET_SUB 288
#define SET_MUL 289
#define SET_DIV 290
#define SET_ASK 291
#define XCHG 292
#define CHOOSE 293
#define BEGIN_TEXT 294
#define END_TEXT 295
#define BEGIN_BLOCK 296
#define END_BLOCK 297
#define ECHO 298
#define FUNC 299
#define EFUNC 300
#define BUILTIN 301
#define SET 302
#define SEQ 303
#define METHOD 304
#define RANGE 305
#define TWO_DOT 306
#define NEW 307
#define INSTR_DELETE 308
#define COPY 309
#define TOK_NEW_VAR 310
#define ARROW_R 311
#define ARROW_L 312
#define TOK_BOGUS 313
#define PLUS_PLUS 314
#define MINUS_MINUS 315
#define TOK_FLOAT 316
#define TOK_BAD_FLOAT 317
#define TOK_INTEGER 318
#define TOK_BAD_INTEGER 319
#define TOK_CHAR 320
#define TOK_BAD_CHAR 321
#define TOK_GARBAGE 322
#define TOK_STRING 323
#define TOK_BAD_STRING 324
#define TOK_IDENTIFIER 325
#define TOK_BAD_IDENTIFIER 326
#define TOK_SYMBOL 327
#define TOK_SYMBOL_ARRAY 328
#define NUMER 329
#define TOK_REAL 330
#define UNARY 331
#define NORMAL_OPER 332




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 67 "../../../src/libxl/grammar.y"
typedef union YYSTYPE {
    xl_code			code;
    xl_integer		integer;
    xl_float		real;
	xl_symbol*		sym;
    xlp_expr_list	e_list;
    xlp_name_list	n_list;

	int				atom;
    xl_str			text;
    slist*			lp;
    int 			ei;
    int 			oper;
} YYSTYPE;
/* Line 1240 of yacc.c.  */
#line 206 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



