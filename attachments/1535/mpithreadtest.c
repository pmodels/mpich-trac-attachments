#include <mpi.h>

#if _WIN32
# include <windows.h>
# include <process.h>
#else
# include <pthread.h>
#endif 

#include <assert.h>
#include <stdio.h>

#ifdef _WIN32
unsigned __stdcall tester( void* pArguments )
#else
void *tester(void *threadid)
#endif
{
    int flag;
    MPI_Is_thread_main(&flag);
    printf( "In second thread, is this thread Main? %s\n",
	 flag ? "Yes" : "No" );

#ifdef _WIN32
    _endthreadex( 0 );
#else
    pthread_exit(NULL);
#endif

	return 0;
} 


int main(int argc, char *argv[])
{
  int size, rank, len, flag;
  char name[MPI_MAX_PROCESSOR_NAME];
  int idx;
#ifdef _WIN32 
  HANDLE threads[5];
#else
  pthread_t threads[5];
#endif
  
  int provided;

  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  assert(provided == MPI_THREAD_MULTIPLE);

  MPI_Is_thread_main(&flag);
  printf( "In main thread, is this thread Main? %s\n",
       flag ? "Yes" : "No" );

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(name, &len);

  printf("Spawning 5 threads: process %d of %d on %s.\n", 
				  rank, size, name);
  for(idx=0; idx<5; idx++) {
	int threadID;
#ifdef _WIN32
	threads[idx] = (HANDLE)_beginthreadex( NULL, 0, 
                &tester, NULL, 0, &threadID );
#else
	pthread_create(&threads[idx], NULL, tester, NULL);
#endif
  }

  printf("Waiting 5 threads: process %d of %d on %s.\n", 
				  rank, size, name);
#ifdef _WIN32
  WaitForMultipleObjects(5, hthread, TRUE, -1);
#else
  for(idx=0; idx<5; idx++) {
    pthread_join(threads[idx], NULL);
  }
#endif
  printf("DONE: process %d of %d on %s.\n", 
				  rank, size, name);

  MPI_Finalize();
  return 0;
}

// vi:sts=4 sw=4 et tw=72
