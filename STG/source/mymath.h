#include "pch.h"
#include "define.h"

#ifndef _MYMATH
#define _MYMATH

class MyMath{
public:
	MyMath();
	void Bezier(double x, double y, int div, int cntr, double p0[], double p1[], double p2[], double p3[], double *angle);
};

#endif