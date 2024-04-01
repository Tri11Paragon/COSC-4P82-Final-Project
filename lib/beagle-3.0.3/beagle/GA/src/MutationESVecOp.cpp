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
 *  \file   beagle/GA/src/MutationESVecOp.cpp
 *  \brief  Source code of class GA::MutationESVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.2 $
 *  $Date: 2007/05/11 19:18:44 $
 */

#include "beagle/GA.hpp"

#include <cfloat>
#include <cmath>
#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a evolution strategy mutation operator.
 *  \param inMutationPbName ES mutation probability parameter name used in register.
 *  \param inMinStrategyName Minimum strategy parameter parameter name.
 *  \param inName Name of the operator.
 */
GA::MutationESVecOp::MutationESVecOp(Beagle::string inMutationPbName,
                                     Beagle::string inMinStrategyName,
                                     Beagle::string inName) :
  MutationOp(inMutationPbName, inName),
  mMinStrategyName(inMinStrategyName)
{ }


/*!
 *  \brief Initialize the ES mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationESVecOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  MutationOp::initialize(ioSystem);
  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    ioSystem.getRegister().deleteEntry(mMutationPbName);
  }

  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    mMutationProba = castHandleT<Float>(ioSystem.getRegister()[mMutationPbName]);
  } else {
    mMutationProba = new Float(1.0f);
    Register::Description lDescription(
      "Individual ES mutation prob.",
      "Float",
      "1.0",
      "ES mutation probability for each individual."
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lDescription);
  }

  if(ioSystem.getRegister().isRegistered(mMinStrategyName)) {
    mMinStrategy = castHandleT<Double>(ioSystem.getRegister()[mMinStrategyName]);
  } else {
    mMinStrategy = new Double(0.01);
    Register::Description lDescription(
      "Minimum strategy parameter",
      "Double",
      "0.01",
      "Minimum mutation strategy parameter allowed."
    );
    ioSystem.getRegister().addEntry(mMinStrategyName, mMinStrategy, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("es.value.max")) {
    mMaxValue = castHandleT<DoubleArray>(ioSystem.getRegister()["es.value.max"]);
  } else {
    mMaxValue = new DoubleArray(1,DBL_MAX);
    std::ostringstream lOSS;
    lOSS << "ES vector maximum values. This can be a scalar, which limit the values for all ES, ";
    lOSS << "or a vector which limit the values individually. If the maximum value vector size is ";
    lOSS << "smaller than the ES vector size, the limit used for the last values of the ES vector ";
    lOSS << "is equal to the last value of the maximum value vector.";
    Register::Description lDescription(
      "Maximum ES vector values",
      "DoubleArray",
      dbl2str(DBL_MAX),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("es.value.max", mMaxValue, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("es.value.min")) {
    mMinValue = castHandleT<DoubleArray>(ioSystem.getRegister()["es.value.min"]);
  } else {
    mMinValue = new DoubleArray(1,-DBL_MAX);
    std::ostringstream lOSS;
    lOSS << "ES vector minimum values. This can be a scalar, which limit the values for all ES, ";
    lOSS << "or a vector which limit the values individually. If the minimum value vector size is ";
    lOSS << "smaller than the ES vector size, the limit used for the last values of the ES vector ";
    lOSS << "is equal to the last value of the minimum value vector.";
    Register::Description lDescription(
      "Minimum ES vector values",
      "DoubleArray",
      dbl2str(-DBL_MAX),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("es.value.min", mMinValue, lDescription);
  }
  Beagle_StackTraceEndM("void GA::MutationESVecOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Mutate an ES individual.
 *  \param ioIndividual ES individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True as individual are always mutated.
 */
bool GA::MutationESVecOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_ValidateParameterM(mMinStrategy->getWrappedValue()>=0.0,mMinStrategyName,"<0");

  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GA::MutationESVecOp",
    string("Applying evolution strategy mutation to an individual")
  );

  for(unsigned int i=0; i<ioIndividual.size(); i++) {
    GA::ESVector::Handle lVector = castHandleT<GA::ESVector>(ioIndividual[i]);
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationESVecOp",
      string("Mutating the ")+uint2ordinal(i+1)+" ES vector"
    );
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationESVecOp",
      string("The ES vector before mutation: ")+lVector->serialize()
    );

    const double lT = 1.0 / std::sqrt(2.0 * std::sqrt(double(lVector->size())));
    const double lTPrime = 1.0 / std::sqrt(2.0 * lVector->size());
    const double lN = ioContext.getSystem().getRandomizer().rollGaussian(0.0, 1.0);
    const double lMinStrategy = mMinStrategy->getWrappedValue();
    for(unsigned int j=0; j<lVector->size(); j++) {
      const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
      const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
      const double lNi = ioContext.getSystem().getRandomizer().rollGaussian(0.0, 1.0);

      (*lVector)[j].mStrategy *= std::exp((lTPrime * lN) + (lT * lNi));
      if((*lVector)[j].mStrategy < lMinStrategy) (*lVector)[j].mStrategy = lMinStrategy;
      (*lVector)[j].mValue += (*lVector)[j].mStrategy * lNi;
      if((*lVector)[j].mValue > lMaxVal) (*lVector)[j].mValue = lMaxVal;
      if((*lVector)[j].mValue < lMinVal) (*lVector)[j].mValue = lMinVal;

      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationESVecOp",
        string("ES mutating by adding ")+dbl2str((*lVector)[j].mStrategy * lNi)+
        string(" to the value and multiplying the strategy by ")+
        dbl2str(std::exp((lTPrime * lN) + (lT * lNi)))+
        string(" to mutate the pair ")+uint2str(j)+
        string(" of the ES vector")
      );
    }

    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationESVecOp",
      string("ES vector after mutation: ")+lVector->serialize()
    );
  }
  return true;
  Beagle_StackTraceEndM("bool GA::MutationESVecOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)");
}


/*!
 *  \brief Read ES mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read mutation operator.
 *  \param inOpMap Operator map to use to read mutation operator.
 */
void GA::MutationESVecOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lMutationPbReadName = inIter->getAttribute("mutationpb").c_str();
  if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
  string lMinStrategyReadName = inIter->getAttribute("minstrategy").c_str();
  if(lMinStrategyReadName.empty() == false) mMinStrategyName = lMinStrategyReadName;
  Beagle_StackTraceEndM("void GA::MutationESVecOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write ES mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void GA::MutationESVecOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::writeContent(ioStreamer, inIndent);
  ioStreamer.insertAttribute("minstrategy", mMinStrategyName);
  Beagle_StackTraceEndM("void GA::MutationESVecOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

