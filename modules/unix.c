/*
* File: 	unix.c
*
* Id:		$Id: unix.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl library
*
* Description:	unix module
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2001,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include <stdlib.h>
#include "axl/axl.h"

#ifdef ZSYS_UNIX
#include <unistd.h>
#endif
#if defined ZSYS_WIN || defined ZSYS_DOS
#include <process.h>
#include <io.h>
#endif

static int  xl_getenv(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    char* s = getenv(xlv_str(argv));
    if( s == NULL )
	s = "";
    xlv_set_str(ret,s);
    return 0;
}

static int  xl_setenv(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
#ifndef ZSYS_WIN
    setenv(xlv_str(argv),xlv_str(argv+1),1);
#endif
    xlv_set_int(ret,0);
    return 0;
}

static int  xl_geteuid(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xlv_set_int(ret,
#ifndef ZSYS_WIN32
	geteuid()
#else
	0
#endif
	);
    return 0;
}
static int  xl_getuid(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xlv_set_int(ret,
#ifndef ZSYS_WIN32
	getuid()
#else
	0
#endif
	);
    return 0;
}
static int  xl_getppid(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xlv_set_int(ret,
#ifndef ZSYS_WIN32
	getppid()
#else
	0
#endif
	);
    return 0;
}
static int  xl_getpid(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xlv_set_int(ret,getpid());
    return 0;
}
static int  xl_getgid(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xlv_set_int(ret,
#ifndef ZSYS_WIN32
	getgid()
#else
	0
#endif
	);
    return 0;
}
static int  xl_access(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    const char* name = xlv_str(argv);
    int  type = xlv_int(argv+1);
    if( name == NULL )
	name = "/";
    xlv_set_int(ret,access(name,type));
    return 0;
}
static int  xl_alarm(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
#ifdef ZSYS_WIN32
    xlv_set_int(ret,-1);
    return 0;
#else
    int sec = xlv_int(argv);
    xlv_set_int(ret,alarm(sec));
    return 0;
#endif
}
static int  xl_pause(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
#ifndef ZSYS_WIN32
    xlv_set_int(ret,pause());
#endif
    return 0;
}

static int  xl_system(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    const char* command = xlv_str(argv);
    xlv_set_int(ret,command != NULL ? system(command) : -1);
    return 0;
}

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY_A(getenv,	xl_getenv)
	ZMOD_ENTRY_A(setenv,	xl_setenv)
	ZMOD_ENTRY_A(getgid,	xl_getgid)
	ZMOD_ENTRY_A(getuid,	xl_getuid)
	ZMOD_ENTRY_A(geteuid,	xl_geteuid)
	ZMOD_ENTRY_A(getpid,	xl_getpid)
	ZMOD_ENTRY_A(getppid,	xl_getppid)
	ZMOD_ENTRY_A(access,	xl_access)
	ZMOD_ENTRY_A(alarm,	xl_alarm)
	ZMOD_ENTRY_A(pause,	xl_pause)
	ZMOD_ENTRY_A(system,	xl_system)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	unix,
	"axlUnix",
	1,0,
	"Unix/Posix module for AXL"
    )
ZMOD_IMPLEMENT_MODULE_END
