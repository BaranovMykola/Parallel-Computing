#pragma once
#include "MatrixProcessing.h"

double symplexMethod(Mat C, Mat a, int n, int m);

Vec extractVecB(Mat a, int n, int m);

void updateDelta(Vec delta, Vec Cb, Mat C, Mat a, int m, int n);

int findMinDeltaIndex(Vec delta, int n);

int findMainIndex(Vec Cb, Mat a, int m, int k);

void updateB(Vec& Cb, Mat a, int r, int k, int m);

void updateA(Mat & a, int m, int n, int r, int k);
