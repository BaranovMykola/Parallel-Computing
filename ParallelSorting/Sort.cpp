#include "Sort.h"
#include <stdlib.h>
#include <vector>
#include <thread>
#include <algorithm>

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

void quickSortParallel(int*& arr, int size)
{
	int p = 4;

	std::vector<std::thread> thrs;
	for (int i = 0; i < p; i++)
	{
		int from = (size / p)*i;
		int to = (size / p)*(i+1);
		if (i + 1 == p)
		{
			to = size - 1;
		}
		thrs.emplace_back(quickSort, arr, from, to);
	}
	for (auto& i : thrs)
	{
		i.join();
	}
	arr = merge(arr, size, p);
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
