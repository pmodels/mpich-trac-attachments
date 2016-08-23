#!/bin/bash

type -a mpixlc_r

mpixlc_r -DBSEND  ibsend.c -o  bsend.exe
mpixlc_r -DIBSEND ibsend.c -o ibsend.exe
mpixlc_r -ISEND   ibsend.c -o  isend.exe
mpixlc_r          ibsend.c -o   send.exe

#llsubmit juqueen.job
# @ job_name = MPI_BSEND-Problem
# @ job_type = bluegene
# @ error = $(job_name).$(jobid).out
# @ output = $(job_name).$(jobid).out
# @ environment = COPY_ALL;
# @ wall_clock_limit = 00:10:00
# @ bg_size = 32
# @ bg_connectivity = MESH
# @ notification = never
# @ notify_user = m.winkel@fz-juelich.de
# @ queue

module list

echo "=============== runjob --label --verbose 0 --ranks-per-node 1 : ./send.exe ====================="
echo "=============== using MPI_SEND()                                           ====================="
runjob --block $1 --label --verbose 0 --ranks-per-node 1 : ./send.exe

echo "=============== runjob --label --verbose 0 --ranks-per-node 1 : ./isend.exe ====================="
echo "=============== using MPI_ISEND()                                           ====================="
runjob --block $1 --label --verbose 0 --ranks-per-node 1 : ./isend.exe

echo "=============== runjob --label --verbose 0 --ranks-per-node 1 : ./bsend.exe ====================="
echo "=============== using MPI_BSEND()                                          ====================="
runjob --block $1 --label --verbose 0 --ranks-per-node 1 : ./bsend.exe

echo "=============== runjob --label --verbose 0 --ranks-per-node 1 : ./ibsend.exe ====================="
echo "=============== using MPI_IBSEND()                                         ====================="
runjob --block $1 --label --verbose 0 --ranks-per-node 1 : ./ibsend.exe


