#include <iostream>
#include <thread>
#include <ctime>
#include <vector>

#include "Timer.h"

typedef int** Mat;

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

void _partialAdd(Mat m1, Mat m2, Mat r, int rows, int cols, int from, int to, int op(int,int))
{
	for (int i = from; i < to; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			r[i][j] = op(m1[i][j], m2[i][j]);
		}
	}
}

void addParallelInvoke(Mat m1, Mat m2, Mat&r, int rows, int cols, int t, int op(int,int))
{
	std::cout << "Creating result matrix... ";
	r = createMat(rows, cols);
	generateMatrix(r, rows, cols, 0);
	std::cout << "Matrix created. Threading started with ["<<t<<"] threads:\t";

	Timer::reset();
	std::vector<std::thread> thrs;
	for (int i = 0; i < t; i++)
	{
		int from = (rows / t)*i;
		int to = (rows / t)*(i+1);
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
void foo();
void bar(void funct());

int addOp(int a, int b) { return a + b; }
int subOp(int a, int b) { return a - b; }

int main()
{
	int rows=5000;
	int cols = 5000;
	int threads = 2;
	Timer::reset();
	std::cout << "Creating matrix " << rows << "x" << cols << ":\t";
	Mat m1 = createMat(rows, cols);
	Mat m2 = createMat(rows, cols);
	generateMatrix((Mat)m1, rows, cols, 0);
	generateMatrix((Mat)m2, rows, cols, 1);
	Timer::show();
	std::cout << std::endl;
	
	Timer::reset();
	Mat r;
	std::cout << "Adding matrix successively:\t";
	add(m1, m2, r, rows, cols, addOp);
	Timer::show();
	std::cout << std::endl;

	for (; threads < 50; threads++)
	{
		Mat rAsync;
		addParallelInvoke(m1, m2, rAsync, rows, cols, threads, addOp);
		std::cout << std::endl;
		bool isAddSuccess = checkAdding(rAsync, rows, cols, 1);
		if (!isAddSuccess)
		{
			std::cout << "Error while adding: " << std::endl;
		}
	}

	std::getchar();
	return 0;
}

void foo()
{
	std::cout << "foo" << std::endl;
}

void bar(void funct())
{
	funct();
}