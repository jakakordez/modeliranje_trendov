#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include "csv.h"

#define STATES		6
#define PAST		5
#define DIMENSIONS	PAST+1
#define MAX_DELTA   0.0005
#define STATE_SPAN  2 * MAX_DELTA / STATES

typedef unsigned long ulong;

float *allocate_matrix();
int *allocate_index();
int fill_matrix(float * matrix, int *data, int n);
void values_to_states(minuteTick *first, int **output, int dataSize);
void predict(float *matrix, int **states, int n);

#endif // !MATRIX_H
