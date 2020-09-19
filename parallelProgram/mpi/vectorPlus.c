#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#define MAXN 10

void vectorPlus(double localx[], double localy[], 
				double localz[], int localn) {
	for (int locali = 0; locali < localn; locali++) {
		localz[locali] = localx[locali] + localy[locali];
	}
}

void getInput(double localx[], char name[], int localn,  
			int my_rank, MPI_Comm comm, int n) {
	double* a = NULL;
	if (my_rank == 0) {
		a = (double*) malloc( n * sizeof(double) );
		printf("Enter vector %s: ", name);
		for (int i = 0; i < n; i++) {
			scanf("%lf", &a[i]);
		}

		MPI_Scatter(a, localn, MPI_DOUBLE, 
					localx, localn, MPI_DOUBLE, 
					0, comm);
		free(a);
	} else {
		MPI_Scatter(a, localn, MPI_DOUBLE,
					localx, localn, MPI_DOUBLE, 
					0, comm);
	}
}

void outputVector(double localz[], char name[], int localn,  
				int my_rank, MPI_Comm comm, int n) {
	double* b = NULL;
	if (my_rank == 0) {
		b = (double*) malloc( n * sizeof(double) );
		MPI_Gather(localz, localn, MPI_DOUBLE, 
					b, localn, MPI_DOUBLE, 
					0, comm);
		printf("%s: ", name);
		for (int i = 0; i < n; i++) {
			printf("%lf ", b[i]);
		}
		printf("\n");
		free(b);
	} else {
		MPI_Gather(localz, localn, MPI_DOUBLE, 
					b, localn, MPI_DOUBLE, 
					0, comm);
	}
}

int main() {
	int my_rank = 0;
	int comm_sz = 0;

	int n = MAXN;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	int localn = MAXN / comm_sz;
	double localx[n], localy[n], localz[n];

	getInput(localx, "x", localn, 
			my_rank, MPI_COMM_WORLD, n);
	getInput(localy, "y", localn, 
			my_rank, MPI_COMM_WORLD, n);

	vectorPlus(localx, localy, localz, localn);

	outputVector(localz, "z", localn, 
				my_rank, MPI_COMM_WORLD, n);

	MPI_Finalize();
	return 0;
}