#pragma once
#include "MatrixProcessing.h"
#include "MatrixMultiply.h"

void makeIMat(Mat mat, int rows, int cols);

void makeTMat(Mat T, Mat A, int n, int k);

Mat solveGaussian(Mat A, Mat B, int n);

Mat solveGaussianParallel(Mat A, Mat B, int n, int p);

