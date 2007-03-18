/*
* File: 	axl_i.h
*
* Id:           $Id: axl_i.h 1.4 04/03/07 21:24:27+01:00 nikt@nowy.dom $
*
* Project:	axl
*
* Decription:	Library INTERNAL header. Don't use in programs.
*
* Author:	Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __axl_i_h_
#define __axl_i_h_

#ifndef INC_ZLINKDLL
#include "zcompat/zlinkdll.h"
#endif
#ifndef INC_ZPIO
#include "zcompat/zpio.h"
#endif

DECL_DLL_ALLOC_VECT(axl);

/* xldebug.c */
void	axl_debug_stream_open();
void	axl_debug_stream_close();
extern ZSTREAM	axl_debug_stream;

/* xl.c */
extern ZSTREAM xlst_parse_out;
extern ZSTREAM xlst_run_out;
extern ZSTREAM xlst_run_in;
extern ZSTREAM xlst_run_err;

#endif /* __axl_i_h_ */
