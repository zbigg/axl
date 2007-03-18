/*
* File:		xmldump.c
*
* Project:	Wmirror
*
* Description:	Analize structure of HTML file
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 2002, 
*   All rights reserved, and is distributed as free software under the 
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>

extern FILE *yyin;

int yywrap(void)
{
    return 1;
}
int yylex(void);

int main(int argc, char **argv)
{
    ++argv, --argc;		/* skip over program name */
    if (argc > 0)
	yyin = fopen(argv[0], "r");
    else
	yyin = stdin;
    yylex();
    return 0;
}

typedef struct tag_param_t {
    char*		name;
    char*		value;
} tag_param_t;

typedef struct tag_t {
    int			type;
    const char*		name;		/* or body if type=TAG_TEXT */
    tag_param_t*	param;
    int			nparams;
    struct tag_t*	sub;
    int			nsubs;
} tag_t;

void process_tag(tag_t* tag);
void lt_begin_tag_def(const char *tag)
{
    printf("t: %s\n", tag);
}

void lt_end_tag_def(const char *tag)
{
    printf("y: %s\n", tag);
}

void lt_tag_parameter(const char *param_name, const char *param_value)
{
    printf("p: %s=%s\n", param_name, param_value);
}

void lt_end_tag(const char *tag)
{
    printf("e: %s\n", tag);
}

void lt_text(const char *text)
{
    printf("b: %s\n", text);
}

void lt_white(const char *text)
{
    printf("b: %s\n", text);
}

void lt_process_tag(tag_t * tag)
{
}
