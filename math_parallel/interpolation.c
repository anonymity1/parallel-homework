#include "cm.h"

static inline float cubic(float x) {
	return x*x*x;
}

extern float lagrange(int n, Point pos[MAXN], float cur) {
	float result = 0.0;
	for (int i = 0; i < n; i++) {
		float tmp = 1.0;
		for (int j = 0; j < n; j++) {
			if (j != i) tmp *= (cur - pos[j].x) / (pos[i].x - pos[j].x);
		} 
		result += tmp * pos[i].y;
	}
}

extern float newton(int n, Point pos[MAXN], float cur) {
	float result = 0.0;
	float a[MAXN][MAXN];
	for (int i = 0; i < n; i++) {
		a[i][0] = pos[i].y;
	}
	for (int j = 1; j < n; j++) {
		for (int i = j; i < n; i++) {
			a[i][j] = (a[i][j-1] - a[i-1][j-1]) / (pos[i].x - pos[i-j].x);
		}
	}
	float tmp = 1.0;
	for (int i = 0; i < n; i++) {
		result += a[i][i] * tmp;
		tmp = cur - pos[i].x;
	}
	return result;
}

static void chaseDecompose0(int dim, float a[][MAXN]) {
	for (int k = 1; k < dim; k++) {
		a[k-1][k] /= a[k][k];
		a[k][k] -= a[k-1][k] * a[k][k-1];
	}
}

static void chaseSolu0(int dim, float aa[][MAXN], float b[MAXN], 
						float res[MAXN]) {
	float a[MAXN][MAXN];
	memcpy(a, aa, sizeof(a));
	memcpy(res, b, sizeof(float)*MAXN);

	chaseDecompose0(dim, a);

	for (int i = 1; i < dim; i++) {
		res[i] -= a[i-1][i] * res[i-1];
	}

	res[dim-1] /= a[dim-1][dim-1];

	for (int i = dim - 2; i >= 0; i--) {
		res[i] = (res[i] - res[i+1] * a[i][i+1]) / a[i][i];
	}
}

static void chaseDecompose1(int dim, float a[][MAXN]) {
	for (int k = 2; k <= dim; k++) {
		a[k-1][k] /= a[k][k];
		a[k][k] -= a[k-1][k] * a[k][k-1];
	}
}

static void chaseSolu1(int dim, float aa[][MAXN], float b[MAXN], 
						float res[MAXN]) {
	float a[MAXN][MAXN];
	memcpy(a, aa, sizeof(a));
	memcpy(res, b, sizeof(float)*MAXN);

	chaseDecompose1(dim, a);

	for (int i = 2; i <= dim; i++) {
		res[i] -= a[i-1][i] * res[i-1];
	}

	res[dim] /= a[dim][dim];

	for (int i = dim - 1; i >= 1; i--) {
		res[i] = (res[i] - res[i+1] * a[i][i+1]) / a[i][i];
	}
}

static float findInf(int n, Point pos[MAXN], float cur) {
	float result = pos[0].x;
	for (int k = n-1; k >= 0; k--) {
		if (cur < pos[k].x) continue;
		result = pos[k].x;
	}
	return result;
}

static float findSup(int n, Point pos[MAXN], float cur) {
	float result = pos[n-1].x;
	for (int k = 0; k < n; k++) {
		if (cur > pos[k].x) continue;
		result = pos[k].x;
	}
	return result;
}

static int findLocation(int n, Point pos[MAXN], float cur) {
	int result = 0;
	for (int k = n-1; k >= 0; k--) {
		if (cur < pos[k].x) continue;
		result = k;
	}
	return result+1;
}

extern float spline1(int n, Point pos[MAXN], float cur, 
						float y1, float y2, float res[MAXN]) {
	float result;
	float h[MAXN], miu[MAXN], lambda[MAXN], d[MAXN];
	float g[MAXN][MAXN], b[MAXN], m[MAXN];
	m[0] = y1, m[n-1] = y2;
	for (int i = 1; i < n; i++) {
		h[i] = pos[i].x - pos[i-1].x;
	}
	for (int i = 1; i < n - 1; i++) {
		miu[i] = h[i] / (h[i] + h[i+1]);
		lambda[i] = 1 - miu[i];
	}
	for (int i = 1; i < n - 1; i++) {
		d[i] = 6 * ( (pos[i+1].y - pos[i].y) / h[i+1] 
				- (pos[i].y - pos[i-1].y) / h[i] ) / (h[i+1] + h[i]);
	}
	for (int i = 1; i < n - 1; i++) {
		g[i][i] = 2;
		b[i] = d[i];
		if (i == 1) {
			g[i][i+1] = lambda[i];
			b[i] -= miu[i] * m[0];
			continue;
		} else if (i == n - 2) {
			g[i][i-1] = miu[i];
			b[i] -= lambda[i] * m[n-1];
			continue;
		} 
		g[i][i+1] = lambda[i];
		g[i][i-1] = miu[i];
	}
	chaseSolu1(n - 2, g, b, res);
	float x1 = findInf(n, pos, cur);
	float x2 = findSup(n, pos, cur);
	int i = findLocation(n, pos, cur);
	result = ( cubic(x1 - cur) * m[i-1] + cubic(cur - x2) * m[i] ) 
				/ (6 * h[i]) + 
			( pos[i-1].y - m[i-1] * h[i] * h[i] / 6 ) 
				* (pos[i].x - cur) / h[i] +
			( pos[i].y - m[i] * h[i] * h[i] / 6) * (cur - m[i]) 
				/ h[i];
	return result;
}

extern float spline2(int n, Point pos[MAXN], float cur, 
						float y1, float y2, float res[MAXN]) {
	float result;
	float p[MAXN], q[MAXN], r[MAXN], s[MAXN];
	float h[MAXN], miu[MAXN], lambda[MAXN], d[MAXN];
	float g[MAXN][MAXN], b[MAXN], m[MAXN];
	m[0] = y1, m[n-1] = y2;
	for (int i = 1; i < n; i++) {
		h[i] = pos[i].x - pos[i-1].x;
	}
	for (int i = 1; i < n - 1; i++) {
		miu[i] = h[i] / (h[i] + h[i+1]);
		lambda[i] = 1 - miu[i];
	}
	for (int i = 1; i < n - 1; i++) {
		d[i] = 6 * ( (pos[i+1].y - pos[i].y) / h[i+1] 
				- (pos[i].y - pos[i-1].y) / h[i] ) / (h[i+1] + h[i]);
	}
	d[0] = 6 * ( (pos[1].y - pos[0].y) / h[1] - m[0] ) / h[1];
	d[n-1] = 6 * ( m[n-1] - (pos[n-1].y - pos[n-2].y) / h[n-1] ) / h[n-1];
	for (int i = 0; i < n; i++) {
		g[i][i] = 2;
		b[i] = d[i];
		if (i == 0) {
			g[i][i+1] = 1;
			continue;
		} else if (i == n - 1) {
			g[i][i-1] = 1;
			continue;
		} 
		g[i][i+1] = lambda[i];
		g[i][i-1] = miu[i];
	}
	chaseSolu0(n, g, b, res);
	float x1 = findInf(n, pos, cur);
	float x2 = findSup(n, pos, cur);
	int i = findLocation(n, pos, cur);
	result = ( cubic(x1 - cur) * m[i-1] + cubic(cur - x2) * m[i] ) 
				/ (6 * h[i]) + 
			( pos[i-1].y - m[i-1] * h[i] * h[i] / 6 ) 
				* (pos[i].x - cur) / h[i] +
			( pos[i].y - m[i] * h[i] * h[i] / 6) * (cur - m[i]) 
				/ h[i];
	return result;
}