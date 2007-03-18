/*
* File:         xlmap.c
*
* Id:           $Id: xlmap.c 1.4 03/11/27 23:47:30+01:00 zbigg@zbigg.(none) $
*
* Project:      libxl
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*   This program is Copyright(c) Zbigniew Zagorski 2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
    
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "zcompat/ztools.h"
/*#include "ztools_i.h"*/
#include "axl/axl.h"

static void* str_dup(const void* a)
{
    return (void*) xls_new_xls( (xl_str) a);
}
static void str_del(void* a)
{
    xls_free((xl_str) a);
}
static int str_cmp(const void* a,const void* b)
{
    return xls_cmp( (xl_str) a, (xl_str)b );
}
xlmap* xlmap_new(void (*f)(void*))
{
    xlmap* x = smap_new(str_cmp,str_del,str_dup);
    if( x ) 
	x->item_del = f;
    return x;
}
xlmap*  xlmap_init(xlmap* m,void (*f)(void*))
{
    smap_init(m,str_cmp,str_del,str_dup);
    m->item_del = f;
    return m;
}

void    xlmap_free(xlmap* b )
{
    smap_free(b);
}
void    xlmap_done(xlmap* b )
{
    smap_done(b);
}

int     xlmap_add(xlmap* bt,const_xl_str s,void* data)
{
    return smap_add(bt,(void*)s,data);
}

void*   xlmap_del(xlmap* bt,const_xl_str s)
{
    return smap_del(bt,(void*)s);
}
xlmi    xlmap_del_i(xlmap* bt,xlmi i)
{
    return smap_del_i(bt,i);
}

void*   xlmap_find(xlmap* bt,const_xl_str key)
{
    return smap_find(bt,(void*)key);
}
xlmi    xlmap_find_i(xlmap* bt,const_xl_str key,xlmi i)
{
    return smap_find_i(bt,(void*)key,i);
}

static int str_to_xlstr_cmp(const char* str, xl_str b)
{
    return strcmp(str, xls_str(b));
}
void*   xlmap_find_str(xlmap* bt,const char* str_key)
{
    return smap_find_cust(bt,str_key,(zt_cmp_f)str_to_xlstr_cmp);
}

xlmi    xlmap_find_str_i(xlmap* bt,const char*  str_key,xlmi i)
{
    return smap_find_cust_i(bt,str_key,(zt_cmp_f)str_to_xlstr_cmp,i);
}
int     xlmap_delete(xlmap* bt)
{
    smap_free(bt);
    return 0;
}

xlmi    xlmap_begin(xlmap* bt,xlmi i)
{
    return smap_begin(bt,i);
}
xlmi    xlmap_end(xlmap* bt,xlmi i)
{
    return smap_end(bt,i);
}

void    xlmi_free(xlmi i)
{
    smapi_free(i);
}

xlmi    xlmi_inc(xlmi i)
{
    return smapi_inc(i);
}
xlmi    xlmi_dec(xlmi i)
{
    return smapi_dec(i);
}
int     xlmi_eq(xlmi a,xlmi b)
{
    return smapi_eq(a,b);
}

void        xlmi_set(xlmi i, void* item)
{
    if( i && i->item )
        i->item->item = item;
}

void*   xlmi_get(xlmi i)
{
    if( i && i->item )
        return i->item->item;
    return NULL;
}
xl_str     xlmi_key(xlmi i)
{
    return (xl_str) smapi_key(i);
}

static void _show(smap_item* tree,int level,char* o,int dir);

int xlmap_show(xlmap* bt )
{
    _show( bt->head, 0, "",0 );
    return 0;
}

static void _show(smap_item* tree,int level,char* o,int dir)
{
    char *x = o,*y,*z;
    if( tree == NULL )
        return;
    y = malloc(strlen(x)+ 7);
        strcpy(y,x);
        strcat(y,"    ");
        strcat(y, dir ? "|" : " " );

    z = malloc(strlen(x)+ 7);
        strcpy(z,x);
        strcat(z,"    ");
        strcat(z, !dir ? "|" : " " );

    if( tree->left != NULL)
        _show(tree->left,level+1,z, 1);
    else{
/*      printf("%s    ,-- L - NULL\n",z);*/
    }
    printf("%s     %c --: %s, w:%i, level: %i,up=%s\n",
        x,
        (!dir ? 0x60 : 0x2c),
        xls_get((xl_str)tree->key),
        tree->weight,
        level,
        tree->up ? ( xls_ptr((xl_str)tree->up->key) ? xls_ptr((xl_str)tree->up->key) : "(no name)"): "none");
    if( tree->right != NULL)
        _show(tree->right,level+1,y, 0);
    else{
/*      printf("%s    `-- R - NULL\n",y); */
    }
    fflush(stdout);
    if( y )
        free(y);
    if( z )
        free(z);
}
