
-------------------------------------------
Chris Tanner
Space Systems Design Lab
Georgia Institute of Technology
christopher.tanner@gatech.edu
-------------------------------------------



On Oct 7, 2008, at 3:04 PM, Rajeev Thakur wrote:

> Can you send us the output printed by configure on the screen? You  
> can send
> it to mpich2-maint@mcs.anl.gov.
>
> Rajeev
>
>
>> -----Original Message-----
>> From: owner-mpich-discuss@mcs.anl.gov
>> [mailto:owner-mpich-discuss@mcs.anl.gov] On Behalf Of
>> Christopher Tanner
>> Sent: Tuesday, October 07, 2008 1:42 PM
>> To: mpich-discuss@mcs.anl.gov
>> Subject: [mpich-discuss] mpif77 with ifort?
>>
>> I compiled MPICH2 (1.0.7) using the Intel compilers and everything
>> seemed to go fine. However, when I performed the 'mpecheckinstall'
>> script in the <mpich2_dir>/sbin/ directory, I received the following
>> errors with F77:
>>
>> *** Link F77 program with the MPI and manual logging
>> libraries ........... \c
>> No.
>>     The failed command is :
>> /usr/local/mpi/mpich2-1.0.7/bin/mpif77 -I/usr/local/mpi/mpich2-1.0.7/
>> include -O2  -c fpilog.f
>> /usr/local/mpi/mpich2-1.0.7/bin/mpif77 -mpe=mpilog  -O2  -o fpilog
>> fpilog.o  -lm
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_commset.po): In
>> function
>> `CLOG_CommSet_read':
>> clog_commset.c:(.text+0x228): undefined reference to
>> `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_commset.po): In
>> function
>> `CLOG_CommSet_append_GIDs':
>> clog_commset.c:(.text+0x73f): undefined reference to
>> `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_commset.po): In
>> function
>> `CLOG_CommSet_add_GID':
>> clog_commset.c:(.text+0x8c4): undefined reference to
>> `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_commset.po): In
>> function
>> `CLOG_CommSet_merge':
>> clog_commset.c:(.text+0xb14): undefined reference to
>> `_intel_fast_memset'
>> clog_commset.c:(.text+0xc46): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_commset.po): In
>> function
>> `CLOG_CommSet_add_intracomm':
>> clog_commset.c:(.text+0xfdd): undefined reference to
>> `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_commset.po): In
>> function
>> `CLOG_CommSet_add_intercomm':
>> clog_commset.c:(.text+0x118c): undefined reference to
>> `_intel_fast_memset'
>> clog_commset.c:(.text+0x1231): undefined reference to
>> `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_commset.po): In
>> function
>> `CLOG_CommSet_create':
>> clog_commset.c:(.text+0x152a): undefined reference to
>> `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_commset.po): In
>> function
>> `CLOG_CommSet_init':
>> clog_commset.c:(.text+0x1624): undefined reference to
>> `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/
>> libmpe.a(clog_commset.po):clog_commset.c:(.text+0x1725): more
>> undefined references to `_intel_fast_memset' follow
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(mpe_log.po): In function
>> `MPE_Log_pack':
>> mpe_log.c:(.text+0xdb6): undefined reference to `_intel_fast_memcpy'
>> mpe_log.c:(.text+0xdec): undefined reference to `_intel_fast_memcpy'
>> mpe_log.c:(.text+0xe33): undefined reference to `_intel_fast_memcpy'
>> mpe_log.c:(.text+0xe76): undefined reference to `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(iterator.po): In function
>> `MPE_CallStack_iteratorInit':
>> iterator.c:(.text+0xd4): undefined reference to `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_uuid.po): In function
>> `CLOG_Uuid_compare':
>> clog_uuid.c:(.text+0x55): undefined reference to `_intel_fast_memcmp'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_uuid.po): In function
>> `CLOG_Uuid_is_equal':
>> clog_uuid.c:(.text+0x63): undefined reference to `_intel_fast_memcmp'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_uuid.po): In function
>> `CLOG_Uuid_generate':
>> clog_uuid.c:(.text+0x170): undefined reference to  
>> `_intel_fast_memcpy'
>> clog_uuid.c:(.text+0x194): undefined reference to  
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_merger.po): In
>> function
>> `CLOG_Merger_next_localblock_hdr':
>> clog_merger.c:(.text+0x1e4): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_merger.po): In
>> function
>> `CLOG_Merger_save_rec':
>> clog_merger.c:(.text+0x41d): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_merger.po): In
>> function
>> `CLOG_Merger_next_sideblock_hdr':
>> clog_merger.c:(.text+0x557): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(clog_merger.po):clog_
>> merger.c:
>> (.text+0x920): more undefined references to
>> `_intel_fast_memcpy' follow
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(group_range_excl.o): In
>> function `MPI_Group_range_excl':
>> group_range_excl.c:(.text+0x210): undefined reference to
>> `__svml_idiv4'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(group_range_incl.o): In
>> function `MPI_Group_range_incl':
>> group_range_incl.c:(.text+0x203): undefined reference to
>> `__svml_idiv4'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ad_open.o): In function
>> `ADIO_Open':
>> ad_open.c:(.text+0x611): undefined reference to `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ad_read_coll.o): In
>> function `ADIOI_GEN_ReadStridedColl':
>> ad_read_coll.c:(.text+0x18fa): undefined reference to
>> `_intel_fast_memcpy'
>> ad_read_coll.c:(.text+0x194c): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ad_read_coll.o): In
>> function `ADIOI_R_Exchange_data':
>> ad_read_coll.c:(.text+0x2c35): undefined reference to
>> `_intel_fast_memcpy'
>> ad_read_coll.c:(.text+0x2dff): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/
>> libmpich.a(ad_write_coll.o):ad_write_coll.c:(.text+0x2a56): more
>> undefined references to `_intel_fast_memcpy' follow
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(cb_config_list.o): In
>> function `ADIOI_cb_config_list_parse':
>> cb_config_list.c:(.text+0x122): undefined reference to
>> `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ch3u_handle_recv_pk
>> t.o): In
>> function `MPIDI_CH3_PktHandler_LockAccumUnlock':
>> ch3u_handle_recv_pkt.c:(.text+0x736): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ch3u_handle_recv_pk
>> t.o): In
>> function `MPIDI_CH3U_Receive_data_found':
>> ch3u_handle_recv_pkt.c:(.text+0x100f): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ch3u_handle_recv_pk
>> t.o): In
>> function `MPIDI_CH3_PktHandler_GetResp':
>> ch3u_handle_recv_pkt.c:(.text+0x1774): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ch3u_handle_recv_pk
>> t.o): In
>> function `MPIDI_CH3_PktHandler_Get':
>> ch3u_handle_recv_pkt.c:(.text+0x1cdb): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ch3u_handle_recv_pk
>> t.o): In
>> function `MPIDI_CH3_PktHandler_Accumulate':
>> ch3u_handle_recv_pkt.c:(.text+0x21d1): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/
>> libmpich.a(ch3u_handle_recv_pkt.o):ch3u_handle_recv_pkt.c:(.text
>> +0x2380): more undefined references to `_intel_fast_memcpy' follow
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ad_write_str.o): In
>> function `ADIOI_GEN_WriteStrided':
>> ad_write_str.c:(.text+0x703): undefined reference to
>> `_intel_fast_memset'
>> ad_write_str.c:(.text+0x8b3): undefined reference to
>> `_intel_fast_memcpy'
>> ad_write_str.c:(.text+0xa55): undefined reference to
>> `_intel_fast_memcpy'
>> ad_write_str.c:(.text+0xd4a): undefined reference to
>> `_intel_fast_memcpy'
>> ad_write_str.c:(.text+0xed2): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ad_nfs_read.o): In
>> function
>> `ADIOI_NFS_ReadStrided':
>> ad_nfs_read.c:(.text+0x4e5): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/
>> libmpich.a(ad_nfs_read.o):ad_nfs_read.c:(.text+0x532): more
>> undefined
>> references to `_intel_fast_memcpy' follow
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ad_prealloc.o): In
>> function
>> `ADIOI_GEN_Prealloc':
>> ad_prealloc.c:(.text+0x17c): undefined reference to
>> `_intel_fast_memset'
>> collect2: ld returned 1 exit status
>> make[1]: *** [fpilog] Error 1
>>
>> Running installation linktest for Fortran collchk program...
>>
>> *** Link F77 program with the MPI collective/datatype checking
>> library ... \c
>> No.
>>     The failed command is :
>> /usr/local/mpi/mpich2-1.0.7/bin/mpif77 -I/usr/local/mpi/mpich2-1.0.7/
>> include -O2  -c wrong_reals.f
>> /usr/local/mpi/mpich2-1.0.7/bin/mpif77 -mpe=mpicheck  -O2  -o
>> wrong_reals wrong_reals.o
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpe.a(iterator.po): In function
>> `MPE_CallStack_iteratorInit':
>> iterator.c:(.text+0xd4): undefined reference to `_intel_fast_memset'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(helper_fns.o): In
>> function
>> `MPIR_Localcopy':
>> helper_fns.c:(.text+0x6b3): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(errutil.o): In function
>> `MPIR_Err_create_code_valist':
>> errutil.c:(.text+0x1ca3): undefined reference to `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ch3u_handle_recv_pk
>> t.o): In
>> function `MPIDI_CH3_PktHandler_LockAccumUnlock':
>> ch3u_handle_recv_pkt.c:(.text+0x736): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ch3u_handle_recv_pk
>> t.o): In
>> function `MPIDI_CH3U_Receive_data_found':
>> ch3u_handle_recv_pkt.c:(.text+0x100f): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/libmpich.a(ch3u_handle_recv_pk
>> t.o): In
>> function `MPIDI_CH3_PktHandler_GetResp':
>> ch3u_handle_recv_pkt.c:(.text+0x1774): undefined reference to
>> `_intel_fast_memcpy'
>> /usr/local/mpi/mpich2-1.0.7/lib/
>> libmpich.a(ch3u_handle_recv_pkt.o):ch3u_handle_recv_pkt.c:(.text
>> +0x1cdb): more undefined references to `_intel_fast_memcpy' follow
>> collect2: ld returned 1 exit status
>> make[1]: *** [wrong_reals] Error 1
>>
>>
>> Everything with C was fine. Is there an issue I have with ifort and
>> mpich2? Thanks!
>>
>> -------------------------------------------
>> Chris Tanner
>> Space Systems Design Lab
>> Georgia Institute of Technology
>> christopher.tanner@gatech.edu
>> -------------------------------------------
>>
>>
>>
>>
>

