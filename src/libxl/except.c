/*
* File: except.c
*
* Id:           $Id: except.c 1.3 03/11/27 21:36:25+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description: Exceptions.
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>

#include "axl/axl.h"

#include "axl_i.h"

static char xle_msg[][30] = {
    "unknown exception",
    "memory exception",
    "io exception",
    "user exception"
};

#define writes(desc,text) write((desc),(text),strlen((text)))

void		xle_throw(xle_type type,const char* msg)
{
/*
    int write(int,const void*,unsigned int);

    writes(2,xle_msg[(int)type]);
    writes(2," : ");
    writes(2,msg);
    writes(2,"\n");
*/
    zuprintf(ZUMODE_ERR,"axl unhandled exception: %s : %s\n",xle_msg[(int)type],msg);

    exit(1);
}



