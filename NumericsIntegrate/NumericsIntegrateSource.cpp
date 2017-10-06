#include <iostream>
#include <vector>
#include <future>
#include <time.h>

double integrate(double(*f)(double), double a, double b, double h = 0.0001)
{
	double N = (b - a) / h;
	double sum=0;
	for (int i = 0; i < N; i++)
	{
		sum += f((a + h*i));
	}
	return sum*h;
}

double integrateParallel(double(*f)(double), double a, double b, int p = 4, double h = 0.0001)
{
	std::vector<std::future<double>> parts;
	double step = (b - a) / p;
	for (int i = 0; i < p; i++)
	{
		double _a = a + i*step;
		double _b = a + (i + 1)*step;
		if (i + 1 == p)
		{
			_b = b;
		}
		parts.push_back(std::async(integrate, f, _a, _b, h));
	}
	double result=0;
	for (auto& i : parts)
	{
		result += i.get();
	}
	return result;
}

double liniar(double x)
{
	return 2 * x;
}

double pow(double x)
{
	return pow(x,2);
}

int main()
{
	double a = 0;
	double b = 3;
	double h = 0.0000001;
	std::cout << "Integrate x^2 from " << a<< " to " << b << " with " << h << " step consistently... ";
	auto start = clock();
	std::cout << "y = " << integrate(pow, 0, 3, h);
	std::cout << " Elapsed " << (clock() - start) / 1000.0 << " sec" << std::endl;
	
	for (int p = 2; p < 10; p++)
	{
		std::cout << "Integrate x^2 from " << a << " to " << b << " with " << h << " step parallel in [" << p <<  "] threads... ";
		start = clock();
		std::cout << "y = " << integrateParallel(pow, 0, 3, p, h);
		std::cout << " Elapsed " << (clock() - start) / 1000.0 << " sec" << std::endl;
	}

	system("pause");
	return 0;
}