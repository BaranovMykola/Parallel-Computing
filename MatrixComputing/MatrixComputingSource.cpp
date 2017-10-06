#include <iostream>
#include <thread>
#include <ctime>
#include <vector>

#include "Timer.h"
#include "MatrixProcessing.h"
#include "MatrixAdding.h"
#include "MatrixMultiply.h"
#include "GaussianMethod.h"
#include "SymplexMethod.h"


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
	int m = 800;
	int n = 800;
	int k = 800;
	Mat A = createMat(m, n);
	Mat B = createMat(n, k);
	
	generateMatrix(A, m, n);
	generateMatrix(B, n, k);

	std::cout << "Multplying matrix successively: ";
	Timer::reset();
	Mat C_ord = multiply(A, B, m,n,k);
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

int GaussianMethodSample()
{
	int n = 100;
		Timer::reset();
		std::cout << "Creating system " << n << " x " << n << " ";
		Mat A = createMat(n, n);
		Mat B = createMat(n, 1);
		generateMatrix(A, n, n);
		generateMatrix(B, n, 1);
		Mat A_paral = createMat(n, n);
		Mat B_paral = createMat(n, 1);
		generateMatrix(A_paral, n, n);
		generateMatrix(B_paral, n, 1);
		Timer::show();
		Timer::reset();
		std::cout << "Started solution process " << std::endl;
		std::thread ord(solveGaussian, A, B, n);
		std::thread paral(solveGaussianParallel, A_paral, B_paral, n, 4);
		ord.join();
		paral.join();
		Timer::show();
		system("pause");
	return 0;
}

int symplexMethodSample()
{
	int dim = 2000;
	int cond = 1000;
	Mat C = createMat(1, dim);
	Mat p = createMat(cond, dim+1);
	generateMatrix(C, 1, dim+1,0);
	generateMatrix(C, 1, dim-cond);
	generateMatrix(p, cond, dim);

	//printMatrix(C, 1, dim);
	double ord = 0;
	Timer::reset();
	ord = symplexMethod(C, p, dim, cond);
	Timer::show();

	C = createMat(1, dim);
	p = createMat(cond, dim + 1);
	generateMatrix(C, 1, dim, 0);
	generateMatrix(C, 1, dim-cond);
	generateMatrix(p, cond, dim+1);
	Timer::reset();
	auto paral = symplexMethodParallel(C, p, dim + 1, cond);
	Timer::show();
	//std::cout << "Correct: " << std::boolalpha << (paral == ord) << std::endl;
	std::cout << "Ord: " << ord << std::endl;
	
	system("pause");
	return 0;
}

int main()
{
	return symplexMethodSample();
}