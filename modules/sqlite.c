/*
* File: 	sqlite.c
*
* Id:		$Id: sqlite.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl extensions library
*
* Description:	SQLite module for AXL
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

#include <sqlite.h>


/*--------------------------------------------------
 * SQLITE RESULT header 
 *
 */
struct sqliter {
	char**	result;
	int	nrows;
	int     ncols;
	int	affected_rows;
	int	insert_row_id;
	sqlite* db;
};

typedef struct sqliter sqliter;
			
sqliter*	sqliter_new(sqlite* db,const char* msg,char** errmsg);
int		sqliter_free(sqliter* res);

int		sqliter_nrows(sqliter* res);
int		sqliter_ncols(sqliter* res);
const char*	sqliter_col_name(sqliter* res,int coln);
int		sqliter_col_id(sqliter* res,const char*);
int		sqliter_affected_rows(sqliter* res);
int		sqliter_insert_id(sqliter* res);
char**		sqliter_get_row(sqliter* res,int row);

const char*	sqliter_get(sqliter* res,int row,int col);
int		sqliter_get_int(sqliter* res,int row,int col);
double		sqliter_get_double(sqliter* res,int row,int col);

const char*	sqliter_nget(sqliter* res,int row,const char* name);
int		sqliter_nget_int(sqliter* res,int row,const char* name);
double		sqliter_nget_double(sqliter* res,int row,const char* name);

/*--------------------------------------------------
 * FORWARD DECLARATIONS
 *
 */


/* SQLite: public object creator */
XLF_DECL(create_SQLite_object);

/* SQLite: postgres connection creation */
XLF_DECL(SQLite_connect);

/* SQLite: connection methods */
XLF_DECL(conn_close);
XLF_DECL(conn_exec);

/* SQLite: result methods */
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
	ZMOD_ENTRY(create_SQLite_object)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	sqlite,
	"axlSQLite",
	1,1,
	"SQLite module for AXL"
    )
ZMOD_IMPLEMENT_MODULE_END


/*--------------------------------------------------
 * MODULE VARIABLES
 *
 */
static int      SQLite_conn_type = -1;
static int      SQLite_result_type = -1;


/*--------------------------------------------------
 * MODULE INITIALIZATION and FINALIZATION
 *
 */
static void module_init(void) {
    SQLite_conn_type = xlv_ptr_register_type("SQLite.conn");
    SQLite_result_type = xlv_ptr_register_type("SQLite.result");
}
static void module_deinit(void) {
}

/*--------------------------------------------------
 * MODULE FUNCTIONS
 *
 */

XLF_DECL(create_SQLite_object)
{
    xl_object SQLite_obj = xlo_new_map();
    strcpy(SQLite_obj->name,".SQLite");

    xlo_add_field_func_builtin	(SQLite_obj,"connect", SQLite_connect,".DMBS.connect");
    xlo_add_field_str		(SQLite_obj,"error","");

    XLF_RETURN_OBJ(SQLite_obj);
}

static void wrapper_DMBS_free(void* p)
{
    if( p ) {
        sqlite_close((sqlite*)p);
    }
}
XLF_DECL(SQLite_connect)
{
    int ok = 0;
    sqlite* c = NULL;

    xl_object SQLite_object = (xl_object)XLF_THIS;
    const char*     filename = XLF_ARG_STR(0,NULL);
    char* error_message = "bad arguments";

    ok = SQLite_object != NULL && filename != NULL;
    c = sqlite_open(filename,2,&error_message);
    if( !ok || !c ) {
        if( SQLite_object )
	    xlo_add_field_str(SQLite_object,"error",error_message);
        XLF_RETURN_NIL();
    } else {
        xl_object conn;
        conn = xl_res_new( c, SQLite_conn_type, (xl_res_destructor)wrapper_DMBS_free);

        xlo_add_field_func_builtin(conn,"close", conn_close,"SQlite.conn.close");
        xlo_add_field_func_builtin(conn,"exec", conn_exec,"SQlite.conn.exec");
        
        strcpy(conn->name,".DMBS.conn");
        XLF_RETURN_OBJ(conn);
    }
}

XLF_DECL(conn_close)
{
    xl_object conn = (xl_object)XLF_THIS;
    
    sqlite* c = (sqlite*)xl_res_get(conn,SQLite_conn_type);
    if( c != NULL ) {
	wrapper_DMBS_free(c);
    }
    xl_res_set(conn,NULL,SQLite_conn_type);
    XLF_RETURN_INT(0);
}

static void wrapper_SQLite_RESULT_free(void* p)
{
    if( p ) {
        /* 
            TODO: insert destructor of SQLite result here 
        */
    }
}
XLF_DECL(conn_exec)
{
    xl_object	conn	= (xl_object) XLF_THIS;
    const char*	query	= XLF_ARG_STR(0,NULL);
    int ok = 0;
    sqliter* result = NULL;
    sqlite* c = xl_res_get(conn,SQLite_conn_type);
    char* errmsg = "bad arguments";
    if( !query ) XLF_RETURN_NIL();
    
    result = sqliter_new(c,query,&errmsg);
    
    if( ! result  ) {
        xlo_add_field_str(conn,"error",errmsg);
        XLF_RETURN_NIL();
    } else {
	xl_object res = xl_res_new( result, SQLite_result_type, (xl_res_destructor)wrapper_SQLite_RESULT_free);

        xlo_add_field_func_builtin(res,"fname", res_fname,"SQLite.result.getvalue_fname");
        xlo_add_field_func_builtin(res,"getvalue", res_getvalue,"SQLite.result.getvalue");
        xlo_add_field_func_builtin(res,"getvalue_integer", res_getvalue_integer,"SQLite.result.getvalue_integer");
        xlo_add_field_func_builtin(res,"getvalue_float", res_getvalue_float,"SQLite.result.getvalue_float");
        xlo_add_field_func_builtin(res,"close", res_close,"SQLite.result.close");

        xlo_add_field_int(res,"ntuples", sqliter_nrows(result));
        xlo_add_field_int(res,"nfields", sqliter_ncols(result));
        xlo_add_field_int(res,"affected_rows", sqliter_affected_rows(result));
        xlo_add_field_int(res,"last_insert_id", sqliter_insert_id(result));

        strcpy(conn->name,".SQLite.result");
        XLF_RETURN_OBJ(res);
    }
}

XLF_DECL(res_getvalue)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    sqliter* r = (sqliter*) xl_res_get(res,SQLite_result_type);
    const char* s;
    if(! r || tuple < 0 || field < 0)
	XLF_RETURN_NIL();

    s = sqliter_get(r,tuple,field);
    
    xlv_set_str(XLF_RET,s);

    xls_erase_blank(ret->v.s);
    
    XLF_RETURN_OK();
}

XLF_DECL(res_getvalue_integer)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    sqliter* r = (sqliter*) xl_res_get(res,SQLite_result_type);
    const char* s;
    if(! r || tuple < 0 || field < 0)
	XLF_RETURN_NIL();

    s = sqliter_get(r,tuple,field);

    if(! r || !s || tuple < 0 || field < 0)
	XLF_RETURN_NIL();
	
    XLF_RETURN_INT(xl_str2int(s,NULL));
}

XLF_DECL(res_getvalue_float)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    sqliter* r = (sqliter*) xl_res_get(res,SQLite_result_type);
    const char* s;
    if(! r || tuple < 0 || field < 0)
	XLF_RETURN_NIL();

    s = sqliter_get(r,tuple,field);	

    XLF_RETURN_INT(xl_str2int(s,NULL));
}

XLF_DECL(res_fname)
{
    xl_object res = (xl_object)XLF_THIS;
    int index = XLF_ARG_INT(0,-1);
    sqliter* r = (sqliter*) xl_res_get(res,SQLite_result_type);
    const char* s;
    if(! r || index < 0)
	XLF_RETURN_NIL();

    s = sqliter_col_name(r,index);

    XLF_RETURN_STR(s);
}
XLF_DECL(res_close)
{
    xl_object res = (xl_object)XLF_THIS;
    sqliter* r = (sqliter*) xl_res_get(res,SQLite_result_type);
    
    if( r != NULL )
	wrapper_SQLite_RESULT_free(r);
    xl_res_set(res,NULL,SQLite_result_type);
    XLF_RETURN_INT(0);
}

/*--------------------------------------------------
 * SQLITE_RESULT implementation
 *
 */


sqliter*	sqliter_new(sqlite* db,const char* msg,char** errmsg)
{
    sqliter* r;
    int e;
    if( !db || !msg) {
        if( errmsg ) *errmsg = "bad arguments: no database specified";
        return NULL;
    }
    r = (sqliter*)calloc( 1, sizeof( sqliter ));
    r->db = db;
    e = sqlite_get_table(db,msg,&r->result, &r->nrows, &r->ncols, errmsg );
    if( e != SQLITE_OK ) {
        free(r);
        return NULL;
    }
    r->affected_rows = sqlite_changes(db);
    r->insert_row_id = sqlite_last_insert_rowid(db);
    return r;
    
}
int		sqliter_free(sqliter* res)
{
    if( res ) {
        if( res->result ) {
            sqlite_free_table(res->result);
            res->result = NULL;
        }
        free(res);
        return 0;
    } else 
        return -1;	
}

int		sqliter_nrows(sqliter* res)
{
    return res ? res->nrows : -1;
}
int		sqliter_ncols(sqliter* res)
{
    return res ? res->ncols : -1;
}
const char*	sqliter_col_name(sqliter* res,int coln)
{
    return !res || ! (coln < res->ncols ) ? NULL
        : res->result[coln];
}

int		sqliter_col_id(sqliter* res,const char* name)
{
    int i;
    for( i = 0; i < res->ncols; i++ ) 
        if( stricmp(res->result[i],name) == 0 ) 
            return i;
        return -1;
}

int		sqliter_affected_rows(sqliter* res)
{
    return res->affected_rows;
}
int		sqliter_insert_id(sqliter* res)
{
    return res->insert_row_id;
}

char**		sqliter_get_row(sqliter* res,int row)
{
    return res->result+res->ncols + row * res->ncols;
}

const char*	sqliter_get(sqliter* res,int row,int col)
{
    return !res || col >= res->ncols || row >= res->nrows || 
        col < 0 || row < 0 
        ? NULL
        : *(res->result+res->ncols + row*res->ncols + col);
}

const char*	sqliter_nget(sqliter* res,int row,const char* name)
{
    int col = sqliter_col_id(res,name);
    return !res || col >= res->ncols || row >= res->nrows || 
        col < 0 || row < 0 
        ? NULL
        : *(res->result+res->ncols + row*res->ncols + col);
}

int		sqliter_get_int(sqliter* res,int row,int col)
{
    const char* x = sqliter_get(res,row,col);
    if( x ) return atoi(x);
    return 0;
}
double		sqliter_get_double(sqliter* res,int row,int col)
{
    const char* x = sqliter_get(res,row,col);
    if( x ) return atof(x);
    return 0.0;	
}

int		sqliter_nget_int(sqliter* res,int row,const char* name)
{
    const char* x = sqliter_nget(res,row,name);
    if( x ) return atoi(x);
    return 0;	
}
double		sqliter_nget_double(sqliter* res,int row,const char* name)
{
    const char* x = sqliter_nget(res,row,name);
    if( x ) return atof(x);
    return 0.0;	
}


