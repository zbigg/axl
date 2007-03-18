/*
* File: 	stdlib/decl.h
*
* Id:		$Id: decl.h 1.3 04/05/22 20:11:42+02:00 zbigg@veinard.dom $
*
* Project:	axl library
*
* Description:	internal declarations
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2001,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __axl_stdlib_decl_h_
#define __axl_stdlib_decl_h_

int  xl_create_io_object(xl_thread thread,int argc,xl_value argv,xl_value ret);
int  xl_create_time_object(xl_thread thread,int argc,xl_value argv,xl_value ret);
int  xl_append_string_methods(xl_thread thread,int argc,xl_value argv,xl_value ret);

XLF_EDECL(dump_ctx);
XLF_EDECL(dump_object);
XLF_EDECL(dump_value);
XLF_EDECL(dump_target);

#endif /* __axl_stdlib_decl_h_ */
