// 
// created by shs on 2019/4/20
// Copyright (c) 2019 shs. All rights reserved
//
#include "testing.h"

static inline void outputInfo() {
	printf("Now please choose the alogorithm you want to use: \n");
}

extern void outputMainInfo() {
	printf("This is a program of mathematics computing. \n");
	printf("  input \'1\' to use gauss elimination. \n");
	printf("  input \'2\' to use LU decomposition. \n");
	printf("  input \'3\' to use sqr decomposition. \n");
	printf("  input \'4\' to use chase decomposition. \n");
	printf("  input \'a\' to use lagrange interpolation. \n");
	printf("  input \'b\' to use newton interpolation. \n");
	printf("  input \'c\' to use spline1 interpolation. \n");
	printf("  input \'d\' to use spline2 interpolation. \n");
	printf("  ...... \n");
	printf("  input \'q\' to quit. \n");
	outputInfo();
}

extern void outputEquRes(int dim, float res[MAXN]) {
	printf("The answer is: ");
	for (int i = 0; i < dim; i++) {
		printf("%f ", res[i]);
	}
	printf("\n");
	outputInfo();
}

extern void outputError() {
	printf("Your input is illegal!\n");
	outputInfo();
}

extern void GaussInfo(int* pdim, float a[][MAXN], float b[MAXN]) {
	int var, equ;
	printf("Please input the number of the variables: ");
	scanf("%d", &var);
	printf("Please input the number of the equations: ");
	scanf("%d", &equ);
	printf("Please input matrix a: \n");
	*pdim = var;
	for (int i = 0; i < equ; i++) {
		for (int j = 0; j < var; j++) {
			scanf("%f", &a[i][j]);
		}
	}
	printf("Please input vector b: \n");
	for (int i = 0; i < equ; i++) {
		scanf("%f", &b[i]);
	}
}

extern void luInfo(int* pdim, float a[][MAXN], float b[MAXN]) {
	int dim;
	printf("Please input the dimensional of the matrix: ");
	scanf("%d", &dim);
	printf("Please input matrix a: \n");
	*pdim = dim;
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			scanf("%f", &a[i][j]);
		}
	}
	printf("Please input vector b: \n");
	for (int i = 0; i < dim; i++) {
		scanf("%f", &b[i]);
	}
}

extern void sqrInfo(int* pdim, float a[][MAXN], float b[MAXN]) {
	int dim;
	printf("Please input the dimensional of the matrix: ");
	scanf("%d", &dim);
	printf("Please input the down triangle of matrix a: \n");
	*pdim = dim;
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j <= i; j++) {
			scanf("%f", &a[i][j]);
			a[j][i] = a[i][j];
		}
	}
	printf("Please input vector b: \n");
	for (int i = 0; i < dim; i++) {
		scanf("%f", &b[i]);
	}
}

extern void chaseInfo(int* pdim, float a[][MAXN], float b[MAXN]) {
	int dim;
	printf("Please input the dimensional of the matrix: ");
	scanf("%d", &dim);
	*pdim = dim;
	printf("Please input the diagonal vector of matrix a: \n");
	for (int i = 0; i < dim; i++) {
		scanf("%f", &a[i][i]);
	}
	printf("Please input the vector l of matrix a: \n");
	for (int i = 1; i < dim; i++) {
		scanf("%f", &a[i][i-1]);
	}
	printf("Please input the vector l of matrix a: \n");
	for (int i = 1; i < dim; i++) {
		scanf("%f", &a[i-1][i]);
	}
	printf("Please input vector b: \n");
	for (int i = 0; i < dim; i++) {
		scanf("%f", &b[i]);
	}
}