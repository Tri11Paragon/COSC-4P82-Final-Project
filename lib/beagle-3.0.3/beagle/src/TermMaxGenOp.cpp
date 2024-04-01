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
 *  \file   beagle/src/TermMaxGenOp.cpp
 *  \brief  Source code of class TermMaxGenOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.2 $
 *  $Date: 2007/05/11 20:04:32 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a termination check operator based on a maximum generation value.
 *  \param inName Name of the operator.
 */
TermMaxGenOp::TermMaxGenOp(Beagle::string inName) :
  TerminationOp(inName)
{ }


/*!
 *  \brief Initialize the maximum generation termination operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void TermMaxGenOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ec.term.maxgen")) {
    mMaxGeneration =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.term.maxgen"));
  } else {
    mMaxGeneration = new UInt(50);
    Register::Description lDescription(
      "Max generation term criterion",
      "UInt",
      "50",
      "Maximum number of generations for the evolution."
    );
    ioSystem.getRegister().addEntry("ec.term.maxgen", mMaxGeneration, lDescription);
  }
  Beagle_StackTraceEndM("void TermMaxGenOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Check if the maximum number of generation is reached.
 *  \param inDeme Actual deme of the evolution.
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool TermMaxGenOp::terminate(const Deme& inDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(ioContext.getGeneration() > mMaxGeneration->getWrappedValue()) {
    Beagle_LogInfoM(
      ioContext.getSystem().getLogger(),
      "termination", "Beagle::TermMaxGenOp",
      std::string("Maximum number of generations (") +
      uint2str(mMaxGeneration->getWrappedValue()) +
      std::string(") termination criterion overpassed")
    );
    Beagle_LogDetailedM(
      ioContext.getSystem().getLogger(),
      "termination", "Beagle::TermMaxGenOp",
      std::string("Actual generation number is: ")+uint2str(ioContext.getGeneration())
    );
    return true;
  }
  if((ioContext.getGeneration() == mMaxGeneration->getWrappedValue()) &&
     (ioContext.getDemeIndex() == (ioContext.getVivarium().size()-1))) {
    Beagle_LogInfoM(
      ioContext.getSystem().getLogger(),
      "termination", "Beagle::TermMaxGenOp",
      std::string("Maximum number of generations (") +
      uint2str(mMaxGeneration->getWrappedValue()) +
      std::string(") termination criterion reached")
    );
    return true;
  }
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "termination", "Beagle::TermMaxGenOp",
    std::string("Maximum number of generations (") +
    uint2str(mMaxGeneration->getWrappedValue()) +
    std::string(") termination criterion not reached ")
  );
  return false;
  Beagle_StackTraceEndM("bool TermMaxGenOp::terminate(const Deme& inDeme, Context& ioContext)");
}


