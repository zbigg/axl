/*
* File: 	stdlib/string.c
*
* Id:		$Id: string.c 1.4 04/05/22 20:11:42+02:00 zbigg@veinard.dom $
*
* Project:	axl standard library
*
* Description:	Standard Library : string routines
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2001-2002,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "axl/axl.h"

#ifdef ZSYS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "decl.h"

XLF_DECL(xl_strlen)
{
    int len;
    xl_value v = argv;
    switch( v->type ) {
    case XLV_STR:
	len = xls_len( v->v.s);
	break;
    case XLV_CHAR:
	len = v->v.ch ? 1 : 0;
	break;
    default:
	len = strlen ( xlv_str( v ) );
    }
    xlv_set_int(ret,len );
    return 0;
}

XLF_DECL(xl_strcmp)
{
    if( argc > 1 )
	xlv_set_int(ret,strcmp( xlv_str(argv), xlv_str(argv+1)) );
    else
	xlv_set_int(ret,0);
    return 0;
}

XLF_DECL(xl_strtrunc)
{
    const char*	s = argc > 0 ? xlv_str(argv) : "";
    int         len = argc > 1 ? xlv_int(argv+1) : 0;
    int max_len = s ? strlen(s) : 0;
    len = zmin( len , max_len);

    xlv_set_strn(ret,s,len);

    return 0;
}

XLF_DECL(xl_strcut)
{
    const char*	s     = argc > 0 ? xlv_str(argv+0) : "";
    int         begin = argc > 1 ? xlv_int(argv+1) : 0;
    int         len   = argc > 2 ? xlv_int(argv+2) : 0;
    int 	s_len = s ? strlen(s) : 0;
    begin = zmax(0,zmin( begin, s_len ));
    if( begin+len >= s_len )
	len = s_len-begin;

    xlv_set_strn(ret,s+begin,len);

    return 0;
}

XLF_DECL(xl_strspn)
{
    const char*	s1 = argc > 0 ? xlv_str(argv+0) : "";
    const char* s2 = argc > 1 ? xlv_str(argv+1) : "";

    xlv_set_int(ret, strspn(s1,s2));

    return 0;
}

XLF_DECL(xl_strcspn)
{
    const char*	s1 = argc > 0 ? xlv_str(argv+0) : "";
    const char* s2 = argc > 1 ? xlv_str(argv+1) : "";
    xlv_set_int(ret, strcspn(s1,s2));
    return 0;
}

XLF_DECL(xl_strchr)
{
    const char*	s1 = argc > 0 ? xlv_str(argv+0) : "";
    int c = argc > 1 ? xlv_char(argv+1) : '\0';
    char* res = strchr(s1,c);
    xlv_set_int(ret, res ? res-s1 : -1);
    return 0;
}


XLF_DECL(xl_strrchr)
{
    const char*	s1 = argc > 0 ? xlv_str(argv+0) : "";
    int c = argc > 1 ? xlv_char(argv+1) : '\0';
    char* res = strrchr(s1,c);
    xlv_set_int(ret, res ? res-s1 : -1);
    return 0;
}

XLF_DECL(xl_strstr)
{
    const char*	s1 = argc > 0 ? xlv_str(argv+0) : "";
    const char*	s2 = argc > 1 ? xlv_str(argv+1) : "";
    const char* res;
    if( !s2 || !s2[0] )
	res = s1;
    else
	res = strstr(s1,s2);
    xlv_set_int(ret, res ? res-s1 : -1);
    return 0;
}

/*
    CTYPE
*/

#define CTYPE_FUNC( name ) 				\
XLF_DECL(xl_##name)					\
{							\
    int c = argc > 0 ? xlv_char(argv+1) : '\0';		\
    xlv_set_int(ret, name( c ) );			\
    return 0;						\
}

#define CTYPE_FUNC_C( name ) 				\
XLF_DECL(xl_##name)					\
{							\
    int c = argc > 0 ? xlv_char(argv+1) : '\0';		\
    xlv_set_char(ret, name( c ) );			\
    return 0;						\
}

CTYPE_FUNC( isalpha );
CTYPE_FUNC( isalnum );
CTYPE_FUNC( isascii );
CTYPE_FUNC( iscntrl );
CTYPE_FUNC( isdigit );
CTYPE_FUNC( isgraph );
CTYPE_FUNC( islower );
CTYPE_FUNC( isprint );
CTYPE_FUNC( ispunct );
CTYPE_FUNC( isspace );
CTYPE_FUNC( isupper );
CTYPE_FUNC( isxdigit );
CTYPE_FUNC_C( tolower );
CTYPE_FUNC_C( toupper );


#define ADD_BFUNC(obj, name )				\
    xlo_add_field_func_builtin(obj, #name, xl_##name,NULL)
int  xl_append_string_methods(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object obj = argv > 0 ? (xl_object) xlv_deref( argv + 0 ) : (xl_object)NULL;
    if( !obj )
	XLF_RETURN_INT(0);

    ADD_BFUNC(obj,strcut);
    ADD_BFUNC(obj,strtrunc);

    ADD_BFUNC(obj,strlen);
    ADD_BFUNC(obj,strcmp);
    ADD_BFUNC(obj,strspn);
    ADD_BFUNC(obj,strcspn);

    ADD_BFUNC(obj,strchr);
    ADD_BFUNC(obj,strrchr);

    ADD_BFUNC(obj,strstr);

    ADD_BFUNC(obj, isalpha );
    ADD_BFUNC(obj, isalnum );
    ADD_BFUNC(obj, isascii );
    ADD_BFUNC(obj, iscntrl );
    ADD_BFUNC(obj, isdigit );
    ADD_BFUNC(obj, isgraph );
    ADD_BFUNC(obj, islower );
    ADD_BFUNC(obj, isprint );
    ADD_BFUNC(obj, ispunct );
    ADD_BFUNC(obj, isspace );
    ADD_BFUNC(obj, isupper );
    ADD_BFUNC(obj, isxdigit );

    ADD_BFUNC(obj, tolower );
    ADD_BFUNC(obj, toupper );

    XLF_RETURN_INT(1);
}
