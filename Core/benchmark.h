#pragma once
#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "csv.h"

void runBenchmark(int iterations, float *Y, float *K, int nY, int nK, float *borders, minuteTick *first, float *testValues);

#endif // !BENCHMARK_H
