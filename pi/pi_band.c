/*pi_band.c	*/
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
#pragma omp parallel private(i,x) reduction(+:sum)
		{
			int tid, nb_thread, debut, fin;
			tid = omp_get_thread_num();
			nb_thread = omp_get_num_threads();
			debut =  (num_steps / nb_thread)*tid;
			fin = debut + (num_steps / nb_thread);
	  for (i=debut;i<= fin; i++){
		  x = (i-0.5)*step;
		  sum = sum + 4.0/(1.0+x*x);
	  }
	#pragma omp single
		{
			debut =  (num_steps / nb_thread)*nb_thread;
			fin = num_steps;
			for (i=debut;i<= fin; i++){
					x = (i-0.5)*step;
					sum = sum + 4.0/(1.0+x*x);
				}
			}
	}
	  pi = step * sum;
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
}
