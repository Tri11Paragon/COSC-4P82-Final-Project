#ifndef Beagle_TransportSendOp_hpp
#define Beagle_TransportSendOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Float.hpp"

namespace Beagle {

class TransportSendOp : public Operator {

public:

  //! TransportSendOp allocator type.
  typedef AllocatorT<TransportSendOp,Operator::Alloc>
          Alloc;
  //! TransportSendOp handle type.
  typedef PointerT<TransportSendOp,Operator::Handle>
          Handle;
  //! TransportSendOp bag type.
  typedef ContainerT<TransportSendOp,Operator::Bag>
          Bag;

  TransportSendOp(const string& inName="TransportSendOp");
  virtual ~TransportSendOp();

  virtual void initialize(System& ioSystem);
  virtual void operate(Deme &ioDeme, Context &ioContext);
  virtual void send(const Individual::Bag& inIndividuals, Context &ioContext)=0;
  virtual void readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);

protected:

  virtual void readAddress(PACC::XML::ConstIterator inIter)=0;

  Float::Handle mProportionToSend;

};


} // end of namespace Beagle

#endif // Beagle_TransportSendOp_hpp
