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
 *  \file   beagle/src/TermMaxFitnessOp.cpp
 *  \brief  Source code of class TermMaxFitnessOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.2 $
 *  $Date: 2007/08/21 02:07:24 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a termination check operator based on a maximum fitness value.
 *  \param inMaxFitness Default maximum fitness value.
 *  \param inName Name of the operator.
 */
TermMaxFitnessOp::TermMaxFitnessOp(float inMaxFitness, Beagle::string inName) :
  TerminationOp(inName),
  mMaxFitnessDefault(inMaxFitness)
{ }


/*!
 *  \brief Initialize the maximum fitness value termination operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void TermMaxFitnessOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ec.term.maxfitness")) {
    mMaxFitness =
      castHandleT<Float>(ioSystem.getRegister().getEntry("ec.term.maxfitness"));
  } else {
    mMaxFitness = new Float(mMaxFitnessDefault);
    Register::Description lDescription(
      "Max fitness term criterion",
      "Float",
      dbl2str(mMaxFitnessDefault),
      "Fitness value to reach before stopping evolution."
    );
    ioSystem.getRegister().addEntry("ec.term.maxfitness", mMaxFitness, lDescription);
  }
  Beagle_StackTraceEndM("void TermMaxFitnessOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Read a max fitness termination operator from a XML subtree.
 *  \param inIter XML iterator from which the operator is read.
 *  \param inOpMap Operator map to use to read operator.
 *  \throw IOException If a reading error occurs.
 */
void TermMaxFitnessOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lFitnessStr = inIter->getAttribute("fitness").c_str();
  if(lFitnessStr.empty() == false) {
    mMaxFitnessDefault = str2dbl(lFitnessStr);
    if(mMaxFitness != NULL) mMaxFitness->getWrappedValue() = mMaxFitnessDefault;
  }
  Beagle_StackTraceEndM("void TermMaxFitnessOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Check if the maximum fitness value is reached.
 *  \param inDeme Actual deme of the evolution.
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool TermMaxFitnessOp::terminate(const Deme& inDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  for(unsigned int i=0; i<inDeme.size(); ++i) {
    FitnessSimple::Handle lFitness =
      castHandleT<FitnessSimple>(inDeme[i]->getFitness());
    if(lFitness->isValid() && (lFitness->getValue() >= mMaxFitness->getWrappedValue())) {
      Beagle_LogInfoM(
        ioContext.getSystem().getLogger(),
        "termination", "Beagle::TermMaxFitnessOp",
        string("Maximum fitness value (") +
        dbl2str(mMaxFitness->getWrappedValue()) +
        string(") termination criterion reached by the ") +
        uint2ordinal(i+1) + string(" individual (") +
        dbl2str(lFitness->getValue()) + string(")")
      );
      return true;
    }
  }

  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "termination", "Beagle::TermMaxFitnessOp",
    string("Maximum fitness value (") +
    dbl2str(mMaxFitness->getWrappedValue()) +
    string(") termination criterion not reached ")  
  );
  return false;
  Beagle_StackTraceEndM("bool TermMaxFitnessOp::terminate(const Deme& inDeme, Context& ioContext)");
}


/*!
 *  \brief Write a max fitness termination operator into a XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void TermMaxFitnessOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag(getName().c_str(), inIndent);
  if(mMaxFitness != NULL)
    ioStreamer.insertAttribute("fitness", dbl2str(mMaxFitness->getWrappedValue()));
  else ioStreamer.insertAttribute("fitness", dbl2str(mMaxFitnessDefault));
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void TermMaxFitnessOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
