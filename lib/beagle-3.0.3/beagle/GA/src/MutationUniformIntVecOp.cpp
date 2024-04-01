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
 *  \file   beagle/GA/src/MutationUniformIntVecOp.cpp
 *  \brief  Source code of class GA::MutationUniformIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4.2.1 $
 *  $Date: 2007/05/09 01:51:03 $
 */

#include "beagle/GA.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct an integer vector GA uniform mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inIntMutatePbName Mutation integer probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::MutationUniformIntVecOp::MutationUniformIntVecOp(Beagle::string inMutationPbName,
                                                     Beagle::string inIntMutatePbName,
                                                     Beagle::string inName) :
  Beagle::MutationOp(inMutationPbName, inName),
  mIntMutatePbName(inIntMutatePbName)
{ }


/*!
 *  \brief Initialize the integer vector GA uniform mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationUniformIntVecOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    ioSystem.getRegister().deleteEntry(mMutationPbName);
  }

  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    mMutationProba = castHandleT<Float>(ioSystem.getRegister()[mMutationPbName]);
  } else {
    mMutationProba = new Float(float(0.1));
    Register::Description lProbaDescription(
      "Individual unif. mutation prob.",
      "Float",
      "0.1",
      "Integer vector uniform mutation probability for each GA individual."
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lProbaDescription);
  }

  if(ioSystem.getRegister().isRegistered(mIntMutatePbName)) {
    mIntMutateProba = castHandleT<Float>(ioSystem.getRegister()[mIntMutatePbName]);
  } else {
    mIntMutateProba = new Float(float(0.1));
    Register::Description lIntDescription(
      "Int unif. mutation probability",
      "Float",
      "0.1",
      "Probability for each integer to be modified by mutation, when an individual is mutated."
    );
    ioSystem.getRegister().addEntry(mIntMutatePbName, mIntMutateProba, lIntDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.int.maxvalue")) {
    mMaxValue = castHandleT<IntArray>(ioSystem.getRegister()["ga.int.maxvalue"]);
  } else {
    mMaxValue = new IntArray(1,INT_MAX);
    std::ostringstream lOSS;
    lOSS << "Maximum values assigned to vector's integers. ";
    lOSS << "Value can be a scalar, which limit the value for all integers ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the maximum value is smaller than the ";
    lOSS << "integer vector size, the limit used for the last values of the integer vector ";
    lOSS << "is equal to the last value of the maximum value vector.";
    Register::Description lDescription(
      "Maximum integer values",
      "IntArray",
      int2str(INT_MAX),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.int.maxvalue", mMaxValue, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.int.minvalue")) {
    mMinValue = castHandleT<IntArray>(ioSystem.getRegister()["ga.int.minvalue"]);
  } else {
    mMinValue = new IntArray(1,INT_MIN);
    std::ostringstream lOSS;
    lOSS << "Minimum values assigned to vector's integers. ";
    lOSS << "Value can be a scalar, which limit the value for all integer ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the minimum value is smaller than the ";
    lOSS << "integer vector size, the limit used for the last values of the integer vector ";
    lOSS << "is equal to the last value of the minimum value vector.";
    Register::Description lDescription(
      "Minimum integer values",
      "IntArray",
      int2str(INT_MIN),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.int.minvalue", mMinValue, lDescription);
  }
  Beagle_StackTraceEndM("void GA::MutationUniformIntVecOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Uniformly mutate an integer vector GA individual.
 *  \param ioIndividual GA individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GA::MutationUniformIntVecOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_ValidateParameterM(mIntMutateProba->getWrappedValue()>=0.0, mIntMutatePbName, "<0");

  bool lMutated = false;
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GA::MutationUniformIntVecOp",
    string("Integer uniform mutation probability is: ")+
    dbl2str(mIntMutateProba->getWrappedValue())
  );

  for(unsigned int i=0; i<ioIndividual.size(); i++) {
    GA::IntegerVector::Handle lIV = castHandleT<GA::IntegerVector>(ioIndividual[i]);
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationUniformIntVecOp",
      string("Uniformly mutating the ")+uint2ordinal(i+1)+" integer vector"
    );
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationUniformIntVecOp",
      string("The integer vector before GA uniform mutation: ")+lIV->serialize()
    );
    for(unsigned int j=0; j<lIV->size(); j++) {
      double lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
      if(lRolledPb <= mIntMutateProba->getWrappedValue()) {
        const int lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
        const int lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
        (*lIV)[j] = ioContext.getSystem().getRandomizer().rollInteger(lMinVal, lMaxVal);
        lMutated = true;
      }
    }
    if(lMutated) {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationUniformIntVecOp",
        string("The integer vector has been uniformly mutated")
      );
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationUniformIntVecOp",
        string("The integer vector after GA uniform mutation: ")+lIV->serialize()
      );
    }
    else {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationUniformIntVecOp",
        string("The integer vector has not been mutated")
      );
    }
  }
  return lMutated;
  Beagle_StackTraceEndM("bool GA::MutationUniformIntVecOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)");
}


/*!
 *  \brief Read a uniform mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read mutation operator.
 *  \param inOpMap Operator map to use to read mutation operator.
 */
void GA::MutationUniformIntVecOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lMutationPbReadName = inIter->getAttribute("mutationpb").c_str();
  if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
  string lIntMutatePbReadName = inIter->getAttribute("mutintpb").c_str();
  if(lIntMutatePbReadName.empty() == false) mIntMutatePbName = lIntMutatePbReadName;
  Beagle_StackTraceEndM("void GA::MutationUniformIntVecOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write uniform mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void GA::MutationUniformIntVecOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::writeContent(ioStreamer, inIndent);
  ioStreamer.insertAttribute("mutintpb", mIntMutatePbName);
  Beagle_StackTraceEndM("void GA::MutationUniformIntVecOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

