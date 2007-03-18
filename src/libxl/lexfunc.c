/*
* File: lex.c
*
* Id:           $Id: lexfunc.c 1.5 04/03/07 21:24:27+01:00 nikt@nowy.dom $
*
* Project:	axl
*
* Description:	yylex helpers
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*
* Conditional defines:
    XL_LEX_USE_BUFFER	- define it to size of input buffer, if you want
			    buffered input for lex
    AXL_COMPILED_INCLUDE_PATH 	- lex include path

  Which are separated by according to platform by ':' or ';'.

  Include files search order:
    1.	current directory for currently parsed file
    2.	lex_user_include_path	- path-like separated list of directories
				  provided by lex_set_include_path()
    3.	AXL_INCLUDE_PATH	- path-like separated list of directories
				  read from environment
    3.	AXL_COMPILED_INCLUDE_PATH		- compiled in path-like separated list of
				  directories
				  on unices it's set by configure, on unix
				  it's NULL


*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dconfig.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"

#include "lexfunc.h"
#include "lex.h"
#include "tools.h"
#if !defined END_TEXT || !defined BEGIN_TEXT
#include "grammar.h"
#endif

#include "axl_i.h"


#if defined(ZSYS_WIN) || defined (ZSYS_MSDOS)
#   define AXL_COMPILED_INCLUDE_PATH NULL
#endif

#ifndef AXL_COMPILED_INCLUDE_PATH
#	ifdef PACKAGE_PKG_INCDIR
#		define AXL_COMPILED_INCLUDE_PATH PACKAGE_PKG_LIBDIR
#	else
#		define AXL_COMPILED_INCLUDE_PATH "/usr/local/lib/axl"
#	endif
#endif

char	lex_linebuf[1024] = { 0 };
int	lex_column = 0;

int	lex_finished_file(void);
void	lex_push_include_ctx(const lex_ctx_t* newctx);
void	lex_pop_include_ctx(void);

static const char*	lex_user_include_path = NULL;

static const char*	lex_set_include_path(const char* s)
{
    const char* x = lex_user_include_path;
    lex_user_include_path = s;
    return x;
}

lex_data_t	lex = {
    lex.ctx_t,	/* current ctx	*/
    0,		/* current ctx number */
    NULL,	/* error_stream */
    NULL,	/* debug_stream */
    {		/* ctx_t*/
		    /*[0] */
	{
	    "(stdin)",	/* filename */
	    Z_DIR_CURRENT,/* path */
	    NULL,	/* input stream */
	    NULL,	/* input stream original */
	    0,		/* owner of input_stream ? */
	    1,		/* line_number */
#if XL_LEX_PRIV_INPUT_BUFFER > 0
	    "", 	/* ibuf */
	    0,		/* ibuf_valid */
	    0,		/* ibuf_size */
	    0		/* ibuf_pos */
#endif
	}
    }
};

sstack* lex_inc_stack = 0;

int	lex_set_error_stream(ZSTREAM s)
{
    lex.error_stream = s;
    return (s!=NULL);
}
int	lex_set_debug_stream(ZSTREAM s)
{
    lex.debug_stream = s;
    return (s!=NULL);
}
int	lex_set_input_stream(ZSTREAM s)
{
    const char* fname;
    if( s == NULL )
	return 0;

    lex.ctx->input_stream = s;

    fname = zfname(s);
    xl_file_path(fname,lex.ctx->path,Z_PATH_MAX);
    xl_file_name(fname,lex.ctx->filename,Z_FILE_MAX);

    lex.ctx->input_stream_owner = 0;
    lex.ctx->line_number = 1;
#if XL_LEX_PRIV_INPUT_BUFFER > 0
    lex.ctx->ibuf_valid = 0;
#endif

    if( 1 ) {
	xlc_debug_new_file(lex_filename);
	xlc_debug_new_line(lex_line_number);
    }
    return 1;
}
int	lex_include_stream(ZSTREAM s)
{
    const char* fname;
    lex_ctx_t	newctx;

    if( s == NULL )
	return -1;
    fname = zfname(s);
    xl_file_path(fname,newctx.path,Z_PATH_MAX);
    xl_file_name(fname,newctx.filename,Z_FILE_MAX);

    newctx.input_stream_owner = 0;
    newctx.line_number = 1;

    lex_push_include_ctx(&newctx);
    return 0;
}

static char* paths_stack[20];
static int paths_ptr = 0;

static int lex_push_find_path(const char* path)
{
    if( path == NULL || paths_ptr == countof(paths_stack) )
	return -1;

    paths_stack[paths_ptr++] = (char*)path;
    return 0;
}

static char* lex_pop_find_path()
{
    if( paths_ptr == 0 ) return NULL;
    return paths_stack[--paths_ptr];
}

int xl_push_include_path(const char* path)
{
    return lex_push_find_path(path);
}

char* xl_pop_include_path()
{
    return lex_pop_find_path();
}
int	__lex_include_file(const char* name)
{
    char*	paths[countof(paths_stack)+5];
    char	fname[Z_PATH_MAX];
    lex_ctx_t	newctx;
    if( name[0] == Z_DIR_SEPARATOR ) {
	strncpy(fname,name,Z_PATH_MAX);
	fname[Z_PATH_MAX-1] = 0;
    } else {
	int ip = 0;
	char* x;
	if( paths_ptr > 0 ) {
	    int i = paths_ptr-1;
	    for( ; i >= 0 ; i-- )
		paths[ip++] = paths_stack[i];
	}
	paths[ip++] = lex.ctx->path;

	if( lex_user_include_path )  paths[ip++] = (char*)lex_user_include_path;

	x = getenv("AXL_INCLUDE_PATH");
	if( x ) paths[ip++] = x;

	/* REMOVE: this deprecated feature */
	x = getenv("XL_INCLUDE_PATH");
	if( x )  paths[ip++] = x;

	paths[ip++] = (char*)xl_cfg_get_str("pkglibdir");

	paths[ip] = 0;

	if( xl_find_file_by_paths(paths,name,ZR_OK,fname,Z_PATH_MAX) < 0 )
	    return -1;
    }
    /* divide it into /path/name */
    xl_file_path(fname,newctx.path,Z_PATH_MAX);
    xl_file_name(fname,newctx.filename,Z_FILE_MAX);

    if( (newctx.input_stream = zopen(fname,ZO_READ)) == NULL )
	return -1;
#ifdef XL_LEX_USE_BUFFER
    newctx.input_stream_org = newctx.input_stream;
    newctx.input_stream = zibuf_creat(newctx.input_stream,XL_LEX_USE_BUFFER);
#endif
    newctx.input_stream_owner = 1;
    newctx.line_number = 1;
#if XL_LEX_PRIV_INPUT_BUFFER > 0
    newctx.ibuf_valid = 0;
#endif
    lex_push_include_ctx(&newctx);
    return 0;
}
int	lex_include_file(const char* name,int safe)
{
    if( __lex_include_file(name) >= 0 )
	return 0;
    else  {
	if( !safe )
	    lex_error(LEX_ERR_INCLUDE,"include %s: %s",name,zstrerror(errno));
	return -1;
    }
}

void	lex_push_include_ctx(const lex_ctx_t* newctx)
{
    lex.ctx = lex.ctx_t + ++lex.ctx_number;
    memcpy(lex.ctx,newctx,sizeof(lex_ctx_t));

    if( 1 ) {
	char buf[Z_PATH_MAX];
	strcpy(buf,lex.ctx->path);
	strcat(buf,Z_DIR_SEPARATOR_T);
	strcat(buf,lex.ctx->filename);
	xlc_debug_new_file(buf);
	xlc_debug_new_line(lex_line_number);
    }
}

void	lex_pop_include_ctx(void)
{
    if( lex.ctx_number > 0 )
	lex.ctx = lex.ctx_t + --lex.ctx_number;

    if( 1 ) {
	char buf[Z_PATH_MAX];
	strcpy(buf,lex.ctx->path);
	strcat(buf,Z_DIR_SEPARATOR_T);
	strcat(buf,lex.ctx->filename);
	xlc_debug_new_file(buf);
	xlc_debug_new_line(lex_line_number);
    }
}
/**
    Return 1 if there is still possiblity to read else 0.
*/
int	lex_finished_file(void)
{
#ifdef XL_LEX_USE_BUFFER
    zclose(lex.ctx->input_stream);
    lex.ctx->input_stream = NULL;
    if( lex.ctx->input_stream_owner )
	zclose(lex.ctx->input_stream_org);
    lex.ctx->input_stream_org = NULL;
#else
    if( lex.ctx->input_stream_owner )
	zclose(lex.ctx->input_stream);
    lex.ctx->input_stream = NULL;
#endif

    if( lex.ctx_number > 0 ) {
	lex_pop_include_ctx();
	return 1;
    } else {
	return 0;
    }
}

extern void lex_context_stack_free(void);

void	lex_free(void)
{
    done_operator_and_keywords_table();
    lex_context_stack_free();

}

int	lex_init(void)
{
    return 0;
}

void	lex_reset(void)
{
    extern void frame_destroy(void);
    frame_destroy();
}

int	lex_switch_table[LEX_LAST+1] = { 1,0 };

struct {
    char* name;
    int code;
} _keyw[] = {
   { "new"	,NEW	},
   { "copy"	,COPY	},
   { "if"	,IF	},
   { "return"	,RETURN },
   { "else"	,ELSE	},
   { "while"	,WHILE	},
   { "for"	,FOR	},
   { "do"	,DO	},
   { "delete"	,INSTR_DELETE	},
   { "func"	,FUNC	},
   { "switch"	,SWITCH },
   { "case"	,CASE	},
   { "default"	,DEFAULT},
   { "in"	,INSTR_IN	},
   { "range"	,RANGE	},
   { "set"	,SET	},
   { "include"	,INCLUDE},
   { "echo"	,ECHO	},
   { "builtin"	,BUILTIN},
   { "global"	,GLOBAL },
   { "with"	,WITH   },
   { "ewhile"	,EWHILE },
   { "efor"	,EFOR   },
   { "safe"	,SAFE	},
   { "var"	,TOK_NEW_VAR },
  /* { "array"    ,TOK_ARRAY },*/
   { 0, 0
 },
};

struct {
    char* name;
    int code;
} _op[] = {
   { "?="	,SET_ASK },
   { "+="	,SET_ADD },
   { "-="	,SET_SUB },
   { "*="	,SET_MUL },
   { "/="	,SET_DIV },
/*
   { "|="	,SET_OR },
   { "&="	,SET_AND },
   { "^="	,SET_XOR },
*/
   { "=="	,EQ	}
,
   { "!="	,NEQ	}
,
   { "<>"	,NEQ	},
   { ">="	,GE	},
   { "<="	,LE	},
/*
    { ":="	,NF },

*/
   { "||"	,OROR	},
   { "&&"	,ANDAND },
   { "^^"	,XORXOR },
   { "{?"	,BEGIN_TEXT },
   { "?}"	,END_TEXT },
   { "><"	,XCHG	},
   { "<-"	,ARROW_L },
   { "->"	,ARROW_R },
   { "::"	,CTX_OP },
   { ".."	,TWO_DOT },
   { "++"	,PLUS_PLUS },
   { "--"	,MINUS_MINUS },
   { 0,0 }
};


sbtree* _op_tree = NULL;
sbtree* _keyw_tree = NULL;
int	lex_op_at_exit_set = 0;

void	init_operator_and_keywords_table(void)
{
    int i;
    memset(&lex_switch_table,sizeof( lex_switch_table ),0);
    if( _op_tree == NULL ) {
	_op_tree = sbtree_new();
	i = 0;
	while ( _op[i].name ) {
	    sbtree_add(_op_tree,_op[i].name,(void*)&(_op[i].code));
	    i++;
	}
	if( !lex_op_at_exit_set ) {
	    xl_atexit(done_operator_and_keywords_table);
	    lex_op_at_exit_set = 1;
	}
    }
    if( _keyw_tree == NULL ) {
	_keyw_tree = sbtree_new();
	i = 0;
	while ( _keyw[i].name ) {
	    sbtree_add(_keyw_tree,_keyw[i].name,(void*)&(_keyw[i].code));
	    i++;
	}
	if( !lex_op_at_exit_set ) {
	    xl_atexit(done_operator_and_keywords_table);
	    lex_op_at_exit_set = 1;
	}
    }
}
void	done_operator_and_keywords_table(void)
{
    if( _op_tree != NULL ) {
	sbtree_delete(_op_tree);
	_op_tree = NULL;
    }
    if( _keyw_tree != NULL ) {
	sbtree_delete(_keyw_tree);
	_keyw_tree = NULL ;
    }
}
int*	operator_id(char first,char second)
{
    char buf[3];
    if( !_op_tree )
	init_operator_and_keywords_table();
    buf[0] = first;
    buf[1] = second;
    buf[2] = 0;
    return (int*) (sbtree_find(_op_tree,buf));
}
int*	keyword_id(const char* keyw)
{
    if( !_keyw_tree )
	init_operator_and_keywords_table();
    return (int*) (sbtree_find(_keyw_tree,keyw));
}
#if XL_LEX_PRIV_INPUT_BUFFER > 0
    /* lex_getc which use lex private input buffer */
int	lex_validate_ibuf(void)
{
    if( ! lex.ctx->ibuf_valid ) {
	register int r;
	if( lex.ctx->input_stream == NULL )
	    return EOF;
	r =  zread(lex.ctx->input_stream,lex.ctx->ibuf,XL_LEX_PRIV_INPUT_BUFFER);
	if( r == 0 ) {
	    if( lex_finished_file() )
		return lex_validate_ibuf();
	    else
		return EOF;
	}
	if( r <= 0 )
	    return EOF;
	lex.ctx->ibuf_valid = 1;
	lex.ctx->ibuf_size = r;
	lex.ctx->ibuf_pos = 0;
	return 0;
    }
    return 0;
}
static int lex_ungetc_c = -2;

int	lex_getc(void)
{
    if( lex_ungetc_c >= -1 ) {
	int c = lex_ungetc_c;
	lex_ungetc_c = -2;
	if ( c == '\n' ) 
	    xlc_debug_new_line(++lex_line_number);
	return c;
    }
	
    if( ! lex.ctx->ibuf_valid )
	if( lex_validate_ibuf() == EOF )
	    return EOF;
    {	/* return data from buffer */
	register char x = lex.ctx->ibuf[lex.ctx->ibuf_pos++];
	lex.ctx->ibuf_valid = (lex.ctx->ibuf_pos < lex.ctx->ibuf_size);
	if ( x == '\n' ) {
	    xlc_debug_new_line(++lex_line_number);
	}
	return x;
    }
}
#else
    /* old version of lex_getc */


int	lex_getc(void)
{
    int c;
    if( lex_input_stream == NULL )
	return EOF;
    c = zfgetc(lex_input_stream);
    if( c < 0 ) {
	if( lex_finished_file() )
	    return lex_getc();
	return EOF;
    }
    if( c == '\n' ) {
	++lex_line_number;
	xlc_debug_new_line(lex_line_number);
    }
    return c;
}
#endif

#if XL_LEX_PRIV_INPUT_BUFFER > 0
void	lex_ungetc(int c)
{
    if( c == '\n' )
	--lex_line_number;
    if( ! lex.ctx->ibuf_valid )
	zungetc(lex_input_stream,c);
    else if( c == EOF )
	lex.ctx->ibuf_valid = 0;
    else if( lex.ctx->ibuf_pos > 0 )
	lex.ctx->ibuf[--(lex.ctx->ibuf_pos)] = (unsigned char)c, lex.ctx->ibuf_valid = 1;
    else if( lex_ungetc_c < -1 )
	lex_ungetc_c = c;
    else {
	zfprintf(zstderr,"lex: can't do ungetc, at %s:%i\n",lex_filename,lex_line_number);
	exit(1);
    }
}
#else
void	lex_ungetc(int c)
{
    if( c == '\n' )
	--lex_line_number;
    zungetc(lex_input_stream,c);
}
#endif

int	lex_ignore(char delimeter)
{
    register char x = delimeter - 1;
    while ( 1 ) {
	if( ! lex.ctx->ibuf_valid )
	    if( lex_validate_ibuf() == EOF )
		return EOF;
	x = lex.ctx->ibuf[lex.ctx->ibuf_pos++];
	lex.ctx->ibuf_valid = (lex.ctx->ibuf_pos < lex.ctx->ibuf_size);
	if ( x == '\n' ) {
	    xlc_debug_new_line(++lex_line_number);
	}
	if( x == delimeter )
	    break;
    }
    return x;
}
int	lex_read_to_sstr(sstr* s,char delimeter)
{
    char buf[200];
    register char x;
    register int  i = 0;
    while ( 1 ) {
	if( ! lex.ctx->ibuf_valid )
	    if( lex_validate_ibuf() == EOF )
		return EOF;
	x = lex.ctx->ibuf[lex.ctx->ibuf_pos++];
	lex.ctx->ibuf_valid = (lex.ctx->ibuf_pos < lex.ctx->ibuf_size);
	if ( x == '\n' )
	    xlc_debug_new_line(++lex_line_number);
	if( x == delimeter )
	    break;
	if( i == sizeof(buf)-1) {
	    buf[i] = 0;
	    if( sstr_add(s,buf) < 0 )
		return EOF;
	    i = 0;
	}
	buf[i++] = x;
    }
    if( i != 0 ) {
	buf[i] = 0;
	if( sstr_add(s,buf) < 0 )
	    return EOF;
    }
    return x;
}

int sstr_add_stray_expr(sstr* s);
int sstr_add_stray_expr_len(sstr* s,int len);

int	lex_read_to_sstr_stray(sstr* s,char delimeter)
{
    char buf[200];
    register char x;
    register int  i = 0;
    int s_len = sstr_len(s);
    while ( 1 ) {
	if( ! lex.ctx->ibuf_valid )
	    if( lex_validate_ibuf() == EOF )
		return EOF;
	x = lex.ctx->ibuf[lex.ctx->ibuf_pos++];
	lex.ctx->ibuf_valid = (lex.ctx->ibuf_pos < lex.ctx->ibuf_size);
	if ( x == '\n' )
	    xlc_debug_new_line(++lex_line_number);
	if( x == '\\' ) {
	/* append 'stray' expersion */
	    if( i != 0 ) {
		buf[i] = 0;
		if( sstr_add_len(s,s_len,buf,i) < 0 )
		    return EOF;
		s_len+=i;
		i = 0;
	    }
	    {
		int y = sstr_add_stray_expr_len(s,s_len);
		if( y < 0 )
		    return EOF;
		s_len+=y;
	    }
	    continue;
	}
	/* test if x is in delimeter */
	if( x == delimeter )
	    break;
	if( i == sizeof(buf)-1) {
	    buf[i] = 0;
	    if( sstr_add_len(s,s_len,buf,i) < 0 )
		return EOF;
	    s_len+=1;
	    i = 0;
	}
	buf[i++] = x;
    }
    if( i != 0 ) {
	buf[i] = 0;
	if( sstr_add_len(s,s_len,buf,i) < 0 )
	    return EOF;
    }
    return x;
}

int	lex_read_cspn_to_sstr_stray(sstr* s,char* delimeter)
{
    char buf[200];
    register char x;
    register int  i = 0;
    int s_len = sstr_len(s);
    while ( 1 ) {
	if( ! lex.ctx->ibuf_valid )
	    if( lex_validate_ibuf() == EOF )
		return EOF;
	x = lex.ctx->ibuf[lex.ctx->ibuf_pos++];
	lex.ctx->ibuf_valid = (lex.ctx->ibuf_pos < lex.ctx->ibuf_size);
	if ( x == '\n' )
	    xlc_debug_new_line(++lex_line_number);
	if( x == '\\' ) {
	/* append 'stray' expersion */
	    if( i != 0 ) {
		buf[i] = 0;
		if( sstr_add_len(s,s_len,buf,i) < 0 )
		    return EOF;
		s_len+=i;
		i = 0;
	    }
	    {
		int y = sstr_add_stray_expr_len(s,s_len);
		if( y < 0 )
		    return EOF;
		s_len+=y;
	    }
	    continue;
	}
	/* test if x is in delimeter */ {
	    register char* d = delimeter;
	    while( *d != '\0' ) {
		if( *d == x )
		    break;
		d++;
	    }
	    if( *d == x )
		break;
	}
	if( i == sizeof(buf)-1) {
	    buf[i] = 0;
	    if( sstr_add_len(s,s_len,buf,i) < 0 )
		return EOF;
	    s_len+=i;
	    i = 0;
	}
	buf[i++] = x;
    }
    if( i != 0 ) {
	buf[i] = 0;
	if( sstr_add_len(s,s_len,buf,i) < 0 )
	    return EOF;
    }
    return x;
}

int	lex_read_to_xls_stray(xl_str* s,char delimeter)
{
    char buf[200];
    register char x;
    register int  i = 0;
    while ( 1 ) {
	if( ! lex.ctx->ibuf_valid )
	    if( lex_validate_ibuf() == EOF )
		return EOF;
	x = lex.ctx->ibuf[lex.ctx->ibuf_pos++];
	lex.ctx->ibuf_valid = (lex.ctx->ibuf_pos < lex.ctx->ibuf_size);
	if ( x == '\n' )
	    xlc_debug_new_line(++lex_line_number);
	if( x == '\\' ) {
	/* append 'stray' expersion */
	    if( i != 0 ) {
		buf[i] = 0;
		if( (*s = xls_addn(*s,buf,i)) == NULL )
		    return EOF;
		i = 0;
	    }
	    {
		int error;
		extern xl_str xls_add_stray_expr(xl_str,int*);
		*s = xls_add_stray_expr(*s,&error);
		if( !s || error )
		    return EOF;
	    }
	    continue;
	}
	/* test if x is in delimeter */
	if( x == delimeter )
	    break;
	if( i == sizeof(buf)-1) {
	    buf[i] = 0;
	    if( (* s = xls_addn(*s,buf,i)) == NULL )
		return EOF;
	    i = 0;
	}
	buf[i++] = x;
    }
    if( i != 0 ) {
	buf[i] = 0;
	if( (*s = xls_addn(*s,buf,i)) == NULL )
	    return EOF;
    }
    return x;
}


int	lex_read_cspn_to_xls_stray(xl_str* s,char* delimeter)
{
    char buf[200];
    register char x;
    register int  i = 0;
    while ( 1 ) {
	if( ! lex.ctx->ibuf_valid )
	    if( lex_validate_ibuf() == EOF )
		return EOF;
	if( lex_ungetc_c >= -1 ) {
	    x = lex_getc();
	} else {
	    x = lex.ctx->ibuf[lex.ctx->ibuf_pos++];
	    lex.ctx->ibuf_valid = (lex.ctx->ibuf_pos < lex.ctx->ibuf_size);
	    if ( x == '\n' )
		xlc_debug_new_line(++lex_line_number);
	}
	if( x == EOF ) return EOF;
	if( x == '\\' ) {
	/* append 'stray' expersion */
	    if( i != 0 ) {
		*s = xls_addn(*s,buf,i);
		i = 0;
	    }
	    {
		int serr;
		extern xl_str xls_add_stray_expr(xl_str,int*);
		*s = xls_add_stray_expr(*s,&serr);
		if( !s || serr ) {
		    xls_free(*s);
		    *s = NULL;
		    return EOF;
		}
	    }
	    continue;
	}
	/* test if x is in delimeter */ {
	    register char* d = delimeter;
	    while( *d != '\0' ) {
		if( *d == x )
		    break;
		d++;
	    }
	    if( *d == x )
		break;
	}
	if( i == sizeof(buf)) {
	    *s = xls_addn(*s,buf,i);
	    i = 0;
	}
	buf[i++] = x;
    }
    if( i != 0 )
	*s = xls_addn(*s,buf,i);
    return x;
}



int	lex_errors = 0;
int	lex_warnings = 0;

void		lex_error(int code,const char* fmt,...)
{
    char buf[Z_PATH_MAX];
    if( lex.error_stream == NULL )
	lex.error_stream = (ZSTREAM) xl_get_ctl_prop(XLCP_PARSE_OUT);
    strcpy(buf,lex.ctx->path);
    strcat(buf,Z_DIR_SEPARATOR_T);
    strcat(buf,lex.ctx->filename);
    zfprintf(lex.error_stream,"%s:%i: ",buf,lex_line_number);
    {
	va_list vl;
	va_start(vl,fmt);
	zvfprintf(lex.error_stream,fmt,vl);
	va_end(vl);
    }
    zfputs(lex.error_stream,"\n");
    lex_errors++;
    there_were_errors = 1;
}
void		lex_warning(int code,const char* fmt,...)
{
    char buf[Z_PATH_MAX];
    if( lex.error_stream == NULL )
	lex.error_stream = (ZSTREAM) xl_get_ctl_prop(XLCP_PARSE_OUT);
    strcpy(buf,lex.ctx->path);
    strcat(buf,Z_DIR_SEPARATOR_T);
    strcat(buf,lex.ctx->filename);
    zfprintf(lex.error_stream,"%s:%i: warning(%i): ",buf,lex_line_number,code);
    switch(code ) {
    case LEX_WARN_TOO_LONG:
	zfprintf(lex.error_stream,"%s too long, cut",fmt);
	break;
    default:
	{
	    va_list vl;
	    va_start(vl,fmt);

	    zvfprintf(lex.error_stream,fmt,vl);
	    va_end(vl);
	}
	break;
    }
    zfputs(lex.error_stream,"\n");
    lex_warnings++;
}


const char*	xl_set_include_path(const char* s)
{
    return lex_set_include_path(s);
}
