/*
* File: 	debug.c
*
* Id:		$Id: dump.c 1.4 04/05/22 20:11:42+02:00 zbigg@veinard.dom $
*
* Project:	axl standard library
*
* Description:	debug module for XL
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "zcompat/zmod.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "sstr.h"
#include "axl/axl.h"

#include "decl.h"

static int	_dump_value(const char* name,xl_value v);

XLF_EDECL(dump_ctx)
{
    xl_context c = thread->var;
    int i = 1;
    while( c ) {
	sbtree* t = c->id;
	zprintf("XLDBG context begin (%i) 0x%08x:(nf:0x%08x,n:0x%08x) ",i,c,c->next_find_ctx,c->prev_ctx);
	if( c == thread->var_global )
	    zprintf("GLOBAL ");
	if( c->this_ref )
	    zprintf("ref:(0x%08x) ",c->this_ref);
	zprintf("{\n");
	if( t ) {
	    SBTI_ENUM_BEGIN(t,value,key,xl_value)
		_dump_value(key,value);
	    SBTI_ENUM_END;
	}
	zprintf("\n}\nXLDBG context end (%i)\n",i);
	c = c->prev_ctx;
	i++;
    }
    xlv_set_int(ret,0);
    return 0;
}

XLF_EDECL(dump_object)
{
    const char* name = xlv_str(argv);
    xl_object o = (xl_object)xlv_deref(argv+1);
    if( !xlo_is_obj(o) ) {
	zprintf("XLDBG %s: not object(%i)\n",name,o->type);
	xlv_set_int(ret,0);
	return 0;
    }
    zprintf("XLDBG begin object (%s) {\n",name);
    if(  o->map ) {
	SBTI_ENUM_BEGIN(o->map,value,key,xl_value)
	    _dump_value(key,value);
	SBTI_ENUM_END;
    }
    zprintf("\n}\nXLDBG end object (%s)\n",name);
    xlv_set_int(ret,0);
    return 0;
}

static int	_dump_value(const char* name,xl_value v)
{
    char vbuf[40];
    const char* vstr = xlv_str(v);
    if( vstr ) {
	int vlen = strlen(vstr);
	strncpy(vbuf,vstr,sizeof(vbuf)-5);
	vbuf[sizeof(vbuf)-5] = '\0';
	if( vlen > sizeof(vbuf)-4 )
	    strcat(vbuf,"...");
    } else if( v->type == XLV_REF )
	sprintf(vbuf,"0x%x",(unsigned int) v->v.ref.p);
    else
	vbuf[0] = '\0';

    zprintf("XLDBG %s: %s(%i) %s\t = '%s'\n",name,
	      v->type == XLV_STR    ? "str"
	    : v->type == XLV_INT    ? "int"
	    : v->type == XLV_FLOAT  ? "flt"
	    : v->type == XLV_REF    ? "ref"
	    : v->type == XLV_RES_PTR ? "ptr"
	    : xlv_is_obj(v)	     ? "obj"
	    : v->type == XLV_FUNC_LABEL ? "fbl"
	    : v->type == XLV_FUNC_BUILTIN ? "fnb"
	    : v->type == XLV_FUNC_IP ? "fni"
	    : v->type == XLV_USER_VAL   ? "usr"
	    : "NIL",v->type,
	    v->type == XLV_REF ? ( v->v.ref.p == NULL ? "NULL" : " OK ") : "    ",
	    vbuf
	);
    return 0;
}
XLF_EDECL(dump_value)
{
    const char* name = xlv_str(argv);
    xl_value v = argv+1;
    _dump_value(name,v);
    xlv_set_int(ret,0);
    return 0;
}
XLF_EDECL(dump_target)
{
    const char* name = xlv_str(argv);
    xl_value v = xlv_deref(argv+1);
    _dump_value(name,v);
    xlv_set_int(ret,0);
    return 0;
}
