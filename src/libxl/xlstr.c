/*
* File: 	xlstr.c
*
* Id:           $Id: xlstr.c 1.4 03/12/05 04:14:12+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:
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
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "axl/axl.h"
#include "axl_i.h"
#include "zcompat/zprintf.h"

#define XLSTR_REF_CNT 1

char* xl_str_empty = "";

xl_str    xls_alloc(unsigned int len)
{
    xl_str a = (xl_str)axl_malloc( sizeof( xl_str_t ) + len + 1 );
    a->capacity = len;
    a->length = 0;
    a->ref_cnt = 1;
    MDMF_FILL(a,"xlst");
    return a;
}

xl_str    xls_realloc(xl_str s,unsigned int len)
{
    unsigned int olen = xls_len(s);
    xl_str a = (xl_str)axl_realloc( (void*)s,sizeof( xl_str_t ) + len + 1 );
    if( s != NULL ) {
        a->capacity = len;
        a->length = zmin(a->capacity,olen);
        xls_get(a)[a->length] = '\0';
    }
    MDMF_FILL(a,"xlst");
    return a;
}

/** Create string.
*/
xl_str	xls_new(const char* s)
{
    return xls_newn(s,s !=NULL ? strlen(s) : 0);
}

xl_str	xls_newn(const char* s,unsigned int len)
{
    xl_str a;
    a = xls_alloc(len);
    a->capacity = len;
    a->length = len;
    a->ref_cnt = 1;
    if( s != NULL ) {
	strncpy( xls_get(a), s,len);
	xls_ptr(a)[len] = '\0';
    } else
	xls_ptr(a) [0] = 0;
    return a;
}

xl_str	xls_ref(const xl_str_t* s)
{
    if( s )
        ((xl_str_t*)s)->ref_cnt+=1;
    return (xl_str)s;
}

xl_str	xls_new_xls(const xl_str_t* s)
{
    if( s ) 
        ((xl_str_t*)s)->ref_cnt+=1;
    return (xl_str)s;
}

void	xls_free(xl_str a)
{
    if( a == NULL ) return;
    a->ref_cnt-=1;
    if( a->ref_cnt <= 0 )
    	axl_free((void*)a);
}

xl_str	xls_set 	(xl_str dest,const char* src)
{
    if( dest && dest->ref_cnt == 1 )
	return xls_setn(dest,src,strlen(src));
    else {
	xls_free(dest);
	return xls_new(src);
    }
}

xl_str	xls_setn 	(xl_str dest,const char* src,unsigned int len)
{
    if( dest == NULL && src == NULL )
	return NULL;
    if( dest != NULL && dest->ref_cnt == 1 ) {
	if( dest == NULL && src != NULL )
	    return xls_newn( src, len );
	else {
	    if( len > dest->capacity ) {
		dest = xls_realloc(dest,len);
		if( dest == NULL )
		    return NULL;
	    }
	    if( src != NULL ) {
		strncpy( xls_ptr( dest ), src, len );
		xls_ptr(dest)[len] = '\0';

		dest->length = len;
	    } else {
		xls_ptr( dest ) [0] = 0;
		dest->length = 0;
	    }
	    return dest;
	}
    } else {
	xls_free(dest);
	return xls_newn(src,len);
    }
}

xl_str	xls_set_xls	(xl_str dest,const xl_str_t* src)
{
    if( dest == NULL )
	return xls_new_xls(src);
    if( dest->ref_cnt == 1 )
	return xls_setn(dest,xls_ptr(src),xls_len(src));
    else {
	((xl_str_t*)src)->ref_cnt+=1;
	xls_free(dest);
	return (xl_str)src;
    }
}

extern xl_str	xls_cat(xl_str dest, ...)
{
    va_list ap;
    xl_str ret;
    va_start(ap,dest);
    ret = xls_catv(dest,ap);
    va_end(ap);
    return ret;
}
extern xl_str	xls_catv(xl_str dest, va_list ap)
{
    char* s;

    while( (s = va_arg(ap,char*)) != NULL )
	dest = xls_add(dest,s);
    return dest;
}

xl_str	xls_addn 	(xl_str dest,const char* src,unsigned int len2)
{
    if( dest == NULL ) {
	if( src != NULL )
	    return xls_newn(src,len2);
	else
	    return NULL;
    }
    if( dest->ref_cnt == 1 ) {
	if( src == NULL )
	    return dest;
	else {
	    unsigned len1 = xls_len(dest);
	    /*  int len2 = length of src */
	    unsigned lens = len1 + len2;
	    if( lens > dest->capacity ) {
		dest = xls_realloc(dest,lens);
		if( dest == NULL )
		    return NULL;
	    }
	    memcpy( xls_get( dest ) + len1, src, len2);
	    xls_get(dest)[lens] = '\0';
	    dest->length = lens;
	    return dest;
	}

    } else {
	xl_str x;
	unsigned len1 = xls_len(dest);
	/*  int len2 = length of src */
	unsigned lens = len1 + len2;
	char* xs;
	if( src == NULL ) return NULL;
	x = xls_alloc(lens);
	if( !x ) return NULL;
	xs = xls_ptr(x);
	memcpy(xs,     xls_get(dest), len1);
	memcpy(xs+len1,src, len2);
	xs[lens] = '\0';
	x->length = lens;
	xls_free(dest);
	return x;
    }
}

xl_str	xls_add 	(xl_str dest,const char* src)
{
    register int x = src ? strlen(src) : 0;
    return xls_addn(dest,src,x);
}

xl_str	xls_add_xls	(xl_str dest,const xl_str_t* src)
{
    if( dest == NULL ) {
	if ( src != NULL )
	    return xls_new_xls(src);
	else
	    return NULL;
    } else
	if( src == NULL ) 
	    return dest;
    return xls_addn(dest,xls_ptr(src),xls_len(src));
}

xl_str	xls_add_char	(xl_str dest,char src)
{
    char x[2];
    x[0] = src;
    x[1] = 0;
    return xls_addn(dest,x,1);
}

xl_str	xls_add_int	(xl_str dest,int i,const char* fmt)
{
    char x[40];
    register int len = sprintf(x,fmt ? fmt : "%i", i);
    return xls_addn(dest,x,len);
}

xl_str	xls_add_float	(xl_str dest,double f,const char* fmt)
{
    char x[80];
    register int len = sprintf(x,fmt ? fmt : "%f", f);
    return xls_addn(dest,x,len);
}
static void	xls_str_erase_blank (char* src,int *len);

extern xl_str	xls_erase_blank (xl_str src)
{
    char* es,*ee;
    char* s;
    int si = 0;
    if( xls_empty(src) )
	return src;
    if( src->ref_cnt > 1 ) {
	xls_free(src);
	src = xls_newn(xls_get(src),xls_len(src));
    }
    s = xls_str(src);
    for( es = s; *es && isspace(*es); es++,si++ ) ;

    for( ee = s + xls_len(src)-1; isspace(*ee) ; ee-- ) {
	*ee = '\0';
	--(src->length);
    }
    if( es > s ) {
	memmove(s,es,src->length - si);
	src->length-=si;
	s[src->length] = '\0';
    }
    return src;
}

static void	xls_str_erase_blank (char* src,int *len)
{
    char* es,*ee;
    char* s = src;
    int si = 0;
    if( !s || !*s ) return;
    for( es = s; *es && isspace(*es); es++,si++ ) ;

    for( ee = s + *len-1; isspace(*ee) ; ee-- ) {
	*ee = '\0';
	--(*len);
    }
    if( es > s ) {
	memmove(s,es,*len - si);
	*len-=si;
	s[*len] = '\0';
    }
    si = *len;
    while( *s )  {
	if( isspace(s[0]) && isspace(s[1]) ) {
	    int x = si-1;
	    xls_str_erase_blank(s+1,&x);
	    *len -=(si-1-x);
	    return;
	}
	s++;
	si--;
    }
}

int	xls_bwrite	(ZSTREAM f,const xl_str_t* src)
{
    xl_uint32 len = xls_len(src);
    int x = 0;
/*    x += zwrite(f,&len,sizeof(len));*/
    x += znwrite_l(f,&len);
    if( x > 0 )
	x += zwrite (f,xls_ptr(src),len);
    return x;
}
xl_str	xls_bread	(ZSTREAM f)
{
    xl_uint32 len;
    xl_str s;
    if( znread_l(f, &len ) <= 0 ) return NULL;
    s = xls_alloc(len);
    if( s == NULL ) return NULL;
    if( zread(f,xls_ptr(s),len) < len ) { xls_free(s); return NULL; }
    xls_ptr(s)[len] = '\0';
    s->length = len;
    return s;
}

xl_str	xlsprintf(xl_str s,const char* fmt, ...)
{

    va_list ap;
    va_start(ap, fmt);
    s = xlsvprintf(s,fmt,ap);
    va_end(ap);
    return s;
}

static int xlsprintf_flush(zvformatter_buff *vbuff)
{
    xl_str s = (xl_str) vbuff->param;
    xl_str ns;
    unsigned nc = s->capacity > 2 ? s->capacity*2 : 3 ;
    unsigned clen = vbuff->curpos - xls_get(s);
    s->length = clen;
    ns = xls_realloc(s,nc);
    if( !ns ) return -1;
    s = ns;
    s->capacity = nc;
    s->length = clen;
    vbuff->curpos = xls_get(s) + clen;
    vbuff->endpos = xls_get(s) + xls_cap(s);
    vbuff->param = (void*)s;
    vbuff->curpos[0] = 0;
    vbuff->endpos[0] = 0;
    return 0;
}

xl_str	xlsvprintf(xl_str s,const char* fmt, va_list ap)
{
    int cc;
    zvformatter_buff vbuff;
    if( s == NULL ) {
	s = xls_alloc(10);
	if( s == NULL ) return NULL;

    }
    if( s->ref_cnt > 1 ) {
	int x = s->capacity;
	xls_free(s);
	s = xls_alloc(x);
    }

    vbuff.curpos = xls_get(s);
    vbuff.endpos = xls_get(s) + xls_cap(s);
    vbuff.param = (void*)s;

    cc = zvformatter(xlsprintf_flush,&vbuff,fmt,ap);

    s = (xl_str)vbuff.param;
    *(vbuff.curpos) = '\0';
    s->length = vbuff.curpos - xls_get(s);

    if( cc == -1 ) {
	xls_free(s);
	return NULL;
    } else
	return s;
}
int xls_cmp(const xl_str_t* a, const xl_str_t* b)
{

    register unsigned r;
    register const char* ac = xls_get(a);
    register const char* bc = xls_get(b);
    register size_t len;
    if( a == b ) return 0;
    if( a )
	if( !b )
	    return 1;
	else ;
    else
	if( !b )
	    return 0;
	else
	    return -1;
/*
    r = *ac - *bc;
    if( r ) return r;
*/
    len = zmin( _xls_len(a),_xls_len(b));
/*
    ac++, bc++, len--;
    if( len <= 0 ) return 0;
    r = *ac - *bc;
    if( r ) return r;

    ac++, bc++,len--;
    if( len <= 0 ) return 0;
*/
    r = memcmp(ac,bc,len);
    if( r ) return r;
    return _xls_len(a) - _xls_len(b);
}

int xls_eq(const xl_str_t* a, const xl_str_t* b)
{
    register int r;
    r = (_xls_len(a) - _xls_len(b)) == 0;
    if( !r ) return 0;
    return xls_cmp(a,b) == 0;
}



