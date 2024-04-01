if COMPILE_TESTS
TEST_SRC_VAR = \
BeagleTest.cpp \
Pass.cpp \
GenotypeString.cpp \
InitializationStringOp.cpp
noinst_LTLIBRARIES = libtest.la
INCLUDES = \
-I. \
-I$(top_srcdir)/PACC \
-I$(top_srcdir)/beagle/include \
-I$(top_srcdir)/beagle/GA/include \
-I$(top_srcdir)/beagle/GP/include
libtest_la_LIBADD  = \
$(top_srcdir)/beagle/src/libbeagle.la \
$(top_srcdir)/PACC/Math/libpacc-math.la \
$(top_srcdir)/PACC/XML/libpacc-xml.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libtest_la_LDFLAGS = -no-undefined
noinst_HEADERS = \
BeagleTest.hpp \
GenotypeString.hpp \
InitializationStringOp.hpp \
Macros.hpp \
Pass.hpp \
Test.hpp
if FAST_COMPILATION
libtest.cpp:
	rm -f libtest.cpp
	echo "// Generated automatically by make" > libtest.cpp
	echo >> libtest.cpp
	echo $(TEST_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libtest.cpp

clean-generic:
	rm -f libtest.cpp

nodist_libtest_la_SOURCES = libtest.cpp
else
libtest_la_SOURCES = $(TEST_SRC_VAR)
endif
endif COMPILE_TESTS
