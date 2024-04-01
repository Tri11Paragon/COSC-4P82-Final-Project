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
 *  \file   beagle/DemeAllocT.hpp
 *  \brief  Implementation of the class template DemeAllocT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:10 $
 */

#ifndef Beagle_DemeAllocT_hpp
#define Beagle_DemeAllocT_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/DemeAlloc.hpp"
#include "beagle/Individual.hpp"
#include "beagle/Stats.hpp"
#include "beagle/BadCastException.hpp"

namespace Beagle {


/*!
 *  \class DemeAllocT beagle/DemeAllocT.hpp "beagle/DemeAllocT.hpp"
 *  \brief Allocator template class for deme derived types.
 *  \param T The deme type allocated.
 *  \param BaseType The base type from which the actual allocator type is derived.
 *  \param IndividualAllocType Individual allocator type used to fill the demes.
 *  \ingroup ECF
 *  \ingroup Pop
 *  \ingroup Allocs
 */
template <class T, class BaseType, class IndividualAllocType>
class DemeAllocT : public BaseType {

public:

  //! DemeAllocT allocator type.
  typedef AllocatorT<DemeAllocT<T,BaseType,IndividualAllocType>,typename BaseType::Alloc>
          Alloc;
  //! DemeAllocT handle type.
  typedef PointerT<DemeAllocT<T,BaseType,IndividualAllocType>,typename BaseType::Handle>
          Handle;
  //! DemeAllocT mixed bag type.
  typedef ContainerT<DemeAllocT<T,BaseType,IndividualAllocType>,typename BaseType::Bag>
          Bag;

  explicit DemeAllocT(typename IndividualAllocType::Handle inIndividualAlloc=NULL);
  explicit DemeAllocT(typename IndividualAllocType::Handle inIndividualAlloc,
                      Beagle::Stats::Alloc::Handle inStatsAlloc,
                      Beagle::HallOfFame::Alloc::Handle inHOFAlloc);
  virtual ~DemeAllocT() { }

  virtual Object*    allocate() const;
  virtual Object*    clone(const Object& inOrigObj) const;
  virtual Container* cloneData(const Container& inOrigContainer) const;
  virtual void       copy(Object& outCopyObj, const Object& inOrigObj) const;
  virtual void       copyData(Container& outCopyContainer, const Container& inOrigContainer) const;

  /*!
   *  \brief Get the individual allocator.
   *  \return Constant handle to the individual allocator.
   */
  inline const typename IndividualAllocType::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<const IndividualAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("const IndividualAllocType::Handle DemeAllocT<T,BaseType,IndividualAllocType>::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the bag contained type allocator.
   *  \return Handle to the bag contained type allocator.
   */
  inline typename IndividualAllocType::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<IndividualAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("IndividualAllocType::Handle DemeAllocT<T,BaseType,IndividualAllocType>::getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the bag contained type allocator.
   *  \param inIndividualAllocType The new bag contained type allocator.
   */
  inline void setContainerTypeAlloc(typename IndividualAllocType::Handle inIndividualAllocType)
  {
    Beagle_StackTraceBeginM();
    BaseType::mContainerTypeAlloc = inIndividualAllocType;
    Beagle_StackTraceEndM("void DemeAllocT<T,BaseType,IndividualAllocType>::setContainerTypeAlloc(IndividualAllocType::Handle inIndividualAllocType)");
  }

};

}


/*!
 *  \brief Constructor of deme allocator from an individual allocator.
 *  \param inIndividualAlloc Type allocator given to the individual allocated.
 */
template <class T, class BaseType, class IndividualAllocType>
Beagle::DemeAllocT<T,BaseType,IndividualAllocType>::DemeAllocT(
  typename IndividualAllocType::Handle inIndividualAlloc) :
  BaseType(inIndividualAlloc)
{ }


/*!
 *  \brief Constructor of deme allocator from an individual allocator and a statistics allocator.
 *  \param inIndividualAlloc Type allocator given to the individual allocated.
 *  \param inStatsAlloc Stats type allocator.
 *  \param inHOFAlloc Hall-of-fame allocator.
 */
template <class T, class BaseType, class IndividualAllocType>
Beagle::DemeAllocT<T,BaseType,IndividualAllocType>::DemeAllocT(
  typename IndividualAllocType::Handle inIndividualAlloc,
  Beagle::Stats::Alloc::Handle inStatsAlloc,
  Beagle::HallOfFame::Alloc::Handle inHOFAlloc) :
  BaseType(inIndividualAlloc, inStatsAlloc, inHOFAlloc)
{ }


/*!
 *  \brief  Allocate a new deme and the heap by giving it the type allocator.
 *  \return Pointer to the allocated deme.
 */
template <class T, class BaseType, class IndividualAllocType>
Beagle::Object* Beagle::DemeAllocT<T,BaseType,IndividualAllocType>::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  T* lT = new T(castHandleT<IndividualAllocType>(BaseType::mContainerTypeAlloc),
                BaseType::mStatsAlloc, BaseType::mHOFAlloc);
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lT;
#else // BEAGLE_NDEBUG
  return new T(castHandleT<IndividualAllocType>(BaseType::mContainerTypeAlloc),
               BaseType::mStatsAlloc, BaseType::mHOFAlloc);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* DemeAllocT<T,BaseType,IndividualAllocType>::allocate() const");
}


/*!
 *  \brief  Allocate a new deme on the heap that is a clone an existing deme.
 *  \param  inOrigObj Constant reference to the original deme object to clone.
 *  \return Pointer to the allocated deme.
 */
template <class T, class BaseType, class IndividualAllocType>
Beagle::Object* Beagle::DemeAllocT<T,BaseType,IndividualAllocType>::clone(
  const Beagle::Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  T* lT = new T(lOrigT);
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lT;
#else // BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  return new T(lOrigT);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* DemeAllocT<T,BaseType,IndividualAllocType>::clone(const Object& inOrigObj) const");
}


/*!
 *  \brief  Allocate a new deme on the heap that is a clone an existing deme.
 *    Make a complete copy of the demes.
 *  \param  inOrigContainer Constant reference to the original deme object to clone.
 *  \return Pointer to the allocated deme.
 */
template <class T, class BaseType, class IndividualAllocType>
Beagle::Container* Beagle::DemeAllocT<T,BaseType,IndividualAllocType>::cloneData(
  const Beagle::Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  T* lT = new T(castHandleT<IndividualAllocType>(BaseType::mContainerTypeAlloc),
                BaseType::mStatsAlloc, BaseType::mHOFAlloc);
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  lT->copyData(lOrigT);
  return lT;
#else // BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  T* lT = new T(castHandleT<IndividualAllocType>(BaseType::mContainerTypeAlloc),
                BaseType::mStatsAlloc, BaseType::mHOFAlloc);
  lT->copyData(lOrigT);
  return lT;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Container* DemeAllocT<T,BaseType,IndividualAllocType>::cloneData(const Container& inOrigContainer) const");
}


/*!
 *  \brief Copy a deme into another.
 *  \param outCopyObj Constant reference to the original deme to copy.
 *  \param inOrigObj  Reference to the deme that is a copy of the original.
 */
template <class T, class BaseType, class IndividualAllocType>
void Beagle::DemeAllocT<T,BaseType,IndividualAllocType>::copy(
  Beagle::Object& outCopyObj, const Beagle::Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  T& lCopyT = castObjectT<T&>(outCopyObj);
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  lCopyT = lOrigT;
  Beagle_StackTraceEndM("void DemeAllocT<T,BaseType,IndividualAllocType>::copy(Object& outCopyObj, const Object& inOrigObj) const");
}


/*!
 *  \brief Copy a deme into another with the contained data.
 *  \param outCopyContainer Constant reference to the original deme to copy.
 *  \param inOrigContainer  Reference to the deme that is a copy of the original.
 */
template <class T, class BaseType, class IndividualAllocType>
void Beagle::DemeAllocT<T,BaseType,IndividualAllocType>::copyData(
  Beagle::Container& outCopyContainer, const Beagle::Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
  T& lCopyT = castObjectT<T&>(outCopyContainer);
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  lCopyT.copyData(lOrigT);
  Beagle_StackTraceEndM("void DemeAllocT<T,BaseType,IndividualAllocType>::copyData(Container& outCopyContainer, const Container& inOrigContainer) const");
}


#endif // Beagle_DemeAlloc_hpp
