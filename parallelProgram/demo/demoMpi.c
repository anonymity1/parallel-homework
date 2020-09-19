#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mpi.h"

const int MAXN = 1e2;

int main() {
	int n = MAXN;
	char greeting[MAXN];
	int my_rank, comm_sz;
	MPI_Status status;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	if (my_rank == 0) {
		printf("Greetings from process %d of %d!\n", 
			my_rank, comm_sz);
		for (int i = 1; i < comm_sz; i++) {
			MPI_Recv(greeting, n, MPI_CHAR, i, 0,
				MPI_COMM_WORLD, &status);
			printf("%s\n", greeting);
		}
	} else {
		sprintf(greeting, "Greetings from process %d of %d!",
			my_rank, comm_sz);
		MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0,
			MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}