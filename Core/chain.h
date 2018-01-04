#pragma once
#ifndef CHAIN_H
#define CHAIN_H

int fillMatrix(float * matrix, int *data, int n);
void normalizeMatrix(float *matrix);
void prepareNormalizationKernel();
void prepareFillMatrixKernel();
void releaseNormalizationKernel();
void releaseFillMatrixKernel();

#endif // !CHAIN_H
