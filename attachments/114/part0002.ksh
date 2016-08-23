I think your logic is ok, although I would move the if (rank==0) outside the

else and use rank in new_global instead of comm_world (because the 
controller is set up in terms of rank 0 in new_global). Then you don't need 
a program_c. 

But it makes no difference in the output. There must be a bug in the 
implementation. 

Rajeev 



> -----Original Message----- 
> From: owner-mpich-discuss@mcs.anl.gov 
> [mailto:owner-mpich-discuss@mcs.anl.gov] On Behalf Of Brad Penoff 
> Sent: Friday, June 27, 2008 4:38 PM 
> To: mpich-discuss@mcs.anl.gov 
> Subject: Re: [mpich-discuss] intercomm merge then intercomm 
> create bug/misunderstanding? 
> 
> On Fri, Jun 27, 2008 at 8:39 AM, Robert Latham 
> <robl@mcs.anl.gov> wrote: 
> > On Fri, Jun 27, 2008 at 01:19:43AM -0700, Brad Penoff wrote: 
> >> However, when I do this, I keep getting this error that I'm 
> >> having difficulty understanding: 
> >> 
> >> mpirun -np 3 ./spawn_merge_create_simple 2      #  the arg 2 is how 
> >> many procs to spawn 
> > 
> > Hey Brad 
> > 
> > I get this same error as well with MPICH2 from SVN (the 
> gang here has 
> > done a lot of work on the dynamic process path since the release...) 
> > 
> > If I only mpiexec one process and let mpi_comm_spawn build the rest, 
> > things seem to go ok.    I can't add too much to help you 
> out: I only 
> > break MPI-2 dynamic process routines, not work on them :> 
> 
> hey Rob, 
> 
> Thanks, ya I hadn't tried the latest MPICH2 from SVN.  For the record, 
> on MPICH2 1.0.7, this same code I sent you guys a link to also works 
> if I run only one process.  This says there's either a bug in MPICH2 
> or my "program_a" logic for creating the intercommunicator is 
> incorrect... 
> 
> I noticed on the new MPICH2 webpage that the bug tracker is a work in 
> progress... Do you know if there is a bug formally logged against 
> this? 
> 
> Thanks again, 
> brad 
> 
> 

