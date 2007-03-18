/*
* File: 	mysql.c
*
* Id:		$Id: mysql.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl library
*
* Description:	AXL MySQL interface
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

#include "zcompat/zmod.h"
#include "sstr.h"
#include "axl/axl.h"
#include <ctype.h>

#ifdef HAVE_MYSQL_MYSQL_H
#include "mysql/mysql.h"
#else 
#include "mysql.h"
#endif

#define _XL_METHOD(name) static int  name (xl_thread thread,int argc,xl_value argv,xl_value ret)

/* public object creator */
_XL_METHOD(create_mysql_object);

/* mysql object */
_XL_METHOD(mysql_xxconnect);

/* connection methods */
_XL_METHOD(conn_close);
_XL_METHOD(conn_exec);

/* result methods */
_XL_METHOD(res_fname);
_XL_METHOD(res_getvalue);
_XL_METHOD(res_close);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
    
	ZMOD_ENTRY_A(create_mysql_object,create_mysql_object)
	
	ZMOD_ENTRY_A(mysql_connect,mysql_xxconnect)
	
	ZMOD_ENTRY_A(connection_close,conn_close)
	ZMOD_ENTRY_A(connection_exec,conn_exec)

	ZMOD_ENTRY_A(result_fname,res_fname)
	ZMOD_ENTRY_A(result_getvalue,res_getvalue)
	ZMOD_ENTRY_A(result_close,res_close)

    ZMOD_ENTRIES_END
    ZMOD_INFO(
	mysql,
	"axlMySQL",
	1,0,
	"AXL MySQL Interface by Zbigniew Zagorski"
    )
ZMOD_IMPLEMENT_MODULE_END


#define _XL_THIS(thread) ((xl_object) xlv_deref(xlx_find(thread->var,"this",NULL) ))

static int  create_mysql_object(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object mysql_object = NULL;
    
    mysql_object = xlo_new_map();
    
    xlo_add_field_func_builtin(mysql_object,"connect", mysql_xxconnect,NULL);
    xlo_add_field_str(mysql_object,"error","");
    
    XLF_RETURN_OBJ(mysql_object);    
}

void wrapper_mysql_close(void* obj) 
{
    if( obj ) {
	mysql_close( (MYSQL*) obj);
    }
}
_XL_METHOD(mysql_xxconnect)
{
    xl_object mysql_object = _XL_THIS(thread);
    xl_object conn = NULL;
    if( argc < 1 ) {
	XLF_RETURN_INT(0);
    } else { /* make connection */
	const char*	dbname	= xlv_str(argv);
	const char*	myhost	= argc >= 2 ? xlv_str(argv+1) : NULL;
	unsigned int	myport	= argc >= 3 ? xlv_int(argv+2) : 0;
	const char*	login	= argc >= 4 ? xlv_str(argv+3) : NULL;
	const char*	pwd	= argc >= 5 ? xlv_str(argv+4) : NULL;

	MYSQL*		myconn; 
	if( myhost && !*myhost) myhost = NULL;
	if( login && !*login)	login = NULL;
	if( pwd && !*pwd) 	pwd = NULL;
	if( (myconn = mysql_init(NULL)) == NULL ) {
	    xlo_add_field_str(mysql_object,"error","couldn't allocate MYSQL object");
	    goto err_1;
	}
	if ( mysql_real_connect(myconn,myhost,login,pwd,
				dbname,myport,NULL,0) == NULL ) {
	    xlo_add_field_str(mysql_object,"error",mysql_error(myconn) ? mysql_error(myconn) : "unknown MySQL error");
	    goto err_2;
	}
	
	conn = xlo_new_res((void*)myconn, NULL,0, wrapper_mysql_close);
	
	xlo_add_field_int(conn,"_conn_ptr",(xl_integer)myconn);
	    
	xlo_add_field_func_builtin(conn,"close", conn_close,NULL);
	xlo_add_field_func_builtin(conn,"exec", conn_exec,NULL);
	
	xlo_add_field_str(conn,"host", myhost);
	xlo_add_field_int(conn,"port", myport);
	xlo_add_field_str(conn,"db",   dbname);
	xlo_add_field_str(conn,"error","(no error)");
	strcpy(conn->name,"msql_res");    
	    
	XLF_RETURN_OBJ(conn);
	return 0;
err_2:
	mysql_close(myconn);
err_1:
    }
    XLF_RETURN_INT(0);
}

_XL_METHOD(conn_close)
{
    xl_object conn = _XL_THIS(thread);
    MYSQL* myconn = (MYSQL*) xlo_get_res(conn);

    if( myconn ) {
    mysql_close(myconn);
    }
    xlo_set_res(conn,NULL);
    
    xlo_add_field_int(conn,"_conn_ptr",0);
    XLF_RETURN_INT(0);
}

void wrapper_mysql_free_result(void* res) 
{
    if( res )
	mysql_free_result((MYSQL_RES*)res);
}
_XL_METHOD(conn_exec)
{
    if( argc > 0 ) {
	xl_object	conn	= _XL_THIS(thread);
	MYSQL* myconn = (MYSQL*) xlo_get_res(conn);
	const char*	query	= xlv_str(argv);
	
	int	 r = mysql_real_query(myconn,query,strlen(query));
	int	ar = mysql_affected_rows(myconn);
	
	if( r == 0 || ar >= 0) {
	    MYSQL_RES* myres = mysql_store_result(myconn);
	    if( ar < 0 && myres == NULL )
		xlo_add_field_str(conn,"error",mysql_error(myconn) ? mysql_error(myconn) : "unknown MySQL error");
	    else {
		xl_object res;
		res = xlo_new_res((void*)myres,NULL,0,wrapper_mysql_free_result);
		xlo_add_field_int(res,"_res_ptr",(xl_integer)myres);
		xlo_add_field_int(res,"_row_number",0);
		xlo_add_field_func_builtin(res,"fname", res_fname,NULL);
		xlo_add_field_func_builtin(res,"field_name", res_fname,NULL);
    		xlo_add_field_func_builtin(res,"getvalue", res_getvalue,NULL);
		xlo_add_field_func_builtin(res,"close", res_close,NULL);
		if( myres ) {
		    xlo_add_field_int(res,"ntuples", mysql_num_rows(myres));
		    xlo_add_field_int(res,"num_rows", mysql_num_rows(myres));
		    xlo_add_field_int(res,"nfields", mysql_num_fields(myres));
		    xlo_add_field_int(res,"num_fields", mysql_num_fields(myres));
		} else {
		    xlo_add_field_int(res,"ntuples", 0);
		    xlo_add_field_int(res,"num_rows", 0);
		    xlo_add_field_int(res,"nfields", 0);
		    xlo_add_field_int(res,"num_fields", 0);
		}
		xlo_add_field_int(res,"affected_rows", ar);
		strcpy(res->name,"msql_res");    
		XLF_RETURN_OBJ(res);
	    }
	} else
	    xlo_add_field_str(conn,"error",mysql_error(myconn) ? mysql_error(myconn) : "unknown MySQL error");
    }
    XLF_RETURN_NIL();
}

_XL_METHOD(res_getvalue)
{
    xl_object res = _XL_THIS(thread);
    MYSQL_RES* myres = (MYSQL_RES*) xlo_get_res(res);
    MYSQL_ROW row;
    unsigned long* lengths;
    int tuple = xlv_int(argv);
    int field = xlv_int(argv+1);
    
    mysql_data_seek(myres,tuple);
    
    row = mysql_fetch_row(myres);
    lengths = mysql_fetch_lengths(myres);
    
    xlv_set_strn(ret,row[field],lengths[field]);

    xls_erase_blank(ret->v.s);
    
    return 0;
}

_XL_METHOD(res_fname)
{
    xl_object res = _XL_THIS(thread);
    MYSQL_RES* myres = (MYSQL_RES*) xlo_get_res(res);
    int	fi	= xlv_int(argv);
    int fnum	= mysql_num_fields(myres);
    MYSQL_FIELD* f = mysql_fetch_fields(myres);
/*
    if( fi < fnum )
	xlv_set_str(ret,f[fi].name);
    else    
	xlv_set_str(ret,"");
*/
    XLF_RETURN_STR( fi < fnum ? f[fi].name : "" );
    return 0;    
}
_XL_METHOD(res_close)
{
    xl_object res = _XL_THIS(thread);
    MYSQL_RES* myres = (MYSQL_RES*) xlo_get_res(res);
    
    if( myres )
    mysql_free_result(myres);
    xlo_set_res(res,NULL);
    
    XLF_RETURN_INT(0);    
}
