#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "data_access_nblock_params.h"

#include "util.h"

#define MPI_TEST_BLOCK_SIZE  F_IWRITE_AT_8_BLKSIZE 
#define MPI_TEST_CONC_WRITES F_IWRITE_AT_8_CONC_WRITES

int main (int argc, char** argv)
{ 
  /* Variables */
  long int data[MPI_TEST_CONC_WRITES][MPI_TEST_BLOCK_SIZE][2];
  long int read_data[MPI_TEST_CONC_WRITES][MPI_TEST_BLOCK_SIZE][2];
  int err_code;
  int count;
  int i,j,k;
  MPI_File fh;
  MPI_Status  status_arr[MPI_TEST_CONC_WRITES];
  MPI_Request request_arr[MPI_TEST_CONC_WRITES];
  FILE *fh2;
  int datatype_size;
  MPI_Datatype cont_type;
  MPI_Offset   write_pos;
  long int exp_val1;
  long int exp_val2;
  long int real_val1;
  long int real_val2;

  MPI_Group universe_group;
  MPI_Group new_group;
  MPI_Comm  new_comm, intercomm, intracomm;
  int *ranks;
  int isparent, root = 0, universe_rank, universe_size;
  
  /* Initialize test: set variables mypid and nnodes, define errors.  */
  MPI_Init(&argc, &argv);
  isparent = spawn_and_verify(argv[0], root, &intercomm);
  err_code = MPI_Intercomm_merge(intercomm, isparent, &intracomm);
  verify_rc(err_code, intercomm);
  MPI_Comm_size(intracomm, &universe_size);
  MPI_Comm_rank(intracomm, &universe_rank);

  /* Start of initialization */

  /******* Generate new MPI-Type: cont_type ******/

  err_code = MPI_Type_contiguous(2,MPI_LONG, &cont_type);
  verify_rc(err_code, intracomm);

  MPI_Type_commit(&cont_type);
  verify_rc(err_code, intracomm);

  MPI_Type_size(cont_type,&datatype_size);


  /* Generate group & communicator with one member */

  j = 0;

  ranks = (int*)malloc(sizeof(int) * (universe_size - 1));
  for(i = 0; i < universe_size; i++){
     if(i != universe_rank){
       ranks[j] = i;
       j += 1;
     }
  }

  MPI_Comm_group(intracomm, &universe_group);
  MPI_Group_excl(universe_group, universe_size - 1, ranks, &new_group);
  MPI_Comm_create(intracomm, new_group, &new_comm);

 
  /* Init data */

  for(i = 0; i < MPI_TEST_CONC_WRITES; i++){ 
    for(j = 0; j < MPI_TEST_BLOCK_SIZE; j++){
       data[i][j][0] = (j + i * MPI_TEST_BLOCK_SIZE + universe_rank * \
                        MPI_TEST_BLOCK_SIZE * MPI_TEST_CONC_WRITES); 
       data[i][j][1] = -(j + i * MPI_TEST_BLOCK_SIZE + universe_rank * \
                        MPI_TEST_BLOCK_SIZE * MPI_TEST_CONC_WRITES);
    }
  }

  /* End of initialization */

  /* Start of test */


  /* open file */

  err_code = MPI_File_open(new_comm,"f_iwrite_at_8.dat",MPI_MODE_RDWR | \
                           MPI_MODE_CREATE , MPI_INFO_NULL, &fh);
  verify_rc(err_code, intracomm);

  /* Start MPI_TEST_CONC_WRITES concurrent nonblocking write-operations */

  for(i = 0; i < MPI_TEST_CONC_WRITES; i++){

    /* write data */

    write_pos = (MPI_TEST_CONC_WRITES * MPI_TEST_BLOCK_SIZE * universe_rank + \
                i * MPI_TEST_BLOCK_SIZE) * datatype_size;

    err_code = MPI_File_iwrite_at(fh,write_pos,data[i],MPI_TEST_BLOCK_SIZE,\
                                  cont_type,&(request_arr[i]));    
    verify_rc(err_code, intracomm);

  }

  /* wait for completion */

  MPI_Waitall(MPI_TEST_CONC_WRITES, request_arr, status_arr);

  /* close file */

  err_code = MPI_File_close(&fh);
  verify_rc(err_code, intracomm);

  MPI_Barrier(intracomm);
 
  /* check written data */

  for(i = 0; i < MPI_TEST_CONC_WRITES; i++){
     MPI_Get_count(&(status_arr[i]), cont_type, &count);
     if(count != MPI_TEST_BLOCK_SIZE){
	 fprintf(stderr,"\nERROR: Wrong number of elements written (%i) --> should be %i.",count,MPI_TEST_BLOCK_SIZE); 
	 MPI_Abort(intracomm, -1);
     }
     else{
#ifdef DEBUG
	 printf("\nWrite operation %i in proc %i wrote %i elements ",i,universe_rank,count);
#endif
     }
  }

  /* test if correct values have been written: */

  if(universe_rank == 0){
      fh2 = fopen("f_iwrite_at_8.dat","rb");
      if(fh2 == 0){
	  fprintf(stderr,"\nfopen failed!\n");
	  MPI_Abort(intracomm, -1);
      }
      
      for(i = 0; i < universe_size; i++){
	  fread(read_data, sizeof(long int) * 2, MPI_TEST_BLOCK_SIZE *	\
		MPI_TEST_CONC_WRITES,fh2);
	  for(j=0; j < MPI_TEST_CONC_WRITES; j++){
	      for(k = 0 ; k < MPI_TEST_BLOCK_SIZE; k++){
		  real_val1 = read_data[j][k][0];
		  real_val2 = read_data[j][k][1];
		  exp_val1  = k + j*MPI_TEST_BLOCK_SIZE + i*MPI_TEST_BLOCK_SIZE * \
		      MPI_TEST_CONC_WRITES;
		  exp_val2  = -exp_val1;
		  
		  if((real_val1 != exp_val1) || (real_val2 != exp_val2)){
		      fprintf(stderr,"\nERROR: Read wrong value (");
		      fprintf(stderr,"%ld:%ld",real_val1,real_val2);
		      fprintf(stderr,") --> should be ");
		      fprintf(stderr,"%ld:%ld", exp_val1, exp_val2);
		      fprintf(stderr,"!\n");
		      MPI_Abort(intracomm, -1);
		  }
	      } /* for (k) */
	  } /* for(j) */
#ifdef DEBUG
	      printf("\nProcess %i wrote correct values.",i);
#endif
    } /* for(i) */
    fclose(fh2); 
  } /* if(mypid == 0) */

  MPI_Barrier(intracomm);

  /* End of test */

  MPI_Comm_free(&intracomm);
  MPI_Finalize();
  return 0;
}

