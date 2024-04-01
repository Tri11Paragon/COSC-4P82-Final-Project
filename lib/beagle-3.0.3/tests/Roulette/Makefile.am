if COMPILE_TESTS
noinst_PROGRAMS      = testRoulette
INCLUDES             = -I. -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include
testRoulette_LDADD   = $(top_srcdir)/PACC/Math/libpacc-math.la \
                       $(top_srcdir)/PACC/Threading/libpacc-threading.la \
                       $(top_srcdir)/PACC/Util/libpacc-util.la \
                       $(top_srcdir)/PACC/XML/libpacc-xml.la \
                       $(top_srcdir)/beagle/src/libbeagle.la
testRoulette_SOURCES = testRoulette.cpp
endif COMPILE_TESTS
