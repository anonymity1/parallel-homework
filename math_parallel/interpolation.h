#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "info.h"

typedef struct Point Point;
struct Point {
	float x;
	float y;
};

#define MAXN 512

extern float lagrange(int n, Point pos[MAXN], float cur);
extern float newton(int n, Point pos[MAXN], float cur);
extern float spline1(int n, Point pos[MAXN], float cur, 
						float y1, float y2, float res[]);
extern float spline2(int n, Point pos[MAXN], float cur, 
						float y1, float y2, float res[]);

#endif