// Serial version.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"
#include "csv.h"
#include "chain.h"
#include "benchmark.h"
#include "prediction.h"


float minPercentage = 1;
float maxPercentage = 1;

int main()
{
	// get borders to transform percentage to state
	float *borders = getExponentialBorders();

	float *values;

	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	float *testValues = (float *)malloc((PREDICT_LAST + PAST) * sizeof(float));
	int sizeY = readCSV(dataFilename(), first, &minPercentage, &maxPercentage, &testValues);
	int sizeK = sizeY;

	/*printf("start\n");
	for (int i = 0; i < PREDICT_LAST + PAST; i++) {
		printf("%f \n", testValues[i]);
	}
	printf("end\n");
	while (1);*/

	float *matrixY = allocate_matrix();
	float *matrixK = allocate_matrix();
	
	runBenchmark(30, matrixY, matrixK, sizeY, sizeK, borders, first, testValues);

	//writeCSV("..\\Output\\matrikaY.csv", matrixY, pow(STATES, DIMENSIONS));

 	model2_report(matrixY);
		
	printf("Done.");
	while (1);
    return 0;
}