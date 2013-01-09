// License: BSD 3-clause
// Author: Dane Larsen
// See LICENSE.txt for the complete license text

#ifndef TIMER_H
#define TIMER_H

#include <time.h>

#define rdtsc(low,high) \
         __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

inline void getcycles(long long int * cycles)
{
    unsigned long low;
    long high;
    rdtsc(low,high);
    *cycles = high; 
    *cycles <<= 32; 
    *cycles |= low; 
}

struct timespec snap_time()
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t;
}

double get_elapsed(struct timespec t1, struct timespec t2)
{
    double ft1 = t1.tv_sec + ((double)t1.tv_nsec / 1000000000.0);
    double ft2 = t2.tv_sec + ((double)t2.tv_nsec / 1000000000.0);
    return ft2 - ft1;
}
    

#endif
