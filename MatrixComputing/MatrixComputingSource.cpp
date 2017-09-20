#include <iostream>
#include <thread>
#include <ctime>
#include <vector>

#include "Timer.h"
#include "MatrixProcessing.h"
#include "MatrixAdding.h"

typedef int** Mat;




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