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
 *  \file   beagle/GA/src/MutationFlipBitStrOp.cpp
 *  \brief  Source code of class GA::MutationFlipBitStrOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14.2.1 $
 *  $Date: 2007/05/09 01:51:03 $
 */

#include "beagle/GA.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a bit string GA flip bit mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inBitMutatePbName Mutation bit probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::MutationFlipBitStrOp::MutationFlipBitStrOp(Beagle::string inMutationPbName,
                                               Beagle::string inBitMutatePbName,
                                               Beagle::string inName) :
  Beagle::MutationOp(inMutationPbName, inName),
  mBitMutatePbName(inBitMutatePbName)
{ }


/*!
 *  \brief Initialize the bit string GA flip bit mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationFlipBitStrOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::initialize(ioSystem);
  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    ioSystem.getRegister().deleteEntry(mMutationPbName);
  }

  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    mMutationProba = castHandleT<Float>(ioSystem.getRegister()[mMutationPbName]);
  } else {
    mMutationProba = new Float(float(1.0));
    Register::Description lProbaDescription(
      "Individual flip mutation prob.",
      "Float",
      "1.0",
      "Bit flip mutation probability for each GA individual."
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lProbaDescription);
  }

  if(ioSystem.getRegister().isRegistered(mBitMutatePbName)) {
    mBitMutateProba = castHandleT<Float>(ioSystem.getRegister()[mBitMutatePbName]);
  } else {
    mBitMutateProba = new Float(float(0.01));
    Register::Description lBitDescription(
      "Flip mutation probability",
      "Float",
      "0.01",
      "Probability for each bit to be inverted by mutation, when an individual is mutated."
    );
    ioSystem.getRegister().addEntry(mBitMutatePbName, mBitMutateProba, lBitDescription);
  }
  Beagle_StackTraceEndM("void GA::MutationFlipBitStrOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Bit flip mutate a bit string GA individual.
 *  \param ioIndividual GA individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GA::MutationFlipBitStrOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_ValidateParameterM(mBitMutateProba->getWrappedValue()>=0.0, mBitMutatePbName, "<0");
  bool lMutated = false;
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GA::MutationFlipBitStrOp",
    string("Bit flip mutation probability is: ")+
    dbl2str(mBitMutateProba->getWrappedValue())
  );

  for(unsigned int i=0; i<ioIndividual.size(); i++) {
    GA::BitString::Handle lBS = castHandleT<GA::BitString>(ioIndividual[i]);
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationFlipBitStrOp",
      string("Flip mutating the ")+uint2ordinal(i+1)+" bitstring"
    );
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationFlipBitStrOp",
      string("The bitstring before GA flip bit mutation: ")+lBS->serialize()
    );
    for(unsigned int j=0; j<lBS->size(); j++) {
      double lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
      if(lRolledPb <= mBitMutateProba->getWrappedValue()) {
        (*lBS)[j] = !(*lBS)[j];
        lMutated = true;
      }
    }
    if(lMutated) {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationFlipBitStrOp",
        string("The bitstring has been flip mutated")
      );
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationFlipBitStrOp",
        string("The bitstring after GA flip bit mutation: ")+lBS->serialize()
      );
    }
    else {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationFlipBitStrOp",
        string("The bitstring has not been mutated")
      );
    }
  }
  return lMutated;
  Beagle_StackTraceEndM("bool GA::MutationFlipBitStrOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)");
}


/*!
 *  \brief Read a bit flip mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read mutation operator.
 *  \param inOpMap Operator map to use to read mutation operator.
 */
void GA::MutationFlipBitStrOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lMutationPbReadName = inIter->getAttribute("mutationpb").c_str();
  if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
  string lBitMutatePbReadName = inIter->getAttribute("mutbitpb").c_str();
  if(lBitMutatePbReadName.empty() == false) mBitMutatePbName = lBitMutatePbReadName;
  Beagle_StackTraceEndM("void GA::MutationFlipBitStrOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write flip bit mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void GA::MutationFlipBitStrOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::writeContent(ioStreamer, inIndent);
  ioStreamer.insertAttribute("mutbitpb", mBitMutatePbName);
  Beagle_StackTraceEndM("void GA::MutationFlipBitStrOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

