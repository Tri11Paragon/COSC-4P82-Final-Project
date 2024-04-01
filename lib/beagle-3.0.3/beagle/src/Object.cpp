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
 *  \file   beagle/src/Object.cpp
 *  \brief  Source code of class Object.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <sstream>

using namespace Beagle;



/*!
 *  \brief Default constructor.
 */
Object::Object() :
  mRefCounter(0)
{ }


/*!
 *  \brief Copy constructor.
 *  \param inOriginal Object to copy.
 */
Object::Object(const Object& inOriginal) :
  mRefCounter(0)
{ }


/*!
 *  \brief Destructor.
 *  \throw Beagle::InternalException If reference counter is not equal to zero.
 */
Object::~Object()
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if(mRefCounter != 0) {
    string lMessage = "Reference counter of actual object is non-zero (value: ";
    lMessage += int2str(mRefCounter);
    lMessage += ")";
    throw Beagle_InternalExceptionM(lMessage);
  }
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object::~Object()");
}


/*!
 *  \brief Copy operator of Object.
 *  \param inOriginal Original object to copy.
 *  \return Actual object.
 */
Object& Object::operator=(const Object& inOriginal)
{
  Beagle_StackTraceBeginM();
  return *this;
  Beagle_StackTraceEndM("Object& Object::operator=(const Object& inOriginal)");
}


/*!
 *  \brief Returns the name of the object, in this case "Non-named Object".
 */
const Beagle::string& Object::getName() const
{
  Beagle_StackTraceBeginM();
  const static string mName("Non-named Object");
  return mName;
  Beagle_StackTraceEndM("const string& Object::getName() const");
}


/*!
 *  \brief  Test if an object is equal to another.
 *  \param  inRightObj Object to be compare to.
 *  \return True if actual object is equal to right object, false if not.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
bool Object::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("isEqual","Object",getName());
  Beagle_StackTraceEndM("bool Object::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief  Test if an object is less than another.
 *  \param  inRightObj Object to be compare to.
 *  \return True if actual object is less than right object, false if not.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
bool Object::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("isLess","Object",getName());
  Beagle_StackTraceEndM("bool Object::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Extract Object from a XML tree.
 *  \param inIter XML iterator to read the object from.
 *  \throw Beagle::InternalException If the method is not overdefined in a subclass.
 */
void Object::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("read", "Object", getName());
  Beagle_StackTraceEndM("void Object::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Transform an Object into a linear string (using write method).
 *  \param inIndent Whether serialized output should be indented.
 *  \param inIndentWidth Number of spaces for each indent. Supplying a value of zero
 *         will produce the data without indentation and without any newlines.
 *  \return String representing the Object.
 */
Beagle::string Object::serialize(bool inIndent, unsigned int inIndentWidth) const
{
  Beagle_StackTraceBeginM();
  std::ostringstream lOSS;
  PACC::XML::Streamer lStreamer(lOSS, inIndentWidth);
  write(lStreamer, inIndent);
  return lOSS.str().c_str();
  Beagle_StackTraceEndM("string Object::serialize(bool inIndent, unsigned int inIndentWidth) const");
}


/*!
 *  \brief Insert an Object into a XML streamer.
 *  \param ioStreamer XML streamer to write the object into.
 *  \param inIndent Whether XML output should be indented.
 *  \throw  Beagle::InternalException If the method is not overdefined in a subclass.
 */
void Object::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("write", "Object", getName());
  Beagle_StackTraceEndM("void Object::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


