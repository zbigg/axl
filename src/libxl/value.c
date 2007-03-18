/*
* File: 	value.c
*
* Id:           $Id: value.c 1.6 04/02/24 01:39:33+01:00 zbigg@marl.(none) $
*
* Project:	axl
*
* Description: value
*
    Defines:
	xl_epsilon: maximum absolute difference for comparing floats
	XL_STR_BUF_ROW_SIZE - length of temporary string (def. 100)
	XL_STR_BUF_COUNT    - number of temporary strings (def. 20)

    Conditional defines:
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/
/*
    TODO:
	* xlv_add   : optimize string concatening

	* xlv_deref
	* xlv_index
	* xlv_field : update interface for those above
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "axl/axl.h"
#include "tools.h"
#include "zcompat/zpio.h"
#include "zcompat/zprintf.h"
#include "axl_i.h"

#ifndef xl_epsilon
/**
    Two floating point values in distance less than
    xl_epsilon are treated as equal.
    So number in range (-xl_epsilon; xl_epsilon) are
    equal to 0.

    See xl_float_cmp
*/
#define xl_epsilon (1e-12)
#endif

#define xlv_alloc(n)	(xl_value)	axl_calloc((n),sizeof( xl_value_t ))
/* valoper.c
    Try to make number (float or integer) from string.
*/
int	    xl_try_str2number(const char* s,xl_float* df,xl_integer* di);

xl_float    xl_str2float(const char* s,int* error);
xl_integer  xl_str2int(const char* s,int* error);

xl_integer xl_str2int(const char* s,int* error)
{
    xl_integer i; xl_float f;
    i = 0; f = 0.0;
    switch( xl_try_str2number(s,&f,&i) ) {
    case 0:
	if( error )
	    *error = 1;
	return 0;
    case XLV_INT:
	break;
    case XLV_FLOAT:
	i = (xl_integer)f;
	break;
    }
    if( error )
	*error = 0;
    return i;
}

xl_float xl_str2float(const char* s,int* error)
{
    xl_integer i; xl_float f;
    i = 0; f = 0.0;
    switch( xl_try_str2number(s,&f,&i) ) {
    case 0:
	if( error )
	    *error = 1;
	return 0.0;
    case XLV_FLOAT:
	break;
    case XLV_INT:
	f = (xl_float)i;
	break;
    }
    if( error )
	*error = 0;
    return f;
}

void	xlv_inc_ref(const xl_value_t* v)
{
    if( !v )
	return;
    if( xlv_is_obj(v) ) {
	xlo_inc_ref((xl_object)v);
    }
}

void	xlv_dec_ref(const xl_value_t* v)
{
    if( !v )
	return;
    if( xlv_is_obj(v) )
	xlo_free( (xl_object) v );
}
xl_value	xlv_new(xl_value o)
{
    xl_value a = xlv_alloc(1);
    XL_FCTX("xlv_new","",(1));
    if( a == NULL )
	return NULL;
    MDMF_FILL(a,"xlv ");
    if( o == NULL ) {
	a->type = XLV_NIL;
	a->flags = 0;
	a->v.i = 0;
	return a;
    }
    xlv_cpy(a,o);
    return a;
}
xl_value	xlv_vnew(int type, ...)
{
    xl_value v = xlv_alloc(1);
    int		xlv_vvset(xl_value v,int type, va_list ap);
    va_list ap;
    va_start(ap,type);
    xlv_vvset(v,type,ap);
    va_end(ap);
    return v;
}

int		xlv_vset(xl_value dest,int type, ...)
{
    int		xlv_vvset(xl_value v,int type, va_list ap);
    va_list ap;
    va_start(ap,type);
    xlv_vvset(dest,type,ap);
    va_end(ap);
    return 0;
}
int		xlv_vvset(xl_value v,int type, va_list ap)
{
    switch( type ) {
    case XLV_CHAR:
	v->v.ch = va_arg(ap,int);
	break;
    case XLV_INT:
	v->v.i = va_arg(ap,xl_integer);
	break;
    case XLV_FLOAT:
	v->v.f = va_arg(ap,xl_float);
	break;
    case XLV_STR:
	{
	    char* pc = va_arg(ap,char*);
	    pc = (pc != NULL) ? pc : "";
	    v->v.s = xls_new(pc);
	    if( v->v.s == NULL ) return -XLE_MEMORY;
	} break;
    case XLV_STACK:
	v->v.stack.index = va_arg(ap,int);
	v->v.stack.stack = NULL;
	break;
    case XLV_REF:
	v->v.ref.p = va_arg(ap,xl_value);
	v->v.ref.parent = va_arg(ap,xl_value);
	break;
    case XLV_FUNC_LABEL:
	break;
    }
    v->type = type;
    MDMF_FILL(v,"xlv ");
    return 0;
}

xl_value	xlv_new_str(const char* s)
{
    xl_value a = xlv_alloc(1);
    if( !a )
	return 0;
    MDMF_FILL(a,"xlv ");
    a->type = XLV_STR;
    a->flags = 0;
    a->v.s = xls_new(s);
    return a;
}

xl_value	xlv_new_strn(const char* s,int length)
{
    xl_value a = xlv_alloc(1);
    if( !a )
	return 0;
    MDMF_FILL(a,"xlv ");
    a->type = XLV_STR;
    a->flags = 0;
    a->v.s = xls_newn(s,length);
    return a;
}

xl_value	xlv_new_int(xl_integer i)
{
    xl_value a = xlv_alloc(1);
    if( !a )
	return 0;
    MDMF_FILL(a,"xlv ");
    a->type = XLV_INT;
    a->flags = 0;
    a->v.i = i;
    return a;
}

xl_value	xlv_new_float(xl_float f)
{
    xl_value a = xlv_alloc(1);
    if( !a )
	return 0;
    MDMF_FILL(a,"xlv ");
    a->type = XLV_FLOAT;
    a->flags = 0;
    a->v.f = f;
    return a;
}

xl_value	xlv_new_ref(xl_value v,xl_value parent)
{
    xl_value a = xlv_alloc(1);
    XL_FCTX("xlv_new",XLF_PTR,(v));
    if( a == NULL || v == NULL )
	return 0;
    MDMF_FILL(a,"xlv ");
    a->flags = 0;
    a->type = XLV_REF;
    xlv_inc_ref( a->v.ref.p = v );
    xlv_inc_ref( a->v.ref.parent = parent );
    return a;
}
/* NOTE: functions xlv_set_func_XXX are in builtin.c */
xl_value	xlv_new_func_builtin(xl_builtin_func adr,const char* name)
{
    xl_value v = xlv_new_int(0);
    XL_FCTX2("xlv_new_func_builtin",XLF_PTR","XLF_PTR,adr,name);
    xlv_set_func_builtin(v,adr,name);
    v->flags = 0;
    return v;
}
xl_value	xlv_new_func_ip(int ip)
{
    xl_value v = xlv_new_int(0);
    xlv_set_func_ip(v,ip);
    v->flags = 0;
    return v;
}
xl_value	xlv_new_func_label(int label_nr)
{
    xl_value v = xlv_new_int(0);
    xlv_set_func_label(v,label_nr);
    v->flags = 0;
    return v;
}

void		xlv_free(xl_value v)
{
    XL_FCTX("xlv_free",XLF_PTR,v);
    if( v ) {
	if( xlv_is_obj(v) )
	    xlo_free((xl_object)v);
	else {
	    xlv_done(v);
	    axl_free(v);
	}
    }
}
void		xlv_frees(xl_value v)
{
    xlv_done(v);
}

void		xlv_done(xl_value v)
{
    XL_FCTX("xlv_done",XLF_PTR,v);
    switch( v->type ) {
    case XLV_MREF:
	if( v->v.mref.obj )
	    xlo_dec_ref(v->v.mref.obj);
	break;
    case XLV_ITERATOR:
	if( v->v.iter.object )
	    xlo_dec_ref(v->v.iter.object);
	break;
    case XLV_USER_VAL:
	if( v->v.user.vt->free )
	    v->v.user.vt->free(v);
	break;
    case XLV_REF:
	xlv_dec_ref( v->v.ref.p );
	if( v->v.ref.parent )
	    xlv_dec_ref(v->v.ref.parent);
	break;
    case XLV_STR:
	xls_free(v->v.s);
	break;
    case XLV_MAP_OBJ:
    case XLV_USER_OBJ:
/*	xlo_free( (xl_object) v);*/
	return;
    case XLV_FUNC_BUILTIN:
	xl_free_func_builtin(v);
	break;
    case XLV_FUNC_IP:
    case XLV_FUNC_LABEL:
	if( v->v.func.params )
	    xlv_fp_free(v->v.func.params);
	v->v.func.params = NULL;
	break;
    default:
	break;
    }
    v->type = XLV_NIL;
    v->flags = 0;    
    MDMF_FILL(v,"xlv ");
}

int		xlv_set(xl_value v,const xl_value_t* o)
{
    XL_FCTX2("xlv_set","v="XLF_PTR",o="XLF_PTR,v,o);
    if( v->type == XLV_STR && o->type == XLV_STR  ) {
	v->v.s = xls_set_xls(v->v.s, o->v.s );
	return v->v.s ? 0 : -XLE_MEMORY;
    }
    _xlv_done(v);
    return xlv_cpy(v,o);
}

int		xlv_mov(xl_value v,xl_value o)
{
    if( xlv_is_obj(v) ) {
	/* something wrong here */
	return -XLE_INTERNAL;
    }
    _xlv_done(v);
    MDMF_FILL(v,"xlv ");
    v->type = o->type;
    v->flags = o->flags;
    v->v = o->v;
    o->type = XLV_NIL;
    o->v.i = 0;
    o->flags = 0;
    return 0;
}
int		xlv_cpy(xl_value v,const xl_value_t* o)
{
    XL_FCTX2("xlv_cpy","v="XLF_PTR",o="XLF_PTR,v,o);
    if( v == NULL || o == NULL )
	return 0;
    switch( o->type ) {
    case XLV_ITERATOR:
	v->v = o->v;
	xlo_inc_ref(v->v.iter.object);
	break;
    case XLV_USER_VAL:
	v->v.user.vt = o->v.user.vt;
	if( o->v.user.vt->dup )
	    o->v.user.vt->dup(o,v);
	else
	    v->v.user.param = o->v.user.param;
	v->flags |= XLVF_DEREF;
	break;
    case XLV_REF:
	xlv_inc_ref( v->v.ref.p = o->v.ref.p );
	xlv_inc_ref( v->v.ref.parent = o->v.ref.parent );
	break;
    case XLV_STR:
	v->v.s = xls_new_xls(o->v.s);
	if( ! v->v.s) return -XLE_MEMORY;
	break;
    case XLV_MAP_OBJ:
    case XLV_RESOURCE:
    case XLV_ARRAY:
    case XLV_USER_OBJ:
	v->type = XLV_REF;
	v->v.ref.p = (xl_value)o;
	v->v.ref.parent = NULL;
	xlv_inc_ref( o );
	return 0;
    case XLV_FUNC_IP:
	v->v.func.address.ip = o->v.func.address.ip;
	if( o->v.func.params )
	    v->v.func.params = xlv_fp_dup(o->v.func.params);
	else
	    v->v.func.params = NULL;
	break;
    case XLV_FUNC_LABEL:
	v->v.func.address.label = o->v.func.address.label;
	if( o->v.func.params )
	    v->v.func.params = xlv_fp_dup(o->v.func.params);
	else
	    v->v.func.params = NULL;
	break;
    case XLV_FUNC_BUILTIN:
	v->v = o->v;
	v->v.func_builtin.name =
	    o->v.func_builtin.name != NULL
		? axl_strdup(o->v.func_builtin.name)
		: NULL;
	break;
    default:
	v->v = o->v;
    }
    v->type = o->type;
    v->flags = o->flags & ~(SET_DONT_COPY_FLAGS);
    return 0;
}

int		xlv_set_str(xl_value v,const char* s)
{
    if( v->type == XLV_STR ) {
	v->v.s = xls_set(v->v.s,s);
	return v->v.s ? 0 : -XLE_MEMORY;
    }
    _xlv_done(v);
    v->v.s = xls_new(s);
    if( ! v->v.s ) return -XLE_MEMORY;
    v->flags = 0;
    v->type = XLV_STR;
    return 0;
}

int		xlv_set_strn(xl_value v,const char* s,unsigned int len)
{
    if( v->type == XLV_STR ) {
	v->v.s = xls_setn(v->v.s,s,len);
	return v->v.s ? 0 : -XLE_MEMORY;
    }
    _xlv_done(v);
    v->v.s = xls_newn(s,len);
    if( ! v->v.s ) return -XLE_MEMORY;
    v->flags = 0;
    v->type = XLV_STR;
    return 0;
}

int		xlv_set_int(xl_value v,xl_integer i)
{
    _xlv_done(v);
    v->v.i = i;
    v->flags = 0;
    v->type = XLV_INT;
    return 0;
}

int		xlv_set_char(xl_value v,int c)
{
    _xlv_done(v);
    v->v.ch = c;
    v->flags = 0;
    v->type = XLV_CHAR;
    return 0;
}

int		xlv_set_float(xl_value v,xl_float f)
{
    _xlv_done(v);
    v->v.f = f;
    v->flags = 0;
    v->type = XLV_FLOAT;
    return 0;
}

int		xlv_set_ptr(xl_value v,void* ptr,int type)
{
    _xlv_done(v);
    v->v.res.pointer = ptr;
    v->v.res.type = type;
    v->type = XLV_RES_PTR;
    v->flags = 0;
    return 0;
}

int		xlv_set_ref(xl_value v,xl_value val,xl_value parent)
{
    _xlv_done(v);

    xlv_inc_ref( v->v.ref.p = val);
    xlv_inc_ref( v->v.ref.parent = parent);

    v->type = XLV_REF;
    v->flags = XLVF_NEED_DEREF;
    return 0;
}
/** PUBLIC: Set value to label function. */
int	xlv_set_func_label(xl_value v,int label_nr)
{
    v->type = XLV_FUNC_LABEL;
    v->v.func.address.label = label_nr;
    v->v.func.params = NULL;
    v->flags = 0;
    return 0;
}

/** PUBLIC: Set value to function addressed by ip. */
int	xlv_set_func_ip(xl_value v,int ip)
{
    v->type = XLV_FUNC_IP;
    v->v.func.address.ip = ip;
    v->v.func.params = NULL;
    v->flags = 0;
    return 0;
}


#ifndef XL_STR_BUF_ROW_SIZE
#define XL_STR_BUF_ROW_SIZE 200
#endif

#ifndef XL_STR_BUF_COUNT
#define XL_STR_BUF_COUNT 10
#endif

static char xl_str_buf[XL_STR_BUF_COUNT][XL_STR_BUF_ROW_SIZE];
static int  xl_str_i = 0;
#define XL_STR_I	( xl_str_i % XL_STR_BUF_COUNT )
#define XL_STR_BUF	( xl_str_buf[XL_STR_I] )
#define XL_STR_BUF_END	( XL_STR_BUF[ XL_STR_BUF_ROW_SIZE - 1 ])
#define XL_STR_BUF_LEN	XL_STR_BUF_ROW_SIZE

unsigned	xlv_str_cap(const xl_value_t* v)
{
    if( !v ) return -1;
    switch( v->type) {
    case XLV_STR:
	return xls_cap(v->v.s);
    case XLV_STR_BUF:
	return v->v.ps.capacity;
    default:
	return sizeof( XL_STR_BUF ) - 1;
    }
}

xl_str          xlv_strx(const xl_value_t* v)
{
    XL_FCTX("xlv_strx","v="XLF_PTR,v);
    v = xlv_deref(v);
    if( v->type == XLV_STR ) {
        return v->v.s;
    }
    return NULL;
}
const char*	xlv_str(const xl_value_t* v)
{
    XL_FCTX("xlv_str","v="XLF_PTR,v);
    v = xlv_deref(v);
    xl_str_i++;
    XL_STR_BUF[0] = 0;
    if( v == NULL ) {
	return XL_STR_BUF;
    }
    if( xlv_need_extract(v) || v->type == XLV_USER_VAL ) {
	xl_value_t x;
	const char* s;
	xlv_extract(v,&x);
	s = xlv_str(&x);
	strncpy(XL_STR_BUF,s,XL_STR_BUF_LEN);
	xlv_done(&x);
	XL_STR_BUF_END = 0;
	return XL_STR_BUF;
    }
    switch( v->type) {
    case XLV_STR:
	return xls_get(v->v.s);
    case XLV_STR_BUF:
	return v->v.ps.s;
    case XLV_INT:
	snprintf(XL_STR_BUF,sizeof(XL_STR_BUF),"%i",(int)v->v.i);
	XL_STR_BUF_END = 0;
	break;
    case XLV_FLOAT:
	snprintf(XL_STR_BUF,sizeof(XL_STR_BUF),"%g",v->v.f);
	XL_STR_BUF_END = 0;
	break;
    case XLV_CHAR:
	XL_STR_BUF[0] = v->v.ch;
	XL_STR_BUF[1] = 0;
	return XL_STR_BUF;
    case XLV_CHAR_REF:
	XL_STR_BUF[0] = xlv_char(v);
	XL_STR_BUF[1] = 0;
	return XL_STR_BUF;

    case XLV_MAP_OBJ:
    case XLV_RESOURCE:
    case XLV_ARRAY:
    case XLV_USER_OBJ:
	sprintf(XL_STR_BUF,"<obj(%s) 0x%08x>",((xl_object)v)->name,(int)v);
	return XL_STR_BUF;
    case XLV_RES_PTR:
	sprintf(XL_STR_BUF,"<resptr:%s:0x%08x>",
	    xlv_ptr_name(v->v.res.type),(unsigned int)v->v.res.pointer);
	return XL_STR_BUF;
    case XLV_FUNC_LABEL:
	sprintf(XL_STR_BUF,"<funclabel: label: %s : params: %i>",
	    get_label_text(v->v.func.address.label),
	    v->v.func.params ? v->v.func.params->number : 0);
	return XL_STR_BUF;
    case XLV_FUNC_IP:
	sprintf(XL_STR_BUF,"<funcip: %li>",v->v.func.address.label);
	return XL_STR_BUF;
    case XLV_FUNC_BUILTIN:
	strcpy(XL_STR_BUF,"<builtin:");
	if( v->v.func_builtin.name != NULL )
	    strncpy((XL_STR_BUF)+8,v->v.func_builtin.name,XL_STR_BUF_LEN-9);
	else
	    strcpy((XL_STR_BUF)+8,"unknown");
	strcat(XL_STR_BUF,">");
	XL_STR_BUF_END = 0;
	return XL_STR_BUF;
    default:
	return "";
    }
    return XL_STR_BUF;
}

char*	res_ptr_tab[200];
int	res_ptr_tab_init = 0;

void*		xlv_ptr(const xl_value_t* v,int type)
{
    if( v->type == XLV_RES_PTR &&
        v->v.res.type == type &&
	type < countof(res_ptr_tab) &&
	res_ptr_tab[type] )
	return v->v.res.pointer;
    return NULL;
}

int		xlv_char(const xl_value_t* v)
{
    XL_FCTX("xlv_int","v="XLF_PTR,v);
    v = xlv_deref(v);
    switch( v->type ) {
    case XLV_USER_VAL:
	{
	    xl_value_t x;
	    xl_integer i;
	    xlv_extract(v,&x);
	    i = xlv_char(&x);
	    xlv_done(&x);
	    return i;
	}
    case XLV_CHAR:
	return v->v.ch;
    case XLV_CHAR_REF:
	if( v->v.chr.parent ) {
	    const char* s = xlv_str(v->v.chr.parent);
	    unsigned cap = xlv_str_cap(v->v.chr.parent);
	    unsigned i = v->v.chr.pos;
	    if( i >= 0 && i <= cap-1 )
		return s[i];
	    else
		return 0;
	}
    case XLV_STR:
    case XLV_STR_BUF:
	return xlv_str(v)[0];
    case XLV_INT:
	return v->v.i;
    default:
	return xlv_int(v);
    }
}

xl_integer		xlv_int(const xl_value_t* v)
{
    XL_FCTX("xlv_int","v="XLF_PTR,v);
    v = xlv_deref(v);
    switch( v->type ) {
    case XLV_USER_VAL:
	{
	    xl_value_t x;
	    xl_integer i;
	    xlv_extract(v,&x);
	    i = xlv_int(&x);
	    xlv_done(&x);
	    return i;
	}
    case XLV_CHAR:
	return v->v.ch;
    case XLV_CHAR_REF:
	return (xl_integer) xlv_char(v);
    case XLV_INT:
	return v->v.i;
    case XLV_REAL:
	return (xl_integer)( v->v.f );
    case XLV_STR_BUF:
	return xl_str2int( v->v.ps.s , NULL );
    case XLV_STR:
	return xl_str2int( xls_get(v->v.s), NULL);
    default:
	return 0;
    }
}

xl_float	xlv_float(const xl_value_t* v)
{
    XL_FCTX("xlv_float","v="XLF_PTR,v);
    v = xlv_deref(v);
    switch( v->type ) {
    case XLV_USER_VAL:
	{
	    xl_value_t x;
	    xl_float f;
	    xlv_extract(v,&x);
	    f = xlv_float(&x);
	    xlv_done(&x);
	    return f;
	}
    case XLV_CHAR:
	return (xl_float)v->v.ch;
    case XLV_CHAR_REF:
	return (xl_float)xlv_char(v);
    case XLV_INT:
	return (xl_float)v->v.i;
    case XLV_REAL:
	return (v->v.f);
    case XLV_STR_BUF:
	return xl_str2float( v->v.ps.s, NULL );
    case XLV_STR:
	return xl_str2float( xls_get(v->v.s),NULL);
    default:
	return 0.0;
    }
}

void		xlv_set_parent(xl_value val,xl_value parent)
{
    XL_FCTX2("xlv_set_parent","v="XLF_PTR",p="XLF_PTR,val,parent);
    if( val == NULL )
	return;
    if( val->type == XLV_REF ) {
	val->v.ref.parent = parent;
	return;
    }
    NOT_READY;
}

void		xlv_clear(xl_value v)
{
    v->type = XLV_NIL;
    v->flags = 0;
}

/*************

    void	xlv_swrite(xl_value v,ZSTREAM f);
    void	xlv_sread(xl_value v,ZSTREAM f);

**************/

#ifndef zwritev
#define zwritev(f, v) zwrite(f, &(v), sizeof( v ))
#endif

#ifndef zreadv
#define zreadv(f, v) zread(f, &(v), sizeof( v ))
#endif

static char *read_str(ZSTREAM in, int *x)
{
    xl_int32 len;
    char *ptr;
    zreadv(in, len);
    ptr = axl_malloc(len + 1);
    if (ptr == NULL)
	return NULL;
    zread(in, ptr, len);
    ptr[len] = 0;
    if (x)
	*x = len + 1;
    return ptr;
}

int	xlv_swrite(const xl_value_t* v,ZSTREAM out)
{
    const char* out_str = NULL;
    unsigned out_str_len = 0;
    zwritev(out,v->type);
    zwritev(out,v->flags);
    switch( v->type ) {
    case XLV_CHAR:
	zwritev(out,v->v.ch);
	break;
    case XLV_INT:
	zwritev(out,v->v.i);
	break;
    case XLV_FLOAT:
	out_str = xlv_str(v);
	out_str_len = strlen(out_str);
	break;
    case XLV_STR_BUF:
	out_str = v->v.ps.s;
	out_str_len = strlen(out_str);
	break;
    case XLV_STR:
	xls_bwrite(out,v->v.s);
	break;
    case XLV_FUNC_IP:
	zwritev(out, v->v.func.address.ip);
	xlv_fp_swrite(v->v.func.params,out);
	break;
    case XLV_FUNC_LABEL:
	zwritev(out, v->v.func.address.label);
	xlv_fp_swrite(v->v.func.params,out);
	break;
    default:
	break;
    }
    if( out_str != NULL ) {
	zwritev(out,out_str_len);
	zwrite(out,out_str,out_str_len);
    }
    return 0;
}

int	xlv_sread(xl_value v,ZSTREAM in)
{
    zreadv(in, v->type);
    zreadv(in,v->flags);
    switch (v->type) {
    case XLV_CHAR:
	zreadv(in,v->v.ch);
	break;
    case XLV_INT:
	zreadv(in, v->v.i);
	break;
    case XLV_FLOAT:
	{
	    char* s = read_str(in, NULL);
	    v->v.f = strtod(s, NULL);
	    axl_free(s);
	}
	break;
    case XLV_STR:
	if( (v->v.s = xls_bread(in)) == NULL )
	    return -1;
	break;
    case XLV_FUNC_IP:
	zreadv(in, v->v.func.address.ip);
	v->v.func.params = xlv_fp_sread(in);
	break;
    case XLV_FUNC_LABEL:
	zreadv(in, v->v.func.address.label);
	v->v.func.params = xlv_fp_sread(in);
	break;
    default:
	break;
    }
    return 0;
}
/********
**** xl_func_params:

    xl_func_params	xlv_fp_dup(xl_func_params fp);
    xl_func_params	xlv_fp_new(int number);
    void		xlv_fp_free(xl_func_params fp);

    void		xlv_fp_swrite(xl_func_params fp,ZSTREAM f);
    xl_func_params 	xlv_fp_sread(ZSTREAM f);

*********/

xl_func_params xlv_fp_dup(const xl_func_params op)
{
    xl_func_params np = xlv_fp_new(op->number);
    int i;
    if( np == NULL ) return NULL;
    np->number = op->number;
    for( i = 0; i < np->number; i++ )
	xlv_set(& np->def[i], & op->def[i] );
    return np;
}

xl_func_params xlv_fp_new(int number)
{
    unsigned int size = sizeof( xl_func_params_t) + sizeof(xl_value_t) * number;
    xl_func_params np = axl_malloc( size );
    if( np == NULL ) return NULL;
    memset(np,0, size);
    MDMF_FILL(np,"xlfp");
    np->number = number;
    return np;
}

void	xlv_fp_free(xl_func_params fp)
{
    int i;
    if( fp == NULL ) return;
    for( i = 0; i < fp->number; i++ )
	xlv_done(fp->def+i);
    axl_free(fp);
}

void	xlv_fp_swrite(const xl_func_params fp,ZSTREAM f)
{
    int i = 0;
    if( fp == NULL ) {
	xl_int32 t = 0;
	zwritev(f,t);
	return;
    }
    zwritev(f,fp->number);
    for(i=0; i < fp->number; i++)
	xlv_swrite(& fp->def[i], f);
}
xl_func_params 	xlv_fp_sread(ZSTREAM in)
{
    xl_int32 number;
    zreadv(in,number);
    if( number == 0 )
	return NULL;
    else {
	int i;
	xl_func_params fp;
	fp = xlv_fp_new(number);
	if( ! fp ) return NULL;
        for(i=0; i<number; i++)
	    xlv_sread( & fp->def[i], in);
	return fp;
    }
}

int	xlv_ptr_register_type(const char* name)
{
    int i = 0;
    if( !res_ptr_tab_init ) {
	memset(res_ptr_tab,0,sizeof(res_ptr_tab));
	res_ptr_tab_init = 1;
    }
    for( i = 0; i < countof(res_ptr_tab); i++ ) {
	if( res_ptr_tab[i] && strcmp(name,res_ptr_tab[i]) == 0 )
	    return i;
	if( res_ptr_tab[i] == NULL ) {
	    res_ptr_tab[i] = (char*)name;
	    return i;
	}
    }
    return -1;
}
int	xlv_ptr_unregister_type(int type)
{
    if( type < countof(res_ptr_tab) )
	res_ptr_tab[type] = NULL;
    return 0;
}
char*	xlv_ptr_name(int type)
{
    if( type >= 0 && type < countof(res_ptr_tab) ) {
	if(  res_ptr_tab[type] )
	    return res_ptr_tab[type];
	else
	    return "not defined";
    }
    return "bad type";
}

int		xlv_ptr_type(const char* name)
{
    return xlv_ptr_register_type(name);
}
