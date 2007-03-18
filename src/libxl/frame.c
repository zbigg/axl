/*
* File:	        frame.c
*
* Id:           $Id: frame.c 1.3 03/11/27 21:36:25+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	frame stack implementation
*
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "axl/axl.h"
#include "parse.h"
#include "tools.h"

#include "axl_i.h"

/* IMPLEMENTED GLOBAL FUNCTIONS */
int     frame_pos();
int     frame_size();
xl_bool frame_begin();
void    frame_finish();
/*int	frame_get_global_sym(xl_str name);*/
/* int	frame_get_sym(xl_str name);*/
/*int	frame_add_sym(xl_str name);*/
void    frame_destroy(void);
void    frame_add_params(xlp_name_list* nl);

xl_symbol* frame_get_sym(xl_str name);

xl_symbol* frame_get_global_sym(xl_str name);
xl_symbol* frame_add_sym(xl_str name);


/*IMPLEMENTATION */

#define define_stack_static_struct(name,type,size)		\
static type	name ## _stack_t[size];                         \
static int	name ## _stack_i = (int)0;			\
								\
void	name ## _push(const type* i)				\
{								\
    if( name ## _stack_i >= countof(name ## _stack_t) -1 )	\
	err_f("internal stack overflow (" #name ")");		\
    name ## _stack_t[name ## _stack_i++] = *i;			\
}								\
								\
void	name ## _pop(void)					\
{								\
    if ( -- name ## _stack_i < 0 )				\
	err_f("internal stack underflow (" #name ")");		\
}								\
type*	name ## _cur(void)					\
{								\
    if( name ## _stack_i > 0 )					\
	return & (name ## _stack_t[name ## _stack_i-1]);	\
    return (type*)NULL;						\
}								\
type*	name ## _prev(int i)					\
{								\
    if( i < name ## _stack_i && name ## _stack_i > 0 )		\
	return & (name ## _stack_t[name ## _stack_i-1-i]);	\
    return (type*)NULL;						\
}

#define declare_stack_static_struct(name,type,size)		\
void	name ## _push(const type* i);				\
void	name ## _pop(void);					\
type*	name ## _cur(void);					\
type*	name ## _prev(int);					

define_stack_static_struct(frame,xl_frame_t,40);

int frame_pos();
int frame_size();
static int _frame_pos = 0;

xl_bool frame_begin()
{
    xl_frame_t f;

    _frame_pos += frame_size();

    f.id = NULL;
    f.size = 0;
    f.bound = xl_false;
    frame_push(&f);
    return xl_true;
}

void frame_finish()
{
    xl_frame_t* x = frame_cur();
    if( x )
	xlmap_free(x->id);
    frame_pop();

    _frame_pos -= frame_size();
}

xl_symbol* frame_get_global_sym(xl_str name)
{
    xl_frame_t* f= &frame_stack_t[0];
    xl_frame_t* i=f;
    int j=0;
    if( frame_stack_i == 0 ) return NULL;
    while( j < frame_stack_i && !f->bound ) {
        i = f;
        f++;
        j++;
    }
    if( j >= frame_stack_i ) return NULL;
    while( i >= f ) {
	if( i->id ) {
	    xlmi_t ii;
	    xlmap_find_i(i->id,name,&ii);
	    if( ii.item )
		return (xl_symbol*)(ii.item->item);
        }
        i--;
    }
    return NULL;
}

xl_symbol* 	frame_get_sym(xl_str name)
{
    xl_frame_t* x = frame_cur();
    int pos = frame_pos();
    int fi;
    int base = 0;
    if( !x )  return NULL;
    fi = 0;
    while( x ) {
	if( x->id ) {
	    xlmi_t i;
	    xlmap_find_i(x->id,name,&i);
            if( i.item ) {
                xl_symbol* sym = (xl_symbol* )(i.item->item);
                if( base ) sym->base = 1;
		return sym;
            }
	}
	if( x->bound ) base = 1;
	fi++;
	x = frame_prev(fi);
	if( x )
	    pos -= x->size;
    }
    return NULL;
}
xl_symbol* 	frame_get_sym_str(const char* name)
{
    xl_frame_t* x = frame_cur();
    int pos = frame_pos();
    int fi;
    int base = 0;
    if( !x )  return NULL;
    fi = 0;
    while( x ) {
	if( x->id ) {
	    xlmi_t i;
	    xlmap_find_str_i(x->id,name,&i);
            if( i.item ) {
                xl_symbol* sym = (xl_symbol* )(i.item->item);
                if( base ) sym->base = 1;
		return sym;
            }
	}
	if( x->bound ) base = 1;
	fi++;
	x = frame_prev(fi);
	if( x )
	    pos -= x->size;
    }
    return NULL;
}
xl_symbol* frame_add_sym(xl_str name)
{
    xl_frame_t* x = frame_cur();
    xl_symbol*  sym;
    if( !x ) return NULL;

    if( (sym = frame_get_sym(name)) != NULL  ) 
	return sym;

    if( !x->id )
	x->id = xlmap_new( (xl_free_t)xlsm_free);
    sym = xlsm_new();
    sym->name = xls_new_xls(name);
    sym->pos = frame_pos() + x->size;
    x->size += sym->size;
    xlmap_add(x->id,name, (void*)sym);
    /* add i to current frame */
    return sym;
}

int frame_size()
{
    xl_frame_t* f = frame_cur();
    return f ? f->size : 0;
}

int frame_pos()
{
    return _frame_pos;
}

void frame_destroy(void)
{
    int fi = 0;
    xl_frame_t* x = frame_prev(fi);
    while( x ) {
	if( x->id ) {
	    xlmap_free(x->id);
	    x->id = NULL;
	}
	fi++;
	x = frame_prev(fi);
    }
    frame_stack_i = 0;
}

void frame_add_params(xlp_name_list* nl)
{
    slist_item* it = nl->names->head;
    xl_frame_t* f = frame_cur();
    int np = nl->count;
    while( it ) {
	xl_str a = (xl_str) it->item;
        xl_symbol* sym = xlsm_new();
	if( !f->id ) f->id = xlmap_new((xl_free_t)xlsm_free);
        sym->pos = --np;
        sym->name = xls_new_xls(a);
	xlmap_add(f->id, a, (void*) sym);
	it = it->next;
    }
}
