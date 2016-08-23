/*
 * testslave.c
 *
 * Copyright (c) 2008 - Roberto Fichera - TeknoSOFT
 *
 * This's an MPI slave application see the testmaster.c
 *
 * for any question send email to robyf@tekno-soft.it
 */

#include <stdio.h>
#include <unistd.h>

#include <assert.h>

#include <mpi.h>

#define TERMINATION  0x0eedb00f

int main( int argc, char *argv[] )
{
  MPI_Comm parent;
  int dataRecv = 0;
  int status = 0;
  char nodeName[128];

  getcwd( nodeName, 128 );

  status = MPI_Init( &argc, &argv );

  if ( status != MPI_SUCCESS )
  {
    fprintf( stderr, "Unable to initialize the MPI subsystem\n" );
    return( 1 );
  }

  status = MPI_Comm_get_parent( &parent );

  if ( status != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Comm_get_parent() return an error\n" );
    goto out;
  }

  if ( parent == MPI_COMM_NULL )
  {
    fprintf( stderr, "The testslave can't be executed as master!\n" );
    goto out;
  }

  /* receive the data */
  status = MPI_Recv( &dataRecv, 1, MPI_INT, 0, 0, parent, MPI_STATUS_IGNORE );

  if ( status != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Recv() return an error on node '%s', destroying thread\n", nodeName );
    goto out;
  }

  /* send it back */
  status = MPI_Send( &dataRecv, 1, MPI_INT, 0, 0, parent );

  if ( status != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Send() return an error on node '%s', destroying thread\n", nodeName );
    goto out;
  }

  /* termination */
  status = MPI_Send( NULL, 0, MPI_BYTE, 0, TERMINATION, parent );

  if ( status != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Send() return an error on node '%s' while notifying the termination\n", nodeName );
  }

out:
  MPI_Finalize();

  return( 0 );
}

