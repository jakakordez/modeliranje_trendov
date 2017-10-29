// Serial version.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"
#include "csv.h"

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

	float *matrix = allocate_matrix();
	float *values;
	int *states;
	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	int size = readCSV(EURUSD_CSV, first, &minPercentage, &maxPercentage);

	/*printf("Printing percentages: \n");
	for (int i = 0; i < size; i++) {
		printf("%f\n", data[i]);
	}
	printf("Min percentage: %f\n", minPercentage);
	printf("Max percentage: %f\n", maxPercentage);*/

	values_to_states(first, &states, size);
	fill_matrix(matrix, states, size);
	
	writeCSV("..\\Output\\matrika.csv", matrix, pow(STATES, DIMENSIONS));
	printf("Done.");
	while (1);
    return 0;
}