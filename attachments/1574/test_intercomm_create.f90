PROGRAM test_case
  USE MPI
  IMPLICIT NONE

  INTEGER :: c0, c1
  INTEGER :: g0, g1
  INTEGER :: group_world
  INTEGER :: a, b, c, d
  INTEGER :: ierr, rank, remote_leader, tag
  INTEGER, DIMENSION(2) :: ranks
  INTEGER :: inter_comm

  CALL MPI_INIT(ierr)
  CALL MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierr)

  ! c0 intracomm ranks.
  a = 32
  b = 24
  ! c1 intracomm ranks.
  c = 25
  d = 26

  CALL MPI_COMM_GROUP(MPI_COMM_WORLD, group_world, ierr)
  ! Create c0.
  ranks = (/ a, b /)
  CALL MPI_GROUP_INCL(group_world, 2, ranks, g0, ierr)
  CALL MPI_COMM_CREATE(MPI_COMM_WORLD, g0, c0, ierr)
  ! Create c1.
  ranks = (/ c, d /)
  CALL MPI_GROUP_INCL(group_world, 2, ranks, g1, ierr)
  CALL MPI_COMM_CREATE(MPI_COMM_WORLD, g1, c1, ierr)

  ! Create intercomm.
  IF (rank == a .OR. rank == b) THEN
    remote_leader = c
    CALL MPI_INTERCOMM_CREATE(c0, 0, MPI_COMM_WORLD, remote_leader, tag,
    Cinter_comm, ierr)
  ELSE IF (rank == c .OR. rank == d) THEN
    remote_leader = a
    CALL MPI_INTERCOMM_CREATE(c1, 0, MPI_COMM_WORLD, remote_leader, tag,
    Cinter_comm, ierr)
  END IF

  CALL MPI_FINALIZE(ierr)
END PROGRAM test_case
