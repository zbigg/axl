/*
* File: 	cgi.c
*
* Id:		$Id: cgi.c 1.2 03/11/27 21:10:20+01:00 zbigg@zbigg.(none) $
*
* Project:	axl library
*
* Description:	cgi module
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

#include "zcompat/zmod.h"
#include "zcompat/zpio.h"
#include <stdlib.h>
#include <stdio.h>
#include "axl/axl.h"

static int  cgi_get_option
		(xl_thread thread,int argc,xl_value argv,xl_value ret);
static int  cgi_set_output_conv
		(xl_thread thread,int argc,xl_value argv,xl_value ret);

ZMOD_IMPLEMENT_MODULE_BEGIN
    ZMOD_NO_INIT
    ZMOD_NO_DEINIT

    ZMOD_NO_LIB_IMP
    ZMOD_NO_EXPORTS
    ZMOD_ENTRIES_BEGIN
	ZMOD_ENTRY(cgi_get_option)
	ZMOD_ENTRY(cgi_set_output_conv)
    ZMOD_ENTRIES_END
    ZMOD_INFO(
	cgi,
	"axlCGI",
	1,1,
	"AXL CGI Module"
    )
ZMOD_IMPLEMENT_MODULE_END


#define _XL_THIS(thread) ((xl_object)xlx_find(thread->var,"this"))

#include "cgi.h"

static int  cgi_get_option(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    const char* name="";
    const char* value;
    if( argc >= 1 ) {
	name = xlv_str(argv);
    }
    value = cgi_getparam_ex((char*)name);
    if( value == NULL )
	value = "";
    xlv_set_str(ret,value);
    return 0;
}
static int  cgi_set_output_conv(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    int t = xlv_int(argv);
    zcharconv_table* ct = malloc(sizeof( zcharconv_table));
    ZSTREAM s;
    switch(t) {
    case 1:
	zcharconv_get(*ct,  ZCHC_ISO_8859_2,	ZCHC_PLAIN);
	break;
    case 2:
	zcharconv_get(*ct,  ZCHC_WIN_1250,	ZCHC_PLAIN);
	break;
    case 3:
	zcharconv_get(*ct,  ZCHC_WIN_1250,	ZCHC_ISO_8859_2);
	break;
    case 4:
	zcharconv_get(*ct,  ZCHC_ISO_8859_2,	ZCHC_WIN_1250);
	break;
    default:
	free(ct);
	xlv_set_int(ret,0);
	return 0;
    }
    s = zcharconv(zstdout,NULL,*ct);
    zf_push(zstdout,s);
    xlv_set_int(ret,0);
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *InputStr = NULL;
int cgi_readed = 0;

#define readed cgi_readed

int cgi_init(void)
{
    char* eqs,
	*ecl;
    int use_query = 0;
    int use_stdin = 0;
    int c_len = 0;
    int len = 0;
    int x;
    char *a,*b;
    if( readed ) {
	return 1;
    }
    ecl = getenv("CONTENT_LENGTH");
    eqs = getenv("QUERY_STRING");
    if( ecl ) {
	use_stdin = 1;
	c_len = atoi(ecl);
    }
    len = c_len;
    if( eqs ) if( strlen(eqs) > 0 ) {
	use_query = 1;
	len+=1 + strlen(eqs);
    }
    InputStr = (char*)malloc(len+2);
    memset(InputStr,len+2,0);
    if(!InputStr) {
	return 0;
    }
    InputStr[0] = 0;
    x = 0;
    if( use_stdin ) {
	x = fread(InputStr,1,c_len,stdin);
	InputStr[x] = 0;
	if( use_query) {
	    InputStr[x] = '&';
	    x++;
	}
    }

    if( use_query ) {
	strcpy(InputStr+x,eqs);
	x+= strlen(eqs);
    }

    a = InputStr;
    for(;;) {
	b = strchr(a,'&');
	if( b == 0) {
	    cgi_parsestring(a,a);
	    break;
	}
	*b++ = 0;

	cgi_parsestring(a,a);
	a = b;
    }
    readed = 1;
    atexit(cgi_done);
    return 1;
}


void cgi_done(void)
{
    if (!readed)
	return;
    if (InputStr != NULL)
	free(InputStr);
    InputStr = NULL;
}

int   cgi_getparamlen(char* name)
{
    char* x;
    if(!name )
	return -1;
    x = cgi_getparam_ex(name);
    if( !x )
	return -1;
    return strlen(x);
}

char* cgi_getparam_ex(char *name)
{

    char* c,*next,*x;
    int   nlen = strlen(name);
    if (!readed)
	if (!cgi_init())
	    return (char*)0;
    c = InputStr;
    while( *c ) {
	while( *c == (signed char) -1 )
	    c++;
	x = strchr(c,'=');
	next = strchr(c,'\0');
	if( x == NULL || next == NULL) {
	    break;
	}
	if( strncmp(name,c,nlen) == 0 ) {
	    return x+1;
	}
	c = ++next;
    }
    return 0;
}

int cgi_getparam(char *name, char *buf)
{
    char* x;
    x = cgi_getparam_ex(name);
    if( x ) {
	strcpy(buf,x);
	return 1;
    }
    return 0;
}

char *cgi_getinputstr()
{
    if (!readed)
	if (!cgi_init())
	    return 0;
    return InputStr;
}

int cgi_parsestring(char *str, char *output)
{
    int number;
    char num_chars[3];
    int in_num = 0;
    char *end_ptr;
    int x = strlen(str);
    int y = 0;
    while (*str && str && output) {
	if (*str == '+') {
	    str++;
	    y++;
	    *output = ' ';
	    output++;
	    in_num = 0;
	} else if (*str == '%') {
	    str++;
	    y++;
	    in_num = 1;
	} else {
	    switch (in_num) {
	    case 1:
		num_chars[0] = *str;
		num_chars[1] = 0;
		str++;
		in_num = 2;
		break;
	    case 2:
		num_chars[1] = *str;
		str++;
		num_chars[2] = 0;
		number = strtol(num_chars, &end_ptr, 16);
		if (*end_ptr == 0) {
		    (*output) = (unsigned char) number;
		    output++;
		}
		in_num = 0;
		break;
	    case 0:
		*output = *str;
		str++;
		y++;
		output++;
		break;
	    }
	}
    }
    *output++ = 0;
    while( y<x) {
	*output++ = '=';
	y++;
    }
    return 1;
}
