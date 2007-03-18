/*
* File: error.c
*
* Id:           $Id: lexerr.c 1.3 03/11/27 21:36:25+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	error reporting tool
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "lexfunc.h"
#include "parse.h"
#include "zcompat/zpio.h"
#if !defined END_TEXT || !defined BEGIN_TEXT
#include "grammar.h"
#endif
#include "tools.h"

#include "axl_i.h"

int	there_were_errors = 0;

ZSTREAM	lex_error_stream = NULL;
extern /* lex.c */ char token_buf[];
extern /* lex.c */ int	last_token;

void	yyerror(const char* errname)
{
    static char buf[300];
    strcpy(buf,"syntax error: ");
    strcat(buf,errname);
    switch ( last_token ) {
    case TOK_INTEGER:
    case TOK_REAL:
	strcat(buf," before number");
	break;
    case TOK_STRING:
	strcat(buf," before character constant");
	break;
    default:
	strcat(buf," before '%s'");
    }
    lex_error(LEX_ERR_SYNTAX,buf,token_buf);
}
