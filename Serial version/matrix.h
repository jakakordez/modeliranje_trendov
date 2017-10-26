#pragma once

#define STATES		6
#define PAST		5
#define DIMENSIONS	PAST+1
#define MAX_DELTA   0.02
#define STATE_SPAN  2 * MAX_DELTA / STATES

typedef unsigned long ulong;

float *allocate_matrix();
int *allocate_index();
int fill_matrix(float * matrix, int *data, int n);