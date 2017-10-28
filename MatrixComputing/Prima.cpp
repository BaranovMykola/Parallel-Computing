#include "Prima.h"
#include <future>

Mat Prima(Mat g, int k)
{
	std::vector<int> incl{ 3 };
	Mat g2 = createMat(k, k);
	generateMatrix(g2, k, k, 0);
	while (incl.size() < k)
	{
		auto ps = getAllPairs(g, k, incl);
		std::sort(ps.begin(), ps.end(), [&](auto i, auto j)
		{
			double f = g[i.first][i.second];
			double s = g[j.first][j.second];
			return f < s;
		});
		incl.push_back(ps.front().first);
		incl.push_back(ps.front().second);
		g2[ps.front().first][ps.front().second] = 1;
		g2[ps.front().second][ps.front().first] = 1;
		std::sort(incl.begin(), incl.end());
		auto u = std::unique(incl.begin(), incl.end());
		incl.erase(u, incl.end());
	}
	return g2;
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

Mat PrimaParallel(Mat g, int k, int p)
{
	std::vector<int> incl{ 3 };
	Mat g2 = createMat(k, k);
	generateMatrix(g2, k, k, 0);
	while (incl.size() < k)
	{

		std::vector<std::future<std::vector<std::pair<int, int>>>> ftrs;
		double h = k / (double)p;

		for (int i = 0; i < p; i++)
		{
			ftrs.push_back(std::async(getPartPairs, g, k, incl, i*h, (i + 1)*h));
		}

		std::vector<std::pair<int, int>> ps;

		for (auto& i : ftrs)
		{
			auto part = i.get();
			ps.insert(ps.end(), part.begin(), part.end());
		}
		
		std::sort(ps.begin(), ps.end(), [&](auto i, auto j)
		{
			double f = g[i.first][i.second];
			double s = g[j.first][j.second];
			return f < s;
		});
		incl.push_back(ps.front().first);
		incl.push_back(ps.front().second);
		g2[ps.front().first][ps.front().second] = 1;
		g2[ps.front().second][ps.front().first] = 1;
		std::sort(incl.begin(), incl.end());
		auto u = std::unique(incl.begin(), incl.end());
		incl.erase(u, incl.end());
	}
	return g2;
}

std::vector<std::pair<int, int>> getPartPairs(Mat g, int k, std::vector<int> incl, int from, int to)
{
	std::vector<std::pair<int, int> > ps;
	for (int i = from; i < to; i++)
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
