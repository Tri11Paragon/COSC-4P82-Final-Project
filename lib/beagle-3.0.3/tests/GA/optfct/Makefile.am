if COMPILE_TESTS
noinst_PROGRAMS   = optfctfloat
INCLUDES          = -I. -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include -I$(top_srcdir)/beagle/GA/include
optfctfloat_LDADD = \
$(top_builddir)/beagle/GA/src/libbeagle-GA.la \
$(top_builddir)/beagle/src/libbeagle.la \
$(top_builddir)/PACC/Math/libpacc-math.la \
$(top_builddir)/PACC/XML/libpacc-xml.la \
$(top_builddir)/PACC/Util/libpacc-util.la
noinst_HEADERS = Functions.hpp OptFctFloatEvalOpT.hpp
optfctfloat_SOURCES = OptFctFloatMain.cpp
endif COMPILE_TESTS
