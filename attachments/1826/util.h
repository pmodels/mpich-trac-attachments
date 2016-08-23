#ifndef DYN_PROC_MODEL_UTIL_H
#define DYN_PROC_MODEL_UTIL_H

#include <stdlib.h>
#include "mpi.h"

#ifdef __cplusplus
extern "C"
{
#endif

     /**
      * Spawn new process for parent group, and get inter-communicator back to parents
      * If the caller is a child process, then no new process will be spawned, and
      * inter-communicator with parent group will be got. Number of children to be
      * spawned is determined by PE_DPM_NP environment variable, by default one child
      * will be spawned.
      * Paramters:
      *        (IN)  progname: name of executable.
      *        (IN)  root: identifier of root process in parent group
      *        (OUT) intercomm: Inter-communicator that connect two group
      * Return: 
      *        A boolean(int) value that can be used to identify if caller process
      *        is in parent group. Zero value means that the caller process is in
      *        child group, otherwise, it is in parent group.
      */
  int spawn_and_verify(char* progname, int root, MPI_Comm* intercomm);


    /**
     * Function that can be used to verify return code of MPI calls.
     * If a non-success return code is passed as parameter, it prints
     * the error string into standard error stream, the terminate 
     * all the processes in communicator.
     * Parameters:
     *         (IN)  rc: return code.
     *         (IN)  comm: communicator containing all concerned processes.
     * Return: None
     */
/*    void verify_rc(int rc, MPI_Comm comm); */
#define verify_rc(rc, comm) \
    { \
	char* errstring;			\
	int stringlen;				\
	if(rc != MPI_SUCCESS)			\
	{							  \
	    errstring = (char*)malloc(sizeof(char)*MPI_MAX_ERROR_STRING); \
	    if(errstring != NULL)					\
	    {								\
		MPI_Error_string(rc, errstring, &stringlen);		\
		errstring[stringlen]='\0';				\
	    }								\
	    fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, errstring); \
	    MPI_Abort(comm, -1);					\
	}								\
    }
    

    int is_error(int error_code, int error_class);


  
#ifdef __cplusplus
}
#endif

#endif /* DYN_PROC_MODEL_UTIL_H */
