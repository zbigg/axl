/*
* Header:	axl/xlcore.h
* Id:           $Id: xlcore.h 1.9 04/03/07 21:24:27+01:00 nikt@nowy.dom $
* Project:	axl
*
* Description:	axl definitions
*
* Author Zbigniew Zagorski <zzbigg@o2.pl>
*
* Copyright:
*
*   This program is Copyright(c) Zbigniew Zagorski 1999-2003,
*   All rights reserved, and is distributed as free software under the
*   license in the file "LICENSE", which is included in the distribution.
*/

#ifndef __xlcore_h_
#define __xlcore_h_

#define INC_XLANG

#ifndef INC_ZSYSTYPE
#include "zcompat/zsystype.h"
#endif

#if defined(ZDEV_GCC)
#define HAVE_INLINE
#endif

#ifndef INC_ZTOOLS
#include "zcompat/ztools.h"
#endif

#ifndef INC_ZPIO
#include "zcompat/zpio.h"
#endif

#ifndef INC_ZPIO
#include "zcompat/zprintf.h"
#endif

#ifndef INC_XLSTR
#include "axl/xlstr.h"
#endif

#ifndef INC_XLMAP
#include "axl/xlmap.h"
#endif

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* fixed length integers */
typedef char	xl_int8;
typedef short	xl_int16;
typedef long	xl_int32;

typedef unsigned char	xl_uint8;
typedef unsigned short	xl_uint16;
typedef unsigned long	xl_uint32;

#ifdef ZLONG_LONG
#define XL_HAVE_INT64	1
typedef ZLONG_LONG	xl_int64;
typedef unsigned ZLONG_LONG xl_uint64;
#endif

/* Basic types */
typedef double		xl_float;
typedef signed long	xl_integer;
/* typedef sstr		xl_str;*/
#ifdef __cplusplus
typedef bool		xl_bool;
#define xl_true		true
#define xl_false	false
#else
typedef int		xl_bool;
#define xl_true		1
#define xl_false	0
#endif

/* Virtual machine and processor records */

struct	_xl_code;
struct	_xl_context;
struct	_xl_instr;
struct	_xl_object;
struct	_xl_thread;
struct	_xl_value;
struct	_xl_vm;
struct  _xl_func_params;
struct xlth_stack_t;

typedef struct _xl_code 	xl_code_t;
typedef struct _xl_context	xl_context_t;
typedef struct _xl_instr	xl_instr_t;
typedef struct _xl_object	xl_object_t;
typedef struct _xl_thread	xl_thread_t;
typedef struct _xl_value	xl_value_t;
typedef struct _xl_vm		xl_vm_t;
typedef struct _xl_func_params xl_func_params_t;

typedef xl_code_t*	xl_code;
typedef xl_context_t*	xl_context;
typedef xl_instr_t*	xl_instr;
typedef xl_object_t*	xl_object;
typedef xl_thread_t*	xl_thread;
typedef xl_value_t*	xl_value;
typedef xl_vm_t*	xl_vm;
typedef xl_func_params_t*	xl_func_params;

typedef void (*xl_free_t)(void*);
/**
    xl_value->type
*/

typedef enum xlv_type_e {
    XLV_NIL		= 0,	/* NIL: empty - no value */
/*
    1 - 15 immediate values
	condition:
	    xlv_is_simple
	behaviour:
		* do not  have destructor
		* do not require dereferencing,
*/
    XLV_INT		= 1,	/* Integer : number at least 32 bits */
    XLV_FLOAT		= 2,	/* Float   : floatng point double precsion */

    XLV_RES_PTR		= 3,	/* Resource Ptr: internal used by builtin
                                             functions can't be changed by
					     axl program */
    XLV_CHAR		= 4,
    XLV_STR_BUF		= 5,

		    /* these below are currently unsupported */
    XLV_PINT		= 6,
    XLV_PFLOAT		= 7,
    XLV_PBYTE		= 8,
/*
    16 - 31
	immediate values,
	condition:
	    xlv_need_destroy
	bevaviour:
		* do not require dereferencing,
		* have destructor
*/
    XLV_STR		= 16,	/* String  : ASCII or UTF-8(future) string */
    XLV_FUNC_IP 	= 17,
    XLV_FUNC_LABEL	= 18,
    XLV_FUNC_BUILTIN 	= 19,
/*
    32  - 47
	xlv_need_deref
	simple dereferenced types
		* may be dereferenced or extracted
		* can have destructor
*/
    XLV_USER_VAL	= 32,
    XLV_REF		= 33,
    XLV_STACK   	= 35,
    XLV_CHAR_REF	= 36,
    XLV_ITERATOR	= 37,
    XLV_MREF            = 38,
/*
    48 - 63
	condition:
	    xlv_is_obj
	behaviour:
	    * Have unspecified size known only by type implementation.
	    So they can be only referenced.
	    * Are accesible by xlo interface.
	    * Have destructor.
	    * May be extracted.
*/
    XLV_MAP_OBJ		= 48,
    XLV_RESOURCE	= 49,
    XLV_ARRAY		= 50,
    XLV_USER_OBJ	= 51,

    XLV_LAST		= 52
} xlv_type;

#define XLV_REFERENCE	XLV_REF
#define XLV_INTEGER	XLV_INT
#define XLV_REAL	XLV_FLOAT
#define XLV_STRING	XLV_STR
#define XLV_OBJ 	XLV_OBJECT
#define XLV_FUNCTION	XLV_FUNC_BUILTIN

typedef enum xlv_flags_e {

/** XLVF_STATIC
    Memory object contained in value MUSTN'T
    be deallocated.
*/
    XLVF_STATIC		= 0x01,

/*  XLVF_CODE
    Memory object contained in value belongs to code and
    MUSTN'T be deallocated.
*/
    XLVF_CODE		= 0x02,

/** XLVF_READ_ONLY
    Value can't be changed by xlv_assign().
*/
    XLVF_READ_ONLY	= 0x04,

/** XLVF_NEED_DEREF
    Value has to be dereferenced, by xlv_deref
	v = xlv_deref(v).
*/
    XLVF_NEED_DEREF	= 0x08,

/** XLVF_NEED_EXTRACT
    Value must be obtained from external resource by xlv_extract.
	v = xlv_extract(v, &tmp_val );
*/
    XLVF_NEED_EXTRACT	= 0x10,

/** XLOF_MAP_PUBLIC
    Member map of an object is public and shouldn't be deallocated.
*/
    XLOF_MAP_PUBLIC	= 0x40,
} xlv_flags;

#define SET_DONT_COPY_FLAGS ( XLVF_READ_ONLY | XLVF_STATIC | XLVF_CODE )

typedef int	(* xl_builtin_func)(
    xl_thread,
    int ,	/* argc - parameter count			*/
    xl_value,	/* argv - addres of first parameter		*/
    xl_value	/* ret	- return value. value unspecified	*/
    );

typedef int	    (*xlv_vt_free_t) (xl_value v);
typedef int	    (*xlv_vt_dup_t)  (const xl_value_t* v,xl_value dest);
typedef int	    (*xlv_vt_field_t)(const xl_value_t* v,xl_value dest,const char* field_name);
typedef int	    (*xlv_vt_index_t)(const xl_value_t* v,xl_value dest,const xl_value_t* index);
typedef int	    (*xlv_vt_assign_t)(xl_value v,xl_value src);
typedef xl_value    (*xlv_vt_deref_t)(const xl_value_t* v);
typedef int	    (*xlv_vt_extract_t)(const xl_value_t* v,xl_value dest);
typedef int	    (*xlv_vt_oper_t) (xl_value v,int op_code,const xl_value_t* p2);

struct _xlv_vtable {
    xlv_vt_free_t    free;
    xlv_vt_dup_t     dup;

    xlv_vt_field_t   field;
    xlv_vt_index_t   index;

    xlv_vt_assign_t  assign;
    xlv_vt_deref_t   deref;
    xlv_vt_extract_t extract;

    xlv_vt_oper_t    oper;
};

typedef int	    (*xlo_vt_free_t) (xl_object obj);
typedef int	    (*xlo_vt_free_member_t) (xl_object obj,const char* name);
typedef xl_object   (*xlo_vt_copy_t)  (const xl_object_t* obj);
typedef int	    (*xlo_vt_field_t)(const xl_object_t* obj,const char* field_name,int mode,xl_value dest);
typedef int	    (*xlo_vt_index_t)(const xl_object_t* obj,const xl_value_t* index,int mode,xl_value dest);
typedef int	    (*xlo_vt_extract_t)(const xl_value_t* obj,xl_value dest);
typedef int	    (*xlo_vt_oper_t) (xl_value v,int op_code,const xl_value_t* p2);

typedef	int	    (*xlo_vt_iter_deref_t)(const xl_object_t* obj,xl_value dest,void* position);
typedef	int	    (*xlo_vt_iter_extract_t)(const xl_object_t* obj,xl_value dest,void* position);
typedef	int	    (*xlo_vt_iter_get_key_t)(const xl_object_t* obj,xl_value dest,void* position);
typedef	void*	    (*xlo_vt_iter_first_t)(const xl_object_t* obj);
typedef	void*	    (*xlo_vt_iter_last_t)(const xl_object_t* obj);
typedef	void*	    (*xlo_vt_iter_move_t)(const xl_object_t* obj,void* position,int offset);

typedef int	    (*xlo_vt_member_assign_t)(const xl_object_t* obj,void* position,const xl_value_t* src);
typedef int	    (*xlo_vt_member_extract_t)(const xl_object_t* obj,void* position,xl_value dest);

struct _xlo_vtable {
    char*                       class_name;
    xlo_vt_free_t		free;
    xlo_vt_free_member_t	free_member;

    xlo_vt_copy_t		copy;

    xlo_vt_field_t		field;
    xlo_vt_index_t		index;

    xlo_vt_extract_t		extract;

    xlo_vt_oper_t		oper;

    xlo_vt_iter_deref_t		iter_deref;
    xlo_vt_iter_extract_t	iter_extract;
    xlo_vt_iter_get_key_t	iter_get_key;
    xlo_vt_iter_first_t		iter_first;
    xlo_vt_iter_last_t		iter_last;
    xlo_vt_iter_move_t		iter_move;

    xlo_vt_member_assign_t	member_assign;
    xlo_vt_member_extract_t	member_extract;
};

struct _xlv_vtable;
typedef struct _xlv_vtable xlv_vtable_t;
typedef xlv_vtable_t*	xlv_vtable;

struct _xlo_vtable;
typedef struct _xlo_vtable xlo_vtable_t;
typedef xlo_vtable_t*	xlo_vtable;

typedef enum xlv_type_e xlv_type_t;
struct _xl_value {
    MDMF_DEF
    /* .type and .flags must be at begining of struct */
    xlv_type_t	type;
    xl_int8	flags;
    union {
	/** XLV_MREF : a reference to member in object ! 
			accessed through 
			    obj->mref_extract
			    obj->mref_assign
	*/
	struct {
	    xl_object	obj;
	    void*	pos;
	} mref;
	/** XLV_ITERATOR */
	struct {
	    xl_object	object;
	    void*	position;
	} iter;
	/** XLV_USER_VAL : user type value */
	struct {
	    xlv_vtable	vt;
	    void*	param;
	} user;

	/* XLV_INT   : immediate integer */
	xl_integer	i;
	/** XLV_PINT : pointer to static integer : unimplemented */
	xl_integer*	pi;
	/* XLV_CHAR  : immediate character */
	int		ch;

	/** XLV_CHAR_REF : character in string */
	struct {
	    int		pos;
	    xl_value	parent;
	} chr;

	/** XLV_FLOAT: immediate float */
	xl_float	f;
	/** XLV_PFLOAT: pointer to static float  : unimplemented */
	xl_float*	pf;
	/** XLV_STR  : string */
	xl_str		s;
	/** XLV_STR_BUF : pointer to static string */
	struct {
	    char*	s;
	    size_t	capacity;
	}   ps;

	/** XLV_VECT : vector of values : unimplemented */
	struct {
	    xl_value	ptr;
	    int		cap;
	} vect;
	/** XLV_REF  : reference to other value */
	struct {
	    xl_value parent;
	    xl_value p;
	} ref;
	/** XLV_STACK : value on stack */
	struct {
	    struct xlth_stack_t* stack;
	    unsigned int index;
	} stack;
	/** XLV_RES_PTR: resource pointer */
	struct {
	    void*	pointer;
	    int	      	type;
	} res;
	/* XLV_FUNC_(IP | LABEL) function */
	struct {
	    union {
		xl_uint32 ip;
		xl_uint32 label;
	    } address;
	    xl_func_params params;
	} func;
	/* XLV_FUNC_BUILTIN */
	struct {
	    xl_builtin_func	address;
	    char*		name;
	} func_builtin;
    } v;
};
#define XL_VAL_SIZE	(sizeof (xl_value_t) )

#define	xlv_stack2val(val) ( (val)->v.stack.stack->ptr + (val)->v.stack.index )

struct _xl_func_params {
    MDMF_DEF
    int		number;
    xl_value_t	def[1];
};

#define XL_VALUE_HEADER		    \
    MDMF_DEF			    \
    xlv_type_t	type;		    \
    xl_int8	flags;

#define XL_OBJECT_HEADER	    \
    XL_VALUE_HEADER		    \
    int 	ref_count;	    \
    xlo_vtable	vt;		    \
    xlo_field_desc* fd;		    \
    xlmap*	map;		    \
    void*	param1;		    \
    void*	param2;		    \
    char	name[30];


typedef struct _xlo_field_desc {
    const char*	    name;
    xl_str	    xl_name;

    xlv_type_t	    type;
    xl_int8	    flags;

    void*	    param;
    size_t	    count;

    size_t	    size;
} xlo_field_desc;

struct _xl_object {
    XL_OBJECT_HEADER

    /* private */
};

#define XL_OBJECT( o )  (xlo_is_obj((xl_object)(o)) ? (xl_object)(o) : (xl_object)NULL )
#define XL_VALUE( v )	(xlv_is_simple((xl_value)(v) ? (xl_value)(v) : (xl_value)NULL )

#define XL_OBJ_SIZE	(sizeof (xl_object_t) )

struct _xl_instr {
    xl_uint16	icode;
    xl_value_t	ival;
};
#define XL_INSTR_SIZE	(sizeof (xl_instr_t) )

typedef enum {
    xl_rt_none = 0,
    xl_rt_void,
    xl_rt_imm,
    xl_rt_val,
    xl_rt_ref,
    xl_rt_reg,
    xl_rt_range
} xl_result_type;

#define XL_RT_VOID	((int)xl_rt_void)
#define XL_RT_IMM	((int)xl_rt_imm)

enum {
    XLI_LEAVE_1	   = 1 << 8,
    XLI_MODIFY_1   = 1 << 9,
    XLI_NEED_REF_1 = 1 << 10,
};

struct _xl_code {
    MDMF_DEF
    xlmap*	labels;
    slist*	functions;

    sbuffer*	_buf;
    xl_instr	buf;

    /** Entry point, relative from start */
    int 	entry_point;

    /** Number of last valid instruction */
    int 	last;

    /** Capacity of buffer */
    int 	cap;

    /** Result size */
    int		rs;

    /** Result type */
    xl_result_type rt;

    /** Number of instruction representing result */
    int 	resn;

    /** Value representing result */
    xl_value	res;
};

struct _xl_symbol {
    xl_str  name;
    int     pos;
    int     base;       /* 0 - STACK FRAME BASED */
                        /* 1 - ABSLOUTE STACK BASED */
    int     size;       /* 0 - atom */
                        /* 1 - atom */
    int     actual_size;
    int     dimc;       /* count of dimensions */
    int*    dim;        /* dimensions */
    xl_code index_code;
    int     index_dim;
};
typedef struct _xl_symbol xl_symbol;

void    xlsm_done(xl_symbol* sym);
void    xlsm_free(xl_symbol* sym);
xl_symbol* xlsm_new();
int     xlsm_add_dim(xl_symbol* sym,int dim);
int xlsm_index_begin(xl_symbol* sym, xl_code code);
int xlsm_index_cont(xl_symbol* sym, xl_code code);
xl_code xlsm_index_detach_code(xl_symbol* sym);

struct _xl_context {
    MDMF_DEF
    sbtree*	id;
    int 	id_owner;
    int 	id_rdonly;
    xl_object	this_ref;
    xl_context	prev_ctx;
    xl_context	next_find_ctx;
    int 	ref_count;

    char	debug_name[40];
};

enum {
    XLTHMOD_EXIT	=	0,
    XLTHMOD_RET		=	1
};

struct xlth_stack_t {
    xl_value_t*	    ptr;
    xl_value_t*	    top;
    xl_value_t*	    end;
    unsigned	    itop;
    unsigned	    icapacity;
};

#define XLTHW_THREAD		1
struct xlth_wait_t {
    int			type;
    union {
	/* XLTHW_THREAD */
	xl_thread	thread;

    } what;
    xl_value		result;

};
struct _xl_thread {
    MDMF_DEF
    char*	name;		/* thread name (unused) */
    xl_code	code;		/* executed code */
    xl_context	var_global;	/* global context */
    xl_context	var;		/* current context */
    struct xlth_stack_t stack;	/* execution stack */
    xl_value_t	regs[ 64 ];	/* registers */

    struct xlth_wait_t wait[16];/* wait objects */
    int		wait_count;	/* count of wait objects */

    xl_value_t	ret;		/* returned value */
    int		result_consumed;/* 1 if user called wait on this thread */

    int 	run;		/* determines if thread is under run */
				/* = !(interrupted || finished)   */

    int 	ip;		/* instruction pointer */
    int 	nip;		/* next instruction pointer */

    int		ic;		/* processed instruction counter */
    int 	sleep;		/* sleep time (unused) */
    int 	uid;		/* user id (unused) */

    int 	interrupted;	/* interrupt number or 0 if not interrupted */
    int		finished;	/* 1 if thread is finished */

    int 	debug;
    xl_str	debug_file;	/* line number and file name of currently */
    int 	debug_line;	/* processed instruction */

    int		run_mode;	/* run mode XLVM_RUN_UNTIL_ ... */
};

struct _xl_vm {
    MDMF_DEF
    slist*	threads;
    int 	active_threads;
    int		running_threads;
    xl_thread	current_thread;
    slist*	bsem;
    sli_t	tl_i,
		tl_begin,
		tl_end;
};

#define /* void */	XLVT_FREE(v)	((v)->v.user.vt->free(v))
#define /* xl_value */  XLVT_DUP(v,d)	((v)->v.user.vt->dup(v,d))

#define /* xl_value */	XLVT_INDEX(v,f)	((v)->v.user.vt->index(v,a))
#define /* xl_value */	XLVT_FIELD(v,i)	((v)->v.user.vt->field(v,i))

#define /* int */	XLVT_SET(v,src)	((v)->v.user.vt->set(v,src))
#define /* xl_value */	XLVT_DEREF(v)	((v)->v.user.vt->deref(v))
#define /* int */	XLVT_OPER(v,o,a)((v)->v.user.vt->oper((v),(o),(a)))

#define	/* xl_integer*/ XL_INT(v)	xlv_int(v)
#define /* const char**/XL_STR(v)	xlv_str(v)
#define /* xl_float */	XL_FLOAT(v)	xlv_float(v)

#define /* xl_value */  XL_FIELD(v,f)		xlv_field(v,f)
#define /* xl_integer */XL_FIELD_INT(v,f)	XLV_INT(xlv_field(v,f))
#define /* const char**/XL_FIELD_STR(v,f)	XLV_STR(xlv_field(v,f))
#define /* xl_float */  XL_FIELD_FLOAT(v,f)	XLV_FLOAT(xlv_field(v,f))


enum {
    XLOP_ADD	    = 1,
    XLOP_SUB	    = 2,
    XLOP_MUL	    = 3,
    XLOP_DIV	    = 4,
    XLOP_MOD	    = 5,
    XLOP_ASSIGN	    = 6,
    XLOP_ASSIGN_ADD = 7,
    XLOP_ASSIGN_SUB = 8,
    XLOP_ASSIGN_MUL = 9,
    XLOP_ASSIGN_DIV = 10,
    XLOP_ASSIGN_AND = 11,
    XLOP_ASSIGN_OR  = 12,
    XLOP_ASSIGN_SHL = 13,
    XLOP_ASSIGN_SHR = 14,
    XLOP_EQ	    = 15,
    XLOP_NEQ	    = 16,
    XLOP_LOWER	    = 17,
    XLOP_GREATER    = 18,
    XLOP_LOWER_EQUAL = 19,
    XLOP_GREATER_EQUAL = 20,
    XLOP_L_AND	    = 21,
    XLOP_L_OR	    = 22,
    XLOP_L_XOR	    = 23,
    XLOP_L_NOT	    = 24,
    XLOP_B_AND	    = 25,
    XLOP_B_OR	    = 26,
    XLOP_B_XOR	    = 27,
    XLOP_B_NOT	    = 28,
    XLOP_SHL	    = 29,
    XLOP_SHR	    = 30,
    XLOP_IN	    = 31,
    XLOP_ASSIGN_CHK = 32,
    XLOP_INDEX	    = 33,
    XLOP_FIELD	    = 34,
    XLOP_PLUSPLUS   = 35,
    XLOP_MINUSMINUS = 36,
    XLOP_CHS	    = 37
};

enum {
    XLVF_DONTCARE	= 0,
    XLVF_DEREF		= 1,
    XLVF_REF		= 2,
    XLVF_CREATE		= 4
};

/* C Extern Identifiers. */

int		xl_cfg_get_value(const char* name, xl_value dest);
const char*	xl_cfg_get_str(const char* name);
xl_object	xl_cfg_get_obj(void);

int		xl_init(void);
void		xl_done(void);
void            xl_atexit(void (*f)(void));

enum XLCP_E {
    XLCP_PARSE_OUT,
    XLCP_RUN_OUT,
    XLCP_RUN_IN,
    XLCP_RUN_ERR
};
int             xl_set_ctl_prop(int i, void*);
void*           xl_get_ctl_prop(int i);

xl_code 	xl_parse(ZSTREAM s,int script_type);		/* DOC: xl.c */
xl_code 	xl_fparse(const char* f,int script_type);	/* DOC: xl.c */
xl_code 	xl_strparse(const char* code,int script_type);	/* DOC: xl.c */

int		xl_run(xl_code code,
		    int ip,
		    int run_mode,				/* XL_RUN_xxx */
		    xl_context ctx,
		    xl_value ret,
		    int		argc,
		    xl_value	argv);				/* DOC: xl.c */

#define XL_RUN_SCRIPT	XLVM_RUN_UNTIL_EXIT
#define XL_RUN_FUNCTION	XLVM_RUN_UNTIL_RETURN

int		xl_push_include_path(const char* path);
char*		xl_pop_include_path();
int		xl_push_library_path(const char* path);
char*		xl_pop_library_path();

int	    	xl_try_str2number(const char* s,xl_float* df,xl_integer* di);

xl_float    	xl_str2float(const char* s,int* error);
xl_integer  	xl_str2int(const char* s,int* error);

/*
 * Value.
 */

/* allocation */
xl_value	xlv_new(xl_value o);
xl_value	xlv_vnew(int type, ...);
xl_value	xlv_new_str(const char* s);
xl_value	xlv_new_int(xl_integer i);
xl_value	xlv_new_float(xl_float f);

xl_value	xlv_new_func_builtin(xl_builtin_func adr,const char* name);
xl_value	xlv_new_func_ip(int ip);
xl_value	xlv_new_func_label(int label_nr);

void		xlv_free(xl_value v);
void		xlv_frees(xl_value v);
void		xlv_done(xl_value v);
void		xlv_clear(xl_value v);

int		xlv_assign(xl_value v,xl_value src);

int		xlv_cpy(xl_value dest,const xl_value_t* src);

int		xlv_set(xl_value dest,const xl_value_t* src);
int		xlv_vset(xl_value dest,int type, ...);
int		xlv_vvset(xl_value dest,int type, va_list ap);

int		xlv_mov(xl_value dest,xl_value src);

int		xlv_set_int(xl_value v,xl_integer i);
int		xlv_set_char(xl_value v,int i);
int		xlv_set_float(xl_value v,xl_float f);
int		xlv_set_str(xl_value v,const char* s);
int		xlv_set_strn(xl_value v,const char* s,unsigned int len);
int		xlv_set_ref(xl_value v,xl_value ref,xl_value parent);
int		xlv_set_var(xl_value v,xl_value var);
int		xlv_set_ptr(xl_value v,void* ptr,int type);

int		xlv_set_func_builtin(xl_value v,xl_builtin_func fadr,const char* name);
int		xlv_set_func_ip(xl_value v,int fip);
int		xlv_set_func_label(xl_value v,int labelnr);

char*		xlv_ptr_name(int type);
int		xlv_ptr_unregister_type(int type);
int		xlv_ptr_register_type(const char* name);
int		xlv_ptr_type(const char* name);

const char*	xlv_str(const xl_value_t* v);
xl_str  	xlv_strx(const xl_value_t* v);
unsigned	xlv_str_cap(const xl_value_t* v);
xl_integer	xlv_int(const xl_value_t* v);
int		xlv_char(const xl_value_t* v);
xl_float	xlv_float(const xl_value_t* v);
void*		xlv_ptr(const xl_value_t* v,int type);

xl_value	xlv_deref(const xl_value_t* v);
int		xlv_extract(const xl_value_t* v,xl_value dest);

void		xlv_set_parent(xl_value val,xl_value parent);
void		xlv_inc_ref(const xl_value_t* v);
void		xlv_dec_ref(const xl_value_t* v);

int		xlv_true(const xl_value_t* v);
int		xlv_cmp(const xl_value_t* v1,const xl_value_t* v2);
int		xlv_add(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_add(xl_value dest, const xl_value_t* src);
int		xlv_sub(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_sub(xl_value dest, const xl_value_t* src);
int		xlv_mul(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_mul(xl_value dest, const xl_value_t* src);
int		xlv_div(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_div(xl_value dest, const xl_value_t* src);
int		xlv_mod(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_mod(xl_value dest, const xl_value_t* src);
int		xlv_chs(xl_value dest,const xl_value_t* a);
int		_xlv_chs(xl_value dest);
int		xlv_l_and(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_l_and(xl_value dest, const xl_value_t* src);
int		xlv_l_xor(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_l_xor(xl_value dest, const xl_value_t* src);
int		xlv_l_or(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_l_or(xl_value dest, const xl_value_t* src);
int		xlv_l_neg(xl_value dest,const xl_value_t* a);
int		_xlv_l_neg(xl_value dest);
#define xlv_l_not xlv_l_neg
#define _xlv_l_not _xlv_l_neg
int		xlv_b_and(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_b_and(xl_value dest, const xl_value_t* src);
int		xlv_b_xor(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_b_xor(xl_value dest, const xl_value_t* src);
int		xlv_b_or(xl_value dest,const xl_value_t* a,const xl_value_t* b);
int		_xlv_b_or(xl_value dest, const xl_value_t* src);
int		xlv_b_not(xl_value dest,const xl_value_t* a);
int		_xlv_b_not(xl_value dest);
int		xlv_field(xl_value val, const char* name, int mode,xl_value dest);
int		xlv_index(xl_value val, const xl_value_t* index, int mode,xl_value dest);

/* Function parameters hanling */
xl_func_params	xlv_fp_dup(const xl_func_params fp);
xl_func_params	xlv_fp_new(int number);
void		xlv_fp_free(xl_func_params fp);
void		xlv_fp_swrite(const xl_func_params fp,ZSTREAM f);
xl_func_params 	xlv_fp_sread(ZSTREAM f);

/* STREAM storing/loading of values */
int		xlv_swrite(const xl_value_t* v,ZSTREAM f);
int		xlv_sread(xl_value v,ZSTREAM f);

#define XLO_DECLARE_VT(name)                                \
static struct _xlo_vtable  name##_vtable;                   \
static int	    name##_vtable_ok = 0;

#define XLO_VT_INIT_BEGIN(name)                             \
    if( ! name##_vtable_ok ) {                              \
        struct _xlo_vtable* vt = &name##_vtable;            \
        memset(vt,0,sizeof(struct _xlo_vtable));            \
        vt->class_name = #name;

#define XLO_VT_INIT_END(name)                               \
        name##_vtable_ok =1;                                \
    }

#define XLO_VT_CALLBACK(name, function)                     \
        vt->name = (xlo_vt_##name##_t)function; 

#define XLO_CHECK_VT(object,name)                           \
        ((object)->vt == &name##_vtable)

#define XLO_CHECK_VT_NAME(object,name)                      \
        (strcmp((object)->vt->class_name,#name) == 0)

#define XLO_VT(name)    &name##_vtable   
/*
    object
*/
/*
##################################################################
##
##	=Module AXL Object API
##		API for manipualting AXL objects
##
##
##	=Overview
##		TODO: write some documentation here
##
*/
xl_object       xlo_new_name(const char* name);
xl_object       xlo_new_namex(const xl_str_t* namex);

/*##################################################################
##	=Function xlo_map_new
##		create new xl_object , type map
##
##	=Synopsis
##	|xl_object	xlo_new_map(void);
##	|xl_object	xlo_map_new(void);
##
##	=Description
##	    Create new <map> object. Instance of new <map> object
##	    is returned. 
##
##	    After using this pointer should be released by <xlo_free>.
##
*/
xl_object	xlo_new_map(void);
xl_object	xlo_map_new(void);

xl_object       xl_array_new(int);

/*
##################################################################
##
##	=Function xlo_res_new
##		create new xl_object , type resource
##
##	=Synopsis
##	|  typedef void (*xl_res_destructor)(void* res);
##	|
##	|  xl_object       xl_res_new(
##	|               void* p, 
##	|               int type,
##	|               xl_res_destructor destructor);
##
##
##	=Description
##	    Create new <resource> object.
##
##	    <resource> objects contain generic C pointer which
##	    holds some resource, reference count, methods and
##	    destructor function.
##
##	    The <res> argument is pointer to resource. The <map>
##	    argument contains methods that can be called on this
##	    resource by AXL program. If <map_copy> is true the new
##	    object will contain a copy of the methods, else it will
##	    use this specific map object <map> that should live
##	    as long as the new object.
##
##	    The <destructor> argument is pointer to "free resource"
##	    callback function which is called when object is destroyed
##	    by garbage collector.
##
##	    The function returns pointer to new xl_object that has
##	    reference_count set to 1.
##
##	    After using this pointer should be released by <xlo_free>.
##
*/
typedef void (*xl_res_destructor)(void* res);
xl_object       xl_res_new(void* p, int type, xl_res_destructor destructor);

void*           xl_res_get(const xl_object_t* o,int type);
void            xl_res_set(xl_object_t* o,void* res,int type);

/*##################################################################
##	=Function xlo_free,xlo_done
##		free reference to xl_object
##	=Synopsis
##	| void		xlo_free(xl_object obj);
##	| void		xlo_done(xl_object obj);
##
##	= Description
##		The <xlo_free> and <xlo_done> function releases reference to xl_object.
##		Specified object can be destroyed , so it shouldn't be used after it has
##		been passed to any of these functions.
*/
void		xlo_free(xl_object obj);
void		xlo_done(xl_object o);

/*##################################################################
##	=Function xlo_free_member
##		delete member of xl_object
##	=Synopsis
##	| void	xlo_free_member(xl_object obj,const char* name);
##
##	= Description
##		Funtction <xlo_free_member> deletes member of specified
##		which name is <name>.
*/
void		xlo_free_member(xl_object obj,const char* name);

/*##################################################################
##	=Function xlo_copy
##		create copy of xl_object
##	=Synopsis
##	| xl_object	xlo_copy(const xl_object_t* src);
##
##	= Description
##		Create copy of <src>. All members of object
##		are also copied. So all "sub-objects" in returned
##		object are also new instances.
##
##	    	After using returned pointer should be released by 
##		<xlo_free>.
##
*/
xl_object	xlo_copy(const xl_object_t* src);

/*##################################################################
##	=Function xlo_field
##		get member of object
##	=Synopsis
##	| xl_value xlo_field(const xl_object_t* obj,
##	|                    const char* name,int flags);
##
##	=Description
##		If <flags> is 0 then no new element is created else
##		new is created when specified by <idx> doesn't
##		exists.
##
##	=Return value
##		Pointer to value which is specfied element of array.
##
*/
int		xlo_field(const xl_object_t* obj, const char* name, int mode,xl_value dest);
xl_value	xlo_ffield(xl_object obj,const char* name);
/*##################################################################
##
##	=Function xlo_index
##		get element of array
##
##	=Synopsis
##	| xl_value xlo_index(const xl_object_t* obj,
##	|                    const xl_value_t* idx,int flags);
##
##	=Description
##		create copy of xl_object
##		If <flags> is 0 then no new element is created else
##		new is created when specified by <idx> doesn't
##		exists.
##	=Return value
##		Pointer to value which is specfied element of array.
##
*/
int		xlo_index(const xl_object_t* obj,const xl_value_t* idx, int mode,xl_value dest);
xl_value	xlo_findex(xl_object obj,const xl_value_t* index);


void		xlo_inc_ref(xl_object v);
void		xlo_dec_ref(xl_object v);

void*		xlo_get_res(const xl_object_t* v);
void		xlo_set_res(xl_object_t* v,void* r);

/*##################################################################
##
##	=Function xlo_set_map_public, xlo_get_map_public
##		set/get public map flag !DEPRECATED!
##
##	=Synopsis
##	| xl_bool   xlo_set_map_public(xl_object obj, xl_bool bpublic);
##	| xl_bool   xlo_get_map_public(const xl_object_t* obj);
##
##	=Description
##
##		"public map" is the flag of object that informs XL library
##		if it have to free map connected with specified object.
##
##		Using these functions you can obtain or change this flag.
##
##	=Return value
##
##		Returns destination state of "public map" of specified object.
##		If state of flag is changed, the "after change" state is returned.
##
*/
xl_bool		xlo_set_map_public(xl_object obj, xl_bool bpublic);
xl_bool		xlo_get_map_public(const xl_object_t* obj);

/*##################################################################
##
##	=Function xlo_set_map, xlo_get_map
##		set/get object map	!DEPRECATED!
##
##	=Synopsis
##	| void      xlo_set_map(xl_object obj, xlmap* map, xl_bool bcopy, xl_bool bpublic);
##	| xlmap    xlo_get_map(const xl_object_t* obj);
##
##	=Description
##
##		"map"	is an xl_map connected with xl object. The map is container for
##		members of an object addressed by names or member pointer.
##
##		<xlo_set_map> sets the new map to specified object. If <bcopy> flag is true
##		then a copy of the map is set. If <bpublic> flag is set and <bcopy> is unset
##		then "public map" flag is set to <bpublic>.
##
##	=Return value
##
##		<xlo_get_map> returns
##
*/
void		xlo_set_map(xl_object obj, xlmap* map, xl_bool bcopy, xl_bool bpublic);
xlmap		xlo_get_map(const xl_object_t* obj);

void		xlo_set_field_desc(xl_object obj, xlo_field_desc* fd);
void		xlo_sort_field_desc(xlo_field_desc* fd, int count);

xl_value	xlo_add_field_int(
    xl_object o,const char* name,xl_integer i);

xl_value	xlo_add_field_float(
    xl_object o,const char* name,xl_integer f);

xl_value	xlo_add_field_str(
    xl_object o,const char* name,const char* s);

xl_value	xlo_add_field_ref(
    xl_object o,const char* name,xl_value v);

xl_value	xlo_add_field_func_builtin(
    xl_object o,const char* name,xl_builtin_func fadr,const char* symb_name);

xl_value	xlo_add_field_func_ip(
    xl_object o,const char* name,int fip);

xl_value	xlo_add_field_func_label(
    xl_object o,const char* name,int labelnr);

xl_value	xlo_add_index_int(
    xl_object o,int idx,xl_integer i);
xl_value	xlo_add_index_float(
    xl_object o,int idx,xl_integer f);

xl_value	xlo_add_index_str(
    xl_object o,int idx,const char* s);


/*
 * value Context.
 */

xl_context	xlx_new(xl_context prev,xl_context next_find);
xl_context	xlx_init(xl_context c,xl_context prev,xl_context next_find);

xl_context	xlx_init_object(xl_context c,xl_object o,xl_context prev,xl_context next_find);
xl_context/*NR*/xlx_copy(const xl_context c);

void		xlx_free(xl_context c);
void		xlx_frees(xl_context c);

xl_context/*NR*/xlx_enter_func(xl_context c);
xl_context/*NR*/xlx_leave(xl_context c);

xl_value	xlx_find(xl_context c,const char* name,xl_value* parent);
xl_value	xlx_set(xl_context c,const char* name,xl_value val,xl_value* parent);
void		xlx_free_variable(xl_context,const char* name);

xl_value /*NR*/	xlx_find_scope(xl_context c,const char* scope,const char* name);
xl_value /*NR*/	xlx_set_scope(xl_context c,const char* scope,const char* name,xl_value val);

xl_value	xlx_set_int(xl_context c,const char* name,xl_integer i);
xl_value	xlx_set_float(xl_context c,const char* name,xl_float fl);
xl_value	xlx_set_str(xl_context c,const char* name,const char* s);
xl_value	xlx_set_ref(xl_context c,const char* name,xl_value obj);

/*
 * Code generation and optimization.
 */

xl_code 	xlc_new(void);
void		xlc_free(xl_code cd);

int		xlc_load(xl_code code,ZSTREAM in);
int		xlc_store(xl_code code,ZSTREAM out);

void		xlc_debug_new_line(int line);
void		xlc_debug_new_file(const char* name);

void		xlc_print(const xl_code);
void		xlc_set_res(xl_code c,xl_result_type,int n);

int		xlc_add_instr(xl_code cd,int icode,xl_value ival);
int		xlc_add_instr_nil(xl_code cd,int icode);
int		xlc_add_instr_int(xl_code cd,int icode,xl_integer val);
int		xlc_add_instr_float(xl_code cd,int icode,xl_float val);
int		xlc_add_instr_str(xl_code cd,int icode,const char* val);
int		xlc_add_instr_strx(xl_code cd,int icode,xl_str val);
int		xlc_add_label(xl_code cd,const char* name);

int		xlc_add_code(xl_code cd,xl_code p);
xl_code 	xlc_join(xl_code cd,xl_code p);

int		xlc_read_labels(xl_code cd);

int		xlc_opt_jumps(xl_code cd);
int		xlc_opt_code(xl_code,int oflags);

xl_instr	xlc_get_instr(xl_code c,int nr);
xl_instr	xlc_get_instr_end(xl_code c,int nr);

xl_value	xlc_get_val(xl_code c,int nr);
xl_value	xlc_get_val_end(xl_code c,int nr);

void            xl_code_gc();
void            xlc_attach_ptr(xl_code ptr);
int             xlc_detach_ptr(xl_code ptr);

/*
 * Virtual Machine & Threads
 */
xl_vm		xlvm_new(void);
void		xlvm_free(xl_vm vm,int wait_for_end);

#define XLVM_RUN_UNTIL_EXIT	0
#define XLVM_RUN_UNTIL_RETURN	1

xl_thread	xlvm_create(
		    xl_vm vm,
		    xl_code code,
		    int ip,
		    int sleep,
		    int uid,
		    int run_mode,
		    xl_context ctx);

int		xlvm_kill(xl_vm vm,xl_thread th);
int		xlvm_wait(xl_vm, xl_thread, xl_value ret_code,int wmode);

#define XLVM_WNOHANG 1

int		xlvm_step(xl_vm vm,int howmany);
int		xlvm_run(xl_vm vm);


void		xlth_enter_func(xl_thread th);
void		xlth_enter_object(xl_thread th,xl_object o);

void		xlth_enter_method(xl_thread th,xl_object o);
void		xlth_enter_object_global(xl_thread th,xl_object o);

void		xlth_leave(xl_thread th);
void		xlth_leave_func(xl_thread th);
void		xlth_leave_method(xl_thread th);

void		xlthd_debug_info(xl_thread thread,xl_value v);
void		xlthd_set_line(xl_thread thread,int line);
void		xlthd_set_file(xl_thread thread,const char* name);

#define 	xlb_func xlv_new_func_builtin
int		xl_load_func_builtin(xl_value v,const char* name);
int		xl_free_func_builtin(xl_value v);
const char*	xl_load_last_error(void);

xl_builtin_func xlb_load(const char* module,const char* name);
int		xlb_free(const char* module,const char* name);

void            xldbg_val_print(ZSTREAM s, const xl_value_t* val,int options);
void            xldbg_set_stream(ZSTREAM s);

xl_bool         xldbg_enable_mask(int mask, xl_bool enable);
int             xldbg_add_mask(const char* name, xl_bool enabled);
int             xldbg_get_mask(const char* name);
xl_bool         xldbg_mask_enabled(int mask);

void            xldbg_trace(int mask, const char* format, ...);
void            xl_trace(int mask, const char* format, ...);
void            xldbg_print(const char* format, ...);
ZSTREAM         xl_get_debug_stream(void);
void	        xl_set_debug_stream(ZSTREAM s);

/*
    error values
*/
#define XLE_UNKNOWN	1
#define XLE_MEMORY	2
#define XLE_IO		4
#define XLE_DIVZERO	8
#define XLE_INTERNAL	16

typedef enum  {
    xle_unknown     = 0,
    xle_memory	    = 1,
    xle_io	    = 2,
    xle_user	    = 3
} xle_type;

void		xle_throw(xle_type number,const char* msg);

typedef enum _except_num {
    xlpe_unknown		= 0,
    xlpe_invalid_instruction	= 1,
    xlpe_unknown_label		= 2,
    xlpe_invalid_jump		= 3,
    xlpe_4			= 4,
    xlpe_invalid_dup		= 5,
    xlpe_invalid_drop		= 6,
    xlpe_7			= 7,
    xlpe_invalid_copy		= 8,
    xlpe_invalid_store		= 9,
    xlpe_invalid_register	= 10,
    xlpe_invalid_function	= 11,
    xlpe_unknown_import 	= 12,
    xlpe_stack_underflow	= 13,
    xlpe_invalid_call		= 14
} except_num;

void	xlp_except(xl_thread thread,except_num num,const char* msg);

/*obsolete*/ void print_instr(int ip,int i ,xl_value v);


int	xlf_call(
    xl_thread	thread,
    xl_value	function,
    xl_value	first_param,
    int 	param_count,
    xl_value	result
    );

extern void* axl_malloc(size_t);
extern void  axl_free(void*);

#if !defined(HAVE_INLINE)

#define xlv_can_write(v)	( !ZFL_ISSET( (v)->flags, XLVF_READ_ONLY ) )
#define xlv_can_read(v)		( 1 )
#define xlv_need_free(v)	( !ZFL_ISSET( (v)->flags, XLVF_STATIC ) )
#define xlv_need_deref(v)	(  (v)->type == XLV_REF || (v)->type == XLV_STACK ||  ZFL_ISSET( (v)->flags, XLVF_NEED_DEREF ) )
#define xlv_need_extract(v)	(  ZFL_ISSET( (v)->flags, XLVF_NEED_EXTRACT ) )

#define _xlv_can_write(v)	( !ZFL_ISSET( (v)->flags, XLVF_READ_ONLY ) )
#define _xlv_can_read(v)	( 1 )
#define _xlv_need_free(v)	( !ZFL_ISSET( (v)->flags, XLVF_STATIC ) )
#define _xlv_need_deref(v)	(  (v)->type == XLV_REF  || (v)->type == XLV_STACK  || ZFL_ISSET( (v)->flags, XLVF_NEED_DEREF ) )
#define _xlv_need_extract(v)	(  ZFL_ISSET( (v)->flags, XLVF_NEED_EXTRACT ) )
#define _xlv_clear(v)		(((v)->type = XLV_NIL),((v)->flags = 0))
#define _xlv_done(v)		do { if ((v)->type > 15 ) xlv_done((v)); } while(0)
#define _xlv_mov(d,s)		do { _xlv_done((d)); *(d) = *(s); _xlv_clear((s)); } while(0)
#define _xlv_int(v)		(xlv_int((v)))
#define _xlv_float(v)		(xlv_float((v)))

#define xlv_is_str(v) ( (v)->type == XLV_STR || (v)->type == XLV_STR_BUF )

#define xlv_is_obj(v) ( (v)->type >= 48 && (v)->type <= 63 )
#define xlo_is_obj(v) ( (v)->type >= 48 && (v)->type <= 63 )
#define xlv_is_simple(v) ( ! xlv_is_obj(v) )

#else

static inline xl_bool xlv_is_obj(const xl_value_t* v)
{
    return v->type >= 48 && v->type <= 63;
}
static inline xl_bool xlo_is_obj(const xl_object_t* v)
{
    return v->type >= 48 && v->type <= 63;
}

static inline xl_bool xlv_is_simple(const xl_value_t* v)
{
    return ! xlv_is_obj(v);
}

static inline xl_bool xlv_is_str(const xl_value_t* v)
{
    return v->type == XLV_STR || v->type == XLV_STR_BUF;
}

static inline xl_bool  xlv_can_write(const xl_value_t* v)
{ return !ZFL_ISSET( (v)->flags, XLVF_READ_ONLY ); }

static inline xl_bool  xlv_can_read(const xl_value_t* v)
{ return 1; }

static inline xl_bool  xlv_can_free(const xl_value_t* v)
{ return !ZFL_ISSET( (v)->flags, XLVF_STATIC); }

static inline xl_bool  xlv_need_deref(const xl_value_t* v)
{ return (v)->type == XLV_STACK || (v)->type == XLV_REF || ZFL_ISSET( (v)->flags, XLVF_NEED_DEREF); }

static inline xl_bool  xlv_need_extract(const xl_value_t* v)
{ return ZFL_ISSET( (v)->flags, XLVF_NEED_EXTRACT); }

static inline xl_bool  _xlv_can_write(const xl_value_t* v)
{ return !ZFL_ISSET( (v)->flags, XLVF_READ_ONLY ); }

static inline xl_bool  _xlv_can_read(const xl_value_t* v)
{ return 1; }

static inline xl_bool  _xlv_can_free(const xl_value_t* v)
{ return !ZFL_ISSET( (v)->flags, XLVF_STATIC); }

static inline xl_bool  _xlv_need_deref(const xl_value_t* v)
{ return (v)->type == XLV_REF  || (v)->type == XLV_STACK || ZFL_ISSET( (v)->flags, XLVF_NEED_DEREF); }

static inline xl_bool  _xlv_need_extract(const xl_value_t* v)
{ return ZFL_ISSET( (v)->flags, XLVF_NEED_EXTRACT); }

static inline void _xlv_clear(xl_value v)
{  v->type = XLV_NIL; v->flags = 0; MDMF_FILL(v,"xlv "); }


static inline void _xlv_done(xl_value v)
{  if( v->type > 15 ) xlv_done(v); else _xlv_clear(v); }

static inline void _xlv_mov(xl_value dest,xl_value src)
{
    _xlv_done(dest);
    dest->type = src->type;
    dest->flags = src->flags;
    dest->v = src->v;
    _xlv_clear(src);
}

static inline xl_integer _xlv_int(const xl_value_t* v)
{
    if( v->type == XLV_INT ) return v->v.i;
    if( v->type == XLV_FLOAT ) return (xl_integer)v->v.f;
    return xlv_int(v);
}

static inline xl_float _xlv_float(const xl_value_t* v)
{
    if( v->type == XLV_INT ) return (xl_float)v->v.i;
    if( v->type == XLV_FLOAT ) return v->v.f;
    return xlv_int(v);
}

#endif	/* End of: inlines */
/*
##################################################################
##
##	=Module AXL User Functions API
##		Set of macros to declara/defina and use AXL builtin
##		functions.
##
##	=Overview
##		AXL User Functions API provides set of macros that encapsulate
##		particulars of AXL User Functions and provides simpified API
##		to create AXL functions.
##
*/

/*
##################################################################
##
##	=Macro XLF_DECL,XLF_EDECL
##		AXL User Function declaration
##
##	=Synopsis
##	|	XLF_DECL(function_name);
##	|	XLF_EDECL(function_name);
##	|
##	|	XLF_DECL(function_name)
##	|	{
##	|	    XLF_RET_NIL();
##	|	}
##	|	XLF_EDECL(function_name)
##	|	{
##	|	    XLF_RET_NIL();
##	|	}
##
##	=Description
##
##		Use this macro instead of hand-written function header to declare or
##		define XL AXL builtin function.
##
##		This macro expands to function returnning integer with
##		various arguments nneded to handle AXL runtime environment and function
##		actual arguments.
##		<XLF_EDECL> expand to function with extern linkage, <XLF_DECL> expands to
##		static linkage. Please use static linkage when possible to avoid 
##		linkage errors.
##
##		To access function arguments use other XLF macros such as: <XLF_THIS>
##		<XLF_ARGC>, <XLF_ARG>, and others.
##
##	=Example of usage
##
##	|XLF_DECL(my_function) {
##	|	XLF_RETURN_STR("test")
##	|}
##
####################################################################
*/
#define _XLF_DECL(name) int  name (xl_thread thread,int argc,xl_value argv,xl_value ret)
#define XLF_DECL(name) static _XLF_DECL(name)
#define XLF_EDECL(name) _XLF_DECL(name)

/*
##################################################################
##
##	=Macro XLF_THIS
##		get "this" pointer
##
##	=Synopsis
##	|	xl_object* This = XLF_THIS;
##
##	=Description
##		Use XLF_THIS macro to obtain pointer to This object
##		in current call environment.
##
##	=Example of usage
##
##	| xl_object* This = XLF_THIS;
##	| xl_object* a;
##	| if( !a ) 
##	|     XLF_RETURN_NIL();
##	| a = xlo_copy(This)
##	| XLF_RETURN_OBJ(a);
####################################################################
*/
#define XLF_THIS (xlv_deref(xlx_find(thread->var,"this",NULL) ))

/*
##################################################################
##
##	=Macro XLF_ARG, XLF_ARGC, XLF_ARG_INT, XLF_ARG_FLOAT, XLF_ARG_STR, XLF_ARG_OBJ, XLF_ARG_PTR
##		get actual argument of AXL User function
##
##	=Synopsis
##	|	xl_value    XLF_ARG(int i);
##	|	int         XLF_ARGC;
##	|	int         XLF_ARG_INT(int i, int default_value)
##	|	xl_float    XLF_ARG_FLOAT(int i, xl_float default_value)
##	|	const char* XLF_ARG_STR(int i, const char* default_value)
##	|	xl_object   XLF_ARG_OBJ(int i, int xl_ptr_type, int default_value)
##	|	void*       XLF_ARG_PTR(int i)
##
##	=Description
##		Use XLF_THIS macro to obtain arguments passed to AXL User function
##		by running program.
##
##		For information how to use pointers passed to AXL program see function
##		<xlv_ptr_type> and <xlv_set_ptr>.
##
##	=Example of usage
##
##	| XLF_DECL(example_function)
##	| {
##	|     xl_value      arg1  = XLF_ARG(0);
##	|     int           size = XLF_ARG_INT(1, 20);
##	|     float         fract = XLF_ARG_FLOAT(2,0.4);
##	|     const char*   dirname = XLF_ARG_STR(3,"/usr/local");
##	|     xl_object     obj = XLF_ARG_
##	|     int           nargs = XLF_ARGC;
##	|     void*         somedata = XLF_ARG_PTR(4,my_ptr_type,NULL);
##	| }
####################################################################
*/
#define XLF_ARGC	(argc)
#define XLF_ARG(i)	(argc > i ? argv+(i) : NULL )
#define XLF_ARG_INT(i,d)	(argc > i ? xlv_int(argv+(i)) : (d) )
#define XLF_ARG_FLOAT(i,d)	(argc > i ? xlv_float(argv+(i)) : (d) )
#define XLF_ARG_STR(i,d)	(argc > i ? xlv_str(argv+(i)) : (d) )
#define XLF_ARG_CHAR(i,d)	(argc > i ? xlv_char(argv+(i)) : (d) )
#define XLF_ARG_OBJ(i)		(argc > i ? (xl_object)xlv_deref(argv+(i)) : (xl_object)NULL )
#define XLF_ARG_PTR(i,t,d)	(argc >i ? xlv_ptr(xlv_deref(argv+(i)),(t)) : (void*)(d))

/*
##################################################################
##
##	=Macro XLF_RET, XLF_RET_OK, XLF_RET_FATAL, XLF_RET_REF, XLF_RET_INT, XLF_RET_FLOAT , XLF_RET_STR , XLF_RET_NIL, XLF_RET_PTR, XLF_RET_OBJ
##		get actual argument of AXL User function
##
##	=Synopsis
##	|	xl_value XLF_RET;
##	|	void	XLF_RET_OK()
##	|	void	XLF_RET_FATAL()
##	|	void	XLF_RET_REF(val)
##	|	void	XLF_RET_INT(int i)
##	|	void	XLF_RET_CHAR(int c)
##	|	void	XLF_RET_FLOAT(float f)
##	|	void	XLF_RET_STR(const char* s)
##	|	void	XLF_RET_NIL()
##	|	void	XLF_RET_PTR(void* ptr, int ptr_type)
##	|	void	XLF_RET_OBJ(xl_object obj)
##
##	=Description
##
##		Use those macros to return value from AXL User function.
##
##	=Example of usage
##
##	| XLF_DECL(example_function_return)
##	| {
##	|     switch( a ) {
##	|     case -1:
##	| 	XLF_RET_OK();
##	|     case 0:
##	| 	XLF_RET_NIL();
##	|     case 1:
##	| 	XLF_RET_INT(2);
##	|     case 2:
##	| 	XLF_RET_FLOAT(2.2);
##	| 	// etc
##	|     }
##	| }
##
####################################################################
*/
#define XLF_RET		(ret)
#define XLF_RET_OK()	XLF_RETURN_OK()
#define XLF_RET_FATAL() XLF_RETURN_FATAL()
#define XLF_RET_VAL	XLF_RETURN_VAL
#define XLF_RET_INT	XLF_RETURN_INT
#define XLF_RET_CHAR	XLF_RETURN_CHAR
#define XLF_RET_FLOAT	XLF_RETURN_FLOAT
#define XLF_RET_STR	XLF_RETURN_STR
#define XLF_RET_OBJ	XLF_RETURN_OBJ
#define XLF_RET_NIL	XLF_RETURN_NIL
#define XLF_RET_PTR	XLF_RETURN_PTR

#define XLF_RETURN_OK()		return 0;
#define XLF_RETURN_FATAL()	return -1;
#define XLF_RETURN_REF(v)		\
    do {				\
	xlv_set_ref(XLF_RET,(v),NULL);	\
	return 0;			\
    } while(0)

#define XLF_RETURN_VAL(v)		\
    do {				\
	xlv_set(XLF_RET,(v));		\
	return 0;			\
    } while(0)

#define XLF_RETURN_INT(i)		\
    do {				\
	xlv_set_int(XLF_RET,(i));	\
	return 0;			\
    } while(0)

#define XLF_RETURN_CHAR(c)		\
    do {				\
	xlv_set_char(XLF_RET,(c));	\
	return 0;			\
    } while(0)

#define XLF_RETURN_FLOAT(f)		\
    do {				\
	xlv_set_float(XLF_RET,(f));	\
	return 0;			\
    } while(0)

#define XLF_RETURN_STR(s)		\
    do {				\
	xlv_set_str(XLF_RET,(s));	\
	return 0;			\
    } while(0)

#define XLF_RETURN_NIL()		\
    do {				\
	xlv_clear(XLF_RET);		\
	return 0;			\
    } while(0)

#define XLF_RETURN_PTR(p,c)		\
    do {				\
	xlv_set_ptr(XLF_RET,(p),(c));	\
	return 0;			\
    } while(0)

#define XLF_RETURN_OBJ(o)		\
    do {				\
	xl_object _jfkdsjl_ = (o);	\
	xlv_set_ref(XLF_RET,(xl_value)(_jfkdsjl_),NULL);	\
	xlv_dec_ref((xl_value)(_jfkdsjl_));	\
	return 0;			\
    } while(0)

#ifdef __cplusplus
};
#endif

#ifndef __FUNCTION__
#define __FUNCTION__ "(unknown)"
#endif

#ifndef NOT_READY
#define NOT_READY	{ zfprintf(zstderr,"Called unready function at %s:%i,(%s)\n",__FILE__,__LINE__,"f : " __FUNCTION__);exit(1); }
#endif

#endif /* End of: force single include of file */
