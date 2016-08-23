/* %I%%Z% %W% %G% %U% %P% */ 
/*Restricted Rights Notice:  Use, reproduction or disclosure is subject to
restrictions set forth in Subcontract #B331593
*/
#include	<stdio.h>

creatnm(char *pre, int id, char *str)	{

	if(sprintf(str,"%s%d",pre,id) < 0) {
	   printf("creatnm: Cannot creat file name (id = %d)\n", id);
	   exit(1);
	}
}


addmsg(int id, char *fn, char *msg) {
FILE *of, *fopen();

	if(!(of = fopen(fn, "a"))) {
	     fprintf(stdout,"PE %d: Cannot open output file %s\n", id, fn);
	     fprintf(stderr,"PE %d: Cannot open output file %s\n", id, fn);
	     fprintf(stdout,"PE %d: PE %d Exiting %s\n", id);
	     fprintf(stderr,"PE %d: PE %d Exiting %s\n", id);
	     exit(1);
	}

	fprintf(of,"%s\n",msg);
	fclose(of);
}
idle(int *time) {
	sleep(*time);
}

