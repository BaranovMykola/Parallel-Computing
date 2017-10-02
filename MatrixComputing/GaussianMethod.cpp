#include "GaussianMethod.h"
#include <iostream>
#include <future>
#include "Timer.h"

using std::cout;
using std::endl;

void makeIMat(Mat mat, int rows, int cols)
{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				mat[i][j] = i == j ? 1 : 0;
			}
		}
}

void makeTMat(Mat T, Mat A, int n, int k)
{
	T[k][k] = 1 / A[k][k];
	for (int i = k + 1; i < n; i++)
	{
		T[i][k] = -A[i][k] / A[k][k];
	}
}

Mat makeVMat(Mat V, Mat U, int n, int k)
{
	for (int i = 0; i < k; i++)
	{
		V[i][k] = -U[i][k];
	}
	return V;
}

Mat solveGaussian(Mat A, Mat B, int n)
{
	Mat T = createMat(n, n);
	int last = 0;
	//cout << "Forward process" << endl;
	int c = 0;
	for (int i = 0; i < n; i++)
	{
		cout << "\t\t[Ordinary]\t" << c++ << " iteration " << endl;
		makeIMat(T, n, n);
		makeTMat(T, A, n, i);
		Mat Ak = multiply(T, A, n, n, n);
		Mat Bk = multiply(T, B, n, n, 1);
		deallocate(A, n, n);
		deallocate(B, n, n);
		A = Ak;
		B = Bk;
		/*Timer::show();
		cout << "\t\t+" << (Timer::check() - last) << endl;
		last = Timer::check();*/
	}
	Mat U = A;
	Mat C = B;
	Mat V = createMat(n, n);
	for (int i = n-1; i >= 1; i--)
	{
		cout << "\t\t[Ordinary]\t" << c++ << " iteration " << endl;
		makeIMat(V, n, n);
		makeVMat(V, U, n, i);
		Mat Uk = multiply(V, U, n, n, n);
		Mat Ck = multiply(V, C, n, n, 1);
		deallocate(U,n,n);
		deallocate(C,n,n);
		U = Uk;
		C = Ck;
		/*Timer::show();
		cout << "\t\t+" << (Timer::check() - last) << endl;
		last = Timer::check();*/
	}
	return C;
}

Mat solveGaussianParallel(Mat A, Mat B, int n, int p)
{
	Mat T = createMat(n, n);
	int last = 0;
	int c = 0;
	for (int i = 0; i < n; i++)
	{
		cout << "[Parallel]\t" << c++ << " iteration" << endl;
		makeIMat(T, n, n);
		makeTMat(T, A, n, i);
		/*auto AkT = std::async(multiply, T, A, n, n, n);
		auto BkT = std::async(multiply, T, B, n, n, 1);*/
		Mat Ak = multiplyParallelInvoke(T, A, n, n, n, p);
		Mat Bk = multiplyParallelInvoke(T, B, n, n, 1,p);
		/*Mat Bk = BkT.get();
		Mat Ak = AkT.get();*/

		std::thread(deallocate,A, n, n).detach();
		std::thread(deallocate,B, n, n).detach();
		A = Ak;
		B = Bk;
		/*Timer::show();
		cout << "\t\t+" << (Timer::check() - last) << endl;
		last = Timer::check();*/
	}
	Mat U = A;
	Mat C = B;
	Mat V = createMat(n, n);
	for (int i = n - 1; i >= 1; i--)
	{
		cout << "[Parallel]\t" << c++ << " iteration" << endl;
		makeIMat(V, n, n);
		makeVMat(V, U, n, i);
		/*auto UkT = std::async(multiply, V, U, n, n, n);
		auto CkT = std::async(multiply, V, C, n, n, 1);
		Mat Ck = CkT.get();
		Mat Uk = UkT.get();*/
		Mat Uk = multiplyParallelInvoke(V, U, n, n, n, p);
		Mat Ck = multiplyParallelInvoke(V, C, n, n, 1,p);
		std::thread(deallocate,U, n, n).detach();
		std::thread(deallocate,C, n, n).detach();
		U = Uk;
		C = Ck;
		/*Timer::show();
		cout << "\t\t+" << (Timer::check() - last) << endl;
		last = Timer::check();*/
	}
	return C;
}