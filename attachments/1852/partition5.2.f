      program main
      implicit none
      include 'mpif.h'

      integer label, comm1, comm2
      integer taskid, allgrp
      integer ierror

      call MPI_INIT (ierror)
      call MPI_COMM_RANK (MPI_COMM_WORLD, taskid, ierror)
      allgrp = MPI_COMM_WORLD
       
      if (taskid/2*2.eq.taskid) then
        label = 1
      else
        label = -1
      endif
      call MPI_COMM_SPLIT(MPI_COMM_WORLD,label,0,comm1,ierror)
      call MPI_COMM_SPLIT(comm1,label,0,comm2,ierror)
      call mpi_finalize(ierror)
      stop
      end
