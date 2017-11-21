
#include "stdafx.h"
#include "stdlib.h"
#include <math.h>
#include "stdio.h"
#include "matrix.h"
#include <time.h>

#include "prediction.h"

// prediction stuff here

void construct_and_predict(int * statesY, int sizeY, float *matrixY, int *statesK, int sizeK, float *matrixK, float *testValues, float *borders) {
	int *next_statesY = (int *)malloc((PREDICT_LAST + PAST) * sizeof(int));
	float *next_valuesY = (float *)malloc((PREDICT_LAST + PAST) * sizeof(float));
	float *errorY = (float *)malloc((PREDICT_LAST + PAST) * sizeof(float));
	for (int i = 0; i < PAST; i++) {
		next_statesY[i] = statesY[sizeY - PREDICT_LAST - PAST + i];
	}
	predict(matrixY, &next_statesY, PREDICT_LAST);
	states_to_values(next_statesY, testValues[0], &next_valuesY, borders, PREDICT_LAST + PAST);
	calculateRelativeError(next_valuesY, testValues, PREDICT_LAST + PAST, &errorY);

	int *next_statesK = (int *)malloc((PREDICT_LAST + PAST) * sizeof(int));
	float *next_valuesK = (float *)malloc((PREDICT_LAST + PAST) * sizeof(float));
	float *errorK = (float *)malloc((PREDICT_LAST + PAST) * sizeof(float));
	for (int i = 0; i < PAST; i++) {
		next_statesK[i] = statesK[sizeK - PREDICT_LAST - PAST + i];
	}
	predict(matrixK, &next_statesK, PREDICT_LAST);
	states_to_values(next_statesK, testValues[0], &next_valuesK, borders, PREDICT_LAST + PAST);
	calculateRelativeError(next_valuesK, testValues, PREDICT_LAST + PAST, &errorK);

	float **values_and_error = (float **)malloc(sizeof(float) * 5);
	values_and_error[0] = testValues;
	values_and_error[1] = next_valuesY;
	values_and_error[2] = errorY;
	values_and_error[3] = next_valuesK;
	values_and_error[4] = errorK;

	writeFloatArrays("..\\Output\\values_and_error.csv",
		values_and_error,
		5,
		PREDICT_LAST + PAST);


	int ** predicted = (int **)malloc(sizeof(int *) * 2);
	predicted[0] = next_statesY;
	predicted[1] = next_statesK;



	writePredictedCSV("..\\Output\\predicted.csv",
		&statesY[sizeY - PREDICT_LAST - PAST],
		predicted,
		2,
		PREDICT_LAST + PAST);
}

void calculateRelativeError(float *predicted, float *actual, int n, float **output) {
	float *out = *output;
	for (int i = 0; i < n; i++) {
		out[i] = (predicted[i] - actual[i]) / actual[i];
	}
}

// argument int **states must contain last PAST values to predict new ones
void predict(float *matrix, int **states, int n) {

	/* initialize random seed: */
	srand(time(NULL));

	int *array = *states;

	for (int i = PAST; i < n + PAST; i++) {
		double random_pick = (double) rand() / RAND_MAX;
		int next_state;
		float max_probability = 0;
		ulong matrix_index = column_index(&array[i - PAST]);
		//printf("\n");

		for (int j = 0; j < STATES; j++) {
			random_pick -= matrix[matrix_index + j];
			if (random_pick <= 0) {
				// printf("picked state: %d, %f\n", j, matrix[matrix_index + j]);
				next_state = j;
				break;
			}
			if (j == STATES - 1) {
				printf("j: %d %f -> ", j, random_pick);
				for (int k = 0; k < STATES; k++) {
					printf("%d ", matrix[matrix_index + k]);
				}
				printf("\n");
				next_state = j;
			}
		}
		/*for (int j = 0; j < STATES; j++) {
			printf("matrix: %f\n", matrix[matrix_index + j]);
			if (matrix[matrix_index + j] > max_probability) {
				max_probability = matrix[matrix_index + j];
				next_state = j;
			}
		}*/
		array[i] = next_state;
	}
}