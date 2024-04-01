#ifndef Beagle_Test_BeagleTest_hpp
#define Beagle_Test_BeagleTest_hpp

#include "beagle/Beagle.hpp"

namespace Beagle {
namespace Test {

class BeagleTest {
public:
           BeagleTest() {}
  virtual ~BeagleTest() {}

  // STAGE ONE
  virtual void setSystem(System::Handle inSystem);
  virtual void setVivarium(Vivarium::Handle inVivarium);
  virtual void setInitializationOp( InitializationOp::Handle inInitOp );

  // STAGE TWO
  virtual System::Handle requestSystem();
  virtual Context::Handle requestContext();
  virtual Vivarium::Handle requestVivarium();

  // STAGE THREE
  virtual void initialize(int &ioArgc, char **ioArgv, bool doPostInit=true);
  virtual void postInit();

  // STAGE FOUR
  virtual Deme::Handle getDeme(unsigned int inIndex=0);

protected:
  virtual void createSystem();
  virtual void createContext();
  virtual void createVivarium();

  System::Handle           mSystem;
  Context::Handle          mContext;
  Vivarium::Handle         mVivarium;

  UIntArray::Handle        mPopSize;
  InitializationOp::Handle mInitOp;

};


} // end of namespace Test
} // end of namespace Beagle


#endif // Beagle_Test_BeagleTest_hpp
