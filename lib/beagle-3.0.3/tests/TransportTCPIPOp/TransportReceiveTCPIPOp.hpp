#ifndef Beagle_TransportReceiveTCPIPOp_hpp
#define Beagle_TransportReceiveTCPIPOp_hpp

#include "TransportReceiveOp.hpp"
#include "TransportTCPIPReceiver.hpp"
#include "DefaultSignalHandler.hpp"


namespace Beagle {

class TransportReceiveTCPIPOp : public TransportReceiveOp {
public:
  //! TransportReceiveTCPIPOp allocator type.
  typedef AllocatorT<TransportReceiveTCPIPOp,TransportReceiveOp::Alloc>
          Alloc;
  //! TransportReceiveTCPIPOp handle type.
  typedef PointerT<TransportReceiveTCPIPOp,TransportReceiveOp::Handle>
          Handle;
  //! TransportReceiveTCPIPOp bag type.
  typedef ContainerT<TransportReceiveTCPIPOp,TransportReceiveOp::Bag>
          Bag;

  TransportReceiveTCPIPOp(const string& inName="TransportReceiveTCPIPOp");
  virtual ~TransportReceiveTCPIPOp();

  virtual void initialize(System& ioSystem);
  virtual void receive(Individual::Bag& outIndividuals, Context& ioContext);
  virtual void readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:
  TransportTCPIPReceiver* mReceiver;
  DefaultSignalHandler mSignalHandler;
};


} // end of namespace Beagle

#endif // Beagle_TransportReceiveTCPIPOp_hpp
