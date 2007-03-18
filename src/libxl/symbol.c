/*
* File:	symbol.c
*
* Id:           $Id: symbol.c 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	functions dealing with internals of symbol information
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
#include "zcompat/zpio.h"
#include "zcompat/ztools.h"

#include "axl_i.h"

/* IMPLEMENTED GLOBAL FUNCTIONS */
int     xlsm_add_dim(xl_symbol* sym,int dim);
xl_symbol* xlsm_new();
void    xlsm_done(xl_symbol* sym);
void    xlsm_free(xl_symbol* sym);

/*IMPLEMENTATION */
xl_symbol* xlsm_new()
{
    xl_symbol* a = axl_calloc(1,sizeof(xl_symbol));
    a->size = 1;
    a->base = 0;
    return a;
}
int    xlsm_add_dim(xl_symbol* sym,int dim)
{
    int* x = axl_realloc(sym->dim, (sym->dimc+1)*sizeof(int));
    int osize = sym->size;
    if( !x ) return -XLE_MEMORY;

    sym->dim = x;
    sym->dim[sym->dimc] = dim;
    sym->size *= dim;
    frame_cur()->size+= sym->size - osize;
    sym->dimc++;
    return 0;
}

void    xlsm_done(xl_symbol* sym)
{
    if( sym->dim ) {
        axl_free(sym->dim);
    }
    if( sym->index_code )
        xlc_free(sym->index_code);
    sym->dim = NULL;
    xls_free(sym->name);
}
void    xlsm_free(xl_symbol* sym)
{
    if( sym != NULL ) {
        xlsm_done(sym);
        axl_free(sym);
    }
}

int xlsm_index_begin(xl_symbol* sym, xl_code code)
{
    if( sym->index_code ) {
        xlc_free(sym->index_code);
    }
    sym->index_code = code;
    sym->index_dim = 0;
    sym->actual_size = sym->size/sym->dim[sym->index_dim];
    if( sym->actual_size > 1 ) 
        sym->index_code = xlcg_operator(XLOP_MUL, sym->index_code, xlcg_load_integer(sym->actual_size));
    return 0;
}
int xlsm_index_cont(xl_symbol* sym, xl_code code)
{
    xl_code c = code;
    sym->index_dim += 1;
    if( sym->index_dim >= sym->dimc ) {
        xlc_free(code);
        return -1;
    }
    sym->actual_size = sym->actual_size/sym->dim[sym->index_dim];
    if( sym->actual_size > 1 ) 
        c = xlcg_operator(XLOP_MUL, code, xlcg_load_integer(sym->actual_size));
    sym->index_code = xlcg_operator(XLOP_ADD, sym->index_code, c);
    return 0;
}
xl_code xlsm_index_detach_code(xl_symbol* sym)
{
    xl_code code = sym->index_code;
    sym->index_code = NULL;
    return code;
}

