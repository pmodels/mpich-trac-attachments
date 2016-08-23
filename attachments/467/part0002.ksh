Hi Guys,

Anybody been successful with combining Boost.MPI and MPICH2 before?

I keep getting this link-time error about 'now' couldn't be found. The
command I get from Boost.Build + Boost.Jam is the following (stripped
of Friendster-specific names/symbols):

    "g++-4.3.3" -L"/usr/lib64"   -o "output" -Wl,--start-group
"main.o" "boost_1_38_0/bin.v2/libs/mpi/build/gcc-4.3.3/debug/link-static/optimization-speed/threading-multi/libboost_mpi-gcc43-mt-d-1_38.a"
"boost_1_38_0/bin.v2/libs/serialization/build/gcc-4.3.3/debug/link-static/optimization-speed/threading-multi/libboost_serialization-gcc43-mt-d-1_38.a"
 -Wl,-Bstatic  -Wl,-Bdynamic -lmpich -lpthread -lrt -lrt
-Wl,--end-group -g -pthread -O2 -Wl,-rpath -Wl,-z,now -Wl,/usr/lib64
-march=opteron -pipe

The auto-configuration worked for MPI:

===============MPI Auto-configuration===============
Found MPICH wrapper compiler: mpicc
MPI compilation flags: -march=opteron -O2 -pipe -Wl,-z,now
-I/usr/include -L/usr/lib64 -Wl,-rpath -Wl,/usr/lib64 -lmpich
-lpthread -lrt
MPI link flags: -march=opteron -O2 -pipe -Wl,-z,now -I/usr/include
-L/usr/lib64 -Wl,-rpath -Wl,/usr/lib64 -lmpich -lpthread -lrt
MPI build features:
<include>/usr/include <library-path>/usr/lib64
<find-shared-library>mpich <find-shared-library>pthread
<find-shared-library>rt <cxxflags>-march=opteron <cxxflags>-O2
<cxxflags>-pipe <cxxflags>-Wl,-z,now <cxxflags>-Wl,-rpath
<cxxflags>-Wl,/usr/lib64 <include>/usr/include
<library-path>/usr/lib64 <find-shared-library>mpich
<find-shared-library>pthread <find-shared-library>rt
<linkflags>-march=opteron <linkflags>-O2 <linkflags>-pipe
<linkflags>-Wl,-z,now <linkflags>-Wl,-rpath <linkflags>-Wl,/usr/lib64
MPI launcher: mpirun -np
====================================================

In my Jamfile I have something like the following:

  exe output : main.cpp : <source>/boost//mpi <library>/mpi//mpi ;

Am I missing anything obvious?

TIA

-- 
Dean Michael Berris | Software Engineer, Friendster, Inc.
blog.cplusplus-soup.com | twitter.com/mikhailberis |
linkedin.com/in/mikhailberis | profiles.friendster.com/mikhailberis |
deanberris.com
_______________________________________________
Unsubscribe & other changes: http://lists.boost.org/mailman/listinfo.cgi/boost
