#include "mpi.h"
#include <stdio.h>

#define N 2

int main(int argc, char **argv) {
    int i, procs[N];
    MPI_Comm parentcomm, intercomm;
    char * cmds[N];
    MPI_Info infos[N];

    MPI_Init(&argc, &argv);

    MPI_Comm_get_parent(&parentcomm);
    if (parentcomm == MPI_COMM_NULL) {
        for (i=0; i<N; i++) {
            procs[i] = 2;
            cmds[i] = "./spawn_multiple";
            MPI_Info_create(&infos[i]);
            MPI_Info_set(infos[i], "hosts", "localhost");
        }
        MPI_Comm_spawn_multiple(N, cmds, MPI_ARGVS_NULL, procs, infos, 0, MPI_COMM_WORLD, &intercomm,
                                MPI_ERRCODES_IGNORE);
        puts("I'm the parent"); 

        for (i=0; i<N; i++)
            if (infos[i] != MPI_INFO_NULL)
                MPI_Info_free(&infos[i]);
    }
    else
        puts("I'm the spawned");

    MPI_Finalize();

    return 0;
}
