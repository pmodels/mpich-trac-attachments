if [ 'anyPrefix'"${AMENDED_PATHS}" == 'anyPrefix' ] ; then
    export AMENDED_PATHS=1
else
    exit
fi

export EDITOR=emacs
export VISUAL=emacs
export EMAIL=maier@ucalgary.ca
export REPLYTO=maier@ucalgary.ca
export PYTHONSTARTUP=${HOME}/.pystartup
export WHEN=never

if test -d /tmp/maier ; then
    export TMPDIR=/tmp/maier
else
    export TMPDIR=/tmp
fi
# for mutt:
export MM_NOASK=1
export MAILDIR=/var/spool/mail

## Source global definitions. /etc/profile lists other possible profiles.
#if [ -f /etc/profile ]; then
#     .  /etc/profile
#fi

UC_HONAM='rolf'

if test -n "${WINDIR}" ; then
    SYS_DRV=/${SYSTEMDRIVE:0:1}
    PROG_FILS="/${SYSTEMDRIVE:0:1}/Program Files"
    # or:
    # export SYSTEM_DRIVE=`cygpath -ua ${SYSTEMDRIVE}`
    # export PROGRAM_FILES=`cygpath -ua "${PROGRAMFILES}"`
    HONAM="`uname -n`"
else
    HONAM="`hostname`"
fi

if [ -d /tmp ]; then
    export CWP_TMPDIR=/tmp/${USER}
    export	  TMP=/tmp/${USER}
fi

if [ -d /usr/share/gnuplot.gih ]; then export GNUPLOT=/usr/share/gnuplot.gih; fi

if [ -d /opt/intel/fc/9.0/bin ]; then
    source /opt/intel/cc/9.0/bin/iccvars.sh
    source /opt/intel/fc/9.0/bin/ifortvars.sh
fi

export PROMAX_DATA_HOME=${HOME}/Promax
if [ `uname -s` == "Linux" ]; then
    export PROMAX_HOME='/ow/ProMAX_2003.19.1'
#   export PROMAX_HOME='disk/nut1/software/ow/ProMAX_2003.19.1'
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PROMAX_HOME}/sys/lib:/usr/lib
fi
if [ `uname -s` == "SunOS" ]; then
    export PROMAX_HOME='/disk/kona1/Software/ProMAX/2003.3.2'
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PROMAX_HOME}/sys/lib:/usr/dt/lib:/usr/lib
fi
#if test -n ${WINDIR} ; then
    if [ -d /home/ow/ProMAX ]; then
	export PROMAX_HOME=/home/ow/ProMAX
	export PATH="${PATH}:${PROMAX_HOME}/port/bin"
    fi
#fi

# Set this in the machine's (Window's) environment:
# (Control Panel -> System -> Advanced -> Environment Variables)
# binmode relates to text storage, ntsec tty to sshd.
# notraverse turns off permission checking when traversing dirs, eg. to get
# include files or libraries.
#if [ "`uname -s`" == "cygwin"]; then	# this may also return CYGWIN_NT-5.1 ...
#	# || `uname -o` == "Cygwin" may crash
#    export CYGWIN='binmode ntsec tty notraverse'; fi

if test -n "${WINDIR}" ; then
    unset MAILCHECK
    if test -z "${CYGWIN}" ; then
	export CYGWIN='binmode ntsec tty notraverse'
    fi

    export G95_CR='FALSE'
    #	   G95_CR: add ^M? True on Windows

    # for man:
    export BROWSER='H:\Program Files\Mozilla Firefox\firefox.exe'
fi

export	G95_CHECKPOINT=0
#	G95_CHECKPOINT: on x86, seconds between checkpoint core dumps. 0: none

export	G95_ENDIAN='NATIVE'
#	G95_ENDIAN: BIG, LITTLE, NATIVE (=default)

export	G95_LIST_SEPARATOR=""
#	G95_LIST_SEPARATOR: any nr of spaces and at most one comma. Deflt: 1 sp

export	G95_ABORT='FALSE'
#	G95_ABORT: dump core on crash. Default: false

export	G95_MEM_SEGMENTS=-1
#	G95_MEM_SEGMENTS: show segs allocd at exit if at most n, 0: noshow,
#			  -n: show all

export	G95_MEM_MAXALLOC=YES
#	G95_MEM_MAXALLOC: show max. nr of bytes allocated in run. Default=NO

export	G95_MEM_TRIM_THRESHOLD=262144
#	G95_MEM_TRIM_THRESHOLD: max amount of top-most memory to hold before
#	returning it to the system. -1: never return any. Default: 262144.
#	For long-lived programs.

# export G95_SIGHUP=String
# export G95_SIGINT=String
# String: IGNORE, SUSPEND, or Abort (-default).

# export G95_UNBUFFERED_x
# If true, unit x is unbuffered.

# Here are some recommended environment variables to assist with debugging from
# the Fortran 90/95/2003 Course, May 2006, lectured by Dr Paul Dellar
# (Mathematics), at http://www.ma.ic.ac.uk/~pdellar/FortranPG.html:
export G95_MEM_INIT=NAN
export G95_FPU_PRECISION=53
export G95_FPU_OVERFLOW=Yes
export G95_FPU_ZERODIV=Yes
export G95_FPU_INVALID=Yes
export G95_FLAGS="-Wall -fimplicit-none -ftrace=full -fbounds-check -finteger=9999 -freal=nan -fpointer=invalid"
# The last arg is expected to follow the g95 command.


# The following strings may need to be changed if this is for anyone else.
# Look for : rolf, maier, /h/

case "${USER}" in
    ("root" | "Administrator" | "Owner" | "maier")
	export PRINTER=//mars/Rainier
	export ENSCRIPT='-B -h -G2rE -C -M Letter --non-printable-format=caret'
	# -G: fancy header
	# -C: line numbering; -E[lang], --highlight[=lang]: highlighting, see
	# enscript --help-highlight; -d DevName; -n NrOfCopies
	# -w lang where lang is one of PostScript, html, overstrike, rtf, ansi
	if [ "${USER}" == "maier" ]; then
	    umask 077
	else
	    umask 022
	fi
	;;
    ("admin")
	# @ RM
	umask 022
	;;
    (*)
	umask 077
	export PRINTER=//mars/Shasta
	;;
esac

if [ ${LD_RUN_PATH} ] ; then
    export LD_RUN_PATH=${LD_RUN_PATH}:/usr/local/lib
else
    export LD_RUN_PATH=/usr/local/lib
fi

case "${HONAM}" in
#   ("impala" | "impala.geo.ucalgary.ca" | "impala.enel.ucalgary.ca")
    ("impala")
	export CVSROOT=":ext:maier@potsi:/usr/local/cvsroot"
	export CVS_RSH=/usr/bin/ssh
	export CWPROOT=/usr/local/Software/su
	# export CWPROOT=/home/impala1/geiger/su
#	export GNS_HOME=/usr/local/Claritas/current
#	if [ -x ${GNS_HOME}/home/Claritas_users.shrc ]; then
#	    source ${GNS_HOME}/home/Claritas_users.shrc
#	fi
	export TMPDIR=/tmp/$USER
	if [ ! -d $TMPDIR ]; then
	    mkdir ${TMPDIR}
	    chmod 755 ${TMPDIR}
	fi
	export MATLAB=/home/impala1/Matlab
	set MATLAB_sc="${MATLAB}"
	# MPDDIR: env. var. from the Installer's Guide:
	# export MPDDIR=/usr/local/mpich
	# export MPD_USE_USER_CONSOLE=yes
	export SUCRHOME=/usr/local/Software/SUCR
	export FOCIHOME=${HOME}/Software/MATLAB/foci
	export POTSIHOME=${HOME}/Software/MATLAB/potsi
	# export POTSIHOME=/home/impala1/geiger/potsi
	# export POTSIHOME=/usr/local/potsi		NONESUCH as yet
	export PATH=$PATH:${HOME}/bin_Linux
	;;
    ("hilo" | "jelly" | "dough" | "nut")
	export CWPROOT=/disk/nut1/Software/su
	export CVSROOT=${USER}/cvs
	export CVS_RSH=/usr/bin/ssh
#	export GNS_HOME=/disk/nut1/software/Claritas/current
	export MATLAB=/disk/nut1/software/matlab
	set MATLAB_sc="${MATLAB}"
	export PRINTER=//mars/Rainier
	# maier only:
	export PATH=$PATH:${HOME}/bin3
	;;
   ("potsi")
	export MATLAB=/Matlab
	set MATLAB_sc="${MATLAB}"
	# Use local libraries (comment for the next line. Meaning?)
	# export POTSIHOME=/usr/local/potsi
	export FOCIHOME=${HOME}/Software/foci
	export POTSIHOME=${HOME}/Software/potsi
	;;
#    ("lamb")
#	export CWPROOT=/disk/nut1/Software/su
#	export GNS_HOME=/usr/local/Claritas/current
#	# Lahey/Fujitsu Fortran 95 Package for Linux
#	export PATH=$PATH:/usr/local/lf9562/bin
#	# maier only:
#	export PATH=$PATH:${HOME}/bin_Linux
#	;;
##    ("maier" | "rolf")
    ("${UC_HONAM}")
	export CVSROOT=${USER}/cvs
	export CVS_RSH=/usr/bin/ssh
	# there are /usr/bin/cvs, /${SYSTEMDRIVE}/Program Files/TortoisCVS/cvs.
	export MATLAB='h:\Program Files\MATLAB\R2007b'
	set MATLAB_sc="${PROG_FILS}/MATLAB/R2007b"
	# Note: the MATLAB compiler, mcc, will not work unless MATLAB points to
	# the installation directory - in DOS format. The MATLAB env. var. must
	# start with uppercase C, or 'mex -v' will complain.
	# A definition of MATLAB with a '?' where the space is only good for
	# Cygwin command lines. Workaround: use the link /matlab.
	# MATLAB is defined under DOS from where it's imported.
	# LD_RUN_PATH for MATLAB (others?) is set unconditionally at the top.
	export CWPROOT=/home/su
	# export MPDDIR=/home/MPICH2
	# MPDDIR env. var. from the Installer's Guide.	
	export FOCIHOME=/${SYSTEMDRIVE:0:1}/home/foci
	export POTSIHOME=/${SYSTEMDRIVE:0:1}/home/potsi/i
#	export SUCRHOME=/${SYSTEMDRIVE:0:1}/home/SUCR
#	ssh-agent bash		# this prevents executig anything more in here.
	;;
    ("pkelapav")
	export CWPROOT=/home/su
	# export MPDDIR=/home/MPICH2
	# MPDDIR: env. var. from the Installer's Guide.
	export PRINTER=//mars/Rainier
	;;
    ("rm")
	# @ rm
	;;
    (*)
	# just a usually good guess, ssh -X obsoletes this:
	# export DISPLAY=maier.geo:0
	;;
esac

# location of MCRInstaller and extractCTF:
if test -z "${MATLAB}" ; then
    if test -n "${OS}" ; then
	if [ ${OS} == "Windows_NT" ] ; then
	    export MATLAB='h:\Program Files\MATLAB\R2007b'
	    set MATLAB_sc="${PROG_FILS}/MATLAB/R2007b"
	fi
    fi
    # else I do not know where to look for MATLAB
fi

if test -n "$MPDDIR" ; then
    if test -d $MPDDIR ; then
	export MPDDIR
    else
	echo 'Where did the MPDDIR, $MPDDIR, go?'
	unset  MPDDIR
    fi
fi

# @ potsi
if test -d /usr/local/OpenOffice/setup ; then
	 . /usr/local/OpenOffice/setup
fi

# The following should not be necessary. If your login window shows SHELL to be
# /usr/bash or /usr/bin/bash, comment this out, else set it to set subsequent
# shells to bash. Note: not all systems have the bash.
#    if [ -x /usr/bin/bash ]; then
#	export SHELL=/usr/bin/bash
#    else
#	export SHELL=/bin/bash
#    fi


# Color code for ls, from Henry Bland:
# export LS_COLORS='fi=00:di=01;34:ln=02;32:pi=40;33:bd=40;33;01:cd=40;33;01:or=40;31;01:ex=01;31:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.gz=01;31:*.bz2=01;31:*.deb=01;31:*.rpm=01;31:*.jpg=01;35:*.png=01;35:*.gif=01;35:*.bmp=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.png=01;35:*.mpg=01;35:*.avi=01;35:*.fli=01;35:*.gl=01;35:*.dl=01;35:*.mar=01;33:'
export LS_COLORS='fi=00:di=01;34:ln=02;32:*.mar=01;33:'
# my stuff: :*.mar=01;33
# 01;00-02,22	normal black on white
# 01;03,06,09-21	bold black
# 01;04		bold black underlined
# 01;05,07	reverse video
# 01;08		white (invisible)
# 01;30 bold black
# 01;31	bold red
# 01;32	bold medium green
# 01;33	yellow
# 01;34	bold blue
# 01;35 bold lilac
# 01;36	bold light blue
# 01;37	glossy white with lilac tinge
# 01;38,39	bold black
# 01;40	black bar
# 01;41	bold black on red background
# 01;42	bold black on green background
# 01;43	bold black on yellow background
# 01;44	bold black on (dark) blue background
# 01;45	bold black on lilac background
# 01;46	bold black on blue background
# 01;47	bold black on a glossy white with lilac tinge background
# 01;48 ...	bold black

# - - - -

# These functions can be fooled: add a/ before a/b/.
function add2pathbase ()
{
# Note: add2pathbase refuses to add a path to PATH if PATH already
# contains the string. Thus add /a before /a/b, not vice versa.
# Paths to non-existent directories are also discarded (test -d).
    if test -d "$1" ; then
	yy=`echo $PATHBASE | grep "$1"`
	if [[ ! ${yy} ]] ; then
	    export PATHBASE="${PATHBASE}:$1"
	fi
    fi
    unset yy
}

# There anyway: (why can't I comment all of this out?)
# save initial values
# Get the MCR from /matlab/toolbox/compiler/deploy/win32/MCRInstaller.exe
# MCRInstaller is a self-extracting archive.
yy="${PROG_FILS}/MATLAB/MATLAB Component Runtime/v75/bin/win32"
if test -d "${yy}" ; then
    export PATHBASE="${yy}:${PATH}"
else
    export PATHBASE=${PATH}
fi
unset yy

if [ "${OSTYPE}" == "cygwin" ]; then
    JAVA_HOME="${PROG_FILS}/Sun"
    add2pathbase "${JAVA_HOME}/jdk/bin"
    unset JAVA_HOME
    add2pathbase '${SYS_DRV}/local/bin'
    # do not use $SYSTEMROOT for /${SYSTEMDRIVE}/WINDOWS
    add2pathbase "${PROG_FILS}/Windows NT/Accessories"
    # Hardware/Software options:
    add2pathbase "${PROG_FILS}/Ahead/Nero StartSmart"
    add2pathbase "${PROG_FILS}/Common Files/Ulead Systems/MPEG"
    add2pathbase "${PROG_FILS}/ConTEXT"
    add2pathbase "${PROG_FILS}/Microsoft Office/Office12"
    add2pathbase "${PROG_FILS}/Microsoft Office/Office11"
#    add2pathbase "$MATLAB/bin/win32"	already there
    # CREWES products:
    add2pathbase "${PROG_FILS}/CREWES/QuadDes"
    add2pathbase "${PROG_FILS}/CREWES/Syngram"
fi

if [ "${OSTYPE}" == "msys" ]; then
    add2pathbase '${SYS_DRV}/msys/1.0/bin:${SYS_DRV}/msys/1.0/mingw/bin:'
fi

add2pathbase /usr/sbin
add2pathbase $HOME/bin

if test -n "${MATLAB_sc}" ; then
    add2pathbase ${MATLAB_sc}/bin
fi
add2pathbase ${HOME}/gnuplot/bin
add2pathbase ${CWPROOT}/bin
#add2pathbase ${GNS_HOME}/bin
#add2pathbase ${GNS_HOME}/utility/misc
#add2pathbase ${GNS_HOME}/utility/scripts
# there is only one file each in utility/old and utility/shf.

# impala:
add2pathbase /usr/java/j2re1.4.2_08/bin
add2pathbase $MPDDIR/bin
# Hugh's impala stuff: $HOME/vnc-4.0-x86_linux:$HOME/vnc-3.3.7-x86_linux

export PATH=${PATHBASE}

# for SarCheck:
if test -f /usr/local/bin/gnuplot ; then
    export GNUPLOTDIR=/usr/local/bin
fi

# - - - -

# These functions can be fooled: add a/ before a/b/.
function add2llp ()
{
    if test -d "$1" ; then
	yy=`echo $LD_LIBRARY_PATH | grep "$1"`
	if [[ ! ${yy} ]] ; then
	    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$1"
	fi
    # else
	# echo 'add2llp:' $1 is not a directory.
    fi
    unset yy
}

#	add2llp /usr/local/lib		probably there already
# moved mpatrol/build/unix/lib*.a here to stop rebuilds of Cygwin from deleting
# the files:

if test -z "${LD_LIBRARY_PATH}" ; then
    export LD_LIBRARY_PATH=/usr/local/lib:.
else
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}":/usr/local/lib:.
fi

if [ -d "${MATLAB_sc}" ]; then
    if [ "${OSTYPE}" == "linux" ]; then
	if [[ ${LD_LIBRARY_PATH} ]] ; then
	    export LD_LIBRARY_PATH=""
	fi
	add2llp "${MATLAB_sc}"/sys/java/jre/win32/jre1.6.0/lib
	add2pathbase "${MATLAB_sc}"/sys/java/jre/win32/jre1.6.0/bin
	export XAPPLRESDIR "${MATLAB_sc}"/X11/app-defaults
	unset MATLAB_sc
    fi
fi

# - - - -

# save initial values
#export MANBASE=${MANPATH}	# the predefined version is incomplete, so:
export MANBASE=/usr/ssl/man:/usr/share/man:/usr/man:/usr/X11R6/man
export MANBASE=${MANBASE}:/usr/X11R6/share/man:/usr/local/man
if [[ -d ${HOME}/man ]]; then export MANBASE=${MANBASE}:${HOME}/man; fi
# if [[ ${WINDOWID} ]];	  then export MANBASE=${MANBASE}:/p/man; fi
if [[ ${WINDOWID} ]]; then
    if [[ -d /cygdrive/p/ ]]; then export MANBASE=${MANBASE}:/cygdrive/p/man
#    else echo '/p, the pub directory, is not available.'
    fi
fi
if [[ -d "$MPDDIR/man" ]]; then export MANBASE=${MANBASE}:"$MPDDIR/man"; fi

export MANPATH=${MANBASE}

#if [ "${HONAM}" == "${UC_HONAM}" ] ; then
#    export FreeUSP=$HOME/Software/FreeUSP
#    if [ ! -d ${FREEUSP} ]; then
#	USP_ARCHIVE=/${SYSTEMDRIVE:0:1}/home/Software_ar/FreeUSP
#	export DEPTROOT=${FreeUSP}/trcgp
#	export TARCH=`${DEPTROOT}/bin/tarch`
#
#	export MANPATH=${MANPATH}:${DEPTROOT}/man:${DEPTROOT}/usp/man
#	export MANPATH=${MANPATH}:${FreeUSP}/dist/man/usp/man
#	export MANPATH=${MANPATH}:${FreeUSP}/dist/man/xaprtools/man
#
#	# PATH additions:
#	# set path = ( ${FreeUSP}/dist/bin ${DEPTROOT}/bin ${DEPTROOT}/usp/bin \
#	# ${DEPTROOT}/bin/${TARCH} ${DEPTROOT}/usp/bin/${TARCH} $path )
#	#
#	pfad=${FreeUSP}/dist/bin:${DEPTROOT}/bin:${DEPTROOT}/usp/bin
#	pfad=${pfad}:${DEPTROOT}/bin/${TARCH}:${DEPTROOT}/usp/bin/${TARCH}
#	PATH=${PATH}:${pfad}; export PATH
#    fi
#fi

#if [[ -f ${HOME}/.profile.user ]]; then
#	source ${HOME}/.profile.user
#fi

################################################################

# make tings accessible to others (eg me as root):
#if [ ${OSTYPE} == "solaris" ]; then
#    effective_user=`/usr/ucb/whoami`
#else
#    effective_user=`whoami`
#fi
#if [ "${effective_user}" == "maier" ] ; then
#elif [ "${HONAM}" == "impala" ]; then

case "${HONAM}" in
    ("${UC_HONAM}")
	source .bashrc
	;;
    ("impala" | "beep" | "hilo" | "jelly" | "dough" | "nut")
#	if [ "${USER}" == 'maier' ]; then
#	    source /home/impala1/maier/.bashrc
#	else
	    source ${HOME}/.bashrc
#	fi
	;;
    ("rm")
	source ${HOME}/.bashrc
	;;
    (*)
	if [ ${OSTYPE} == "cygwin" ]; then
	    # uname -s == CYGWIN_NT-5.0, OSTYPE == cygwin, OS == Windows_NT
	    source //mars/crewes1\$/maier/.bashrc
	else if test -r /home/unclass/maier/.bashrc ; then
	    source /home/unclass/maier/.bashrc
	else
	    source ${HOME}/.bashrc
	fi;	fi
	;;
esac

#if [ ${PPID} == 1 ] && [ "${HONAM}" == "${UC_HONAM}" ] ; then
#    # Do this only on my PC and only when starting Cygwin, not when testing.
#    # Any window started from the Cygwin icon has PPID 1. Rxvt-s never do.
#    if [ -S /tmp/mpd2.console_maier ] ; then
#	echo 'The socket /tmp/mpd2.console_maier exists already.'
#	echo Remove the socket and login again if this is not a test.
#    else
#	echo Starting MPD
#	STARTED_MPD=1
#	mpd --listenport=4444 &
#    fi
#fi
#
#if [ "${HONAM}" == "pkelapav" ]; then
#    if [ "`mpdtrace`" != "pkelapav" ]; then
#	mpd --host=maier --port=4444 --noconsole &
#    fi
#fi


# for df, du, ls, ...: (default now under Linux, may change in the future):
#export BLOCK_SIZE=1024
# default PS1 from CREWES PCs, for portability:
# export PS1="\[\033]0;\w\007 \033[32m\]\u@\h \[\033[33m\w\033[0m\]> "

# links in /:
# /c -> /cygdrive/a	diskette drive
# /c -> /cygdrive/c
# /c -> /cygdrive/e	CD-RW
# /f -> /cygdrive/f	DVD-RAM (was foghorn)
## /h -> /cygdrive/h	chocolat/matlab
# /h -> /cygdrive/h	system
# /i -> /cygdrive/i	used to be chocolat/classes, no USB
# /m -> /cygdrive/m	crewes1$
# /n -> /cygdrive/n	N:/user
# /p -> /cygdrive/j	used to be pub, no USB
# /p -> /cygdrive/p	pub
# /matlab, /work
