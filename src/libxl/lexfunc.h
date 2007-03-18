/*
* Header: lexfunc.h
*
* Id:           $Id: lexfunc.h 1.5 04/02/24 01:39:33+01:00 zbigg@marl.(none) $
*
* Project:	axl
*
* Description:	yylex helpers
*
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __lexfunc_h_
#define __lexfunc_h_

#include "zcompat/zpio.h"
#include "zcompat/ztools.h"
/* C Extern Identifiers. */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * IO
 */

extern	/* error.c */	ZFILE*	lex_error_stream;

int	lex_getc(void);
void	lex_ungetc(int c);

/** Ignore chars until delimeter is found. */
int	lex_ignore(char delimeter);

/** Read to sstr until delimter is found, without parsing stray expressions.
*/
int	lex_read_to_sstr(sstr* s,char delimeter);

/** Read to sstr until delimter is found, parsing stray expressions.
*/
int	lex_read_to_sstr_stray(sstr* s,char delimeter);

/** Read to sstr until one char from delimter is found, parsing stray expressions.
*/
int	lex_read_cspn_to_sstr_stray(sstr* s,char* delimeter);

void	lex_free(void);
void	lex_reset(void);

/*
 * Operators and keywords lists
 */
void	init_operator_and_keywords_table(void);
void	done_operator_and_keywords_table(void);
int*	operator_id(char first,char second);
int*	keyword_id(const char* keyw);


#ifdef __cplusplus
}
#endif

#endif /* force single include of file */
