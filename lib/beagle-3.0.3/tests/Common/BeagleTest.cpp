#include "Test.hpp"


using namespace Beagle;
using namespace Test;
using namespace std;


// ********** STAGE ONE *****************
// Set objects to specific values

void 
BeagleTest::setSystem(System::Handle inSystem)
{
  if (mSystem!=NULL) {
    Beagle_LogBasicM(
      mSystem->getLogger(),
      "setSystem","Beagle::Test::BeagleTest",
      "Warning: Called setSystem() when a system already exists"
    );
  }
  mSystem = inSystem;
}


void 
BeagleTest::setVivarium( Vivarium::Handle inVivarium )
{
  if (mSystem==NULL) createSystem();
  if (mVivarium!=NULL) {
    Beagle_LogBasicM(
      mSystem->getLogger(),
      "setVivarium","Beagle::Test::BeagleTest",
      "Warning: Called setVivarium() when a vivarium already exists"
    );
  }
  mVivarium = inVivarium;
}


void 
BeagleTest::setInitializationOp( InitializationOp::Handle inInitOp )
{
  if (mSystem==NULL) createSystem();
  if (mInitOp!=NULL) {
    Beagle_LogBasicM(
      mSystem->getLogger(),
      "setInitializationOp","Beagle::Test::BeagleTest",
      "Warning: Called setInitializationOp() when an initialization operators already exists"
    );
  }
  mInitOp = inInitOp;
}



// ********** STAGE TWO *****************
// Get the object that will be required


System::Handle
BeagleTest::requestSystem() 
{
  if (mSystem==NULL) {
    createSystem();
  }
  return mSystem;
}


Context::Handle
BeagleTest::requestContext()
{
  if (mContext==NULL) {
    createContext();
  }
  return mContext;
}

Vivarium::Handle
BeagleTest::requestVivarium() 
{
  if (mVivarium==NULL) {
    createVivarium();
  }
  return mVivarium;
}




void
BeagleTest::createSystem()
{
  // Check that the system doesn't already exist
  if (mSystem!=NULL) {
    Beagle_LogBasicM(
      mSystem->getLogger(),
      "createSystem","Beagle::Test::BeagleTest",
      "Warning: Called createSystem() when the system already exists"
      );
    return;
  }

  // Create a new system
  mSystem = new Beagle::System;
}


void
BeagleTest::createContext()
{
  // Ensure that a system exists
  if (mSystem==NULL) createSystem();

  // Check that context doesn't already exist
  if (mContext!=NULL) {
    Beagle_LogBasicM(
      mSystem->getLogger(),
      "createContext","Beagle::Test::BeagleTest",
      "Warning: Called createContext() when a context already exists"
      );
    return;
  }

  // Create context
  Beagle_LogDebugM(
    mSystem->getLogger(),
    "createContext","Beagle::Test::BeagleTest",
    "Creating a context"
  );
  mContext = castHandleT<Context>(mSystem->getContextAllocator().allocate());
  mContext->setSystemHandle( mSystem );
}


void
BeagleTest::createVivarium()
{
  // Ensure that a system exists
  if (mSystem==NULL) createSystem();

  // Ensure that a context exists
  if (mContext==NULL) createContext();

  // Check that a vivarium doesn't already exist
  if (mVivarium!=NULL) {
    Beagle_LogBasicM(
      mSystem->getLogger(),
      "createVivarium","Beagle::Test::BeagleTest",
      "Warning: Called createVivarium() when a vivarium already exists"
      );
    return;
  }

  // Create a vivarium
  Beagle_LogDebugM(
    mSystem->getLogger(),
    "createVivarium","Beagle::Test::BeagleTest",
    "Creating a vivarium"
  );
  mVivarium = new Vivarium(new GenotypeString::Alloc, new FitnessSimple::Alloc);

  // Seeing we're defaulting to "GenotypeString", use the default
  // initialization operator
  mInitOp = new InitializationStringOp;
}





// ********** STAGE THREE *****************
// Initialize everything


// No objects are valid for use until after the class has been initialized.
// The idea is to "get" each object and then call initialize().
void
BeagleTest::initialize(int &ioArgc, char **ioArgv, bool doPostInit)
{
  if (mSystem==NULL) createSystem();

  // Add population size parameter
  if(mSystem->getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(mSystem->getRegister().getEntry("ec.pop.size"));
  } else {
    mPopSize = new UIntArray(1,10);
    string lLongDescrip("Number of demes and size of each deme of the population. ");
    lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
    lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
    lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
    lLongDescrip += "deme.";
    Register::Description lDescription(
      "Vivarium and demes sizes",
      "UIntArray",
      "10",
      lLongDescrip
    );
    mSystem->getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }

  // Initialize system
  mSystem->initialize(ioArgc, ioArgv);

  // Initialize the operators
  if (mVivarium!=NULL) {
    Beagle_NonNullPointerAssertM( mInitOp );
    mInitOp->initialize( *mSystem );
  }

  // Consider an automatic call to postInit()
  if (doPostInit) postInit();
}

void
BeagleTest::postInit()
{
  // postInit the operators
  if (mVivarium!=NULL) {
    Beagle_NonNullPointerAssertM( mInitOp );
    mInitOp->postInit( *mSystem );
  }

  // postInit the System
  Beagle_NonNullPointerAssertM( mSystem );
  mSystem->postInit();

  // Check if the vivarium exists
  if (mVivarium!=NULL) {
    // Fill the vivarium
    Beagle_NonNullPointerAssertM( mPopSize );
    mVivarium->resize( mPopSize->size() );
    Beagle_NonNullPointerAssertM( mInitOp );
    Beagle_NonNullPointerAssertM( mContext );
    for (unsigned int i=0; i<mVivarium->size(); i++) {
      mContext->setDemeIndex( i );
      mContext->setDemeHandle( mVivarium->at(i) );
      mInitOp->operate( *(mVivarium->at(i)), *mContext );
    }
  }
}




// ********** STAGE FOUR *****************
// Get objects after initialization


Deme::Handle
BeagleTest::getDeme(unsigned int inIndex) 
{
  if (mVivarium==NULL) {
    createVivarium();
  }
  Beagle_AssertM( inIndex < mVivarium->size() );
  return mVivarium->at(inIndex);
}
