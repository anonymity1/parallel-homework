#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "testing.h"
#include "cm.h"

#include <math.h>

int main() {
	outputMainInfo();
	int dim, n;
	float a[MAXN][MAXN], b[MAXN];
	float lu[MAXN][MAXN];
	float res[MAXN];
	float result, cur;
	float y1, y2;
	Point pos[MAXN];
	while (true) {
		char method = getchar();
		fflush(stdin);
		if (method == 'q') {
			exit(0);
		}
		switch (method) {
			case '1':
				GaussInfo(&dim, a, b);
				GaussSolu(dim, a, b, res);
				outputEquRes(dim, res);
				break;
			case '2':
				luInfo(&dim, a, b);
				luDecompose(dim, a, lu);
				luSolu(dim, lu, b, res);
				outputEquRes(dim, res);
				break;
			case '3':
				sqrInfo(&dim, a, b);
				sqrSolu(dim, a, b, res);
				outputEquRes(dim, res);
				break;
			case '4':
				chaseInfo(&dim, a, b);
				chaseSolu(dim, a, b, res);
				outputEquRes(dim, res);
				break;
			case 'a':
				lagrangeInfo(&n, pos, &cur);
				result = lagrange(n, pos, cur);
				outputInterpRes(result);
			case 'b':
				newtonInfo(&n, pos, &cur);
				result = newton(n, pos, cur);
				outputInterpRes(result);
			case 'c':
				splineInfo1(&n, pos, &cur, &y1, &y2);
				result = spline1(n, pos, cur, y1, y2, res);
				outputSplineRes(result, res);
			case 'd':
				splineInfo2(&n, pos, &cur, &y1, &y2);
				result = spline2(n, pos, cur, y1, y2, res);
				outputSplineRes(result, res);
			case '\n':
				continue;
			default:
				outputError();
				continue;
		}
	}
	return 0;
}