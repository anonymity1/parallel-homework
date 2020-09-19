// 
// created by shs on 2019/4/20
// Copyright (c) 2019 shs. All rights reserved
//

#ifndef GAUSS_H
#define GAUSS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "info.h"

extern void GaussSolu(int dim, float a[][MAXN], float b[MAXN], float res[MAXN]);
extern void luDecompose(int dim, float a[][MAXN], float lu[][MAXN]);
extern void luSolu(int dim, float lu[][MAXN], float b[MAXN], float res[MAXN]);
extern void sqrSolu(int dim, float g[][MAXN], float b[MAXN], float res[MAXN]); 
extern void chaseSolu(int dim, float a[][MAXN], float b[MAXN], float res[MAXN]);

#endif 