#pragma once
#include <ctime>
#include <iostream>
class Timer
{
public:
	Timer();
	~Timer();
	static void reset() { start = clock(); }
	static int check() { return clock()-start; }
	static void show() { std::cout << "Elapsed " << check() / 1000.0 << "s." << std::endl; }
private:
	static int start;
};

