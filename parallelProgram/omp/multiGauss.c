// 
// Gauss.c
//
// created by shs on 2019/4/25
// Copyright (C) shs 2019. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <math.h>
#include "omp.h"

const int MAXN = 1000;

int main(int argc, char* argv[]) {
	int thread_count = strtol(argv[1], NULL, 10);

	int i, j, k; 
	int tmp;
	int n = MAXN;
	double a[MAXN][MAXN], b[MAXN];
	double start, finish, duration;
	start = omp_get_wtime();
#	pragma omp parallel num_threads(thread_count) \
	shared(a, b) \
	private(i, j, k, tmp)
	for (k = 0; k < n; k++) {
		for (tmp = k; tmp < n; tmp++) {
			a[k][tmp] /= a[k][k];
		} 
		b[k] /= a[k][k];
		a[k][k] = 1.0;
#		pragma omp for
		for (i = k + 1; i < n; i++) {
			for (j = k + 1; j < n; j++) {
				a[i][j] -= a[k][j] * a[i][k];
			}
			a[i][k] = 0.0;
		}
	}

#	pragma omp parallel num_threads(thread_count) \
	shared(a, b) \
	private(k, j) 
	for (k = n - 1; k >= 0; k--) {
#		pragma omp for
		for (j = k + 1; j < n; j++) {
			b[k] -= a[k][j] * b[j];
		}
	}
	finish = omp_get_wtime();
	duration = finish - start;
	printf("the total time: %f\n", duration);
	return 0; 
}