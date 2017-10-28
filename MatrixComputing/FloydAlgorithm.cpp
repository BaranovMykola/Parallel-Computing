#include "FloydAlgorithm.h"

#include <algorithm>
#include <vector>
#include <thread>

Mat Floyd(Mat W, int _k)
{
	for (int k = 0; k < _k; k++)
	{
		Mat Wk = createMat(_k, _k);
		for (int i = 0; i < _k; i++)
		{
			for (int j = 0; j < _k; j++)
			{
				Wk[i][j] = std::min({ W[i][k] + W[k][j], W[i][j] });
			}
		}
		deallocate(W, _k, _k);
		W = Wk;
	}
	return W;
}

Mat FloydParallel(Mat W, int _k, int p)
{
	for (int k = 0; k < _k; k++)
	{
		Mat Wk = createMat(_k, _k);
		std::vector<std::thread> thrs;
		double h = _k / (double)p;
		for (double i = 0; i < p; i++)
		{
			thrs.emplace_back(_FloydPart, W, Wk, _k, k, i*h, (i + 1)*h);
		}

		for (auto& i : thrs)
		{
			i.join();
		}
		deallocate(W, _k, _k);
		W = Wk;
	}
	return W;
}

void _FloydPart(Mat W, Mat Wk, int _k, int k, int from, int to)
{
	for (int i = from; i < to; i++)
	{
		for (int j = 0; j < _k; j++)
		{
			Wk[i][j] = std::min({ W[i][k] + W[k][j], W[i][j] });
		}
	}
}

#include <iostream>
#include <vector>
#include <string>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

Mat OpenCLFloyd(Mat W, int _k)
{
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


	const size_t N = _k*_k;

	try
	{
		// Get list of OpenCL platforms.
		std::vector<cl::Platform> platform;
		cl::Platform::get(&platform);

		if (platform.empty())
		{
			std::cerr << "OpenCL platforms not found." << std::endl;
			return Mat();
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
			return Mat();
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
			return Mat();
		}

		cl::Kernel add(program, "add");

		// Prepare input data.
		//	W[0] = new double[4]{ 0,-2,3,-3 };
		//	W[1] = new double[4]{ INT_MAX,0,2,INT_MAX };
		//	W[2] = new double[4]{ INT_MAX,INT_MAX,0,-3 };
		//	W[3] = new double[4]{ 4,5,5,0 };
		std::vector<double> a(N);
		for (int i = 0; i < _k; i++)
		{
			for (int j = 0; j < _k; j++)
			{
				a[i*_k + j] = W[i][j];
			}
		}

		for (int i = 0;i < N;++i)
		{
			std::cout << a[i] << " ";
			if ((i + 1) % (int)std::sqrt(N) == 0)
			{
				std::cout << std::endl;
			}
		}

		std::vector<double> b(N, 1);
		std::vector<double> c(N);
		for (int z = 0; z < _k; z++)
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
				if ((i + 1) % (int)std::sqrt(N) == 0)
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
	return Mat();
}