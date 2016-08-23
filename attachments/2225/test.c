#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    MPI_Info info;
    const char *key = "key", *val = "val";
    char buff[3 + 1]; /* strlen("val") + 1 */
    int flag;

    MPI_Init(&argc, &argv);

    MPI_Info_create(&info);
    MPI_Info_set(info, key, val);
    MPI_Info_get(info, key, sizeof(buff)-1, buff, &flag);
    if (flag)
        printf("key found: vaule=%s\n", buff);
    else
        printf("key not found\n");
    MPI_Info_free(&info);

    MPI_Finalize();

    return 0;
}
