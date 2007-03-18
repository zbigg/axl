/*
* File:		objenv.c
*
* Id:           $Id: objenv.c 1.3 03/11/27 21:36:25+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	object interfacing with environment variables

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

#include <stdio.h>
#include <stdlib.h>
#include "zcompat/ztools.h"
#include "axl/axl.h"
#include "zcompat/zprintf.h"
#include "tools.h"

#include "axl_i.h"

extern xl_object xl_env_new(void);

#if 1
static int	xl_env_index(const xl_value_t* env,xl_value dest,const xl_value_t* index);
static int	xl_env_field(const xl_value_t* env,xl_value dest,const char* field);

static int	xl_envo_free(xl_value);
static int	xl_envo_dup(const xl_value_t*,xl_value);
static int	xl_envo_assign(xl_value,xl_value);
static int	xl_envo_extract(const xl_value_t*,xl_value);

static xlv_vtable_t env_vtable = {
    NULL, 		/* free */
    NULL, 		/* dup */
    xl_env_field,	/* field */
    xl_env_index,	/* index */
    NULL, 		/* assign	*/
    NULL, 		/* deref	*/
    NULL, 		/* extract	*/
    NULL  		/* oper		*/
};

static xlv_vtable_t envo_vtable = {
    xl_envo_free, 	/* free		*/
    xl_envo_dup, 	/* dup		*/
    NULL,		/* field	*/
    NULL,		/* index	*/
    xl_envo_assign, 	/* assign	*/
    NULL, 		/* deref	*/
    xl_envo_extract, 	/* extract	*/
    NULL  		/* oper		*/
};

static int	xl_env_field(const xl_value_t* env,xl_value dest,const char* field)
{
    dest->type = XLV_USER_VAL;
    dest->flags = XLVF_NEED_EXTRACT;
    dest->v.user.vt = & envo_vtable;
    dest->v.user.param = xls_new( field );
    return 0;
}

static int	xl_env_index(const xl_value_t* env,xl_value dest,const xl_value_t* index)
{
    const char* env_name = xlv_str( index );
    dest->type = XLV_USER_VAL;
    dest->flags = XLVF_NEED_EXTRACT;
    dest->v.user.vt = & envo_vtable;
    dest->v.user.param = xls_new( env_name );
    return 0;
}



extern xl_object xl_env_new(void)
{
    xl_value v = xlv_new(NULL);
    v->type 	= XLV_USER_VAL;
    v->flags 	= XLVF_READ_ONLY;
    v->v.user.vt = &env_vtable;
    v->v.user.param = 0;
    return (xl_object)v;
}


static int	xl_envo_free(xl_value v)
{
    xls_free((xl_str)v->v.user.param);
    return 0;
}
static int	xl_envo_dup(const xl_value_t* v,xl_value dest)
{
    dest->v.user.vt     = v->v.user.vt;
    dest->v.user.param  = xls_new_xls((xl_str) v->v.user.param);
    dest->flags         = v->flags;
    return 0;
}

static int	xl_envo_assign(xl_value v,xl_value src)
{
    const char* ename = xls_str( (xl_str) v->v.user.param);

    const char* value = xlv_str(src);

    if( ename && value ) {
#ifdef HAVE_SETENV
	setenv(ename,value,1);
#else
	char *s = zsaprintf(NULL,0,"%s=%s",ename,value);

	putenv(s);

	zpio_free(s);
#endif
    }
    return 0;
}
static int	xl_envo_extract(const xl_value_t* v,xl_value dest)
{
    char* ename = xls_str( (xl_str) v->v.user.param);
    char* value;
    xlv_clear(dest);
    value = getenv(ename);
    xlv_set_str(dest, value ? value : "");
    return 0;
}


/*
static int	xl_envo_free(xl_value v)
{
    return 0;
}
static int	xl_envo_dup(xl_value v,xl_value dest);
{
    dest->v.user.vt     = v->v.user.vt;
    dest->v.user.param  = HERE_YOUR_DUP_FUNCTION(v->v.user.param);
    return 0;
}
static int	xl_envo_field(xl_value v,xl_value dest,const char* name,int flags)
{
    return -1;
}
static int	xl_envo_index(xl_value v,xl_value dest,const xl_value idx,int flags)
{
    return -1;
}
static int	xl_envo_assign(xl_value v,xl_value src)
{
    return -1;
}
static int	xl_envo_extract(xl_value v,xl_value dest)
{
    return -1;
}
static xl_value	xl_envo_deref(xl_value v)
{
    return NULL;
}
*/
#else
static xlo_vtable_t vtable;
static xl_bool	    vtable_ok = xl_false;

static int	    vt_field(const xl_object_t* obj,const xl_value_t* index,int mode,xl_value dest)
{
    dest->type = 0;
}
static int	    vt_index(const xl_object_t* obj,const xl_value_t* index,int mode,xl_value dest);
static int	    vt_member_assign(const xl_value_t* obj,void* position,const xl_value_t* src);
static int	    vt_member_extract(const xl_value_t* obj,void* position,xl_value dest);

extern xl_object xl_env_new(void)
{
    xl_object* a = xlo_alloc(0);
    if( ! vtable_ok ) {
	memset(&vtable,0,sizeof(vtable));
    }
    a->vt = vtable;
    return XL_OBJECT(a);
}
#endif
