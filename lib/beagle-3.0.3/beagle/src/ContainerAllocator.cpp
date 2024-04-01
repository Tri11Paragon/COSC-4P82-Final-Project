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
 *  \file   beagle/src/ContainerAllocator.cpp
 *  \brief  Source code of class ContainerAllocator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Constructor of container allocator.
 *  \param inContainerTypeAlloc Type allocator given to the container allocated.
 */
ContainerAllocator::ContainerAllocator(Object::Alloc::Handle inContainerTypeAlloc) :
  mContainerTypeAlloc(inContainerTypeAlloc)
{ }


/*!
 *  \brief  Allocate a plain new container on the heap by giving it the type allocator.
 *  \return Pointer to the allocated container.
 *  \throw InternalException When the program goes out of memory.
 */
Object* ContainerAllocator::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  Container* lContainer = new Container(mContainerTypeAlloc);
  if(!lContainer) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lContainer;
#else // BEAGLE_NDEBUG
  return new Container(mContainerTypeAlloc);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* ContainerAllocator::allocate() const");
}


/*!
 *  \brief  Allocate a new container on the heap that is a clone an existing container.
 *  \param  inOrigObj Constant reference to the original container object to clone.
 *  \return Pointer to the allocated container.
 *  \throw  InternalException When the program goes out of memory.
 *  \throw  BadCastException If the type of the parameter is not as specified.
 */
Object* ContainerAllocator::clone(const Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const Container& lOrigContainer = castObjectT<const Container&>(inOrigObj);
  Container* lContainer = new Container(lOrigContainer);
  if(!lContainer) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lContainer;
#else // BEAGLE_NDEBUG
  const Container& lOrigContainer = castObjectT<const Container&>(inOrigObj);
  return new Container(lOrigContainer);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* ContainerAllocator::clone(const Object& inOrigObj) const");
}


/*!
 *  \brief  Allocate a new container on the heap that is a clone an existing container with a copy
 *    of its data.
 *  \param  inOrigObj Constant reference to the original container object to clone.
 *  \return Pointer to the allocated container.
 *  \throw  InternalException When the program goes out of memory.
 */
Container* ContainerAllocator::cloneData(const Container& inOrigObj) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  Container* lContainer = new Container(mContainerTypeAlloc);
  if(!lContainer) throw Beagle_InternalExceptionM("Out-of-memory!");
  lContainer->copyData(inOrigObj);
  return lContainer;
#else // BEAGLE_NDEBUG
  Container* lContainer = new Container(mContainerTypeAlloc);
  lContainer->copyData(inOrigObj);
  return lContainer;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Container* ContainerAllocator::cloneData(const Container& inOrigObj) const");
}


/*!
 *  \brief Copy a container into another.
 *  \param outCopyObj Constant reference to the original container to copy.
 *  \param inOrigObj Reference to the container that is a copy of the original.
 *  \throw BasCastException If the types of the parameters are not as specified.
 */
void ContainerAllocator::copy(Object& outCopyObj, const Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  Container& lCopyContainer = castObjectT<Container&>(outCopyObj);
  const Container& lOrigContainer = castObjectT<const Container&>(inOrigObj);
  lCopyContainer = lOrigContainer;
  Beagle_StackTraceEndM("void ContainerAllocator::copy(Object& outCopyObj, const Object& inOrigObj) const");
}


/*!
 *  \brief Copy a container into another, with a clone of its data.
 *  \param outCopyObj Constant reference to the original container to copy.
 *  \param inOrigObj Reference to the container that is a copy of the original.
 */
void ContainerAllocator::copyData(Container& outCopyObj, const Container& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  outCopyObj.copyData(inOrigObj);
  Beagle_StackTraceEndM("void ContainerAllocator::copyData(Container& outCopyObj, const Container& inOrigObj) const");
}

