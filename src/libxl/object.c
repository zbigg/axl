/*
* File: object.c
*
* Id:           $Id: object.c 1.7 04/01/31 00:13:51+01:00 zbigg@nowy.dom $
*
* Project:	axl
*
* Description:

* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/
/*
## =Module AXL Object API
##
##  =Description
##

##  =Return value
##
##
*/
#if !defined NDEBUG
#define XL_TRACE_OBJ_LIFE 1
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>

#include "axl/axl.h"
#include "tools.h"

#include "axl_i.h"


/* GLOBALS */
xl_object       xlo_new_name(const char* name);
xl_object       xlo_new_namex(const xl_str_t* namex);

xl_object	xlo_new_map(void);
xl_object	xlo_new_user(void);

void            xlo_register_constructor( const xl_str_t* name, xl_object (*f)(void));
int             xlo_unregister_constructor( const char* name);

void		xlo_inc_ref(xl_object o);
void		xlo_dec_ref(xl_object o);
xl_object	xlo_copy(const xl_object_t* o);
void		xlo_done(xl_object o);
void		xlo_frees(xl_object o);
void		xlo_free(xl_object o);

int		xlo_field(const xl_object_t* obj, const char* name, int mode,xl_value dest);
xl_value	xlo_ffield(xl_object obj,const char* name);

int		xlo_index(const xl_object_t* obj,const xl_value_t* idx, int mode,xl_value dest);
xl_value	xlo_findex(xl_object obj,const xl_value_t* index);

int		xlo_desc_add_builtin(xl_object obj,xl_builtin_func);
/* LOCAL */
static void	append_members(xl_object dest,const xl_object_t* src);
static void 	xlv_enum_free(xl_value v);

#ifdef XL_TRACE_OBJ_LIFE
static int	live_objects = 0;
static int      TRACE_OBJECT_REF = -1;
xl_bool     xl_trace_object_life = xl_true;
xl_bool     xl_trace_object_referencing = xl_true;
#endif


xl_object	xlo_alloc(size_t size)
{
    xl_object a = axl_calloc(1,size ? size : sizeof(xl_object_t));
    if( !a ) return NULL;
    a->type = XLV_MAP_OBJ;
    a->map = 0;
    a->ref_count = 1;
    return a;
}
/*
*/
static xlmap* constructor_map = NULL;
static void init_default_constructors(void);

#ifdef XL_TRACE_OBJ_LIFE
static void trace_action(xl_object obj,const char* action)   {
    char* descr = obj->name;
    if( TRACE_OBJECT_REF < 0 )
        TRACE_OBJECT_REF = xldbg_get_mask("object-ref");
    if( (!descr || !descr[0]) && obj->vt && obj->vt->class_name )
        descr = obj->vt->class_name;
    xl_trace(TRACE_OBJECT_REF,"XLDBG(objects:%02i): %s OBJ(%s) 0x%08x rc=%i\n",live_objects,action,descr,obj,obj->ref_count);
}
#endif

/*
## =Function xlo_new_name, xlo_new_namex
##
## =Synopsis
##  |   xl_object       xlo_new_name(const char* name);
##  |   xl_object       xlo_new_namex(const xl_str_t* name);
##  =Description
##
##
##
##  =Return value
##	    These functions return pointer to new xl_object that has
##	    reference_count set to 1.
##
##	    After using this pointer should be released by <xlo_free>.
##
*/
xl_object       xlo_new_name(const char* name)
{
    xl_object (*obj_constructor)(void);
    xl_object obj = NULL;
    if( !constructor_map ) init_default_constructors();
    if( !constructor_map ) return NULL;
    obj_constructor = xlmap_find_str(constructor_map, name);
    if( !obj_constructor ) return NULL;
    obj = obj_constructor();

#ifdef XL_TRACE_OBJ_LIFE
    ++live_objects;
    if( xl_trace_object_life ) trace_action(obj,"created by name");
#endif
    return obj;
}


xl_object       xlo_new_namex(const xl_str_t* namex)
{
    xl_object (*obj_constructor)(void);
    xl_object obj = NULL;
    if( !constructor_map ) init_default_constructors();
    if( !constructor_map ) return NULL;
    obj_constructor = xlmap_find(constructor_map, (xl_str)namex);
    if( !obj_constructor ) return NULL;
    obj = obj_constructor();
#ifdef XL_TRACE_OBJ_LIFE
    ++live_objects;
    if( xl_trace_object_life ) trace_action(obj,"created by namex");
#endif
    return obj;
}

xl_object	xlo_map_new(void)
{
    xl_object a = xlo_alloc(0);
    if( !a ) return NULL;
    MDMF_FILL(a,"xlom");
    a->type = XLV_MAP_OBJ;
    a->flags = 0;
    a->map = 0;
    a->ref_count = 1;
    sprintf(a->name,"map");

#ifdef XL_TRACE_OBJ_LIFE
    ++live_objects;
    if( xl_trace_object_life ) trace_action(a,"created map");
#endif

    return a;
}
/*
## =Function xlo_new_map
##
## =Synopsis
##  |   xl_object       xlo_new_map(void)
##
##  =Description
##
##	    Create new <map> object.
##	    The new object has empty members map.
##
##  =Return value
##	    The function returns pointer to new xl_object that has
##	    reference_count set to 1.
##
##	    After using this pointer should be released by <xlo_free>.         
*/
xl_object       xlo_new_map(void)
{
    return xlo_map_new();
}
/*
## =Function xlo_copy
##      create copy of xl_object
## =Synopsis
##  |   xl_object	xlo_copy(const xl_object_t* o)
##
##  =Description
##      Create copy of xl_object.
##
##      A new instance of xl_object is created. All fields and objects 
##      contained in <o> are copied, so there are no references between 
##      <o> and created object.
##
##  =Return value
##	The function returns pointer to new xl_object that has
##	reference_count set to 1.
##
##	After using this pointer should be released by <xlo_free>. 
*/
xl_object	xlo_copy(const xl_object_t* o)
{
    xl_object onew;
    if( ! o ) return NULL;
    switch( o->type ) {
    case XLV_MAP_OBJ:
	onew = xlo_alloc(0);
	break;
    case XLV_USER_OBJ:
	if( o->vt && o->vt->copy )
	    return o->vt->copy(o);
	else
	    return NULL;
    default:
	return NULL;
    }

#ifdef XL_TRACE_OBJ_LIFE
    ++live_objects;
    if( xl_trace_object_life ) trace_action(onew,"copy");
#endif

    MDMF_FILL(onew,"xlo!");

    onew->type = o->type;
    onew->ref_count = 1;

    if( o != NULL )
	append_members(onew,o);
    sprintf(onew->name,o->name);
    return onew;
}

void		xlo_done(xl_object o)
{
    xlo_frees(o);
}

void		xlo_frees(xl_object o)
{
    if( !o ) return;
    --(o->ref_count);
#ifdef XL_TRACE_OBJ_LIFE
    if( xl_trace_object_referencing ) trace_action(o,"releasing");
#endif
    if( o->ref_count <= 0 ) {

#ifdef XL_TRACE_OBJ_LIFE
        --live_objects;
        if( xl_trace_object_life ) trace_action(o,"deleted");
#endif
	if( o->map && !ZFL_ISSET(o->flags,XLOF_MAP_PUBLIC) ) {
	    void xlv_enum_free(xl_value v);
	    sbtree_free(o->map,(sbtree_free_f)xlv_enum_free);
	}

	if( o->type == XLV_USER_OBJ && o->vt && o->vt->free )
	    o->vt->free(o);
    }
}

static void xlv_enum_free(xl_value v)
{
    if( v->type == XLV_REF )
	v->v.ref.parent = NULL;
    xlv_free(v);
}
/*
## =Function xlo_free
##      release a xl_object
## =Synopsis
##  |   void		xlo_free(xl_object o)
##
##  =Description
##
##      Free reference to xl_object. Don't use <o> pointer
##      after calling xlo_free on it unless you're sure what 
##      are you doing.
##      
##      If reference count of this object reached 0 then
##      it's memory and atached resources are released
##      back to system.
##      
*/
void		xlo_free(xl_object o)
{
    if( !o || !xlo_is_obj(o) ) return;
    xlo_frees(o);
    if( o->ref_count <= 0 ) {
	axl_free(o);
    }
}

static void		append_members(xl_object dest,const xl_object_t* src)
{
    if( src == NULL || dest == NULL || !xlo_is_obj(src) || !xlo_is_obj(dest))
	return;
    if( src->map ) {
	xl_value v;
	if( ! dest->map )
	    dest->map = sbtree_new();
    
	SBTI_ENUM_BEGIN(src->map,old,key,xl_value)
	    old = xlv_deref(old);
	    if( xlv_is_obj(old) )
		v = (xl_value) xlo_copy((xl_object)old);
	    else
		v = xlv_new(old);
	    sbtree_add(dest->map,key,v);
	SBTI_ENUM_END;
    }
}
/*
## =Function xlo_free_member
##          delete member of xl_object
## =Synopsis
##  |   void		xlo_free_member(xl_object obj,const char* name)
##
##  =Description
##
##		Delete member of object.
*/
void		xlo_free_member(xl_object obj,const char* name)
{
    if( !obj || !xlo_is_obj(obj) || !obj->map )
	return;
    else if( obj->type == XLV_USER_OBJ ) {
	if( obj->vt && obj->vt->free_member )
	    obj->vt->free_member(obj,name);
    } else {
	xl_value v = (xl_value) sbtree_del(obj->map,name);
	if( v )
	    xlv_free(v);
    }
}
/*
## =Function xlo_field
##
## =Synopsis
##  |   int xlo_field(const xl_object_t* obj, const char* name, 
##  |                 int mode,xl_value dest);
##
##  =Description
##		If <flags> is 0 then no new element is created else
##		new is created when specified by <idx> doesn't
##		exists.
##  =Return value
##		Pointer to value which is specfied element of array.
*/
int		xlo_field(const xl_object_t* obj, const char* name, int mode,xl_value dest)
{
    xl_value v;
    if( !obj || !xlo_is_obj(obj) || !name || !dest )
	return -1;
    switch( obj->type ) {
    case XLV_MAP_OBJ:
	if( !obj->map && ZFL_ISSET(mode, XLVF_CREATE) ) {
	    ((xl_object)obj)->map = sbtree_new();
	}
	if( obj->map ) {
	    if( (v = (xl_value) sbtree_find(obj->map,name)) ) {
		xlv_set_ref(dest,v,(xl_value)obj);
		return 0;
	    }
	    /* no member was found */
	    if( ZFL_ISSET(mode, XLVF_CREATE) ) {
		v = xlv_new(NULL);
		v->type = XLV_NIL;
		if( sbtree_add(obj->map,name,v) < 0 ) {
		    xlv_free(v);
		    return -1;
		}
		xlv_set_ref(dest,v,(xl_value)obj);
		return 0;
	    } else
		return -1;
	}
	break;
    case XLV_USER_OBJ:
	{
	    int r = -1;
	    if( obj->map ) {
		if( (v = (xl_value) sbtree_find(obj->map,name)) ) {
		    xlv_set_ref(dest,v,(xl_value)obj);
		    return 0;
		}
	    }
	    if( obj->vt && obj->vt->field )
		r = obj->vt->field(obj,name,mode,dest);
	    /* in case nothing found */
	    if( r < 0 && ZFL_ISSET(mode, XLVF_CREATE) ) {
		/* we can try create an element, if it's allowed */
		if( !obj->map )
		    ((xl_object)obj)->map = sbtree_new();
		v = xlv_new(NULL);
		v->type = XLV_NIL;
		if( sbtree_add(obj->map,name,v) < 0 ) {
		    xlv_free(v);
		    return -1;
		}
		xlv_set_ref(dest,v,(xl_value)obj);
		return 0;
	    }
	    return r;
	}
    default:
	return -1; /* NOT AN OBJECT !!! */
    }
    return 0;
}
xl_value	xlo_ffield(xl_object obj,const char* name)
{
    xl_value_t dest;
    xlv_clear(&dest);
    if( xlo_field(obj,name,XLVF_CREATE | XLVF_REF, &dest) < 0 )
	return NULL;
    if( dest.type == XLV_REF && dest.v.ref.p ) {
	xlo_dec_ref(obj);
	return dest.v.ref.p;
    }
    return NULL;
}
/*
## =Function xlo_index
##
## =Synopsis
##  |   int	xlo_index(const xl_object_t* obj,const xl_value_t* idx, 
##  |                     int mode,xl_value dest);
##
##	=Description
##		create copy of xl_object
##		If <flags> is 0 then no new element is created else
##		new is created when specified by <idx> doesn't
##		exists.
##		
##	=Return value
##		Pointer to value which is specfied element of array.
##
*/
int		xlo_index(const xl_object_t* obj,const xl_value_t* idx, int mode,xl_value dest)
{
    xl_value v;
    const char* name;
    if( !obj || !xlo_is_obj(obj) || !idx || !dest )
	return -1;
    switch( obj->type ) {
    case XLV_MAP_OBJ:
	name = xlv_str(idx);
	if( !obj->map && ZFL_ISSET(mode, XLVF_CREATE) ) {
	    ((xl_object)obj)->map = sbtree_new();
	}	
	if( obj->map ) {
	    if( (v = (xl_value) sbtree_find(obj->map,name)) ) {
		xlv_set_ref(dest,v,(xl_value)obj);
		return 0;
	    }
	    /* no member was found */
	    if( ZFL_ISSET(mode, XLVF_CREATE) ) {
		v = xlv_new(NULL);
		v->type = XLV_NIL;
		if( sbtree_add(obj->map,name,v) < 0 ) {
		    xlv_free(v);
		    return -1;
		}
		xlv_set_ref(dest,v,(xl_value)obj);
		return 0;
	    } else
		return -1;
	}
	break;
    case XLV_USER_OBJ:
	if( obj->vt && obj->vt->index )
	    return obj->vt->index(obj,idx,mode,dest);
	return -1;
    default:
	return -1; /* NOT AN OBJECT !!! */
    }
    return 0;
}

xl_value	xlo_findex(xl_object obj,const xl_value_t* index)
{
    xl_value_t dest;
    xlv_clear(&dest);
    if( xlo_index(obj,index,XLVF_CREATE | XLVF_REF, &dest) < 0 )
	return NULL;
    if( dest.type == XLV_REF && dest.v.ref.p ) {
	xlo_dec_ref(obj);
	return dest.v.ref.p;
    }
    return NULL;
}

xl_value	xlo_add_field_int(xl_object o,const char* name,xl_integer i)
{
    xl_value a = xlo_ffield(o,name);
    if( a != NULL )
	xlv_set_int(a,i);
    return a;
}

xl_value	xlo_add_field_float(xl_object o,const char* name,xl_integer f)
{
    xl_value a = xlo_ffield(o,name);
    if( a != NULL )
	xlv_set_float(a,f);
    return a;
}
xl_value	xlo_add_field_str(xl_object o,const char* name,const char* s)
{
    xl_value a = xlo_ffield(o,name);
    if( a != NULL )
	xlv_set_str(a,s);
    return a;
}
/*
xl_value	xlo_add_field_func(xl_object o,const char* name,xl_builtin_func f)
{
    xl_value a = xlo_field(o,name);
    if( a != NULL )
	xlv_set_ref(a,xlb_func(f));
    return a;
}
*/
xl_value	xlo_add_field_ref(xl_object o,const char* name,xl_value v)
{
    xl_value a = xlo_ffield(o,name);
    if( a != NULL )
	xlv_set_ref(a,v,NULL);
    return a;
}

xl_value	xlo_add_field_func_builtin(xl_object o,const char* name,xl_builtin_func fadr,const char* symb_name)
{
    xl_value a = xlo_ffield(o,name);
    if( a != NULL )
	xlv_set_func_builtin(a,fadr,symb_name);
    return a;
}
xl_value	xlo_add_field_func_ip(xl_object o,const char* name,int fip)
{
    xl_value a = xlo_ffield(o,name);
    if( a != NULL )
	xlv_set_func_ip(a,fip);
    return a;
}
xl_value	xlo_add_field_func_label(xl_object o,const char* name,int labelnr)
{
    xl_value a = xlo_ffield(o,name);
    if( a != NULL )
	xlv_set_func_label(a,labelnr);
    return a;
}

xl_value	xlo_add_index_int(
    xl_object o,int idx,xl_integer i
    )
{
    xl_value v;
    xl_value_t x;
    x.type = XLV_INT;
    x.flags = 0;
    x.v.i = idx;
    v = xlo_findex(o,&x);
    if( v != NULL )
	xlv_set_int(v,i);
    return v;
}
xl_value	xlo_add_index_float(
    xl_object o,int idx,xl_integer f
    )
{
    xl_value v;
    xl_value_t x;
    x.type = XLV_INT;
    x.flags = 0;
    x.v.i = idx;
    v = xlo_findex(o,&x);
    if( v != NULL )
	xlv_set_float(v,f);
    return v;
}

xl_value	xlo_add_index_str(xl_object o,
    int idx,const char* s
    )
{
    xl_value v;
    xl_value_t x;
    x.type = XLV_INT;
    x.flags = 0;
    x.v.i = idx;
    v = xlo_findex(o,&x);
    if( v != NULL )
	xlv_set_str(v,s);
    return v;
}
/*
## =Function xlo_inc_ref
##
## =Synopsis
##  |   void		xlo_inc_ref(xl_object o);
##
##  =Description
##
##
##
##  =Return value
##
##
*/
void		xlo_inc_ref(xl_object o)
{
    o->ref_count++;
#ifdef XL_TRACE_OBJ_LIFE
    if( xl_trace_object_referencing ) trace_action(o,"referenced");
#endif
}
/*
## =Function xlo_dec_ref
##
## =Synopsis
##  |   void		xlo_dec_ref(xl_object o);
##
##  =Description
##
##
##
##  =Return value
##
##
*/
void		xlo_dec_ref(xl_object o)
{
    xlo_frees(o);
}
static xl_object _new_array(void)
{
    return xl_array_new(1);
}
static void init_default_constructors(void)
{
    xl_str name = xls_new("");
    xlo_register_constructor(name=xls_set(name,"array"), _new_array);
    xlo_register_constructor(name=xls_set(name,"map"), xlo_new_map);
    xls_free(name);
}
void	    xlo_done_module(void)
{
    if( constructor_map ) {
        xlmap_free(constructor_map);
        constructor_map = NULL;
    }
}


void       xlo_register_constructor( const xl_str_t* name, xl_object (*f)(void))
{
    if( !constructor_map ) {
        constructor_map = xlmap_new(NULL);
        if( !constructor_map ) { return; }
        init_default_constructors();
        xl_atexit(xlo_done_module);
    }
    xlmap_add(constructor_map,name,f);
}
int         xlo_unregister_constructor( const char* name)
{
    xlmi_t iter;
    if( !constructor_map ) return -1;
    xlmap_find_str_i(constructor_map,name,&iter);
    if( !iter.item ) return -1;
    xlmap_del_i(constructor_map,&iter);
    return 0;
}
