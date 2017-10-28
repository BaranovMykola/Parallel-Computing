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

std::vector<double> OpenCLFloyd(Mat W, int _k)
{
	static const char source[] =
		"#if defined(cl_khr_fp64)\n"
		"#  pragma OPENCL EXTENSION cl_khr_fp64: enable\n"
		"#elif defined(cl_amd_fp64)\n"
		"#  pragma OPENCL EXTENSION cl_amd_fp64: enable\n"
		"#else\n"
		"#  error double precision is not supported\n"
		"#endif\n"
		"kernel void Floyd(\n"
		"       ulong n,\n"
		"       global const double *W_,\n"
		"       global double *Wk_1,\n"
		"       ulong k\n"
		"       )\n"
		"{\n"
		"    size_t i = get_global_id(0);\n"
		"	int n2 = sqrt((float)n);"
		"	int _i = ((i)/n2);"
		"	int _j = (i%n2);"
		"	Wk_1[_i*n2+_j] = W_[_i*n2+k]+W_[k*n2+_j] < W_[_i*n2+_j] ? W_[_i*n2+k]+W_[k*n2+_j] : W_[_i*n2+_j];"
		"}\n";
	const size_t N = _k*_k;
	std::vector<double> Wk_1(N);
	try
	{
		std::vector<cl::Platform> platform;
		cl::Platform::get(&platform);

		if (platform.empty())
		{
			std::cerr << "OpenCL platforms not found." << std::endl;
			throw;
		}

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
			throw;
		}

		std::cout << device[0].getInfo<CL_DEVICE_NAME>() <<"\t";

		cl::CommandQueue queue(context, device[0]);

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
			throw;
		}

		cl::Kernel Floyd(program, "Floyd");

		std::vector<double> W_(N);
		for (int i = 0; i < _k; i++)
		{
			for (int j = 0; j < _k; j++)
			{
				W_[i*_k + j] = W[i][j];
			}
		}

		std::vector<double> b(N, 1);
		for (int z = 0; z < _k; z++)
		{


			// Allocate device buffers and transfer input data to device.
			cl::Buffer A(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
						 W_.size() * sizeof(double), W_.data());

			cl::Buffer B(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
						 b.size() * sizeof(double), b.data());

			cl::Buffer C(context, CL_MEM_READ_WRITE,
						 Wk_1.size() * sizeof(double));

			// Set kernel parameters.
			Floyd.setArg(0, static_cast<cl_ulong>(N));
			Floyd.setArg(1, A);
			Floyd.setArg(2, C);
			Floyd.setArg(3, static_cast<cl_ulong>(z));

			// Launch kernel on the compute device.
			queue.enqueueNDRangeKernel(Floyd, cl::NullRange, N, cl::NullRange);

			// Get result back to host.
			queue.enqueueReadBuffer(C, CL_TRUE, 0, Wk_1.size() * sizeof(double), Wk_1.data());

			W_ = Wk_1;
		}
	}
	catch (const cl::Error &err)
	{
		std::cerr
			<< "OpenCL error: "
			<< err.what() << "(" << err.err() << ")"
			<< std::endl;
		throw;
	}
	return Wk_1;
}