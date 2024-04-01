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
 *  \file   beagle/GA/src/InitIntVecOp.cpp
 *  \brief  Source code of class GA::InitIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4.2.1 $
 *  $Date: 2007/05/09 01:51:03 $
 */

#include "beagle/GA.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a GA integer vectors uniformly distributed initialization operator.
 *  \param inIntVectorSize Size of the integer vectors initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the operator.
 */
GA::InitIntVecOp::InitIntVecOp(unsigned int inIntVectorSize,
                               Beagle::string inReproProbaName,
                               Beagle::string inName) :
  InitializationOp(inReproProbaName, inName),
  mIntVectorSize(new UInt(inIntVectorSize))
{ }


/*!
 *  \brief Initialize the GA integer vectors uniformly distributed initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitIntVecOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  InitializationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ga.init.vectorsize")) {
    mIntVectorSize = castHandleT<UInt>(ioSystem.getRegister()["ga.init.vectorsize"]);
  } else {
    Register::Description lDescription(
      "Initial integer vectors sizes",
      "UInt",
      mIntVectorSize->serialize(),
      "Integer vector size of initialized individuals."
    );
    ioSystem.getRegister().addEntry("ga.init.vectorsize", mIntVectorSize, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.init.maxvalue")) {
    mMaxInitValue = castHandleT<IntArray>(ioSystem.getRegister()["ga.init.maxvalue"]);
  } else {
    mMaxInitValue = new IntArray(1,INT_MAX);
    std::ostringstream lOSS;
    lOSS << "Maximum initialization values assigned to vector's integers. ";
    lOSS << "Value can be a scalar, which limit the initialization value for all integers ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the maximum initialization value is smaller than the ";
    lOSS << "integer vector size, the limit used for the last values of the integer vector ";
    lOSS << "is equal to the last value of the maximum initialization value vector.";
    Register::Description lDescription(
      "Maximum initialization values",
      "IntArray",
      int2str(INT_MAX),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.init.maxvalue", mMaxInitValue, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.init.minvalue")) {
    mMinInitValue = castHandleT<IntArray>(ioSystem.getRegister()["ga.init.minvalue"]);
  } else {
    mMinInitValue = new IntArray(1,INT_MIN);
    std::ostringstream lOSS;
    lOSS << "Minimum initialization values assigned to vector's integers. ";
    lOSS << "Value can be a scalar, which limit the initialization value for all integer ";
    lOSS << "vector parameters, or a vector which limit the value for the parameters ";
    lOSS << "individually. If the minimum initialization value is smaller than the ";
    lOSS << "integer vector size, the limit used for the last values of the integer vector ";
    lOSS << "is equal to the last value of the minimum initialization value vector.";
    Register::Description lDescription(
      "Minimum initialization values",
      "IntArray",
      int2str(INT_MIN),
      lOSS.str().c_str()
    );
    ioSystem.getRegister().addEntry("ga.init.minvalue", mMinInitValue, lDescription);
  }
  Beagle_StackTraceEndM("void GA::InitIntVecOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Initialize integer-valued GA individual with numbers uniformly distributed in a given range.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void GA::InitIntVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if(mIntVectorSize->getWrappedValue() == 0) {
    string lMessage = "GA::InitIntVecOp::initIndividual: ";
    lMessage += "integer vector size parameter is zero; ";
    lMessage += "could not initialize the individuals!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
#endif // BEAGLE_NDEBUG
  outIndividual.resize(1);
  GA::IntegerVector::Handle lIntVector = castHandleT<GA::IntegerVector>(outIndividual[0]);
  lIntVector->resize(mIntVectorSize->getWrappedValue());
  for(unsigned int j=0; j<lIntVector->size(); ++j) {
    const int lMaxVal = j<mMaxInitValue->size() ? (*mMaxInitValue)[j] : mMaxInitValue->back();
    const int lMinVal = j<mMinInitValue->size() ? (*mMinInitValue)[j] : mMinInitValue->back();
    (*lIntVector)[j] = ioContext.getSystem().getRandomizer().rollInteger(lMinVal,lMaxVal);
  }
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "initialization", "Beagle::GA::InitIntVecOp",
    string("Integer vector initialized as: ")+lIntVector->serialize()
  );
  Beagle_StackTraceEndM("void GA::InitIntVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)");
}

