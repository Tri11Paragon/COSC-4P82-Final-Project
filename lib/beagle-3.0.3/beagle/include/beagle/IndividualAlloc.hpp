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
 *  \file   beagle/IndividualAlloc.hpp
 *  \brief  Definition of the class IndividualAlloc.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:14 $
 */

#ifndef Beagle_IndividualAlloc_hpp
#define Beagle_IndividualAlloc_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/ContainerAllocatorT.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/Fitness.hpp"

namespace Beagle {

/*!
 *  \brief Typedef of the type from which class Beagle::IndividualAlloc inherits.
 *  \warning This is a workaround for MS Visual C++ .NET, which seems to have difficulty
 *    to fall on its feet with the type Genotype::Bag::Alloc.
 *  \related Beagle::IndividualAlloc
 *  \ingroup Allocs
 */
typedef Genotype::Bag::Alloc
        IndividualAllocBaseType;

/*!
 *  \class IndividualAlloc beagle/IndividualAlloc.hpp "beagle/IndividualAlloc.hpp"
 *  \brief Individual specific allocator type.
 *  \ingroup ECF
 *  \ingroup Pop
 *  \ingroup Allocs
 */
class IndividualAlloc : public IndividualAllocBaseType {

public:

  //! IndividualAlloc allocator type.
  typedef AllocatorT<IndividualAlloc,IndividualAllocBaseType::Alloc>
          Alloc;
  //! IndividualAlloc handle type.
  typedef PointerT<IndividualAlloc,IndividualAllocBaseType::Handle>
          Handle;
  //! IndividualAlloc bag type.
  typedef ContainerT<IndividualAlloc,IndividualAllocBaseType::Bag>
          Bag;

  explicit IndividualAlloc(Genotype::Alloc::Handle inGenotypeAlloc=NULL);
  explicit IndividualAlloc(Genotype::Alloc::Handle inGenotypeAlloc,
                           Fitness::Alloc::Handle inFitnessAlloc);
  virtual ~IndividualAlloc() { }

  virtual Object*    allocate() const;
  virtual Object*    clone(const Object& inOrigObj) const;
  virtual Container* cloneData(const Container& inOrigContainer) const;
  virtual void       copy(Object& outCopyObj, const Object& inOrigObj) const;
  virtual void       copyData(Container& outCopyContainer, const Container& inOrigContainer) const;

  /*!
   *  \brief Get the fitness allocator of the individual.
   *  \return Fitness allocator.
   */
  inline const Fitness::Alloc::Handle getFitnessAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mFitnessAlloc;
    Beagle_StackTraceEndM("const Fitness::Alloc::Handle IndividualAlloc::getFitnessAlloc() const");
  }

  /*!
   *  \brief Get the fitness allocator of the individual.
   *  \return Fitness allocator.
   */
  inline Fitness::Alloc::Handle getFitnessAlloc()
  {
    Beagle_StackTraceBeginM();
    return mFitnessAlloc;
    Beagle_StackTraceEndM("Fitness::Alloc::Handle IndividualAlloc::getFitnessAlloc()");
  }

  /*!
   *  \brief Set the fitness allocator of the individual.
   *  \param inFitnessAlloc Fitness allocator.
   */
  inline void setFitnessAlloc(Fitness::Alloc::Handle inFitnessAlloc)
  {
    Beagle_StackTraceBeginM();
    mFitnessAlloc = inFitnessAlloc;
    Beagle_StackTraceEndM("void IndividualAlloc::setFitnessAlloc(Fitness::Alloc::Handle inFitnessAlloc)");
  }

  /*!
   *  \brief Get the genotype allocator.
   *  \return Constant handle to the genotype allocator.
   */
  inline const Genotype::Alloc::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<const Genotype::Alloc>(mContainerTypeAlloc);
    Beagle_StackTraceEndM("const Genotype::Alloc::Handle IndividualAlloc::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the genotype allocator.
   *  \return Handle to the genotype allocator.
   */
  inline Genotype::Alloc::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<Genotype::Alloc>(mContainerTypeAlloc);
    Beagle_StackTraceEndM("Genotype::Alloc::Handle IndividualAlloc::getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the genotype allocator.
   *  \param inGenotypeAlloc The new genotype allocator.
   */
  inline void setContainerTypeAlloc(Genotype::Alloc::Handle inGenotypeAlloc)
  {
    Beagle_StackTraceBeginM();
    mContainerTypeAlloc = inGenotypeAlloc;
    Beagle_StackTraceEndM("void IndividualAlloc::setContainerTypeAlloc(Genotype::Alloc::Handle inGenotypeAlloc)");
  }

protected:

  Fitness::Alloc::Handle mFitnessAlloc;  //!< Fitness allocator.

};

}

#endif // Beagle_IndividualAlloc_hpp
