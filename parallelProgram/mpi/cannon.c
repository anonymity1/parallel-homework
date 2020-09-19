#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include "mpi.h"

#define N 1024

double A[N][N], B[N][N], C[N][N];

int location(int row, int column, int sqrtSize) {
	return ( (row + sqrtSize) % sqrtSize) * sqrtSize + 
		(column + sqrtSize) % sqrtSize;
}

void outputMat(double matrix[][N], int row, int column, char* matrixName) {
	printf("%s\n", matrixName);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			printf("%6.0f ", matrix[i][j]);
		}
		printf("\n");
	}
}

int main() {

	int sqrtSize, dim, splitMatrixElementNum, row, column;
	double start, finish;

	int my_rank, comm_sz;
	MPI_Status status;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	sqrtSize = (int)sqrt((double)comm_sz);

	if ( sqrtSize * sqrtSize != comm_sz ||
		N % sqrtSize != 0 ) {
		if (my_rank == 0) {
			printf("this program do not support this number\n");
			MPI_Finalize();
			exit(1);
		}
	}

	dim = N / sqrtSize; // the dimension of the split martix
	splitMatrixElementNum = dim * dim; // the element number of the split martix

	double *a, *b, *c, *tmp_a, *tmp_b;
	a = (double*)malloc(splitMatrixElementNum * sizeof(double) );
	b = (double*)malloc(splitMatrixElementNum * sizeof(double) );
	c = (double*)malloc(splitMatrixElementNum * sizeof(double) );
	tmp_a = (double*)malloc(splitMatrixElementNum * sizeof(double) );
	tmp_b = (double*)malloc(splitMatrixElementNum * sizeof(double) );

	for (int i = 0; i < splitMatrixElementNum; i++) {
		c[i] = 0.0;
	}

	if (my_rank == 0) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				A[i][j] = rand() % 10;
				B[i][j] = rand() % 10;
			}
		}
		start = MPI_Wtime();

		int xMin, xMax, yMin, yMax;

		// the mapping between \
		   the inital location of the split matrix and \
		   the respective split matrix in one demision of each thread

		// first left to right, then up to down
		for (int i = 0; i < comm_sz; i++) {
			xMin = (i % sqrtSize) * dim;
			xMax = (i % sqrtSize + 1) * dim - 1;
			yMin = ( ( i - (i % sqrtSize) ) / sqrtSize) * dim;
			yMax = ( ( i - (i % sqrtSize) ) / sqrtSize + 1) * dim - 1;

			int sum = 0;

			for (int j = yMin; j <= yMax; j++) {
				for (int k = xMin; k <= xMax; k++) {
					tmp_a[sum] = A[j][k];
					tmp_b[sum] = B[j][k];
					sum++;
				}
			} 

			if (i == 0) {
				for (int m = 0; m < sum; m++) {
					a[m] = tmp_a[m];
					b[m] = tmp_b[m];
				}
			} else {
				MPI_Send(tmp_a, splitMatrixElementNum, MPI_DOUBLE, i, 1,
					MPI_COMM_WORLD);
				MPI_Send(tmp_b, splitMatrixElementNum, MPI_DOUBLE, i, 2,
					MPI_COMM_WORLD);
			}
		}

	} else {

		MPI_Recv(a, splitMatrixElementNum, MPI_DOUBLE, 0, 1,
			MPI_COMM_WORLD, &status);
		MPI_Recv(b, splitMatrixElementNum, MPI_DOUBLE, 0, 2,
			MPI_COMM_WORLD, &status);
	}

	// the coordinate of the split Matrix in the big matrix
	column = my_rank % sqrtSize;
	row = (my_rank - column) / sqrtSize;

	// first step: initalize the location of each split Matrix 
	// each split matrix of A was moved 'row' steps to left  
	// each split matrix of B was moved 'column' steps to up
	MPI_Sendrecv(a, splitMatrixElementNum, MPI_DOUBLE,
				location(row, column - row, sqrtSize), 1,
		tmp_a, splitMatrixElementNum, MPI_DOUBLE,
				location(row, column + row, sqrtSize), 1,
		MPI_COMM_WORLD, &status);

	MPI_Sendrecv(b, splitMatrixElementNum, MPI_DOUBLE,
				location(row - column, column, sqrtSize), 1,
		tmp_b, splitMatrixElementNum, MPI_DOUBLE,
				location(row + column, column, sqrtSize), 1,
		MPI_COMM_WORLD, &status);

	for (int i = 0; i < splitMatrixElementNum; i++) {
		a[i] = tmp_a[i];
		b[i] = tmp_b[i];
	}

	// 1.beginning: calculate the mutiply result
	// 2.for each step: 
	// 	 each split matrix of A was moved '1' steps to left
	//   each split matrix of B was moved '1' steps to up
	// 3.if implement n times then end
	//   else return 1
	for (int i = 0; i < sqrtSize; i++) {

		for (int j = 0; j < dim; j++) 
			for (int k = 0; k < dim; k++) 
				for (int m = 0; m < dim; m++) 
					c[j*dim + k] += a[j*dim + m] * b[m*dim + k];

		MPI_Sendrecv(a, splitMatrixElementNum, MPI_DOUBLE, 
					location(row, column - 1, sqrtSize), 1,
			tmp_a, splitMatrixElementNum, MPI_DOUBLE,
					location(row, column + 1, sqrtSize), 1,
			MPI_COMM_WORLD, &status);

		MPI_Sendrecv(b, splitMatrixElementNum, MPI_DOUBLE,
					location(row - 1, column, sqrtSize), 1,
			tmp_b, splitMatrixElementNum, MPI_DOUBLE,
					location(row + 1, column, sqrtSize), 1,
			MPI_COMM_WORLD, &status);

		for (int j = 0; j < splitMatrixElementNum; j++) {
			a[j] = tmp_a[j];
			b[j] = tmp_b[j];
		}
	}

	if (my_rank == 0) {
		int xMin, xMax, yMin, yMax;

		// value the no.0 process to matrix C 
		for (int i = 0; i < dim; i++) 
			for (int j = 0; j < dim; j++) 
				C[i][j] = c[i*dim + j];

		// receive the value of other processes
		for (int i = 1; i < comm_sz; i++) {
			MPI_Recv(c, splitMatrixElementNum, MPI_DOUBLE, i, 1,
				MPI_COMM_WORLD, &status);
			xMin = (i % sqrtSize) * dim;
			xMax = (i % sqrtSize + 1) * dim - 1;
			yMin = ( ( i - (i % sqrtSize) ) / sqrtSize) * dim;
			yMax = ( ( i - (i % sqrtSize) ) / sqrtSize + 1) * dim - 1;

			int k1 = 0;
			for (int j = yMin; j <= yMax; j++) {
				int k2 = 0;
				for (int m = xMin; m <= xMax; m++) {
					C[j][m] = c[k1*dim + k2];
					k2++;
				}
				k1++;
			}
		}

	} else {

		MPI_Send(c, splitMatrixElementNum, MPI_DOUBLE, 0, 1, 
			MPI_COMM_WORLD);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if (my_rank == 0) {
		finish = MPI_Wtime();
		printf("N: %d.\nthe total time of the work: %f.\n", 
			N, finish - start);
	}
	free(a); free(b); free(c); 
	free(tmp_a); free(tmp_b);

	MPI_Finalize();
	return 0;
}