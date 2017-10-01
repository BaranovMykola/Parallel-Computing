#pragma once
typedef double** Mat;

Mat createMat(int rows, int cols);

void generateMatrix(Mat mat, int rows, int cols);

void generateMatrix(Mat mat, int rows, int cols, int value);

void printMatrix(Mat mat, int rows, int cols);

bool isMatrixEqual(Mat A, Mat B, int m, int n);

void deallocate(Mat A, int m, int n);
