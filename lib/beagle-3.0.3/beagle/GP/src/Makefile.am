BEAGLE_GP_SRC_VAR = \
ADF.cpp \
And.cpp \
Argument.cpp \
Context.cpp \
Cos.cpp \
CrossoverOp.cpp \
CrossoverConstrainedOp.cpp \
Deme.cpp \
EphemeralDouble.cpp \
EvaluationOp.cpp \
Evolver.cpp \
ExecutionException.cpp \
Exp.cpp \
FitnessKoza.cpp \
Individual.cpp \
InitializationOp.cpp \
InitFullConstrainedOp.cpp \
InitFullOp.cpp \
InitGrowConstrainedOp.cpp \
InitGrowOp.cpp \
InitHalfConstrainedOp.cpp \
InitHalfOp.cpp \
Invoker.cpp \
Log.cpp \
MaxNodesExecutionException.cpp \
MaxTimeExecutionException.cpp \
Module.cpp \
ModuleCompressOp.cpp \
ModuleExpandOp.cpp \
ModuleVectorComponent.cpp \
MutationShrinkConstrainedOp.cpp \
MutationShrinkOp.cpp \
MutationStandardConstrainedOp.cpp \
MutationStandardOp.cpp \
MutationSwapConstrainedOp.cpp \
MutationSwapOp.cpp \
MutationSwapSubtreeConstrainedOp.cpp \
MutationSwapSubtreeOp.cpp \
Nand.cpp \
Nor.cpp \
Not.cpp \
Or.cpp \
Primitive.cpp \
PrimitiveSet.cpp \
PrimitiveSuperSet.cpp \
PrimitiveUsageStatsOp.cpp \
ResourcesExceededExecutionException.cpp \
Sin.cpp \
StatsCalcFitnessKozaOp.cpp \
StatsCalcFitnessSimpleOp.cpp \
System.cpp \
TermMaxHitsOp.cpp \
Tree.cpp \
ValidateOp.cpp \
Vivarium.cpp \
Xor.cpp
lib_LTLIBRARIES = libbeagle-GP.la
INCLUDES = \
-I$(top_srcdir)/PACC \
-I$(top_srcdir)/beagle/include \
-I$(top_srcdir)/beagle/GA/include \
-I$(top_srcdir)/beagle/GP/include
libbeagle_GP_la_LIBADD  = \
$(top_srcdir)/beagle/GA/src/libbeagle-GA.la \
$(top_srcdir)/beagle/src/libbeagle.la \
$(top_srcdir)/PACC/Math/libpacc-math.la \
$(top_srcdir)/PACC/XML/libpacc-xml.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libbeagle_GP_la_LDFLAGS = -no-undefined -release @VERSION@
if FAST_COMPILATION
libbeagle-GP.cpp:
	rm -f libbeagle-GP.cpp
	echo "// Generated automatically by make" > libbeagle-GP.cpp
	echo >> libbeagle-GP.cpp
	echo $(BEAGLE_GP_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libbeagle-GP.cpp

clean-generic:
	rm -f libbeagle-GP.cpp

nodist_libbeagle_GP_la_SOURCES = libbeagle-GP.cpp
else
libbeagle_GP_la_SOURCES = $(BEAGLE_GP_SRC_VAR)
endif
