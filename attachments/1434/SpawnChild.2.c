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
    MPI_Comm_get_parent(&InterComm);
    MPI_Intercomm_merge(InterComm, 1, &IntraComm);
    MPI_Comm_free(&InterComm);

    MPI_Comm_size(IntraComm, &NumberProcesses);
    MPI_Comm_rank(IntraComm, &ID);
    MPI_Get_processor_name(ProcessorName, &ProcessorNameLength);
    printf("Start child process %d of %d on %s in universe of %d\n",
	   ID, NumberProcesses, ProcessorName, *UniverseSize);

    MPI_Comm_disconnect(&IntraComm);

    MPI_Finalize();
    return 0;
}
