#include "MatrixAdding.h"
#include <iostream>
#include "Timer.h"
#include <vector>
#include <thread>
#include <future>

void add(Mat m1, Mat m2, Mat& r, int rows, int cols, int op(int, int))
{
	r = createMat(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			r[i][j] = op(m1[i][j], m2[i][j]);
		}
	}
}

void _partialAdd(Mat m1, Mat m2, Mat r, int rows, int cols, int from, int to, int op(int, int))
{
	for (int i = from; i < to; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			r[i][j] = op(m1[i][j], m2[i][j]);
		}
	}
}

void addParallelInvoke(Mat m1, Mat m2, Mat&r, int rows, int cols, int t, int op(int, int))
{
	std::cout << "Creating result matrix... ";
	r = createMat(rows, cols);
	generateMatrix(r, rows, cols, 0);
	std::cout << "Matrix created. Threading started with [" << t << "] threads:\t";

	Timer::reset();
	std::vector<std::thread> thrs;
	for (int i = 0; i < t; i++)
	{
		int from = (rows / t)*i;
		int to = (rows / t)*(i + 1);
		if (i + 1 == t)
		{
			to = rows;
		}
		thrs.emplace_back(_partialAdd, m1, m2, r, rows, cols, from, to, op);
		//thrs.push_back(std::thread(_partialAdd, m1, m2, r, rows, cols, from, to))
	}
	for (auto& i : thrs)
	{
		i.join();
	}
	Timer::show();
}

bool checkAdding(Mat r, int rows, int cols, int val)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (r[i][j] != val)
			{
				return false;
			}
		}
		delete r[i];
	}
	delete r;
	return true;
}

int addOp(int a, int b) { return a + b; }
int subOp(int a, int b) { return a - b; }