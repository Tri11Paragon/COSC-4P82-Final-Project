/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/src/InitializationOp.cpp
 *  \brief  Source code of class LinGP::InitializationOp.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

#include <sstream>

#include "beagle/GP.hpp"
#include "beagle/RunTimeException.hpp"
#include "beagle/LinGP/System.hpp"
#include "beagle/LinGP/Context.hpp"
#include "beagle/LinGP/InitializationOp.hpp"
#include "beagle/LinGP/Program.hpp"
#include "beagle/LinGP/Individual.hpp"


using namespace Beagle;


/*!
 *  \brief Construct a linear GP initialization operator.
 *  \param inBitStringSize Size of the programs initialized.
 *  \param inName Name of the operator.
 */
LinGP::InitializationOp::InitializationOp(IntArray inMinInitSize,
                                          IntArray inMaxInitSize,
                                          Beagle::string inName) :
  Beagle::InitializationOp(inName),
  mMinInitSize(new IntArray(inMinInitSize)),
  mMaxInitSize(new IntArray(inMaxInitSize))
{ }


/*!
 *  \brief Initialize the linear GP initialization operator.
 *  \param ioSystem System of the evolution.
 */
void LinGP::InitializationOp::initialize(Beagle::System& ioSystem)
{
  Beagle::InitializationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("lingp.init.minsize")) {
    mMinInitSize = castHandleT<IntArray>(ioSystem.getRegister()["lingp.init.minsize"]);
  } else {
    string lDefaultValue;
    for(unsigned int i=0; i<mMinInitSize->size(); i++) {
      lDefaultValue += uint2str((*mMinInitSize)[i]);
      if(i != (mMinInitSize->size()-1)) lDefaultValue += "/";
    }
    string lLongDescrip("Minimum program size for individuals at initialization. ");
    lLongDescrip += "The size of the IntArray is the number of program composing the ";
    lLongDescrip += "linear GP individuals, while each value of the vector is the minimum ";
    lLongDescrip += "number of instructions composing the programs. ";
    lLongDescrip += "IntArray format is S1/S2/.../Sn, where Si is ith value.";
    Register::Description lDescription(
      "Min individual init. sizes",
      "IntArray",
      lDefaultValue,
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("lingp.init.minsize", mMinInitSize, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("lingp.init.maxsize")) {
    mMaxInitSize = castHandleT<IntArray>(ioSystem.getRegister()["lingp.init.maxsize"]);
  } else {
    string lDefaultValue;
    for(unsigned int i=0; i<mMaxInitSize->size(); i++) {
      lDefaultValue += uint2str((*mMaxInitSize)[i]);
      if(i != (mMaxInitSize->size()-1)) lDefaultValue += "/";
    }
    string lLongDescrip("Maximum program size for individuals at initialization. ");
    lLongDescrip += "The size of the IntArray is the number of program composing the ";
    lLongDescrip += "linear GP individuals, while each value of the vector is the maximum ";
    lLongDescrip += "number of instructions composing the programs. ";
    lLongDescrip += "IntArray format is S1/S2/.../Sn, where Si is ith value.";
    Register::Description lDescription(
      "Max individual init. sizes",
      "IntArray",
      lDefaultValue,
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("lingp.init.maxsize", mMaxInitSize, lDescription);
  }
}


/*!
 *  \brief Initialize an linear GP individual.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 */
void LinGP::InitializationOp::initIndividual(Beagle::Individual& outIndividual,
                                             Beagle::Context& ioContext)
{
#ifndef BEAGLE_NDEBUG
  if(mMinInitSize->size() == 0) {
    string lMessage = "LinGP::InitializationOp::initIndividual: ";
    lMessage += "Size of the minimum iniitalization size parameter is zero. ";
    lMessage += "Could not initialize the individuals!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  if(mMaxInitSize->size() == 0) {
    string lMessage = "LinGP::InitializationOp::initIndividual: ";
    lMessage += "Size of the maximum iniitalization size parameter is zero. ";
    lMessage += "Could not initialize the individuals!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  if(mMinInitSize->size() != mMaxInitSize->size()) {
    string lMessage = "LinGP::InitializationOp::initIndividual: ";
    lMessage += "Size of the maximum and minimum iniitalization size parameter mismatch. ";
    lMessage += "Could not initialize the individuals!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }

  for(unsigned int k=0; k<mMinInitSize->size(); k++) {
    if((*mMinInitSize)[k] == 0) {
      string lMessage = "LinGP::InitializationOp::initIndividual: Value of the ";
      lMessage += uint2ordinal(k);
      lMessage += " minimum initialization size parameter is zero. ";
      lMessage += "Could not initialize the individuals!";
      throw Beagle_RunTimeExceptionM(lMessage);
    }
    if((*mMaxInitSize)[k] == 0) {
      string lMessage = "LinGP::InitializationOp::initIndividual: Value of the ";
      lMessage += uint2ordinal(k);
      lMessage += " maximum initialization size parameter is zero. ";
      lMessage += "Could not initialize the individuals!";
      throw Beagle_RunTimeExceptionM(lMessage);
    }
    if((*mMaxInitSize)[k] > (*mMaxInitSize)[k]) {
      string lMessage = "LinGP::InitializationOp::initIndividual: Value of the ";
      lMessage += uint2ordinal(k);
      lMessage += " minimum initialization size parameter is bigger than ";
      lMessage += " maximum initialization size parameter. ";
      lMessage += "Could not initialize the individuals!";
      throw Beagle_RunTimeExceptionM(lMessage);
    }
  }
#endif // BEAGLE_NDEBUG

  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "initialization", "Beagle::LinGP::InitializationOp",
    string("Resizing the individual to ")+
    uint2str(mMinInitSize->size())+" programs"
  );
  outIndividual.resize(mMinInitSize->size());
  LinGP::Context& lLinGPContext = castObjectT<LinGP::Context&>(ioContext);
  LinGP::InstructionSuperSet& lInsSS = lLinGPContext.getSystem().getInstructionSuperSet();

  for(unsigned int i=0; i<outIndividual.size(); ++i) {
    LinGP::Program::Handle lActualProgram = castHandleT<LinGP::Program>(outIndividual[i]);
    unsigned int lInitProgramSize =
      lLinGPContext.getSystem().getRandomizer().rollInteger((*mMinInitSize)[i], (*mMaxInitSize)[i]);
    lActualProgram->resize(lInitProgramSize);
    for(unsigned int j=0; j<lInitProgramSize; ++j) {
      (*lActualProgram)[j] =
        lInsSS[i]->selectRandomInstruction(lLinGPContext.getSystem())->giveReference(lLinGPContext);
    }
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "initialization", "Beagle::LinGP::InitializationOp",
      string("Program ")+uint2str(i+1)+
      string(" has ")+uint2str(lInitProgramSize)+" initialized instructions"
    );
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "initialization", "Beagle::LinGP::InitializationOp",
      uint2ordinal(i+1)+
      string(" program is initialized as: ")+lActualProgram->serialize()
    );
  }
}

