#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "csv.h"

#define STATES		10
#define PAST		8
#define DIMENSIONS	PAST+1
#define MAX_DELTA   0.0001 // krci in siri graf
#define EXPONENT	3
#define STATE_SPAN  2 * MAX_DELTA / STATES

typedef unsigned long ulong;

float *allocate_matrix();
int *allocate_index();
void acolumn_index(int *states, ulong index);
int fill_matrix(float * matrix, int *data, int n);
void values_to_states(minuteTick *first, int **outputY, int **outputK, int *dataSizeY, int *dataSizeK, float *borders);
void predict(float *matrix, int **states, int n);
ulong column_index(int* states);
void model2_report(float *matrix);
ulong index(int* states);

#endif // !MATRIX_H
