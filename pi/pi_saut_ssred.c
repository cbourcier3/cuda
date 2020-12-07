/*pi_saut_ssred.c */
#include <stdio.h>
#include "omp_repair.h"
static long num_steps = 100000000; // 100 millions
double step;
int main ()
{
	  int i;
	  double x, pi, sum = 0.0;
	  double start_time, run_time;
	  step = 1.0/(double) num_steps;
	  start_time = omp_get_wtime();
#pragma omp parallel private(i,x)
{
	int tid, nb_thread;
	double sum_loc =0.0;
	tid = omp_get_thread_num();
	nb_thread = omp_get_num_threads();
	  for (i=tid;i<= num_steps; i+=nb_thread ){
		  x = (i-0.5)*step;
		  sum_loc = sum_loc + 4.0/(1.0+x*x);
	  }
#pragma omp atomic
			sum += sum_loc;
}
	  pi = step * sum;
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
}
