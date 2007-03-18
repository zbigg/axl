/*
* Header:	xltree.c
*
* Id:           $Id: xltree.c 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	code tree used by compiler
		implementation
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
/*
    TODO:
	*
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>
#include "codegen.h"

#include "parse.h"
#if !defined END_TEXT || !defined BEGIN_TEXT
#include "y.tab.h"
#endif
#include "arch.h"
#include "tools.h"

#include "zcompat/zprintf.h"
#include "axl_i.h"
#include "xltree.h"

xltree	xltree_new(int size)
{
    xltree t;
    t = (xltree)axl_calloc(1, sizeof(xltree_t));
    return xltree_init(t,size);
}
void	xltree_free(xltree t)
{
    if( t ) {
	xltree_done(t);
	axl_free(t);
    }
}

xltree	xltree_init(xltree t,int size)
{
    t->sub = 0;
    t->sub_capacity = 0;
    t->sub_count = 0;
    t->generate = 0;
    t->optimize = 0;
    xltree_resize(t,size);
    return t;
}
void	xltree_done(xltree t)
{
    int i;
    for( i = 0 ; i < t->sub_count; i++ )
	if( t->sub[i] )
	    xltree_free(t->sub[i]);
    t->sub_count = 0;
    xltree_resize(t,0);
}

/** Minimalize memory usage.
    No recursion.
*/
void	xltree_compact(xltree t)
{
    if( t->sub_capacity > t->sub_count )
	xltree_resize(t,t->sub_count);
}

/** Minimalize memory usage.
    Recursive
*/
void	xltree_compact_r(xltree t)
{
    int i;
    for( i = 0 ; i < t->sub_count; i++ )
	if( t->sub[i] )
	    xltree_compact_r(t->sub[i]);
    xltree_compact(t);
}

/** Resize dependencies buffer.

    When growing
	- set all newly created pointers to NULL
    When shrinking
	- free al sub-trees
*/
void	xltree_resize(xltree t,int new_size)
{
    int a,b,i;
    xltree* bt;
    if( !t || t->sub_capacity == new_size) return;

    a = new_size;
    b = t->sub_capacity;

    if( a < b ) {
	for( i = a; i < b; i++ )
	    if( t->sub[i] )
		xltree_free(t->sub[i]);
    }
    bt = realloc(t->sub,sizeof(xltree) * new_size);
    if( !bt ) {
	return;
    }
    t->sub = bt;
    t->sub_capacity = new_size;
    if( a > b ) {
	for( i = b; i < a; i++ )
	    t->sub[i] = NULL;
    }
}

int	xltree_add(xltree to,xltree what)
{
    return xltree_add_end(to,what);
}
int	xltree_add_end(xltree t,xltree what)
{
    int pos;
    if( t->sub_count == t->sub_capacity ) {
	xltree_resize(t,t->sub_capacity+2);
	if( t->sub_count == t->sub_capacity )
	    return -1;
    }
    pos = t->sub_count;
    t->sub[pos] = what;
    ++(t->sub_count);
    return pos;
}

int	xltree_ins_front(xltree to,xltree what)
{
    return xltree_ins(to,what,0);
}
int 	xltree_ins(xltree t,xltree what,int pos)
{
    if( t->sub_count == t->sub_capacity ) {
	xltree_resize(t,t->sub_capacity+2);
	if( t->sub_count == t->sub_capacity )
	    return -1;
    }
    if( pos == t->sub_count )
	return xltree_add_end(t,what);

    memmove(t->sub+pos+1,t->sub+pos, sizeof(xltree) * ( t->sub_count - pos ) );

    t->sub[pos] = what;

    return pos;
}

xltree 	xltree_del(xltree t,int pos)
{
    xltree st;
    st = t->sub[pos];

    memmove(t->sub+pos,t->sub+pos+1, sizeof(xltree) * ( t->sub_count - pos - 1) );

    --(t->sub_count);

    t->sub[t->sub_count] = 0;

    return st;
}

xltree	xltree_bi_op(xltree_desc* desc,xltree left_expr,xltree right_expr)
{
    return xltree_instr_2(desc,left_expr,right_expr);
}
xltree	xltree_un_op(xltree_desc* desc,xltree expr)
{
    return xltree_instr_1(desc,expr);
}
xltree	xltree_instr_1(xltree_desc* desc,xltree expr1)
{
    xltree t = xltree_instr_n(desc,1);
    t->sub[0] = expr1;
    t->sub_count = 1;
    return t;
}
xltree	xltree_instr_2(xltree_desc* desc,xltree expr1,xltree expr2)
{
    xltree t = xltree_instr_n(desc,2);
    t->sub[0] = expr1;
    t->sub[1] = expr2;
    t->sub_count = 2;
    return t;
}
xltree	xltree_instr_3(xltree_desc* desc,xltree expr1,xltree expr2,xltree expr3)
{
    xltree t = xltree_instr_n(desc,3);
    t->sub[0] = expr1;
    t->sub[1] = expr2;
    t->sub[2] = expr3;
    t->sub_count = 3;
    return t;
}
xltree	xltree_instr_4(xltree_desc* desc,xltree expr1,xltree expr2,xltree expr3,xltree expr4)
{
    xltree t = xltree_instr_n(desc,3);
    t->sub[0] = expr1;
    t->sub[1] = expr2;
    t->sub[2] = expr3;
    t->sub[3] = expr3;
    t->sub_count = 4;
    return t;
}
xltree	xltree_instr_n(xltree_desc* desc,int n)
{
    xltree t = xltree_new(n);
    t->generate = desc->generate;
    t->optimize = desc->optimize;
    return t;
}
xltree	xltree_instr_vn(xltree_desc* desc,int n,...)
{
    va_list ap;
    xltree t;
    int i;
    va_start(ap,n);

    t = xltree_instr_n(desc,n);
    for( i = 0 ; i < n ; i++ )
	t->sub[i] = va_arg(ap,xltree);
    va_end(ap);
    return t;
}
