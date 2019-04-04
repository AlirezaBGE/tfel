AC_DEFUN([CHECK_CASTEM_UNIX_TYPE],[
    AC_REQUIRE([AC_CANONICAL_HOST])dnl
    AC_CHECKING("castem unix type")
    if test "x$TFEL_WIN32" = "xyes";
    then
	CASTEM_UNIX_TYPE=WIN32
	CASTEM_CPPFLAGS="-DWIN32"
    elif test "x$TFEL_WIN64" = "xyes";
    then
	CASTEM_UNIX_TYPE=WIN64
	CASTEM_CPPFLAGS="-DWIN64"
    elif test "x$TFEL_ARCH32" = "xyes" ;
    then
	if test "x$TFEL_LINUX" = "xyes";
	then
	    CASTEM_UNIX_TYPE=UNIX32
	    CASTEM_CPPFLAGS="-DLINUX32 -DTHREAD"
	    CASTEM_FFLAGS=""
	    CASTEM_CFLAGS=""
	else
	    CASTEM_UNIX_TYPE=UNIX32
	    CASTEM_CPPFLAGS="-DSUN -DUNIX32 -DTHREAD"
	    CASTEM_FFLAGS=""
	    CASTEM_CFLAGS=""
	fi
    else
	if test "x$TFEL_LINUX" = "xyes";
	then
	    CASTEM_UNIX_TYPE=UNIX64
	    CASTEM_CPPFLAGS="-DLINUX64 -DUNIX64 -DTHREAD"
	    CASTEM_FFLAGS=""
	    CASTEM_CFLAGS=""
	else
	    CASTEM_UNIX_TYPE=UNIX64
	    CASTEM_CPPFLAGS="-DSUN -DUNIX64 -DTHREAD"
	    CASTEM_FFLAGS=""
	    CASTEM_CFLAGS=""
	fi
    fi
    AC_SUBST(CASTEM_FFLAGS)
    AC_SUBST(CASTEM_CFLAGS)
    AC_SUBST(CASTEM_CPPFLAGS)
    AC_SUBST(CASTEM_UNIX_TYPE)
    AC_DEFINE_UNQUOTED(CASTEM_CPPFLAGS,"$CASTEM_CPPFLAGS")
    CPPFLAGS="$CASTEM_CPPFLAGS $CPPFLAGS"
])