#include "Sort.h"
#include <stdlib.h>

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

int* createRandArr(int size, int seed, int upper = INT_MAX)
{
	int* arr = new int[size];
	srand(seed);
	for (int i = 0; i < size; i++)
	{
		arr[i] = rand() % upper;
	}
	return arr;
}
