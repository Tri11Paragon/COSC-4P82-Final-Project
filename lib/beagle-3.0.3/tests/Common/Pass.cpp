#include "Pass.hpp"

using namespace std;
using namespace Beagle;
using namespace Test;

const static int Pass = 0;
const static int Fail = 1;


int pass(std::string inMessage)
{
  cerr << "PASS (" << inMessage << ")" << endl;
  return Pass;
}

int fail(std::string inMessage)
{
  cerr << "FAIL (" << inMessage << ")" << endl;
  return Fail;
}
