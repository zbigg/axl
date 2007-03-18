/*
* Header:	tools.h
*
* Id:           $Id: tools.h 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	some helper functions defintions for xltools.c
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __tools_h_
#define __tools_h_

#include "zcompat/zpio.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char* err_string_start;
extern char* err_string_end;
extern char* err_f_string_start;
extern char* err_f_string_end;

char*	strdup2(const char* s1,const char* s2);
char*	strdup3(const char* s1,const char* s2,const char* s3);
void	err_f(const char* format,...);
void	err(const char* format,...);

int	xl_file_path(const char* fname,char* buf,int max);
int	xl_file_name(const char* fname,char* buf,int max);

int	xl_find_file_by_path(
    const char* path,
    const char* file,
    int 	type,
    char*	buf,
    size_t	max);

int	xl_find_file_by_paths(
    char*	paths[],
    const char* file,
    int 	type,
    char*	buf,
    size_t	max);

#if defined(ZSYS_UNIX) && 1
#define ANSI_C_INVERT(a)	"\033[30;47m" a "\033[0m"
#define ANSI_C_RED(a)		"\033[0;31m"  a "\033[0m"
#define ANSI_C_GREEN(a) 	"\033[0;32m"  a "\033[0m"
#define ANSI_C_YELLOW(a)	"\033[0;33m"  a "\033[0m"
#define ANSI_C_BLUE(a)		"\033[0;34m"  a "\033[0m"
#define ANSI_C_MAGNETA(a)	"\033[0;35m"  a "\033[0m"
#define ANSI_C_CYAN(a)		"\033[0;36m"  a "\033[0m"
#else
#define ANSI_C_INVERT(a)	a
#define ANSI_C_RED(a)		a
#define ANSI_C_GREEN(a) 	a
#define ANSI_C_YELLOW(a)	a
#define ANSI_C_BLUE(a)		a
#define ANSI_C_MAGNETA(a)	a
#define ANSI_C_CYAN(a)		a
#endif

void xl_fctx_add(char* fmt,...);
void xl_fctx_dump(ZSTREAM f);
void xl_set_fctx_sig(void);
void xl_restore_fctx_sig(void);

#if 0
#define XL_FCTX(f,fmt,p)		xl_fctx_add(ANSI_C_BLUE("%s:%i")"-> %s(" fmt ")",__FILE__,__LINE__,(f),p)
#define XL_FCTX2(f,fmt,p1,p2)		xl_fctx_add(ANSI_C_BLUE("%s:%i")"-> %s(" fmt ")",__FILE__,__LINE__,(f),p1,p2)
#define XL_FCTX3(f,fmt,p1,p2,p3)	xl_fctx_add(ANSI_C_BLUE("%s:%i")"-> %s(" fmt ")",__FILE__,__LINE__,(f),p1,p2,p3)
#define XL_FCTX4(f,fmt,p1,p2,p3,p4)	xl_fctx_add(ANSI_C_BLUE("%s:%i")"-> %s(" fmt ")",__FILE__,__LINE__,(f),p1,p2,p3,p4)
#define XL_FCTX5(f,fmt,p1,p2,p3,p4,p5)	xl_fctx_add(ANSI_C_BLUE("%s:%i")"-> %s(" fmt ")",__FILE__,__LINE__,(f),p1,p2,p3,p4,p5)
#else
#define XL_FCTX(f,fmt,p)
#define XL_FCTX2(f,fmt,p1,p2)
#define XL_FCTX3(f,fmt,p1,p2,p3)
#define XL_FCTX4(f,fmt,p1,p2,p3,p4)
#define XL_FCTX5(f,fmt,p1,p2,p3,p4,p5)
#endif

#define XLF_PTR 	ANSI_C_RED("<0x%08x>")

#if 0
char*	_zstrdup(const char* s);
void*	_zmalloc(size_t sz);
void	_zfree(void* p);
extern	int malloc_count;


#define malloc _zmalloc
#define free _zfree
#define strdup _zstrdup
#endif

#ifdef __cplusplus
}
#endif
#endif
