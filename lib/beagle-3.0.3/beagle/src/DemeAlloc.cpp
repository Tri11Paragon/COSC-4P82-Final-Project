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
 *  \file   beagle/src/DemeAlloc.cpp
 *  \brief  Source code of class DemeAlloc.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Constructor of deme allocator from an individual allocator.
 *  \param inIndividualAlloc Type allocator given to the individual allocated.
 */
DemeAlloc::DemeAlloc(Individual::Alloc::Handle inIndividualAlloc) :
  DemeAllocBaseType(inIndividualAlloc),
  mHOFAlloc(new HallOfFame::Alloc),
  mStatsAlloc(new Stats::Alloc)
{ }


/*!
 *  \brief Constructor of deme allocator from an individual allocator and a statistics allocator.
 *  \param inIndividualAlloc Type allocator given to the individual allocated.
 *  \param inStatsAlloc Statistics type allocator.
 *  \param inHOFAlloc Hall-of-fame allocator.
 */
DemeAlloc::DemeAlloc(Individual::Alloc::Handle inIndividualAlloc,
                     Stats::Alloc::Handle inStatsAlloc,
                     HallOfFame::Alloc::Handle inHOFAlloc) :
  DemeAllocBaseType(inIndividualAlloc),
  mHOFAlloc(inHOFAlloc),
  mStatsAlloc(inStatsAlloc)
{ }


/*!
 *  \brief  Allocate a new deme and the heap by giving it the type allocator.
 *  \return Pointer to the allocated Deme.
 */
Object* DemeAlloc::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  Deme* lDeme = new Deme(castHandleT<Individual::Alloc>(mContainerTypeAlloc),
                         mStatsAlloc, mHOFAlloc);
  if(!lDeme) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lDeme;
#else // BEAGLE_NDEBUG
  return new Deme(castHandleT<Individual::Alloc>(mContainerTypeAlloc),
                  mStatsAlloc, mHOFAlloc);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* DemeAlloc::allocate() const");
}


/*!
 *  \brief  Allocate a new deme on the heap that is a clone an existing deme.
 *  \param  inOrigObj Constant reference to the original deme object to clone.
 *  \return Pointer to the allocated deme.
 *  \throw  InternalException If the program run out of memory.
 *  \throw  BadCastException If the type asserted is not valid.
 */
Object* DemeAlloc::clone(const Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const Deme& lOrigIndiv = castObjectT<const Deme&>(inOrigObj);
  Deme* lDeme = new Deme(lOrigIndiv);
  if(!lDeme) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lDeme;
#else // BEAGLE_NDEBUG
  const Deme& lOrigIndiv = castObjectT<const Deme&>(inOrigObj);
  return new Deme(lOrigIndiv);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* DemeAlloc::clone(const Object& inOrigObj) const");
}


/*!
 *  \brief  Allocate a new deme on the heap that is a clone an existing deme.
 *  \param  inOrigContainer Constant reference to the original deme object to clone.
 *  \return Pointer to the allocated deme.
 *  \throw  InternalException If the program run out of memory.
 *  \throw  BadCastException If the type asserted is not valid.
 */
Container* DemeAlloc::cloneData(const Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const Deme& lOrigIndiv = castObjectT<const Deme&>(inOrigContainer);
  Deme* lDeme = new Deme(castHandleT<Individual::Alloc>(mContainerTypeAlloc),
                         mStatsAlloc, mHOFAlloc);
  if(!lDeme) throw Beagle_InternalExceptionM("Out-of-memory!");
  lDeme->copyData(lOrigIndiv);
  return lDeme;
#else // BEAGLE_NDEBUG
  const Deme& lOrigIndiv = castObjectT<const Deme&>(inOrigContainer);
  Deme* lDeme = new Deme(castHandleT<Individual::Alloc>(mContainerTypeAlloc),
                         mStatsAlloc, mHOFAlloc);
  lDeme->copyData(lOrigIndiv);
  return lDeme;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Container* DemeAlloc::cloneData(const Container& inOrigContainer) const");
}


/*!
 *  \brief Copy a deme into another.
 *  \param outCopyObj Constant reference to the original deme to copy.
 *  \param inOrigObj  Reference to the deme that is a copy of the original.
 *  \throw BadCastException If the type asserted is not valid.
 */
void DemeAlloc::copy(Object& outCopyObj, const Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  Deme& lCopyIndiv = castObjectT<Deme&>(outCopyObj);
  const Deme& lOrigIndiv = castObjectT<const Deme&>(inOrigObj);
  lCopyIndiv = lOrigIndiv;
  Beagle_StackTraceEndM("void DemeAlloc::copy(Object& outCopyObj, const Object& inOrigObj) const");
}


/*!
 *  \brief Copy a deme into another.
 *  \param outCopyContainer Constant reference to the original deme to copy.
 *  \param inOrigContainer Reference to the deme that is a copy of the original.
 *  \throw BadCastException If the type asserted is not valid.
 */
void DemeAlloc::copyData(Container& outCopyContainer, const Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
  Deme& lCopyIndiv = castObjectT<Deme&>(outCopyContainer);
  const Deme& lOrigIndiv = castObjectT<const Deme&>(inOrigContainer);
  lCopyIndiv.copyData(lOrigIndiv);
  Beagle_StackTraceEndM("void DemeAlloc::copyData(Container& outCopyContainer, const Container& inOrigContainer) const");
}

