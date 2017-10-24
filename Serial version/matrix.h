#pragma once

#define STATES		9
#define PAST		5
#define DIMENSIONS	PAST+1

typedef unsigned long ulong;

float *allocate_matrix();
int *allocate_index();
