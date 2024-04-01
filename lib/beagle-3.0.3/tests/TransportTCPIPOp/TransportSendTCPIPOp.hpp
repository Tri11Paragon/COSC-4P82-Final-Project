#ifndef Beagle_TransportSendTCPIPOp_hpp
#define Beagle_TransportSendTCPIPOp_hpp

#include "TransportSendOp.hpp"
#include "Socket.hpp"


namespace Beagle {

class TransportSendTCPIPOp : public TransportSendOp {
public:
  //! TransportSendTCPIPOp allocator type.
  typedef AllocatorT<TransportSendTCPIPOp,TransportSendOp::Alloc>
          Alloc;
  //! TransportSendTCPIPOp handle type.
  typedef PointerT<TransportSendTCPIPOp,TransportSendOp::Handle>
          Handle;
  //! TransportSendTCPIPOp bag type.
  typedef ContainerT<TransportSendTCPIPOp,TransportSendOp::Bag>
          Bag;

  TransportSendTCPIPOp(const string& inName="TransportSendTCPIPOp");
  virtual ~TransportSendTCPIPOp();

  virtual void initialize(System& ioSystem);
  virtual void send(const Individual::Bag& inIndividuals, Context &ioContext);
  virtual bool send(const PACC::Socket::Address& inAddress,
                    const std::string& inMessage,
                    Context &ioContext);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

  virtual void readAddress(PACC::XML::ConstIterator inIter);

  std::vector< PACC::Socket::Address > mAddresses;

};


} // end of namespace Beagle

#endif // Beagle_TransportSendTCPIPOp_hpp
