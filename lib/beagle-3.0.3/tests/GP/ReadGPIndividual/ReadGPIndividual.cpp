#include <iostream>

#include "beagle/GP.hpp"

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

    // Create and initialize the system
    Beagle::GP::System::Handle lSystem = new Beagle::GP::System(lSet); 
    lSystem->initialize(argc, argv);
    lSystem->postInit();

    // Get filename from command-line arguments
    string lFileName = "indi.xml";
    if(argc>1) lFileName=argv[1];

    // Create and read individual
    Beagle::GP::Individual::Handle lIndividual = new Beagle::GP::Individual;
    lIndividual->readFromFile(lFileName, *lSystem);

    // Write individual to screen
    std::cout << "Read individual: " << lIndividual->serialize() << std::endl;
  }
  catch(Beagle::Exception& inException) {
    inException.terminate();
  }
  catch(std::exception& inException) {
    std::cerr << "Standard exception catched: " << inException.what() << std::endl;
    std::terminate();
  }
  catch(...) {
    std::cerr << "An unknown exception was thrown, exiting!" << std::endl;
  }
}
