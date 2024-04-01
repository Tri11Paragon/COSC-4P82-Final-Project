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
 *  \file   beagle/src/IfThenElseOp.cpp
 *  \brief  Source code of class IfThenElseOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a if-then-else operator.
 *  \param inConditionTag Tag in the register of the parameter to test.
 *  \param inConditionValue Value the test of the value.
 *  \param inName Name of the operator.
 */
IfThenElseOp::IfThenElseOp(Beagle::string inConditionTag,
                           Beagle::string inConditionValue,
                           Beagle::string inName) :
  Operator(inName),
  mConditionTag(inConditionTag),
  mConditionValue(inConditionValue)
{ }


/*!
 *  \brief Return a new copy of if-then-else operator.
 *  \return Handle to new copy of if-then-else operator.
 */
Operator::Handle IfThenElseOp::giveReference()
{
  Beagle_StackTraceBeginM();
  return new IfThenElseOp;
  Beagle_StackTraceEndM("Operator::Handle IfThenElseOp::giveReference()");
}


/*!
 *  \brief Initialize the if-then-else operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void IfThenElseOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  for(unsigned int i=0; i<mPositiveOpSet.size(); i++) {
    if(mPositiveOpSet[i]->isInitialized() == false) {
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "if-then-else", "Beagle::Evolver",
        string("Initializing operator \"") + mPositiveOpSet[i]->getName() + "\""
      );
      mPositiveOpSet[i]->initialize(ioSystem);
      mPositiveOpSet[i]->setInitialized();
    }
  }
  for(unsigned int j=0; j<mNegativeOpSet.size(); j++) {
    if(mNegativeOpSet[j]->isInitialized() == false) {
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "if-then-else", "Beagle::IfThenElseOp",
        string("Initializing operator \"") + mNegativeOpSet[j]->getName() + "\""
      );
      mNegativeOpSet[j]->initialize(ioSystem);
      mNegativeOpSet[j]->setInitialized();
    }
  }
  Beagle_StackTraceEndM("void IfThenElseOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Insert an operator in the positive set of the if-then-else operator.
 *  \param inName Name of operator to insert.
 *  \param inOpMap Operator map to use to get the operator.
 *  \throw RunTimeOperator If named operator is not found in operator map.
 *  \warning Insert your operators before the initialization!
 */
void IfThenElseOp::insertPositiveOp(Beagle::string inName, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  OperatorMap::iterator lIterOp = inOpMap.find(inName);
  if(lIterOp == inOpMap.end()) {
    std::ostringstream lOSS;
    lOSS << "Operator \"" << inName << "\" not found in operator map!";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  Operator::Handle lOpHdl = castHandleT<Operator>(lIterOp->second);
  mPositiveOpSet.push_back(lOpHdl->giveReference());
  Beagle_StackTraceEndM("void IfThenElseOp::insertPositiveOp(string inName, OperatorMap& inOpMap)");
}


/*!
 *  \brief Insert an operator in the negative set of the if-then-else operator.
 *  \param inName Name of operator to insert.
 *  \param inOpMap Operator map to use to get the operator.
 *  \throw RunTimeOperator If named operator is not found in operator map.
 *  \warning Insert your operators before the initialization!
 */
void IfThenElseOp::insertNegativeOp(Beagle::string inName, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  OperatorMap::iterator lIterOp = inOpMap.find(inName);
  if(lIterOp == inOpMap.end()) {
    std::ostringstream lOSS;
    lOSS << "Operator \"" << inName << "\" not found in operator map!";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  Operator::Handle lOpHdl = castHandleT<Operator>(lIterOp->second);
  mNegativeOpSet.push_back(lOpHdl->giveReference());
  Beagle_StackTraceEndM("void IfThenElseOp::insertNegativeOp(string inName, OperatorMap& inOpMap)");
}


/*!
 *  \brief Apply the if-then-else operation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 *  \throw RunTimeException If condition tag does not to valid parameter.
 *  \par Note:
 *    The operator first look for the tagged parameter in the register. If the parameter
 *    is not in the register, RunTimeException is throw. If the value is registered,
 *    a test is made with the value contained in the operator. If the values are equal,
 *    then the positive operator set is executed, else the negative operator set is executed.
 */
void IfThenElseOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(ioContext.getSystem().getRegister().isRegistered(mConditionTag) == false) {
    std::ostringstream lOSS;
    lOSS << "Tag \"" << mConditionTag << "\" does not refer to a registered parameter!";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  Object::Handle lTestedObject = ioContext.getSystem().getRegister().getEntry(mConditionTag);
  string lTestedValue = lTestedObject->serialize();

  if(lTestedValue == mConditionValue) {
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "if-then-else", "Beagle::IfThenElseOp",
      string("Value of Boolean parameter \"")+
      mConditionTag + string("\" is equal to the condition \"")+
      mConditionValue + "\""
    );
    Beagle_LogDetailedM(
      ioContext.getSystem().getLogger(),
      "if-then-else", "Beagle::IfThenElseOp",
      "Applying positive operator set"
    );  
    for(unsigned int i=0; i<mPositiveOpSet.size(); i++) {
      Beagle_LogDetailedM(
        ioContext.getSystem().getLogger(),
        "if-then-else", "Beagle::IfThenElseOp",
        string("Applying \"")+mPositiveOpSet[i]->getName()+string("\"")
      ); 
      mPositiveOpSet[i]->operate(ioDeme, ioContext);
    }
  }
  else {
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "if-then-else", "Beagle::IfThenElseOp",
      string("Value of Boolean parameter \"")+mConditionTag+
      string("\", which is \"")+lTestedValue+
      string("\", is not equal to the condition (")+mConditionValue + ")"
    );
    Beagle_LogDetailedM(
      ioContext.getSystem().getLogger(),
      "if-then-else", "Beagle::IfThenElseOp",
      "Applying negative operator set"
    ); 
    for(unsigned int i=0; i<mNegativeOpSet.size(); i++) {
      Beagle_LogDetailedM(
        ioContext.getSystem().getLogger(),
        "if-then-else", "Beagle::IfThenElseOp",
        string("Applying \"")+mNegativeOpSet[i]->getName()+string("\"")
      ); 
      mNegativeOpSet[i]->operate(ioDeme, ioContext);
    }
  }
  Beagle_StackTraceEndM("void IfThenElseOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Post-initialization hook the if-then-else operator.
 *  \param ioSystem System to use to post-init the operator.
 */
void IfThenElseOp::postInit(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  for(unsigned int i=0; i<mPositiveOpSet.size(); i++) {
    if(mPositiveOpSet[i]->isPostInitialized() == false) {
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "if-then-else", "Beagle::IfThenElseOp",
        string("Calling post-initialization hook of operator \"") +
        mPositiveOpSet[i]->getName() + "\""
      );
      mPositiveOpSet[i]->postInit(ioSystem);
      mPositiveOpSet[i]->setPostInitialized();
    }
  }
  for(unsigned int j=0; j<mNegativeOpSet.size(); j++) {
    if(mNegativeOpSet[j]->isPostInitialized() == false) {
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "if-then-else", "Beagle::IfThenElseOp",
        string("Calling post-initialization hook of operator \"") +
        mNegativeOpSet[j]->getName() + "\""
      );
      mNegativeOpSet[j]->postInit(ioSystem);
      mNegativeOpSet[j]->setPostInitialized();
    }
  }
  Beagle_StackTraceEndM("void IfThenElseOp::postInit(System& ioSystem)");
}


/*!
 *  \brief Read if-then-else operator from XML subtree and operator map.
 *  \param inIter XML iterator from which the operator is read.
 *  \param inOpMap Operator map to use to read operator.
 *  \throw IOException If a reading error occurs.
 */
void IfThenElseOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  mConditionTag = inIter->getAttribute("parameter").c_str();
  if(mConditionTag.empty()) {
    std::ostringstream lOSS;
    lOSS << "attribute \"parameter\" for operator " << getName() << " expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  mConditionValue = inIter->getAttribute("value").c_str();

  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if(lChild->getValue() == "PositiveOpSet") {
        mPositiveOpSet.clear();
        for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
          if(lChild2->getType() == PACC::XML::eData) {
            OperatorMap::iterator lIterOp = inOpMap.find(lChild2->getValue().c_str());
            if(lIterOp == inOpMap.end()) {
              std::ostringstream lOSS;
              lOSS << "Operator \"" << lChild2->getValue() << "\" not found in operator map.\n";
              lOSS << "The operators that do exist in the operator map are:";
              // Go through operators in the map, listing their keys
              OperatorMap::iterator lItr;
              for (lItr=inOpMap.begin(); lItr!=inOpMap.end(); ++lItr) {
                lOSS << "\n- " << lItr->first;
              }
              throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
            }
            Operator::Handle lOp = castHandleT<Operator>(lIterOp->second);
            lOp = lOp->giveReference();
            mPositiveOpSet.push_back(lOp);
            lOp->readWithMap(lChild2, inOpMap);
          }
        }
      }
      else if(lChild->getValue() == "NegativeOpSet") {
        mNegativeOpSet.clear();
        for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
          if(lChild2->getType() == PACC::XML::eData) {
            OperatorMap::iterator lIterOp = inOpMap.find(lChild2->getValue().c_str());
            if(lIterOp == inOpMap.end()) {
              std::ostringstream lOSS;
              lOSS << "Operator \"" << lChild2->getValue() << "\" not found in operator map!";
              throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
            }
            Operator::Handle lOp = castHandleT<Operator>(lIterOp->second);
            lOp = lOp->giveReference();
            mNegativeOpSet.push_back(lOp);
            lOp->readWithMap(lChild2, inOpMap);
          }
        }
      }
    }
  }
  Beagle_StackTraceEndM("void IfThenElseOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write if-then-else operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
*  \param inIndent Whether XML output should be indented.
 */
void IfThenElseOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag(getName().c_str(), inIndent);
  ioStreamer.insertAttribute("parameter", mConditionTag);
  ioStreamer.insertAttribute("value", mConditionValue);
  ioStreamer.openTag("PositiveOpSet", inIndent);
  for(unsigned int i=0; i<mPositiveOpSet.size(); ++i) mPositiveOpSet[i]->write(ioStreamer, inIndent);
  ioStreamer.closeTag();
  ioStreamer.openTag("NegativeOpSet", inIndent);
  for(unsigned int i=0; i<mNegativeOpSet.size(); ++i) mNegativeOpSet[i]->write(ioStreamer, inIndent);
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void IfThenElseOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}




