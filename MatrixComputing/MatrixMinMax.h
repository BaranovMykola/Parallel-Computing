#pragma once
#include "MatrixProcessing.h"

double _maxMinSearch(Mat mat, int rFrom, int rTo, int cFrom, int cTo);

double maxMinSearch(Mat mat, int r, int c);

void foo();

void maxMinSearchPart(Mat mat, int rFrom, int rTo, int cFrom, int cTo, double* res);

double minSearchParallel(Mat mat, int r, int c, int p);

double searchMaxMin(Mat mat, int r, int c, int p);

double searchMaxMin(Mat mat, int r, int c, int from, int to, int p);

double searchMin(Vec row, int from, int to);

double _searchMin(Vec row, int from, int to);
