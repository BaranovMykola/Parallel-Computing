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


double searchMaxMinParallel(Mat mat, int r, int c, int p)
{
	std::vector<std::future<double>> split;
	double step = r / (double)p;
	for (int i = 0; i < p; i++)
	{
		int from = i*step;
		int to = (i + 1)*step;
		int extraP = (p - r) / r;
		split.push_back(std::async(static_cast<double(*)(Mat,int,int,int,int,int)>(searchMaxMinParallel), mat,r,c, from, to,extraP+1));
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

double searchMaxMinParallel(Mat mat, int r, int c, int from, int to, int p)
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

