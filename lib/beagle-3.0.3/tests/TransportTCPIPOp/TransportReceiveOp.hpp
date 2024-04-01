#ifndef Beagle_TransportReceiveOp_hpp
#define Beagle_TransportReceiveOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/UInt.hpp"


namespace Beagle {

class TransportReceiveOp : public Operator {

public:

  //! TransportReceiveOp allocator type.
  typedef AllocatorT<TransportReceiveOp,Operator::Alloc>
          Alloc;
  //! TransportReceiveOp handle type.
  typedef PointerT<TransportReceiveOp,Operator::Handle>
          Handle;
  //! TransportReceiveOp bag type.
  typedef ContainerT<TransportReceiveOp,Operator::Bag>
          Bag;

  TransportReceiveOp(const string& inName="TransportReceiveOp");
  virtual ~TransportReceiveOp();

  virtual void initialize(System& ioSystem);
  virtual void operate(Deme &ioDeme, Context &ioContext);
  virtual void receive(Individual::Bag& outIndividuals, Context &ioContext)=0;

protected:

  UInt::Handle mReceiveFrequency;

};


} // end of namespace Beagle

#endif // Beagle_TransportReceiveOp_hpp
