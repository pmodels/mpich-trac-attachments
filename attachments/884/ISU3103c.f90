!     ISU3103c.f90
      use mpi 
      IMPLICIT NONE 
      INTEGER    RANK, SIZE, IERR, COMM, errs 
      INTEGER    MAX, I, K, dest
      INTEGER   STATUS(MPI_STATUS_SIZE)

      integer, parameter :: nb=2
      integer :: blklen(nb)=(/1,1/)
      integer :: types(nb)=(/MPI_DOUBLE_PRECISION,MPI_CHAR/)
      integer(kind=MPI_ADDRESS_KIND) :: disp(nb)=(/0,8/)

      integer, parameter :: amax=200
      integer :: type1, type2, extent
      real    :: a(amax)

      errs = 0 
      call mpi_init( ierr ) 
      COMM = MPI_COMM_WORLD 
      CALL MPI_COMM_RANK(COMM,RANK,IERR) 
      CALL MPI_COMM_SIZE(COMM,SIZE,IERR) 
      dest=size-1
 
      if (rank == 0) print *, "MPI_UNDEFINED =", MPI_UNDEFINED

      call MPI_Type_create_struct(nb, blklen, disp, types, type1, ierr)
      call MPI_Type_commit(type1, ierr)
      call MPI_Type_extent(type1, extent, ierr)
      if (rank == 0) print *, "Datatype type1 extent=", extent

      CALL MPI_TYPE_CONTIGUOUS(4, Type1, Type2, ierr) 
      CALL MPI_TYPE_COMMIT(Type2, ierr) 
      call MPI_Type_extent(Type2, extent, ierr)
      if (rank == 0) print *, "Datatype Type2 extent=", extent

     do k=1,17

      IF(rank.EQ.0) THEN 

!       send k copies of datatype Type1
        CALL MPI_SEND(a, k, Type1, dest, 0, comm, ierr) 

      ELSE if (rank == dest) then

        CALL MPI_RECV(a, 200, Type2, 0, 0, comm, status, ierr) 
        CALL MPI_GET_COUNT(status, Type2, i, ierr)
        if (i == MPI_UNDEFINED) then
          print *, "k=",k,"  MPI_Get_count returns MPI_UNDEFINED"
        else
          print *, "k=",k,"  MPI_Get_count returns", i
        endif
        CALL MPI_GET_ELEMENTS(status, Type2, i, ierr)
        print *, "k=",k,"  MPI_Get_elements returns", i

      ELSE
!       thix rank does not particupate
      END IF 
     enddo

      call mpi_finalize( ierr )

      END
