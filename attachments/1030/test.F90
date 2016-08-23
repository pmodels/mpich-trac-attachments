!  MPI datatype problem with MPICH2
!
!  works correctly with other MPIs,
!  tested with MPICH, MVAPICH, Open MPI, SGI MPT, Intel MPI, IBM PE
!
!  correct output of the array is:
!  -1.0   1.0   2.0   5.0
!  -1.0   3.0   4.0   6.0
!
!  output from MPICH2-1.2.1p1:
!  -1.0   1.0   2.0   5.0
!   6.0   3.0   4.0  -1.0
!
program test

  implicit none
  
  include 'mpif.h'
  
  ! define size of an MPI_REAL (in bytes)
  integer, parameter :: MPI_REAL_size = 4

  integer :: rank, nproc
  integer :: datatype, err, status(MPI_STATUS_SIZE)

  real, pointer :: array(:,:), buf(:)
  integer :: aext(2), astart, aend, dt_size, dt_ext, i, j, size_exp
  integer, parameter :: cnt = 2
  integer :: blens(cnt), types(cnt), displ(cnt)
  
  call mpi_init(err)
  call mpi_comm_size(mpi_comm_world,nproc,err)
  call mpi_comm_rank(mpi_comm_world,rank,err)
  
  if(nproc/=2) then
    print *,'run with 2 procs, nproc = ',nproc
    stop
  end if
  
  ! allocate and initialize the array
  
  aext = (/4,2/)
  allocate(array(0:aext(1)-1,0:aext(2)-1)) 
  array = -1.0
  

  ! create type to receive 2 non-contiguous sections of the array
  
  size_exp = 0
  
  ! 1st section selects elements 1 and 2 out of 1st dimension, complete 2nd dim.
  ! should receive the values 1, 2, 3, 4 from rank 1
  astart = 1
  aend   = 2
  call build_array_section_type(aext(1),astart,aend,types(1))
  blens(1) = aext(2)
  displ(1) = 0
  size_exp = size_exp + aext(2) * (aend-astart+1) * MPI_REAL_size
  
  ! 2nd section selects last element of 1st dimension, complete 2nd dim.
  ! should receive the values 5, 6 from rank 1
  ! 
  ! note: the bug only happens if the *LAST* elements of 1st dim. are in the type
  ! increaseing the size of the 1st dim. makes the problem disappear
  astart = 3
  aend   = 3
  call build_array_section_type(aext(1),astart,aend,types(2))
  blens(2) = aext(2)
  displ(2) = 0
  size_exp = size_exp + aext(2) * (aend-astart+1) * MPI_REAL_size

  ! create type
  call MPI_Type_struct(cnt, blens, displ, types, datatype, err)
  call MPI_Type_commit(datatype, err)
  call MPI_Type_size(datatype, dt_size, err)
  call MPI_Type_extent(datatype, dt_ext, err)

  if(rank==0) then
    ! rank 0 receives into the array
    
    print *,'datatype size & extent (bytes): ',dt_size,dt_ext
    print *,'                      expected: ',size_exp,PRODUCT(aext)*MPI_REAL_size
    
    call MPI_Recv(array(0,0), 1, datatype, 1, 42, MPI_COMM_WORLD, status, err)
    
    call MPI_Get_count(status, MPI_REAL, dt_size, err)
    print *,'                      received: ',dt_size * MPI_REAL_size
    
    ! print received array
    do j=0,aext(2)-1
      do i=0,aext(1)-1
        write(*,'(F6.1)',advance='no') array(i,j)
      end do
      write(*,*)
    end do

  elseif(rank==1) then
    ! rank 1 sends just a bunch of reals
    
    allocate(buf(dt_size/MPI_REAL_size))
    do i=1,size(buf)
      buf(i)=REAL(i)
    end do

    call MPI_Send(buf, dt_size/MPI_REAL_size, MPI_REAL, 0, 42, MPI_COMM_WORLD, err)  

    deallocate(buf)

  end if  


  call MPI_Type_free(datatype, err)
  do i=1,cnt
    call MPI_Type_free(types(i), err)
  end do
  
  call mpi_finalize(err)

  deallocate(array)


contains


  !! create a datatype for a 1D real array subsection
  !!
  !! - a subsection of the first dimension is defined via astart, aend
  !! - indexes are assumed to start with 0, that means:
  !!   - 0 <= astart <= aend < aext
  !! - astart and aend are inclusive
  !! 
  !! example:
  !!
  !! aext = 8, astart=2, aend=4 would produce:
  !!
  !! index     | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
  !! 1D array   ###############################
  !! datatype   LB      ###########             UB
    
  subroutine build_array_section_type(aext,astart,aend,datatype)
    implicit none
    integer, intent(in)  :: aext         !! size of array
    integer, intent(in)  :: astart       !! start of array section
    integer, intent(in)  :: aend         !! end of array section
    integer, intent(out) :: datatype     !! resulting mpi data type

    integer, parameter :: count=3
    integer :: displ(count)
    integer :: types(count), blens(count), err

    ! create type for subsection in 1st dimension

    ! lower bound marker
    types(1) = MPI_LB
    displ(1) = 0
    blens(1) = 1
    
    ! subsection starting at abnd(1)
    displ(2) = astart * MPI_REAL_size
    types(2) = MPI_REAL
    blens(2) = aend - astart + 1

    ! upper bound marker
    types(3) = MPI_UB
    displ(3) = aext * MPI_REAL_size
    blens(3) = 1

    ! debug output
    write(1000+rank,*) 'types ',types(1:count)
    write(1000+rank,*) 'displ ',displ(1:count)
    write(1000+rank,*) 'blens ',blens(1:count)
    
    call MPI_Type_struct(count, blens, displ, types, datatype, err)
    
    if(err/=MPI_SUCCESS) stop 'MPI_Type_create_struct'
        
  end subroutine build_array_section_type



end program test
