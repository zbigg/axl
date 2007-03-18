/**************************************************\
* File:		grammar.y
*
* Id:           $Id: grammar.y 1.7 04/02/24 01:39:33+01:00 zbigg@marl.(none) $
*
* Project:	axl
*
* Description: parser definition (grammar etc.)
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*
\***************************************************/

%{

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>

#include "parse.h"

#ifdef _MSC_VER
/*
   Yacc generate labels that are not used, disable warning in MSC.
*/
#pragma warning (disable: 4102) // 'yyerrlab' : unreferenced label
#endif

%}

/*
 *    START OF YACC DECLARATIONS
 */
%token	INCLUDE SAFE
%token	FOR WHILE DO EWHILE EFOR
%token	RETURN
%token	EQ NEQ GE LE NF INSTR_IN NIN
%token	OROR ANDAND XORXOR
%token	IF ELSE
%token	SWITCH CASE DEFAULT
%token	WITH
%token	GLOBAL LOCAL NIL
%token	CTX_OP	TOK_ARRAY

%token	SET_ADD SET_SUB SET_MUL SET_DIV SET_ASK XCHG
%token	CHOOSE /* ?: */

%token	BEGIN_TEXT END_TEXT
%token	BEGIN_BLOCK END_BLOCK /* {? ?} */ ECHO
%token	FUNC EFUNC BUILTIN
%token	SET SEQ METHOD RANGE TWO_DOT
%token	NEW INSTR_DELETE COPY TOK_NEW_VAR
%token	ARROW_R ARROW_L /* <- -> */
%token	TOK_BOGUS
%token	PLUS_PLUS MINUS_MINUS
%token  TOK_INIT_EXPR
/* bad tokens */


%union {
    xl_code			code;
    xl_integer		integer;
    xl_float		real;
	xl_symbol*		sym;
    xlp_expr_list	e_list;
    xlp_name_list	n_list;

	int				atom;
    xl_str			text;
    slist*			lp;
    int 			ei;
    int 			oper;
}

%token <real>	TOK_FLOAT TOK_BAD_FLOAT
%token <integer> TOK_INTEGER TOK_BAD_INTEGER
%token <integer> TOK_CHAR TOK_BAD_CHAR
%token <text>	TOK_GARBAGE
%token <text>	TOK_STRING TOK_BAD_STRING
%token <text>	TOK_IDENTIFIER TOK_BAD_IDENTIFIER
%token <sym>	TOK_SYMBOL TOK_SYMBOL_ARRAY


%type <text>	name
%type <code>	input  instr	more_instr more_instr_doc block
%type <code>	expr expr_assign	value_mapping_list unary_value	delete_instr
%type <code>	primary	immediate	value		value_no_unary 

%type <code>	addition multiplication comparision	
%type <code>	alternative conjugation 

%type <code>	case_element primary_lvalue
/*
%type <code>	set	sequence	range
*/
%type <code>	func_decl
%type <text>	func_decl_prolog
%type <code>	parameter case_expr 
%type <e_list>	expr_list expr_list_ne nu_expr_list_ne 

%type <n_list>	param_list param_list_ne func_param_list
%type <code>	obj_constructor 
%type <code>    map_constructor
%type <code>	value_mapping
/*
 *	OPERATORS PRIORITY
 */
%left TOK_IDENTIFIER TOK_REAL NUMER TOK_STRING
%left '{' ',' ';'

%right <oper> ':'
%right <oper> '=' SET_ADD SET_SUB SET_MUL SET_DIV SET_ASK
%right <oper> '?'
%left <oper> OROR
%left <oper> ANDAND
%left <oper> XORXOR
%left <oper> '|'
%left <oper> '^'
%left <oper> '&'
%left <oper> EQ NEQ LE GE '<' '>' INSTR_IN NIN
%right <oper> UNARY
%left <oper> '+' '-'
%left <oper> '*' '/' '%'
%left	NORMAL_OPER
%right <oper> PLUS_PLUS MINUS_MINUS '~' '!'
%left <oper> '.' '(' '[' ']'
%right <oper> FUNC EFUNC COPY

%%

/*
 *	START OF GRAMMAR
 */

input	: xxx_frame_begin program 
		{
			xl_code_global = xlcg_frame(xl_code_global);
			frame_finish();
		}
	;

xxx_frame_begin 
	: /* empty */ { frame_begin(); }
	;
program
	: /* empty */
	| program instr ';'
	    {
		xl_code_global = xlc_join(
			xl_code_global ? 
				xl_code_global : 
				xlc_new() ,
			$2);
	    }
	| program INCLUDE TOK_STRING ';'
	    {
			if( lex_include_file(xls_get($3),0) < 0 ) {
				YYABORT;
			}
	    }
	    
	| program INCLUDE TOK_STRING SAFE ';'
	    {
			lex_include_file(xls_get($3),1);
	    }
	;

more_instr_doc
	: /* empty */
	    {
			int sym = xlsym_search("");
			$$ = xlcg_load_str(xlsym_get_xlstr(sym));
	    }
	| more_instr_doc '%' '=' expr ';'
	    {
		$$ = xlcg_operator(XLOP_ADD,$1,$4);
	    }
	| more_instr_doc '%' INCLUDE TOK_STRING ';'
	    {
		$$ = $1;
		if( lex_include_file(xls_get($4),0) < 0 ) {
			YYABORT;
		}
	    }
	| more_instr_doc '%' INCLUDE TOK_STRING SAFE ';'
	    {
		$$ = $1;
		lex_include_file(xls_get($4),1);
	    }
	| more_instr_doc '$' expr ';'
	    {
		$$ = xlcg_operator(XLOP_ADD,$1,$3);
	    }
	| more_instr_doc TOK_GARBAGE
	    {
		$$ = xlcg_operator(XLOP_ADD,$1,xlcg_load_str($2));
	    }
	| more_instr_doc '%' instr ';'
	    {
		$$ = xlc_join($1,$3);
	    }
	;

more_instr
	: /* empty */
	    {
		$$ = xlc_new();
	    }
	| more_instr instr ';'
	    {
		$$ = xlc_join($1,$2);
	    }
	| more_instr INCLUDE TOK_STRING ';'
	    {
		$$ = $1;
		if( lex_include_file(xls_get($3),0) < 0 )
		    YYABORT;
	    }
	| more_instr INCLUDE TOK_STRING SAFE ';'
	    {
		$$ = $1;
		lex_include_file(xls_get($3),1);
	    }
	;

block
	:	'{'
			{ lex_if_push( 0 ); frame_begin(); }
		more_instr
			{ lex_if_pop();  }
		'}'
	    {
		$$ = xlcg_frame($3);
		frame_finish();
	    }
	;

if_starter : /* empty */
	    {
		lex_if_push( 1 );
	    }
	    ;
if_finish  : /* empty */
	    {
		lex_if_pop();
	    }
	    ;
instr
	:  block
	|  expr {
		$$ = xlcg_flush_after_expr($1);
	    }
/*	| TOK_IDENTIFIER expr_list_ne
	    {
		$$ = xlcg_load_var(XLCG_SCOPE_CURRENT,$1);
		$$ = xlcg_call(xlcg_load_var(XLCG_SCOPE_CURRENT,$1),$2.code,$2.count); 
		axl_free($1);		
	    }
*/
	| primary nu_expr_list_ne
	    {
		$$ = xlcg_flush_after_expr(
			xlcg_call($1,$2.code,$2.count)
		    );
	    }
	| ECHO expr_list_ne
	    {
		$$ = xlcg_echo_expr_n($2.code,$2.count);
	    }
	| DO instr IF '(' expr ')'
	    {
		$$ = xlcg_do_if($2,$5);
	    }
	| IF '(' expr ')' if_starter instr ELSE if_finish instr
	    {
		$$ = xlcg_if_else($3,$6,$9);
	    }
	| IF '(' expr ')' if_starter instr TOK_BOGUS
	    {
		$$ = xlcg_if($3,$6);
		lex_if_pop();
	    }
	| WHILE '(' expr ')' instr
	    {
		$$ = xlcg_while($3,$5);
	    }
	| DO instr WHILE '(' expr ')'
	    {
		$$ = xlcg_do_while($2,$5);
	    }
	| FOR 
	    { frame_begin(); }
	  '(' instr ';' expr ';' instr ')' instr
	    {
		$$ = xlcg_frame(xlcg_for($4,$6,$8,$10));
		frame_finish();
	    }
	| RETURN expr
	    {
		$$ = xlcg_return($2);
	    }
	| SWITCH '(' expr ')'
			    { ++lex_switch; frame_begin(); }
		'{' case_element '}'
	    {
		frame_finish();
		$$ = xlcg_switch($3,$7);
		--lex_switch;
	    }
	| WITH '(' expr ')' instr
	    {
		$$ = xlcg_with($3,$5);
	    }
	| GLOBAL expr
	    {
		$$ = xlcg_make_global($2);
	    }
	| delete_instr
	| error
	    {
		$$ = xlc_new();
	    }
	;
case_element
	: /* empty */
	    {
		$$ = xlc_new();
	    }
	| case_expr ARROW_R instr ';' case_element
	    {
		$$ = xlcg_if_else($1,$3,$5);
	    }
	| DEFAULT ARROW_R instr ';'
	    {
		$$ = $3;
	    }
	;

case_expr
	: expr 		{ $$ = $1; }
	| EQ expr	{ $$ = xlcg_operator(XLOP_EQ,xlcg_case(),$2); }
	| NEQ expr	{ $$ = xlcg_operator(XLOP_EQ,xlcg_case(),$2); }
	| '>' expr	{ $$ = xlcg_operator(XLOP_GREATER,xlcg_case(),$2); }
	| '<' expr	{ $$ = xlcg_operator(XLOP_LOWER,xlcg_case(),$2); }
	| GE  expr	{ $$ = xlcg_operator(XLOP_GREATER_EQUAL,xlcg_case(),$2); }
	| LE  expr	{ $$ = xlcg_operator(XLOP_LOWER_EQUAL,xlcg_case(),$2); }
	| INSTR_IN expr	{ $$ = xlcg_operator(XLOP_IN,xlcg_case(),$2); }
	;


expr
	: expr_assign
	| alternative
	;

expr_assign
	: primary_lvalue SET_ADD expr		{ $$ = xlcg_operator(XLOP_ASSIGN_ADD,$1,$3); }
	| primary_lvalue SET_SUB expr		{ $$ = xlcg_operator(XLOP_ASSIGN_SUB,$1,$3); }
	| primary_lvalue SET_MUL expr		{ $$ = xlcg_operator(XLOP_ASSIGN_MUL,$1,$3); }
	| primary_lvalue SET_DIV expr		{ $$ = xlcg_operator(XLOP_ASSIGN_DIV,$1,$3); }
	| primary_lvalue SET_ASK expr		{ $$ = xlcg_operator(XLOP_ASSIGN_CHK,$1,$3); }
	| primary_lvalue   '='   expr		{ $$ = xlcg_operator(XLOP_ASSIGN,$1,$3);     }
	| alternative '?' expr ':' expr		{ $$ = xlcg_choose($1,$3,$5);	}
	| alternative '?' ':' expr		{ $$ = xlcg_choose($1,NULL,$4); }
	
	| EWHILE '(' expr ')' expr		{ $$ = xlcg_ewhile($3,$5);	}
	| EFOR '(' instr ';' expr ';' instr ')' expr
	    {
		$$ = xlcg_efor($3,$5,$7,$9);
	    }
	;

alternative
	: conjugation OROR alternative	{ $$ = xlcg_operator(XLOP_L_OR,$1,$3); }
	| conjugation
	;
conjugation
	: comparision ANDAND conjugation { $$ = xlcg_operator(XLOP_L_AND,$1,$3); }
	| comparision
	;

comparision
	: addition EQ comparision	{ $$ = xlcg_operator(XLOP_EQ,$1,$3); }
	| addition NEQ comparision	{ $$ = xlcg_operator(XLOP_NEQ,$1,$3); }
	| addition '>' comparision	{ $$ = xlcg_operator(XLOP_GREATER,$1,$3); }
	| addition '<' comparision	{ $$ = xlcg_operator(XLOP_LOWER,$1,$3); }
	| addition GE  comparision	{ $$ = xlcg_operator(XLOP_GREATER_EQUAL,$1,$3); }
	| addition LE  comparision	{ $$ = xlcg_operator(XLOP_LOWER_EQUAL,$1,$3); }
	| addition INSTR_IN comparision	{ $$ = xlcg_operator(XLOP_IN,$1,$3); }
	| addition
	;

addition
	: addition '+' multiplication	{ $$ = xlcg_operator(XLOP_ADD,$1,$3); }
	| addition '-' multiplication	{ $$ = xlcg_operator(XLOP_SUB,$1,$3); }
	| addition '&' multiplication	{ $$ = xlcg_operator(XLOP_B_AND,$1,$3); }
	| addition '|' multiplication	{ $$ = xlcg_operator(XLOP_B_OR,$1,$3); }
	| multiplication
	;

multiplication
	: multiplication '*' value	{ $$ = xlcg_operator(XLOP_MUL,$1,$3); }
	| multiplication '/' value	{ $$ = xlcg_operator(XLOP_DIV,$1,$3); }
	| multiplication '%' value	{ $$ = xlcg_operator(XLOP_MOD,$1,$3); }
	| value
	;

unary_value
	:  '!' value			{ $$ = xlcg_operator(XLOP_L_NOT,NULL,$2); }
	|  '~' value			{ $$ = xlcg_operator(XLOP_B_NOT,NULL,$2); }
	|  '-' value	    %prec UNARY { $$ = xlcg_operator(XLOP_SUB,NULL,$2); }
	|  PLUS_PLUS value  %prec UNARY { $$ = xlcg_operator(XLOP_PLUSPLUS,NULL,$2); }
	|  MINUS_MINUS value %prec UNARY { $$ = xlcg_operator(XLOP_MINUSMINUS,NULL,$2); }
	|  COPY value			{ $$ = xlcg_copy_object($2); }
	;

immediate 
	: TOK_FLOAT			{ $$ = xlcg_load_float($1); }
	| TOK_STRING		{ $$ = xlcg_load_str($1); }
	| TOK_INTEGER		{ $$ = xlcg_load_integer($1); }
	| TOK_CHAR			{ $$ = xlcg_load_char($1); }
	| TOK_BAD_STRING	{ $$ = xlcg_load_str(xls_new("")); }	    
	| TOK_BAD_INTEGER	{ $$ = xlcg_load_integer(0); }
	| TOK_BAD_FLOAT		{ $$ = xlcg_load_float(0.0); }
	| TOK_BAD_CHAR		{ $$ = xlcg_load_char('\0'); }
	;
obj_constructor
	: NEW map_constructor		{ $$ = $2; }
	| NEW name			{ $$ = xlcg_new_object($2); }
	| NEW				{ $$ = xlcg_copy_object(NULL); }
	;

map_constructor
	:	'{' value_mapping_list '}'
		{ $$ = xlcg_create_map($2); } 
	;

value_mapping_list
	: value_mapping
	| value_mapping_list ',' value_mapping
		{	$$ = xlcg_join_mapping($1,$3);	 }
	;
value_mapping
	: name ARROW_R expr
		{	$$ = xlcg_create_mapping($1,$3); }
	;
value	: unary_value	%prec UNARY
	| primary	%prec UNARY
	;

value_no_unary : primary
	;

delete_instr
	: INSTR_DELETE name
	    { $$ = xlcg_delete_var($2);  } 
	| INSTR_DELETE primary '[' expr ']' 
	    { $$ = xlcg_delete_index($2,$4); } 
	| INSTR_DELETE primary '.' name 
	    { $$ = xlcg_delete_member($2,$4); } 
	;

primary_lvalue
	: TOK_NEW_VAR name		{ $$ = xlcg_load_var(xl_true,$2); }
	| TOK_IDENTIFIER 		{ $$ = xlcg_load_var(xl_false,$1); }
	| TOK_SYMBOL	{ 
			    if( $1->size == 1 ) $$ = xlcg_load_sym($1);
			    else {
				yyerror("array not expected here !");
				YYABORT;
			    }
			}
	| '(' expr ')'			{ $$ = $2; }
	| primary '[' expr ']'		{ $$ = xlcg_operator(XLOP_INDEX,$1,$3); }
	| primary '.' name		{ $$ = xlcg_load_var_rec($1,$3); }
	| primary PLUS_PLUS		{ $$ = xlcg_operator(XLOP_PLUSPLUS,$1,NULL); }
	| primary MINUS_MINUS		{ $$ = xlcg_operator(XLOP_MINUSMINUS,$1,NULL); }
	| primary '(' expr_list ')'	{ $$ = xlcg_call($1,$3.code,$3.count); }
	;
	
primary
	: immediate
	| obj_constructor
	| primary_lvalue
	
	| CASE				{ $$ = xlcg_case(); }
	|   BEGIN_TEXT
					    { lex_if_push( 0 ); }
	    more_instr_doc
					    { lex_if_pop(); }
	    END_TEXT			{ $$ = $3; }

	| func_decl
	| BUILTIN TOK_STRING			{ $$ = xlcg_load_builtin($2,0); }
	| BUILTIN TOK_STRING SAFE		{ $$ = xlcg_load_builtin($2,1); }

	;

func_decl
	: func_decl_prolog func_param_list block
	    { 
			xl_code body = xlcg_frame($3);
			frame_finish(); /* finish arguments frame */

			$$ = xlcg_create_func($1,body, & ($2) ); 
	    }
	| func_decl_prolog func_param_list ARROW_R '(' 
	    { 
		frame_begin();
	    }
	  expr ')' 
	    { 
			xl_code body = xlcg_frame(xlcg_return($6));
			frame_finish(); /* finish internal function frame */
			frame_finish(); /* finish arguments frame */

			$$ = xlcg_create_func($1,body, & ($2)); 
	    }
	| func_decl_prolog func_param_list 
	  ARROW_R 
	  BEGIN_TEXT  
	    {
		frame_begin();
		lex_if_push( 0 ); 
	     }
	  more_instr_doc
		{ lex_if_pop(); }
	  END_TEXT			
	    { 
			xl_code body = xlcg_frame(xlcg_return($6));
			frame_finish(); /* finish internal function frame */
			frame_finish(); /* finish arguments frame */
			$$ = xlcg_create_func($1,body , & ($2)); 
	    }
	;
func_decl_prolog
	: FUNC			{	$$ = NULL;	}
	| FUNC name		{	$$ = $2;	}
	;
	
func_param_list
	: '(' param_list ')' {
		frame_begin();
		frame_add_params(& $2);
		frame_cur()->size = $2.count + xl_first_frame_in_functio_offset;
		frame_cur()->bound = xl_true;
		$$ = $2;
		}
	;

parameter
	: expr		{ $$ = xlcg_dereference($1); }
	| '&' expr	{ $$ = $2; }
	;
	    
nu_expr_list_ne
	: value_no_unary
	    {
		$$.code = $1;
		$$.count = 1;
	    }
	| nu_expr_list_ne ',' parameter
	    {
		$$.code = xlc_join ($1.code,$3);
		$$.count = $1.count + 1;
	    }
	;
	
expr_list
	: /* empty */
	    {
		$$.code = xlc_new();
		$$.count = 0;
	    }
	| expr_list_ne
	    {
		$$ = $1;
	    }
	;

expr_list_ne
	: parameter
	    {
		$$.code = $1;
		$$.count = 1;
	    }
	| parameter ',' expr_list_ne
	    {
		$$.code = xlc_join ($1,$3.code);
		$$.count = $3.count + 1;
	    }
	;

param_list
	: /* empty */
	    {
		$$.names = slist_new();
		$$.values = slist_new();
		$$.count = 0;
	    }
	| param_list_ne
	    { $$ = $1; }
	;

name	
	: TOK_SYMBOL			{ $$ = $1->name; }
	| TOK_SYMBOL_ARRAY		{ $$ = $1->name; }
	| TOK_IDENTIFIER		{ $$ = $1; }
	;
	
param_list_ne
	: name
	    { 
		$$.names = slist_new();
		$$.values = slist_new();
		$$.count = 1;
		slist_add($$.names,(void*)$1);
		slist_add($$.values,(void*)xlcg_load_nil());
	    }
	| name '=' expr
	    {
		$$.names = slist_new();
		$$.values = slist_new();
		$$.count = 1;
		slist_add($$.names,(void*)$1);
		slist_add($$.values,(void*)$3);
	    }
	| name ',' param_list_ne
	    {
		if( !$3.names ) {
		    $$.names = slist_new();
		    $$.count = 0;
		} else
		    $$ = $3;
		slist_add($$.names,(void*)$1);
		slist_add($$.values,xlcg_load_nil());
		$$.count++;
	    }
	| name '=' expr ',' param_list_ne
	    {
		if( ! $5.names ) {
		    $$.names = slist_new();
		    $$.count = 0;
		} else
		    $$ = $5;
		slist_add($$.names,(void*)$1);
		slist_add($$.values,(void*)$3);
		$$.count++;
	    }
	;

%%
