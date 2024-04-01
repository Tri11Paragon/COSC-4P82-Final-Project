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
 *  \file   beagle/ContainerAllocator.hpp
 *  \brief  Definition of class ContainerAllocator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5.2.1 $
 *  $Date: 2007/05/09 01:51:10 $
 */

#ifndef Beagle_ContainerAllocator_hpp
#define Beagle_ContainerAllocator_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"


namespace Beagle {

// Forward declarations
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;

/*!
 *  \class ContainerAllocator beagle/ContainerAllocator.hpp "beagle/ContainerAllocator.hpp"
 *  \brief Basic container allocator.
 *  \ingroup OOF
 *  \ingroup Allocs
 *  \ingroup DataStruct
 */
class ContainerAllocator : public Object::Alloc {

public:

  //! ContainerAllocator allocator type.
  typedef AllocatorT<ContainerAllocator,Object::Alloc::Alloc>
          Alloc;
  //! ContainerAllocator handle type.
  typedef PointerT<ContainerAllocator,Object::Alloc::Handle>
          Handle;
  //! ContainerAllocator bag type.
  typedef ContainerT<ContainerAllocator,Object::Alloc::Bag>
          Bag;

  explicit ContainerAllocator(Object::Alloc::Handle inContainerTypeAlloc=NULL);
  virtual ~ContainerAllocator() { }

  virtual Object*    allocate() const;
  virtual Object*    clone(const Object& inOrigObj) const;
  virtual Container* cloneData(const Container& inOrigObj) const;
  virtual void       copy(Object& outCopyObj, const Object& inOrigObj) const;
  virtual void       copyData(Container& outCopyObj, const Container& inOrigObj) const;

  /*!
   *  \brief Get the container type allocator.
   *  \return Constant handle to the container type allocator.
   */
  inline const Object::Alloc::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mContainerTypeAlloc;
    Beagle_StackTraceEndM("const Object::Alloc::Handle ContainerAllocator::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the container type allocator.
   *  \return Handle to the container type allocator.
   */
  inline Object::Alloc::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return mContainerTypeAlloc;
    Beagle_StackTraceEndM("Object::Alloc::Handle ContainerAllocator::getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the container type allocator.
   *  \param inContainerTypeAlloc The new uniform bag contained type allocator.
   */
  inline void setContainerTypeAlloc(Object::Alloc::Handle inContainerTypeAlloc)
  {
    Beagle_StackTraceBeginM();
    mContainerTypeAlloc = inContainerTypeAlloc;
    Beagle_StackTraceEndM("void ContainerAllocator::setContainerTypeAlloc(Object::Alloc::Handle inContainerTypeAlloc)");
  }

protected:

  Object::Alloc::Handle mContainerTypeAlloc;  //!< Container type allocator handle.

};

}

#endif // Beagle_ContainerAllocator_hpp

