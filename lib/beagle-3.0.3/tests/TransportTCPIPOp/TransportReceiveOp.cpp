#include "beagle/Beagle.hpp"
#include "TransportReceiveOp.hpp"

using namespace Beagle;

TransportReceiveOp::TransportReceiveOp(const string& inName) :
  Operator(inName)
{}


TransportReceiveOp::~TransportReceiveOp()
{}


void 
TransportReceiveOp::initialize(System& ioSystem)
{
}

void 
TransportReceiveOp::operate(Deme &ioDeme, Context &ioContext)
{
  // Sort the deme
  std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());

  // Receive individuals
  Individual::Bag lBag(ioDeme.getTypeAlloc());
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "transport", "Beagle::TransportReceiveOp",
    string("Receiving transported individuals")
  );
  receive(lBag,ioContext);

  // Check that the bag is not empty
  if (lBag.empty()) {
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "transport", "Beagle::TransportReceiveOp",
      "No individuals were received"
    );
    return;
  }

  // Replace the bottom of the deme with the received individuals
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "transport", "Beagle::TransportReceiveOp",
    string("Received ")+uint2str(lBag.size())+
    string(" individuals.  Replacing the worst individuals of the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+string(" deme.")
  );
  for (unsigned int i=0; i<lBag.size(); i++) {
    unsigned int lReplaceIndex = ioDeme.size()-1-i;
    ioDeme[lReplaceIndex] = lBag[i];
    ioDeme[lReplaceIndex]->getFitness()->setInvalid();
  }
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "transport", "Beagle::TransportReceiveOp",
    string("Transportation complete")
  );
}
