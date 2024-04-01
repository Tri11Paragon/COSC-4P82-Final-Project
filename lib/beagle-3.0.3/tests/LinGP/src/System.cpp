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
 *  \file   beagle/LinGP/src/System.cpp
 *  \brief  Implementation of the type LinGP::System.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

#include "beagle/GP.hpp"
#include "beagle/LinGP/System.hpp"
#include "beagle/LinGP/InstructionSuperSet.hpp"
#include "beagle/LinGP/Context.hpp"
#include "beagle/System.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new linear GP system.
 */
LinGP::System::System() :
  Beagle::System(new LinGP::Context::Alloc),
  mInstructionSuperSet(new LinGP::InstructionSuperSet)
{ }


/*!
 *  \brief Construct a new linear GP system.
 *  \param inSuperSet Handle to the instruction super set to use.
 */
LinGP::System::System(LinGP::InstructionSuperSet::Handle inSuperSet) :
  Beagle::System(new LinGP::Context::Alloc),
  mInstructionSuperSet(inSuperSet)
{ }


/*!
 *  \brief Construct a new linear GP system.
 *  \param inSuperSet Handle to the instruction super set to use.
 *  \param inContextAllocator Context allocator to use.
 */
LinGP::System::System(LinGP::InstructionSuperSet::Handle inSuperSet,
                      Beagle::Context::Alloc::Handle inContextAllocator) :
  Beagle::System(inContextAllocator),
  mInstructionSuperSet(inSuperSet)
{ }


/*!
 *  \brief Construct a new linear GP system.
 *  \param inSuperSet Handle to the instruction super set to use.
 *  \param inContextAllocator Context allocator to use.
 *  \param inRandomizer Handle to the randomizer used.
 *  \param inRegister Handle to the register used.
 *  \param inLogger Handle to the logger used.
 */
LinGP::System::System(LinGP::InstructionSuperSet::Handle inSuperSet,
                      Beagle::Context::Alloc::Handle inContextAllocator,
                      Randomizer::Handle inRandomizer,
                      Register::Handle inRegister,
                      Logger::Handle inLogger) :
  Beagle::System(inContextAllocator, inRandomizer, inRegister, inLogger),
  mInstructionSuperSet(inSuperSet)
{ }


/*!
 *  \brief Construct a new linear GP system.
 *  \param inSet Handle to the instruction set to use.
 */
LinGP::System::System(LinGP::InstructionSet::Handle inSet) :
  Beagle::System(new LinGP::Context::Alloc),
  mInstructionSuperSet(new InstructionSuperSet)
{
  mInstructionSuperSet->insert(inSet);
}


/*!
 *  \brief Construct a new linear GP system.
 *  \param inSet Handle to the instruction set to use.
 *  \param inContextAllocator Context allocator to use.
 */
LinGP::System::System(LinGP::InstructionSet::Handle inSet,
                      Beagle::Context::Alloc::Handle inContextAllocator) :
  Beagle::System(inContextAllocator),
  mInstructionSuperSet(new InstructionSuperSet)
{
  mInstructionSuperSet->insert(inSet);
}


/*!
 *  \brief Construct a new linear GP system.
 *  \param inSet Handle to the instruction set to use.
 *  \param inContextAllocator Context allocator to use.
 *  \param inRandomizer Handle to the randomizer used.
 *  \param inRegister Handle to the register used.
 *  \param inLogger Handle to the logger used.
 */
LinGP::System::System(LinGP::InstructionSet::Handle inSet,
                      Beagle::Context::Alloc::Handle inContextAllocator,
                      Randomizer::Handle inRandomizer,
                      Register::Handle inRegister,
                      Logger::Handle inLogger) :
  Beagle::System(inContextAllocator, inRandomizer, inRegister, inLogger),
  mInstructionSuperSet(new InstructionSuperSet)
{
  mInstructionSuperSet->insert(inSet);
}


/*!
 *  \brief Initialize the system.
 *  \param ioArgc Number of elements on the command-line.
 *  \param ioArgv Element on the command-line.
 */
void LinGP::System::initialize(int& ioArgc, char** ioArgv)
{
  // Initializing the system.
  Beagle_LogDetailedM(
    getLogger(),
    "system", "Beagle::LinGP::System",
    "Initializing the system"
  );
  mLogger->initialize(*this);
  mRandomizer->initialize(*this);
  mRegister->initialize(*this);
  mInstructionSuperSet->initialize(*this);

  // Parsing the command-line.
  Beagle_LogTraceM(
    (*mLogger),
    "system", "Beagle::LinGP::System",
    "Parsing the command-line"
  );
  mRegister->parseCommandLine(*this, ioArgc, ioArgv);
}


/*!
 *  \brief Initialize the system.
 *  \param inRegisterFileName Name of the filename containing the system configuration.
 */
void LinGP::System::initialize(string inRegisterFileName)
{
  // Initializing the system.
  Beagle_LogDetailedM(
    getLogger(),
    "system", "Beagle::LinGP::System",
    "Initializing the system"
  );
  mLogger->initialize(*this);
  mRandomizer->initialize(*this);
  mRegister->initialize(*this);
  mInstructionSuperSet->initialize(*this);

  // Reading configuration file.
  if(inRegisterFileName.empty() == false) {
    Beagle_LogTraceM(
      (*mLogger),
      "system", "Beagle::LinGP::System",
      "Reading system configuration file"
    );
    mRegister->readParametersFile(inRegisterFileName, *this);
  }
}


