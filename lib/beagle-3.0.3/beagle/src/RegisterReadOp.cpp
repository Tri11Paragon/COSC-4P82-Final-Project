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
 *  \file   beagle/src/RegisterReadOp.cpp
 *  \brief  Source code of class RegisterReadOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:21 $
 */

#include "beagle/Beagle.hpp"

#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a parameters reading operator.
 *  \param inName Name of the read parameter operator.
 */
RegisterReadOp::RegisterReadOp(Beagle::string inName) :
  Operator(inName),
  mReadingInterval(NULL)
{ }


/*!
 *  \brief Initialize the parameters reading operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void RegisterReadOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ec.conf.readinterval")) {
    mReadingInterval =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.conf.readinterval"));
  } else {
    mReadingInterval = new UInt(0);
    string lLongDescrip = "Interval between each register read, in number of generations. ";
    lLongDescrip += "The name of the register file is specified by parameter ";
    lLongDescrip += "\"rg.read.filename\". When zero, the register is read only once, ";
    lLongDescrip += "during initialization.";
    Register::Description lDescription(
      "Register reading interval",
      "UInt",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.conf.readinterval", mReadingInterval, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ec.conf.file")) {
    mFileName = castHandleT<String>(ioSystem.getRegister().getEntry("ec.conf.file"));
  }
  else {
    mFileName = new String("");
    string lLongDescripFN = "The name of a configuration file containing ";
    lLongDescripFN += "evolver and parameter values. A typical configuration file can ";
    lLongDescripFN += "be created with parameter \"ec.conf.dump\".";
    Register::Description lFileNameDescription(
      "Configuration filename",
      "String",
      "\"\"",
      lLongDescripFN
    );
    ioSystem.getRegister().addEntry("ec.conf.file", mFileName, lFileNameDescription);
  }
  Beagle_StackTraceEndM("void RegisterReadOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the characteristic parameters reading operation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 */
void RegisterReadOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(mReadingInterval->getWrappedValue() == 0) return;
  if(ioContext.getGeneration() == 0) return;
  if((ioContext.getGeneration() % mReadingInterval->getWrappedValue()) != 0) return;
  if(ioContext.getDemeIndex() != 0) return;
  if(mFileName->getWrappedValue().empty()) return;
  if(mFileName->getWrappedValue() == "") return;
  ioContext.getSystem().getRegister().readParametersFile(mFileName->getWrappedValue(),
                                                         ioContext.getSystem());
  Beagle_StackTraceEndM("void RegisterReadOp::operate(Deme& ioDeme, Context& ioContext)");
}
