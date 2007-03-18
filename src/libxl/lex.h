/*
* Header: lex.h
*
* Id:           $Id: lex.h 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	lex definitions
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

#ifndef __lex_h_
#define __lex_h_

#include "axl/axl.h"

#define lex_input_stream (lex.ctx->input_stream)
#define lex_filename	(lex.ctx->filename)
#define lex_line_number (lex.ctx->line_number)

#define lex_val(i) (lex_switch_table[(i)])

#define LEX_MAX_INCLUDES 15
#define LEX_IF_ELSE	11
#define LEX_SWITCH	10
#define LEX_LAST	20

#define lex_if_else	lex_val(LEX_IF_ELSE)
#define lex_switch	lex_val(LEX_SWITCH)

/* C Extern Identifiers. */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef XL_LEX_PRIV_INPUT_BUFFER
#define XL_LEX_PRIV_INPUT_BUFFER	1024
#endif
struct _lex_ctx_t {
    char	filename[Z_FILE_MAX];
    char	path[Z_PATH_MAX];
    ZSTREAM	input_stream;
    ZSTREAM	input_stream_org;
    int 	input_stream_owner;
    int 	line_number;
#if XL_LEX_PRIV_INPUT_BUFFER > 0
/** lex private input buffer */
    char	ibuf[XL_LEX_PRIV_INPUT_BUFFER];
/** set to 0 if data must be reread */
    int 	ibuf_valid;

/** size of data in buffer */
    int 	ibuf_size;
/** current position in buffer */
    int 	ibuf_pos;
#endif
};
typedef struct _lex_ctx_t lex_ctx_t;

struct	_lex_data_t {
    lex_ctx_t*	ctx;
    int 	ctx_number;
/** lex error output stream */
    ZSTREAM	error_stream;
/** lex debug output stream */
    ZSTREAM	debug_stream;
    lex_ctx_t	ctx_t[LEX_MAX_INCLUDES];
};
typedef struct _lex_data_t lex_data_t;

extern lex_data_t	lex;

int		lex_init(void);
int		lex_set_error_stream(ZSTREAM s);
int		lex_set_debug_stream(ZSTREAM s);
int		lex_set_input_stream(ZSTREAM s);
/** Nie zmienia katalogu biezacego
*/
int		lex_include_stream(ZSTREAM s);
int		lex_include_file(const char* name,int safe);

extern int	lex_switch_table[LEX_LAST+1];
extern int	there_were_errors; /* deprecated */
extern int	lex_errors;	/* = 0; */
extern int	lex_warnings;	/* = 0; */

/* Type of context --------v				*/
#define C_DOCUMENT	0 /* document			*/
#define C_CODE		1 /* code			*/
#define C_INTCODE	2 /* code inside the document	*/

void		lex_context_push(int i);
int		lex_context_pop(void);

extern void	lex_if_push(int);
extern void	lex_if_pop(void);
extern int	lex_if_cur(void);


int		sstr_add_stray_expr_len(sstr* s,int len);
int		sstr_add_stray_expr(sstr* s);

extern void	yyerror(const char* errname);
extern int	yyparse(void);
extern int	yylex(void);

extern int	c_context;
extern char	linebuf[1024];	/* contains text of currently processed line */
extern char	token_buf[1024];	/* contains body of current token */
extern int	last_token;/* code of last token */

void		lex_error(int code,const char* fmt,...);
void		lex_warning(int code,const char* fmt,...);

#define LEX_ERR_SYNTAX	1
#define LEX_ERR_STRAY	2
#define LEX_ERR_NUMBER	3
#define LEX_ERR_INCLUDE 4

#define LEX_WARN_TOO_LONG	1
#ifdef __cplusplus
}
#endif

#endif /* force single include of file */
