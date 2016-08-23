#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define NREP  10

float deltat(struct timeval tv1, struct timeval tv2);

int main (int argc, char *argv[])
{

  int ierr, myid, nproc, len; 
  
  char filename[10];
  char pname[1000];

  struct timeval t1, t2; 
  float zeit1[100], zeit2[100];

  int nph, devmask, n, i, knownhost;
  int numhosts = 0;
  int nb_top, nb_bot, dest;

  float *sendbuf_t, *sendbuf_b, *recbuf_t, *recbuf_b;
  float FAC;

  FILE *fp;

  MPI_Request request;

  //set the size of the messages
  int nx, ny, nz;
  nx = 400;
  ny = 400;
  nz = 5;

  if (argc > 3){
	nx = atoi(argv[1]);
	ny = atoi(argv[2]);
	nz = atoi(argv[3]);
  }
  
  n = nx*ny*nz;  

  FAC = 1000.0f  * n * sizeof(float) / ( 1 << 20 ) ;

  sendbuf_t = (float*) malloc(n*sizeof(float));
  recbuf_t = (float*) malloc(n*sizeof(float));
  sendbuf_b = (float*) malloc(n*sizeof(float));
  recbuf_b = (float*) malloc(n*sizeof(float));

  srand48(1);
  for (i=0; i<n; i++) {
    sendbuf_t[i] = (float) drand48();
    sendbuf_b[i] = (float) drand48();
  }

  ierr = MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  
  if (myid == 0) printf("datasize = 400x400x%d points  (%10.3f MB)\n",
	nz, (float) n*sizeof(float) / (1 << 20 ) );

  ierr = MPI_Get_processor_name(pname, &len);
  printf("process %d (total: %d) running on %s \n",
      myid, nproc,pname);
 
  nb_top = ( myid > 0 ? myid-1 : MPI_PROC_NULL );
  nb_bot = ( myid < nproc-1 ? myid+1 : MPI_PROC_NULL );

  ierr = MPI_Barrier(MPI_COMM_WORLD);

  if (myid == 0) {
    //process 0 sends out the message and receives the message from each of the other processes
    for (dest = 1; dest < nproc; dest++) { 
      gettimeofday(&t1, NULL);
      for (i=0; i<NREP; i++) 
	ierr = MPI_Send(sendbuf_t, n, MPI_FLOAT, dest,
	    10000+dest+i*100, MPI_COMM_WORLD);
      gettimeofday(&t2, NULL);
      zeit1[dest] = deltat(t1, t2)/NREP;

      gettimeofday(&t1, NULL);
      for (i=0; i<NREP; i++) 
	ierr = MPI_Recv(recbuf_t, n, MPI_FLOAT, dest,
	    20000+dest+i*100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      gettimeofday(&t2, NULL);
      zeit2[dest] = deltat(t1, t2)/NREP;
    }
    printf("\n\nsimple blocking Send/Receive test\n");
    printf("this blocks until the transfer is complete\n");
    printf("    MPI process:");
    for (dest = 1; dest < nproc; dest++ ) printf("%8d",dest);
    printf("\n");
    printf("SEND time in ms ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",zeit1[dest]);
    printf("\n");
    printf("     bw in MB/s ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",FAC/zeit1[dest]);
    printf("\n");

    printf("RECV time in ms ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",zeit2[dest]);
    printf("\n");
    printf("     bw in MB/s ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",FAC/zeit2[dest]);
    printf("\n");
    

  } else {
    for (i=0; i<NREP; i++) 
	ierr = MPI_Recv(recbuf_t, n, MPI_FLOAT, 0,
	    10000+myid+i*100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (i=0; i<NREP; i++) 
	ierr = MPI_Send(sendbuf_t, n, MPI_FLOAT, 0,
	    20000+myid+i*100, MPI_COMM_WORLD);
  }


//////////////////////////////////////////////////////////////////////
  ierr = MPI_Barrier(MPI_COMM_WORLD);

  if (myid == 0) {
    for (dest = 1; dest < nproc; dest++) {
      gettimeofday(&t1, NULL);
      for (i=0; i<1; i++)
        ierr = MPI_Isend(sendbuf_t, n, MPI_FLOAT, dest,
            30000+dest+i*100, MPI_COMM_WORLD, &request);
      gettimeofday(&t2, NULL);
      zeit1[dest] = deltat(t1, t2);
      MPI_Wait(&request, MPI_STATUS_IGNORE);
      gettimeofday(&t2, NULL);
      zeit2[dest] = deltat(t1, t2);
    }
    
    printf("\n\nnon-blocking Send/Receive test\n");
    printf("this call is asynchronous, i.e. returns immediately\n");
    printf("COMPLETE time is the time until transfer has actually finished\n");
    printf("    MPI process:");
    for (dest = 1; dest < nproc; dest++ ) printf("%6d ",dest);
    printf("\n");
    printf("SEND time in ms ");
    for (dest = 1; dest < nproc; dest++ ) {
      printf("%8.1f",zeit1[dest]);
      if (zeit1[dest] < 0.1) zeit1[dest] = -FAC;
    }
    printf("\n");
    printf("     bw in MB/s ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",FAC/zeit1[dest]);
    printf("\n");
    printf("COMPLETE time   ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",zeit2[dest]);
    printf("\n");
    printf("     bw in MB/s ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",FAC/zeit2[dest]);
    printf("\n");


    for (dest = 1; dest < nproc; dest++) {

      gettimeofday(&t1, NULL);
      for (i=0; i<1; i++)
        ierr = MPI_Irecv(recbuf_t, n, MPI_FLOAT, dest,
            40000+dest+i*100, MPI_COMM_WORLD, &request);
      gettimeofday(&t2, NULL);
      zeit1[dest] = deltat(t1, t2);
      MPI_Wait(&request, MPI_STATUS_IGNORE);
      gettimeofday(&t2, NULL);
      zeit2[dest] = deltat(t1, t2);
    }
    printf("\n");

    printf("RECV time in ms ");
    for (dest = 1; dest < nproc; dest++ ) {
      printf("%8.1f",zeit1[dest]);
      if (zeit1[dest] < 0.05) zeit1[dest] = -FAC;
    }
    printf("\n");
    printf("     bw in MB/s ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",FAC/zeit1[dest]);
    printf("\n");
    printf("COMPLETE time   ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",zeit2[dest]);
    printf("\n");
    printf("     bw in MB/s ");
    for (dest = 1; dest < nproc; dest++ ) printf("%8.1f",FAC/zeit2[dest]);
    printf("\n");


  } else {
    for (i=0; i<NREP; i++)
        ierr = MPI_Irecv(recbuf_t, n, MPI_FLOAT, 0,
            30000+myid+i*100, MPI_COMM_WORLD, &request);
    for (i=0; i<NREP; i++)
        ierr = MPI_Isend(sendbuf_t, n, MPI_FLOAT, 0,
            40000+myid+i*100, MPI_COMM_WORLD, &request);
  }




  ierr = MPI_Barrier(MPI_COMM_WORLD);

  if (myid == 0) {
    printf("\n\nexchanging data using MPI_Sendrecv (two separate SendRecv() calls)\n");
    printf("time in miliseconds, bandwidth in MB/s\n");
    printf("  up   is the transfer towards lower process ids\n");
    printf("  down is the other direction.\n");
  }
  gettimeofday(&t1, NULL);
  for (i = 0; i< NREP; i++ ) {

    ierr = MPI_Sendrecv(sendbuf_t, n, MPI_FLOAT, nb_top, 1,
			recbuf_b,  n, MPI_FLOAT, nb_bot, 1,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  gettimeofday(&t2, NULL);
  if ( ierr ) printf("WARNING: error during MPI call: %d\n",ierr);
  zeit1[myid] = deltat(t1, t2)/NREP;

  gettimeofday(&t1, NULL);

  for (i = 0; i< NREP; i++ ) {
    ierr = MPI_Sendrecv(sendbuf_b, n, MPI_FLOAT, nb_bot, 2,
			recbuf_t,  n, MPI_FLOAT, nb_top, 2,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  gettimeofday(&t2, NULL);
  if ( ierr ) printf("WARNING: error during MPI call: %d\n",ierr);
  zeit2[myid] = deltat(t1, t2)/NREP;
  
  // gather results
  
  ierr = MPI_Gather(zeit1+myid, 1, MPI_FLOAT, zeit1, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  ierr = MPI_Gather(zeit2+myid, 1, MPI_FLOAT, zeit2, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (myid == 0) {
  printf("%7s%20s%20s%20s\n"," ",
      "    |------ up ------|","  |------ down ------| "," |----- total -----|");
  printf("%7s%8s%13s%8s%13s%8s%13s\n","MPI id ","time",
      "bw   ","time","bw   ","time","bw  ");
  for (dest = 0; dest < nproc; dest++ ) 
	printf("%4d   %8.1f ( %8.1f )%8.1f ( %8.1f )%8.1f ( %8.1f )\n",
	       dest, zeit1[dest], (nproc-1)*FAC/zeit1[dest], zeit2[dest], (nproc-1)*FAC/zeit2[dest],
	       zeit1[dest]+zeit2[dest], 2.0*(nproc-1)* FAC/(zeit1[dest]+zeit2[dest]));
  }



  int maxsize = n;
  int nsizes, isize;
  int sizes[10000];
  nsizes = 1; 
  sizes[0] = n;
  while (maxsize > 2000 ) {
    maxsize = maxsize >> 1;
    sizes[nsizes++]=1.5*maxsize;
    sizes[nsizes++]=maxsize;
  }
  maxsize = 1050;
  while (maxsize > 50 ) {
    maxsize -= 50;
    sizes[nsizes++]=maxsize;
  }

  if (myid == 0) {
    fp = fopen("MPI_bandwidth.dat","w");
    printf("\n\n#combined Sendrecv\n");
    printf("#%10s %9s %11s\n","size [kB]","time [ms]"," bw [MB/s]");  
    if (fp!=NULL) {
      fprintf(fp,"\n\n#combined Sendrecv\n");
      fprintf(fp,"#%10s %9s %11s\n","size [kB]","time [ms]"," bw [MB/s]");  
    }
  }
  
  for (isize = 0; isize<nsizes; isize++) {  
    float fs = 4.0f*sizes[isize]/1024.0f;


    gettimeofday(&t1, NULL);

    for (i = 0; i< NREP; i++ ) {
      MPI_Barrier(MPI_COMM_WORLD);
      ierr = MPI_Sendrecv(sendbuf_t, sizes[isize], MPI_FLOAT, nb_top, 2,
			  recbuf_b,  sizes[isize], MPI_FLOAT, nb_bot, 2,
			  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      ierr = MPI_Sendrecv(sendbuf_b, sizes[isize], MPI_FLOAT, nb_bot, 4,
			  recbuf_t, sizes[isize], MPI_FLOAT, nb_top, 4,
			  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Barrier(MPI_COMM_WORLD);
    }
    gettimeofday(&t2, NULL);
    if ( ierr ) printf("WARNING: error during MPI call: %d\n",ierr);
    zeit2[myid] = deltat(t1, t2)/NREP;
    ierr = MPI_Gather(zeit2+myid, 1, MPI_FLOAT, zeit2, 
		      1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (myid == 0) {
      printf("%10.3f ", fs );
      printf("%9.3f %8.1f \n",  
	  zeit2[myid], 2.0*(nproc-1)* fs /zeit2[myid]);
      if (fp!=NULL) {
	fprintf(fp,"%10.3f ", fs );
	fprintf(fp,"%9.3f %8.1f \n",  
	  zeit2[myid], 2.0*(nproc-1)* fs /zeit2[myid]);
      }
    }
    fflush(stdout);
  }



/***************************************************************************/



  ierr = MPI_Barrier(MPI_COMM_WORLD);

  
  if (myid == 0) {
    printf("\n\n#bandwidth & latency test:\n");
    printf("#(simple send to specified host, varying data size)\n");
    printf("#%10s "," ");
    for (dest = 1; dest < nproc; dest++) 
	printf("  |---- %4d     ----|",dest);
    printf("\n#%10s "," size [kB]");
    for (dest = 1; dest < nproc; dest++) 
	printf("%11s  %9s","time [ms]","bw [MB/s]");
    printf("\n");
    if (fp!=NULL) {
      fprintf(fp,"\n\n#bandwidth & latency test:\n");
      fprintf(fp,"#(simple send to specified host, varying data size)\n");
      fprintf(fp,"#%10s "," ");
      for (dest = 1; dest < nproc; dest++) 
	  fprintf(fp,"  |---- %4d     ----|",dest);
      fprintf(fp,"\n#%10s "," size [kB]");
      for (dest = 1; dest < nproc; dest++) 
	fprintf(fp,"%11s  %9s","time [ms]","bw [MB/s]");
      fprintf(fp,"\n");
    }
    
    for (isize = 0; isize<nsizes; isize++) {  
      float fs = 4.0f*sizes[isize]/1024.0f;
      printf("%10.3f ", fs); 
      if (fp!=NULL) fprintf(fp,"%10.3f ", fs); 
      for (dest = 1; dest < nproc; dest++) { 
	gettimeofday(&t1, NULL);
	for (i=0; i<NREP; i++) {
	  ierr = MPI_Send(sendbuf_t, sizes[isize], MPI_FLOAT, dest,
	     70000+dest+i*100, MPI_COMM_WORLD);
	  }
	gettimeofday(&t2, NULL);
	zeit1[dest] = deltat(t1, t2)/NREP;
	printf("%9.3f  %9.1f ",zeit1[dest], fs/zeit1[dest]);
	if (fp !=NULL) fprintf(fp,"%9.3f  %9.1f ",zeit1[dest], fs/zeit1[dest]);
      }
      printf("\n");
      if (fp!=NULL) fprintf(fp,"\n");
    }
    

  } else {
    for (isize = 0; isize<nsizes; isize++) {  
      for (i=0; i<NREP; i++) 
	ierr = MPI_Recv(recbuf_t, sizes[isize], MPI_FLOAT, 0,
	    70000+myid+i*100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }

  if (myid == 0) {
    if (fp!=NULL) fclose(fp);
  }



  free(sendbuf_t);
  free(recbuf_t);
  free(sendbuf_b);
  free(recbuf_b);


 
  MPI_Finalize();
  return (0);


}




////////////////////////////////////////////////////////////////////////////////
//
// calc time difference for two timevals
// returns delta in ms
//
float deltat(struct timeval tv1, struct timeval tv2)
{
  float zeit = (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec)*1e-6;
  zeit *= 1.0e3;

  return (zeit);

}

