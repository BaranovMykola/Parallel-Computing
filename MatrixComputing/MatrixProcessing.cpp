#include "MatrixProcessing.h"
#include "stdlib.h"
#include <iostream>

Mat createMat(int rows, int cols)
{
	Mat m = new int*[rows];
	for (int i = 0; i < rows; i++)
	{
		m[i] = new int[cols];
	}
	return m;
}

void generateMatrix(Mat mat, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			mat[i][j] = rand() % 10;
		}
	}
}

void generateMatrix(Mat mat, int rows, int cols, int value)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			mat[i][j] = value;
		}
	}
}

void printMatrix(Mat mat, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
}