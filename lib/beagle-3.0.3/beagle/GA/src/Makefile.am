BEAGLE_GA_SRC_VAR = \
BitString.cpp \
CMAHolder.cpp \
CrossoverBlendESVecOp.cpp \
CrossoverBlendFltVecOp.cpp \
CrossoverIndicesIntVecOp.cpp \
CrossoverSBXFltVecOp.cpp \
ESVector.cpp \
EvolverBitString.cpp \
EvolverES.cpp \
EvolverFloatVector.cpp \
EvolverIntegerVector.cpp \
FloatVector.cpp \
InitBitStrOp.cpp \
InitCMAFltVecOp.cpp \
InitESVecOp.cpp \
InitFltVecOp.cpp \
InitIndicesIntVecOp.cpp \
InitIntVecOp.cpp \
IntegerVector.cpp \
MutationCMAFltVecOp.cpp \
MutationESVecOp.cpp \
MutationFlipBitStrOp.cpp \
MutationGaussianFltVecOp.cpp \
MutationShuffleIntVecOp.cpp \
MutationUniformIntVecOp.cpp \
MuWCommaLambdaCMAFltVecOp.cpp \
TermCMAOp.cpp
lib_LTLIBRARIES = libbeagle-GA.la
INCLUDES = \
-I$(top_srcdir)/PACC \
-I$(top_srcdir)/beagle/include \
-I$(top_srcdir)/beagle/GA/include
libbeagle_GA_la_LIBADD  = \
$(top_srcdir)/beagle/src/libbeagle.la \
$(top_srcdir)/PACC/Math/libpacc-math.la \
$(top_srcdir)/PACC/XML/libpacc-xml.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libbeagle_GA_la_LDFLAGS = -no-undefined -release @VERSION@
if FAST_COMPILATION
libbeagle-GA.cpp:
	rm -f libbeagle-GA.cpp
	echo "// Generated automatically by make" > libbeagle-GA.cpp
	echo >> libbeagle-GA.cpp
	echo $(BEAGLE_GA_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libbeagle-GA.cpp

clean-generic:
	rm -f libbeagle-GA.cpp

nodist_libbeagle_GA_la_SOURCES = libbeagle-GA.cpp
else
libbeagle_GA_la_SOURCES = $(BEAGLE_GA_SRC_VAR)
endif
