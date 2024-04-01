#include "Test.hpp"
#include <sstream>

using namespace Beagle;
using namespace Test;
using namespace std;

void 
InitializationStringOp::initIndividual(Individual& outIndividual, Context& ioContext)
{
  std::ostringstream lOSS;
  lOSS << "Individual#" << ioContext.getDemeIndex() << "-" << ioContext.getIndividualIndex();
  outIndividual.resize(1);
  GenotypeString& lGenotype = castObjectT<GenotypeString&>(*(outIndividual[0]));
  lGenotype.setData( lOSS.str().c_str() );
}

