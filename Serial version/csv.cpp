
#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "csv.h"
#include "matrix.h"

typedef unsigned long ulong;

#define LINE_MAX 50
#define TIME_INTERVAL 60

int dataSize = 0;

void writeCSV(char *filename, float *data, int n) {
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) return;
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%f\n", data[i]);
	}
	fclose(fp);
}

float *getExponentialBorders() {
	float * result = (float *)malloc((STATES - 1)* sizeof(float));
	float currPercent = MAX_DELTA;
	for (int i = 0; i < STATES / 2; i++) {
		currPercent /= 2;
		result[i] = -currPercent;
		result[STATES - i - 2] = currPercent;
	}
	if (STATES % 2 == 0) result[STATES / 2 - 1] = 0;
	return result;
}

float *getAverageBorders() {
	float * result = (float *) malloc(STATES * sizeof(float));
	float currPercent = 1 - MAX_DELTA;
	for (int i = 0; i < STATES-1; i++) {
		currPercent += STATE_SPAN;
		result[i] = currPercent;
	}
	return result;
}

int percentageToState(float percentage, float *borders) {
	for (int i = 0; i < STATES - 1; i++) {
		if (percentage < borders[i]) return i;
	}
	return STATES - 1;
}

int readCSV(char *filename, minuteTick *first, float *min, float *max) {
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) return NULL;
	char * line;
	char buf[LINE_MAX];
	ulong currentTimeStamp = 0;

	minuteTick *tmp = first;
	ulong unix;
	float price;

	// read first line and initialize current time stamp
	if (fgets(buf, sizeof buf, fp) == NULL) {
		printf("fgets is null\n");
	}
	if (sscanf(buf, "%lu,%f", &unix, &price) == 2) {
		dataSize++;
		currentTimeStamp = unix + TIME_INTERVAL;
		tmp->closePrice = price;
	}

	// main loop through file
	while (fgets(buf, sizeof buf, fp) != NULL) {
		if (sscanf(buf, "%lu,%f\n", &unix, &price) == 2) {
			while (currentTimeStamp < unix) {
				currentTimeStamp += TIME_INTERVAL;
				tmp->next = (minuteTick *)malloc(sizeof(minuteTick));
				tmp = tmp->next;
				tmp->closePrice = price;
				dataSize++;
				//printf("new tick at %lu, price: %f\n", currentTimeStamp, price);
			}
		}
		else {
			printf("scanf was unsucessful at current data size: %d\n", dataSize);
		}
	}

	printf("reading is finished\n");
	// dataSize-- because we are calculating percentages
	dataSize--;
	
	return dataSize;
}