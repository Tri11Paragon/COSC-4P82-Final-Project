BEAGLE_COEV_SRC_VAR = \
EvaluationOp.cpp \
GPEvaluationOp.cpp \
TermBroadcastOp.cpp
lib_LTLIBRARIES = libbeagle-Coev.la
INCLUDES = \
-I$(top_srcdir)/PACC \
-I$(top_srcdir)/beagle/include \
-I$(top_srcdir)/beagle/GA/include \
-I$(top_srcdir)/beagle/GP/include \
-I$(top_srcdir)/beagle/Coev/include
libbeagle_Coev_la_LIBADD  = \
$(top_srcdir)/beagle/GP/src/libbeagle-GP.la \
$(top_srcdir)/beagle/GA/src/libbeagle-GA.la \
$(top_srcdir)/beagle/src/libbeagle.la \
$(top_srcdir)/PACC/Threading/libpacc-threading.la \
$(top_srcdir)/PACC/Math/libpacc-math.la \
$(top_srcdir)/PACC/XML/libpacc-xml.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libbeagle_Coev_la_LDFLAGS = -no-undefined -release @VERSION@
if FAST_COMPILATION
libbeagle-Coev.cpp:
	rm -f libbeagle-Coev.cpp
	echo "// Generated automatically by make" > libbeagle-Coev.cpp
	echo >> libbeagle-Coev.cpp
	echo $(BEAGLE_COEV_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libbeagle-Coev.cpp

clean-generic:
	rm -f libbeagle-Coev.cpp

nodist_libbeagle_Coev_la_SOURCES = libbeagle-Coev.cpp
else
libbeagle_Coev_la_SOURCES = $(BEAGLE_COEV_SRC_VAR)
endif
