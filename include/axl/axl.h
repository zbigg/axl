/*
* File:		axl/axl.h
* Id:           $Id: axl.h 1.2 03/11/27 21:10:21+01:00 zbigg@zbigg.(none) $
* Project:	axl
*
* Description:	axl definitions
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*   This program is Copyright(c) Zbigniew Zagorski 2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __axl_h_
#define __axl_h_
#define INC_AXL

#ifndef INC_ZSYSTYPE
#include "zcompat/zsystype.h"
#endif

#if defined(ZDEV_GCC)
#define HAVE_INLINE
#endif

#ifndef INC_ZTOOLS
#include "zcompat/ztools.h"
#endif

#ifndef INC_ZPIO
#include "zcompat/zpio.h"
#endif

#ifndef INC_ZPIO
#include "zcompat/zprintf.h"
#endif

#ifndef INC_XLSTR
#include "axl/xlstr.h"
#endif

#ifndef INC_XLMAP
#include "axl/xlmap.h"
#endif

#ifndef INC_XLCORE
#include "axl/xlcore.h"
#endif

#endif /* End of: force single include of file */
