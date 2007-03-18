/*
* File: 	wxaxl/module.c
*
* Id:		$Id: module.c 1.1 04/02/07 22:24:32+01:00 nikt@nowy.dom $
*
* Project:	wxAXL
*
* Description:	wxAXL - AXL to wxWindows binding 
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2004,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include "axl/axl.h"


XLF_EDECL( wxAxl_main );
XLF_EDECL( wxAxl_Factory );

ZMOD_IMPLEMENT_MODULE_BEGIN

    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
        ZMOD_ENTRY(wxAxl_main)
	ZMOD_ENTRY(wxAxl_Factory)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	stdaxl,
	"AXL Std Module",
	1,0,
	"Standard Library module for AXL"
    )
ZMOD_IMPLEMENT_MODULE_END

