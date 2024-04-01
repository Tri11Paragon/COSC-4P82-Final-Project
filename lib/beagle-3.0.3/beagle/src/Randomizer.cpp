/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/src/Randomizer.cpp
 *  \brief  Source code of class Randomizer.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.2 $
 *  $Date: 2007/05/11 19:13:09 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a randomizer.
 *  \param inSeed Random number generator seed, can be overwritten by the registered seed.
 */
Randomizer::Randomizer(unsigned long inSeed) :
  Component("Randomizer"),
  PACC::Randomizer(inSeed),
  mRegisteredSeed(NULL),
  mSeed(*state)
{ }


/*!
 *  \brief Initialize the randomizer. The initial seed value is the timer.
 *  \param ioSystem Reference to the system.
 */
void Randomizer::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    ioSystem.getLogger(),
    "random", "Beagle::Randomizer",
    "Initializing randomizer"
  );

 if(ioSystem.getRegister().isRegistered("ec.rand.seed")) {
    mRegisteredSeed = castHandleT<ULong>(ioSystem.getRegister().getEntry("ec.rand.seed"));
  } else {
    mRegisteredSeed = new ULong(0);
    string lLongDescrip = "Randomizer seed. A zero value means that ";
#ifdef BEAGLE_HAVE_DEV_URANDOM
    lLongDescrip += "the seed will be initialized using the /dev/urandom device.";
#else // BEAGLE_HAVE_DEV_URANDOM
    lLongDescrip += "the seed will be initialized using the current system time.";
#endif // BEAGLE_HAVE_DEV_URANDOM
    Register::Description lDescription(
      "Randomizer seed",
      "ULong",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.rand.seed", mRegisteredSeed, lDescription);
  }
  Beagle_StackTraceEndM("void Randomizer::initialize(System& ioSystem)");
}


/*!
 *  \brief Post-initializing the randomizer.
 *  \param ioSystem Evolutionary system.
 */
void Randomizer::postInit(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    ioSystem.getLogger(),
    "random", "Beagle::Randomizer",
    "Post-initializing randomizer"
  );

  if(mRegisteredSeed->getWrappedValue() == 0) {
#ifdef BEAGLE_HAVE_DEV_URANDOM
    std::ifstream lURandom("/dev/urandom", std::ios::binary);
    lURandom.read((char*)&mSeed, sizeof(unsigned long));
    lURandom.close();
#else // BEAGLE_HAVE_DEV_URANDOM
    mSeed = MTRand::hash(time(NULL), clock());
#endif // BEAGLE_HAVE_DEV_URANDOM
    MTRand::seed(mSeed);
    mRegisteredSeed->getWrappedValue() = mSeed;
  }
  else if(mRegisteredSeed->getWrappedValue() != mSeed) {
    seed(mRegisteredSeed->getWrappedValue());
    mSeed = mRegisteredSeed->getWrappedValue();
  }
  Beagle_StackTraceEndM("void Randomizer::postInit(System& ioSystem)");
}


/*!
 *  \brief Read the state of the randomizer from a XML subtree.
 *  \param inIter XML iterator to read the mersenne twister randomizer from.
 *  \param ioSystem Evolutionary system used to read randomizer.
 */
void Randomizer::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Randomizer"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Randomizer> expected!");

  string lSeedStr = inIter->getAttribute("seed");
  if(lSeedStr.empty())
    throw Beagle_IOExceptionNodeM(*inIter, "expected randomizer seed!");
  mSeed = str2uint(lSeedStr);

  if(mSeed!=0) {
    PACC::XML::ConstIterator lChild = inIter->getFirstChild();
    if((!lChild) || (lChild->getType()!=PACC::XML::eString))
      throw Beagle_IOExceptionNodeM(*lChild, "expected randomizer state!");
    setState(lChild->getValue());
  }
  Beagle_StackTraceEndM("void Randomizer::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)");
}


/*!
 *  \brief Write the state of the randomizer into a XML streamer.
 *  \param ioStreamer XML streamer to write the randomizer into.
 *  \param inIndent Whether XML output should be indented.
 */
void Randomizer::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.insertAttribute("seed", uint2str(mSeed));
  if(mSeed!=0) ioStreamer.insertStringContent(getState());
  Beagle_StackTraceEndM("void Randomizer::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
