/*
* File: 	stdlib/mod.c
*
* Id:		$Id: mod.c 1.3 04/05/22 20:11:42+02:00 zbigg@veinard.dom $
*
* Project:	axl library
*
* Description:	AXL Standard Library - main module file
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2001,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include "axl/axl.h"

#include "decl.h"

ZMOD_IMPLEMENT_MODULE_BEGIN

    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY_A(create_io_object,xl_create_io_object)
	ZMOD_ENTRY_A(create_time_object,xl_create_time_object)
	ZMOD_ENTRY_A(append_string_methods,xl_append_string_methods)
	ZMOD_ENTRY(dump_object)
	ZMOD_ENTRY(dump_target)
	ZMOD_ENTRY(dump_value)
	ZMOD_ENTRY(dump_ctx)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	stdaxl,
	"AXL Std Module",
	1,0,
	"Standard Library module for AXL"
    )
ZMOD_IMPLEMENT_MODULE_END

