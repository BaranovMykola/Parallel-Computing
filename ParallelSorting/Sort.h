#pragma once
#include <stdlib.h>

void quickSort(int arr[], int left, int right);

int* merge(int * arr, int size, int p);

int getMinInterval(int * arr, int min, int size, int p, std::vector<int> merged);

int* createRandArr(int size, int seed, int upper = INT_MAX);