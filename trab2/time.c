#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DEFAULT_ITERATIONS 100
int main (int argc, char *argv[])
{
 long nTimes = DEFAULT_ITERATIONS;
 struct timespec t_start;
 struct timespec t_end;
 clock_gettime(CLOCK_MONOTONIC, &t_start);
 for (unsigned long i = 0; i < nTimes; ++i) {
 // code to evaluate
 }
 clock_gettime(CLOCK_MONOTONIC, &t_end);
 double total_time = (t_end.tv_sec - t_start.tv_sec ) + (t_end.tv_nsec - t_start.tv_nsec )/1e9;
 double average_time = (total_time/nTimes)*1e6;
 printf("Total elapsed time = %9.6lf s (average %7.2lf us)\n", total_time, average_time);
} 
