It's blocking in waitany. This morning after doing some other work in  
the same program I was actually able to get a message truncated error  
but now, after many runs, I'm back to the same problem. Compiling the  
program with a different MPI implementation returns a truncate error  
as expected. Waitany has 3 requests, only one has a buffer smaller  
than the incoming message (the only request where count > 1).

#0  0x900bc40c in poll ()
#1  0x000bb3e2 in MPIDU_Sock_wait () at /usr/include/c++/4.0.0/ 
iostream:76
#2  0x0006bb29 in MPIDI_CH3I_Progress () at /usr/include/c++/4.0.0/ 
iostream:76
#3  0x0002e006 in MPI_Waitany () at /usr/include/c++/4.0.0/iostream:76
#4  0x001346f7 in MPI::Request::Waitany (v1=3, v2=0xbffff860,  
v4=@0xbffff848) at /Users/rk/Development/MDS/Worker.cpp:965
#5  0x0000608e in MDS::Worker::run (this=0xbffff8e4) at /Users/rk/ 
Development/MDS/Worker.cpp:54
#6  0x00003645 in main (argc=2, argv=0xbffff9fc) at /Users/rk/ 
Development/MDS/demo.cpp:61


On Aug 12, 2008, at 10:33 AM, Rajeev Thakur wrote:

> So is it MPI_Waitany that blocks or MPI_Irecv? Are all the posted  
> Irecvs for
> the smaller size of 100?
>
> Rajeev
>
>> -----Original Message-----
>> From: owner-mpich-discuss@mcs.anl.gov
>> [mailto:owner-mpich-discuss@mcs.anl.gov] On Behalf Of Robert Kubrick
>> Sent: Monday, August 11, 2008 10:38 PM
>> To: mpich-discuss@mcs.anl.gov
>> Subject: Re: [mpich-discuss] MPI_Irecv does block
>>
>> This is the stack of one of the receiving process. The last
>> MPI call is MPI_Waitany(), as I am using a number of
>> MPI_Irecv() requests.
>> If I change the size of buf in the receiver to handle all the
>> elements sent by the master, the program runs with no problems.
>>
>> Attaching to process 8743.
>> Reading symbols for shared libraries . done Reading symbols
>> for shared libraries ....... done 0x900bc40c in poll ()
>> (gdb) where
>> #0  0x900bc40c in poll ()
>> #1  0x000ba702 in MPIDU_Sock_wait () at /usr/include/c++/4.0.0/
>> iostream:76
>> #2  0x0006b799 in MPIDI_CH3I_Progress () at /usr/include/c++/4.0.0/
>> iostream:76
>> #3  0x0002d49e in MPI_Waitany () at /usr/include/c++/4.0.0/ 
>> iostream:76
>> #4  0x001344cd in MPI::Request::Waitany (v1=3, v2=0xbffff860,
>> v4=@0xbffff848) at /Users/rk/Development/tst/Worker.cpp:965
>> #5  0x000060cc in MDS::Worker::run (this=0xbffff8e8) at /Users/rb/
>> Development/tst/Worker.cpp:57
>> #6  0x00003645 in main (argc=2, argv=0xbffff9fc) at /Users/rb/
>> Development/tst/demo.cpp:61
>>
>> On Aug 11, 2008, at 10:22 PM, Robert Kubrick wrote:
>>
>>> I want to send a number of elements through MPI_Send. However the
>>> receiving process does not know the maximum number of
>> elements in the
>>> array (it might be very large). The code will look something like:
>>>
>>> MASTER:
>>> char buf[500];
>>> ...
>>> MPI_Send(&buf, 500, SomeDerivedType, 1, ...);
>>>
>>> ==============
>>>
>>> WORKER:
>>> char buf[100];
>>> const int Master = 0;
>>> MPI_Irecv(&buf, 100, SomeDerivedType, Master, ...);
>>>
>>> I noticed that here the worker process blocks (despite the
>> MPI_Irecv
>>> non-blocking call) because it doesn't have enough room in
>> the buffer
>>> to receive all the elements. I know one way to solve the
>> problem is to
>>> use MPI_Probe, but is this the expected behavior if the
>> buffer is too
>>> small? Shouldn't MPI_Irecv return an error to avoid blocking?
>>>
>>
>>
>

