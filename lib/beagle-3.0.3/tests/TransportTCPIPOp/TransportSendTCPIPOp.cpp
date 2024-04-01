#include "beagle/Beagle.hpp"
#include "TransportSendTCPIPOp.hpp"
#include "Socket.hpp"

#include <string>
#include <iostream>

using namespace Beagle;
using namespace std;

TransportSendTCPIPOp::TransportSendTCPIPOp(const string& inName) :
  TransportSendOp(inName)
{
}


TransportSendTCPIPOp::~TransportSendTCPIPOp()
{
}


void 
TransportSendTCPIPOp::initialize(System& ioSystem)
{
  TransportSendOp::initialize(ioSystem);
}


void 
TransportSendTCPIPOp::send(const Individual::Bag& inIndividuals, Context &ioContext)
{
  if (mAddresses.empty()) {
    Beagle_LogBasicM(
      ioContext.getSystem().getLogger(),
      "transport", "Beagle::TransportSendTCPIPOp",
      "Warning!  There are no addresses to send individuals"
    );
    return;
  }

  // Convert the bag into a string
  string lMessage = inIndividuals.serialize();

  // Send to addresses
  for (unsigned int i=0; i<mAddresses.size(); i++) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "transport", "Beagle::TransportSendTCPIPOp",
      string("Sending to ")+uint2ordinal(i+1)+string(" address")
    );
    if (send(mAddresses[i], lMessage, ioContext)) {
      Beagle_LogDetailedM(
        ioContext.getSystem().getLogger(),
	"transport", "Beagle::TransportSendTCPIPOp",
	string("Successfully sent ")+uint2str(inIndividuals.size())+
	string(" individuals to ")+uint2ordinal(i+1)+string(" address")
      );
    }
  }
}


bool TransportSendTCPIPOp::send(const PACC::Socket::Address& inAddress, 
				const string& inMessage,
				Context &ioContext)
{
  try {
    // Setup a cafe connection
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "transport", "Beagle::TransportSendTCPIPOp",
      string("Attempting to connect to ")+string(inAddress.getHostName().c_str())+
      string(" (")+string(inAddress.getIPAddress().c_str())+string(") on port ")+
      uint2str(inAddress.getPortNumber())
    );
    PACC::Socket::Cafe lConnection( inAddress );

    // Send the message
    lConnection.sendMessage( inMessage.c_str() );
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "transport", "Beagle::TransportSendTCPIPOp",
      string("Successfully sent message")
    );
    return true;
  }
  catch (PACC::Socket::Exception& inException) {
    if (inException.getErrorCode() == PACC::Socket::eConnectionRefused) {
      Beagle_LogBasicM(
        ioContext.getSystem().getLogger(),
        "transport", "Beagle::TransportSendTCPIPOp",
        string("Warning!  Connection refused when attempting to connect to ")+
        string(inAddress.getHostName().c_str())+string(" (")+
        string(inAddress.getIPAddress().c_str())+
        string(") on port ")+uint2str(inAddress.getPortNumber())
      );
      return false;
    }
    else {
      throw;
    }
  }
}


void TransportSendTCPIPOp::readAddress(PACC::XML::ConstIterator inNode)
{
  Beagle_AssertM( inNode->getValue() == "Address" );
  UInt::Handle lPort;
  String::Handle lName;
  for (PACC::XML::ConstIterator lNode = inNode->getFirstChild();
       lNode;
       lNode = lNode->getNextSibling()) {
    // Read port
    if (lNode->getValue() == "Port") {
      lPort = new UInt;
      PACC::XML::ConstIterator lPortNode = lNode->getFirstChild();
      Beagle_AssertM( lPortNode->getType() == PACC::XML::eString );
      std::istringstream lISS( lPortNode->getValue() );
      lISS >> lPort->getWrappedValue();
    }
    // Read name
    else if (lNode->getValue() == "Name") {
      lName = new String;
      PACC::XML::ConstIterator lNameNode = lNode->getFirstChild();
      Beagle_AssertM( lNameNode->getType() == PACC::XML::eString );
      std::istringstream lISS( lNameNode->getValue().c_str() );
      lName->getWrappedValue() = lNameNode->getValue().c_str();
    }
  }
  if (lPort!=NULL && lName!=NULL) {
    PACC::Socket::Address lAddress(*lPort,lName->getWrappedValue().c_str());
    mAddresses.push_back( lAddress );
  }
  else {
    // error reading address
  }
}


void 
TransportSendTCPIPOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  using namespace std;
  cout << "In TransportSendTCPIPOp::write()\n";
  /*
  ioStreamer.openTag(getName());
  ioStreamer.openTag("Addresses");

  for (unsigned int i=0; i<mAddresses.size(); i++) {
    ioStreamer.openTag("Address");
    ioStreamer.insertString( uint2str(mAddresses[i].getPortNumber()), "Port");
    ioStreamer.insertString( mAddresses[i].getHostName(), "Name");
    ioStreamer.closeTag(); // Address
  }

  ioStreamer.closeTag(); // Addresses
  ioStreamer.closeTag(); // getName()
  */
}
