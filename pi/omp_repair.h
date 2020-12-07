/*******************************************************
 * Copyright (C) 2010-2011 Cédric EMONNEAU
 * contact@cedric-emonneau.com
 *
 * This file is only intended to compile quickly with
 * openmp in serial & parallel
 *
 * This file can be copied and/or distributed without
 * the express permission of Cédric EMONNEAU
 *******************************************************/
#ifndef __OPENMP_REPAIR__
#define __OPENMP_REPAIR__
#ifdef _OPENMP
	#include <omp.h>
#else
	#ifdef _WIN32
		#include <windows.h>
		int _gettimeofday(struct timeval* p, void* tz) {
			ULARGE_INTEGER ul;
			FILETIME ft;
			GetSystemTimeAsFileTime(&ft);
			ul.LowPart = ft.dwLowDateTime;
			ul.HighPart = ft.dwHighDateTime;
			ul.QuadPart /= 10ULL;
			ul.QuadPart -= 11644473600000000ULL;
			p->tv_usec = (long) (ul.QuadPart % 1000000LL);
			p->tv_sec = (long) (ul.QuadPart / 1000000LL);
			return 0;
		}
	#else
		#include <sys/time.h>
	#endif
	double _openmpRepairWtime(){
		struct timeval tp;
		#ifndef _WIN32
			gettimeofday(&tp, NULL);
		#else
			_gettimeofday(&tp,NULL);
		#endif
		return (tp.tv_sec * 1000.0 + tp.tv_usec / 1000)/1000.0;
	}
	#define omp_get_num_threads() 1
	#define omp_get_thread_num() 0
	#define omp_get_wtime() _openmpRepairWtime()
	#define omp_set_num_threads(num)
	#define omp_in_parallel() 0
#endif
#endif
