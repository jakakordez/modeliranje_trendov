#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include "csv.h"

#include "matrix.h"

float *allocate_matrix() {
	ulong size = pow(STATES, DIMENSIONS);
	printf("allocating matrix with %d cells\n", size);
	return (float *)calloc(size, sizeof(float));
}

int *allocate_index() {
	return (int *)calloc(DIMENSIONS, sizeof(int));
}

void values_to_states(minuteTick *first, int **output, int dataSize) {
	int *states = (int *)malloc(sizeof(int) * (dataSize));

	// get borders to transform percentage to state
	float *borders = getExponentialBorders();
	minuteTick *iterator = first;
	minuteTick *deleting = iterator;
	for (int i = 0; i < dataSize; i++) {
		states[i] = percentageToState(1 - iterator->closePrice / iterator->next->closePrice, borders);
		/*if (*min > states[i])
		*min = states[i];
		else if (*max < states[i])
		*max = states[i];*/

		// move iterator to next tick
		iterator = iterator->next;
		free(deleting);
		deleting = iterator;
	}
	printf("Number of ticks: %d\n", dataSize);
	*output = states;
}

ulong column_index(int* states) {
	ulong index = 0;
	for (int i = 0; i < PAST; i++) {
		if (states[i] >= STATES) {
			printf("Error %d\n", states[i]);
		}
		index += states[i];
		index *= STATES;
	}
	return index;
}

ulong index(int* states) {
	ulong index = column_index(states);
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
			//printf("matrix[%d] = %f\n", i*STATES + j, matrix[i*STATES + j]);
		}
		//printf("\n");
	}
	return 1;
}

// argument int **states must contain last PAST values to predict new ones
void predict(float *matrix, int **states, int n) {
	int *array = *states;

	for (int i = PAST; i < n + PAST; i++) {
		int next_state;
		float max_probability = 0;
		ulong matrix_index = column_index(&array[i - PAST]);
		printf("\n");
		for (int j = 0; j < STATES; j++) {
			printf("matrix: %f\n", matrix[matrix_index + j]);
			if (matrix[matrix_index + j] > max_probability) {
				max_probability = matrix[matrix_index + j];
				next_state = j;
			}
		}
		array[i] = next_state;
	}
}