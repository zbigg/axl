/*
* File: lex.c
*
* Id:           $Id: lex.c 1.6 04/02/24 01:39:33+01:00 zbigg@marl.(none) $
*
* Project:	axl
*
* Description:	yylex
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "axl/axl.h"

#include "xltree.h"
#include "parse.h"
#include "lexfunc.h"
#include "tools.h"

#if !defined END_TEXT || !defined BEGIN_TEXT
#include "grammar.h"
#endif

#include "axl_i.h"

/* Type of context --------v				*/
#define C_DOCUMENT	0 /* document			*/
#define C_CODE		1 /* code		*/
#define C_INTCODE	2 /* code inside the document	*/

int	c_context = C_CODE;
char	linebuf[1024] = "";	/* contains text of currently processed line */
char	token_buf[1024] = "";	/* contains body of current token */
int	last_token = ';';	/* code of last token */
int	last_char = ';';

#ifndef countof
#define countof(a) ( sizeof(a) / sizeof( (a)[0]) )
#endif

#define isodigit(c) ( (c) >= '0' && (c) <= '7' )
#define octdigit2int(a) ( (a) - '0' )

int	sstr_add_stray_expr(sstr* s);
static int get_stray_char(void);
static int hexdigit2int(char a);

static int push_tmpval(YYSTYPE* t,int token);
static int pop_tmpval(YYSTYPE* t);
static int tmpval_used();

extern int lex_read_cspn_to_xls_stray(xl_str* s,char* delimeter);
extern int lex_read_to_xls_stray(xl_str* s,char delimeter);

/*xl_str  symbol_str = NULL;*/

int yylex(void)
{
    int 	a,c;
    int 	to_return = -1;

#define return_token \
	to_return =

    if( tmpval_used() ) {
	return last_token = pop_tmpval(&yylval);
    }
    c = lex_getc();
    if( (c_context == C_DOCUMENT ) &&
	( last_token == ';' || last_token == BEGIN_TEXT )
	)  {
	    xl_str tmpsx = NULL;
	    lex_ungetc(c);
	    while(1) {
		c = lex_read_cspn_to_xls_stray(&tmpsx,"?$%");
		if( c == '?' ) {
		    int tmp = lex_getc();
		    if( tmp == '}' ) {
			lex_ungetc(tmp);
			break;
		    } else {
			char tmpb[3];
			tmpb[0] = c;
			tmpb[1] = tmp;
			tmpb[2] = 0;
			tmpsx = xls_addn(tmpsx,tmpb,2);
		    }
		} else
		    break;
	    }
	    if( tmpsx != NULL && xls_len(tmpsx) != 0 ) {
                int symid = xlsym_searchx(tmpsx);
		lex_ungetc(c);

                xls_free(tmpsx);
                tmpsx = xlsym_get_xlstr(symid);
		yylval.text = tmpsx;
		return_token TOK_GARBAGE;
	    }
    }
    if( to_return < 0 ) {
	while( 1 ) {
	    while( isspace(c) ) {
		c = lex_getc();
	    }
	    if( c == '#' ) {
		c = lex_ignore('\n');
	    } else if( c == '/' ) {
		int tmp = lex_getc();
		if( tmp == '/' ) {
		    c = lex_ignore('\n');
		} else if( tmp == '*' ) {
		    do {
			c = lex_ignore('*');
			tmp = lex_getc();
		    } while ( tmp != '/' && tmp != EOF );
		    if( tmp == EOF )
			c = EOF;
		    else
			c = lex_getc();
		} else {
		    lex_ungetc(tmp);
		    break;
		}
	    }
	    else
		break;
	}
	if( isdigit(c) || c == '.' ) {
	    char	buf[100];
	    char	bbuf[10];
	    int 	base = 0;
	    int 	have_float = 0;
	    int 	have_hex = 0;
	    char*	str_err;
	    int 	last_c = c;
	    a = 0;
	    buf[a++] = c;
	    buf[a] = 0;
	    c = lex_getc();
	    if( buf[0] == '.' ) {
		if( !isdigit(c) ) {
		    lex_ungetc(c);
		    token_buf[0] = '.';
		    token_buf[1] = 0;
		    return_token '.';
		    goto lex_return;
		} else	{
		    buf[0] = '0';
		    buf[1] = '.';
		    buf[2] = 0;
		    a = 2;
		    have_float = 1;
		}
	    }
	    if( c == 'X' || c == 'x' ) {
		do {
		    if( a < sizeof(buf) -1 )
			buf[a++] = c;
		    else
			a++;
		} while( isalnum( c = lex_getc()) );
		have_hex = 1;
		base = 16;
	    } else {
		while(
		       isalnum(c)
		    || c == '.'
		    || ( last_c == 'e' && c == '-' )
		    || c == '#'
		    ) {
		    if( c == '#' ) {
			base = strtol(buf,&str_err,10);
			if( str_err != NULL && *str_err != 0 ) {
			    base = -1;
			}
			a = 0;
			strncpy(bbuf,buf,sizeof(bbuf)-1);
			bbuf[sizeof(bbuf)-1] = 0;
			buf[0] = 0;
			have_float = 0;
		    } else if( a < sizeof(buf) -1 ) {
			buf[a++] = c;
			buf[a] = 0;
		    } else
			a++;
		    if( c == 'e' || c == '.' ) {
			if( base != 0 )
			    base = -1;
			else
			    have_float = 1;
		    }
		    last_c = c;
		    c = lex_getc();
		}
	    }
	    if( a < sizeof(buf)  )
		buf[a] = 0;
	    else {
		buf[sizeof(buf)-1] = 0;
		lex_warning(LEX_WARN_TOO_LONG,"number");
	    }
	    lex_ungetc(c);
	    strncpy(token_buf,buf,sizeof(token_buf));
	    if( have_float ) {
		yylval.real = strtod(buf,&str_err);
		if( str_err != NULL && *str_err != 0 ) {
		    lex_error(LEX_ERR_NUMBER,"real constant error :%s",buf);
		    return_token TOK_BAD_FLOAT;
		} else
		    return_token TOK_FLOAT;
	    } else if( have_hex ) {
		yylval.integer = strtol(buf,&str_err,16);
		if( str_err != NULL && *str_err != 0 ) {
		    lex_error(LEX_ERR_NUMBER,"hexadecimal constant error :%s",buf);
		    return_token TOK_BAD_INTEGER;
		} else
		    return_token TOK_INTEGER;
	    } else {
		if( base >= 0 && base < 37 )
		    yylval.integer = strtol(buf,&str_err,base);
		if( (str_err != NULL && *str_err != 0)
		    || base < 0
		    || base > 37 ) {
		    if( base != 0 )
			lex_error(LEX_ERR_NUMBER,"numeric constant with base error :%s#%s",bbuf,buf);
		    else
			lex_error(LEX_ERR_NUMBER,"numeric constant error :%s",buf);
		    return_token TOK_BAD_INTEGER;
		} else
		    return_token TOK_INTEGER;
	    }
	} else if( isalpha(c) || c == '_' ) {
	    char buf[200];
	    int* id;
	    a = 0;
	    buf[a++] = c;
	    while( isalnum(c = lex_getc()) || c == '_' )
		if( a < sizeof(buf) -1 )
		    buf[a++] = c;
		else
		    a++;
	    if( a < sizeof(buf) )
		buf[a] = 0;
	    else {
		buf[sizeof(buf)-1] = 0;
		lex_warning(LEX_WARN_TOO_LONG,"identifier");
	    }
	    strncpy(token_buf,buf,sizeof(token_buf));
	    lex_ungetc(c);

	    id = keyword_id(buf);
	    if( id ) {
		switch( *id ) {
		case CASE:
		    if( lex_switch == 0 )
			lex_error(0,"case outside switch statement");
		    break;
		case FOR:
		case EFOR:
		    if( c_context == C_INTCODE ) {
			lex_context_push(C_INTCODE);
			lex_context_push(C_INTCODE);
		    }
		    break;
		}
		return_token *id;
	    } else {
                xl_symbol* sym;
                int symid = xlsym_search(buf);
                xl_str name = xlsym_get_xlstr(symid);
                if( (sym = frame_get_sym(name)) != NULL ) {
                    yylval.sym = sym;
                    if( sym->dimc > 0 )
                        return_token TOK_SYMBOL_ARRAY;
                    else
                        return_token TOK_SYMBOL;                     
                } else {
                    yylval.text = name;
		    return_token TOK_IDENTIFIER;
                }
	    }
	} else if( c == '\'') {
	    c = lex_getc();
	    if( c == '\\' ) {
		c = get_stray_char();
	    }
	    yylval.integer = c;
	    a = lex_getc();
	    if( a != '\'' )
		return_token TOK_CHAR;
	    else
		return_token TOK_BAD_CHAR;
	} else if( c == '"' ) {
	    xl_str tmps = NULL;
            int symid;
	    if( lex_read_to_xls_stray(&tmps,'\"') != '\"' )
		lex_error(LEX_ERR_STRAY,"error in expression after stray");
            if( tmps == NULL ) {
                symid = xlsym_search("");
            } else {
                symid = xlsym_searchx(tmps);
                xls_free(tmps);
            }
            tmps = xlsym_get_xlstr(symid);
            yylval.text = tmps;
	    strncpy(token_buf,xls_get(tmps),sizeof(token_buf));
	    return_token TOK_STRING;
	} else	if( c == EOF ) {
	    return_token 0;
	} else {
	    a = lex_getc();
	    if( a != EOF ) {
		int* id;
		id = operator_id((char)c,(char)a);
		if( id != NULL ) {
		    switch( *id ) {
		    case BEGIN_TEXT:
			lex_context_push(C_DOCUMENT);
			break;
		    case END_TEXT:
			lex_context_pop();
			break;
		    }
		    token_buf[0] = c;
		    token_buf[1] = a;
		    token_buf[2] = 0;
		    yylval.oper = *id;
		    return_token  *id;
		    goto lex_return;
		}
	    }
	    lex_ungetc(a);
	    switch( c ) {
	    case '{':
		lex_context_push(C_CODE);
		break;
	    case '$':
	    case '%':
		if( c_context == C_DOCUMENT )
		    lex_context_push(C_INTCODE);
		break	;
	    case ';':
		if( c_context == C_INTCODE )
		    lex_context_pop();
		break;
	    case '}':
		lex_context_pop();
		break;
	    }
	    token_buf[0] = c;
	    token_buf[1] = 0;
	    yylval.oper = c;
	    return_token c;
	}
    }
lex_return:
    if( lex_if_cur() == 1 &&
	( to_return == ';' )
      ) {
	push_tmpval(&yylval,to_return);
	last_char = c;
	return TOK_BOGUS;
    }
    last_char = c;
    return last_token = to_return;
}

sstack* lex_context_stack = 0;

 void lex_context_stack_free(void) 
{
    if( lex_context_stack ) {
	sstack_free(lex_context_stack);
	lex_context_stack = 0;
    }
}
void	lex_context_push(int i)
{
    if( !lex_context_stack ) {
	lex_context_stack = sstack_new(30);
	xl_atexit(lex_context_stack_free);
    }
    sstack_pushvar(lex_context_stack,c_context);
    c_context = i;
}
int	lex_context_pop(void)
{
    int a = c_context;
    if( sstack_left(lex_context_stack) > 0 )
	sstack_popvar(lex_context_stack,c_context);
    else
	return 0;
    return a;
}

static int hexdigit2int(char a)
{
    if( a >= '0' && a <= '9' )
	return	a - '0' ;
    if( a >= 'a' && a <= 'f' )
	return	a - 'a'  + 10;
    if( a >= 'A' && a <= 'F' )
	return	a - 'A'  + 10;
    return 0;
}
int sstr_add_stray_expr(sstr* s)
{
    return sstr_add_stray_expr_len(s,sstr_len(s));
}

static int get_stray_char(void)
{
    int a,b,c,x,i;
    c = lex_getc();
    if( isodigit(c) ) {
	/********* octal character constant */
	x = octdigit2int(c);
	i = 0;
	b = lex_getc();
	while ( isodigit(b) && i < 3 && b != EOF ) {
	    x = x << 3;
	    x |= octdigit2int(b);
	    i++;
	    b = lex_getc();
	}
	if( i < 3 && b != EOF )
	    lex_ungetc(b);
	return x;
    }
    if( c == 'x' ) {
 /********* hexadecimal character constant */
	a = lex_getc();
	if( ! isxdigit(a) )
	    return -1;
	x = hexdigit2int((char)a);
	i = 0;
	b = lex_getc();
	while ( isxdigit( b ) && b != EOF  ) {
	    x = x << 4;
	    x |= hexdigit2int((char)b);
	    i++;
	    b = lex_getc();
	}
	if( b != EOF )
	    lex_ungetc(b);
	return x;
    }
    switch( c ) {
    case 'n': return '\n';
    case 't': return '\t';
    case 'v': return '\v';
    case 'b': return '\b';
    case 'r': return '\r';
    case 'f': return '\f';
    case 'a': return '\a';
    default : return c;
    }
    return 0;
}
int sstr_add_stray_expr_len(sstr* s,int len)
{
    int a,b,c,x,i;
    c = lex_getc();
    if( isodigit(c) ) {
	/********* octal character constant */
	x = octdigit2int(c);
	i = 0;
	b = lex_getc();
	while ( isodigit(b) && i < 3 && b != EOF ) {
	    x = x << 3;
	    x |= octdigit2int(b);
	    i++;
	    b = lex_getc();
	}
	if( i < 3 && b != EOF )
	    lex_ungetc(b);
	sstr_addc_len(s,len,(unsigned char)x);
	return 1;
    }
    if( c == 'x' ) {
	/********* hexadecimal character constant */
	a = lex_getc();
	if( ! isxdigit(a) )
	    return -1;
	x = hexdigit2int((char)a);
	i = 0;
	b = lex_getc();
	while ( isxdigit( b ) && b != EOF  ) {
	    x = x << 4;
	    x |= hexdigit2int((char)b);
	    i++;
	    b = lex_getc();
	}
	if( b != EOF )
	    lex_ungetc(b);
	sstr_addc_len(s,len,(char)(x & 0xff));
	return 1;
    }
    switch( c ) {
    case '\r':
	c = lex_getc();
	if( c != '\n' )
	    lex_ungetc(c);
	return 0;
    case '\n':
	return 0;
    case 'n': sstr_addc_len(s,len,'\n');return 1;
    case 't': sstr_addc_len(s,len,'\t');return 1;
    case 'v': sstr_addc_len(s,len,'\v');return 1;
    case 'b': sstr_addc_len(s,len,'\b');return 1;
    case 'r': sstr_addc_len(s,len,'\r');return 1;
    case 'f': sstr_addc_len(s,len,'\f');return 1;
    case 'a': sstr_addc_len(s,len,'\a');return 1;
    default : sstr_addc_len(s,len,(char)c);return 1;
    }
    return -1;
}


xl_str xls_add_stray_expr(xl_str s,int* error)
{
    int a,b,c,x,i;
    if( error ) *error = 0;
    c = lex_getc();
    if( isodigit(c) ) {
	/********* octal character constant */
	x = octdigit2int(c);
	i = 0;
	b = lex_getc();
	while ( isodigit(b) && i < 3 && b != EOF ) {
	    x = x << 3;
	    x |= octdigit2int(b);
	    i++;
	    b = lex_getc();
	}
	if( i < 3 && b != EOF )
	    lex_ungetc(b);
	s = xls_add_char(s,(unsigned char)x);
	return s;
    }
    if( c == 'x' ) {
	/********* hexadecimal character constant */
	a = lex_getc();
	if( ! isxdigit(a) ) {
	    if( error) *error = 1;
	    return s;
	}
	x = hexdigit2int((char)a);
	i = 0;
	b = lex_getc();
	while ( isxdigit( b ) && b != EOF  ) {
	    x = x << 4;
	    x |= hexdigit2int((char)b);
	    i++;
	    b = lex_getc();
	}
	if( b != EOF )
	    lex_ungetc(b);
	s = xls_add_char(s,(unsigned char)(x && 0xff));
	return s;
    }
    switch( c ) {
    case '\r':
	c = lex_getc();
	if( c != '\n' )
	    lex_ungetc(c);
	return s;
    case '\n':
	return s;
    case 'n': c = '\n'; break;
    case 't': c = '\t'; break;
    case 'v': c = '\v'; break;
    case 'b': c = '\b'; break;
    case 'r': c = '\r'; break;
    case 'f': c = '\f'; break;
    case 'a': c = '\a'; break;
    }
    s = xls_add_char(s,(char)c);
    return s;
}


static YYSTYPE tmpval[20];
static int	tmptoken[20];
static int	tmpused = 0;

static int push_tmpval(YYSTYPE* t,int token)
{
    if( tmpused < countof(tmpval) ) {
	tmpval[tmpused] = *t;
	tmptoken[tmpused++] = token;
	return tmpused;
    }
    err_f("internal stack overflow (tmpval)");
    return 0;
}
static int pop_tmpval(YYSTYPE* t)
{
    if( tmpused > 0 ) {
	*t = tmpval[tmpused-1];
	return tmptoken[--tmpused];
    }
    return 0;
}
static int tmpval_used()
{
    return tmpused > 0;
}

#define define_stack_static_simple(name,type,size)		\
static type	name ## _stack_t[size];                         \
static int	name ## _stack_i = (int)0;			\
								\
void	name ## _push(type i)					\
{								\
    if( name ## _stack_i >= countof(name ## _stack_t) -1 )	\
	err_f("internal stack overflow (" #name ")");		\
    name ## _stack_t[name ## _stack_i++] = i;			\
}								\
void	name ## _pushp(const type* i)				\
{								\
    if( name ## _stack_i >= countof(name ## _stack_t) -1 )	\
	err_f("internal stack overflow (" #name ")");		\
    name ## _stack_t[name ## _stack_i++] = *i;			\
}								\
								\
void	name ## _pop(void)					\
{								\
    if ( -- name ## _stack_i < 0 )				\
	err_f("internal stack underflow (" #name ")");		\
}								\
type	name ## _cur(void)					\
{								\
    if( name ## _stack_i > 0 )					\
	return name ## _stack_t[name ## _stack_i-1];		\
    return (type)0;						\
}								\
type	name ## _curp(void)					\
{								\
    if( name ## _stack_i > 0 )					\
	return name ## _stack_t[name ## _stack_i-1];		\
    return (type)0;						\
}

#define declare_stack_static_simple(name,type,size)		\
void	name ## _push(type i);					\
void	name ## _pop(void);					\
type	name ## _cur(void);

define_stack_static_simple(lex_if,int,40);


