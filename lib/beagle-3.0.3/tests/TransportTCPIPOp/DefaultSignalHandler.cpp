#include "DefaultSignalHandler.hpp"

DefaultSignalHandler::DefaultSignalHandler() 
{
  setCustomAction(eSigInt);
  setCustomAction(eSigTerm);
}

DefaultSignalHandler::~DefaultSignalHandler() 
{}

void 
DefaultSignalHandler::main(SignalType inSignalNumber)
{
  switch(inSignalNumber) {
    case eSigInt:
    case eSigTerm:
      throw "This is the default signal handler cleaning things up.";
      break;
    default:
      break;
  }
}
