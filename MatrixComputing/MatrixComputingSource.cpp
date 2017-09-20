#include <iostream>
#include <thread>
#include <ctime>
#include <vector>

#include "Timer.h"
#include "MatrixProcessing.h"
#include "MatrixAdding.h"
#include "MatrixMultiply.h"

typedef int** Mat;


int matrixAddingSample()
{
	int rows = 5000;
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

int matrixMultiplingSample()
{
	int m = 4000;
	int n = 4000;
	int k = 4000;
	Mat A = createMat(m, n);
	Mat B = createMat(n, k);
	
	generateMatrix(A, m, n);
	generateMatrix(B, n, k);

	std::cout << "Multplying matrix successively: ";
	Timer::reset();
	//Mat C_ord = multiply(A, B, m,n,k);
	Timer::show();
	for (int p = 4; p < 100; p++)
	{
		if (p > 10)
		{
			p += 4;
		}
		std::cout << "Multplying matrix parallel in [" << p << "]threads: ";
		Timer::reset();
		Mat C_paral = multiplyParallelInvoke(A, B, m, n, k, p);
		Timer::show();
		//std::cout << "Processing success: " << std::boolalpha << isMatrixEqual(C_ord, C_paral, m,k) << std::endl;
		deallocate(C_paral,m,k);
	}


	std::getchar();
	return 0;
}

int main()
{
	matrixMultiplingSample();
}