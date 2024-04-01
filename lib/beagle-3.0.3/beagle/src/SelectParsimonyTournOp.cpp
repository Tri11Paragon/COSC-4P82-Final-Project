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
 *  \file   beagle/src/SelectParsimonyTournOp.cpp
 *  \brief  Source code of class SelectParsimonyTournOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4.2.1 $
 *  $Date: 2007/05/09 01:51:21 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a lexicographical parsimony pressure tournament selection operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the tournament selection operator.
 */
SelectParsimonyTournOp::SelectParsimonyTournOp(Beagle::string inReproProbaName, 
											   Beagle::string inName) :
  SelectionOp(inReproProbaName, inName),
  mNumberParticipants(NULL)
{ }


/*!
 *  \brief Initialize the parsimony tournament selection operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void SelectParsimonyTournOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  SelectionOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ec.sel.tournsize")) {
    mNumberParticipants =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.sel.tournsize"));
  } else {
    mNumberParticipants = new UInt(2);
    Register::Description lDescription(
      "Selection tournaments size",
      "UInt",
      "2",
      "Number of participants for tournament selection."
    );
    ioSystem.getRegister().addEntry("ec.sel.tournsize", mNumberParticipants, lDescription);
  }
  Beagle_StackTraceEndM("void SelectParsimonyTournOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Select an individual using the parsimony tournament selection method.
 *  \param ioPool Individual pool to use for selection.
 *  \param ioContext Context of the evolution.
 *  \return Index of the choosen individual in the pool.
 */
unsigned int SelectParsimonyTournOp::selectIndividual(Individual::Bag& ioPool, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_ValidateParameterM(mNumberParticipants->getWrappedValue() > 0,
                            "ec.sel.tournsize", ">0");

  unsigned int lChoosenIndividual =
    ioContext.getSystem().getRandomizer().rollInteger(0,((unsigned int)ioPool.size())-1);
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "selection", "Beagle::SelectParsimonyTournOp",
    string("Starting by choosing the ")+
    uint2ordinal(lChoosenIndividual+1)+" individual"
  );

  for(unsigned int j=1; j<mNumberParticipants->getWrappedValue(); j++) {
    unsigned int lTriedIndividual =
      ioContext.getSystem().getRandomizer().rollInteger(0,((unsigned int)ioPool.size())-1);
    if(ioPool[lChoosenIndividual]->isLess(*ioPool[lTriedIndividual])) {
      lChoosenIndividual = lTriedIndividual;
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "selection", "Beagle::SelectParsimonyTournOp",
        string("Trying the ")+uint2ordinal(lTriedIndividual+1)+
        " individual -> choosing it"
      );
    }
    else if(ioPool[lChoosenIndividual]->isEqual(*ioPool[lTriedIndividual])) {
      if(ioPool[lTriedIndividual]->getSize() < ioPool[lChoosenIndividual]->getSize()) {
        lChoosenIndividual = lTriedIndividual;
        Beagle_LogDebugM(
          ioContext.getSystem().getLogger(),
          "selection", "Beagle::SelectParsimonyTournOp",
          string("Trying the ")+uint2ordinal(lTriedIndividual+1)+
          " individual -> choosing it as it is smaller (parsimony)"
        );
      }
      else {
        Beagle_LogDebugM(
          ioContext.getSystem().getLogger(),
          "selection", "Beagle::SelectParsimonyTournOp",
          string("Trying the ")+uint2ordinal(lTriedIndividual+1)+
          " individual -> the previously choosen one is smaller (parsimony)"
        );
      }
    }
    else {
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "selection", "Beagle::SelectParsimonyTournOp",
        string("Trying the ")+uint2ordinal(lTriedIndividual+1)+
        " individual -> the previously choosen one is better"
      );
    }
  }

  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "selection", "Beagle::SelectParsimonyTournOp",
    string("Selecting the ")+uint2ordinal(lChoosenIndividual+1)+
    " individual"
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "selection", "Beagle::SelectParsimonyTournOp",
    string("Selected individual is: ")+
    ioPool[lChoosenIndividual]->serialize()
  );

  return lChoosenIndividual;
  Beagle_StackTraceEndM("unsigned int SelectParsimonyTournOp::selectIndividual(Individual::Bag& ioPool, Context& ioContext)");
}
