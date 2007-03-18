/*
* File: 	valoper.c
*
* Id:           $Id: valoper.c 1.4 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:  operations on value
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
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "axl/axl.h"
#include "tools.h"
#include "zcompat/zpio.h"
#include "zcompat/zprintf.h"
#include "axl_i.h"

#ifndef XL_FLOAT_EPSILON
/**
    Two floating point values in distance less than
    xl_epsilon are treated as equal.
    So number in range (-xl_epsilon; xl_epsilon) are
    equal to 0.

    At runtine current xl_epsilon value can be read/written
    by xl_float_(get|set)_epsilon

    See xl_float_cmp
    This is default value.
*/
#define XL_FLOAT_EPSILON (1e-12)
#endif


int xlv_oper_bi( xl_value dest, const xl_value_t* src, int (*f)(xl_value dest, const xl_value_t* src));
int xlv_oper_un( xl_value dest, int (*f)(xl_value dest));

int xlv_oper_bi_dup( xl_value dest, const xl_value_t* a, const xl_value_t* b, int (*f)(xl_value dest, const xl_value_t* src));
int xlv_oper_un_dup( xl_value dest, const xl_value_t* a, int (*f)(xl_value dest));

int xlv_oper_bi( xl_value dest, const xl_value_t* src, int (*f)(xl_value dest, const xl_value_t* src))
{
    xl_value_t tdest, tsrc;
    xl_value   adest;
    const xl_value_t* asrc;
    int r;
    if( !xlv_can_write(dest) || !xlv_can_read(src) )
	return -1; /* EPERM */

    if( xlv_need_extract( src ) )
	if( (r = xlv_extract(src, &tsrc)) < 0 )
	    goto err_0;
	else
	    asrc = &tsrc;
    else
	asrc = src;

    if( xlv_need_extract( dest ) )
	if( (r = xlv_extract(dest, &tdest)) < 0 )
	    goto err_1;
	else
	    adest = &tdest;
    else
	adest = dest;

    r = f(adest,asrc);

    if( xlv_need_extract( dest ) )
	if( ( r = xlv_assign( dest, adest )) < 0 )
	    goto err_2;
err_2:
    if( adest != dest ) xlv_done(&tdest);
err_1:
    if( asrc != src ) xlv_done(&tsrc);
err_0:
    return r;
}

int xlv_oper_un( xl_value dest, int (*f)(xl_value dest))
{
    xl_value_t tdest;
    xl_value   adest;
    int r;
    if( !xlv_can_write(dest) )
	return -1; /* EPERM */

    if( xlv_need_extract( dest ) )
	if( (r = xlv_extract(dest, &tdest)) < 0 )
	    goto err_0;
	else
	    adest = &tdest;
    else
	adest = dest;

    if( ( r = f(adest) ) < 0 )
	goto err_1;

    if( xlv_need_extract( dest ) )
	if( ( r = xlv_assign( dest, adest )) < 0 )
	    goto err_1;
err_1:
    if( adest != dest ) xlv_done(&tdest);
err_0:
    return r;
}

int xlv_oper_bi_dup( xl_value dest, const xl_value_t* a, const xl_value_t* b, int (*f)(xl_value dest, const xl_value_t* src))
{
    xl_value_t tb;
    const xl_value_t*   pb;
    int r;
    if( !xlv_can_write(dest) || !xlv_can_read(a) || ! xlv_can_read(b) )
	return -1; /* EPERM */

    if( xlv_need_extract( a ) ) {
	if( (r = xlv_extract(a, dest)) < 0 )
	    return r;
    } else
	if( (r = xlv_cpy( dest , a ) ) < 0 )
	    return r;

    if( xlv_need_extract( b ) )
	if( (r = xlv_extract(b, &tb)) < 0 )
	    goto err_1;
	else
	    pb = &tb;
    else
	pb = b;

    if( (r = f(dest,pb)) < 0 )
	goto err_2;

    return 0;
err_2:
    if( pb != b ) xlv_done(&tb);
err_1:
    xlv_done(dest);
    return r;
}
int xlv_oper_un_dup( xl_value dest, const xl_value_t* a, int (*f)(xl_value dest))
{
    xl_value   pa;
    int r;
    if( !xlv_can_write(dest) || !xlv_can_read(a) )
	return -1; /* EPERM */

    if( xlv_need_extract( a ) ) {
	if( (r = xlv_extract(a, dest)) < 0 )  {
	    return r;
	}
	pa = dest;
    } else {
	if( (r = xlv_cpy( dest , a ) ) < 0 ) {
	    return r;
	}
	pa = dest;
    }

    r = f(pa);
    return r;
}
/*
#define _xlv_int( b )   xlv_int ((b))
#define _xlv_float( b ) xlv_float( (b))
#define _xlv_done( b ) xlv_done( (b ) )
*/

int xl_try_str2number(const char* s,xl_float* df,xl_integer* di)
{
    char* err = NULL;
    int state = 0;
    int words = 0;
    int word = 0;
    const char* os = s;
    for( ; *s; s++ ) {
	if( isspace(*s) ) {	/* if there is more than one word exit */
	    word = 0;
    	    do s++;
	    while( isspace(*s));
	    if( *s ) { word = 1; words++; }
	} else {
	    if( word == 0 ) {
		words++;
		word = 1;
	    }
	}
	if( words > 1 ) {
	    return 0;
	}

	if( (*s >= 'a' && *s <= 'f' )||
	    (*s >= 'A' && *s <= 'F' ) ) {
	    if( state == 0 || state == 2 )
		state = 2; /* have hex */
	    else
		return 0;
	}
	switch( *s ) {
	case '.':
	    if( state == 0 )
		state = 1; /* have float */
	    else
		return 0;
	    break;
	case 'e': case 'E':
	    if( state == 0 || state == 1 )
		state = 1; /* have float */
	    else if( state == 2 )
		/* nothing */ ; /* already is hex so don't change it */
	    else
		return 0;
	    break;
	case 'x': case 'X':
	    if( state == 0 )
		state = 2; /* have hex */
	    else
		return 0;
	    break;
	default:
	    break;
	}
    }
    s = os;
    if( state == 1 ) {	/* have float */
	*df = strtod(s,&err);
	if( !err || !err[0] || isspace(err[0]) ) {
	    return XLV_FLOAT;
	}
    }
    *di = strtol(s,&err,0);
    if( !err || !err[0] || isspace(err[0]) ) {
	return XLV_INT;
    }
    return 0;
}
int _xlv_add(xl_value dest,const xl_value_t* b)
{
    switch( dest->type ) {
    case XLV_INT:
	if( b->type == XLV_FLOAT ) {	/* promote to float */
	    dest->type = XLV_FLOAT;
	    dest->v.f = (xl_float)dest->v.i + xlv_float(b);
	} else {
	    dest->type = XLV_INT;
	    dest->v.i += _xlv_int( b );
	}
	return 0;
    case XLV_FLOAT:
	dest->v.f += _xlv_float( b );
	return 0;
    case XLV_STR:
	dest->v.s = xls_add(dest->v.s,xlv_str(b));
	return dest->v.s == NULL ? -XLE_MEMORY : 0;
    case XLV_NIL:
	return xlv_cpy(dest,b);
    default:
	return 0;
    }
}

int _xlv_sub(xl_value dest, const xl_value_t* b)
{
    switch( dest->type ) {
    case XLV_INT:
	if( b->type == XLV_FLOAT ) {	/* promote to float */
	    dest->type = XLV_FLOAT;
	    dest->v.f = (xl_float)dest->v.i - xlv_float(b);
	} else {
	    dest->type = XLV_INT;
	    dest->v.i -= _xlv_int( b );
	}
	return 0;
    case XLV_FLOAT:
	dest->v.f -= _xlv_float( b );
	return 0;
    case XLV_STR:
	{
	    xl_integer i; xl_float f;
	    switch( xl_try_str2number(xls_get(dest->v.s),&f,&i) ) {
	    case XLV_INT:
		_xlv_done(dest);
		if( b->type == XLV_FLOAT ) {	/* promote to float */
		    dest->type = XLV_FLOAT;
		    dest->v.f = (xl_float)i - xlv_float(b);
		} else {
		    dest->type = XLV_INT;
		    dest->v.i = i - xlv_int(b);
		}
		return 0;
	    case XLV_FLOAT:
		_xlv_done(dest);
		dest->type = XLV_FLOAT;
		dest->v.f = f - xlv_float(b);
		return 0;
	    default:
		return 0;
	    }
	}
	return 0;
    case XLV_NIL:
	return xlv_cpy(dest,b);
    default:
	return 0;
    }
}

int _xlv_chs(xl_value dest)
{
    switch( dest->type ) {
    case XLV_INT:
	dest->v.i = - dest->v.i;
	return 0;
    case XLV_FLOAT:
	dest->v.f = - dest->v.f;
	return 0;
    case XLV_STR:
	{
	    xl_integer i; xl_float f;
	    switch( xl_try_str2number(xls_get(dest->v.s),&f,&i) ) {
	    case XLV_INT:
		_xlv_done(dest);
		dest->type = XLV_INT;
		dest->v.i = -i;
		return 0;
	    case XLV_FLOAT:
		_xlv_done(dest);
		dest->type = XLV_FLOAT;
		dest->v.f = -f;
		return 0;
	    default:
		return 0;
	    }
	}
	return 0;
    default:
	return 0;
    }
}

int _xlv_mul(xl_value dest, const xl_value_t* b)
{
    switch( dest->type ) {
    case XLV_INT:
	if( b->type == XLV_FLOAT ) {	/* promote to float */
	    dest->type = XLV_FLOAT;
	    dest->v.f = (xl_float)dest->v.i * xlv_float(b);
	} else {
	    dest->type = XLV_INT;
	    dest->v.i *= _xlv_int( b );
	}
	return 0;
    case XLV_FLOAT:
	dest->v.f  *= _xlv_float( b );
	return 0;
    case XLV_STR:
	{
	    xl_integer i; xl_float f;
	    switch( xl_try_str2number(xls_get(dest->v.s),&f,&i) ) {
	    case XLV_INT:
		_xlv_done(dest);
		if( b->type == XLV_FLOAT ) {	/* promote to float */
		    dest->type = XLV_FLOAT;
		    dest->v.f = (xl_float)i * xlv_float(b);
		} else {
		    dest->type = XLV_INT;
		    dest->v.i = i * xlv_int(b);
		}
		return 0;
	    case XLV_FLOAT:
		_xlv_done(dest);
		dest->type = XLV_FLOAT;
		dest->v.f = f * xlv_float(b);
		return 0;
	    default:
		return 0;
	    }
	}
	break;
    case XLV_NIL:
	return xlv_cpy(dest,b);
    default:
	return 0;
    }
}

int _xlv_div(xl_value dest, const xl_value_t* b)
{
    xl_integer i;
    xl_float f;
    switch( dest->type ) {
    case XLV_INT:
	if( b->type == XLV_FLOAT ) {	/* promote to float */
	    f = _xlv_float(b);
	    if( f == 0.0 ) return -XLE_DIVZERO;
	    dest->type = XLV_FLOAT;
	    dest->v.f = (xl_float)dest->v.i / f;
	} else {
	    i = _xlv_int(b);
	    if( i == 0 ) return -XLE_DIVZERO;
	    dest->type = XLV_INT;
	    dest->v.i /= i;
	}
	return 0;
    case XLV_FLOAT:
	f = _xlv_float(b);
	if( f == 0.0 ) return -XLE_DIVZERO;
	dest->v.f /= f;
	return 0;
    case XLV_STR:
	{
	    switch( xl_try_str2number(xls_get(dest->v.s),&f,&i) ) {
	    case XLV_INT:
		_xlv_done(dest);
		if( b->type == XLV_FLOAT ) {	/* promote to float */
		    dest->type = XLV_FLOAT;
		    dest->v.f = (xl_float)i;
		    f = xlv_float(b);
		    if( f == 0.0 ) return -XLE_DIVZERO;
		    dest->v.f /= (xl_float)f;
		} else {
		    dest->type = XLV_INT;
		    dest->v.i = i;
		    i = xlv_int(b);
		    if( i == 0 ) return -XLE_DIVZERO;
		    dest->v.i /= i;
		}
		return 0;
	    case XLV_FLOAT:
		_xlv_done(dest);
		dest->type = XLV_FLOAT;
		dest->v.f = f;
		f = xlv_float(b);
		if( f == 0.0 ) return -XLE_DIVZERO;
		dest->v.f /= (xl_float)f;
		return 0;
	    default:
		return 0;
	    }
	}
	return 0;
    case XLV_NIL:
	return xlv_cpy(dest,b);
    default:
	return 0;
    }
}

int _xlv_mod(xl_value dest, const xl_value_t* b)
{
    xl_float f;xl_integer i;
    switch( dest->type ) {
    case XLV_INT:
	i = _xlv_int(b);
	if( i == 0 ) return -XLE_DIVZERO;
	dest->v.i %= i;
	return 0;
    case XLV_FLOAT:
	f = _xlv_float(b);
	if( f == 0.0 ) return -XLE_DIVZERO;
	dest->v.f = fmod( dest->v.f, f);
	return 0;
    case XLV_STR:
	{
	    switch( xl_try_str2number(xls_get(dest->v.s),&f,&i) ) {
	    case XLV_INT:
		_xlv_done(dest);
		if( b->type == XLV_FLOAT ) {	/* promote to float */
		    dest->type = XLV_FLOAT;
		    dest->v.f = (xl_float)i;
		    f = xlv_float(b);
		    if( f == 0.0 ) return -XLE_DIVZERO;
		    dest->v.f = fmod(dest->v.f,f);
		} else {
		    dest->type = XLV_INT;
		    dest->v.i = i;
		    i = xlv_int(b);
		    if( i == 0 ) return -XLE_DIVZERO;
		    dest->v.i %= i;
		}
		return 0;
	    case XLV_FLOAT:
		_xlv_done(dest);
		dest->type = XLV_FLOAT;
		dest->v.f = f;
		f = xlv_float(b);
		if( f == 0.0 ) return -XLE_DIVZERO;
		dest->v.f = fmod(dest->v.f,f);
		return 0;
	    default:
		return 0;
	    }
	}
	return 0;
    case XLV_NIL:
	return xlv_cpy(dest,b);
    default:
	return 0;
    }
}

int _xlv_l_and(xl_value dest, const xl_value_t* b)
{
    int i = xlv_true(dest) && xlv_true(b);
    _xlv_done(dest);
    dest->type = XLV_INT;
    dest->v.i = i;
    return 0;
}

int _xlv_l_or(xl_value dest,const xl_value_t* b)
{
    int i = xlv_true(dest) || xlv_true(b);
    _xlv_done(dest);
    dest->type = XLV_INT;
    dest->v.i = i;
    return 0;
}

int _xlv_l_xor(xl_value dest,const xl_value_t* b)
{
    int i = xlv_true(dest) ^ xlv_true(b);
    _xlv_done(dest);
    dest->type = XLV_INT;
    dest->v.i = i;
    return 0;
}

int _xlv_l_neg(xl_value dest)
{
    int i = !xlv_true(dest);
    _xlv_done(dest);
    dest->type = XLV_INT;
    dest->flags = 0;
    dest->v.i = i;
    return 0;
}

int _xlv_b_and(xl_value dest,const xl_value_t* b)
{
    xl_integer i = _xlv_int(dest) & _xlv_int(b);
    _xlv_done(dest);
    dest->type = XLV_INT;
    dest->v.i = i;
    return 0;
}

int _xlv_b_or(xl_value dest,const xl_value_t* b)
{
    xl_integer i = _xlv_int(dest) | _xlv_int(b);
    _xlv_done(dest);
    dest->type = XLV_INT;
    dest->v.i = i;
    return 0;
}

int _xlv_b_xor(xl_value dest,const xl_value_t* b)
{
    xl_integer i = _xlv_int(dest) ^ _xlv_int(b);
    _xlv_done(dest);
    dest->type = XLV_INT;
    dest->v.i = i;
    return 0;
}

int _xlv_b_not(xl_value dest)
{
    xl_integer i = ~xlv_int(dest);
    _xlv_done(dest);
    dest->type = XLV_INT;
    dest->v.i = i;
    return 0;
}


int	xlv_add(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_add);
}
int	xlv_sub(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_sub);
}
int	xlv_mul(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_mul);
}
int	xlv_div(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_div);
}
int	xlv_mod(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_mod);
}

int	xlv_l_and(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_l_and);
}
int	xlv_l_xor(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_l_xor);
}
int	xlv_l_or(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_l_or);
}
int	xlv_l_neg(xl_value dest,const xl_value_t* a)
{
    return xlv_oper_un_dup(dest,a,_xlv_l_neg);
}

int	xlv_b_and(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_b_and);
}

int	xlv_b_xor(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_b_xor);
}

int	xlv_b_or(xl_value dest,const xl_value_t* a,const xl_value_t* b)
{
    return xlv_oper_bi_dup(dest,a,b,_xlv_b_or);
}

int	xlv_b_not(xl_value dest,const xl_value_t* a)
{
    return xlv_oper_un_dup(dest,a,_xlv_b_not);
}

int	xlv_chs(xl_value dest,const xl_value_t* a)
{
    return xlv_oper_un_dup(dest,a,_xlv_chs);
}

static xl_float xl_epsilon = XL_FLOAT_EPSILON;

xl_float xl_float_get_epsilon()
{
    return xl_epsilon;
}

void	xl_float_set_epsilon(xl_float neps)
{
    xl_epsilon = neps;
}

int xl_float_cmp(xl_float a,xl_float b)
{
    register xl_float n = a - b;
    if( n >= 0.0 ) {
	if ( n > xl_epsilon )
	    return 1;
	return 0;
    }
    if( n < -xl_epsilon )
	return -1;
    return 0;
}

int	xlv_cmp(const xl_value_t* v1,const xl_value_t* v2)
{
/*
    zfprintf(axl_debug_stream,"xlvcmp: comparing %s:%i with %s:%i\n",xlv_str(v1),v1->type,xlv_str(v2),v2->type);
*/
    switch( v1->type ) {
    case XLV_CHAR_REF:
	return xlv_char(v1)- xlv_char(v2);
    case XLV_CHAR:
	if( v2->type == XLV_CHAR )
	    return v1->v.ch - v2->v.ch;
	else
	    return v1->v.ch - xlv_char(v2);
    case XLV_INT:
	if( v2->type == XLV_INT )
	    return v1->v.i - v2->v.i;
	else
	    return v1->v.i - _xlv_int(v2);
    case XLV_FLOAT:
	if( v2->type == XLV_FLOAT )
	    return xl_float_cmp(v1->v.f,v2->v.f);
	else
	    return xl_float_cmp(v1->v.f,_xlv_float(v2));
    case XLV_STR_BUF:
    case XLV_STR:
	return strcmp(xlv_str(v1),xlv_str(v2));
    default:
	return -2;
    }
}

int	xlv_true(const xl_value_t* v)
{
    switch( v->type ) {
    case XLV_INT:
        return v->v.i != 0;
    case XLV_CHAR:
        return v->v.ch != 0;
    case XLV_CHAR_REF:
	return 1;
    case XLV_FLOAT:
	return xl_float_cmp(v->v.f,0.0) != 0;
    case XLV_STR:
	if( xls_get(v->v.s) )
	    return xls_get(v->v.s)[0] != 0;
	else
	    return 0;
    case XLV_STR_BUF:
	if( v->v.ps.s )
	    return v->v.ps.s[0] != 0;
	else
	    return 0;
    case XLV_MAP_OBJ:
    case XLV_RESOURCE:
    case XLV_ARRAY:
    case XLV_USER_OBJ:
	return 1;
    case XLV_RES_PTR:
	return v->v.res.pointer != NULL;
    case XLV_STACK:
    case XLV_REF:
	return xlv_true( xlv_deref( v ) );
    case XLV_FUNC_BUILTIN:
	return v->v.func_builtin.address != NULL;
    case XLV_FUNC_IP:
    case XLV_FUNC_LABEL:
	return 1;
    default:
	return 0;
    }
    return 0;
}

xl_value	xlv_deref(const xl_value_t* v)
{
    while( v != NULL ) {
	switch( v->type ) {
	case XLV_ITERATOR:
	    if( v->v.iter.object )
		return NULL;
	    else
		return NULL;
	    continue;
	case XLV_USER_VAL:
	    if( xlv_need_deref(v) )
		if( v->v.user.vt->deref )
		    v =  v->v.user.vt->deref(v);
		else
		    return (xl_value)v;
	    else
		return (xl_value)v;
	    continue;
	case XLV_REF:
	    v = v->v.ref.p;
	    continue;
	case XLV_STACK:
	    v = v->v.stack.stack->ptr + v->v.stack.index;
	    continue;
	default:
	    return (xl_value)v;
	}
    }
    return (xl_value)v;
}

/*  those are in valmeth.c */
xl_object	xl_int_methods(void);
xl_object	xl_char_methods(void);
xl_object	xl_float_methods(void);
xl_object	xl_str_methods(void);

int		xlv_field(xl_value obj, const char* name, int mode,xl_value dest)
{
    xl_value v;
    XL_FCTX2("xlv_index","v="XLF_PTR",n=%s",val,xlv_str(index));
    if( !obj )
	return -XLE_INTERNAL;
    obj = xlv_deref(obj);
    switch( obj->type ) {
    case XLV_MAP_OBJ:
    case XLV_RESOURCE:
    case XLV_ARRAY:
    case XLV_USER_OBJ:
	return xlo_field((xl_object)obj,name,mode,dest);
    case XLV_USER_VAL:
	if( obj->v.user.vt->field )
	    return obj->v.user.vt->field(obj, dest, name);
	return -1;
    case XLV_INT:
	v = xlo_ffield(xl_int_methods(),name);
	if( v == NULL )
	    return -1;
	break;
    case XLV_CHAR:
    case XLV_CHAR_REF:
	v = xlo_ffield(xl_char_methods(),name);
	if( v == NULL )
	    return -1;
	break;
    case XLV_FLOAT:
	v = xlo_ffield(xl_float_methods(),name);
	if( v == NULL )
	    return -1;
	break;
    case XLV_STR_BUF:
    case XLV_STR:
	v = xlo_ffield(xl_str_methods(),name);
	if( v == NULL )
	    return -1;
	break;
    default:
	return -XLE_INTERNAL;
    }
    if( ZFL_ISSET(mode, XLVF_REF) ) {
	dest->type = XLV_REF;
	dest->v.ref.p = v;
	xlv_inc_ref(v);
	dest->v.ref.parent = obj;
	xlv_inc_ref(obj);
	return 0;
    }
    if( ZFL_ISSET(mode, XLVF_DEREF) ) {
	v = xlv_deref(v);
	return xlv_extract(v,dest);
    }
    return xlv_cpy(dest,v);
}

int		xlv_index(xl_value obj,const xl_value_t* index, int mode,xl_value dest)
{
    xl_value v;
    XL_FCTX2("xlv_index","v="XLF_PTR",n=%s",val,xlv_str(index));
    if( !obj )
	return -XLE_INTERNAL;
    obj = xlv_deref(obj);
    if( xlv_is_obj(obj) ) {
	return xlo_index((xl_object)obj,index,mode,dest);
    } else if( obj->type == XLV_USER_VAL ) {
	if( obj->v.user.vt->index )
	    return obj->v.user.vt->index(obj, dest, index);
	return -1;
    } else if( xlv_is_str(obj) ) {
	int i = xlv_int(index);
	dest->type = XLV_CHAR_REF;
	dest->v.chr.parent = obj;
	dest->v.chr.pos = i;
	return 0;
    } else {
	return -XLE_INTERNAL;
    }
    switch( mode ) {
    case XLVF_DEREF:
	v = xlv_deref(v);
	xlv_extract(v,dest);
	break;
    case XLVF_REF:
	dest->type = XLV_REF;
	dest->v.ref.p = v;
	xlv_inc_ref(v);

	dest->v.ref.parent = obj;
	xlv_inc_ref(obj);

	break;
    default:
	xlv_cpy(dest,v);
	break;
    }
    return 0;
}

int	xlv_extract(const xl_value_t* v,xl_value dest)
{
    int r;
    switch( v->type ) {
    case XLV_MREF:
        {
            xl_object o = v->v.mref.obj;
            void* p = v->v.mref.pos;
            if( !o || !o->vt || !o->vt->member_extract)
                return -1;
            return o->vt->member_extract(o,p,dest);
        }
    case XLV_USER_VAL:
	if( v->v.user.vt->extract )
	    return v->v.user.vt->extract(v,dest);
	return -1;
    case XLV_CHAR_REF:
	if (v->v.chr.parent ) {
	    if( xlv_is_str(v->v.chr.parent) ) {
		const char* s = xlv_str(v->v.chr.parent);
		unsigned cap = xlv_str_cap(v->v.chr.parent);
		unsigned i = v->v.chr.pos;
		i = zmin(cap-1,i);
		i = zmax(0,i);
		dest->type = XLV_CHAR;
		if( i >= 0 && i <= cap-1 )
		    dest->v.ch = s[i];
		else
		    dest->v.ch = 0;
		return 0;
	    }
	}
	dest->type = XLV_STR;
	dest->v.s = xlsprintf(NULL,"<badcharref v_chr_parent=\"0x%08x\">",v->v.chr.parent);
	return 0;
    case XLV_REF:
    case XLV_STACK:
	r = xlv_cpy(dest, xlv_deref(v));
	dest->flags &= ~ (XLVF_NEED_EXTRACT);
	return r;
    default:
	r = xlv_cpy(dest, v);
	dest->flags &= ~ (XLVF_NEED_EXTRACT);
	return r;
    }
    return 0;
}

int	xlv_assign(xl_value v,xl_value src)
{
    switch (v->type ) {
    case XLV_MAP_OBJ:
    case XLV_RESOURCE:
    case XLV_ARRAY:
    case XLV_USER_OBJ:
	return -XLE_INTERNAL;
    case XLV_MREF:
        {
            xl_object o = v->v.mref.obj;
            void* p = v->v.mref.pos;
            if( !o || !o->vt || !o->vt->member_assign)
                return -1;
            return o->vt->member_assign(o,p,src);
        }

    case XLV_CHAR_REF:
	if (src->v.chr.parent ) {
	    if( xlv_is_str(src->v.chr.parent) ) {
		const char* s = xlv_str(src->v.chr.parent);
		unsigned cap = xlv_str_cap(src->v.chr.parent);
		unsigned i = src->v.chr.pos;
		i = zmin(cap-1,i);
		i = zmax(0,i);
		s = s+i;    /* UNICODE FIX */
		v->type = XLV_CHAR;
		v->v.ch = s[i];
		return 0;
	    }
	}
	return -1;
    case XLV_USER_VAL:
	if( v->v.user.vt->assign )
	    return v->v.user.vt->assign(v,src);
	return -1;
    default:
	return xlv_set(v,src);
    }
    return 0;
}
