#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "csv.h"

#define STATES		5
#define PAST		5
#define DIMENSIONS	PAST+1
#define MAX_DELTA   0.001 // krci in siri graf
#define EXPONENT	3
#define STATE_SPAN  2 * MAX_DELTA / STATES

typedef unsigned long ulong;

float *allocate_matrix();
int *allocate_index();
void acolumn_index(int *states, ulong index); 
void states_to_values(int *states, float initialValue, float **output, float *borders, int n);
void values_to_states(minuteTick *first, int **outputY, int **outputK, int *dataSizeY, int *dataSizeK, float *borders);
ulong column_index(int* states);
void model2_report(float *matrix);
ulong index(int* states);

#endif // !MATRIX_H
