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
 *  \file   beagle/GP/src/TermMaxHitsOp.cpp
 *  \brief  Implementation of the class TermMaxHitsOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:07 $
 */

#include "beagle/GP.hpp"

#include <cmath>

using namespace Beagle;


/*!
 *  \brief Construct a new TermMaxHitsOp termination operator.
 *  \param inMaxHits Number of hits required to stop the evolution.
 *  \param inName Name of the evaluation operator.
 */
GP::TermMaxHitsOp::TermMaxHitsOp(unsigned int inMaxHits, Beagle::string inName) :
  TerminationOp(inName),
  mMaxHits(NULL),
  mMaxHitsDefault(inMaxHits)
{ }


/*!
 *  \brief Initialize the TermMaxHitsOp termination operator.
 *  \param ioSystem System used to initialize the operator.
 */
void GP::TermMaxHitsOp::initialize(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  TerminationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("gp.term.maxhits")) {
    mMaxHits = castHandleT<UInt>(ioSystem.getRegister()["gp.term.maxhits"]);
  }
  else {
    mMaxHits = new UInt(mMaxHitsDefault);
    std::ostringstream lOSS;
    lOSS << "Number of hits required in an individual ";
    lOSS << "in order for the evolution process to terminate. ";
    lOSS << "If 0, termination criteria is ignored.";
    Register::Description lDescription(
      "Max hits term criterion",
      "UInt",
      uint2str(mMaxHitsDefault),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("gp.term.maxhits", mMaxHits, lDescription);
  }
  Beagle_StackTraceEndM("void GP::TermMaxHitsOp::initialize(Beagle::System& ioSystem)");
}


/*!
 *  \brief Read a max hits termination operator from a XML subtree.
 *  \param inIter XML iterator from which the operator is read.
 *  \param inOpMap Operator map to use to read operator.
 *  \throw IOException If a reading error occurs.
 */
void GP::TermMaxHitsOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lHitsStr = inIter->getAttribute("hits").c_str();
  if(lHitsStr.empty() == false) {
    mMaxHitsDefault = str2uint(lHitsStr);
    if(mMaxHits != NULL) mMaxHits->getWrappedValue() = mMaxHitsDefault;
  }
  Beagle_StackTraceEndM("void GP::TermMaxHitsOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Check if the evolution must terminate.
 *  \param inDeme Actual deme of the evolution.
 *  \param ioContext Actual evolution context.
 *  \return True if the ending criterion is reached, false if not.
 */
bool GP::TermMaxHitsOp::terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(mMaxHits->getWrappedValue() == 0) return false;
  for(unsigned int i=0; i<inDeme.size(); i++) {
    const GP::FitnessKoza::Handle lFitness =
      castHandleT<const GP::FitnessKoza>(inDeme[i]->getFitness());
    if(mMaxHits->getWrappedValue() <= lFitness->getHits()) {
      Beagle_LogInfoM(
        ioContext.getSystem().getLogger(),
        "termination", "Beagle::GP::TermMaxHitsOp",
        string("Maximum number of hits (") +
        uint2str(mMaxHits->getWrappedValue()) +
        string(") termination criterion reached")
      );
      Beagle_LogInfoM(  
        ioContext.getSystem().getLogger(),
        "termination", "Beagle::GP::TermMaxHitsOp",
        string("The ")+uint2ordinal(i+1) +
        string(" individual of the deme has ") +
        uint2str(lFitness->getHits())+string(" hits")
      ); 
      return true;
    }
  }
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "termination", "Beagle::GP::TermMaxHitsOp",
    string("Maximum number of hits (") +
    uint2str(mMaxHits->getWrappedValue()) +
    string(") termination criterion not reached")
  ); 
  return false;
  Beagle_StackTraceEndM("bool GP::TermMaxHitsOp::terminate(const Beagle::Deme& inDeme, Beagle::Context& ioContext)");
}


/*!
 *  \brief Write an max hits termination operator into a XML stream.
 *  \param ioStreamer XML streamer in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::TermMaxHitsOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag(getName().c_str(), inIndent);
  if(mMaxHits != NULL) ioStreamer.insertAttribute("hits", uint2str(mMaxHits->getWrappedValue()));
  else ioStreamer.insertAttribute("hits", uint2str(mMaxHitsDefault));
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void GP::TermMaxHitsOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


