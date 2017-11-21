#pragma once
#ifndef PREDICTION_H
#define PREDICTION_H

#define PREDICT_LAST 200

void construct_and_predict(int * statesY, int sizeY, float *matrixY, int *statesK, int sizeK, float *matrixK, float *testValues, float *borders);
void predict(float *matrix, int **states, int n);
void calculateError(float *predicted, float *actual, int n, float **output);

#endif // !PREDICTION_H
