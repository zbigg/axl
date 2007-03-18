/*
* File:         xlmap.h
*
* Id:           $Id: xlmap.h 1.3 03/11/27 21:10:21+01:00 zbigg@zbigg.(none) $
*
* Project:      libxl
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*   This program is Copyright(c) Zbigniew Zagorski 2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/
#ifndef __xlmap_h_
#define __xlmap_h_

#ifndef INC_ZTOOLS
#include "zcompat/ztools.h"
#endif
#ifndef INC_XLSTR
#include "axl/xlstr.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef smap	xlmap;
typedef smapi	xlmi;
typedef smapi_t	xlmi_t;

/*struct	_xl_value;
typedef struct _xl_value	xl_value_t;
*/
xlmap*	    xlmap_new(void (*f)(void*));
xlmap*	    xlmap_init(xlmap* m,void (*f)(void*));
void	    xlmap_free(xlmap* b);
void	    xlmap_done(xlmap* b);

int	    xlmap_add(xlmap* bt,const_xl_str s,void* data);

void*       xlmap_del(xlmap* bt,const_xl_str s);
xlmi	    xlmap_del_i(xlmap* bt,xlmi i);

void*       xlmap_find(xlmap* bt,const_xl_str key);
xlmi	    xlmap_find_i(xlmap* bt,const_xl_str key,xlmi i);
void*       xlmap_find_str(xlmap* bt,const char* str_key);
xlmi        xlmap_find_str_i(xlmap* bt,const char*  str_key,xlmi i);

int	    xlmap_delete(xlmap* bt);

xlmi	    xlmap_begin(xlmap* bt,xlmi i);
xlmi	    xlmap_end(xlmap* bt,xlmi i);

void	    xlmi_free(xlmi i);
xlmi	    xlmi_inc(xlmi i);
xlmi	    xlmi_dec(xlmi i);
int	    xlmi_eq(xlmi a,xlmi b);
void        xlmi_set(xlmi i, void* item);
void*       xlmi_get(xlmi i);
xl_str	    xlmi_key(xlmi i);
int	    xlmap_show(xlmap* bt );

#ifdef __cplusplus
};
#endif

#endif
