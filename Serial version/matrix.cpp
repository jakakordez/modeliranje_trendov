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

void values_to_states(minuteTick *first, int **outputY, int **outputK, int *dataSizeY, int *dataSizeK) {
	int sizeY = *dataSizeY-1;
	int sizeK = *dataSizeK-1;
	int *statesY = (int *)malloc(sizeof(int) * (sizeY));
	int *statesK = (int *)malloc(sizeof(int) * (sizeK));
	int *numOfState = (int *)calloc(STATES, sizeof(int));
	// value2 is value 2 ticks behind, value1 is value 1 tick behind, value0 is current value
	float value2 = 0;
	float value1 = 0;
	float value0 = 0;
	float previousK = 0;

	// get borders to transform percentage to state
	float *borders = getExponentialBorders();

	minuteTick *iterator = first;
	for (int i = 0; i < sizeY; i++) {

		// change previous values
		value2 = value1;
		value1 = iterator->closePrice;
		value0 = iterator->next->closePrice;


		statesY[i] = percentageToState(value0 / value1 - 1, borders);
		if (i > 0 && statesY[i] == statesY[i - 1]) {
			sizeY--;
			i--;
		}
		numOfState[statesY[i]]++;

		// move iterator to next tick
		iterator = iterator->next;
	}
	iterator = first;
	minuteTick *deleting = iterator;
	for (int i = 0; i < sizeK; i++) {

		// change previous values
		value2 = value1;
		value1 = iterator->closePrice;
		value0 = iterator->next->closePrice;

		if (i > 0 && (value2 <= value1 && value1 <= value0 || value2 >= value1 && value1 >= value0)) {
			previousK = previousK * value0 / value1;
		}
		else {
			previousK = value0 / value1;
		}
		statesK[i] = percentageToState(previousK - 1, borders);
		if (i > 0 && statesK[i] == statesK[i - 1]) {
			sizeK--;
			i--;
		}
		//printf("state %d; Y: %d, K: %d\n", i, statesY[i], statesK[i]);
		/*if (*min > states[i])
		*min = states[i];
		else if (*max < states[i])
		*max = states[i];*/

		// move iterator to next tick
		iterator = iterator->next;
		free(deleting);
		deleting = iterator;
	}
	printf("Number of ticks: %d\n", sizeY);
	for (int i = 0; i < STATES; i++) {
		printf("state %d: %d\n", i, numOfState[i]);
	}
	*dataSizeY = sizeY;
	*dataSizeK = sizeK;
	*outputY = statesY;
	*outputK = statesK;
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

// se ne dokoncana funkcija.. jo zdj delam.
void predict_probability(float *matrix, float **pointer_to_vector, int days_ahead) {
	float *vector = *pointer_to_vector;
	for (int i = 0; i < days_ahead; i++) {

	}
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