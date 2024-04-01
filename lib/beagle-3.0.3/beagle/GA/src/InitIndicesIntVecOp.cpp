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
 *  \file   beagle/GA/src/InitIndicesIntVecOp.cpp
 *  \brief  Source code of class GA::InitIndicesIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.3.2.1 $
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
GA::InitIndicesIntVecOp::InitIndicesIntVecOp(unsigned int inIntVectorSize,
                                             Beagle::string inReproProbaName,
                                             Beagle::string inName) :
  InitializationOp(inReproProbaName, inName),
  mIntVectorSize(new UInt(inIntVectorSize))
{ }


/*!
 *  \brief Initialize the GA integer vectors uniformly distributed initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitIndicesIntVecOp::initialize(System& ioSystem)
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
  Beagle_StackTraceEndM("void GA::InitIndicesIntVecOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Initialize integer-valued GA individual with numbers uniformly distributed in a given range.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void GA::InitIndicesIntVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if(mIntVectorSize->getWrappedValue() == 0) {
    string lMessage = "GA::InitIndicesIntVecOp::initIndividual: ";
    lMessage += "integer vector size parameter is zero; ";
    lMessage += "could not initialize the individuals!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
#endif // BEAGLE_NDEBUG
  outIndividual.resize(1);
  GA::IntegerVector::Handle lIntVector = castHandleT<GA::IntegerVector>(outIndividual[0]);
  lIntVector->resize(mIntVectorSize->getWrappedValue());
  for(unsigned int j=0; j<lIntVector->size(); ++j) (*lIntVector)[j] = (int)j;
  std::random_shuffle(lIntVector->begin(),lIntVector->end(),ioContext.getSystem().getRandomizer());
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "initialization", "Beagle::GA::InitIndicesIntVecOp",
    string("Integer vector initialized as: ")+lIntVector->serialize()
  );
  Beagle_StackTraceEndM("void GA::InitIndicesIntVecOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)");
}

