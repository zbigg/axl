/*
* File: 	xl.c
*
* Id:           $Id: xl.c 1.5 04/03/07 21:24:29+01:00 nikt@nowy.dom $
*
* Project:	axl
*
* Description:	High level axl routines
*
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
#include <string.h>
#include "dconfig.h"

#include "axl/axl.h"

#include "parse.h"

#include "arch.h"
#include "lexfunc.h"
#include "lex.h"
#include "codegen.h"
#include "zcompat/zpio.h"
#include "tools.h"
#include "axl_i.h"

#if (defined(ZSYS_WIN) || defined(ZSYS_DOS))
#   if !defined(ZDEV_MINGW) && !defined(ZDEV_CYGWIN)
#       define RELATIVE_RES_PATH
#   endif
#endif


#ifndef XL_BUILD
#   define XL_BUILD ZARCH_NAME"-"ZSYS_NAME"-"ZDEV_NAME
#endif

/* EXTERNS */
extern int	xlp_step(xl_thread thread,int howmany);
extern int 	yyparse(void);
/*
##################################################################
##
##	=Module XL Core
##		<TODO> add documentation here
##
##	Description ...
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	dsah blahblah blahblah blahblah blah blah blah blah blah.
##
##	Description ...
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah
##	blah blahblah blahblah blahblah blah blah blah blah blah.
##
##################################################################
*/
/* GLOBALS */

ZSTREAM xlst_parse_out;
ZSTREAM xlst_run_out;
ZSTREAM xlst_run_in;
ZSTREAM xlst_run_err;

int		xl_cfg_get_value(const char* name, xl_value dest);
const char*	xl_cfg_get_str(const char* name);
xl_object	xl_cfg_get_obj(void);

int		xl_init(void);
void		xl_done(void);
void            xl_atexit(void (*f)(void));

xl_code		xl_parse(ZSTREAM f,int script_type);
xl_code		xl_fparse(const char* name,int script_type);
xl_code 	xl_strparse(const char* code,int script_type);
int		xl_run(
		    xl_code	code,
		    int		ip,
		    int		run_mode,
		    xl_context	ctx,
		    xl_value	ret,
		    int		argc,
		    xl_value	argv);
/* LOCALS */

/* IMPLEMENTATION */

static xl_object cfg = NULL;

int _xl_init = 0;
extern int	xl_get_library_path(char* buf, unsigned int size);

#ifdef RELATIVE_RES_PATH
static void joinpath(char* dest, const char* path, const char* subdir)
{
    strcpy(dest,path);
    strcat(dest,Z_DIR_SEPARATOR_T);
    strcat(dest,subdir);
}
#endif

int		xl_cfg_get_value(const char* name, xl_value dest)
{
    xl_init();
    if( cfg == NULL ) {
	char buf[Z_PATH_MAX];
	cfg = xlo_new_map();
	if( ! cfg ) return -XLE_MEMORY;
	strcpy(cfg->name,".axl_cfg");

	if( xl_get_library_path(buf,sizeof(buf)) >= 0 )
	    xlo_add_field_str(cfg,"xlpath",buf);
#ifdef RELATIVE_RES_PATH
	{ /* remove dll name from end of library path */
	    char* ls = strrchr(buf,Z_DIR_SEPARATOR);
	    if( ls ) *ls = 0;
	}
	{
            char buf2[Z_PATH_MAX];
	    xlo_add_field_str(cfg,"bindir", buf );
	    joinpath(buf2,buf,"lib");
	    xlo_add_field_str(cfg,"libdir", buf2 );
	    joinpath(buf2,buf,"include");
	    xlo_add_field_str(cfg,"incdir", buf2 );
	    joinpath(buf2,buf,"conf");
	    xlo_add_field_str(cfg,"confdir", buf2 );
	    joinpath(buf2,buf,"modules");
	    xlo_add_field_str(cfg,"pkglibdir", buf2 );
	    joinpath(buf2,buf,"include"Z_DIR_SEPARATOR_T"axl");
	    xlo_add_field_str(cfg,"pkgincdir", buf2 );
	}
#else
	xlo_add_field_str(cfg,"bindir", PACKAGE_BINDIR );
	xlo_add_field_str(cfg,"libdir", PACKAGE_LIBDIR );
	xlo_add_field_str(cfg,"incdir", PACKAGE_INCDIR );
	xlo_add_field_str(cfg,"pkglibdir", PACKAGE_PKG_LIBDIR );
	xlo_add_field_str(cfg,"pkgincdir", PACKAGE_PKG_INCDIR );
	xlo_add_field_str(cfg,"confdir", PACKAGE_CONFDIR );
#endif
	xlo_add_field_str(cfg,"version", VERSION );
	xlo_add_field_str(cfg,"build", XL_BUILD );
	xlo_add_field_str(cfg,"fullname", "axl-"VERSION"("XL_BUILD")" );
    }
    if( dest && name)
	return xlo_field(cfg,name,0,dest);
    return 0;

}

xl_object   xl_cfg_get_obj(void)
{
    if(!cfg )
	xl_cfg_get_value(NULL,NULL);
    return cfg;
}
const char* xl_cfg_get_str(const char* name)
{
    xl_value x;
    const char* s;
    if(!cfg )
	xl_cfg_get_value(NULL,NULL);
    x = xlo_ffield(cfg,name);

    s = xlv_str(x);
    return s ? s : "unknown";
}

void	xldbg_init_module(void);

DEFINE_VECTOR(void*,fini_array);
IMPLEMENT_VECTOR(void*,fini_array);

fini_array* xlfi = NULL;
void	xlo_init_module(void);

void            xl_atexit(void (*f)(void)) {
    void* fx = f;
    if( !xlfi ) xlfi = fini_array_new(10);
    fini_array_push_back(xlfi, &fx);
}
void    xl_atexit_run(void) {
    int i,size = fini_array_count(xlfi);
    if( !xlfi ) return;
    for( i = 0; i < size; i++ ) {
        void (*f)(void) = *(void (**)(void)) fini_array_geti(xlfi,i);
        f();
    }
    fini_array_set_size(xlfi,0);
}

void    xl_module_symtab_init();
void    xl_module_symtab_done();
void	lex_free(void);
void	xlv_done_methods(void);
void    xl_code_done_module(void);
void	xldbg_done_module(void);

int		xl_init(void)
{
    if( _xl_init ) return 0;
    if( !xlfi )
        xlfi = fini_array_new(10);
    atexit( xl_done );
    xldbg_init_module();
    xl_module_symtab_init();
    _xl_init = 1;

    // initialize standard streams
    xlst_run_out = zstdout;
    xlst_parse_out = zstderr;
    xlst_run_err = zstderr;
    xlst_run_in = zstdin;

    // initialize standard callbacks


    return 0;
}

void		xl_done(void)
{
    if( !_xl_init ) return;
    if( cfg )
	xlo_free(cfg); cfg = NULL;
    {
	extern char* _debug_file;
	if( _debug_file ) axl_free(_debug_file);
    }
    lex_free();
    xl_module_symtab_done();
    xlv_done_methods();
    xl_code_done_module();
    xldbg_done_module();
    if( xlfi ) {
        xl_atexit_run();
        fini_array_free(xlfi);
        xlfi = NULL;
    }
    _xl_init = 0;
}
/*
##################################################################
##
##  =Function xl_strparse
##	compile AXL bytecode, read from memory
##
##  =Synopsis
##	|#include "axl/axl.h"
## 	|xl_code     xl_strparse(const char* code,int script_type);
##
##  =Description
##	This is wrapper to <xl_parse> function.
##
##	To execute compiled code use VM or xl_run function.
##
##  =Return value
##	The <xl_parse> function returns pointer to compiled code or
##	NULL on error. Returned pointer to code should be destryed
##	by xlc_free.
##
##################################################################
*/
xl_code 	xl_strparse(const char* code,int script_type)
{
    ZSTREAM fs = zstropen((char*)code,0,ZO_READ,ZMB_STATIC);
    xl_code c = xl_parse(fs,script_type);
    zclose(fs);
    return c;
}

/*
##################################################################
##
##  =Function xl_fparse
##	compile AXL bytecode, read from file
##
##  =Synopsis
##	|#include "axl/axl.h"
## 	|xl_code     xl_fparse(const char* file_name,int script_type);
##
##  =Description
##	This is wrapper to <xl_parse> function.
##
##	To execute compiled code use VM or xl_run function.
##
##  =Return value
##	The <xl_parse> function returns pointer to compiled code or
##	NULL on error. Returned pointer to code should be destroyed
##	by xlc_free.
##
##################################################################
*/
xl_code     xl_fparse(const char* name,int script_type)
{
    ZSTREAM f;
    xl_code c;
    if( name == NULL )
	return NULL;
    zfopen_preffered = ZFOP_ANSI;
    f = zfopen(name,ZO_READ);
    if( f == NULL )
	return NULL;
    c = xl_parse(f,script_type);
    zclose(f);
    return c;
}

/*
##################################################################
##
##  =Function xl_parse
##	compile AXL bytecode, read from stream
##
##  =Synopsis
##	|#include "axl/axl.h"
## 	|xl_code     xl_parse(ZSTREAM f,int script_type);
##
##  =Description
##
##	To execute compiled code use VM or xl_run function.
##
##  =Return value
##	The <xl_parse> function returns pointer to compiled code or
##	NULL on error. Returned pointer to code should be destroyed
##	by xlc_free.
##
##################################################################
*/
xl_code xl_code_global = NULL;
xl_code     xl_parse(ZSTREAM f,int script_type)
{
    xl_code code;
    int     pr;
    extern int there_were_errors;

    if( f == NULL )
	return 0;

/* TODO: for multithreaded library LOCK MUTEX*/

    there_were_errors = 0;

    lex_set_input_stream(f);
    xl_set_fctx_sig();
    code = xl_code_global = xlc_new();
#if 0
    {
	extern int yydebug;
	yydebug = 0;
    }
#endif
    if (( pr = (int) yyparse () ) != 0 || there_were_errors ) {
	xlc_free(xl_code_global);
	return NULL;
    }
#if defined XLC_PRINT_CODE || 0
    xlc_print(xl_code_global);
#endif
    code = xl_code_global;
    xlc_detach_ptr(code);
    xl_code_gc();
    xl_code_global = NULL;

    lex_reset();

/* TODO: for multithreaded library UNLOCK MUTEX*/

    xlcg_exit(code,0);

    xlc_read_labels(code);
    xl_restore_fctx_sig();

    return code;
}

/*
##################################################################
##
##  =Function xl_run
##	execute AXL bytecode
##
##  =Synopsis
##	|#include "axl/axl.h"
## 	|int	xl_run( xl_code		code,
## 	|		int		ip,
## 	|		int		run_mode,
## 	|		xl_context	ctx,
## 	|		xl_value	ret,
## 	|		int		argc,
## 	|		xl_value	argv)
##
##  =Description
##	Function <xl_run> executes AXL bytecode pointed by <code>.
##	It starts execution at <ip> and run until EXIT or RETURN
##	instruction is spotted depending on <run_mode>.
##
##	Value of <run_mode> have can be one of:
##	  * XL_RUN_SCRIPT - run until the exit instruction.
##			    Exit instruction is appended to all
##			    code generated by xl_XXXparse functions.
##	  * XL_RUN_FUNCTION - run until the return instruction
##			    Value returned by return instruction
##			    is passed to place pointed by <ret>.
##
##	The <ctx> argument is optional and provides global variable
##	context within script will be executed.
##
##  =Passing arguments
##	If code:ip point to begining of function the <argc> and <argv>
##	parameters are parametrers passed to function. <argc> must contain
##	number of parameters and <argv> is array of initialized
##	xl_values length at least <argc>.
##
##  =Return value
##	The <xl_run> function return values:
##		* -1 - on error.
##		* 0  - on success, *<ret> contains returned value.
##
##################################################################
*/
int		xl_run(
		    xl_code	code,
		    int		ip,
		    int		run_mode,
		    xl_context	ctx,
		    xl_value	ret,
		    int		argc,
		    xl_value	argv)

{
    xl_value_t* xlths_add(xl_thread th,int no);	/* proc.c */
    xl_thread_t th;
    int ok = 0;
    xlv_clear(ret);
    if( !code ) return -1;

    memset(&th,0,sizeof(th));
    th.code = code;
    th.nip = ip;
    th.run = 1;
    th.run_mode = run_mode;
    if( run_mode == XL_RUN_FUNCTION ) {
	if( argc > 0 ) {
	    int i;
	    xl_value s = xlths_add(&th,argc);
	    for( i = 0; i < argc; i++ )
		xlv_cpy(s - argc+i, argv+i);
	    xlv_set_int( &th.regs[REG_PARAM_PTR], 0 );
	    xlv_set_int( &th.regs[REG_PARAM_COUNT], argc );
	}
    }
/*  th.ic = 0;
    th.ip = 0;
    th.sleep = 0;
    th.uid = 0;
    th.interrupted = 0;
    th.debug_file = NULL;
    memset(&t->stack,0,sizeof(t->stack));
    for( i = 0; i < countof(th.regs) ; i++ ) {
	_xlv_clear( &th.regs[i] );
    }
    all above zeroed my memset
    ------------------------
*/
    if( th.ip > code->last || ip < 0 )
	return -1;
    if( ctx ) {
	th.var = th.var_global = ctx;
	ctx->ref_count ++ ;
    } else
	th.var = th.var_global = xlx_new(NULL,NULL);

    while( 1 ){
	switch( xlp_step(&th,-1) ) {
	case 1:		/* still running, so nothing happens */
	    continue; /* while (1) */
	case 0:		/* stopped: finished or interrupted */
	    ok = 1;
	    break;
	case -1:	/* error */
	    ok = 0;
	    break;
	}
	break;
    }
    if( ok ) { 	/* save the result */
	xlv_set(ret, &th.ret);
    }
    xlv_done(&th.ret);

    {		/* destroy registers and stack */
	unsigned i;
	for( i = 0; i < countof(th.regs) ; i++ ) {
	    xlv_done( &th.regs[i] );
	}

	for( i = 0; i < th.stack.itop; i++ )
	    xlv_done(th.stack.ptr+i);

	if( th.stack.ptr )  
	    axl_free(th.stack.ptr);
    }
		/* destroy all variable contexts */
    while( th.var ) {
	xl_context tmp = th.var->prev_ctx;
	xlx_free(th.var);
	th.var = tmp;
    }

    xls_free(th.debug_file);

    return ok ? 0 : -1;
}

int	xl_call(xl_value func,
		xl_context ctx,
		int argc,
		xl_value argv);

int	xl_call_method(
		xl_value obj,
		xl_value func,
		xl_context ctx,
		int argc,
		xl_value argv);

int	xl_call_method_name(
		xl_value	obj,
		const char*	name,
		xl_context	ctx,
		int		argc,
		xl_value	argv);

/*
##################################################################
##  =Function xl_set_ctl_prop,xl_get_ctl_prop
##      short description of xl_set_ctl_prop
##
##  =Synopsis
##  |int             xl_set_ctl_prop(int i, void* param);
##  |void*           xl_get_ctl_prop(int i);
##
##  =Description
##      These functions define interface for "runtime"
##      probing and customization of library behaviour. They allow
##      to get/set some callback functions input/output/diagnostic streams
##      etc.
##
##      <xl_set_ctl_prop> function is is used to customize routines and objects
##      used by xl library while running.
##      
##      <xl_get_ctl_prop> function probes for specific object.
##
##      Both functions takes <i> as an argument which identifies 
##      specific object. Streams of type ZSTREAM that
##      can be probed/set with functions: 
##
##          - XLCP_PARSE_OUT    : destination of parser messages
##          - XLCP_RUN_OUT      : destination of 'echo' xl runtime
##                          command, so called <stdout>
##          - XLCP_RUN_IN       : <stdin> of xl program
##          - XLCP_RUN_ERR      : <stderr> of xl program
##                      
##  
##  =Return values
##      <xl_set_ctl_prop> returns 0 on success, -1 on error.
##      <xl_get_ctl_prop> returns usually NULL on error, and
##          specific - usually pointer - value on success.
*/



/*
    xlst_run_out = zstdout;
    xlst_parse_out = zstderr;
    xlst_run_err = zstderr;
    xlst_run_in = zstdin;
*/
int             xl_set_ctl_prop(int i, void* p)
{
    switch(i) {

    case XLCP_PARSE_OUT:
        xlst_parse_out = (ZSTREAM)p;
        return 0;
    case XLCP_RUN_OUT:
        xlst_run_out = (ZSTREAM)p;
        return 0;
    case XLCP_RUN_IN:
        xlst_run_in = (ZSTREAM)p;
        return 0;
    case XLCP_RUN_ERR:
        xlst_run_err = (ZSTREAM)p;
        return 0;
    default:
        return -1;
    }
}
void*           xl_get_ctl_prop(int i) {
    switch(i) {
    case XLCP_PARSE_OUT:
        return (void*) xlst_parse_out;
    case XLCP_RUN_OUT:
        return (void*) xlst_run_out;
    case XLCP_RUN_IN:
        return (void*) xlst_run_in;
    case XLCP_RUN_ERR:
        return (void*) xlst_run_err;
    default:
        return (void*)NULL;
    }
}
