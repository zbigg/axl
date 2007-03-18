/*
* File: 	@MOD_NAME@.c
*
* Project:	-
*
* Description:	-
*
* Author:	-
*
* Copyright:
*
*   This program is - ,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include "axl/axl.h"
/*
    To declare or define AXL function use XLF_DECL macro.
    It extends to function header that has storage class static,
    name specified in the only parameter and arguments that
    can be accesed by XLF_xxx macros.
*/
XLF_DECL(@MOD_NAME@_func);

ZMOD_IMPLEMENT_MODULE_BEGIN

    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
/*
    This is an table containing entries in module exports
    table. If some function is to be exported by ZMOD engine
    it should by listed here.
    
    If you want to export symbol `foo' by name just write:
	ZMOD_ENTRY(foo)
    If you want to export symbol 'bar' but you want it to be
    visible as 'rab' then write:
	ZMOD_ENTRY_A(rab,bar)
*/
	ZMOD_ENTRY(@MOD_NAME@_func)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	@MOD_NAME@,			/* module name (unquoted)	*/
	"axl@MOD_NAME@",		/* module fullname 		*/
	1,0,				/* module major,minor version	*/
	"@MOD_NAME@ module for AXL"	/* module description/copyright */
    )
ZMOD_IMPLEMENT_MODULE_END
/*
    This is your example AXL function.
    It returns string value to AXL program.
    
    See XLF_xxx macros documentation for information
    about writing your own AXL functions.
*/
XLF_DECL(@MOD_NAME@_func)
{
    XLF_RETURN_STR("@MOD_NAME@ says: Hello world");
}
