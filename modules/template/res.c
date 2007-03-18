/*
* File: 	resource.c
*
* Id:		$Id: res.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl extensions library
*
* Description:	NOTREADY !!! resource template  module for AXL
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

#include "zcompat/zmod.h"
#include "zcompat/zpio.h"
#include <stdlib.h>
#include <stdio.h>
#include "axl/axl.h"


/*--------------------------------------------------
 * FORWARD DECLARATIONS
 *
 */


/* RESOURCE: public object creator */
XLF_DECL(create_RESOURCE_object);

/* RESOURCE: postgres connection creation */
XLF_DECL(RESOURCE_connect);

/* RESOURCE: connection methods */
XLF_DECL(conn_close);
XLF_DECL(conn_exec);

/* RESOURCE: result methods */
XLF_DECL(res_fname);
XLF_DECL(res_getvalue);
XLF_DECL(res_getvalue_float);
XLF_DECL(res_getvalue_integer);
XLF_DECL(res_close);

static void module_init(void);
static void module_deinit(void);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_DEF_INIT(module_init);
    ZMOD_DEF_DEINIT(module_deinit);

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY(create_RESOURCE_object)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	RESOURCE,
	"axlRESOURCE",
	1,1,
	"RESOURCE module for AXL"
    )
ZMOD_IMPLEMENT_MODULE_END


/*--------------------------------------------------
 * MODULE VARIABLES
 *
 */
static int      RESOURCE_conn_type = -1;
static int      RESOURCE_result_type = -1;


/*--------------------------------------------------
 * MODULE INITIALIZATION and FINALIZATION
 *
 */
static void module_init(void) {
    RESOURCE_conn_type = xlv_ptr_register_type("RESOURCE.conn");
    RESOURCE_result_type = xlv_ptr_register_type("RESOURCE.result");
}
static void module_deinit(void) {
}

/*--------------------------------------------------
 * MODULE FUNCTIONS
 *
 */

XLF_DECL(create_RESOURCE_object)
{
    xl_object RESOURCE_obj = xlo_new_map();
    strcpy(RESOURCE_obj->name,".RESOURCE");

    xlo_add_field_func_builtin	(RESOURCE_obj,"connect", RESOURCE_connect,".DMBS.connect");
    xlo_add_field_str		(RESOURCE_obj,"error","");

    XLF_RETURN_OBJ(RESOURCE_obj);
}

static void wrapper_DMBS_free(void* p)
{
    if( p ) {
        /* 
            TODO: destroy RESOURCE connection here 
        */
    }
}
XLF_DECL(RESOURCE_connect)
{
    xl_object RESOURCE_object = XLF_THIS;
    int ok = 0;
    void* c = NULL;
    char* error_message = "bad arguments";
    /*
        TODO: make connection here
    */
    if( !ok ) {
        if( RESOURCE_object ) 
	    xlo_add_field_str(RESOURCE_object,"error",error_message);

        XLF_RETURN_NIL();
    } else {
        xl_object conn;
        conn = xl_res_new( c, RESOURCE_conn_type, (xl_res_destructor)wrapper_DMBS_free);
        
        xlo_add_field_func_builtin(conn,"close", conn_close,"RESOURCE.conn.close");
        xlo_add_field_func_builtin(conn,"exec", conn_exec,"RESOURCE.conn.exec");

        strcpy(conn->name,".DMBS.conn");
        XLF_RETURN_OBJ(conn);
    }
}

XLF_DECL(conn_close)
{
    xl_object conn = (xl_object)XLF_THIS;
    
    void* c = xl_res_get(conn,RESOURCE_conn_type);
    if( c != NULL ) {
	wrapper_DMBS_free(c);
    }
    xl_res_set(conn,NULL,RESOURCE_conn_type);
    XLF_RETURN_INT(0);
}

static void wrapper_RESOURCE_RESULT_free(void* p)
{
    if( p ) {
        /* 
            TODO: insert destructor of RESOURCE result here 
        */
    }
}
XLF_DECL(conn_exec)
{
    xl_object	conn	= (xl_object) XLF_THIS;
    const char*	query	= XLF_ARG_STR(0,NULL);
    int ok = 0;
    void* result = NULL;
    void* c = xl_res_get(conn,RESOURCE_conn_type);
    if( !query ) XLF_RETURN_NIL();
        /* 
            TODO: insert query execution code here
        */
    if( ! ok  ) {
        xlo_add_field_str(conn,"error","RESOURCE error !");
        XLF_RETURN_NIL();
    } else {
	xl_object res = xl_res_new( result, RESOURCE_result_type, (xl_res_destructor)wrapper_RESOURCE_RESULT_free);

        xlo_add_field_func_builtin(res,"fname", res_fname,"RESOURCE.result.getvalue_fname");
        xlo_add_field_func_builtin(res,"getvalue", res_getvalue,"RESOURCE.result.getvalue");
        xlo_add_field_func_builtin(res,"getvalue_integer", res_getvalue_integer,"RESOURCE.result.getvalue_integer");
        xlo_add_field_func_builtin(res,"getvalue_float", res_getvalue_float,"RESOURCE.result.getvalue_float");
        xlo_add_field_func_builtin(res,"close", res_close,"RESOURCE.result.close");

        xlo_add_field_int(res,"ntuples", 0 /* TODO: number of rows */);
        xlo_add_field_int(res,"nfields", 0 /* TODO: number of fields */);

        strcpy(conn->name,".RESOURCE.result");
        XLF_RETURN_OBJ(res);
    }
}

XLF_DECL(res_getvalue)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    void* r = (void*) xl_res_get(res,RESOURCE_result_type);
    char* s = NULL;
    
        /* 
            TODO: insert a getvalue execution here !
        */    

    if(! r || !s || tuple < 0 || field < 0)
	XLF_RETURN_NIL();
	
    xlv_set_str(XLF_RET,s);

    xls_erase_blank(ret->v.s);
    
    XLF_RETURN_OK();
}

XLF_DECL(res_getvalue_integer)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    void* r = (void*) xl_res_get(res,RESOURCE_result_type);
    char* s = NULL;
    
        /* 
            TODO: insert a getvalue execution here !
        */    

    if(! r || !s || tuple < 0 || field < 0)
	XLF_RETURN_NIL();
	

    XLF_RETURN_INT(xl_str2int(s,NULL));
}

XLF_DECL(res_getvalue_float)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    void* r = (void*) xl_res_get(res,RESOURCE_result_type);
    char* s = NULL;
    
        /* 
            TODO: insert a getvalue execution here !
        */    

    if(! r || !s || tuple < 0 || field < 0)
	XLF_RETURN_NIL();
	

    XLF_RETURN_INT(xl_str2int(s,NULL));
}

XLF_DECL(res_fname)
{
    xl_object res = (xl_object)XLF_THIS;
    int index = XLF_ARG_INT(0,-1);
    void* r = (void*) xl_res_get(res,RESOURCE_result_type);

    if( !r || index < 0 )
	XLF_RETURN_NIL();
    
        /* 
            TODO: get field name code here
        */

    XLF_RETURN_STR("");
}
XLF_DECL(res_close)
{
    xl_object res = (xl_object)XLF_THIS;
    void* r = (void*) xl_res_get(res,RESOURCE_result_type);
    
    if( r != NULL )
	wrapper_RESOURCE_RESULT_free(r);
    xl_res_set(res,NULL,RESOURCE_result_type);
    XLF_RETURN_INT(0);
}
