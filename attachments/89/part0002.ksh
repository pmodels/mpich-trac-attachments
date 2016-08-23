I attached the debugger to the receiver process and I always get the  
same stack I showed before. I don't know how MPIDU_Sock_wait() is  
implemented, but it looks like it calls poll() with timeout set to -1  
(blocking indefinitely), or else I should be able to catch a  
different last call after the interrupt.

This is the code on the receiving side. The send is for 18 elements,  
changing MaxElements from 10 to 20 fixes the problem:

void MDS::Worker::run(void)
{
   const unsigned MaxElements = 10;

   DST::MyType s;
   DST::MyType2 soList[MaxElements];
   DST::Admin adm;

   // Post all receives
   enum RECV { MyType_RECV, MyType2_RECV, ADMIN_RECV, MAX_RECV };
   MPI::Request req[MAX_RECV];
   req[MyType_RECV] = _comm.Irecv(&s, 1, _types.MyType(), MasterRank,  
DST::MyType);
   req[MyType2_RECV] = _comm.Irecv(soList, MaxElements, _types.MyType2 
(), MasterRank, DST::MyType2);
   req[ADMIN_RECV] = _comm.Irecv(&adm, 1, _types.adm(), MasterRank,  
DST::ADMIN);

   bool done = false;
   MPI::Status status;
   while( !done ) {

     cout << "Worker " << MyRank << " Waitany" << endl;
     int idx = MPI::Request::Waitany(MAX_RECV, req, status);
     cout << "Worker " << MyRank
          << " received data, index " << idx
          << ", tag " << status.Get_tag()
          << ", error code " << status.Get_error()
          << endl;

     switch( status.Get_tag() ) {

     case DST::MyType:
       cout << "Worker " << MyRank << " received MyType " << s.ticker  
<< endl;
       req[idx] = _comm.Irecv(&s, 1, _types.MyType(), MasterRank,  
DST::MyType);
       break;

     case DST::MyType2:
       cout << "Worker " << MyRank << " received " << status.Get_count 
(_types.MyType2()) << " MyType Elements" << endl;
       req[idx] = _comm.Irecv(soList, MaxMyType, _types.MyType2(),  
MasterRank, DST::MyType2);
       break;

     case DST::ADMIN:
       cout << "Worker " << MyRank << " received admin message " <<  
adm << endl;
       done = true;
       break;

     default:
       cout << "Worker " << MyRank << " received unknown tag " <<  
status.Get_tag() << endl;
       break;
     }
   }

   //req[MyType_RECV].Free();
   //req[MyType2_RECV].Free();
   //req[ADMIN_RECV].Free();
}


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

