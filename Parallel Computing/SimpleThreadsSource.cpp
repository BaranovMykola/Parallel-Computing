#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <future>

using namespace std;

void countPrint(int i)
{
	for (int k = 0; k < 100; k++)
	{
		cout << i++ << endl;

	}
}

void charPrint(char ch)
{
	for (int k = 0; k < 100; k++)
	{
		cout << ch << endl;
		ch = (ch + 1) % 128;
	}
}

vector<int>* generateVec(int lenght)
{
	vector<int>* arrVec = new vector<int>[2];
	for (int k = 0; k < 2; k++)
	{
		int count = 1;
		generate_n(std::back_inserter(arrVec[k]), lenght, [&]()
		{
			return count++;
		});
	}
	return arrVec;
}

int sumVec(vector<int> v1, vector<int> v2)
{
	vector<int> res;
	std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(res), [](int a, int b) { return a + b; });
	return accumulate(res.begin(), res.end(), 0);
}

int multVec(vector<int> v1, vector<int> v2)
{
	vector<int> res;
	std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(res), [](int a, int b) { return a * b; });
	return accumulate(res.begin(), res.end(), 1, [](int mult, int i)
	{
		return mult *= i;
	});
}

int main()
{
	std::future<vector<int>*> futureVec = std::async(&generateVec, 4);
	auto vecArr = futureVec.get();

	auto sum = std::async(&sumVec, vecArr[0], vecArr[1]);
	auto mult = std::async(&multVec, vecArr[0], vecArr[1]);

	cout << "Sum = " << sum.get() << endl;
	cout << "Mult = " << mult.get() << endl;
	std::getchar();
	return 0;
}