// OpenMPI version.cpp : Defines the entry point for the console application.
//

#define HAVE_STRUCT_TIMESPEC

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "mpi.h"

#include "matrix.h"
#include "csv.h"
#include "chain.h"
#include "benchmark.h"
#include "prediction.h"

float minPercentage = 1;
float maxPercentage = 1;

int main(int argc, char **argv)
{
	int size;
	int id;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	if (id == 0) {
		printf("%d/%d\n", id, size);
		float *borders = getExponentialBorders();
		float *values;

		minuteTick *first = (minuteTick *)malloc(sizeof(minuteTick));
		float *testValues = (float *)malloc((PREDICT_LAST + PAST) * sizeof(float));
		int sizeY = readCSV(dataFilename(), first, &minPercentage, &maxPercentage, &testValues);
		int sizeK = sizeY;

		float *matrixY = allocate_matrix();
		float *matrixK = allocate_matrix();

		int *statesY;
		int *statesK;
		values_to_states(first, &statesY, &statesK, &sizeY, &sizeK, borders);
		double start, stop;

		start = MPI_Wtime();

		fillMatrix(matrixY, statesY, sizeY);
		//fillMatrix(matrixK, statesK, sizeK);

		normalizeMatrix(matrixY);
		//normalizeMatrix(matrixK);

		stop = MPI_Wtime();
		double diff = stop - start;
		printf("Time: %lf\n", diff);
//		MPI_Abort(MPI_COMM_WORLD, 0);
		int finish = 1;
		for (int i = 1; i < size; i++)
		{
			MPI_Send(&finish, 1, MPI_INTEGER, i, 1, MPI_COMM_WORLD);
		}
	}
	else {
		float *buf = (float *)malloc(STATES * sizeof(float));
		int finish;
		MPI_Request req;
		MPI_Status status;
		MPI_Request workReq;
		MPI_Irecv(&finish, 1, MPI_INTEGER, 0, 1, MPI_COMM_WORLD, &req);
		MPI_Irecv(buf, STATES, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &workReq);
		int flag = 0;
		while (1) {
			flag = 0;
			MPI_Test(&workReq, &flag, MPI_STATUS_IGNORE);
			if (flag) {
				float sum = 0;
				for (int j = 0; j < STATES; j++) {
					sum += buf[j];
				}
				if (sum != 0) {
					for (int j = 0; j < STATES; j++) {
						buf[j] /= sum;
					}
				}
				MPI_Send(buf, STATES, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
				MPI_Irecv(buf, STATES, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &workReq);
			}

			flag = 0;
			MPI_Test(&req, &flag, MPI_STATUS_IGNORE);
			if (flag) break;
			
			Sleep(10);

		}
	}
	MPI_Finalize();
	
	return 0;
}


