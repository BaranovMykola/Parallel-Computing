#include "SymplexMethod.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <thread>
#include <future>

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
	try
	{

		do
		{

			//cout << "P0" << endl;
			//printVec(P0, m);
			//cout << "a" << endl;
			//printMatrix(a, m, n);
			//cout << "delta" << endl;
			//printVec(delta, n);	

			int minDeltaIndex = findMinDeltaIndex(delta, n);
			int k = minDeltaIndex;
			int r = findMainIndex(P0, a, m, k);
			Cb[r] = C[0][k];
	/*		cout << "Cb" << endl;
			printVec(Cb, m);*/
			if (delta[k] >= 0)
			{
				break;
			}
			double max = a[0][k];
			for (int i = 0; i < m; i++)
			{
				if (a[i][k] > max)
				{
					max = a[i][k];
				}
			}
			if (max <= 0)
			{
				return INT_MAX;
			}

			updateB(&P0, a, r, k, m);


			updateA(&a, m, n, r, k);
			updateDelta(delta, Cb, C, a, m, n);

			Vec functSeries = createMat(1, m)[0];
			std::transform(Cb, Cb + m, P0, functSeries, [](double a, double b) { return a*b; });
			//cout << "\t\tFOO=" << std::accumulate(functSeries, functSeries + n, 0.0) << endl;
			//cout << "==============================" << endl;
		}
		while (true);
	}
	catch (int)
	{

	}

	cout << "Stopped" << endl;

	//printMatrix(a, m, n);
	Vec functSeries = createMat(1, m)[0];
	std::transform(Cb, Cb + m, P0, functSeries, [](double a, double b) { return a*b; });
	return std::accumulate(functSeries, functSeries + m, 0.0);


}

double symplexMethodParallel(Mat C, Mat a, int n, int m)
{
	Vec P0 = extractVecB(a, n, m);
	Vec b = P0;
	Mat _Cb = createMat(1, m);
	generateMatrix(_Cb, 1, m, 0);
	Vec Cb = _Cb[0];

	Vec delta = createMat(1, n)[0];
	updateDelta(delta, Cb, C, a, m, n);
	try
	{
		do
		{

			/*cout << "P0" << endl;
			printVec(P0, m);
			cout << "a" << endl;
			printMatrix(a, m, n);
			cout << "delta" << endl;
			printVec(delta, n);*/

			int minDeltaIndex = findMinDeltaIndex(delta, n);
			int k = minDeltaIndex;
			int r = findMainIndex(P0, a, m, k);
			Cb[r] = C[0][k];
		/*	cout << "Cb" << endl;
			printVec(Cb, m);*/
			if (delta[k] >= 0)
			{
				break;
			};


			updateB(&P0, a, r, k, m);
			updateAParallel(&a, m, n, r, k, 4);

			updateDelta(delta, Cb, C, a, m, n);
			cout << "==============================" << endl;
			system("pause");
		}
		while (true);
	}
	catch (int)
	{

	}

	cout << "Stopped" << endl;

	//printMatrix(a, m, n);
	Vec functSeries = createMat(1, m)[0];
	std::transform(Cb, Cb + m, P0, functSeries, [](double a, double b) { return a*b; });
	return std::accumulate(functSeries, functSeries + m, 0.0);
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
		if (a[i][k] == 0)
		{
			ratio[i] = INT_MAX;
			continue;
		}
		ratio[i] = Cb[i] / a[i][k];
	}
	auto minIter = std::min_element(ratio, ratio + m);
	int index = std::distance(ratio, minIter);
	return index;
}

void updateB(Vec* P0, Mat a, int r, int k, int m)
{
	Vec b = *P0;
	Vec _b = createMat(1, m)[0];
	for (int i = 0; i < m; i++)
	{
		if (a[r][k] == 0)
		{
			throw 0;
		}
		_b[i] = i == r ? b[r] / a[r][k] : b[i] - (b[r] / a[r][k])*a[i][k];
	}
	*P0 = _b;
	// DEALOCATE
}

bool updateA(Mat* a, int m, int n, int r, int k, int from, int to)
{
	if (from == to)
	{
		from = 0;
		to = m;
	}
	Mat _a = createMat(m, n);
	for (int i = from; i < to; i++)
	{
		if ((*a)[r][k] == 0)
		{
			if (from != to)
			{
				return false;
			}
			throw 0;
		}
		for (int j = 0; j < n; j++)
		{
			_a[i][j] = i == r ? (*a)[r][j] / (*a)[r][k] : (*a)[i][j] - ((*a)[r][j] / (*a)[r][k])*(*a)[i][k];
		}
	}
	if (from == to)
	{
		deallocate(*a, m, n);
		*a = _a;
	}
	return true;
}

void updateAParallel(Mat* a, int m, int n, int r, int k, int p)
{
	std::vector<std::future<bool>> thrs;
	for (int i = 0; i < p; i++)
	{
		int from = (m / p)*i;
		int to = (m / p)*(i + 1);
		if (i + 1 == p)
		{
			to = p;
		}
		thrs.push_back(std::async(updateA,a, m, n, r, k, from, to));
		//thrs.push_back(std::thread(_partialAdd, m1, m2, r, rows, cols, from, to))
	}
	bool t = false;
	for (auto& i : thrs)
	{
		if(!i.get());
		{
			t = true;
		}
	}
	if (t) { throw 0; }
}