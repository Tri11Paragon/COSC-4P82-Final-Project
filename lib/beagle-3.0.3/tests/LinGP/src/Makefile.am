if COMPILE_TESTS
BEAGLE_LINGP_SRC_VAR = \
Evolver.cpp \
Individual.cpp \
InitializationOp.cpp \
Instruction.cpp \
InstructionSet.cpp \
InstructionSuperSet.cpp \
MutationOp.cpp \
Program.cpp \
System.cpp
noinst_LTLIBRARIES = libbeagle-LinGP.la
INCLUDES           = \
-I$(top_srcdir)/PACC \
-I$(top_srcdir)/beagle/include \
-I$(top_srcdir)/beagle/GA/include \
-I$(top_srcdir)/beagle/GP/include \
-I$(top_srcdir)/tests/LinGP/include
libbeagle_LinGP_la_LIBADD  = \
$(top_srcdir)/beagle/GP/src/libbeagle-GP.la \
$(top_srcdir)/beagle/GA/src/libbeagle-GA.la \
$(top_srcdir)/beagle/src/libbeagle.la \
$(top_srcdir)/PACC/Math/libpacc-math.la \
$(top_srcdir)/PACC/XML/libpacc-xml.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libbeagle_LinGP_la_LDFLAGS = -no-undefined
if FAST_COMPILATION
libbeagle-LinGP.cpp:
	rm -f libbeagle-LinGP.cpp
	echo "// Generated automatically by make" > libbeagle-LinGP.cpp
	echo >> libbeagle-LinGP.cpp
	echo $(BEAGLE_LINGP_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libbeagle-LinGP.cpp

clean-generic:
	rm -f libbeagle-LinGP.cpp

nodist_libbeagle_LinGP_la_SOURCES = libbeagle-LinGP.cpp
else
libbeagle_LinGP_la_SOURCES = $(BEAGLE_LINGP_SRC_VAR)
endif
endif COMPILE_TESTS
