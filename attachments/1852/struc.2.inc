c %I%%Z% %W% %G% %U% %P%
c Restricted Rights Notice:  Use, reproduction or disclosure is subject to
c restrictions set forth in Subcontract #B331593
      integer MAX_NPROCS,NUM_PROCS
      parameter (MAX_NPROCS = 64, NUM_PROCS=64)

      integer MaxMsg
      parameter(MaxMsg=1048576*10)
      
      integer MAX_NBYTES 
      parameter(MAX_NBYTES=1048576*10)

      integer MAX_MSG_LEN
      parameter(MAX_MSG_LEN=256)

      integer ierror
c
c	! Test info messages
	character*(MAX_MSG_LEN) F_BGN_MSG, F_END_MSG, F_ERR_MSG
c
	parameter (F_BGN_MSG = 
     >'("DIAG # ", i4, 4x, "Test Case # ", i5, 2x, "BEGIN TEST"
     >  )')
	parameter (F_END_MSG = 
     >'("DIAG # ", i4, 4x, "Test Case # ", i5, 2x, "END TEST"
     >  )')
	parameter (F_ERR_MSG = 
     >'("DIAG # ", i4, 4x, "Test Case # ", i5, 2x, "ERROR IN TEST"
     >  )')
c
c      Information required for util.f
c
      integer MAX_FMT_LEN,FL_LEN,FL_PRE_LEN,FL_UNIT_NUM			 
      parameter (MAX_FMT_LEN = 256, FL_UNIT_NUM=10)
      parameter (FL_PRE_LEN = 32, FL_LEN=FL_PRE_LEN + 6)
      character*(FL_PRE_LEN) FL_PRE_STR
      parameter (FL_PRE_STR = 'File\0')
