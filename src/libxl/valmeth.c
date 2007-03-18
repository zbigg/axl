/*
* File: 	valmeth.c
*
* Id:           $Id: valmeth.c 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:  all methods that can be called on
*	        simple values
*
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2002,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*
* TODO:
    1. add fmt parameter to metods: intm_2str and float_2str
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

XLF_DECL(basic_return_self);
XLF_DECL(basic_2int);
XLF_DECL(basic_2char);
XLF_DECL(basic_2string);
XLF_DECL(basic_2float);

XLF_DECL(strm_length);
XLF_DECL(strm_size);
XLF_DECL(strm_shift);
XLF_DECL(strm_trim);

XLF_DECL(intm_2str);

XLF_DECL(floatm_2str);

xl_object	xl_int_methods(void);
xl_object	xl_char_methods(void);
xl_object	xl_float_methods(void);
xl_object	xl_str_methods(void);

static xl_object into = NULL;
static xl_object charo = NULL;
static xl_object floato = NULL;
static xl_object stro = NULL;

void	    xlv_done_methods(void)
{
    xlo_free(into);
    xlo_free(charo);
    xlo_free(floato);
    xlo_free(stro);
}
xl_object	xl_int_methods(void)
{
    if( into != NULL )
	return into;
    into = xlo_new_map();
    strcpy(into->name,".axl.into");
    xlo_add_field_func_builtin(into,"integer",basic_return_self,NULL);
    xlo_add_field_func_builtin(into,"char",basic_2char,NULL);
    xlo_add_field_func_builtin(into,"float",basic_2float,NULL);
    xlo_add_field_func_builtin(into,"string",intm_2str,NULL);

    return into;
}

xl_object	xl_char_methods(void)
{
    if( charo != NULL )
	return charo;
    charo = xlo_new_map();
    strcpy(charo->name,".axl.charo");

    xlo_add_field_func_builtin(charo,"integer",basic_2int,NULL);
    xlo_add_field_func_builtin(charo,"char",basic_return_self,NULL);
    xlo_add_field_func_builtin(charo,"float",basic_2float,NULL);
    xlo_add_field_func_builtin(charo,"string",basic_2string,NULL);

    return charo;
}

xl_object	xl_float_methods(void)
{
    if( floato != NULL )
	return floato;
    floato = xlo_new_map();
    strcpy(floato->name,".axl.floato");

    xlo_add_field_func_builtin(floato,"integer",basic_2int,NULL);
    xlo_add_field_func_builtin(floato,"char",basic_2char,NULL);
    xlo_add_field_func_builtin(floato,"float",basic_return_self,NULL);
    xlo_add_field_func_builtin(floato,"string",floatm_2str,NULL);

    return floato;
}

xl_object	xl_str_methods(void)
{
    if( stro != NULL )
	return stro;
    stro = xlo_new_map();

    strcpy(stro->name,".axl.stro");
    xlo_add_field_func_builtin(stro,"integer",basic_2int,NULL);
    xlo_add_field_func_builtin(stro,"float",basic_2float,NULL);
    xlo_add_field_func_builtin(stro,"char",basic_2char,NULL);
    xlo_add_field_func_builtin(stro,"string",basic_return_self,NULL);

    xlo_add_field_func_builtin(stro,"length",strm_length,NULL);
    xlo_add_field_func_builtin(stro,"size",strm_size,NULL);
    xlo_add_field_func_builtin(stro,"shift",strm_shift,NULL);
    xlo_add_field_func_builtin(stro,"trim",strm_trim,NULL);

    return stro;
}

XLF_DECL(basic_return_self)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	if( argc >= 1 ) {
	    this = XLF_ARG(0);
	    if( this == NULL)
		XLF_RETURN_NIL();
	}
    XLF_RETURN_VAL(this);
}

XLF_DECL(basic_2int)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	if( argc >= 1 ) {
	    this = XLF_ARG(0);
	    if( this == NULL)
		XLF_RETURN_NIL();
	}
    XLF_RETURN_INT(xlv_int(this));
}

XLF_DECL(basic_2char)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	if( argc >= 1 ) {
	    this = XLF_ARG(0);
	    if( this == NULL)
		XLF_RETURN_NIL();
	}
    XLF_RETURN_CHAR(xlv_char(this));
}

XLF_DECL(basic_2float)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	if( argc >= 1 ) {
	    this = XLF_ARG(0);
	    if( this == NULL)
		XLF_RETURN_NIL();
	}
    XLF_RETURN_FLOAT(xlv_float(this));
}

XLF_DECL(basic_2string)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	if( argc >= 1 ) {
	    this = XLF_ARG(0);
	    if( this == NULL)
		XLF_RETURN_NIL();
	}
    XLF_RETURN_STR(xlv_str(this));
}

XLF_DECL(strm_length)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	XLF_RETURN_INT(0);
    XLF_RETURN_INT( strlen ( xlv_str(this)) );
}
XLF_DECL(strm_size)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	XLF_RETURN_INT(0);
    XLF_RETURN_INT( strlen ( xlv_str(this)) + 1 );
}

XLF_DECL(strm_trim)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	XLF_RETURN_STR("");
    switch( this->type ) {
    case XLV_STR:
	xls_erase_blank(this->v.s);
	break;
    case XLV_STR_BUF:
	break;
    default:
	break;
    }
    XLF_RETURN_REF( this );
}

XLF_DECL(strm_shift)
{
    xl_value this = XLF_THIS;
    int c;
    xl_str ss;
    if( this == NULL)
	XLF_RETURN_INT(0);
    if( this->type != XLV_STR )
	XLF_RETURN_INT(0);
    ss = this->v.s;
    if( xls_len(ss) > 0 ) {	/* do a shift */
	char* s  = xls_ptr(ss);
	char* n  = s+1;
	c = s[0];
	memmove(s,n, xls_len(ss)-1);
	this->v.s->length--;
    } else {			/* nothing more to shift, so return NULL */
	c = '\0';
    }
    ret->type = XLV_CHAR;
    ret->v.ch = c;
    return 0;
}


/* TODO: add fmt parameter */
XLF_DECL(intm_2str)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	if( argc >= 1 ) {
	    this = XLF_ARG(0);
	    if( this == NULL)
		XLF_RETURN_NIL();
	}
    XLF_RETURN_STR(xlv_str(this));
}

/* TODO: add fmt parameter */
XLF_DECL(floatm_2str)
{
    xl_value this = XLF_THIS;
    if( this == NULL)
	if( argc >= 1 ) {
	    this = XLF_ARG(0);
	    if( this == NULL)
		XLF_RETURN_NIL();
	}
    XLF_RETURN_STR(xlv_str(this));
}
