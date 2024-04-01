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
 *  \file   beagle/LinGP/src/InstructionSuperSet.cpp
 *  \brief  Implementation of the type LinGP::InstructionSuperSet.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

#include "beagle/GP.hpp"
#include "beagle/LinGP/InstructionSuperSet.hpp"
#include "beagle/LinGP/InstructionSet.hpp"
#include "beagle/LinGP/System.hpp"

using namespace Beagle;


/*!
 *  \brief Initialize the super set of instructions.
 */
void LinGP::InstructionSuperSet::initialize(LinGP::System& ioSystem)
{
  for(unsigned int i=0; i<size(); i++) (*this)[i]->initialize(ioSystem);
}


/*!
 *  \brief Insert a nes set of instructions in the super set.
 *  \param inInstructionSet Inserted instruction set.
 */
void LinGP::InstructionSuperSet::insert(LinGP::InstructionSet::Handle inInstructionSet)
{
  push_back(inInstructionSet);
}

/*!
 *  \brief Read an instruction super set from a Beagle XML stream.
 *  \param inIter XML node used to read the super set from.
 *  \throw Beagle::IOException If size atribute not present.
 */
void LinGP::InstructionSuperSet::read(PACC::XML::ConstIterator inIter)
{
  if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "InstructionSuperSet"))
    throw Beagle_IOExceptionNodeM((*inIter), "tag <InstructionSuperSet> expected!");
  size_type lPSIndex = 0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="InstructionSet")) {
      (*this)[lPSIndex++]->read(lChild);
    }
  }
}


/*!
 *  \brief Write an instruction super set into a Beagle XML stream.
 *  \param ioStreamer XMl streamer to write the super set into.
 *  \param inIndent Whether XML output should be indented.
 */
void LinGP::InstructionSuperSet::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  ioStreamer.openTag("InstructionSuperSet", inIndent);
  for(unsigned int i=0; i<size(); i++) (*this)[i]->write(ioStreamer);
  ioStreamer.closeTag();
}
