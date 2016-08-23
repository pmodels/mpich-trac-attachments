

On Oct 24, 2008, at 12:36 PM, mpich2 wrote:

> ----------------------------------------------- 
> +----------------------------
>  Reporter:  "Terje Dokland" <dokland@uab.edu>  |        Owner:
>      Type:  bug                                |       Status:  new
>  Priority:  major                              |    Milestone:
> Component:  mpich2                             |   Resolution:
>  Keywords:                                     |
> ----------------------------------------------- 
> +----------------------------
>
>
> Comment (by Rajeev Thakur):
>
>  {{{
>
>  Can you resend the configure logs? The files you sent contain nothing
>  other
>  than the word "(None)".
>
>  Rajeev
>
>> -----Original Message-----
>> From: owner-mpich2-bugs@mcs.anl.gov
>> [mailto:owner-mpich2-bugs@mcs.anl.gov] On Behalf Of mpich2
>> Sent: Friday, October 24, 2008 12:24 PM
>> To: undisclosed-recipients:
>> Subject: [mpich2-maint] #230: Compiling mpich2 with
>>
>> -----------------------------------------------+--------------
>> --------------
>>  Reporter:  "Terje Dokland" <dokland@uab.edu>  |        Type:  bug
>>    Status:  new                                |    Priority:  major
>> Milestone:                                     |   Component:  mpich2
>> -----------------------------------------------+--------------
>> --------------
>>
>>
>>  {{{
>>
>>  I am trying to compile mpich2-1.0.7 with gfortran as the fortran90
>>  compiler
>>  I use the command:
>>  ./configure --prefix=/home/dokland/mpich2-install --enable-mpi-f90
>>  --enable-mpi-f77 | & tee configure.lo
>>  which ends with
>>  configure: error: Unable to determine Fortran integer kinds for MPI
>>  types.
>>  I have set the flag
>>  setenv F90FLAGS "-i4"
>>  as instructed with respect to the g95 compiler. Indeed, I
>> can get it to
>>  work with the g95 compiler, but then mpif90 does not compile
>> my program.
>>
>>  gfortran version is GNU Fortran (GCC) 4.2.5 20081006 (prerelease)
>>  The configure logs are attached.
>>  Suggestions would be very welcome.
>>
>>
>>
>>
>>
>>  }}}
>>
>>
>> --
>> Ticket URL: <https://trac.mcs.anl.gov/projects/mpich2/ticket/230>
>>
>>
>
>  }}}
>
> -- 
> Ticket URL: <https://trac.mcs.anl.gov/projects/mpich2/ticket/ 
> 230#comment:>

