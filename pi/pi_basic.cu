
#include <stdio.h>
#include "omp_repair.h"
#include <cuda.h>
static long num_steps = 1000000; // 100 millions
double step;

#if __CUDA_ARCH__ < 600
__device__ double atomicAdd2(double* address, double val)
{
    unsigned long long int* address_as_ull =
                              (unsigned long long int*)address;
    unsigned long long int old = *address_as_ull, assumed;

    do {
        assumed = old;
        old = atomicCAS(address_as_ull, assumed,
                        __double_as_longlong(val +
                               __longlong_as_double(assumed)));

    // Note: uses integer comparison to avoid hang in case of NaN (since NaN != NaN)
    } while (assumed != old);

    return __longlong_as_double(old);
}
#endif

__global__ void cal_pi(long num_steps, double step, double *sum)
{
		int i;
		double x;
		double local;
		local=0.0;
	for (i=1;i<= num_steps; i++){
		x = (i-0.5)*step;
		local += 4.0/(1.0+x*x);
	}
		atomicAdd2(sum, local);
}

int main ()
{
	  double pi, sum = 0.0;
	  double start_time, run_time;
	  step = 1.0/(double) num_steps;
	  start_time = omp_get_wtime();
		double *dev_sum;
    // capture the start time
    cudaEvent_t     start, stop;
    cudaEventCreate( &start );
    cudaEventCreate( &stop );
    cudaEventRecord( start, 0 );
//
		cudaMalloc((void **)&dev_sum, sizeof(double));
		cudaMemset(dev_sum, 0, sizeof(double));
		cal_pi<<<1,1>>>(num_steps,step,dev_sum);
		cudaMemcpy(&sum, dev_sum, sizeof(double), cudaMemcpyDeviceToHost);

	  pi = step * sum;
    // get stop time, and display the timing results
		    cudaEventRecord( stop, 0 );
		    cudaEventSynchronize( stop );
		    float   elapsedTime;
		    cudaEventElapsedTime( &elapsedTime,
		                                        start, stop );
		    printf( "Time to compute :  %3.1f ms\n", elapsedTime );
		    cudaEventDestroy( start );
		    cudaEventDestroy( stop );	
	  run_time = omp_get_wtime() - start_time;
	  printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
}
