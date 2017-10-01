#pragma once
#include "MatrixProcessing.h"

void add(Mat m1, Mat m2, Mat & r, int rows, int cols, int op(int, int));

void _partialAdd(Mat m1, Mat m2, Mat r, int rows, int cols, int from, int to, int op(int, int));

void addParallelInvoke(Mat m1, Mat m2, Mat & r, int rows, int cols, int t, int op(int, int));

bool checkAdding(Mat r, int rows, int cols, int val);

int addOp(int a, int b);

int subOp(int a, int b);
