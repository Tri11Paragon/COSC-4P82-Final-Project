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
 *  \file   beagle/src/VivariumAlloc.cpp
 *  \brief  Source code of class VivariumAlloc.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Constructor of vivarium allocator from a deme allocator.
 *  \param inDemeAlloc Type allocator given to the deme allocated.
 */
VivariumAlloc::VivariumAlloc(Deme::Alloc::Handle inDemeAlloc) :
  VivariumAllocBaseType(inDemeAlloc),
  mHOFAlloc(new HallOfFame::Alloc),
  mStatsAlloc(new Stats::Alloc)
{ }


/*!
 *  \brief Constructor of vivarium allocator from a deme allocator and a statistics allocator.
 *  \param inDemeAlloc Type allocator given to the deme allocated.
 *  \param inStatsAlloc Statistics allocator.
 *  \param inHOFAlloc Hall-of-fame allocator.
 */
VivariumAlloc::VivariumAlloc(Deme::Alloc::Handle inDemeAlloc,
                             Stats::Alloc::Handle inStatsAlloc,
                             HallOfFame::Alloc::Handle inHOFAlloc) :
  VivariumAllocBaseType(inDemeAlloc),
  mHOFAlloc(inHOFAlloc),
  mStatsAlloc(inStatsAlloc)
{ }


/*!
 *  \brief  Allocate a new vivarium and the heap by giving it the type allocator.
 *  \return Pointer to the allocated vivarium.
 */
Object* VivariumAlloc::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  Vivarium* lVivarium = new Vivarium(castHandleT<Deme::Alloc>(mContainerTypeAlloc),
                                     mStatsAlloc, mHOFAlloc);
  if(!lVivarium) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lVivarium;
#else // BEAGLE_NDEBUG
  return new Vivarium(castHandleT<Deme::Alloc>(mContainerTypeAlloc),
                      mStatsAlloc, mHOFAlloc);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* VivariumAlloc::allocate() const");
}


/*!
 *  \brief  Allocate a new vivarium on the heap that is a clone an existing vivarium.
 *  \param  inOrigObj Constant reference to the original vivarium object to clone.
 *  \return Pointer to the allocated vivarium.
 *  \throw  InternalException If the program run out of memory.
 *  \throw  BadCastException If the type asserted is not valid.
 */
Object* VivariumAlloc::clone(const Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const Vivarium& lOrigIndiv = castObjectT<const Vivarium&>(inOrigObj);
  Vivarium* lVivarium = new Vivarium(lOrigIndiv);
  if(!lVivarium) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lVivarium;
#else // BEAGLE_NDEBUG
  const Vivarium& lOrigIndiv = castObjectT<const Vivarium&>(inOrigObj);
  return new Vivarium(lOrigIndiv);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* VivariumAlloc::clone(const Object& inOrigObj) const");
}


/*!
 *  \brief  Allocate a new vivarium on the heap that is a clone an existing vivarium.
 *  \param  inOrigContainer Constant reference to the original vivarium object to clone.
 *  \return Pointer to the allocated vivarium.
 *  \throw  InternalException If the program run out of memory.
 *  \throw  BadCastException If the type asserted is not valid.
 */
Container* VivariumAlloc::cloneData(const Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const Vivarium& lOrigIndiv = castObjectT<const Vivarium&>(inOrigContainer);
  Vivarium* lVivarium = new Vivarium(castHandleT<Deme::Alloc>(mContainerTypeAlloc),
                                     mStatsAlloc, mHOFAlloc);
  if(!lVivarium) throw Beagle_InternalExceptionM("Out-of-memory!");
  lVivarium->copyData(lOrigIndiv);
  return lVivarium;
#else // BEAGLE_NDEBUG
  const Vivarium& lOrigIndiv = castObjectT<const Vivarium&>(inOrigContainer);
  Vivarium* lVivarium = new Vivarium(castHandleT<Deme::Alloc>(mContainerTypeAlloc),
                                     mStatsAlloc, mHOFAlloc);
  lVivarium->copyData(lOrigIndiv);
  return lVivarium;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Container* VivariumAlloc::cloneData(const Container& inOrigContainer) const");
}


/*!
 *  \brief Copy a vivarium into another.
 *  \param outCopyObj Constant reference to the original vivarium to copy.
 *  \param inOrigObj  Reference to the vivarium that is a copy of the original.
 *  \throw BadCastException If the type asserted is not valid.
 */
void VivariumAlloc::copy(Object& outCopyObj, const Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  Vivarium& lCopyIndiv = castObjectT<Vivarium&>(outCopyObj);
  const Vivarium& lOrigIndiv = castObjectT<const Vivarium&>(inOrigObj);
  lCopyIndiv = lOrigIndiv;
  Beagle_StackTraceEndM("void VivariumAlloc::copy(Object& outCopyObj, const Object& inOrigObj) const");
}


/*!
 *  \brief Copy a vivarium into another.
 *  \param outCopyContainer Constant reference to the original vivarium to copy.
 *  \param inOrigContainer Reference to the vivarium that is a copy of the original.
 *  \throw BadCastException If the type asserted is not valid.
 */
void VivariumAlloc::copyData(Container& outCopyContainer, const Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
  Vivarium& lCopyIndiv = castObjectT<Vivarium&>(outCopyContainer);
  const Vivarium& lOrigIndiv = castObjectT<const Vivarium&>(inOrigContainer);
  lCopyIndiv.copyData(lOrigIndiv);
  Beagle_StackTraceEndM("void VivariumAlloc::copyData(Container& outCopyContainer, const Container& inOrigContainer) const");
}

