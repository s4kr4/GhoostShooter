#include "pch.h"
#include "mymath.h"
#include "control.h"

MyMath::MyMath(){}

//ベジェ曲線
void MyMath::Bezier(double x, double y, int div, int cntr, double p0[], double p1[], double p2[],  double p3[], double *angle){

	double u = (1.0/div)*cntr;

	double p01[2] = {(1.0-u)*p0[0] + u*p1[0], (1.0-u)*p0[1] + u*p1[1]};
	double p12[2] = {(1.0-u)*p1[0] + u*p2[0], (1.0-u)*p1[1] + u*p2[1]};
	double p23[2] = {(1.0-u)*p2[0] + u*p3[0], (1.0-u)*p2[1] + u*p3[1]};

	double p02[2] = {(1.0-u)*p01[0] + u*p12[0], (1.0-u)*p01[1] + u*p12[1]};
	double p13[2] = {(1.0-u)*p12[0] + u*p23[0], (1.0-u)*p12[1] + u*p23[1]};

	double p03[2] = {(1.0-u)*p02[0] + u*p13[0], (1.0-u)*p02[1] + u*p13[1]};

	*angle = atan2(p03[1]-y, p03[0]-x)*180/PI;
}
