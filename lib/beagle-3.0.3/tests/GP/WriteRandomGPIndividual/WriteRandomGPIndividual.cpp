#include <iostream>

#include "beagle/GP.hpp"

using namespace std;
using namespace Beagle;


int main(int argc, char** argv) {
  try {
    // Create a set of primitives
    GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
    lSet->insert(new GP::Add);
    lSet->insert(new GP::Subtract);
    lSet->insert(new GP::Multiply);
    lSet->insert(new GP::Divide);
    lSet->insert(new GP::TokenT<Double>("X"));
    lSet->insert(new GP::EphemeralDouble);

    // Create a system
    Beagle::GP::System::Handle lSystem = 
      new Beagle::GP::System( lSet );

    // Create operators
    Beagle::GP::InitHalfOp::Handle lInitHalfOp =
      new Beagle::GP::InitHalfOp;


    // Initialize operators
    lInitHalfOp->initialize( *lSystem );
    // Initialize System
    lSystem->initialize( argc, argv );
    // Post-init System
    lSystem->postInit();
    // Post-init operators
    lInitHalfOp->postInit( *lSystem );


    // Create a Context and set it up
    Beagle::GP::Context lContext; 
    lContext.setSystemHandle(lSystem);


    // Create individual and initialize using ramped-half-and-half
    Beagle::GP::Individual::Handle lIndividual
      = new Beagle::GP::Individual;

    lInitHalfOp->initIndividual( *lIndividual, lContext );


    // Write individual to screen
    PACC::XML::Streamer lXMLStreamer( cout );
    lIndividual->write( lXMLStreamer );

    cout << endl;
  }
  catch ( Beagle::Exception& inException ) {
    inException.terminate();
  }
  catch ( ... ) {
    cout << "An non-Beagle exception was thrown" << endl;
  }
}
