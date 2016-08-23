!**********************************************************************
!   pi3f90.f - compute pi by integrating f(x) = 4/(1 + x**2)     
!     
!   Each node: 
!    1) receives the number of rectangles used in the approximation.
!    2) calculates the areas of it's rectangles.
!    3) Synchronizes for a global summation.
!   Node 0 prints the result.
!
!  Variables:
!
!    pi  the calculated result
!    n   number of points of integration.  
!    x           midpoint of each rectangle's interval
!    f           function to integrate
!    sum,pi      area of rectangles
!    tmp         temporary scratch space for global summation
!    i           do loop index
!****************************************************************************
program main
!!!use mpi
implicit none
include 'mpif.h'

 double precision  PI25DT
 parameter        (PI25DT = 3.141592653589793238462643d0)

 double precision  mypi, pi, h, sum, x, f, a, tick1, tick2
 integer n, myid, numprocs, i, rc, ierr
 parameter(n=700000000)
!                                 function to integrate
 f(a) = 4.d0 / (1.d0 + a*a)
 
 call MPI_INIT( ierr )
 call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
 call MPI_COMM_SIZE( MPI_COMM_WORLD, numprocs, ierr )

!!! do while (.TRUE.)
!!!    if(myid == 0) write(*,*) 'loop'

 call MPI_BARRIER(MPI_COMM_WORLD, ierr)
 tick1 = MPI_Wtime()
 
    h = 1.0d0/n
 
    sum  = 0.0d0
    do i = myid+1, n, numprocs
       x = h * (dble(i) - 0.5d0)
       sum = sum + f(x)
    enddo
    mypi = h * sum

!                                 collect all the partial sums
    call MPI_REDUCE(mypi,pi,1,MPI_DOUBLE_PRECISION,MPI_SUM,0, &
                    MPI_COMM_WORLD,ierr)
 call MPI_BARRIER(MPI_COMM_WORLD, ierr)
 tick2 =  MPI_Wtime()
!                                 node 0 prints the answer.
    if (myid .eq. 0) then
        write(6, 97) pi, abs(pi - PI25DT), (tick2-tick1), numprocs
 97     format('  pi is approximately: ', F18.16, &
               '  Error is: ', F18.16, ' wall clock time=',F10.3,'secs on ', I3,'PEs')
    endif

!!! end do

 call MPI_FINALIZE(rc)
 stop
end




