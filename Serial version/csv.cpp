
#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "csv.h"

typedef unsigned long ulong;

#define LINE_MAX 50

int dataSize = 0;

void writeCSV(char *filename, float *data, int n) {
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) return;
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%f\n", data[i]);
	}
	fclose(fp);
}

int readCSV(char *filename, float **data, float *min, float *max) {
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) return NULL;
	char * line;
	char buf[LINE_MAX];
	ulong currentTimeStamp = 0;

	minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
	minuteTick *tmp = first;
	ulong unix;
	float price;

	// read first line and initialize current time stamp
	if (fgets(buf, sizeof buf, fp) == NULL) {
		printf("fgets is null\n");
	}
	if (sscanf(buf, "%lu,%f", &unix, &price) == 2) {
		dataSize++;
		currentTimeStamp = unix + 60;
		tmp->closePrice = price;
	}

	// main loop through file
	while (fgets(buf, sizeof buf, fp) != NULL) {
		if (sscanf(buf, "%lu,%f\n", &unix, &price) == 2) {
			while (currentTimeStamp < unix) {
				currentTimeStamp += 60;
				tmp->next = (minuteTick *)malloc(sizeof(minuteTick));
				tmp = tmp->next;
				tmp->closePrice = price;
				dataSize++;
				printf("new tick at %lu, price: %f\n", currentTimeStamp, price);
			}
		}
		else {
			printf("scanf was unsucessful at current data size: %d\n", dataSize);
		}
	}

	// dataSize-- because we are calculating percentages
	dataSize--;
	float *percentage = (float *)malloc(sizeof(float) * (dataSize));
	minuteTick *iterator = first;
	minuteTick *deleting = iterator;
	for (int i = 0; i < dataSize; i++) {
		percentage[i] = iterator->closePrice / iterator->next->closePrice;
		if (*min > percentage[i])
			*min = percentage[i];
		else if (*max < percentage[i])
			*max = percentage[i];

		// move iterator to next tick
		iterator = iterator->next;
		free(deleting);
		deleting = iterator;
	}
	printf("Number of minutes: %d", dataSize);
	*data = percentage;
	return dataSize;
}