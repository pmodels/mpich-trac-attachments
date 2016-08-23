          program main
          implicit none

          include 'mpif.h'
          integer i,j,k,a,errors
          integer nnodes,mypid

          integer sbuf(1024*128)
          integer rbuf(1024*128)
          integer res
          integer scounts(33)
          integer rcounts(33)
          integer sdispls(33),rdispls(33)
          character*256 msg_string
          integer sdatatype,rdatatype

          call MPI_INIT(errors)
          call MPI_COMM_SIZE(MPI_COMM_WORLD, nnodes, errors)
          call MPI_COMM_RANK(MPI_COMM_WORLD, mypid, errors)

          if(nnodes .gt. 32) then
                        write(*,*) "Error: This case must run with ",
     >      "at most 32 tasks."
                        stop 255
          endif

          DO i=1, 1024*128
                sbuf(i) = mypid*10 + i;
          END DO

          sdispls(1) = 0
      DO i=1, nnodes
                rcounts(i) = mypid + 1
                scounts(i) = i * (mypid + 1)
                sdispls(i+1) = sdispls(i) + scounts(i) + 1;
          END DO


          sdatatype = MPI_INTEGER
          rdatatype = MPI_INTEGER

          call MPI_ALLTOALLV(MPI_IN_PLACE, 0, 0, sdatatype, sbuf,
     >      scounts, sdispls, sdatatype, MPI_COMM_WORLD, errors)

          call MPI_BARRIER(MPI_COMM_WORLD, errors)
          call MPI_ALLTOALL(sdispls,1,sdatatype,rdispls,1,rdatatype,
     >          MPI_COMM_WORLD,errors)

          call MPI_BARRIER(MPI_COMM_WORLD, errors)

          if (errors .ne. 0) then
                  write(*,*) 'MPI_Alltoallv return error number.'
                  stop 245
          endif

          DO i=1, nnodes
                  a=sdispls(i)
                  DO j=1, scounts(i)
                         a = a+1
                         res = 10*(i-1) + j + rdispls(i)
                         if(sbuf(a) .ne. res) then
                errors=errors+1
                 write(*,*) 'rank', mypid, a, ': sbuf[',a,']=',sbuf(a),
     >                          ', expected ', res
                         endif
                  ENDDO
          ENDDO
          if (errors .ne. 0) then
                  write(*,*) 'alltoallv1 got incorrect results!'
                  stop 255
          else
                  write(*,*) 'alltoallv1 completed successfully.'
          endif

          call MPI_FINALIZE(errors)
          END program












