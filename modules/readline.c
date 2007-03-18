/*
* File: 	modules/readline.c
*
* Id:		$Id: readline.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl library
*
* Description:	AXL readline module
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2002,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include "sstr.h"
#include "axl/axl.h"
#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_READLINE_READLINE_H
#include <readline/readline.h>
#elif defined HAVE_READLINE_H
#include <readline.h>
#else
/* what we need from libreadline ? */
extern char*	readline(const char*);
#endif
#ifdef HAVE_READLINE_HISTORY_H
#include <readline/history.h>
#elif defined HAVE_HISTORY_H
#else
/* what we need from libhistory ? */

#endif

XLF_DECL(xl_readline);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY_A(readline,xl_readline)
    ZMOD_ENTRIES_END

    ZMOD_INFO(
	readline,
	"axlReadline",
	1,0,
	"AXL readline interface by Zbigniew Zagorski 2002"
    )
ZMOD_IMPLEMENT_MODULE_END

XLF_DECL(xl_readline)
{
    const char *p = XLF_ARG_STR(0,NULL);
    char* s;
    s = readline((char*)p);
    xlv_set_str(XLF_RET,(s));
    free(s);
    XLF_RETURN_OK();
}



