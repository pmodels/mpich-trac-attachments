/*
 * Testmaster.c
 *
 * Copyright (c) 2008 - Roberto Fichera - TeknoSOFT
 *
 * This's a multithreaded MPI master application which try to stress the MPI subsystem
 * when in MPI_THREAD_MULTIPLE
 *
 * for any question send email to robyf@tekno-soft.it
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <assert.h>

#include <pthread.h>

#include <mpi.h>

#define REQUIRE( __cond ) assert( __cond )

#define TERMINATION  0x0eedb00f
#define SLAVEJOB "./testslave.sh"

typedef struct MPIThread
{
  /* pthread stuff */
  pthread_t           thread;
  pthread_attr_t      threadAttr;
  int                 running;
}
MPIThread;

typedef struct NodeThread
{
  MPIThread           *thread;
  char                *nodeName;

  /* MPI stuff */
  MPI_Info            info;
  MPI_Comm            childComm;

  struct NodeThread *next;
}
NodeThread;


static void *NodeThread_threadMain( void *arg );


static MPIThread *MPIThread_new( void )
{
  return( malloc( sizeof( MPIThread ) ) );
}

static int MPIThread_init( MPIThread *self )
{
  int status;

  REQUIRE( self );

  status = pthread_attr_init( &self->threadAttr );
  REQUIRE( status == 0 );

  status = pthread_attr_setdetachstate( &self->threadAttr, PTHREAD_CREATE_DETACHED );
  assert( status == 0 );

  self->running = 0;
}

static int MPIThread_start( MPIThread *self, void * ( *threadMain )( void* ), void *arg )
{
  int status;

  REQUIRE( self );
  REQUIRE( threadMain );

  status = pthread_create( &self->thread, &self->threadAttr, threadMain, (void*)arg );

  if ( status == 0 )
  {
    self->running = 1;
  }

  return( status );
}

static inline int MPIThread_isRunning( MPIThread *self )
{
  REQUIRE( self );

  return( self->running );
}

static inline void MPIThread_setRunning( MPIThread *self, int running )
{
  REQUIRE( self );

  self->running = running;
}

static void MPIThread_clear( MPIThread *self )
{
  int status;

  REQUIRE( self );

  status = pthread_attr_destroy( &self->threadAttr );
  REQUIRE( status == 0 );
}

static MPIThread_delete( MPIThread *self )
{
  REQUIRE( self );
  free( self );
}

/*
 * build a ring of nodes
 */

static NodeThread *NodeThread_create( const char *nodeName )
{
  NodeThread *self = NULL;
  int status = 0;

  REQUIRE( nodeName );

  self = (NodeThread*)malloc( sizeof( NodeThread ) );
  REQUIRE( self );

  status = MPI_Info_create( &self->info );

  if ( status != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Info_create() return an error\n" );
    abort();
  }

  self->thread = MPIThread_new();
  REQUIRE( self->thread );

  MPIThread_init( self->thread );

  self->nodeName = strdup( nodeName );
  REQUIRE( self->nodeName );

  self->next = NULL;

  return( self );
}

static NodeThread *NodeThread_initFromFile( const char *filename, int *numNodes )
{
  NodeThread *head = NULL;
  NodeThread *tail = NULL;
  FILE *fd = NULL;
  char hostName[1024];
  char nodeName[1024];
  int len;

  REQUIRE( filename );
  REQUIRE( numNodes );

  gethostname( hostName, 1024 );

  if ( ( fd = fopen( filename, "r" ) ) == NULL )
  {
    fprintf( stderr, "Unable to open() the file '%s'\n",  filename );
    goto out;
  }

  *numNodes = 0;

  /* read the node file one host per line */
  while( !feof( fd ) )
  {
    if ( fgets( nodeName, 1024, fd ) == NULL )
    {
      continue;
    }

    len = strlen( nodeName );

    if ( len < 1 )
    {
      continue;
    }

    /* remove the final \n */
    nodeName[ len - 1 ] = '\0';

    if ( strcmp( hostName, nodeName ) == 0 )
    {
      continue;
    }

    if ( !head )
    {
      head = tail = NodeThread_create( nodeName );
      REQUIRE( head );
    }
    else /* add in the tail */
    {
      tail->next = NodeThread_create( nodeName );
      REQUIRE( tail->next );

      tail = tail->next;
    }

    (*numNodes)++;

    fprintf( stderr, "... adding node #%d host is '%s'\n", *numNodes, nodeName );
  }

  /*
   * if everything is ok make it a ring
   */
  if ( head && tail )
  {
    tail->next = head;
  }

  fclose( fd );

  fprintf( stderr, "A %d node's ring has been made\n", *numNodes );


out:
  return( head );
}

static int NodeThread_isRunning( NodeThread *self )
{
  REQUIRE( self );
  REQUIRE( self->thread );

  return( MPIThread_isRunning( self->thread ) );
}

static int NodeThread_start( NodeThread *self )
{
  int status = 0;

  REQUIRE( self );
  REQUIRE( self->thread );

  return( MPIThread_start( self->thread, NodeThread_threadMain, self ) );
}

static NodeThread *NodeThread_next( NodeThread *self )
{
  REQUIRE( self );

  return( self->next );
}

static void NodeThread_setSlaveInfo( NodeThread *self, char *key, const char *value )
{
  int error = 0;

  REQUIRE( self );
  REQUIRE( key );
  REQUIRE( value );

  error = MPI_Info_set( self->info, key, (char*)value );

  if ( error != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Info_set( '%s', '%s' ) return an error\n", key, value );
  }
}

static void NodeThread_resetSlaveInfo( NodeThread *self, char *key )
{
  int error = 0;

  REQUIRE( self );
  REQUIRE( key );

  error = MPI_Info_delete( self->info, key );

  if ( error != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Info_delete( '%s' ) return an error\n", key );
  }
}

static int NodeThread_spawnSlave( NodeThread *self )
{
  int retVal = 0;
  int error = 0;
  int *errorCode = NULL;
  char cwd[256];

  REQUIRE( self );

  fprintf( stderr, "Setting up the host as '%s'\n", self->nodeName );

  NodeThread_setSlaveInfo( self, "host", self->nodeName );

  getcwd( cwd, 256 );

  fprintf( stderr, "Setting the work directory as '%s'\n", cwd );
  NodeThread_setSlaveInfo( self, "wdir", cwd );

  /* spawn the command */
  fprintf( stderr, "Spawning a task '%s' on node '%s'\n", SLAVEJOB, self->nodeName );

  error = MPI_Comm_spawn( (char *)SLAVEJOB, (char**)NULL,       /* command and its arguments */
                          1,                                    /* number of process to spawn */
                          self->info,                           /* communicator info for the process */
                          0,                                    /* we are always root */
                          MPI_COMM_WORLD,                       /* our communicator */
                          &self->childComm,                     /* child intercommunicator */
                          errorCode );                          /* set of error codes */

  if ( error != MPI_SUCCESS )
  {
    fprintf( stderr, "Unable to spawn the command with MPI_Comm_spawn() on node '%s'\n", self->nodeName );
    goto out;
  }

  error = MPI_Comm_set_errhandler( self->childComm, MPI_ERRORS_RETURN );

  if ( error != MPI_SUCCESS )
  {
    fprintf( stderr, "Unable to set the errorhandler with the command with MPI_Comm_set_errhandler() on node '%s'\n", self->nodeName );
    goto out;
  }

  retVal = 1;

out:
  return( retVal );
}

static void NodeThread_waitSlaveTermination( NodeThread *self )
{
  int error = 0;

  REQUIRE( self );

  fprintf( stderr, "Receiving the notification of the slave termination on node '%s'\n", self->nodeName );

  /* wait the program termination */
  error = MPI_Recv( NULL, 0, MPI_BYTE, 0, TERMINATION,
                      self->childComm, MPI_STATUS_IGNORE );

  if ( error != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Recv() return an error while waiting the slave termination on node '%s'\n", self->nodeName );
  }
}

static void NodeThread_destroySlave( NodeThread *self )
{
  int error = 0;

  REQUIRE( self );

  fprintf( stderr, "Closing the slave communication with node '%s'\n", self->nodeName );

  error = MPI_Comm_disconnect( &self->childComm );

  if ( error != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Comm_disconnect() return an error with node '%s'\n", self->nodeName );
  }

  // fprintf( stderr, "Removing the host on node '%s'\n", self->nodeName );
  NodeThread_resetSlaveInfo( self, "host" );

  // fprintf( stderr, "Removing the work dir on node '%s'\n", self->nodeName );
  NodeThread_resetSlaveInfo( self, "wdir" );
}


/*
 ******* THE MAIN PROGRAM *********
 */

int main( int argc, char *argv[] )
{
  NodeThread *head = NULL;
  NodeThread *current = NULL;
  int numNodes = 0;
  int numJobs = 0;
  int jobCount = 0;
  int provided, err;

  if ( argc != 3 )
  {
    fprintf( stderr, "Usage: testmaster <numJobs> <nodefile>\n" );
    exit( 1 );
  }

  numJobs = atoi( argv[1] );

  fprintf( stderr, "Intializing MPI ...\n" );

  err = MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided );

  if ( err != MPI_SUCCESS )
  {
    printf( "MPI_Init_thread() doesn't return MPI_SUCCESS\n" );
    return( -1 );
  }

  if ( provided != MPI_THREAD_MULTIPLE )
  {
    fprintf( stderr, "Your MPI implementation doesn't provide the MPI_THREAD_MULTIPLE, please fix it\n" );
    return( 2 );
  }

  fprintf( stderr, "Loading the node's ring from file '%s'\n", argv[2] );

  head = NodeThread_initFromFile( argv[2], &numNodes );

  if ( !head )
  {
    fprintf( stderr, "Unable to build the node's ring\n" );
    exit( 2 );
  }

  fprintf( stderr, "At least one node is available, let's start to distribute %d job across %d nodes!!!\n",
            numJobs, numNodes );

  current = head;

  while( jobCount < numJobs )
  {
    if ( !NodeThread_isRunning( current ) )
    {
      if ( NodeThread_start( current ) != 0 )
      {
        fprintf( stderr, "Something has failed, aborting!!!!\n" );
        abort();
      }

      jobCount++;

      fprintf( stderr, "****************** Starting job #%d\n", jobCount );
    }

    current = NodeThread_next( current );

  }

  /* just too tired to code the cleanup */
  sleep( 10 );

  MPI_Finalize();

  return( 0 );
}


static void *NodeThread_threadMain( void *arg )
{
  NodeThread *self = (NodeThread *)arg;
  int status = 0;
  int dataSend = 0;
  int dataRecv = 0;
  unsigned int seed = 0;

  REQUIRE( self );

  srand( time( NULL ) );

  NodeThread_spawnSlave( self );

  dataSend = rand_r( &seed );

  /* send an int */
  status = MPI_Send( &dataSend, 1, MPI_INT, 0, 0, self->childComm );

  if ( status != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Send() return an error on node '%s', destroying thread\n", self->nodeName );
    goto destroy;
  }

  /* receive it back */

  status = MPI_Recv( &dataRecv, 1, MPI_INT, 0, 0, self->childComm, MPI_STATUS_IGNORE );

  if ( status != MPI_SUCCESS )
  {
    fprintf( stderr, "MPI_Recv() return an error on node '%s', destroying thread\n", self->nodeName );
    goto destroy;
  }

  if ( dataSend != dataRecv )
  {
    fprintf( stderr, "############ sent( %d ) != received( %d ) ############\n" );
  }

  NodeThread_waitSlaveTermination( self );

  sleep( 1 );

 destroy:

  NodeThread_destroySlave( self );

  MPIThread_setRunning( self->thread, 0 );

  return( NULL );
}

