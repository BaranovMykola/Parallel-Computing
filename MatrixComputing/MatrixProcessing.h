#pragma once
typedef int** Mat;

Mat createMat(int rows, int cols);

void generateMatrix(Mat mat, int rows, int cols);

void generateMatrix(Mat mat, int rows, int cols, int value);

void printMatrix(Mat mat, int rows, int cols);
