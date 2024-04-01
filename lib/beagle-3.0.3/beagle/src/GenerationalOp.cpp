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
 *  \file   beagle/src/GenerationalOp.cpp
 *  \brief  Source code of class GenerationalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Build generational operator.
 *  \param inName Name of the generational operator.
 */
GenerationalOp::GenerationalOp(Beagle::string inName) :
  ReplacementStrategyOp(inName)
{ }


/*!
 *  \brief Initialize the operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void GenerationalOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  ReplacementStrategyOp::initialize(ioSystem); 

  if(ioSystem.getRegister().isRegistered("ec.elite.keepsize")) {
    mElitismKeepSize =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.elite.keepsize"));
  } else {
    mElitismKeepSize = new UInt(1);
    Register::Description lDescription(
      "Elitism keep size",
      "UInt",
      "1",
      "Number of individuals keep as is with strong n-elitism."
    );
    ioSystem.getRegister().addEntry("ec.elite.keepsize", mElitismKeepSize, lDescription);
  }
  Beagle_StackTraceEndM("void GenerationalOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the operation on a deme in the given context.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void GenerationalOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(getRootNode());
  Beagle_NonNullPointerAssertM(mElitismKeepSize);
  Beagle_ValidateParameterM(mElitismKeepSize->getWrappedValue() <= ioDeme.size(),
                            "ec.elite.keepsize",
                            "The elistism keepsize must be less than the deme size!"); 

  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::GenerationalOp",
    string("Processing using generational replacement strategy the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  Beagle_LogObjectM(
    ioContext.getSystem().getLogger(),
    Logger::eTrace,
    "replacement-strategy", "Beagle::GenerationalOp",
    (*this)
  );

  RouletteT<unsigned int> lRoulette;
  buildRoulette(lRoulette, ioContext);

  Individual::Bag lOffsprings;
  if(mElitismKeepSize->getWrappedValue() > 0) {
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
    for(unsigned int i=0; i<mElitismKeepSize->getWrappedValue(); ++i) {
      lOffsprings.push_back(ioDeme[0]);
      std::pop_heap(ioDeme.begin(), (ioDeme.end()-i), IsLessPointerPredicate());
    }
  }
  for(unsigned int i=mElitismKeepSize->getWrappedValue(); i<ioDeme.size(); ++i) {
    unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
    BreederNode::Handle lSelectedBreeder=getRootNode();
    for(unsigned int j=0; j<lIndexBreeder; ++j)
      lSelectedBreeder=lSelectedBreeder->getNextSibling();
    Beagle_NonNullPointerAssertM(lSelectedBreeder);
    Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
    Individual::Handle lBredIndiv =
      lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
    Beagle_NonNullPointerAssertM(lBredIndiv);
    lOffsprings.push_back(lBredIndiv);
  }

  for(unsigned int j=0; j<lOffsprings.size(); ++j) ioDeme[j] = lOffsprings[j];
  Beagle_StackTraceEndM("void GenerationalOp::operate(Deme& ioDeme, Context& ioContext)");
}


