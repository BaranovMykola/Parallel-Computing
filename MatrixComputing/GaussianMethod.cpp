#include "GaussianMethod.h"
#include <iostream>

using std::cout;
using std::endl;

void makeIMat(Mat mat, int rows, int cols)
{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				mat[i][j] = i == j ? 1 : 0;
			}
		}
}

void makeTMat(Mat T, Mat A, int n, int k)
{
	T[k][k] = 1 / A[k][k];
	for (int i = k + 1; i < n; i++)
	{
		T[i][k] = -A[i][k] / A[k][k];
	}
}

Mat makeVMat(Mat V, Mat U, int n, int k)
{
	for (int i = 0; i < k; i++)
	{
		V[i][k] = -U[i][k];
	}
	return V;
}

Mat solveGaussian(Mat A, Mat B, int n)
{
	Mat T = createMat(n, n);
	for (int i = 0; i < n; i++)
	{
		makeIMat(T, n, n);
		makeTMat(T, A, n, i);
		Mat Ak = multiply(T, A, n, n, n);
		Mat Bk = multiply(T, B, n, n, 1);
		deallocate(A, n, n);
		deallocate(B, n, n);
		A = Ak;
		B = Bk;
	}
	Mat U = A;
	Mat C = B;
	Mat V = createMat(n, n);
	for (int i = n-1; i >= 1; i--)
	{
		makeIMat(V, n, n);
		makeVMat(V, U, n, i);
		Mat Uk = multiply(V, U, n, n, n);
		Mat Ck = multiply(V, C, n, n, 1);
		deallocate(U,n,n);
		deallocate(C,n,n);
		U = Uk;
		C = Ck;
	}
	return C;
}
