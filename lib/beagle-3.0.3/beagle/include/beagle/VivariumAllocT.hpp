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
 *  \file   beagle/VivariumAllocT.hpp
 *  \brief  Implementation of the class template VivariumAllocT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#ifndef Beagle_VivariumAllocT_hpp
#define Beagle_VivariumAllocT_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/VivariumAlloc.hpp"
#include "beagle/Deme.hpp"
#include "beagle/Stats.hpp"
#include "beagle/BadCastException.hpp"

namespace Beagle {


/*!
 *  \class VivariumAllocT beagle/VivariumAllocT.hpp "beagle/VivariumAllocT.hpp"
 *  \brief Allocator template class for vivarium derived types.
 *  \param T The vivarium type allocated.
 *  \param BaseType The base type from which the actual allocator type is derived.
 *  \param DemeAllocType Deme allocator type used to fill the Vivariums.
 *  \ingroup ECF
 *  \ingroup Pop
 *  \ingroup Allocs
 */
template <class T, class BaseType, class DemeAllocType>
class VivariumAllocT : public BaseType {

public:

  //! VivariumAllocT allocator type.
  typedef AllocatorT<VivariumAllocT<T,BaseType,DemeAllocType>,typename BaseType::Alloc>
          Alloc;
  //! VivariumAllocT handle type.
  typedef PointerT<VivariumAllocT<T,BaseType,DemeAllocType>,typename BaseType::Handle>
          Handle;
  //! VivariumAllocT mixed bag type.
  typedef ContainerT<VivariumAllocT<T,BaseType,DemeAllocType>,typename BaseType::Bag>
          Bag;

  explicit VivariumAllocT(typename DemeAllocType::Handle inDemeAlloc=NULL);
  explicit VivariumAllocT(typename DemeAllocType::Handle inDemeAlloc,
                          Beagle::Stats::Alloc::Handle inStatsAlloc,
                          Beagle::HallOfFame::Alloc::Handle inHOFAlloc);
  virtual ~VivariumAllocT() { }

  virtual Object*    allocate() const;
  virtual Object*    clone(const Object& inOrigObj) const;
  virtual Container* cloneData(const Container& inOrigContainer) const;
  virtual void       copy(Object& outCopyObj, const Object& inOrigObj) const;
  virtual void       copyData(Container& outCopyContainer, const Container& inOrigContainer) const;

  /*!
   *  \brief Get the deme allocator.
   *  \return Constant handle to the deme allocator.
   */
  inline const typename DemeAllocType::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<const DemeAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("const DemeAllocType::Handle VivariumAllocT<T,BaseType,DemeAllocType>::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the bag contained type allocator.
   *  \return Handle to the bag contained type allocator.
   */
  inline typename DemeAllocType::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<DemeAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("DemeAllocType::Handle VivariumAllocT<T,BaseType,DemeAllocType>::getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the bag contained type allocator.
   *  \param inDemeAllocType The new bag contained type allocator.
   */
  inline void setContainerTypeAlloc(typename DemeAllocType::Handle inDemeAllocType)
  {
    Beagle_StackTraceBeginM();
    BaseType::mContainerTypeAlloc = inDemeAllocType;
    Beagle_StackTraceEndM("void VivariumAllocT<T,BaseType,DemeAllocType>::setContainerTypeAlloc(DemeAllocType::Handle inDemeAllocType)");
  }

};

}


/*!
 *  \brief Constructor of vivarium allocator from a deme allocator.
 *  \param inDemeAlloc Type allocator given to the deme allocated.
 */
template <class T, class BaseType, class DemeAllocType>
Beagle::VivariumAllocT<T,BaseType,DemeAllocType>::VivariumAllocT(
  typename DemeAllocType::Handle inDemeAlloc) :
  BaseType(inDemeAlloc)
{ }


/*!
 *  \brief Constructor of vivarium allocator from a deme allocator and a statistics allocator.
 *  \param inDemeAlloc Type allocator given to the deme allocated.
 *  \param inStatsAlloc Statistics type allocator.
 *  \param inHOFAlloc Hall-of-fame allocator.
 */
template <class T, class BaseType, class DemeAllocType>
Beagle::VivariumAllocT<T,BaseType,DemeAllocType>::VivariumAllocT(
  typename DemeAllocType::Handle inDemeAlloc,
  Beagle::Stats::Alloc::Handle inStatsAlloc,
  Beagle::HallOfFame::Alloc::Handle inHOFAlloc) :
  BaseType(inDemeAlloc, inStatsAlloc, inHOFAlloc)
{ }


/*!
 *  \brief  Allocate a new vivarium and the heap by giving it the type allocator.
 *  \return Pointer to the allocated vivarium.
 */
template <class T, class BaseType, class DemeAllocType>
Beagle::Object* Beagle::VivariumAllocT<T,BaseType,DemeAllocType>::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  T* lT = new T(castHandleT<DemeAllocType>(BaseType::mContainerTypeAlloc),
                BaseType::mStatsAlloc, BaseType::mHOFAlloc);
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lT;
#else // BEAGLE_NDEBUG
  return new T(castHandleT<DemeAllocType>(BaseType::mContainerTypeAlloc),
               BaseType::mStatsAlloc, BaseType::mHOFAlloc);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* VivariumAllocT<T,BaseType,DemeAllocType>::allocate() const");
}


/*!
 *  \brief  Allocate a new vivarium on the heap that is a clone an existing vivarium.
 *  \param  inOrigObj Constant reference to the original vivarium object to clone.
 *  \return Pointer to the allocated vivarium.
 */
template <class T, class BaseType, class DemeAllocType>
Beagle::Object* Beagle::VivariumAllocT<T,BaseType,DemeAllocType>::clone(
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
  Beagle_StackTraceEndM("Object* VivariumAllocT<T,BaseType,DemeAllocType>::clone(const Object& inOrigObj) const");
}


/*!
 *  \brief  Allocate a new vivarium on the heap that is a clone an existing vivarium.
 *    Make a complete copy of the vivarium.
 *  \param  inOrigContainer Constant reference to the original vivarium object to clone.
 *  \return Pointer to the allocated vivarium.
 */
template <class T, class BaseType, class DemeAllocType>
Beagle::Container* Beagle::VivariumAllocT<T,BaseType,DemeAllocType>::cloneData(
  const Beagle::Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  T* lT = new T(castHandleT<DemeAllocType>(BaseType::mContainerTypeAlloc),
                BaseType::mStatsAlloc, BaseType::mHOFAlloc);
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  lT->copyData(lOrigT);
  return lT;
#else // BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  T* lT = new T(castHandleT<DemeAllocType>(BaseType::mContainerTypeAlloc),
                BaseType::mStatsAlloc, BaseType::mHOFAlloc);
  lT->copyData(lOrigT);
  return lT;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Container* VivariumAllocT<T,BaseType,DemeAllocType>::cloneData(const Container& inOrigContainer) const");
}


/*!
 *  \brief Copy a vivarium into another.
 *  \param outCopyObj Constant reference to the original vivarium to copy.
 *  \param inOrigObj  Reference to the vivarium that is a copy of the original.
 */
template <class T, class BaseType, class DemeAllocType>
void Beagle::VivariumAllocT<T,BaseType,DemeAllocType>::copy(
  Beagle::Object& outCopyObj, const Beagle::Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  T& lCopyT = castObjectT<T&>(outCopyObj);
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  lCopyT = lOrigT;
  Beagle_StackTraceEndM("void VivariumAllocT<T,BaseType,DemeAllocType>::copy(Object& outCopyObj, const Object& inOrigObj) const");
}


/*!
 *  \brief Copy a vivarium into another with the contained data.
 *  \param outCopyContainer Constant reference to the original vivarium to copy.
 *  \param inOrigContainer  Reference to the vivarium that is a copy of the original.
 */
template <class T, class BaseType, class DemeAllocType>
void Beagle::VivariumAllocT<T,BaseType,DemeAllocType>::copyData(
  Beagle::Container& outCopyContainer, const Beagle::Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
  T& lCopyT = castObjectT<T&>(outCopyContainer);
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  lCopyT.copyData(lOrigT);
  Beagle_StackTraceEndM("void VivariumAllocT<T,BaseType,DemeAllocType>::copyData(Container& outCopyContainer, const Container& inOrigContainer) const");
}


#endif // Beagle_VivariumAlloc_hpp
