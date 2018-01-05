#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "csv.h"

#define STATES		8
#define PAST		4
#define DIMENSIONS	PAST+1
#define MAX_DELTA   0.001 // krci in siri graf
#define EXPONENT	3
#define STATE_SPAN  2 * MAX_DELTA / STATES

#define ps_min(a, b)	((a)<(b)?(a):(b))

typedef unsigned long ulong;

float *allocate_matrix();
int *allocate_index();
void acolumn_index(int *states, ulong index); 
void states_to_values(int *states, float initialValue, float **output, float *borders, int n);
void values_to_states(minuteTick *first, int **outputY, int **outputK, int *dataSizeY, int *dataSizeK, float *borders);
ulong column_index(int* states);
void model2_report(float *matrix);
ulong index(int* states);

int hash(float *matrix);

#endif // !MATRIX_H
