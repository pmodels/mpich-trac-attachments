/* extracted from mpi4/getel.c */
#include "mpi.h"
#include <stdio.h>

int main( argc, argv )
int argc;
char **argv;
{
    char omessage[300];
    int i,j,count,sizemsg;
    int numtasks,me;
    int intmsg[300];
    double dmsg[300];
    MPI_Status status;

    MPI_Init( &argc, &argv );
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

 for (j=1;j<26;j++) {
  if( me == 0 ) {
    for (i=0;i<300;i++) omessage[i] = i;
    for (i=0;i<300;i++) dmsg[i] = (double) 2*i;

/*  Mismatch message count and datatype - CHAR to DOUBLE */
    sizemsg=j;
    MPI_Send(omessage, j, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
  }
  else if( me == 1 ) {
    MPI_Recv(dmsg, 300, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_DOUBLE, &count);
    if (count == MPI_UNDEFINED ) {
      printf("      j=%2d MPI_Get_count    is MPI_UNDEFINED.\n",j);
    } else {
      printf("      j=%2d MPI_Get_count    is %d.\n",j,count);
    }
    MPI_Get_elements(&status, MPI_DOUBLE, &count);
    if (count == MPI_UNDEFINED ) {
      printf("      j=%2d MPI_Get_elements is MPI_UNDEFINED.\n",j);
    } else {
      printf("      j=%2d MPI_Get_elements is %d.\n",j,count);
    }
    if ( (j % 8) == 0 && count != j/8 ) {
      printf("FAIL: j=%2d MPI_Get_elements should return j/8=%d, not %d.\n",j,j/8,count);
    } else if ( (j % 8) !=0 && count!=MPI_UNDEFINED) {
      printf("FAIL: j=%2d MPI_Get_elements should return MPI_UNDEFINED, not %d.\n",j,count);
    }
  }
 }

 for (j=1;j<10;j++) {
  if( me == 0 ) {
    for (i=0;i<300;i++) intmsg[i] = i;
    for (i=0;i<300;i++) dmsg[i] = (double) 2*i;

/*  Mismatch message count and datatype - INT to DOUBLE */
    sizemsg=j;
    MPI_Send(intmsg, sizemsg, MPI_INT, 1, 0, MPI_COMM_WORLD);
  }
  else if( me == 1 ) {
    MPI_Recv(dmsg, 300, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_DOUBLE, &count);
    if (count == MPI_UNDEFINED ) {
      printf("      j=%2d MPI_Get_count    is MPI_UNDEFINED.\n",j);
    } else {
      printf("      j=%2d MPI_Get_count    is %d.\n",j,count);
    }
    MPI_Get_elements(&status, MPI_DOUBLE, &count);
    if (count == MPI_UNDEFINED ) {
      printf("      j=%2d MPI_Get_elements is MPI_UNDEFINED.\n",j);
    } else {
      printf("      j=%2d MPI_Get_elements is %d.\n",j,count);
    } 
    if ( (j % 2) == 0 && count != j/2 ) {
      printf("FAIL: j=%2d MPI_Get_elements should return j/2=%d, not %d.\n",j,j/2,count);
    } else if ( (j % 2) !=0 && count!=MPI_UNDEFINED) {
      printf("FAIL: j=%2d MPI_Get_elements should return MPI_UNDEFINED, not %d.\n",j,count);
    }
  }
 }

    MPI_Finalize(); 
}
