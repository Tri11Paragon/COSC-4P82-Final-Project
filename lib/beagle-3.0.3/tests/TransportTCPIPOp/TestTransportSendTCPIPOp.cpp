#include <iostream>

#include "beagle/Beagle.hpp"


#include "Test.hpp"

#include "Socket.hpp"
#include "TransportSendTCPIPOp.hpp"

using namespace std;
using namespace Beagle;

int main(int argc, char** argv) {
  try {
    Test::BeagleTest lBeagleTest;
    System::Handle lSystem = lBeagleTest.requestSystem();
    Context::Handle lContext = lBeagleTest.requestContext();
    lBeagleTest.requestVivarium();
    lBeagleTest.initialize(argc,argv);

    // Create a transport operator
    TransportSendTCPIPOp::Handle lTransportOp = new TransportSendTCPIPOp;
    lTransportOp->initialize( *lSystem );

    // Read configuration
    PACC::XML::Document lParser;
    lParser.parse("TransportSendTCPIPOp.conf");
    PACC::XML::ConstIterator lNode = lParser.getFirstRoot();
    OperatorMap lEmptyMap;
    lTransportOp->readWithMap( lNode, lEmptyMap );

    // Test the operator
    Deme::Handle lDeme = lBeagleTest.getDeme();
    lTransportOp->operate(*lDeme, *lContext);
  }
  Beagle_Test_CatchEverythingM;
}


