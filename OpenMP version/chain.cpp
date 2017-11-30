#include "stdafx.h"
#include <math.h>

#include "chain.h"
#include "matrix.h"

int fillMatrix(float * matrix, int *data, int n) {
	int num_of_samples = n - PAST - 1;
	for (int i = 0; i < num_of_samples; i++) {
		ulong num = index(&data[i]);
		//printf("index: %d\n", num);
		matrix[num] += 1;
	}
	return 1;
}

void normalizeMatrix(float *matrix) {
	for (int i = 0; i < pow(STATES, PAST); i++) {
		float sum = 0;
		for (int j = 0; j < STATES; j++) {
			sum += matrix[i*STATES + j];
		}
		if (sum == 0) continue;
		for (int j = 0; j < STATES; j++) {
			matrix[i*STATES + j] /= sum;
			//printf("matrix[%d] = %f\n", i*STATES + j, matrix[i*STATES + j]);
		}
		//printf("\n");
	}
}

