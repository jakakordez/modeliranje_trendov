#include "stdafx.h"
#include "math.h"
#include "stdlib.h"

#include "matrix.h"

float *allocate_matrix() {
	int size = pow(STATES, DIMENSIONS);
	printf("allocating matrix with %d cells\n", size);
	return (float *)calloc(size, sizeof(float));
}

int *allocate_index() {
	return (int *)calloc(DIMENSIONS, sizeof(int));
}

ulong index(int* states) {
	ulong index = 0;
	for (int i = 0; i < PAST; i++) {
		if (states[i] >= STATES) {
			printf("Error %d\n", states[i]);
		}
		index += states[i];
		index *= STATES;
	}
	index += states[PAST];	
	return index;
}

int fill_matrix(float * matrix, int *data, int n) {
	int num_of_samples = n - PAST - 1;
	for (int i = 0; i < num_of_samples; i++) {
		ulong num = index(&data[i]);
		//printf("index: %d\n", num);
		matrix[num] += 1;
	}
	for (int i = 0; i < pow(STATES, PAST); i++) {
		float sum = 0;
		for (int j = 0; j < STATES; j++) {
			sum += matrix[i*STATES + j];
		}
		if (sum == 0) continue;
		for (int j = 0; j < STATES; j++) {
			matrix[i*STATES + j] /= sum;
			printf("matrix[%d] = %f\n", i*STATES + j, matrix[i*STATES + j]);
		}
		printf("\n");
	}
	return 1;
}