      PARAMETER( M = 1000 )
      DIMENSION A_LOC(M),A(M)
C
      DIMENSION IN(2)
      DATA IN /3,7/
C
      INCLUDE 'mpif.h'
C
      CALL MPI_INIT(IER)
      CALL MPI_COMM_RANK( MPI_COMM_WORLD, MYID, IER )
      CALL MPI_COMM_SIZE( MPI_COMM_WORLD, NUMPROCS, IER )
C
C     ZERO CLEAR
      DO I=1,M
        A(I)=0.0
        A_LOC(I)=0.0
      END DO
C
C     JUST MAKEUP DATA
      DO I=1,M
        IF(MOD(I,IN(MYID+1)).EQ.0) A_LOC(I)=TRANSFER(1,0.0)
      END DO
C
C     THIS IS THE BODY OF TESTING.
      CALL MPI_ALLREDUCE(A_LOC,A,M,MPI_REAL,MPI_MAX,
     +                   MPI_COMM_WORLD,IER)
C
      CALL MPI_FINALIZE(IER)
C
C     CHECK TEST RESULT
      NUM=0
      DO I=1,M
        IF(TRANSFER(A(I),0).NE.0) NUM=NUM+1
      END DO
C
C     OUTPUT
      WRITE(*,*) (A(K),K=1,25)
      WRITE(*,*) MYID,NUM
C
      END
