#include "beagle/Beagle.hpp"

#include <iostream>

using namespace std;

int main() {
  try {
    Beagle_StackTraceBeginM();
    Beagle_StackTraceBeginM();
    throw Beagle_RunTimeExceptionM("This is a test");
    Beagle_StackTraceEndM("Inner");
    Beagle_StackTraceEndM("Outer");
  }
  catch (Beagle::Exception& inException) {
    cout << "Caught Beagle::Exception:" << endl;
    inException.explain(cerr);
  }
}
