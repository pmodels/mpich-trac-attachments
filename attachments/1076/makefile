#------settings-------------------
ARCH         = lib
ARCHFLAGS    = /nologo
RANLIB       = echo

CC           = cl
CFLAGS       = /nologo /O2

FORTRAN	     = ifort
FFLAGS       = 

LOADER       = link
LOADOPTS     = 

#---------------------------------

#-------- change here-------------

FINCS        = /include:"C:\Documents and Settings\K830429\My Documents\Visual Studio Projects\Netlib64\mpich2\include"
FDEFS	     = 

CINCS        = -I.
CDEFS        = 

LIBPATH      = /LIBPATH:"C:\Documents and Settings\K830429\My Documents\Visual Studio Projects\Netlib64\mpich2\lib"

LIBS	= fmpich2.lib mpi.lib

INTEL   = 

OBJS = TEST.obj

f77exm = TEST64.exe

#-------- end of change here ----------

all:	$(f77exm)

$(f77exm): $(OBJS)
	$(LOADER) $(OBJS) $(INTEL) $(LIBS) $(LIBPATH) $(LOADOPTS) /out:$@

.c.obj:
	$(CC) $(CFLAGS) $(CINCS) $(CDEFS) -c $<

.f.obj:
	$(FORTRAN) $(FFLAGS) $(FINCS) $(FDEFS) -c $<

.for.obj:
	$(FORTRAN) $(FFLAGS) $(FINCS) $(FDEFS) -c $<

.f90.obj:
	$(FORTRAN) $(FFLAGS) $(FINCS) $(FDEFS) -c $<

clean:
	del *.obj
