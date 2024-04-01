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
 *  \file   beagle/src/Component.cpp
 *  \brief  Source code of class Component.
 *  \author Matthew Walker
 *  \author Christian Gagne 
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Component class constructor.
 *  \param inName Name of the component.
 */
Component::Component(Beagle::string inName) :
  NamedObject(inName) 
{ }


/*!
 *  \brief Initialize the component.
 *  \param ioSystem Evolutionary system.
 */
void Component::initialize(System& ioSystem)
{ }


/*!
 *  \brief Post-initialize the component.
 *  \param ioSystem Evolutionary system.
 */
void Component::postInit(System& ioSystem)
{ }


/*!
 *  \brief Read the system component.
 *  \param inIter Iterator to XML node to read component from.
 *  \param ioSystem Evolutionary system.
 */
void Component::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(inIter->getType() != PACC::XML::eData)
    throw Beagle_IOExceptionNodeM(*inIter, "tag expected!");

  if(inIter->getValue() != getName().c_str()) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected, but ";
    lOSS << "got tag <" << inIter->getValue() << "> instead!";
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  Beagle_StackTraceEndM("void Component::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)");
}


/*!
 *  \brief Write the system component.
 *  \param ioStreamer XML streamer used to write component into.
 *  \param inIndent Whether XML output should be indented.
 */
void Component::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag(getName().c_str(), inIndent);
  writeContent(ioStreamer, inIndent);
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Component::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


/*!
 *  \brief Write content of the system component.
 *  \param ioStreamer XML streamer used to write content of component.
 *  \param inIndent Whether XML output should be indented.
 */
void Component::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{ }
