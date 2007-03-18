/*
* File: 	stdlib/io.c
*
* Id:		$Id: io.c 1.6 04/05/22 20:11:42+02:00 zbigg@veinard.dom $
*
* Project:	axl standard library
*
* Description:	Standard Library Input/Output routines
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2002,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "sstr.h"
#include "axl/axl.h"

#include "decl.h"

static int xl_type_ZSTREAM = -1;
/****************** basic routines */
XLF_DECL(xl_io_feof);
#if 0
XLF_DECL(xl_io_read);
XLF_DECL(xl_io_write);
#endif
XLF_DECL(xl_io_seek);
XLF_DECL(xl_io_close);

/****************** initializing STREAMS */
XLF_DECL(xl_io_creat);
XLF_DECL(xl_io_open);
XLF_DECL(xl_io_fopen);
XLF_DECL(xl_io_sopen);
XLF_DECL(xl_io_popen);
XLF_DECL(xl_io_mbopen);
XLF_DECL(xl_io_stropen);

/****************** CHAR input/output */
XLF_DECL(xl_io_getc);
XLF_DECL(xl_io_fgetc);
XLF_DECL(xl_io_putc);
XLF_DECL(xl_io_fputc);

/****************** LINE input/output */
XLF_DECL(xl_io_puts);
XLF_DECL(xl_io_fputs);
XLF_DECL(xl_io_gets);
XLF_DECL(xl_io_fgets);
XLF_DECL(xl_io_fsize);

XLF_DECL(xl_io_read_pipe);



/****************** basic routines */
/*
    std.io.read
*/

#if 0
XLF_DECL(xl_io_read)
{
/*
    register ZSTREAM s = (ZSTREAM) xlv_int(argv);
    register size_t  len = (size_t) xlv_int(argv);
*/
    xlv_set_int(ret,-1);
    return 0;
}
/*
    std.io.write
*/
XLF_DECL(xl_io_write)
{
    xlv_set_int(ret,0);
    return 0;
}
#endif

/*
    std.io.seek
*/
XLF_DECL(xl_io_seek)
{
    register ZSTREAM s	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    register zoff_t ofs =( argc > 1) ? (zoff_t) xlv_int(argv+1) : (zoff_t)0;
    register int whence =( argc > 2) ? xlv_int(argv+2) : ZSEEK_SET;
    if( !s ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    xlv_set_int(ret,(int)zseek(s,ofs,whence));
    return 0;
}

/*
    std.io.feof
*/
XLF_DECL(xl_io_feof)
{
    register ZSTREAM s	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    if( !s ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    xlv_set_int(ret,zeof(s));
    return 0;
}

/*
    std.io.close
*/
XLF_DECL(xl_io_close)
{
    register ZSTREAM s	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    if( !s ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    xlv_set_int(ret,zclose(s));
    return 0;
}

/****************** initializing STREAMS */
/*
    std.io.creat
*/
XLF_DECL(xl_io_creat)
{
    xlv_set_int(ret,0);
    return 0;
}
/*
    std.io.open
*/
XLF_DECL(xl_io_open)
{
    register const char* name	= argc >= 1 ? xlv_str(argv) : NULL;
    register int flags 		= argc >= 0 ? xlv_int(argv+1) : ZO_READ;
    ZSTREAM s = zopen(name,flags);
    if( s ) zsetbuf(s,1024,0);
    xlv_set_ptr(ret,s,xl_type_ZSTREAM);
    return 0;
}
/*
    std.io.fopen
*/
XLF_DECL(xl_io_fopen)
{
    register const char* name 	= argc >= 1 ? xlv_str(argv) : NULL;
    register int flags 		= argc >= 2 ? xlv_int(argv+1) : ZO_READ;
    ZSTREAM s = zfopen(name,flags);
    if( s ) zsetbuf(s,1024,0);
    xlv_set_ptr(ret,s,xl_type_ZSTREAM);
    return 0;
}
/*
    std.io.sopen
*/
XLF_DECL(xl_io_sopen)
{
    register const char* name = argc >= 1 ? xlv_str(argv) : NULL;
    register short port = 	argc >= 2 ? (short)xlv_int(argv+1) : 0;
    ZSTREAM s = zsopen(name,port);
    if( s ) zsetbuf(s,1024,0);
    xlv_set_ptr(ret,s,xl_type_ZSTREAM);
    return 0;
}
/*
    std.io.popen
*/
XLF_DECL(xl_io_popen)
{
    register const char* name = argc >= 1 ? xlv_str(argv) : NULL;
    register int flags = 	argc >= 2 ? xlv_int(argv+1) : ZO_READ;
    ZSTREAM s = zpopen(name,flags);
    if( s ) zsetbuf(s,1024,0);
    xlv_set_ptr(ret,s,xl_type_ZSTREAM);
    return 0;
}
/*
    std.io.mbopen
*/
XLF_DECL(xl_io_mbopen)
{
    xlv_set_int(ret,0);
    return 0;
}
/*
    std.io.stropen
*/
XLF_DECL(xl_io_stropen)
{
    xlv_set_int(ret,0);
    return 0;
}
/****************** CHAR input/output */
/*
    std.io.getc
*/
XLF_DECL(xl_io_getc)
{
    xlv_set_int(ret,0);
    return 0;
}
/*
    std.io.fgetc
*/
XLF_DECL(xl_io_fgetc)
{
    xlv_set_int(ret,0);
    return 0;
}
/*
    std.io.putc
*/
XLF_DECL(xl_io_putc)
{
    register int c  =  xlv_int(argv);
    xlv_set_int(ret,zputc(c));
    return 0;
}
/*
    std.io.fputc
*/
XLF_DECL(xl_io_fputc)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    register int c  =  xlv_int(argv+1);
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    xlv_set_int(ret,zfputc(f,c));
    return 0;
}

/****************** LINE input/output */
/*
    std.io.puts
*/
XLF_DECL(xl_io_puts)
{
    register const char* s  =  xlv_str(argv);
    xlv_set_int(ret,zputs(s));
    return 0;
}
/*
    std.io.fputs
*/
XLF_DECL(xl_io_fputs)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    register const char* s  =  xlv_str(argv+1);
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    xlv_set_int(ret,zfputs(f,s));
    return 0;
}

static int do_fgets(ZSTREAM s, xl_str* str)
{
    char buf[200];
    while( 1 ) {
	int r;
	buf[0] = 0;
	r = zfgets(s, buf , sizeof(buf)-1 );
	if( r < 0 ) {
	    xls_free(*str);
	    *str = NULL;
	    return -1;
	}
	if( r == 0 ) {
	    return -1;
	}
	*str = xls_add(*str,buf);
	if( buf[r-1] == 0 || r < sizeof(buf)-1 )
	    return 0;
    }
}
/*
    std.io.gets
*/
XLF_DECL(xl_io_gets)
{
    xl_str s = NULL;
    if( do_fgets(zstdin, &s ) < 0 || !s ) {
	XLF_RETURN_INT(-1);
    } else {
	xlv_done(ret);
	ret->v.s = s;
	ret->type  = XLV_STR;
	ret->flags = 0;
	return 0;
    }
}
/*
    std.io.fgets
*/
XLF_DECL(xl_io_fgets)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    xl_str s = NULL;
    if( !f || do_fgets(f, &s ) < 0 || !s ) {
	XLF_RETURN_INT(-1);
    } else {
	xlv_done(ret);
	ret->v.s = s;
	ret->type  = XLV_STR;
	ret->flags = 0;
	return 0;
    }
}

/*
    std.io.open
*/
XLF_DECL(xl_io_fsize)
{
    register const char* name = xlv_str(argv);
    struct stat st;
    if( stat(name,&st) == -1 )
         xlv_set_int(ret,0);
    else
         xlv_set_int(ret,st.st_size);
    return 0;
}

XLF_DECL(xl_io_to_file)
{
    register const char* name = xlv_str(argv);
    register const char* str = xlv_str(argv+1);
    int size = strlen(str);
    ZSTREAM f = zfopen(name,ZO_CREAT | ZO_TRUNC | ZO_WRITE);
    if( !f ) {
        xlv_set_int(ret,-1);
	return 0;
    }
    zwrite(f,str,size);
    xlv_set_int(ret,zclose(f));
    return 0;
}


XLF_DECL(xl_io_read_pipe)
{
    const char* command = XLF_ARG_STR(0,NULL);
    ZSTREAM f;
    if( command == NULL )
        XLF_RET_STR("");
    xlv_done(XLF_RET);
    f = zpopen(command,ZO_READ);
    if( !f )
        XLF_RETURN_NIL();
    
    { /* do a read ! */
        xl_str s = NULL;
        int r,readed = 0;
        char buf[1024];
        while ( (r = zread(f,buf,sizeof(buf)-1) ) > 0 ) {
            buf[r] = '\0';
            s = xls_add(s,buf);
            readed += r;
        }
        if( readed <= 0 )
            XLF_RETURN_NIL();
        
        XLF_RET->type = XLV_STR;
        XLF_RET->v.s = s;
    }
    zclose(f);
    return 0;
}

XLF_DECL(xl_io_from_file)
{
    register const char* name = xlv_str(argv);
    struct stat st;
    int size = 0,isize;
    if( stat(name,&st) == -1 ) {
        xlv_set_int(ret,-4);
	return 0;
    }
    size = st.st_size;
    xlv_done(ret);

    ret->type = XLV_STR;
    ret->v.s = xls_alloc(size);
    if( !ret->v.s ) {
	return -1;
    }
    ret->v.s->length = 0;
    ret->v.s->capacity = size;
    ret->v.s->ref_cnt = 1;
    xls_str(ret->v.s)[0] = '\0';

    {
	ZSTREAM f = zfopen(name,ZO_READ);
	if( !f ) {
    	    xlv_set_int(ret,-2);
	    return 0;
	}
	isize = zread(f,xls_str(ret->v.s), size );
	if( isize < 0 ) {
	    xlv_set_int(ret,-3);
	    zclose(f);
	    return 0;
	}
	xls_get(ret->v.s)[isize] = '\0';
	ret->v.s->length = isize;
	zclose(f);
    }
    return 0;
}

XLF_DECL(xl_io_bwritel)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    unsigned long l = xlv_int(argv+1);
    int r;
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    r = zbwrite_l(f,&l);
    xlv_set_int(ret,r);
    return 0;
}

XLF_DECL(xl_io_bwriteh)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    unsigned short l = (unsigned short)xlv_int(argv+1);
    int r;
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    r = zbwrite_h(f,&l);
    xlv_set_int(ret,r);
    return 0;
}

XLF_DECL(xl_io_bwritec)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    unsigned char l = (unsigned char)xlv_int(argv+1);
    int r;
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    r = zbwrite_c(f,&l);
    xlv_set_int(ret,r);
    return 0;
}

XLF_DECL(xl_io_bwrites)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    const char* s = xlv_str(argv+1);
    int r;
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    r = zbwrite_s(f,&s);
    xlv_set_int(ret,r);
    return 0;
}

XLF_DECL(xl_io_breadl)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    unsigned long l;
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    zbread_l(f,&l);
    xlv_set_int(ret,l);
    return 0;
}

XLF_DECL(xl_io_breadh)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    unsigned short l;
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    zbread_h(f,&l);
    xlv_set_int(ret,l);
    return 0;
}

XLF_DECL(xl_io_breadc)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    unsigned char l;
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    zbread_c(f,&l);
    xlv_set_int(ret,l);
    return 0;
}

XLF_DECL(xl_io_breads)
{
    register ZSTREAM f	= (ZSTREAM) xlv_ptr(argv,xl_type_ZSTREAM);
    char* s;
    if( !f ) {
	xlv_set_int(ret,-1);
	return 0;
    }
    zbread_s(f,&s);
    xlv_set_str(ret,s);
    axl_free(s);
    return 0;
}

int  xl_create_io_object(xl_thread thread,int argc,xl_value argv,xl_value ret)
{
    xl_object obj;
    obj = xlo_new_map();
    xlo_add_field_func_builtin(obj,"open",	xl_io_open,"axl.io.open");
    xlo_add_field_func_builtin(obj,"close",	xl_io_close,NULL);
    xlo_add_field_func_builtin(obj,"seek",	xl_io_seek,NULL);
    xlo_add_field_func_builtin(obj,"mbopen",	xl_io_mbopen,NULL);
    xlo_add_field_func_builtin(obj,"feof",	xl_io_feof,NULL);
    xlo_add_field_func_builtin(obj,"creat",	xl_io_creat,NULL);
    xlo_add_field_func_builtin(obj,"fopen",	xl_io_fopen,NULL);
    xlo_add_field_func_builtin(obj,"sopen",	xl_io_sopen,NULL);
    xlo_add_field_func_builtin(obj,"popen",	xl_io_popen,NULL);
    xlo_add_field_func_builtin(obj,"stropen",   xl_io_stropen,NULL);
    xlo_add_field_func_builtin(obj,"getc",	xl_io_getc,NULL);
    xlo_add_field_func_builtin(obj,"fgetc",	xl_io_fgetc,NULL);
    xlo_add_field_func_builtin(obj,"putc",	xl_io_putc,NULL);
    xlo_add_field_func_builtin(obj,"fputc",	xl_io_fputc,NULL);
    xlo_add_field_func_builtin(obj,"gets",	xl_io_gets,NULL);
    xlo_add_field_func_builtin(obj,"fgets",	xl_io_fgets,NULL);
    xlo_add_field_func_builtin(obj,"puts",	xl_io_puts,NULL);
    xlo_add_field_func_builtin(obj,"fputs",	xl_io_fputs,NULL);
    xlo_add_field_func_builtin(obj,"fsize",	xl_io_fsize,NULL);

    xlo_add_field_func_builtin(obj,"bwritel",	xl_io_bwritel,NULL);
    xlo_add_field_func_builtin(obj,"bwriteh",	xl_io_bwriteh,NULL);
    xlo_add_field_func_builtin(obj,"bwritec",	xl_io_bwritec,NULL);
    xlo_add_field_func_builtin(obj,"bwrites",	xl_io_bwrites,NULL);

    xlo_add_field_func_builtin(obj,"breadl",	xl_io_breadl,NULL);
    xlo_add_field_func_builtin(obj,"breadh",	xl_io_breadh,NULL);
    xlo_add_field_func_builtin(obj,"breadc",	xl_io_breadc,NULL);
    xlo_add_field_func_builtin(obj,"breads",	xl_io_breads,NULL);

    xlo_add_field_func_builtin(obj,"read_pipe",	xl_io_read_pipe,NULL);
    xlo_add_field_func_builtin(obj,"read_file",	xl_io_from_file,NULL);
    xlo_add_field_func_builtin(obj,"write_file", xl_io_to_file,NULL);


    xlv_set_ref(ret,(xl_value)obj,NULL);

    if( xl_type_ZSTREAM < 0 )
	xl_type_ZSTREAM = xlv_ptr_register_type("axliostream");

    xlv_set_ptr(xlo_ffield(obj,"stdin"),zstdin,xl_type_ZSTREAM);
    xlv_set_ptr(xlo_ffield(obj,"stdout"),zstdout,xl_type_ZSTREAM);
    xlv_set_ptr(xlo_ffield(obj,"stderr"),zstderr,xl_type_ZSTREAM);

    strcpy(obj->name,".axl.io");
    xlo_dec_ref(obj);
    return 0;
}
