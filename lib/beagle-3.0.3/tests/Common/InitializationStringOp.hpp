#ifndef Beagle_Test_InitializationStringOp_hpp
#define Beagle_Test_InitializationStringOp_hpp

#include "beagle/Beagle.hpp"

namespace Beagle {
namespace Test {

class InitializationStringOp : public InitializationOp {
public:
  virtual void initIndividual(Individual& outIndividual, Context& ioContext);
};

} // end of namespace Test
} // end of namespace Beagle


#endif // Beagle_Test_InitializationStringOp_hpp
