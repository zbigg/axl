/*
* Header:	treegen.c
*
* Id:           $Id: treegen.c 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	tree implementation for specific codes
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
#include "treegen.h"

extern xltree_desc xltd_arith_op;
extern xltree_desc xltd_while;
extern xltree_desc xltd_for;
extern xltree_desc xltd_if;
extern xltree_desc xltd_else;
extern xltree_desc xltd_block;
extern xltree_desc xltd_echo;
extern xltree_desc xltd_comma;

static xl_code arith_op_generate(xltree t);
static xl_code while_generate(xltree t);
static xl_code for_generate(xltree t);
static xl_code if_generate(xltree t);
static xl_code if_else_generate(xltree t);
static xl_code block_generate(xltree t);
static xl_code echo_generate(xltree t);
static xl_code comma_generate(xltree t);

static xl_code arith_op_generate(xltree t)
{
    return NULL;
}
static xl_code while_generate(xltree t)
{
    return NULL;
}
static xl_code for_generate(xltree t)
{
    return NULL;
}
static xl_code if_generate(xltree t)
{
    return NULL;
}
static xl_code if_else_generate(xltree t)
{
    return NULL;
}
static xl_code block_generate(xltree t)
{
    return NULL;
}
static xl_code echo_generate(xltree t)
{
    return NULL;
}
static xl_code comma_generate(xltree t)
{
    return NULL;
}


xltree_desc xltd_arith_op   = { arith_op_generate, NULL , "" };
xltree_desc xltd_while	    = { while_generate, NULL, "" };
xltree_desc xltd_for	    = { for_generate, NULL, "" };
xltree_desc xltd_if	    = { if_generate, NULL, "" };
xltree_desc xltd_if_else    = { if_else_generate, NULL, "" };
xltree_desc xltd_block	    = { block_generate, NULL, "" };
xltree_desc xltd_echo	    = { echo_generate, NULL, "" };
xltree_desc xltd_comma	    = { comma_generate, NULL, "" };

xltree	xltg_serial()
{
    return xltg_new(3);
}

xltree	xltree_parallel()
{
    return xltg_serial();
}

xltree	xltg_bi_op  (int opcode,xltree left,xltree right)
{
    xltree x = xltree_bi_op(NULL,left,right);
    x->ip1 = opcode;
    return x;
}
xltree	xltg_un_op  (int opcode,xltree expr)
{
    xltree x = xltree_un_op(NULL,expr);
    x->ip1 = opcode;
    return x;

xltree	xltg_choose (xltree cond, xltree expr_true, xltree expr_false)
{
    xltree x = xltree_instr_3(NULL,cond,expr_true,expr_false);
    return x;
}

xltree	xltg_if	    (xltree cond, xltree body)
{
    return NULL;
}

xltree	xltg_if_else(xltree cond, xltree body_true, xltree body_false)
{
    return NULL;
}

xltree	xltg_while  (xltree cond, xltree body)
{
    return NULL;
}

xltree	xltg_do_while(xltree body, xltree cond)
{
    return NULL;
}

xltree	xltg_for(xltree init_expr,xltree cond,xltree end,xltree body)
{
    return NULL;
}

xltree	xltg_switch(xltree case_expr,xltree body)
{
    return NULL;
}

xltree	xltg_with(xltree obj,xltree body)
{
    return NULL;
}

xltree	xltg_return(xltree expr)
{
    return NULL;
}


xltree	xltg_deref(xltree expr)
{
    return NULL;
}


xltree	xltg_call(xltree func,xltree params)
{
    return NULL;
}


xltree	xltg_call_method(xltree object,xltree method,xltree params)
{
    return NULL;
}

/**
*/
xltree	xltg_create_func(xltree body,xlp_name_list* names)
{
    return NULL;
}

/**
*/
xltree	xltg_flush_after_expr(xltree expr)
{
    return NULL;
}

/**
*/
xltree	xltg_echo(xltree expr)
{
    return NULL;
}

/**
*/
xltree	xltg_load_nil()
{
    return NULL;
}

xltree	xltg_load_int(xl_integer i)
{
    return NULL;
}

xltree	xltg_load_float(xl_float)
{
    return NULL;
}

xltree  xltg_load_char(int c)
{
    return NULL;
}

xltree	xltg_load_str(const char* str,int str_flags /* XLTGS_xxx */)
{
    return NULL;
}

xltree	xltg_load_var(const char* name,int str_flags /* XLTGS_xxx */)
{
    return NULL;
}

xltree	xltg_load_builtin(const char* name,int str_flags /* XLTGS_xxx */)
{
    return NULL;
}

xltree	xltg_load(xl_value v)
{
    return NULL;
}

xltree	xltg_case()
{
    return NULL;
}

xltree	xltg_make_global(xltree expr)
{
    return NULL;
}

xltree	xltg_make_instruction(xltree expr)
{
    return NULL;
}

xltree	xltg_copy_object(xltree obj)
{
    return NULL;
}

xltree	xltg_member(xltree obj,const char* name,int str_flags /* XLTGS_xxx */)
{
    return NULL;
}


xltree	xltg_delete_var(const char* name,int str_flags /* XLTGS_xxx */)
{
    return NULL;
}

xltree	xltg_delete_member(xltree obj,const char* name,int str_flags /* XLTGS_xxx */)
{
    return NULL;
}

xltree	xltg_delete_index(xltree obj,xltree index_expr)
{
    return NULL;
}

