/*
* File:		codegen.c
*
* Id:           $Id: codegen.c 1.6 04/02/24 01:39:33+01:00 zbigg@marl.(none) $
*
* Project:	axl
*
* Description: assembler code generation for AXL virtual machine
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


#include "parse.h"
#include "codegen.h"
#include "arch.h"
#include "tools.h"

#include "zcompat/zprintf.h"

#include "axl_i.h"

#define xecho printf

xl_code 	xlcg_infinite_loop(xl_code code);
int		get_next_label(void);
const char*	get_label_text(int i);


static char gnl_b[20] = "aaaa";
/*
static char gnl_o[26] = "abcdefghijklmnopqrstuvwyxz"
*/
static char gnl_o[26] = "ngtcjpwohudkqymfxbivlearsz";
static int  gnl_i = 0;

int get_next_label(void)
{
    return gnl_i++;
}
const char* get_label_text(int i)
{
    int  b=0;
    gnl_b[b++] = '$';
    if( i == 0 ) {
	gnl_b[b++] = gnl_o[0];
    } else while( i > 0 ) {
	gnl_b[b++] = gnl_o[i % 26];
	i = i / 26;
    }
    gnl_b[b] = 0;
    return gnl_b;
}

/** Create a code for a given operator.
*/
xl_code xlcg_operator(int op,xl_code left,xl_code right)
{
    int a;
    int imm = 0;
    int rimm = 1,limm = 1;
    xl_value_t n;
    xlv_clear(&n);
    {
	if( left != NULL )
	    limm = (left->rt == xl_rt_imm);
	if( right != NULL )
	    rimm = (right->rt == xl_rt_imm);
	imm = limm && rimm;
    }
    switch( op ) {
/* MATH operations */
    case XLOP_ADD:
	if( imm ) {
	    xlv_add(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else {
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_ADD);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_SUB:
	if( left == NULL ) {
	    if( imm ) {
		xlv_chs(&n,right->res);
		xlc_free(right);
		return xlcg_load(&n);
	    } else {
		xlc_add_instr_int(right,INSTR_STD_OPER_UN, XLOP_CHS);
		return right;
		xlc_set_res(right,xl_rt_val,0);
	    }
	}
	if( imm ) {
	    xlv_sub(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else {
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_SUB);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_MUL:
	if( imm ) {
	    xlv_mul(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else {
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_MUL);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_DIV:
	if( imm ) {
	    xlv_div(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_DIV);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_MOD:
	if( imm ) {
	    xlv_mod(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_MOD);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;

/* BINARY operations */
    case XLOP_B_AND:
	if( imm ) {
	    xlv_b_and(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_B_AND);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_B_OR:
	if( imm ) {
	    xlv_b_or(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_B_OR);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_B_XOR:
	if( imm ) {
	    xlv_b_xor(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_B_XOR);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_B_NOT:
	if( imm ) {
	    xlv_b_not(&n,right->res);
	    xlc_free(right);
	    return xlcg_load(&n);
	} else {
	    xlc_add_instr_int(right,INSTR_STD_OPER_UN, XLOP_B_NOT);
	    xlc_set_res(right,xl_rt_val,0);
	    return right;
	}
	break;
/* LOGICAL operations */
    case XLOP_L_AND:
	if( imm ) {
	    xlv_l_and(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else	{
/******       ALGO	 ******\
      R 	0->1
    dup +1	1->2
    fls :a	2->1
    drp 	1->0
      L 	0->1
:a
    bool	1->1
\********	       ********/
	    int a = get_next_label();
	    xlc_add_instr_str(left,INSTR_FALSE | XLI_LEAVE_1,get_label_text(a));
	    xlc_add_instr_int(left,INSTR_DROP,1);
	    left = xlc_join(left,right);
	    xlc_add_label(left,get_label_text(a));
	    xlc_add_instr_nil(left,INSTR_BOOL);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_L_OR:
	if( imm ) {
	    xlv_l_or(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else	{
/******       ALGO	 ******\
      R 	0->1
    dup +1	1->2
    true :a	2->1
    drp 	1->0
      L 	0->1
:a
    bool	1->1
\********	       ********/
	    int a = get_next_label();
	    xlc_add_instr_str(left,INSTR_TRUE | XLI_LEAVE_1,get_label_text(a));
	    xlc_add_instr_int(left,INSTR_DROP,1);
	    left = xlc_join(left,right);
	    xlc_add_label(left,get_label_text(a));
	    xlc_add_instr_nil(left,INSTR_BOOL);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_L_XOR:
	if( imm ) {
	    xlv_l_xor(&n,left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load(&n);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI, XLOP_L_XOR);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_L_NOT:
	if( imm ) {
	    xlv_l_neg(&n,right->res);
	    xlc_free(right);
	    return xlcg_load(&n);
	} else	{
	    xlc_add_instr_int(right,INSTR_STD_OPER_UN, XLOP_L_NOT);
	    xlc_set_res(right,xl_rt_val,0);
	    return right;
	}

/* ASSIGN operations */
    case XLOP_ASSIGN:
	left = xlc_join(left,right);
	xlc_add_instr_nil(left,INSTR_STORE);
	xlc_set_res(left,xl_rt_ref,0);
	break;
    case XLOP_ASSIGN_ADD:
	left = xlc_join(left,right);
	xlc_add_instr_int(left,INSTR_STD_OPER_BI | XLI_MODIFY_1, XLOP_ADD);
	xlc_set_res(left,xl_rt_ref,0);
	break;
    case XLOP_ASSIGN_SUB:
	left = xlc_join(left,right);
	xlc_add_instr_int(left,INSTR_STD_OPER_BI | XLI_MODIFY_1, XLOP_SUB);
	xlc_set_res(left,xl_rt_ref,0);
	break;
    case XLOP_ASSIGN_MUL:
	left = xlc_join(left,right);
	xlc_add_instr_int(left,INSTR_STD_OPER_BI | XLI_MODIFY_1, XLOP_MUL);
	xlc_set_res(left,xl_rt_ref,0);
	break;
    case XLOP_ASSIGN_DIV:
	left = xlc_join(left,right);
	xlc_add_instr_int(left,INSTR_STD_OPER_BI | XLI_MODIFY_1, XLOP_DIV);
	xlc_set_res(left,xl_rt_ref,0);
	break;
    case XLOP_ASSIGN_CHK:
/******       ALGO	 ******\
	dup left
	valid
	true

\********	       ********/
	a = get_next_label();
	xlc_add_instr_int(left,INSTR_DUP,1);
	xlc_add_instr_nil(left,INSTR_VALID);
	xlc_add_instr_str(left,INSTR_TRUE,get_label_text(a));
	left = xlc_join(left,right);
	xlc_add_instr_nil(left,INSTR_STORE);
	xlc_add_label(left,get_label_text(a));
	xlc_set_res(left,xl_rt_ref,0);
	break;
    case XLOP_EQ:
	if( imm ) {
	    int n = xlv_cmp(left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load_integer(n == 0);
	} else {
	    left = xlc_join(left,right);
	    xlc_add_instr_nil(left,INSTR_EQUAL);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_NEQ:
	if( imm ) {
	    int n = xlv_cmp(left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load_integer(n != 0);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_nil(left,INSTR_NOT_EQUAL);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_GREATER: {
	if( imm ) {
	    int n = xlv_cmp(left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load_integer(n > 0);
	} else	    left = xlc_join(left,right);
	    xlc_add_instr_nil(left,INSTR_GREATER);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_LOWER:
	if( imm ) {
	    int n = xlv_cmp(left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load_integer(n < 0);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_nil(left,INSTR_LESS);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_GREATER_EQUAL:
	if( imm ) {
	    int n = xlv_cmp(left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load_integer(n >= 0);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_nil(left,INSTR_GREATER_EQUAL);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
    case XLOP_LOWER_EQUAL:
	if( imm ) {
	    int n = xlv_cmp(left->res,right->res);
	    xlc_free(left);xlc_free(right);
	    return xlcg_load_integer(n <= 0);
	} else	{
	    left = xlc_join(left,right);
	    xlc_add_instr_nil(left,INSTR_LESS_EQUAL);
	    xlc_set_res(left,xl_rt_val,0);
	}
	break;
/* OTHER operations */
    case XLOP_INDEX:
	left = xlc_join(left,right);
	xlc_add_instr_nil(left,INSTR_INDEX);
	xlc_set_res(left,xl_rt_ref,0);
	break;
    case XLOP_PLUSPLUS:
	if( left ) {	/* postfix ++ : x++ */
	    if( imm )
		lex_error(0,"++(postfix) reference required");
	    lex_warning(0,"++(postfix) operator not implemented, acts as prefix");
	    xlc_add_instr_int(left,INSTR_LOAD_VAL,1);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI | XLI_MODIFY_1, XLOP_ADD );
	    xlc_set_res(left,xl_rt_ref,0);
	    return left;
	} else {	/* prefix ++ : ++x */
	    if( imm )
		lex_error(0,"++(postfix) reference required");
	    xlc_add_instr_int(right,INSTR_LOAD_VAL,1);
	    xlc_add_instr_int(right,INSTR_STD_OPER_BI | XLI_MODIFY_1, XLOP_ADD );
	    return right;
	}
	break;
    case XLOP_MINUSMINUS:
	if( left ) {	/* postfix -- : x-- */
	    if( imm )
		lex_error(0,"++(postfix) reference required");
	    lex_warning(0,"--(postfix) operator not implemented, acts as prefix");
	    xlc_add_instr_int(left,INSTR_LOAD_VAL,1);
	    xlc_add_instr_int(left,INSTR_STD_OPER_BI | XLI_MODIFY_1, XLOP_SUB );
	    return left;
	} else {	/* prefix -- : --x */
	    if( imm )
		lex_error(0,"++(postfix) reference required");
	    xlc_add_instr_int(right,INSTR_LOAD_VAL,1);
	    xlc_add_instr_int(right,INSTR_STD_OPER_BI | XLI_MODIFY_1, XLOP_SUB );
	    return right;
	}
	break;
    }
    return left;
}
/** Create a code for 'if' statement.
*/
xl_code xlcg_if(xl_code cond,xl_code code)
{
    if( cond->rt == xl_rt_imm ) /* condition is immediate */
	if( xlv_true(cond->res) ) {
	    xlc_free(cond);
	    return code;
	} else {
	    xlc_free(cond);
	    xlc_free(code);
	    return xlc_new();
	}
    else {
/******       ALGO	 ******\
	%if not $cond jmp a
	    %code
	:a
\****** 		 ******/
	int a = get_next_label();

	xlc_add_instr_str(cond,INSTR_FALSE,get_label_text(a));

	cond = xlc_join(cond,code);

	xlc_add_label(cond,get_label_text(a));
	xlc_set_res(cond,xl_rt_void,0);
	return cond;
    }
}

/** Create a code for 'if else' statement.
*/
xl_code xlcg_if_else(xl_code cond,xl_code if_true,xl_code if_false)
{
    if( cond->rt == xl_rt_imm ) /* condition is immediate */
	if( xlv_true(cond->res) ) {
	    xlc_free(cond);
	    xlc_free(if_false);
	    return if_true;
	} else {
	    xlc_free(cond);
	    xlc_free(if_true);
	    return if_false;
	}
    else {
/******       ALGO	 ******\
	%if not $cond jmp a
	    %if_true
	    %jmp b
	:a
	    %if_false
	:b
\****** 		 ******/
	int a = get_next_label();
	int b = get_next_label();

	xlc_add_instr_str(cond,INSTR_FALSE,get_label_text(a));

	cond = xlc_join(cond,if_true);

	xlc_add_instr_str(cond,INSTR_JMP,get_label_text(b));

	xlc_add_label(cond,get_label_text(a));

	cond = xlc_join(cond,if_false);

	xlc_add_label(cond,get_label_text(b));
	xlc_set_res(cond,xl_rt_void,0);

	return cond;
    }
}

/** Create a code for ' ? : ' operator.
*/
xl_code xlcg_choose(xl_code cond,xl_code if_true,xl_code if_false)
{
    if( cond->rt == xl_rt_imm ) /* condition is immediate */
	if( xlv_true(cond->res) ) {
	    xlc_free(cond);
	    xlc_free(if_false);
	    return if_true ? if_true : xlc_new();
	} else {
	    xlc_free(cond);
	    if( if_true )
		xlc_free(if_true);
	    return if_false;
	}
    else {
	if( if_true != NULL ) {
/******       ALGO	 ******\
	%if not $cond jmp a
	    $if_true
	    %jmp b
	:a
	    $if_false
	:b
*/
	    int a = get_next_label();
	    int b = get_next_label();

	    xlc_add_instr_str(cond,INSTR_FALSE,get_label_text(a));

	    cond = xlc_join(cond,if_true);

	    xlc_add_instr_str(cond,INSTR_JMP,get_label_text(b));

	    xlc_add_label(cond,get_label_text(a));

	    cond = xlc_join(cond,if_false);

	    xlc_add_label(cond,get_label_text(b));

	    xlc_set_res(cond,xl_rt_val,1);
	    return cond;
	} else {
/******       ALGO	 ******\
	$cond
	%dup
	%if not > jmp a
	    %jmp b
	:a
	    %drop
	    $if_false
	:b


\********	       ********/
	    int a = get_next_label();
	    xlc_add_instr_int(cond,INSTR_DUP,1);
	    xlc_add_instr_str(cond,INSTR_TRUE,get_label_text(a));
	    xlc_add_instr_int(cond,INSTR_DROP,1);
	    cond = xlc_join  (cond,if_false);
	    xlc_add_label(cond,get_label_text(a));
	    xlc_set_res(cond,xl_rt_val,1);
	    return cond;
    	}
    }
}

/** Create a code for 'while' statement.
*/
xl_code xlcg_while(xl_code cond,xl_code loop)
{
    if( cond->rt == xl_rt_imm )
	if( xlv_true(cond->res) ) { /* condition is always TRUE */
	    xlc_free(cond);
	    return xlcg_infinite_loop(loop);
	} else {		/* condition is always FALSE */
	    lex_warning(0,"while  condition always false");
	    xlc_free(cond);
	    xlc_free(loop);
	    return xlc_new();
	}
    else {
/******       ALGO	 ******\
	:a
	%if not $cond jmp b
	    %code
	    %jmp a
	:b
\********	       ********/
	int a = get_next_label();
	int b = get_next_label();

	xl_code x;
	x = xlc_new();
	xlc_add_label(x,get_label_text(a));

	cond = xlc_join(x,cond);

	xlc_add_instr_str(cond,INSTR_FALSE,get_label_text(b));

	cond = xlc_join(cond,loop);

	xlc_add_instr_str(cond,INSTR_JMP,get_label_text(a));

	xlc_add_label(cond,get_label_text(b));
	xlc_set_res(cond,xl_rt_void,0);
	return cond;
    }
}

xl_code xlcg_ewhile(xl_code cond,xl_code loop)
{
    xl_code c,x;
    xlc_add_instr_int(loop,INSTR_STD_OPER_BI,XLOP_ADD);
    c = xlcg_while(cond,loop);
    x = xlc_new();
    xlc_add_instr_nil(x,INSTR_LOAD_VAL);
    return xlc_join(x,c);
}

/** Create a code for 'do while' statement.
*/
xl_code xlcg_do_while(xl_code loop,xl_code cond)
{
    if( cond->rt == xl_rt_imm )
	if( xlv_true(cond->res) ) { /* condition is always TRUE */
	    xlc_free(cond);
	    return xlcg_infinite_loop(loop);
	} else {	/* condition is always FALSE, we have one run of loop */
	    lex_warning(0,"do while condition always false");
	    xlc_free(cond);
	    return loop;
	}
    else {
/******       ALGO	 ******\
	:a
	    %code
	%if $cond jmp a
\********	       ********/
	int a = get_next_label();
	xl_code x = xlc_new();

	xlc_add_label(x,get_label_text(a));

	loop = xlc_join(x,loop);

	loop = xlc_join(loop,cond);

	xlc_add_instr_str(loop,INSTR_TRUE,get_label_text(a));

	xlc_set_res(loop,xl_rt_void,0);
	return loop;
    }
}
/** Create a code for 'for' statement.
*/
xl_code xlcg_for(
    xl_code start,
    xl_code cond,
    xl_code end,
    xl_code code)
{
    if( cond->rt == xl_rt_imm )
	if( xlv_true(cond->res) ) {	/* infinite loop of start+ (code+end) */
	    xlc_free(cond);
	    return xlc_join(
		start,
		xlcg_infinite_loop(
		    xlc_join(
			code,
			end)
		    )
		);
	} else {/* always false */
	    lex_warning(0,"for condition always false");
	    xlc_free(cond);
	    xlc_free(end);
	    xlc_free(code);
	    return start;
	}
    else {
/******       ALGO	 ******\
	%start
	:a
	%if not $cond jmp b
	    %code
	    %end
	    %jmp a
	:b
\********	       ********/
	int a = get_next_label();
	int b = get_next_label();
	xl_code x = start;
	xlc_add_label(x,get_label_text(a));

	x = xlc_join(x,cond);
	xlc_add_instr_str(x,INSTR_FALSE,get_label_text(b));

	x = xlc_join(x,code);
	x = xlc_join(x,end);

	xlc_add_instr_str(x,INSTR_JMP,get_label_text(a));

	xlc_add_label(x,get_label_text(b));
	xlc_set_res(x,xl_rt_void,0);
	return x;
    }
}

/** Create a code for 'efor' statement.
*/
xl_code xlcg_efor(
    xl_code start,
    xl_code cond,
    xl_code end,
    xl_code code)
{
    xlc_add_instr_int(code,INSTR_STD_OPER_BI,XLOP_ADD);
    xlc_add_instr_nil(start,INSTR_LOAD_VAL);
    return xlcg_for(start,cond,end,code);
}

/** Create a code for 'do ..if' statement.
*/
xl_code xlcg_do_if(xl_code code,xl_code cond)
{
    if( cond->rt == xl_rt_imm )
	if( xlv_true(cond->res) ) {
	    xlc_free(cond);
	    return code;
	} else {
	    xlc_free(cond);
	    xlc_free(code);
	    return xlc_new();
	}
    else {
/******       ALGO	 ******\
	%if $cond jmp a
	    %code
	:a
\********	       ********/
	int a = get_next_label();

	xlc_add_instr_str(cond,INSTR_FALSE,get_label_text(a));

	cond = xlc_join(cond,code);

	xlc_add_label(cond,get_label_text(a));
	xlc_set_res(cond,xl_rt_void,0);

	return cond;
    }
}

/** Create a code for 'switch' statement.
*/
xl_code xlcg_switch(
    xl_code case_code,
    xl_code switch_body)
{
/******       ALGO	 ******\
	$case_code
	<r_case
	%switch_body
	>r_case
\********	       ********/
    xl_code x = xlc_new();
    xlc_add_instr_int(x,INSTR_LOAD_REG,REG_CASE);

    x = xlc_join(x,case_code);

    xlc_add_instr_int(x,INSTR_STORE_REG,REG_CASE);

    x = xlc_join(x,switch_body);

    xlc_add_instr_int(x, INSTR_STORE_REG , REG_CASE);

    xlc_set_res(x,xl_rt_void,0);
    return x;
}

/** Create a code for 'return' statement.
*/
xl_code xlcg_return(xl_code return_value)
{
    if( return_value )
	if( return_value->rt != xl_rt_imm )
	    xlc_add_instr_nil(return_value,INSTR_DEREF);
    xlc_add_instr_nil(return_value,INSTR_RET);
    xlc_set_res(return_value,xl_rt_void,0);
    return return_value;
}
/** Create a code for call function statement.
*/
xl_code xlcg_call(xl_code ref,xl_code call_param,int param_count)
{
/******       ALGO	 ******\
	$ func_ref		func
	$ call_param		func (params )
	$call	param_count
	drop param_count + 1
	$ r_return
\********	       ********/
    ref = xlc_join(ref,call_param);
    xlc_add_instr_int(ref,INSTR_CALL,	       param_count);
    xlc_set_res(ref,xl_rt_ref,1);
    return ref;
}

extern xl_code	xlcg_create_func(xl_str name,xl_code body,xlp_name_list* names)
{
/******       ALGO	 ******\
	jmp :a
	:entry_point
	    %code
	    $0
	    %ret
	:a
\********	       ********/
    xl_code	x = xlc_new();
    int 	a = get_next_label(),
		ep = get_next_label();

    xlc_add_instr_str(x,INSTR_JMP,get_label_text(a));	/* jmp :a */
    xlc_add_label(x,get_label_text(ep));		/* :entry_point */

/*    {
	sli_t  param_i, param_end;
	slist_begin(names->names,&param_i);
	slist_end(names->names,&param_end);
	sli_inc( & param_end );

	for ( ; ! sli_eq( &param_i, &param_end); sli_inc( &param_i) ) {
	    xl_str param_name = sli_get( &param_i );
	    xls_free(param_name);
	}
    }
*/
    slist_delete(names->names);

    if( name ) {
        xl_str a = xls_add(xls_ref(name),"_entry_point");
        xlc_add_label(x,xls_get(a));
        xls_free(a);
    }
    x = xlc_join(x,body);

    xlc_add_instr_int(x,INSTR_LOAD_VAL,0);		/* $0 */
/*    xlc_add_instr_int(x,INSTR_STORE_REG,REG_RETURN);*//* <0 RET */
    xlc_add_instr_nil(x,INSTR_RET);			/* ret */
    xlc_add_label(x,get_label_text(a)); 		/* :a */
    
    if( name ) 
        x = xlc_join(x, xlcg_load_var(xl_true, xls_ref(name)));
    /* Load all default parameters.*/
    {
	sli_t  param_i, param_end;
	slist_begin(names->values,&param_i);
	slist_end(names->values,&param_end);
	sli_inc( & param_end );

	for ( ; ! sli_eq( &param_i, &param_end); sli_inc( &param_i) ) {
	    xl_code def_param_code = (xl_code) sli_get( &param_i );
	    x = xlc_join(x,def_param_code);
	}
    }
    slist_delete(names->values);
    if( name )
        xlc_add_label(x,xls_get(name));
        
    /* Push label number and parameter info of function as returned value.
    */
    {   
	xl_value_t v;
	v.type = XLV_FUNC_LABEL;
	v.flags = 0;
	v.v.func.address.label = ep;
	v.v.func.params = xlv_fp_new( names->count );
	xlc_add_instr(x,INSTR_LOAD_VAL,&v);
    }
    if( name )
        xlc_add_instr_nil(x,INSTR_STORE);
   
    xlc_set_res(x,name ? xl_rt_ref : xl_rt_imm,1);
    /*if( name ) xls_free(name);*/
    return x;
}

/** Flush tmp stack after expression.
*/
xl_code xlcg_flush_after_expr(xl_code expr)
{
    if( expr->rt == xl_rt_imm ) {
	xlc_free(expr);
	return xlc_new();
    }
    xlc_add_instr_int(expr,INSTR_DROP,1);
    xlc_set_res(expr,xl_rt_void,0);
    return expr;
}

/** Create code for echo expression.
*/
xl_code xlcg_echo_expr(xl_code expr)
{
    xlc_add_instr_int(expr,INSTR_ECHO,1);
    xlc_set_res(expr,xl_rt_void,0);
    return expr;
}

extern xl_code	xlcg_echo_expr_n(
    xl_code expr,
    int n)
{
    xlc_add_instr_int(expr,INSTR_ECHO,n);
    xlc_set_res(expr,xl_rt_void,0);
    return expr;
}

/** Create code for integer load.
*/
xl_code xlcg_load_integer(xl_integer i)
{
    xl_code a = xlc_new();
    xlc_add_instr_int(a,INSTR_LOAD_VAL,i);
    xlc_set_res(a,xl_rt_imm,1);
    return a;
}

/** Create code for character load.
*/
xl_code xlcg_load_char(int c)
{
    xl_value_t x;
    xl_code a = xlc_new();
    x.type = XLV_CHAR;
    x.v.ch = c;
    x.flags = 0;
    xlc_add_instr(a,INSTR_LOAD_VAL,&x);
    xlc_set_res(a,xl_rt_imm,1);
    return a;
}

xl_code xlcg_load_nil(void)
{
    xl_code a = xlc_new();
    xlc_add_instr_nil(a,INSTR_LOAD_VAL);
    xlc_set_res(a,xl_rt_imm,1);
    return a;
}

/** Create code for float load.
*/
xl_code xlcg_load_float(xl_float f)
{
    xl_code a = xlc_new();
    xlc_add_instr_float(a,INSTR_LOAD_VAL,f);
    xlc_set_res(a,xl_rt_imm,1);
    return a;
}

/** Create code for variable load.
*/
xl_code xlcg_load_sym(xl_symbol* sym)
{
    xl_code a;
    int code = -1;
    int param;
    if( sym->base == 0 && sym->size == 1) {  /* an frame based symbol REG_PP+sym_offset*/
        code = INSTR_PARAM;
        param = sym->pos;
        a = xlc_new();
    } else if( sym->base == 1 && sym->size == 1) {
        code = INSTR_PARAM;
        param = -sym->pos-1;
        a = xlc_new();
    } else if( sym->dimc > 0 ) { /* local array */
        /*int pos = sym->base ==1 ? sym->pos+1 : sym->pos;*/
        xl_code a = xlcg_load_integer(sym->pos);
        xl_code c = xlsm_index_detach_code(sym);
        xl_code x = xlcg_operator(XLOP_ADD,a,c);
        if( sym->base == 1 ) 
            xlc_add_instr_int(x,INSTR_STD_OPER_UN,XLOP_CHS);
        xlc_add_instr_nil(x, INSTR_PARAM);
        return x;
    }
    if (code >= 0 ) {
        xlc_add_instr_int(a, code, param);
        return a;
    } else {
        xlc_free(a);
        return NULL;
    }
}
xl_code xlcg_load_var(xl_bool define,xl_str name)
{
    xl_code a;
    
    xl_symbol* sym_n = frame_get_sym(name);
    if( !sym_n && define ) {
	sym_n = frame_add_sym(name);
    }
    if( sym_n ) 
	a = xlcg_load_sym(sym_n);
    else {
        a = xlc_new();
	xlc_add_instr_strx(a,INSTR_LOAD_VAR,name);
    }
    /*xls_free(name);*/
    xlc_set_res(a,xl_rt_ref,1);
    return a;
}

xl_code xlcg_load_builtin(xl_str name,int safe)
{
    xl_str namex;
    xl_code a = xlc_new();
    namex = xlsprintf(NULL,"%s:%s",safe ? "safebuiltin" : "builtin", xls_get(name));
    /*xls_free(name);*/
    xlc_add_instr_strx(a,INSTR_LOAD_VAR,namex);
    xls_free(namex);

    xlc_set_res(a,xl_rt_ref,1);

    return a;
}

xl_code xlcg_load_var_rec(xl_code rec,xl_str name)
{
    xlc_add_instr_strx(rec,INSTR_FIELD,name);
    /*xls_free(name);*/
    return rec;
}

/** Create code for string load.
*/
xl_code xlcg_load_str(xl_str s)
{
    xl_code a = xlc_new();
    xlc_add_instr_strx(a,INSTR_LOAD_VAL,s);
    xlc_set_res(a,xl_rt_imm,1); 

    /*xls_free(s);*/
    return a;
}

xl_code xlcg_case(void)
{
    xl_code a = xlc_new();
    xlc_add_instr_int(a,INSTR_LOAD_REG,REG_CASE);
    xlc_set_res(a,xl_rt_reg,0);
    return a;
}

xl_code xlcg_load_set_imm(xl_code code,int number)
{
    err_f("<set> code generation not implemented");
    return NULL;
}
/** Create code for range load.
*/
/*
#define XLCG_R_OPEN	    1
#define XLCG_R_LEFTOPEN     2
#define XLCG_R_RIGHTOPEN    3
#define XLCG_R_CLOSED	    4
*/

xl_code xlcg_load_range_imm(xl_code left,xl_code right,int how)
{
    err_f("<range> code generation not implemented");
    return NULL;
}

xl_code xlcg_exit(xl_code left,xl_code result)
{
    if( result )
	left = xlc_join(left,result);
    xlc_add_instr_int(left,INSTR_EXIT,0);
    xlc_set_res(left,xl_rt_void,0);
    return left;
}


xl_code xlcg_copy_object(xl_code obj)
{
    if( obj == NULL ) {
	obj = xlc_new();
	xlc_add_instr_int(obj,INSTR_COPY,0);
    } else {
	xlc_add_instr_int(obj,INSTR_COPY,1);
    }
    return obj;
}

xl_code	xlcg_new_object(xl_str str)
{
    xl_code obj;
    obj = xlc_new();
    xlc_add_instr_strx(obj,INSTR_COPY,str);
    /*xls_free(str);*/
    return obj;
}

xl_code xlcg_dereference(xl_code expr)
{
    if( expr->rt != xl_rt_imm )
	xlc_add_instr_nil(expr,INSTR_DEREF);
    return expr;
}

xl_code xlcg_with(xl_code expr,xl_code instr)
{
    xlc_add_instr_int(expr,INSTR_ENTER_CTX,0);
    expr = xlc_join(expr,instr);
    xlc_add_instr_int(expr,INSTR_LEAVE_CTX,0);
    xlc_set_res(expr,xl_rt_void,0);
    return expr;
}

xl_code xlcg_make_global(xl_code expr)
{
    xlc_add_instr_int(expr,INSTR_ENTER_CTX,1);
    xlc_set_res(expr,xl_rt_void,0);
    return expr;
}

xl_code xlcg_load(xl_value v)
{
    xl_code a = xlc_new();
    xlc_add_instr(a,INSTR_LOAD_VAL,v);
    xlc_set_res(a,xl_rt_imm,1);
    return a;
}

xl_code xlcg_infinite_loop(xl_code code)
{
/******       ALGO	 ******
	:a
	    %code
	    %jmp a
\********	       ********/
    int a	= get_next_label();
    xl_code x	= xlc_new();
    xlc_add_label(x,get_label_text(a));
    x = xlc_join(x,code);
    xlc_add_instr_str(x,INSTR_JMP,get_label_text(a));
    return x;

}

xl_code	xlcg_delete_index(xl_code obj, xl_code index)
{
    obj = xlc_join(obj,index);
    xlc_add_instr_nil(obj,INSTR_FREE_VAR);
    xlc_set_res(obj,xl_rt_void,0);
    return obj;
}
xl_code	xlcg_delete_member(xl_code obj, xl_str name)
{
    xl_str x = xls_cat(NULL,".",xls_get(name));
    /*xls_free(name);*/
    xlc_add_instr_strx(obj,INSTR_FREE_VAR,x);
    xls_free(x);

    xlc_set_res(obj,xl_rt_void,0);
    return obj;
}

xl_code	xlcg_delete_var(xl_str name)
{
    xl_code i = xlc_new();
    xlc_add_instr_strx(i,INSTR_FREE_VAR,name);
    /*xls_free(name);*/

    xlc_set_res(i,xl_rt_void,0);
    return i;
}

xl_code xlcg_frame(xl_code code)
{
    int frame_size = frame_cur()->size;
    xl_code cd;

    if( frame_size == 0 ) return code;

    cd = xlcg_load_nil();
    
    if( frame_size > 1 )
        xlc_add_instr_int(cd,INSTR_DUP,frame_size-1);

    cd = xlc_join(cd,code);

    xlc_add_instr_int(cd,INSTR_DROP,frame_size);

    return cd;
}

xl_code	xlcg_join_mapping(xl_code first, xl_code next)
{
    return xlc_join(first,next);
}

xl_code	xlcg_create_mapping(xl_str name, xl_code expr)
{
    xl_code a = xlc_new();
    xlc_add_instr_int(a,INSTR_DUP,1);
    xlc_add_instr_strx(a,INSTR_FIELD, name);
    /*xls_free(name);*/
    a = xlc_join(a, expr);
    xlc_add_instr_nil(a,INSTR_STORE);
    xlc_add_instr_int(a,INSTR_DROP,1);
    return a;
}

xl_code xlcg_create_map(xl_code mapping)
{
    xl_code a = xlcg_copy_object(NULL);
    return xlc_join(a,mapping);
}
