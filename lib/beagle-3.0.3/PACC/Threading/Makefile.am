PACC_THREADING_SRC_VAR = \
Condition.cpp \
Exception.cpp \
Mutex.cpp \
Semaphore.cpp \
Thread.cpp \
ThreadPool.cpp
paccthreadingdir = $(includedir)/Threading
paccthreading_HEADERS = \
Condition.hpp \
Exception.hpp \
Mutex.hpp \
Semaphore.hpp \
Task.hpp \
Thread.hpp \
ThreadPool.hpp
lib_LTLIBRARIES = libpacc-threading.la
INCLUDES = -I$(top_srcdir)/PACC
libpacc_threading_la_LIBADD  = $(top_srcdir)/PACC/Util/libpacc-util.la
libpacc_threading_la_LDFLAGS = -no-undefined -release @PACC_VERSION@
if FAST_COMPILATION
libpacc-threading.cpp:
	rm -f libpacc-threading.cpp
	echo "// Generated automatically by make" > libpacc-threading.cpp
	echo >> libpacc-threading.cpp
	echo $(PACC_THREADING_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libpacc-threading.cpp

clean-generic:
	rm -f libpacc-threading.cpp

nodist_libpacc_threading_la_SOURCES = libpacc-threading.cpp
else
libpacc_threading_la_SOURCES = $(PACC_THREADING_SRC_VAR)
endif
