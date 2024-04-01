BEAGLE_SRC_VAR = \
Allocator.cpp \
AssertException.cpp \
BadCastException.cpp \
BreederNode.cpp \
BreederOp.cpp \
Component.cpp \
ConfigurationDumper.cpp \
Container.cpp \
ContainerAllocator.cpp \
Context.cpp \
CrossoverOp.cpp \
DecimateOp.cpp \
Deme.cpp \
DemeAlloc.cpp \
EvaluationOp.cpp \
EvaluationMultipleOp.cpp \
Evolver.cpp \
Exception.cpp \
Fitness.cpp \
FitnessSimple.cpp \
FitnessSimpleMin.cpp \
FitnessMultiObj.cpp \
FitnessMultiObjMin.cpp \
GenerationalOp.cpp \
Genotype.cpp \
HallOfFame.cpp \
HierarchicalFairCompetitionOp.cpp \
IfThenElseOp.cpp \
Individual.cpp \
IndividualAlloc.cpp \
IndividualBag.cpp \
IndividualSizeFrequencyStatsOp.cpp \
InitializationOp.cpp \
InternalException.cpp \
InvalidateFitnessOp.cpp \
IOException.cpp \
Logger.cpp \
LoggerXML.cpp \
Map.cpp \
Matrix.cpp \
MigrationOp.cpp \
MigrationRandomRingOp.cpp \
MilestoneReadOp.cpp \
MilestoneWriteOp.cpp \
MuCommaLambdaOp.cpp \
MuPlusLambdaOp.cpp \
MutationOp.cpp \
NamedObject.cpp \
NPGA2Op.cpp \
NSGA2Op.cpp \
Object.cpp \
ObjectException.cpp \
Operator.cpp \
OversizeOp.cpp \
ParetoFrontHOF.cpp \
ParetoFrontCalculateOp.cpp \
RandomShuffleDemeOp.cpp \
Randomizer.cpp \
Register.cpp \
RegisterReadOp.cpp \
ReplacementStrategyOp.cpp \
RunTimeException.cpp \
SelectionOp.cpp \
SelectParsimonyTournOp.cpp \
SelectRandomOp.cpp \
SelectRouletteOp.cpp \
SelectTournamentOp.cpp \
StatsCalculateOp.cpp \
StatsCalcFitnessMultiObjOp.cpp \
StatsCalcFitnessSimpleOp.cpp \
Stats.cpp \
SteadyStateOp.cpp \
System.cpp \
TargetedException.cpp \
TerminationOp.cpp \
TermMaxEvalsOp.cpp \
TermMaxFitnessOp.cpp \
TermMaxGenOp.cpp \
TermMinFitnessOp.cpp \
ValidationException.cpp \
Vector.cpp \
Vivarium.cpp \
VivariumAlloc.cpp
lib_LTLIBRARIES = libbeagle.la
INCLUDES = -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include
libbeagle_la_LIBADD = \
$(top_srcdir)/PACC/Math/libpacc-math.la \
$(top_srcdir)/PACC/XML/libpacc-xml.la \
$(top_srcdir)/PACC/Util/libpacc-util.la
libbeagle_la_LDFLAGS = -no-undefined -release @VERSION@
if FAST_COMPILATION
libbeagle.cpp:
	rm -f libbeagle.cpp
	echo "// Generated automatically by make" > libbeagle.cpp
	echo >> libbeagle.cpp
	echo $(BEAGLE_SRC_VAR) | sed -e 's/ *\([a-zA-Z0-9_\-]*\.cpp\)/\#include \"\1\"=/g' | tr '=' '\n' >> libbeagle.cpp

clean-generic:
	rm -f libbeagle.cpp

nodist_libbeagle_la_SOURCES = libbeagle.cpp
else
libbeagle_la_SOURCES = $(BEAGLE_SRC_VAR)
endif
