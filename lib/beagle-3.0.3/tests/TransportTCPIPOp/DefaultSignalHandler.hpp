#ifndef DefaultSignalHandler_hpp
#define DefaultSignalHandler_hpp

#include "Util.hpp"

class DefaultSignalHandler : public PACC::SignalHandler {

public:

  DefaultSignalHandler();
  virtual ~DefaultSignalHandler();

  virtual void main(SignalType inSignalNumber);

};

#endif // DefaultSignalHandler_hpp
