#include <iostream>
#include "Sort.h"

int main()
{
	auto arr = createRandArr(10, 1, 10);
	quickSort(arr, 0, 9);
	for (int i = 0; i < 10; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::getchar();
	return 0;
}