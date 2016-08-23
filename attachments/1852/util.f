c %I%%Z% %W% %G% %U% %P%
c Restricted Rights Notice:  Use, reproduction or disclosure is subject to
c restrictions set forth in Subcontract #B331593
c -----------------------------------------------------------------------------
c 	Write out test info into a unique file
c	
c	Note: the preprocessor "prpc" chokes on the character declaration
c	statements. Donot use prpc on this file until this problem is taken
c	care of. Regular xlf works ok.
c -----------------------------------------------------------------------------
	subroutine test_info(id, d_num, c_num, msg, fmt)

	implicit none
	include 'param.inc'

	integer id, d_num, c_num
	character msg
	character*(MAX_FMT_LEN) fmt

	character*(FL_LEN) myfile
	character*(FL_PRE_LEN) fname
	integer mf

c*****	Begin:

	fname = FL_PRE_STR
	mf = FL_UNIT_NUM		!file unit number (must be < 99)
	call creatnm(fname, %val(id), myfile)
	open(mf, FILE=myfile, STATUS='unknown')
	rewind(mf)

	write(mf, fmt) d_num, c_num
	close(mf)
	call addmsg(%val(id), myfile, msg)

	return
	end

