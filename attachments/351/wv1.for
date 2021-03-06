C ****************************************************************************
C  FILE: mpi_wave.f
C  OTHER FILES: mpi_wave.h, draw_wave.c
C  DESCRIPTION:
C    MPI Concurrent Wave Equation  - Fortran Example
C    This program implements the concurrent wave equation described 
C    in Chapter 5 of Fox et al., 1988, Solving Problems on Concurrent
C    Processors, vol 1.  
C    A vibrating string is decomposed into points.  Each processor is 
C    responsible for updating the amplitude of a number of points over
C    time. At each iteration, each processor exchanges boundary points with
C    nearest neighbors.  This version uses low level sends and receives
C    to exchange boundary points.
C AUTHOR: Blaise Barney. Adapted from Ros Leibensperger, Cornell Theory
C   Center. Converted to MPI: George L. Gusciora, MHPCC (1/95)  
C LAST REVISED: 12/20/2001 Blaise Barney
C ****************************************************************************
C  Explanation of constants and variables used in common blocks and
C  include files
C    MASTER           = task ID of master
C    E_OUT1, E_OUT2   = message types
C    taskid           = task ID
C    numtasks         = number of tasks
C    tpoints          = total points along wave
C    nsteps           = number of time steps
C    npoints          = number of points handled by this task
C    first            = index of first point handled by this task
C    values(0:1001)   = values at time t
C    oldval(0:1001)   = values at time (t-dt)
C    newval(0:1001)   = values at time (t+dt)

      program wave_send
c      implicit none
      include 'mpi_wave.h'
      include 'mpif.h'

C     Routine for creating the X graph of the wave
cexternal draw_wave
      integer taskid, numtasks
      integer status(3), ierr
	double precision endt,startt
      common/config/taskid, numtasks,startt,endt
      integer left, right, nbuf(4)

C     Determine number of tasks and taskid
      call MPI_INIT( ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, taskid, ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, numtasks, ierr )

C     Determine left and right neighbors
      if (taskid .eq. numtasks-1) then
         right = 0
      else 
         right = taskid + 1
      end if

      if (taskid .eq. 0) then
         left = numtasks - 1
      else 
         left = taskid - 1
      end if

C     Get program parameters and initialize wave values
      if (taskid .eq. MASTER) then
         startt=mpi_wtime()
	   call init_master
      
	else
         call init_workers
      end if
      call init_line

C     Update values along the wave for nstep time steps
      call update(left, right)

C     Master collects results from workers and prints
      if (taskid .eq. MASTER) then
         call out_master
	endt=mpi_wtime()
	print*,endt-startt
      else 
         call out_workers
      end if
      call MPI_FINALIZE(ierr)
      end

C  ------------------------------------------------------------------------
C     Master obtains input values from user
C  ------------------------------------------------------------------------ 

      subroutine init_master

c      implicit none
      include 'mpi_wave.h'
      include 'mpif.h'

      integer taskid, numtasks
      double precision endt,startt
	common/config/taskid, numtasks,startt,endt
      integer tpoints, nsteps
      common/inputs/tpoints, nsteps
      integer request, request1, request2
      integer MAXPOINTS, MAXSTEPS
      integer status(3), ierr
 
      parameter (MAXPOINTS = 800)
      parameter (MAXSTEPS = 10000)
      integer buffer(2)

      tpoints = MAXPOINTS
      nsteps = 0
c      print *, 'Starting mpi_wave using', numtasks, 'tasks.'
c      print *, 'Using',tpoints,'points on the vibrating string.'
 
      do while ((nsteps .lt. 1) .or. (nsteps .gt. MAXSTEPS))
c         write (*,*) 'Enter number of time steps (1-',MAXSTEPS,'):'
c         read (*,*) nsteps
	nsteps=10000
         if ((nsteps .lt. 1) .or. (nsteps .gt. MAXSTEPS))
     &        write (*,*) 'enter value between 1 and ', MAXSTEPS
      end do

c      write (*,10) taskid, tpoints, nsteps
c 10   format(I5, ': points = ', I5, ' steps = ', I5)

C     Broadcast total points, time steps
	buffer(1) = tpoints
      buffer(2) = nsteps
      call MPI_BCAST(buffer, 2, MPI_INTEGER, MASTER, MPI_COMM_WORLD,
     &               ierr)
      end

C  -------------------------------------------------------------------------
C     Workers receive input values from master
C  -------------------------------------------------------------------------
 
      subroutine init_workers

c	implicit none
      include 'mpi_wave.h'
      include 'mpif.h'
	double precision endt,startt
      integer taskid, numtasks
      common/config/taskid, numtasks,endt,startt

      integer tpoints, nsteps
      common/inputs/tpoints, nsteps

      integer buffer(2)
      integer status(3), ierr
      integer request, request1, request2

C     Receive time advance parameter, total points, time steps
      call MPI_BCAST(buffer, 2, MPI_INTEGER, MASTER, MPI_COMM_WORLD,
     &               ierr)
      tpoints = buffer(1)
      nsteps =  buffer(2)

      end

C  ------------------------------------------------------------------------
C     Initialize points on line
C  -----------------------------------------------------------------------

      subroutine init_line

c      implicit none
	double precision endt,startt
      integer taskid, numtasks
      common/config/taskid, numtasks,endt,startt
      integer tpoints, nsteps
      common/inputs/tpoints, nsteps
      integer npoints, first
      common/decomp/npoints, first
      real*8 values(0:1001), oldval(0:1001), newval(0:1001)
      common/data/values, oldval, newval
      real*8 PI
      parameter (PI = 3.14159265)
      integer nmin, nleft, npts, i, j, k
      real*8 x, fac

C     Calculate initial values based on sine curve
      nmin = tpoints/numtasks
      nleft = mod(tpoints, numtasks)
      fac = 2.0 * PI

      k = 0
      do i = 0, numtasks-1
         if (i .lt. nleft) then
            npts = nmin + 1
         else
            npts = nmin
         endif
         if (taskid .eq. i) then
            first = k + 1
            npoints = npts
c	    write (*,15) taskid, first, npts
c 15         format ('task=',I5, ': first = ', I5, ' npoints = ', I5)
            do j = 1, npts
               x = float(k)/float(tpoints - 1)
               values(j) = sin (fac * x)
               k = k + 1
            end do
         else 
            k = k + npts
         end if
      end do
      do i = 1, npoints
         oldval(i) = values(i)
      end do
      end

C  -------------------------------------------------------------------------
C     Update all values along line a specified number of times 
C  -------------------------------------------------------------------------
 
      subroutine update(left, right)
c      implicit none
      integer left, right
	double precision endt,startt
      common/config/taskid, numtasks,startt,endt
      include 'mpi_wave.h'
      include 'mpif.h'
      integer npoints, first
      common/decomp/npoints, first
      integer tpoints, nsteps
      common/inputs/tpoints, nsteps
      real*8 values(0:1001), oldval(0:1001), newval(0:1001)
      common/data/values, oldval, newval
      integer E_RtoL, E_LtoR
      integer status(3), ierr
      integer request, request1, request2
      parameter (E_RtoL = 10)
      parameter (E_LtoR = 20)
      integer i, j, id_rtol, id_ltor, nbytes
      real*8 dtime, c, dx, tau, sqtau

      dtime = 0.3
      c = 1.0
      dx = 1.0
      tau = (c * dtime / dx)
      sqtau = tau * tau

C     Update values for each point along string
      do i = 1, nsteps

C        Exchange data with "left-hand" neighbor
         if (first .ne. 1) then
            call MPI_ISEND(values(1),1,MPI_DOUBLE_PRECISION,left,
     &                     E_RtoL,MPI_COMM_WORLD,request,ierr )
            call MPI_RECV( values(0),1,MPI_DOUBLE_PRECISION,left,
     &                     E_LtoR, MPI_COMM_WORLD, status, ierr )
            call MPI_WAIT(request, status, ierr)
         end if
C        Exchange data with "right-hand" neighbor
	 if (first+npoints-1 .ne. tpoints) then
            call MPI_ISEND(values(npoints),1,MPI_DOUBLE_PRECISION,
     &               right,E_LtoR,MPI_COMM_WORLD,request,ierr)
            call MPI_RECV(values(npoints+1), 1, MPI_DOUBLE_PRECISION,
     &                 right,E_RtoL, MPI_COMM_WORLD, status, ierr )
            call MPI_WAIT(request, status, ierr)
         end if

C        Update points along line
         do j = 1, npoints
C           Global endpoints
            if ((first+j-1 .eq. 1).or.(first+j-1 .eq. tpoints))then
               newval(j) = 0.0
	        else
C              Use wave equation to update points
               newval(j) = (2.0 * values(j)) - oldval(j)
     &         + (sqtau * (values(j-1) - (2.0 * values(j)) 
     &         + values(j+1)))
            end if
         end do

         do j = 1, npoints
            oldval(j) = values(j)
	    values(j) = newval(j)
         end do

      end do

      end

C  ------------------------------------------------------------------------
C     Receive results from workers and print
C  ------------------------------------------------------------------------ 

      subroutine out_master

c      implicit none
      include 'mpi_wave.h'
      include 'mpif.h'
      double precision endt,startt
      integer taskid, numtasks
      common/config/taskid, numtasks,endt,startt
      integer tpoints, nsteps
      common/inputs/tpoints, nsteps
      integer npoints, first
      common/decomp/npoints, first
      real*8 values(0:1001), oldval(0:1001), newval(0:1001)
      common/data/values, oldval, newval
      integer i, j, source, nbytes, start, npts, buffer(2), tpts
      integer status(3), ierr
      integer request, request1, request2
      real*8 results(1000) 
	open(1,file='out')
C     Store worker's results in results array
      do i = 1, numtasks - 1
C        Receive number of points and first point
         call MPI_RECV( buffer, 2, MPI_INTEGER, i,
     &                  E_OUT1, MPI_COMM_WORLD, status, ierr )
         start = buffer(1)
         npts = buffer(2)

C        Receive results
         call MPI_RECV(results(start),npts,MPI_DOUBLE_PRECISION,
     &      i, E_OUT2, MPI_COMM_WORLD, status, ierr )
      end do

C     Store master's results in results array
      do i = first, first+npoints-1
         results(i) = values(i)
      end do


C     Print results
c      print *,'Printing final results...'
      do i=1, tpoints
c         write (*,200) results(i)
	   write(1,200) results(i)
         j = MOD(i,10)
         if (j .eq. 0) then
c            print *,' '
            write(1,*)' ' 
	   endif
      enddo
 200  format(f6.3,' ',$)
c      print *, ' '
c      print *, 'Click the EXIT button or use CTRL-C to quit'
	
c     print *, ' '
	close(1)
c	print*,'time',endt-startt

C     Display results with draw_wave routine
c      call draw_wave(%REF(results))

      end

C  -------------------------------------------------------------------------
C     Send the updated values to the master
C  -------------------------------------------------------------------------
 
      subroutine out_workers

c      implicit none
      include 'mpi_wave.h'
      include 'mpif.h'
	double precision endt,startt
      common/config/taskid, numtasks,startt,endt
      integer npoints, first
      common/decomp/npoints, first

      real*8 values(0:1001), oldval(0:1001), newval(0:1001)
      common/data/values, oldval, newval

      integer buffer(2), nbuf(4), msgid, nbytes, allmsg
      integer status(3), ierr
      integer request, request1, request2

C     Send first point and number of points handled to master
      buffer(1) = first
      buffer(2) = npoints
      call MPI_ISEND(buffer,2,MPI_INTEGER,MASTER,E_OUT1, 
     &               MPI_COMM_WORLD,request1,ierr )

C     Send results to master
      call MPI_ISEND(values(1),npoints,MPI_DOUBLE_PRECISION,MASTER,
     &               E_OUT2,MPI_COMM_WORLD,request2,ierr )
      call MPI_WAIT(request1, status, ierr)
      call MPI_WAIT(request2, status, ierr)

      end
