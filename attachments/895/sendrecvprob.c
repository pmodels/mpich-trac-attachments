#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/*
  cl /IC:\sb\mpich2-1.2\src\include\win32 /IC:\sb\mpich2-1.2\src\include sendrecvprob.c C:\sb\mpich2-1.2\winbuild\x86\Debug\nemesis\mpich2nemesisd.lib
  C:\sb\mpich2-1.2\winbuild\x86\Debug\bin\mpiexec -l -hosts 2 uk-ellise uk-ellise-lt \\mathworks\UK\shome\eellis\work\MPI-C\sendrecvprob.exe 10 64000000
  [1]Fatal error in MPI_Recv: Other MPI error, error stack:
  [1]MPI_Recv(187)............................: MPI_Recv(buf=047B0020, count=64000000, MPI_UNSIGNED_CHAR, src=0, tag=0, MPI_COMM_WORLD, status=00000001) failed
  [1]MPIDI_CH3I_Progress(150).................:
  [1]MPID_nem_mpich2_blocking_recv(948).......:
  [1]MPID_nem_newtcp_module_poll(154).........:
  [1]MPID_nem_newtcp_module_connpoll(1841)....:
  [1]state_commrdy_handler(1663)..............:
  [1]MPID_nem_newtcp_module_recv_handler(1575):
  [1]MPID_nem_handle_pkt(492).................:
  [1]MPIDI_CH3_PktHandler_RndvClrToSend(252)..: failure occurred while attempting to send message data
  [1]MPID_nem_newtcp_iSendContig(376).........:
  [1]MPID_nem_newtcp_iSendContig(375).........:  Unable to write to a socket, An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.
  [1] (errno 10055)

  job aborted:
  rank: node: exit code[: error message]
  0: uk-ellise: 123
  1: uk-ellise-lt: 1: process 1 exited without calling finalize

*/

int main( int argc, char ** argv ) {
    
    int numIter = 0, numBytes = 0;
    int src, dest, i;
    int rank, size;
    unsigned char * sendBuf, * recvBuf;

    if ( argc != 3 ) {
        return 10 + argc;
    }
    numIter = atoi( argv[1] );
    numBytes = atoi( argv[2] );

    sendBuf = ( unsigned char * ) malloc( numBytes );
    assert( sendBuf != 0 );
    recvBuf = ( unsigned char * ) malloc( numBytes );
    assert( recvBuf != 0 );

    MPI_Init( 0, 0 );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    
    for ( i = 0; i < numIter; ++i ) {
        
        MPI_Request req;
        int offset = i % size;
        
        dest = ( rank + offset ) % size;
        src  = ( rank - offset + size ) % size;
        
        MPI_Isend( sendBuf, numBytes, MPI_UNSIGNED_CHAR, dest, 0, MPI_COMM_WORLD, &req );
        MPI_Recv( recvBuf, numBytes, MPI_UNSIGNED_CHAR, src,  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        
        MPI_Wait( &req, MPI_STATUS_IGNORE );
    }
    
    MPI_Finalize();

    free( sendBuf );
    free( recvBuf );
}
