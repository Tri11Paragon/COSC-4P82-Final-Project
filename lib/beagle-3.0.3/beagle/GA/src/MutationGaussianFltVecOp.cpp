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
 *  \file   beagle/GA/src/MutationGaussianFltVecOp.cpp
 *  \brief  Source code of class GA::MutationGaussianFltVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19.2.2 $
 *  $Date: 2007/05/11 19:18:44 $
 */

#include "beagle/GA.hpp"

#include <cfloat>
#include <float.h>    // To get FLT_MAX on Borland C++ Builder
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a real-valued GA Gaussian mutation operator.
 *  \param inMutationPbName Gaussian mutation probability parameter name used in register.
 *  \param inMutateFloatPbName Single value Gaussian mutation probability name used in the register.
 *  \param inMutateGaussMuName Gaussian mutation mean value parameter name used in register.
 *  \param inMutateGaussSigmaName Gaussian mutation std deviation parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::MutationGaussianFltVecOp::MutationGaussianFltVecOp(Beagle::string inMutationPbName,
                                                       Beagle::string inMutateFloatPbName,
                                                       Beagle::string inMutateGaussMuName,
                                                       Beagle::string inMutateGaussSigmaName,
                                                       Beagle::string inName) :
  MutationOp(inMutationPbName, inName),
  mMutateFloatPbName(inMutateFloatPbName),
  mMutateGaussMuName(inMutateGaussMuName),
  mMutateGaussSigmaName(inMutateGaussSigmaName)
{ }


/*!
 *  \brief Initialize the real-valued GA Gaussian mutation operator.
 *  \param ioSystem System of the evolution.
 */
void GA::MutationGaussianFltVecOp::initialize(System& ioSystem)
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
      "Indiv. Gaussian mutation prob.",
      "Float",
      "1.0",
      "Gaussian mutation probability for each real-valued GA individual."
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lDescription);
  }

  if(ioSystem.getRegister().isRegistered(mMutateFloatPbName)) {
    mMutateFloatPb = castHandleT<Float>(ioSystem.getRegister()[mMutateFloatPbName]);
  } else {
    mMutateFloatPb = new Float(0.1f);
    Register::Description lDescription(
      "Value's Gaussian mutation prob.",
      "Float",
      "0.1",
      "Single value Gaussian mutation probability for the real-valued GA individual."
    );
    ioSystem.getRegister().addEntry(mMutateFloatPbName, mMutateFloatPb, lDescription);
  }

  if(ioSystem.getRegister().isRegistered(mMutateGaussMuName)) {
    mMutateGaussMu = castHandleT<DoubleArray>(ioSystem.getRegister()[mMutateGaussMuName]);
  } else {
    mMutateGaussMu = new DoubleArray(1,0.0);
    std::ostringstream lOSS;
    lOSS << "Mean of values added to real-values vector numbers at each Gaussian mutation. ";
    lOSS << "Value can be a scalar, which state the mu value for all float ";
    lOSS << "vector parameters, or a vector which state the mu for the parameters ";
    lOSS << "individually. If the mu vector size is smaller than the ";
    lOSS << "float vector size, the mu used for the last values of the float vector ";
    lOSS << "is equal to the last value of the mu vector.";
    Register::Description lDescription(
      "Gaussian mutation mean",
      "DoubleArray",
      "0.0",
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry(mMutateGaussMuName, mMutateGaussMu, lDescription);
  }

  if(ioSystem.getRegister().isRegistered(mMutateGaussSigmaName)) {
    mMutateGaussSigma = castHandleT<DoubleArray>(ioSystem.getRegister()[mMutateGaussSigmaName]);
  } else {
    mMutateGaussSigma = new DoubleArray(1,0.1);
    std::ostringstream lOSS;
    lOSS << "Standard deviation of values added to real-values vector numbers at ";
    lOSS << "each Gaussian mutation. Value can be a scalar, which state the sigma for ";
    lOSS << "all float vector parameters, or a vector which state the sigma ";
    lOSS << "for the parameters individually. If the sigma vector size is smaller than the ";
    lOSS << "float vector size, the sigma used for the last values of the ";
    lOSS << "float vector is equal to the last value of the sigma vector.";
    Register::Description lDescription(
      "Gaussian mutation std deviation",
      "DoubleArray",
      "0.1",
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry(mMutateGaussSigmaName, mMutateGaussSigma, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.float.maxvalue")) {
    mMaxValue = castHandleT<DoubleArray>(ioSystem.getRegister()["ga.float.maxvalue"]);
  } else {
    mMaxValue = new DoubleArray(1,DBL_MAX);
    std::ostringstream lOSS;
    lOSS << "Maximum values assigned to vector's floats. ";
    lOSS << "Value can be a scalar, which limit the value for all float ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the maximum value is smaller than the ";
    lOSS << "float vector size, the limit used for the last values of the float vector ";
    lOSS << "is equal to the last value of the maximum value vector.";
    Register::Description lDescription(
      "Maximum vector values",
      "DoubleArray",
      dbl2str(DBL_MAX),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.float.maxvalue", mMaxValue, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.float.minvalue")) {
    mMinValue = castHandleT<DoubleArray>(ioSystem.getRegister()["ga.float.minvalue"]);
  } else {
    mMinValue = new DoubleArray(1,-DBL_MAX);
    std::ostringstream lOSS;
    lOSS << "Minimum  values assigned to vector's floats. ";
    lOSS << "Value can be a scalar, which limit the value for all float ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the minimum value is smaller than the ";
    lOSS << "float vector size, the limit used for the last values of the float vector ";
    lOSS << "is equal to the last value of the minimum value vector.";
    Register::Description lDescription(
      "Minimum values",
      "DoubleArray",
      dbl2str(-DBL_MAX),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.float.minvalue", mMinValue, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.float.inc")) {
    mIncValue = castHandleT<DoubleArray>(ioSystem.getRegister()["ga.float.inc"]);
  } else {
    mIncValue = new DoubleArray(1,0.0);
    std::ostringstream lOSS;
    lOSS << "Increments of valid values assigned to vector's floats. ";
    lOSS << "Value can be a scalar, which limit the value for all float ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the value is not evenly divisible by the ";
    lOSS << "increment, the value will be set to the closest valid ";
    lOSS << "value.";
    Register::Description lDescription(
      "Increments of valid values",
      "DoubleArray",
      "0.0",
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.float.inc", mIncValue, lDescription);
  }
  Beagle_StackTraceEndM("void GA::MutationGaussianFltVecOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Gaussian mutate a real-valued GA individual.
 *  \param ioIndividual Real-valued GA individual to mutate.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
bool GA::MutationGaussianFltVecOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  bool lMutated = false;
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GA::MutationGaussianFltVecOp",
    string("Gaussian mutations with mean of ")+
    mMutateGaussMu->serialize()+
    string(", and standard deviation of ")+
    mMutateGaussSigma->serialize()
  );

  for(unsigned int i=0; i<ioIndividual.size(); i++) {
    GA::FloatVector::Handle lVector = castHandleT<GA::FloatVector>(ioIndividual[i]);
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationGaussianFltVecOp",
      string("Gaussian mutation the ")+uint2ordinal(i+1)+" float vector"
    );
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "mutation", "Beagle::GA::MutationGaussianFltVecOp",
      string("The float vector before GA Gaussian mutation: ")+lVector->serialize()
    );
    for(unsigned int j=0; j<lVector->size(); j++) {
      const float lRolledPb = ioContext.getSystem().getRandomizer().rollUniform();
      if(lRolledPb <= mMutateFloatPb->getWrappedValue()) {
        const double lMaxVal = j<mMaxValue->size() ? (*mMaxValue)[j] : mMaxValue->back();
        const double lMinVal = j<mMinValue->size() ? (*mMinValue)[j] : mMinValue->back();
        const double lIncVal = j<mIncValue->size() ? (*mIncValue)[j] : mIncValue->back();
        const double lMu =
          j<mMutateGaussMu->size() ? (*mMutateGaussMu)[j] : mMutateGaussMu->back();
        const double lSigma =
          j<mMutateGaussSigma->size() ? (*mMutateGaussSigma)[j] : mMutateGaussSigma->back();
        Beagle_AssertM(lSigma>=0.0);
        const double lMValue = ioContext.getSystem().getRandomizer().rollGaussian(lMu,lSigma);
        (*lVector)[j] += lMValue;
        if(lIncVal!=0.0) (*lVector)[j] = lIncVal * round((*lVector)[j] / lIncVal);
        if((*lVector)[j] > lMaxVal) (*lVector)[j] = lMaxVal;
        if((*lVector)[j] < lMinVal) (*lVector)[j] = lMinVal;
        lMutated = true;
        Beagle_LogDebugM(
          ioContext.getSystem().getLogger(),
          "mutation", "Beagle::GA::MutationGaussianFltVecOp",
          string("Gaussian mutating by adding ")+dbl2str(lMValue)+
          string(" to the value at the index ")+uint2str(j)+
          string(" of the float vector")
        );
      }
    }
    if(lMutated) {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationGaussianFltVecOp",
        string("The float vector has been mutated")
      );
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationGaussianFltVecOp",
        string("The float vector after GA Gaussian mutation: ")+lVector->serialize()
      );
    }
    else {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::GA::MutationGaussianFltVecOp",
        string("The float vector has not been mutated")
      );
    }
  }
  return lMutated;
  Beagle_StackTraceEndM("bool GA::MutationGaussianFltVecOp::mutate(Beagle::Individual& ioIndividual, Context& ioContext)");
}


/*!
 *  \brief Read a Gaussian mutation operator from a XML subtree.
 *  \param inIter XML iterator to use to read mutation operator.
 *  \param inOpMap Operator map to use to read mutation operator.
 */
void GA::MutationGaussianFltVecOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lMutationPbReadName = inIter->getAttribute("mutationpb").c_str();
  if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
  string lMutateFloatPbReadName = inIter->getAttribute("mutfloatpb").c_str();
  if(lMutateFloatPbReadName.empty() == false) mMutateFloatPbName = lMutateFloatPbReadName;
  string lMutateGaussMuReadName = inIter->getAttribute("mutgaussmu").c_str();
  if(lMutateGaussMuReadName.empty() == false) mMutateGaussMuName = lMutateGaussMuReadName;
  string lMutateGaussSigmaReadName = inIter->getAttribute("mutgausssigma").c_str();
  if(lMutateGaussSigmaReadName.empty() == false) mMutateGaussSigmaName = lMutateGaussSigmaReadName;
  Beagle_StackTraceEndM("void GA::MutationGaussianFltVecOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write Gaussian mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void GA::MutationGaussianFltVecOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::writeContent(ioStreamer, inIndent);
  ioStreamer.insertAttribute("mutfloatpb", mMutateFloatPbName);
  ioStreamer.insertAttribute("mutgaussmu", mMutateGaussMuName);
  ioStreamer.insertAttribute("mutgausssigma", mMutateGaussSigmaName);
  Beagle_StackTraceEndM("void GA::MutationGaussianFltVecOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


