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
 *  \file   beagle/src/MuCommaLambdaOp.cpp
 *  \brief  Source code of class MuCommaLambdaOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Build (Mu,Lambda) replacement strategy operator.
 *  \param inLMRatioName Lamda over Mu parameter name used in the register.
 *  \param inName Name of the (Mu,Lambda) operator.
 */
MuCommaLambdaOp::MuCommaLambdaOp(Beagle::string inLMRatioName, Beagle::string inName) :
  ReplacementStrategyOp(inName),
  mLMRatioName(inLMRatioName)
{ }


/*!
 *  \brief Initialize the operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void MuCommaLambdaOp::initialize(System& ioSystem)
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

  if(ioSystem.getRegister().isRegistered(mLMRatioName)) {
    mLMRatio = castHandleT<Float>(ioSystem.getRegister().getEntry(mLMRatioName));
  } else {
    mLMRatio = new Float((float)7.0);
    Register::Description lDescription(
      "(Lambda / Mu) ratio",
      "Float",
      "7.0",
      string("(Lambda / Mu) ratio, which mean how much bigger is the child population from ") +
      string("the parent population during a (Mu+Lambda), (Mu,Lambda) or NSGA2 ")+
      string("replacement process.")
    );
    ioSystem.getRegister().addEntry(mLMRatioName, mLMRatio, lDescription);
  }
  Beagle_StackTraceEndM("void MuCommaLambdaOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the (Mu,Lambda) replacement strategy operation on a deme.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void MuCommaLambdaOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(mElitismKeepSize);
  Beagle_ValidateParameterM(mLMRatio->getWrappedValue() >= 1.0,
                            mLMRatioName,
                            "The LM ratio must be higher or equal to 1.0.");
  Beagle_ValidateParameterM(mElitismKeepSize->getWrappedValue() <= ioDeme.size(),
                            "ec.elite.keepsize",
                            "The elistism keepsize must be less than the deme size!");

  // Special case: using the (mu,lambda) only to shrink back the population.
  if(getRootNode()==NULL) {
    UIntArray::Handle lPopSize;
    if(ioContext.getSystem().getRegister().isRegistered("ec.pop.size")) {
      lPopSize = castHandleT<UIntArray>(ioContext.getSystem().getRegister()["ec.pop.size"]);
    }
    else {
      std::ostringstream lOSS;
      lOSS << "Population size parameter \"ec.pop.size\" is not found in register!";
      throw ValidationException(lOSS.str().c_str());
    }
    const unsigned int lDemeSize = (*lPopSize)[ioContext.getDemeIndex()];
    Beagle_AssertM(ioDeme.size() > lDemeSize);
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
    for(unsigned int i=0; i<lDemeSize; ++i) {
      std::pop_heap(ioDeme.begin(), (ioDeme.end()-i), IsLessPointerPredicate());
    }
    ioDeme.erase(ioDeme.begin(), ioDeme.begin()+(ioDeme.size()-lDemeSize));
    Beagle_AssertM(ioDeme.size()==lDemeSize);
    return;
  }

  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::MuCommaLambdaOp",
    string("Using (mu,lambda) replacement strategy to process the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  Beagle_LogObjectM(
    ioContext.getSystem().getLogger(),
    Logger::eTrace,
    "replacement-strategy", "Beagle::MuCommaLambdaOp",
    (*this)
  );

  RouletteT<unsigned int> lRoulette;
  buildRoulette(lRoulette, ioContext);

  Individual::Bag lOffsprings;
  if(mElitismKeepSize->getWrappedValue() > 0) {
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
    for(unsigned int i=0; i<mElitismKeepSize->getWrappedValue(); ++i) {
      lOffsprings.push_back(ioDeme[0]);
      std::push_heap(lOffsprings.begin(), lOffsprings.end(), IsLessPointerPredicate());
      std::pop_heap(ioDeme.begin(), (ioDeme.end()-i), IsLessPointerPredicate());
    }
  }

  unsigned int lLambda =
    (unsigned int)std::ceil(mLMRatio->getWrappedValue()*float(ioDeme.size()));
  for(unsigned int i=0; i<lLambda; ++i) {
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

  Beagle_AssertM(lOffsprings.size() > ioDeme.size());
  for(unsigned int i=0; i<lOffsprings.size(); ++i) {
    if((lOffsprings[i]->getFitness()==NULL) || (lOffsprings[i]->getFitness()->isValid()==false)) {
      ioDeme.resize(0);
      ioDeme.insert(ioDeme.end(), lOffsprings.begin(), lOffsprings.end());
      return;
    }
  }

  std::make_heap(lOffsprings.begin(), lOffsprings.end(), IsLessPointerPredicate());
  for(unsigned int j=0; j<ioDeme.size(); ++j) {
    ioDeme[j] = lOffsprings[0];
    std::pop_heap(lOffsprings.begin(), (lOffsprings.end()-j), IsLessPointerPredicate());
  }
  Beagle_StackTraceEndM("void MuCommaLambdaOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Read a MuCommaLamdaOp operator for XML tree.
 *  \param inIter XML iterator to use to read MuCommaLamdaOp operator.
 *  \param inOpMap Operator map to use to read MuCommaLamdaOp operator.
 */
void MuCommaLambdaOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lLMRatioReadName = inIter->getAttribute("ratio_name").c_str();
  if(lLMRatioReadName.empty() == false) mLMRatioName = lLMRatioReadName;

  ReplacementStrategyOp::readWithMap(inIter, inOpMap);
  Beagle_StackTraceEndM("void MuCommaLambdaOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write MuCommaLamdaOp operator into XML streamer.
 *  \param ioStreamer XML streamer to write MuCommaLamdaOp operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void MuCommaLambdaOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag(getName().c_str(), inIndent);
  ioStreamer.insertAttribute("ratio_name", mLMRatioName);
  if(getRootNode() != NULL) getRootNode()->write(ioStreamer, inIndent);
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void MuCommaLambdaOp::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

