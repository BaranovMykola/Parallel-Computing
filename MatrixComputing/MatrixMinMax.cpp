#include "MatrixMinMax.h"
#include <algorithm>
#include <vector>
#include <future>
#include <thread>
#include <iterator>
#include <list>

double _maxMinSearch(Mat mat, int rFrom, int rTo, int cFrom, int cTo)
{
	Vec min = createMat(1, rTo-rFrom)[0];
	//double* min = new double[rTo - rFrom];
	for (int i = rFrom; i < rTo; i++)
	{
		min[i-rFrom] = mat[i][0];
		for (int j = cFrom; j < cTo; j++)
		{
			if (mat[i][j] < min[i-rFrom])
			{
				min[i-rFrom] = mat[i][j];
			}
		}
	}
	auto m = *std::max_element(min, min + rTo - rFrom);
	delete[] min;
	return 0;
}

double maxMinSearch(Mat mat, int r, int c)
{
	return _maxMinSearch(mat, 0, r, 0, c);
}

void foo() {}

void maxMinSearchPart(Mat mat, int rFrom, int rTo, int cFrom, int cTo, double* res)
{
	*res = _maxMinSearch(mat, rFrom, rTo, cFrom, cTo);
}

double minSearchParallel(Mat mat, int r, int c, int p)
{
	printMatrix(mat, r, c);
	int w = sqrt(p*(r / (double)c));
	int h = w*(c / (double)r);

	std::list<std::thread> blocks;
	double* results = new double[w*h];
	int k = 0;
	int rStep = r / h;
	int cStep = c / w;
	for (int i = 0; i < h; i++)
	{
		int rFrom = i*rStep;
		int rTo = (i + 1)*rStep;
		if (i + 1 == h)
		{
			rTo = r;
		}
		for (int j = 0; j < w; j++)
		{
			int cFrom = j*cStep;
			int cTo = (j + 1)*cStep;
			if (j + 1 == w)
			{
				rTo = r;
			}
			blocks.emplace_back(maxMinSearchPart,mat, rFrom, rTo, cFrom, cTo, &results[k++]);
		}
	}

	for (auto& i : blocks)
	{
		i.join();
	}
	auto min = *std::min_element(results, results + w*h);
	delete[] results;
	return min;
}


double searchMaxMin(Mat mat, int r, int c, int p)
{
	std::vector<std::future<double>> split;
	double step = r / (double)p;
	for (int i = 0; i < p; i++)
	{
		int from = i*step;
		int to = (i + 1)*step;
		int extraP = (p - r) / r;
		split.push_back(std::async(static_cast<double(*)(Mat,int,int,int,int,int)>(searchMaxMin), mat,r,c, from, to,extraP+1));
	}
	Vec maxs = createMat(1, p)[0];
	Vec ins = maxs;
	for (auto& i : split)
	{
		*(ins++) = i.get();
	}
	auto maxElem = *std::max_element(maxs, maxs+ p);
	delete[]maxs;
	return maxElem;
}

double searchMaxMin(Mat mat, int r, int c, int from, int to, int p)
{
	Vec mins = createMat(1, to - from)[0];
	Vec ins = mins;
	for (int i = from; i < to; i++)
	{
		*(ins++) = searchMin(mat[i], c, p);
	}
	auto maxElem = *std::max_element(mins, mins + to - from);
	delete[] mins;
	return maxElem;
}


double searchMin(Vec row, int c, int p)
{
	std::vector<std::future<double>> split;
	double step = c / (double)p;
	for (int i = 0; i < p; i++)
	{
		int from = i*step;
		int to = (i + 1)*step;
		split.push_back(std::async(_searchMin, row, from, to));
	}
	Vec mins = createMat(1, p)[0];
	Vec ins = mins;
	for (auto& i : split)
	{
		*(ins++) = i.get();
	}
	auto minElem = *std::min_element(mins, mins + p);
	delete[]mins;
	return minElem;
}

double _searchMin(Vec row, int from, int to)
{
	double min = row[from];
	for (int i = from; i < to; i++)
	{
		if (min > row[i])
		{
			min = row[i];
		}
	}
	return min;
}

