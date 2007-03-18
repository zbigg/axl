 /*
   * File:	chconv1.c
   * Id:	$Id: chconv1.c 1.2 04/01/19 00:47:58+01:00 nikt@nowy.dom $
   * Project:	axl
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

#include "zcompat/zsystype.h"

#include <errno.h>
#include <string.h>
#include <zcompat/zpio.h>
#include <stdlib.h>
#include "zcompat/zprintf.h"

int test_charset(const char* name, const char* text)
{
    zcconv_t*	to = zcconv_new(name,"utf-8");
    zcconv_t*	back = zcconv_new("utf-8",name);
    int result = 0;
    if( !to  || !back) {
	zfprintf(zstderr,"converter %s to utf-8 not found\n");
	return -1;
    }
    {
	size_t len = strlen(text);
	size_t to_size = zcconv_dest_size(to,text,len);
	size_t back_size;
	char* buf_to = malloc(to_size);
	char* buf_back;
	zcconv(to, text, buf_to, to_size);
	
	back_size = zcconv_dest_size(back,buf_to,to_size);
	buf_back = malloc(back_size);
	zcconv(back, buf_to, buf_back, back_size);
	
	result = strcmp(text, buf_back) == 0;
	zprintf("%s to utf-8 test: %s\n",name,result ? "SUCCESS" : "FAILED");
	zprintf("original : size=%3i text='%s'\n",len, text);
//	zprintf("encoded  : size=%3i text='%s'\n",to_size, buf_to);
	zprintf("decoded  : size=%3i text='%s'\n",back_size, buf_back);
	free(buf_to);
	free(buf_back);
    }
    
    if( to ) 
	zcconv_free(to);
    if( back ) 
	zcconv_free(back);
    return result;
}

int test_charsets(const char* from_name,const char* to_name, const char* text)
{
    zcconv_t*	to = zcconv_new(from_name,to_name);
    zcconv_t*	back = zcconv_new(to_name,from_name);
    int result = 0;
    if( !to  || !back) {
	zfprintf(zstderr,"converter %s to/from %s not found\n",from_name,to_name);
	return -1;
    }
    {
	size_t len = strlen(text);
	size_t to_size = zcconv_dest_size(to,text,len);
	size_t back_size;
	char* buf_to = malloc(to_size);
	char* buf_back;

	zcconv(to, text, buf_to, to_size);
	
	back_size = zcconv_dest_size(back,buf_to,to_size);
	buf_back = malloc(back_size);
	zcconv(back, buf_to, buf_back, back_size);
	
	result = strcmp(text, buf_back) == 0;
	zprintf("%s to %s test: %s\n",from_name,to_name,result ? "SUCCESS" : "FAILED");
	zprintf("original : size=%3i text='%s'\n",len, text);
//	zprintf("encoded  : size=%3i text='%s'\n",to_size, buf_to);
	zprintf("decoded  : size=%3i text='%s'\n",back_size, buf_back);
	free(buf_to);
	free(buf_back);
    }
    
    if( to ) 
	zcconv_free(to);
    if( back ) 
	zcconv_free(back);
    return result;
}

int main(argc, argv)
    int argc;
    char **argv;
{
    int result = 1;
    const char* x = "° ”£—∆Ø¨¶±ÍÛ≥±∂øºÒ";
    const char* w = "πÊÍ≥ÛÒúüø•∆ £”—åèØ";
    result = result && test_charset("iso-8859-2",x);
    result = result && test_charset("windows-1250",w);
    result = result && test_charsets("iso-8859-2","windows-1250",x);
    result = result && test_charsets("windows-1250","iso-8859-2",w);
    return result ? 0 : 1;
}
