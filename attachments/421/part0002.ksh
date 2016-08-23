Hello,

I have been trying to build MPICH2 with PVFS2 support per the PVFS2  
Quick Start Guide and the MPICH2 Installer's Guide and have been  
experiencing the error shown in the attached log files.

The system being used is setup as follows:
-Operating System: CentOS 5.2 (x86_64 install)
-Kernel Version: 2.6.18-92.1.22.el5
-Compiler: gcc 4.1.2

-MPICH2 Version: 1.0.8
-PVFS2 Version: 2.8.0

After untarring the packages and installing PVFS2 (to its default  
location) with no problems, I issue the following commands to build  
MPICH2:
> ./configure --with-pvfs2=/usr/local 2>&1 | tee configure.log
> make 2>&1 | tee make.log

The make command fails during the build process for the ROMIO PVFS2  
components. Is this a result of something I am doing wrong during the  
install or is this a known bug? Thanks for your help.

-Christopher Mitchell
  Research Assistant - Computer Architecture and Storage Systems Group
  UCF Department of Electrical Engineering and Computer Science


----------------------------------------------------------------
This message was sent using IMP, the Internet Messaging Program.

