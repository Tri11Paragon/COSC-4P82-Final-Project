#include <cmath>
#include "beagle/Beagle.hpp"
#include "TransportSendOp.hpp"

using namespace Beagle;

TransportSendOp::TransportSendOp(const string& inName) :
  Operator(inName)
{}


TransportSendOp::~TransportSendOp()
{}


void 
TransportSendOp::initialize(System& ioSystem)
{
  Beagle_LogVerboseM(
    ioSystem.getLogger(),
    "transport", "Beagle::TransportSendOp",
    "Initializing TransportSendOp"
  );


  // Add proportion to send parameter
  if(ioSystem.getRegister().isRegistered("ec.transport.send")) {
    mProportionToSend = castHandleT<Float>(ioSystem.getRegister().getEntry("ec.transport.send"));
  } else {
    mProportionToSend = new Float(0.05);
    Register::Description lDescription
      ("Proportion to transport",
       "Float",
       "0.05",
       "Proportion of the population to send (sent individuals are not removed from the deme)");
    ioSystem.getRegister().addEntry("ec.transport.send", mProportionToSend, lDescription);
  }
}

void 
TransportSendOp::operate(Deme &ioDeme, Context &ioContext)
{
  // Check there are individuals in the deme
  if (ioDeme.empty()) {
    Beagle_LogBasicM(
      ioContext.getSystem().getLogger(),
      "transport", "Beagle::TransportSendOp",
      "Warning: There are no individuals in the deme, unable to transport any individuals"
    );
    return;
  }

  // Sort the deme
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "transport", "Beagle::TransportSendOp",
    "Sorting the deme"
  );
  std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());

  // Place the top mProportionToSend into a bag
  Beagle_NonNullPointerAssertM( mProportionToSend );

  // Check that there are sufficient individuals
  unsigned int lNumberToSend 
    = static_cast<unsigned int>(std::ceil(mProportionToSend->getWrappedValue() * ioDeme.size()));
  if (lNumberToSend > ioDeme.size()) {
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "transport", "Beagle::TransportSendOp",
      string("There are only ")+uint2str(ioDeme.size())+
      string(" individuals in the deme; sending all of them")
    );
    lNumberToSend=ioDeme.size();
  }

  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "transport", "Beagle::TransportSendOp",
    string("Collecting the ")+uint2str(lNumberToSend)+
    string(" best individuals of the ")+uint2ordinal(ioContext.getDemeIndex()+1)+
    string(" deme for transport")
  );
  Individual::Bag lBag;
  lBag.resize(lNumberToSend);
  for (unsigned int i=0; i<lNumberToSend; i++) {
    lBag[i] = ioDeme[i];
  }

  // Send the bag of individuals
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "transport", "Beagle::TransportSendOp",
    string("Sending ")+uint2str(lBag.size())+
    string(" individuals for transport")
  );
  send(lBag,ioContext);
}


void 
TransportSendOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  /*
  using namespace std;
  cout << "In TransportSendOp::readWithMap()\n";
  cout << "inIter = ";
  PACC::XML::Streamer lStreamer(cout);
  inIter->serialize(lStreamer);
  cout << endl;
  */

  Operator::readWithMap(inIter,inOpMap);
  for (PACC::XML::ConstIterator lIter = inIter->getFirstChild();
       lIter;
       lIter = lIter->getNextSibling() ) {
    // Read addresses
    if (lIter->getValue() == "Addresses") {
      for (PACC::XML::ConstIterator lAddressIter = lIter->getFirstChild();
	   lAddressIter;
	   lAddressIter = lAddressIter->getNextSibling() ) {
	if ( lAddressIter->getValue() == "Address" ) {
	  readAddress( lAddressIter );
	}
      }
    }
  }
}
