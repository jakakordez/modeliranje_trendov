#define HAVE_STRUCT_TIMESPEC

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "matrix.h"
#include "csv.h"
#include "chain.h"
#include "benchmark.h"

float minPercentage = 1;
float maxPercentage = 1;

int main(void)
{
	/*float *borders = getExponentialBorders();

	float *values;

	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	int sizeY = readCSV(dataFilename(), first, &minPercentage, &maxPercentage);
	int sizeK = sizeY;

	float *matrixY = allocate_matrix();
	float *matrixK = allocate_matrix();

	runBenchmark(30, matrixY, matrixK, sizeY, sizeK, borders, first);*/

	return 0;
}

