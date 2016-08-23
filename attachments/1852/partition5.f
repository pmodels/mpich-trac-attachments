      program main
      implicit none
      include 'struc.inc'
      include 'mpif.h'

      integer pgid, key, label, gid1, gid2
      integer numtask, taskid, buf(4), allgrp
      character*80 pers_msg

      integer istatus(MPI_STATUS_SIZE),direction
      integer type, dest
      integer dims(512),newcomm_cart,newcomm
      logical periods(512)
      integer group_world,newgroup

        call MPI_INIT (ierror)


      call MPI_COMM_SIZE (MPI_COMM_WORLD, numtask, ierror)
      call MPI_COMM_RANK (MPI_COMM_WORLD, taskid, ierror)
      allgrp = MPI_COMM_WORLD
       
c      call mp_environ(numtask,taskid)
c      call mp_task_query(buf,4,3)
c      allgrp = buf(4)
      if (taskid/2*2.eq.taskid) then
        label = 1
      else
        label = -1
      endif
      key = 0
      pers_msg='Partition5: testing mp_partition with incorrect pgid\0'
      call test_info (taskid, 0, 0, pers_msg, F_BGN_MSG)
c      call mp_sync(allgrp)
      call MPI_BARRIER(allgrp,ierror)
c      call mp_partition(allgrp,key,label,gid1)
      call MPI_COMM_SPLIT(allgrp,label,key,gid1,ierror)		
c      call mp_partition(gid1+1023,key,label,gid2)
      call MPI_COMM_SPLIT(gid1+1023,label,key,gid2,ierror)
      pers_msg = 'Partition5: INCORRECT return from mp_partition.\0'
      call test_info (taskid, 0, 0, pers_msg, F_ERR_MSG)
      call mpi_finalize(ierror) !D101859
      stop
      end
 
