dnl
dnl Initialize an autoconf project that use PACC::Math.
dnl
dnl  by Christian Gagne, 09/08/2005
dnl
dnl PACC_MATH_INIT(PACC_MATH_PATH
dnl                [, PACC_MATH_INCLUDEPATH]
dnl                [, PACC_MATH_LIBPATH])
dnl
AC_DEFUN([PACC_MATH_INIT],
[
dnl Set headers and libraries path correctly
AC_ARG_WITH(pacc-math,
  [  --with-pacc-math=DIR       specify installation path of PACC::Math],
  [pacc_math_path="$withval"],
  [pacc_math_path="$1"])
AC_ARG_WITH(pacc-math-include,
  [  --with-pacc-math-include=DIR specify exact dir for PACC::Math headers],
  [pacc_math_path_include="$withval"])
AC_ARG_WITH(pacc-math-libdir,
  [  --with-pacc-math-libdir=DIR specify exact dir for PACC::Math libraries],
  [pacc_math_path_libdir="$withval"])
if test -z "$pacc_math_path"; then
  pacc_math_path="$1"
fi
if test -z "$pacc_math_path_include"; then
  if test -z "$2"; then
    pacc_math_path_include="$pacc_math_path"/include
  else
    pacc_math_path_include="$2"
  fi
fi
if test -z "$pacc_math_path_libdir"; then
  if test -z "$3"; then
    pacc_math_path_libdir="$pacc_math_path"/lib
  else
    pacc_math_path_libdir="$3"
  fi
fi
CPPFLAGS="$CPPFLAGS -I$pacc_math_path_include"
LIBS="$pacc_math_path_libdir/libpacc-math.la $LIBS"
dnl Finally, do a little compilation test.
AC_CACHE_CHECK(if PACC::Math headers are correctly included,
pacc_math_include_test,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_CPP([Math.hpp],
            pacc_math_include_test=yes,
            pacc_math_include_test=no)
 AC_LANG_RESTORE
])
])



dnl
dnl Initialize an autoconf project that use PACC::Threading.
dnl
dnl  by Christian Gagne, 09/08/2005
dnl
dnl PACC_THREADING_INIT(PACC_THREADING_PATH
dnl                     [, PACC_THREADING_INCLUDEPATH]
dnl                     [, PACC_THREADING_LIBPATH])
dnl
AC_DEFUN([PACC_THREADING_INIT],
[
dnl Set headers and libraries path correctly
AC_ARG_WITH(pacc-threading,
  [  --with-pacc-threading=DIR       specify installation path of PACC::Threading],
  [pacc_threading_path="$withval"],
  [pacc_threading_path="$1"])
AC_ARG_WITH(pacc-threading-include,
  [  --with-pacc-threading-include=DIR specify exact dir for PACC::Threading headers],
  [pacc_threading_path_include="$withval"])
AC_ARG_WITH(pacc-threading-libdir,
  [  --with-pacc-threading-libdir=DIR specify exact dir for PACC::Threading libraries],
  [pacc_threading_path_libdir="$withval"])
if test -z "$pacc_threading_path"; then
  pacc_threading_path="$1"
fi
if test -z "$pacc_threading_path_include"; then
  if test -z "$2"; then
    pacc_threading_path_include="$pacc_threading_path"/include
  else
    pacc_threading_path_include="$2"
  fi
fi
if test -z "$pacc_threading_path_libdir"; then
  if test -z "$3"; then
    pacc_threading_path_libdir="$pacc_threading_path"/lib
  else
    pacc_threading_path_libdir="$3"
  fi
fi
CPPFLAGS="$CPPFLAGS -I$pacc_threading_path_include"
LIBS="$pacc_threading_path_libdir/libpacc-threading.la $LIBS"
dnl Finally, do a little compilation test.
AC_CACHE_CHECK(if PACC::Threading headers are correctly included,
pacc_threading_include_test,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_CPP([Threading.hpp],
            pacc_threading_include_test=yes,
            pacc_threading_include_test=no)
 AC_LANG_RESTORE
])
])



dnl
dnl Initialize an autoconf project that use PACC::Util.
dnl
dnl  by Christian Gagne, 09/08/2005
dnl
dnl PACC_UTIL_INIT(PACC_UTIL_PATH
dnl                [, PACC_UTIL_INCLUDEPATH]
dnl                [, PACC_UTIL_LIBPATH])
dnl
AC_DEFUN([PACC_UTIL_INIT],
[
dnl Set headers and libraries path correctly
AC_ARG_WITH(pacc-util,
  [  --with-pacc-util=DIR       specify installation path of PACC::Util],
  [pacc_util_path="$withval"],
  [pacc_util_path="$1"])
AC_ARG_WITH(pacc-util-include,
  [  --with-pacc-util-include=DIR specify exact dir for PACC::Util headers],
  [pacc_util_path_include="$withval"])
AC_ARG_WITH(pacc-util-libdir,
  [  --with-pacc-util-libdir=DIR specify exact dir for PACC::Util libraries],
  [pacc_util_path_libdir="$withval"])
if test -z "$pacc_util_path"; then
  pacc_util_path="$1"
fi
if test -z "$pacc_util_path_include"; then
  if test -z "$2"; then
    pacc_util_path_include="$pacc_util_path"/include
  else
    pacc_util_path_include="$2"
  fi
fi
if test -z "$pacc_util_path_libdir"; then
  if test -z "$3"; then
    pacc_util_path_libdir="$pacc_util_path"/lib
  else
    pacc_util_path_libdir="$3"
  fi
fi
CPPFLAGS="$CPPFLAGS -I$pacc_util_path_include"
LIBS="$pacc_util_path_libdir/libpacc-util.la $LIBS"
dnl Finally, do a little compilation test.
AC_CACHE_CHECK(if PACC::Util headers are correctly included,
pacc_util_include_test,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_CPP([Util.hpp],
            pacc_util_include_test=yes,
            pacc_util_include_test=no)
 AC_LANG_RESTORE
])
])



dnl
dnl Initialize an autoconf project that use PACC::XML.
dnl
dnl  by Christian Gagne, 09/08/2005
dnl
dnl PACC_XML_INIT(PACC_XML_PATH
dnl               [, PACC_XML_INCLUDEPATH]
dnl               [, PACC_XML_LIBPATH])
dnl
AC_DEFUN([PACC_XML_INIT],
[
dnl Set headers and libraries path correctly
AC_ARG_WITH(pacc-xml,
  [  --with-pacc-xml=DIR       specify installation path of PACC::XML],
  [pacc_xml_path="$withval"],
  [pacc_xml_path="$1"])
AC_ARG_WITH(pacc-xml-include,
  [  --with-pacc-xml-include=DIR specify exact dir for PACC::XML headers],
  [pacc_xml_path_include="$withval"])
AC_ARG_WITH(pacc-xml-libdir,
  [  --with-pacc-xml-libdir=DIR specify exact dir for PACC::XML libraries],
  [pacc_xml_path_libdir="$withval"])
if test -z "$pacc_xml_path"; then
  pacc_xml_path="$1"
fi
if test -z "$pacc_xml_path_include"; then
  if test -z "$2"; then
    pacc_xml_path_include="$pacc_xml_path"/include
  else
    pacc_xml_path_include="$2"
  fi
fi
if test -z "$pacc_xml_path_libdir"; then
  if test -z "$3"; then
    pacc_xml_path_libdir="$pacc_xml_path"/lib
  else
    pacc_xml_path_libdir="$3"
  fi
fi
CPPFLAGS="$CPPFLAGS -I$pacc_xml_path_include"
LIBS="$pacc_xml_path_libdir/libpacc-xml.la $LIBS"
dnl Finally, do a little compilation test.
AC_CACHE_CHECK(if PACC::XML headers are correctly included,
pacc_xml_include_test,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_CPP([XML.hpp],
            pacc_xml_include_test=yes,
            pacc_xml_include_test=no)
 AC_LANG_RESTORE
])
])



dnl
dnl Initialize an autoconf project that use Open BEAGLE.
dnl
dnl  by Christian Gagne, 14/10/2002, modified 10/10/2004 and 09/08/2005
dnl
dnl OB_BEAGLE_INIT(BEAGLE_PATH
dnl                [, BEAGLE_INCLUDEPATH]
dnl                [, BEAGLE_LIBPATH]
dnl                [, BEAGLE_GA_INCLUDEPATH]
dnl                [, BEAGLE_GA_LIBPATH]
dnl                [, BEAGLE_GP_INCLUDEPATH]
dnl                [, BEAGLE_GP_LIBPATH]
dnl                [, BEAGLE_COEV_INCLUDEPATH]
dnl                [, BEAGLE_COEV_LIBPATH])
dnl
AC_DEFUN([OB_BEAGLE_INIT],
[
dnl Open BEAGLE optimization mode option.
AC_ARG_ENABLE(optimization,
  [  --enable-optimization    enable optimization mode [default=no]],
  [case $enableval in
  yes) enable_optimization=yes;;
  no) enable_optimization=no;;
  *) enable_optimization=no;; esac],
  enable_optimization=no)
if test "$enable_optimization" = yes; then
  CFLAGS=`echo $CFLAGS | sed 's/\(?*\)-g\(?*\)/\1\2/'`
  CXXFLAGS=`echo $CXXFLAGS | sed 's/\(?*\)-g\(?*\)/\1\2/'`
else
  CFLAGS=`echo $CFLAGS | sed 's/\(?*\)-O[[0-9]]\(?*\)/\1\2/'`
  CXXFLAGS=`echo $CXXFLAGS | sed 's/\(?*\)-O[[0-9]]\(?*\)/\1\2/'`
fi
dnl Set headers and libraries path correctly
AC_ARG_WITH(beagle,
  [  --with-beagle=DIR       specify installation path of Open BEAGLE],
  [ob_beagle_path="$withval"],
  [ob_beagle_path="$1"])
AC_ARG_WITH(beagle-include,
  [  --with-beagle-include=DIR specify exact dir for Open BEAGLE headers],
  [ob_beagle_path_include="$withval"])
AC_ARG_WITH(beagle-GA-include,
  [  --with-beagle-GA-include=DIR specify exact dir for Open BEAGLE GA headers],
  [ob_beagle_path_ga_include="$withval"])
AC_ARG_WITH(beagle-GP-include,
  [  --with-beagle-GP-include=DIR specify exact dir for Open BEAGLE GP headers],
  [ob_beagle_path_gp_include="$withval"])
AC_ARG_WITH(beagle-Coev-include,
  [  --with-beagle-Coev-include=DIR specify exact dir for Open BEAGLE co-evolution headers],
  [ob_beagle_path_coev_include="$withval"])
AC_ARG_WITH(beagle-libdir,
  [  --with-beagle-libdir=DIR specify exact dir for Open BEAGLE libraries],
  [ob_beagle_path_libdir="$withval"])
AC_ARG_WITH(beagle-GA-libdir,
  [  --with-beagle-GA-libdir=DIR specify exact dir for Open BEAGLE GA libraries],
  [ob_beagle_path_ga_libdir="$withval"])
AC_ARG_WITH(beagle-GP-libdir,
  [  --with-beagle-GP-libdir=DIR specify exact dir for Open BEAGLE GP libraries],
  [ob_beagle_path_gp_libdir="$withval"])
AC_ARG_WITH(beagle-Coev-libdir,
  [  --with-beagle-Coev-libdir=DIR specify exact dir for Open BEAGLE co-evolution libraries],
  [ob_beagle_path_coev_libdir="$withval"])
if test -z "$ob_beagle_path"; then
  ob_beagle_path="$1"
fi
if test -z "$ob_beagle_path_include"; then
  if test -z "$2"; then
    ob_beagle_path_include="$ob_beagle_path"/include
  else
    ob_beagle_path_include="$2"
  fi
fi
if test -z "$ob_beagle_path_libdir"; then
  if test -z "$3"; then
    ob_beagle_path_libdir="$ob_beagle_path"/lib
  else
    ob_beagle_path_libdir="$3"
  fi
fi
if test -z "$ob_beagle_path_ga_include"; then
  if test -z "$4"; then
    ob_beagle_path_ga_include=$ob_beagle_path_include
  else
    ob_beagle_path_ga_include="$4"
  fi
fi
if test -z "$ob_beagle_path_ga_libdir"; then
  if test -z "$5"; then
    ob_beagle_path_ga_libdir=$ob_beagle_path_libdir
  else
    ob_beagle_path_ga_libdir="$5"
  fi
fi
if test -z "$ob_beagle_path_gp_include"; then
  if test -z "$6"; then
    ob_beagle_path_gp_include=$ob_beagle_path_include
  else
    ob_beagle_path_gp_include="$6"
  fi
fi
if test -z "$ob_beagle_path_gp_libdir"; then
  if test -z "$7"; then
    ob_beagle_path_gp_libdir=$ob_beagle_path_libdir
  else
    ob_beagle_path_gp_libdir="$7"
  fi
fi
if test -z "$ob_beagle_path_coev_include"; then
  if test -z "$8"; then
    ob_beagle_path_coev_include=$ob_beagle_path_include
  else
    ob_beagle_path_coev_include="$8"
  fi
fi
if test -z "$ob_beagle_path_coev_libdir"; then
  if test -z "$9"; then
    ob_beagle_path_coev_libdir=$ob_beagle_path_libdir
  else
    ob_beagle_path_coev_libdir="$9"
  fi
fi
CPPFLAGS="$CPPFLAGS -I$ob_beagle_path_include -I$ob_beagle_path_ga_include -I$ob_beagle_path_gp_include -I$ob_beagle_path_coev_include"
LIBS="$ob_beagle_path_coev_libdir/libbeagle-Coev.la $ob_beagle_path_gp_libdir/libbeagle-GP.la $ob_beagle_path_ga_libdir/libbeagle-GA.la $ob_beagle_path_libdir/libbeagle.la $LIBS"
dnl Finally, do a little compilation test.
AC_CACHE_CHECK(if Open BEAGLE headers are correctly included,
ob_beagle_include_test,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_CPP([beagle/beagle.hpp beagle/GA.hpp beagle/GP.hpp beagle/Coev.hpp],
            ob_beagle_include_test=yes,
            ob_beagle_include_test=no)
 AC_LANG_RESTORE
])
])



dnl ***************************************************************************
dnl ***************************************************************************
AC_DEFUN([ACX_PTHREAD], [
AC_REQUIRE([AC_CANONICAL_HOST])
AC_LANG_SAVE
AC_LANG_C
acx_pthread_ok=no

# We used to check for pthread.h first, but this fails if pthread.h
# requires special compiler flags (e.g. on True64 or Sequent).
# It gets checked for in the link test anyway.

# First of all, check if the user has set any of the PTHREAD_LIBS,
# etcetera environment variables, and if threads linking works using
# them:
if test x"$PTHREAD_LIBS$PTHREAD_CFLAGS" != x; then
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        AC_MSG_CHECKING([for pthread_join in LIBS=$PTHREAD_LIBS with CFLAGS=$PTHREAD_CFLAGS])
        AC_TRY_LINK_FUNC(pthread_join, acx_pthread_ok=yes)
        AC_MSG_RESULT($acx_pthread_ok)
        if test x"$acx_pthread_ok" = xno; then
                PTHREAD_LIBS=""
                PTHREAD_CFLAGS=""
        fi
        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi

# We must check for the threads library under a number of different
# names; the ordering is very important because some systems
# (e.g. DEC) have both -lpthread and -lpthreads, where one of the
# libraries is broken (non-POSIX).

# Create a list of thread flags to try.  Items starting with a "-" are
# C compiler flags, and other items are library names, except for "none"
# which indicates that we try without any flags at all.

acx_pthread_flags="pthreads none -Kthread -kthread lthread -pthread -pthreads -mthreads pthread --thread-safe -mt"

# The ordering *is* (sometimes) important.  Some notes on the
# individual items follow:

# pthreads: AIX (must check this before -lpthread)
# none: in case threads are in libc; should be tried before -Kthread and
#       other compiler flags to prevent continual compiler warnings
# -Kthread: Sequent (threads in libc, but -Kthread needed for pthread.h)
# -kthread: FreeBSD kernel threads (preferred to -pthread since SMP-able)
# lthread: LinuxThreads port on FreeBSD (also preferred to -pthread)
# -pthread: Linux/gcc (kernel threads), BSD/gcc (userland threads)
# -pthreads: Solaris/gcc
# -mthreads: Mingw32/gcc, Lynx/gcc
# -mt: Sun Workshop C (may only link SunOS threads [-lthread], but it
#      doesn't hurt to check since this sometimes defines pthreads too;
#      also defines -D_REENTRANT)
# pthread: Linux, etcetera
# --thread-safe: KAI C++

case "${host_cpu}-${host_os}" in
        *solaris*)

        # On Solaris (at least, for some versions), libc contains stubbed
        # (non-functional) versions of the pthreads routines, so link-based
        # tests will erroneously succeed.  (We need to link with -pthread or
        # -lpthread.)  (The stubs are missing pthread_cleanup_push, or rather
        # a function called by this macro, so we could check for that, but
        # who knows whether they'll stub that too in a future libc.)  So,
        # we'll just look for -pthreads and -lpthread first:

        acx_pthread_flags="-pthread -pthreads pthread -mt $acx_pthread_flags"
        ;;
esac

if test x"$acx_pthread_ok" = xno; then
for flag in $acx_pthread_flags; do

        case $flag in
                none)
                AC_MSG_CHECKING([whether pthreads work without any flags])
                ;;

                -*)
                AC_MSG_CHECKING([whether pthreads work with $flag])
                PTHREAD_CFLAGS="$flag"
                ;;

                *)
                AC_MSG_CHECKING([for the pthreads library -l$flag])
                PTHREAD_LIBS="-l$flag"
                ;;
        esac

        save_LIBS="$LIBS"
        save_CFLAGS="$CFLAGS"
        LIBS="$PTHREAD_LIBS $LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Check for various functions.  We must include pthread.h,
        # since some functions may be macros.  (On the Sequent, we
        # need a special flag -Kthread to make this header compile.)
        # We check for pthread_join because it is in -lpthread on IRIX
        # while pthread_create is in libc.  We check for pthread_attr_init
        # due to DEC craziness with -lpthreads.  We check for
        # pthread_cleanup_push because it is one of the few pthread
        # functions on Solaris that doesn't have a non-functional libc stub.
        # We try pthread_create on general principles.
        AC_TRY_LINK([#include <pthread.h>],
                    [pthread_t th; pthread_join(th, 0);
                     pthread_attr_init(0); pthread_cleanup_push(0, 0);
                     pthread_create(0,0,0,0); pthread_cleanup_pop(0); ],
                    [acx_pthread_ok=yes])

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        AC_MSG_RESULT($acx_pthread_ok)
        if test "x$acx_pthread_ok" = xyes; then
                break;
        fi

        PTHREAD_LIBS=""
        PTHREAD_CFLAGS=""
done
fi

# Various other checks:
if test "x$acx_pthread_ok" = xyes; then
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Detect AIX lossage: threads are created detached by default
        # and the JOINABLE attribute has a nonstandard name (UNDETACHED).
        AC_MSG_CHECKING([for joinable pthread attribute])
        AC_TRY_LINK([#include <pthread.h>],
                    [int attr=PTHREAD_CREATE_JOINABLE;],
                    ok=PTHREAD_CREATE_JOINABLE, ok=unknown)
        if test x"$ok" = xunknown; then
                AC_TRY_LINK([#include <pthread.h>],
                            [int attr=PTHREAD_CREATE_UNDETACHED;],
                            ok=PTHREAD_CREATE_UNDETACHED, ok=unknown)
        fi
        if test x"$ok" != xPTHREAD_CREATE_JOINABLE; then
                AC_DEFINE(PTHREAD_CREATE_JOINABLE, $ok,
                          [Define to the necessary symbol if this constant
                           uses a non-standard name on your system.])
        fi
        AC_MSG_RESULT(${ok})
        if test x"$ok" = xunknown; then
                AC_MSG_WARN([we do not know how to create joinable pthreads])
        fi

        AC_MSG_CHECKING([if more special flags are required for pthreads])
        flag=no
        case "${host_cpu}-${host_os}" in
                *-aix* | *-freebsd*)     flag="-D_THREAD_SAFE";;
                *solaris* | *-osf* | *-hpux*) flag="-D_REENTRANT";;
        esac
        AC_MSG_RESULT(${flag})
        if test "x$flag" != xno; then
                PTHREAD_CFLAGS="$flag $PTHREAD_CFLAGS"
        fi

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        # More AIX lossage: must compile with cc_r
        AC_CHECK_PROG(PTHREAD_CC, cc_r, cc_r, ${CC})
else
        PTHREAD_CC="$CC"
fi

AC_SUBST(PTHREAD_LIBS)
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_CC)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_pthread_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_PTHREAD,1,[Define if you have POSIX threads libraries and header files.]),[$1])
        :
else
        acx_pthread_ok=no
        $2
fi
AC_LANG_RESTORE
])dnl ACX_PTHREAD



AC_DEFUN([CHECK_ZLIB],
#
# Handle user hints
#
[AC_MSG_CHECKING(if zlib is wanted)
AC_ARG_WITH(zlib,
[  --with-zlib=DIR root directory path of zlib installation [defaults to
                    /usr/local or /usr if not found in /usr/local]
  --without-zlib to disable zlib usage completely],
[if test "$withval" != no ; then
  AC_MSG_RESULT(yes)
  ZLIB_HOME="$withval"
else
  AC_MSG_RESULT(no)
fi], [
AC_MSG_RESULT(yes)
ZLIB_HOME=/usr/local
if test ! -f "${ZLIB_HOME}/include/zlib.h"
then
        ZLIB_HOME=/usr
fi
])
#
# Locate zlib, if wanted
#
if test -n "${ZLIB_HOME}"
then
        ZLIB_OLD_LDFLAGS=$LDFLAGS
        ZLIB_OLD_CPPFLAGS=$LDFLAGS
        LDFLAGS="$LDFLAGS -L${ZLIB_HOME}/lib"
        CPPFLAGS="$CPPFLAGS -I${ZLIB_HOME}/include"
        AC_LANG_SAVE
        AC_LANG_C
        AC_CHECK_LIB(z, inflateEnd, [zlib_cv_libz=yes], [zlib_cv_libz=no])
        AC_CHECK_HEADER(zlib.h, [zlib_cv_zlib_h=yes], [zlib_cv_zlib_h=no])
        AC_LANG_RESTORE
        if test "$zlib_cv_libz" = "yes" -a "$zlib_cv_zlib_h" = "yes"
        then
                #
                # If both library and header were found, use them
                #
                AC_CHECK_LIB(z, inflateEnd)
                AC_MSG_CHECKING(zlib in ${ZLIB_HOME})
                AC_MSG_RESULT(ok)
        else
                #
                # If either header or library was not found, revert and bomb
                #
                AC_MSG_CHECKING(zlib in ${ZLIB_HOME})
                LDFLAGS="$ZLIB_OLD_LDFLAGS"
                CPPFLAGS="$ZLIB_OLD_CPPFLAGS"
                AC_MSG_RESULT(failed)
        fi
fi
])
