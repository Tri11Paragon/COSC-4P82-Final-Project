#include <iostream>

#include "beagle/Beagle.hpp"
#include "Test.hpp"

#include "Socket.hpp"
#include "Threading.hpp"
#include "TransportReceiveTCPIPOp.hpp"

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
    TransportReceiveTCPIPOp::Handle lTransportOp = new TransportReceiveTCPIPOp();
    lTransportOp->initialize( *lSystem );

    // Read configuration
    PACC::XML::Document lParser;
    lParser.parse("TransportReceiveTCPIPOp.conf");
    PACC::XML::ConstIterator lNode = lParser.getFirstRoot();
    OperatorMap lEmptyMap;
    lTransportOp->readWithMap( lNode, lEmptyMap );

    // Wait for individuals to be sent
    PACC::Threading::Thread::sleep(10.);

    // Test the operator
    Deme::Handle lDeme = lBeagleTest.getDeme();
    lTransportOp->operate(*lDeme, *lContext);
  }
  Beagle_Test_CatchEverythingM;
}

