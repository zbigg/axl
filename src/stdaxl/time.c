/*
* File: 	stdlib/time.c
*
* Id:		$Id: time.c 1.4 04/05/22 20:11:42+02:00 zbigg@veinard.dom $
*
* Project:	axl standard library
*
* Description:	Standard Library : time routines
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
#include "axl/axl.h"

#include <sys/stat.h>
#include <time.h>

#ifdef ZSYS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "decl.h"

static void time_obj2tm(xl_object o,struct tm* tm)
{
    tm->tm_year = xlv_int( xlo_ffield (o, "year" ) ) - 1900;
    tm->tm_mon  = xlv_int( xlo_ffield (o, "month") ) - 1;
    tm->tm_mday = xlv_int( xlo_ffield (o, "day"  ) );
    tm->tm_wday = xlv_int( xlo_ffield (o, "day_of_week") );
    tm->tm_yday = xlv_int( xlo_ffield (o, "day_of_year") );
    tm->tm_hour = xlv_int( xlo_ffield (o, "hour"  ) );
    tm->tm_min  = xlv_int( xlo_ffield (o, "minute") );
    tm->tm_sec  = xlv_int( xlo_ffield (o, "second") );
}
XLF_DECL(xl_time_obj_stamp)
{
    time_t t;
    struct tm tm;
    xl_object This = (xl_object)XLF_THIS;

    time_obj2tm(This,&tm);

    t = mktime(&tm);

    xlv_set_int(ret,(xl_integer) t);
    return 0;
}

XLF_DECL(xl_time_obj_asctime)
{
    const char* s;
    struct tm tm;
    xl_object This = (xl_object)XLF_THIS;

    time_obj2tm(This,&tm);

    s = asctime(&tm);

    xlv_set_str(ret,s);
    return 0;
}

XLF_DECL(xl_time_obj_strftime)
{
    xl_object This = (xl_object)XLF_THIS;
    const char* fmt = argc > 0 ? xlv_str( argv ) : "";
    char buf[200];
    struct tm tm;

    time_obj2tm(This,&tm);
    strftime(buf,sizeof(buf)-1,fmt,&tm);
    buf[199] = 0;
    xlv_set_str(ret,buf);
    return 0;
}

static xl_object create_time_obj(
    int year, int month, int day,
    int dow, int doy,int hour, int minute,int second)
{
    xl_object x = xlo_new_map();
    xlo_add_field_int(x,"year",year);		    /* STD */
    xlo_add_field_int(x,"month",month);		    /* STD */
    xlo_add_field_int(x,"day",day);		    /* STD */
    xlo_add_field_int(x,"day_of_week",dow);	    /* STD */
    xlo_add_field_int(x,"day_of_year",doy);	    /* STD */
    xlo_add_field_int(x,"hour",hour);		    /* STD */
    xlo_add_field_int(x,"minute",minute);	    /* STD */
    xlo_add_field_int(x,"second",second);	    /* STD */

    xlo_add_field_func_builtin(x,"stamp",xl_time_obj_stamp,NULL);/* STD */
    xlo_add_field_func_builtin(x,"strftime",xl_time_obj_strftime,NULL);/*alias*/
    xlo_add_field_func_builtin(x,"fmt",xl_time_obj_strftime,NULL);/* STD */
    xlo_add_field_func_builtin(x,"asctime",xl_time_obj_asctime,NULL);/*STD*/

    return x;
}

static xl_object create_time_obj_tm(
struct tm* t)
{
    return create_time_obj(
	    t->tm_year + 1900,
	    t->tm_mon  + 1,
	    t->tm_mday,
	    t->tm_wday,
	    t->tm_yday,
	    t->tm_hour,
	    t->tm_min,
	    t->tm_sec);
}

/****************** basic routines */
/*
    sys.time.get
*/


XLF_DECL(xl_time_local)
{
    xl_object x;
    struct tm lt;
    time_t    t;
    if( argc > 0 )
	t = xlv_int(argv);
    else
	time(&t);

    memcpy(&lt,localtime(&t),sizeof(lt));
    x = create_time_obj_tm(&lt);
    xlv_set_ref(ret,(xl_value)x,NULL);
    xlo_free(x);
    return 0;
}


XLF_DECL(xl_time_gmt)
{
    xl_object x;
    struct tm lt;
    time_t    t;
    if( argc > 0 )
	t = xlv_int(argv);
    else
	time(&t);

    memcpy(&lt,gmtime(&t),sizeof(lt));
    x = create_time_obj_tm(&lt);
    xlv_set_ref(ret,(xl_value)x,NULL);
    return 0;
}

XLF_DECL(xl_time_get)
{
    xl_object x;
    struct tm lt;
    time_t    t;
    time(&t);
    memcpy(&lt,localtime(&t),sizeof(lt));
    x = create_time_obj_tm(&lt);
    xlv_set_ref(ret,(xl_value)x,NULL);
    xlo_free(x);
    return 0;
}
/*
    sys.time.set

    15.02.2002: dropped out of the standard
*/
XLF_DECL(xl_time_set)
{
    xlv_set_int(ret,-1);
    return 0;
}
/*
    sys.time.get_file_modify
*/
XLF_DECL(xl_time_get_file_modify)
{
    struct stat st;
    const char* fname = xlv_str(argv);
    if( stat(fname,&st) == -1 )
        xlv_set_int(ret,-1);
    else
        xlv_set_int(ret,st.st_mtime);
    return 0;
}
/*
    sys.time.get_file_access
*/
XLF_DECL(xl_time_get_file_access)
{
    struct stat   st;
    const char* fname = xlv_str(argv);
    if( stat(fname,&st) == -1 )
        xlv_set_int(ret,-1);
    else
        xlv_set_int(ret,st.st_atime);
    return 0;
}
/*
    sys.time.get_file_create
*/
XLF_DECL(xl_time_get_file_create)
{

    struct stat st;
    const char* fname = xlv_str(argv);
    if( stat(fname,&st) == -1 )
        xlv_set_int(ret,-1);
    else
        xlv_set_int(ret,st.st_ctime);
    return 0;
}
/*
    sys.time.time
*/
XLF_DECL(xl_time_time)
{
    time_t t;
    time(&t);
    xlv_set_int(ret,(xl_integer) t);
    return 0;
}

int  xl_create_time_object(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object xl_time_obj;
    xl_time_obj = xlo_new_map();
    {
	xlo_add_field_func_builtin(xl_time_obj,"local",	xl_time_local,NULL);
	xlo_add_field_func_builtin(xl_time_obj,"gmt",	xl_time_gmt,NULL);
	xlo_add_field_func_builtin(xl_time_obj,"get",	xl_time_get,NULL);
	xlo_add_field_func_builtin(xl_time_obj,"set",	xl_time_set,NULL);

	xlo_add_field_func_builtin(xl_time_obj,"get_file_modify",xl_time_get_file_modify,NULL);
	xlo_add_field_func_builtin(xl_time_obj,"get_file_create",xl_time_get_file_create,NULL);
	xlo_add_field_func_builtin(xl_time_obj,"get_file_access",xl_time_get_file_access,NULL);

	xlo_add_field_func_builtin(xl_time_obj,"file_modify_stamp",xl_time_get_file_modify,NULL);
	xlo_add_field_func_builtin(xl_time_obj,"file_create_stamp",xl_time_get_file_create,NULL);
	xlo_add_field_func_builtin(xl_time_obj,"file_access_stamp",xl_time_get_file_access,NULL);

	xlo_add_field_func_builtin(xl_time_obj,"time",	xl_time_time,NULL);
	xlo_add_field_func_builtin(xl_time_obj,"stamp",	xl_time_time,NULL);
    }
    xlv_set_ref(ret,(xl_value)xl_time_obj,NULL);
    strcpy(xl_time_obj->name,".axl.time");
    xlo_free(xl_time_obj);
    return 0;
}
