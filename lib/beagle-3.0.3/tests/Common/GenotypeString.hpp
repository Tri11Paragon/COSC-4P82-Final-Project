#ifndef Beagle_Test_GenotypeString_hpp
#define Beagle_Test_GenotypeString_hpp

#include "beagle/Genotype.hpp"

namespace Beagle {
namespace Test {


class GenotypeString : public Genotype {
public:
  typedef AllocatorT< GenotypeString, Genotype::Alloc > Alloc;
  typedef PointerT< GenotypeString, Genotype::Handle > Handle;
  typedef ContainerT< GenotypeString, Genotype::Bag > Bag;

  void setData(const string& inData);
  void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:
  string mData;
};


} // end of namespace Test
} // end of namespace Beagle


#endif // Beagle_Test_GenotypeString_hpp
