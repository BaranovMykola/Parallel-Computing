#include "Sort.h"
#include <stdlib.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <queue>
#include <future>
#include <chrono>
#include <set>
#include <iterator>

typedef std::pair<int, int> interval;
using namespace std::chrono_literals;

void quickSort(int arr[], int left, int right)
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j)
	{
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j)
		{
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

int partialquickSort(int arr[], int left, int right)
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j)
	{
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j)
		{
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};
	return arr[i] == pivot ? i : j;
}
std::vector<interval> foo() { return std::vector<interval>(); }

void quickSortParallel(int*& arr, int size)
{
	int n = 5;
	int p = pow(2,n);
	std::vector<std::thread> t;

	std::vector<int> pins;
	//int half = partialquickSort(arr, 0, size - 1);
	///*int quadr = partialquickSort(arr, 0, half);
	//int th = partialquickSort(arr, half, size-1);*/
	//int quadr;
	//int th;

	//auto q1 = std::async(partialquickSort, arr, 0, half);
	//auto t1 = std::async(partialquickSort, arr, half, size - 1);
	//quadr = q1.get();
	//th = t1.get();

	//pins.insert(pins.end(), {0,quadr, half, th, size-1});

	std::set<int> ints;
	ints.insert(0);
	ints.insert(size-1);
	int half = partialquickSort(arr, 0, size - 1);
	ints.insert(half);

	int k = 2;
	while (k < p)
	{
		std::vector<std::future<int>> pins;

		for (int j = 0; j < ints.size()-1; j++)
		{
			pins.emplace_back(std::async(partialquickSort, arr, *std::next(ints.begin(), j), *std::next(ints.begin(), j+1)));
		}

		for (auto& i : pins)
		{
			ints.insert(i.get());
			++k;
		}
	}
	
	pins.clear();
	std::copy(ints.begin(), ints.end(), std::back_inserter(pins));
	

	for (int i = 0; i < pins.size()-1; i++)
	{
		t.emplace_back(quickSort, arr, pins[i], pins[i + 1]);
	}

	for (auto& i : t)
	{
		i.join();
	}
}

int* merge(int* arr, int size, int p)
{
	int* res = new int[size];
	std::vector<int> index;
	for (int i = 1; i < size; i += size / p)
	{
		index.push_back(i - 1);
	}

	for (int i = 0; i < size; i++)
	{

		auto minIndex = std::min_element(index.begin(), index.end(), [&](int i, int j) { return arr[i] < arr[j]; });
		/*res[i] = arr[*minIndex];
		++*minIndex;*/
		/*if (*minIndex%(size/p) == 0)
		{
			index.erase(minIndex);
		}*/
		
	}
	return res;
}

int getMinInterval(int* arr, int min, int size, int p, std::vector<int> merged)
{
	int minIndex = 0;
	for (int i = 1; i < size; i+=size/p)
	{
		if (arr[i - 1] < min && (std::find(merged.begin(), merged.end(), i-1)==merged.end()))
		{
			min = arr[i - 1];
			minIndex = i - 1;
		}
	}
	return minIndex;
}

int* createRandArr(int size, int seed, int upper)
{
	int* arr = new int[size];
	srand(seed);
	for (int i = 0; i < size; i++)
	{
		arr[i] = rand() % upper;
	}
	return arr;
}
