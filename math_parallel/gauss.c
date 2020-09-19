// 
// created by shs on 2019/4/20
// Copyright (c) 2019 shs. All rights reserved
//

#include "cm.h"

static void test(int dim, float a[][MAXN]) {
	int i, j;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			printf("%lf ", a[i][j]);
		}
		printf("\n");
	}
}

extern void GaussSolu(int dim, float aa[][MAXN], float b[MAXN], 
						float res[MAXN]) {
	int i, j, k, tmp;
	float a[MAXN][MAXN] = {0};
	memcpy(a, aa, sizeof(a));
	memcpy(res, b, sizeof(float)*MAXN);
	for (k = 0; k < dim; k++) {
		int sup = a[k][k];
		for (i = k + 1; i < dim; i++) {
			if (sup < a[i][k]) {
				tmp = i;
			}
		}
		if (tmp != k) {
			for (j = k; j < dim; j++) {
				swap(&a[tmp][j], &a[k][j]);
			}
			swap(&res[tmp], &res[k]);
		}
		for (j = k + 1; j < dim; j++) {
			a[k][j] /= a[k][k];
		}
		res[k] /= a[k][k];
		a[k][k] = 1.0;
		for (i = k + 1; i < dim; i++) {
			for (j = k + 1; j < dim; j++) {
				a[i][j] -= a[i][k] * a[k][j];
			}
			res[i] -= a[i][k] * res[k];
			a[i][k] = 0.0;
		}
	}
	for (k = dim - 1; k >= 0; k--) {
		for (j = dim - 1; j > k; j--) {
			res[k] -= res[j] * a[k][j];	
		}
	}
}

extern void luDecompose(int dim, float a[][MAXN], float lu[][MAXN]) {
	int i, j, k;
	for (k = 0; k < dim; k++) {
		for (j = k; j < dim; j++) {
			lu[k][j] = a[k][j];
			for (i = 0; i < k; i++) {
				lu[k][j] -= lu[i][j] * lu[k][i];
			} 
		}
		for (i = k + 1; i < dim; i++) {
			lu[i][k] = a[i][k];
			for (j = 0; j < k; j++) {
				lu[i][k] -= lu[i][j] * lu[j][k];
			}
			lu[i][k] /= lu[k][k];
		}
	}
}

extern void luSolu(int dim, float lu[][MAXN], float b[MAXN], 
					float res[MAXN]) {
	int i, j, k;
	for (i = 0; i < dim; i++) {
		res[i] = b[i];
		for (j = 0; j < i; j++) {
			res[i] -= lu[i][j] * b[j];
		}
	}

	for (i = dim - 1; i >= 0; i--) {
		for (j = dim - 1; j > i; j--) {
			res[i] -= lu[i][j] * res[j];
		}
		res[i] /= lu[i][i];
	}
}

extern void sqrSolu(int dim, float g[][MAXN], float b[MAXN], 
						float res[MAXN]) {
	int i, j, k;

	for (k = 0; k < dim; k++) {
		for (j = 0; j < k; j++) {
			g[k][k] -= g[k][j];
		}
		g[k][k] = sqrt(g[k][k]);
		for (i = k + 1; i < dim; i++) {
			for (j = 0; j < k; j++) {
				g[i][k] -= g[i][j] * g[k][j];
			}
			g[i][k] /= g[k][k];
		}
	}

	for (i = 0; i < dim; i++) {
		res[i] = b[i];
		for (k = 0; k < i; k++) {
			res[i] -= g[i][k] * b[k];
		}
		res[i] /= g[i][i];
	}
}

static void chaseDecompose(int dim, float a[][MAXN]) {
	for (int k = 1; k < dim; k++) {
		a[k-1][k] /= a[k][k];
		a[k][k] -= a[k-1][k] * a[k][k-1];
	}
}

extern void chaseSolu(int dim, float aa[][MAXN], float b[MAXN], 
						float res[MAXN]) {
	float a[MAXN][MAXN];
	memcpy(a, aa, sizeof(a));
	memcpy(res, b, sizeof(float)*MAXN);

	chaseDecompose(dim, a);

	for (int i = 1; i < dim; i++) {
		res[i] -= a[i-1][i] * res[i-1];
	}

	res[dim-1] /= a[dim-1][dim-1];

	for (int i = dim - 2; i >= 0; i--) {
		res[i] = (res[i] - res[i+1] * a[i][i+1]) / a[i][i];
	}
}