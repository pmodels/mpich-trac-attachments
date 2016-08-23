
/*
 *
 * PROGRAM TEST for MPI_COMM_SPAWN_MULTIPLE
 *
 * the manager must be executed with the exec command of the worker as first input parameter
 *
 * program MANAGER
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



int main ( int argc, char* argv[] ) {
	
	
	
	PetscErrorCode		ierr;
	int					manager_rank,manager_size;
	char				host[MPI_MAX_PROCESSOR_NAME];
	MPI_Comm			intercomm;
	
	
	
	
	// *** PETSc SESSION ***
	
	//  Initialization of Petsc session
	ierr=PetscInitialize(&argc,&argv); CHKERRQ(ierr);
	ierr=PetscPrintf(PETSC_COMM_WORLD,"\n***** STARTING THE TEST PROGRAM ****\n\n"); CHKERRQ(ierr);
	
	
	
	// *** GET INFORMATION ABOUT THE WORLD COMMUNICATOR ***
	
	// Get the size and the rank within the comm
	MPI_Comm_size(PETSC_COMM_WORLD,&manager_size);
	ierr=PetscPrintf(PETSC_COMM_WORLD," Size:\t\t%i processes\n\t***\n",manager_size); CHKERRQ(ierr);
	MPI_Comm_rank(PETSC_COMM_WORLD,&manager_rank);
	// Get the name of the host
	ierr=PetscGetHostName(host,MPI_MAX_PROCESSOR_NAME); CHKERRQ(ierr);
	ierr=PetscSynchronizedPrintf(PETSC_COMM_WORLD," Rank %i runs on host: %s\n",manager_rank,host); CHKERRQ(ierr);
	ierr=PetscSynchronizedFlush(PETSC_COMM_WORLD); CHKERRQ(ierr);
	ierr=PetscPrintf(PETSC_COMM_WORLD,"\t***\n"); CHKERRQ(ierr);
	
	
	
	// *** PRINTING INFORMATION ABOUT THE ARGV ***
	
	ierr=PetscPrintf(PETSC_COMM_WORLD," The master program has %i arguments\n",argc); CHKERRQ(ierr);
	char * opts;
	ierr=PetscOptionsGetAll(&opts); CHKERRQ(ierr);
	int len=0;
	char c=opts[0];
	while (c!='\0') {
		len++;
		c=opts[len];
	}
	ierr=PetscPrintf(PETSC_COMM_WORLD," The string that contain the PetscOption database has %i characters\n",len); CHKERRQ(ierr);
	ierr=PetscPrintf(PETSC_COMM_WORLD,"\t***\n"); CHKERRQ(ierr);
	
	
	
	// *** EXECUTE THE SLAVE PROGRAM ***
	
	ierr=PetscPrintf(PETSC_COMM_WORLD," Now we are executing: %s program\n", argv[1]); CHKERRQ(ierr);
	// Setting the executable to launch and the number of process for each slave program, and the argv
	char 	*commands[manager_size];
	int 	proc[manager_size];
	char	**argvs[manager_size];
	for (int i=0;i<manager_size;i++) { commands[i]=argv[1]; proc[i]=1; argvs[i]=argv; }
	// All the other rank send to the rank 0 his host
	if ( manager_rank!=0 ) { ierr=MPI_Send(host,MPI_MAX_PROCESSOR_NAME,MPI_CHAR,0,69,PETSC_COMM_WORLD); CHKERRQ(ierr); }
	// Rank 0 creates the MPI_Info object and receives the host from the other ranks
	MPI_Info info[manager_size];
	if ( manager_rank==0 ) {
		char 	recv_host[MPI_MAX_PROCESSOR_NAME];
		// new rank 0 host
		ierr=MPI_Info_create(&info[manager_rank]); CHKERRQ(ierr);
		ierr=MPI_Info_set(info[manager_rank],"host",host); CHKERRQ(ierr);
		for (int i=1;i<manager_size;i++) {
			MPI_Status	recv_status;
			ierr=MPI_Recv(recv_host,MPI_MAX_PROCESSOR_NAME,MPI_CHAR,i,69,PETSC_COMM_WORLD,&recv_status); CHKERRQ(ierr);
			ierr=MPI_Info_create(&info[i]); CHKERRQ(ierr);
			ierr=MPI_Info_set(info[i],"host",recv_host); CHKERRQ(ierr);
		}
	}
	// Array with the errors
	int		spawn_errors[manager_size];
	// Launching the slaves
	ierr=MPI_Comm_spawn_multiple(manager_size,commands,argvs,proc,info,0,PETSC_COMM_WORLD,&intercomm,spawn_errors); CHKERRQ(ierr);
	// Check for some errors
	for ( int i=0;i<manager_size;i++ ) {
		if ( spawn_errors[i]!=MPI_SUCCESS ) { ierr=PetscPrintf(PETSC_COMM_WORLD,"Error with spawning process %i\n",i); CHKERRQ(ierr); return EXIT_FAILURE; }
	}
	
	
	// *** END OF THE PROGRAM AND PETSC SESSION ***
	
	ierr=PetscPrintf(PETSC_COMM_WORLD,"\n**** THE MASTER PROGRAM IS EXITING ****\n"); CHKERRQ(ierr);
	ierr=PetscFinalize(); CHKERRQ(ierr);
	return EXIT_SUCCESS;
	
}