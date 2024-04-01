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
 *  \file   beagle/src/Allocator.cpp
 *  \brief  Source code of class Allocator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief  Allocate a new object on the heap.
 *  \return Pointer to the allocated object.
 */
Beagle::Object* Beagle::Allocator::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  Object* lObject = new Object;
  if(!lObject) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lObject;
#else // BEAGLE_NDEBUG
  return new Object;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Beagle::Object* Beagle::Allocator::allocate() const");
}


/*!
 *  \brief  Allocate a new object on the heap that is a clone of an existing object.
 *  \param  inOrigObj Constant reference to the original object to clone.
 *  \return Pointer to the allocated object.
 */
Beagle::Object* Beagle::Allocator::clone(const Beagle::Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  Object* lObject = new Object(inOrigObj);
  if(!lObject) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lObject;
#else // BEAGLE_NDEBUG
  return new Object(inOrigObj);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Beagle::Object* Beagle::Allocator::clone(const Beagle::Object& inOrigObj) const");
}


/*!
 *  \brief Copy an object to another.
 *  \param outCopyObj Constant reference to the original object to copy.
 *  \param inOrigObj  Reference to the object that is a copy of the original.
 */
void Beagle::Allocator::copy(Beagle::Object& outCopyObj,
                             const Beagle::Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  outCopyObj = inOrigObj;
  Beagle_StackTraceEndM("void Beagle::Allocator::copy(Beagle::Object& outCopyObj, const Beagle::Object& inOrigObj) const");
}

