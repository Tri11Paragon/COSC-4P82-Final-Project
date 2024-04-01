dnl
dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_cxx_have_ext_hash_map.html
dnl
dnl Modified by Christian Gagne, August-September 2007
dnl For checking availability in namespaces stdext (MSVC++) and __gnu_cxx (GCC 3.1-4)
dnl

AC_DEFUN([AC_CXX_STD_HASHMAP],[
AC_CACHE_CHECK(whether the compiler supports std::hash_map,
ac_cv_cxx_std_hashmap,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <hash_map>
using std::hash_map;],
 [],
 ac_cv_cxx_std_hashmap=yes, ac_cv_cxx_std_hashmap=no)
 AC_LANG_RESTORE
])
if test "ac_cv_cxx_std_hashmap" = yes; then
  AC_DEFINE(HAVE_STD_HASHMAP,,[define if the compiler supports std::hash_map])
fi
])

AC_DEFUN([AC_CXX_GNUCXX_HASHMAP],[
AC_CACHE_CHECK(whether the compiler supports __gnu_cxx::hash_map,
ac_cv_cxx_gnucxx_hashmap,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <ext/hash_map>
using __gnu_cxx::hash_map;],
 [],
 ac_cv_cxx_gnucxx_hashmap=yes, ac_cv_cxx_gnucxx_hashmap=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_gnucxx_hashmap" = yes; then
  AC_DEFINE(HAVE_GNUCXX_HASHMAP,,[define if the compiler supports __gnu_cxx::hash_map])
fi
])

AC_DEFUN([AC_CXX_STDEXT_HASHMAP],[
AC_CACHE_CHECK(whether the compiler supports stdext::hash_map,
ac_cv_cxx_stdext_hashmap,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <stdext/hash_map>
using stdext::hash_map;],
 [],
 ac_cv_cxx_stdext_hashmap=yes, ac_cv_cxx_stdext_hashmap=no)
 AC_LANG_RESTORE
])
if test "ac_cv_cxx_stdext_hashmap" = yes; then
  AC_DEFINE(HAVE_STDEXT_HASHMAP,,[define if the compiler supports stdext::hash_map])
fi
])
