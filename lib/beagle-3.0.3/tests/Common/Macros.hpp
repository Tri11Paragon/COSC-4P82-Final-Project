#ifndef Beagle_Test_Macro_hpp
#define Beagle_Test_Macro_hpp

#include "beagle/Exception.hpp"

#define Beagle_Test_CatchEverythingM \
catch ( Beagle::Exception& inException ) { \
  cerr << "A Beagle exception was thrown:" << endl; \
  inException.terminate(); \
} \
catch ( PACC::Socket::Exception& inException ) { \
  cerr << "A PACC::Socket exception was thrown:" << endl; \
  cerr << inException.getMessage() << endl; \
} \
catch ( std::exception& inException ) { \
  cerr << "A standard exception was thrown:" << endl; \
  cerr << inException.what(); \
} \
catch ( std::string& inException ) { \
  cerr << "A standard string was thrown:" << endl; \
  cerr << "Its contents are: \"" << inException << "\"" << endl; \
} \
catch ( char* inException ) { \
  cerr << "A character pointer was thrown:" << endl; \
  cerr << "Its contents are: \"" << inException << "\"" << endl; \
} \
catch ( int inException ) { \
  cerr << "An integer was thrown:" << endl; \
  cerr << "Its value is: \"" << inException << "\"" << endl; \
} \
catch ( ... ) { \
  cerr << "An unknown exception was thrown" << endl; \
} \


#endif // Beagle_Test_Macro_hpp
