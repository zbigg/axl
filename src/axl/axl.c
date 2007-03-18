 /*
   * File:	axl.c
   *
   * Id:	$Id: axl.c 1.11 04/05/22 20:13:21+02:00 zbigg@veinard.dom $
   *
   * Project:	axl
   *
   * Author Zbigniew Zagorski <zzbigg@o2.pl>
   *
   * Copyright:
   *
   *   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
   *   All rights reserved, and is distributed as free software under the
   *   license in the file "LICENSE", which is included in the distribution.

##
##  =Command axl
##     execute, compile or assembly AXL program
##
##  =Synopsis
##   | axl [ OPTIONS ] [ file ] [PARAMETERS ...]
##
##  =Description
##
##	Default behavior is to execute <scriptfile> (or bytecode) in file
##	filename with list of PARAMETERS.
##
##  =Options
##	<axl> accepts following options
##	 *   -c
##	 *   --compile     - compile to bytecode only don't execute
##	 *   -C
##	 *   --show-code - show colored program assembler on stdout code and
##                           exit
##	 *   -S
##	 *   --output-asm   - output assembler code of program
##	 *   -b
##       *   --bytecode     - filename is bytecode not script
##	 *   -o name
##	 *   --output name  - output to specified file
##	 *   -I dir
##	 *   --inc-dir dir  - search for includes in dir
##	 *   -L dir
##	 *   --lib-dir dir  - search for modules in dir
##	 *  -D name=value   - add variable name with specified value
##	 * -T[no-]name
##       * --trace-[no-]name - enable/disable trace for runtime
##			      trece is enabled only when axl were compiled 
##                            with --enable-debug
##	 *   -e 'script'    - execute 'script' from command line
##	                      instead of filename
##	 *   -ch            - show CGI default header (text/html)
##	 *   -d
##	 *   --dump-cfg     - dump configuration
##	 *   --help	    - show this screen on stdout and exit
##       *   --version	    - show program version
##
##	The <I>, <L>, and <D> options accepts also "postfix" method
##	of parameter passing, so you can pass -Isomedir instead of -I somedir.
##	Just like in gcc.
##
##      Rest of arguments are passed to AXL program.
##
##	Note that search directories set by -I and -L directives are pushed
##	onto the stack which is searched from top to down. So, the first
##	directory added will be searched last, and last added will be searched
##	first.
##	

 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dconfig.h"

#include "zcompat/zsystype.h"

#if defined ZSYS_WIN32
#include <io.h>
#else
#ifdef ZSYS_UNIX
#include <unistd.h>
#endif
#endif
#include <errno.h>
#include <zcompat/zpio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "axl/axl.h"
#include "zcompat/zprintf.h"

#include "zcompat/zlinkdll.h"
#include "sgetopt.h"

#ifdef MDMF_DEBUG



#define malloc mdmf_malloc
#define calloc mdmf_calloc
#define realloc mdmf_realloc
#define free mdmf_free
#define strdup mdmf_strdup
#endif

char *pname = "axl";

int	opt_bytecode = 0;
int	opt_compile = 0;
int	opt_show_code = 0;
int	opt_output_asm = 0;
char*	opt_output = 0;
char*	opt_exec = 0;
char*	opt_axl_path = PACKAGE_BINDIR"/axl";
char*	opt_script = 0;
int	opt_cgi_header = 0;
int	opt_verbose_level = 0;

xl_context global_ctx = NULL;

void sgo_usage(const char* msg);
void usage();
int opt_include_dir(const char* s);
int opt_lib_dir(const char* s);
int opt_help(const char* s);
int opt_version(const char* s);

int opt_define_var(const char* s);
int opt_trace(const char* s);
int opt_dump_cfg(const char* msg);
int opt_dump_cfg_html(const char* msg);

void	out_msg(int err,const char* fmt, ...);
void	show_cgi_header(void);

static	int	exec_script(const char *name, int argc, char **argv);
static  int	output_bytecode_script(const char* name,const char* output, xl_code code);
static	xl_code	get_code();
static	int	exec_code(xl_code,int argc,char** argv);
static  int	output_asm(const char* out, xl_code code);


static	xl_object create_arg_object(int argc, char **argv);
extern xl_object xl_env_new(void);


sgo_rec options[] = {
    { "inc-dir" ,"I"  ,SGO_ALLOW_POSTFIX, SGO_CALLBACK,  (void*)opt_include_dir,0,"add include directory"  },
    { "lib-dir" ,"L"  ,SGO_ALLOW_POSTFIX, SGO_CALLBACK,  (void*)opt_lib_dir,0,"add modules directory"  },
    { "compile" ,"c"  ,SGO_NO_PARAM,      SGO_BOOL,	 (void*)&opt_compile,0,"compile only"  },
    { "bytecode","b"  ,SGO_NO_PARAM,      SGO_BOOL,	 (void*)&opt_bytecode,0,"interpret input file as bytecode"  },
    { "output"  ,"o"  ,SGO_ALLOW_POSTFIX, SGO_STRING,    (void*)&opt_output,0,"set output file name" },
    { "show-code"  ,"C"  ,SGO_NO_PARAM, SGO_BOOL,    (void*)&opt_show_code,0,"show code on stdout" },
    { "trace-","T"   , SGO_ALLOW_POSTFIX, SGO_CALLBACK, (void*)opt_trace,0,"enable or disable(prepend name with no-) specified trace" },
    { "output-asm"  ,"S"  ,SGO_NO_PARAM, SGO_BOOL,    (void*)&opt_output_asm,0,"output assembler code" },
/*    { "axl-path","ap" ,SGO_ALLOW_POSTFIX, SGO_STRING,    (void*)&opt_axl_path,0,"set axl path" },*/
    { "exec","e"      ,SGO_ALLOW_POSTFIX, SGO_STRING,    (void*)&opt_exec,0,"interpret code from command line" },
    { "cgi-header" ,"ch"  ,SGO_NO_PARAM	, SGO_BOOL,  (void*)&opt_cgi_header,0,"prepend everything with cgi header" },
    { "verbose","b"   ,SGO_ALLOW_POSTFIX,	SGO_INT, (void*)&opt_verbose_level,0,"set verbosity level"},
    { "dump-cfg" ,"d" ,SGO_NO_PARAM	, SGO_CALLBACK,  (void*)&opt_dump_cfg,0,"show configuration" },
    { "dump-cfg-html" ,"dh" ,SGO_NO_PARAM	, SGO_CALLBACK,  (void*)&opt_dump_cfg_html,0,"show configuration in html" },
    { "version" ,"v"  ,SGO_NO_PARAM	, SGO_CALLBACK,  (void*)&opt_version,0,"show version" },
    { "define"  ,"D"  ,SGO_ALLOW_POSTFIX, SGO_CALLBACK,  (void*)opt_define_var,0,"define variable NAME=VALUE" },
    { "help"    ,"h"  ,SGO_NO_PARAM	, SGO_CALLBACK,	 (void*)&opt_help,0,"show thie helo screen" },
};

int main(argc, argv)
    int argc;
    char **argv;
{
    int result = 0;
    pname = *argv++;
#ifdef ZSYS_WIN
    opt_axl_path = pname;
#endif
    pname = "axl";
    argc--;

#if 0 ||  defined( ZSYS_WIN )
    SET_DLL_ALLOC_VECT(zpio);
    SET_DLL_ALLOC_VECT(ztools);
    SET_DLL_ALLOC_VECT(axl);
#endif

#ifdef MDMF_DEBUG
    atexit(mdmf_stats);
#endif
    xl_init();
    atexit(xl_done);

    if( sgetopt(options,sizeof(options)/sizeof(sgo_rec),
	&argc,&argv,0,sgo_usage) < 0 ) {
	usage();
	exit(1);
    }
    errno = 0;
    if( opt_cgi_header )
	show_cgi_header();
    if( !opt_exec ) {
	if( argv[0] )
	    opt_script = argv[0];
	else
	    sgo_usage("missing -e 'script' or script name");
    }


    {
	xl_code code;
	code = get_code();
	if( !code ) {
	    if ( errno )
		out_msg(1, "%s: %s\n", *argv, zstrerror(errno));
	    return 1;
	}
/*	zprintf("%s: verbose: obtaining code succesfull\n",pname);*/
	if( opt_show_code ) {
/*	    zprintf("%s: verbose: prining code\n",pname);*/
	    xlc_print(code);
	    result = 0;
	} else
	if( opt_output_asm ) {
	    if( output_asm(opt_output, code ) < 0 ) {
		out_msg(1,"%s: %s\n", *argv, zstrerror(errno));
		result = 1;
	    } else
		result = 0;

	} else
	    if( opt_compile ) {
/*		zprintf("%s: verbose: generating bytecode script\n",pname);*/
		result = output_bytecode_script(opt_script,opt_output,code);
/*		zprintf("%s: verbose: generating bytecode script done ... %i\n",pname,result);*/
		result = result < 0 ? 1 : 0;
	    } else {
/*		zprintf("%s: verbose: executing code\n",pname);*/
		result = exec_code(code,argc,argv);
/*		zprintf("%s: verbose: executing code ... done %i\n",pname,result);*/
	    }
	xlc_free(code);
    }
    if( global_ctx )
	xlx_free(global_ctx);

    return result;
}


static	xl_code	get_code()
{
    xl_code code;
    if (opt_bytecode) {
	ZSTREAM in;
	in = zopen(opt_script, ZO_READ);
	if (in == NULL)
	    return NULL;
	zsetbuf(in,1024,0);
	{ /* check if we have #! /usr/bin/axl -b or something on first line */
	    char buf[100];
	    zfgets(in,buf,sizeof(buf)-1);
	    if( strncmp(buf,"#!",2) != 0 )
		zseek(in,0,ZSEEK_SET);
	}
	code = xlc_new();
	if (xlc_load(code,in)) {
	    xlc_free(code);
	    return NULL;
	}
	if (zclose(in) < 0) {
	    xlc_free(code);
	    return NULL;
	}
	xlc_read_labels(code);
	errno = 0;
    } else if( opt_exec ) {
	errno = 0;
	code = xl_strparse(opt_exec,0);
    } else {
	errno = 0;
	code = xl_fparse(opt_script,0);
    }
    return code;
}

static	int	exec_code(xl_code code,int argc,char** argv)
{
    int r;
    xl_value_t ret;
    xl_object arg,cfg;
    xl_object env;
    if( global_ctx == NULL )
	global_ctx = xlx_new(NULL,NULL);
    if( !global_ctx ) {
	out_msg(1,"%s: cannot create context !\n",pname);
	return 1;
    }

    arg = create_arg_object(argc, argv);
    xlx_set_ref(global_ctx, "arg", (xl_value) arg);
    xlo_free(arg);
/*
    env = xl_env_new();
    xlx_set(global_ctx,"env",(xl_value)env,NULL);
    xlv_free((xl_value)env);
*/

    cfg = xl_cfg_get_obj();
    xlx_set_ref(global_ctx,"axl_cfg",(xl_value)cfg);


    r = xl_run(code,code->entry_point,XL_RUN_SCRIPT,global_ctx, &ret,0 ,NULL);
    if( r < 0 )
	r = 1;
    else {
	r = xlv_int(&ret);
	xlv_done(&ret);
    }
    return r;
}

static int output_bytecode_script(const char* name,const char* output, xl_code code)
{
    char buf[Z_PATH_MAX];
    ZSTREAM f;
    if( !output ) {
	zpio_file_set_ext(name,".xlb",buf,sizeof(buf));
	output = buf;
    }
    if( code == NULL )
	return -1;

    f = zopen(output,ZO_WRITE | ZO_CREAT | ZO_TRUNC );

    if( f == NULL )
	return -1;

    zfprintf(f,"#!/usr/bin/env axl -b\n");

    xlc_store(code,f);

    return zclose(f);
}

static xl_object create_arg_object(int argc, char **argv)
{
    xl_object o = xlo_new_map();
    int i = 0;
    strcpy(o->name,".arg");
    if (!o)
	return NULL;
    xlo_add_field_int(o, "count", argc);
    while (*argv)
	xlo_add_index_str(o, i++, *argv++);
    return o;
}

void usage(FILE* out)
{
    fprintf(out,"Usage: %s [PARAMETERS] filename [ARGUMENTS ... ]\n",pname);
    fprintf(out,"Arguments can be:\n");
    sgetopt_genusage(options,sizeof(options)/sizeof(sgo_rec),out);
    fprintf(out,"Default behavior is to compile <filename> and execute it with ARGUMENTS\n");
}

int opt_include_dir(const char* s)
{
    xl_push_include_path((char*)s);
    return 0;
}
int opt_lib_dir(const char* s)
{
    xl_push_library_path((char*)s);
    return 0;
}
void sgo_usage(const char* msg)
{
    out_msg(1,"%s: %s\n",pname,msg);
    usage(stderr);
    exit(1);
}
int opt_help(const char* s)
{
    usage(stdout);
    out_msg(0,"axl, version %s\n",VERSION);
    exit(0);
    return 0;
}
int opt_version(const char* s)
{
    out_msg(0,
	"axl, version %s\n"
	,VERSION);
    exit(0);
    return 0;
}

int opt_define_var(const char* s)
{
    if( global_ctx == NULL ) {
	global_ctx = xlx_new(NULL,NULL);
	if( !global_ctx ) {
	    out_msg(1,"%s: cannot create context !\n",pname);
	    return -1;
	}
    }
    {
	char name[300];
	const char* x;
	int l;
	x = strchr(s,'=');
	if( x == NULL ) { /* consider whole string as name */
	    xlx_set_int(global_ctx,s,1);
	    return 0;
	}
	l = x-s;
	l = zmin(sizeof(name)-1,l);
	strncpy(name,s,l);
	name[l] = '\0';
	x++;
	xlx_set_str(global_ctx,name,x);
    }
    return 0;
}

int opt_trace(const char* s)
{
    int enable = 1;
    if( !s ) return 0;
    if( strncmp(s,"no-",3)==0) {
        s += 3;
        enable = 1;
    }
    xldbg_add_mask(s,enable);
    return 0;
}
int opt_dump_cfg(const char* msg)
{
   out_msg(0,"axl - " VERSION "\n"
	      "configuration (from shared library)\n"
	      "version   = %s\n"
	      "build     = %s\n"
	      "fullname  = %s\n"
	      "bindir    = %s\n"
	      "libdir    = %s\n"
	      "incdir    = %s\n"
	      "confdir   = %s\n"
	      "pkglibdir = %s\n"
	      "pkginc    = %s\n",
	      xl_cfg_get_str("version"),
	      xl_cfg_get_str("build"),
	      xl_cfg_get_str("fullname"),
	      xl_cfg_get_str("bindir"),
	      xl_cfg_get_str("libdir"),
	      xl_cfg_get_str("incdir"),
	      xl_cfg_get_str("confdir"),
	      xl_cfg_get_str("pkglibdir"),
	      xl_cfg_get_str("pkgincdir")
	      );
     exit(0);
}
int opt_dump_cfg_html(const char* msg)
{
    opt_cgi_header = 1;
    opt_dump_cfg(msg);
    exit(0);
}

xl_str plain2html(const char* s);

void	out_msg(int err,const char* fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    if( opt_cgi_header ) {
	xl_str x = plain2html(fmt);
	show_cgi_header();
	if( err ) {
	    zfprintf(zstdout,"<h2>AXL: This is an error message!</h2>");
	}
	zvfprintf(zstdout,xls_get(x),ap);
	xls_free(x);
    } else {
	if( err )
	    zvfprintf(zstderr,fmt,ap);
	else
	    zvfprintf(zstdout,fmt,ap);
    }
    va_end(ap);
}
static int cgi_hs = 0;

void	show_cgi_header(void)
{
    if( !cgi_hs ) {
	cgi_hs = 1;
	zprintf(
	    "Content-type: text/html\r\n\r\n");
    }
}
struct _unk_{
    char* ptn;
    char* rpl;
} rpt[] = {
    {"<"	,"&lt;" },
    {">"	,"&gt"	},
    {"&"	,"&amp"	},
    {"\r\n\r\n"	,"<p>\n"	},
    {"\n\n"	,"<p>\n"	},
    {"\r\n"	,"<br>\n"	},
    {"\n"	,"<br>\n"	}
};
xl_str plain2html(const char* s)
{
    xl_str dest = NULL;
    const char* x = s,*y;
    int i;
    int len;
    while( s && *s ) {
	for( i = 0; i < countof(rpt); i++ )
	    if( (y = strstr(s,rpt[i].ptn)) != NULL ) {
		len = y-s;
		dest = xls_addn(dest,s,len);
		dest = xls_add(dest,rpt[i].rpl);
		s = y + strlen(rpt[i].ptn);
		break;
	    }
	if( i == countof(rpt) )
	    break;
    }
    if( dest == NULL )
	dest =  xls_new(x);
    return dest;
}


/* from xlasm.c */
int	    xlasm_put_instr(ZSTREAM s,const xl_instr_t* instr);
int	    xlasm_put_value(ZSTREAM s,const xl_value_t* instr);

static  int	output_asm(const char* out, xl_code code)
{
    ZSTREAM sout = out ?
	zopen(out,ZO_NEW | ZO_TRUNC) : zstdout;
    int r = 0;
    int i;
    if( ! sout )
	return -1;

    zfprintf(sout,"# generated by axl -S \n");

    for( i = 0; i < code->last; i++ ) {
	zprintf("%i:  ",i);
	xlasm_put_instr( zstdout, code->buf+i);
    }
    
    if( sout != zstdout )
	r = zclose(sout);
    return r;
}

/*

    MEMORY TRACKING STUFF !

*/

#ifdef MDMF_DEBUG

#endif
