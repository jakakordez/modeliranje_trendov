#define _CRT_SECURE_NO_DEPRECATE

#include "stdafx.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>

#include "csv.h"
#include "matrix.h"
#include "prediction.h"

typedef unsigned long ulong;

#define LINE_MAX 50
#define TIME_INTERVAL 60

int dataSize = 0;

char *BTC_CSV = "..\\Data\\btcnCNY_2011-01-01_2017-10-01.csv";
char *EURUSD_CSV = "..\\Data\\EURUSD_M1_2000_2016.csv";

char *dataFilename() {
	return EURUSD_CSV;
}

void writeCSV(char *filename, float *data, int n) {
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) return;
	for (int i = 0; i < n; i++) {
		if (i % STATES == 0)
			fprintf(fp, "\n%f", data[i]);
		else
			fprintf(fp, ", %f", data[i]);
	}
	fclose(fp);
}

void writeFloatArrays(char *filename, float **predicted, int num_of_arrays, int n) {
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) return;
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%d", i);
		for (int j = 0; j < num_of_arrays; j++) {
			fprintf(fp, ", %f", predicted[j][i]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void writePredictedCSV(char *filename, int *data, int **predicted, int num_of_predictions, int n) {
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) return;
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%d, %d", i, data[i]);
		for (int j = 0; j < num_of_predictions; j++) {
			fprintf(fp, ", %d", predicted[j][i]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

float *getExponentialBorders() {
	float * result = (float *)malloc((STATES - 1) * sizeof(float));
	float currPercent = MAX_DELTA;
	for (int i = 0; i < STATES / 2; i++) {
		if (STATES / 2 - i == 2) currPercent /= EXPONENT;
		currPercent /= EXPONENT;
		result[i] = -currPercent;
		result[STATES - i - 2] = currPercent;
	}
	printf("Borders:");
	if (STATES % 2 == 0) result[STATES / 2 - 1] = 0;
	for (int i = 0; i < STATES - 1; i++) {
		printf(" %f", result[i]);
	}
	printf("\n");
	return result;
}

float *getAverageBorders() {
	float * result = (float *)malloc(STATES * sizeof(float));
	float currPercent = 1 - MAX_DELTA;
	for (int i = 0; i < STATES - 1; i++) {
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

int readCSV(char *filename, minuteTick *first, float *min, float *max, float **testValues) {
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) return NULL;
	char * line;
	char buf[LINE_MAX];
	ulong currentTimeStamp = 0;

	minuteTick *tmp = first;
	int valuesAhead = 1;
	minuteTick *lastTestValues = first;
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
				if (valuesAhead >= PREDICT_LAST + PAST) 
					lastTestValues = lastTestValues->next;
				else
					valuesAhead++;

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

	for (int i = 0; i < PREDICT_LAST + PAST; i++) {
		(*testValues)[i] = lastTestValues->closePrice;
		lastTestValues = lastTestValues->next;
	}

	printf("reading is finished\n");
	// dataSize-- because we are calculating percentages
	dataSize--;

	return dataSize;
}