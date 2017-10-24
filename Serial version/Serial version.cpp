// Serial version.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"

#define LINE_MAX 50

// struct is used just temporary while reading file
typedef struct Min {
	struct Min *next;
	double closePrice;
} minuteTick;

double minPercentage = 1;
double maxPercentage = 1;
int dataSize = 0;

double *readCSV(char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) return NULL;
	char * line;
	char buf[LINE_MAX];
	ulong currentTimeStamp = 0;

	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	minuteTick *tmp = first;
	ulong unix;
	double price;

	// read first line and initialize current time stamp
	if (fgets(buf, sizeof buf, fp) == NULL) {
		printf("fgets is null\n");
	}
	if (sscanf(buf, "%lu,%lf", &unix, &price) == 2) {
		dataSize++;
		currentTimeStamp = unix + 60;
		tmp->closePrice = price;
	}

	// main loop through file
	while (fgets(buf, sizeof buf, fp) != NULL) {
		if (sscanf(buf, "%lu,%lf\n", &unix, &price) == 2) {
			while (currentTimeStamp < unix) {
				currentTimeStamp += 60;
				tmp->next = (minuteTick *)malloc(sizeof(minuteTick));
				tmp = tmp->next;
				tmp->closePrice = price;
				dataSize++;
				printf("new tick at %lu, price: %lf\n", currentTimeStamp, price);
			}
		}
		else {
			printf("scanf was unsucessful at current data size: %d\n", dataSize);
		}
	}

	// dataSize-- because we are calculating percentages
	dataSize--;
	double *percentage = (double *)malloc(sizeof(double) * (dataSize));
	minuteTick *iterator = first;
	minuteTick *deleting = iterator;
	for (int i = 0; i < dataSize; i++) {
		percentage[i] = iterator->closePrice / iterator->next->closePrice;
		if (minPercentage > percentage[i])
			minPercentage = percentage[i];
		else if (maxPercentage < percentage[i])
			maxPercentage = percentage[i];

		// move iterator to next tick
		iterator = iterator->next;
		free(deleting);
		deleting = iterator;
	}
	printf("Number of minutes: %d", dataSize);
	return percentage;
}

int main()
{
	float *matrix = allocate_matrix();
	double * data = readCSV("..\\..\\btcnCNY_2011-01-01_2017-10-01.csv");
	if (data == NULL) {
		printf("Data is null\n");
	}
	else {
		printf("Printing percentages: \n");
		for (int i = 0; i < dataSize; i++) {
			printf("%f\n", data[i]);
		}
		printf("Min percentage: %f\n", minPercentage);
		printf("Max percentage: %f\n", maxPercentage);
	}
	//while (1);
    return 0;
}