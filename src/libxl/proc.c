/*
* File:		proc.c
*
* Id:           $Id: proc.c 1.10 04/03/07 21:24:29+01:00 nikt@nowy.dom $
*
* Project:	axl
*
* Description:	procesor of virtual machine.

 Conditional defines:
     XLP_MAX_ECHO_WRITE	- maximal write size in bytes for
                        ECHO instruction
		        On windows when writing to a pipe it's not
			possible to write more than pipe size,
			which is usually about 0.5kB

			Default 500 bytes

     XLD_PROC_TRACE	- before every instruction processor shows
			 stack and what is to be done, and shows
			 position in sources
     XLD_PROC_MSG	- shows some messages from processor

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
#include <signal.h>

#include "axl/axl.h"
#include "arch.h"

#include "zcompat/zpio.h"
#include "zcompat/zprintf.h"

#include "axl_i.h"
#include "tools.h"

#ifndef XLP_MAX_ECHO_WRITE
#define XLP_MAX_ECHO_WRITE 500
#endif

#ifdef XLD_PROC_MSG
#define XLP_DEBUG 1
#endif

#if !defined(NDEBUG)
#define XLD_PROC_TRACE
#endif

#ifndef XLP_DEBUG
#define XLP_DEBUG 0
#endif

extern int xlv_active_count;
char* except_table[] = {
/* 0  */    "unknown processor exception",
/* 1  */    "invalid_instruction",
/* 2  */    "unknown label",
/* 3  */    "invalid jump",
/* 4  */    NULL,
/* 5  */    "invalid dup",
/* 6  */    "invalid drop",
/* 7  */    NULL,
/* 8  */    "invalid copy",
/* 9  */    "invalid store",
/* 10 */    "invalid register",
/* 11 */    "invalid function",
/* 12 */    "unknown import",
/* 13 */    "stack underflow",
/* 14 */    "invalid call"
};

/* valoper.c */
int xlv_oper_bi(
    xl_value dest,
    xl_value src,
    int (*f)(xl_value dest, xl_value src));
int xlv_oper_un(
    xl_value dest,
    int (*f)(xl_value dest));

int xlv_oper_bi_dup(
    xl_value dest,
    xl_value a,
    xl_value b,
    int (*f)(xl_value dest, xl_value src));

int xlv_oper_un_dup(
    xl_value dest,
    xl_value a,
    int (*f)(xl_value dest));

void*	oper_callback_table[] =
{
    (void*)NULL,
    (void*)_xlv_add, 	/*XLOP_ADD	    = 1,*/
    (void*)_xlv_sub, 	/*XLOP_SUB	    = 2,*/
    (void*)_xlv_mul, 	/*XLOP_MUL	    = 3,*/
    (void*)_xlv_div, 	/*XLOP_DIV	    = 4,*/
    (void*)_xlv_mod,	/*XLOP_MOD	    = 5,*/
    (void*)NULL,	/*XLOP_ASSIGN	    = 6,*/
    (void*)_xlv_add,	/*XLOP_ASSIGN_ADD   = 7,*/
    (void*)_xlv_sub,	/*XLOP_ASSIGN_SUB   = 8,*/
    (void*)_xlv_mul,	/*XLOP_ASSIGN_MUL   = 9,*/
    (void*)_xlv_div,	/*XLOP_ASSIGN_DIV   = 10,*/
    (void*)_xlv_b_and,	/*XLOP_ASSIGN_AND   = 11,*/
    (void*)_xlv_b_or,	/*XLOP_ASSIGN_OR    = 12,*/
    (void*)NULL,	/*XLOP_ASSIGN_SHL   = 13,*/
    (void*)NULL,	/*XLOP_ASSIGN_SHR   = 14,*/
    (void*)NULL,	/*XLOP_EQ	    = 15,*/
    (void*)NULL,	/*XLOP_NEQ	    = 16,*/
    (void*)NULL,	/*XLOP_LOWER	    = 17,*/
    (void*)NULL,	/*XLOP_GREATER      = 18,*/
    (void*)NULL,	/*XLOP_LOWER_EQUAL  = 19,*/
    (void*)NULL,	/*XLOP_GREATER_EQUAL= 20,*/
    (void*)_xlv_l_and,	/*XLOP_L_AND	    = 21,*/
    (void*)_xlv_l_or,	/*XLOP_L_OR	    = 22,*/
    (void*)_xlv_l_xor,	/*XLOP_L_XOR	    = 23,*/
    (void*)_xlv_l_not,	/*XLOP_L_NOT	    = 24,*/
    (void*)_xlv_b_and,	/*XLOP_B_AND	    = 25,*/
    (void*)_xlv_b_or,	/*XLOP_B_OR	    = 26,*/
    (void*)_xlv_b_xor,	/*XLOP_B_XOR	    = 27,*/
    (void*)_xlv_b_not,	/*XLOP_B_NOT	    = 28,*/
    (void*)NULL,	/*XLOP_SHL	    = 29,*/
    (void*)NULL,	/*XLOP_SHR	    = 30,*/
    (void*)NULL,	/*XLOP_IN	    = 31,*/
    (void*)NULL,	/*XLOP_ASSIGN_CHK   = 32,*/
    (void*)NULL,	/*XLOP_INDEX	    = 33,*/
    (void*)NULL,	/*XLOP_FIELD	    = 34,*/
    (void*)_xlv_add,	/*XLOP_PLUSPLUS     = 35,*/
    (void*)_xlv_sub,	/*XLOP_MINUSMINUS   = 36 */
    (void*)_xlv_chs	/*XLOP_CHS	    = 37 */
};
void	xlths_dump(ZSTREAM f,xl_thread thread,int max);


extern int	    xlasm_put_instr(ZSTREAM s,const xl_instr_t* instr);
extern int	    xlasm_put_value(ZSTREAM s,const xl_value_t* instr);

#define EXTRACT_AND_DEREF(var,tmp)				\
	var = xlv_need_deref( var ) ? xlv_deref(var) : var;	\
	if( xlv_need_extract( var ) ) {				\
	    if( xlv_extract(var,&tmp) < 0 ) {			\
		;						\
	    }							\
	    var = &tmp;						\
	} 


int	xlths_resize(struct xlth_stack_t* s,int new_size)
{
    int old_size = s->icapacity;
    xl_value_t* new_ptr;
    if( new_size == old_size ) return 0;
    new_ptr = axl_realloc(s->ptr, sizeof(xl_value_t)*new_size);
    if( !new_ptr )
	return -1;
    if( new_size > old_size ) {
	memset(new_ptr + s->itop, 0, new_size - s->itop);
    }
    s->ptr = new_ptr;
    s->end = new_ptr + new_size;
    s->icapacity = new_size;
    s->top = s->ptr + s->itop;
    return 0;
}

xl_value_t* xlths_add(xl_thread th,int no)
{
    register unsigned x = th->stack.itop+no;
    if( x >= th->stack.icapacity ) {
	if( xlths_resize(&th->stack,(th->stack.icapacity+1)*2) < 0 )
	    return NULL;
    } else if( x < (th->stack.icapacity / 4 ) && x > 16 ) {
	if( xlths_resize(&th->stack,th->stack.icapacity / 2) < 0 )
	    return NULL;
    }
    th->stack.itop = x;
    if( no > 0 )
	memset(th->stack.top, 0, no * sizeof (xl_value_t));
    return th->stack.top += no;
}

#define TH_REG_SP	thread->regs[REG_SP].v.i
#define TH_REG_PP	thread->regs[REG_PP].v.i
#define TH_REG_PC	thread->regs[REG_PC].v.i
#define TH_REG_CX	thread->regs[REG_CX].v.i

/*
    THIS MUST actual with INSTR_CALL/RET
*/
const int xl_first_frame_in_functio_offset = 5;

int TRACE_STACK = -1;

int	xlp_step(xl_thread thread,int howmany)
{
    xl_value	s;
    xl_value_t	tmp,tmp1,tmp2;
    xl_value	m,n;
    xl_value	v;
    xl_uint16	icode;
    xl_uint16	iflags;
    xl_instr	i;
    int 	count = howmany;
    int 	a,b;
    xlv_clear(&tmp);
    xlv_clear(&tmp1);
    xlv_clear(&tmp2);
    while( thread->run && ( count == -1 || count-- > 0 )) {
	thread->ip = thread->nip;
	/* the default value of next ip */
	thread->nip++;
	thread->ic++;
	i = &( thread->code->buf[ thread->ip ] );
	icode  = (i->icode) & 0x00ff;
	iflags = (i->icode) & 0xff00;
	s = thread->stack.top;

#if defined XLD_PROC_TRACE
        if( TRACE_STACK == -1 )
            TRACE_STACK =  xldbg_get_mask("stack");
        if(  xldbg_mask_enabled(TRACE_STACK) ) {
            if( icode != INSTR_NOP) {
                xlths_dump(xl_get_debug_stream(),thread,10);
                xldbg_print(">>>>>> before %i instr:",thread->ip);
                xlasm_put_instr(xl_get_debug_stream(),i);
            }
        }
#endif
	switch( icode ) {
	case INSTR_NOP:
	    xlthd_debug_info(thread,&i->ival);
	    break;

/********************/
/** INSTR_LOAD_REG

    Load a register value.

    Copies value from specified register into the stack.

    Paramater	: register number
    Stack	: +1

	OUTPUT <value>

    Complexity: 1
*/
	case INSTR_LOAD_REG:
	    if( i->ival.type != XLV_INT )
		xlp_except(thread,xlpe_invalid_register,"must be integer number");

	    s = xlths_add(thread,1);

	    xlv_set(s-1,& thread->regs[i->ival.v.i]);

	    break;

/********************/
/** INSTR_LOAD_VAL

    Load an immediate value.

    Loads an immediate value into the stack.
    Value from instruction is copied into the stack.
    It can be : integer, floating point or string.

    Paramater	: value
    Stack	: +1

	OUTPUT <value>

    Complexity: 1
*/
	case INSTR_LOAD_VAL:
	    switch( i->ival.type )  {
	    case XLV_FUNC_IP:
	    case XLV_FUNC_LABEL:
		if( i->ival.v.func.params != NULL ) {
		    register int 	N = i->ival.v.func.params->number;
		    register xl_value	iv = i->ival.v.func.params->def;
		    register int 	i = 1;
		    iv += N-1;
		    for(; i <= N; i++, iv--)
			xlv_mov(iv,s-i);
		    s = xlths_add(thread,-N);
		}
	    default:
		s = xlths_add(thread,1);

		xlv_set(s-1,&i->ival);

		break;
	    }
	    break;

/********************/
/** INSTR_LOAD_VAR

    Load reference to variable.

    Puts a reference to named variable visible
    in curent context.

    Paramater:
	- name of variable(string)
	- builtin function name formatted like this:
	    builtin:<module_name>:<symbol_name>

    Stack: +1

	OUTPUT <reference to variable>

    Complexity: CTX_GET
*/
	case INSTR_LOAD_VAR:
	    {
		register char* vname = xls_get(i->ival.v.s);
		xl_value parent = 0;
		v = NULL;
		if( strncmp(vname,"builtin:",8) == 0 ) {
		    s = xlths_add(thread,1);
		    if( xl_load_func_builtin(s-1,vname+8) == -1 ) {
			char* x = zsaprintf(NULL,0,"%s : %s",vname+8,xl_load_last_error());
			xlp_except(thread,xlpe_unknown_import,x);
			axl_free(x);
			xlv_clear(s-1);
		    }
		    break /* from switch instr->code */;
		} else if( strncmp(vname,"safebuiltin:",12) == 0 ) {
		    s = xlths_add(thread,1);
		    if( xl_load_func_builtin(s-1,vname+12) == -1 )
			xlv_clear(s-1);
		    break; /* from switch instr->code */
		} else if( vname[0] == '_' && vname[1] == '_' ) {
		    vname+=2;
		    xlv_clear(&tmp);
		    if( strcmp(vname,"ip") == 0 ) {
			s = xlths_add(thread,1);
			xlv_set_int(s-1,thread->ip);
			break /* from switch instr->code */;
		    } else if( strcmp(vname,"ic") == 0 ) {
			s = xlths_add(thread,1);
			xlv_set_int(s-1,thread->ic);
			break /* from switch instr->code */;
		    }
		    vname-=2;
		}
		if( v == NULL ) 
		    v = xlx_find(thread->var,vname,&parent);
		
                if( v ) {
		    /*v = xlv_deref(v);*/
                
                } else /* ( v == NULL )*/ {
		    tmp.type = XLV_NIL;
                    tmp.flags = 0;
		    v = xlx_set(thread->var,vname,&tmp,&parent);
		}

		s = xlths_add(thread,1);
		xlv_set_ref(s-1,v,parent);
	    }
 	    break /* from switch instr->code */;

/********************/
/** INSTR_INDEX

    Return reference to array element.

    Stack: -1

	INPUT <table object> <index>

	OUTPUT <reference to variable>

    Complexity:
	Should be constant, but in
	future this operation can be overloaded
	with user function.
*/
	case INSTR_INDEX:
	
	    m = s-2;
	    n = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    EXTRACT_AND_DEREF(n,tmp2);
/*
	    m = xlv_need_deref( s-2 ) ? xlv_deref(s-2) : s-2;
	    n = xlv_need_deref( s-1 ) ? xlv_deref(s-1) : s-1;
	    if( xlv_need_extract(m) ) {
		a |= xlv_extract(m,&tmp1);
		m = &tmp1;
	    }
	    if( xlv_need_extract(n) ) {
		a |= xlv_extract(n,&tmp2);
		m = &tmp2;
	    }
*/
	    /* Retrieve value reference to indexed value */
	    a = xlv_is_obj(m)
		? xlo_index(XL_OBJECT(m) ,n ,XLVF_REF | XLVF_CREATE, &tmp)
		: xlv_index(           m ,n ,XLVF_REF | XLVF_CREATE, &tmp);

	    if( a < 0) {
		xlp_except(thread,xlpe_unknown,"index not found or out of memory");
	    }
	    xlv_done(s-2);
	    xlv_mov (s-2,&tmp);
	    xlv_done(s-1);

	    s = xlths_add(thread,-1);
	    _xlv_done(&tmp1);
	    _xlv_done(&tmp2);
	    break; /* from switch instr->code */

/********************/
/** INSTR_FIELD

    Return reference of object field.

    Paramater	: name of field
    Stack	: 0

	INPUT <record object>

	OUTPUT <reference to field>

    Complexity: CTX_FIND
*/

/********************/
	case INSTR_FIELD:
	    m = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    /* Retrieve value reference to field value */

	    a = xlv_is_obj(m)
		? xlo_field(XL_OBJECT(m),xls_get(i->ival.v.s),XLVF_REF | XLVF_CREATE, &tmp)
		: xlv_field(           m,xls_get(i->ival.v.s),XLVF_REF | XLVF_CREATE, &tmp);

	    if( a < 0 ) {
		xlp_except(thread,xlpe_unknown,"member not found or out of memory");
	    }
	    xlv_done(s-1);
	    xlv_mov (s-1,&tmp);
	    _xlv_done(&tmp1);
	    break /* from switch instr->code */;

/********************/
/** INSTR_STORE

    Store value into reference.

    Parameter	: none
    Stack	: -1

	INPUT <reference to variable> <value>

	OUTPUT <reference to variable>

    Complexity: 1
*/
	case INSTR_STORE:
	    m = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    /* m = xlv_need_deref( s-1 ) ? xlv_deref(s-1) : s-1; */
            n = s-2;
	    a = 0;
	    switch ( s[-2].type ) {
	    case XLV_REF:
		a = xlv_assign(s[-2].v.ref.p, m);
		break;
	    case XLV_STACK:
		a = xlv_assign(xlv_stack2val(s-2), m);
		break;
	    default:
		xlp_except(thread,xlpe_invalid_store,"must be reference");
		a = -1;
	    }
	    if( a < 0 )
		xlp_except(thread,xlpe_invalid_store,"must be reference");
	    xlv_done(s-1);
	    _xlv_done(&tmp1);
	    s = xlths_add(thread,-1);
	    break /* from switch instr->code */;

/********************/
/** INSTR_STORE_REG

    Store value into register.

    Parameter	: register number
    Stack	: -1

	INPUT <value>

	OUTPUT

    Complexity: 1
*/
	case INSTR_STORE_REG:
	    if( i->ival.type != XLV_INT )
		xlp_except(thread,xlpe_invalid_register,"must be integer number");
	    xlv_set  ( & thread->regs[i->ival.v.i],s-1 );
	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	    break /* from switch instr->code */;

/********************/
/** INSTR_STORE_CAST

    Store with type cast(UNUSED)(NOT IMPLEMENTED).

    Stores value in place where the reference points
    without changing its [variable] type.

    Stack: -1

	INPUT <reference to variable> <value>

	OUTPUT <reference to variable>

    Complexity: 1
*/
	case INSTR_STORE_CAST:
	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	    NOT_READY;
	    break /* from switch instr->code */;

/********************/
	case INSTR_COPY:
            if( i->ival.type == XLV_NIL ) {
                /*
                    s-1 is the string literal containing name of the object 
                    type for example
                        map <default>
                        array 
                */
                v = (xl_value)xlo_new_namex( xlv_strx(s-1) );

            } if( i->ival.type == XLV_STR ) {
                s = xlths_add(thread,1);
                v = (xl_value)xlo_new_namex( xlv_strx(&i->ival));
            } else if( i->ival.type == XLV_INT ) {
                if( i->ival.v.i == 0 ) {
                    v = (xl_value)xlo_new_map();
                    s = xlths_add(thread,1);
                } else {
                    m = xlv_deref(s-1);
                    if( !xlv_is_obj(m) ) {
                        xlp_except(thread,xlpe_invalid_copy,"must be object");
                        break /* from switch instr->code */;
                    }
                    v = (xl_value) xlo_copy( (xl_object) m);
                    xlv_done(s-1);
                }
            } else if( i->ival.type != XLV_INT ) {
		xlp_except(thread,xlpe_invalid_copy,"must be int");
		break /* from switch instr->code */;
	    }
	    xlv_set_ref(s-1,v,NULL);
	    xlv_dec_ref(v);
	    break /* from switch instr->code */;

/********************/
/** INSTR_STD_OPER_BI:

    Operate on two values.

    Flags     :
	* XLI_MODIFY_1
	    Don't use temporary, operate directly
	    on values on stack. With this flag set
	    value (or resource) referenced by stack(-2)
	    may be changed.

    Parameter	: integer : operator code XLOP_xxx
    Stack	: -1

    INPUT   <valueA> <valueB>

    OUTPUT  <valueA OPER valueB>
*/
	case INSTR_STD_OPER_BI: 	/* m OPER n */
	    {
		register int (* oper_f)( xl_value,xl_value ) =
		    (int (*)( xl_value,xl_value))
			oper_callback_table[i->ival.v.i];
		if( !oper_f ) {
		    xlp_except(thread,xlpe_invalid_instruction,"unknown binary operator(NULL)");
		    break /* from switch instr->code */;
		}
		m = xlv_deref(s-2);
		EXTRACT_AND_DEREF(m,tmp1);
		n = xlv_deref(s-1);
		EXTRACT_AND_DEREF(n,tmp2);
		if( ZFL_ISSET( iflags , XLI_MODIFY_1 ) )
		    a = xlv_oper_bi(m,n,oper_f);
		else {
		    xlv_clear(&tmp);
		    a = xlv_oper_bi_dup(&tmp,m,n,oper_f);
		    xlv_done(s-2);
		    xlv_mov(s-2,&tmp);
		}
		if( a < 0 ) {
		    xlp_except(thread,xlpe_invalid_instruction,"operator call failed");
		}
		xlv_done(s-1);
		s = xlths_add(thread,-1);
		_xlv_done(&tmp1);
		_xlv_done(&tmp2);
	    }
	    break /* from switch instr->code */;

/********************/
/** INSTR_STD_OPER_UN:

    Operate on one value.

    Flags     :
	* XLI_MODIFY_1
	    Don't use temporary, operate directly
	    on values on stack. With this flags set
	    value (or resource) referenced by stack(-1)
	    may be changed.

    Parameter	: integer : operator code XLOP_xxx
    Stack	: -1

    INPUT   <valueA>

    OUTPUT  <OPER valueA>
*/
	case INSTR_STD_OPER_UN: 	/* OPER m */
	    {
		register int (* oper_f)( xl_value ) =
		    (int (*)( xl_value ))
			oper_callback_table[i->ival.v.i];
		if( !oper_f ) {
		    xlp_except(thread,xlpe_invalid_instruction,"unknown unary operator(NULL)");
		    break /* from switch instr->code */;
		}
		m = xlv_deref(s-2);
		EXTRACT_AND_DEREF(m,tmp1);
		if( ZFL_ISSET( iflags , XLI_MODIFY_1 ) )
		    a = xlv_oper_un(m,oper_f);
		else {
		    xlv_clear(&tmp);
		    a = xlv_oper_un_dup(&tmp,m,oper_f);
		    xlv_done(s-1);
		    xlv_mov(s-1,&tmp);
		}
		if( a < 0 ) {
		    xlp_except(thread,xlpe_invalid_instruction,"operator call failed");
		}
		_xlv_done(&tmp1);
	    }
	    break /* from switch instr->code */;

/********************/
/** INSTR_EQUAL

    Check if two values are equal.

    Parameter	: none
    Stack	: -1

    INPUT   <valueA> <valueB>

    OUTPUT  < valueA == valueB >
*/
	case INSTR_EQUAL:	/* m == n */

	    m = s-2;
	    n = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    EXTRACT_AND_DEREF(n,tmp2);
	
	    a = xlv_cmp(m,n);

	    xlv_done(s-2);
	    xlv_set_int(s-2,a == 0);
	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	
	    _xlv_done(&tmp1);
	    _xlv_done(&tmp2);
	    break /* from switch instr->code */;

/********************/
/** INSTR_NOT_EQUAL

    Check if two values are different.

    Parameter	: none
    Stack	: -1

    INPUT   <valueA> <valueB>

    OUTPUT  < valueA != valueB >
*/
	case INSTR_NOT_EQUAL:	/* m != n */
	    m = s-2;
	    n = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    EXTRACT_AND_DEREF(n,tmp2);

	    a = xlv_cmp(m,n);
	    xlv_frees(s-2);
	    xlv_set_int(s-2,a != 0);

	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	
	    _xlv_done(&tmp1);
	    _xlv_done(&tmp2);
	    break /* from switch instr->code */;

/********************/
/** INSTR_LESS

    Check if valueA is less than valueB

    Parameter	: none
    Stack	: -1

    INPUT   <valueA> <valueB>

    OUTPUT  < valueA < valueB >
*/
	case INSTR_LESS:	/* m < n */
	    m = s-2;
	    n = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    EXTRACT_AND_DEREF(n,tmp2);

	    a = xlv_cmp(m,n);
	    xlv_frees(s-2);
	    xlv_set_int(s-2,a < 0);
	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	    _xlv_done(&tmp1);
	    _xlv_done(&tmp2);
	    break /* from switch instr->code */;

/********************/
/** INSTR_GREATER

    Check if valueA is greater than valueB

    Parameter	: none
    Stack	: -1

    INPUT   <valueA> <valueB>

    OUTPUT  < valueA > valueB >
*/
	case INSTR_GREATER:	/* m > n */
	    m = s-2;
	    n = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    EXTRACT_AND_DEREF(n,tmp2);

	    a = xlv_cmp(m,n);
	    b = xlv_cmp(n,m);
	    xlv_done(s-2);
	    xlv_set_int(s-2,a > 0);

	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	    _xlv_done(&tmp1);
	    _xlv_done(&tmp2);

	    break /* from switch instr->code */;

/********************/
/** INSTR_LESS_EQUAL

    Check if valueA is less than valueB, or if they are equal.

    Parameter	: none
    Stack	: -1

    INPUT   <valueA> <valueB>

    OUTPUT  < valueA <= valueB >
*/
	case INSTR_LESS_EQUAL:	/* m <= n */
	    m = s-2;
	    n = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    EXTRACT_AND_DEREF(n,tmp2);

	    a = xlv_cmp(m,n);
	    xlv_frees(s-2);
	    xlv_set_int(s-2,a <= 0);

	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	    _xlv_done(&tmp1);
	    _xlv_done(&tmp2);

	    break /* from switch instr->code */;

/********************/
/** INSTR_GREATER_EQUAL

    Check if valueA is greater than valueB, or if they are equal.

    Parameter	: none
    Stack	: -1

    INPUT   <valueA> <valueB>

    OUTPUT  < valueA >= valueB >
*/
	case INSTR_GREATER_EQUAL:/* m >= n */
	    m = s-2;
	    n = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    EXTRACT_AND_DEREF(n,tmp2);

	    a = xlv_cmp(m,n);
	    xlv_frees(s-2);
	    xlv_set_int(s-2,a >= 0);
	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	    _xlv_done(&tmp1);
	    _xlv_done(&tmp2);

	    break /* from switch instr->code */;

/********************/
/** INSTR_BOOL

    Convert value to boolean state 0 or 1.

    Parameter	: none
    Stack	: 0

    INPUT   <valueA>

    OUTPUT  < bool(valueA) >
*/
	case INSTR_BOOL:
	    n = s-1;
	    EXTRACT_AND_DEREF(n,tmp1);
	    tmp.type = XLV_INT;
	    tmp.v.i = xlv_true(n);
	    xlv_set(s-1,&tmp);
	    tmp.type = XLV_NIL;
	    _xlv_done(&tmp1);

	    break /* from switch instr->code */;

/********************/
/** INSTR_VALID

    Check is value is valid.

    Parameter	: none
    Stack	: 0

    INPUT   <valueA>

    OUTPUT  < 0 or 1 >
*/
	case INSTR_VALID:
	    n = s-1;
	    EXTRACT_AND_DEREF(n,tmp1);

	    tmp.type = XLV_INT;
	    tmp.v.i = (n->type >= XLV_INT) && (n->type <= XLV_LAST);

	    xlv_set(s-1,&tmp);
	    tmp.type = XLV_NIL;
	    _xlv_done(&tmp1);
	    break /* from switch instr->code */;

/********************/
/** INSTR_CMP

    Compare valueA with valueB and return:
	negative  when    valueA < ValueB
	positive  when    valueA < ValueB
	zero	  when    valueA == ValueB

    Parameter	: none
    Stack	: -1

    INPUT   <valueA> <valueB>

    OUTPUT  < integer: compare result >
*/
	case INSTR_CMP:
	    m = s-2;
	    n = s-1;
	    EXTRACT_AND_DEREF(m,tmp1);
	    EXTRACT_AND_DEREF(n,tmp2);

	    a = xlv_cmp(m,n);

	    xlv_done(s-2);
	    xlv_set_int(s-2,a);

	    xlv_done(s-1);
	    s = xlths_add(thread,-1);
	    _xlv_done(&tmp1);
	    _xlv_done(&tmp2);
	    break /* from switch instr->code */;

/********************/
/** INSTR_JMP

    Make unconditional jump.

    Parameter	:
	str : name of label
	int : ip address
    Stack	: 0

    Does't change stack.
*/
	case INSTR_JMP:
case_INSTR_JMP:
	    if( i->ival.type == XLV_STR ) {
		if( thread->code->labels ) {
		    a = (int) xlmap_find(thread->code->labels,i->ival.v.s);
		    thread->nip = a;
		} else
		    xlp_except(thread,xlpe_unknown_label,"can't jump");
		continue /* switch instr->code */;
	    } else if( i->ival.type == XLV_INT ) {
		thread->nip = i->ival.v.i;
	    } else {
		xlp_except(thread,xlpe_invalid_jump,"must be interger or string");
	    }
	    break /* from switch instr->code */;

/********************/
/** INSTR_TRUE

    Make conditional jump if valueC is true.

    Parameter	:
	str : name of label
	int : ip address
    Stack	:
	XLI_LEAVE_1     set: 0
	    INPUT  <valueC>
	    OUTPUT <valueC>

	XLI_LEAVE_1 not set: -1
	    INPUT  <valueC>
	    OUTPUT

*/
	case INSTR_TRUE:
	    a = 0;
	    n = s-1;
	    EXTRACT_AND_DEREF(n,tmp1);
	    a = xlv_true(n);

	    if( !ZFL_ISSET( iflags , XLI_LEAVE_1 ) ) {
		xlv_done(s-1);
		s = xlths_add(thread,-1);
	    }
	    _xlv_done(&tmp1);

	    if( a )
		goto case_INSTR_JMP;
	    break /* from switch instr->code */;

/********************/
/** INSTR_FALSE

    Make conditional jump if valueC is false.

    Parameter	:
	str : name of label
	int : ip address
    Stack	:
	XLI_LEAVE_1     set: 0
	    INPUT  <valueC>
	    OUTPUT <valueC>

	XLI_LEAVE_1 not set: -1
	    INPUT  <valueC>
	    OUTPUT

*/
	case INSTR_FALSE:
	    a = 0;

	    n = s-1;
	    EXTRACT_AND_DEREF(n,tmp1);
	    a = xlv_true(n);


	    if( !ZFL_ISSET(iflags,XLI_LEAVE_1) ) {
		xlv_done(s-1);
		s = xlths_add(thread,-1);
	    }
	    _xlv_done(&tmp1);

	    if( ! a )
		goto case_INSTR_JMP;
	    break /* from switch instr->code */;

/********************/

	case INSTR_DUP:
	    if( i->ival.type != XLV_INT )
		xlp_except(thread,xlpe_invalid_dup,"must be int");
	    b = i->ival.v.i; /* duplicate count */
	    for(a=0;a < b;a++) {
		s = xlths_add(thread,1);
		xlv_cpy(s-1, s-2);
	    }
	    break /* from switch instr->code */;

/********************/

	case INSTR_DROP:
	    if( i->ival.type != XLV_INT )
		xlp_except(thread,xlpe_invalid_drop,"must be int");
	    b = i->ival.v.i; /* drop count */

	    for(a = 1; a <= b; a++) {
		xlv_done(s-a);
	    }

	    s = xlths_add(thread,-b);
	    break;
/********************/

	case INSTR_CALL:
	    /* NOW stack before call is:
		s-N-1	func_ptr
		s-N	param1
		s-N+1	param2
		...
		s-1 	paramN	- N - parameter count
	    */
	    /* After call
		...
		s-6-N-1    	func_ptr
		s-6-N      	param1			^ <-- PP
		s-6-(N-1)	param1			| PC
		   ...					|
		s-6		paramN			v
		s-5		return IP		  <-- SP
		s-4		old CX
		s-3		old PP
		s-2		old PC
		s-1		old SP
	    */

	    b = i->ival.v.i;		/* ACT parameter count */
	    {
		n = xlv_deref(s-b-1);		/* function pointer */
		EXTRACT_AND_DEREF(n,tmp1);
		/* here check if function was called with enough
		    number of parameters */
		if( (n->type == XLV_FUNC_LABEL || n->type == XLV_FUNC_IP)
		    && n->v.func.params != NULL ) {
		    /* Those two types of functions can have default
			parameters. */
		    int N = n->v.func.params->number;
		    if ( b < N ) {
			xl_value iv = n->v.func.params->def;
			iv+=N-b;
			while( b < N ) {
			    s = xlths_add(thread,1);
			    xlv_set(s-1,--iv);
			    b++;
			}
		    }
		}
		_xlv_done(&tmp1);
	    }
	    s = xlths_add(thread,5);

	    xlv_set_int(s-5, thread->nip);
	    xlv_set_int(s-4,TH_REG_CX);
	    xlv_set_int(s-3,TH_REG_PP);
	    xlv_set_int(s-2,TH_REG_PC);
	    xlv_set_int(s-1,TH_REG_SP);

	    TH_REG_PP = thread->stack.itop - 6 - b+1;
	    TH_REG_PC = b;
	    TH_REG_CX = 0;
	    TH_REG_SP = thread->stack.itop - 5;

	    _xlv_clear(&tmp);

#if 0 && defined XLD_PROC_TRACE
            if( xldbg_mask_enabled(TRACE_STACK) ) {
	        xlths_dump(zstdout,thread,10);
	        zprintf("----- before real CALL\n");
            }
#endif
	    switch( xlf_call(thread, (s-b-6) , (s-b-5) ,b, &tmp) ) {
	    case 1:
		/* the call changed thread->nip */
		continue /* while !thread->run */;
	    case 2:
		/* called builtin */
		s = xlths_add(thread,1);
		xlv_mov(s-1,&tmp);
		break; /* to INSTR_RET */

	    case 3:
		NOT_READY;
		/* thread is interrupted */
		/*
		    This case is used when xlf_call has made a blocking call
		    which not stopped the vm. Vm will wait for result and then
		    get back to the same execution point. With returned
		    result in REG_RETURN.

		    (not implemented)
		*/
		thread->nip = thread->ip+1; /* skip this instruction */
		thread->run = 0;
		continue /* while !thread->run */;
	    default:
		/* an error ocurred */
		;
	    }

/********************/
	case INSTR_RET:
#if 1 && defined XLD_PROC_TRACE
            if( xldbg_mask_enabled(TRACE_STACK) ) {
	        xlths_dump(xl_get_debug_stream(),thread,10);
	        xldbg_print("----- before real RET\n");
            }
#endif
	    if( thread->run_mode == XLVM_RUN_UNTIL_RETURN ) {
		/* there was no call so exit immediately */
		xlv_set(&thread->ret, xlv_deref(s-1) );
		thread->finished = 1;
		thread->run = 0;
		continue /* while !thread->run */;
	    }
	    xlv_set(&tmp,xlv_deref(s-1));

	    b = TH_REG_PP - thread->stack.itop;

	    n = s;
	    s = thread->stack.ptr + TH_REG_SP;


	    if( TH_REG_SP == 0 ) {
		xlv_set(&thread->ret, xlv_deref(&tmp) );
		thread->finished = 1;
		thread->run = 0;
		continue;
	    }

	    m = s - TH_REG_PC - 1;
	    if( (m->type == XLV_REF && m->v.ref.parent != NULL && xlv_is_obj(m->v.ref.parent) ) || (
		m->type == XLV_MREF && m->v.ref.parent != NULL && xlv_is_obj((xl_value)m->v.mref.obj))
		) {
		/* leave method */
		xlth_leave_method(thread);
	    } else /* leave generic function */
		xlth_leave_func(thread);

	    TH_REG_CX = s[1].v.i;
	    TH_REG_PP = s[2].v.i;
	    TH_REG_PC = s[3].v.i;
	    TH_REG_SP = s[4].v.i;

	    thread->nip = s[0].v.i;

	    for( n=n-1; n > m; n-- )
		xlv_done(n);
	    s = xlths_add(thread,b);
	    xlv_mov(s-1,&tmp);
	    break; /* from switch instr->code */;


/********************/

	case INSTR_MARK:
	    NOT_READY;
	    break /* from switch instr->code */;

/********************/

	case INSTR_RELEASE:
	    NOT_READY;
	    break /* from switch instr->code */;

/********************/
/** INSTR_ECHO
    (int) parameter is number of values which will be echoed
*/
	case INSTR_ECHO:
	    b = i->ival.v.i;
	    for(a = b; a>0; a-- ) {
		register const char* str
		    = xlv_str(xlv_deref(s-a));
		register int l =  strlen(str);
		register int r = 0;
		while( l > 0 ) {
		    r = zmin(XLP_MAX_ECHO_WRITE,l);
		    zwrite(xlst_run_out,str,r);
		    l -= r;
		    str += r;
		}
		xlv_done(s-a);
	    }

	    s = xlths_add(thread,-b);

	    break /* from switch instr->code */;

/********************/
/** INSTR_GET
    This is somewhat stupid instruction.

*/
	case INSTR_GET:
	    b = i->ival.v.i;
	    if( b == 0 ) {
		b = s[-1].v.i + 1;
	    } else {
		s = xlths_add(thread,1);
		b++;
	    }
	    if( b == 0 )
		break /* from switch instr->code */;
	    xlv_cpy(s-1,s-b);
	    break /* from switch instr->code */;

/********************/
/** INSTR_EXIT
    Exit thread.

    Stack	    : 0
*/
	case INSTR_EXIT:
	    xlv_set_int(&thread->ret,0);
	    thread->finished = 1;
	    thread->run = 0;
	    return 0;
/*
	case INSTR_:
	    break;
*/
	case INSTR_PARAM:
	    /* Get parameter number ->	b */
            if( i->ival.type == XLV_NIL ) {
                b = xlv_int(s-1);
	        s = xlths_add(thread,-1);
            } else
	        b = i->ival.v.i;
	    /* Get base pointer  ->	a */
            a = b >= 0 ? TH_REG_PP : 0;
            if( b < 0 ) 
                b = -b-1;
	    s = xlths_add(thread,1);
            s[-1].type = XLV_STACK;
            s[-1].flags = XLVF_NEED_DEREF;
            s[-1].v.stack.stack = &thread->stack;
            s[-1].v.stack.index = (b+a);
	    break /* from switch instr->code */;

	case INSTR_DEREF:
	    m = xlv_deref(s-1);
	    EXTRACT_AND_DEREF(m,tmp1);
	    xlv_inc_ref(m);
	    if( m->type == XLV_CHAR_REF ) {
		xlv_extract(m,&tmp);
		xlv_mov(s-1,&tmp);
	    } else  if( m != s-1 ) {
                xlv_mov(&tmp,s-1);
		xlv_set(s-1,m);
                xlv_done(&tmp);
	    }
	    xlv_dec_ref(m);
	    _xlv_done(&tmp1);
	    break; /* from switch instr->code */;

/********************/
/** INSTR_LEAVE_CTX

    Leave current CTX.
*/
	case INSTR_LEAVE_CTX:
	    xlth_leave(thread);
	    break /* from switch instr->code */;

/********************/
/** INSTR_ENTER_CTX

    Enters to the new context.

    if (int) parameter is
	0 enter to the context created from object on stack
	1 enetr global context
*/
	case INSTR_ENTER_CTX:
	    m = xlv_deref(s-1);
	    if( !xlv_is_obj(m) )
		xlp_except(thread,xlpe_invalid_store,"with expression must be an object");
	    switch( i->ival.v.i ) {
	    case 0:
		xlth_enter_object(thread,(xl_object)m);
		break;
	    case 1:
		xlth_enter_object_global(thread,(xl_object)m);
		break;
	    default:
		xlp_except(thread,xlpe_invalid_instruction,"invalid instruction (ENTER_CTX)");
	    }
	    xlv_done(s-1);
	    s = xlths_add(thread,-1);

	    break /* from switch instr->code */;
/********************/
/** INSTR_FREE_VAR


    if (parameter is string)
	* and its first char is '.' free member of object
	    pointed by top of stack
	* else free variable
    else
	free element indexed by s-1 in object s-2
*/
	case INSTR_FREE_VAR:
	    {
		if( i->ival.type == XLV_STR ) {
		    const char* fname = xls_str(i->ival.v.s);
		    if( fname[0] == '.' ) {
			xl_object fobj = (xl_object) xlv_deref(s-1);
			if( fobj && xlo_is_obj(fobj) )
			    xlo_free_member(fobj,fname+1);
			xlv_done(s-1);
			s = xlths_add(thread,-1);
		    } else
			xlx_free_variable(thread->var,fname);
		} else {
		    const char* fname = xlv_str(s-1);
		    xl_object	fobj = (xl_object) xlv_deref(s-2);
		    if( fobj && xlo_is_obj(fobj) )
			xlo_free_member(fobj,fname);
		    xlv_done(s-1);
		    xlv_done(s-2);
		    s = xlths_add(thread,-2);
		}
	    }
	    break /* from switch instr->code */;
	default:
	    xlp_except(thread,xlpe_invalid_instruction,"invalid instruction(UNKNOWN)");
	    break /* from switch instr->code */;
	}
    }
    return thread->run;
}

void	xlp_except(xl_thread thread,except_num num,const char* msg)
{
    int ei = (int)num % countof(except_table);
    char* es = except_table[ei] ? except_table[ei] : "unknown";
    if( thread != NULL ) {
	thread->run = 0;
	thread->finished = 1;
	thread->interrupted = 2; /* processor exception */
	fprintf(stderr,ANSI_C_RED("XL PROCESSOR EXCEPTION")" : %s : %s\nat ip=%i\n",es,msg,thread->ip);
	if( thread->debug_file != NULL ) {
	    fprintf(stderr,"in file `%s` near line %i\n",xls_get(thread->debug_file),thread->debug_line);
	}
    } else
	fprintf(stderr,ANSI_C_RED("XL PROCESSOR EXCEPTION")" : %s : %s\n",es,msg);
    /*
    exit(ei);
    */
    /*
    raise(SIGSEGV);
    */
    abort();
}

static void xlp_print_ptr(ZSTREAM f,void* ptr)
{
    if( ptr == NULL )
	zfprintf(f, ANSI_C_RED("!<NULL>!") );
    else
	zfprintf(f, ANSI_C_GREEN("<0x%08x>") ,ptr);
}

void	xlths_dump(ZFILE* f,xl_thread thread,int max)
{
    int i;
    int pr = thread->regs[REG_PP].v.i;
    int sp = thread->stack.itop;
    int pc = thread->regs[REG_PC].v.i;
    xl_value t,s;
    max = zmin( max , sp );
    s = t = thread->stack.top;
    zfprintf(f,
	ANSI_C_GREEN("XLDBG(stack): BEGIN at IP=%i (SP=%i,PR=%i,PC=%i)\n"),
	    thread->ip,
	    sp,
	    pr,
	    pc);
    for(i=0;i < max;i++ ) {
	--s;
	zfprintf(f,"XLDBG(stack):     %02i=%02i-%02i:%02i ",sp-(i+1),sp,i+1,s->type);
        {
            void    xldbg_val_print(ZSTREAM s, const xl_value_t* val,int options);
            xldbg_val_print(f,s,0xff);
        }

	if( pr > 0 ) {
	    if( i == sp-pr-pc-1  )
		zfprintf(f,ANSI_C_YELLOW("<- return IP"));
	    if( i == sp-pr-pc-2  )
		zfprintf(f,ANSI_C_YELLOW("<- previous CX"));
	    if( i == sp-pr-pc-3  )
		zfprintf(f,ANSI_C_YELLOW("<- previous PP"));
	    if( i == sp-pr-pc-4  )
		zfprintf(f,ANSI_C_YELLOW("<- previous PC"));
	    if( i == sp-pr-pc-5  )
		zfprintf(f,ANSI_C_YELLOW("<- previous SP"));
            if( pc > 0 ) {
	        if( i == sp-pr-1 )
		    zfprintf(f,ANSI_C_YELLOW("<- first argument "));
	        if( i == sp-pr-pc )
    		    zfprintf(f,ANSI_C_YELLOW("<- last argument"));
            }
	    if( i == sp-pr )
		 zfprintf(f,ANSI_C_YELLOW("<- function called "));
	}

	zfprintf(f,"\n");
    }
    zfprintf(f,"XLDBG(stack): END");
}

int	xlf_call(
    xl_thread thread,
    xl_value function,
    xl_value first_param,
    int param_count,
    xl_value result
    )
{
    xl_value parent =
	function->type == XLV_REF && function->v.ref.parent != NULL
	    ? function->v.ref.parent : 
	function->type == XLV_MREF && function->v.mref.obj 
	    ? (xl_value)function->v.mref.obj
	    : NULL;
    int method = parent != NULL && xlv_is_obj(parent);
    int to_ret = 0;
    xl_value_t function_temp;
    xlv_clear(&function_temp);
    if( method ) {
	xlth_enter_method(thread,(xl_object)parent);
    } else
	xlth_enter_func(thread);

/** TODO : <this> should be special identifier.
    It should be accessed by register or something,
    not through a context.
*/
    xlx_set_ref(thread->var,"this",parent);

    function = xlv_deref(function);
    if( xlv_need_extract(function) ) {
        if( xlv_extract(function, &function_temp) < 0 )
            return  -1;       
        function = &function_temp;
    }
    switch( function->type ) {
    case XLV_USER_VAL:
    case XLV_USER_OBJ:
	/** TODO:
	    * add call method to vtable with interface:
		int call(xl_value v,
		    xl_value parent,
		    xl_thread th,
		    int argc,
		    xl_value argv,
		    xl_value ret)
		which returns 0 on succes or -1 on error
	*/
	xlp_except(thread,xlpe_invalid_function,"USER call not supported yet");
	to_ret = -1;
	break;
    case XLV_STR:
	if( thread->code->labels ) {
	    int a = (int) sbtree_find(thread->code->labels,xls_str(function->v.s));
	    if( a <= 0 ) {
		xlp_except(thread,xlpe_unknown_label,"label not found");
		to_ret = -1;
	    } else {
		thread->ip = a;
		/* Return 1 means that we have changed ip */
		to_ret = 1;
	    }
	} else {
	    xlp_except(thread,xlpe_unknown_label,"no labels generated");
	    to_ret = 1;
	}
	break;
    case XLV_FUNC_BUILTIN:
	if( function->v.func_builtin.address != NULL ) {
	    (function->v.func_builtin.address)(thread,param_count,first_param,result);
	    /* Return 2 means that we have called builtin */
	    to_ret = 2;
	} else {
	    xlp_except(thread,xlpe_invalid_function,"NULL address of builtin function");
	    to_ret = -1;
	}
	break;
    case XLV_FUNC_LABEL:
	{
	    const char* get_label_text(int i);	/* codegen.c : FIX IT*/
	    xl_str s;
	    int ip = -1;
	    s =  xls_new( get_label_text( function->v.func.address.label ) );
	    if( s && thread->code->labels )
		ip = (int) xlmap_find(thread->code->labels,s );
	    xls_free(s);
	    if( ip >= 0  ) {
		thread->nip = ip;
		to_ret = 1;
	    } else {
		xlp_except(thread,xlpe_unknown_label,"bad label");
		to_ret = -1;
	    }
	}
	break;
    case XLV_FUNC_IP:
	thread->nip = function->v.func.address.ip;
	to_ret = 1;
/*	xlp_except(thread,xlpe_invalid_function,"call address type or not supported"); */
	break;
    default:
	xlp_except(thread,xlpe_invalid_call,"not function or label name");
	to_ret = -1;
    }
/*
    if( method )
	xlth_leave_method(thread);
    else
	xlth_leave_func(thread);
*/
    xlv_done(&function_temp);
    return to_ret;
}

void	xlthd_debug_info(xl_thread thread,xl_value v)
{
    if( v == NULL || thread == NULL )
	return;
    switch( v->type ) {
    case XLV_INT:
	thread->debug_line = v->v.i;
	break;
    case XLV_STR:

	{
	    const char* x = xls_get(v->v.s);
	    if( x && *x == '$') 
		thread->debug_file = xls_set_xls(thread->debug_file,v->v.s);
	}
	break;
    default:
	break;
    }
#if 0 || defined XLD_PROC_TRACE
    if( xldbg_mask_enabled(TRACE_STACK) ) {
        if( thread->debug_file )
            xldbg_print(
            ANSI_C_YELLOW("XLDBG(debug): file: <") "%s"  ANSI_C_YELLOW(">: %i") "\n",xls_get(thread->debug_file),thread->debug_line);
        else
            xldbg_print(
            ANSI_C_YELLOW("XLDBG(debug): file: <")ANSI_C_YELLOW(">: %i") "\n",thread->debug_line);
    }
#endif
    return;
}

void		xlthd_set_line(xl_thread thread,int line)
{
    if( thread == NULL )
	return;
    thread->debug_line = line;
}
void		xlthd_set_file(xl_thread thread,const char* name)
{
    if( thread == NULL )
	return;
    thread->debug_file = xls_set(thread->debug_file,name);
}
