#include "beagle/Beagle.hpp"
#include "TransportReceiveTCPIPOp.hpp"
#include "Socket.hpp"

#include <string>
#include <iostream>

using namespace Beagle;
using namespace std;

TransportReceiveTCPIPOp::TransportReceiveTCPIPOp(const string& inName) :
  TransportReceiveOp(inName),
  mReceiver(NULL)
{}


TransportReceiveTCPIPOp::~TransportReceiveTCPIPOp()
{
  if (mReceiver!=NULL) {
    // Stop receiver thread
    mReceiver->cancel();
    // Wait till thread's stopped
    mReceiver->wait();
    delete mReceiver;
  }
}


void 
TransportReceiveTCPIPOp::initialize(System& ioSystem)
{}

void 
TransportReceiveTCPIPOp::receive(Individual::Bag& outIndividuals,
				 Context& ioContext)
{
  Beagle_AssertM( mReceiver!=NULL );

  // Get data from receiver thread
  vector<string> lData;
  mReceiver->getData( lData );

  PACC::XML::Document lParser;
  for (unsigned int i=0; i<lData.size(); i++) {
    // Parse data
    istringstream lISS(lData[i].c_str());
    lParser.parse(lISS,"istringstream");

    // Read data
    PACC::XML::ConstIterator lNode = lParser.getFirstRoot();
    outIndividuals.readWithContext( lNode, ioContext );
  }
}


void 
TransportReceiveTCPIPOp::readWithMap(PACC::XML::ConstIterator inNode, OperatorMap& inOpMap)
{
  Beagle_AssertM( mReceiver==NULL );
  TransportReceiveOp::readWithMap(inNode,inOpMap);

  // Get <Port>
  PACC::XML::ConstIterator lNode = inNode->getFirstChild();
  Beagle_AssertM( lNode->getValue() == "Port" );

  // Get associated string
  lNode = lNode->getFirstChild();
  Beagle_AssertM( lNode->getType() == PACC::XML::eString );

  // Read string
  std::istringstream lISS( lNode->getValue() );
  unsigned int lPortNumber;
  lISS >> lPortNumber;

  // Create and start the receiver
  try {
    mReceiver = new TransportTCPIPReceiver(lPortNumber);
  }
  catch (PACC::Socket::Exception& inException) {
    throw Beagle_RunTimeExceptionM
      (string("An exception was thrown while creating the TCPIP receiver thread.")+
       string("  The exception message was: ")+inException.getMessage().c_str());
  }
  mReceiver->run();
}


void 
TransportReceiveTCPIPOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  if ( mReceiver==NULL ) return;
  ioStreamer.openTag(getName().c_str(), inIndent);
  ioStreamer.insertStringContent( uint2str(mReceiver->getPortNumber()).c_str(), "Port");
  ioStreamer.closeTag(); // getName()
}
