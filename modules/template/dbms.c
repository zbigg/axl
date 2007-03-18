/*
* File: 	DBMS.c
*
* Id:		$Id: dbms.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl extensions library
*
* Description:	DBMS module for AXL
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


/* DBMS: public object creator */
XLF_DECL(create_DBMS_object);

/* DBMS: postgres connection creation */
XLF_DECL(DBMS_connect);

/* DBMS: connection methods */
XLF_DECL(conn_close);
XLF_DECL(conn_exec);

/* DBMS: result methods */
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
	ZMOD_ENTRY(create_DBMS_object)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	DBMS,
	"axlDBMS",
	1,1,
	"DBMS module for AXL"
    )
ZMOD_IMPLEMENT_MODULE_END


/*--------------------------------------------------
 * MODULE VARIABLES
 *
 */
static int      DBMS_conn_type = -1;
static int      DBMS_result_type = -1;


/*--------------------------------------------------
 * MODULE INITIALIZATION and FINALIZATION
 *
 */
static void module_init(void) {
    DBMS_conn_type = xlv_ptr_register_type("DBMS.conn");
    DBMS_result_type = xlv_ptr_register_type("DBMS.result");
}
static void module_deinit(void) {
}

/*--------------------------------------------------
 * MODULE FUNCTIONS
 *
 */

XLF_DECL(create_DBMS_object)
{
    xl_object DBMS_obj = xlo_new_map();
    strcpy(DBMS_obj->name,".DBMS");

    xlo_add_field_func_builtin	(DBMS_obj,"connect", DBMS_connect,".DMBS.connect");
    xlo_add_field_str		(DBMS_obj,"error","");

    XLF_RETURN_OBJ(DBMS_obj);
}

static void wrapper_DMBS_free(void* p)
{
    if( p ) {
        /* 
            TODO: destroy DBMS connection here 
        */
    }
}
XLF_DECL(DBMS_connect)
{
    xl_object DBMS_object = XLF_THIS;
    int ok = 0;
    void* c = NULL;
    char* error_message = "bad arguments";
    /*
        TODO: make connection here
    */
    if( !ok ) {
        if( DBMS_object ) 
	    xlo_add_field_str(DBMS_object,"error",error_message);

        XLF_RETURN_NIL();
    } else {
        xl_object conn;
        conn = xl_res_new( c, DBMS_conn_type, (xl_res_destructor)wrapper_DMBS_free);
        
        xlo_add_field_func_builtin(conn,"close", conn_close,"DBMS.conn.close");
        xlo_add_field_func_builtin(conn,"exec", conn_exec,"DBMS.conn.exec");

        strcpy(conn->name,".DMBS.conn");
        XLF_RETURN_OBJ(conn);
    }
}

XLF_DECL(conn_close)
{
    xl_object conn = (xl_object)XLF_THIS;
    
    void* c = xl_res_get(conn,DBMS_conn_type);
    if( c != NULL ) {
	wrapper_DMBS_free(c);
    }
    xl_res_set(conn,NULL,DBMS_conn_type);
    XLF_RETURN_INT(0);
}

static void wrapper_DBMS_RESULT_free(void* p)
{
    if( p ) {
        /* 
            TODO: insert destructor of DBMS result here 
        */
    }
}
XLF_DECL(conn_exec)
{
    xl_object	conn	= (xl_object) XLF_THIS;
    const char*	query	= XLF_ARG_STR(0,NULL);
    int ok = 0;
    void* result = NULL;
    void* c = xl_res_get(conn,DBMS_conn_type);
    if( !query ) XLF_RETURN_NIL();
        /* 
            TODO: insert query execution code here
        */
    if( ! ok  ) {
        xlo_add_field_str(conn,"error","DBMS error !");
        XLF_RETURN_NIL();
    } else {
	xl_object res = xl_res_new( result, DBMS_result_type, (xl_res_destructor)wrapper_DBMS_RESULT_free);

        xlo_add_field_func_builtin(res,"fname", res_fname,"DBMS.result.getvalue_fname");
        xlo_add_field_func_builtin(res,"getvalue", res_getvalue,"DBMS.result.getvalue");
        xlo_add_field_func_builtin(res,"getvalue_integer", res_getvalue_integer,"DBMS.result.getvalue_integer");
        xlo_add_field_func_builtin(res,"getvalue_float", res_getvalue_float,"DBMS.result.getvalue_float");
        xlo_add_field_func_builtin(res,"close", res_close,"DBMS.result.close");

        xlo_add_field_int(res,"ntuples", 0 /* TODO: number of rows */);
        xlo_add_field_int(res,"nfields", 0 /* TODO: number of fields */);

        strcpy(conn->name,".DBMS.result");
        XLF_RETURN_OBJ(res);
    }
}

XLF_DECL(res_getvalue)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    void* r = (void*) xl_res_get(res,DBMS_result_type);
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
    void* r = (void*) xl_res_get(res,DBMS_result_type);
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
    void* r = (void*) xl_res_get(res,DBMS_result_type);
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
    void* r = (void*) xl_res_get(res,DBMS_result_type);

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
    void* r = (void*) xl_res_get(res,DBMS_result_type);
    
    if( r != NULL )
	wrapper_DBMS_RESULT_free(r);
    xl_res_set(res,NULL,DBMS_result_type);
    XLF_RETURN_INT(0);
}
