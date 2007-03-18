/*
* File: 	builtin.c
*
* Id:           $Id: builtin.c 1.4 04/01/28 21:05:19+01:00 zbigg@nowy.dom $
*
* Project:	axl
*
* Description:	builtin functions implementation and zmod interface
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

#include "dconfig.h"

#include <stdlib.h>
#include <string.h>
#include "zcompat/zprintf.h"

#include "zcompat/zmod.h"
#include "axl/axl.h"

#include "axl_i.h"
#include "tools.h"

#if defined ZSYS_WIN || defined ZSYS_MSDOS
#define AXL_COMPILED_LIBRARY_PATH NULL
#endif

#ifndef AXL_COMPILED_LIBRARY_PATH
#	ifdef PACKAGE_PKG_LIBDIR
#		define AXL_COMPILED_LIBRARY_PATH PACKAGE_PKG_LIBDIR
#	else
#		define AXL_COMPILED_LIBRARY_PATH "/usr/include/axl"
#	endif
#endif

static char	builtin_error[255] = "(success)";
extern int	xl_builtin_is_ok;

static int	xl_builtin_init(void);
static void	set_lib_path(void);
/** PUBLIC: return DLL error */
const char*	xl_load_last_error(void)
{
    return builtin_error;
}

/** PUBLIC: Get address of builtin symbol */
xl_builtin_func xlb_load(const char* module,const char* name)
{
    return (xl_builtin_func)zmod_lock_sym(module,name);
}
/** PUBLIC: Release builtin symbol */
int		xlb_free(const char* module,const char* name)
{
    return zmod_free_sym(module,name);
}
/** PUBLIC: Fill v with proper builtin function data */
int	xl_load_func_builtin(xl_value v,const char* name)
{
    char buf[200];
    char* sym;
    char* mod;
    xl_builtin_func f;
    if( xl_builtin_is_ok == 0 )
	if( !xl_builtin_init() ) {
	    strcpy(builtin_error,"can't initialize DLL subsystem");
	    return -1;
	}
    if( (sym = strchr(name,':')) != NULL ) {
	strncpy(buf,name,sym-name);
	buf[sym-name] = 0;
	mod = buf;
	sym = sym+1;
    } else {
	return -1;
    }
    set_lib_path();
    f = xlb_load(mod,sym);
    if( f != NULL )
	return xlv_set_func_builtin(v,f,name);
    else
	snprintf(builtin_error,512,"can't load %s module: %s",mod,zmod_error());
    return -1;
}

/** PUBLIC: Release builtin function from v. */
int		xl_free_func_builtin(xl_value v)
{
    char* name;
    if( !v ) return 0;
    name = v->v.func_builtin.name;
    if( name != NULL )
	axl_free(name);
    v->v.func_builtin.name = NULL;
    v->v.func_builtin.address = NULL;
    return 0;
    
/*
    if( (sym = strchr(name,':')) != NULL ) {
	strncpy(buf,name,sym-name);
	buf[sym-name] = 0;
	mod = buf;
	sym = sym+1;
    } else {
	return -1;
    }
    {
	int r = xlb_free(mod,sym);
	axl_free(name);
	return r;
    }
*/
}

/** PUBLIC: Set value to builtin function pointer. */
int	xlv_set_func_builtin(xl_value v,xl_builtin_func func,const char* name)
{
    v->type = XLV_FUNC_BUILTIN;
    v->v.func_builtin.address = func;
    v->v.func_builtin.name = (name != NULL) ? axl_strdup(name) : NULL;
    return 0;
}

/** PRIVATE: */
static int _lib_path_set = 0;
static int _zmod_path_is_mine = 0;
static void	set_lib_path(void)
{
    if( !_lib_path_set ) {
	char *path,
	     *env_axl_lib_path,
	     *cfg_axl_lib_path;
	static char buf[2000];
	env_axl_lib_path = getenv("AXL_LIBRARY_PATH");
	cfg_axl_lib_path = (char*)xl_cfg_get_str("pkglibdir");
	if( !env_axl_lib_path ) {
	    path = zsaprintf(NULL,0,
		"%s%s%s",
		cfg_axl_lib_path,Z_PATH_SEPARATOR_T,
		zmod_set_find_path(NULL));
	} else {
	    path = zsaprintf(NULL,0,
		"%s%s%s%s%s",
		env_axl_lib_path, Z_PATH_SEPARATOR_T,
		cfg_axl_lib_path,Z_PATH_SEPARATOR_T,
		zmod_set_find_path(NULL));
	}
	strncpy(buf,path,sizeof(buf));
	buf[sizeof(buf)-1] = 0;
	zmod_set_find_path(buf);
	zpio_free(path);
	_zmod_path_is_mine = 1;
	_lib_path_set = 1;
    }
}

const char* xl_set_library_path(const char* p)
{
    NOT_READY;
}

int xl_push_library_path(const char* p)
{
    return zmod_push_find_path((char*)p);
}

char* xl_pop_library_path()
{
    return zmod_pop_find_path();
}

#ifdef ZMOD_DLX
#include "axl.exp"
static int xl_builtin_is_ok = 0;
static int xl_builtin_init(void)
{
    DLXImport(axl_export_table);
    return xl_builtin_is_ok = 1;
}
#else
static int xl_builtin_is_ok = 1;
static int xl_builtin_init(void)
{
    return 1;
}
#endif

