/*
* File:		symtab.c
*
* Id:           $Id: symtab.c 1.1 04/01/19 00:47:58+01:00 nikt@nowy.dom $
*
* Project:	axl
*
* Description: assembler code generation for AXL virtual machine
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#include <axl/axl.h>

/*
    initialize/deinitialize symbol table 
    used by
        xl_init()
        xl_done()
*/
void    xl_module_symtab_init();
void    xl_module_symtab_done();
/*
##  =Function xlsym_get_str,xlsym_get_xlstr
##      get name of symbol
##
##  =Synopsis
##  |const char*     xlsym_get_str(int id);
##  |xl_str          xlsym_get_xlstr(int id);
##
##  =Description
##      Get name of specified symbol.
##
##      All symbol names are kept in symbol table until
##      <xl_done> or <xlsym_free> is called.
##
##  =Return values
##      Return <READ-ONLY> string to user.
##      <xl_str> can be locked by xl_str ref.
##      If no such symbol is present in symbol table
##      the emtpy string - not NULL - is returned.
##
*/
const char*     xlsym_get_str(int id);
xl_str          xlsym_get_xlstr(int id);

/*
##  =Function xlsym_search,xlsym_searchx
##      search for symbol in symtable
##
##  =Synopsis
##  |int             xlsym_search(const char* name);
##  |int             xlsym_searchx(const xl_str_t* name);
##
##  =Description
##      Search for symbol <name> in symbol table. If no such symbol
##      is present then new symbol entry is created.
##
##      If you use xlsym_searchx then, passed string can be locked by
##      <xls_ref>.
##  =Return values
##      Returns <identifier> of symbol that can be used by
##      <xlsym_get_str>,<xlsym_get_xlstr>.
##
*/
int             xlsym_search(const char* x);
int             xlsym_searchx(const xl_str_t* s);
/*
##  =Function xlsym_reset
##      reset all symbols
##
##  =Synopsis
##  |#include <something.h>
##  |void            xlsym_reset();
##
##  =Description
##      Reset all symbols in symbol table. Should be
##      called before new compilation.
##
*/
void            xlsym_reset();
/*
##  =Function xlsym_free
##      short description of xlsym_free
##
##  =Synopsis
##  |void            xlsym_free();
##
##  =Description
##      free all symbols in symbol table
##
*/
void            xlsym_free();


static xlmap*  symbol_map = NULL;

DEFINE_VECTOR(xl_str, xlstrvec);
IMPLEMENT_VECTOR(xl_str, xlstrvec);

xlstrvec* symbol_vector = NULL;

void    xl_module_symtab_init()
{
    symbol_map = xlmap_new(NULL);
    symbol_vector = xlstrvec_new(0);
}

void    xl_module_symtab_done()
{
    xlsym_free();
}

void            xlsym_reset()
{
    /* 
        there are no data associated with symbols yet
        so no reset is needed.
    */
}

void xlsym_dump(ZSTREAM s);
void            xlsym_free()
{
/*    xlsym_dump(zstderr);*/
    if( symbol_map )
        xlmap_free(symbol_map);
    symbol_map = NULL;
    if( symbol_vector ) 
        xlstrvec_free(symbol_vector);
    symbol_vector = NULL;
}

const char*     xlsym_get_str(int id)
{
    return xls_ptr(xlsym_get_xlstr(id));
}
xl_str          xlsym_get_xlstr(int id)
{
    size_t cnt = xlstrvec_count(symbol_vector);
    xl_str* t = xlstrvec_getp(symbol_vector);
    if( id < 0 || (size_t)id >= cnt || !t )
        return NULL;
    return t[id];
}

int             xlsym_search(const char* x)
{
    size_t cnt = xlstrvec_count(symbol_vector);
    xl_str* t = xlstrvec_getp(symbol_vector);
    xlmi_t found_iter;
    xlmap_find_str_i(symbol_map,x,&found_iter);
    if( xlmi_key(&found_iter) ) {
        /* we've found */
        return (int) xlmi_get(&found_iter);
    } else {
        int new_id = cnt;
        xl_str name = xls_new(x);
        if( xlstrvec_set_min_size(symbol_vector,cnt+1) ) {
            return -1; /* VERY WRONG */
        }
        symbol_vector->count++;
        t = xlstrvec_getp(symbol_vector);
        t[new_id] = name;
        xlmap_add(symbol_map,name,(void*)new_id);
        xls_free(name);
        return new_id;
    }
}
int             xlsym_searchx(const xl_str_t* s)
{
    size_t cnt = xlstrvec_count(symbol_vector);
    xl_str* t = xlstrvec_getp(symbol_vector);
    xlmi_t found_iter;
    xlmap_find_i(symbol_map,s,&found_iter);
    if( xlmi_key(&found_iter) ) {
        /* we've found */
        return (int) xlmi_get(&found_iter);
    } else {
        int new_id = cnt;
        const xl_str_t* name = s;
        if( xlstrvec_set_min_size(symbol_vector,cnt+1) ) {
            return -1; /* VERY WRONG */
        }
        symbol_vector->count++;
        t = xlstrvec_getp(symbol_vector);
        t[new_id] = (xl_str)name;
        xlmap_add(symbol_map,name,(void*)new_id);
        return new_id;
    }
}

void xlsym_dump(ZSTREAM s) 
{
    size_t cnt = xlstrvec_count(symbol_vector);
    xl_str* t = xlstrvec_getp(symbol_vector);
    size_t i;
    if( s == NULL )
        s = zstdout;

    zfprintf(s,"SYMTAB: begin dump\n");
    for(i=0; i<cnt; i++) {
        zfprintf(s,"SYMTAB: symbol %03i name=%s\n",i, xls_str(t[i]));
    }
    zfprintf(s,"SYMTAB: end dump\n");
}