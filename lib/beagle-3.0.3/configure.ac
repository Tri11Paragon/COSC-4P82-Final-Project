dnl Process this file with autoconf to produce a configure script.
AC_INIT([Open BEAGLE], 3.0.3,[cgagne@gmail.com],[beagle])
AC_CONFIG_SRCDIR([beagle/src/Allocator.cpp])
AC_CONFIG_AUX_DIR([config])
AC_REVISION([$Revision: 1.33.2.7 $])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config/config-h:config/config-h.in])
AX_PREFIX_CONFIG_H([beagle/include/beagle/config.hpp],[BEAGLE],[config/config-h])

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_LIBTOOL

dnl Set language.
AC_LANG([C++])

dnl Open BEAGLE optimization mode option.
AC_ARG_ENABLE(optimization,
  [  --enable-optimization    enable optimization mode [default=no]],
  [case $enableval in
  yes) enable_optimization=yes;;
  no) enable_optimization=no;;
  *) enable_optimization=no;; esac],
  enable_optimization=no)
if test "$enable_optimization" = yes; then
  AC_DEFINE(NDEBUG,,[define if some debug code is disabled])
fi

dnl Open BEAGLE full debug mode option.
AC_ARG_ENABLE(full-debug,
  [  --enable-full-debug      enable full debug mode [default=no]],
  [case $enableval in
  yes) enable_full_debug=yes;;
  no) enable_full_debug=no;;
  *) enable_full_debug=no;; esac],
  enable_full_debug=no)
if test "$enable_full_debug" = yes; then
  AC_DEFINE(FULL_DEBUG,,[define if full debug mode is active])
fi

dnl Open BEAGLE fast compilation mode option.
AC_ARG_ENABLE(fast-compilation,
  [  --enable-fast-compilation [=no/yes] enable fast compilation mode [default=yes]],, enable_fast_compilation="yes")
if test "$enable_fast_compilation" = "yes"; then
  AC_DEFINE(FAST_COMPILATION,,[define if fast compilation mode is active])
fi
AM_CONDITIONAL(FAST_COMPILATION, test "$enable_fast_compilation" = "yes")

dnl Open BEAGLE tests compilation mode option.
AC_ARG_ENABLE(tests,
  [  --enable-tests [=no/yes] enable compilation of tests [default=no]],, enable_tests="no")
if test "$enable_tests" = "yes"; then
  AC_DEFINE(COMPILE_TESTS,,[define if compilation of tests is active])
fi
AM_CONDITIONAL(COMPILE_TESTS, test "$enable_tests" = "yes")

dnl Checks for libraries.
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_CXX_BOOL
OB_CXX_LONG_LONG
OB_CXX_HAVE_ISFINITE
AC_CXX_TEMPLATES
AC_CXX_TYPENAME
AC_CXX_TEMPLATE_SCOPED_ARGUMENT_MATCHING
AC_CXX_DEFAULT_TEMPLATE_PARAMETERS
AC_CXX_TEMPLATE_QUALIFIED_BASE_CLASS
AC_CXX_TEMPLATE_QUALIFIED_RETURN_TYPE
AC_CXX_CONST_CAST
AC_CXX_REINTERPRET_CAST
AC_CXX_STATIC_CAST
AC_CXX_DYNAMIC_CAST
AC_CXX_EXCEPTIONS
AC_CXX_EXPLICIT
AC_CXX_NAMESPACES
AC_CXX_RTTI
AC_CXX_HAVE_STD
AC_CXX_HAVE_STL
AC_CXX_HAVE_NUMERIC_LIMITS
AC_CXX_HAVE_SSTREAM

dnl Checks for hash_map and hash_set
AC_ARG_ENABLE(hashset,
  [  --enable-hashset [=no/yes] enable hash_set STL extension [default=no]],, enable_hashset="no")
if test "$enable_hashset" = "yes"; then
  AC_CXX_STD_HASHSET
  AC_CXX_GNUCXX_HASHSET
  AC_CXX_STDEXT_HASHSET
fi
AC_ARG_ENABLE(hashmap,
  [  --enable-hashmap [=no/yes] enable hash_map STL extension [default=no]],, enable_hashmap="no")
if test "$enable_hashmap" = "yes"; then
  AC_CXX_STD_HASHMAP
  AC_CXX_GNUCXX_HASHMAP
  AC_CXX_STDEXT_HASHMAP
fi

dnl Check for file /dev/urandom
AC_CHECK_FILE([/dev/urandom],[AC_DEFINE([HAVE_DEV_URANDOM],[],[define if file /dev/urandom exists.])])

dnl Check for POSIX threads support.
ACX_PTHREAD
LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"

dnl Check for zlib support.
CHECK_ZLIB

dnl Stop configure process if some C++ essentials are not present.
if test "$ac_cv_cxx_templates" = no; then
  AC_MSG_ERROR([["sorry, templates support needed"]])
fi
if test "$ac_cv_cxx_template_scoped_argument_matching" = no; then
  AC_MSG_ERROR([["sorry, template scoped argument matching support needed"]])
fi
if test "$ac_cv_cxx_default_template_parameters" = no; then
  AC_MSG_ERROR([["sorry, default template parameters support needed"]])
fi
if test "$ac_cv_cxx_typename" = no; then
  AC_MSG_ERROR([["sorry, typename qualifier needed"]])
fi
if test "$ac_cv_cxx_exceptions" = no; then
  AC_MSG_ERROR([["sorry, support for exception handling needed"]])
fi
if test "$ac_cv_cxx_namespaces" = no; then
  AC_MSG_ERROR([["sorry, namespaces support needed"]])
fi
if test "$ac_cv_cxx_have_std" = no; then
  AC_MSG_ERROR([["sorry, namespace std needed"]])
fi
if test "$ac_cv_cxx_have_stl" = no; then
  AC_MSG_ERROR([["sorry, no standard template library found"]])
fi
if test "$ac_cv_cxx_have_sstream" = no; then
  AC_MSG_ERROR([["sorry, no sstream found. If you are using gcc less or equal to 2.95.2, see http://gcc.gnu.org/faq.html#2.95sstream to patch your compiler"]])
fi

dnl Specific variables definition for PACC
PACC_VERSION='1.3.1b'
AC_SUBST(PACC_VERSION)
if test "$zlib_cv_libz" = yes; then
  CPPFLAGS="$CPPFLAGS -DPACC_ZLIB"
fi

dnl Create makefiles.
AC_CONFIG_FILES([Makefile
                 PACC/Makefile
                 PACC/Math/Makefile
                 PACC/SVG/Makefile
                 PACC/Socket/Makefile
                 PACC/Threading/Makefile
                 PACC/Util/Makefile
                 PACC/XML/Makefile
                 beagle/Makefile
                 beagle/include/Makefile
                 beagle/include/beagle/Makefile
                 beagle/src/Makefile
                 beagle/GA/Makefile
                 beagle/GA/include/Makefile
                 beagle/GA/include/beagle/Makefile
                 beagle/GA/include/beagle/GA/Makefile
                 beagle/GA/src/Makefile
                 beagle/GP/Makefile
                 beagle/GP/include/Makefile
                 beagle/GP/include/beagle/Makefile
                 beagle/GP/include/beagle/GP/Makefile
                 beagle/GP/src/Makefile
                 beagle/Coev/Makefile
                 beagle/Coev/include/Makefile
                 beagle/Coev/include/beagle/Makefile
                 beagle/Coev/include/beagle/Coev/Makefile
                 beagle/Coev/src/Makefile
                 tests/Makefile
                 tests/Common/Makefile
                 tests/Exception/Makefile
                 tests/GA/Makefile
                 tests/GA/optfct/Makefile
                 tests/GP/Makefile
                 tests/GP/FitnessTestGPIndividual/Makefile
                 tests/GP/ReadGPIndividual/Makefile
                 tests/GP/WriteRandomGPIndividual/Makefile
                 tests/LinGP/Makefile
                 tests/LinGP/include/Makefile
                 tests/LinGP/include/beagle/Makefile
                 tests/LinGP/include/beagle/LinGP/Makefile
                 tests/LinGP/src/Makefile
                 tests/Roulette/Makefile
                 tests/Thread/Makefile
                 tests/TransportTCPIPOp/Makefile
])
AC_OUTPUT
