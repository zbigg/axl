/*
* Header:	treegen.h
*
* Id:           $Id: treegen.h 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	tree implementation for specific codes
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

#ifndef __treegen_h_
#define __treegen_h_

#define INC_TREEGEN

#ifndef INC_XLANG
#include "axl/axl.h"
#endif

#ifndef INC_XLTREE
#include "xltree.h"
#endif
/*
typedef struct _xlp_expr_list {
	xltree code;
	int		count;
} xlp_expr_list;
*/


extern xltree_desc xltd_arith_op;
extern xltree_desc xltd_while;
extern xltree_desc xltd_for;
extern xltree_desc xltd_if;
extern xltree_desc xltd_else;
extern xltree_desc xltd_block;
extern xltree_desc xltd_echo;

/* C extern Identifiers. */
#ifdef __cplusplus
extern "C" {
#endif

/** Link statements in series, so
    execution will be serial.
*/
xltree	xltg_serial();

/** Link statements, so it will
    be executed parallely.
*/
xltree	xltree_parallel();

/** Create binary operator expression tree.

    Opcode is defined by one of XLOP_xxx constants.

    Currently suppeorted operators:
	* none
*/
xltree	xltg_bi_op  (int opcode,xltree left,xltree right);

/** Create unary operator expression tree.

    Opcode is defined by one of XLOP_xxx constants.

    Currently suppeorted operators:
	* none
*/
xltree	xltg_un_op  (int opcode,xltree expr);

/**
*/
xltree	xltg_choose (xltree cond, xltree expr_true, xltree expr_false);
/**
*/
xltree	xltg_if	    (xltree cond, xltree body);
/**
*/
xltree	xltg_if_else(xltree cond, xltree body_true, xltree body_false);
/**
*/
xltree	xltg_while  (xltree cond, xltree body);
/**
*/
xltree	xltg_ewhile  (xltree cond, xltree body);
/**
*/
xltree	xltg_do_while(xltree body, xltree cond);
/**
*/
xltree	xltg_for(xltree init_expr,xltree cond,xltree end,xltree body);
/**
*/
xltree	xltg_efor(xltree init_expr,xltree cond,xltree end,xltree body);
/**
*/

xltree	xltg_switch(xltree case_expr,xltree body);
/**
*/
xltree	xltg_with(xltree obj,xltree body);
/**
*/
xltree	xltg_return(xltree expr);
/**
*/
xltree	xltg_deref(xltree expr);
/**
*/
xltree	xltg_call(xltree func,xltree params);
/**
*/
xltree	xltg_call_method(xltree object,xltree method,xltree params);
/**
*/
xltree	xltg_create_func(xltree body,xltree params);
/**
*/
xltree	xltg_make_instruction(xltree expr);
/**
*/
xltree	xltg_echo(xltree expr);
/**
*/
xltree	xltg_load_nil();
xltree	xltg_load_int(xl_integer i);
xltree  xltg_load_char(int c);
xltree	xltg_load_float(xl_float);
xltree	xltg_load_str(const char* str,int str_flags /* XLTGS_xxx */);
xltree	xltg_load_var(const char* name,int str_flags /* XLTGS_xxx */);
xltree	xltg_load_builtin(const char* name,int str_flags /* XLTGS_xxx */,int safe);
xltree	xltg_load(xl_value v);
xltree	xltg_case();
xltree	xltg_make_global(xltree expr);
xltree	xltg_make_instruction(xltree expr);
xltree	xltg_copy_object(xltree obj);
xltree	xltg_member(xltree obj,const char* name,int str_flags /* XLTGS_xxx */);

xltree	xltg_delete_var(const char* name,int str_flags /* XLTGS_xxx */);
xltree	xltg_delete_member(xltree obj,const char* name,int str_flags /* XLTGS_xxx */);
xltree	xltg_delete_index(xltree obj,xltree index_expr);

/* C extern Identifiers. */
#ifdef __cplusplus
}
#endif

#define xltg_load_integer(I) xltg_load_int(I)

/** XLTGS_SUP_STR
    Dup string, caller is responsible
    for deallocating string.
*/
#define XLTGS_DUP_STR	    0

/** XLTGS_ACQUIRE_STR
    Remember pointer and use this, specified memory
    location.
    Location should be allocated by
    malloc()/calloc()/realloc().
    After use string will be deallocated by
    library by free().
*/
#define XLTGS_ACQUIRE_STR   1

#endif /* force single include of file */
