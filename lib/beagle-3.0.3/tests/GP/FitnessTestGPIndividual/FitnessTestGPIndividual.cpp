#include "SymbRegEvalOp.hpp"

#include <iostream>


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
    Beagle::GP::System::Handle lSystem = new Beagle::GP::System(lSet); 

    // Create operators
    Beagle::GP::EvaluationOp::Handle lEvalOp = new SymbRegEvalOp;

    // Initialize operators and system
    lSystem->initialize(argc, argv);
    lEvalOp->initialize(*lSystem);
    lSystem->postInit();
    lEvalOp->postInit(*lSystem);

    // Get filename from command-line arguments
    std::string lFileName = "indi.xml";
    if(argc>1) lFileName=argv[1];

    // Create and read individual
    Beagle::GP::Individual::Handle lIndividual = new Beagle::GP::Individual;
    lIndividual->readFromFile(lFileName.c_str(), *lSystem);

    // Write individual to screen
    std::cout << "Read individual: " << lIndividual->serialize() << std::endl;

    // Evaluate individual
    Beagle::Fitness::Handle lFitness = lEvalOp->test(lIndividual, lSystem);

    // Write fitness to screen
    std::cout << "Fitness: " << std::endl;
    std::cout << lFitness->serialize() << std::endl;

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


