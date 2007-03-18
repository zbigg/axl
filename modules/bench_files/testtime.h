/* -*- zz-c -*- */
/* testtime.c
 *
 * Author Zbigniew Zagorski <zzbigg@o2.pl>
 *
 *
 * Compile under GCC
 */

#ifndef __testtime_h_
#define __testtime_h_

#include "zcompat/zsystype.h"

#ifndef ZSYS_WIN32

#include <sys/time.h>
#include <unistd.h>

#else

#include <time.h>
// #include <windows.h>

#endif

#ifdef __cplusplus
extern "C" {
#endif

int	dev_random();

#ifndef ZSYS_WIN32
double	time2double(struct timeval* tv);
#else
double	time2double(time_t time);
#endif

 double 	currenttime();
 int	new_counter();
 int	update_counter(int i);
 int		set_counter_state(int i,int state);

#define pause_counter(i) set_counter_state(i,1)
#define resume_counter(i) set_counter_state(i,0)

 double 	stop_counter(int i);
 double 	get_counter_start(int i);
 double 	get_counter_end(int i);
 double 	get_counter_pause(int i);
 double 	get_counter_total(int i);
 int	end_counter(int i);

#define MAX_COUNTERS 100

#ifdef __cplusplus
}
#endif

#endif
