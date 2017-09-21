#include "Sort.h"
#include <stdlib.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <queue>
#include <future>
#include <chrono>

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

std::vector<interval> partialquickSort(int arr[], int left, int right)
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

	std::vector<interval> q;
	/* recursion */
	if (left < j)
		//quickSort(arr, left, j);
		q.push_back(std::make_pair(left, j));
	if (i < right)
		q.push_back(std::make_pair(i, right));
	return q;
}
std::vector<interval> foo() { return std::vector<interval>(); }

void quickSortParallel(int*& arr, int size)
{
	std::queue<interval> q;
	int p = 4;
	std::vector<std::thread> t;
	std::vector<std::future<std::vector<interval>>> f;
	
	std::nth_element(arr, arr + size / 2, arr + size);
	std::nth_element(arr, arr + size / 4, arr + size/2);
	int thoffset = size*(2 / 3.0);
	std::nth_element(arr+size/2, arr + (int)thoffset, arr + size);


	

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
