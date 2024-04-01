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
 *  \file   beagle/ContainerAllocatorT.hpp
 *  \brief  Definition of the class template ContainerAllocatorT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5.2.1 $
 *  $Date: 2007/05/09 01:51:10 $
 */

#ifndef Beagle_ContainerAllocatorT_hpp
#define Beagle_ContainerAllocatorT_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/BadCastException.hpp"


namespace Beagle {

/*!
 *  \class ContainerAllocatorT beagle/ContainerAllocatorT.hpp "beagle/ContainerAllocatorT.hpp"
 *  \brief Templated class defining a T-type container allocator.
 *  \param T The container type allocated.
 *  \param BaseType The base type from which the actual allocator type is derived.
 *  \param ContainerTypeAllocType The container's type allocator type.
 *  \ingroup OOF
 *  \ingroup Allocs
 *  \ingroup DataStruct
 */
template <class T, class BaseType, class ContainerTypeAllocType>
class ContainerAllocatorT : public BaseType {

public:

  //! ContainerAllocatorT allocator type.
  typedef AllocatorT<ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>,typename BaseType::Alloc>
          Alloc;
  //! ContainerAllocatorT handle type.
  typedef PointerT<ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>,typename BaseType::Handle>
          Handle;
  //! ContainerAllocatorT bag type.
  typedef ContainerT<ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>,typename BaseType::Bag>
          Bag;

  explicit ContainerAllocatorT(typename ContainerTypeAllocType::Handle inContainerTypeAlloc=NULL);
  virtual ~ContainerAllocatorT() { }

  virtual Object*    allocate() const;
  virtual Object*    clone(const Object& inOrigObj) const;
  virtual Container* cloneData(const Container& inOrigContainer) const;
  virtual void       copy(Object& outCopyObj, const Object& inOrigObj) const;
  virtual void       copyData(Container& outCopyObj, const Container& inOrigObj) const;

  /*!
   *  \brief Get the container type allocator.
   *  \return Constant handle to the container type allocator.
   */
  inline const typename ContainerTypeAllocType::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<const ContainerTypeAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("const ContainerTypeAllocType::Handle ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the container type allocator.
   *  \return Handle to the container type allocator.
   */
  inline typename ContainerTypeAllocType::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<ContainerTypeAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("ContainerTypeAllocType::Handle ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the container type allocator.
   *  \param inContainerTypeAlloc The new container type allocator.
   */
  inline void setContainerTypeAlloc(typename ContainerTypeAllocType::Handle inContainerTypeAlloc)
  {
    Beagle_StackTraceBeginM();
    BaseType::mContainerTypeAlloc = inContainerTypeAlloc;
    Beagle_StackTraceEndM("void ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::setContainerTypeAlloc(ContainerTypeAllocType::Handle inContainerTypeAlloc)");
  }

};

}


/*!
 *  \brief Constructor of a T-type container allocator.
 *  \param inContainerTypeAlloc Type allocator given to the container allocated.
 */
template <class T, class BaseType, class ContainerTypeAllocType>
Beagle::ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::ContainerAllocatorT(
  typename ContainerTypeAllocType::Handle inContainerTypeAlloc) :
  BaseType(inContainerTypeAlloc)
{ }


/*!
 *  \brief  Allocate a plain new container on the heap by giving it the type allocator.
 *  \return Pointer to the allocated container.
 *  \throw  InternalException When the program goes out of memory.
 */
template <class T, class BaseType, class ContainerTypeAllocType>
Beagle::Object* Beagle::ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  T* lT = new T(castHandleT<ContainerTypeAllocType>(BaseType::mContainerTypeAlloc));
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  return lT;
#else // BEAGLE_NDEBUG
  return new T(castHandleT<ContainerTypeAllocType>(BaseType::mContainerTypeAlloc));
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::allocate() const");
}


/*!
 *  \brief  Allocate a new container on the heap that is a clone an existing object.
 *  \param  inOrigObj Constant reference to the original container object to clone.
 *  \return Pointer to the allocated container.
 *  \throw  InternalException When the program goes out of memory.
 *  \throw  BadCastException If the type of the parameter is not as specified.
 */
template <class T, class BaseType, class ContainerTypeAllocType>
Beagle::Object* Beagle::ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::clone(
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
  Beagle_StackTraceEndM("Object* ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::clone(const Object& inOrigObj) const");
}


/*!
 *  \brief  Allocate a new container on the heap that is a clone an existing object with a copy
 *    of the data contained.
 *  \param  inOrigContainer Constant reference to the original container object to clone.
 *  \return Pointer to the allocated container.
 *  \throw  InternalException When the program goes out of memory.
 *  \throw  BadCastException If the type of the parameter is not as specified.
 */
template <class T, class BaseType, class ContainerTypeAllocType>
Beagle::Container* Beagle::ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::cloneData(
  const Beagle::Container& inOrigContainer) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  T* lT = new T(castHandleT<ContainerTypeAllocType>(BaseType::mContainerTypeAlloc));
  if(!lT) throw Beagle_InternalExceptionM("Out-of-memory!");
  lT->copyData(lOrigT);
  return lT;
#else // BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigContainer);
  T* lT = new T(castHandleT<ContainerTypeAllocType>(BaseType::mContainerTypeAlloc));
  lT->copyData(lOrigT);
  return lT;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Container* ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::cloneData(const Container& inOrigContainer) const");
}


/*!
 *  \brief Copy a container into another.
 *  \param outCopyObj Constant reference to the original container to copy.
 *  \param inOrigObj  Reference to the container that is a copy of the original.
 *  \throw BadCastException If the type of the parameter is not as specified.
 */
template <class T, class BaseType, class ContainerTypeAllocType>
void Beagle::ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::copy(
  Beagle::Object& outCopyObj, const Beagle::Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  T& lCopyT = castObjectT<T&>(outCopyObj);
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  lCopyT = lOrigT;
  Beagle_StackTraceEndM("void ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::copy(Object& outCopyObj, const Object& inOrigObj) const");
}


/*!
 *  \brief Copy a container into another with all the data contained.
 *  \param outCopyObj Constant reference to the original container to copy.
 *  \param inOrigObj Reference to the container that is a copy of the original.
 *  \throw BadCastException If the type of the parameter is not as specified.
 */
template <class T, class BaseType, class ContainerTypeAllocType>
void Beagle::ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::copyData(
  Beagle::Container& outCopyObj, const Beagle::Container& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  T& lCopyT = castObjectT<T&>(outCopyObj);
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  lCopyT.copyData(lOrigT);
  Beagle_StackTraceEndM("void ContainerAllocatorT<T,BaseType,ContainerTypeAllocType>::copyData(Container& outCopyObj, const Container& inOrigObj) const");
}


#endif // Beagle_ContainerAllocatorT_hpp

