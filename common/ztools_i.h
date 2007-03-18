/*
* Header:	ztools_i.h
*
* Project:	ztools
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*   This program is Copyright(c) Zbigniew Zagorski 1999-2001,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __ztools_i_h_
#define __ztools_i_h_

#include "zlinkdll.h"

DECL_DLL_ALLOC_VECT(ztools);

#define zt_malloc   ztools_malloc
#define zt_calloc   ztools_calloc
#define zt_realloc  ztools_realloc
#define zt_free	    ztools_free
#define zt_strdup   ztools_strdup

#endif /* force single include of file */

