#include "SymplexMethod.h"
#include <iostream>
#include <algorithm>
#include <numeric>

using std::cout;
using std::endl;

double symplexMethod(Mat C, Mat a, int n, int m) 
{
	Vec P0 = extractVecB(a, n, m);
	Vec b = P0;
	Mat _Cb = createMat(1, m);
	generateMatrix(_Cb, 1, m, 0);
	Vec Cb = _Cb[0];

	Vec delta = createMat(1, n)[0];
	updateDelta(delta, Cb, C, a, m, n);
	do
	{

		cout << "P0" << endl;
		printVec(P0, m);
		cout << "a" << endl;
		printMatrix(a, m, n);
		cout << "delta" << endl;
		printVec(delta, n);	

		int minDeltaIndex = findMinDeltaIndex(delta, n);
		int k = minDeltaIndex;
		int r = findMainIndex(P0, a, m, k);
		Cb[r] = C[0][k];
		cout << "Cb" << endl;
		printVec(Cb, m);
		if (delta[k] >= 0)
		{
			break;
		};

		updateB(P0, a, r, k, m);

		//printVec(P0, m);

		updateA(a, m, n, r, k);
		updateDelta(delta, Cb, C, a, m, n);
		cout << "==============================" << endl;
	}
	while (true);

	cout << "Stopped" << endl;

	//printMatrix(a, m, n);
	Vec functSeries = createMat(1, m)[0];
	std::transform(Cb, Cb + m, P0, functSeries, [](double a, double b) { return a*b; });
	return std::accumulate(functSeries, functSeries + n, 0.0);


}


Vec extractVecB(Mat a, int n, int m)
{
	Vec b = new double[m];
	for (int i = 0; i < m; i++)
	{
		b[i] = a[i][n];
	}
	return b;
}

void updateDelta(Vec delta, Vec Cb, Mat C, Mat a, int m, int n)
{
	for (int j = 0; j < n; j++)
	{
		delta[j] = 0;
		for (int i = 0; i < m; i++)
		{
			delta[j] += Cb[i] * a[i][j];
		}
		delta[j] -= C[0][j];
	}
}

int findMinDeltaIndex(Vec delta, int n)
{
	auto minIter = std::min_element(delta, delta + n);
	int index = std::distance(delta, minIter);
	return index;
}

int findMainIndex(Vec Cb, Mat a, int m, int k)
{
	Vec ratio = createMat(1, m)[0];
	for (int i = 0; i < m; i++)
	{
		ratio[i] = Cb[i] / a[i][k];
	}
	auto minIter = std::min_element(ratio, ratio + m);
	int index = std::distance(ratio, minIter);
	return index;
}

void updateB(Vec& P0, Mat a, int r, int k, int m)
{
	Vec b = P0;
	Vec _b = createMat(1, m)[0];
	for (int i = 0; i < m; i++)
	{
		_b[i] = i == r ? b[r] / a[r][k] : b[i] - (b[r] / a[r][k])*a[i][k];
	}
	P0 = _b;
	// DEALOCATE
}

void updateA(Mat& a, int m, int n, int r, int k)
{
	Mat _a = createMat(m, n);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			_a[i][j] = i == r ? a[r][j] / a[r][k] : a[i][j] - (a[r][j] / a[r][k])*a[i][k];
		}
	}
	deallocate(a, m, n);
	a = _a;
}