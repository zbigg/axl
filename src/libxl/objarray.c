/*
* File:		objarray.c
*
* Id:           $Id: objarray.c 1.6 04/02/07 22:24:32+01:00 nikt@nowy.dom $
*
* Project:	axl
*
* Description:	object containing plain integer intexed array of
*		simple values

* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights arrayerved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include "axl/axl.h"
#include "tools.h"

#include "axl_i.h"

/* INTERFACE */
typedef void (*xl_array_destructor)(void* array);

typedef struct xl_array {
    XL_OBJECT_HEADER

    xl_value_t*	array;
    size_t	count;
    size_t	capacity;
} xl_array;

xl_object	xl_array_new(int initial_size);


xlo_field_desc*	
        xlo_find_field(xlo_field_desc* fd, int count, const char* name);
int	xlo_extract_field(void* s, xlo_field_desc* field, xl_value dest);
int	xlo_assign_field(void* s, xlo_field_desc* field, xl_value v);
void	xlo_sort_field_desc(xlo_field_desc* fd, int count);


/* IMPLEMENTATION */

/* FORWARD DECLARATIONS */

static xl_array* vt_dup(const xl_array* a);
static void	vt_free(xl_array* obj);
static int      vt_field(const xl_array* a,const char* field_name,int mode,xl_value dest);
static int      vt_index(const xl_array* a,const xl_value_t* index,int mode,xl_value dest);
static int      vt_member_assign(const xl_array* a,xlo_field_desc* index,const xl_value_t* src);
static int      vt_member_extract(const xl_array* a,xlo_field_desc* index,xl_value dest);

static void	do_done_range(xl_array* a,size_t begin,size_t end);
static void	do_done(xl_array* a);
static xl_bool	do_set_capacity(xl_array* a,size_t new_c);

static struct _xlo_vtable  array_vtable;
static int	    array_vtable_ok = 0;

XLF_DECL(xl_append);
XLF_DECL(xl_prepend);
XLF_DECL(xl_insert);
XLF_DECL(xl_clear);
XLF_DECL(xl_shift);

xlo_field_desc array_fields[] = {
    { "count",      NULL, XLV_INT, 0,           (void*)offsetof( xl_array,count ), 1, sizeof(int) },
    { "capacity",   NULL, XLV_INT, 0,           (void*)offsetof( xl_array,capacity ), 1, sizeof(int) },

    { "append",     NULL, XLV_FUNC_BUILTIN, 0, (void*) xl_append, 1, 0 },
    { "prepend",    NULL, XLV_FUNC_BUILTIN, 0, (void*) xl_prepend, 1, 0 },
    { "clear",      NULL, XLV_FUNC_BUILTIN, 0, (void*) xl_clear, 1, 0 },
    { "insert",     NULL, XLV_FUNC_BUILTIN, 0, (void*) xl_insert, 1, 0 },
    { "shift",	    NULL, XLV_FUNC_BUILTIN, 0, (void*) xl_insert, 1, 0 },
    { NULL,         NULL, XLV_NIL, 0, NULL, 0, 0 } 
};
/* 
    GLOBALS 
*/

xl_object	xl_array_new(int initial_size);

static xl_array* vt_dup(const xl_array* a) {
    xl_array* b = (xl_array*)xl_array_new(a->count);
    if( !b ) return NULL;
    
    /* copy contens of array */
    {
	size_t i = 0;
	for( ; i < a->count; i++ )
	    xlv_set(b->array+i, a->array+i);
    }
    b->count = a->count;
    return b;
}

xl_object	xl_array_new(int initial_size)
{
    xl_array* a;
    if( !array_vtable_ok ) {
	memset(&array_vtable,0,sizeof(array_vtable));
	array_vtable.copy = (xlo_vt_copy_t)vt_dup;
	array_vtable.free = (xlo_vt_free_t)vt_free;
	array_vtable.field = (xlo_vt_field_t)vt_field;
	array_vtable.index = (xlo_vt_index_t)vt_index;
        array_vtable.member_assign = (xlo_vt_member_assign_t)vt_member_assign;
        array_vtable.member_extract = (xlo_vt_member_extract_t)vt_member_extract;
        array_vtable.class_name = "array";


	array_vtable_ok = 1;
	xlo_sort_field_desc(array_fields, countof(array_fields)-1);
    }

    a = axl_calloc( 1, sizeof( xl_array ));
    if( !a ) return NULL;

    a->type = XLV_USER_OBJ;

    a->ref_count = 1;
    a->vt = &array_vtable;

    if( !do_set_capacity(a,initial_size) ) {
	axl_free(a);
	return NULL;
    }

    return XL_OBJECT(a);
}
/*
    LOCALS    
*/
static void do_done(xl_array* a) {
    do_done_range(a,0,a->count);
    a->count = 0;
}
static void do_done_range(xl_array* a,size_t begin,size_t end) {
    size_t i;
    for( i = begin; i < end; i++ )
	xlv_done(a->array+i);
}

static xl_bool	do_set_capacity(xl_array* a,size_t new_c)
{
    xl_value_t* tmp;
    if( new_c == a->capacity ) return xl_true;
    if( new_c < a->count ) {
	do_done_range(a,new_c,a->count);
	a->count = new_c;
    }
    tmp = axl_realloc(a->array, new_c * sizeof( xl_value_t));
    if( !tmp ) return xl_false;

    if( new_c > a->capacity ) {
	// init new values to XLV_NIL
	memset(tmp + a->capacity,0, (new_c - a->capacity)* sizeof(xl_value_t));
    }
    a->array = tmp;
    a->capacity = new_c;
    return xl_true;
}

static void vt_free(xl_array* obj)
{
    if( !obj ) return;
    do_done(obj);
    if( obj->array != NULL )
	axl_free(obj->array);
    obj->array = NULL;
    obj->capacity = 0;
}

int xlv_set_mref(xl_value dest, xl_object parent, void* pos)
{
    if( !dest ) return -1;
    _xlv_done(dest);
    dest->v.mref.obj = parent;
    dest->v.mref.pos = pos;
    xlo_inc_ref(parent);
    dest->type = XLV_MREF;
    dest->flags = XLVF_NEED_EXTRACT;
    return 0;
}

static int vt_field(const xl_array* a,const char* field_name,int mode,xl_value dest)
{
    xlo_field_desc* f;
    f = xlo_find_field(array_fields,countof(array_fields)-1,field_name);
    if( !f ) return -1;
    xlv_set_mref(dest,XL_OBJECT(a),f);
    return 0;
}
static int vt_index(const xl_array* a,const xl_value_t* index,int mode,xl_value dest)
{
    int idx = xlv_int(index);

    if( idx < 0 )
	idx = a->count + idx;
    if( idx >= (int)a->count || idx < 0) {
	if( idx >= 0 && (mode & XLVF_CREATE) ) {
	    // resize it to have such capacity and count = idx
	    if( (int)a->capacity < idx+1 )
		if( ! do_set_capacity((xl_array*)a,idx+1) )
		    return -XLE_MEMORY;
	    ((xl_array*)a)->count = idx+1;
	} else {
	    // return NIL value
	    xlv_clear(dest);
	    return 0;
	}
    }
    xlv_set_ref(dest, a->array+idx, (xl_value)XL_OBJECT(a));
    return 0;
}

static int vt_member_assign(const xl_array* a,xlo_field_desc* index,const xl_value_t* src)
{
    return -1;    
}
static int vt_member_extract(const xl_array* a,xlo_field_desc* index,xl_value dest)
{
    return xlo_extract_field((void*)a, index, dest);
}
XLF_DECL(xl_append)
{
    xl_array* a = (xl_array*)XLF_THIS;

    int narg = XLF_ARGC;
    size_t required_cap = a->count+narg;
    if( a->capacity < required_cap ) {
	if( ! do_set_capacity((xl_array*)a,required_cap) )
	    return -XLE_MEMORY;
    }
    {
	int i;
	for( i = 0; i < 0+narg; i++ ) 
	    xlv_set(a->array+i+a->count, XLF_ARG(i) );
    }
    a->count += narg;
    
    XLF_RETURN_REF(XL_OBJECT(a));
}
/*
    move elements in array 
	* resize array if needed
	* zero up  all left xl_values
	* update array->count
    if len < 0 then all after <from> is moved
*/
static int move_elems(xl_array* a, size_t from, size_t to, size_t len)
{
    if( len == (size_t)-1 ) 
	len = a->count - from;
    if( from > to ) {
	do_done_range(a,from,to);
	memmove(a->array+to, a->array+from, len*sizeof(xl_value_t));
	a->count -= from-to;
    } else if( from < to ) {
	size_t required_capacity = to+len;
	if( a->capacity < required_capacity )
	    if( ! do_set_capacity(a, required_capacity) )
		return -XLE_MEMORY;
	memmove(a->array+to, a->array+from, len*sizeof(xl_value_t));
	memset(a->array+from, 0, (to-from)*sizeof(xl_value_t));
	a->count += to-from;
    }
    return 0;
}

XLF_DECL(xl_prepend)
{
    xl_array* a = (xl_array*)XLF_THIS;

    int narg = XLF_ARGC;
    move_elems(a,0,narg,-1);
    {
	int i;
	for( i = 0; i < 0+narg; i++ ) 
	    xlv_set(a->array+i, XLF_ARG(i) );
    }
    /*a->count += narg;*/
    
    XLF_RETURN_REF(XL_OBJECT(a));
}

XLF_DECL(xl_insert)
{
    xl_array* a = (xl_array*)XLF_THIS;

    int narg = XLF_ARGC-1;
    int pos = XLF_ARG_INT(0,-1);
    if( narg <= 0 || pos < 0 ) 
	/* ignore without error do nothing */
	XLF_RETURN_OBJ(XL_OBJECT(a));
    move_elems(a,pos,pos+narg,-1);
    {
	int i;
	for( i = 0; i < narg; i++ ) 
	    xlv_set(a->array+pos+i, XLF_ARG(i+1) );
    }
    /*a->count += narg;*/
    
    XLF_RETURN_REF(XL_OBJECT(a));
}

XLF_DECL(xl_clear)
{
    XLF_RETURN_INT(0);
}
XLF_DECL(xl_shift)
{
    XLF_RETURN_VAL(0);
}

/*
    this stuff will go into object.c
*/

static int field_desc_cmp(xlo_field_desc* a,xlo_field_desc* b)
{
    return strcmp(a->name,b->name);
}
void		xlo_sort_field_desc(xlo_field_desc* fd, int count)
{
    qsort(fd,
	count,sizeof(xlo_field_desc), 
	( int (*)(const void*, const void*))field_desc_cmp);
}

xlo_field_desc*	xlo_find_field(xlo_field_desc* fd, int count, const char* name)
{
    int n	= count;
    xlo_field_desc* s = fd;
    int		last = count;
    int		first = 0;
    do {
	int ci = (last+first) / 2;
	int c = strcmp(name,s[ci].name);
	if( c > 0 ) {
	    first = ci+1;
	    if( first >= n )
		return NULL;
	} else if( c < 0 ) {
	    last = ci;
	    if( last == 0 )
		return NULL;
	} else
	    return s+ci;
    } while( last != first );
    return NULL;
}
#define FIELD_PTR(s,o) (((char*)s)+(int)o)
int xlo_assign_field(void* s, xlo_field_desc* field, xl_value v)
{
    void* p = FIELD_PTR(s,(int)field->param);
    switch( field->type ) {
    case XLV_INT:
	{
	    int x = xlv_int(v);
	    if( field->size == sizeof(char) ) 
		*(char*)p = x;
	    else if( field->size == sizeof(short) )
		*(short*)p = x;
	    else if( field->size == sizeof(int) )
		*(int*)p = x;
	    else if( field->size == sizeof(long) )
		*(long*)p = x;
#ifdef ZLONG_LONG
	    else if( field->size == sizeof( ZLONG_LONG ) )
		*(ZLONG_LONG*)p = x;
#endif
		return 0;
	}
    case XLV_FLOAT:
	{
	    xl_float x = xlv_float(v);
	    if( field->size == sizeof(float) )
		*(float*)p = (float)x;
	    else if( field->size == sizeof(double) )
		*(double*)p = x;
	    else if( field->size == sizeof(long double) )
		*(long double*)p = x;
	    return 0;
	}
    case XLV_STR:
	{
	    const char* s = xlv_str(v);
	    xl_str* x = (xl_str*)p;
	    *x = xls_set(*x,s);
	    return 0;
	}
    case XLV_STR_BUF:
	{
	    const char* s = xlv_str(v);
	    int l = min(strlen(s),field->size-1);

	    strncpy( (char*)p, s, l);
	    ((char*)p)[l] = 0;
	    return 0;
	}
    default:
	return -1;
    }
    return 0;
}

int xlo_extract_field(void* s, xlo_field_desc* field, xl_value dest)
{
    void* p = FIELD_PTR(s,(int)field->param);
    switch( field->type ) {
    case XLV_INT:
	{
	    xl_integer x;
	    if( field->size == sizeof(char) ) 
		x = *(char*)p;
	    else if( field->size == sizeof(short) )
		x = *(short*)p;
	    else if( field->size == sizeof(int) )
		x = *(int*)p;
	    else if( field->size == sizeof(long) )
		x = *(long*)p;
#ifdef ZLONG_LONG
	    else if( field->size == sizeof( ZLONG_LONG ) )
		x = (xl_integer) *(ZLONG_LONG*)p;
#endif
	    xlv_set_int(dest,x);
	    return 0;

	}
    case XLV_FLOAT:
	{
	    xl_float x;
	    if( field->size == sizeof(float) )
		x = *(float*)p;
	    else if( field->size == sizeof(double) )
		x = *(double*)p;
	    else if( field->size == sizeof(long double) )
		x = *(long double*)p;
	    return xlv_set_float(dest,x);
	}
    case XLV_STR:
	{
	    xl_str* x = (xl_str*)p;
	    return xlv_set_strn(dest,xls_get(*x), xls_len(*x));
	}
    case XLV_STR_BUF:
	{
	    xlv_set_str(dest,(char*)p);
	    return 0;
	}
    case XLV_FUNC_BUILTIN:
        {
            return xlv_set_func_builtin(dest,(xl_builtin_func)field->param,NULL);
        }
    default:
	return -1;
    }
    return 0;
}
