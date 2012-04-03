/*
* Header:	xlstr.h
*
* Id:           $Id: xlstr.h 1.3 03/11/27 21:10:21+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	xl string definition
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

#ifndef __xlstr_h_
#define __xlstr_h_

#define INC_XLSTR
#include <string.h>
#ifndef INC_ZDEFS
#include "zcompat/zdefs.h"
#endif

/* #define WITHOUT_ZPIO*/

#ifndef WITHOT_ZPIO
#ifndef INC_ZPIO
#include "zcompat/zpio.h"
#endif
#endif

/* C Extern Identifiers. */
#ifdef __cplusplus
extern "C" {
#endif

struct _xl_str {
    MDMF_DEF
    int		ref_cnt;
    size_t	capacity;
    int		length;
    char	first[10];
};
typedef struct	_xl_str	 xl_str_t;

typedef	struct	_xl_str* xl_str;

typedef	const struct	 _xl_str* const_xl_str;

extern char* xl_str_empty;

extern xl_str	xls_ref(const xl_str_t* s);
extern void	xls_free(xl_str a);
extern xl_str   xls_alloc(unsigned int len);
extern xl_str   xls_realloc(xl_str s,unsigned int new_len);

extern xl_str	xls_new(const char* s);
extern xl_str	xls_newn(const char* s,unsigned int len);
extern xl_str	xls_new_xls(const xl_str_t* s);
/*
    xls_cmp

    Compare strings. Bevaviour like strcmp().
*/
extern int	xls_cmp(const xl_str_t* a, const xl_str_t* b);

/*
    xls_eq

    Check if strings are equal. Optimized for hash tables where only
    equal/not equal condition is checked.
*/
extern int      xls_eq(const xl_str_t* a, const xl_str_t* b);
/** Set contens.
*/
extern xl_str	xls_set 	(xl_str dest,const char* src);
extern xl_str	xls_setn	(xl_str dest,const char* src,unsigned int len);
extern xl_str	xls_set_xls	(xl_str dest,const xl_str_t* src);

extern xl_str	xls_cat(xl_str dest, ...);
extern xl_str	xls_catv(xl_str dest, va_list ap);

/** Concatenate with a string with known length.
*/
xl_str	xls_addn 	(xl_str dest,const char* src,unsigned int len);
/** Concatenate with a string.
*/
extern xl_str	xls_add 	(xl_str dest,const char* src);

/** Concatenate with a "xls" string.
*/
extern xl_str	xls_add_xls	(xl_str dest,const xl_str_t* src);

/** Add a char to the string.
*/
extern xl_str	xls_add_char	(xl_str dest,char src);
/** Add a integer to the string.
*/
extern xl_str	xls_add_int	(xl_str dest,int i,const char* fmt);
/** Add a float to the string.
*/
extern xl_str	xls_add_float	(xl_str dest,double f,const char* fmt);

#ifndef WITHOUT_ZPIO
extern int	xls_bwrite	(ZSTREAM f,const xl_str_t* src);
extern xl_str	xls_bread	(ZSTREAM f);
#endif

extern xl_str	xls_erase_blank (xl_str src);

xl_str	xlsprintf(xl_str s,const char* fmt, ...);
xl_str	xlsvprintf(xl_str s,const char* fmt, va_list ap);
/*
extern	void*	axl_malloc(unsigned int);
extern	void	axl_free(void*);
*/
#if !defined(HAVE_INLINE) && !defined(__cplusplus)
#define _xls_get( a )	((a)->first)
#define _xls_str( a )	((a)->first)
#define _xls_ptr( a )	((a)->first)
#define _xls_len( a )	((a)->length)
#define _xls_empty( a ) (xls_str(a)[0] == '\0' )
#define _xls_cap( a )	((a)->capacity)


#define xls_get( a ) (a != NULL ? _xls_get(a) : NULL )
#define xls_str( a ) (a != NULL ? _xls_str(a) : xl_str_empty )
#define xls_ptr( a ) (a != NULL ? _xls_ptr(a) : NULL )
#define xls_len( a ) (a != NULL ? _xls_len(a) : 0 )
#define xls_empty( a ) (a != NULL ?  _xls_empty(a) : 1 )
#define xls_cap( a ) (a != NULL ? _xls_cap(a) : 0 )




#else

static inline char*	_xls_get(const xl_str_t* a)
{
    return (char*)a->first;
}

static inline char*	_xls_str(const xl_str_t* a)
{
    return (char*)a->first;
}
static inline int	_xls_empty(const xl_str_t* a)
{
    return _xls_str(a)[0] == '\0';
}
static inline char*	_xls_ptr(const xl_str_t* a)
{
    return (char*)a->first;
}
static inline int	_xls_len(const xl_str_t* a)
{
    return a->length;
}
static inline int	_xls_cap(const xl_str_t* a)
{
    return a->capacity;
}

inline static char*	xls_get(const xl_str_t* a)
{
    return (a != NULL ? (char*)((a)->first) : NULL );
}

static inline char*	xls_str(const xl_str_t* a)
{
    return (a != NULL ? (char*)((a)->first) : xl_str_empty );
}
static inline int	xls_empty(const xl_str_t* a)
{
    return (xls_str(a)[0] == '\0' );
}
static inline char*	xls_ptr(const xl_str_t* a)
{
    return (a != NULL ? (char*)((a)->first) : NULL );
}
static inline int	xls_len(const xl_str_t* a)
{
    return (a != NULL ? (a)->length : 0 );
}
static inline int	xls_cap(const xl_str_t* a)
{
    return (a != NULL ? (a)->capacity : 0 );
}


#endif


#ifdef __cplusplus
}
#endif

#endif /* force single include of file */
