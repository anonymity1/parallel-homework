#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpi.h"

#define matrixSize 800
#define rowSizeA matrixSize
#define columnSizeA matrixSize
#define rowSizeB columnSizeA
#define columnSizeB matrixSize

void outputInfo(int n) { // comm_sz
	printf("1 process for management and %d for calculation.\n", n - 1);
}

void outputMat(int tmp[][matrixSize], int row, int column, char* MatrixName) {
	printf("%s\n", MatrixName);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			printf("%d ", tmp[i][j]);
		}
		printf("\n");
	}
}

int main() {

	int my_rank, comm_sz;
	int rows, offset;
	
	int a[rowSizeA][columnSizeA], 
		b[rowSizeB][columnSizeB], res[rowSizeA][columnSizeB];

	MPI_Status status;
	MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

    if (my_rank == 0) { // main thread
    	for (int i = 0; i < rowSizeA; i++) {
    		for (int j = 0; j < columnSizeA; j++) {
    			a[i][j] = rand() % matrixSize;
    		}
    	}
    	for (int i = 0; i < rowSizeB; i++) {
    		for (int j = 0; j < columnSizeB; j++){
    			b[i][j] = rand() % matrixSize;
    		}
    	}

    	outputInfo(comm_sz);
    	// outputMat(a, rowSizeA, columnSizeA, "Matrix A: ");
    	// outputMat(b, rowSizeB, columnSizeB, "Matrix B: ");
    	double start = MPI_Wtime();

    	offset = 0;
    	for (int dest = 1; dest < comm_sz; dest++) {
    		if (dest <= rowSizeA % (comm_sz - 1)) {
    			rows = rowSizeA / (comm_sz - 1) + 1;
    		} else {
    			rows = rowSizeA / (comm_sz - 1);
    		}

    		// send 'rows' rows to process dest
    		printf("Send %d rows to process %d.\n", rows, dest);
    		MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
    		MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
    		MPI_Send(&a[offset][0], rows*columnSizeA, MPI_INT, 
    			dest, 1, MPI_COMM_WORLD);
    		MPI_Send(&b, rowSizeB*columnSizeB, MPI_INT, 
    			dest, 1, MPI_COMM_WORLD);
    		offset = offset + rows;
    	}

    	for (int i = 1; i < comm_sz; i++) {

    		// recieve rows from 
    		MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&res[offset][0], rows*columnSizeB, 
            	MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            printf("Recieve result from %d process.\n", i);
    	}
    	// outputMat(res, rowSizeA, columnSizeB, "result: ");
    	double finish = MPI_Wtime();
    	printf("the total time of the work: %f.\n", finish - start);
    } else {
    	// other threads

    	MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    	MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    	MPI_Recv(&a, rows*columnSizeA, 
    		MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    	MPI_Recv(&b, rowSizeB*columnSizeB, 
    		MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

    	for (int k = 0; k < columnSizeB; k++) {
    		for (int i = 0; i < rows; i++) {
    			res[i][k] = 0;
    			for (int j = 0; j < columnSizeA; j++) {
    				res[i][k] = res[i][k] + a[i][j] * b[j][k];
    			}
    		}
    	}

    	MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&res, rows*columnSizeB, MPI_INT, 0, 2, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}