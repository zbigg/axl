/*
* File: 	simple.c
*
* Id:		$Id: simple.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl extensions library
*
* Description:	simple module for AXL
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include <stdlib.h>
#include <stdio.h>
#include "axl/axl.h"


/*--------------------------------------------------
 * FORWARD DECLARATIONS
 *
 */

XLF_DECL(simple_function);
XLF_DECL(create_simple_object);

static void module_init(void);
static void module_deinit(void);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_DEF_INIT(module_init);
    ZMOD_DEF_DEINIT(module_deinit);

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY(simple_function)
	ZMOD_ENTRY(create_simple_object)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	simple,
	"axlsimple",
	1,1,
	"simple module for AXL"
    )
ZMOD_IMPLEMENT_MODULE_END


/*--------------------------------------------------
 * MODULE VARIABLES
 *
 */


/*--------------------------------------------------
 * MODULE INITIALIZATION and FINALIZATION
 *
 */
static void module_init(void) {
}
static void module_deinit(void) {
}

/*--------------------------------------------------
 * MODULE FUNCTIONS
 *
 */

XLF_DECL(simple_function)
{
    XLF_RETURN_STR("hello, ``simple'' is speaking !");
}

XLF_DECL(simple_method);
XLF_DECL(create_simple_object)
{
    xl_object simple_obj = xlo_new_map();
    xlo_add_field_str		(simple_obj,"name","simple");
    xlo_add_field_func_builtin	(simple_obj,"method", simple_method,".simple.method");
    XLF_RETURN_OBJ(simple_obj);
}

XLF_DECL(simple_method)
{
    xl_object self = (xl_object)XLF_THIS;
    
    XLF_RETURN_STR("i'm the simple.method()");
}
