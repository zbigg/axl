/*
* File:		context.c
*
* Id:           $Id: context.c 1.5 04/01/28 21:05:19+01:00 zbigg@nowy.dom $
*
* Project:	axl
*
* Description: Context
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
#include "tools.h"
#include "axl/axl.h"

#include "axl_i.h"

#define xlx_alloc(n) ( (xl_context)axl_calloc(n,sizeof(xl_context_t)) )

#if 0
#define XLD_TRACE_CONTEXT
#endif

#if 0 || defined XLD_CTX_TRACE
#define DEBUG_CONTEXT_GC
#endif

#ifdef XL_TRACE_CONTEXT_LIFE
static int	live_contexts = 0;
static int      TRACE_CONTEXT_REF = -1;
xl_bool     	xl_trace_object_life = xl_true;
xl_bool     	xl_trace_object_referencing = xl_true;
static trace_action(xl_context* ctx,const char* action)   {
    char* descr = obj->name;
    if( TRACE_OBJECT_REF < 0 )
        TRACE_OBJECT_REF = xldbg_get_mask("object-ref");
    if( (!descr || !descr[0]) && obj->vt && obj->vt->class_name )
        descr = obj->vt->class_name;
    xl_trace(TRACE_OBJECT_REF,"XLDBG(objects:%02i): %s OBJ(%s) 0x%08x rc=%i\n",live_objects,action,descr,obj,obj->ref_count);
}
#endif

#define axl_debug_stream zstdout
int ctx_active = 0;

void xlx_throw(int number)
{
    xle_throw(number,"xl_context:");
}

xl_context	xlx_news(void)
{
    xl_context n = (xl_context) axl_calloc(1,sizeof( xl_context_t) );
    ctx_active++;
#ifdef DEBUG_CONTEXT_GC
    zfprintf(axl_debug_stream,"CTX (0x%08x) created (NEWS) active: %i\n",n,ctx_active);
#endif
    return n;
}
xl_context	xlx_new(xl_context prev,xl_context next_find)
{
    xl_context a = (xl_context) axl_calloc(1,sizeof( xl_context_t) );
    MDMF_FILL(a,"xlxg");
    ctx_active++;
#ifdef DEBUG_CONTEXT_GC
    zfprintf(axl_debug_stream,"CTX (0x%08x): created (GEN) active: %i\n",a,ctx_active);
#endif
    return xlx_init(a,prev,next_find);
}

xl_context	xlx_init(xl_context c,xl_context prev,xl_context next_find)
{
    c->id = sbtree_new();
    c->id_owner = 1;
    c->id_rdonly = 0;
    c->prev_ctx = prev;
    c->next_find_ctx = next_find;
    c->ref_count = 1;
    c->this_ref = NULL;
    MDMF_FILL(c,"xlxg");
    return c;
}

xl_context	xlx_init_object(xl_context c,xl_object o,xl_context prev,xl_context next_find)
{
    c->id = o->map;
    c->id_owner = 0;
    c->id_rdonly = 0;
    c->prev_ctx = prev;
    c->next_find_ctx = next_find;
    c->ref_count = 1;
    c->this_ref = o;
    xlo_inc_ref(o);
    MDMF_FILL(c,"xlxo");
    return c;
}

xl_context	xlx_copy(const xl_context c)
{
    NOT_READY;
    return NULL;
}

void		xlx_free(xl_context c)
{
    xlx_frees(c);
    if( c->ref_count <= 0 )
	axl_free(c);
}

void		xlx_frees(xl_context c)
{
    c->ref_count--;
    if( c->ref_count <= 0 ) {

	if( c->this_ref != NULL )
	    xlo_free(c->this_ref);
	if( c->id_owner ) {

	    sbtree_free(c->id,	( sbtree_free_f) xlv_free );
	}
#ifdef DEBUG_CONTEXT_GC
	ctx_active--;
	zfprintf(axl_debug_stream,"CTX (0x%08x): deleted (FREES) active: %i\n",c,ctx_active);
#endif
	c->id = 0;
    }
}
void		xlx_free_variable(xl_context c,const char* name)
{
    if( c && c->id ) {
	xl_value v = (xl_value) sbtree_del(c->id, name);
	if( v )
	    xlv_free(v);
    }
}

xl_value	xlx_find(xl_context c,const char* name,xl_value* parent)
{
    xl_value a;
    while ( c != NULL ) {
	if( c->id == NULL && c->this_ref != NULL ) {
	    if( c->this_ref->map == NULL ) {
		c = c->next_find_ctx;
		continue;
	    } else
		c->id = c->this_ref->map;
	}
	if( c->id != NULL )
	    a = (xl_value) sbtree_find(c->id,name);
	else
	    a = NULL;
	if( a != NULL ) {
	    if( parent )
		*parent = (xl_value) c->this_ref;
	    return a;
	}
	c = c->next_find_ctx;
    }
    return NULL;
}

xl_value	xlx_set(xl_context c,const char* name,xl_value v,xl_value* parent)
{
    xl_value a;
    if( (a = xlx_find(c,name,parent)) ) {
	xlv_set(a,v);
	return a;
    } else if(! c->id_rdonly ) {
	a = xlv_new(v);
	if( a != NULL ) {
	    if( c->id == NULL) {
		if( c->this_ref ) {
		    if( c->this_ref->map )
			c->id = c->this_ref->map;
		    else
			c->id = c->this_ref->map = sbtree_new();
		    c->id_owner = 0;
		    c->id_rdonly = 0;
		} else {
		    c->id = sbtree_new();
		    c->id_owner = 1;
		    c->id_rdonly = 0;
		}
	    }
	    sbtree_add(c->id,name,a);
	}
	if( parent )
	    *parent = (xl_value) c->this_ref;
	return a;
    }
    return NULL;
}

void		xlx_unset(xl_context c,const char* name) {
    NOT_READY;
}
xl_value	xlx_find_scope(xl_context c,const char* scope,const char* name) {
    NOT_READY;
}

xl_value	xlx_set_scope(xl_context c,const char* scope,const char* name,xl_value v) {
    NOT_READY;
}

xl_value	xlx_set_int(xl_context c,const char* name,xl_integer i)
{
    xl_value_t a;
    a.type = XLV_INT;
    a.flags = 0;
    a.v.i = i;
    return xlx_set(c,name,&a,NULL);
}
xl_value	xlx_set_float(xl_context c,const char* name,xl_float fl)
{
    xl_value_t a;
    a.type = XLV_FLOAT;
    a.flags = 0;
    a.v.f = fl;
    return xlx_set(c,name,&a,NULL);
}
xl_value	xlx_set_str(xl_context c,const char* name,const char* s)
{
    xl_value_t a;
    a.type = XLV_STR;
    a.flags = 0;
    a.v.s = xls_new(s);
    xlx_set(c,name,&a,NULL);
    xlv_frees(&a);
    return 0;
}
xl_value	xlx_set_ref(xl_context c,const char* name,xl_value obj)
{
    xl_value_t a;
    a.type = XLV_NIL;
    a.flags = 0;
    xlv_set_ref(&a,obj,NULL);
    xlx_set(c,name,&a,NULL);
    xlv_frees(&a);
    return 0;
}

static int ctx_number = 0;
void		xlth_enter_func(xl_thread th)
{
    xl_context a =  xlx_news();
    a->this_ref = NULL;
    a->prev_ctx = th->var;
    a->next_find_ctx =
	(th->var == th->var_global)
	    ? th->var
	    : th->var->next_find_ctx != NULL
		? th->var->next_find_ctx
		: th->var_global;
    a->id_owner = 1;
    a->id_rdonly = 0;
    th->var = a;
#ifdef XLD_TRACE_CONTEXT
    snprintf(a->debug_name,sizeof( a->debug_name), "CTX (#%i) func",ctx_number);
    zprintf(ANSI_C_YELLOW("entered to")": %s\n",a->debug_name);
#endif
    ctx_number++;
}

void		xlth_enter_object(xl_thread th,xl_object o)
{
    xl_context a = xlx_news();
    xlx_init_object(a,o,
	th->var,
	th->var);
    th->var = a;
#ifdef XLD_TRACE_CONTEXT
    snprintf(a->debug_name,sizeof( a->debug_name), "CTX (#%i) with <object 0x%08x>",ctx_number,(unsigned int)o);
    zprintf(ANSI_C_YELLOW("entered to")": %s\n",a->debug_name);
#endif
    ctx_number++;
}

void		xlth_enter_method(xl_thread th,xl_object o)
{
    xl_context	obj = xlx_news(),
		loc = xlx_news();
    /* Create context from object */
    xlx_init_object(obj,o,th->var,th->var_global);
    /* Create context for function */
    xlx_init(loc,obj,obj);
    th->var = loc;
}

void		xlth_leave_method(xl_thread th)
{
    xlth_leave_func(th);
    xlth_leave(th);
}

void		xlth_enter_object_global(xl_thread th,xl_object o)
{
    xl_context a = xlx_news();
    xlx_init_object(
	a,
	o,
	th->var_global->prev_ctx,
	th->var_global->next_find_ctx
	);
    th->var_global->prev_ctx = a;
    th->var_global->next_find_ctx = a;
#ifdef XLD_TRACE_CONTEXT
    snprintf(a->debug_name,sizeof( a->debug_name), "CTX GLOB <object 0x%08x>",(unsigned int)o);
    zprintf(ANSI_C_YELLOW("entered to")": %s\n",a->debug_name);
#endif
}

void		xlth_leave_func(xl_thread th)
{
    int ctx_is_object = 1;
    do {
	ctx_is_object = (th->var->this_ref != NULL);
	xlth_leave(th);
    } while( ctx_is_object );
}

void		xlth_leave(xl_thread th)
{
    xl_context a = th->var->prev_ctx;

#ifdef XLD_TRACE_CONTEXT
    zprintf(ANSI_C_YELLOW("left to")": %s\n",a->debug_name);
#endif

    xlx_free(th->var);
    th->var = a;
    if( th->var == 0 ) {
	zprintf("CTX: no context left to run into !\n");
	th->run = 0;
	th->interrupted = 1;
    }
    --ctx_number;
}
