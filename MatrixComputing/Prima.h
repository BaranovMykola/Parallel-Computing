#pragma once
#include "MatrixProcessing.h"
#include <vector>
#include <utility>
#include <algorithm>

Mat Prima(Mat g, int k);

std::vector<std::pair<int, int> > getAllPairs(Mat g, int k,std::vector<int> incl);

Mat PrimaParallel(Mat g, int k, int p);

std::vector<std::pair<int, int>> getPartPairs(Mat g, int k, std::vector<int> incl, int from, int to);