dnl
dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_cxx_have_ext_hash_set.html
dnl
dnl Modified by Christian Gagne, August 01, 2007
dnl Now check for availability in namespaces stdext (MSVC++) and __gnu_cxx (GCC 3.1-4)
dnl

AC_DEFUN([AC_CXX_STD_HASHSET],[
AC_CACHE_CHECK(whether the compiler supports std::hash_set,
ac_cv_cxx_std_hashset,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <hash_set>
using std::hash_set;],
 [],
 ac_cv_cxx_std_hashset=yes, ac_cv_cxx_std_hashset=no)
 AC_LANG_RESTORE
])
if test "ac_cv_cxx_std_hashset" = yes; then
  AC_DEFINE(HAVE_STD_HASHSET,,[define if the compiler supports std::hash_set])
fi
])

AC_DEFUN([AC_CXX_GNUCXX_HASHSET],[
AC_CACHE_CHECK(whether the compiler supports __gnu_cxx::hash_set,
ac_cv_cxx_gnucxx_hashset,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <ext/hash_set>
using __gnu_cxx::hash_set;],
 [],
 ac_cv_cxx_gnucxx_hashset=yes, ac_cv_cxx_gnucxx_hashset=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_gnucxx_hashset" = yes; then
  AC_DEFINE(HAVE_GNUCXX_HASHSET,,[define if the compiler supports __gnu_cxx::hash_set])
fi
])

AC_DEFUN([AC_CXX_STDEXT_HASHSET],[
AC_CACHE_CHECK(whether the compiler supports stdext::hash_set,
ac_cv_cxx_stdext_hashset,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <stdext/hash_set>
using stdext::hash_set;],
 [],
 ac_cv_cxx_stdext_hashset=yes, ac_cv_cxx_stdext_hashset=no)
 AC_LANG_RESTORE
])
if test "ac_cv_cxx_stdext_hashset" = yes; then
  AC_DEFINE(HAVE_STDEXT_HASHSET,,[define if the compiler supports stdext::hash_set])
fi
])
