#include "FloydAlgorithm.h"

#include <algorithm>

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
