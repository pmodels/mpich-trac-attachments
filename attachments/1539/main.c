#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MPITAG 0xDEAD

void run_regression(int rank, int size)
{
	int *recv_result;

	int tag = MPITAG;
	MPI_Status status;
	MPI_Request *p_recv_request = NULL;

	if (rank == 0) {
		int i, j=-1;
		int idx;
		p_recv_request = (MPI_Request*) calloc((size-1),  sizeof(MPI_Request));
		recv_result = (int*) calloc((size-1), sizeof(int));
		//Initially, 
		for (i=1; i<size; i++) {
			MPI_Irecv(&recv_result[i-1], 1, MPI_INT, i, tag, 
									MPI_COMM_WORLD, &p_recv_request[i-1]);
		}
		for (i=1; i<size; i++) {
			MPI_Waitany(size-1, p_recv_request, &idx, &status);
			printf("Rank-%d Got signal from Rank-%d\n", rank, idx+1);
			MPI_Send(&j, 1, MPI_INT, idx+1, tag, MPI_COMM_WORLD);
		}	
	}
	else {
		//Send ready to master
		int i = 1;
		MPI_Send(&i, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
		printf("Rank-%d Sent ready signal to Rank-0\n", rank);
		system ("/home/install/mpich/mpich2-1.4.1/linux_x86_64/bin/mpiexec -n 3 hostname");
		//system ("hostname");
		MPI_Recv(&i, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
	}
	return;
}	

int main (int argc, char* argv[])
{
	int rank, size;
	int flag = 0;
	MPI_Status status;
	
	MPI_Initialized(&flag);
	if (!flag) {
		printf ("Calling MPI initialize \n");
		MPI_Init(&argc, &argv);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	run_regression (rank, size);
	printf ("Rank-%d Calling finalize\n", rank);
	MPI_Finalize();

	return 0;
}

