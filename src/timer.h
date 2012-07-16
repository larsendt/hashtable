#ifndef TIMER_H
#define TIMER_H

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

#endif
