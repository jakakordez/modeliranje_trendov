#pragma once
#ifndef CSV_H
#define CSV_H

// struct is used just temporary while reading file
typedef struct Min {
	struct Min *next;
	double closePrice;
} minuteTick;

int readCSV(char *filename, minuteTick *first, float *min, float *max);
void writeCSV(char *filename, float *data, int n);
void writePredictedCSV(char *filename, int *data, int *predicted, int n);
float *getExponentialBorders();
float *getAverageBorders();
int percentageToState(float percentage, float *borders);

#endif // !CSV_H