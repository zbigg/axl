/*
* File:		vm.c
*
* Id:           $Id: vm.c 1.3 03/11/27 21:36:26+01:00 zbigg@zbigg.(none) $
*
* Project:	axl
*
* Description:	Virtual Machine & Threads implmentation
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

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "zcompat/ztools.h"
#include "axl/axl.h"

#include "axl_i.h"

#define TH_LOCK(th)
#define TH_UNLOCK(th)

#define VM_LOCK(vm)
#define VM_UNLOCK(vm)

#define TH_WAIT_FINISHED(th)	do { } while( !th->finished );

/* GLOBALS */
xl_vm		xlvm_new(void);
void		xlvm_free(xl_vm vm,int wait_for_end);
int		xlvm_wait(xl_vm vm, xl_thread th, xl_value ret_value,int wmode);
xl_thread	xlvm_create(
		    xl_vm vm,
		    xl_code code,
		    int ip,
		    int sleep,
		    int uid,
		    int run_mode,
		    xl_context ctx);

int		xlvm_step(xl_vm vm,int howmany);
int		xlvm_run(xl_vm vm);

/* LOCALS */
static int		xlvm_add_thread(xl_vm vm,xl_thread t);
static int		xlvm_del_thread(xl_vm vm,xl_thread t);
static int		xlvm_destroy_thread(xl_vm vm,xl_thread t);
static xl_thread	xlvm_next_thread(xl_vm vm);
static void		xlvm_try_to_resume(xl_vm, xl_thread thread);

/* IMPLEMENTATION */

xl_vm		xlvm_new(void)
{
    xl_vm a = (xl_vm) axl_calloc( 1 ,sizeof( xl_vm_t ) );
    if( !a )
	return NULL;
    /* here init vm LOCK */
    VM_LOCK(vm);
    a->threads = slist_new();
    slist_begin(a->threads,&a->tl_begin);
    slist_end(a->threads,&a->tl_end);
    sli_inc( & a->tl_end);

    a->tl_i = a->tl_begin;
    a->current_thread = 0;
    a->active_threads = 0;
    a->running_threads = 0;
    VM_UNLOCK(vm);
    return a;
}

void		xlvm_free(xl_vm vm,int wait_for_end)
{
    sli_t i;
    VM_LOCK(vm);
    while( !slist_empty(vm->threads) ) {
	slist_begin(vm->threads,&i);
	xlvm_destroy_thread(vm, (xl_thread) sli_get(&i));
    }
    VM_UNLOCK(vm);
    /* TODO: here destroy vm LOCK */
    axl_free(vm);
}

/*
    GLOBAL

    create new thread object in VM context

    thread is automaticaly inserted into VM Thread Queue
*/
xl_thread	xlvm_create(
		    xl_vm vm,
		    xl_code code,
		    int ip,
		    int sleep,
		    int uid,
		    int run_mode,
		    xl_context ctx)
{
    xl_thread	t = (xl_thread)axl_calloc( 1, sizeof(xl_thread_t) );
    int i;
    if( t == NULL )
	return NULL;
    t->code = code;
    memset(&t->stack,0,sizeof(t->stack));

    t->debug_file = NULL;
    t->debug_line = 1;
    t->interrupted = 0;
    t->finished = 0;

    t->nip = ip;

    t->run = 1;
    t->run_mode = run_mode;

    t->ip = 0;
    t->ic = 0;
    t->sleep = sleep;
    t->uid = uid;
    xlv_clear( &t->ret );
    /* here init th LOCK */
    if( ctx ) {
	t->var = t->var_global = ctx;
	ctx->ref_count ++ ;
    } else
	t->var = t->var_global = xlx_new(NULL,NULL);
#ifdef XLD_TRACE_CONTEXT
    strcpy(t->var_global->debug_name,"CTX (#0) ___GLOBAL___");
#endif
    for( i = 0; i < countof(t->regs) ; i++ ) {
	_xlv_clear( t->regs + i );
    }

    TH_UNLOCK(t);
    /* add to VM's thread list */
    if( !xlvm_add_thread(vm,t) ) {
	/* here destroy th LOCK */
	xlx_free(t->var);
	axl_free( t );
	return NULL;
    }
    return t;
}
/*
    GLOBAL

    kill thread, which mean that is interrupted by signal 1

*/
int		xlvm_kill(xl_vm vm,xl_thread t)
{
    if( !t ) return -1;
    TH_LOCK(t);
    t->interrupted = 1;	/* killed 			*/
    t->finished = 1;	/* won't be executed anymore	*/
    t->run = 0;
    TH_UNLOCK(t);
    return 0;
}

/*
    GLOBAL


*/
int		xlvm_wait(xl_vm vm, xl_thread th, xl_value ret_value,int wmode)
{
    if( wmode == XLVM_WNOHANG && !th->finished ) {
	xlv_clear(ret_value);
	return -1;
    }
    TH_WAIT_FINISHED(th);

    xlv_set(ret_value,& th->ret);

    th->result_consumed = 1;

    xlvm_destroy_thread(vm,th);
    return 0;
}

/* processor function */
extern int	xlp_step(xl_thread th,int howmany);

int		xlvm_step(xl_vm vm,int howmany)
{
    xl_thread th = 0;
    int done= 0;
    int i = howmany < 0 ? 1 : howmany;
    if( vm->active_threads == 0 )
	return 0;
    while( i > 0 ) {
	th = xlvm_next_thread(vm);
	if( !th ) return 0;

	switch( xlp_step(th,1) ) {
	case 0:		/* stopped: finished or interrupted */
	    vm->running_threads--;
	    break;
	case 1:		/* still running */
			/* so nothing happens */
	    break;
	case -1:	/* error */
	    vm->running_threads--;
	    th->run = 0;
	    return -1;
	}
	if( howmany > 0 )
	    --i;
	done++;
    }
    return done;
}

int		xlvm_run(xl_vm vm)
{
    int r;
    r = xlvm_step(vm,-1) != -1;
    return r;
}

/* LOCAL
    add thread to VM Thread Queue
*/
static int		xlvm_add_thread(xl_vm vm,xl_thread t)
{
    VM_LOCK(vm);
    if( slist_add(vm->threads,(void*)t) < 0 ) {
	VM_UNLOCK(vm);
	return 0;
    }
    slist_begin(vm->threads,& vm->tl_begin);
    slist_end(vm->threads,& vm->tl_end);
    sli_inc(& vm->tl_end);
    if( t->run )
	vm->running_threads++;
    if( !t->finished )
	vm->active_threads++;
    if( vm->active_threads <= 1 )
	vm->tl_i = vm->tl_begin;
    VM_UNLOCK(vm);
    return 1;
}

/* LOCAL
    delete thread from VM Thread Queue
*/
static int		xlvm_del_thread(xl_vm vm,xl_thread t)
{
    VM_LOCK(vm);
    if( slist_del(vm->threads,(void*)t) == 0 ) {
	VM_UNLOCK(vm);
	return 0;
    }
    slist_begin(vm->threads,& vm->tl_begin);
    slist_end(vm->threads,& vm->tl_end);
    sli_inc(& vm->tl_end);
    if( t->run )
	vm->running_threads--;
    if( !t->finished )
	vm->active_threads--;

    if( vm->active_threads <= 1 )
	vm->tl_i = vm->tl_begin;
    VM_UNLOCK(vm);
    return 1;
}

/* LOCAL
    destroy thread object

    if thread runs it is killed
    if thread is still in VMTQ then it is removed
*/
static int		xlvm_destroy_thread(xl_vm vm,xl_thread t)
{
    int i;
    xlvm_kill(vm,t);

    /* delete from VM's thread list */
    xlvm_del_thread(vm,t);

    /* TODO: here destroy th LOCK */

    for( i = 0; i < countof(t->regs) ; i++ ) {
	_xlv_done( t->regs + i );
    }

    {
	unsigned i;
	for( i = 0; i < t->stack.itop; i++ )
	    xlv_done(t->stack.ptr+i);
    }

    _xlv_done(&t->ret);

    while( t->var != NULL ) {
	xl_context tmp = t->var->prev_ctx;
	xlx_free(t->var);
	t->var = tmp;
    }
    axl_free(t);

    return 1;
}
/*	LOCAL

    get next thread in VM Thread Queue
*/
static xl_thread	xlvm_next_thread(xl_vm vm)
{
    xl_thread th;
    if( vm->running_threads == 0 ) return NULL;
    do {
	VM_LOCK(vm);
	th = sli_get(&vm->tl_i);	/* get first thread from queue */
	if( !th )	return 0;

	sli_inc(&vm->tl_i);		/* set iterator to next thread */
	if( sli_eq(&vm->tl_i, &vm->tl_end) )
	    slist_begin(vm->threads,& vm->tl_i);
	VM_UNLOCK(vm);
	if( th->run == 0 )		/* try to resume sleeping thread */
	    xlvm_try_to_resume(vm,th);
    } while( th->run == 0 );		/* if sleeping continue searching */

    vm->current_thread = th;
    return th;
}
/*	LOCAL

*/
static void		xlvm_try_to_resume(xl_vm vm, xl_thread th)
{
    int i,k;
    struct xlth_wait_t* wi = th->wait;
    if( th->run ) return;

    for( k = i = 0; i < th->wait_count && k < countof( th->wait);
	k++, wi++ ) if( wi->type ) {
	switch( wi->type ) {
	case XLTHW_THREAD:
	    {
		xl_value_t rval;
		rval.type = XLV_NIL;
		if( xlvm_wait(vm,wi->what.thread, & rval, XLVM_WNOHANG ) < 0 )
		    break;

		if( wi->result)
		    xlv_assign(wi->result,&rval);
		xlv_done(&rval);
		wi->type = 0;
		th->wait_count--;
	    }
	    break;
	default:
	    break;
	}
	i++;
    }

    th->run = th->wait_count == 0;
}
