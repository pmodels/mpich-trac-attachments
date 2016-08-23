c 1.3@(#) src/fvt/ppe/MPI/mpiccl/ftest_src/param.inc, fvt, fvt_r2012, r20121317a 7/5/07 23:24:24 /home/cmvc/hpssl/vc/1/5/3/1/s.54
c Restricted Rights Notice:  Use, reproduction or disclosure is subject to
c restrictions set forth in Subcontract #B331593
c       Change the following according to the system
        include 'mpif.h'
        integer LOW_IND               ! SYSTEM SPECIFIC
        parameter (LOW_IND      = 0)  !lowest pe index, 0 - EUI, 1 - simulator

c       ! error number parameters, to be used with mp_stopall
        integer PRB1_SRC, PRB1_TYP, PRB1_LEN
        integer PRB2_SRC, PRB2_TYP, PRB2_LEN
        integer PRB3_SRC, PRB3_TYP, PRB3_LEN

        parameter (PRB1_SRC     = 100,
     >             PRB1_TYP     = 101,
     >             PRB1_LEN     = 102,
     >             PRB2_SRC     = 103,
     >             PRB2_TYP     = 104,
     >             PRB2_LEN     = 105,
     >             PRB3_SRC     = 106,
     >             PRB3_TYP     = 107,
     >             PRB3_LEN     = 108)

        integer BSR11_SRC, BSR11_PARAM, BSR11_LEN, BSR11_BUF
        integer BSR12_SRC, BSR12_PARAM, BSR12_LEN, BSR12_BUF
        integer BSR13_PARAM, BSR13_LEN, BSR13_BUF
        parameter (BSR11_SRC    = 200,
     >             BSR11_LEN    = 201,
     >             BSR11_PARAM  = 202,
     >             BSR11_BUF    = 203,
     >             BSR12_SRC    = 204,
     >             BSR12_LEN    = 205,
     >             BSR12_PARAM  = 206,
     >             BSR12_BUF    = 207)
        parameter (BSR13_LEN    = 208,
     >             BSR13_PARAM  = 209,
     >             BSR13_BUF    = 210)

c       ! error messages are in msgs.inc

c       ! other global parameters
        integer MAX_BUF, MAXMSG_SIZE, MAXMSG_TYPE

        parameter (MAX_BUF      =  262145)      ! (256K+1) 4 byte int
        parameter (MAXMSG_SIZE  = 1048576*8)      !   (1M+1) 4 byte int
        parameter (MAXMSG_TYPE  =   65536)      !     64K

c       !unit message parameters for program control
        integer MSG_SIZE, MSG_TYPE
        parameter (MSG_SIZE     = 32)
        parameter (MSG_TYPE     =  3)

c       ! file related parameters
        integer FL_PRE_LEN, FL_LEN, FL_UNIT_NUM
        parameter (FL_PRE_LEN   = 32)   !if changed change dim for FL_PRE_STR
        parameter (FL_LEN       = FL_PRE_LEN + 6) !total file name length
        parameter (FL_UNIT_NUM  = 10)   !file unit number (must be < 99)

        character*32 FL_PRE_STR
        parameter (FL_PRE_STR   = 'File\0')

c       ! file info message related parameters
        character*2 NULSTR
        parameter (NULSTR=  '\0')

        integer MAX_FMT_LEN, MAX_MSG_LEN
        parameter (MAX_FMT_LEN          = 256)  !if changed, change in msgs.inc
        parameter (MAX_MSG_LEN          = 256)

c       !misc
        integer ERR, NO_ERR
        parameter(ERR           = 0,
     >            NO_ERR        = 1)

        integer WILDC_BUF_SZ          !used to indicate buffersize (int)
        parameter (WILDC_BUF_SZ = 32)

c       ! Diagnostic test indices

        integer D_SEND40
        integer D_PROBE1, D_PROBE2, D_PROBE3
        integer D_BSR1, D_BSR2

        parameter (D_SEND40     = 40)
        parameter (D_PROBE1     = 50,
     >             D_PROBE2     = 51,
     >             D_PROBE3     = 52)
        parameter (D_BSR1       = 60,
     >             D_BSR2       = 61)

c       ! Max test cases for each diagnostic
        integer MAX_PRB_CASES

        parameter (MAX_PRB_CASES        = 4)

c       ! Individual cases
        integer ALL_EXPLICIT, SRC_DONTCR, TYPE_DONTCR, BOTH_DONTCR
        integer MANY_TO_ONE, MULTI_TYPES
        integer EXPLICIT_EXCH, EXPLICIT_RING, BSR_MANY_TO_ONE
        integer BSR_SRC_DONTCR1, BSR_SRC_DONTCR2
        integer SRC_DEST_NULL, BSR_MULTI_TYPES
        parameter (ALL_EXPLICIT         = 1,
     >             SRC_DONTCR           = 2,
     >             TYPE_DONTCR          = 3,
     >             BOTH_DONTCR          = 4)
        parameter (MANY_TO_ONE          = 5,
     >             MULTI_TYPES          = 6)
        parameter (EXPLICIT_EXCH        = 11,
     >             EXPLICIT_RING        = 12,
     >             BSR_MANY_TO_ONE      = 13,
     >             BSR_SRC_DONTCR1      = 14,
     >             BSR_SRC_DONTCR2      = 15)
        parameter (SRC_DEST_NULL        = 16,
     >             BSR_MULTI_TYPES      = 17)

c
c  blocking send receive parameters
      INTEGER*4 D_001,D_002,D_003,D_004,D_005,D_006,D_007,D_008         MYP00010
      INTEGER*4 D_009,D_010,D_011,D_012,D_013,D_014,D_015               MYP00020
      PARAMETER(D_001 = 1)                                              MYP00030
      PARAMETER(D_002 = 2)                                              MYP00040
      PARAMETER(D_003 = 3)                                              MYP00050
      PARAMETER(D_004 = 4)                                              MYP00060
      PARAMETER(D_005 = 5)                                              MYP00070
      PARAMETER(D_006 = 6)                                              MYP00080
      PARAMETER(D_007 = 7)                                              MYP00090
      PARAMETER(D_008 = 8)                                              MYP00100
      PARAMETER(D_009 = 9)                                              MYP00110
      PARAMETER(D_010 = 10)                                             MYP00120
      PARAMETER(D_011 = 11)                                             MYP00130
      PARAMETER(D_012 = 12)                                             MYP00140
      PARAMETER(D_013 = 13)                                             MYP00150
      PARAMETER(D_014 = 14)                                             MYP00160
      PARAMETER(D_015 = 15)                                             MYP00170



