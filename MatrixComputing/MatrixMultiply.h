#pragma once
typedef int** Mat;

Mat multiply(Mat A, Mat B, int m, int n, int k);

Mat multiplyParallelInvoke(Mat A, Mat B, int m, int n, int k, int p);

void _partialMultMat(Mat A, Mat B, Mat C, int m, int n, int k, int mFrom, int mTo);


