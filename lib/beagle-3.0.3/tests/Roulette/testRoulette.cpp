#include "beagle/Beagle.hpp"
#include <iostream>


using namespace std;

int main(int argc, char** argv) 
{
  Beagle::System::Handle lSystem = new Beagle::System;
  lSystem->initialize(argc, argv);

  Beagle::Context lContext;
  lContext.setSystemHandle( lSystem );

  Beagle::RouletteT<string> lRoulette;
  lRoulette.insert("first", 2.0);
  lRoulette.insert("second", 1.0);
  lRoulette.insert("third", 0.5);

  cout << "Selection: " << lRoulette.select(lContext.getSystem().getRandomizer()) << endl;
  lRoulette.optimize();
  cout << "Selection: " << lRoulette.select(lContext.getSystem().getRandomizer()) << endl;

  cout << "Finished." << endl;
}
