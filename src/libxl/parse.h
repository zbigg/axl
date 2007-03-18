/*
* Header: parse.h
*
* Id:           $Id: parse.h 1.4 04/02/24 01:39:33+01:00 zbigg@marl.(none) $
*
* Project: axl
*
* Description: definitions for parser: grammar.y
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __parse_h_
#define __parse_h_

#include "axl/axl.h"
#include "xltree.h"


extern	xl_code	xl_code_global;

extern	xltree xltree_root;

typedef struct xl_frame_t {
    int		size;
    xlmap*	id;
    xl_bool	bound;
} xl_frame_t;

typedef struct _xlp_name_list {
	slist*	names;
	slist*	values;
	int	count;
} xlp_name_list;


typedef struct _xlp_expr_list {
	xl_code code;
	int		count;
} xlp_expr_list;
/*
    begin a new frame 
*/
xl_frame_t* frame_cur();
int     frame_pos();
int     frame_size();
xl_bool frame_begin();
void    frame_finish();
/*int	frame_get_global_sym(xl_str name);*/
/* int	frame_get_sym(xl_str name);*/
/*int	frame_add_sym(xl_str name);*/
void    frame_destroy(void);
void    frame_add_params(xlp_name_list* nl);

xl_symbol* frame_get_sym(xl_str name);
xl_symbol* frame_get_sym_str(const char* name);
xl_symbol* frame_get_global_sym(xl_str name);
xl_symbol* frame_add_sym(xl_str name);

extern const int xl_first_frame_in_functio_offset;

#include "codegen.h"
#include "lex.h"
#include "treegen.h"
#include "axl_i.h"

/*
 *  Symbol able functions. (symtab.c)
 */
const char*     xlsym_get_str(int id);
xl_str          xlsym_get_xlstr(int id);

int             xlsym_search(const char* x);
int             xlsym_searchx(const xl_str_t* s);

void            xlsym_reset();

void            xlsym_free();

#endif /* force single include of file */
