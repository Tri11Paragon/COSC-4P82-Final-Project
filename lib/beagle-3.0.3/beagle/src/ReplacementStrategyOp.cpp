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
 *  \file   beagle/src/ReplacementStrategyOp.cpp
 *  \brief  Implementation of class ReplacementStrategyOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:21 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>

using namespace Beagle;


/*!
 *  \brief Build replacement strategy operator.
 *  \param inName Name of the replacement strategy operator.
 */
ReplacementStrategyOp::ReplacementStrategyOp(Beagle::string inName) :
  Operator(inName)
{ }


/*!
 *  \brief Build routing tables for breeding children.
 *  \param outRoulette Normalized probability tables.
 *  \param ioContext Evolutionary context used.
 */
void ReplacementStrategyOp::buildRoulette(RouletteT<unsigned int>& outRoulette,
                                          Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(getRootNode());

  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::ReplacementStrategyOp",
    "Building routing tables of the replacement strategy"
  );

  outRoulette.clear();
  unsigned int i=0;
  for(BreederNode::Handle lChild=getRootNode(); lChild!=NULL; lChild=lChild->getNextSibling()) {
    outRoulette.insert(i++, lChild->getBreederOp()->getBreedingProba(lChild->getFirstChild()));
  }

  if(std::fabs(1.0 - outRoulette.back().first) > 0.01) {
    Beagle_LogInfoM(
      ioContext.getSystem().getLogger(),
      "replacement-strategy", "Beagle::ReplacementStrategyOp",
      string("Sum of probabilities of breeder operators children to ") +
      string("replacement strategy named \"") + getName() +
      string("\" is different from 1.0 (value: ") + dbl2str(outRoulette.back().first) +
      string(")")
    );
  }

  outRoulette.optimize();
  Beagle_StackTraceEndM("void ReplacementStrategyOp::buildRoulette(RouletteT<unsigned int>& outRoulette, Context& ioContext) const");
}


/*!
 *  \brief Initialize the operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void ReplacementStrategyOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(getRootNode() != NULL) getRootNode()->initialize(ioSystem);
  Beagle_StackTraceEndM("void ReplacementStrategyOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Call post initialization hook of the operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void ReplacementStrategyOp::postInit(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(getRootNode() != NULL) getRootNode()->postInit(ioSystem);
  Beagle_StackTraceEndM("void ReplacementStrategyOp::postInit(System& ioSystem)");
}


/*!
 *  \brief Read a replacement strategy operator for XML subtree.
 *  \param inIter XML iterator to use to read replacement strategy operator.
 *  \param inOpMap Operator map to use to read replacement strategy operator.
 */
void ReplacementStrategyOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }

  setRootNode(NULL);
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      OperatorMap::iterator lIterOp = inOpMap.find(lChild->getValue().c_str());
      if(lIterOp == inOpMap.end()) continue;

      BreederOp::Handle lOp;
      try {
        lOp = castHandleT<BreederOp>(lIterOp->second);
      }
      catch(BadCastException&) {
        std::ostringstream lOSS;
        lOSS << "Operator \"" << lChild->getValue() << "\" is not a breeder operator!";
        throw Beagle_IOExceptionNodeM(*lChild, lOSS.str().c_str());
      }
      lOp = castHandleT<BreederOp>(lOp->giveReference());

      if(getRootNode() == NULL) {
        setRootNode(new BreederNode(lOp));
        getRootNode()->readWithMap(lChild, inOpMap);
      }
      else {
        BreederNode::Handle lBreederChild;
        for(lBreederChild=getRootNode(); lBreederChild->getNextSibling()!=NULL;
            lBreederChild=lBreederChild->getNextSibling());
        lBreederChild->setNextSibling(new BreederNode(lOp));
        lBreederChild->getNextSibling()->readWithMap(lChild, inOpMap);
      }
    }
  }
  Beagle_StackTraceEndM("void ReplacementStrategyOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write replacement strategy operator into XML streamer.
 *  \param ioStreamer XML streamer to write replacement strategy operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void ReplacementStrategyOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag(getName().c_str(), inIndent);
  for(BreederNode::Handle lNode=getRootNode(); lNode!=NULL; lNode=lNode->getNextSibling()) {
    lNode->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void ReplacementStrategyOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

