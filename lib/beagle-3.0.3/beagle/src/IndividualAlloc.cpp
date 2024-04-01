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
 *  \file   beagle/src/IndividualAlloc.cpp
 *  \brief  Source code of class IndividualAlloc.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Constructor of individual allocator from a genotype allocator.
 *  \param inGenotypeAlloc Type allocator given to the genotype allocated.
 */
IndividualAlloc::IndividualAlloc(Genotype::Alloc::Handle inGenotypeAlloc) :
  IndividualAllocBaseType(inGenotypeAlloc),
  mFitnessAlloc(new FitnessSimple::Alloc)
{ }


/*!
 *  \brief Constructor of individual allocator from a genotype allocator and a fitness allocator.
 *  \param inGenotypeAlloc Type allocator given to the genotype allocated.
 *  \param inFitnessAlloc Fitness type allocator.
 */
IndividualAlloc::IndividualAlloc(Genotype::Alloc::Handle inGenotypeAlloc,
                                 Fitness::Alloc::Handle inFitnessAlloc) :
  IndividualAllocBaseType(inGenotypeAlloc),
  mFitnessAlloc(inFitnessAlloc)
{ }


/*!
 *  \brief  Allocate a new individual and the heap by giving it the type allocator.
 *  \return Pointer to the allocated individual.
 */
Object* IndividualAlloc::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  Individual* lIndividual =
    new Individual(castHandleT<Genotype::Alloc>(mContainerTypeAlloc), mFitnessAlloc);
  if(!lIndividual) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lIndividual;
#else // BEAGLE_NDEBUG
  return new Individual(castHandleT<Genotype::Alloc>(mContainerTypeAlloc), mFitnessAlloc);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* IndividualAlloc::allocate() const");
}


/*!
 *  \brief  Allocate a new individual on the heap that is a clone an existing individual.
 *  \param  inOrigObj Constant reference to the original individual object to clone.
 *  \return Pointer to the allocated individual.
 *  \throw  InternalException If the program run out of memory.
 *  \throw  BadCastException If the type asserted is not valid.
 */
Object* IndividualAlloc::clone(const Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const Individual& lOrigIndiv = castObjectT<const Individual&>(inOrigObj);
  Individual* lIndividual = new Individual(lOrigIndiv);
  if(!lIndividual) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lIndividual;
#else // BEAGLE_NDEBUG
  const Individual& lOrigIndiv = castObjectT<const Individual&>(inOrigObj);
  return new Individual(lOrigIndiv);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* IndividualAlloc::clone(const Object& inOrigObj) const");
}


/*!
 *  \brief  Allocate a new individual on the heap that is a clone an existing individual.
 *  \param  inOrigContainer Constant reference to the original individual object to clone.
 *  \return Pointer to the allocated individual.
 *  \throw  InternalException If the program run out of memory.
 *  \throw  BadCastException If the type asserted is not valid.
 */
Container* IndividualAlloc::cloneData(const Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const Individual& lOrigIndiv = castObjectT<const Individual&>(inOrigContainer);
  Individual* lIndividual =
    new Individual(castHandleT<Genotype::Alloc>(mContainerTypeAlloc), mFitnessAlloc);
  if(!lIndividual) throw Beagle_InternalExceptionM("Out-of-memory!");
  lIndividual->copyData(lOrigIndiv);
  return lIndividual;
#else // BEAGLE_NDEBUG
  const Individual& lOrigIndiv = castObjectT<const Individual&>(inOrigContainer);
  Individual* lIndividual =
    new Individual(castHandleT<Genotype::Alloc>(mContainerTypeAlloc), mFitnessAlloc);
  lIndividual->copyData(lOrigIndiv);
  return lIndividual;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Container* IndividualAlloc::cloneData(const Container& inOrigContainer) const");
}


/*!
 *  \brief Copy an individual into another.
 *  \param outCopyObj Constant reference to the original individual to copy.
 *  \param inOrigObj  Reference to the individual that is a copy of the original.
 *  \throw BadCastException If the type asserted is not valid.
 */
void IndividualAlloc::copy(Object& outCopyObj, const Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  Individual& lCopyIndiv = castObjectT<Individual&>(outCopyObj);
  const Individual& lOrigIndiv = castObjectT<const Individual&>(inOrigObj);
  lCopyIndiv = lOrigIndiv;
  Beagle_StackTraceEndM("void IndividualAlloc::copy(Object& outCopyObj, const Object& inOrigObj) const");
}


/*!
 *  \brief Copy an individual into another.
 *  \param outCopyContainer Constant reference to the original individual to copy.
 *  \param inOrigContainer Reference to the individual that is a copy of the original.
 *  \throw BadCastException If the type asserted is not valid.
 */
void IndividualAlloc::copyData(Container& outCopyContainer, const Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
  Individual& lCopyIndiv = castObjectT<Individual&>(outCopyContainer);
  const Individual& lOrigIndiv = castObjectT<const Individual&>(inOrigContainer);
  lCopyIndiv.copyData(lOrigIndiv);
  Beagle_StackTraceEndM("void IndividualAlloc::copyData(Container& outCopyContainer, const Container& inOrigContainer) const");
}

