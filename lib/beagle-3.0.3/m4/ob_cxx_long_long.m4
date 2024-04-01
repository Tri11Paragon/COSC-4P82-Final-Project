AC_DEFUN([OB_CXX_LONG_LONG],
[AC_CACHE_CHECK(for long long int, ob_cv_cxx_long_long,
[
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[long long int i;],
   ob_cv_cxx_long_long=yes,
   ob_cv_cxx_long_long=no)
   ])
if test $ob_cv_cxx_long_long = yes; then
  AC_DEFINE(HAVE_LONG_LONG,,[define if long long int type is supported])
fi
])
