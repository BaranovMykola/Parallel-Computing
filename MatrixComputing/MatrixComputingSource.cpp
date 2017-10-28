//#include <iostream>
//#include <thread>
//#include <ctime>
//#include <vector>
//
//#include "Timer.h"
//#include "MatrixProcessing.h"
//#include "MatrixAdding.h"
//#include "MatrixMultiply.h"
//#include "GaussianMethod.h"
//#include "SymplexMethod.h"
//#include "MatrixMinMax.h"
//#include "FloydAlgorithm.h"
//#include "Prima.h"
//
//int matrixAddingSample()
//{
//	int rows = 5000;
//	int cols = 5000;
//	int threads = 2;
//	Timer::reset();
//	std::cout << "Creating matrix " << rows << "x" << cols << ":\t";
//	Mat m1 = createMat(rows, cols);
//	Mat m2 = createMat(rows, cols);
//	generateMatrix((Mat)m1, rows, cols, 0);
//	generateMatrix((Mat)m2, rows, cols, 1);
//	Timer::show();
//	std::cout << std::endl;
//
//	Timer::reset();
//	Mat r;
//	std::cout << "Adding matrix successively:\t";
//	add(m1, m2, r, rows, cols, addOp);
//	Timer::show();
//	std::cout << std::endl;
//
//	for (; threads < 50; threads++)
//	{
//		Mat rAsync;
//		addParallelInvoke(m1, m2, rAsync, rows, cols, threads, addOp);
//		std::cout << std::endl;
//		bool isAddSuccess = checkAdding(rAsync, rows, cols, 1);
//		if (!isAddSuccess)
//		{
//			std::cout << "Error while adding: " << std::endl;
//		}
//	}
//	std::getchar();
//	return 0;
//}
//
//int matrixMultiplingSample()
//{
//	int m = 800;
//	int n = 800;
//	int k = 800;
//	Mat A = createMat(m, n);
//	Mat B = createMat(n, k);
//	
//	generateMatrix(A, m, n);
//	generateMatrix(B, n, k);
//
//	std::cout << "Multplying matrix successively: ";
//	Timer::reset();
//	Mat C_ord = multiply(A, B, m,n,k);
//	Timer::show();
//	for (int p = 4; p < 100; p++)
//	{
//		if (p > 10)
//		{
//			p += 4;
//		}
//		std::cout << "Multplying matrix parallel in [" << p << "]threads: ";
//		Timer::reset();
//		Mat C_paral = multiplyParallelInvoke(A, B, m, n, k, p);
//		Timer::show();
//		//std::cout << "Processing success: " << std::boolalpha << isMatrixEqual(C_ord, C_paral, m,k) << std::endl;
//		deallocate(C_paral,m,k);
//	}
//
//
//	std::getchar();
//	return 0;
//}
//
//int GaussianMethodSample()
//{
//	int n = 100;
//		Timer::reset();
//		std::cout << "Creating system " << n << " x " << n << " ";
//		Mat A = createMat(n, n);
//		Mat B = createMat(n, 1);
//		generateMatrix(A, n, n);
//		generateMatrix(B, n, 1);
//		Mat A_paral = createMat(n, n);
//		Mat B_paral = createMat(n, 1);
//		generateMatrix(A_paral, n, n);
//		generateMatrix(B_paral, n, 1);
//		Timer::show();
//		Timer::reset();
//		std::cout << "Started solution process " << std::endl;
//		std::thread ord(solveGaussian, A, B, n);
//		std::thread paral(solveGaussianParallel, A_paral, B_paral, n, 4);
//		ord.join();
//		paral.join();
//		Timer::show();
//		system("pause");
//	return 0;
//}
//
//int symplexMethodSample()
//{
//	int dim = 2000;
//	int cond = 1000;
//	Mat C = createMat(1, dim);
//	Mat p = createMat(cond, dim+1);
//	generateMatrix(C, 1, dim+1,0);
//	generateMatrix(C, 1, dim-cond);
//	generateMatrix(p, cond, dim);
//
//	Timer::reset();
//	std::cout << "Ordinary symplex method... ";
//	symplexMethod(C, p, dim, cond);
//	Timer::show();
//
//	C = createMat(1, dim);
//	p = createMat(cond, dim + 1);
//	generateMatrix(C, 1, dim, 0);
//	generateMatrix(C, 1, dim-cond);
//	generateMatrix(p, cond, dim+1);
//	for (int t = 2; t < 10; t++)
//	{
//		std::cout << "Parallel symple method in " << t << " threads... ";
//		Timer::reset();
//		symplexMethodParallel(C, p, dim + 1, cond, t);
//		Timer::show();
//	}
//	system("pause");
//	return 0;
//}
//
//int minMaxSearchSample()
//{
//	int r = 10000;
//	int c = 10000;
//	Timer::reset();
//	std::cout << "Creating matrix " << r << "x" << c << std::endl;
//	Mat mat = createMat(r,c);
//	generateMatrix(mat, r,c);
//	std::cout << "Created. ";
//	Timer::show();
//	for (int p = 1; p < 30; p++)
//	{
//		Timer::reset();
//		std::cout << "[" << p << "] thread..." << std::endl;
//		std::cout << "Max min = " << searchMaxMinParallel(mat, r, c, p) << std::endl;
//		Timer::show();
//	}
//	system("pause");
//	return 0;
//}
//
//int FloydSample()
//{
//	Mat W = createMat(4, 4);
//	W[0] = new double[4]{ 0,-2,3,-3 };
//	W[1] = new double[4]{ INT_MAX,0,2,INT_MAX };
//	W[2] = new double[4]{ INT_MAX,INT_MAX,0,-3 };
//	W[3] = new double[4]{ 4,5,5,0 };
//	printMatrix(W, 4, 4);
//	auto Wk = FloydParallel(W, 4, 4);
//	printMatrix(Wk, 4, 4);
//	system("pause");
//	return 0;
//}
//
//int PrimaSample()
//{
//	int k = 100;
//	Mat g = createMat(k,k);
//	generateMatrix(g, k, k);
//	/*g[0] = new double[7]{ 0,7,0,5,0,0,0 };
//	g[1] = new double[7]{ 7,0,8,9,7,0,0 };
//	g[2] = new double[7]{ 0,8,0,0,5,0,0 };
//	g[3] = new double[7]{ 5,9,0,0,15,6,0 };
//	g[4] = new double[7]{ 0,7,5,15,0,8,9 };
//	g[5] = new double[7]{ 0,0,0,6,8,0,11 };
//	g[6] = new double[7]{ 0,0,0,0,9,11,0 };*/
//	Timer::reset();
//	Prima(g, k);
//	Timer::show();
//	Timer::reset();
//	PrimaParallel(g, k, 4);
//	Timer::show();
//
//	system("pause");
//	return 0;
//}
//
//int main()
//{
//	return PrimaSample();
//}


#include <iostream>
#include <vector>
#include <string>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

// Compute c = a + b.
static const char source[] =
"#if defined(cl_khr_fp64)\n"
"#  pragma OPENCL EXTENSION cl_khr_fp64: enable\n"
"#elif defined(cl_amd_fp64)\n"
"#  pragma OPENCL EXTENSION cl_amd_fp64: enable\n"
"#else\n"
"#  error double precision is not supported\n"
"#endif\n"
"kernel void add(\n"
"       ulong n,\n"
"       global const double *a,\n"
"       global const double *b,\n"
"       global double *c,\n"
"       ulong k\n"
"       )\n"
"{\n"
"    size_t i = get_global_id(0);\n"
//"    if (i+1 <= n) {\n"
//"       c[i*(int)sqrt((float)n)+i] = c[i*(int)sqrt((float)n)+i]+1;"
"int n2 = sqrt((float)n);"
"int _i = ((i)/n2);"
"int _j = (i%n2);"
"c[_i*n2+_j] = a[_i*n2+k]+a[k*n2+_j] < a[_i*n2+_j] ? a[_i*n2+k]+a[k*n2+_j] : a[_i*n2+_j];"
//"    }\n"
"}\n";

int main()
{
	const size_t N = 16;

	try
	{
		// Get list of OpenCL platforms.
		std::vector<cl::Platform> platform;
		cl::Platform::get(&platform);

		if (platform.empty())
		{
			std::cerr << "OpenCL platforms not found." << std::endl;
			return 1;
		}

		// Get first available GPU device which supports double precision.
		cl::Context context;
		std::vector<cl::Device> device;
		for (auto p = platform.begin(); device.empty() && p != platform.end(); p++)
		{
			std::vector<cl::Device> pldev;

			try
			{
				p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

				for (auto d = pldev.begin(); device.empty() && d != pldev.end(); d++)
				{
					if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;

					std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();

					if (
						ext.find("cl_khr_fp64") == std::string::npos &&
						ext.find("cl_amd_fp64") == std::string::npos
						) continue;

					device.push_back(*d);
					context = cl::Context(device);
				}
			}
			catch (...)
			{
				device.clear();
			}
		}

		if (device.empty())
		{
			std::cerr << "GPUs with double precision not found." << std::endl;
			return 1;
		}

		std::cout << device[0].getInfo<CL_DEVICE_NAME>() << std::endl;

		// Create command queue.
		cl::CommandQueue queue(context, device[0]);

		// Compile OpenCL program for found device.
		cl::Program program(context, cl::Program::Sources(
			1, std::make_pair(source, strlen(source))
		));

		try
		{
			program.build(device);
		}
		catch (const cl::Error&)
		{
			std::cerr
				<< "OpenCL compilation error" << std::endl
				<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0])
				<< std::endl;
			system("pause");
			return 1;
		}

		cl::Kernel add(program, "add");

		// Prepare input data.
		//	W[0] = new double[4]{ 0,-2,3,-3 };
		//	W[1] = new double[4]{ INT_MAX,0,2,INT_MAX };
		//	W[2] = new double[4]{ INT_MAX,INT_MAX,0,-3 };
		//	W[3] = new double[4]{ 4,5,5,0 };
		std::vector<double> a{ 0,-2,3,-3,INT_MAX,0,2,INT_MAX ,INT_MAX ,INT_MAX ,0,-3,4,5,5,0 };
		std::vector<double> b(N, 1);
		std::vector<double> c(N);
		for (int z = 0; z < 4; z++)
		{


		// Allocate device buffers and transfer input data to device.
		cl::Buffer A(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					 a.size() * sizeof(double), a.data());

		cl::Buffer B(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					 b.size() * sizeof(double), b.data());

		cl::Buffer C(context, CL_MEM_READ_WRITE,
					 c.size() * sizeof(double));

		// Set kernel parameters.
		add.setArg(0, static_cast<cl_ulong>(N));
		add.setArg(1, A);
		add.setArg(2, B);
		add.setArg(3, C);
		add.setArg(4, static_cast<cl_ulong>(z));

		// Launch kernel on the compute device.
		queue.enqueueNDRangeKernel(add, cl::NullRange, N, cl::NullRange);

		// Get result back to host.
		queue.enqueueReadBuffer(C, CL_TRUE, 0, c.size() * sizeof(double), c.data());

		// Should get '3' here.
		for (int i = 0;i < N;++i)
		{
			std::cout << c[i] << " ";
			if ((i+1)%(int)std::sqrt(N) == 0)
			{
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;
		a = c;
	}
		}
	catch (const cl::Error &err)
	{
		std::cerr
			<< "OpenCL error: "
			<< err.what() << "(" << err.err() << ")"
			<< std::endl;
	//	return 1;
	}
	system("pause");
}