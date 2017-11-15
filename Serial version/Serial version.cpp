// Serial version.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"
#include "csv.h"
#include "chain.h"
#include "benchmark.h"

#define PREDICT_LAST 200

float minPercentage = 1;
float maxPercentage = 1;

int main()
{
	// get borders to transform percentage to state
	float *borders = getExponentialBorders();

	float *values;

	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	int sizeY = readCSV(dataFilename(), first, &minPercentage, &maxPercentage);
	int sizeK = sizeY;

	float *matrixY = allocate_matrix();
	float *matrixK = allocate_matrix();	
	
	runBenchmark(30, matrixY, matrixK, sizeY, sizeK, borders, first);

	//writeCSV("..\\Output\\matrikaY.csv", matrixY, pow(STATES, DIMENSIONS));

 	model2_report(matrixY);

	/*int *next_statesY = (int *)malloc((PREDICT_LAST + PAST) * sizeof(int));
	for (int i = 0; i < PAST; i++) {
		next_statesY[i] = statesY[sizeY - PREDICT_LAST - PAST + i];
	}
	predict(matrixY, &next_statesY, PREDICT_LAST);
	
	int *next_statesK = (int *)malloc((PREDICT_LAST + PAST) * sizeof(int));
	for (int i = 0; i < PAST; i++) {
		next_statesK[i] = statesK[sizeK - PREDICT_LAST - PAST + i];
	}
	predict(matrixK, &next_statesK, PREDICT_LAST);

	int ** predicted = (int **) malloc(sizeof(int *) * 2);
	predicted[0] = next_statesY;
	predicted[1] = next_statesK;

	writePredictedCSV("..\\Output\\predicted.csv", 
		&statesY[sizeY - PREDICT_LAST - PAST],
		predicted,
		2,
		PREDICT_LAST + PAST);*/
		
	printf("Done.");
	while (1);
    return 0;
}