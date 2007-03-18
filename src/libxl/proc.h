/*
* Header:	proc.h
*
* Id:           $Id: proc.h 1.2 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	prototypes for functions which procesor library must serv
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __proc_h_
#define __proc_h_

#include "vm.h"

/* C Extern Identifiers. */
#ifdef __cplusplus
extern "C" {
#endif

int	xlp_step(xl_thread th,int howmany);

#ifdef __cplusplus
}
#endif

#endif /* force single include of file */
