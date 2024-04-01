dnl Process this file with autoconf to produce a configure script.
AC_INIT([Co-evolved GP symbolic regression example],[3.0.0],[cgagne@gmail.com],[coev_symbreg])
AC_CONFIG_SRCDIR([coev_symbreg/CoSymEvalOp.cpp])
AC_CONFIG_AUX_DIR([config])
AC_REVISION([$Revision: 1.4 $])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([coev_symbreg/config.hpp:coev_symbreg/config.hpp.in])

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_LN_S
AC_PROG_LIBTOOL

dnl Set language.
AC_LANG([C++])

dnl Checks for libraries.
AC_HEADER_STDC

dnl Check for zlib support.
CHECK_ZLIB

dnl Check for POSIX threads support.
ACX_PTHREAD
LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"

dnl Initialize PACC libraries with the given paths.
PACC_UTIL_INIT([../../../../PACC], [../../../../PACC], [../../../../PACC/Util])
PACC_XML_INIT([../../../../PACC], [../../../../PACC], [../../../../PACC/XML])
PACC_MATH_INIT([../../../../PACC], [../../../../PACC], [../../../../PACC/Math])
PACC_THREADING_INIT([../../../../PACC], [../../../../PACC], [../../../../PACC/Threading])

dnl Initialize Open BEAGLE with the given paths.
OB_BEAGLE_INIT([../../../../beagle],
               [../../../../beagle/include],
               [../../../../beagle/src],
               [../../../../beagle/GA/include],
               [../../../../beagle/GA/src],
               [../../../../beagle/GP/include],
               [../../../../beagle/GP/src],
               [../../../../beagle/Coev/include],
               [../../../../beagle/Coev/src])

dnl Create makefiles.
AC_CONFIG_FILES([Makefile coev_symbreg/Makefile])
AC_OUTPUT
