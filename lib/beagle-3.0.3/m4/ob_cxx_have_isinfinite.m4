AC_DEFUN([OB_CXX_HAVE_ISFINITE],
[AC_CACHE_CHECK(whether the compiler supports isfinite macro,
ac_cv_cxx_have_isfinite,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#define _GNU_SOURCE
                 #include <cmath>],
                [double d=0; int n = isfinite(d);],
                ac_cv_cxx_have_isfinite=yes,
                ac_cv_cxx_have_isfinite=no)
  AC_LANG_RESTORE
])
if test $ac_cv_cxx_have_isfinite = yes; then
  AC_DEFINE(HAVE_ISFINITE,,[define if the macro isfinite exist])
fi
])
