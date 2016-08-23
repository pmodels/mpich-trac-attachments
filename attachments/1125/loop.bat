set mpi="c:\program files\mpich2\bin\mpiexec"
set port=10505
set nwork=8
for %%i in (a b c d e f g h i j k l m n o p q r s t u v w x y z) DO %mpi% -hosts 1 127.0.0.1 %nwork% -port %port% -phrase FOO c:\sb\test
