#pragma once
#include <stdlib.h>

void quickSort(int arr[], int left, int right);

static int* createRandArr(int size, int seed, int upper)
{
		int* arr = new int[size];
		srand(seed);
		for (int i = 0; i < size; i++)
		{
			arr[i] = rand() % upper;
		}
		return arr;
}