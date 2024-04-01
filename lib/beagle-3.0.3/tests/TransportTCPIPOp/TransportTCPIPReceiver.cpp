#include "beagle/Beagle.hpp"
#include "TransportTCPIPReceiver.hpp"

using namespace std;
using namespace Beagle;

const unsigned int TransportTCPIPReceiver::QueueMax = 10;

TransportTCPIPReceiver::TransportTCPIPReceiver(unsigned int inPortNumber) :
  mPortNumber(inPortNumber)
{
  bind(mPortNumber);
  listen(QueueMax);
  /*
  cout << "Created TransportTCPIPReceiver on port " << mPortNumber << endl;
  cout << "getSockAddress = " << getSockAddress().getIPAddress() << ":" << getSockAddress().getPortNumber() << endl;
  */
}


TransportTCPIPReceiver::~TransportTCPIPReceiver()
{
  //cout << "Destroying TransportTCPIPReceiver" << endl;
  wait(); // Is this necessary?
}


void
TransportTCPIPReceiver::getData(vector<string>& outData)
{
  mWriteDataMutex.lock(); // Ensure that data isn't written during read
  for (DataType::const_iterator lItr = mData.begin();
       lItr != mData.end();
       lItr++) {
    outData.push_back( lItr->second );
  }
  mData.clear(); // Clear data so that it's not read again
  mWriteDataMutex.unlock();
}


void 
TransportTCPIPReceiver::main() 
{
  /*
  cout << "In TransportTCPIPReceiver::main():" << endl;
  cout << "getSockAddress = " << getSockAddress().getIPAddress() << ":" << getSockAddress().getPortNumber() << endl;
  cout << "In TransportTCPIPReceiver::main(): closing port and returning!" << endl;
  close(); return;
  */

  while (!mCancel) {
    // Wait for activity
    while (!waitForActivity(10)) {
      makeCancellationPoint();
    }
      
    // Accept connection
    int lDescriptor = accept();
    PACC::Socket::Cafe lConnection(lDescriptor);
      
    // Who's the connection from?
    PACC::Socket::Address lAddress = lConnection.getPeerAddress();
    //cout << "** Received connection from " << lAddress.getIPAddress() << ":" << lAddress.getPortNumber() << endl;

    // Get message
    std::string lMessage;
    lConnection.receiveMessage(lMessage);
      
    // Close connection
    lConnection.close();
      
    // Get address as string
    std::string lAddressString;
    lAddressString = lAddress.getIPAddress()+std::string(":")+uint2str(lAddress.getPortNumber()).c_str();
      
    // Store message (overwrite if necessary)
    mWriteDataMutex.lock();
    mData[lAddressString.c_str()] = lMessage.c_str();
    mWriteDataMutex.unlock();
  }
}
