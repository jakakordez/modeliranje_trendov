
#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "chain.h"
#include "matrix.h"

#define THREADS	2

typedef struct _fill_args {
	int id;
	float *matrix;
	int *data;
	int n;
} fill_args;
void *fill_thread(void *arg) {
	fill_args *ar = (fill_args *)arg;
	int num_of_samples = ar->n - PAST - 1;

	int chunkSize = (pow(STATES, PAST) + THREADS - 1) / THREADS;
	int start = chunkSize*ar->id;
	int end = ps_min(start + chunkSize, pow(STATES, PAST));
	for (int i = start; i < end; i++) {
		ulong num = index(&(ar->data[i]));
		ar->matrix[num] += 1;
	}
	return NULL;
}

int fillMatrix(float * matrix, int *data, int n) {
	pthread_t threads[THREADS];

	for (int i = 0; i < THREADS; i++) {
		fill_args *args = (fill_args *)malloc(sizeof(fill_args));
		args->id = i;
		args->matrix = matrix;
		args->data = data;
		args->n = n;
		pthread_create(&threads[i], NULL, fill_thread, (void *)args);
	}
	for (int i = 0; i < THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	return 1;
}

typedef struct _normalize_args {
	int id;
	float *matrix;
} normalize_args;

void *normalize_thread(void *arg)
{
	normalize_args *ar = (normalize_args *)arg;
	int chunkSize = (pow(STATES, PAST) + THREADS - 1) / THREADS;
	int start = chunkSize*ar->id;
	int end = ps_min(start + chunkSize, pow(STATES, PAST));
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
	//free(arg);
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


void prepare() {}