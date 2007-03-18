 /*
   * File:	xlasm.c
   *
   * Id:	$Id: xlasm.c 1.5 04/01/28 21:05:20+01:00 zbigg@nowy.dom $
   *
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
#include "zcompat/zpio.h"

#include <stdlib.h>
#include <string.h>

#include "axl/axl.h"
#include "arch.h"
#include "axl_i.h"

/* GLOBALS */

int	    xlasm_put_instr(ZSTREAM s,const xl_instr_t* instr);

int	    xlasm_put_value(ZSTREAM s,const xl_value_t* instr);

static const char* get_label_text(int i);


/* IMPLEMENTATION */

static struct instr_name {
    char* name;
    char* fmt;
    int   par;
    char* comment;
} i_data[] =
{
    { "LABEL",	    0,  1, "" },
    { "EXIT",	    0,  1, "wyjscie z programu" },
    { "LOAD IMM",   0,  1, "zaladuj %s na stos" },
    { "LOAD VAR",   0,  1, "zaladuj referencje do zmiennej %s na stos" },
    { "INDEX",	    0,  0, "zaladuj referencje do elementu [-1] obiektu [-2]" },
    { "FIELD",	    0,  1, "zaladuj referencje do pola %s obiektu [-1]" },
    { "STORE",	    0,  0, "zapisz [-2] do zmiennej wskazywanej przez [-1]" },
    { "STORE_CAST", 0,  1, "" },
    { "OPER_BI",    0,  1, "operator binarny na [-2] i [-1] zostawia w [-1]" },
    { "OPER_UN",    0,  1, "operator unarny(?) na [-1]" },
    { "!!!",	    0,  1, "" },
    { "!!!",	    0,  1, "" },
    { "!!!",	    0,  1, "" },
    { "LOG AND",    0,  1, "logiczne and [-1] = [-2] && [-1] (jako integer)" },
    { "LOG OR",	    0,  1, "logiczne or  [-1] = [-2] || [-1] (jako integer)"},
    { "LOG XOR",    0,  1, "logiczne xor [-1] = [-2] ^^ [-1] (jako integer)"},
    { "LOG NEG",    0,  1, "logiczne not [-1] = ! [-1] (jako integer)"},
    { "BIN AND",    0,  1, "binarne and [-1] = [-2] & [-1] (jako integer)"},
    { "BIN OR",	    0,  1, "binarne or  [-1] = [-2] | [-1] (jako integer)"},
    { "BIN XOR",    0,  1, "binarne xor [-1] = [-2] ^ [-1] (jako integer)"},
    { "BIN NEG",    0,  1, "binarne neg [-1] = ~ [-1] (jako integer)"},
    { "EQUAL",	    0,  0, "porownanie: [-1] = [-2] == [-1]"    },
    { "NOT EQUAL",  0,  0, "porownanie: [-1] = [-2] != [-1]"    },
    { "LESS",	    0,	0, "porownanie: [-1] = [-2] < [-1]" },
    { "GREATER",    0,  0, "porownanie: [-1] = [-2]  > [-1]"    },
    { "LESS EQUAL", 0,  0, "porownanie: [-1] = [-2] <= [-1]"    },
    { "GREATER EQUAL", 0,  0, "porownanie: [-1] = [-2] => [-1]"   },
    { "CMP",	    0,  0, "porownanie: [-1] = [-2] == [-1]"   },
    { "JMP",	    0,  1, "skok bezwarunkowy do %s"            },
    { "JTRUE",	    0,  1, "skok do %s jezeli [-1] jest prawda"   },
    { "JFALSE",	    0,  1, "skok do %s jezeli [-1] jest falsz"   },
    { "DUP",	    0,  1, "zduplikowanie %s razy wartosci z wierzcholka stosu"   },
    { "DROP",	    0,  1, "odrzucenie %s wartosci z wierzcholka stosu"   },
    { "STORE REG",  0,  1, "zapisz w rejestrze %s"              },
    { "CALL",	    0,  1, "wysolaj funkcje [-1-N] z N parametrami gdzie N=%s"   },
    { "RET",	    0,  0, "zwroc wartosc"                      },
    { "MARK",	    0,  0, ""                                   },
    { "RELEASE",    0,  0, ""                                   },  
    { "CHS",	    0,  0, "zmiana znaku w s[-1] (jako integer)"   },
    { "ECHO",	    0,  1, "wypisz %s wartosci ze stosu na stdout"   },
    { "GET",	    0,  1, "nie powinna byæ u¿ywana !!"         },
    { "LOAD REG",   0,  1, "zaladuj wartosc rejestru %s na stos"   },
    { "LOAD FRM",   0,  1, "zaladuj referencje do wartosci wzgledem ramki stosu: %i"   },
    { "COPY",	    0,  1, "utworz lub skopiuj obiekt i pozostaw referencje o niego na stosie"   },
    { "DEREF",	    0,  0, "wyluskaj wartosc ze zmiennej"   },
    { "ENTER",	    0,  1, "wejdz w kontekst z obiektem w [-1]"   },
    { "LEAVE",	    0,  0, "wyjdz z kontekstu"   },
    { "FREE VAR",   0,  1, "skasuj zmienna wskazywana przez [-1]"   },
    { "VALID",	    0,  1, "sprawdz czy wartosc [-1] nie jest nil"   },
    { "BOOL",	    0,  1, "przeksztalc wartosc w [-1] na logiczna"   }
};


char* xlop2str[] = {
    "",
    "+ ",/*XLOP_ADD	    = 1*/
    "- ",/*XLOP_SUB	    = 2*/
    "* ",/*XLOP_MUL	    = 3*/
    "/ ",/*XLOP_DIV	    = 4*/
    "% ",/*XLOP_MOD	    = 5*/
    "= ",/*XLOP_ASSIGN	    = 6*/
    "+=",/*XLOP_ASSIGN_ADD = 7*/
    "-=",/*XLOP_ASSIGN_SUB = 8*/
    "*=",/*XLOP_ASSIGN_MUL = 9*/
    "/=",/*XLOP_ASSIGN_DIV = 10*/
    "&=",/*XLOP_ASSIGN_AND = 11*/
    "|=",/*XLOP_ASSIGN_OR  = 12*/
    "<=",/*XLOP_ASSIGN_SHL = 13*/
    ">=",/*XLOP_ASSIGN_SHR = 14*/
    "==",/*XLOP_EQ	    = 15*/
    "!=",/*XLOP_NEQ	    = 16*/
    "< ",/*XLOP_LOWER	    = 17*/
    "> ",/*XLOP_GREATER    = 18*/
    "<=",/*XLOP_LOWER_EQUAL = 19*/
    ">=",/*XLOP_GREATER_EQUAL = 20*/
    "&&",/*XLOP_L_AND	    = 21*/
    "||",/*XLOP_L_OR	    = 22*/
    "^^",/*XLOP_L_XOR	    = 23*/
    "! ",/*XLOP_L_NOT	    = 24*/
    "& ",/*XLOP_B_AND	    = 25*/
    "| ",/*XLOP_B_OR	    = 26*/
    "^ ",/*XLOP_B_XOR	    = 27*/
    "~ ",/*XLOP_B_NOT	    = 28*/
    "<<",/*XLOP_SHL	    = 29*/
    ">>",/*XLOP_SHR	    = 30*/
    "in",/*XLOP_IN	    = 31*/
    "?=",/*XLOP_ASSIGN_CHK = 32*/
    "[]",/*XLOP_INDEX	    = 33*/
    ". ",/*XLOP_FIELD	    = 34*/
    "++",/*XLOP_PLUSPLUS   = 35*/
    "--",/*XLOP_MINUSMINUS = 36*/
    "- " /*XLOP_CHS	    = 37*/
};
int	    xlasm_put_instr(ZSTREAM s,const xl_instr_t* instr)
{
    int has_param = 1;
    int icode = instr->icode & 0xff;
    int iflags = instr->icode & 0xff00;
    char flagsbuf[] = "  ";
    if( 0 && i_data[icode].comment && *(i_data[icode].comment) ) {
	zfprintf(s,"# ");
	zfprintf(s,i_data[icode].comment,xlv_str(&instr->ival));
	zfprintf(s,"\n");
    }
    if( ZFL_ISSET(iflags,XLI_LEAVE_1) ) 
	flagsbuf[0] = 'L';
    if( ZFL_ISSET(iflags,XLI_MODIFY_1) ) 
	flagsbuf[1] = 'M';
    switch( icode ) {
    case 0:
	zfprintf(s,":%s",xlv_str(&instr->ival));
	has_param = 0;
	break;
    case INSTR_STD_OPER_BI:
    case INSTR_STD_OPER_UN:
	zfprintf(s,"%12s %2s %s ", 
	    i_data[icode].name,
	    xlop2str[instr->ival.v.i],flagsbuf);
	has_param = 0;
	break;
    default:
	zfprintf(s,"%15s %s  ", i_data[icode].name,flagsbuf);
	has_param = i_data[icode].par;
	break;
    }
    if( has_param ) {
	xlasm_put_value(s,& instr->ival);
    }
    zfprintf(s,"\n");
    return 0;
}

int	    xlasm_put_value(ZSTREAM s,const xl_value_t* v)
{
    const char* sv = NULL;
    if( v )
	sv = xlv_str(v);
    switch( v->type ) {
    case XLV_NIL:
	zfprintf(s,"nil");
	break;
    case XLV_INT:
    case XLV_FLOAT:
	zfprintf(s,"%s",sv);
	break;
    case XLV_CHAR:
    case XLV_CHAR_REF:
	zfprintf(s,"\'%s\'", sv);
	break;
    case XLV_STR_BUF:
    case XLV_STR:
	zfprintf(s,"\"%s\"", sv);
	break;
    case XLV_FUNC_IP:
    case XLV_FUNC_LABEL:
	zfprintf(s,"func %s %i", 
	    get_label_text(v->v.func.address.label),
	    v->v.func.params ? v->v.func.params->number : 0 );
	break;
    case XLV_FUNC_BUILTIN:
	zfprintf(s,"func builtin: 0x%08x",v->v.func_builtin.address);
	if( v->v.func_builtin.name )
	    zfprintf(s," : %s",v->v.func_builtin.name);
	break;
    default:
	zfprintf(s,"<internal type>");
    }
    return 0;
}


static char gnl_b[20] = "aaaa";
static char gnl_o[26] = "ngtcjpwohudkqymfxbivlearsz";

static const char* get_label_text(int i)
{
    int  b=0;
    gnl_b[b++] = '$';
    if( i == 0 ) {
	gnl_b[b++] = gnl_o[0];
    } else while( i > 0 ) {
	gnl_b[b++] = gnl_o[i % 26];
	i = i / 26;
    }
    gnl_b[b] = 0;
    return gnl_b;
}

