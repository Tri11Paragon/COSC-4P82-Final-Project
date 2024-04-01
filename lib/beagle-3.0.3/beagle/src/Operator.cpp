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
 *  \file   beagle/src/Operator.cpp
 *  \brief  Source code of class Operator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:20 $
 */

#include "beagle/Beagle.hpp"

#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a new operator.
 *  \param inName Name of the operator.
 */
Operator::Operator(Beagle::string inName) :
  NamedObject(inName),
  mInitialized(false),
  mPostInitialized(false)
{ }


/*!
 *  \brief Give a reference to the current operator.
 *  \return Handle to the refered operator.
 */
PointerT<Operator,NamedObject::Handle> Operator::giveReference()
{
  Beagle_StackTraceBeginM();
  return Operator::Handle(this);
  Beagle_StackTraceEndM("PointerT<Operator,NamedObject::Handle> Operator::giveReference()");
}


/*!
 *  \brief Reading a operator with method read is undefined. Use readWithMap instead.
 */
void Operator::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("read","Operator",getName());
  Beagle_StackTraceEndM("void Operator::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Read an operator from a XML subtree.
 *  \param inIter XML iterator from which the operator is read.
 *  \param inOpMap Operator map to use to read operator.
 *  \throw IOException If a reading error occurs.
 */
void Operator::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  Beagle_StackTraceEndM("void Operator::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write an operator into an XML stream.
 *  \param ioStreamer XML stream in which the operator is written.
 *  \param inIndent Whether XML output should be indented.
 */
void Operator::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag(getName().c_str(), inIndent);
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Operator::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

