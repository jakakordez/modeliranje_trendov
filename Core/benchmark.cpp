
#include "stdafx.h"
#include <stdio.h>

#include "benchmark.h"
#include "matrix.h"
#include "chain.h"

#include <Windows.h>

double get_wall_time() {
	LARGE_INTEGER time, freq;
	if (!QueryPerformanceFrequency(&freq)) {
		//  Handle error
		return 0;
	}
	if (!QueryPerformanceCounter(&time)) {
		//  Handle error
		return 0;
	}
	return (double)time.QuadPart / freq.QuadPart;
}

void runBenchmark(int iterations, float *Y, float *K, int nY, int nK, float *borders, minuteTick *first) {
	int *statesY;
	int *statesK;
	values_to_states(first, &statesY, &statesK, &nY, &nK, borders);
	double start, stop;
	for (int i = 0; i < iterations; i++) {
		start = get_wall_time();

		fillMatrix(Y, statesY, nY);
		fillMatrix(K, statesK, nK);

		normalizeMatrix(Y);
		normalizeMatrix(K);

		stop = get_wall_time();
		double diff = stop - start;
		printf("Time: %lf\n", diff);
	}
}