#pragma once

// struct is used just temporary while reading file
typedef struct Min {
	struct Min *next;
	double closePrice;
} minuteTick;

int readCSV(char *filename, int **data, float *min, float *max);
void writeCSV(char *filename, float *data, int n);
float *getExponentialBorders();
float *getAverageBorders();
int percentageToState(float percentage, float *borders);
