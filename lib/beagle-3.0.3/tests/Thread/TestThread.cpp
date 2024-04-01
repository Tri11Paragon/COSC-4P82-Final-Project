#include <iostream>
#include <cstdio>
#include <string>

#include "Threading.hpp"
#include "beagle/Beagle.hpp"

using namespace std;
using namespace Beagle;
using namespace PACC;

class TestThread : public Threading::Thread {
public:
  TestThread() {}
  ~TestThread() { wait(); }

  void setChar(char inChar)
  { 
    mChar = inChar;
  }

protected:
  virtual void main()
  {
    while (true) {
      putchar(mChar);
      fflush(stdout);
    }
  }

private:
  char mChar;
};




int main(int argc, char** argv) {
  TestThread lThread1;
  lThread1.setChar('1');

  TestThread lThread2;
  lThread2.setChar('2');

  cout << "Starting thread one" << endl;
  lThread1.run();
  cout << "Starting thread two" << endl;
  lThread2.run();

  cout << "Finished." << endl;
}
