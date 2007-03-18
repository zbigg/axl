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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 20 "../../../src/libxl/grammar.y"


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>

#include "parse.h"

#ifdef _MSC_VER
/*
   Yacc generate labels that are not used, disable warning in MSC.
*/
#pragma warning (disable: 4102) // 'yyerrlab' : unreferenced label
#endif



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

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
/* Line 191 of yacc.c.  */
#line 264 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 276 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1104

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  103
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  50
/* YYNRULES -- Number of rules. */
#define YYNRULES  162
/* YYNRULES -- Number of states. */
#define YYNSTATES  326

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    95,     2,     2,   100,    92,    84,     2,
      97,   102,    90,    88,    77,    89,    96,    91,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    79,    78,
      85,    80,    86,    81,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    98,     2,    99,    83,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,    82,   101,    94,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    87,    93
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     6,     7,     8,    12,    17,    23,    24,
      30,    36,    43,    48,    51,    56,    57,    61,    66,    72,
      73,    74,    80,    81,    82,    84,    86,    89,    92,    99,
     109,   117,   123,   130,   131,   142,   145,   146,   155,   161,
     164,   166,   168,   170,   171,   177,   182,   184,   187,   190,
     193,   196,   199,   202,   205,   209,   213,   217,   221,   225,
     229,   235,   240,   246,   256,   258,   262,   264,   268,   270,
     274,   278,   282,   286,   290,   294,   298,   300,   304,   308,
     312,   316,   318,   322,   326,   330,   332,   335,   338,   341,
     344,   347,   350,   352,   354,   356,   358,   360,   362,   364,
     366,   369,   372,   374,   378,   380,   384,   388,   390,   392,
     394,   397,   403,   408,   410,   412,   415,   417,   419,   420,
     421,   427,   431,   434,   437,   442,   447,   451,   453,   456,
     460,   462,   466,   467,   475,   476,   477,   486,   488,   491,
     495,   497,   499,   504,   509,   513,   518,   520,   523,   525,
     529,   530,   532,   534,   538,   539,   541,   543,   545,   547,
     549,   553,   557
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     104,     0,    -1,   105,   106,    -1,    -1,    -1,   106,   114,
      78,    -1,   106,     3,    68,    78,    -1,   106,     3,    68,
       4,    78,    -1,    -1,   107,    92,    80,   119,    78,    -1,
     107,    92,     3,    68,    78,    -1,   107,    92,     3,    68,
       4,    78,    -1,   107,   100,   119,    78,    -1,   107,    67,
      -1,   107,    92,   114,    78,    -1,    -1,   108,   114,    78,
      -1,   108,     3,    68,    78,    -1,   108,     3,    68,     4,
      78,    -1,    -1,    -1,    76,   110,   108,   111,   101,    -1,
      -1,    -1,   109,    -1,   119,    -1,   134,   147,    -1,    43,
     149,    -1,     7,   114,    21,    97,   119,   102,    -1,    21,
      97,   119,   102,   112,   114,    22,   113,   114,    -1,    21,
      97,   119,   102,   112,   114,    58,    -1,     6,    97,   119,
     102,   114,    -1,     7,   114,     6,    97,   119,   102,    -1,
      -1,     5,   115,    97,   114,    78,   119,    78,   114,   102,
     114,    -1,    10,   119,    -1,    -1,    23,    97,   119,   102,
     116,    76,   117,   101,    -1,    26,    97,   119,   102,   114,
      -1,    27,   119,    -1,   133,    -1,   145,    -1,     1,    -1,
      -1,   118,    56,   114,    78,   117,    -1,    25,    56,   114,
      78,    -1,   119,    -1,    11,   119,    -1,    12,   119,    -1,
      86,   119,    -1,    85,   119,    -1,    13,   119,    -1,    14,
     119,    -1,    16,   119,    -1,   134,    32,   119,    -1,   134,
      33,   119,    -1,   134,    34,   119,    -1,   134,    35,   119,
      -1,   134,    36,   119,    -1,   134,    80,   119,    -1,   120,
      81,   119,    79,   119,    -1,   120,    81,    79,   119,    -1,
       8,    97,   119,   102,   119,    -1,     9,    97,   114,    78,
     119,    78,   114,   102,   119,    -1,   120,    -1,   121,    18,
     120,    -1,   121,    -1,   122,    19,   121,    -1,   122,    -1,
     123,    11,   122,    -1,   123,    12,   122,    -1,   123,    86,
     122,    -1,   123,    85,   122,    -1,   123,    13,   122,    -1,
     123,    14,   122,    -1,   123,    16,   122,    -1,   123,    -1,
     123,    88,   124,    -1,   123,    89,   124,    -1,   123,    84,
     124,    -1,   123,    82,   124,    -1,   124,    -1,   124,    90,
     131,    -1,   124,    91,   131,    -1,   124,    92,   131,    -1,
     131,    -1,    95,   131,    -1,    94,   131,    -1,    89,   131,
      -1,    59,   131,    -1,    60,   131,    -1,    54,   131,    -1,
      61,    -1,    68,    -1,    63,    -1,    65,    -1,    69,    -1,
      64,    -1,    62,    -1,    66,    -1,    52,   128,    -1,    52,
     151,    -1,    52,    -1,    76,   129,   101,    -1,   130,    -1,
     129,    77,   130,    -1,   151,    56,   119,    -1,   125,    -1,
     134,    -1,   134,    -1,    53,   151,    -1,    53,   134,    98,
     119,    99,    -1,    53,   134,    96,   151,    -1,   126,    -1,
     127,    -1,    55,   151,    -1,    70,    -1,    24,    -1,    -1,
      -1,    39,   135,   107,   136,    40,    -1,    97,   119,   102,
      -1,   134,    59,    -1,   134,    60,    -1,   134,    97,   148,
     102,    -1,   134,    98,   119,    99,    -1,   134,    96,   151,
      -1,   137,    -1,    46,    68,    -1,    46,    68,     4,    -1,
     143,    -1,   141,   142,   109,    -1,    -1,   141,   142,    56,
      97,   138,   119,   102,    -1,    -1,    -1,   141,   142,    56,
      39,   139,   107,   140,    40,    -1,    44,    -1,    44,   151,
      -1,    97,   150,   102,    -1,    72,    -1,   144,    -1,    73,
      98,   119,    99,    -1,   144,    98,   119,    99,    -1,    55,
      70,    31,    -1,   145,    98,    63,    99,    -1,   119,    -1,
      84,   119,    -1,   132,    -1,   147,    77,   146,    -1,    -1,
     149,    -1,   146,    -1,   146,    77,   149,    -1,    -1,   152,
      -1,    72,    -1,    73,    -1,    70,    -1,   151,    -1,   151,
      80,   119,    -1,   151,    77,   152,    -1,   151,    80,   119,
      77,   152,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   143,   143,   151,   153,   156,   164,   171,   179,   183,
     187,   194,   199,   203,   207,   215,   218,   222,   228,   237,
     239,   236,   248,   253,   258,   259,   269,   275,   279,   283,
     287,   292,   296,   301,   300,   307,   312,   311,   319,   323,
     327,   328,   332,   339,   342,   346,   353,   354,   355,   356,
     357,   358,   359,   360,   401,   402,   403,   404,   405,   406,
     407,   408,   410,   411,   415,   418,   419,   422,   423,   427,
     428,   429,   430,   431,   432,   433,   434,   438,   439,   440,
     441,   442,   446,   447,   448,   449,   453,   454,   455,   456,
     457,   458,   462,   463,   464,   465,   466,   467,   468,   469,
     472,   473,   474,   478,   483,   484,   488,   491,   492,   495,
     499,   501,   503,   508,   509,   510,   511,   512,   514,   516,
     513,   519,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   533,   541,   540,   555,   560,   552,   570,   571,   574,
     584,   592,   603,   604,   614,   615,   620,   621,   625,   630,
     639,   643,   650,   655,   664,   669,   674,   675,   676,   679,
     687,   695,   706
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INCLUDE", "SAFE", "FOR", "WHILE", "DO", 
  "EWHILE", "EFOR", "RETURN", "EQ", "NEQ", "GE", "LE", "NF", "INSTR_IN", 
  "NIN", "OROR", "ANDAND", "XORXOR", "IF", "ELSE", "SWITCH", "CASE", 
  "DEFAULT", "WITH", "GLOBAL", "LOCAL", "NIL", "CTX_OP", "TOK_ARRAY", 
  "SET_ADD", "SET_SUB", "SET_MUL", "SET_DIV", "SET_ASK", "XCHG", "CHOOSE", 
  "BEGIN_TEXT", "END_TEXT", "BEGIN_BLOCK", "END_BLOCK", "ECHO", "FUNC", 
  "EFUNC", "BUILTIN", "SET", "SEQ", "METHOD", "RANGE", "TWO_DOT", "NEW", 
  "INSTR_DELETE", "COPY", "TOK_NEW_VAR", "ARROW_R", "ARROW_L", 
  "TOK_BOGUS", "PLUS_PLUS", "MINUS_MINUS", "TOK_FLOAT", "TOK_BAD_FLOAT", 
  "TOK_INTEGER", "TOK_BAD_INTEGER", "TOK_CHAR", "TOK_BAD_CHAR", 
  "TOK_GARBAGE", "TOK_STRING", "TOK_BAD_STRING", "TOK_IDENTIFIER", 
  "TOK_BAD_IDENTIFIER", "TOK_SYMBOL", "TOK_SYMBOL_ARRAY", "NUMER", 
  "TOK_REAL", "'{'", "','", "';'", "':'", "'='", "'?'", "'|'", "'^'", 
  "'&'", "'<'", "'>'", "UNARY", "'+'", "'-'", "'*'", "'/'", "'%'", 
  "NORMAL_OPER", "'~'", "'!'", "'.'", "'('", "'['", "']'", "'$'", "'}'", 
  "')'", "$accept", "input", "xxx_frame_begin", "program", 
  "more_instr_doc", "more_instr", "block", "@1", "@2", "if_starter", 
  "if_finish", "instr", "@3", "@4", "case_element", "case_expr", "expr", 
  "alternative", "conjugation", "comparision", "addition", 
  "multiplication", "unary_value", "immediate", "obj_constructor", 
  "map_constructor", "value_mapping_list", "value_mapping", "value", 
  "value_no_unary", "delete_instr", "primary", "@5", "@6", "func_decl", 
  "@7", "@8", "@9", "func_decl_prolog", "func_param_list", 
  "primary_symbol", "sym_array_ref", "array_decl", "parameter", 
  "nu_expr_list_ne", "expr_list", "expr_list_ne", "param_list", "name", 
  "param_list_ne", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   123,    44,    59,    58,
      61,    63,   124,    94,    38,    60,    62,   331,    43,    45,
      42,    47,    37,   332,   126,    33,    46,    40,    91,    93,
      36,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   103,   104,   105,   106,   106,   106,   106,   107,   107,
     107,   107,   107,   107,   107,   108,   108,   108,   108,   110,
     111,   109,   112,   113,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   115,   114,   114,   116,   114,   114,   114,
     114,   114,   114,   117,   117,   117,   118,   118,   118,   118,
     118,   118,   118,   118,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   120,   120,   121,   121,   122,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   123,
     123,   123,   124,   124,   124,   124,   125,   125,   125,   125,
     125,   125,   126,   126,   126,   126,   126,   126,   126,   126,
     127,   127,   127,   128,   129,   129,   130,   131,   131,   132,
     133,   133,   133,   134,   134,   134,   134,   134,   135,   136,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   137,   138,   137,   139,   140,   137,   141,   141,   142,
     143,   143,   144,   144,   145,   145,   146,   146,   147,   147,
     148,   148,   149,   149,   150,   150,   151,   151,   151,   152,
     152,   152,   152
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     2,     0,     0,     3,     4,     5,     0,     5,
       5,     6,     4,     2,     4,     0,     3,     4,     5,     0,
       0,     5,     0,     0,     1,     1,     2,     2,     6,     9,
       7,     5,     6,     0,    10,     2,     0,     8,     5,     2,
       1,     1,     1,     0,     5,     4,     1,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       5,     4,     5,     9,     1,     3,     1,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     3,     1,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     3,     1,     3,     3,     1,     1,     1,
       2,     5,     4,     1,     1,     2,     1,     1,     0,     0,
       5,     3,     2,     2,     4,     4,     3,     1,     2,     3,
       1,     3,     0,     7,     0,     0,     8,     1,     2,     3,
       1,     1,     4,     4,     3,     4,     1,     2,     1,     3,
       0,     1,     1,     3,     0,     1,     1,     1,     1,     1,
       3,     3,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       3,     0,     4,     1,     0,    42,     0,    33,     0,     0,
       0,     0,     0,     0,     0,   117,     0,     0,   118,     0,
     137,     0,   102,     0,     0,     0,     0,     0,    92,    98,
      94,    97,    95,    99,    93,    96,   116,   140,     0,    19,
       0,     0,     0,     0,    24,     0,    25,    64,    66,    68,
      76,    81,   107,   113,   114,    85,    40,   108,   127,     0,
     130,   141,    41,     0,     0,     0,     0,     0,     0,     0,
      35,   108,     0,     0,     0,    39,     8,     0,   146,   152,
      27,   158,   156,   157,   138,   128,     0,   100,   101,   158,
     156,   157,     0,   110,    91,   108,   158,   115,    89,    90,
       0,    15,    88,    87,    86,     0,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   122,
     123,     0,     0,   150,     0,   148,   109,    26,   154,     0,
       0,     0,     0,     6,     0,     0,     0,     0,     0,     0,
     150,     0,     0,     0,   119,   147,     0,   129,     0,   104,
       0,     0,     0,   144,     0,     0,   121,     0,     0,    65,
      67,    69,    70,    73,    74,    75,    80,    79,    72,    71,
      77,    78,    82,    83,    84,    54,    55,    56,    57,    58,
      59,   126,   146,     0,   151,     0,     0,     0,   159,   155,
       0,   131,     0,     0,     7,     0,     0,     0,     0,     0,
       0,    22,    36,     0,    13,     0,     0,     0,   153,     0,
     103,     0,   112,     0,   142,     0,     0,     0,    61,     0,
     124,   125,   149,   139,     0,     0,   134,   132,   143,   145,
       0,    31,     0,     0,    62,     0,     0,     0,    38,     0,
       0,     0,     0,   120,   105,   106,   111,     0,    21,    16,
      60,   161,   160,     8,     0,     0,    32,    28,     0,     0,
      43,     0,     0,    14,    12,     0,    17,     0,   135,     0,
       0,     0,    23,    30,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    46,     0,    10,     9,    18,   162,
       0,   133,     0,     0,     0,    47,    48,    51,    52,    53,
       0,    50,    49,    37,     0,    11,   136,     0,    63,    29,
       0,     0,    34,    45,    43,    44
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     1,     2,     4,   154,   165,    44,   101,   226,   246,
     304,    45,    64,   247,   292,   293,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    87,   158,   159,    55,   135,
      56,    71,    76,   217,    58,   264,   263,   300,    59,   139,
      60,    61,    62,    79,   137,   193,   194,   197,   198,   199
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -228
static const short yypact[] =
{
    -228,    16,  -228,  -228,   448,  -228,   -54,  -228,   -57,   602,
     -55,   -53,   866,   -38,   -13,  -228,    -3,   866,  -228,   734,
      -5,    27,   -10,  1007,   960,     9,   960,   960,  -228,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,    -1,  -228,
     960,   960,   960,   866,  -228,    43,  -228,    41,   106,   110,
      14,     1,  -228,  -228,  -228,  -228,  -228,   913,  -228,    42,
    -228,    46,    49,    -2,    51,   866,    37,   866,   602,    -5,
    -228,   119,   866,   866,   866,  -228,  -228,   866,  -228,    73,
    -228,  -228,  -228,  -228,  -228,   152,    -5,  -228,  -228,   -42,
     -25,    -1,   -11,  -228,  -228,    -8,   126,  -228,  -228,  -228,
     866,  -228,  -228,  -228,  -228,    60,  -228,   800,   960,   960,
     960,   960,   960,   960,   960,   960,   960,   960,   960,   960,
     960,   960,   960,   960,   866,   866,   866,   866,   866,  -228,
    -228,   866,    -5,   734,   866,  -228,    -8,    86,    -5,   -35,
     866,    97,    88,  -228,   602,    62,    70,    72,    75,   103,
     734,    85,   101,   102,    76,  -228,   734,  -228,   -62,  -228,
     132,    -5,   866,  -228,    91,   349,  -228,   866,   127,  -228,
    -228,  -228,  -228,  -228,  -228,  -228,     1,     1,  -228,  -228,
       1,     1,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,    60,   108,  -228,   113,   734,   111,   -68,  -228,
     -36,  -228,   120,   123,  -228,   140,   602,   866,   866,   866,
     866,  -228,  -228,   602,  -228,   525,   866,   165,  -228,    -5,
    -228,   866,    74,   124,  -228,   156,   125,   149,  -228,   866,
    -228,  -228,  -228,  -228,    -5,   866,  -228,  -228,  -228,  -228,
     866,  -228,   129,   130,  -228,   151,   602,   157,  -228,   166,
     866,   158,   159,  -228,  -228,  -228,    77,     0,  -228,  -228,
    -228,  -228,   161,  -228,   866,   162,  -228,  -228,   602,    11,
     668,     2,   163,  -228,  -228,   167,  -228,    -5,    76,   133,
     602,   137,  -228,  -228,   866,   866,   866,   866,   866,   187,
     866,   866,   143,   190,  -228,   169,  -228,  -228,  -228,  -228,
     208,  -228,   147,   866,   602,  -228,  -228,  -228,  -228,  -228,
     602,  -228,  -228,  -228,   602,  -228,  -228,   602,  -228,  -228,
     172,   173,  -228,  -228,   668,  -228
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -228,  -228,  -228,  -228,    -7,  -228,   114,  -228,  -228,  -228,
    -228,    15,  -228,  -228,   -72,  -228,    58,   150,   146,    83,
    -228,    26,  -228,  -228,  -228,  -228,  -228,    38,     5,  -228,
    -228,    -4,  -228,  -228,  -228,  -228,  -228,  -228,  -228,  -228,
    -228,  -228,  -228,    63,  -228,  -228,   -18,  -228,   -12,  -227
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -141
static const short yytable[] =
{
      57,    80,   142,   236,   275,    57,   295,   261,    84,   234,
      88,    93,   235,    97,    63,   219,     3,  -116,  -116,    92,
      95,   200,    95,    95,    66,   110,   111,   112,   113,    94,
     114,    98,    99,   282,  -140,  -140,    95,    95,    95,   220,
      65,    39,    67,   146,    68,   102,   103,   104,   129,   130,
     299,   129,   130,   136,  -116,  -116,  -116,    97,   147,    72,
      81,   237,    82,    83,    57,    81,    86,    82,    83,   283,
      70,  -140,  -140,  -140,   160,    75,   143,    78,   276,    96,
     296,    82,    83,   149,    73,   161,   150,   162,   132,   150,
     134,   121,   122,   123,    74,    85,   115,   100,   116,   117,
     118,   105,   119,   120,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
     191,   106,   107,   145,   108,   148,   182,   183,   184,   109,
     151,   152,   153,  -126,  -126,   155,  -125,  -125,   218,   138,
      57,   176,   177,   214,   140,   180,   181,   141,   144,   222,
     156,   124,   125,   126,   127,   128,   157,   163,   164,   205,
     203,    57,   166,   196,   206,   168,   204,   207,   215,   208,
    -126,  -126,  -126,  -125,  -125,  -125,   216,   209,   129,   130,
     227,   210,   185,   186,   187,   188,   189,   211,   221,   190,
     224,   192,   195,   171,   172,   173,   174,   175,   202,   131,
     178,   179,    57,   212,   213,   253,   229,   160,    78,    57,
     230,    57,   231,   233,    78,   132,   150,   134,   240,   238,
     223,   241,   239,   256,   257,   228,   258,   259,   248,   268,
     251,   266,   267,   270,   271,   301,   273,   274,   277,   303,
     280,   297,    57,   310,   313,   298,   314,   315,   316,   317,
     323,   324,   325,   201,    78,   170,   278,   254,   169,   232,
       0,   269,     0,     0,    57,   242,   243,   244,   245,     0,
       0,     0,     0,     0,   252,     0,    57,     0,     0,   255,
       0,     0,     0,   281,     0,     0,     0,   260,     0,     0,
       0,     0,     0,   262,     0,   302,     0,     0,   265,     0,
      57,     0,     0,     0,     0,     0,    57,     0,   272,     0,
      57,     0,     0,    57,     0,     0,     0,     0,     0,   319,
       0,     0,   279,     0,     0,   320,     0,     0,   294,   321,
       0,     0,   322,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   305,   306,   307,   308,   309,     0,   311,   312,
       5,     0,   225,     0,     7,     8,     9,    10,    11,    12,
       0,   318,     0,     0,     0,     0,     0,     0,     0,     0,
      13,     0,    14,    15,     0,    16,    17,     0,     0,     0,
       0,     0,   294,     0,     0,     0,     0,     0,    18,     0,
       0,     0,    19,    20,     0,    21,     0,     0,     0,     0,
       0,    22,    23,    24,    25,     0,     0,     0,    26,    27,
      28,    29,    30,    31,    32,    33,     0,    34,    35,    36,
       0,    37,    38,     0,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,    41,    42,     0,    43,     0,    -2,     5,
     -20,     6,     0,     7,     8,     9,    10,    11,    12,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    13,
       0,    14,    15,     0,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,    19,    20,     0,    21,     0,     0,     0,     0,     0,
      22,    23,    24,    25,     0,     0,     0,    26,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,    36,     0,
      37,    38,     0,     0,    39,     0,     5,     0,   249,     0,
       7,     8,     9,    10,    11,    12,     0,    40,     0,     0,
       0,     0,    41,    42,     0,    43,    13,     0,    14,    15,
       0,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    18,     0,     0,     0,    19,    20,
       0,    21,     0,     0,     0,     0,     0,    22,    23,    24,
      25,     0,     0,     0,    26,    27,    28,    29,    30,    31,
      32,    33,     0,    34,    35,    36,     0,    37,    38,     0,
       0,    39,     0,     5,     0,   250,     0,     7,     8,     9,
      10,    11,    12,     0,    40,     0,     0,     0,     0,    41,
      42,     0,    43,    13,     0,    14,    15,     0,    16,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,     0,     0,     0,    19,    20,     0,    21,     0,
       0,     0,     0,     0,    22,    23,    24,    25,     0,     0,
       0,    26,    27,    28,    29,    30,    31,    32,    33,     0,
      34,    35,    36,     0,    37,    38,    10,    11,    39,   284,
     285,   286,   287,     0,   288,     0,     0,     0,     0,     0,
       0,    40,    15,   289,     0,     0,    41,    42,     0,    43,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     0,
       0,     0,    20,     0,    21,     0,     0,     0,     0,     0,
      22,     0,    24,    69,     0,     0,     0,    26,    27,    28,
      29,    30,    31,    32,    33,     0,    34,    35,    36,     0,
      37,    38,    10,    11,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   290,   291,     0,     0,    40,    15,     0,
       0,     0,    41,    42,     0,    43,     0,     0,     0,     0,
       0,     0,     0,    18,     0,     0,     0,     0,    20,     0,
      21,     0,     0,     0,     0,     0,    22,     0,    24,    69,
       0,     0,     0,    26,    27,    28,    29,    30,    31,    32,
      33,     0,    34,    35,    36,     0,    37,    38,    10,    11,
       0,     0,     0,     0,     0,     0,     0,     0,    77,     0,
       0,     0,     0,    40,    15,     0,     0,     0,    41,    42,
       0,    43,     0,     0,     0,     0,     0,     0,     0,    18,
       0,     0,     0,     0,    20,     0,    21,     0,     0,     0,
       0,     0,    22,     0,    24,    69,     0,     0,     0,    26,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
      36,     0,    37,    38,    10,    11,     0,     0,     0,   167,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      15,     0,     0,     0,    41,    42,     0,    43,     0,     0,
       0,     0,     0,     0,     0,    18,     0,     0,     0,     0,
      20,     0,    21,     0,     0,     0,     0,     0,    22,     0,
      24,    69,     0,     0,     0,    26,    27,    28,    29,    30,
      31,    32,    33,     0,    34,    35,    36,    15,    37,    38,
       0,     0,     0,     0,     0,   124,   125,   126,   127,   128,
       0,     0,    18,     0,     0,    40,     0,    20,     0,    21,
      41,    42,     0,    43,     0,    22,     0,     0,    69,     0,
       0,     0,   129,   130,    28,    29,    30,    31,    32,    33,
       0,    34,    35,    36,    15,    37,    38,     0,     0,     0,
       0,     0,     0,   131,     0,     0,     0,     0,     0,    18,
       0,     0,     0,     0,    20,     0,    21,     0,     0,   132,
     133,   134,    22,     0,    24,    69,     0,     0,     0,    26,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
      36,    15,    37,    38,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,    40,
       0,    20,     0,    21,    41,    42,     0,    43,     0,    22,
       0,     0,    69,     0,     0,     0,     0,     0,    28,    29,
      30,    31,    32,    33,     0,    34,    35,    89,     0,    90,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43
};

static const short yycheck[] =
{
       4,    19,     4,    39,     4,     9,     4,   234,    20,    77,
      22,    23,    80,    25,    68,    77,     0,    59,    60,    23,
      24,    56,    26,    27,     9,    11,    12,    13,    14,    24,
      16,    26,    27,    22,    59,    60,    40,    41,    42,   101,
      97,    76,    97,     6,    97,    40,    41,    42,    59,    60,
     277,    59,    60,    57,    96,    97,    98,    69,    21,    97,
      70,    97,    72,    73,    68,    70,    76,    72,    73,    58,
      12,    96,    97,    98,    86,    17,    78,    19,    78,    70,
      78,    72,    73,    68,    97,    96,    97,    98,    96,    97,
      98,    90,    91,    92,    97,    68,    82,    98,    84,    85,
      86,    43,    88,    89,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     132,    78,    81,    65,    18,    67,   121,   122,   123,    19,
      72,    73,    74,    59,    60,    77,    59,    60,   156,    97,
     144,   115,   116,    67,    98,   119,   120,    98,    97,   161,
      77,    32,    33,    34,    35,    36,     4,    31,   100,   144,
      63,   165,   102,    77,   102,   107,    78,    97,    92,    97,
      96,    97,    98,    96,    97,    98,   100,   102,    59,    60,
     165,    78,   124,   125,   126,   127,   128,   102,    56,   131,
      99,   133,   134,   110,   111,   112,   113,   114,   140,    80,
     117,   118,   206,   102,   102,    40,    79,   219,   150,   213,
     102,   215,    99,   102,   156,    96,    97,    98,    78,    99,
     162,   206,    99,    99,    68,   167,   101,    78,   213,    78,
     215,   102,   102,    76,    68,   102,    78,    78,    77,   102,
      78,    78,   246,    56,   101,    78,    56,    78,    40,   102,
      78,    78,   324,   139,   196,   109,   263,   219,   108,   196,
      -1,   246,    -1,    -1,   268,   207,   208,   209,   210,    -1,
      -1,    -1,    -1,    -1,   216,    -1,   280,    -1,    -1,   221,
      -1,    -1,    -1,   268,    -1,    -1,    -1,   229,    -1,    -1,
      -1,    -1,    -1,   235,    -1,   280,    -1,    -1,   240,    -1,
     304,    -1,    -1,    -1,    -1,    -1,   310,    -1,   250,    -1,
     314,    -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,   304,
      -1,    -1,   264,    -1,    -1,   310,    -1,    -1,   270,   314,
      -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   284,   285,   286,   287,   288,    -1,   290,   291,
       1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    23,    24,    -1,    26,    27,    -1,    -1,    -1,
      -1,    -1,   324,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    43,    44,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    52,    53,    54,    55,    -1,    -1,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    72,    73,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    94,    95,    -1,    97,    -1,     0,     1,
     101,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    23,    24,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    43,    44,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      52,    53,    54,    55,    -1,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,    -1,    -1,    76,    -1,     1,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    89,    -1,    -1,
      -1,    -1,    94,    95,    -1,    97,    21,    -1,    23,    24,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    43,    44,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    68,    69,    70,    -1,    72,    73,    -1,
      -1,    76,    -1,     1,    -1,    80,    -1,     5,     6,     7,
       8,     9,    10,    -1,    89,    -1,    -1,    -1,    -1,    94,
      95,    -1,    97,    21,    -1,    23,    24,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    43,    44,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    52,    53,    54,    55,    -1,    -1,
      -1,    59,    60,    61,    62,    63,    64,    65,    66,    -1,
      68,    69,    70,    -1,    72,    73,     8,     9,    76,    11,
      12,    13,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    24,    25,    -1,    -1,    94,    95,    -1,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    54,    55,    -1,    -1,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    -1,    68,    69,    70,    -1,
      72,    73,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    -1,    -1,    89,    24,    -1,
      -1,    -1,    94,    95,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,    55,
      -1,    -1,    -1,    59,    60,    61,    62,    63,    64,    65,
      66,    -1,    68,    69,    70,    -1,    72,    73,     8,     9,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    -1,
      -1,    -1,    -1,    89,    24,    -1,    -1,    -1,    94,    95,
      -1,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    52,    -1,    54,    55,    -1,    -1,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    72,    73,     8,     9,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      24,    -1,    -1,    -1,    94,    95,    -1,    97,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      54,    55,    -1,    -1,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    -1,    68,    69,    70,    24,    72,    73,
      -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      -1,    -1,    39,    -1,    -1,    89,    -1,    44,    -1,    46,
      94,    95,    -1,    97,    -1,    52,    -1,    -1,    55,    -1,
      -1,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    70,    24,    72,    73,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    -1,    96,
      97,    98,    52,    -1,    54,    55,    -1,    -1,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    24,    72,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    89,
      -1,    44,    -1,    46,    94,    95,    -1,    97,    -1,    52,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    72,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,   104,   105,     0,   106,     1,     3,     5,     6,     7,
       8,     9,    10,    21,    23,    24,    26,    27,    39,    43,
      44,    46,    52,    53,    54,    55,    59,    60,    61,    62,
      63,    64,    65,    66,    68,    69,    70,    72,    73,    76,
      89,    94,    95,    97,   109,   114,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   131,   133,   134,   137,   141,
     143,   144,   145,    68,   115,    97,   114,    97,    97,    55,
     119,   134,    97,    97,    97,   119,   135,    84,   119,   146,
     149,    70,    72,    73,   151,    68,    76,   128,   151,    70,
      72,    73,   134,   151,   131,   134,    70,   151,   131,   131,
      98,   110,   131,   131,   131,   119,    78,    81,    18,    19,
      11,    12,    13,    14,    16,    82,    84,    85,    86,    88,
      89,    90,    91,    92,    32,    33,    34,    35,    36,    59,
      60,    80,    96,    97,    98,   132,   134,   147,    97,   142,
      98,    98,     4,    78,    97,   119,     6,    21,   119,   114,
      97,   119,   119,   119,   107,   119,    77,     4,   129,   130,
     151,    96,    98,    31,   119,   108,   102,    79,   119,   120,
     121,   122,   122,   122,   122,   122,   124,   124,   122,   122,
     124,   124,   131,   131,   131,   119,   119,   119,   119,   119,
     119,   151,   119,   148,   149,   119,    77,   150,   151,   152,
      56,   109,   119,    63,    78,   114,   102,    97,    97,   102,
      78,   102,   102,   102,    67,    92,   100,   136,   149,    77,
     101,    56,   151,   119,    99,     3,   111,   114,   119,    79,
     102,    99,   146,   102,    77,    80,    39,    97,    99,    99,
      78,   114,   119,   119,   119,   119,   112,   116,   114,     3,
      80,   114,   119,    40,   130,   119,    99,    68,   101,    78,
     119,   152,   119,   139,   138,   119,   102,   102,    78,   114,
      76,    68,   119,    78,    78,     4,    78,    77,   107,   119,
      78,   114,    22,    58,    11,    12,    13,    14,    16,    25,
      85,    86,   117,   118,   119,     4,    78,    78,    78,   152,
     140,   102,   114,   102,   113,   119,   119,   119,   119,   119,
      56,   119,   119,   101,    56,    78,    40,   102,   119,   114,
     114,   114,   114,    78,    78,   117
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 144 "../../../src/libxl/grammar.y"
    {
			xl_code_global = xlcg_frame(xl_code_global);
			frame_finish();
		}
    break;

  case 3:
#line 151 "../../../src/libxl/grammar.y"
    { frame_begin(); }
    break;

  case 5:
#line 157 "../../../src/libxl/grammar.y"
    {
		xl_code_global = xlc_join(
			xl_code_global ? 
				xl_code_global : 
				xlc_new() ,
			yyvsp[-1].code);
	    }
    break;

  case 6:
#line 165 "../../../src/libxl/grammar.y"
    {
			if( lex_include_file(xls_get(yyvsp[-1].text),0) < 0 ) {
				YYABORT;
			}
	    }
    break;

  case 7:
#line 172 "../../../src/libxl/grammar.y"
    {
			lex_include_file(xls_get(yyvsp[-2].text),1);
	    }
    break;

  case 8:
#line 179 "../../../src/libxl/grammar.y"
    {
			int sym=xlsym_search("");
			yyval.code = xlcg_load_str(xlsym_get_xlstr(sym));
	    }
    break;

  case 9:
#line 184 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_operator(XLOP_ADD,yyvsp[-4].code,yyvsp[-1].code);
	    }
    break;

  case 10:
#line 188 "../../../src/libxl/grammar.y"
    {
		yyval.code = yyvsp[-4].code;
		if( lex_include_file(xls_get(yyvsp[-1].text),0) < 0 ) {
			YYABORT;
		}
	    }
    break;

  case 11:
#line 195 "../../../src/libxl/grammar.y"
    {
		yyval.code = yyvsp[-5].code;
		lex_include_file(xls_get(yyvsp[-2].text),1);
	    }
    break;

  case 12:
#line 200 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_operator(XLOP_ADD,yyvsp[-3].code,yyvsp[-1].code);
	    }
    break;

  case 13:
#line 204 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_operator(XLOP_ADD,yyvsp[-1].code,xlcg_load_str(yyvsp[0].text));
	    }
    break;

  case 14:
#line 208 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlc_join(yyvsp[-3].code,yyvsp[-1].code);
	    }
    break;

  case 15:
#line 215 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlc_new();
	    }
    break;

  case 16:
#line 219 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlc_join(yyvsp[-2].code,yyvsp[-1].code);
	    }
    break;

  case 17:
#line 223 "../../../src/libxl/grammar.y"
    {
		yyval.code = yyvsp[-3].code;
		if( lex_include_file(xls_get(yyvsp[-1].text),0) < 0 )
		    YYABORT;
	    }
    break;

  case 18:
#line 229 "../../../src/libxl/grammar.y"
    {
		yyval.code = yyvsp[-4].code;
		lex_include_file(xls_get(yyvsp[-2].text),1);
	    }
    break;

  case 19:
#line 237 "../../../src/libxl/grammar.y"
    { lex_if_push( 0 ); frame_begin(); }
    break;

  case 20:
#line 239 "../../../src/libxl/grammar.y"
    { lex_if_pop();  }
    break;

  case 21:
#line 241 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_frame(yyvsp[-2].code);
		frame_finish();
	    }
    break;

  case 22:
#line 248 "../../../src/libxl/grammar.y"
    {
		lex_if_push( 1 );
	    }
    break;

  case 23:
#line 253 "../../../src/libxl/grammar.y"
    {
		lex_if_pop();
	    }
    break;

  case 25:
#line 259 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_flush_after_expr(yyvsp[0].code);
	    }
    break;

  case 26:
#line 270 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_flush_after_expr(
			xlcg_call(yyvsp[-1].code,yyvsp[0].e_list.code,yyvsp[0].e_list.count)
		    );
	    }
    break;

  case 27:
#line 276 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_echo_expr_n(yyvsp[0].e_list.code,yyvsp[0].e_list.count);
	    }
    break;

  case 28:
#line 280 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_do_if(yyvsp[-4].code,yyvsp[-1].code);
	    }
    break;

  case 29:
#line 284 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_if_else(yyvsp[-6].code,yyvsp[-3].code,yyvsp[0].code);
	    }
    break;

  case 30:
#line 288 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_if(yyvsp[-4].code,yyvsp[-1].code);
		lex_if_pop();
	    }
    break;

  case 31:
#line 293 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_while(yyvsp[-2].code,yyvsp[0].code);
	    }
    break;

  case 32:
#line 297 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_do_while(yyvsp[-4].code,yyvsp[-1].code);
	    }
    break;

  case 33:
#line 301 "../../../src/libxl/grammar.y"
    { frame_begin(); }
    break;

  case 34:
#line 303 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_frame(xlcg_for(yyvsp[-6].code,yyvsp[-4].code,yyvsp[-2].code,yyvsp[0].code));
		frame_finish();
	    }
    break;

  case 35:
#line 308 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_return(yyvsp[0].code);
	    }
    break;

  case 36:
#line 312 "../../../src/libxl/grammar.y"
    { ++lex_switch; frame_begin(); }
    break;

  case 37:
#line 314 "../../../src/libxl/grammar.y"
    {
		frame_finish();
		yyval.code = xlcg_switch(yyvsp[-5].code,yyvsp[-1].code);
		--lex_switch;
	    }
    break;

  case 38:
#line 320 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_with(yyvsp[-2].code,yyvsp[0].code);
	    }
    break;

  case 39:
#line 324 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_make_global(yyvsp[0].code);
	    }
    break;

  case 41:
#line 329 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlc_new();
		}
    break;

  case 42:
#line 333 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlc_new();
	    }
    break;

  case 43:
#line 339 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlc_new();
	    }
    break;

  case 44:
#line 343 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_if_else(yyvsp[-4].code,yyvsp[-2].code,yyvsp[0].code);
	    }
    break;

  case 45:
#line 347 "../../../src/libxl/grammar.y"
    {
		yyval.code = yyvsp[-1].code;
	    }
    break;

  case 46:
#line 353 "../../../src/libxl/grammar.y"
    { yyval.code = yyvsp[0].code; }
    break;

  case 47:
#line 354 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_EQ,xlcg_case(),yyvsp[0].code); }
    break;

  case 48:
#line 355 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_EQ,xlcg_case(),yyvsp[0].code); }
    break;

  case 49:
#line 356 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_GREATER,xlcg_case(),yyvsp[0].code); }
    break;

  case 50:
#line 357 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_LOWER,xlcg_case(),yyvsp[0].code); }
    break;

  case 51:
#line 358 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_GREATER_EQUAL,xlcg_case(),yyvsp[0].code); }
    break;

  case 52:
#line 359 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_LOWER_EQUAL,xlcg_case(),yyvsp[0].code); }
    break;

  case 53:
#line 360 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_IN,xlcg_case(),yyvsp[0].code); }
    break;

  case 54:
#line 401 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_ASSIGN_ADD,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 55:
#line 402 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_ASSIGN_SUB,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 56:
#line 403 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_ASSIGN_MUL,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 57:
#line 404 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_ASSIGN_DIV,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 58:
#line 405 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_ASSIGN_CHK,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 59:
#line 406 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_ASSIGN,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 60:
#line 407 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_choose(yyvsp[-4].code,yyvsp[-2].code,yyvsp[0].code);	}
    break;

  case 61:
#line 408 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_choose(yyvsp[-3].code,NULL,yyvsp[0].code); }
    break;

  case 62:
#line 410 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_ewhile(yyvsp[-2].code,yyvsp[0].code);	}
    break;

  case 63:
#line 412 "../../../src/libxl/grammar.y"
    {
		yyval.code = xlcg_efor(yyvsp[-6].code,yyvsp[-4].code,yyvsp[-2].code,yyvsp[0].code);
	    }
    break;

  case 65:
#line 418 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_L_OR,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 67:
#line 422 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_L_AND,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 69:
#line 427 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_EQ,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 70:
#line 428 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_NEQ,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 71:
#line 429 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_GREATER,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 72:
#line 430 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_LOWER,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 73:
#line 431 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_GREATER_EQUAL,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 74:
#line 432 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_LOWER_EQUAL,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 75:
#line 433 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_IN,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 77:
#line 438 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_ADD,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 78:
#line 439 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_SUB,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 79:
#line 440 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_B_AND,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 80:
#line 441 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_B_OR,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 82:
#line 446 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_MUL,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 83:
#line 447 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_DIV,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 84:
#line 448 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_MOD,yyvsp[-2].code,yyvsp[0].code); }
    break;

  case 86:
#line 453 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_L_NOT,NULL,yyvsp[0].code); }
    break;

  case 87:
#line 454 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_B_NOT,NULL,yyvsp[0].code); }
    break;

  case 88:
#line 455 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_SUB,NULL,yyvsp[0].code); }
    break;

  case 89:
#line 456 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_PLUSPLUS,NULL,yyvsp[0].code); }
    break;

  case 90:
#line 457 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_MINUSMINUS,NULL,yyvsp[0].code); }
    break;

  case 91:
#line 458 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_copy_object(yyvsp[0].code); }
    break;

  case 92:
#line 462 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_float(yyvsp[0].real); }
    break;

  case 93:
#line 463 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_str(yyvsp[0].text); }
    break;

  case 94:
#line 464 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_integer(yyvsp[0].integer); }
    break;

  case 95:
#line 465 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_char(yyvsp[0].integer); }
    break;

  case 96:
#line 466 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_str(xls_new("")); }
    break;

  case 97:
#line 467 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_integer(0); }
    break;

  case 98:
#line 468 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_float(0.0); }
    break;

  case 99:
#line 469 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_char('\0'); }
    break;

  case 100:
#line 472 "../../../src/libxl/grammar.y"
    { yyval.code = yyvsp[0].code; }
    break;

  case 101:
#line 473 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_new_object(yyvsp[0].text); }
    break;

  case 102:
#line 474 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_copy_object(NULL); }
    break;

  case 103:
#line 479 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_create_map(yyvsp[-1].code); }
    break;

  case 105:
#line 485 "../../../src/libxl/grammar.y"
    {	yyval.code = xlcg_join_mapping(yyvsp[-2].code,yyvsp[0].code);	 }
    break;

  case 106:
#line 489 "../../../src/libxl/grammar.y"
    {	yyval.code = xlcg_create_mapping(yyvsp[-2].text,yyvsp[0].code); }
    break;

  case 110:
#line 500 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_delete_var(yyvsp[0].text);  }
    break;

  case 111:
#line 502 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_delete_index(yyvsp[-3].code,yyvsp[-1].code); }
    break;

  case 112:
#line 504 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_delete_member(yyvsp[-2].code,yyvsp[0].text); }
    break;

  case 115:
#line 510 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_var(xl_true,yyvsp[0].text); }
    break;

  case 116:
#line 511 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_var(xl_false,yyvsp[0].text); }
    break;

  case 117:
#line 512 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_case(); }
    break;

  case 118:
#line 514 "../../../src/libxl/grammar.y"
    { lex_if_push( 0 ); }
    break;

  case 119:
#line 516 "../../../src/libxl/grammar.y"
    { lex_if_pop(); }
    break;

  case 120:
#line 517 "../../../src/libxl/grammar.y"
    { yyval.code = yyvsp[-2].code; }
    break;

  case 121:
#line 519 "../../../src/libxl/grammar.y"
    { yyval.code = yyvsp[-1].code; }
    break;

  case 122:
#line 520 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_PLUSPLUS,yyvsp[-1].code,NULL); }
    break;

  case 123:
#line 521 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_MINUSMINUS,yyvsp[-1].code,NULL); }
    break;

  case 124:
#line 522 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_call(yyvsp[-3].code,yyvsp[-1].e_list.code,yyvsp[-1].e_list.count); }
    break;

  case 125:
#line 523 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_operator(XLOP_INDEX,yyvsp[-3].code,yyvsp[-1].code); }
    break;

  case 126:
#line 524 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_var_rec(yyvsp[-2].code,yyvsp[0].text); }
    break;

  case 128:
#line 526 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_builtin(yyvsp[0].text,0); }
    break;

  case 129:
#line 527 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_load_builtin(yyvsp[-1].text,1); }
    break;

  case 131:
#line 534 "../../../src/libxl/grammar.y"
    { 
			xl_code body = xlcg_frame(yyvsp[0].code);
			frame_finish(); /* finish arguments frame */

			yyval.code = xlcg_create_func(yyvsp[-2].text,body, & (yyvsp[-1].n_list) ); 
	    }
    break;

  case 132:
#line 541 "../../../src/libxl/grammar.y"
    { 
		frame_begin();
	    }
    break;

  case 133:
#line 545 "../../../src/libxl/grammar.y"
    { 
			xl_code body = xlcg_frame(xlcg_return(yyvsp[-1].code));
			frame_finish(); /* finish internal function frame */
			frame_finish(); /* finish arguments frame */

			yyval.code = xlcg_create_func(yyvsp[-6].text,body, & (yyvsp[-5].n_list)); 
	    }
    break;

  case 134:
#line 555 "../../../src/libxl/grammar.y"
    {
		frame_begin();
		lex_if_push( 0 ); 
	     }
    break;

  case 135:
#line 560 "../../../src/libxl/grammar.y"
    { lex_if_pop(); }
    break;

  case 136:
#line 562 "../../../src/libxl/grammar.y"
    { 
			xl_code body = xlcg_frame(xlcg_return(yyvsp[-2].code));
			frame_finish(); /* finish internal function frame */
			frame_finish(); /* finish arguments frame */
			yyval.code = xlcg_create_func(yyvsp[-7].text,body , & (yyvsp[-6].n_list)); 
	    }
    break;

  case 137:
#line 570 "../../../src/libxl/grammar.y"
    {	yyval.text = NULL;	}
    break;

  case 138:
#line 571 "../../../src/libxl/grammar.y"
    {	yyval.text = yyvsp[0].text;	}
    break;

  case 139:
#line 574 "../../../src/libxl/grammar.y"
    {
		frame_begin();
		frame_add_params(& yyvsp[-1].n_list);
		frame_cur()->size = yyvsp[-1].n_list.count + xl_first_frame_in_functio_offset;
		frame_cur()->bound = xl_true;
		yyval.n_list = yyvsp[-1].n_list;
		}
    break;

  case 140:
#line 584 "../../../src/libxl/grammar.y"
    { 
										if( yyvsp[0].sym->size == 1 )
											yyval.code = xlcg_load_sym(yyvsp[0].sym);
										else {
											yyerror("array not expected here !");
											YYABORT;
										}
									}
    break;

  case 141:
#line 592 "../../../src/libxl/grammar.y"
    {
										if( yyvsp[0].sym->actual_size == 1)
											yyval.code = xlcg_load_sym(yyvsp[0].sym);
										else {
											yyerror("array dimesions doesn't fit indexing");
											YYABORT;
										}
									}
    break;

  case 142:
#line 603 "../../../src/libxl/grammar.y"
    { yyval.sym = yyvsp[-3].sym; xlsm_index_begin(yyval.sym,yyvsp[-1].code); }
    break;

  case 143:
#line 605 "../../../src/libxl/grammar.y"
    { yyval.sym = yyvsp[-3].sym; 
				if( xlsm_index_cont(yyval.sym,yyvsp[-1].code) < 0 ) {
					yyerror("array dimesions doesn't fit indexing");
					YYABORT;
				}
			}
    break;

  case 144:
#line 614 "../../../src/libxl/grammar.y"
    { yyval.sym = frame_add_sym(yyvsp[-1].text); }
    break;

  case 145:
#line 615 "../../../src/libxl/grammar.y"
    { yyval.sym = yyvsp[-3].sym; xlsm_add_dim(yyval.sym, yyvsp[-1].integer); }
    break;

  case 146:
#line 620 "../../../src/libxl/grammar.y"
    { yyval.code = xlcg_dereference(yyvsp[0].code); }
    break;

  case 147:
#line 621 "../../../src/libxl/grammar.y"
    { yyval.code = yyvsp[0].code; }
    break;

  case 148:
#line 626 "../../../src/libxl/grammar.y"
    {
		yyval.e_list.code = yyvsp[0].code;
		yyval.e_list.count = 1;
	    }
    break;

  case 149:
#line 631 "../../../src/libxl/grammar.y"
    {
		yyval.e_list.code = xlc_join (yyvsp[-2].e_list.code,yyvsp[0].code);
		yyval.e_list.count = yyvsp[-2].e_list.count + 1;
	    }
    break;

  case 150:
#line 639 "../../../src/libxl/grammar.y"
    {
		yyval.e_list.code = xlc_new();
		yyval.e_list.count = 0;
	    }
    break;

  case 151:
#line 644 "../../../src/libxl/grammar.y"
    {
		yyval.e_list = yyvsp[0].e_list;
	    }
    break;

  case 152:
#line 651 "../../../src/libxl/grammar.y"
    {
		yyval.e_list.code = yyvsp[0].code;
		yyval.e_list.count = 1;
	    }
    break;

  case 153:
#line 656 "../../../src/libxl/grammar.y"
    {
		yyval.e_list.code = xlc_join (yyvsp[-2].code,yyvsp[0].e_list.code);
		yyval.e_list.count = yyvsp[0].e_list.count + 1;
	    }
    break;

  case 154:
#line 664 "../../../src/libxl/grammar.y"
    {
		yyval.n_list.names = slist_new();
		yyval.n_list.values = slist_new();
		yyval.n_list.count = 0;
	    }
    break;

  case 155:
#line 670 "../../../src/libxl/grammar.y"
    { yyval.n_list = yyvsp[0].n_list; }
    break;

  case 156:
#line 674 "../../../src/libxl/grammar.y"
    { yyval.text = yyvsp[0].sym->name; }
    break;

  case 157:
#line 675 "../../../src/libxl/grammar.y"
    { yyval.text = yyvsp[0].sym->name; }
    break;

  case 158:
#line 676 "../../../src/libxl/grammar.y"
    { yyval.text = yyvsp[0].text; }
    break;

  case 159:
#line 680 "../../../src/libxl/grammar.y"
    { 
		yyval.n_list.names = slist_new();
		yyval.n_list.values = slist_new();
		yyval.n_list.count = 1;
		slist_add(yyval.n_list.names,(void*)yyvsp[0].text);
		slist_add(yyval.n_list.values,(void*)xlcg_load_nil());
	    }
    break;

  case 160:
#line 688 "../../../src/libxl/grammar.y"
    {
		yyval.n_list.names = slist_new();
		yyval.n_list.values = slist_new();
		yyval.n_list.count = 1;
		slist_add(yyval.n_list.names,(void*)yyvsp[-2].text);
		slist_add(yyval.n_list.values,(void*)yyvsp[0].code);
	    }
    break;

  case 161:
#line 696 "../../../src/libxl/grammar.y"
    {
		if( !yyvsp[0].n_list.names ) {
		    yyval.n_list.names = slist_new();
		    yyval.n_list.count = 0;
		} else
		    yyval.n_list = yyvsp[0].n_list;
		slist_add(yyval.n_list.names,(void*)yyvsp[-2].text);
		slist_add(yyval.n_list.values,xlcg_load_nil());
		yyval.n_list.count++;
	    }
    break;

  case 162:
#line 707 "../../../src/libxl/grammar.y"
    {
		if( ! yyvsp[0].n_list.names ) {
		    yyval.n_list.names = slist_new();
		    yyval.n_list.count = 0;
		} else
		    yyval.n_list = yyvsp[0].n_list;
		slist_add(yyval.n_list.names,(void*)yyvsp[-4].text);
		slist_add(yyval.n_list.values,(void*)yyvsp[-2].code);
		yyval.n_list.count++;
	    }
    break;


    }

/* Line 999 of yacc.c.  */
#line 2536 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 719 "../../../src/libxl/grammar.y"


