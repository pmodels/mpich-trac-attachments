Hi again.

I had to make the modification below to make the testsuite of my
mpi4py complete. As this testsuite exercises almost all MPI features
in a single run (I mean, between MPI_Init/MPI_Finalize), I guess
that's the reason this problem showed up for me (and probably not for
you).

IMHO, 32 entries could still be not enough if in the near future the
usage of those finalizers increases in the mpich2 codebase.


Index: src/mpi/init/finalize.c
===================================================================
--- src/mpi/init/finalize.c	(revision 3665)
+++ src/mpi/init/finalize.c	(working copy)
@@ -38,7 +38,7 @@
     int  priority;           /* priority is used to control the order
 				in which the callbacks are invoked */
 } Finalize_func_t;
-#define MAX_FINALIZE_FUNC 16
+#define MAX_FINALIZE_FUNC 32
 static Finalize_func_t fstack[MAX_FINALIZE_FUNC];
 static int fstack_sp = 0;
 static int fstack_max_priority = 0;


-- 
Lisandro Dalcín
---------------
Centro Internacional de Métodos Computacionales en Ingeniería (CIMEC)
Instituto de Desarrollo Tecnológico para la Industria Química (INTEC)
Consejo Nacional de Investigaciones Científicas y Técnicas (CONICET)
PTLC - Güemes 3450, (3000) Santa Fe, Argentina
Tel/Fax: +54-(0)342-451.1594
