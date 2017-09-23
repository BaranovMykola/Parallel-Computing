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

void quickSortParallel(int*& arr, int size, int n)
{
	int p = pow(2,n);
	std::vector<std::thread> sortingThreads;
	std::vector<int> pivots;
	std::set<int> sortedPivots;

		int half = partialquickSort(arr, 0, size - 1);
	sortedPivots.insert(0);
	sortedPivots.insert(size-1);
	sortedPivots.insert(half);
	
	int k = 2;
	while (k < p)
	{
		std::vector<std::future<int>> pins;

		for (int j = 0; j < sortedPivots.size()-1; j++)
		{
			pins.emplace_back(std::async(partialquickSort, arr, *std::next(sortedPivots.begin(), j), *std::next(sortedPivots.begin(), j+1)));
		}

		for (auto& i : pins)
		{
			sortedPivots.insert(i.get());
			++k;
		}
	}
	
	std::copy(sortedPivots.begin(), sortedPivots.end(), std::back_inserter(pivots));
	

	for (int i = 0; i < pivots.size()-1; i++)
	{
		sortingThreads.emplace_back(quickSort, arr, pivots[i], pivots[i + 1]);
	}

	for (auto& i : sortingThreads)
	{
		i.join();
	}
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
