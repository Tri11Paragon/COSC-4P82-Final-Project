# Stuff for precompiled headers
INCLUDES = -I$(top_srcdir)/PACC -I$(top_srcdir)/beagle/include
pch: $(HEADERS)
	rm -f Beagle.hpp.gch
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) -x c++-header $(CXXFLAGS) -c -o Beagle.hpp.gch Beagle.hpp
install-data-hook:
	if test -f Beagle.hpp.gch; then $(INSTALL_DATA) Beagle.hpp.gch $(DESTDIR)$(includedir)/Beagle.hpp.gch; fi;
clean-generic:
	if test -f Beagle.hpp.gch; then rm -f Beagle.hpp.gch; fi;

beagledir = $(includedir)/beagle
EXTRA_DIST = config.hpp.msvcpp
nodist_beagle_HEADERS = config.hpp
beagle_HEADERS = \
AbstractAllocT.hpp \
AbstractContainerAllocT.hpp \
Allocator.hpp \
AllocatorT.hpp \
ArrayT.hpp \
AssertException.hpp \
BadCastException.hpp \
Beagle.hpp \
Bool.hpp \
BreederNode.hpp \
BreederOp.hpp \
castObjectT.hpp \
Char.hpp \
Component.hpp \
ComponentMap.hpp \
ConfigurationDumper.hpp \
ContainerAllocator.hpp \
ContainerAllocatorT.hpp \
Container.hpp \
ContainerT.hpp \
Context.hpp \
CrossoverOp.hpp \
DecimateOp.hpp \
Deme.hpp \
DemeAlloc.hpp \
DemeAllocT.hpp \
Double.hpp \
EvaluationMultipleOp.hpp \
EvaluationOp.hpp \
Evolver.hpp \
Exception.hpp \
Fitness.hpp \
FitnessSimple.hpp \
FitnessSimpleMin.hpp \
FitnessMultiObj.hpp \
FitnessMultiObjMin.hpp \
Float.hpp \
GenerationalOp.hpp \
Genotype.hpp \
HallOfFame.hpp \
HierarchicalFairCompetitionOp.hpp \
IfThenElseOp.hpp \
Individual.hpp \
IndividualAlloc.hpp \
IndividualAllocT.hpp \
IndividualBag.hpp \
IndividualSizeFrequencyStatsOp.hpp \
InitializationOp.hpp \
InternalException.hpp \
Int.hpp \
InvalidateFitnessOp.hpp \
IOException.hpp \
IsEqualMapPairPredicate.hpp \
IsEqualPointerPredicate.hpp \
IsLessMapPairPredicate.hpp \
IsLessPointerPredicate.hpp \
IsMorePointerPredicate.hpp \
Logger.hpp \
LoggerXML.hpp \
Long.hpp \
macros.hpp \
Map.hpp \
Matrix.hpp \
MigrationOp.hpp \
MigrationRandomRingOp.hpp \
MilestoneReadOp.hpp \
MilestoneWriteOp.hpp \
MuCommaLambdaOp.hpp \
MuPlusLambdaOp.hpp \
MutationOp.hpp \
NamedObject.hpp \
NPGA2Op.hpp \
NSGA2Op.hpp \
ObjectException.hpp \
Object.hpp \
Operator.hpp \
OperatorMap.hpp \
OversizeOp.hpp \
ParetoFrontHOF.hpp \
ParetoFrontCalculateOp.hpp \
Pointer.hpp \
PointerT.hpp \
Randomizer.hpp \
RandomShuffleDemeOp.hpp \
Register.hpp \
RegisterReadOp.hpp \
ReplacementStrategyOp.hpp \
RouletteT.hpp \
RunTimeException.hpp \
SelectionOp.hpp \
SelectParsimonyTournOp.hpp \
SelectRandomOp.hpp \
SelectRouletteOp.hpp \
SelectTournamentOp.hpp \
Short.hpp \
Stats.hpp \
StatsCalcFitnessMultiObjOp.hpp \
StatsCalcFitnessSimpleOp.hpp \
StatsCalculateOp.hpp \
SteadyStateOp.hpp \
String.hpp \
System.hpp \
TargetedException.hpp \
TerminationOp.hpp \
TermMaxEvalsOp.hpp \
TermMaxFitnessOp.hpp \
TermMaxGenOp.hpp \
TermMinFitnessOp.hpp \
UChar.hpp \
UInt.hpp \
ULong.hpp \
UShort.hpp \
ValidationException.hpp \
Vector.hpp \
Vivarium.hpp \
VivariumAlloc.hpp \
VivariumAllocT.hpp \
WrapperT.hpp
