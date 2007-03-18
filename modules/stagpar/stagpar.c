/*
* File: 	stagpar.c.c
*
* Id:		$Id: stagpar.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl library
*
* Description:	AXL Template module
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
#include "sstr.h"
#include "axl/axl.h"
#include <ctype.h>

XLF_DECL(create_stagpar_object);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY_A(exported_function_name,create_stagpar_object)
    ZMOD_ENTRIES_END

    ZMOD_INFO(
	stagpar,
	"AXL stagpar interface",
	1,0,
	"AXL Simple Tag Parser Module"
    )
ZMOD_IMPLEMENT_MODULE_END
extern int lt_lex();

int lt_wrap()
{
    return 1;
}
void ltc_begin_tag_def(const char *tag)
{
    printf("t: %s\n", tag);
}

void ltc_end_tag_def(const char *tag)
{
    printf("y: %s\n", tag);
}

void ltc_tag_parameter(const char *param_name, const char *param_value)
{
    printf("p: %s=%s\n", param_name, param_value);
}

void ltc_end_tag(const char *tag)
{
    printf("e: %s\n", tag);
}

void ltc_text(const char *text)
{
    printf("b: %s\n", text);
}

void ltc_white(const char *text)
{
    printf("b: %s\n", text);
}

XLF_DECL(create_stagpar_object)
{
    XLF_RETURN_INT(0);
}



