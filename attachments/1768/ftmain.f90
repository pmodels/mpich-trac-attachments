



program ftmain
!-----------------------------------------------------------------------
!                                                        since '94/07/31
!                                                       update '09/09/14
!
!     Electronic structure calculation 
!       with the plane-wave        method for solving Khon-Sham equation,
!        and the finite difference method for solving Poisson equation.
!
!     External potential is calculated by pseudopotentials.
!
!         make pwp      for serial calculation
!         make pwpmpi   for MPI
!-----------------------------------------------------------------------
implicit none
include 'mpif.h'

integer :: world            ! global, default communicator
integer :: nodes            ! the number of nodes in communicator world
integer :: myid             ! node ID in communicator world
integer :: world_qm         ! communicator

integer :: ierr


!-----Start parallel environment and keep my node ID
call MPI_INIT(ierr)
world = MPI_COMM_WORLD

call MPI_COMM_RANK( world, myid, ierr )

call MPI_COMM_SIZE( world, nodes, ierr )


if( myid == 0 ) then
    write(*,*) 'Starting--nodes = ', nodes
end if


call get_newcomm( myid, nodes, world, world_qm )


!------internode syncronization
call MPI_BARRIER(world,ierr)

!-----release communicator
call MPI_COMM_FREE( world_qm, ierr )

!-----Finalize the parallel environment
call MPI_FINALIZE(ierr)


stop
end program




subroutine get_newcomm( myid, nodes, world, world_qm )
!-----------------------------------------------------------------------
!     define and create new communicators
!-----------------------------------------------------------------------
implicit none
include 'mpif.h'
integer :: myid, nodes, world, world_qm

!-----declare local variables
integer :: group            ! group of communicator world
!integer, allocatable, dimension(:) :: extract_ids  ! node IDs to be extracted
integer, allocatable, dimension(:) :: include_ids  ! node IDs to be included
integer :: iworld           ! temporal communicator
integer :: igroup           ! temporal group
!integer :: nextract
integer :: i, nmd, ii
integer :: status
integer :: ierr



!-----extract group from communicator
call MPI_COMM_GROUP( world, group, ierr )


!-----allocate memory for include_ids
allocate( include_ids(0:nodes-1), stat=status )


!-----create new communicator for QM nodes
!nextract = nodes - 1

    do nmd = 0, nodes - 1

       call MPI_BARRIER(world,ierr)
       if( myid == 0 ) write(*,*) 'check_newcomm do-start', nmd

       ii = - 1
       do i = nmd, nmd
          ii = ii + 1
          include_ids(ii) = i
       end do

       !-----create new group by extracting some nodes
!       call MPI_GROUP_EXCL( group, nextract, extract_ids(0), igroup, ierr )
       call MPI_GROUP_INCL( group, 1, include_ids(0), igroup, ierr )
       if( ierr /= MPI_SUCCESS ) then
           write(*,*) 'group_incl: ierr =',ierr
           stop
       end if

       !-----create new communicator from new group
       call MPI_COMM_CREATE( world, igroup, iworld, ierr )
       if( ierr /= MPI_SUCCESS ) then
           write(*,*) 'comm_create: ierr =',ierr
           stop
       end if

       !-----release group
       call MPI_GROUP_FREE( igroup, ierr )

       if( myid >= nmd .and. myid <= nmd ) then
           world_qm = iworld
       end if

       call MPI_BARRIER(world,ierr)
       if( myid == 0 ) write(*,*) 'check_newcomm do-end'

    end do

!-----release group
call MPI_GROUP_FREE( group, ierr )

!-----deallocate memory for include_ids
deallocate( include_ids, stat=status )


return
end subroutine
