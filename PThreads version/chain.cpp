
#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "chain.h"
#include "matrix.h"

#define THREADS	2

int fillMatrix(float * matrix, int *data, int n) {
	// parallel stuff
	return 1;
}

typedef struct _normalize_args {
	int id;
	float *matrix;
} normalize_args;

void *normalize_thread(void *arg)
{
	normalize_args *ar = (normalize_args *)arg;
	int n = pow(STATES, PAST) / THREADS;
	int start = n*ar->id;
	int end = (n + 1)*ar->id;
	for (int i = start; i < end; i++) {
		float sum = 0;
		for (int j = 0; j < STATES; j++) {
			sum += ar->matrix[i*STATES + j];
		}
		if (sum == 0) continue;
		for (int j = 0; j < STATES; j++) {
			ar->matrix[i*STATES + j] /= sum;
		}
	}
	free(arg);
	return NULL;
}

void normalizeMatrix(float *matrix) {
	pthread_t threads[THREADS];

	for (int i = 0; i < THREADS; i++) {
		normalize_args *args = (normalize_args *)malloc(sizeof(normalize_args));
		args->id = i;
		args->matrix = matrix;
		pthread_create(&threads[i], NULL, normalize_thread, (void *)args);
	}
	for (int i = 0; i < THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
}

