#pragma once
#include "MatrixProcessing.h"

double symplexMethod(Mat C, Mat a, int n, int m);

double symplexMethodParallel(Mat C, Mat a, int n, int m, int p);

Vec extractVecB(Mat a, int n, int m);

void updateDelta(Vec delta, Vec Cb, Mat C, Mat a, int m, int n);

int findMinDeltaIndex(Vec delta, int n);

int findMainIndex(Vec Cb, Mat a, int m, int k);

void updateB(Vec* Cb, Mat a, int r, int k, int m);

bool updateA(Mat* a, int m, int n, int r, int k, int from=0, int to=0);

void updateAParallel(Mat * a, int m, int n, int r, int k, int p);
