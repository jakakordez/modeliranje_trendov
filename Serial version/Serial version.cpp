// Serial version.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"
#include "csv.h"

#define PREDICT_LAST 200

float minPercentage = 1;
float maxPercentage = 1;

char *BTC_CSV = "..\\Data\\btcnCNY_2011-01-01_2017-10-01.csv.csv";
char *EURUSD_CSV = "..\\Data\\EURUSD_M1_2000_2016.csv";

int main()
{
	/*float *a = getExponentialBorders();
	for (int i = 0; i < STATES-1 ; i++) {
		printf("state %d, max: %f\n", i, a[i]);
	}
	while (1);
	return 0;*/

	float *matrixY = allocate_matrix();
	float *matrixK = allocate_matrix();
	float *values;
	int *statesY;
	int *statesK;
	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	int sizeY = readCSV(EURUSD_CSV, first, &minPercentage, &maxPercentage);
	int sizeK = sizeY;

	/*printf("Printing percentages: \n");
	for (int i = 0; i < size; i++) {
		printf("%f\n", data[i]);
	}
	printf("Min percentage: %f\n", minPercentage);
	printf("Max percentage: %f\n", maxPercentage);*/

	values_to_states(first, &statesY, &statesK, &sizeY, &sizeK);
	printf("size: Y - %d, K - %d\n", sizeY, sizeK);
	fill_matrix(matrixY, statesY, sizeY - PREDICT_LAST);
	fill_matrix(matrixK, statesK, sizeK - PREDICT_LAST);


	int *next_statesY = (int *)malloc((PREDICT_LAST + PAST) * sizeof(int));
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
		PREDICT_LAST + PAST);


	writeCSV("..\\Output\\matrikaY.csv", matrixY, pow(STATES, DIMENSIONS));
	printf("Done.");
	while (1);
    return 0;
}