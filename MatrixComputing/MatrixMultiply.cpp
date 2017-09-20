#include "MatrixMultiply.h"
#include "MatrixProcessing.h"
#include <vector>
#include <thread>
#include <future>

Mat multiply(Mat A, Mat B, int m, int n, int k)
{
	Mat C = createMat(m, k);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < k; j++)
		{
			double ij = 0;
			for (int _k = 0; _k < n; _k++)
			{
				ij += A[i][_k] * B[_k][j];
			}
			C[i][j] = ij;
		}
	}
	return C;
}

Mat multiplyParallelInvoke(Mat A, Mat B, int m, int n, int k, int p)
{
	Mat C = createMat(m, k);
	std::vector<std::thread> thrs;
	for (int i = 0; i < p; i++)
	{
		int from = (m / p)*i;
		int to = (m / p)*(i + 1);
		if (i + 1 == p)
		{
			to = m;
		}
		thrs.emplace_back(_partialMultMat, A, B, C, m, n, k, from, to);
	}
	for (auto& i : thrs)
	{
		i.join();
	}
	return C;
}

void _partialMultMat(Mat A, Mat B, Mat C, int m, int n, int k, int mFrom, int mTo)
{
	for (int i = mFrom; i < mTo; i++)
	{
		for (int j = 0; j < k; j++)
		{
			double ij = 0;
			for (int _k = 0; _k < n; _k++)
			{
				ij += A[i][_k] * B[_k][j];
			}
			C[i][j] = ij;
		}
	}
}