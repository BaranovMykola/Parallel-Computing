#include <gtest\gtest.h>
#include <iostream>
#include <ctime>
#include "Sort.h"
#include "Sort.cpp"

const int ArraySizeAlg = 1000;
const int ArraySeedAlg = 1;
bool isSorted(int arr[], int size);

TEST(TestAlg, quickSortOrdinary)
{
	int* arr = createRandArr(ArraySizeAlg, ArraySeedAlg);
	quickSort(arr, 0, ArraySizeAlg - 1);
	EXPECT_TRUE(isSorted(arr, ArraySizeAlg));
}


int main(int argc, char* argv[])
{
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