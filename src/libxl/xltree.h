/*
* Header:	xltree.h
*
* Id:           $Id: xltree.h 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	code tree used by compiler
*
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2002,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __xltree_h_
#define __xltree_h_

#define INC_XLTREE

#ifndef INC_XLANG
#include "axl/axl.h"
#endif

struct _xltree;
typedef struct _xltree* xltree;
typedef struct _xltree  xltree_t;

struct _xltree {
    /** */

    xltree* sub;
    int	    sub_count;
    int	    sub_capacity;

    xl_code (*generate)(xltree tree);
    int	    (*optimize)(xltree tree);

    /** Type of expression eg. EXPR_IMM | EXPR_VOID | EXPR_VAL */
    int	    expr_type;
    int	    ip1;
    int	    ip2;
    int	    pp1;
    int	    pp2;
    char*   sp1;
    char*   sp2;
};

typedef struct _xltree_desc {
    xl_code (*generate)(xltree tree);
    int	    (*optimize)(xltree tree);
    char    name[32];
} xltree_desc;


/* C extern Identifiers. */
#ifdef __cplusplus
extern "C" {
#endif

xltree	xltree_new(int size);
void	xltree_free(xltree t);

xltree	xltree_init(xltree t,int size);
void	xltree_done(xltree t);

/** Minimalize memory usage.
    No recursion.
*/
void	xltree_compact(xltree t);

/** Minimalize memory usage.
    Recursive
*/
void	xltree_compact_r(xltree t);

/** Resize dependencies buffer.

    When growing
	- set all newly created pointers to NULL
    When shrinking
	- free al sub-trees
*/
void	xltree_resize(xltree t,int new_size);


int	xltree_add(xltree to,xltree what);
int	xltree_add_end(xltree to,xltree what);
int	xltree_ins_front(xltree to,xltree what);

int	xltree_ins(xltree to,xltree what,int pos);
xltree 	xltree_del(xltree t,int pos);

xltree	xltree_bi_op(xltree_desc* desc,xltree left_expr,xltree right_expr);
xltree	xltree_un_op(xltree_desc* desc,xltree expr);
xltree	xltree_instr_1(xltree_desc* desc,xltree expr1);
xltree	xltree_instr_2(xltree_desc* desc,xltree expr1,xltree expr2);
xltree	xltree_instr_3(xltree_desc* desc,xltree expr1,xltree expr2,xltree expr3);
xltree	xltree_instr_4(xltree_desc* desc,xltree expr1,xltree expr2,xltree expr3,xltree expr4);
xltree	xltree_instr_n(xltree_desc* desc,int n);
xltree	xltree_instr_vn(xltree_desc* desc,int n,...);

/* C extern Identifiers. */
#ifdef __cplusplus
}
#endif

#endif /* force single include of file */
