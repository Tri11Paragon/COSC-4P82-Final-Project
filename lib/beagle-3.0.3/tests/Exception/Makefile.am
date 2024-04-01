if COMPILE_TESTS
noinst_PROGRAMS      = testStackTrace
INCLUDES             = -I. -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include
testStackTrace_LDADD = $(top_srcdir)/beagle/src/libbeagle.la \
                       $(top_srcdir)/PACC/Threading/libpacc-threading.la \
                       $(top_srcdir)/PACC/XML/libpacc-xml.la \
                       $(top_srcdir)/PACC/Util/libpacc-util.la \
                       $(top_srcdir)/PACC/Math/libpacc-math.la
testStackTrace_SOURCES = TestStackTrace.cpp
endif COMPILE_TESTS
