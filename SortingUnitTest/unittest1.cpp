#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ParallelSorting/Sort.h"
#include "../ParallelSorting/Sort.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SortingUnitTest
{		
	TEST_CLASS(SortTest)
	{
	public:
		
		TEST_METHOD(oridnaryQuickSort)
		{
			quickSort(ordinaryArr, 0, sizeArr - 1);
			Assert::IsTrue(isSorted(ordinaryArr, sizeArr));
		}

		static void createArrays()
		{
			ordinaryArr = createRandArr(sizeArr, seed, upper);
			parallelArr = createRandArr(sizeArr, seed, upper);
		}

		static bool isSorted(int* arr, int size)
		{
			for (int i = 0; i < size-1; i++)
			{
				if (arr[i] > arr[i + 1])
				{
					return false;
				}
			}
			return true;
		}
		
		TEST_CLASS_CLEANUP(deallocateMemory)
		{
			delete SortTest::ordinaryArr;
			delete SortTest::parallelArr;
		}

		TEST_CLASS_INITIALIZE(initializeMemory)
		{
			SortTest::createArrays();
		}

	private:
		static int* ordinaryArr;
		static int* parallelArr;
		static const int sizeArr = 100;
		static const int seed = 1;
		static const int upper = INT_MAX;
	};

	int* SortTest::ordinaryArr = nullptr;
	int* SortTest::parallelArr = nullptr;
}