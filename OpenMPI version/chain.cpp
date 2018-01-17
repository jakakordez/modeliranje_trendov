#include "stdafx.h"
#include <math.h>
#include "mpi.h"

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
	int pcnt = 0;
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	size--;
	int i;
	int n = pow(STATES, PAST);
	for (i = 0; i < n; i++) {
		MPI_Send((float *)&matrix[i*STATES], STATES, MPI_FLOAT, pcnt + 1, 0, MPI_COMM_WORLD);
		pcnt = (pcnt + 1) % size;
	}
	pcnt = 0;
	MPI_Status status;
	for (i = 0; i < n; i++)
	{
		MPI_Recv((float *)&matrix[i*STATES], STATES, MPI_FLOAT, pcnt + 1, 0, MPI_COMM_WORLD, &status);
		pcnt = (pcnt + 1) % size;
	}
}

void prepare() {}