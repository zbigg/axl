/*
* File: 	bench.c
*
* Id:		$Id: bench.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl library
*
* Description:	benchamrk module
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2001,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include <stdio.h>
#include "testtime.h"
#include "axl/axl.h"

static int  xl_bench_create_bench(xl_thread thread,int argc,xl_value argv,xl_value ret);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY_A(create_bench,xl_bench_create_bench)
	ZMOD_ENTRY_A(timer,xl_bench_create_bench)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	bench,
	"axlBench",
	1,0,
	"BenchMark module for axl\n"
	"provides constructor: create_bench"
    )
ZMOD_IMPLEMENT_MODULE_END

static int  xl_bench_pause	 (xl_thread thread,int argc,xl_value argv,xl_value ret);
static int  xl_bench_resume	 (xl_thread thread,int argc,xl_value argv,xl_value ret);
static int  xl_bench_done	 (xl_thread thread,int argc,xl_value argv,xl_value ret);
static int  xl_bench_start_time  (xl_thread thread,int argc,xl_value argv,xl_value ret);
static int  xl_bench_total_time  (xl_thread thread,int argc,xl_value argv,xl_value ret);
static int  xl_bench_finish_time (xl_thread thread,int argc,xl_value argv,xl_value ret);

static xl_object b_object = NULL;

static xl_object create_object(void)
{
    if( b_object != NULL )
	return xlo_new(b_object);

    b_object = xlo_new(NULL);

    xlo_add_field_func_builtin(b_object,"pause", xl_bench_pause,NULL);
    xlo_add_field_func_builtin(b_object,"resume", xl_bench_resume,NULL);
    xlo_add_field_func_builtin(b_object,"done", xl_bench_done,NULL);
    xlo_add_field_func_builtin(b_object,"start_time", xl_bench_start_time,NULL);
    xlo_add_field_func_builtin(b_object,"total_time", xl_bench_total_time,NULL);
    xlo_add_field_func_builtin(b_object,"finish_time", xl_bench_finish_time,NULL);

    return b_object;
}
static int  xl_bench_create_bench(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object o = create_object();
    xlv_inc_ref((xl_value)o);
    xlo_add_field_int(o,"id", new_counter());
    xlv_set_ref(ret,(xl_value)o,NULL);
    return 0;
}

#define _XL_THIS(thread) ((xl_object) xlv_target(xlx_find(thread->var,"this",NULL) ))

static int  xl_bench_pause(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object self = _XL_THIS(thread);
    int     cnt = xlv_int( xlo_field(self,"id"));

    pause_counter(cnt);
    xlv_clear(ret);
    return 0;
}
static int  xl_bench_resume(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object self = _XL_THIS(thread);
    int     cnt = xlv_int( xlo_field(self,"id"));

    resume_counter(cnt);
    xlv_clear(ret);
    return 0;
}
static int  xl_bench_done(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object self = _XL_THIS(thread);
    int     cnt = xlv_int( xlo_field(self,"id"));

    end_counter(cnt);
    xlv_free((xl_value)self);
    xlv_clear(ret);
    return 0;
}
static int  xl_bench_start_time(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object self = _XL_THIS(thread);
    int     cnt = xlv_int( xlo_field(self,"id"));

    xlv_set_float(ret,(xl_float)get_counter_start(cnt));
    return 0;
}
static int  xl_bench_total_time(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object self = _XL_THIS(thread);
    int     cnt = xlv_int( xlo_field(self,"id"));
    xlv_set_float(ret,(xl_float)get_counter_total(cnt));
    return 0;
}
static int  xl_bench_finish_time(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object self = _XL_THIS(thread);
    int     cnt = xlv_int( xlo_field(self,"id") );
    xlv_set_float(ret,(xl_float)get_counter_end(cnt));
    return 0;
}

#include "testtime.c"
