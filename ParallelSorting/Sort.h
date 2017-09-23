#pragma once
#include <stdlib.h>
#include <vector>

void quickSort(int arr[], int left, int right);

void quickSortParallel(int*& arr, int size, int n = 5);

int partialquickSort(int arr[], int left, int right);

int* createRandArr(int size, int seed, int upper = INT_MAX);


