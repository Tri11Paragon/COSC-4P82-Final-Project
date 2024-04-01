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
 *  \file   beagle/AbstractContainerAllocT.hpp
 *  \brief  Definition of the class template AbstractContainerAllocT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:08 $
 */

#ifndef Beagle_AbstractContainerAllocT_hpp
#define Beagle_AbstractContainerAllocT_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"


namespace Beagle {

/*!
 *  \class AbstractContainerAllocT beagle/AbstractContainerAllocT.hpp
 *     "beagle/AbstractContainerAllocT.hpp"
 *  \brief Templated class defining a T-type abstract container allocator.
 *  \param T The container type allocated.
 *  \param BaseType The base type from which the actual allocator type is derived.
 *  \param ContainerTypeAllocType The container's type allocator type.
 *  \ingroup OOF
 *  \ingroup Allocs
 */
template <class T, class BaseType, class ContainerTypeAllocType>
class AbstractContainerAllocT : public BaseType {

public:

  //! AbstractContainerAllocT allocator type.
  typedef AbstractAllocT<AbstractContainerAllocT<T,BaseType,ContainerTypeAllocType>,
                         typename BaseType::Alloc>
          Alloc;
  //! AbstractContainerAllocT handle type.
  typedef PointerT<AbstractContainerAllocT<T,BaseType,ContainerTypeAllocType>,
                   typename BaseType::Handle>
          Handle;
  //! AbstractContainerAllocT bag type.
  typedef ContainerT<AbstractContainerAllocT<T,BaseType,ContainerTypeAllocType>,
                     typename BaseType::Bag>
          Bag;

  explicit AbstractContainerAllocT(typename ContainerTypeAllocType::Handle inContainerTypeAlloc=NULL);
  virtual ~AbstractContainerAllocT() { }

  /*!
   *  \brief  Allocate a brand new container and the heap.
   *  \return Pointer to the allocated container.
   */
  virtual Object* allocate() const =0;

  /*!
   *  \brief  Allocate a new container on the heap that is a clone an existing container.
   *  \param  inOrigObj Constant reference to the original container to clone.
   *  \return Pointer to the allocated container.
   */
  virtual Object* clone(const Object& inOrigObj) const =0;

  /*!
   *  \brief  Allocate a new container on the heap that is a clone an existing container
   *    and its data.
   *  \param  inOrigContainer Constant reference to the original container to clone.
   *  \return Pointer to the allocated container.
   */
  virtual Container* cloneData(const Container& inOrigContainer) const =0;

  /*!
   *  \brief Copy an container into another.
   *  \param inOrigObj Constant reference to the original container to copy.
   *  \param outCopyObj Reference to the container that is a copy of the original.
   */
  virtual void copy(Object& outCopyObj, const Object& inOrigObj) const =0;

  /*!
   *  \brief Copy an container into another, with its data.
   *  \param outCopyContainer Reference to the container that is a copy of the original.
   *  \param inOrigContainer Constant reference to the original container to copy.
   */
  virtual void copyData(Container& outCopyContainer, const Container& inOrigContainer) const =0;

  /*!
   *  \brief Get the container type allocator.
   *  \return Constant handle to the container type allocator.
   */
  inline const typename ContainerTypeAllocType::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<const ContainerTypeAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("const ContainerTypeAllocType::Handle AbstractContainerAllocT<T,BaseType,ContainerTypeAllocType>::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the container type allocator.
   *  \return Handle to the container type allocator.
   */
  inline typename ContainerTypeAllocType::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<ContainerTypeAllocType>(BaseType::mContainerTypeAlloc);
    Beagle_StackTraceEndM("ContainerTypeAllocType::Handle AbstractContainerAllocT<T,BaseType,ContainerTypeAllocType>::getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the container type allocator.
   *  \param inContainerTypeAlloc The new container type allocator.
   */
  inline void setContainerTypeAlloc(typename ContainerTypeAllocType::Handle inContainerTypeAlloc)
  {
    Beagle_StackTraceBeginM();
    BaseType::mContainerTypeAlloc = inContainerTypeAlloc;
    Beagle_StackTraceEndM("void AbstractContainerAllocT<T,BaseType,ContainerTypeAllocType>::setContainerTypeAlloc(ContainerTypeAllocType::Handle inContainerTypeAlloc)");
  }

};

}


/*!
 *  \brief Constructor of a T-type container allocator.
 *  \param inContainerTypeAlloc Type allocator given to the container allocated.
 */
template <class T, class BaseType, class ContainerTypeAllocType>
Beagle::AbstractContainerAllocT<T,BaseType,ContainerTypeAllocType>::AbstractContainerAllocT(
  typename ContainerTypeAllocType::Handle inContainerTypeAlloc) :
  BaseType(inContainerTypeAlloc)
{ }


#endif // Beagle_AbstractContainerAllocT_hpp

