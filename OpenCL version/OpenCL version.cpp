// OpenCL version.cpp : Defines the entry point for the console application.
//

#define HAVE_STRUCT_TIMESPEC

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "csv.h"
#include "chain.h"
#include "benchmark.h"
#include "prediction.h"

float minPercentage = 1;
float maxPercentage = 1;

int main()
{
	float *borders = getExponentialBorders();

	float *values;

	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	float *testValues = (float *)malloc((PREDICT_LAST + PAST) * sizeof(float));
	int sizeY = readCSV(dataFilename(), first, &minPercentage, &maxPercentage, &testValues);
	int sizeK = sizeY;

	float *matrixY = allocate_matrix();
	float *matrixK = allocate_matrix();

	runBenchmark(1, matrixY, matrixK, sizeY, sizeK, borders, first, testValues);
	printf("Done.");
	while (1);
	return 0;
}

