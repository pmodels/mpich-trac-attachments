#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

typedef struct {
	int    num_reqs;
	int    iters;
} params_t;

int comm_rank, comm_size;

void usage()
{
	fprintf(stderr, "./unexpected [options]\n\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t--reqs           Number of requests\n");
	fprintf(stderr, "\t--iters          Number of iterations\n");
	fprintf(stderr, "\n");
	MPI_Abort(MPI_COMM_WORLD, -1);
}

void get_parameters(int t_argc, char ** t_argv, params_t * params)
{
	int argc = t_argc;
	char ** argv = t_argv;

	params->num_reqs = -1;
	params->iters = -1;

	while (--argc && ++argv) {
		if (!strcmp(*argv, "--reqs")) {
			--argc; ++argv;
			params->num_reqs = atoi(*argv);
			continue;
		}
		if (!strcmp(*argv, "--iters")) {
			--argc; ++argv;
			params->iters = atoi(*argv);
			continue;
		}
	}

	if (params->num_reqs == -1 || params->iters == -1)
		usage();
}

int main(int argc, char ** argv)
{
	MPI_Request * reqs;
	MPI_Status  * status;
	params_t params;
	int i, j;
	double time, total_time;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

	get_parameters(argc, argv, &params);
	reqs = (void *) malloc((params.num_reqs + 1) * sizeof(MPI_Request));
	status = (void *) malloc((params.num_reqs + 1) * sizeof(MPI_Status));

	MPI_Barrier(MPI_COMM_WORLD);

	if (!comm_rank) {
		for (j = 0; j < params.iters; j++) {
			/* Send a bunch of messages of tag 0, and one message of tag 1 */
			for (i = 0; i < params.num_reqs; i++) {
				MPI_Isend(NULL, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &reqs[i]);
			}
			MPI_Isend(NULL, 0, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &reqs[i]);

			MPI_Barrier(MPI_COMM_WORLD);
			MPI_Waitall(params.num_reqs + 1, reqs, status);
			MPI_Barrier(MPI_COMM_WORLD);
		}
	}
	else {
		total_time = 0;
		for (j = 0; j < params.iters; j++) {
			/* This barrier should queue up all the messages with
			 * tags 0 and 1 in the unexpected queue. */
			MPI_Barrier(MPI_COMM_WORLD);

			/* Try to receive the message with tag 1; this will
			 * run through the entire unexpected message queue */
			time = MPI_Wtime();
			MPI_Recv(NULL, 0, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status[0]);
			total_time += MPI_Wtime() - time;

			/* Receive the remaining queued messages */
			for (i = 0; i < params.num_reqs; i++) {
				MPI_Recv(NULL, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status[0]);
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}

		printf("[%d] Queue search time: %.3f us\n", params.num_reqs, total_time * 1000000.0 / params.iters);
	}

	free(reqs);
	MPI_Finalize();

	return 0;
}
