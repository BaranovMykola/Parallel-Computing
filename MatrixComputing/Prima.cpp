#include "Prima.h"

Vec Prima(Mat g, int k)
{
	std::vector<int> incl{ 3 };
	getAllPairs(g, k, incl);
	return Vec();
}

std::vector<std::pair<int, int>> getAllPairs(Mat g, int k, std::vector<int> incl)
{
	std::vector<std::pair<int, int> > ps;
	for (int i = 0; i < k; i++)
	{
		for (int j = i; j < k; j++)
		{
			auto it = std::find(incl.begin(), incl.end(), i);
			auto jt = std::find(incl.begin(), incl.end(), j);
			if ((it != incl.end() || jt != incl.end())
				&& !(it != incl.end() && jt != incl.end())
				&& g[i][j] != 0)
			{
				ps.emplace_back(i, j);
			}
		}
	}
	return ps;
}
