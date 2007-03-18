/*
* Header:	codegen.h
*
* Id:           $Id: codegen.h 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	code generation routines
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

#ifndef __codegen_h_
#define __codegen_h_

#include "axl/axl.h"
#include "treegen.h"


/* C Extern Identifiers. */
#ifdef __cplusplus
extern "C" {
#endif

/** Create a code for a given operator.
*/
extern xl_code	xlcg_operator(
    int op,
    xl_code left,
    xl_code right);
/** Create a code for 'if' statement.
*/
extern xl_code	xlcg_if(
    xl_code cond,
    xl_code code);
/** Create a code for 'if else' statement.
*/
extern xl_code	xlcg_if_else(
    xl_code cond,
    xl_code if_true,
    xl_code if_false);

/** Create a code for ' ? : ' operator.
*/
extern xl_code	xlcg_choose(
    xl_code cond,
    xl_code if_true,
    xl_code if_false);
/** Create a code for 'while' statement.
*/
extern xl_code	xlcg_while(
    xl_code cond,
    xl_code loop);

/** Create a code for 'while' statement.
*/
extern xl_code	xlcg_ewhile(
    xl_code cond,
    xl_code loop);
/** Create a code for 'do while' statement.
*/
extern xl_code	xlcg_do_while(
    xl_code loop,
    xl_code cond);
/** Create a code for 'for' statement.
*/
extern xl_code	xlcg_for(
    xl_code start,
    xl_code cond,
    xl_code end,
    xl_code code);
/** Create a code for 'efor' statement.
*/
extern xl_code	xlcg_efor(
    xl_code start,
    xl_code cond,
    xl_code end,
    xl_code code);

/** Create a code for 'do ..if' statement.
*/
extern xl_code	xlcg_do_if(
    xl_code code,
    xl_code cond);

/** Create a code for 'switch' statement.
*/
extern xl_code	xlcg_switch(
    xl_code case_code,xl_code switch_body);

/** Create a code for 'return' statement.
*/
extern xl_code	xlcg_return(
    xl_code return_value);

/** Create a code for call function statement.
*/
extern xl_code	xlcg_call(
    xl_code func_ref,
    xl_code call_param,
    int     param_count);

/** Create a code for method call statement.
*/
extern xl_code	xlcg_call_method(
    xl_code obj_ref,
    xl_code func_ref,
    xl_code call_param,
    int     param_count);

/** Flush tmp stack after expression.
*/
extern xl_code	xlcg_flush_after_expr(
    xl_code expr);


/** Create code for echo expression.
*/
extern xl_code	xlcg_echo_expr(
    xl_code expr);

/** Create code for echo n expressions.
*/
extern xl_code	xlcg_echo_expr_n(
    xl_code expr,
    int n);

extern xl_code xlcg_load_nil(void);

/** Create code for integer load.
*/
extern xl_code	xlcg_load_integer(xl_integer i);

/** Create code for character load.
*/
extern xl_code	xlcg_load_char(int i);

/** Create code for float load.
*/
extern xl_code	xlcg_load_float(xl_float f);

/** Create code for reference load.
*/
extern xl_code	xlcg_load_reference(xl_float f);


xl_code xlcg_load_sym(xl_symbol* sym);

/** Create code for variable load.
*/
#define XLCG_SCOPE_GLOBAL	((const char*) -1)
#define XLCG_SCOPE_CURRENT	((const char*)	0)
extern xl_code	xlcg_load_var(xl_bool define,xl_str name);

/** Create code for member/method load.
*/
extern xl_code	xlcg_load_var_rec(xl_code rec,xl_str name);

/** Create code for string load.
*/
extern xl_code	xlcg_load_str(xl_str s);

/** Create code for unknown value.
*/
extern xl_code	xlcg_load(xl_value v);

/** Create code for set load.
*/
extern xl_code xlcg_load_set_imm(xl_code code,int number);
/** Create code for range load.
*/
#define XLCG_R_OPEN	    1
#define XLCG_R_LEFTOPEN     2
#define XLCG_R_RIGHTOPEN    3
#define XLCG_R_CLOSED	    4

extern xl_code xlcg_load_range_imm(xl_code left,xl_code right,int how);

extern xl_code	xlcg_case(void);

extern xl_code	xlcg_exit(xl_code left,xl_code result);

extern xl_code	xlcg_create_func(xl_str name,xl_code body,xlp_name_list* names);

extern xl_code	xlcg_copy_object(xl_code obj);
extern xl_code	xlcg_new_object(xl_str name);
extern xl_code	xlcg_load_builtin(xl_str name,int safe);

extern xl_code	xlcg_dereference(xl_code expr);

extern xl_code	xlcg_with(xl_code expr,xl_code instr);

extern xl_code	xlcg_make_global(xl_code expr);

extern xl_code	xlcg_delete_index(xl_code obj, xl_code index);
extern xl_code	xlcg_delete_member(xl_code obj, xl_str);
extern xl_code	xlcg_delete_var(xl_str name);

extern xl_code xlcg_frame(xl_code code);

extern xl_code	xlcg_join_mapping(xl_code first, xl_code next);
extern xl_code	xlcg_create_mapping(xl_str name, xl_code expr);
extern xl_code  xlcg_create_map(xl_code mapping);

#ifdef __cplusplus
}
#endif

#endif /* force single include of file */
