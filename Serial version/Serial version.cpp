// Serial version.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"
#include "csv.h"

#include <Windows.h>

#define PREDICT_LAST 200

float minPercentage = 1;
float maxPercentage = 1;

char *BTC_CSV = "..\\Data\\btcnCNY_2011-01-01_2017-10-01.csv";
char *EURUSD_CSV = "..\\Data\\EURUSD_M1_2000_2016.csv";

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

int main()
{
	// get borders to transform percentage to state
	float *borders = getExponentialBorders();

	float *values;
	int *statesY;
	int *statesK;
	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	int sizeY = readCSV(EURUSD_CSV, first, &minPercentage, &maxPercentage);
	int sizeK = sizeY;

	float *matrixY = allocate_matrix();
	float *matrixK = allocate_matrix();
	for (int i = 0; i < 1; i++) {
		int sy = sizeY;
		int sk = sizeK;
		double start, stop;
		start = get_wall_time();

		values_to_states(first, &statesY, &statesK, &sy, &sk, borders);
		//printf("size: Y - %d, K - %d\n", sizeY, sizeK);
		fill_matrix(matrixY, statesY, sy - PREDICT_LAST);
		fill_matrix(matrixK, statesK, sk - PREDICT_LAST);

		stop = get_wall_time();
		double diff = stop - start;
		printf("Time: %lf\n", diff);
	}
	//writeCSV("..\\Output\\matrikaY.csv", matrixY, pow(STATES, DIMENSIONS));

 	model2_report(matrixY);

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
		
	printf("Done.");
	while (1);
    return 0;
}