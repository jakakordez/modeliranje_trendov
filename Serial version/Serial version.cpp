// Serial version.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"
#include "csv.h"

float minPercentage = 1;
float maxPercentage = 1;

int main()
{
	/*float *a = getExponentialBorders();
	for (int i = 0; i < STATES-1 ; i++) {
		printf("state %d, max: %f\n", i, a[i]);
	}
	while (1);
	return 0;*/

	float *matrix = allocate_matrix();
	int * data;
	int size = readCSV("..\\btcnCNY_2011-01-01_2017-10-01.csv", &data, &minPercentage, &maxPercentage);
	if (data == NULL) {
		printf("Data is null\n");
	}
	else {
		/*printf("Printing percentages: \n");
		for (int i = 0; i < size; i++) {
			printf("%f\n", data[i]);
		}
		printf("Min percentage: %f\n", minPercentage);
		printf("Max percentage: %f\n", maxPercentage);*/
		fill_matrix(matrix, data, size);
	}
	writeCSV("matrika.csv", matrix, pow(STATES, DIMENSIONS));
	printf("Done.");
	while (1);
    return 0;
}