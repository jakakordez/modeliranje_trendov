#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"


float *allocate_matrix() {
	int size = pow(STATES, DIMENSIONS);
	return (float *)malloc(size);
}

int *allocate_index() {
	return (int *)malloc(DIMENSIONS);
}

ulong index(int* states) {
	ulong index = 0;
	for (int i = 0; i < PAST; i++) {
		index += states[i];
		index *= STATES;
	}
	index += states[PAST];
	return index;
}