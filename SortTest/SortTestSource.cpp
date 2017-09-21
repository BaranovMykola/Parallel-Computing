#include <gtest\gtest.h>
#include <iostream>
#include <ctime>
#include "Sort.h"
#include "Sort.cpp"

const int ArraySizeAlg = 12;
const int ArraySeedAlg = 3;
const int ArraySizePerf = 10000000;
const int ArraySeedPerf = 1;
bool isSorted(int arr[], int size);
int* performanceArrOrdinary = nullptr;
int* performanceArrParallel = nullptr;


TEST(TestAlg, quickSortOrdinary)
{
	int* arr = createRandArr(ArraySizeAlg, ArraySeedAlg);
	quickSort(arr, 0, ArraySizeAlg - 1);
	EXPECT_TRUE(isSorted(arr, ArraySizeAlg));
}

TEST(TestAlg, quickSortParallel)
{
	int* arr = createRandArr(ArraySizeAlg, ArraySeedAlg, 10);
	quickSortParallel(arr, ArraySizeAlg);
	EXPECT_TRUE(isSorted(arr, ArraySizeAlg));
}

TEST(TestPerformance, quickSortOrdinary)
{
	quickSort(performanceArrOrdinary, 0, ArraySizePerf - 1);
}

TEST(TestPerformance, quickSortParallel)
{
	quickSortParallel(performanceArrParallel, ArraySizePerf);
}


int main(int argc, char* argv[])
{
	performanceArrOrdinary = createRandArr(ArraySizePerf, ArraySeedPerf);
	performanceArrParallel = createRandArr(ArraySizePerf, ArraySeedPerf);
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}
bool isSorted(int arr[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		if (arr[i] > arr[i + 1])
		{
			return false;
		}
	}
	return true;
}
