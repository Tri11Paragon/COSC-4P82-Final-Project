#ifndef Beagle_TransportTCPIPReceiver_hpp
#define Beagle_TransportTCPIPReceiver_hpp

#include <string>
#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/UInt.hpp"

#include "Threading.hpp"
#include "Socket.hpp"


namespace Beagle {

class TransportTCPIPReceiver : public PACC::Threading::Thread, private PACC::Socket::TCP {
public:
  TransportTCPIPReceiver(unsigned int inPortNumber);
  virtual ~TransportTCPIPReceiver();

  void getData(std::vector<string>& outData);
  unsigned int getPortNumber() { return mPortNumber; }

protected:

  static const unsigned int QueueMax;
  typedef std::map< const string,string > DataType;
  DataType mData; // Address -> Individual data
  PACC::Threading::Mutex mWriteDataMutex;
  unsigned int mPortNumber;

  virtual void main();
};


} // end of namespace Beagle

#endif // Beagle_TransportTCPIPReceiver_hpp
