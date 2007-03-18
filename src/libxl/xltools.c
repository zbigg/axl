/*
* File:		xltools.c
*
* Id:           $Id: xltools.c 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	some helper functions.
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

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "zcompat/zsystype.h"
#include "zcompat/zprintf.h"
#include "tools.h"

#include "axl_i.h"

#if defined ZSYS_UNIX || defined ZDEV_DJGPP
#include <unistd.h>
#include <signal.h>
#define HAVE_SIGNAL
#else
#include <io.h>
#endif

char* strdup2(const char* s1,const char* s2)
{
    char* s = 0;
    int len;
    int a = strlen(s1);
    len = a+strlen(s2)+1;
    s =(char*)axl_malloc(len);
    if( !s )
	return 0;
    strcpy(s,s1);
    strcpy(s+a,s2);
    return s;
}
char* strdup3(const char* s1,const char* s2,const char* s3)
{
    char* s = 0;
    int len = 1;
    int a,b;
    len += (a = strlen(s1));
    len += (b = strlen(s2));
    len += strlen(s3);
    s =(char*)axl_malloc(len);
    if( !s )
	return 0;
    strcpy(s,s1);
    strcpy(s+a,s2);
    strcpy(s+a+b,s3);
    return s;
}

#if 0
char* err_string_start	= ANSI_C_RED( "B³±d: ");
char* err_string_end	= "\n";
char* err_f_string_start= ANSI_C_RED( "B³±d krytyczny:");
char* err_f_string_end	= "\nZakoñczenie programu.\n";
#else
char* err_string_start	= ANSI_C_RED( "Error: " );
char* err_string_end	= "\n";
char* err_f_string_start= ANSI_C_RED("Fatal error:");
char* err_f_string_end	= "\nTerminating program.\n";
#endif

void err_f(const char* format,...)
{
    va_list vl;
    va_start(vl,format);
    fprintf(stderr,err_f_string_start);
    vfprintf(stderr,format,vl);
    fprintf(stderr,err_f_string_end);
    va_end(vl);
    exit(1);
}
void err(const char* format,...)
{
    va_list vl;
    va_start(vl,format);
    fprintf(stderr,err_string_start);
    vfprintf(stderr,format,vl);
    fprintf(stderr,err_string_end);
    va_end(vl);
    exit(1);
}

#ifdef HAVE_ZPIO
int	xl_find_file_by_path(
    const char* path,
    const char* file,
    int 	type,
    char*	buf,
    size_t	max)
{
    return zpio_find_file_by_path(path,file,type,buf,max);
}
#else
int	xl_find_file_by_path(
    const char* path,
    const char* file,
    int 	type,
    char*	buf,
    size_t	max)
{
    char	tmp[Z_PATH_MAX];
    const char*	c;
    if( path == NULL || file == NULL )
	return 0;
    if( zpio_path_is_abs(file) )
	return zpio_access(file,type);
    c = path;
    while( *c != '\0' ) {
	int len = strcspn(c,Z_PATH_SEPARATOR_T);
	if( len > 0 ) {
	    strncpy(tmp,c,len);
	    if( tmp[len-1] != Z_DIR_SEPARATOR ) {
		tmp[len] = Z_DIR_SEPARATOR;
		tmp[len+1] = 0;
	    } else
		tmp[len] = 0;
	} else {
	    tmp[0] = 0;
	}
	strncat(tmp,file,Z_PATH_MAX-1);
	tmp[Z_PATH_MAX-1] = 0;

	if( zpio_access(tmp,type) == 0 ) {
	    if( buf != NULL ) {
		strncpy(buf,tmp,max);
		buf[max-1] = 0;
	    }
	    return 0;
	}
	c += len;
	if( *c++ == '\0' )
	    return -1;
    }
    return -1;
}
#endif
int	xl_find_file_by_paths(
    char*	path[],
    const char* file,
    int 	type,
    char*	buf,
    size_t	max)
{
    do {
	if( xl_find_file_by_path(*(path++),file,type,buf,max) == 0 )
	    return 0;
    } while( *path != NULL );
    return -1;
}

int	xl_file_path(const char* fname,char* buf,int max)
{
    char* x;
    if( fname == NULL || buf == NULL )
	return -1;
    if( (x = strrchr(fname,Z_DIR_SEPARATOR)) != NULL ) {
	int len = x-fname+1;
	len = len > max ? max : len;
	strncpy(buf,fname,len);
	buf[len] = 0;
    } else {
	strcpy(buf,Z_DIR_CURRENT);
    }
    return 0;
}

int	xl_file_name(const char* fname,char* buf,int max)
{
    char* x;
    if( fname == NULL || buf == NULL )
	return -1;
    if( (x = strrchr(fname,Z_DIR_SEPARATOR)) != NULL ) {
	strncpy(buf,x+1,max);
	buf[max-1] = 0;
    } else {
	strncpy(buf,fname,max);

    }
    buf[max-1] = 0;
    return 0;
}

#define XL_CONTEXT_SIZE 	20

int	context_p = 0;
char	context_a[XL_CONTEXT_SIZE][200] = {"","","","",""};
void fctx_sig(int no);

#ifdef HAVE_SIGNAL
void*	xl_org_sigsegv = NULL;
void xl_restore_fctx_sig(void)
{
    signal(SIGSEGV,xl_org_sigsegv);
}

void xl_set_fctx_sig(void)
{
    xl_org_sigsegv = signal(SIGSEGV,fctx_sig);
}
void fctx_sig(int no)
{
    xl_fctx_dump(zstderr);
    signal(SIGSEGV,xl_org_sigsegv);
    raise(no);
}
#else /* HAVE_SIGNAL */
void xl_restore_fctx_sig(void)
{
}

void xl_set_fctx_sig(void)
{
}
#endif /* haven't got signals */

void xl_fctx_add(char* fmt,...)
{
    char* ptr;
    va_list va;
    va_start(va,fmt);
    ptr = context_a[(( context_p ) % XL_CONTEXT_SIZE )];
    zvsnprintf(ptr,sizeof(context_a[0]),fmt,va);


#ifdef XL_DBG_CONTEXT_STDERR__
    zfprintf(zstderr,"%s\n",ptr);
#endif
#ifdef XL_DBG_CONTEXT_STDOUT__
    zfprintf(zstdout,"%s\n",ptr);
#endif
    context_p++;
}

void xl_fctx_dump(ZSTREAM f)
{
    int a,i = 0;
    zfprintf(f,"Function context: %i\n",context_p);
    context_p = (context_p % XL_CONTEXT_SIZE );
    for(a = (context_p-1);i<XL_CONTEXT_SIZE;a--,i++) {
	 a = a % XL_CONTEXT_SIZE;
	 if( a < 0 )
	     a+= XL_CONTEXT_SIZE;
	 zfprintf(f,"%i: %s\n",i,context_a[a]);
    }
}

/***********************************************************/
#if 0
#undef strdup
#undef free
#undef malloc
char*	(_zstrdup)(const char* s)
{
    return malloc_count++, axl_strdup(s);
}

int malloc_count = 0;
void* (_zmalloc)(size_t sz)
{
  void *p;

  /* malloc (0) is unpredictable; avoid it.  */
  if (sz == 0)
    sz = 1;
  p = (void *) axl_malloc (sz);
/*  if( p != NULL ) */
      malloc_count++;
  /*  printf("allocC %04i : 0x%08x\n",sz,p); */
  return p;
}
void _zfree(void* p)
{
    /*	  printf("freeC     : 0x%08x\n",p); */
    malloc_count--;
    axl_free(p);
}

#endif
