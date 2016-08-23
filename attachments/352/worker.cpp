
/*
 *
 * PROGRAM TEST for MPI_COMM_SPAWN_MULTIPLE
 *
 * process WORKER
 *
 * Author:  Federico Golfrè Andreasi
 * Created: 12-01-2009
 *
*/


#include "mpi.h"
#include "petsc.h"
#include "petscsys.h"
#include <string>

using namespace std;



int main (int argc, char *argv[]) {
	
	
	PetscErrorCode		ierr;
	int					worker_rank,worker_size;
	char				host[MPI_MAX_PROCESSOR_NAME];
	
	
	
	// *** PETSc SESSION ***
	
	// Initialization of a PETSc session
	ierr=PetscInitialize(&argc,&argv); CHKERRQ(ierr);
	ierr=PetscPrintf(PETSC_COMM_WORLD,"\n **** STARTED THE WORKER PROGRAM ****\n\n"); CHKERRQ(ierr);
	
	
	
	// *** GET INFORMATION ABOUT THE WORKER WORLD COMMUNICATOR ***
	
	// Get the size and the rank within the worker comm
	MPI_Comm_size(PETSC_COMM_WORLD,&worker_size);
	ierr=PetscPrintf(PETSC_COMM_WORLD," Size:\t\t%i processes\n\t***\n",worker_size); CHKERRQ(ierr);
	MPI_Comm_rank(PETSC_COMM_WORLD,&worker_rank);
	// Get the name of the host
	ierr=PetscGetHostName(host,MPI_MAX_PROCESSOR_NAME); CHKERRQ(ierr);
	ierr=PetscSynchronizedPrintf(PETSC_COMM_WORLD," Rank %i runs on host: %s\n",worker_rank,host); CHKERRQ(ierr);
	ierr=PetscSynchronizedFlush(PETSC_COMM_WORLD); CHKERRQ(ierr);
	ierr=PetscPrintf(PETSC_COMM_WORLD,"\t***\n"); CHKERRQ(ierr);
	
	
	
	// *** PRINT THE ARGV AND ARGC VARIABLES ***
	
	ierr=PetscPrintf(PETSC_COMM_WORLD," Argc=%i\n",argc); CHKERRQ(ierr);
	for (int i=0;i<=argc;i++) {
		ierr=PetscPrintf(PETSC_COMM_WORLD,"\targv[%i]=%s\n",i,argv[i]); CHKERRQ(ierr);
	}
	ierr=PetscPrintf(PETSC_COMM_WORLD,"\t***\n"); CHKERRQ(ierr);
	
	
	
	// *** END OF PETSC SESSION ***
	
	ierr=PetscPrintf(PETSC_COMM_WORLD," **** WORKER PROGRAM EXITS ****\n\n"); CHKERRQ(ierr);
	ierr=PetscFinalize(); CHKERRQ(ierr);
	return EXIT_SUCCESS;
	
}