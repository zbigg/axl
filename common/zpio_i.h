/*
* Header:	zpio_i.h
*
* Project:	zpio
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*   This program is Copyright(c) Zbigniew Zagorski 1999-2002,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/
/*
    This header contains:
    * definition of zpio_xxx alloc functions

    * configuration macro-definitions for
      system dependent functions



*/
#ifndef __zpio_i_h_
#define __zpio_i_h_

#ifdef HAVE_ZLINKDLL_H
#include "zlinkdll.h"

DECL_DLL_ALLOC_VECT(zpio);

#else

#define zpio_malloc	malloc
#define zpio_calloc	calloc
#define zpio_realloc	realloc
#define zpio_free	free
#define zpio_strdup	strdup

#endif

#ifndef HAVE_STRCASECMP
int strcasecmp(const char*, const char*);
#endif
/*
    SYSDEP functions

    Define functions that are
    implemented in system dependent
    modules
*/

#ifdef ZSYS_WIN	/* MS-Windows(tm) */

/* error.c -> win/werror.c */
#define SYSDEP_ZPERROR
#define SYSDEP_ZSPERROR
#define SYSDEP_ZSTRERROR

/* getcon.c -> win/zgetcon.c */
#define SYSDEP_ZPIO_GETCON
#define SYSDEP_ZPIO_FREECON

/* stdio.c ->win/zstdio.c */
#define SYSDEP_ZPIO_GET_STD_STREAM

/* zprintf -> win/zerror.c */
#define SYSDEP_ZVUPRINTF

/* pipe.c  -> win/wpipe.c */
#define SYSDEP_ZPOPEN

#endif /* END OF: MS-Windows(tm) */



/* pipe.c  -> unix/upipe.c */
#if defined(HAVE_FORK) && defined(HAVE_PIPE)
#define SYSDEP_ZPOPEN
#endif

#ifdef ZSYS_UNIX /* UNIX-Compatible */
#endif

#endif /* force single include of file */

