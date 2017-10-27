#include "FloydAlgorithm.h"

#include <algorithm>
#include <vector>
#include <thread>

Mat Floyd(Mat W, int _k)
{
	for (int k = 0; k < _k; k++)
	{
		Mat Wk = createMat(_k, _k);
		for (int i = 0; i < _k; i++)
		{
			for (int j = 0; j < _k; j++)
			{
				Wk[i][j] = std::min({ W[i][k] + W[k][j], W[i][j] });
			}
		}
		deallocate(W, _k, _k);
		W = Wk;
	}
	return W;
}

Mat FloydParallel(Mat W, int _k, int p)
{
	for (int k = 0; k < _k; k++)
	{
		Mat Wk = createMat(_k, _k);
		std::vector<std::thread> thrs;
		double h = _k / (double)p;
		for (double i = 0; i < p; i++)
		{
			thrs.emplace_back(_FloydPart, W, Wk, _k, k, i*h, (i + 1)*h);
		}

		for (auto& i : thrs)
		{
			i.join();
		}
		deallocate(W, _k, _k);
		W = Wk;
	}
	return W;
}

void _FloydPart(Mat W, Mat Wk, int _k, int k, int from, int to)
{
	for (int i = from; i < to; i++)
	{
		for (int j = 0; j < _k; j++)
		{
			Wk[i][j] = std::min({ W[i][k] + W[k][j], W[i][j] });
		}
	}
}
