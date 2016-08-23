/*
 MPI parallel Pi estimation using slowly coverged
 Libniz series Pi = 4 * (1 - 1/3 + 1/5 - 1/7 ...)
 
 MPI implementation

 NOTE! For demonstration only - for higher accuracy 
 it may loop forever!

                            by Rumen G.Bogdanovski
*/
#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>

#define MASTER 0
#define ACC 1e-9

/* parallel function to make partial estimates of PI */
double pi_estim(int proc,int nprocs,double acc) {
	unsigned long long n;
	int mul;
	double estim=0,nth;
	acc=acc/4;
	n=proc;
	do {
		if (n%2==0) { mul=-1; } 
		else { mul=1; }
		nth = (double)mul/(2*n-1);
		estim+=nth;
		n+=nprocs;
	} while (fabs(nth) > acc);
	
	estim*= 4;
	return estim;
}

int main(int argc,char *argv[]) {	
	int proc, nprocs;
	double estim=0, pi=0;
	struct timeval t1,t2;
	double elapsed_time;
	double acc=ACC;    /* Accuracy treshold */
	
 	MPI_Init(&argc, &argv);
   	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
   	MPI_Comm_rank(MPI_COMM_WORLD,&proc);
	
	if (proc==MASTER) {
	    printf("MPI procs to spawn: %d\n", nprocs);
	    printf("Estimate accuracy: %.20f (%.3e) with %d bit arythmetic\n", 
		acc, acc, sizeof(pi)*8);
	    gettimeofday(&t1,NULL);
	}

	estim=pi_estim(proc+1,nprocs,acc);
	
	/* collect the partial estimates and sum them */
	MPI_Reduce(&estim,&pi,1,MPI_DOUBLE,MPI_SUM,MASTER,MPI_COMM_WORLD);
                    
	if (proc==MASTER) {
		gettimeofday(&t2,NULL);
		elapsed_time = (t2.tv_sec - t1.tv_sec);
		elapsed_time += (t2.tv_usec - t1.tv_usec) / 1e6;
		printf("Calculated: Pi = %.20f\n",pi);
		printf("Elapsed time: %.03f s\n", elapsed_time);
	}
	MPI_Finalize ();
}
