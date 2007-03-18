/*
* File: 	code.c
*
* Id:		$Id: code.c 1.4 04/01/28 21:05:19+01:00 zbigg@nowy.dom $
*
* Project:	xlang
*
* Description: Code generation & optimization
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include "zcompat/ztools.h"
#include "axl/axl.h"
#include "tools.h"

#include "axl_i.h"

#define xlc_alloc(n) ( (xl_code)axl_calloc(n,sizeof(xl_code_t)) )

char* _debug_file = NULL;
int _debug_line 	= -1;

void xlc_throw(int number)
{
    xle_throw(number,"xl_code:");
}

void    xl_code_init_module(void);
void    xl_code_done_module(void);

xl_code xlc_new(void)
{
    xl_code a = xlc_alloc(1);
    if( !a )
	xlc_throw(XLE_MEMORY);

    MDMF_FILL(a,"xlcd");
    a->last = 0;
    a->cap = 0;
    a->rt = xl_rt_void;
    xlc_attach_ptr(a);
    return a;
}

void		xlc_free(xl_code a)
{
    int i;
    if( a ) {
	if( a->labels ) {
	    xlmap_free(a->labels);
	    a->labels = 0;
	}
	if( a->buf ) {
	    for( i=0; i < a->last; i++)
		xlv_frees(& (a->buf + i)->ival);
	    sbuffer_free(a->_buf);
	}
        xlc_detach_ptr(a);
	axl_free(a);
    }
}
static int		xlc_resize_buffer(xl_code cd,int new_size)
{
    if( cd->_buf == NULL ) {
	cd->_buf = sbuffer_new(new_size * sizeof(xl_instr_t) );
	if( cd->_buf == NULL )
	    xlc_throw(XLE_MEMORY);
    } else if( sbuffer_resize(cd->_buf,new_size * sizeof(xl_instr_t)) < 0 )
	xlc_throw(XLE_MEMORY);
    cd->buf = (xl_instr_t*) cd->_buf->t;
    cd->cap = new_size;
    return 1;
}

int	    xlasm_put_instr(ZSTREAM s,const xl_instr_t* instr);
int	    xlasm_put_value(ZSTREAM s,const xl_value_t* instr);

void		xlc_print(const xl_code c)
{
    int i;
    if( !c || !c->buf )
	return;

    for( i = 0; i < c->last; i++ ) {
	zprintf("%i:  ",i);
	xlasm_put_instr( zstdout, c->buf+i);
    }
}
int		xlc_add_instr(xl_code cd,int icode,xl_value ival)
{
    xl_instr i;
    if( cd == NULL )
	return 0;
#if 1
    if( _debug_file != NULL ) {
	char* x = _debug_file;
	_debug_file = NULL;
	xlc_add_instr_str(cd,0,x);
	axl_free(x);
    }
    if( _debug_line > 0 ) {
	int x = _debug_line;
	_debug_line = -1;
	xlc_add_instr_int(cd,0,x);
    }
#endif
    if( cd->last >= cd->cap ) {
	xlc_resize_buffer(cd, (cd->cap != 0) ? (cd->cap * 2) : 2 );
    }
    i = & cd->buf[cd->last++];
    i->icode = icode;
    xlv_mov(& i->ival, ival);
    if( cd->resn != 0 )
	cd->res = xlc_get_val_end(cd,cd->resn);
    return 1;
}
int		xlc_add_instr_nil(xl_code cd,int icode)
{
    xl_value_t i;
    i.type = XLV_NIL;
    i.flags = 0;
    return xlc_add_instr(cd,icode,&i);
}

int		xlc_add_instr_int(xl_code cd,int icode,xl_integer val)
{
    xl_value_t i;
    i.type = XLV_INTEGER;
    i.v.i = val;
    i.flags = 0;
    return xlc_add_instr(cd,icode,&i);
}

int		xlc_add_instr_float(xl_code cd,int icode,xl_float val)
{
    xl_value_t i;
    i.type = XLV_FLOAT;
    i.v.f = val;
    i.flags = 0;
    return xlc_add_instr(cd,icode,&i);
}

int		xlc_add_instr_str(xl_code cd,int icode,const char* val)
{
    xl_value_t i;
    i.type = XLV_STR;
    i.v.s = xls_new(val);
    i.flags = 0;
    return xlc_add_instr(cd,icode,&i);
}
int		xlc_add_instr_strx(xl_code cd,int icode,xl_str val)
{
    xl_value_t i;
    i.type = XLV_STR;
    i.v.s = xls_new_xls(val);
    i.flags = 0;
    return xlc_add_instr(cd,icode,&i);
}

int		xlc_add_label(xl_code cd,const char* name)
{
    xl_value_t i;
    i.type = XLV_STR;
    i.v.s = xls_new(name);
    i.flags = 0;
    return xlc_add_instr(cd, 0 ,&i);
}

int		xlc_add_code(xl_code cd,xl_code p)
{
    int i;
    int end;

    xlmi begin,last;
    if( cd == NULL || p == NULL )
	return 0;
    end = p->last;
    for(i=0;i<end; i++)
	xlc_add_instr(cd,
		      p->buf[i].icode,
		      & (p->buf[i].ival) );
    if( p->labels != NULL ) {
	begin = xlmap_begin(p->labels,0);
	last = xlmi_inc( xlmap_end(p->labels,0) );
	if( !cd->labels )
	    cd->labels = xlmap_new(NULL);
	for( ; !xlmi_eq(begin,last); begin = xlmi_inc(begin) ) {
	    if( xlmap_add( cd->labels, xlmi_key(begin), xlmi_get(begin) ) < 0 ) {
		xle_throw(0,"xl_code: duplicate labels(xlc_add_code)");
	    }
	}
	xlmi_free(begin);
	xlmi_free(last);
    }
    if( p->resn != 0 ) {
	cd->resn = p->last + p->resn;
	cd->rt = p->rt;
	cd->res = xlc_get_val(cd,cd->resn);
    }

    cd->rs += p->rs;

    return 0;
}

xl_code 	xlc_join(xl_code cd,xl_code p)
{
    xlmi begin,last;
    if( cd == NULL || p == NULL )
	return cd ? cd : p;
    if( cd->last+p->last >= cd->cap ) {
	xlc_resize_buffer(cd, cd->last+p->last + 2 );
    }
    memcpy(cd->buf + cd->last,p->buf,p->last* sizeof( xl_instr_t));
    cd->last+=p->last;
    if( p->labels ) {
	if( cd->labels == NULL ) {
	    cd->labels = p->labels;
	    p->labels = NULL;
	} else {
	    begin = xlmap_begin(p->labels,NULL);
	    last = xlmap_end(p->labels,NULL);
	    xlmi_inc(last);
	    for( ; !xlmi_eq(begin,last); xlmi_inc(begin) ) {
		if( xlmap_add( cd->labels, xlmi_key(begin), xlmi_get(begin) ) < 0 )
		    xle_throw(0,"xl_code: duplicate labels(xlc_join)");
	    }
	    xlmi_free(begin);
	    xlmi_free(last);
	    if( p->labels ) {
		xlmap_free(p->labels);
		p->labels = 0;
	    }
	}
    }
    if( p->resn != 0 ) {
	cd->resn = p->last + p->resn;
	cd->rt = p->rt;
	cd->res = xlc_get_val(cd,cd->resn);
    }

    cd->rs += p->rs;

    if( p->buf ) {
	sbuffer_free(p->_buf);
    }
    xlc_detach_ptr(p);
    axl_free(p);
    return cd;
}

int		xlc_read_labels(xl_code cd)
{
    int i;
    XL_FCTX("xlc_read_labels","",(1));
    if( ! cd )
	return 0;
    if( !cd->labels )
	cd->labels = xlmap_new(NULL);
    for(i=0;i< cd->last; i++ )
	if( cd->buf[i].icode == 0 && 
	    cd->buf[i].ival.type == XLV_STR &&  
	    xls_get(cd->buf[i].ival.v.s)[0] == '$') {
	    xlmap_add(cd->labels,cd->buf[i].ival.v.s,(void*)i);
    }
    return 1;
}

int		xlc_opt_jumps(xl_code cd)
{
    XL_FCTX("xlc_opt_jumps","",(1));
    return 0;
}

int		xlc_opt_code(xl_code cd,int oflags)
{
    XL_FCTX("xlc_opt_code","",(1));
    return 0;
}

void		xlc_debug_new_line(int line)
{
    _debug_line = line;
}
void		xlc_debug_new_file(const char* name)
{
    if( _debug_file ) axl_free(_debug_file);
    _debug_file = name != NULL ? axl_strdup(name) : NULL;
}

xl_instr	xlc_get_instr(xl_code c,int nr)
{
    XL_FCTX("xlc_get_instr","%i",(nr));
    return c->buf+nr;
}

xl_instr	xlc_get_instr_end(xl_code c,int nr)
{
    XL_FCTX("xlc_get_instr_end","%i",(nr));
    return xlc_get_instr(c,c->last-nr);
}

xl_value	xlc_get_val(xl_code c,int nr)
{
    XL_FCTX("xlc_get_val","%i",(nr));
    return & ( xlc_get_instr(c,nr)->ival );
}
xl_value	xlc_get_val_end(xl_code c,int nr)
{
    XL_FCTX("xlc_get_val_end","%i",(nr));
    return & ( xlc_get_instr_end(c,nr)->ival );
}

void	xlc_set_res(xl_code c,xl_result_type rt,int n)
{
    c->rt = rt;
    switch( rt ) {
    case xl_rt_void:
    case xl_rt_none:
	c->rs = 0;
	break;
    case xl_rt_imm:
	c->resn = c->last - n;
	c->res = xlc_get_val_end(c,n);
    case xl_rt_val:
    case xl_rt_ref:
    case xl_rt_reg:
	c->rs = 1;
	break;
    case xl_rt_range:
	c->rs = 2;
	break;
    }
    if( rt != xl_rt_imm ) {
	c->resn = 0;
	c->res = NULL;
    }
}

int xlc_load(xl_code code,ZSTREAM in)
{
    while (!zeof(in)) {
	xl_instr_t i;
	if( zread(in, &i.icode, sizeof( i.icode) ) < sizeof(i.icode) )
	    return zeof(in) ? 0 : -1;
	if( xlv_sread(& i.ival, in) < 0 )
	    return -1;
	if( xlc_add_instr(code, i.icode, &i.ival) < 0 )
	    return -1;
    }
    return 0;
}

int xlc_store(xl_code code,ZSTREAM out)
{
    int n = 0;
    int r=-1,c;
    xl_instr_t* i = code->buf;
    while( n < code->last ) {
	c = zwrite(out,& i->icode,sizeof(i->icode) );
	if( c < 0 ) return r;
	r+=c;
	c = xlv_swrite(&i->ival, out);
	r+=c;
	if( c < 0 ) return r;
	i++;n++;
    }
    return r;
}
DEFINE_SBUFFER_TYPE(xl_code,xl_code_array);
IMPLEMENT_SBUFFER_TYPE(xl_code,xl_code_array);

xl_code_array* ca = NULL;

void    xl_code_init_module(void)
{
}
void    xl_code_done_module(void)
{
    if( ca ) {
        xl_code_gc();
        xl_code_array_free(ca);
    }
    ca = NULL;
}

/* desttoy all unwanted code !*/
void    xl_code_gc()
{ 
    xl_code* b;
    size_t size,i;
    b = xl_code_array_getp(ca);
    size = xl_code_array_capacity(ca);
    for(i = 0; i < size; i++ ) {
        if( b[i] != NULL ) {
            xlc_free(b[i]);
            b[i] = NULL;
        }
    }
}

void    xlc_attach_ptr(xl_code ptr)
{
    xl_code* b;
    size_t size,i;
    if( !ca ) ca = xl_code_array_new(10);
    b = xl_code_array_getp(ca);
    size = xl_code_array_capacity(ca);
    for(i = 0; i < size; i++ ) {
        if( b[i] == NULL ) {
            b[i] = ptr;
            return;
        }
    }
    if( i == size ) {
        if( xl_code_array_set_size(ca,size*2) < 0 ) return; /* FATAL ERROR */
        b = xl_code_array_getp(ca);
        b[i] = ptr;
    }
    return;
}
int     xlc_detach_ptr(xl_code ptr)
{
    xl_code* b;
    size_t size,i;
    if( !ca ) return -1;
    b = xl_code_array_getp(ca);
    size = xl_code_array_capacity(ca);
    for(i = 0; i < size; i++ ) {
        if( b[i] == ptr ) {
            b[i] = NULL;
            return 0;
        }
    }
    return -1;
}

