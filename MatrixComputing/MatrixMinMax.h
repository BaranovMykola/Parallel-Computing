#pragma once
#include "MatrixProcessing.h"

double _maxMinSearch(Mat mat, int rFrom, int rTo, int cFrom, int cTo);

double maxMinSearch(Mat mat, int r, int c);

double searchMaxMinParallel(Mat mat, int r, int c, int p);

double searchMaxMinParallel(Mat mat, int r, int c, int from, int to, int p);

double searchMin(Vec row, int from, int to);

double _searchMin(Vec row, int from, int to);
