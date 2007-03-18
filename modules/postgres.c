/*
* File: 	postgres.c
*
* Id:		$Id: postgres.c 1.5 04/01/31 00:13:51+01:00 zbigg@nowy.dom $
*
* Project:	axl library
*
* Description:	PostgreSQL interface
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
#include "axl/axl.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


#if defined(HAVE_POSTGRESQL_LIBPQ_FE_H)
#	include "postgresql/libpq-fe.h"
#elif defined(HAVE_PGSQL_LIBPQ_FE_H)
#	include "pgsql/libpq-fe.h"
#else
#	include "libpq-fe.h"
#endif


/* public object creator */
XLF_DECL(create_postgres_object);

/* postgres object */
XLF_DECL(post_connect);

/* connection methods */
XLF_DECL(conn_close);
XLF_DECL(conn_exec);

/* result methods */
XLF_DECL(res_fname);
XLF_DECL(res_getvalue);
XLF_DECL(res_getvalue_float);
XLF_DECL(res_getvalue_integer);
XLF_DECL(res_close);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
    
	ZMOD_ENTRY_A(create_postgres_object,create_postgres_object)
	
	ZMOD_ENTRY_A(postgres_connect,post_connect)
	
	ZMOD_ENTRY_A(connection_close,conn_close)
	ZMOD_ENTRY_A(connection_exec,conn_exec)

	ZMOD_ENTRY_A(result_fname,res_fname)
	ZMOD_ENTRY_A(result_getvalue,res_getvalue)
	ZMOD_ENTRY_A(result_getvalue_integer,res_getvalue_integer)
	ZMOD_ENTRY_A(result_getvalue_float,res_getvalue_float)
	ZMOD_ENTRY_A(result_close,res_close)

    ZMOD_ENTRIES_END
    ZMOD_INFO(
	postgres,
	"axlPostgreSQL",
	1,0,
	"PostgreSQL interface for AXL"
    )
ZMOD_IMPLEMENT_MODULE_END

#define dbgprintf(a)
XLF_DECL(create_postgres_object)
{
    xl_object post_obj = xlo_new_map();
    strcpy(post_obj->name,"postgres");
    xlo_add_field_func_builtin	(post_obj,"connect", post_connect,"postgres.connect");
    xlo_add_field_str		(post_obj,"error","");
    XLF_RETURN_OBJ(post_obj);
}

static void wrapper_PQfinish(void* p)
{
    if( p ) {
	PQfinish((PGconn*)p);
	dbgprintf(("pg_conn: internal destructor for connection;\n"));
    }
}
XLF_DECL(post_connect)
{
    if( argc < 1 ) {
	xlv_set_int(ret,0);
	return 0;
    } else { /* make connection */
	const char*	dbname	=             xlv_str(argv);
	const char*	pghost	= argc >= 2 ? xlv_str(argv+1) : NULL;
	const char*	pgport	= argc >= 3 ? xlv_str(argv+2) : NULL;
	const char*	login	= argc >= 4 ? xlv_str(argv+3) : NULL;
	const char*	pwd	= argc >= 5 ? xlv_str(argv+4) : NULL;
	xl_object 	post_obj = (xl_object)XLF_THIS;
	PGconn*		c;
	if( pghost && !*pghost) pghost = NULL;
	if( pgport && !*pgport) pgport = NULL;
	if( login && !*login) login = NULL;
	if( pwd && !*pwd) pwd = NULL;
	c = PQsetdbLogin( pghost,pgport, NULL, NULL, dbname, login, pwd );
	if( PQstatus(c) == CONNECTION_BAD ) {
	    xlo_add_field_str(post_obj,"error",
			    PQerrorMessage(c) 
				? PQerrorMessage(c) 
				: "unknown PostgreSQL error");
	    PQfinish(c);	    
	} else {
	    xl_object conn;
	    
	    conn = xl_res_new((void*)c, 0, wrapper_PQfinish);
	    
	    xlo_add_field_func_builtin(conn,"close", conn_close,"pg_conn.close");
	    xlo_add_field_func_builtin(conn,"exec", conn_exec,"pg_conn.exec");
	    xlo_add_field_str(conn,"host", PQhost(c) ? PQhost(c) : "" );
	    xlo_add_field_int(conn,"port", PQport(c) ? atoi( PQport(c) ) : 0);
	    xlo_add_field_str(conn,"db",   PQdb(c));
	    xlo_add_field_str(conn,"login",login);
	    xlo_add_field_str(conn,"error","");
	    xlo_add_field_str(conn,"name","connection");
	    strcpy(conn->name,"pg_conn");
	    XLF_RETURN_OBJ(conn);
	}
    }
    XLF_RETURN_NIL();
}

XLF_DECL(conn_close)
{
    xl_object conn = (xl_object)XLF_THIS;
    PGconn* c = xl_res_get(conn,0);
    if( c != NULL )
	PQfinish(c);
    dbgprintf(("pg_conn: close\n"));
    xl_res_set(conn,NULL,0);
    XLF_RETURN_INT(0);
}

static void wrapper_PQclear(void* p)
{
    if( p ) {
	PQclear((PGresult*)p);
	dbgprintf(("pg_result: internal destructor for result();\n"));
    }
}
XLF_DECL(conn_exec)
{
    if( argc > 0 ) {
	xl_object	conn	= (xl_object) XLF_THIS;
	const char*	query	= XLF_ARG_STR(0,NULL);
	PGconn* 	c 	= xl_res_get(conn,0);
	PGresult*	r	= c && query ? PQexec(c,query) : NULL;
	switch( PQresultStatus(r) ) {
	case PGRES_EMPTY_QUERY:
	case PGRES_COMMAND_OK:
	case PGRES_TUPLES_OK:
	    {
		xl_object res = xl_res_new((void*)r,  0, wrapper_PQclear);

		xlo_add_field_func_builtin(res,"fname", res_fname,"pg_result.getvalue_fname");
		xlo_add_field_func_builtin(res,"getvalue", res_getvalue,"pg_result.getvalue");
		xlo_add_field_func_builtin(res,"getvalue_integer", res_getvalue_integer,"pg_result.getvalue_integer");
		xlo_add_field_func_builtin(res,"getvalue_float", res_getvalue_float,"pg_result.getvalue_float");
		xlo_add_field_func_builtin(res,"close", res_close,"pg_result.close");
		xlo_add_field_int(res,"_row_number",0);
		xlo_add_field_int(res,"ntuples", PQntuples(r));
		xlo_add_field_int(res,"nfields", PQnfields(r));
		strcpy(conn->name,"pg_result");
		XLF_RETURN_OBJ(res);
	    }
	    return 0;
	default:
	    xlo_add_field_str(conn,"error",PQerrorMessage(c) ? PQerrorMessage(c) : "unknown PostgreSQL error");
	}
    }
    XLF_RETURN_NIL();
}

XLF_DECL(res_getvalue)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    PGresult* r = (PGresult*) xl_res_get(res,0);
    char* s = PQgetvalue(r,tuple,field);
    
    if(! r || !s || tuple < 0 || field < 0)
	XLF_RETURN_NIL();
	
    xlv_set_str(XLF_RET,PQgetvalue(r,tuple,field));

    xls_erase_blank(ret->v.s);
    
    XLF_RETURN_OK();
}

XLF_DECL(res_getvalue_integer)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    PGresult* r = (PGresult*) xl_res_get(res,0);
    char* s = PQgetvalue(r,tuple,field);
    
    if(! r || !s || tuple < 0 || field < 0)
	XLF_RETURN_NIL();
	
    XLF_RETURN_INT(xl_str2int(s,NULL));
}

XLF_DECL(res_getvalue_float)
{
    xl_object res = (xl_object)XLF_THIS;    
    int tuple     = XLF_ARG_INT(0,-1);
    int field	  = XLF_ARG_INT(1,-1);
    PGresult* r = (PGresult*) xl_res_get(res,0);
    char* s = PQgetvalue(r,tuple,field);
    
    if(! r || !s || tuple < 0 || field < 0)
	XLF_RETURN_NIL();
        
    XLF_RETURN_FLOAT(xl_str2float(s,NULL));
}

XLF_DECL(res_fname)
{
    xl_object res = (xl_object)XLF_THIS;
    PGresult* r = (PGresult*) xl_res_get(res,0);
    int index = XLF_ARG_INT(0,-1);
    if( !r || index < 0 || index > PQnfields(r) )
	XLF_RETURN_NIL();
    
    XLF_RETURN_STR(PQfname(r,index));
}
XLF_DECL(res_close)
{
    xl_object res = (xl_object)XLF_THIS;
    PGresult* r = (PGresult*) xl_res_get(res,0);
    
    dbgprintf(("POSTGRES: result.close();\n"));
    if( r != NULL )
	PQclear(r);
    xl_res_set(res,NULL,0);
    XLF_RETURN_INT(0);
}
