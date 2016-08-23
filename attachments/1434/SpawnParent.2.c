/* -*- Mode: C; c-basic-offset:4 ; -*- */

#include "mpi.h"
#include <stdio.h>
#include <malloc.h>

int main(int argc, char* argv[])
{
    int ID, NumberProcesses;
    char ProcessorName[MPI_MAX_PROCESSOR_NAME];
    int ProcessorNameLength;
    int *UniverseSize, Flag;
    MPI_Comm InterComm, IntraComm;

    MPI_Init(&argc, &argv);
    MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &UniverseSize, &Flag);
    MPI_Comm_size(MPI_COMM_WORLD, &NumberProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);

    MPI_Get_processor_name(ProcessorName, &ProcessorNameLength);
    printf("Start parent process %d of %d on %s in universe of %d\n",
	   ID, NumberProcesses, ProcessorName, *UniverseSize);

    if (Flag && (*UniverseSize > 1) && (NumberProcesses == 1))
	{
	    int i;

	    for (i = 0; i < 3; ++i)
		{
		    int Error;
		    int* ErrorCodes;

		    ErrorCodes = (int*) calloc(*UniverseSize - 1, sizeof(int));
		    printf("Spawn %d children\n", *UniverseSize - 1);
		    Error = MPI_Comm_spawn("./SpawnChild", MPI_ARGV_NULL, *UniverseSize - 1,
					   MPI_INFO_NULL, 0, MPI_COMM_WORLD,
					   &InterComm, ErrorCodes);
		    if (Error)
			printf("Can't spawn child processes\n");
		    MPI_Intercomm_merge(InterComm, 0, &IntraComm);
                    MPI_Comm_free(&InterComm);
		    free(ErrorCodes);
		    MPI_Comm_size(IntraComm, &NumberProcesses);
		    MPI_Comm_rank(IntraComm, &ID);

                    MPI_Comm_disconnect(&IntraComm);
		    sleep(1);
		}
	}

    sleep(10);

    MPI_Finalize();
    return 0;
}
