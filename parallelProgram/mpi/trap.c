#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#define MAXN 1e2

double target(double a) {
	return a * a;
}

double trap(double a, double b, double h) {
	return (a + b) * h / 2;
}

void getInput(int my_rank, int comm_sz, 
			double* pa, double* pb, int* pn) {
	int dest; 
	if (my_rank == 0) {
		printf("Enter a, b and n: ");
		scanf("%lf %lf %d", pa, pb, pn);
	}
	MPI_Bcast(pa, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(pb, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(pn, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

int main() {
	int my_rank = 0;
	int comm_sz = 0;

	int n = MAXN, localn = 0;
	double a = 0.0, b = 3.0, h = 0;
	double locala = 0, localb = 0;
	double localIntegral = 0, totalIntegral = 0; 

	double start, finish, duration;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	getInput(my_rank, comm_sz, &a, &b, &n);

	start = MPI_Wtime();

	h = (b - a) / n;
	localn = n / comm_sz;

	locala = a + my_rank * localn * h;
	localb = locala + localn * h;
	localIntegral = 
		trap(target(locala), target(localb), localn * h);

	MPI_Reduce(&localIntegral, 
		&totalIntegral, 1, MPI_DOUBLE, MPI_SUM, 0, 
		MPI_COMM_WORLD);

	// MPI_Allreduce: all process get the result, no fifth vriable.

	if (my_rank == 0) {
		double finish = MPI_Wtime();
		duration = finish - start;
		printf("the total time of the program is: %lf. \n", duration);
		printf("Our estimate localIntegral is: %lf. \n", totalIntegral);
	}

	MPI_Finalize();
}