/*
* File:	axl_i.c
*
* Id:           $Id: axl_i.c 1.4 04/02/07 22:24:32+01:00 nikt@nowy.dom $
*
* Project:	axl
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*   This program is Copyright(c) Zbigniew Zagorski 2000-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include "zcompat/zlinkdll.h"
#include "zcompat/zpio.h"

#ifdef ZSYS_WIN
#include <windows.h>
#pragma hdrstop
#endif

#ifdef MDMF_DEBUG
#define malloc mdmf_malloc
#define calloc mdmf_calloc
#define realloc mdmf_realloc
#define free mdmf_free
#define strdup mdmf_strdup
#endif

DEF_DLL_ALLOC_VECT_FUNC(axl)

    SET_DLL_ALLOC_VECT_PR(axl,zpio);
    SET_DLL_ALLOC_VECT_PR(axl,ztools);

END_DLL_ALLOC_VECT_FUNC(axl)

ZSTREAM	axl_debug_stream = 0;
void	axl_debug_stream_open()
{
#ifdef ZSYS_WIN
    axl_debug_stream = zsopen("localhost",999);
#else
    axl_debug_stream = 0; /* zopen("/dev/tty11",ZO_WRITE);*/
#endif
    if( !axl_debug_stream ) {
	axl_debug_stream = zstderr;
    }
}

void	axl_debug_stream_close()
{
    if( axl_debug_stream && axl_debug_stream != zstderr ) {
	zclose(axl_debug_stream);
    }
    axl_debug_stream = zstderr;
}

#ifdef _DEBUG
#define XL_NAME "xld"
#else
#define XL_NAME "xl"
#endif
int	xl_get_library_path(char* buf, unsigned int size)
{
#ifdef ZSYS_WIN
    HMODULE x = GetModuleHandle(XL_NAME);
    if( x ) {
	GetModuleFileName(x,buf,size);
	return 0;
    }
    return -1;
#else
    return -1;
#endif
}
