#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "mpi.h"

int spawn_and_verify(char* progname, int root, MPI_Comm* intercomm)
{
     int myworld_sz, myworld_rank;
     MPI_Comm parentcomm;

     /* prepare MPI parameters */
     MPI_Comm_size(MPI_COMM_WORLD,&myworld_sz);
     MPI_Comm_rank(MPI_COMM_WORLD,&myworld_rank);
     
     /* check if this process is spawned */
     MPI_Comm_get_parent( &parentcomm );

     if (parentcomm == MPI_COMM_NULL)
     {
	  /* I am a parent */
	  int remotecomm_sz = 0;
	  int rc = 0;
	  int idx = 0;
	  int* errcodes = NULL;
	  int newprocs = 1;
	  char* newprocsp = NULL;

	  /* get environment setting for new procs number */
	  newprocsp = getenv("PE_DPM_NP");
	  if(newprocsp != NULL)
	  {
	      newprocs = atoi(newprocsp);
	      if(newprocs <= 0)
		  newprocs = 1;
	  }
	  
#ifdef DEBUG
	  fprintf(stdout, "I am in parent group, #%d will spawn new processes : %s\n",
		  root, progname);
#endif
      
	  /* allocate memory for error codes */
	  errcodes = (int*)malloc(sizeof(int)*newprocs);
	  if(errcodes == NULL)
	  {
	       fprintf(stderr, "memory allocation failed for errcodes\n");
	       MPI_Abort(MPI_COMM_WORLD, -1);
	  }

	  /* Create np more processes if no process is spawned yet */
	  rc = MPI_Comm_spawn(progname, MPI_ARGV_NULL, newprocs, MPI_INFO_NULL,
			      root, MPI_COMM_WORLD, intercomm, errcodes);
	  if (rc != MPI_SUCCESS)
	  {
	       /* no child process is spawned */
	       if (myworld_rank == root)
		    fprintf(stderr, "No child process can be spawned\n");
	  
	       /* Abort the operation */
	       free(errcodes);
	       MPI_Abort(MPI_COMM_WORLD, -1);
	  }
	  else
	  {
	       /* check how mamy process are spawned using the size of intercomm */
	       MPI_Comm_remote_size(*intercomm, &remotecomm_sz);
	       if(newprocs != remotecomm_sz)
	       {
		    /* there are failed processes */
		    if(myworld_rank == root)
		    {
			 fprintf(stderr, "%d process(es) is/are failed to be spawned.\n", newprocs - remotecomm_sz);

			 /* check the reason of failure */
			 for (idx = 0; idx < newprocs; idx++)
			 {
			      if (errcodes[idx] != MPI_SUCCESS)
			      {
				   /* this proc is failed */
				   fprintf(stderr, "New process #%d is failed to be spawned\n", idx);
			      }

			 }
		    }

	       }
	  }
	  free(errcodes);
	  return 1;
     }
     else
     {
	  /* I am a child */
#ifdef DEBUG
	  fprintf(stdout, "%d: I am in child group.\n", myworld_rank);
#endif
	  *intercomm = parentcomm;
	  return 0;
     }
}

/* 
void verify_rc(int rc, MPI_Comm comm)
{
    char errstring[MPI_MAX_ERROR_STRING];
    int stringlen;
    if(rc != MPI_SUCCESS)
    {
	MPI_Error_string(rc, errstring, &stringlen);
	errstring[stringlen]='\0';
	fprintf(stderr, "%s\n", errstring);
	MPI_Abort(comm, -1);
    }
}
*/

/*************************************************/
/* is_error(int error_code, int error_class)     */
/*                                               */
/* returns 1 if error_code is in error_class     */
/* and 0 otherwise                               */
/*************************************************/

int is_error(int error_code, int error_class){
    int ec;
    MPI_Error_class(error_code, &ec);
    if(ec == error_class) return 1;
    else return 0;
}
