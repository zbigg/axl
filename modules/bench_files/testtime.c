/* -*- zz-c -*- */
/* testtime.c
 *
 * Author Zbigniew Zagorski <zzbigg@o2.pl>
 *
 */
#include "testtime.h"

#include <stdio.h>

#include <zsystype.h>
#ifdef ZSYS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

typedef struct counter_t {
    int 	busy;
    double	start;
    double	end;
    double	pause;
    int 	paused;
    double	total;
    double	last;
} counter_t;

struct counter_t counters[MAX_COUNTERS];
int c_number;

double currenttime(void)
{
#ifdef ZSYS_WIN32

    return  ((double) GetTickCount() ) / 1000.0;
/*    time_t t;
    time(&t);
    return (double)t;
*/
#else
    struct timeval tv;
    gettimeofday(&tv,(void*)0);
    return (double)tv.tv_sec+((double)tv.tv_usec)/1000000;
#endif
}
int new_counter()
{
    double cur;
    int i = 0;
    while(i < MAX_COUNTERS) {
	if( counters[i].busy == 0) {
	    counters[i].busy = 1;
	    counters[i].pause = 0;
	    counters[i].paused = 1;
	    counters[i].total = 0;
	    counters[i].start = counters[i].end = counters[i].last = cur
		= currenttime();
	    return i;
	}
	i++ ;
    };
    return -1;
}
int update_counter(int i)
{
    counter_t* a = &counters[i];
    double cur = currenttime();
    double d;
    d = cur - a->last;
    a->last = cur;
    if( a->paused ) {
	a->pause+=d;
    } else {
	a->total += d;
	a->end = cur;
    }
    return 0;
}
int set_counter_state(int i,int state)
{
    update_counter(i);
    counters[i].paused = state;
    return state;
}
double stop_counter(int i)
{
    update_counter(i);
    counters[i].paused = 1;
    return end_counter(i),get_counter_total(i);
}

double get_counter_start(int i)
{
    update_counter(i);
    return counters[i].start;
}
double get_counter_end(int i)
{
    update_counter(i);
    return counters[i].end;
}
double get_counter_pause(int i)
{
    update_counter(i);
    return counters[i].pause;
}
double get_counter_total(int i)
{
    update_counter(i);
    return counters[i].total;
}
int end_counter(int i)
{
    counters[i].busy = 0;
    return 0;
}
