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
 *  \file   beagle/IndividualAllocT.hpp
 *  \brief  Implementation of the class template IndividualAllocT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:14 $
 */

#ifndef Beagle_IndividualAllocT_hpp
#define Beagle_IndividualAllocT_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/IndividualAlloc.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/Fitness.hpp"
#include "beagle/BadCastException.hpp"

namespace Beagle {


/*!
 *  \class IndividualAllocT beagle/IndividualAllocT.hpp "beagle/IndividualAllocT.hpp"
 *  \brief Allocator template class for individual derived types.
 *  \param T The individual type allocated.
 *  \param BaseType The base type from which the actual allocator type is derived.
 *  \param GenotypeAllocType Genotype allocator type used to fill the individuals.
 *  \ingroup ECF
 *  \ingroup Pop
 *  \ingroup Allocs
 */
template <class T, class BaseType, class GenotypeAllocType>
class IndividualAllocT : public BaseType {

public:

  //! IndividualAllocT allocator type.
  typedef AllocatorT<IndividualAllocT<T,BaseType,GenotypeAllocType>,typename BaseType::Alloc>
          Alloc;
  //! IndividualAllocT handle type.
  typedef PointerT<IndividualAllocT<T,BaseType,GenotypeAllocType>,typename BaseType::Handle>
          Handle;
  //! IndividualAllocT mixed bag type.
  typedef ContainerT<IndividualAllocT<T,BaseType,GenotypeAllocType>,typename BaseType::Bag>
          Bag;

  explicit IndividualAllocT(typename GenotypeAllocType::Handle inGenotypeAlloc=NULL);
  explicit IndividualAllocT(typename GenotypeAllocType::Handle inGenotypeAlloc,
                            Beagle::Fitness::Alloc::Handle inFitnessAlloc);
  virtual ~IndividualAllocT() { }

  virtual Object*    allocate() const;
  virtual Object*    clone(const Object& inOrigObj) const;
  virtual Container* cloneData(const Container& inOrigContainer) const;
  virtual void       copy(Object& outCopyObj, const Object& inOrigObj) const;
  virtual void       copyData(Container& outCopyContainer, const Container& inOrigContainer) const;

  /*!
   *  \brief Get the genotype allocator.
   *  \return Constant handle to the genotype allocator.
   */
  inline const typename GenotypeAllocType::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<const GenotypeAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("const GenotypeAllocType::Handle IndividualAllocT<T,BaseType,GenotypeAllocType>::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the bag contained type allocator.
   *  \return Handle to the bag contained type allocator.
   */
  inline typename GenotypeAllocType::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<GenotypeAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("GenotypeAllocType::Handle IndividualAllocT<T,BaseType,GenotypeAllocType>getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the bag contained type allocator.
   *  \param inGenotypeAllocType The new bag contained type allocator.
   */
  inline void setContainerTypeAlloc(typename GenotypeAllocType::Handle inGenotypeAllocType)
  {
    Beagle_StackTraceBeginM();
    BaseType::mContainerTypeAlloc = inGenotypeAllocType;
    Beagle_StackTraceEndM("void IndividualAllocT<T,BaseType,GenotypeAllocType>setContainerTypeAlloc(GenotypeAllocType::Handle inGenotypeAllocType)");
  }

};

}


/*!
 *  \brief Constructor of individual allocator from a genotype allocator and a fitness allocator.
 *  \param inGenotypeAlloc Type allocator given to the genotype allocated.
 */
template <class T, class BaseType, class GenotypeAllocType>
Beagle::IndividualAllocT<T,BaseType,GenotypeAllocType>::IndividualAllocT(
  typename GenotypeAllocType::Handle inGenotypeAlloc) :
  BaseType(inGenotypeAlloc)
{ }


/*!
 *  \brief Constructor of individual allocator from a genotype allocator and a fitness allocator.
 *  \param inGenotypeAlloc Type allocator given to the genotype allocated.
 *  \param inFitnessAlloc Fitness type allocator.
 */
template <class T, class BaseType, class GenotypeAllocType>
Beagle::IndividualAllocT<T,BaseType,GenotypeAllocType>::IndividualAllocT(
  typename GenotypeAllocType::Handle inGenotypeAlloc,
  Beagle::Fitness::Alloc::Handle inFitnessAlloc) :
  BaseType(inGenotypeAlloc, inFitnessAlloc)
{ }


/*!
 *  \brief  Allocate a new individual and the heap by giving it the type allocator.
 *  \return Pointer to the allocated individual.
 */
template <class T, class BaseType, class GenotypeAllocType>
Beagle::Object* Beagle::IndividualAllocT<T,BaseType,GenotypeAllocType>::allocate() const
{
#ifndef BEAGLE_NDEBUG
  T* lT = new T(castHandleT<GenotypeAllocType>(BaseType::mContainerTypeAlloc), 
                BaseType::mFitnessAlloc);
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lT;
#else // BEAGLE_NDEBUG
  return new T(castHandleT<GenotypeAllocType>(BaseType::mContainerTypeAlloc),
               BaseType::mFitnessAlloc);
#endif // BEAGLE_NDEBUG
}


/*!
 *  \brief  Allocate a new individual on the heap that is a clone an existing individual.
 *  \param  inOrigObj Constant reference to the original individual object to clone.
 *  \return Pointer to the allocated individual.
 */
template <class T, class BaseType, class GenotypeAllocType>
Beagle::Object* Beagle::IndividualAllocT<T,BaseType,GenotypeAllocType>::clone(
  const Beagle::Object& inOrigObj) const
{
#ifndef BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  T* lT = new T(lOrigT);
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lT;
#else // BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  return new T(lOrigT);
#endif // BEAGLE_NDEBUG
}


/*!
 *  \brief  Allocate a new individual on the heap that is a clone an existing individual.
 *    Make a complete copy of the individuals.
 *  \param  inOrigContainer Constant reference to the original individual object to clone.
 *  \return Pointer to the allocated individual.
 */
template <class T, class BaseType, class GenotypeAllocType>
Beagle::Container* Beagle::IndividualAllocT<T,BaseType,GenotypeAllocType>::cloneData(
  const Beagle::Container& inOrigContainer) const
{
#ifndef BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  T* lT = new T(castHandleT<GenotypeAllocType>(BaseType::mContainerTypeAlloc),
                BaseType::mFitnessAlloc);
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  lT->copyData(lOrigT);
  return lT;
#else // BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  T* lT = new T(castHandleT<GenotypeAllocType>(BaseType::mContainerTypeAlloc),
                BaseType::mFitnessAlloc);
  lT->copyData(lOrigT);
  return lT;
#endif // BEAGLE_NDEBUG
}


/*!
 *  \brief Copy a individual into another.
 *  \param outCopyObj Constant reference to the original individual to copy.
 *  \param inOrigObj  Reference to the individual that is a copy of the original.
 */
template <class T, class BaseType, class GenotypeAllocType>
void Beagle::IndividualAllocT<T,BaseType,GenotypeAllocType>::copy(
  Beagle::Object& outCopyObj, const Beagle::Object& inOrigObj) const
{
  T& lCopyT = castObjectT<T&>(outCopyObj);
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  lCopyT = lOrigT;
}


/*!
 *  \brief Copy a individual into another with the contained data.
 *  \param outCopyContainer Constant reference to the original individual to copy.
 *  \param inOrigContainer  Reference to the individual that is a copy of the original.
 */
template <class T, class BaseType, class GenotypeAllocType>
void Beagle::IndividualAllocT<T,BaseType,GenotypeAllocType>::copyData(
  Beagle::Container& outCopyContainer, const Beagle::Container& inOrigContainer) const
{
  T& lCopyT = castObjectT<T&>(outCopyContainer);
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  lCopyT.copyData(lOrigT);
}


#endif // Beagle_IndividualAlloc_hpp
