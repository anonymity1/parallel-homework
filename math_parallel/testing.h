// 
// created by shs on 2019/4/20
// Copyright (c) 2019 shs. All rights reserved
//

#ifndef TEST_C
#define TEST_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cm.h"

#define MAXN 512

extern void outputMainInfo();
extern void outputEquRes(int dim, float res[MAXN]);
extern void outputInterpRes(float result);
extern void outputSplineRes(float result, float res[]);
extern void outputError();

extern void sqrInfo(int* pdim, float a[][MAXN], float b[MAXN]);
extern void luInfo(int* pdim, float a[][MAXN], float b[MAXN]);
extern void GaussInfo(int* pdim, float a[][MAXN], float b[MAXN]);
extern void chaseInfo(int* pdim, float a[][MAXN], float b[MAXN]);

extern void lagrangeInfo(int* pn, Point pos[MAXN], float *pcur);
extern void newtonInfo(int* pn, Point pos[MAXN], float *pcur);
extern void splineInfo1(int *pn, Point pos[MAXN], float *pcur, 
						float* y1, float* y2);
extern void splineInfo2(int *pn, Point pos[MAXN], float *pcur, 
						float* y1, float* y2);

#endif