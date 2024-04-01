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
 *  \file   beagle/GA/src/InitBitStrOp.cpp
 *  \brief  Source code of class GA::InitBitStrOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:03 $
 */

#include "beagle/GA.hpp"

#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct a GA bit string initialization operator.
 *  \param inNumberBits Number of bits of the strings initialized.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the operator.
 */
GA::InitBitStrOp::InitBitStrOp(unsigned int inNumberBits,
                               Beagle::string inReproProbaName,
                               Beagle::string inName) :
  InitializationOp(inReproProbaName, inName),
  mNumberBits(new UInt(inNumberBits)),
  mBitOneProba(NULL)
{ }


/*!
 *  \brief Initialize the GA bit string initialization operator.
 *  \param ioSystem System of the evolution.
 */
void GA::InitBitStrOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  InitializationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ga.init.numberbits")) {
    mNumberBits = castHandleT<UInt>(ioSystem.getRegister()["ga.init.numberbits"]);
  } else {
    string lLongDescrip("Number of bits used to initialize individuals.");
    Register::Description lDescription(
      "Initial number of bits in strings",
      "UInt",
      mNumberBits->serialize(),
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ga.init.numberbits", mNumberBits, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.init.bitpb")) {
    mBitOneProba = castHandleT<Float>(ioSystem.getRegister()["ga.init.bitpb"]);
  } else {
    mBitOneProba = new Float(0.5);
    string lLongDescrip = "Distribution probability of bit values. ";
    lLongDescrip += "A probability of 1.0 means that the bits values are all initialized to 1, ";
    lLongDescrip += "while a probability of 0.0 means that they are all initialized to 0. ";
    lLongDescrip += "Probability of 0.5 means that the bits are uniformly, ";
    lLongDescrip += "randomly initialized with equally 0s and 1s.";
    Register::Description lDescription(
      "Initial bit distribution prob.",
      "Float",
      "0.5",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ga.init.bitpb", mBitOneProba, lDescription);
  }
  Beagle_StackTraceEndM("void GA::InitBitStrOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Initialize an individual made of GA bit strings.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void GA::InitBitStrOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if(mNumberBits->getWrappedValue() == 0) {
    string lMessage = "GA::InitBitStrOp::initIndividual: number of bits to use to ";
    lMessage += " initialize individuals parameter is zero. ";
    lMessage += "Could not initialize the individuals!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
#endif // BEAGLE_NDEBUG
  outIndividual.resize(1);
  GA::BitString::Handle lActualBitString = castHandleT<GA::BitString>(outIndividual[0]);
  lActualBitString->resize(mNumberBits->getWrappedValue());
  for(unsigned int i=0; i<lActualBitString->size(); ++i) {
    (*lActualBitString)[i] =
      (ioContext.getSystem().getRandomizer().rollUniform() < mBitOneProba->getWrappedValue());
  }
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "initialization", "Beagle::GA::InitBitStrOp",
    string("Bit string initialized as: ")+lActualBitString->serialize()
  );
  Beagle_StackTraceEndM("void GA::InitBitStrOp::initIndividual(Beagle::Individual& outIndividual, Context& ioContext)");
}
