#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

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

void states_to_values(int *states, float initialValue, float **output, float *borders, int n) {
	printf("initial value: %f\n", initialValue);
	float *out = *output;
	out[0] = initialValue;
	printf("borders\n");
	for (int i = 0; i < STATES; i++) {
		printf(" %f, ", borders[i]);
	}
	printf("\n");

	for (int i = 1; i < n; i++) {
		float percent;
		if (states[i] == 0) {
			percent = 1 + (- MAX_DELTA + borders[0]) / 2;
			// output[i] = (2-MAX_DELTA + borders[0]) * output[i-1] / 2;
		}
		else if (states[i] == STATES - 1) {
			percent = 1 + (borders[STATES - 2] + MAX_DELTA) / 2;
			// output[i] = (2+borders[STATES - 1] + MAX_DELTA) * output[i - 1] / 2;
		}
		else {
			percent = 1 + (borders[states[i]] + borders[states[i] - 1]) / 2;
			// output[i] = (2+borders[states[i]] + borders[states[i]-1]) * output[i - 1] / 2;
		}
		printf("state: %d, percent: %f\n", states[i], percent);
		out[i] = percent * out[i - 1];
		printf("v: %d, %f\n", i, out[i]);
	}
}

void values_to_states(minuteTick *first, int **outputY, int **outputK, int *dataSizeY, int *dataSizeK, float *borders) {
	int sizeY = *dataSizeY - 1;
	int sizeK = *dataSizeK - 1;
	int *statesY = (int *)malloc(sizeof(int) * (sizeY));
	int *statesK = (int *)malloc(sizeof(int) * (sizeK));
	int *numOfState = (int *)calloc(STATES, sizeof(int));
	// value2 is value 2 ticks behind, value1 is value 1 tick behind, value0 is current value
	float value2 = 0;
	float value1 = 0;
	float value0 = 0;
	float previousK = 0;

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
		//free(deleting);
		deleting = iterator;
	}
	printf("Number of ticks: %d\n", sizeY);
	for (int i = 0; i < STATES; i++) {
		printf("state %d: %d\n", i, numOfState[i]);
	}
	printf("\n");
	*dataSizeY = sizeY;
	*dataSizeK = sizeK;
	*outputY = statesY;
	*outputK = statesK;
}
#define absol(a) ((a < 0)?-a:a)
#define K 10
void model2_report(float *matrix) {
	int states[PAST];
	int bestTable[K][PAST];
	float probs[K];
	int min_index = 0;
	float pos = 0;
	float neg = 0;
	ulong index = 0;

	for (int i = 0; i < K; i++) {
		probs[i] = 0;
	}

	for (int i = 0; i < pow(STATES, PAST); i++) {
		pos = 0;
		neg = 0;
		for (int j = 0; j < STATES; j++) {
			if (j < STATES / 2) pos += matrix[index];
			else neg -= matrix[index];
			index++;
		}
		if (-neg > pos) pos = neg;
		if (absol(pos) < 0.99f) {
			acolumn_index(states, index);
			if (absol(pos) > probs[min_index]) {
				probs[min_index] = pos;
				memcpy(bestTable[min_index], states, sizeof(states));
			}

			min_index = 0;
			for (int j = 1; j < K; j++) {
				if (absol(probs[j]) < absol(probs[min_index])) min_index = j;
			}
		}
	}

	printf("Model2 report\n");
	for (int j = 0; j < K; j++) {
		for (int k = 0; k < PAST; k++) {
			printf("%i -> ", bestTable[j][k]);
		}
		printf("%f\n", probs[j]);
	}
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

void acolumn_index(int *states, ulong index) {
	index /= STATES;
	for (int i = PAST - 1; i >= 0; i--) {
		states[i] = index%STATES;
		index /= STATES;
	}
}

// se ne dokoncana funkcija.. jo zdj delam.
void predict_probability(float *matrix, float **pointer_to_vector, int days_ahead) {
	float *vector = *pointer_to_vector;
	for (int i = 0; i < days_ahead; i++) {

	}
}