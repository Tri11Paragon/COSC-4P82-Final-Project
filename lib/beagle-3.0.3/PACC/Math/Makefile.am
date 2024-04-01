PACC_MATH_SRC_VAR = \
Matrix.cpp \
QRandSequencer.cpp \
Vector.cpp
paccmathdir = $(includedir)/Math
paccmath_HEADERS = \
Matrix.hpp \
QRandSequencer.hpp \
Vector.hpp
lib_LTLIBRARIES = libpacc-math.la
INCLUDES = -I$(top_srcdir)/PACC
libpacc_math_la_LIBADD  = \
$(top_srcdir)/PACC/XML/libpacc-xml.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libpacc_math_la_LDFLAGS = -no-undefined -release @PACC_VERSION@
if FAST_COMPILATION
libpacc-math.cpp:
	rm -f libpacc-math.cpp
	echo "// Generated automatically by make" > libpacc-math.cpp
	echo >> libpacc-math.cpp
	echo $(PACC_MATH_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libpacc-math.cpp

clean-generic:
	rm -f libpacc-math.cpp

nodist_libpacc_math_la_SOURCES = libpacc-math.cpp
else
libpacc_math_la_SOURCES = $(PACC_MATH_SRC_VAR)
endif