/*
* File:		lexer.l
*
* Project:	xmldump
*
* Description:	lexer that parses HTML file
*
* Author Zbigniew Zagorski <longmanz@polbox.com>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2002,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/
%{
#include <string.h>
void	ltc_begin_tag_def(const char* tag);
void	ltc_end_tag_def(const char* tag);
void	ltc_tag_parameter(const char* param_name,const char* param_value);
void	ltc_end_tag(const char*);
void	ltc_text(const char*);
void	ltc_white(const char*);
static  int dtd_depth = 0;
static  char	last_tag[300];
static  int	state_before_comment = 1;
%}
%x IN_TAG
%x IN_DTD_TAG
%x COMMENT
DIGIT	[0-9]
ID	[^ <>\t\r\n]+
WHITE	[ \t\r\n]
PID	[^ \t\r\n<>]+
TEXT	([^ \">\t\r\n])+
QUOTED_TEXT (\"[^\"]*\")+

PARAM	[A-Za-z0-9_.-]+(=(\"[^\"]*\"|[^ \">\t\r\n]+))?
DTD_PID ![^ \t\r\n<>]+
%%

\<!--[^ \t\r\n]* {
		    state_before_comment = YYSTATE;
		    BEGIN(COMMENT);
		}
<COMMENT>-->	{
		    BEGIN(state_before_comment);
		}
<COMMENT>.	/* ignore */

\<{DTD_PID}	{
		    ltc_begin_tag_def(lt_text+1);
		    dtd_depth++;
		    BEGIN(IN_DTD_TAG);
		}
\<{PID}		{
		    ltc_begin_tag_def(lt_text+1);
		    strncpy(last_tag,lt_text+1,sizeof(last_tag));
		    last_tag[sizeof(last_tag)-1] = '\0';
		    BEGIN(IN_TAG);
		}

<IN_TAG>\??\>	{
		    ltc_end_tag_def(last_tag);
		    if( lt_text[0] == '?' && last_tag[0] == '?' )
			ltc_end_tag(last_tag);
		    BEGIN(INITIAL);
		}
<IN_TAG>\/\>	{
		    ltc_end_tag_def(last_tag);
		    ltc_end_tag(last_tag);
		    BEGIN(INITIAL);
		}

<IN_TAG>{PARAM} {
		    char* pv = strchr(lt_text,'=');
		    if( pv == NULL ) {
			ltc_tag_parameter(lt_text,"");
		    } else {
			*pv = 0;
			pv++;
			if( *pv == '\"') {
			    char* e = strchr(pv+1,'\"');
			    if( e ) *e = '\0';
			    pv++;
			}
			ltc_tag_parameter(lt_text,pv);
		    }
		}


<IN_DTD_TAG>\<{DTD_PID}
		{
		    ltc_begin_tag_def(lt_text+1);
		    dtd_depth++;
		}
<IN_DTD_TAG>\>	{
		    char* x = strchr(lt_text,'>');
		    *x = 0;
		    ltc_end_tag_def("");
		    dtd_depth--;
		    if( dtd_depth <= 0 )
			BEGIN(INITIAL);
		}
<IN_DTD_TAG>{TEXT}  ltc_text(lt_text);
<IN_DTD_TAG>{QUOTED_TEXT} {
	    	    char *pv = lt_text;
		    if( *pv == '\"') {
			char* e = strchr(pv+1,'\"');
			if( e )
			    *e = '\0';
			pv++;
		    }
		    ltc_text(pv);
		}

\<\/{PID}\> 	{
		    char* x = strchr(lt_text,'>');
		    *x = 0;
		    ltc_end_tag(lt_text+2);
		    BEGIN(INITIAL);
		}

<IN_TAG>{WHITE}+	/*ignore*/
<IN_DTD_TAG>{WHITE}+ 	/*ignore*/

[^<\r\n \t]+ 	ltc_text(lt_text);
[\r\n \t]+	ltc_white(lt_text);
%%


