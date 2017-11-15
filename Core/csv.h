#pragma once
#ifndef CSV_H
#define CSV_H

// struct is used just temporary while reading file
typedef struct Min {
	struct Min *next;
	float closePrice;
} minuteTick;

int readCSV(char *filename, minuteTick *first, float *min, float *max);
void writeCSV(char *filename, float *data, int n);
void writePredictedCSV(char *filename, int *data, int **predicted, int num_of_predictions, int n);
float *getExponentialBorders();
float *getAverageBorders();
int percentageToState(float percentage, float *borders);
char *dataFilename();

#endif // !CSV_H