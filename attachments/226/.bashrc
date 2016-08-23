if [ 'anyPrefix'"${AMENDED_PATHS}" == 'anyPrefix' ] ; then
    . ${HOME}/.profile
fi

## Source global definitions
#if [ -f /etc/bashrc ]; then
#    .   /etc/bashrc
#fi
#if [ -f /etc/bash.bashrc ]; then	# Cygwin only, empty file
#    .   /etc/bash.bashrc
#fi

# Make filename completion case insensitive:
shopt -s nocaseglob
# also requires 'set completion-ignore-case on' in ~/.inputrc.

MY_HONAM='rolf'
if test -n "${WINDIR}" ; then
    HONAM="`uname -n`"
else
    HONAM="`hostname`"
fi

# From: http://biocycle.atmos.colostate.edu/~johnk/Xterm-Title.html
PROMPT_COMMAND='echo -ne "\033]0;${USER}@${HOSTNAME}: ${PWD}\007"'

function ccc ()
{
    gcc -DISMAIN -o $1 ${1}.c
}

function fff ()
{
# case word in [ [(] pattern [ | pattern ] ... ) list ;; ] ... esac

#    if test -f ${1}.F; then
#	g77 -o $1 ${1}.F
#    else
#	g77 -o $1 ${1}.f
#    fi

    if test -f ${1}.for; then
	g77 -o $1 ${1}.for
    else if test -f ${1}.FOR; then
	g77 -o $1 ${1}.FOR
    else if test -f ${1}.f; then
	g77 -o $1 ${1}.f
    else if test -f ${1}.F; then
	g77 -o $1 ${1}.F
    else if test -f ${1}.f90; then
	g95 -o $1 ${1}.f90
    else if test -f ${1}.F90; then
	g95 -o $1 ${1}.F90
    else if test -f ${1}.f95; then
	g95 -o $1 ${1}.f95
    else if test -f ${1}.F95; then
	g95 -o $1 ${1}.F95
    else
	echo File has no recognized Fortran suffix.
    fi;
    fi;
    fi;
    fi;
    fi;
    fi;
    fi;
    fi
}

# Using /n instead of /cygdrive/n which caught a cold today (7 Feb 2008).
# (/n works because /n is a link using //mars/...)

if [ "${OSTYPE}" == "cygwin" ]; then

#
    alias fp=fixpdf
    function fixpdf ()
    {
	cygstart "$1"
	more "$1".indextxt; echo ' L A S T : ' "$1"; ls -1 *.pdf
    }

    # requires SYSTEMDRIVE in '' expression.
    function mirror ()
    {
	# cp -i  "$1" `cygpath -a "$1" | sed -e 's*/h/home/maier*/n*'`  OR:
	TMPFILE=`mktemp -t mirror.XXXXXXXXXX` || exit 1
	echo `cygpath -a "$1"` > $TMPFILE

	sed -e "s%`cygpath -a $HOMEDRIVE`/home/maier%/n%" $TMPFILE >${TMPFILE}.2
	if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
	    sed -e 's*/h/home/*//mars/crewes1\$/*' $TMPFILE > ${TMPFILE}.2
	    if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		sed -e 's*/home/*//mars/crewes1\$/*' $TMPFILE > ${TMPFILE}.2
		if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		    sed -e 's*~*//mars/crewes1\$/maier*' $TMPFILE > ${TMPFILE}.2
		fi
	    fi
	fi
	if  test -f "$1" ; then
	    /bin/cp -i "$1" "`cat ${TMPFILE}.2`"
	else
	    echo 'Error: mirror acts only on normal files.'
	fi
	/bin/rm -f $TMPFILE ${TMPFILE}.2
    }
    
    function mirrorc ()
    {
	# cp -i "$1" `cygpath -a "$1" | sed -e 's*/h/home/maier*/c/maier*'` OR:
	TMPFILE=`mktemp -t mirror.XXXXXXXXXX` || exit 1
	echo `cygpath -a "$1"` > $TMPFILE

	sed -e "s%`cygpath -a $HOMEDRIVE`/home/maier%/c/maier%" $TMPFILE > ${TMPFILE}.2
	if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
	    sed -e 's*/h/home/*/c/maier/*' $TMPFILE > ${TMPFILE}.2
	    if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		sed -e 's*/home/*/c/maier/*' $TMPFILE > ${TMPFILE}.2
		if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		    sed -e 's*~*/c/maier*' $TMPFILE > ${TMPFILE}.2
		fi
	    fi
	fi
	if  test -f "$1" ; then
	    /bin/cp -i "$1" "`cat ${TMPFILE}.2`"
	else
	    echo 'Error: mirrorc acts only on normal files.'
	fi
	/bin/rm -f $TMPFILE ${TMPFILE}.2
    }
    
# Remote desktop connection:
    function desk ()
    {
	cygstart "$SYSTEMROOT/system32/mstsc.exe" /v:$1
    }

    function diffn ()
    {
	# cp -i   "$1" `cygpath -a "$1" | sed -e 's*/cygdrive/h/home/maier*/n*'`  OR:
	TMPFILE=`mktemp -t mirror.XXXXXXXXXX` || exit 1
	echo `cygpath -a "$1"` > $TMPFILE
	sed -e 's*/cygdrive/h/home/maier*//mars/crewes1\$/maier*' $TMPFILE > ${TMPFILE}.2
	if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
	    sed -e 's*/home/maier*//mars/crewes1\$/maier*' $TMPFILE > ${TMPFILE}.2
	    if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		sed -e 's*~*//mars/crewes1\$/maier*' $TMPFILE > ${TMPFILE}.2
	    fi
	fi
	if  test -f "$1" ; then
	    echo $1 VERSUS `cat ${TMPFILE}.2`
	    /bin/diff "$1" "`cat ${TMPFILE}.2`"
	else
	    echo 'Error: diffn acts only on normal files.'
	fi
	/bin/rm -f $TMPFILE ${TMPFILE}.2
    }
    
    function diffc ()
    {
	# cp -i   "$1" `cygpath -a "$1" | sed -e 's*/cygdrive/h/home/maier*/c/maier*'`  OR:
	TMPFILE=`mktemp -t mirror.XXXXXXXXXX` || exit 1
	echo `cygpath -a "$1"` > $TMPFILE
	sed -e 's*/cygdrive/h/home/maier*/c/maier*' $TMPFILE > ${TMPFILE}.2
	if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
	    sed -e 's*/home/maier*/c/maier*' $TMPFILE > ${TMPFILE}.2
	    if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		sed -e 's*~*/c/maier*' $TMPFILE > ${TMPFILE}.2
	    fi
	fi
	if  test -f "$1" ; then
	    echo $1 VERSUS `cat ${TMPFILE}.2`
	    /bin/diff "$1" "`cat ${TMPFILE}.2`"
	else
	    echo 'Error: diffn acts only on normal files.'
	fi
	/bin/rm -f $TMPFILE ${TMPFILE}.2
    }
    
    # pwdn takes an argument, unlike pwd.
    function pwdn ()
    {
	echo -n s/"`echo -n $HOME | sed -e 's/\//\\\\\//g'`"/\\\/n/>/tmp/zzz
	if [ "$1" == "" ] ; then
	    echo `echo -n $PWD | sed -f /tmp/zzz`/
	else
	    echo `echo -n  $1  | sed -f /tmp/zzz`/
	fi
	/bin/rm /tmp/zzz
    }

    function pwdc ()
    {
	echo -n s/"`echo -n $HOME | sed -e 's/\//\\\\\//g'`"/\\\/c\\\maier/>/tmp/zzz
	if [ "$1" == "" ] ; then
	    echo `echo -n $PWD | sed -f /tmp/zzz`/
	else
	    echo `echo -n  $1  | sed -f /tmp/zzz`/
	fi
	/bin/rm /tmp/zzz
    }

    # The next two macros list the equivalent of $HOME/current_dir under /N.
    function lsn ()
    {
	if [ "$1" == "" ] ; then
	   /bin/ls -AsFvh `cygpath -a "$PWD" | sed -e "s+${HOME}+/n+"`
	else
	   /bin/ls -AsFvh `cygpath -a  "$1"  | sed -e "s+${HOME}+/n+"`
	fi
    }

    function lln ()
    {
	if [ "$1" == "" ] ; then
	    /bin/ls -AsFvlGg `cygpath -a "$PWD" | sed -e "s+${HOME}+/n+"`
	else
	    /bin/ls -AsFvhlGg `cygpath -a  "$1"  | sed -e "s+${HOME}+/n+"`
	fi
    }

    function lsc ()
    {
	if [ "$1" == "" ] ; then
	   /bin/ls -AsFkvh `cygpath -a "$PWD" | sed -e "s+${HOME}+/c/maier+"`
	else
	   /bin/ls -AsFkvh `cygpath -a  "$1"  | sed -e "s+${HOME}+/c/maier+"`
	fi
    }

    function llc ()
    {
	if [ "$1" == "" ] ; then
	    /bin/ls -AsFvlgG `cygpath -a "$PWD" | sed -e "s+${HOME}+/c/maier+"`
	else
	    /bin/ls -AsFvlgG `cygpath -a  "$1"  | sed -e "s+${HOME}+/c/maier+"`
	fi
    }

    # cdn: go to the equivalent directory on the N drive. No spaces allowed.
    function cdn ()
    {
	if [ "$1" == "" ] ; then
	    cd `cygpath -a "$PWD" | sed -e "s+${HOME}+/n+"`
	else
	    cd `cygpath -a  "$1"  | sed -e "s+${HOME}+/n+"`
	fi
    }

    function cdc ()
    {
	if [ "$1" == "" ] ; then
	    cd `cygpath -a "$PWD" | sed -e "s+${HOME}+/c/maier+"`
	else
	    cd `cygpath -a  "$1"  | sed -e "s+${HOME}+/c/maier+"`
	fi
    }

    function mvn ()
    {
	TMPFILE=`mktemp -t mirror.XXXXXXXXXX` || exit 1

	echo `cygpath -a "$1"` > $TMPFILE
	sed -e 's*/cygdrive/h/home/maier*/n*' $TMPFILE > \
	    ${TMPFILE}.2
	if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
	    sed -e 's*/home/maier*/n*' $TMPFILE > ${TMPFILE}.2
	    if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		sed -e 's*~*/n*' $TMPFILE > ${TMPFILE}.2
	    fi
	fi

	echo `cygpath -a "$2"` > $TMPFILE
	sed -e 's*/cygdrive/h/home/maier*/n*' $TMPFILE > \
	    ${TMPFILE}.3
	if test -z "`diff $TMPFILE ${TMPFILE}.3`" ; then
	    sed -e 's*/home/maier*/n*' $TMPFILE > ${TMPFILE}.3
	    if test -z "`diff $TMPFILE ${TMPFILE}.3`" ; then
		sed -e 's*~*/n*' $TMPFILE > ${TMPFILE}.3
	    fi
	fi

	if test -f "$1" ; then
	    /bin/mv -i "`cat ${TMPFILE}.2`" "`cat ${TMPFILE}.3`"
	else
	    echo 'Error: mvn acts only on normal files.'
	fi
	/bin/rm -f $TMPFILE ${TMPFILE}.2 ${TMPFILE}.3
    }

    function mvc ()
    {
	TMPFILE=`mktemp -t mirror.XXXXXXXXXX` || exit 1

	echo `cygpath -a "$1"` > $TMPFILE
	sed -e 's*/cygdrive/h/home/maier*/c/maier*' $TMPFILE > \
	    ${TMPFILE}.2
	if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
	    sed -e 's*/home/maier*/c/maier*' $TMPFILE > ${TMPFILE}.2
	    if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		sed -e 's*~*/c/maier*' $TMPFILE > ${TMPFILE}.2
	    fi
	fi

	echo `cygpath -a "$2"` > $TMPFILE
	sed -e 's*/cygdrive/h/home/maier*/c/maier*' $TMPFILE > \
	    ${TMPFILE}.3
	if test -z "`diff $TMPFILE ${TMPFILE}.3`" ; then
	    sed -e 's*/home/maier*/c/maier*' $TMPFILE > ${TMPFILE}.3
	    if test -z "`diff $TMPFILE ${TMPFILE}.3`" ; then
		sed -e 's*~*/c/maier*' $TMPFILE > ${TMPFILE}.3
	    fi
	fi

	if test -f "$1" ; then
	    /bin/mv -i "`cat ${TMPFILE}.2`" "`cat ${TMPFILE}.3`"
	else
	    echo 'Error: mvn acts only on normal files.'
	fi
	/bin/rm -f $TMPFILE ${TMPFILE}.2 ${TMPFILE}.3
    }

    function rmn ()
    {
	TMPFILE=`mktemp -t mirror.XXXXXXXXXX` || exit 1
	echo `cygpath -a "$1"` > $TMPFILE
	sed -e 's*/cygdrive/h/home/maier*/n*' $TMPFILE > \
	    ${TMPFILE}.2
	if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
	    sed -e 's*/home/maier*/n*' $TMPFILE > ${TMPFILE}.2
	    if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		sed -e 's*~*/n*' $TMPFILE > ${TMPFILE}.2
	    fi
	fi
	if test -f "`cat ${TMPFILE}.2`" ; then
	    /bin/rm -i "`cat ${TMPFILE}.2`"
	else
	    if test -e "`cat ${TMPFILE}.2`" ; then
		echo 'Error: rmn acts only on normal files.'
	    else
		echo "`cat ${TMPFILE}.2`" does not exist.
	    fi
	fi
	/bin/rm -f $TMPFILE ${TMPFILE}.2
    }
    
    function rmc ()
    {
	TMPFILE=`mktemp -t mirror.XXXXXXXXXX` || exit 1
	echo `cygpath -a "$1"` > $TMPFILE
	sed -e 's*/cygdrive/h/home/maier*/c/maier*' $TMPFILE > \
	    ${TMPFILE}.2
	if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
	    sed -e 's*/home/maier*/c/maier*' $TMPFILE > ${TMPFILE}.2
	    if test -z "`diff $TMPFILE ${TMPFILE}.2`" ; then
		sed -e 's*~*/c/maier*' $TMPFILE > ${TMPFILE}.2
	    fi
	fi
	if test -f "`cat ${TMPFILE}.2`" ; then
	    /bin/rm -i "`cat ${TMPFILE}.2`"
	else
	    if test -e "`cat ${TMPFILE}.2`" ; then
		echo 'Error: rmc acts only on normal files.'
	    else
		echo "`cat ${TMPFILE}.2`" does not exist.
	    fi
	fi
	/bin/rm -f $TMPFILE ${TMPFILE}.2
    }
    
#    function st ()
#    {
#	start `cygpath -w $1`
#	# from http://www.fexx.org/tools/start-en.html
#    }
    alias st=cygstart
    alias nsis='cygstart "$PROGRAMFILES"/NSIS/makensis /PAUSE'
    # More nsis args: /O LogFileName, /V 0...4 verbosity, /CMDHELP usage info.

#    rx ()  
#    {
#	for ((i = 0; $i < ${1}; ++i))
#	    do (/usr/bin/rxvt -rv -cr yellow -sl 2000 -geometry 81x66 \
#		-e /usr/bin/bash & )
#	done
#	# -fn -sony-fixed-medium-r-normal--12-9-10-6-c-81-iso8859-1 
#	# -fn courier
#	# -adobe-courier-medium-o-normal--10-8-75-75-m-60-iso10646-1 too wide
#	# In order of decreasing size:
#	# -misc-fixed-medium-r-semicondensed--0-0-75-75-c-0-iso10646-1
#	# -b&h-luxi mono-medium-r-normal--0-0-0-0-m-0-microsoft-cp1252
#	# lucidasanstypewriter-12
#    }

    alias cdtemp='cd /h/Documents\ and\ Settings/maier.GEO/Local\ Settings/Temp'
    alias fire='cygstart "$BROWSER"'
fi

if [ "${HONAM}" == "impala" ]; then
    rup ()
    {
	echo -n "      "; uptime
	echo -n node01; rsh -n node01 uptime
	echo -n node02; rsh -n node02 uptime
	echo -n node03; rsh -n node03 uptime
	echo -n node04; rsh -n node04 uptime
	echo -n node05; rsh -n node05 uptime
	echo -n node06; rsh -n node06 uptime
	echo -n node07; rsh -n node07 uptime
	echo -n node08; rsh -n node08 uptime
	echo -n node09; rsh -n node09 uptime
	echo -n node10; rsh -n node10 uptime
    }
fi


## The Suns have a problem with the line `for ((i = 0; $i < ${1}; ++i))'
#if [ "${OSTYPE}" == "msys" ]; then
#    rx ()  
#    {
#	for ((i = 0; $i < ${1}; ++i))
#	    do (/usr/bin/rxvt -rv -cr yellow -sl 2000 -geometry 81x66 \
#		-e /usr/bin/sh & )
#	done
#	# -fn -sony-fixed-medium-r-normal--12-9-10-6-c-81-iso8859-1 
#	# -fn courier
#	# -adobe-courier-medium-o-normal--10-8-75-75-m-60-iso10646-1 too wide
#    }
#fi

if test -n "${WINDIR}" ; then
    function a2p2 ()
    {
	a2ps --medium=Letter -2 --output=- ${1} | ps2pdf - > ${1}.pdf
	echo Set \'flip on long edge\' to print ${1}.pdf
	${SYSTEMDRIVE}/Program\ Files/Adobe/Acrobat\ 7.0/Reader/AcroRd32 \
	    ${1}.pdf &
    }
    
    function a2p4 ()
    {
	a2ps --medium=Letter -4 --output=- ${1} | ps2pdf - > ${1}.pdf
	echo Set \'flip on long edge\' to print ${1}.pdf
	${SYSTEMDRIVE}/Program\ Files/Adobe/Acrobat\ 7.0/Reader/AcroRd32 \
	    ${1}.pdf &
    }
    
    alias dos='cmd /c'
    alias doswin='cmd /c CMD E:ON F:ON V:ON /K "PATH=%WINDIR%\system32;%WINDIR%;%WINDIR%\system32\wbem;%PROGRAMFILES%\MATLAB\R2007b\bin\win32"'
    #+: %PROGRAMFILES%\ulead systems\mpeg
    #+: %PROGRAMFILES%\MATLAB Component Runtime\v75\runtime\win32
    alias wman='cygstart http://cygwin.com/docs.html'
fi

alias E='emacs -nw'
alias EH='emacs -nw ${HOME}/HowTo/HowTo.mar'
alias EW='emacs -nw ${HOME}/mail/w'
alias dvi2pdf='dvipdfm -p letter -y 0.25in'
alias h=history
alias j=jobs
alias m=more
alias s=source
alias up=uptime
alias cp='/bin/cp -i'
alias mv='/bin/mv -i'
alias rm='/bin/rm -i'
alias cal='cal -m'
alias md5='openssl dgst -md5'
alias scp='scp -p'
alias null='cat > /dev/null'
alias back='cd -'
alias date="date +'%a %b %d %Y, %H:%M'"
alias cup='/bin/rm -i *~ .*~ *% .*% *.i .\#* \#*\# .\#*\# *.bak *.BAK *.CKP .*.bak .*.BAK .*.CKP *.stackdump'
alias wget='wget -m -k -K -E'
alias in755='install -m 755 -pt'
alias in644='install -m 644 -pt'
alias in700='install -m 700 -pt'
alias in600='install -m 600 -pt'
alias space='df -k /disk/jelly1 /disk/dough1 /disk/dough2 /disk/nut1 /disk/nut2 /home/impala1'
#alias 81='repeat 81 echo -n x ; echo'
alias 81='echo 123456789012345678901234567890123456789012345678901234567890123456789012345678901'
alias xhost_all='xhost +jelly +dough +nut +kona'

alias makensis="'/h/Program Files/NSIS/makensis.exe'"

#alias ls='/bin/ls -AsFkvh --color=$WHEN'
#alias lt='/bin/ls -AsFkvlhrt --color=$WHEN'
#alias ll='/bin/ls -AsFkvlgG --color=$WHEN'
# make sure lsn, ... get the same args.
alias ls='/bin/ls -AsFkvh'
alias lt='/bin/ls -AsFkvhrt1'
alias ll='/bin/ls -AsFvlgG --time-style=long-iso'
alias lll='/bin/ls -AsFvl'
# ls arguments:
#  -b, --escape
#	  print octal escapes for nongraphic characters: \NNN
#  -B, --ignore-backups
#	  do not list implied entries ending with ~
#  -L, --dereference
#	  show file information rather that symbolic link info
#  -X     sort alphabetically by entry extension

alias psg="ps | head -n 1; ps | grep \!*"
if [ "${OSTYPE}" == "cygwin" ]; then
    alias psme="ps | head -n 1; ps | grep $UID"
else
    alias psme="ps | head -n 1; ps | grep $USER"
fi

if test -n "$PROMAX_HOME" ; then
alias gmake='$PROMAX_HOME/sys/bin/gmake -I$PROMAX_HOME/port/include/make'
alias Makeexec='$PROMAX_HOME/port/bin/Makeexec -I$PROMAX_HOME/port/include/make'
fi

# Solaris sets umask as a shell variable to 022 in bash, nothing in csh.
# Linus sets umask to 022 as an environment variable in /etc/csh.login and
# /etc/profile. This is also true for Cygwin now.

alias ps1='ps -afl'
alias ps2='ps -aflW'

# alias whereis='hash -t'	which OS is this for?
if test -n "${WINDIR}" ; then
    # lpr produced no output on our network printers, so ... (need PRINTER var)
    # alias lpr='/h/WINDOWS/system32/lpr -S mars -P Rainier'	(obsolete)
    # Add -o l (small L) to print binary files, eg. (binary only?) postscript
    alias lprn="`cygpath -a ${SYSTEMDRIVE}`/WINDOWS/system32/notepad /p"
    # lprn prints 63 lines / page.
    # For lprn I have the following settings in notepad:
    # File / Page Setup / Header = &f --- &d &t
    # The footer value is at its default: Page &p
    # lprn prints 63 lines / page.
    alias word='st WINWORD'
    alias calc='st calc'
fi

function ssx ()
{
    xhost + $2; ssh -Y -c 3des -e none $1'@'$2
    # -Y is supposed to be safer than -X but on Cygwin gives
    # Warning: No xauth data; using fake authentication data for X11 forwarding.
}


if [ "${OSTYPE}" == "cygwin" ]; then
    alias startx='startx -- :0.0 -clipboard -rootless -scrollbars -multiplemonitors'
    alias start1='startx -- :0.0 -clipboard -rootless -scrollbars'
    # Setup of X11:
    # 0) wmaker.inst (at install time only; no longer needed,
    #	 used to give a nicer X env.)
    # 1) startx; 2) xhost Target; 3) ssh -X Target;
    # either 4) export DISPLAY=OrigMach:0.0	(should not be needed with -X)
    #	  or 4) setenv DISPLAY OrigMach:0.0	(should not be needed with -X)
    # -X should make steps 2 and 4 unnecessary.
    # -Y does the same thing as -X but enables trusted X11Forwarding
    # -scrollbars does not work/is ignored on some versions of Cygwin.
    # -emulate3buttons Tout_in_ms
    alias rxvt1='/bin/rxvt -rv -cr yellow -sl 2000 -geometry 81x45 -e /bin/bash &'
    alias con='/bin/rxvt -rv -cr yellow -ic -C -n Console -title Console -e /bin/bash &'
    alias lock='rundll32.exe user32.dll, LockWorkStation'
    alias susp='cygstart $WINDIR/System32/rundll32.exe powrprof.dll,SetSuspendState'
    # Powercfg.cpl says whether the system will standby, hybernate, or shut off.
fi
if [ "${OSTYPE}" == "msys" ]; then
    alias clear=clsb
    alias defragment='dfrg.msc'
    alias rxvt1='/bin/rxvt -rv -cr yellow -sl 2000 -geometry 81x45 &'
    alias con='/bin/rxvt -rv -cr yellow -ic -C -n Console -title Console &'
fi
if test -d /usr/bin/vncserver ; then
    alias startvnc="/usr/bin/vncserver -geometry 1010x700 -depth 16"
fi
# alias w4='for ((i=0;$i < 4; ++i)); do (/usr/bin/rxvt -rv -cr yellow -sl 2000 -geometry 81x45 -e /usr/bin/bash & ); done'
