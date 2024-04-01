PACC_UTIL_SRC_VAR = \
Date.cpp \
Randomizer.cpp \
SignalHandler.cpp \
Timer.cpp \
Tokenizer.cpp
paccutildir = $(includedir)/Util
paccutil_HEADERS = \
Assert.hpp \
Date.hpp \
MTRand.hpp \
Randomizer.hpp \
RandomPermutation.hpp \
SignalHandler.hpp \
StringFunc.hpp \
Timer.hpp \
Tokenizer.hpp
lib_LTLIBRARIES = libpacc-util.la
INCLUDES = -I$(top_srcdir)/PACC
libpacc_util_la_LDFLAGS = -no-undefined -release @PACC_VERSION@
if FAST_COMPILATION
libpacc-util.cpp:
	rm -f libpacc-util.cpp
	echo "// Generated automatically by make" > libpacc-util.cpp
	echo >> libpacc-util.cpp
	echo $(PACC_UTIL_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libpacc-util.cpp

clean-generic:
	rm -f libpacc-util.cpp

nodist_libpacc_util_la_SOURCES = libpacc-util.cpp
else
libpacc_util_la_SOURCES = $(PACC_UTIL_SRC_VAR)
endif
