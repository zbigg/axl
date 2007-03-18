/*
* File:		objres.c
*
* Id:           $Id: objres.c 1.3 03/11/27 21:36:25+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	object containing some internal (program or system) handle

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

#include "zcompat/ztools.h"
#include "axl/axl.h"
#include "zcompat/zprintf.h"
#include "tools.h"

#include "axl_i.h"

/* INTERFACE */
/*typedef void (*xl_res_destructor)(void* res);*/

typedef struct {
    XL_OBJECT_HEADER

    int                 res_type;
    void*		res;
    xl_res_destructor destr;
} xl_res;

xl_object	xl_res_new(void* resource,  int type,xl_res_destructor destr);


/* 

  IMPLEMENTATION 
  
*/

/* 
    FORWARD DECLARATIONS 
 */

static void vt_free(xl_res* obj);

/*static struct _xlo_vtable  res_vtable;
static int	    res_vtable_ok = 0;
*/

/* 
    GLOBALS 
*/

XLO_DECLARE_VT(resource)

/*
## =Function xl_res_new
##      create new xl_object , type resource
## =Synopsis
##  |   xl_object	xl_res_new(void* resource, int type, xl_res_destructor destr)
##
##  =Description
##          TODO:   update  documentation for new resource semantics.
##
##	    Create new <resource> object.
##
##	    <resource> objects contain generic C pointer which
##	    holds some resource, reference count, methods and
##	    destructor function.
##
##	    The <res> argument is pointer to resource. The <map>
##	    argument contains methods that can be called on this
##	    resource by AXL program. If <map_copy> is true the new
##	    object will contain a copy of the methods, else it will
##	    use this specific map object <map> that should live
##	    as long as the new object.
##
##	    The <destructor> argument is pointer to "free resource"
##	    callback function which is called when object is destroyed
##	    by garbage collector.
##
##
##  =Return value
##	    The function returns pointer to new xl_object that has
##	    reference_count set to 1.
##
##	    After using this pointer should be released by <xlo_free>.
*/
xl_object	xl_res_new(void* resource, int type, xl_res_destructor destr)
{
    xl_res* a;
    XLO_VT_INIT_BEGIN(resource)
        XLO_VT_CALLBACK(free, vt_free)
    XLO_VT_INIT_END(resource)

    a = axl_calloc( 1, sizeof( xl_res ));
    if( !a ) return NULL;

    a->type = XLV_USER_OBJ;
    a->vt = XLO_VT(resource);

    a->res = resource;
    a->destr = destr;
    a->res_type = type;

    a->ref_count = 1;

    return XL_OBJECT(a);
}
/*
    LOCALS    
*/
static void vt_free(xl_res* obj)
{
    if( !obj ) return;
    if ( obj->destr ) {
	obj->destr(obj->res);
	obj->res = NULL;
    }
}

void* xl_res_get(const xl_object_t* o,int type)
{
    xl_res* ro = (xl_res*)o;
    if( !XLO_CHECK_VT(o,resource) ) return NULL;
    if( ro->res_type != type ) return NULL;
    return ro->res;
}

void xl_res_set(xl_object_t* o,void* res,int type)
{
    xl_res* ro = (xl_res*)o;
    if( !XLO_CHECK_VT(o,resource) ) return;
    if( !ro->res_type == type ) return;
    ro->res = res;
}
