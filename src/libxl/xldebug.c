/*
* File:	        xldebug.c
*
* Id:           $Id: xldebug.c 1.6 04/03/07 21:24:29+01:00 nikt@nowy.dom $
*
* Project:	axl
*
* Purpose:      debug and report facilities
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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "axl/axl.h"
#include "zcompat/zpio.h"

#include "axl_i.h"

void    xldbg_val_print(ZSTREAM s, const xl_value_t* val,int options);
void    xldbg_set_stream(ZSTREAM s);

xl_bool xldbg_enable_mask(int mask, xl_bool enable);
int     xldbg_add_mask(const char* name, xl_bool enabled);
void    xldbg_trace(int mask, const char* format, ...);
void    xl_trace(int mask, const char* format, ...);

ZSTREAM xl_get_debug_stream(void);
void	xl_set_debug_stream(ZSTREAM s);

#define CASENAME(a,b) case a: return b;

ZSTREAM xldbg_stream = NULL;

/*#################################
const char* xl_encode_q_str(const char* str,size_t len, char* dest,size_t dest_size)
{
}
*/
/*#################################
## =Function xldbg_val_get_type_name
##
## =Synopsis
##  |   const char* xldbg_val_get_type_name(int type)
##
##  =Description
##
##
##
##  =Return value
##
##
*/
const char* xldbg_val_get_type_name(int type)
{
    switch( type ) {
        CASENAME(XLV_NIL,   "nil");		
        CASENAME(XLV_INT,   "int");
        CASENAME(XLV_FLOAT, "flt");
        CASENAME(XLV_RES_PTR,"ptr");
        CASENAME(XLV_CHAR,  "chr");
        CASENAME(XLV_STR_BUF,"chb");	
        CASENAME(XLV_PINT,  "pin");
        CASENAME(XLV_PFLOAT,"pfl");
        CASENAME(XLV_PBYTE, "pby");
        CASENAME(XLV_STR,   "xst");
        CASENAME(XLV_FUNC_IP,"fip");
        CASENAME(XLV_FUNC_LABEL,"flb");
        CASENAME(XLV_FUNC_BUILTIN,"fbu");
        CASENAME(XLV_USER_VAL,"uva");
        CASENAME(XLV_REF,   "ref");
        CASENAME(XLV_STACK,"stk");
        CASENAME(XLV_CHAR_REF,"crf");
        CASENAME(XLV_ITERATOR,"ite");
        CASENAME(XLV_MREF,  "mrf");
        CASENAME(XLV_MAP_OBJ,"omp");
        CASENAME(XLV_USER_OBJ,"ous");
        CASENAME(XLV_LAST,  "lst");
    default:
        return "unk";
    }
}
/*#################################
## =Function xldbg_val_print
##
## =Synopsis
##  |   void    xldbg_val_print(ZSTREAM s, const xl_value_t* val,int options);
##
##  =Description
##
##
##
##  =Return value
##
##
*/
void    xldbg_val_print(ZSTREAM s, const xl_value_t* val,int options)
{
    xl_bool follow_ref = 1;
    xl_bool show_val = 1;
    xl_bool only_last = 0;
    xl_value_t temp;
    xl_value next;
    xlv_clear(&temp);
    while( val ) {
        xl_bool vderef = xlv_need_deref(val),
            vnextr = xlv_need_extract(val),
            vro = !xlv_can_write(val),
            vobj = xlv_is_obj(val);
        zfprintf(s, "%s%s%s(%02i:%s)", 
            vro     ? "R" : "",
            vnextr  ? "E" : "",
            vderef  ? "D" : "",
            val->type,
            xldbg_val_get_type_name(val->type));
        
        if( show_val ) {
            if( val->type != XLV_NIL && vnextr ) {
                if( xlv_extract(val,&temp) < 0 ) 
                    zfprintf(s,"EXTRACT ERROR");
                else {
                    zfprintf(s,"EXT->");
                    xldbg_val_print(s,&temp,options);
                    xlv_done(&temp);
                }
                break;
            } else if( vobj ) {
                zfprintf(s,"OBJ");
            } else if( vderef && follow_ref && (next = xlv_deref(val)) != val) {
                next = xlv_deref(val);
                switch(val->type) {
                case XLV_REF:
                    if( val->v.ref.parent )
                        zfprintf(s,"REF [fo:0x%08x] ->",val->v.ref.parent);
                    else
                        zfprintf(s,"REF->");
                    if( !val->v.ref.p ) 
                        zfprintf(s,"[NULL]");
                    break;
                case XLV_STACK:
                    zfprintf(s,"STK[#%i]->",val->v.stack.index);
                    break;
                default:
                    zfprintf(s,"->");
                }
                
                val = next;
                continue;
            }
            zfprintf(s,"=");
            {
                int xlasm_put_value();
                xlasm_put_value(s,val);
            }
            break;
        }
    }
}

typedef struct mask_t {
    char*       name;
    long        flags;
    ZSTREAM     output;
} mask_t;

mask_t  mask_table[100];
int     mask_table_capacity =         100;
int     mask_table_i = 0;

/*#################################
##
## =Function xldbg_add_mask
## =Synopsis
##   |int     xldbg_add_mask(const char* name, xl_bool enabled);
##
## =Description
##
## =Return value
##
##
*/
static void del_names(void) {
    int i;
    for(i=0;i< mask_table_i;i++) {
        if(mask_table[i].name ) axl_free(mask_table[i].name);
        mask_table[i].name = NULL;
    }
}
int     xldbg_add_mask(const char* name, xl_bool enabled)
{
    if( mask_table_i >= mask_table_capacity )
        return -1;
    mask_table[mask_table_i].name = axl_strdup((char*)name);
    mask_table[mask_table_i].flags = enabled ? 1 : 0;
    mask_table[mask_table_i].output = NULL;
    if( mask_table_i == 0 ) 
        atexit(del_names);
    return mask_table_i++;
}

/*#################################
## =Function xldbg_enable_mask
##
## =Synopsis
##  |   xl_bool     xldbg_enable_mask(int mask, xl_bool enable);
##
##  =Description
##
##
##
##  =Return value
##
##
*/
xl_bool     xldbg_enable_mask(int mask, xl_bool enable)
{
    if( mask >= 0 && mask_table_i >= mask_table_capacity )
        return xl_false;
    return (mask_table[mask].flags = enable);
}

xl_bool     xldbg_mask_enabled(int mask)
{
    return mask >= 0 && 
        mask < mask_table_i &&
        mask_table[mask].flags;
}
int     xldbg_get_mask(const char* name)
{
    int i =0;
    for( ; i < mask_table_i ; i++ ) {
        if( mask_table[i].name && strcmp(mask_table[i].name,name) == 0 )
            return i;
    }
    return xldbg_add_mask(name,xl_false);
}
/*#################################
## =Function xldbg_trace
##
## =Synopsis
##  |   void    xldbg_trace(int mask, const char* format, ...);
##  |   void    xl_trace(int mask, const char* format, ...);
##
##  =Description
##
##      <mask> is mask id obtained by xldbg_get/add_mask
##
##      Informative message is sent to default xl debug stream
##      if specified mask is enabled.
##
*/
void    xldbg_trace(int mask, const char* format, ...)
{
    if( mask >= 0 && 
        mask < mask_table_i &&
        mask_table[mask].flags )
    {
        ZSTREAM s = mask_table[mask].output;
        va_list ap;
        va_start(ap,format);
        if( !s ) 
            s = xl_get_debug_stream();
        zvfprintf(s,format,ap);
        va_end(ap);
    }
}
void    xldbg_print(const char* format, ...)
{
    ZSTREAM s;
    va_list ap;
    s = xl_get_debug_stream();
    va_start(ap,format);
    if( s && format ) 
        zvfprintf(s,format,ap);
    va_end(ap); 
}

void    xl_trace(int mask, const char* format, ...)
{
    if( mask >= 0 && 
        mask < mask_table_i &&
        mask_table[mask].flags )
    {
        ZSTREAM s = mask_table[mask].output;
        va_list ap;
        va_start(ap,format);
        if( !s ) 
            s = xl_get_debug_stream();
        zvfprintf(s,format,ap);
        va_end(ap);
    }
}

ZSTREAM xl_get_debug_stream(void)
{
    if( xldbg_stream == NULL )
	xldbg_stream = zstream_ref((ZSTREAM) xl_get_ctl_prop(XLCP_PARSE_OUT));
    return xldbg_stream;
}

void	xl_set_debug_stream(ZSTREAM s)
{
    if( xldbg_stream )
	zclose(xldbg_stream);
    xldbg_stream = zstream_ref(s);
}
void	xldbg_done_module(void);
void	xldbg_init_module(void);

void	xldbg_done_module(void)
{
    del_names();
    if( xldbg_stream )
	zclose(xldbg_stream);
    xldbg_stream = NULL;
}
void	xldbg_init_module(void)
{
}
