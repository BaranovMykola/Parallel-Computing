#pragma once
#include "MatrixProcessing.h"
#include <vector>
#include <utility>
#include <algorithm>

Mat Prima(Mat g, int k);

std::vector<std::pair<int, int> > getAllPairs(Mat g, int k,std::vector<int> incl);