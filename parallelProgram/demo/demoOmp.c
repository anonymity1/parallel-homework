#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include "omp.h"

const int MAXN = 1000;

int main(int argc, char* argv[]) {
	int thread_count = strtol(argv[1], NULL, 10);

	int i, j, k;
	int a[MAXN][MAXN];
	int n = MAXN;
	double start, finish, duration;
	start = omp_get_wtime();
#	pragma omp parallel num_threads(thread_count) \
	shared(a) \
	private(i, j, k)
	for (i = 0; i < n; i++) {
		for (j = i; j < n; j++) {
#			pragma omp for
			for (k = i; k < n; k++) {
				a[i][j] = a[k][k] * a[i][i];
			}
		}
	}
	finish = omp_get_wtime();
	duration = finish - start;
	printf("the total time: %f\n", duration);
	return 0;
}