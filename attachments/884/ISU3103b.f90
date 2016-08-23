!  derived from MPICH2 test f90/cray_spr/get_elements_count2.f90
   program get_elements_count2
! 
!  Purpose:  check Get_count and Get_elements for derived type
!  Author :  Jim Hoekstra - ISU
!  Date   ;  6/17/2009
!
   implicit none
   include 'mpif.h'
   integer, parameter :: verbose=0
   integer, parameter :: cmax=100,dmax=100,imax=60
   integer, parameter :: nb=2
   integer :: comm,rank,size,dest,ierror,errs=0
   integer :: status(MPI_STATUS_SIZE)
   integer :: i,ii,count,ka,j,jj,k,kj,krat,tag=100
   integer :: blklen(nb)=(/2,2/)
   integer :: types(nb)=(/MPI_DOUBLE_PRECISION,MPI_INTEGER/)
   integer(kind=MPI_ADDRESS_KIND) :: disp(nb)
   integer :: newtype,ntlen,ians(20),ians1(3)
   double precision :: dbuff(dmax), a
   integer :: ibuff(imax)
   character :: cbuff(cmax)='X'

   call MPI_Init(ierror)
   kj=kind(j) ! assumes that this usually maps to 4 or 8 (default64)
!  call MPI_Sizeof (j, kj, ierror)
   ka=kind(a) ! assumes that this usually maps to 8 
!  call MPI_Sizeof (a, ka, ierror)
   ntlen=2*ka+2*kj
   krat=ntlen/kj
   disp=(/0,2*ka/)

!  calculate answers for expected i values for Get_elements with derived type
   ians1(1)=ka
   ians1(2)=2*ka
   ians1(3)=2*ka+kj
   if (verbose > 0) print *, (ians1(k),k=1,3)
   jj=0
   do j=1,20,4
     ians(j)=jj+ka/kj
     ians(j+1)=jj+2*(ka/kj)
     ians(j+2)=jj+2*(ka/kj)+1
     ians(j+3)=jj+2*(ka/kj)+2
     if (verbose > 0) print *, (ians(k),k=j,j+3)
     jj=jj+ntlen/kj
   enddo 

   comm=MPI_COMM_WORLD
   call MPI_Comm_size(comm, size, ierror)
   dest=size-1
   call MPI_Comm_rank(comm, rank, ierror)

   if (verbose > 0) print *, MPI_UNDEFINED

   call MPI_Type_create_struct(nb, blklen, disp, types, newtype, ierror)
   call MPI_Type_commit(newtype, ierror)

   ii=1  ! counts elements for 1st receive
   jj=1  ! counts elements for 2nd receive
   do i=1,20
    if (rank == 0) then
      call MPI_Send(cbuff, i, MPI_BYTE, dest, 100, comm, ierror)
      call MPI_Send(ibuff, i, MPI_INTEGER, dest, 100, comm, ierror)

    else if (rank == dest) then

!     first receive
      call MPI_Recv(dbuff, dmax, newtype, 0, 100, comm, status, ierror)
!       check on MPI_Get_elements
      call MPI_Get_elements(status, newtype, count, ierror)
        if (verbose > 0) print *, i,' R1 Get_elements', count
        if (i .ne. ians1(ii)) then
          if (count .ne. MPI_UNDEFINED) then
           errs=errs+1
           write (*,fmt="(' R1 Get_elements  i=',i2,' count=',i8,&
                         &'  but should be MPI_UNDEFINED')") i,count
          endif
        else
          if (count .ne. ii) then
           errs=errs+1
           write (*,fmt="(' R1 Get_elements  i=',i2,' count=',i8,&
                         &'  but should be ',i2)") i,count,ii
          endif
          ii=ii+1
        endif

!     second receive
      call MPI_Recv(dbuff, dmax, newtype, 0, 100, comm, status, ierror)
!       check on MPI_Get_elements
      call MPI_Get_elements(status, newtype, count, ierror)
        if (verbose > 0) print *, i,' R2 Get_elements', count
        if (i .ne. ians(jj)) then
          if (count .ne. MPI_UNDEFINED) then
           errs=errs+1
           write (*,fmt="(' R2 Get_elements  i=',i2,' count=',i8,&
                         &'  but should be MPI_UNDEFINED')") i,count
          endif
        else
          if (count .ne. jj) then
           errs=errs+1
           write (*,fmt="(' R2 Get_elements  i=',i2,' count=',i8,&
                         &'  but should be ',i2)") i,count,jj
          endif
          jj=jj+1
        endif

    else
!     other ranks do not participate
    endif
   enddo

   if (rank .eq. dest) print *, 'errs=',errs
   call MPI_Finalize(ierror) 
   end
