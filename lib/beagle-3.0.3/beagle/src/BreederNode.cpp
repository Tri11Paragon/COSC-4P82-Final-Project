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
 *  \file   beagle/src/BreederNode.cpp
 *  \brief  Source code of class BreederNode.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct breeder node.
 *  \param inBreederOp Breeder operator associated to node.
 *  \param inFirstChild First breeder node child to actual node.
 *  \param inNextSibling Next sibling breeder node to actual node.
 */
BreederNode::BreederNode(PointerT<BreederOp,Operator::Handle> inBreederOp,
                         PointerT<BreederNode,Object::Handle> inFirstChild,
                         PointerT<BreederNode,Object::Handle> inNextSibling) :
  mBreederOp(inBreederOp),
  mChild(inFirstChild),
  mSibling(inNextSibling)
{ }


/*!
 *  \brief Initialize breeder node structure.
 *  \param ioSystem Evolutionary system to use to initialize the system.
 */
void BreederNode::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if((mBreederOp != NULL) && (mBreederOp->isInitialized() == false)) {
    Beagle_LogTraceM(
      ioSystem.getLogger(),
      "breeder", "Beagle::BreederNode",
      string("Initializing operator \"") + mBreederOp->getName() + "\""
    );
    mBreederOp->initialize(ioSystem);
    mBreederOp->setInitialized();
  }
  if(mChild != NULL) mChild->initialize(ioSystem);
  if(mSibling != NULL) mSibling->initialize(ioSystem);
  Beagle_StackTraceEndM("void BreederNode::initialize(System& ioSystem)");
}


/*!
 *  \brief Call post initialize hooks of breeder node structure.
 *  \param ioSystem Evolutionary system to use to post-init the system.
 */
void BreederNode::postInit(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if((mBreederOp != NULL) && (mBreederOp->isPostInitialized() == false)) {
    Beagle_LogTraceM(
      ioSystem.getLogger(),
      "breeder", "Beagle::BreederNode",
      string("Calling post-initialization hook of operator \"") +
      mBreederOp->getName() + "\""
    );
    mBreederOp->postInit(ioSystem);
    mBreederOp->setPostInitialized();
  }
  if(mChild != NULL) mChild->postInit(ioSystem);
  if(mSibling != NULL) mSibling->postInit(ioSystem);
  Beagle_StackTraceEndM("void BreederNode::postInit(System& ioSystem)");
}


/*!
 *  \brief Reading a breeder node with method read is undefined. Use readWithMap instead.
 */
void BreederNode::read(PACC::XML::ConstIterator)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("read","BreederNode",getName());
  Beagle_StackTraceEndM("void BreederNode::read(PACC::XML::ConstIterator)");
}


/*!
 *  \brief Read a breeder tree for XML tree.
 *  \param inIter XML iterator to use to read breeder node.
 *  \param inOpMap Operator map to use to read breeder node.
 */
void BreederNode::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  setBreederOp(NULL);
  setFirstChild(NULL);
  setNextSibling(NULL);

  if(inIter->getType() == PACC::XML::eData) {
    OperatorMap::iterator lIterOp = inOpMap.find(inIter->getValue().c_str());
    if(lIterOp == inOpMap.end()) {
      std::ostringstream lOSS;
      lOSS << "Operator \"" << inIter->getValue() << "\" is not a breeder operator!";
      throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
    }
    try {
      mBreederOp = castHandleT<BreederOp>(lIterOp->second);
    }
    catch(BadCastException&) {
      std::ostringstream lOSS;
      lOSS << "Operator \"" << inIter->getValue() << "\" is not a breeder operator!";
      throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
    }
    mBreederOp = castHandleT<BreederOp>(mBreederOp->giveReference());
    mBreederOp->readWithMap(inIter, inOpMap);
  }

  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      OperatorMap::iterator lIterOp = inOpMap.find(lChild->getValue().c_str());
      if(lIterOp == inOpMap.end()) continue;

      PointerT<BreederOp,Operator::Handle> lOp;
      try {
        lOp = castHandleT<BreederOp>(lIterOp->second);
      }
      catch(BadCastException&) {
        std::ostringstream lOSS;
        lOSS << "Operator \"" << lChild->getValue() << "\" is not a breeder operator!";
        throw Beagle_IOExceptionNodeM(*lChild, lOSS.str().c_str());
      }
      lOp = castHandleT<BreederOp>(lOp->giveReference());
      if(getFirstChild() == NULL) {
        setFirstChild(new BreederNode(lOp));
        getFirstChild()->readWithMap(lChild, inOpMap);
      }
      else {
        PointerT<BreederNode,Object::Handle> lBreederChild;
        for(lBreederChild=getFirstChild(); lBreederChild->getNextSibling()!=NULL;
            lBreederChild=lBreederChild->getNextSibling());
        lBreederChild->setNextSibling(new BreederNode(lOp));
        lBreederChild->getNextSibling()->readWithMap(lChild, inOpMap);
      }
    }
  }
  Beagle_StackTraceEndM("void BreederNode::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write breeder operator into XML streamer.
 *  \param ioStreamer XML streamer to write breeder operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void BreederNode::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  if(mBreederOp != NULL) {
    ioStreamer.openTag(mBreederOp->getName().c_str(), inIndent);
    mBreederOp->writeContent(ioStreamer, inIndent);
  }
  for(BreederNode::Handle lChild=getFirstChild(); lChild!=NULL;
      lChild=lChild->getNextSibling()) {
    lChild->write(ioStreamer, inIndent);
  }
  if(mBreederOp != NULL) ioStreamer.closeTag();
  Beagle_StackTraceEndM("void BreederNode::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

